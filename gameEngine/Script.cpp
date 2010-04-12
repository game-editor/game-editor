/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// Script.cpp: implementation of the Script class.
//
//////////////////////////////////////////////////////////////////////

#include "Script.h"
#include "GameControl.h"
#include "GenericScript.h"
#include <math.h>

#ifdef USE_RAKNET
#include "../RakNet/Source/Rand.h"
#endif

#ifdef _WIN32_WCE
#include "libce/include/time.h"
#else
#include <time.h>
#endif

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME) && defined(USE_ACTIVATION)
#	include "SecuredSections.h"
#endif

#ifndef STAND_ALONE_GAME
#include "../gameEditor/Tutorial.h"
#endif

#include "eic.h"
#include "MachSet.h"
#include "modules.h"
#include "preproc.h"


#undef malloc
#undef realloc
#undef free
#include <stdlib.h>

#include "dlmalloc.h"

#ifdef _WIN32_WCE
extern "C" int IsSmartphoneDevice();


#include <winioctl.h>

extern "C" __declspec(dllimport) BOOL KernelIoControl(DWORD
													  dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf,
													  DWORD nOutBufSize, LPDWORD lpBytesReturned );

#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _DEVICE_ID
{
	DWORD dwSize;
	DWORD dwPresetIDOffset;
	DWORD dwPresetIDBytes;
	DWORD dwPlatformIDOffset;
	DWORD dwPlatformIDBytes;
} DEVICE_ID, *PDEVICE_ID;


#endif

//char *newFunctionCall = NULL; //To update function calls in old geds
//SDL_mutex *parserStringMutEx = SDL_CreateMutex();
#define MIN_ACTOR_VARS 26





extern "C"
{
#include "global.h"
#include "symbol.h"
#include "lexer.h"
#include "reset.h"


enum {m_char = 0, m_short, m_int, m_long, m_llong, m_float, m_double, m_pointer, m_struct};
extern int EiC_getsize(int type);

extern void EiC_interpret(environ_t * env);
extern int EiC_analyseCode(code_t *C);
extern void OptimizeCode(code_t *c);
extern int EiC_interActive;
code_t firstInst;
int storeFirstInst = 0;
extern int EiC_interpON;
extern unsigned long gedMaxGameMem;






extern unsigned lex_curpos, lex_lastpos, lex_str_start_pos, bCanInsertSymbol;

void module_stdio(void);
void module_math(void);
void module_fcntl(void);
void module_string(void);
void module_ctype(void);
void module_dirent(void);
void module_assert(void);
void module_time(void);
void module_unistd(void);
void module_stat(void);
void module_stdarg(void);
void module_errno(void);
void module_stdlib(void);
void module_termios(void);
void module_signal(void);
}

#ifndef STAND_ALONE_GAME
extern "C" void showFunc(type_expr **t, FILE *fp);

void printFunction(char *functionName)
{
	symentry_t *sym = (symentry_t *)isSymbolInEic(functionName);
	if(sym && (EiC_gettype(sym->type) == t_funcdec || EiC_gettype(sym->type) == t_func || EiC_gettype(sym->type) == t_builtin))
	{
		FILE *arq = fopen("func.txt", "w");
		fprintf(arq, "%s -> ", sym->id);
		showFunc(&sym->type, arq);
		fclose(arq);
		exit(0);
	}
}
#endif

/*#include "eic.h" //maks
#include "MachSet.h"
extern "C"
{
#include "global.h"
#include "symbol.h"

extern void EiC_interpret(environ_t * env);
extern int EiC_interActive;
code_t *firstInst = NULL;
int storeFirstInst = 0;
}
void testExecution()
{
	//////////////////////////////////////////////////////
	EiC_init_EiC();

	int i = 0;
	EiC_parseString("int i @ %ld;", (long)&i);

	//Parser
	firstInst = NULL;
	storeFirstInst = 1;
	EiC_parseString("void oi() {i += 3;};");
	storeFirstInst = 0;

	//Direct execution
	DWORD t1 = GetTickCount();
	for(long n = 0; n < 100000; n++)
	{
		EiC_interActive = 0;
		code_t code = EiC_ENV->CODE;
		EiC_ENV->CODE.nextinst = EiC_ENV->CODE.binst = 0;
		EiC_ENV->CODE.inst = firstInst;
		EiC_ENV->CODE.prev = &code;		

		EiC_interpret(EiC_ENV);
		
		
		
		EiC_ENV->CODE = code;
	}
	DWORD tExecutionDirect = GetTickCount() - t1;

	
	//Parser execution: 55 vezes mais rápido !!!
	DWORD t2 = GetTickCount();
	for(n = 0; n < 100000; n++)
	{
		EiC_parseString("oi();");
	}
	DWORD tExecutionParser = GetTickCount() - t2;
	//////////////////////////////////////////////////////
}*/


#ifndef STAND_ALONE_GAME
Actor *currentEventActor = NULL;
stAction *currentAction = NULL;
extern "C" char currentFunction[128];
extern "C" int CurrentExecutionLine();
extern "C" int InRunEiC();

GlMap< gedString, int, GlStringHash > mapOffset;

typedef GlMap< void *, char, GlPointerHash<void *> > MapBuiltinFunc;
MapBuiltinFunc mapBuiltinFunc;

extern "C" void AddToFastBuiltinFuncTable(void *vfunc)//maks
{
	mapBuiltinFunc.Add(vfunc, 1);
}

extern "C" int IsValidBuiltinFunc(void *vfunc)//maks
{
	return (mapBuiltinFunc.Find1(vfunc) != NULL)?1:0;
}

ListString stackFunctionName;
extern "C" void PushFunctionName(char *name)
{
	stackFunctionName.PushBack(name);
}

extern "C" void PopFunctionName()
{
	if(stackFunctionName.Count())
	{
		stackFunctionName.PopBack();
	}
}

gedString GetCurrentFunctionName()
{
	if(stackFunctionName.Count())
	{
		return stackFunctionName.Back();
	}

	return "";
}

extern "C" void ResetFunctionNameStack()
{
	stackFunctionName.Clear();
}

extern "C" int FunctionNameStackSize()
{
	return stackFunctionName.Count();
}

#else
//No check to avoid slow down
extern "C" void AddToFastBuiltinFuncTable(void *vfunc)
{
	
}

extern "C" int IsValidBuiltinFunc(void *vfunc)
{
	return 1;
}

extern "C" void PushFunctionName(char *name)
{
	
}

extern "C" void PopFunctionName()
{
	
}


extern "C" void ResetFunctionNameStack()
{
	
}

extern "C" int FunctionNameStackSize()
{
	return 0;
}

#endif


extern "C" int CheckBreakPoints(int line)
{
	//Check the breakpoints at current code and current line
	//Must consider the recursion, so need execute through the Script class
	int bBreak = 0;
	static bool bNotBreackPointCheck = false;

	

#ifndef STAND_ALONE_GAME
	/*if(GameControl::Get()->getGameMode() && !bNotBreackPointCheck)
	{
		static Script *bp = NULL;

		if(!bp)
		{
			bp = new Script();
			if(!bp->Parser("canmove != 1;", true, false, false))
			{
				delete bp;
				bp = NULL;
			}
		}

		if(bp)
		{
			int oldConditionalResult = Script::getConditionalResult();
			Script::setConditionalResult(false);

			bNotBreackPointCheck = true;
			bp->Execute(NULL, NULL);
			bNotBreackPointCheck = false;

			bBreak = Script::getConditionalResult();
			Script::setConditionalResult(oldConditionalResult);	
		}
		//EiC_parseString("int gedBreak  @ %ld = canmove == 1;", &bBreak);
	}*/
#endif

	return bBreak;
}

#define setEp(P,sz)  (P).ep = (char*)(P).p + (sz)

static val_t eic_abs(void)
{
    val_t v;
    v.dval = fabs(arg(0,getargs(),double));
    return v;
}



stTime getTime()
{
	stTime t;

	time_t timer;
	struct tm *localTime;


	time( &timer );
	t.sec_utc = timer;

	

	localTime = localtime( &timer ); //Convert to local time

	t.sec  = localTime->tm_sec;
	t.min  = localTime->tm_min;
	t.hour = localTime->tm_hour;

	t.mday = localTime->tm_mday;
	t.mon  = localTime->tm_mon + 1;
	t.year = localTime->tm_year + 1900;

	t.wday = localTime->tm_wday;
	t.yday = localTime->tm_yday;

	return t;
}

val_t eic_getTime(void)
{
    val_t v;
    static stTime t; /* create some memory */

    t = getTime();

    /* set safe */
    v.p.sp = v.p.p = &t;
    v.p.ep = (char*)&t + sizeof(stTime);

    return v;
}

char *getOwner()
{	
	static char owner[128];
	memset(owner, 0, 128);
	
#ifdef _WIN32_WCE
	unsigned long dwDisposition, dwOwnerSize;
	HKEY m_hKey;

	if(RegCreateKeyEx(
		HKEY_CURRENT_USER, 
		TEXT("\\ControlPanel\\Owner"), 
		0, 
		NULL, 
		0, 
		0, 
		NULL, 
		&m_hKey, 
		&dwDisposition) == ERROR_SUCCESS)
	{
		//Smartphone: REG_SZ
		//Pocket PC: Binary

		unsigned long dwType = IsSmartphoneDevice()?REG_SZ:REG_BINARY;
		TCHAR *value = IsSmartphoneDevice()?_T("Name"):_T("Owner");

		//Get buffer size
		if (
			RegQueryValueEx(
			m_hKey, 
			value,  
			0, 
			&dwType, 
			NULL, 
			&dwOwnerSize
			) == ERROR_SUCCESS) 
		{
			//Allocate buffer
			TCHAR *Owner = (TCHAR *)malloc(dwOwnerSize*sizeof(TCHAR));
			
			if(Owner)
			{
				//Get owner info
				memset(Owner, 0, dwOwnerSize*sizeof(TCHAR));		
				
				
				if (
					RegQueryValueEx(
					m_hKey, 
					value,  
					0, 
					&dwType, 
					(PBYTE)Owner, 
					&dwOwnerSize
					) == ERROR_SUCCESS) 
				{		
					int i, len = min(127, wcslen(Owner));
					for(i = 0; i < len; i++)
					{
						if(owner[i] != 0 && (owner[i] < 32 || owner[i] > (32 + 95)))
						{
							//To avoid problems with unicode chars
							owner[i] = '.';
							
						}
						else
						{
							owner[i] = (char)Owner[i];	
						}
					}				
				}

				free(Owner);
			}
		}
		
				
		
		RegCloseKey(m_hKey);
	}	
#endif

	return owner;
}


static val_t eic_getOwner(void)
{
    val_t v;
	v.ival = 0;
	char *owner = getOwner();

	v.p.sp = v.p.p = owner;
	
	setEp( v.p, strlen(owner));
    return v;
}



//------------------------------------------------------------------------------
//
// Gets the 10 byte preset id and the 6 byte platform id from the device, and
// returns it as a serial number in form (xxxxxxxxxx-xxx-xxx). The passed in
// buffer should be at least 20 characters in length.
//
//------------------------------------------------------------------------------
char *getHardwareID()
{	
	// Clear both buffers
	static char serial[20];
	memset(serial, 0, 20);

#ifdef __iPhone__
	strcpy(serial,"iPhone OS");
#endif

#ifdef _WIN32_WCE

	const UINT SERIAL_NUM_LEN = 16;
	const UINT PRESET_ID_LEN = 10;
	const UINT PLATFORM_ID_LEN = 6;
	
	unsigned long dwOutBytes;
	const INT BUF_SIZE = 128;
	byte arrOutBuff[BUF_SIZE];
	byte sn[SERIAL_NUM_LEN];

	memset(arrOutBuff, 0, BUF_SIZE);

	BOOL res = TRUE;
	
	// Some devices, like Dell, need the dwSize element of the DEVICE_ID
	// structure to be initialized.
	DEVICE_ID* p = (DEVICE_ID*)arrOutBuff;
	p->dwSize = BUF_SIZE;
	
	res = ::KernelIoControl(IOCTL_HAL_GET_DEVICEID, 0, 0, arrOutBuff,
		BUF_SIZE, &dwOutBytes);

	if(res)
	{
		int presetIdOffset = p->dwPresetIDOffset;
		int platformIdOffset = p->dwPlatformIDOffset;
		
		memcpy(&sn[0], &arrOutBuff[presetIdOffset], PRESET_ID_LEN);
		memcpy(&sn[PRESET_ID_LEN], &arrOutBuff[platformIdOffset],
			PLATFORM_ID_LEN);	

		//Use CRC32 to get a 16 digit string
		U32 crc1 = Crc32(&sn[0], PRESET_ID_LEN);
		U32 crc2 = Crc32(&sn[PRESET_ID_LEN], PLATFORM_ID_LEN);

		sprintf(serial, "%04X%04X", crc1, crc2);

		//Now, subst 1 -> K, 0 -> M, to avoid confusions

		unsigned int i;
		for(i = 0; i < strlen(serial); i++)
		{
			if(serial[i] == '1') serial[i] = 'K';
			else if(serial[i] == '0') serial[i] = 'M';
		}
	}
#endif
		
	return serial;
}

static val_t eic_getHardwareID(void)
{
    val_t v;
	v.ival = 0;
	char *hardwareID = getHardwareID();

	v.p.sp = v.p.p = hardwareID;
	
	setEp( v.p, strlen(hardwareID));
    return v;
}

double rand(double t)
{
	//return t * (rand() / (double)RAND_MAX);
	return t*randomMT()/4294967296.0;
}

double sign(double x)
{
	if(x == 0.0) return 0.0;
	else if(x > 0.0) return 1.0;
	else return -1.0;
}

double radtodeg(double teta)
{
	return teta*180.0/Script::PI;
}

double degtorad(double teta)
{
	return teta*Script::PI/180.0;
}



static val_t eic_rand(void)
{
    val_t v;
    v.dval = rand(arg(0,getargs(),double));
    return v;
}

static val_t eic_sign(void)
{
    val_t v;
    v.dval = sign(arg(0,getargs(),double));
    return v;
}

static val_t eic_radtodeg(void)
{
    val_t v;
    v.dval = radtodeg(arg(0,getargs(),double));
    return v;
}

static val_t eic_degtorad(void)
{
    val_t v;
    v.dval = degtorad(arg(0,getargs(),double));
    return v;
}

double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

static val_t eic_distance(void)
{
    val_t v;
    v.dval = distance(arg(0,getargs(),double),
		arg(1,getargs(),double),
		arg(2,getargs(),double),
		arg(3,getargs(),double)
		);
    return v;
}

double direction(double x1, double y1, double x2, double y2)
{
	/*
	Direction between (x1, y1) and (x2, y2)
	(0 to 360, from x axis, counterclockwise, in degrees)
	*/
	
	double angle = 0.0;
	
	if(x1 != x2 || y1 != y2)
	{
		if(y1 == y2)
		{
			if(x2 < x1) angle = 180.0;
			else angle = 0.0;
		}
		else if(x1 == x2)
		{
			if(y2 > y1) angle = 270.0;
			else angle = 90.0;
		}
		else
		{			
			angle = atan2(y1 - y2, x2 - x1)*180.0/Script::PI;
			if(angle < 0.0)
			{
				angle = 360.0 + angle;	
			}
		}
	}

	return angle;
}

double getAngle(double dx, double dy)
{
	return direction(0.0, 0.0, dx, dy);
}

static val_t eic_direction(void)
{
    val_t v;
    v.dval = direction(arg(0,getargs(),double),
		arg(1,getargs(),double),
		arg(2,getargs(),double),
		arg(3,getargs(),double)
		);
    return v;
}

void vectoradd(double *angle1, double *magnitude1, double angle2, double magnitude2)
{
	/*
	Add vector2 (angle2, magnitude2) to vector1 (angle1, magnitude1)
	Values returned in vector1
	Angles in degrees
	*/

	///////////////////////////////////
	//test
	/*int x1 = -163, y1 = 49,
		x2 = -175, y2 = -73;


	double a1, v1, a2, v2;

	a1 = direction(0, 0, x1, y1);
	v1 = distance(0, 0,  x1, y1);

	a2 = direction(0, 0, x2, y2);
	v2 = distance(0, 0,  x2, y2);

	vectoradd(&a1, &v1, a2, v2);*/
	////////////////////////////////////

	//Components
	double	teta1 = degtorad(*angle1),
			teta2 = degtorad(angle2),
			x1, y1, x2, y2, xres, yres;

	x1 = (*magnitude1)*cos(teta1);
	y1 = -(*magnitude1)*sin(teta1);

	x2 = magnitude2*cos(teta2);
	y2 = -magnitude2*sin(teta2);

	//Resultant
	xres = x1 + x2;
	yres = y1 + y2;

	*magnitude1 = sqrt(xres*xres + yres*yres);
	*angle1 = direction(0, 0, xres, yres);
}

static val_t eic_vectoradd(void)
{
    val_t v;
	v.ival = 0;

    vectoradd(
		(double *)arg(0,getargs(),ptr_t).p,		
		(double *)arg(1,getargs(),ptr_t).p,
		arg(2,getargs(),double),
		arg(3,getargs(),double)
		);

    return v;
}

#ifndef GP2X
double round(double x)
{
	if(x - floor(x) < .5) return floor(x);
	else return ceil(x);

	//More fast, but don't works (MorseAnim/Morse.ged)
	/*if(x - (int)(x) < .5) return (int)(x);
	else return (int)(x) + 1;*/
}
#endif

static val_t eic_round(void)
{
    val_t v;
    v.dval = round(arg(0,getargs(),double));
    return v;
}

/*double min(double a, double b)
{
	if(a < b) return a;
	else return b;
}*/

static val_t eic_min(void)
{
    val_t v;
    v.dval = min(arg(0,getargs(),double), arg(1,getargs(),double));
    return v;
}

/*double max(double a, double b)
{
	if(a >= b) return a;
	else return b;
}*/

static val_t eic_max(void)
{
    val_t v;
    v.dval = max(arg(0,getargs(),double), arg(1,getargs(),double));
    return v;
}

static val_t eic_setpen(void)
{
    val_t v;
	v.ival = 0;

    exec_setpen(arg(0,getargs(), int), arg(1,getargs(), int), arg(2,getargs(), int),
						 arg(3,getargs(), double),
						 arg(4,getargs(), int));
    return v;
}

static val_t eic_moveto(void)
{
    val_t v;
	v.ival = 0;

    exec_moveto(arg(0,getargs(), int), arg(1,getargs(), int));
    return v;
}

static val_t eic_lineto(void)
{
    val_t v;
	v.ival = 0;

    exec_lineto(arg(0,getargs(), int), arg(1,getargs(), int));
    return v;
}

static val_t eic_screen_to_actor(void)
{
    val_t v;
	v.ival = 0;

    exec_screen_to_actor((int *)arg(0,getargs(),ptr_t).p, (int *)arg(1,getargs(),ptr_t).p);

    return v;
}

static val_t eic_actor_to_screen(void)
{
    val_t v;
	v.ival = 0;

    exec_actor_to_screen((int *)arg(0,getargs(),ptr_t).p, (int *)arg(1,getargs(),ptr_t).p);

    return v;
}

static val_t eic_erase(void)
{
    val_t v;
	v.ival = 0;

    exec_erase(arg(0,getargs(), int), arg(1,getargs(), int), arg(2,getargs(), int),
						 arg(3,getargs(), double));
    return v;
}

static val_t eic_putpixel(void)
{
    val_t v;
	v.ival = 0;

    exec_putpixel(arg(0,getargs(), int), arg(1,getargs(), int));
    return v;
}

static val_t eic_savecanvas(void)
{
    val_t v;
	v.ival = 0;

    exec_savecanvas();
    return v;
}

static val_t eic_restorecanvas(void)
{
    val_t v;
	v.ival = 0;

    exec_restorecanvas();
    return v;
}

static val_t eic_draw_from(void)
{
    val_t v;
	v.ival = 0;	

	if(GameControl::Get()->getGameMode())
	{
		char *actorName = (char *)arg(0,getargs(),ptr_t).p;
		int x = arg(1,getargs(), int), y = arg(2,getargs(), int);
		double scale = arg(3,getargs(), double);
		bool bScreen = false;

		if(scale < 0.0 || !actorName) return v;
				
		Actor *eventActor = Action::getActualEventActor();
		Actor *collideActor = Action::getActualCollideActor();
		Actor *actor = NULL;
		KrCanvas *canvas = eventActor->getCanvas();
		
		if(!canvas) return v;

		//actor can't be the event actor (The event actor is the canvas!)
		if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
		{
			actor = collideActor;
		}		
		else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
		{
			if(eventActor->getParent() != GameControl::Get()->GetAxis() && IS_VALID_ACTOR(eventActor->getParent()))
			{
				actor = eventActor->getParent();
			}
		}
		else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
		{
			if(IS_VALID_ACTOR(eventActor->getCreator()))
			{
				actor = eventActor->getCreator();
			}
		}		
		else
		{			
			actor = GameControl::Get()->GetActor(actorName, true, false, false);
		}

		if(!actor && strcmp(actorName, S_SCREEN) == 0)
		{
			//From the screen
			bScreen = true;
		}
		

		if(!IS_VALID_ACTOR(actor) && !bScreen) return v;
		
		//Draw
		KrPaintInfo paintInfo(canvas->Pixels(), canvas->Width(), canvas->Height());
		KrRect clip;
		clip.Set(0, 0, paintInfo.width-1, paintInfo.height-1);

		if(bScreen)
		{
			//no screen now
			/*SDL_Rect sr, dr;

			//From
			sr.x = 0;
			sr.y = 0;
			sr.w = engine->Surface()->w;
			sr.h = engine->Surface()->h;

			//To
			sr.x = x;
			sr.y = y;
			sr.w = 100;
			sr.h = 100;

			//Blit
			SDL_BlitSurface(engine->Surface(), &sr, paintInfo.surface, &dr);*/
		}
		else
		{
			KrMatrix2 *pCompositeXForm = actor->getImage()->getCompositeXForm();
			KrMatrix2 compositeXFormCopy = *pCompositeXForm;

			//Change the actor position
			*pCompositeXForm = *eventActor->getImage()->getCompositeXForm();
			pCompositeXForm->Set(GlFixed(x), GlFixed(y), GlFixed(scale), GlFixed(scale));

			//Draw
			/*if(actor->getTextActor()) //No text now
			{
			engine->Tree()->DrawWalk(clip, actor->getTextActor()->getImage(), &paintInfo);
			}
			else*/
			{
				//Normal actor
				actor->getImage()->Draw(&paintInfo, clip);
			}

			//Restore actor position
			*pCompositeXForm = compositeXFormCopy;
		}

		//Refresh
		canvas->Refresh();
	}

    return v;
}

static stActorVars *actorCollisionArray = NULL;
static stActorVars *getAllActorsInCollision(Actor *actorTest, int &nActors)
{
	if(!actorTest) return NULL;

	if(actorCollisionArray) dlfree(actorCollisionArray);
	actorCollisionArray = NULL;


	GlDynArray<KrImage*> hittest;
	nActors = 0;

	engine->Tree()->CheckAllCollision(actorTest->getImage(), &hittest);
	
	//Count
	int n = hittest.Count(), i;
	for(i = 0; i < n; i++)
	{						
		KrImage *p = hittest.Item(i);
		Actor *actorHit = (Actor *)p->getParentActor();
		
		if(actorHit && 
		   actorHit != actorTest &&
		   actorHit->getRunning() &&			   
		   actorHit->getEnableCollisionDetection())
		{
			nActors++;
		}
	}


	//Alloc
	actorCollisionArray = /*new stActorVars[nActors];*/(stActorVars *)dlmalloc(Actor::getStructActorSize()*nActors);
	if(!actorCollisionArray) return NULL;


	//Assign
	char *pActorCollisionArray = (char *)actorCollisionArray; //To correct pointer operation
	nActors = 0;
	for(i = 0; i < n; i++)
	{						
		KrImage *p = hittest.Item(i);
		Actor *actorHit = (Actor *)p->getParentActor();
		
		if(actorHit && 
		   actorHit != actorTest &&
		   actorHit->getRunning() &&			   
		   actorHit->getEnableCollisionDetection())
		{
			memcpy(pActorCollisionArray/*actorCollisionArray+nActors*/, actorHit->getScriptVars(), Actor::getStructActorSize());
			pActorCollisionArray += Actor::getStructActorSize();
			nActors++;
		}
	}


	return actorCollisionArray;
}

static val_t eic_getAllActorsInCollision(void)
{
	/*
	Actor *getAllActorsInCollision(char *cloneName, int *nActors);
	cloneName: nameactor.cloneindex (Ex.: ship.1, ship.2, ...), "Event Actor" or "Collide Actor"
	
	Return actor's array with all actors colliding with cloneName if success or  NULL if don't have collisions
	Actor count will be returned in nActors

	The returned array will be valid until the next call to getAllActorsInCollision
	The returned array is read only.

	Sample: Make "Event Actor" parent of all colliding actors

		int n;
		Actor *actors;
 
		actors =  getAllActorsInCollision("Event Actor", &n);
 
		if(actors)
		{
			int i;
			for(i = 0; i < n; i++)
			{
				ChangeParent(actors[i].clonename, "Event Actor");
			}
		}
	*/

    val_t v;
	v.p.sp = v.p.p = NULL;
	
	if(GameControl::Get()->getGameMode())
	{
		char *actorName = (char *)arg(0,getargs(),ptr_t).p;
		int *nActors = (int *)arg(1,getargs(),ptr_t).p;
		*nActors = 0;

		if(!actorName) 
		{
			return v;
		}
		
		Actor *eventActor = Action::getActualEventActor();
		Actor *collideActor = Action::getActualCollideActor();
		stActorVars *actor = NULL;
		
		if(strcmp(actorName, S_EVENT_ACTOR) == 0)
		{
			actor = getAllActorsInCollision(eventActor, *nActors);	
		}
		else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
		{
			actor = getAllActorsInCollision(collideActor, *nActors);
		}		
		else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
		{
			if(eventActor->getParent() != GameControl::Get()->GetAxis() && IS_VALID_ACTOR(eventActor->getParent()))
			{
				actor = getAllActorsInCollision(eventActor->getParent(), *nActors);
			}
		}
		else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
		{
			if(IS_VALID_ACTOR(eventActor->getCreator()))
			{
				actor = getAllActorsInCollision(eventActor->getCreator(), *nActors);
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone actor
			Actor *actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actor = getAllActorsInCollision(actionActor, *nActors);
			}
		}
		

		if(actor && *nActors > 0)
		{
			v.p.sp = v.p.p = actor;
			setEp( v.p, Actor::getStructActorSize()*(*nActors) );
		}
		else
		{
			v.p.sp = v.p.p = NULL;
			return v;
		}
	}
	
	
    return v;
}


static val_t eic_getAnimIndex(void)
{
	/*
	int getAnimIndex(char *animName);
	animName: a valid animation name in the current Event Actor

	Get the index of an animation in Event Actor
	Return animation index if success, -1 on error
	*/

    val_t v;
	v.ival = -1;

	if(GameControl::Get()->getGameMode())
	{
		Actor *eventActor = Action::getActualEventActor();

		v.ival = eventActor->GetAnimationIndex((char *)arg(0,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_getAnimName(void)
{
	/*
	char *getAnimName(int animIndex);
	animIndex: a animaiton index in the Event Actor

	Get the name of an animation in Event Actor
	Return animation name if success, "" on error
	*/

    val_t v;
	v.p.sp = v.p.p = NULL;

	static char animName[NAME_LIMIT + 1];

	if(GameControl::Get()->getGameMode())
	{
		Actor *eventActor = Action::getActualEventActor();
		int index = arg(0,getargs(), int);

		if(index < 0)
		{
			strcpy(animName, "");
		}
		else
		{
			gedString name(eventActor->AnimationName(index + 2));
			strcpy(animName, name.getCharBuf());
		}

		v.p.sp = v.p.p = animName;		
		setEp( v.p, strlen(animName) + 1);
	}
	
    return v;
}

static val_t eic_getclone(void)
{
	/*
	Actor *getclone(const char *cloneName);
	cloneName: nameactor.cloneindex (Ex.: ship.1, ship.2, ...)
	Get actor with name cloneName
	Return actor if success, invalid actor (with cloneindex = -1 and name = "invalid actor") on error
	*/

    val_t v;
	v.p.sp = v.p.p = NULL;

	if(GameControl::Get()->getGameMode())
	{
		char *clonename = (char *)arg(0,getargs(),ptr_t).p;
		Actor *actor = GameControl::Get()->GetActor(clonename, true, false, false);

		if(actor && actor > COLLIDE_ACTOR)
		{
			Script::getListCreateActors()->PushBack(actor);

			//Don't set twice (Solve the issue 92)
			if(!actor->getSetActualValues()) actor->SetActualValues(NEED_ALL);

			v.p.sp = v.p.p = actor->getScriptVars();
		}
		else
		{
			v.p.sp = v.p.p = Script::invalidScriptActor;
		}
	}
	
	setEp( v.p, Actor::getStructActorSize() );
    return v;
}

Actor *getactor(int x, int y)
{
	Actor *actor = NULL;
	
	KrVector2T< GlFixed > screen;
	GameControl::Get()->GetAxis()->getImage()->ObjectToScreen( x, y, &screen );
	
	GlDynArray<KrImage*> hittest;
	engine->Tree()->HitTest(screen.x.ToInt(), screen.y.ToInt(), 0, &hittest); 
	
	if(hittest.Count() > 0)
	{
		//Get actor			
		KrImage *p = hittest.Item(0);
		actor = (Actor *)p->getParentActor();
	}

	return actor;
}

static val_t eic_getactor(void)
{
	/*
	Actor *getactor(int x, int y);
	x, y: game coordinates
	Returns actor  at position (x,y) if success, invalid actor (with cloneindex = -1 and name = "invalid actor") on error	
	*/

    val_t v;
	v.p.sp = v.p.p = NULL;

	if(GameControl::Get()->getGameMode())
	{
		Actor *actor = getactor(arg(0,getargs(), int), arg(1,getargs(), int));		

		if(actor > COLLIDE_ACTOR)
		{
			Script::getListCreateActors()->PushBack(actor);
			//Don't set twice (Solve the issue 92)
			if(!actor->getSetActualValues()) actor->SetActualValues(NEED_ALL);
			v.p.sp = v.p.p = actor->getScriptVars();
		}
		else
		{
			v.p.sp = v.p.p = Script::invalidScriptActor;
		}
	}
	
	setEp( v.p, Actor::getStructActorSize() );
    return v;
}




void includeCLibs()
{
	char buf[64]; 
	module_math();
	module_stdio();
	module_stdlib();
	module_string();

	//helper functions
	EiC_add_builtinfunc("radtodeg",eic_radtodeg);
	EiC_parseString("double radtodeg(double a);");

	EiC_add_builtinfunc("degtorad",eic_degtorad);
	EiC_parseString("double degtorad(double a);");

	EiC_add_builtinfunc("round",eic_round);
	EiC_parseString("double round(double a);");

	EiC_add_builtinfunc("min",eic_min);
	EiC_parseString("double min(double a, double b);");

	EiC_add_builtinfunc("max",eic_max);
	EiC_parseString("double max(double a, double b);");

	EiC_add_builtinfunc("distance",eic_distance);
	EiC_parseString("double distance(double x1, double y1, double x2, double y2);");

	EiC_add_builtinfunc("direction",eic_direction);
	EiC_parseString("double direction(double x1, double y1, double x2, double y2);");

	EiC_add_builtinfunc("vectoradd",eic_vectoradd);
	EiC_parseString("void vectoradd(double *angle1, double *magnitude1, double angle2, double magnitude2);");
	
	//drawing functions
	EiC_add_builtinfunc("setpen",eic_setpen);
	EiC_parseString("void setpen(int r, int g, int b, double transp, int pensize);");

	EiC_add_builtinfunc("moveto",eic_moveto);
	EiC_parseString("void moveto(int x, int y);");

	EiC_add_builtinfunc("lineto",eic_lineto);
	EiC_parseString("void lineto(int x, int y);");

	EiC_add_builtinfunc("screen_to_actor",eic_screen_to_actor);
	EiC_parseString("void screen_to_actor(int *x, int *y);");

	EiC_add_builtinfunc("actor_to_screen",eic_actor_to_screen);
	EiC_parseString("void actor_to_screen(int *x, int *y);");

	EiC_add_builtinfunc("erase",eic_erase);
	EiC_parseString("void erase(int r, int g, int b, double transp);");

	EiC_add_builtinfunc("putpixel",eic_putpixel);
	EiC_parseString("void putpixel(int x, int y);");

	EiC_add_builtinfunc("savecanvas",eic_savecanvas);
	EiC_parseString("void savecanvas();");

	EiC_add_builtinfunc("restorecanvas",eic_restorecanvas);
	EiC_parseString("void restorecanvas();");

	EiC_add_builtinfunc("draw_from", eic_draw_from);
	EiC_parseString("void draw_from(const char *actorname, int x, int y, double scale);");

	//Some C functions (I know... is stupid place this includes here.... but....)
	EiC_add_builtinfunc("abs",eic_abs);
	EiC_parseString("double abs(double a);");

	EiC_add_builtinfunc("sign",eic_sign);
	EiC_parseString("double sign(double a);");

	EiC_add_builtinfunc("rand",eic_rand);
	EiC_parseString("double rand(double a);");	

dodefine("HUGE_VAL=1.0e38");

EiC_parseString("double acos(double a);");
EiC_parseString("double asin(double a);");
EiC_parseString("double atan(double a);");
EiC_parseString("double atan2(double a, double b);");
EiC_parseString("double cos(double a);");
EiC_parseString("double sin(double a);");
EiC_parseString("double tan(double a);");
EiC_parseString("double cosh(double a);");
EiC_parseString("double sinh(double a);");
EiC_parseString("double tanh(double a);");
EiC_parseString("double exp(double a);");
EiC_parseString("double frexp(double a, int * b);");
EiC_parseString("double ldexp(double a, int b);");
EiC_parseString("double log(double a);");
EiC_parseString("double log10(double a);");
EiC_parseString("double modf(double a, double *b);");
EiC_parseString("double pow(double a, double b);");
EiC_parseString("double sqrt(double a);");
EiC_parseString("double ceil(double a);");
//EiC_parseString("double fabs(double a);");
EiC_parseString("double floor(double a);");
EiC_parseString("double fmod(double a, double b);");

EiC_parseString("typedef unsigned int size_t;");
EiC_parseString("void *malloc(size_t size);");
EiC_parseString("void *calloc(size_t num, size_t size);");
EiC_parseString("void *realloc(void *memblock, size_t size);");
EiC_parseString("void free(void *memblock);");
EiC_parseString("void *memcpy( void *dest, const void *src, size_t count );");
EiC_parseString("void *memmove( void *dest, const void *src, size_t count );");
EiC_parseString("int memcmp( const void *buf1, const void *buf2, size_t count );");
EiC_parseString("void *memset( void *dest, int c, size_t count );");

EiC_parseString("int atoi(const char *string);");
EiC_parseString("double atof(const char *string);");
EiC_parseString("long atol(const char *string); ");

sprintf(buf, "EOF=%ld", EOF); dodefine(buf);
sprintf(buf, "NULL=%ld", NULL); dodefine(buf);
sprintf(buf, "SEEK_END=%ld", SEEK_END); dodefine(buf);
sprintf(buf, "SEEK_CUR=%ld", SEEK_CUR); dodefine(buf);
sprintf(buf, "SEEK_SET=%ld", SEEK_SET); dodefine(buf);



/*#ifdef _WIN32_WCE
	EiC_parseString("typedef void FILE;");	
#elif WIN32
	EiC_parseString("struct _iobuf {\
        char *_ptr;\
        int   _cnt;\
        char *_base;\
        int   _flag;\
        int   _file;\
        int   _charbuf;\
        int   _bufsiz;\
        char *_tmpfname;\
        };\
		typedef struct _iobuf FILE;");
#elif __SYMBIAN32__
	EiC_parseString("typedef long _fpos_t;");
	EiC_parseString("struct __sbuf {\
	unsigned char *	_base;\
	int				_size;\
	};");

	EiC_parseString("struct __sFILE {\
	unsigned char *_p;\
	int			 _r;\
	int			 _w;\
	short			 _flags;\
	short			 _file;\
	struct __sbuf	 _bf;\
	int			 _lbfsize;\
	void *		 _cookie;\
	int		(*_read) (void * _cookie, char *_buf, int _n);\
	int		(*_write)(void * _cookie, const char *_buf, int _n);\
	_fpos_t	(*_seek) (void * _cookie, _fpos_t _offset, int _whence);\
	int		(*_close)(void * _cookie);\
	struct __sbuf		_ub;\
	unsigned char *	_up;\
	int				_ur;\
	unsigned char		_ubuf[3];\
	unsigned char		_nbuf[1];\
	struct __sbuf		_lb;\
	int				_blksize;\
	int				_offset;\
	struct _reent *	_data;\
	};");
#else
#endif*/

//Generic FILE
#if defined(_WIN32_WCE)
EiC_parseString("#define FILE void");
#else
EiC_parseString("typedef struct { char dummy[%ld]; } FILE;", sizeof(FILE));
#endif

EiC_parseString("long ftell(FILE *fp);");
EiC_parseString("int feof(FILE *stream);");
EiC_parseString("int sprintf(char *buf, const char * fmt, ...);");
EiC_parseString("int fprintf(FILE * fp,const char *fmt, ...);");
EiC_parseString("int fgetc(FILE * fp);");
EiC_parseString("int fputc(int c, FILE * fp);");
EiC_parseString("int fputs(const char *s, FILE *fp);");
EiC_parseString("char * fgets(char *s, int n, FILE *fp);");
EiC_parseString("size_t fread(void *buf, size_t elsize, size_t nelems, FILE * fp);");
EiC_parseString("size_t fwrite(const void *buf, size_t elsize, size_t nelems, FILE * fp);");
EiC_parseString("FILE *freopen(const char *name, const char *mode, FILE *fp);");
EiC_parseString("FILE *fopen(const char *name, const char *mode);");
EiC_parseString("int fclose(FILE * fp);");
EiC_parseString("int fscanf(FILE *fp, const char *fmt, ...);");
EiC_parseString("int sscanf(const char *str,const char *fmt, ...);");
EiC_parseString("int fseek(FILE *fp, long off, int origin);");
EiC_parseString("int rename(const char *oldname, const char *newname);");

EiC_parseString("char *strcpy(char * dst, const char * src);");
EiC_parseString("char *strncpy(char * dst, const char * src, size_t n);");
EiC_parseString("char *strcat(char * s1, const char * s2);");
EiC_parseString("char *strncat(char * s1, const char *s2, size_t n);");
EiC_parseString("int strcmp(const char * s1, const char * s2);");
EiC_parseString("int strncmp(const char * s1, const char *s2, size_t n);");
EiC_parseString("char *strchr(const char *s, int c);");
EiC_parseString("size_t  strlen(const char *s);");

GLASSERT(Script::GetError() == "");
}



////////////////////////////////////
//Actions function

static val_t eic_ChangeAnimation(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeAnimation((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p,
			arg(2,getargs(),int));
	}

    return v;
}

static val_t eic_ChangeAnimationDirection(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeAnimationDirection((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),int));
	}

    return v;
}

static val_t eic_ChangeTransparency(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeTransparency((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double));
	}
	
    return v;
}

static val_t eic_CollisionState(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execCollisionState((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),int));
	}
	
    return v;
}

static val_t eic_VisibilityState(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execVisibilityState((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),int));
	}
	
    return v;
}

static val_t eic_ChangePath(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangePath((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p,
			arg(2,getargs(),int));
	}

    return v;
}

static val_t eic_FollowMouse(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execFollowMouse((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),int));
	}
	
    return v;
}

static val_t eic_ChangeParent(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeParent((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_SendActivationEvent(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execActivationEvent((char *)arg(0,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_RequestActorOwnership(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		Actor *eventActor = Action::getActualEventActor();
		Actor *collideActor = Action::getActualCollideActor();
		char *name = (char *)arg(0,getargs(),ptr_t).p;
		char *cloneName = name;

		if(strcmp(name, S_EVENT_ACTOR) == 0)
		{
			cloneName = eventActor->getCloneName();
		}
		else if(strcmp(name, S_PARENT_ACTOR) == 0)
		{
			if(eventActor->getParent() != GameControl::Get()->GetAxis()) 
			{
				cloneName = eventActor->getParent()->getCloneName();
			}
		}
		else if(strcmp(name, S_CREATOR_ACTOR) == 0)
		{
			cloneName = eventActor->getCreator()->getCloneName();
		}
		else if(strcmp(name, S_COLLIDE_ACTOR) == 0)
		{
			if(collideActor)
			{
				cloneName = collideActor->getCloneName();
			}
		}

		v.ival = GameControl::Get()->RequestActorOwnership(cloneName)?1:0;
	}
	
    return v;
}

static val_t eic_ReleaseActorOwnership(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		Actor *eventActor = Action::getActualEventActor();
		Actor *collideActor = Action::getActualCollideActor();
		char *name = (char *)arg(0,getargs(),ptr_t).p;
		char *cloneName = name;

		if(strcmp(name, S_EVENT_ACTOR) == 0)
		{
			cloneName = eventActor->getCloneName();
		}
		else if(strcmp(name, S_PARENT_ACTOR) == 0)
		{
			if(eventActor->getParent() != GameControl::Get()->GetAxis()) 
			{
				cloneName = eventActor->getParent()->getCloneName();
			}
		}
		else if(strcmp(name, S_CREATOR_ACTOR) == 0)
		{
			cloneName = eventActor->getCreator()->getCloneName();
		}
		else if(strcmp(name, S_COLLIDE_ACTOR) == 0)
		{
			if(collideActor)
			{
				cloneName = collideActor->getCloneName();
			}
		}

		v.ival = GameControl::Get()->ReleaseActorOwnership(cloneName)?1:0;
	}
	
    return v;
}

static val_t eic_Connect(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode() && GameControl::Get()->GetNetWork())
	{
		char *ip = (char *)arg(0,getargs(),ptr_t).p;
		int port = arg(1,getargs(),int);

		GameControl::Get()->Connect(ip, port);
	}
	
    return v;
}

static val_t eic_NetworkStart(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		int port = arg(0,getargs(),int);
		v.ival = GameControl::Get()->InitRakNet(port);
	}
	
    return v;
}

static val_t eic_NetworkShutdown(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		GameControl::Get()->DestroyRakNet();
	}
	
    return v;
}


stCursor *pCursorArgs = NULL;
static val_t eic_ChangeCursor(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeCursor((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p,
			arg(2,getargs(),int),
			arg(3,getargs(),int),
			arg(4,getargs(),int),
			arg(5,getargs(),int));
	}
#ifndef STAND_ALONE_GAME
	else if(pCursorArgs)
	{
		//Get args only
		pCursorArgs->imgName = (char *)arg(1,getargs(),ptr_t).p;
		pCursorArgs->nFramesH = arg(2,getargs(),int);
		pCursorArgs->nFramesV = arg(3,getargs(),int);
		pCursorArgs->hotSpotX = arg(4,getargs(),int);
		pCursorArgs->hotSpotY = arg(5,getargs(),int);

		pCursorArgs = NULL;
	}
#endif
	
    return v;
}

static val_t eic_CreateActor(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		Actor *actor = execCreateActor((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p,
			(char *)arg(2,getargs(),ptr_t).p,
			(char *)arg(3,getargs(),ptr_t).p,
			arg(4,getargs(),int),
			arg(5,getargs(),int),
			arg(6,getargs(),int));

		if(actor)
		{
			Script::getListCreateActors()->PushBack(actor);
			actor->SetActualValues(NEED_ALL);
			v.p.sp = v.p.p = actor->getScriptVars();
		}
		else
		{
			v.p.sp = v.p.p = Script::invalidScriptActor;
		}
	}
	
	setEp( v.p, Actor::getStructActorSize() );
    return v;
}

static val_t eic_DestroyActor(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execDestroyActor((char *)arg(0,getargs(),ptr_t).p);
	}

    return v;
}

static val_t eic_CreateTimer(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execCreateTimer((char *)arg(0,getargs(),ptr_t).p, 
			(char *)arg(1,getargs(),ptr_t).p,					
			arg(2,getargs(),int));
	}
	
    return v;
}

static val_t eic_DestroyTimer(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execDestroyTimer((char *)arg(0,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_ChangeZDepth(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execChangeZDepth((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double));
	}
	
    return v;
}

static val_t eic_PlaySound(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execPlaySound((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double),
			arg(2,getargs(),int),
			0);
	}
	
    return v;
}

static val_t eic_PlaySound2(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execPlaySound((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double),
			arg(2,getargs(),int),
			(Sint16)(arg(3,getargs(),double)*255.0)); //(-1.0 to 1.0)
	}
	
    return v;
}

static val_t eic_PlayMusic(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execPlayMusic((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double),
			arg(2,getargs(),int),
			0);
	}
	
    return v;
}

static val_t eic_GetJoystick1Axis(void)
{	extern SDL_Joystick * joystick;
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = SDL_JoystickGetAxis(joystick,arg(0,getargs(),int)); 
							
	}
	
    return v;
	
	
}

static val_t eic_GetJoystick1Button(void)
{	extern SDL_Joystick * joystick;
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = SDL_JoystickGetButton(joystick,arg(0,getargs(),int)); 
		
	}
	
    return v;
	
	
}

static val_t eic_PlayMusic2(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execPlayMusic((char *)arg(0,getargs(),ptr_t).p, 
			arg(1,getargs(),double),
			arg(2,getargs(),int),
			arg(3,getargs(),int));
	}
	
    return v;
}

static val_t eic_setPan(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode() && GameControl::Get()->getAudioOpened())
	{
		int channel = arg(0,getargs(),int);
		double pan = arg(1,getargs(),double)*255.0;

		if(channel == 1) return v; //0 error, 1 music (no pan for music)
		if(pan < -255.0) pan = -255.0;
		else if(pan > 255.0) pan = 255.0;

		if(channel >= 2) channel -= 2;
		else if(channel <= 0) channel = -1; //Set all channels

		Mix_SetPanning(channel, (pan <= 0)?255:(255-pan), (pan < 0)?(255+pan):255);
	}
	
    return v;
}

static val_t eic_setVolume(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode() && GameControl::Get()->getAudioOpened())
	{
		int channel = arg(0,getargs(),int);
		double volume = arg(1,getargs(),double);

		if(volume < 0.0) volume = 0.0;
		else if(volume > 1.0) volume = 1.0;
		
		volume *= SDL_MIX_MAXVOLUME;
		
		if(channel == 1)
		{
			Mix_VolumeMusic(volume);
		}
		else
		{		
			if(channel >= 2) channel -= 2;
			else if(channel <= 0) channel = -1; //Set all channels

			Mix_Volume(channel, (int)volume);
		}
	}
	
    return v;
}

static val_t eic_stopSound(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode() && GameControl::Get()->getAudioOpened())
	{
		int channel = arg(0,getargs(),int);
		
		
		
		if(channel == 1)
		{
			Mix_HaltMusic();
		}
		else
		{		
			if(channel >= 2) channel -= 2;
			else if(channel <= 0) channel = -1; //Set all channels

			Mix_HaltChannel(channel);
		}
	}
	
    return v;
}

static val_t eic_ToAnteriorPosition(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execToAnteriorPosition((char *)arg(0,getargs(),ptr_t).p);
	}

    return v;
}

static val_t eic_openUrl(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		gedString url((char *)arg(0,getargs(),ptr_t).p);
		gedString aux(url);
		aux.lower();

		if(aux.substr(0, 7) != "http://" && aux.substr(0, 6) != "ftp://")
		{
			url = gedString("http://") + url;
		}

		openUrl(url.c_str());
	}

    return v;
}

static val_t eic_saveVars(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		GameControl::Get()->SaveVars((char *)arg(0,getargs(),ptr_t).p,
			(char *)arg(1,getargs(),ptr_t).p);
	}

    return v;
}

static val_t eic_loadVars(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		GameControl::Get()->LoadVars((char *)arg(0,getargs(),ptr_t).p,
			(char *)arg(1,getargs(),ptr_t).p);
	}

    return v;
}

static val_t eic_EventEnable(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execEventEnable((char *)arg(0,getargs(),ptr_t).p,
			arg(1,getargs(),unsigned long));
	}
	
    return v;
}

static val_t eic_EventDisable(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execEventDisable((char *)arg(0,getargs(),ptr_t).p,
			arg(1,getargs(),unsigned long));
	}
	
    return v;
}

static val_t eic_ExitGame(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execExitGame();
	}
	
    return v;
}

static val_t eic_LoadGame(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execLoadGame((char *)arg(0,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_SuspendGame(void)
{
    val_t v;
	v.ival = 0;
	
	GameControl::Get()->SuspendGame(true);
	
    return v;
}

static val_t eic_PauseGameOn(void)
{
    val_t v;
	v.ival = 0;
	
	GameControl::Get()->PauseGame(true);
	
    return v;
}

static val_t eic_PauseGameOff(void)
{
    val_t v;
	v.ival = 0;
	
	GameControl::Get()->PauseGame(false);
	
    return v;
}

static val_t eic_ActorCount(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		v.ival = execActorCount((char *)arg(0,getargs(),ptr_t).p);
	}
	
    return v;
}

static val_t eic_CollisionFree(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execCollisionFree((char *)arg(0,getargs(),ptr_t).p,
			arg(1,getargs(),int),
			arg(2,getargs(),int));
	}
	
    return v;
}


static val_t eic_MoveTo(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		char *actorName = (char *)arg(0,getargs(),ptr_t).p;

		v.ival = execMoveTo(actorName,
			arg(1,getargs(),double),
			arg(2,getargs(),double),
			arg(3,getargs(),double),
			(char *)arg(4,getargs(),ptr_t).p,
			(char *)arg(5,getargs(),ptr_t).p,
			STRAIGHT_LINE);

		//Adjust event or collide actor
		if(actorName)
		{
			Actor *eventActor = Action::getActualEventActor();
			Actor *collideActor = Action::getActualCollideActor();
			
			/*if(strcmp(actorName, S_EVENT_ACTOR) == 0)
			{
				Script::getEventActorVars()->xvelocity = eventActor->getVelocityX();	
				Script::getEventActorVars()->yvelocity = eventActor->getVelocityY();	
			}
			else*/ if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
			{
				if(collideActor)
				{
					Script::getCollideActorVars()->xvelocity = eventActor->getVelocityX();	
					Script::getCollideActorVars()->yvelocity = eventActor->getVelocityY();
				}
			}
		}
	}
/*#ifndef STAND_ALONE_GAME
	else if(newFunctionCall)
	{
		//Don't works if call use undeclarated vars
		//Get args only
		sprintf(newFunctionCall, "MoveTo(\"%s\", %f, %f, %f, \"%s\");",
			(char *)arg(0,getargs(),ptr_t).p,
			arg(1,getargs(),double),
			arg(2,getargs(),double), 
			arg(3,getargs(),double), 
			"Game Center"//(char *)arg(4,getargs(),ptr_t).p
			);
		
		newFunctionCall = NULL;
	}
#endif*/
	
    return v;
}

static val_t eic_PhysicalBounce(void)
{
    val_t v;
	v.ival = 0;

	if(GameControl::Get()->getGameMode())
	{
		v.ival = execPhysicalBounce(
			arg(0,getargs(),int),
			arg(1,getargs(),int),
			arg(2,getargs(),double),
			arg(3,getargs(),double),
			arg(4,getargs(),double),
			arg(5,getargs(),double));		
	}
	
    return v;
}

static val_t eic_GetKeyState(void)
{
    val_t v;
	v.ival = 0;
#ifndef __APPLE__
	if(GameControl::Get()->getGameMode())
	{
		int numkeys = 0;
		Uint8 *keystate = SDL_GetKeyState(&numkeys);	

		memset(Script::keyState, 0, sizeof(Script::keyState));
		memcpy(Script::keyState, keystate, max(sizeof(Script::keyState), numkeys));
		v.p.sp = v.p.p = Script::keyState;
	}
	
	setEp( v.p, sizeof(Script::keyState));
#endif
    return v;
}

static val_t eic_getLastKey(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		SystemAddress owner = UNASSIGNED_SYSTEM_ADDRESS;
		Actor *eventActor = Action::getActualEventActor();

		if(eventActor && eventActor->getNetActor())
		{
			owner = eventActor->GetOwner();
		}

		v.ival = GameControl::Get()->GetLastKey(owner);
	}
	
	return v;
}

static char keyDesc[256];
static val_t eic_getKeyText(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		int key = arg(0,getargs(), int);
		if(key < 0) key = 0;

		strcpy(keyDesc, SDL_GetKeyName((SDLKey)key));

		//Common fonts has up characters only
		int len = strlen(keyDesc);
		for(int i = 0; i < len; i++)
		{
			if('a' <= keyDesc[i] && keyDesc[i] <= 'z')
			{
				keyDesc[i] += 'A' - 'a';
			}
		}
		

		v.p.sp = v.p.p = keyDesc;		
		
		setEp( v.p, len + 1);
	}	
	
	return v;
}

static val_t eic_remapKey(void)
{
    val_t v;
	v.ival = 0;
	
	if(GameControl::Get()->getGameMode())
	{
		GameControl::Get()->MapUserKey((SDLKey)arg(0,getargs(), int), (SDLKey)arg(1,getargs(), int));
	}	
	
	return v;
}


////////////////////////////////////
#ifndef __iPhone__
Uint8 Script::keyState[SDLK_LAST + 1];
#endif
int Script::frame = 0;
int Script::real_fps = 0;
int Script::mousex = 0;
int Script::mousey = 0;
double Script::musicVol = 0;
gedString Script::error = "";
stActorVars *Script::eventActorVars = NULL;
stActorVars *Script::collideActorVars = NULL;
ListActors Script::listCreateActors;
int Script::conditionalResult = 0;
double Script::PI = 4*atan((double)1);

/*StackInt Script::stackInt;
StackDouble Script::stackDouble;
StackActorVars Script::stackActorVars;
StackActorVarsPtr Script::stackActorVarsPtr;*/
StackListActors Script::stackListActors;


struct stActorVars *Script::invalidScriptActor = NULL;
struct stActorVars *Script::fakeEditActor = NULL;

gedString baseStructActor = "typedef struct stActorVars\
						{\
							double x;\
							double y;\
							double xprevious;\
							double yprevious;\
							double xscreen;\
							double yscreen;\
							double r;\
							double g;\
							double b;\
							double transp;\
							double pathxpos;\
							double pathypos;\
							double animpos;\
							int animindex;\
							int nframes;\
							double angle;\
							double xvelocity;\
							double yvelocity;\
							double directional_velocity;\
							int width;\
							int height;\
							double textNumber;\
							char text[MAX_SCRIPTTEXT];\
							const char name[ACTOR_NAME];\
							const char clonename[CLONE_NAME];\
							const long cloneindex;";	
gedString endStructActor = "} Actor;";

gedString timeStruct = "typedef struct st_Time\
		{\
			int sec;\
			int min;\
			int hour;\
\
			int mday;\
			int mon;\
			int year;\
\
			int wday;\
			int yday;\
\
			unsigned long sec_utc;\
		} stTime;";

MapUsedActor Script::mapLocalUserVar;

class SymInfo
{
public:
	symentry_t **iSym;
	int offset;
};

SymInfo *symInfo = NULL;
int totalVars = 0;
symentry_t **iSymParent, **iSymCreator;

Script::Script()
{
	Init();	
	bParserOk = false;
	bAddedWithError = false;
	updateFlags = 0;
	conditionalResult = 0;
	functionAddress = NULL;	
}

Script::~Script()
{
	if(GameControl::Get()->getDestroingGame())
	{
		//May be some actors in usedActors don't exists
		//So, clean up the symbols to avoid erros in next game
		//Solve bees/bees_demo.ged, bot/04 seed.ged bug

		//Create fake actors if necessary
		for(int i = 0; i < usedActor.Count(); i++)
		{
			RemoveSymbol(usedActor[i].c_str());
		}
	}

	RemoveSymbol(functionName.c_str());		
}


void Script::myDisplay(char *msg)
{
	if(strlen(msg) > 0)
	{
#ifndef STAND_ALONE_GAME
		gedString currentError;

		if(GameControl::Get() && GameControl::Get()->getGameMode())
		{
			//Runtime error
			if(InRunEiC())
			{
				//No error on parser in game mode
				return;
			}

			if(!error.length() || error.at(error.length() - 1) == '\n')
			{
				if(currentEventActor)
				{
					currentError += currentEventActor->getActorName();
					currentError += " -> ";
				}
				
				currentError += Action::GetDescription(currentAction, false);
				
				if(!GetCurrentFunctionName().empty())
				{
					//Global code function
					currentError += ", in ";
					currentError += GetCurrentFunctionName();					
					currentError += " function";					
				}

				long line = CurrentExecutionLine();

				if(line >= 0)
				{
					currentError += ", line ";
					currentError += gedString(line);
				}

				currentError += ": ";

				//Stop execution
				SDL_Event event;
				memset(&event, 0, sizeof(SDL_Event));
				event.type = SDL_KEYUP;
				event.key.keysym.sym = SDLK_ESCAPE;
				SDL_PushEvent(&event);
			}			
		}

		currentError += msg;

		if(error.find(currentError) == gedString::npos)
		{
			//Avoid duplications
			error += currentError;
		}
#else
		error += msg;
#endif
	}
}

/*static long sizeLong, sizeDouble;
static void calibrateSize()
{
	//Get real align/size
	
	
	EiC_parseString("long _geds1 @ %ld;\
					 long _geds2 @ %ld;\
					 \
					 struct{\
						char cdummy;\
						double l;\
					} sl;\
					\
					 struct _gedstAlign {\
						long a;\
						double b;\
						long c;\
						};\
					 \
					 \
					 struct _gedstAlign _gedt;\
					 _geds1 = (char *)&_gedt.b - (char *)&_gedt.a;\
					 _geds2 = (char *)&_gedt.c - (char *)&_gedt.b;", (long)&sizeLong, (long)&sizeDouble);

	Script::RemoveSymbol("_geds1");
	Script::RemoveSymbol("_geds2");
	Script::RemoveSymbol("_gedt");
	Script::RemoveSymbol("_gedstAlign");
}*/

static long getRealActorSize()
{
	//Get real Actor align/size
	long s1 = 0;

	Script::RemoveSymbol("_gedActors1");
	Script::RemoveSymbol("_gedt");
	Script::RemoveSymbol("_gedActorAlign");
	

	EiC_interpON = 1;
	EiC_parseString("long _gedActors1 @ %ld;\
												  \
					 struct _gedActorAlign {\
					 Actor a;\
					 char b;\
					 };\
					 \
					 struct _gedActorAlign _gedt;\
					 _gedActors1 = (char *)&_gedt.b - (char *)&_gedt.a;", (long)&s1);
	EiC_interpON = 0;

	
	Script::RemoveSymbol("_gedActors1");
	Script::RemoveSymbol("_gedt");
	Script::RemoveSymbol("_gedActorAlign");

	return s1;
}

#ifdef _DEBUG
extern "C" void showcode(FILE *fp, InsT_t * inst,int i);

typedef GlMap< U64, U32, GlNumberHash<U64> >		MapStats;
typedef GlMapIterator< U64, U32, GlNumberHash<U64> >		MapStatsIterator;


void CodeAnalyse(int nGroup)
{
	//Stats from execution.dat from EiC_interpret

	MapStats stats;
	U32 total = 0;

	
	FILE *fp = fopen("execution.dat", "rb");
	if(!fp) return;

	//Most executed instructions
	if(nGroup == 1)
	{
		while(!feof(fp))
		{
			U16 op = 0;
			fread(&op, sizeof(op), 1, fp); total++;
			
			if(op)
			{
				U32 *count = stats[op];
				
				if(count)
				{
					stats.Add(op, *count + 1);
				}
				else
				{
					stats.Add(op, 1);
				}
			}
		}
	}
	else if(nGroup == 2)
	{
		//Most executed instructions pair
		U16 currentOp = 0, lastOp = 0;
		
		while(!feof(fp) && fread(&currentOp, sizeof(U16), 1, fp) == 1)
		{
			total++;

			if(currentOp)
			{
				if(currentOp && lastOp)
				{					
					U64 op = lastOp << 16 | currentOp;
					U32 *count = stats[op];
					
					if(count)
					{
						stats.Add(op, *count + 1);
					}
					else
					{
						stats.Add(op, 1);
					}
				}

				lastOp = currentOp;
			}
			else
			{
				lastOp = 0;
			}			
		}
	}
	else if(nGroup == 3)
	{
		//Most executed instructions tri
		U16 currentOp = 0, lastOp1 = 0, lastOp2 = 0;
		
		while(!feof(fp) && fread(&currentOp, sizeof(U16), 1, fp) == 1)
		{
			total++;

			if(currentOp)
			{
				if(currentOp && lastOp1 && lastOp2)
				{			
					U64 op = (U64)lastOp2 << 32 | lastOp1 << 16 | currentOp;
					U32 *count = stats[op];
					
					if(count)
					{
						stats.Add(op, *count + 1);
					}
					else
					{
						stats.Add(op, 1);
					}
				}

				lastOp2 = lastOp1;
				lastOp1 = currentOp;				
			}
			else
			{
				lastOp1 = lastOp2 = 0;
			}
		}
	}
	else if(nGroup == 4)
	{
		//Most executed four instructions
		U16 currentOp = 0, lastOp1 = 0, lastOp2 = 0, lastOp3 = 0;
		
		while(!feof(fp) && fread(&currentOp, sizeof(U16), 1, fp) == 1)
		{
			total++;

			if(currentOp)
			{
				if(currentOp && lastOp1 && lastOp2 && lastOp3)
				{			
					U64 op = (U64)lastOp3 << 48 | (U64)lastOp2 << 32 | lastOp1 << 16 | currentOp;
					U32 *count = stats[op];
					
					if(count)
					{
						stats.Add(op, *count + 1);
					}
					else
					{
						stats.Add(op, 1);
					}
				}

				lastOp3 = lastOp2;
				lastOp2 = lastOp1;				
				lastOp1 = currentOp;				
			}
			else
			{
				lastOp1 = lastOp2 = lastOp3 = 0;
			}
		}
	}

	fclose(fp);

	//Generate stats
	fp = fopen("execStats.txt", "w");

	MapStatsIterator it(stats);
	for(it.Begin(); !it.Done(); it.Next())
	{
		InsT_t inst;
		inst.ext = 0;

#ifndef STAND_ALONE_GAME
		inst.line = 0;
#endif

		inst.val.ival = 0;
		//inst.val1.ival = 0;

		U64 op = *it.Key();
		U32 count =  *it.Value();

		U16 op1 = (op >> 48) & 0xFFFF;
		U16 op2 = (op >> 32) & 0xFFFF;
		U16 op3 = (op >> 16) & 0xFFFF;
		U16 op4 = op & 0xFFFF;

		if(op1)
		{
			inst.opcode = op1;
			showcode(fp, &inst, -1);
			fprintf(fp, "_");
		}

		if(op2)
		{
			inst.opcode = op2;
			showcode(fp, &inst, -1);
			fprintf(fp, "_");
		}

		if(op3)
		{
			inst.opcode = op3;
			showcode(fp, &inst, -1);
			fprintf(fp, "_");
		}

		inst.opcode = op4;
		showcode(fp, &inst, -1);
		fprintf(fp, "\t%05.2f\n", 100.0*count/(float)total);
	}

	fclose(fp);

	exit(0);
}
#endif


void Script::Init()
{
	static bool bInit = false;
	if(bInit) return;
	bInit = true;
	firstInst.inst = NULL;

#ifdef _DEBUG
	//CodeAnalyse(4);
#endif

	EiC_interpON = 0; //Turn off the interpreter to speed up the parser

	GLASSERT(EiC_getsize(m_char) == sizeof(char));
	GLASSERT(EiC_getsize(m_short) == sizeof(short));
	GLASSERT(EiC_getsize(m_int) == sizeof(int));
	GLASSERT(EiC_getsize(m_long) == sizeof(long));
	GLASSERT(EiC_getsize(m_float) == sizeof(float));
	GLASSERT(EiC_getsize(m_double) == sizeof(double));
	GLASSERT(EiC_getsize(m_pointer) == sizeof(void *));

	fakeEditActor = (struct stActorVars *)dlmalloc(Actor::getStructActorSize());
	invalidScriptActor = (struct stActorVars *)dlmalloc(Actor::getStructActorSize());
	eventActorVars = (struct stActorVars *)dlmalloc(Actor::getStructActorSize());
	

	memset(invalidScriptActor, 0, Actor::getStructActorSize());
	strcpy(invalidScriptActor->name, "invalid actor");
	invalidScriptActor->cloneindex = -1;

	memset(fakeEditActor, 0, Actor::getStructActorSize());

	PI = 4*atan((double)1);

	EiC_init_EiC();
	EiC_setMessageDisplay(myDisplay);

	char buf[64];
	dodefine("true=1");
	dodefine("false=0");

	EiC_parseString("unsigned long gedMaxGameMem @ %ld;", (long)&gedMaxGameMem);

	EiC_parseString("const double PI=%f;", PI);

	sprintf(buf, "BOTH_AXIS=%ld", BOTH_AXIS); dodefine(buf);
	sprintf(buf, "X_AXIS=%ld", X_AXIS); dodefine(buf);
	sprintf(buf, "Y_AXIS=%ld", Y_AXIS); dodefine(buf);
	sprintf(buf, "NONE_AXIS=%ld", NONE_AXIS); dodefine(buf);
	sprintf(buf, "FORWARD=%ld", FORWARD); dodefine(buf);
	sprintf(buf, "BACKWARD=%ld", BACKWARD); dodefine(buf);
	sprintf(buf, "STOPPED=%ld", STOPPED); dodefine(buf);
	sprintf(buf, "CUSTOM=%ld", CUSTOM); dodefine(buf);
	sprintf(buf, "NO_CHANGE=%ld", NO_CHANGE); dodefine(buf);
	sprintf(buf, "DISABLE=%ld", DISABLE); dodefine(buf);
	sprintf(buf, "ENABLE=%ld", ENABLE); dodefine(buf);
	sprintf(buf, "DONT_DRAW_ONLY=%ld", DONT_DRAW_ONLY); dodefine(buf);

	sprintf(buf, "MOVE_EVENT_ACTOR_ONLY=%ld", MOVE_EVENT_ACTOR_ONLY); dodefine(buf);
	sprintf(buf, "MOVE_COLLIDE_ACTOR_ONLY=%ld", MOVE_COLLIDE_ACTOR_ONLY); dodefine(buf);
	sprintf(buf, "MOVE_BOTH_ACTORS=%ld", MOVE_BOTH_ACTORS); dodefine(buf);
	sprintf(buf, "USE_CALCULATED_MASS=%ld", USE_CALCULATED_MASS); dodefine(buf);
	sprintf(buf, "USE_SPECIFIED_MASS=%ld", USE_SPECIFIED_MASS); dodefine(buf);

	sprintf(buf, "HIGH_PRIORITY_MUSIC=%ld", HIGH_PRIORITY_MUSIC); dodefine(buf);
	sprintf(buf, "MEDIUM_PRIORITY_MUSIC=%ld", MEDIUM_PRIORITY_MUSIC); dodefine(buf);
	sprintf(buf, "LOW_PRIORITY_MUSIC=%ld", LOW_PRIORITY_MUSIC); dodefine(buf);

	sprintf(buf, "EVENTMOUSEBUTTONDOWN=%ld", EVENTMOUSEBUTTONDOWN); dodefine(buf);
	sprintf(buf, "EVENTMOUSEBUTTONUP=%ld", EVENTMOUSEBUTTONUP); dodefine(buf);
	sprintf(buf, "EVENTMOUSEENTER=%ld", EVENTMOUSEENTER); dodefine(buf);
	sprintf(buf, "EVENTMOUSELEAVE=%ld", EVENTMOUSELEAVE); dodefine(buf);
	sprintf(buf, "EVENTMOVEFINISH=%ld", EVENTMOVEFINISH); dodefine(buf);
	sprintf(buf, "EVENTANIMATION=%ld", EVENTANIMATION); dodefine(buf);
	sprintf(buf, "EVENTANIMATIONFINISH=%ld", EVENTANIMATIONFINISH); dodefine(buf);
	sprintf(buf, "EVENTPATHFINISH=%ld", EVENTPATHFINISH); dodefine(buf);
	sprintf(buf, "EVENTKEYDOWN=%ld", EVENTKEYDOWN); dodefine(buf);
	sprintf(buf, "EVENTKEYUP=%ld", EVENTKEYUP); dodefine(buf);
	sprintf(buf, "EVENTTIMER=%ld", EVENTTIMER); dodefine(buf);
	sprintf(buf, "EVENTCOLLISION=%ld", EVENTCOLLISION); dodefine(buf);
	sprintf(buf, "EVENTCOLLISIONFINISH=%ld", EVENTCOLLISIONFINISH); dodefine(buf);
	sprintf(buf, "EVENTCREATE=%ld", EVENTCREATE); dodefine(buf);
	sprintf(buf, "EVENTDESTROYACTOR=%ld", EVENTDESTROYACTOR); dodefine(buf);
	sprintf(buf, "EVENTOUTOFVISION=%ld", EVENTOUTOFVISION); dodefine(buf);
	sprintf(buf, "EVENTACTIVATIONEVENT=%ld", EVENTACTIVATIONEVENT); dodefine(buf);
	sprintf(buf, "EVENTALL=%ld", EVENTALL); dodefine(buf);	

	sprintf(buf, "MAX_SCRIPTTEXT=%ld", MAX_SCRIPTTEXT); dodefine(buf);
	sprintf(buf, "ACTOR_NAME=%ld", ACTOR_NAME); dodefine(buf);
	sprintf(buf, "CLONE_NAME=%ld", CLONE_NAME); dodefine(buf);	
	
	//Actor struct
	EiC_parseString((char *)(baseStructActor + endStructActor).c_str());

	//Time struct
	EiC_parseString((char *)timeStruct.c_str());

	//Keyboard defination
	sprintf(buf, "KEY_BACKSPACE=%ld", SDLK_BACKSPACE); dodefine(buf);
	sprintf(buf, "KEY_TAB=%ld", SDLK_TAB); dodefine(buf);
	sprintf(buf, "KEY_CLEAR=%ld", SDLK_CLEAR); dodefine(buf);
	sprintf(buf, "KEY_RETURN=%ld", SDLK_RETURN); dodefine(buf);
	sprintf(buf, "KEY_PAUSE=%ld", SDLK_PAUSE); dodefine(buf);
	sprintf(buf, "KEY_ESCAPE=%ld", SDLK_ESCAPE); dodefine(buf);
	sprintf(buf, "KEY_SPACE=%ld", SDLK_SPACE); dodefine(buf);
	sprintf(buf, "KEY_EXCLAIM=%ld", SDLK_EXCLAIM); dodefine(buf);
	sprintf(buf, "KEY_QUOTEDBL=%ld", SDLK_QUOTEDBL); dodefine(buf);
	sprintf(buf, "KEY_HASH=%ld", SDLK_HASH); dodefine(buf);
	sprintf(buf, "KEY_DOLLAR=%ld", SDLK_DOLLAR); dodefine(buf);
	sprintf(buf, "KEY_AMPERSAND=%ld", SDLK_AMPERSAND); dodefine(buf);
	sprintf(buf, "KEY_QUOTE=%ld", SDLK_QUOTE); dodefine(buf);
	sprintf(buf, "KEY_LEFTPAREN=%ld", SDLK_LEFTPAREN); dodefine(buf);
	sprintf(buf, "KEY_RIGHTPAREN=%ld", SDLK_RIGHTPAREN); dodefine(buf);
	sprintf(buf, "KEY_ASTERISK=%ld", SDLK_ASTERISK); dodefine(buf);
	sprintf(buf, "KEY_PLUS=%ld", SDLK_PLUS); dodefine(buf);
	sprintf(buf, "KEY_COMMA=%ld", SDLK_COMMA); dodefine(buf);
	sprintf(buf, "KEY_MINUS=%ld", SDLK_MINUS); dodefine(buf);
	sprintf(buf, "KEY_PERIOD=%ld", SDLK_PERIOD); dodefine(buf);
	sprintf(buf, "KEY_SLASH=%ld", SDLK_SLASH); dodefine(buf);
	sprintf(buf, "KEY_0=%ld", SDLK_0); dodefine(buf);
	sprintf(buf, "KEY_1=%ld", SDLK_1); dodefine(buf);
	sprintf(buf, "KEY_2=%ld", SDLK_2); dodefine(buf);
	sprintf(buf, "KEY_3=%ld", SDLK_3); dodefine(buf);
	sprintf(buf, "KEY_4=%ld", SDLK_4); dodefine(buf);
	sprintf(buf, "KEY_5=%ld", SDLK_5); dodefine(buf);
	sprintf(buf, "KEY_6=%ld", SDLK_6); dodefine(buf);
	sprintf(buf, "KEY_7=%ld", SDLK_7); dodefine(buf);
	sprintf(buf, "KEY_8=%ld", SDLK_8); dodefine(buf);
	sprintf(buf, "KEY_9=%ld", SDLK_9); dodefine(buf);
	sprintf(buf, "KEY_COLON=%ld", SDLK_COLON); dodefine(buf);
	sprintf(buf, "KEY_SEMICOLON=%ld", SDLK_SEMICOLON); dodefine(buf);
	sprintf(buf, "KEY_LESS=%ld", SDLK_LESS); dodefine(buf);
	sprintf(buf, "KEY_EQUALS=%ld", SDLK_EQUALS); dodefine(buf);
	sprintf(buf, "KEY_GREATER=%ld", SDLK_GREATER); dodefine(buf);
	sprintf(buf, "KEY_QUESTION=%ld", SDLK_QUESTION); dodefine(buf);
	sprintf(buf, "KEY_AT=%ld", SDLK_AT); dodefine(buf);
	sprintf(buf, "KEY_LEFTBRACKET=%ld", SDLK_LEFTBRACKET); dodefine(buf);
	sprintf(buf, "KEY_BACKSLASH=%ld", SDLK_BACKSLASH); dodefine(buf);
	sprintf(buf, "KEY_RIGHTBRACKET=%ld", SDLK_RIGHTBRACKET); dodefine(buf);
	sprintf(buf, "KEY_CARET=%ld", SDLK_CARET); dodefine(buf);
	sprintf(buf, "KEY_UNDERSCORE=%ld", SDLK_UNDERSCORE); dodefine(buf);
	sprintf(buf, "KEY_BACKQUOTE=%ld", SDLK_BACKQUOTE); dodefine(buf);
	sprintf(buf, "KEY_a=%ld", SDLK_a); dodefine(buf);
	sprintf(buf, "KEY_b=%ld", SDLK_b); dodefine(buf);
	sprintf(buf, "KEY_c=%ld", SDLK_c); dodefine(buf);
	sprintf(buf, "KEY_d=%ld", SDLK_d); dodefine(buf);
	sprintf(buf, "KEY_e=%ld", SDLK_e); dodefine(buf);
	sprintf(buf, "KEY_f=%ld", SDLK_f); dodefine(buf);
	sprintf(buf, "KEY_g=%ld", SDLK_g); dodefine(buf);
	sprintf(buf, "KEY_h=%ld", SDLK_h); dodefine(buf);
	sprintf(buf, "KEY_i=%ld", SDLK_i); dodefine(buf);
	sprintf(buf, "KEY_j=%ld", SDLK_j); dodefine(buf);
	sprintf(buf, "KEY_k=%ld", SDLK_k); dodefine(buf);
	sprintf(buf, "KEY_l=%ld", SDLK_l); dodefine(buf);
	sprintf(buf, "KEY_m=%ld", SDLK_m); dodefine(buf);
	sprintf(buf, "KEY_n=%ld", SDLK_n); dodefine(buf);
	sprintf(buf, "KEY_o=%ld", SDLK_o); dodefine(buf);
	sprintf(buf, "KEY_p=%ld", SDLK_p); dodefine(buf);
	sprintf(buf, "KEY_q=%ld", SDLK_q); dodefine(buf);
	sprintf(buf, "KEY_r=%ld", SDLK_r); dodefine(buf);
	sprintf(buf, "KEY_s=%ld", SDLK_s); dodefine(buf);
	sprintf(buf, "KEY_t=%ld", SDLK_t); dodefine(buf);
	sprintf(buf, "KEY_u=%ld", SDLK_u); dodefine(buf);
	sprintf(buf, "KEY_v=%ld", SDLK_v); dodefine(buf);
	sprintf(buf, "KEY_w=%ld", SDLK_w); dodefine(buf);
	sprintf(buf, "KEY_x=%ld", SDLK_x); dodefine(buf);
	sprintf(buf, "KEY_y=%ld", SDLK_y); dodefine(buf);
	sprintf(buf, "KEY_z=%ld", SDLK_z); dodefine(buf);
	sprintf(buf, "KEY_PAD_0=%ld", SDLK_KP0); dodefine(buf);
	sprintf(buf, "KEY_PAD_1=%ld", SDLK_KP1); dodefine(buf);
	sprintf(buf, "KEY_PAD_2=%ld", SDLK_KP2); dodefine(buf);
	sprintf(buf, "KEY_PAD_3=%ld", SDLK_KP3); dodefine(buf);
	sprintf(buf, "KEY_PAD_4=%ld", SDLK_KP4); dodefine(buf);
	sprintf(buf, "KEY_PAD_5=%ld", SDLK_KP5); dodefine(buf);
	sprintf(buf, "KEY_PAD_6=%ld", SDLK_KP6); dodefine(buf);
	sprintf(buf, "KEY_PAD_7=%ld", SDLK_KP7); dodefine(buf);
	sprintf(buf, "KEY_PAD_8=%ld", SDLK_KP8); dodefine(buf);
	sprintf(buf, "KEY_PAD_9=%ld", SDLK_KP9); dodefine(buf);
	sprintf(buf, "KEY_PAD_PERIOD=%ld", SDLK_KP_PERIOD); dodefine(buf);
	sprintf(buf, "KEY_PAD_DIVIDE=%ld", SDLK_KP_DIVIDE); dodefine(buf);
	sprintf(buf, "KEY_PAD_MULTIPLY=%ld", SDLK_KP_MULTIPLY); dodefine(buf);
	sprintf(buf, "KEY_PAD_MINUS=%ld", SDLK_KP_MINUS); dodefine(buf);
	sprintf(buf, "KEY_PAD_PLUS=%ld", SDLK_KP_PLUS); dodefine(buf);
	sprintf(buf, "KEY_PAD_ENTER=%ld", SDLK_KP_ENTER); dodefine(buf);
	sprintf(buf, "KEY_PAD_EQUALS=%ld", SDLK_KP_EQUALS); dodefine(buf);
	sprintf(buf, "KEY_UP=%ld", SDLK_UP); dodefine(buf);
	sprintf(buf, "KEY_DOWN=%ld", SDLK_DOWN); dodefine(buf);
	sprintf(buf, "KEY_RIGHT=%ld", SDLK_RIGHT); dodefine(buf);
	sprintf(buf, "KEY_LEFT=%ld", SDLK_LEFT); dodefine(buf);
	sprintf(buf, "KEY_INSERT=%ld", SDLK_INSERT); dodefine(buf);
	sprintf(buf, "KEY_HOME=%ld", SDLK_HOME); dodefine(buf);
	sprintf(buf, "KEY_END=%ld", SDLK_END); dodefine(buf);
	sprintf(buf, "KEY_PAGEUP=%ld", SDLK_PAGEUP); dodefine(buf);
	sprintf(buf, "KEY_PAGEDOWN=%ld", SDLK_PAGEDOWN); dodefine(buf);
	sprintf(buf, "KEY_F1=%ld", SDLK_F1); dodefine(buf);
	sprintf(buf, "KEY_F2=%ld", SDLK_F2); dodefine(buf);
	sprintf(buf, "KEY_F3=%ld", SDLK_F3); dodefine(buf);
	sprintf(buf, "KEY_F4=%ld", SDLK_F4); dodefine(buf);
	sprintf(buf, "KEY_F5=%ld", SDLK_F5); dodefine(buf);
	sprintf(buf, "KEY_F6=%ld", SDLK_F6); dodefine(buf);
	sprintf(buf, "KEY_F7=%ld", SDLK_F7); dodefine(buf);
	sprintf(buf, "KEY_F8=%ld", SDLK_F8); dodefine(buf);
	sprintf(buf, "KEY_F9=%ld", SDLK_F9); dodefine(buf);
	sprintf(buf, "KEY_F10=%ld", SDLK_F10); dodefine(buf);
	sprintf(buf, "KEY_F11=%ld", SDLK_F11); dodefine(buf);
	sprintf(buf, "KEY_F12=%ld", SDLK_F12); dodefine(buf);
	sprintf(buf, "KEY_F13=%ld", SDLK_F13); dodefine(buf);
	sprintf(buf, "KEY_F14=%ld", SDLK_F14); dodefine(buf);
	sprintf(buf, "KEY_F15=%ld", SDLK_F15); dodefine(buf);
	sprintf(buf, "KEY_NUMLOCK=%ld", SDLK_NUMLOCK); dodefine(buf);
	sprintf(buf, "KEY_CAPSLOCK=%ld", SDLK_CAPSLOCK); dodefine(buf);
	sprintf(buf, "KEY_SCROLLOCK=%ld", SDLK_SCROLLOCK); dodefine(buf);
	sprintf(buf, "KEY_RSHIFT=%ld", SDLK_RSHIFT); dodefine(buf);
	sprintf(buf, "KEY_LSHIFT=%ld", SDLK_LSHIFT); dodefine(buf);
	sprintf(buf, "KEY_RCTRL=%ld", SDLK_RCTRL); dodefine(buf);
	sprintf(buf, "KEY_LCTRL=%ld", SDLK_LCTRL); dodefine(buf);
	sprintf(buf, "KEY_RALT=%ld", SDLK_RALT); dodefine(buf);
	sprintf(buf, "KEY_LALT=%ld", SDLK_LALT); dodefine(buf);
#ifndef __APPLE__ //maks:remove this when merge the SDL 1.3 changes to not break game made for multiple platforms
	sprintf(buf, "KEY_RMETA=%ld", SDLK_RMETA); dodefine(buf);
	sprintf(buf, "KEY_LMETA=%ld", SDLK_LMETA); dodefine(buf);
	sprintf(buf, "KEY_LWINDOWS=%ld", SDLK_LSUPER); dodefine(buf);
	sprintf(buf, "KEY_RWINDOWS=%ld", SDLK_RSUPER); dodefine(buf);
#endif
	sprintf(buf, "KEY_ALT_GR=%ld", SDLK_MODE); dodefine(buf);
	sprintf(buf, "KEY_HELP=%ld", SDLK_HELP); dodefine(buf);
	sprintf(buf, "KEY_PRINT=%ld", SDLK_PRINT); dodefine(buf);
	sprintf(buf, "KEY_SYSREQ=%ld", SDLK_SYSREQ); dodefine(buf);
	sprintf(buf, "KEY_BREAK=%ld", SDLK_BREAK); dodefine(buf);
	sprintf(buf, "KEY_MENU=%ld", SDLK_MENU); dodefine(buf);
	sprintf(buf, "KEY_MAC_POWER=%ld", SDLK_POWER); dodefine(buf);
	sprintf(buf, "KEY_EURO=%ld", SDLK_EURO); dodefine(buf);
#ifndef __APPLE__ //maks:remove this when merge the SDL 1.3 changes to not break game made for multiple platforms

	sprintf(buf, "KEY_POCKET_UP=%ld", SDLK_POCKET_UP); dodefine(buf);
	sprintf(buf, "KEY_POCKET_DOWN=%ld", SDLK_POCKET_DOWN); dodefine(buf);
	sprintf(buf, "KEY_POCKET_LEFT=%ld", SDLK_POCKET_LEFT); dodefine(buf);
	sprintf(buf, "KEY_POCKET_RIGHT=%ld", SDLK_POCKET_RIGHT); dodefine(buf);
	sprintf(buf, "KEY_POCKET_A=%ld", SDLK_POCKET_A); dodefine(buf);
	sprintf(buf, "KEY_POCKET_B=%ld", SDLK_POCKET_B); dodefine(buf);
	sprintf(buf, "KEY_POCKET_C=%ld", SDLK_POCKET_C); dodefine(buf);
	sprintf(buf, "KEY_POCKET_START=%ld", SDLK_POCKET_START); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX1=%ld", SDLK_POCKET_AUX1); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX2=%ld", SDLK_POCKET_AUX2); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX3=%ld", SDLK_POCKET_AUX3); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX4=%ld", SDLK_POCKET_AUX4); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX5=%ld", SDLK_POCKET_AUX5); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX6=%ld", SDLK_POCKET_AUX6); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX7=%ld", SDLK_POCKET_AUX7); dodefine(buf);
	sprintf(buf, "KEY_POCKET_AUX8=%ld", SDLK_POCKET_AUX8); dodefine(buf);

	sprintf(buf, "KEY_GP2X_BUTTON_UP=%ld", SDLK_GP2X_BUTTON_UP); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_DOWN=%ld", SDLK_GP2X_BUTTON_DOWN); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_LEFT=%ld", SDLK_GP2X_BUTTON_LEFT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_RIGHT=%ld", SDLK_GP2X_BUTTON_RIGHT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_UPLEFT=%ld", SDLK_GP2X_BUTTON_UPLEFT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_UPRIGHT=%ld", SDLK_GP2X_BUTTON_UPRIGHT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_DOWNLEFT=%ld", SDLK_GP2X_BUTTON_DOWNLEFT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_DOWNRIGHT=%ld", SDLK_GP2X_BUTTON_DOWNRIGHT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_CLICK=%ld", SDLK_GP2X_BUTTON_CLICK); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_A=%ld", SDLK_GP2X_BUTTON_A); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_B=%ld", SDLK_GP2X_BUTTON_B); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_X=%ld", SDLK_GP2X_BUTTON_X); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_Y=%ld", SDLK_GP2X_BUTTON_Y); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_L=%ld", SDLK_GP2X_BUTTON_L); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_R=%ld", SDLK_GP2X_BUTTON_R); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_START=%ld", SDLK_GP2X_BUTTON_START); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_SELECT=%ld", SDLK_GP2X_BUTTON_SELECT); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_VOLUP=%ld", SDLK_GP2X_BUTTON_VOLUP); dodefine(buf);
	sprintf(buf, "KEY_GP2X_BUTTON_VOLDOWN=%ld", SDLK_GP2X_BUTTON_VOLDOWN); dodefine(buf);
#endif
	
	//Internal functions
	EiC_add_builtinfunc("GetJoystick1Axis", eic_GetJoystick1Axis);
	EiC_parseString("int GetJoystick1Axis(int axis);");

	EiC_add_builtinfunc("GetJoystick1Button", eic_GetJoystick1Button);
	EiC_parseString("int GetJoystick1Button(int num);");
	
	EiC_add_builtinfunc("GetKeyState", eic_GetKeyState);
	EiC_parseString("char *GetKeyState(void);");

	EiC_add_builtinfunc("getLastKey", eic_getLastKey);
	EiC_parseString("int getLastKey(void);");

	EiC_add_builtinfunc("getKeyText", eic_getKeyText);
	EiC_parseString("char *getKeyText(int key);");

	EiC_add_builtinfunc("remapKey", eic_remapKey);
	EiC_parseString("void remapKey(int fromKey, int toKey);");

	EiC_add_builtinfunc("getAnimIndex", eic_getAnimIndex);
	EiC_parseString("int getAnimIndex(const char *animName);");

	EiC_add_builtinfunc("getAnimName", eic_getAnimName);
	EiC_parseString("char *getAnimName(int animIndex);");

	EiC_add_builtinfunc("ChangeAnimation", eic_ChangeAnimation);
	EiC_parseString("int ChangeAnimation(const char *actorName, const char *animationName, int state);");

	EiC_add_builtinfunc("ChangeAnimationDirection", eic_ChangeAnimationDirection);
	EiC_parseString("int ChangeAnimationDirection(const char *actorName, int state);");

	EiC_add_builtinfunc("ChangeTransparency", eic_ChangeTransparency);
	EiC_parseString("int ChangeTransparency(const char *actorName, double transp);");

	EiC_add_builtinfunc("CollisionState", eic_CollisionState);
	EiC_parseString("int CollisionState(const char *actorName, int state);");

	EiC_add_builtinfunc("VisibilityState", eic_VisibilityState);
	EiC_parseString("int VisibilityState(const char *actorName, int state);");

	EiC_add_builtinfunc("ChangePath", eic_ChangePath);
	EiC_parseString("int ChangePath(const char *actorName, const char *pathName, int axis);");

	EiC_add_builtinfunc("FollowMouse", eic_FollowMouse);
	EiC_parseString("int FollowMouse(const char *actorName, int axis);");

	EiC_add_builtinfunc("ChangeParent", eic_ChangeParent);
	EiC_parseString("int ChangeParent(const char *actorName, const char *parentName);");

	EiC_add_builtinfunc("SendActivationEvent", eic_SendActivationEvent);
	EiC_parseString("int SendActivationEvent(const char *cloneName);");

#if GAME_EDITOR_VERSION >= VERSION_NETWORK
	EiC_add_builtinfunc("ReleaseActorOwnership", eic_ReleaseActorOwnership);
	EiC_parseString("int ReleaseActorOwnership(const char *cloneName);");

	EiC_add_builtinfunc("RequestActorOwnership", eic_RequestActorOwnership);
	EiC_parseString("int RequestActorOwnership(const char *cloneName);");

	EiC_add_builtinfunc("Connect", eic_Connect);
	EiC_parseString("void Connect(const char *ipAddress, int port);");

	EiC_add_builtinfunc("NetworkStart", eic_NetworkStart);
	EiC_parseString("int NetworkStart(int port);");

	EiC_add_builtinfunc("NetworkShutdown", eic_NetworkShutdown);
	EiC_parseString("void NetworkShutdown();");
#endif

	EiC_add_builtinfunc("ChangeCursor", eic_ChangeCursor);
	EiC_parseString("int ChangeCursor(const char *actorName, const char *imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY);");

	EiC_add_builtinfunc("DestroyActor", eic_DestroyActor);
	EiC_parseString("int DestroyActor(const char *actorName);");

	EiC_add_builtinfunc("CreateTimer", eic_CreateTimer);
	EiC_parseString("int CreateTimer(const char *actorName, const char *timerName, int milliseconds);");

	EiC_add_builtinfunc("DestroyTimer", eic_DestroyTimer);
	EiC_parseString("int DestroyTimer(const char *timerName);");

	EiC_add_builtinfunc("ChangeZDepth", eic_ChangeZDepth);
	EiC_parseString("int ChangeZDepth(const char *actorName, double zdepth);");

	EiC_add_builtinfunc("PlaySound", eic_PlaySound);
	EiC_parseString("int PlaySound(const char *soundPath, double volume, int loop);");

	EiC_add_builtinfunc("PlaySound2", eic_PlaySound2);
	EiC_parseString("int PlaySound2(const char *soundPath, double volume, int loop, double pan);");

	EiC_add_builtinfunc("PlayMusic", eic_PlayMusic);
	EiC_parseString("int PlayMusic(const char *soundPath, double volume, int loop);");

	EiC_add_builtinfunc("PlayMusic2", eic_PlayMusic2);
	EiC_parseString("int PlayMusic2(const char *soundPath, double volume, int loop, int priority);");

	EiC_add_builtinfunc("setPan", eic_setPan);
	EiC_parseString("void setPan(int channel, double pan);");

	EiC_add_builtinfunc("setVolume", eic_setVolume);
	EiC_parseString("void setVolume(int channel, double volume);");

	EiC_add_builtinfunc("stopSound", eic_stopSound);
	EiC_parseString("void stopSound(int channel);");

	EiC_add_builtinfunc("ToAnteriorPosition", eic_ToAnteriorPosition);
	EiC_parseString("int ToAnteriorPosition(const char *actorName);");

	EiC_add_builtinfunc("EventEnable", eic_EventEnable);
	EiC_parseString("int EventEnable(const char *actorName, unsigned long event);");

	EiC_add_builtinfunc("EventDisable", eic_EventDisable);
	EiC_parseString("int EventDisable(const char *actorName, unsigned long event);");

	EiC_add_builtinfunc("ExitGame", eic_ExitGame);
	EiC_parseString("int ExitGame(void);");

	EiC_add_builtinfunc("LoadGame", eic_LoadGame);
	EiC_parseString("int LoadGame(const char *gamePath);");	

	EiC_add_builtinfunc("SuspendGame", eic_SuspendGame);
	EiC_parseString("void SuspendGame(void);");

	EiC_add_builtinfunc("PauseGameOn", eic_PauseGameOn);
	EiC_parseString("void PauseGameOn(void);");

	EiC_add_builtinfunc("PauseGameOff", eic_PauseGameOff);
	EiC_parseString("void PauseGameOff(void);");

	EiC_add_builtinfunc("ActorCount", eic_ActorCount);
	EiC_parseString("int ActorCount(const char *actorName);");

	EiC_add_builtinfunc("CollisionFree", eic_CollisionFree);
	EiC_parseString("int CollisionFree(const char *actorName, int x, int y);");

	EiC_add_builtinfunc("MoveTo", eic_MoveTo);
	EiC_parseString("int MoveTo(const char *actorName, double x, double y, double velocity, const char *relativeActor, const char *avoidActor);");

	EiC_add_builtinfunc("PhysicalResponse", eic_PhysicalBounce);
	EiC_parseString("int PhysicalResponse(int moveType, int massType, double massEventActor, double massCollideActor, double eventVelocityMultiplier, double collideVelocityMultiplier);");

	EiC_add_builtinfunc("openUrl", eic_openUrl);
	EiC_parseString("void openUrl(const char *url);");

	EiC_add_builtinfunc("saveVars", eic_saveVars);
	EiC_parseString("void saveVars(const char *file, const char *group);");

	EiC_add_builtinfunc("loadVars", eic_loadVars);
	EiC_parseString("void loadVars(const char *file, const char *group);");

	EiC_add_builtinfunc("getTime", eic_getTime);
	EiC_parseString("stTime getTime(void);");

	EiC_add_builtinfunc("getOwner", eic_getOwner);
	EiC_parseString("char *getOwner(void);");

	EiC_add_builtinfunc("getHardwareID", eic_getHardwareID);
	EiC_parseString("char *getHardwareID(void);");


	//Global vars
	EiC_parseString("int %s @ %ld;", S_VAR_FRAMES, (long)&frame);
	EiC_parseString("int %s @ %ld;", S_REAL_FPS, (long)&real_fps);
	EiC_parseString("int %s @ %ld;", S_VAR_XMOUSE, (long)&mousex);
	EiC_parseString("int %s @ %ld;", S_VAR_YMOUSE, (long)&mousey);
	EiC_parseString("double %s @ %ld;", S_VAR_MUSIC_VOLUME, (long)&musicVol);
	EiC_parseString("int conditionalResult @ %ld;", (long)&conditionalResult);

	//calibrateSize();

	
	InitAuxVars();
	includeCLibs();
	
	EiCp_setResetStart();	

	InitActorFunctions(); //After set reset start point
}




void Script::RemoveSymbol(const char *symbol, bool bTestMacros)
{	
	//if(!isSymbolInEic(symbol)) return;

	int macroId = -1;

	if(isSymbolInEic(symbol))
	{
		//Remove symbol
		char buf[256];
		sprintf(buf, ":rm %s", symbol);
	
		{
			//MuteEx mutex(parserStringMutEx); //Need this here?		
			EiC_parseString(buf);		
		}
	
		/*int i;
	    symentry_t *sym;
	    
	    for (i = 0; i < HSIZE; i++)
		{
			for(sym = EiC_HTAB[i]; sym; sym = sym->next) 
			{	
				if(strcmp(sym->id, symbol) == 0)
				{
					EiC_remsym(sym);
					return;
				}		
			}
		}*/

	}
	else if(bTestMacros && (macroId = EiC_ismacroid((char *)symbol)) > -1)
	{
		//Remove macro
		remmacroid(macroId);
	}
	
}

void Script::AddActor(Actor *actor, const char *actorName)
{
	if(!actorName && actor) actorName = actor->getActorName(); //<-- Ok

	if(actorName)
	{
		//MuteEx mutex(parserStringMutEx); //Need this here?
		
		void *addr;
		if(actor)
			addr = actor->getScriptVars();
		else
			addr = fakeEditActor;

		symentry_t *sym = (symentry_t *)isSymbolInEic(actorName);

		if(sym)
		{
			//Update adress
			val_t v, *old;
			v.p.sp = v.p.p = v.p.ep = addr;

			if(EiC_ENV->AR)
			{
				old = &EiC_ENV->AR[sym->val.ival].v;
				*old = v;
			}
		}
		else 
			EiC_parseString("Actor %s @ %ld;", actorName, (long)addr);

/*#ifdef DEBUG
		GLOUTPUT("%s:	addr = %X\n", actorName, addr);
#endif*/
	}
}

void Script::InvalidateActor(const char *actorName) const
{
	//Avoid memory erros

	symentry_t *sym = (symentry_t *)isSymbolInEic(actorName);
	if(sym)
	{
		//Update adress
		val_t v, *old;
		v.p.sp = v.p.p = v.p.ep = fakeEditActor;
		
		old = &EiC_ENV->AR[sym->val.ival].v;
		*old = v;
	}
}

void Script::RemoveActor(Actor *actor)
{
	if(!GameControl::Get()->getGameMode()) 
		RemoveSymbol(actor->getActorName());
}

void Script::EditOn()
{
	//Insert edit actors
	/*MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActorIterator itList(*it.Value());	
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			Actor *actor = itList.Current();
			if(actor->EditMode())
			{
				if(actor != eventActor)				 
				{
					AddActor(actor);
				}
			}			
		}
	}*/
}

void Script::EditOff()
{
	//Verify used actors
	usedActor.Clear();

	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(code.find(*it.Key()) != gedString::npos)
		{
			usedActor.PushBack(*it.Key());
		}		
	}

	if(code.find(S_COLLIDE) != gedString::npos) usedActor.PushBack(S_COLLIDE);
	if(code.find(S_CREATOR) != gedString::npos) usedActor.PushBack(S_CREATOR);
	if(code.find(S_PARENT) != gedString::npos) usedActor.PushBack(S_PARENT);

	usedActor.Resize(usedActor.Count());

	//Remove edit actors (don't remove eventActor vars)
	/*for(it.Begin(); !it.Done(); it.Next())
	{
		ListActorIterator itList(*it.Value());	
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			Actor *actor = itList.Current();
			if(actor->EditMode())
			{
				if(actor != eventActor) RemoveActor(actor);
			}			
		}
	}*/
}

const gedString& Script::GetError()
{
	return error;
}

bool Script::Parser(const gedString& str, bool bConditional, bool bOnDemandLoad, bool bEditScript)
{
	if(functionName.length() == 0) functionName = GenerateFunctionName();

	gedString sopen = "void ";
	sopen += functionName + "() {";

	RemoveSymbol(functionName.c_str());	
	code = str;	

#ifndef STAND_ALONE_GAME
	if(bEditScript)
	{
		//To avoid unexpected results for the user
		//Add the internal variables and actors locally
		sopen += "double x, y, xprevious, yprevious, xscreen, yscreen; ";
		sopen += "double r, g, b, transp, pathxpos, pathypos, animpos; ";
		sopen += "int animindex, nframes; ";
		sopen += "double angle, xvelocity, yvelocity, directional_velocity;";
		sopen += "int width, height; ";
		sopen += "double textNumber; ";
		sopen += "char *text; ";
		sopen += "const char *name, *clonename; ";
		sopen += "const long cloneindex; ";

		sopen += "double musicvol; ";
		sopen += "int frame, real_fps, xmouse, ymouse; ";

		sopen += "Actor collide, parent, creator; ";

		//User actors
		sopen += "Actor ";
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(actor->EditMode())
				{
					sopen += actor->getActorName();
					sopen += ", ";
					break; //Don't duplicate actors
				}			
			}			
		}

		if(sopen[sopen.length() - 2] == ',')
		{
			sopen.set(sopen.length() - 2, ';');
		}
	}
#endif

	if(code.find("rand") != gedString::npos)
	{
		updateFlags |= NEED_RAND;
	}

	if(code.find("xmouse") != gedString::npos || code.find("ymouse") != gedString::npos)
	{
		updateFlags |= NEED_MOUSE_POSITION;
	}
	
	if(code.find("width") != gedString::npos || code.find("height") != gedString::npos)
	{
		updateFlags |= NEED_DIMENTIONS;
	}

	if(code.find("directional_velocity") != gedString::npos)
	{
		updateFlags |= NEED_DIRECTIONAL_VELOCITY;
	}

	if(code.find("angle") != gedString::npos)
	{
		updateFlags |= NEED_ANGLE;
	}
	
	if(bOnDemandLoad /*&& GameControl::Get()->GetGameFileVersion() <= 12*/)
	{
		//Create fake actors if necessary
		for(int i = 0; i < usedActor.Count(); i++)
		{
			if(!isSymbolInEic(usedActor[i].c_str()))
			{
				AddActor(NULL, usedActor[i].c_str());
			}
		}
	}

	error.clear();

	{
		//MuteEx mutex(parserStringMutEx); //Need this here?
		
		//Use firstInst and storeFirstInst to get function address
		firstInst.inst = NULL;
		storeFirstInst = 1;
		EiC_parseStringSingle((char *)((sopen + (bConditional?"conditionalResult = ":"") + str + "\n}").c_str()));
		storeFirstInst = 0;

		if(firstInst.inst)
		{
			functionAddress = firstInst.inst;
			OptimizeCode(&firstInst);
		}
	}

	//Only reject if generate errors
	//Accept warnings
	//bParserOk = error.length() == 0;

	if(error.find("Error") == gedString::npos)
	{
		bParserOk = true;
	}
	else
	{
		bParserOk = false;
	}

	
	if(!bParserOk)
		functionAddress = NULL;
	
	if(!bOnDemandLoad)
		EditOff();
	
	return bParserOk;
}

bool Script::Parser(bool bOnDemandLoad, bool bReportErrors)
{
	//After load all actors
	
	bool bConditional = false;
	int term = code.find(";");
	if(term != gedString::npos && 
	   term == code.length() - 1 &&
	   (code.find(">") != gedString::npos ||
	   code.find("<") != gedString::npos ||
	   code.find("==") != gedString::npos))
	{
			bConditional = true;
	}

	Parser(code, bConditional, bOnDemandLoad);
	if(error.length() > 0 && bConditional)
	{
		//Try again
		Parser(code, false, bOnDemandLoad);
	}

	bool res = error.length() == 0;
	if(!bReportErrors) error.clear(); //Ignore error messages

	return res;
}

void Script::SetFunctionName(gedString name)
{
	RemoveSymbol(functionName.c_str());
	functionName = name;
}


int recursion = 0;
bool Script::Execute(Actor *eventActor, Actor *collideActor
#ifndef STAND_ALONE_GAME
					 , stAction *_currentAction
#endif
					 )
{
	if(!bParserOk) 
		return false;


	if(recursion) StoreStaticData();
	recursion++;

	if(eventActor) 
	{

#ifndef STAND_ALONE_GAME
		//For runtime error
		currentAction = _currentAction;	
		currentEventActor = eventActor;
#endif




		//Set invalid script actor: set cloneindex only for fast execution
		invalidScriptActor->cloneindex = -1;
		*invalidScriptActor->name = 0;
		*invalidScriptActor->clonename = 0;

		fakeEditActor->cloneindex = -1;
		*fakeEditActor->name = 0;
		*fakeEditActor->clonename = 0;

		//Store actual values
		int i;
		for(i = 0; i < usedActor.Count(); i++)
		{
			//Don't allow load. Solve the msrp.ged bug (see below)
			Actor *actor = GameControl::Get()->GetActor(usedActor[i], true, false, false);

			//If don't allow to load, causes a bug in view_pos_bug.ged
			//So, load if the actor are not destroyed before
			if(!actor)
			{
				//Allow load if was not destroyed before
				if(GameControl::Get()->GetActorIndex(usedActor[i].getCharBuf(), false))
				{
					actor = GameControl::Get()->GetActor(usedActor[i], true);
				}
			}

			if(actor > COLLIDE_ACTOR)
			{
				AddActor(actor); //Update actor address
				if(actor != eventActor) actor->SetActualValues(updateFlags);
			}
			else if(actor == COLLIDE_ACTOR && collideActor)
			{
				if(collideActor != eventActor) collideActor->SetActualValues(updateFlags);
			}
			else if(actor == PARENT_ACTOR)
			{
				if(eventActor->getParent() != GameControl::Get()->GetAxis() && IS_VALID_ACTOR(eventActor->getParent()))
				{
					eventActor->getParent()->SetActualValues(updateFlags);
					*iSymParent = (symentry_t *)eventActor->getParent()->getScriptVars();
				}
				else
				{
					*iSymParent = (symentry_t *)fakeEditActor;
				}			
			}
			else if(actor == CREATOR_ACTOR)
			{
				Actor *creator = eventActor->getCreator();
				if(IS_VALID_ACTOR(creator))
				{
					creator->SetActualValues(updateFlags);
					*iSymCreator = (symentry_t *)creator->getScriptVars();
				}
				else
				{
					*iSymCreator = (symentry_t *)fakeEditActor;
				}			
			}
			else
			{
				//No running actors with this name
				//Solve the bug in msrp.ged 'creating the player when there is no lives'
				//(the player was created due to reference player.x in tower -> Draw Actor event)
				AddActor(NULL, usedActor[i].getCharBuf());
			}
		}	

		eventActor->SetActualValues(updateFlags);

		//error.clear(); //Don't clear here to get runtime errors
		conditionalResult = 0;
		listCreateActors.Clear();

		//Get global vars
		frame = GameControl::Get()->getGameFrame();
		real_fps = GameControl::Get()->getRealFrameRate();
		if(GameControl::Get()->getAudioOpened())
		{
			musicVolAnt = musicVol = Mix_VolumeMusic(-1)/(double)SDL_MIX_MAXVOLUME;
		}

		//Mouse
		if((updateFlags & NEED_MOUSE_POSITION) && eventActor->getNetActor())
		{
			//Get the mouse position take in account the remote controller of the eventActor
			GameControl::Get()->GetMousePosition(mousex, mousey, eventActor->GetOwner());		
		}
		else
		{
			//Get local values to be executed in a Global Code
			mousex = GameControl::Get()->getMouseX();
			mousey = GameControl::Get()->getMouseY();
		}

		//Add event and collide
		AddActorVars(eventActor, collideActor);
	}
	
	
	//Execute
	/*try 
	{*/
	{
		//MuteEx mutex(parserStringMutEx); //Need this here?
		CallDirectExecution();
	}
	/*} 
	catch(...)
	{
		//Ignore script error (NULL references...)
		//Infinite loop
		error = "";
	}*/

	if(eventActor) 
	{
		//Adjust vars
		int i;
		for(i = 0; i < usedActor.Count(); i++)
		{
			Actor *actor = GameControl::Get()->GetActor(usedActor[i], true, false, false); //Don't allow load. Solve the msrp.ged bug (see below)

			if(actor > COLLIDE_ACTOR)
			{
				AddActor(actor); //Update actor address (might have been destroyed)
				actor->UpdateScriptChanges(updateFlags);
			}
			else if(actor == COLLIDE_ACTOR && collideActor)
			{
				collideActor->UpdateScriptChanges(updateFlags);
			}
			else if(actor == PARENT_ACTOR)
			{
				if(eventActor->getParent() != GameControl::Get()->GetAxis() && IS_VALID_ACTOR(eventActor->getParent()))
				{
					eventActor->getParent()->UpdateScriptChanges(updateFlags);
				}
			}
			else if(actor == CREATOR_ACTOR)
			{
				if(IS_VALID_ACTOR(eventActor->getCreator()))
				{
					eventActor->getCreator()->UpdateScriptChanges(updateFlags);
				}
			}
		}

		double  xvelocity = eventActor->getVelocityX(),
			yvelocity = eventActor->getVelocityY();

		//*eventActor->getScriptVars() = *eventActorVars;
		if(eventActor->getMoveTo())
		{
			//Priority to MoveTo
			eventActor->setVelocityX(xvelocity);
			eventActor->setVelocityY(yvelocity);
		}

		eventActor->UpdateScriptChanges(updateFlags);

		//Adjust created actors
		ListActorsIterator itActors(listCreateActors);	
		for(itActors.Begin(); !itActors.Done(); itActors.Next())
		{
			itActors.Current()->UpdateScriptChanges(updateFlags);
		}
		listCreateActors.Clear();

		if(musicVol != musicVolAnt && GameControl::Get()->getAudioOpened()) Mix_VolumeMusic((int)(musicVol*SDL_MIX_MAXVOLUME));
	}

	recursion--;
	if(recursion) RestoreStaticData();
	
	return true;
}

void Script::Reset()
{
	//Reset chrash when remove symbol is called at second game load
	//Crash when call New Game twice
	//After load checkers.ged twice, don't move the pieces
	//1945_4.ged without collisions

	//Enable the reset in SetGameMode when solve this issues
	return;

	//Remove Game Editor built in 
	RemoveSymbol("getAllActorsInCollision");
	RemoveSymbol("getclone");
	RemoveSymbol("getactor");
	RemoveSymbol("CreateActor");
	
	EiCp_ResetStart();
	
	//Add the Game Editor built in again to avoid crash when look for this functions
	InitActorFunctions(); 
}

gedString Script::GenerateFunctionName()
{
	static int count = 0;
	char expressionName[256];
	sprintf(expressionName, "ex%ld%ld%ld", GameControl::Get()->getGameFrame(), SDL_GetTicks(), count + GameControl::Get()->getExpressionsCount());
	count++;

	return expressionName;
}



void Script::Load(const gedString &name)
{
	//Load script by name

	//Get script location
	SDL_RWops *src = GameControl::Get()->GetGameFilePointer();
	if(!src) return;

	Uint32 index = GameControl::Get()->GetScriptIndex(name);
	if(!index) return;

	//Move file pointer
	Uint32 pos = SDL_RWtell( src ); //Save actual position
	SDL_RWseek( src, index, SEEK_SET );
	
	//Load
	Script *pScript = new Script();
	pScript->Load(src, GameControl::Get()->GetGameFileVersion());
	GameControl::Get()->AddExpression(pScript);

	//Parser

#ifdef STAND_ALONE_GAME
	bool bOnDemandLoad = true;
#else
	//Solve the export to editor differences when remove an actor in editor, 
	//and the symbol keeps in the EiC (1945 optmized (export error).ged)
	bool bOnDemandLoad = GameControl::Get()->getGameMode(); 
#endif

	if(!pScript->Parser(bOnDemandLoad))
	{
		//Parser generic scripts and try again
		GenericScript::ParserLoadedCode();
		pScript->Parser(bOnDemandLoad);
	}
	
	//Restore file pointer
	SDL_RWseek( src, pos, SEEK_SET );
}

void Script::Load(SDL_RWops *src, int version)
{
	if(version > 1)
	{	
		//Read name
		functionName.Read(src);

		//Read code
		code.Read(src);

#ifndef STAND_ALONE_GAME
		if(version < 38)
		{
			code.replaceAll("diretional_velocity", "directional_velocity");
		}
#endif

		if(version > 8)
		{
			//Load used actors
			usedActor.Clear();
			Uint32 count = GameControl::Read32( src );	
			gedString name;

			usedActor.SetCount(count);
			usedActor.Resize(usedActor.Count());
			
			for(int i = 0; i < count; i++)
			{
				ReadString(src, &name);
				usedActor.SetItem(i, name);
			}			
		}
		

#ifndef STAND_ALONE_GAME
		if(version < 49)
		{
			//Move to with relative option
			UpdateMoveTo(code);
		}
#endif
	}
	else
	{
		//Backward compatibility
		code = "";

		//Read name
		Uint16 sizeName = GameControl::Read16(src);
		char *buf = (char *)dlmalloc(sizeName);
		GameControl::Read(src, buf, sizeName, 1);
		functionName = "ex";
		functionName += buf;
		dlfree(buf);
		
		//Read expressions
		Uint16 nExpressions = GameControl::Read16(src);
		int i;
		for(i = 0; i < nExpressions; i++)
		{
			Uint16 lenght = GameControl::Read16(src);
			char *buf = (char *)dlmalloc(lenght);
			
			if(buf)
			{
				GameControl::Read(src, buf, lenght, 1);
				
				for(int i1 = 0; i1 < lenght; i1++)
				{
					if(buf[i1] == '_') buf[i1] = '.';
				}
				
				code += buf;
				code += ";\n";
				dlfree(buf);
			}
		}
		
		//Read actor vars
		Uint16 nActors = GameControl::Read16(src);
		
		for(i = 0; i < nActors; i++)
		{
			//Load name
			Uint16 lenght = GameControl::Read16(src);
			char *buf = (char *)dlmalloc(lenght);
			GameControl::Read(src, buf, lenght, 1);
			
			//Read vars
			/*Uint32 varBit =*/ GameControl::Read32(src);
			
			//mapActorVar[buf].varBit = varBit;
			dlfree(buf);
		}
		
		//Read global user vars
		Uint16 nUserVar = GameControl::Read16(src);
		
		for(i = 0; i < nUserVar; i++)
		{
			//Load name
			Uint16 lenght = GameControl::Read16(src);
			char *buf = (char *)dlmalloc(lenght);
			GameControl::Read(src, buf, lenght, 1);
			
			//listUserVar.push_back(buf);
			dlfree(buf);
		}
		
		//Read global vars
		/*Uint32 globalVars =*/ GameControl::Read32(src);		
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}

bool IsObject(int t)
{
    if(t != t_func && t !=t_funcdec && t != t_builtin) return true;
	
	return false;
}

bool IsFunction(int t)
{
    if(t == t_func || t == t_builtin) return true;
	
	return false;
}



/*
This is a fastest way to build a stack
A 4K value is good and avoid the use of conditionals and function calls
*/

int *sInt = NULL;
double *sDouble = NULL;
symentry_t **sVars = NULL;
stActorVars **sPtr = NULL;

int spInt = 0, spDouble = 0, spVars = 0, spPtr = 0, maxStack = 0;

#ifndef STAND_ALONE_GAME
int *sCurrentAction = NULL;
int spCurrentAction = 0;
int *sCurrentEventActor = NULL;
int spCurrentEventActor = 0;
#endif



void Script::StoreStaticData()
{
	//Store static data (for script in script execution)
	//stackInt.push(conditionalResult); //fail in conditional action, on restore

	/*stackInt.PushFront(frame);
	stackInt.PushFront(real_fps);
	stackInt.PushFront(mousex);
	stackInt.PushFront(mousey);

	stackDouble.PushFront(musicVol);

	ActorVarsContainer cont(eventActorVars);
	stackActorVars.PushFront(cont);
	stackActorVarsPtr.PushFront(collideActorVars);*/

	
	if(spInt >= maxStack)
	{
		maxStack += 10;

		sInt = (int *)dlrealloc(sInt, 5*maxStack*sizeof(int));		
		sDouble = (double *)dlrealloc(sDouble, maxStack*sizeof(double));
		sVars = (symentry_t **)dlrealloc(sVars, (totalVars + 2)*maxStack*sizeof(symentry_t **)); //+2 parent and creator
		sPtr = (stActorVars **)dlrealloc(sPtr, maxStack*sizeof(stActorVars *));

		

#ifndef STAND_ALONE_GAME		
		sCurrentEventActor = (int *)dlrealloc(sCurrentEventActor, maxStack*sizeof(int));	
		sCurrentAction = (int *)dlrealloc(sCurrentAction, maxStack*sizeof(int));
#endif
	}

	sInt[spInt++] = frame;
	sInt[spInt++] = real_fps;
	sInt[spInt++] = mousex;
	sInt[spInt++] = mousey;

	sDouble[spDouble++] = musicVol;

	//Copy actor vars (Avoid loop on fixed vars to speed up)
	GLASSERT(MIN_ACTOR_VARS == 26);
	SymInfo *si = symInfo;

	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	sVars[spVars++] = *(si->iSym); si++;
	

	for(int i = MIN_ACTOR_VARS; i < totalVars; i++)
	{
		sVars[spVars++] = *symInfo[i].iSym;
	}

	//Solve ParentNestedScripts.ged
	sVars[spVars++] = *iSymParent;
	sVars[spVars++] = *iSymCreator;

	sPtr[spPtr++] = collideActorVars;
	

	if(listCreateActors.Size()) 
	{
		sInt[spInt++] = 1;
		stackListActors.PushFront(listCreateActors);
	}
	else
		sInt[spInt++] = 0;

	
#ifndef STAND_ALONE_GAME	
	sCurrentEventActor[spCurrentEventActor++] = (int)currentEventActor;
	sCurrentAction[spCurrentAction++] = (int)currentAction;
#endif
}

void Script::RestoreStaticData()
{
#ifndef STAND_ALONE_GAME
	currentAction = (stAction *)sCurrentAction[--spCurrentAction];	
	currentEventActor = (Actor *)sCurrentEventActor[--spCurrentEventActor];	
#endif
	

	if(sInt[--spInt])
	{
		listCreateActors	= stackListActors.Front(); stackListActors.PopFront(); 
	}
	

	//This will restore the collide, parent and creator on nasted scripts?
	collideActorVars = sPtr[--spPtr];

	*iSymCreator = sVars[--spVars];
	*iSymParent = sVars[--spVars];
	
	
	//Copy actor vars (Avoid loop on fixed vars to speed up)
	int i;
	for(i = totalVars-1; i >= MIN_ACTOR_VARS; i--)
	{
		*symInfo[i].iSym = sVars[--spVars];
	}

	SymInfo *si = symInfo + i;

	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];
	*((si--)->iSym) = sVars[--spVars];



	musicVol = sDouble[--spDouble];

	mousey = sInt[--spInt];
	mousex = sInt[--spInt];
	real_fps = sInt[--spInt];
	frame = sInt[--spInt];

	/*collideActorVars	= stackActorVarsPtr.Front(); stackActorVarsPtr.PopFront();    
	*eventActorVars		= *stackActorVars.Front().get(); stackActorVars.PopFront();

	musicVol			= stackDouble.Front(); stackDouble.PopFront();

	mousey				= stackInt.Front(); stackInt.PopFront();
	mousex				= stackInt.Front(); stackInt.PopFront();
	real_fps			= stackInt.Front(); stackInt.PopFront();
	frame				= stackInt.Front(); stackInt.PopFront();*/

	//conditionalResult	= stackInt.Front(); stackInt.PopFront();
}


void Script::CallDirectExecution()
{	
	//55 times faster than execution with EiC_parseString("%s();", functionName.c_str());
	//and no growing memory here

	if(!functionAddress)
		return; //Some error in parser. Avoid crash.

	
	//Execute


	//Works fine
	/*code_t code;
	memset(&code, 0, sizeof(code));
	code.inst = (InsT_t *)functionAddress;
	EiC_callBack(&code);*/


	
	//More fast: works on all testes, demos and games
	//Execution time: 234 machine cycles

	/*code_t code = EiC_ENV->CODE;
	EiC_ENV->CODE.nextinst = EiC_ENV->CODE.binst = 0;
	//EiC_ENV->lsp = 0; //Don't use this: loop on forCreateFunc.ged
	EiC_ENV->CODE.inst = (InsT_t *)functionAddress;
	EiC_ENV->CODE.prev = &code;		
	
	EiC_interpret(EiC_ENV);	

	EiC_ENV->CODE.prev = NULL;  
    EiC_ENV->CODE = code;*/


	//More fast
	//Execution time: 20 machine cycles

#ifndef DEBUG //Show crash in debug mode
	GED_TRY
	{
#endif


	int nextinst = EiC_ENV->CODE.nextinst, binst = EiC_ENV->CODE.binst;
	InsT_t *inst = EiC_ENV->CODE.inst;

	EiC_ENV->CODE.nextinst = EiC_ENV->CODE.binst = 0;
	EiC_ENV->CODE.inst = (InsT_t *)functionAddress;
	
	EiC_interpret(EiC_ENV);	

	EiC_ENV->CODE.inst = inst;
	EiC_ENV->CODE.nextinst = nextinst;
	EiC_ENV->CODE.binst = binst;

#ifndef DEBUG

	}
	GED_CATCH
	{
#ifndef STAND_ALONE_GAME
		myDisplay("Crash in the code.\n   If the error persists, please contact: support@game-editor.com");
#endif
	}

#endif



/*#ifdef _DEBUG
	printf("%4d:",p); showcode(&InSt[p],p);  printf("\tSTK = (%ld, %ld), ToP = %ld", STK[ToP].v.ival, ToP>0?STK[ToP-1].v.ival:0, ToP); putchar('\n'); //maks
#endif*/
}

bool Script::IsActorVariable(const gedString &varName)
{
	if(
	   //Verify struct vars
	   varName == "x" || 
	   varName == "y" ||	   
	   varName == "xscreen" ||
	   varName == "yscreen" ||
	   varName == "r" ||
	   varName == "g" ||
	   varName == "b" ||
	   varName == "transp" ||
	   varName == "pathxpos" ||	   
	   varName == "pathypos" ||
	   varName == "animpos" ||	   
	   varName == "angle" ||
	   varName == "xvelocity" ||
	   varName == "yvelocity" ||
	   varName == "directional_velocity" ||	   
	   varName == "textNumber" ||
	   varName == "text" ||

	   varName == "xprevious" ||
	   varName == "yprevious" ||
	   varName == "width" ||	   
	   varName == "height" ||
	   varName == "animindex" ||
	   varName == "nframes" ||
	   varName == "name" ||
	   varName == "clonename" ||
	   varName == "cloneindex" ||

	   mapLocalUserVar[varName]	     
	   )
	{
		return true;
	}

	return false;
}

bool Script::IsVariableReadOnly(const gedString &varName)
{
	if(
	   varName == S_VAR_FRAMES ||
	   varName == S_REAL_FPS ||
	   varName == S_VAR_XMOUSE ||
	   varName == S_VAR_YMOUSE ||

	   varName == "xprevious" ||
	   varName == "yprevious" ||
	   varName == "width" ||	   
	   varName == "height" ||
	   varName == "animindex" ||
	   varName == "nframes" ||
	   varName == "name" ||
	   varName == "clonename" ||
	   varName == "cloneindex"  
	   )
	{
		return true;
	}

	return false;
}

bool Script::CreateLocalUserVar(const gedString &varName, int type)
{
	if(IsActorVariable(varName))
	{
		return false;
	}

	mapLocalUserVar.Add(varName, type);
	UpdateLocalUserVars();

	return true;
}

#ifndef STAND_ALONE_GAME
int getOffset(const char *varName)
{
	int *p = mapOffset.Find1(varName);
	return p?*p:-1;
}
#endif

//FILE *arqSym = NULL;
symentry_t **getSymPointer(const char *varName)
{
	//Store sym pointers to fast event actor switch
	symentry_t *sym = (symentry_t *)isSymbolInEic(varName);
	
	if(sym)
	{
		return &(EiC_ENV->AR[sym->val.ival].v.sym);	
	}

	return NULL;
}

void storeSymPointer(const char *varName, int i, int offset)
{
	//Store sym pointers to fast event actor switch
	symentry_t *sym = (symentry_t *)isSymbolInEic(varName);
	
	if(sym)
	{
		symInfo[i].iSym = &(EiC_ENV->AR[sym->val.ival].v.sym);	
		/*if(i == 0) 
			symInfo[i].offset = 0;
		else
		{
			symInfo[i].offset = (char *)(*symInfo[i].iSym) - (char *)(*symInfo[i-1].iSym) + symInfo[i-1].offset;
		}*/

		symInfo[i].offset = offset;

#ifndef STAND_ALONE_GAME
		mapOffset.Add(varName, offset);
#endif

		/*if(arqSym)
		{
			fprintf(arqSym, "%3ld %32s: %ld\n", i, varName, offset);
		}*/
	}
}


inline void Script::AddActorVars(Actor *eventActor, Actor *collideActor)
{
	//*eventActorVars = *eventActor->getScriptVars();	
	
	//Update Event Actors Vars
	//Don't works
	//Code is generated with pointers to eventActorVars
	//May be fixed making the generated code ref to double pointers

	SymInfo *si = symInfo;
	
	//Store fixed MIN_ACTOR_VARS vars (avoid loop to speed up)
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;
	*(si->iSym) = (symentry_t *)((char *)eventActor->getScriptVars() + si->offset); si++;

	
	for(int i = MIN_ACTOR_VARS; i < totalVars; i++)
	{
		*symInfo[i].iSym = (symentry_t *)((char *)eventActor->getScriptVars() + symInfo[i].offset);
	}



	//Update collide
	if(collideActor)
	{
		collideActorVars = collideActor->getScriptVars();
		AddActor(collideActor, S_COLLIDE);
	}
}

void Script::InitAuxVars()
{
	int i = 0;

	totalVars = MIN_ACTOR_VARS + mapLocalUserVar.size();
	symInfo = (SymInfo *)dlrealloc(symInfo, totalVars*sizeof(SymInfo));

#ifndef STAND_ALONE_GAME
	mapOffset.Clear();
#endif

	/*remove((GameControl::Get()->getEditorPath() + DIR_SEP + "variables.txt").c_str());
	arqSym = fopen((GameControl::Get()->getEditorPath() + DIR_SEP + "variables.txt").c_str(), "w");*/
	

	//Event Actor vars
	EiC_parseString("double x @ %ld;", (long)&eventActorVars->x); storeSymPointer("x", i++, (char *)&eventActorVars->x - (char *)eventActorVars);
	EiC_parseString("double y @ %ld;", (long)&eventActorVars->y); storeSymPointer("y", i++, (char *)&eventActorVars->y - (char *)eventActorVars);
	EiC_parseString("double xprevious @ %ld;", (long)&eventActorVars->xprevious); storeSymPointer("xprevious", i++, (char *)&eventActorVars->xprevious - (char *)eventActorVars);
	EiC_parseString("double yprevious @ %ld;", (long)&eventActorVars->yprevious); storeSymPointer("yprevious", i++, (char *)&eventActorVars->yprevious - (char *)eventActorVars); 
	EiC_parseString("double xscreen @ %ld;", (long)&eventActorVars->xscreen); storeSymPointer("xscreen", i++, (char *)&eventActorVars->xscreen - (char *)eventActorVars);
	EiC_parseString("double yscreen @ %ld;", (long)&eventActorVars->yscreen); storeSymPointer("yscreen", i++, (char *)&eventActorVars->yscreen - (char *)eventActorVars);
	EiC_parseString("double r @ %ld;", (long)&eventActorVars->r); storeSymPointer("r", i++, (char *)&eventActorVars->r - (char *)eventActorVars);
	EiC_parseString("double g @ %ld;", (long)&eventActorVars->g); storeSymPointer("g", i++, (char *)&eventActorVars->g - (char *)eventActorVars);
	EiC_parseString("double b @ %ld;", (long)&eventActorVars->b); storeSymPointer("b", i++, (char *)&eventActorVars->b - (char *)eventActorVars);
	EiC_parseString("double transp @ %ld;", (long)&eventActorVars->transp); storeSymPointer("transp", i++, (char *)&eventActorVars->transp - (char *)eventActorVars);
	EiC_parseString("double pathxpos @ %ld;", (long)&eventActorVars->pathxpos); storeSymPointer("pathxpos", i++, (char *)&eventActorVars->pathxpos - (char *)eventActorVars);
	EiC_parseString("double pathypos @ %ld;", (long)&eventActorVars->pathypos); storeSymPointer("pathypos", i++, (char *)&eventActorVars->pathypos - (char *)eventActorVars);
	EiC_parseString("double animpos @ %ld;", (long)&eventActorVars->animpos); storeSymPointer("animpos", i++, (char *)&eventActorVars->animpos - (char *)eventActorVars);
	EiC_parseString("int animindex @ %ld;", (long)&eventActorVars->animindex); storeSymPointer("animindex", i++, (char *)&eventActorVars->animindex - (char *)eventActorVars);
	EiC_parseString("int nframes @ %ld;", (long)&eventActorVars->nframes); storeSymPointer("nframes", i++, (char *)&eventActorVars->nframes - (char *)eventActorVars);
	EiC_parseString("double angle @ %ld;", (long)&eventActorVars->angle); storeSymPointer("angle", i++, (char *)&eventActorVars->angle - (char *)eventActorVars);
	EiC_parseString("double xvelocity @ %ld;", (long)&eventActorVars->xvelocity); storeSymPointer("xvelocity", i++, (char *)&eventActorVars->xvelocity - (char *)eventActorVars);
	EiC_parseString("double yvelocity @ %ld;", (long)&eventActorVars->yvelocity); storeSymPointer("yvelocity", i++, (char *)&eventActorVars->yvelocity - (char *)eventActorVars);
	EiC_parseString("double directional_velocity @ %ld;", (long)&eventActorVars->diretional_velocity); storeSymPointer("directional_velocity", i++, (char *)&eventActorVars->diretional_velocity - (char *)eventActorVars);
	EiC_parseString("int width @ %ld;", (long)&eventActorVars->width); storeSymPointer("width", i++, (char *)&eventActorVars->width - (char *)eventActorVars);
	EiC_parseString("int height @ %ld;", (long)&eventActorVars->height); storeSymPointer("height", i++, (char *)&eventActorVars->height - (char *)eventActorVars);
	EiC_parseString("double textNumber @ %ld;", (long)&eventActorVars->textNumber); storeSymPointer("textNumber", i++, (char *)&eventActorVars->textNumber - (char *)eventActorVars);

	/*EiC_parseString("char text[MAX_SCRIPTTEXT] @ %ld;", (long)&eventActorVars->text); storeSymPointer("text", i++);
	EiC_parseString("char name[ACTOR_NAME] @ %ld;", (long)&eventActorVars->name); storeSymPointer("name", i++);
	EiC_parseString("char clonename[CLONE_NAME] @ %ld;", (long)&eventActorVars->clonename); storeSymPointer("clonename", i++);*/

	//The Actor struct must use var[...] instead *var
	//Solve the jazz_e_bob\data xfer test
	EiC_parseString("char *text;"); storeSymPointer("text", i++, (char *)&eventActorVars->text - (char *)eventActorVars);
	EiC_parseString("const char *name;"); storeSymPointer("name", i++, (char *)&eventActorVars->name - (char *)eventActorVars);
	EiC_parseString("const char *clonename;"); storeSymPointer("clonename", i++, (char *)&eventActorVars->clonename - (char *)eventActorVars);

	EiC_parseString("const long cloneindex @ %ld;", (long)&eventActorVars->cloneindex); storeSymPointer("cloneindex", i++, (char *)&eventActorVars->cloneindex - (char *)eventActorVars);


	
	long offset;
	EiC_parseString("long _gedActor_offset1 @ %ld;", (long)&offset);
	EiC_parseString("Actor _ged_Actor;");
	char buf[128];
	
	//int sizeStruct = sizeof(stActorVars);
	MapUsedActorIterator it(mapLocalUserVar);
	for(it.Begin(); !it.Done(); it.Next())
	{
		gedString varDeclaration;
		int varType = *it.Value();//, varSize = 0;	

		//Calc address to fix localVarsBug.ged				 and collisionGroups.ged
		//Calc from Actor struct
		sprintf(buf, "_gedActor_offset1 = (char *)&_ged_Actor.%s - (char *)&_ged_Actor;", (*it.Key()).c_str());

		EiC_interpON = 1;
		EiC_parseString(buf);
		EiC_interpON = 0;
		
		switch(varType)
		{
		case INTEGER_VAR:
			{
				varDeclaration += "long ";
				varDeclaration += *it.Key();
				varDeclaration += " @ %ld;";

				EiC_parseString((char *)varDeclaration.c_str(), (long)(((char *)eventActorVars) + offset/*sizeStruct*/));
				//varSize = max(sizeLong, EiC_getsize(m_long)); //Fix localVarsBug.ged				
			}
			break;
		case REAL_VAR:
			{
				varDeclaration += "double ";
				varDeclaration += *it.Key();
				varDeclaration += " @ %ld;";

				EiC_parseString((char *)varDeclaration.c_str(), (long)(((char *)eventActorVars) + offset/*sizeStruct*/));
				//varSize = max(sizeDouble, EiC_getsize(m_double)); //Fix localVarsBug.ged				
			}
			break;
		case STRING_VAR:
			{
				/*varDeclaration += "char ";
				varDeclaration += *it.Key();
				varDeclaration += "[MAX_SCRIPTTEXT]  @ %ld;";*/
				varDeclaration += "char *";
				varDeclaration += *it.Key();
				varDeclaration += ";";

				EiC_parseString((char *)varDeclaration.c_str());
				//varSize = MAX_SCRIPTTEXT;
			}
			break;
		}

		
		//sizeStruct += varSize;

		storeSymPointer(it.Key()->c_str(), i++, offset);
	}

	/*fprintf(arqSym, "\n\n\n");
	fclose(arqSym);*/

	RemoveSymbol("_gedActor_offset1");
	RemoveSymbol("_ged_Actor");


	//Collide Actor: Defined in AddActor
	EiC_parseString("Actor collide @ %ld;", (long)fakeEditActor);		
	EiC_parseString("Actor parent @ %ld;", (long)fakeEditActor);
	EiC_parseString("Actor creator @ %ld;", (long)fakeEditActor);

	iSymParent = getSymPointer(S_PARENT);
	iSymCreator = getSymPointer(S_CREATOR);
}

void Script::InitActorFunctions()
{
	RemoveSymbol("getAllActorsInCollision");
	EiC_add_builtinfunc("getAllActorsInCollision",eic_getAllActorsInCollision);
	EiC_parseString("Actor *getAllActorsInCollision(const char *cloneName, int *nActors);");

	RemoveSymbol("getclone");
	EiC_add_builtinfunc("getclone",eic_getclone);
	EiC_parseString("Actor *getclone(const char *cloneName);");

	RemoveSymbol("getactor");
	EiC_add_builtinfunc("getactor",eic_getactor);
	EiC_parseString("Actor *getactor(int x, int y);");

	RemoveSymbol("CreateActor");
	EiC_add_builtinfunc("CreateActor", eic_CreateActor);
	EiC_parseString("Actor *CreateActor(const char *creatorName, const char *animationName, const char *parentName, const char *pathName, int xpos, int ypos, int absolutePosition);");
}

void Script::DestroyLocalUserVars()
{
	//Remove vars
	MapUsedActorIterator it(mapLocalUserVar);
	for(it.Begin(); !it.Done(); it.Next())
	{
		RemoveSymbol(it.Key()->c_str());
	}

	mapLocalUserVar.Clear();


	//Restore Actor struct
	error = "";
	RemoveSymbol("stActorVars");
	RemoveSymbol("Actor");
	EiC_parseString((char *)(baseStructActor + endStructActor).c_str());

	int sizeStruct = getRealActorSize();//sizeof(stActorVars);

	//Adjust actors
	MapActorIterator itActor(mapActors);
	for(itActor.Begin(); !itActor.Done(); itActor.Next())
	{
		ListActor *listActor = itActor.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(IS_VALID_ACTOR(actor))
			{
				if(isSymbolInEic(actor->getActorName()))
				{
					RemoveActor(actor);
					AddActor(actor);
				}

				actor->IncreaseActorStruct(sizeStruct);
			}			
		}
	}
	
	//Aux vars
	fakeEditActor = (struct stActorVars *)dlrealloc(fakeEditActor, Actor::getStructActorSize());
	invalidScriptActor = (struct stActorVars *)dlrealloc(invalidScriptActor, Actor::getStructActorSize());
	eventActorVars = (struct stActorVars *)dlrealloc(eventActorVars, Actor::getStructActorSize());

	InitAuxVars();
	InitActorFunctions();

	maxStack = 0;
}





void Script::LoadLocalUserVars(SDL_RWops *src, bool bMergeGames)
{
	if(!bMergeGames && mapLocalUserVar.size() > 0) DestroyLocalUserVars();

	//Load global vars
	Uint16 nVars = GameControl::Read16(src);

	for(int i = 0; i < nVars; i++)
	{
		//Load name
		Uint16 lenght = GameControl::Read16(src);
		char *buf = (char *)dlmalloc(lenght);
		GameControl::Read(src, buf, lenght, 1);

		//Read vars
		Uint8 type;
		GameControl::Read(src, &type, sizeof(type), 1);
		mapLocalUserVar.Add(buf, type);

		dlfree(buf);
	}

	if(mapLocalUserVar.size() > 0) UpdateLocalUserVars();
}

void Script::UpdateLocalUserVars()
{
	//Add user vars to Actor struct
	gedString newStructActor = baseStructActor;
	

	MapUsedActorIterator it(mapLocalUserVar);
	for(it.Begin(); !it.Done(); it.Next())
	{
		gedString varDeclaration;
		int varType = *it.Value();	
		
		switch(varType)
		{
		case INTEGER_VAR:
			{
				varDeclaration += "long ";
				varDeclaration += *it.Key();
				varDeclaration += ";";
				//sizeStruct += max(sizeLong, EiC_getsize(m_long));

				newStructActor += varDeclaration;				
			}
			break;
		case REAL_VAR:
			{
				varDeclaration += "double ";
				varDeclaration += *it.Key();
				varDeclaration += ";";
				//sizeStruct += max(sizeDouble, EiC_getsize(m_double));

				newStructActor += varDeclaration;
			}
			break;
		case STRING_VAR:
			{
				varDeclaration += "char ";
				varDeclaration += *it.Key();
				varDeclaration += "[MAX_SCRIPTTEXT];";
				//sizeStruct += MAX_SCRIPTTEXT;

				newStructActor += varDeclaration;
			}
			break;
		}
	}

	newStructActor += endStructActor;

	error = "";
	RemoveSymbol("stActorVars");
	RemoveSymbol("Actor");
	EiC_parseString((char *)newStructActor.c_str());
	
	int sizeStruct = getRealActorSize();//sizeof(stActorVars);

	//Adjust actors
	MapActorIterator itActor(mapActors);
	for(itActor.Begin(); !itActor.Done(); itActor.Next())
	{
		ListActor *listActor = itActor.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];

			if(actor)
			{
				if(isSymbolInEic(actor->getActorName()))
				{
					RemoveActor(actor);
					AddActor(actor);
				}

				actor->IncreaseActorStruct(sizeStruct);
			}			
		}
	}
	
	//Aux vars
	fakeEditActor = (struct stActorVars *)dlrealloc(fakeEditActor, Actor::getStructActorSize());
	invalidScriptActor = (struct stActorVars *)dlrealloc(invalidScriptActor, Actor::getStructActorSize());
	eventActorVars = (struct stActorVars *)dlrealloc(eventActorVars, Actor::getStructActorSize());

	InitAuxVars();
	InitActorFunctions();

	maxStack = 0;
}


#ifndef STAND_ALONE_GAME

void Script::PopulateFunctions(ListPop *list)
{
	int i;
    symentry_t *sym;
	ListString itens;

	bool bTutorialSuspendCompatible = Tutorial::IsCompatible(VERSION_GAME_SUSPEND);
	bool bTutorialGetAnimIndexCompatible = Tutorial::IsCompatible(VERSION_GET_ANIMINDEX);
	bool bShowOldFunctions = !Tutorial::IsCompatible(VERSION_GET_ANIMINDEX);
	bool bTutorialNetworkCompatible = Tutorial::IsCompatible(VERSION_NETWORK);
    
    for (i = 0; i < HSIZE; i++)
	{
		for(sym = EiC_HTAB[i]; sym; sym = sym->next) 
		{	
			if(IsFunction(EiC_gettype(sym->type)) && strncmp(sym->id, "ex", 2) != 0)
			{
				if(!bTutorialSuspendCompatible)
				{
					if( strcmp(sym->id, "SuspendGame") == 0 ||
						strcmp(sym->id, "PauseGameOn") == 0 ||
						strcmp(sym->id, "PauseGameOff") == 0 ||
						strcmp(sym->id, "getOwner") == 0 ||
						strcmp(sym->id, "getHardwareID") == 0 ||
						strcmp(sym->id, "getTime") == 0)
					{
						continue;
					}

				}

				if(!bTutorialGetAnimIndexCompatible)
				{
					if( strcmp(sym->id, "getAnimIndex") == 0 ||
						strcmp(sym->id, "getAnimName") == 0 )
					{
						continue;
					}
				}

				if(!bTutorialNetworkCompatible)
				{
					if( strcmp(sym->id, "ReleaseActorOwnership") == 0 ||
						strcmp(sym->id, "RequestActorOwnership") == 0 ||
						strcmp(sym->id, "NetworkStart") == 0 ||
						strcmp(sym->id, "NetworkShutdown") == 0 ||
						strcmp(sym->id, "Connect") == 0 ||
						strcmp(sym->id, "draw_from") == 0)
					{
						continue;
					}
				}

				if(!bShowOldFunctions)
				{
					if( strcmp(sym->id, "div") == 0 ||
						strcmp(sym->id, "getenv") == 0 ||
						strcmp(sym->id, "ldexp") == 0 ||
						strcmp(sym->id, "ldiv") == 0 ||
						strcmp(sym->id, "modf") == 0 ||
						strcmp(sym->id, "putenv") == 0 ||
						strcmp(sym->id, "rename") == 0 ||
						strcmp(sym->id, "frexp") == 0 ||
						strcmp(sym->id, "strcspn") == 0 ||			
						strcmp(sym->id, "freopen") == 0 )
					{
						continue;
					}
				}

				itens.PushBack(sym->id);
			}		
		}
	}

	itens.Sort();

	for(i = 0; i < itens.Count(); i++)
	{
		list->AddText(itens[i]);
	}	
}

void Script::PopulateVars(ListPop *list)
{
	int i;
    symentry_t *sym;
	ListString itens;
    
    for (i = 0; i < HSIZE; i++)
	{
		for(sym = EiC_HTAB[i]; sym; sym = sym->next) 
		{	
			if(sym->sclass != c_typedef &&
			   IsObject(EiC_gettype(sym->type))  && 
			   strcmp(sym->id, "rm") != 0 && 
			   strcmp(sym->id, "conditionalResult") != 0 &&
			   strcmp(sym->id, "stActorVars") != 0 &&
			   strcmp(sym->id, "Actor") != 0 &&
			   strcmp(sym->id, "st_Time") != 0 &&
			   strcmp(sym->id, "stTime") != 0 &&
			   strcmp(sym->id, S_COLLIDE) != 0 &&
			   strcmp(sym->id, S_CREATOR) != 0 &&
			   strcmp(sym->id, S_PARENT) != 0 &&
			   strcmp(sym->id, "pathxpos") != 0 &&
			   strcmp(sym->id, "pathypos") != 0 &&
			   strcmp(sym->id, "FILE") != 0 &&
			   strcmp(sym->id, "PI") != 0 &&
			   strcmp(sym->id, "size_t") != 0 &&
			   strcmp(sym->id, "gedMaxGameMem") != 0 &&			   
			   !GameControl::Get()->GetActor(sym->id))
			{
				//Accept in old tutorials				
				if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
				{
					if(strcmp(sym->id, S_VAR_FRAMES) != 0 &&
						strcmp(sym->id, S_VAR_MUSIC_VOLUME) != 0 &&
						strcmp(sym->id, S_REAL_FPS) != 0 &&
						strcmp(sym->id, S_VAR_XMOUSE) != 0 &&
						strcmp(sym->id, S_VAR_YMOUSE) != 0)
					{
						itens.PushBack(sym->id);
					}
				}
				else
				{
					itens.PushBack(sym->id);
				}
			}		
		}
	}

	itens.Sort();	

	for(i = 0; i < itens.Count(); i++)
	{
		list->AddText(itens[i]);
	}
}

void Script::PopulateActorVars(ListPop *list)
{
	list->AddText("x");
	list->AddText("y");
	list->AddText("xprevious");
	list->AddText("yprevious");
	list->AddText("xscreen");
	list->AddText("yscreen");
	list->AddText("xvelocity");
	list->AddText("yvelocity");
	list->AddText("angle");
	list->AddText("directional_velocity");
	list->AddText("r");
	list->AddText("g");
	list->AddText("b");
	list->AddText("transp");

	//Why use pathxpos and pathypos?
	if(!Tutorial::IsOff() && !Tutorial::IsCompatible(VERSION_PREFERENCES))
	{
		list->AddText("pathxpos");
		list->AddText("pathypos");
	}

	list->AddText("animpos");	
	list->AddText("animindex");
	list->AddText("nframes");	
	list->AddText("width");
	list->AddText("height");
	list->AddText("textNumber");
	list->AddText("text");
	list->AddText("name");
	list->AddText("clonename");
	list->AddText("cloneindex");

	//Add the global vars here
	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		list->AddText(S_VAR_FRAMES);
		list->AddText(S_VAR_MUSIC_VOLUME);
		list->AddText(S_REAL_FPS);
		list->AddText(S_VAR_XMOUSE);
		list->AddText(S_VAR_YMOUSE);
	}
}

void Script::PopulateLocalUserVars(ListPop *list)
{
	MapUsedActorIterator it(mapLocalUserVar);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		Uint8 type = *it.Value();
		char buf[128];

		gedString sType;

		switch(type)
		{
		case INTEGER_VAR:
			sType = "Integer";
			break;

		case REAL_VAR:
			sType = "Real";
			break;

		case STRING_VAR:
			sType = "String";
			break;
		}

		sprintf(buf, "%s (Actor, %s)", it.Key()->c_str(), sType.c_str());
		int i = list->AddText(buf);
		list->SetItemData(i, (void *)LOCAL_VAR);
	}
}

void Script::SaveLocalUserVars(SDL_RWops *src)
{
	SDL_WriteLE16(src, mapLocalUserVar.size());

	MapUsedActorIterator it(mapLocalUserVar);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		//Save name
		Uint16 lenght = it.Key()->length()+1;
		SDL_WriteLE16(src, lenght);
		SDL_RWwrite(src, it.Key()->c_str(), lenght, 1);

		//Save vars
		Uint8 type = *it.Value();
		SDL_RWwrite(src, &type, sizeof(type), 1);
	}
}

void Script::Save(SDL_RWops *src)
{
#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOBEGIN
#endif


	//Save name
	functionName.Write(src);

	if(!GameControl::Get()->getFastFile())
	{
		ChangePaths(code);
	}

	//Save script
	code.Write(src);	

	//Save used actors
	SDL_WriteLE32(src, usedActor.Count());

	for(int i = 0; i < usedActor.Count(); i++)
	{
		WriteString(src, usedActor[i]);
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOEND
#endif
}


void Script::UpdateMoveTo(gedString& code)
{
	int i, start = 0;
	gedString function("MoveTo");

	while((i = code.find(function, start)) != gedString::npos)
	{			
		//Get all arguments
		
		int iFin = code.find(';', i);
		if(iFin != gedString::npos)
		{
			gedString firstPart, lastPart(code.substr(iFin + 1));

			if(i > 0)
			{
				firstPart = code.substr(0, i);
			}

			gedString functionCall(code.substr(i, iFin - i + 1));

			if(GameControl::Get()->GetGameFileVersion() < 36)
			{
				//Relative only
				functionCall = functionCall.substr(0, functionCall.rfind(')'));
				functionCall += ", \"Game Center\", \"\");";
			}
			else if(GameControl::Get()->GetGameFileVersion() < 49)
			{
				//Relative and Avoid
				functionCall = functionCall.substr(0, functionCall.rfind(')'));
				functionCall += ", \"\");";
			}

			/*char buf[256];
			newFunctionCall = buf;
						
			gedString errorTmp = Script::error;
			
			EiC_interpON = 1;
			EiC_parseStringSingle(functionCall.getCharBuf());
			EiC_interpON = 0;
			
			Script::error = errorTmp;*/
			
			code = firstPart + functionCall + lastPart;
		}
		
		start = iFin;
	}
}

void Script::ReplacePath(gedString& code, gedString function, int nStringArg)
{
	int i, i0, start = 0;
	nStringArg *= 2; //nStringArg from 0, count first "  and final  "

	while((i = code.find(function, start)) != gedString::npos)
	{
		
		int i1 = code.find('\"', i), nStr = 0;

		//Have strings?
		i0 = code.find(')', i);
		if(i0 != gedString::npos &&  i1 > i0)
		{
			//String out side function (function(variable); function1("string"))
			//Go to next function
			start = i0;
			continue;
		}

		//Go to correct argument
		while(i1 != gedString::npos)
		{
			if(nStringArg == nStr)
			{
				//Argument to change found
				break;
			}

			nStr++;
			i1 = code.find('\"', i1 + 1);
		} 

		if(i1 != gedString::npos)
		{
			int i2 = code.find('\"', i1+1) - 1;
			
			if(i2 > i1)
			{
				gedString	firstPart(code.substr(0, i1+1)),
							lastPart(code.substr(i2 + 1)),
							path(code.substr(i1+1, i2 - i1)),
							file("data/"),
							fromPath, fileName;

				if(path.substr(0, 4) == "data")
				{
					//Relative to old game directory
					gedString gamePath(GameControl::Get()->getOldGamePath());
					
					if(gamePath.empty())
					{
						//Try from new game directory
						gamePath = GameControl::Get()->getGamePath();
					}
					
					if(!gamePath.empty())
					{
						path = gamePath + DIR_SEP + path;
					}
					else
					{
						path = gamePath + path;
					}
				}
				
				char *newName = (char *)dlmalloc(path.length() + 1);
				correctDirectoryName(path.c_str(), newName);
				
				file +=  gedString(newName).GetFileName();
				code = firstPart + file + lastPart;
				
				start = i1;

				//Copy to data directory
				SplitPath(newName, fileName, fromPath);
				dlfree(newName);

				copyfile(path.c_str(), "data", fromPath.c_str());

				if(function == "ChangeCursor")
				{
					//Get all arguments
					
					int iFin = code.find(';', i1);
					if(iFin != gedString::npos)
					{
						gedString functionCall(code.substr(i, iFin - i + 1));
						stCursor cursor;
						pCursorArgs = &cursor;


						gedString errorTmp = Script::error;

						EiC_interpON = 1;
						EiC_parseStringSingle(functionCall.getCharBuf());
						EiC_interpON = 0;

						Script::error = errorTmp;						
						
						//Add cursor to resource for game export
						KrSpriteResource *resource;
						
						if((resource = engine->Vault()->GetSpriteResource(cursor.imgName)) == NULL)
						{		
							resource = LoadTiledSprite(path, 
								cursor.imgName, 
								cursor.nFramesH, 
								cursor.nFramesV, 
								NULL, NULL, 
								cursor.hotSpotX, cursor.hotSpotY);
							
							SDL_ClearError(); //Ignore file errors
							
							if(resource)
							{
								engine->Vault()->AddResource(resource);
							}
						}
					}
				}
			}
			else
				break;
		}
		else
			break;
	}
}

ListSounds Script::GetSoundList()
{
	//Verify if this script play sounds
	ListSounds list;

	if(code.find("PlaySound") == gedString::npos &&
		code.find("PlayMusic") == gedString::npos)
		return list;

	ExtractSoundPath(code, "PlayMusic", list); //Includes PlayMusic2
	ExtractSoundPath(code, "PlaySound", list); //Includes PlaySound2

	return list;
}

void Script::ChangePaths(gedString& code)
{
	//Search for PlaySound, PlaySound2, PlayMusic and PlayMusic2
	//and change the path to data/...

	ReplacePath(code, "PlayMusic", 0);
	ReplacePath(code, "PlaySound", 0);
	ReplacePath(code, "ChangeCursor", 1);
}

static void ExtractCreateTimer(gedString code, ListString& list)
{
	int i;
	gedString function("CreateTimer");
	while((i = code.find(function)) != gedString::npos)
	{
		int i1 = code.find(',', i);
		if(i1 != gedString::npos)
		{
			int i2 = code.find('\"', i1+1);
			
			if(i2 > i1)
			{
				int i3 = code.find('\"', i2+1) - 1;
				gedString sTimer(code.substr(i2+1, i3 - i2));				
				list.PushBack(sTimer);		
			}
		}
		
		code = code.substr(i + function.length(), code.length() - i + 1);
	}
}

static void ExtractDestroyTimer(gedString code, ListString& list)
{
	int i;
	gedString function("DestroyTimer");
	while((i = code.find(function)) != gedString::npos)
	{
		int i1 = code.find('\"', i);
		if(i1 != gedString::npos)
		{
			int i2 = code.find('\"', i1+1) - 1;
			
			if(i2 > i1)
			{
				gedString sTimer(code.substr(i1+1, i2 - i1));				
				list.PushBack(sTimer);		
			}
		}
		
		code = code.substr(i + function.length(), code.length() - i + 1);
	}
}

ListString Script::GetTimerList()
{
	//Verify if this script play sounds
	ListString list;

	if(code.find("CreateTimer") == gedString::npos &&
		code.find("DestroyTimer") == gedString::npos)
		return list;

	ExtractCreateTimer(code, list);
	ExtractDestroyTimer(code, list);


	return list;
}

void Script::RemoveLocalVar(const gedString& variable)
{
	mapLocalUserVar.Remove(variable);

	gedString aux(variable);
	RemoveSymbol(aux.c_str());
	UpdateLocalUserVars();
}

ListTokens GetTokens(gedString str)
{
	ListTokens tokens;
	
	int tokenId;
	bCanInsertSymbol = 0; //Analyze but don't add any symbol

	EiC_initlex((char *)str.c_str());

	while ((tokenId = EiC_lexan()) != DONE) 
	{
		//lex_curpos don't works for strings
		char *s = EiC_LEXEM;
		int ini = lex_lastpos, fin = lex_curpos;

		if(tokenId == STR)
		{
			s = (char *)token->Val.p.p;
			ini = lex_str_start_pos;
			fin = lex_str_start_pos + strlen(s);
		}
		else if(tokenId == ID)
		{
			symentry_t *sym = (symentry_t *)isSymbolInEic(s);
			if(sym)
			{
				int t = EiC_gettype(sym->type);
				if(t == t_func || t == t_funcdec || t == t_builtin)
				{
					//Is a function
					tokenId = FUNCTION;
				}
			}
		}

		tokens.PushBack(Token(s, ini, fin, tokenId));
	}

	bCanInsertSymbol = 1;

	return tokens;
}
#endif //#ifndef STAND_ALONE_GAME






