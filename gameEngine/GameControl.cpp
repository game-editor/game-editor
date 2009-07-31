// GameControl.cpp: implementation of the GameControl class.
//
//////////////////////////////////////////////////////////////////////

#ifdef linux
#include <stdlib.h>
#endif

#include "../RakNet/Source/SocketLayer.h"
#include <stdio.h>
#include "GameControl.h"
#include "Actor.h"
#include "PathPoint.h"
#include "Path.h"
#include "Tile.h"
#include "Sequence.h"
#include "ActorEdit.h"
#include "RegionLoad.h"
#include "ActivationEventsCanvas.h"
#include "GenericScript.h"
#include "SDL_endian.h"
#include "compression.h"
#include "PathFinder/GeoPathfinder.h"



#include "../RakNet/Source/StringTable.h"
#include "../RakNet/Source/RakNetworkFactory.h"
#include "../RakNet/Source/MessageIdentifiers.h"
#include "../RakNet/Source/GetTime.h"
#include "../RakNet/Source/BitStream.h"
#include "../RakNet/Source/NetworkIDManager.h"

NetworkIDManager networkIDManager;

extern "C"
{
#include "global.h"
#include "symbol.h"
}

#define DEFAULT_GAME_PORT 21034
#define DEFAULT_CONNECTION_LOST_TIMEOUT (5*60) //5min

#if defined(WIN32) && !defined(_WIN32_WCE)
RECT rectGameWindow = {0, 0, 0, 0};
#endif

#ifndef STAND_ALONE_GAME
bool bUpdateScriptPanel = true;
void UpdatePanelScript();
#endif

//ReplicaManager replicaManager;
double rand(double t);

#ifndef GP2X
double round(double x);
#endif

void WrongNetworkGameVersion();

#ifndef STAND_ALONE_GAME
#include "../gameEditor/ActorProperty.h"
#include "../wxGameEditor/GameGraph.h"
#include "uuid.cpp"

void AddToPanelActors(Actor *_actor, bool bSendMesage);
void RemoveFromPanelActors(Actor *_actor);
void RemoveAllPanelActors();
void RemoveSelectedActor();

#endif


extern "C" int ReleaseMemory(int bytes);
extern "C" int MemoryAllocationError(int bytes);
extern "C" void _ffexit();
extern "C" void EiCp_initiateResetUser(size_t *p_ENV_bot, size_t *p_lut_bot, size_t *p_pp_bot, size_t *p_xalloc_bot); //maks
extern "C" void EiCp_ResetUser(size_t u_ENV_bot, size_t u_lut_bot, size_t u_pp_bot, size_t u_xalloc_bot); //maks
extern "C" int SDL_SurfaceLost(); //maks 

extern "C"
{
#include "global.h"
}

#ifdef _WIN32_WCE
extern "C" void GetPokcetPCScreenDimensions(int *physicalWidth, int *physicalHeight);
extern "C" int IsSmartphoneDevice();
#endif

extern double distance(double x1, double y1, double x2, double y2);

#ifdef WIN32
extern "C" int gedIsBadWritePtr(void *lp, int size)
{
	return IsBadWritePtr(lp, size);
}

extern "C" int gedIsBadReadPtr(void *lp, int size)
{
	return IsBadReadPtr(lp, size);
}

#endif

#if defined(WIN32) && !defined(_WIN32_WCE)
NormalPriority::NormalPriority(bool _bEnable)
{
#ifndef DEBUG
	bEnable = _bEnable;
	if(bEnable)
	{
		//Avoid high priorities when in activities other than render timer
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	}
#endif
}

NormalPriority::~NormalPriority()
{
#ifndef DEBUG
	if(GameControl::Get()->getGameMode() && bEnable)
	{
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	}
#endif
}
#endif

SDL_Surface *SetVideoMode(int width, int height, int bpp, Uint32 flags);

extern gedString sGameEditorCaption;

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
#	include "SecuredSections.h"
#	include <time.h>
#	include "../gameEditor/AboutDlg.h"

/*
 /-------------------------------------\
|     DATE_MAC_CONV                     |------------------------------------|
|\-------------------------------------/                                    
| Converting a date string with the format from __DATE__ to ISO standard     
|                                                                            
|                                                                            
|                                                                            
| Example:   "Sep 16 1992" -> "1992-09-16"                                   
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    strcpy(string, date_mac_conv(string) ) ;                                 
|                                                                            
| HEADER:                                                                    
|    stdio.h     : scanf, sprint                                             
|    string.h    : strcpy, strcmp                                            
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    pszDate     : String containing __DATE__ format.                        
|                                                                            
| PROTOTYPE:                                                                 
|    char _CfnTYPE *date_mac_conv(char *pszDate) ;                            
|                                                                            
| RETURN VALUE:                                                              
|    char        : ISO date format YYYY-MM-DD (length 8 char)                
|                                                                            
| MODULE:                                                                    
|    date__.c                                                                
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1992-09-16/Erik Bachmann                                                   
\---------------------------------------------------------------------------|*/

char *date_mac_conv(char *pszDate)
{
   char  *paMonth[12] = {"Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug",
                        "Sep", "Oct", "Nov", "Dec"} ;
                                       /* Names of months */

   static char  szString[12] ;                /* String fore convertion */
   char  cMonth[4],                    /* Month */
         cDay[3],                      /* Day */
         cYear[5] ;                    /* Year */
   int   iCounter  ;                   /* Local counter */

   /*-----------------------------------------------------------*/
   
   strcpy(szString, pszDate) ;            /* Get string to convert */
   
   sscanf(szString, "%s %s %s", cMonth, cDay, cYear) ;
                                       /* Split string */
   
   for ( iCounter = 11  ; iCounter >= 0 ; iCounter--)
                                       /* loop for testing month */
   {
      if (strcmp(cMonth, paMonth[iCounter]) == 0)
                                       /* - IF month is found */
      {
         sprintf(cMonth, "%0.2i", iCounter + 1 ) ;
                                       /* - - Insert counter + 1 as month */
         break ;
      }
   }

   /* Check valid day */
   iCounter = atoi( cDay ) ;
   if ( ( 1 > iCounter ) || ( 31 < iCounter ) )
      strcpy( cDay, "01" ) ;

   sprintf(szString, "%04.4s-%02.2s-%02.2s", cYear,cMonth,cDay) ;
                                       /* Merge ISO date */

   return( (char *) szString ) ;
}  /*** date_mac_conv() ***/


//////////////////////////////////////////////////
#ifdef USE_ACTIVATION

#include "eSellerate.h"
#pragma comment(lib,"esellerateLibrary.lib")

#define PUBLISHER_ID		""
#define ACTIVATION_SET_ID	""


static bool eSellerValidate() 
{
	eSellerate_ErrorCode result = eSellerate_FAILURE;
	char serialNumber[256];

	/*{

		char s[128];
		sprintf(s, "\n\n\n1 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "w");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		
		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR); 
	}*/
	
	//strcpy(serialNumber, "8684-5CD9-4329-7F74-A9AA-47EE-4C7B-224B-79B8-11AA-CFBC-ABB0-C14C-825F-E250");
	if(!GetEnvironmentVariable("USERKEY",  serialNumber, 255))
		return false;


	/*{
		char s[128];
		sprintf(s, "1.1 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/

	result = eSellerate_InstallEngine();

	if(result != eSellerate_SUCCESS)
	{
		PanelQuestion *panel = new PanelQuestion("Bad installation.\nPlease, reinstall.", "Error", "Close", NO_BUTTON, ALIGN_LEFT);
		panel->Wait();
		delete panel;

		GameControl::Get()->setExecuteUpToFrame(30);
		return false;
	}

	/*{
		char s[128];
		sprintf(s, "2 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/
	
	result = eSellerate_ValidateActivation (
    PUBLISHER_ID, // Publisher ID
    ACTIVATION_SET_ID, // Activation Set ID used at activation
    serialNumber    // serial number that was activated
	);

	/*{
		char s[128];
		sprintf(s, "3 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/
	
	if(result != eSellerate_SUCCESS)
	{
		WaitCursor wait;
		PanelInfo *pInfo = new PanelInfo("Checking activation...", "Info", ALIGN_CENTER, false);
		engine->Draw();
		
		/*{
		char s[128];
		sprintf(s, "4 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/
		
		result = eSellerate_ActivateSerialNumber (
			PUBLISHER_ID, // Publisher ID
			ACTIVATION_SET_ID, // Activation Set ID used at activation
			serialNumber,   // serial number that was activated
			eSellerate_TRUE // ask to connect, if need be
			);

		/*{
		char s[128];
		sprintf(s, "5 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/
		
		delete pInfo;
	}

	/*{
		char s[128];
		sprintf(s, "6 (%ld)\n", result);

		FILE *arq = fopen("out.txt", "a+");
		if(arq)
		{
			fprintf(arq, s);
			fclose(arq);
		}

		MessageBox(NULL, s, "Error", MB_OK | MB_ICONERROR);
	}*/

	//No more checks
	GameControl::Get()->setCheckFrame(103680000);
	
	if ( result != eSellerate_SUCCESS )
	{
		gedString error, 
			title("Activation Error"),
			info("\nIf you receive this error and think is incorrect,\ncontact support@game-editor.com for more information.");
		
		if(result == -25000)
		{
			error = "ID unknown.\n";
		}
		else if(result == -25001)
		{
			error = "Key Not Found.\n\nIf you placed your order in the past few minutes,\nthe servers may still be processing it.\nPlease wait about 15 minutes and try activating your serial number again.\nIf you receive this error at that time, contact support@game-editor.com for more information.";
		}
		else if(result == -25003)
		{
			error = "Invalid Key.\n";
			error += info;
		}
		else if(result == -25004)
		{
			error = "Returned product.\n";
			error += info;
		}
		else if(result == -25005)
		{
			error = "Activation limit reached.\n\nGame Editor can be installed on only\none computer with the same key.";
			error += info;
		}
		else if(result == -25009)
		{
			error = "Key already associated with another order.\n";
			error += info;
		}
		else if(result == eSellerate_NOENGINE)
		{
			error = "Bad installation.\nPlease, reinstall Game Editor";
			title = "Startup Error";
		}
		else
		{
			error = "Game Editor need to be activated.\n\nPlease, connect and try execute again.";
			error += info;
		}
		
		PanelQuestion *panel = new PanelQuestion(error, title, "Close", NO_BUTTON, ALIGN_LEFT);
		panel->Wait();
		delete panel;		
		
		GameControl::Get()->setExecuteUpToFrame(30);
		return false;
	}
	
	return true;  
}

#endif //USE_ACTIVATION
//////////////////////////////////////////////////

void Inf2()
{
	//////////////////////////////////////////////////////
	//Disable expiration verification
	//Comment this block in release version
	//Use for tests only
	/*GameControl::Get()->setExecuteUpToFrame(103680000);
	GameControl::Get()->setCheckFrame(103680000);
	return;/**/
	//////////////////////////////////////////////////////


	//Check if can run this version
	NANOBEGIN

	char buf[256], keyCreated[20];

	/*strcpy(buf, "0"); if(1)*/if(GetEnvironmentVariable("EXTRAINFO",  buf, 255))
	{
		long purchasedDays = atol(buf);

		if(purchasedDays == 0)
		{
			//Life time
			GameControl::Get()->setExecuteUpToFrame(103680000);

			#ifdef USE_ACTIVATION
			eSellerValidate();
			#endif
		}
		else
		{
			//Calc the remaining update days
			if(GetEnvironmentVariable("KEYCREATED",  buf, 255))
			{
				//KEYCREATED = YYYY.MM.DD
				strcpy(keyCreated, buf);

				struct tm tmKeyCreated, tmCompilationTime;
				time_t tCompilationTime, tKeyCreated;

				memset(&tmCompilationTime, 0, sizeof(struct tm));
				memset(&tmKeyCreated, 0, sizeof(struct tm));

				//Get key creation date
				sscanf(buf, "%4ld.%2ld.%2ld", &tmKeyCreated.tm_year, &tmKeyCreated.tm_mon, &tmKeyCreated.tm_mday);

				/////////////////////////////////////////
				//maks:teste Keep this until release the new interface
				if(purchasedDays >= 30 && tmKeyCreated.tm_year >= 2006)
				{
					//New interface promotion
					//Can execute
					GameControl::Get()->setExecuteUpToFrame(103680000);

					#ifdef USE_ACTIVATION
					eSellerValidate();
					#endif
				} else {
				/////////////////////////////////////////

				tmKeyCreated.tm_year -= 1900;	//tm_year = year - 1900
				tmKeyCreated.tm_mon -= 1;		//January = 0

				tKeyCreated = mktime(&tmKeyCreated);

				//Get compilation time
				sscanf(date_mac_conv(__DATE__), "%4ld-%2ld-%2ld", &tmCompilationTime.tm_year, &tmCompilationTime.tm_mon, &tmCompilationTime.tm_mday);

				tmCompilationTime.tm_year -= 1900;	//tm_year = year - 1900
				tmCompilationTime.tm_mon -= 1;		//January = 0

				tCompilationTime = mktime(&tmCompilationTime);
				 

				//Calc diff
				double seconds = difftime(tCompilationTime, tKeyCreated);

				if((long)((seconds/3600.0)/24.0) <= purchasedDays)
				{
					//Can execute
					GameControl::Get()->setExecuteUpToFrame(103680000);

					#ifdef USE_ACTIVATION
					eSellerValidate();
					#endif
				}
				else
				{
					//Show register panel
					GameControl::Get()->setExecuteUpToFrame(72000); //10min

					new RegisterPanel(keyCreated, purchasedDays);
				}

				} //maks:teste Keep this until release the new interface
			}
			else
			{
				//Stripped version
				GameControl::Get()->setExecuteUpToFrame(72000); //10min
			}
		}
	}
	else
	{
		//Stripped version
		GameControl::Get()->setExecuteUpToFrame(72000); //10min
	}

	//No more checks
	GameControl::Get()->setCheckFrame(103680000);

	NANOEND
}


#endif


#ifdef WIN32
#	include "../gameEditor/ResourceEditor/ResourceEditor.h"
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
#	include "../gameEditor/pocketpc_resource.h"
#endif

#if defined(WIN32)
extern "C" HWND SDL_Window;
extern "C" HINSTANCE SDL_Instance;
#endif

//extern "C"int DX5_WaitForVerticalBlank();

//#define DEBUG_FPS 10
#ifdef DEBUG_FPS
int fpsTotal = 0, fpsCount = 0;
#endif

#ifdef _WIN32_WCE
#	include "../SDL/SDL/src/video/gapi/modGX.h"
#endif


#ifndef STAND_ALONE_GAME
#include "ActorTip.h"
#include "../gameEditor/MainPanel.h"
#include "../gameEditor/ActorText.h"
#include "../gameEditor/Config.h"
#include "../gameEditor/Tutorial.h"
#include "../gameEditor/LoadSaveGame.h"
#include "../gameEditor/UndoControl.h"
#endif //#ifndef STAND_ALONE_GAME

#include "dlmalloc.h"

///////////////////////////////////////////////////////////////////
//Fingerprint

#if !defined(STAND_ALONE_GAME)
gedString pro_fingerprint("2102/1974");

#if defined(WIN32)
gedString GetMachineFingerprint()
{
	gedString fingerprint(pro_fingerprint);

	char buf[32];
	if(GetEnvironmentVariable("FINGERPRINT",  buf, 31))
	{
		fingerprint = buf;
	}

	return fingerprint;
}
#endif

#endif

#if !defined(STAND_ALONE_GAME)

void GameControl::SaveFingerPrint(SDL_RWops *src)
{
	gedString fingerprint, save;

#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(_DEBUG)
	char buf[32];
	if(GetEnvironmentVariable("FINGERPRINT",  buf, 31))
	{
		fingerprint = buf;
	}
#else
	//Professional or Linux
	fingerprint = pro_fingerprint;
#endif
	
	//Make save buf
	GlRandom random(SDL_GetTicks()); 
	char savebuf[64];
		
	//24 chars (8 + 4 + 9 + 3)
	sprintf(savebuf, "%08X    %s   ", random.Rand(), fingerprint.c_str());
		
	save = savebuf;
	save.Write(src);
}
#endif

gedString GameControl::LoadFingerPrint(SDL_RWops *src)
{
	gedString fingerprint, saved;

	//Load size
	saved.Read(src);

#if !defined(STAND_ALONE_GAME) && defined(WIN32)
	
	//Ignore rand data
	if(saved.length() > 21)
	{
		saved.getCharBuf()[21] = 0;
		fingerprint = (saved.getCharBuf()+12);
	}

#endif
	
	return fingerprint;
}

#ifdef DEBUG
bool bCanRemoveFingerPrint = false;
void GameControl::RemoveFingerPrint(SDL_RWops *src)
{
#ifndef STAND_ALONE_GAME
	if(bCanRemoveFingerPrint)
	{
		Uint32 pos = SDL_RWtell(src);
		
		SaveFingerPrint(src);
		
		SDL_RWseek( src, pos, SEEK_SET );
	}
#endif
}
#endif
///////////////////////////////////////////////////////////////////

extern SDL_mutex *timerMutEx;
extern "C" int EiC_interpON;



bool TutorialIndentationCompatible()
{
#ifndef STAND_ALONE_GAME
	return Tutorial::IsCompatible(VERSION_IDENTATION);
#else
	return false;
#endif
}


extern "C" int GetGameMode()
{
	if(GameControl::Get())
	{
		return GameControl::Get()->getGameMode();
	}
	
	return 0;
}

extern "C" int ShowFriendlyMessages()
{
#ifndef STAND_ALONE_GAME
	if(GameControl::Get())
	{
		return GameControl::Get()->getGameMode();
	}
#endif
	
	return 0;
}

extern "C" int GetStandAloneMode()
{
	if(GameControl::Get())
	{
		return GameControl::Get()->getStandAloneMode();
	}
	
	return 0;
}

extern "C" const char *GetGameTitle()
{
	if(GameControl::Get() && GameControl::Get()->getGameTitle().getCharBuf())
	{
		return GameControl::Get()->getGameTitle().getCharBuf();
	}

	return "Game Paused";
}

int GetFlipPocketPCScreen()
{
	if(GameControl::Get() && GameControl::Get()->getFlipPocketPCScreen())
	{
		return 1;
	}

	return 0;
}

bool IsTileActor(KrImNode *node)
{
	if(node && node->getParentActor())
	{
		return ((Actor *)node->getParentActor())->getTile() != NULL;
	}

	return false;
}

//Marketing functions
#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME)

/*char *couponCode[] = 
{
"GE-SPECIAL-OFFER-KACJBF0", //30%
"GE-SPECIAL-OFFER-EAF34GD", //28%
"GE-SPECIAL-OFFER-NIURKEW", //26%
"GE-SPECIAL-OFFER-34OJFS9", //24%
"GE-SPECIAL-OFFER-JF34MVO", //22%
"GE-SPECIAL-OFFER-2L385JV", //20%
"GE-SPECIAL-OFFER-93KLFMV", //18%
"GE-SPECIAL-OFFER-LDKFS63", //16%
"GE-SPECIAL-OFFER-CKCJDIA", //14%
"GE-SPECIAL-OFFER-AJS3DNC", //12%
"GE-SPECIAL-OFFER-SJS3HDN", //10%
"GE-SPECIAL-OFFER-KCDASOF", //8%
"GE-SPECIAL-OFFER-KAIERW3", //6%
"GE-SPECIAL-OFFER-KDKFJSO", //4%
"GE-SPECIAL-OFFER-YEJGLSD"  //2%
};*/

void openBuySite()
{
	//Don't fix buy site here to a better management on with Armadillo
	char site[256], daysInstalled[64];
	if(GetEnvironmentVariable("WEBSITEURL",  site, 255))
	{
		gedString s(site);

		if(s == "http://game-editor.com/orders.html" && GetEnvironmentVariable("DAYSINSTALLED",  daysInstalled, 255))
		{
			//Add coupon code

			/*int i = atol(daysInstalled);
			if(i < 0) i = 0;
			
			if(i >= 0 && i < 15)
			{
				s = "http://game-editor.com/orders-";
				s += couponCode[i];
				s += ".html";
				s.lower();
			}*/
			
			//Append the trial day
			s += "?day=";
			s += daysInstalled;			
		}

		openUrl(s.getCharBuf());
	}
}
#endif

#define EDITOR_FRAME_RATE 120 //Use >= 60 to avoid flick in editor mode

gedString EDITOR_DAT("editor.dat");

gedString AXIS_NAME("Axis");
gedString REGION_NAME("gedRegion");
gedString ACTIVATION_EVENTS_CANVAS_NAME("gedActivationEventsCanvas");
gedString ACTOR_TIP_NAME("gedActorTip");
gedString ACTOR_GED_ICON("icon");
gedString ACTOR_GED_TEXT("text");
gedString AXIS_GRID_CANVAS_NAME("gedAxisGridCanvas");
gedString AXIS_PATH_CANVAS_NAME("gedAxisPAthCanvas");

//"Made with Game Editor\nhttp://game-editor.com"
const long geInfo[] = {389161L, 469027L, 460990L, 460676L, 223969L, 396652L, 466134L, 447150L, 469330L, 245648L, 374345L, 511473L, 473841L, 500155L, 1769L, 410091L, 289715L, 306854L, 328411L, 304444L, 347783L, 186636L, 266046L, 381081L, 385316L, 372830L, 91149L, 10524L, 7430L, 298423L, 333127L, 378078L, 358835L, 126226L, 333645L, 335304L, 390211L, 276155L, 357003L, 308902L, 81951L, 363779L, 342618L, 329341L, 0L};

#ifdef _DEBUG
void createCipherText(gedString plainText)
{
	GLOUTPUT("const long geInfo[] = {");
	for(int i = 0; i < plainText.length(); i++)
	{
		long c = rand();
		c = (c & 0xFFF00FFF) | ((plainText[i] ^ (0x12 + i)) << 12);
		GLOUTPUT("%ldL, ", c);
	}
	GLOUTPUT("0L};");
}
#endif

#if !defined(STAND_ALONE_GAME) && defined(__linux__)
static bool	bCanResize = true;
#endif

gedString decodeCipherText(const long *cipherText)
{
	gedString plainText;
	int i = 0;

	while(cipherText[i])
	{
		char c = (cipherText[i] >> 12) ^ (0x12 + i);

		plainText += c;
		i++;
	}

	return plainText;
}

/*Uint32 GameControl::RealFrameRateCallback(Uint32 interval, void *param)
{
	//Don't use, more acurated after draw event
	//Calc real frame rate
	static unsigned long lastGameFrame = 0;

	if(gameFrame > lastGameFrame) realFrameRate = gameFrame - lastGameFrame;
	lastGameFrame = gameFrame;

	return 1000;
}*/

#ifdef APPLICATION_THREAD_TIMERS
volatile bool bCallGameTick = false;
int GameControl::TimerCallback( void *pParam )
{
	//Need this separeted thread to avoid frame flood in slow machines
	//Use only CanDraw in the main loop don't works (can't get the user input on slow machines)

#if defined(WIN32) && !defined(_WIN32_WCE) && !defined(DEBUG)
	//More smooth render (look fluidez.ged)
	//Don't play the music in the Pocket PC
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#endif
	
	while(1)
	{
		if(bCanAddFrameRateEvent)
		{
			/*SDL_Event event;
			event.type = SDL_TIMER_EVENT;
			
			//Slow
			SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );*/

			bCallGameTick = true;						
		}
		/*#ifdef DEBUG
		else if(GameControl::Get()->getGameMode()) 
		{
			static long nDrops = 0;
			GLOUTPUT("*** Frame lost (bCanAddFrameRateEvent, %ld)\n", ++nDrops);
		}
		#endif*/


		/*#ifdef DEBUG
		{
		static U32 tick = 0;
		U32 currentTick = SDL_GetTicks();
		U32 realFrameRate = 1000.0/(currentTick - tick);

		if(GameControl::Get()->getGameMode()) GLOUTPUT("*** TimerCallback interval: %ld ms, fps: %ld\n", currentTick - tick, realFrameRate);

		tick = currentTick;
		}
		#endif*/
		
		SDL_Delay(GameControl::Get()->getFrameTimeInterval());
	}	
	
	return 0;
}
#else
Uint32 GameControl::TimerCallback(Uint32 interval, void *param)
{
	if(bCanAddFrameRateEvent)
	{
		SDL_Event event;
		event.type = SDL_TIMER_EVENT;
		
		SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );		
		//bCanAddFrameRateEvent = false; //Don't put here: fail on Mouse trace tutorial
	}
	
	return GameControl::Get()->getFrameTimeInterval();
}
#endif

#if defined(WIN32) && !defined(_WIN32_WCE)
//Don't use separated threads on Pocket PC (More 2 screens copy!)
//Don't use on Linux
//#define RENDER_THREAD //Don't use now (very slow and show ghost images)
#endif

SDL_mutex *queuePathFinderMutEx = SDL_CreateMutex();

#ifdef RENDER_THREAD
SDL_mutex *renderMutEx = SDL_CreateMutex();
extern "C"int DX5_WaitForVerticalBlank();

int RenderThread( void *pParam )
{
	while(GameControl::getGameEngineIsRunning())
	{
		if(engine && engine->Tree()) 
		{
			MuteEx mutex(renderMutEx);
			
			if((engine->Surface()->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN)
			{
				//Vsync done by flip function
				SDL_FlipSimple(engine->Surface());
			}
			else
			{
				DX5_WaitForVerticalBlank();
				//SDL_UpdateRects( engine->Surface(), rectArray->Count(), sdlrect );
				SDL_UpdateRectSimple(engine->Surface());
			}
		}
	}

    return 0;
}
#endif

GameControl *GameControl::gameControl = NULL;
unsigned long GameControl::gameFrame = 0;
int GameControl::realFrameRate = 30;
bool GameControl::bGameEngineIsRunning = false;
gedString GameControl::editorPath;
Uint32 GameControl::lastGameFileVersion = 0;


//bStartGameRender = true in StartGameRender to not flood the event queue
//solve the maximize bug
bool GameControl::bCanAddFrameRateEvent = false;
Uint32 GameControl::frameTimeInterval = 30;
Uint32 GameControl::realFrameTimeInterval = 30;
Uint32 GameControl::lastTick = 0;

gedString GameControl::GenerateUniqueID()
{
#ifndef STAND_ALONE_GAME
	TUUID uuid;		
	return uuid.AsString();
#else
	char id[32];
	unsigned long n = RakNet::GetTime() + SDL_GetTicks() % 1000;

	sprintf(id, "%ld", n);

	return id;
#endif
}

#ifdef USE_LLVM
void testClang(); //maks:teste
#endif

GameControl::GameControl()
{
	//createCipherText("Made with Game Editor\nhttp://game-editor.com");
	//decodeCipherText(geInfo);	

#if defined(DEBUG) && defined(USE_LLVM)
	testClang(); //maks:teste
#endif
			
	gameControl = this;

	bGameEngineIsRunning = true;
	bExportMode = false;
	bAudioOpened = false;
	bFastFile = false;

	bPauseGame = bSuspendGame = false;
	bCacheClean = false;
	
	audioSamplerRate = 0;
	bDestroingGame = false;
	sdlAudioIndex = NULL;
	indexedGame = NULL;
	rakPeer = NULL;
	networkIDManager.SetIsNetworkIDAuthority(true);
	bWaitingForConnectionComplete = false;

	bGameMode = false;
	bLoadGameUsedInGameMode = false;
	bPathMode = false;
	bStandAloneMode = false;
	bActorSnap = false;
	bFlipPocketPCScreen = false;
	bUseMotionCorrection = false;
	bSaveCache = false;
	gameVersion = 0;

	bUseGameEditorServer = true;
	bAutoStartNetwork = true;
	bRestartNetworkAfterPause = false;
	gameNetworkPort = DEFAULT_GAME_PORT;
	gameEditorServer = UNASSIGNED_SYSTEM_ADDRESS;
	connectionLostTimeOut = DEFAULT_CONNECTION_LOST_TIMEOUT;
	pendingActions.setZeroMemory(true);
	pendingActions.setAutoExpand(true);

#ifndef STAND_ALONE_GAME
	gameID = GenerateUniqueID();
#endif

	bInitialCheck = false;

	lastTick = 0;
	gameFrame = 0;
	axis = NULL;
	music = NULL;

	viewActor = NULL;

	

#ifdef APPLICATION_THREAD_TIMERS
	timerThread = NULL;
#else
	timerId = 0;
#endif

	mouseX = mouseY = 0;
	bMouseButtonDown = false;
	bModified = false;
	bCheckOutOfVision = false;
	tipTime = 0;

#if !defined(GAME_EDITOR_PROFESSIONAL) && defined(STAND_ALONE_GAME)
	gameEditorInformation = NULL;
#endif


	//Verify screen supported resolutions
	resX = 640;
	resY = 480;

	
#ifdef _WIN32_WCE
	//Min resolution on PocketPC
	int w, h;
	GetPokcetPCScreenDimensions(&w, &h);	
	resX = w;
	resY = h;

	/*HINSTANCE gxHandle = LoadLibrary(TEXT("GX.DLL"));
	if(gxHandle)
	{		
		FUNC4 GXGetDisplayProperties	= (FUNC4)GetProcAddress(gxHandle, TEXT("?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ"));
		if(GXGetDisplayProperties)
		{
			GXDisplayProperties m_dispProps = GXGetDisplayProperties();
			resX = m_dispProps.cxWidth;
			resY = m_dispProps.cyHeight;
		}		
		
		FreeLibrary(gxHandle);
	}*/
#elif defined(__SYMBIAN32__)
	SDL_Rect **modes;
	const SDL_VideoInfo *vinfo = SDL_GetVideoInfo();

	if(!SDL_VideoModeOK(resX, resY, 0/*vinfo->vfmt->BitsPerPixel*/, VIDEO_FLAGS))
	{
		/* Get available fullscreen/hardware modes */
		modes=SDL_ListModes(vinfo->vfmt, VIDEO_FLAGS | SDL_FULLSCREEN);
		
		if(modes != (SDL_Rect **)0 && modes != (SDL_Rect **)-1)
		{		
			for(int i=0;modes[i];++i);

			resX = modes[i-1]->w;
			resY = modes[i-1]->h;
		}
	}
#elif defined(GP2X)
	resX = 320;
	resY = 240;	
#endif

	backgroundColor.all = 0;

	SetGameProperties(resX, resY, 30, false, 44100, true, 8, true, "", "", true, backgroundColor, true, 20, true); //fps = 30: default frame rate
	//SetTimer(EDITOR_FRAME_RATE); //Set editor frame rate

	//Set editor directory
	char buf[PATH_LIMIT];
	editorPath = ged_getcwd(buf, PATH_LIMIT);	

	//Main loop control
	actorButtonDown = currentActor = actorDrag = actorModal = NULL;
	currentTile = NULL;
	pathPoint = NULL;
	mainActor = NULL;
	itemSelect = 0;
	startKyraSplash = 0;

	InitPocketPCKeys();

	//timerRealFpsCalcId = SDL_AddTimer(1000, RealFrameRateCallback, NULL);

#ifdef RENDER_THREAD
	SDL_CreateThread(RenderThread, NULL);
#endif

	SDL_CreateThread(PathFinderThread, NULL);
	

	//Default actors
	mapRunningActor.Add(S_EVENT_ACTOR, EVENT_ACTOR);
	mapRunningActor.Add(S_COLLIDE_ACTOR, COLLIDE_ACTOR);
	mapRunningActor.Add(S_CREATOR_ACTOR, CREATOR_ACTOR);
	mapRunningActor.Add(S_PARENT_ACTOR, PARENT_ACTOR);	
	mapRunningActor.Add(AXIS_NAME, NULL);

	mapRunningActor.Add(S_COLLIDE, COLLIDE_ACTOR);
	mapRunningActor.Add(S_CREATOR, CREATOR_ACTOR);
	mapRunningActor.Add(S_PARENT, PARENT_ACTOR);

	mapEditActor.Add(S_EVENT_ACTOR, EVENT_ACTOR);
	mapEditActor.Add(S_COLLIDE_ACTOR, COLLIDE_ACTOR);
	mapEditActor.Add(S_CREATOR_ACTOR, CREATOR_ACTOR);
	mapEditActor.Add(S_PARENT_ACTOR, PARENT_ACTOR);	
	mapEditActor.Add(AXIS_NAME, NULL);

	mapEditActor.Add(S_COLLIDE, COLLIDE_ACTOR);
	mapEditActor.Add(S_CREATOR, CREATOR_ACTOR);
	mapEditActor.Add(S_PARENT, PARENT_ACTOR);

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	totalFrames = 0;

	//Update days check
	GlRandom random(SDL_GetTicks()); 

	//Up to 600 frames
	checkFrame = random.Rand(600);

	//Max execute frames without check
	executeUpToFrame = 10*60*120;
#endif

#ifndef STAND_ALONE_GAME
	screenEditWidth = 640;
	screenEditWidth = 480; 
#endif

	u_ENV_bot = -1;
}

void GameControl::StartGameRender() 
{
	bCanAddFrameRateEvent = true;
	
#ifndef STAND_ALONE_GAME
	//Don't call this in stand alone (bypass game fps)
	SetTimer(EDITOR_FRAME_RATE);
#endif
} 


void GameControl::ReadEditorResources()
{
#ifndef STAND_ALONE_GAME
	engine->Vault()->LoadDatFile(EDITOR_DAT.getCharBuf());	
#endif
}

void GameControl::AddEditorResource(char *fileName)
{
	KrBinaryDataResource *binData = new KrBinaryDataResource(fileName);
	if(binData->LoadFile(fileName))
	{
		engine->Vault()->AddResource(binData);
	}
}



void GameControl::WriteEditorResources()
{
#ifdef _DEBUG
	//Save editor resources
	EditorDirectory editDir;

	AddEditorResource("maksbutton.bmp");
	AddEditorResource("makscursorMao.bmp");
	AddEditorResource("makscursorResize1.bmp");
	AddEditorResource("makscursorResize2.bmp");
	AddEditorResource("maksfont.bmp");
	AddEditorResource("maksiconActor.gif");
	AddEditorResource("maksiconPath.bmp");
	AddEditorResource("maksiconText.bmp");
	AddEditorResource("makskyra1.jpg");
	AddEditorResource("makslistPop.bmp");
	AddEditorResource("makslistPopBig.bmp");
	AddEditorResource("maksSDL_now.gif");
	AddEditorResource("makstileSample.bmp");
	//AddEditorResource("makswaitCursor.bmp");	
	
	SDL_RWops *src = ged_SDL_RWFromFile(EDITOR_DAT.getCharBuf(), "wb");
	if(src)
		engine->Vault()->SaveDatFile(src);
#endif
}

void GameControl::CreateEngines()
{
#ifdef _DEBUG

	ged_mkdir("bin");
	Compression comp;

	comp.DoCompression("gameEditorWindows.exe", "bin\\windows.bin");
	comp.DoCompression("gameEditorPocket.exe", "bin\\pocketpc.bin");
	comp.DoCompression("gameEditorLinux", "bin\\linux.bin");
	comp.DoCompression("gameEditorPocket.exe.gpe", "bin\\gp2x.bin");

#endif
}

bool InModalActor(void *actor)
{
	return GameControl::Get()->InModal((Actor *)actor);
}

bool InGameMode()
{
	return GameControl::Get()->getGameMode();
}

bool InStandAloneMode()
{
	return GameControl::Get()->getStandAloneMode();
}

void InvalidateGameScreen()
{
	engine->InvalidateScreen();
}


void GameControl::PushFindPathRequisition(Actor *actor, char *obstacleActor, double posX, double posY, double endX, double endY, double width, double height, double velocity)
{
	MuteEx mutex(queuePathFinderMutEx);

	QueuePathFinderIterator it(queuePathFinder);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Current()->toActor == actor)
		{
			//Only one path requisition per actor
			PathFinderMessage req(actor, obstacleActor, posX, posY, endX, endY, width, height, velocity);		
			*(it.Current()) = req;
			return;
		}
	}

	PathFinderMessage *msg = new PathFinderMessage(actor, obstacleActor, posX, posY, endX, endY, width, height, velocity);		
	queuePathFinder.PushBack(msg);
}

void GameControl::ClearPathFinder()
{
	//Remove all pending requisitions
	{
		MuteEx mutex(queuePathFinderMutEx);

		while(!queuePathFinder.Empty())
		{
			PathFinderMessage *msg = queuePathFinder.Front();
			queuePathFinder.PopFront();
			delete msg;
		}
	}
}

int GameControl::PathFinderThread(void *pParam)
{
	//Give a normal priority to this thread
	//If use below normal, will slow down a lot the path calc


	while(GameControl::Get())
	{
		//Get path requisition
		bool bHaveRequisition = false;
		PathFinderMessage *req = NULL;

		{
			MuteEx mutex(queuePathFinderMutEx);
			if(!GameControl::Get()->queuePathFinder.Empty())
			{
				req = GameControl::Get()->queuePathFinder.Front();				
				//Don't delete now to avoid flood
			}			
		}

		//Calculate path
		Path *path = NULL;

		if(req)
		{
			GeoPathFinder& gp = *GeoPathFinder::Get(req->obstacleActor.getCharBuf());
			MuteEx mutex(gp.GetMutex()); 

			if(gp.originalBoxes.Count() > 0)
			{
				path = GameControl::Get()->CalculatePathFinder(gp, *req);				
			}
		}
		else
		{
			//Give a long delay here
			SDL_Delay(100);
			continue;
		}

		if(req && IS_VALID_ACTOR(req->toActor))
		{
			//Test NULL path later to solve 'MoveTo Obstacle Draw Actor.ged' bug
			GameControl::Get()->PostMessage(NULL, req->toActor, (Uint32)path, EXECUTE_PATH_FINDER);
		}

		if(req)
		{
			//Delete req here
			MuteEx mutex(queuePathFinderMutEx);
			GameControl::Get()->queuePathFinder.PopFront();
			delete req;
		}

		//Sleep a little
		SDL_Delay(10);
	}

	return 0;
}

Path *GameControl::CalculatePathFinder(GeoPathFinder &gp, PathFinderMessage &req)
{
	Path *path = NULL;
	
	//Swap end with start to get path even if the destination is invalid
	/*gp.destination = Vec2f(req.posX, req.posY);
	gp.pos = Vec2f(xEnd - axisX, yEnd - axisY);/**/
	gp.pos = Vec2f(req.posX, req.posY);
	gp.destination = Vec2f(req.endX, req.endY);/**/

	gp.dimensions = Vec2f(req.width, req.height);
	

	//Calculate the best path
	GlDynArray <Vec2f> controlPoints;

	if(gp.findGeoAStar(gp.pos, gp.destination, controlPoints))
	{
		GlDynArray <Vec2f> smoothedPath;
		gp.smoothPath(smoothedPath);

		//Create game path
		int numSmoothedPoints = smoothedPath.size();
		if(numSmoothedPoints > 1)
		{
			//Get path lenght
			int pathLenght = 0, i = 0;
			for(i=1; i < numSmoothedPoints; i++)
			{
				pathLenght += distance(smoothedPath[i-1].x, smoothedPath[i-1].y, smoothedPath[i].x, smoothedPath[i].y);
			}

			//Set the path velocity
			pathLenght /= req.velocity;
			if(pathLenght < numSmoothedPoints) pathLenght = numSmoothedPoints;
			if(pathLenght <= 2) pathLenght = 2;

			//Create the path
			//Vec2f ini(smoothedPath[numSmoothedPoints-1]);
			Vec2f ini(smoothedPath[0]);
			path = new Path("", numSmoothedPoints, pathLenght);		


			//Add the other points relative to the first path point
			//for(i=numSmoothedPoints-1; i >= 0; i--)
			for(i=0; i < numSmoothedPoints; i++)
			{	
				path->AddKey(smoothedPath[i].x - ini.x, smoothedPath[i].y - ini.y, false);
			}

			path->ConstantVelocity();
		}
	}

	return path;
}

SDL_mutex *queueMessageMutEx = SDL_CreateMutex();
void GameControl::PostMessage(Actor *from, gedString &toName, Uint32 messageId, Uint32 message)
{
	/*
	Post message from actor in queue message
	to all actors with name toName
	The messages will be processed one at once on main message loop
	*/
	
	MuteEx mutex(queueMessageMutEx);

	ListActor *listActor = mapActors[toName];	
	for(int il = 0; listActor && il < listActor->Count(); il++)
	{
		Actor *toActor = (*listActor)[il];
		ActorMessage *msg = new ActorMessage(from, toActor, messageId, message, toActor->getActorName());		
		queueMessage.PushBack(msg);
	}	
}

Actor *GameControl::GetActor(const gedString &actorName, bool bRunning, bool bExplicitlyCreateActor, bool bAllowLoad)
{
#ifdef STAND_ALONE_GAME
	//From running actors
	Actor **actor = NULL, *actorFinded = NULL;
	bool bNewFromEdit = true;

	if(bRunning || bStandAloneMode)
	{
		actor = mapRunningActor[actorName];
		if(actor && *actor) 
		{
			actorFinded =  *actor;
		}
	}
	else 
	{
		if(RegionLoad::getCreatingActors())
		{
			actor = mapRunningActor[actorName];
			if(actor && *actor) 
			{
				actorFinded =  *actor;
			}
		}
		else
		{
			bNewFromEdit = false;
		}
	}

	//From edit actors
	if(!actorFinded)
	{
		//By using if(bRunning) here will make 1945_4.ged don't run right
		actor = mapEditActor[actorName];
		if(actor && *actor) 
		{
			if(bNewFromEdit)
			{
				Actor *newActor = NULL;
				if((*actor)->getCreateAtStartup())
				{
					if(GetActorIndex(actorName.getCharBuf(), bExplicitlyCreateActor) != NULL)
					{
						if(!actorName.hasPoint())
						{
							//Create any clone
							newActor = (*actor)->NewActor(false);	
						}
						else
						{
							//Need to create the specified clone
							//Solve the Move to Region 2.ged actor move to a wrong position
							newActor = (*actor)->NewActor(false, true, (*actor)->getX(), (*actor)->getY(), false, true);	
						}	
					}
				}
				else 
				{
					newActor = *actor;
					newActor->getImage()->SetVisible(false);
				}
				
				actorFinded = newActor;
			}
			else
			{
				actorFinded = *actor;
			}
		}
		
		//From disk
		if(!actorFinded && (bStandAloneMode || !bGameMode) && bAllowLoad)
		{
			//actorFinded = Actor::Load(actorName.getCharBuf(), bExplicitlyCreateActor);
			
			//Solve the bug in Delta_V/17.ged (show intructions when create ship)
			if(actorName.find(".") == gedString::npos)
			{
				actorFinded = Actor::Load((actorName + ".0").getCharBuf(), bExplicitlyCreateActor); //Get first
			}
			
			if(!actorFinded) 
			{
				//Try even if there is not a clone name
				//solve parentView 2 (ball bug).ged
				actorFinded = Actor::Load(actorName.getCharBuf(), bExplicitlyCreateActor);
			}
		}
	}

	if(actorFinded && actorFinded > COLLIDE_ACTOR)
		actorFinded->UpdateRegionLoad();

	return actorFinded;

#else 

	if(actorName == AXIS_NAME)
	{
		return NULL;
	}
	else if(actorName == S_EVENT_ACTOR)
	{
		return EVENT_ACTOR;
	}
	else if(actorName == S_CREATOR_ACTOR || actorName == S_CREATOR)
	{
		return CREATOR_ACTOR;
	}
	else if(actorName == S_PARENT_ACTOR || actorName == S_PARENT)
	{
		return PARENT_ACTOR;
	}
	else if(actorName == S_COLLIDE_ACTOR || actorName == S_COLLIDE)
	{
		return COLLIDE_ACTOR;
	}

	int i = actorName.find(".");
	long cloneindex = -1;

	if(i > 0) 
	{
		if(actorName.find(".", i + 1) != gedString::npos)
		{
			//Invalid actor name: name.index.index
			//Solve the bug in alpha8_debug.ged 
			//"Don't change animation in exported version"
			return NULL;
		}
	}

	gedString name = actorName.substr(0,i);

	if(i > 0) cloneindex = atol(actorName.substr(i+1).c_str());
	
	Actor *actorFinded = NULL;
	ListActor *listActor = mapActors[name];
	if(listActor)
	{
		if(!listActor->size())
			return NULL;
		
		
		//Works on Delta_V/20.ged
		if(bRunning || bStandAloneMode)
		{
			//First, try from running actors			
			for(int il = 0; il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				//Test  && DeleteConfirmation to solve the bug
				//in Move to Region 2.ged actor move to a wrong position

				if((/*bStandAloneMode ||*/ actor->getRunning()) && !actor->getDeleteConfirmation() && (cloneindex == -1 || actor->getCloneIndex() == cloneindex))
				{
					/*if(bStandAloneMode && !actor->getCreateAtStartup())
						actor->getImage()->SetVisible(true);*/ //"Create" now

					actorFinded = actor;
					break;
				}			
			}

			//Try from edit actors
			//By using  && bRunning here will make 1945_4.ged don't run right
			//But there is another problem: if you get edit actors, you will change the actors that are not running.
			//Create an actor actor2, put the 'Create at startup' to no, and, in some script, put the code:
			//
			//	actor2.transp = .5;
			//
			//After exit the game mode, the actor2 in the stage will have the transparency modified!

			if(!actorFinded && /*!bStandAloneMode &&*/ actorName != AXIS_NAME)
			{
				for(int il = 0; il < listActor->Count(); il++)
				{
					Actor *actor = (*listActor)[il];
					if(!actor->getRunning() && (cloneindex == -1 || actor->getCloneIndex() == cloneindex))
					{		
						Actor *newActor = NULL;
						if(actor->getCreateAtStartup())
						{
							bool bActorDestroyed = false;
					
							if(cloneindex == -1)
							{
								//Try first available clone (solve parentView 2 (ball bug).ged)
								//Works with Vector Smash0.6.ged
								//bActorDestroyed = GetActorIndex((actorName+".0").getCharBuf(), bExplicitlyCreateActor) == NULL;
								bActorDestroyed = GetActorIndex(actor->getCloneName(), bExplicitlyCreateActor) == NULL;
								
							}
							else
							{
								bActorDestroyed = GetActorIndex(actorName.getCharBuf(), bExplicitlyCreateActor) == NULL;
							}

							if(!bActorDestroyed)
							{
								if(cloneindex == -1)
								{
									//Create any clone
									newActor = actor->NewActor(false);	
								}
								else
								{
									//Need to create the specified clone
									//Solve the Move to Region 2.ged actor move to a wrong position
									newActor = actor->NewActor(false, true, actor->getX(), actor->getY(), false, true);	
								}
							}
						}

						else 
						{
							newActor = actor;
							newActor->getImage()->SetVisible(false);
						}


						actorFinded = newActor;
						break;
					}			
				}
			}
		}
		else
		{
			if(RegionLoad::getCreatingActors())
			{
				//First, try from running actors				
				for(int il = 0; il < listActor->Count(); il++)
				{
					Actor *actor = (*listActor)[il];
					if(actor->getRunning() && (cloneindex == -1 || actor->getCloneIndex() == cloneindex))
					{
						actorFinded = actor;
						break;
					}			
				}

				//Try from edit actors
				if(!actorFinded)
				{
					for(int il = 0; il < listActor->Count(); il++)
					{
						Actor *actor = (*listActor)[il];
						if(!actor->getRunning() && (cloneindex == -1 || actor->getCloneIndex() == cloneindex))
						{		
							Actor *newActor = NULL;
							if(actor->getCreateAtStartup())
							{
								bool bActorDestroyed = false;
								
								if(cloneindex == -1)
								{			
									//Try first available clone (solve parentView 2 (ball bug).ged)
									//Works with Vector Smash0.6.ged
									//bActorDestroyed = GetActorIndex((actorName+".0").getCharBuf(), bExplicitlyCreateActor) == NULL;
									bActorDestroyed = GetActorIndex(actor->getCloneName(), bExplicitlyCreateActor) == NULL;
								}
								else
								{
									bActorDestroyed = GetActorIndex(actorName.getCharBuf(), bExplicitlyCreateActor) == NULL;
								}
								
												
								if(!bActorDestroyed)
								{
									if(cloneindex == -1)
									{
										//Create any clone
										newActor = actor->NewActor(false);	
									}
									else
									{
										//Need to create the specified clone
										//Solve the Move to Region 2.ged actor move to a wrong position
										newActor = actor->NewActor(false, true, actor->getX(), actor->getY(), false, true);	
									}
								}
								
							}
							else 
							{
								newActor = actor;
								newActor->getImage()->SetVisible(false);
							}
							
							actorFinded = newActor;
				 			break;
						}			
					}
				}
			}
			else
			{
				for(int il = 0; il < listActor->Count(); il++)
				{
					Actor *actor = (*listActor)[il];
					if(!actor->getRunning() && (cloneindex == -1 || actor->getCloneIndex() == cloneindex))
					{
						actorFinded = actor;
						break;
					}			
				}
			}
		}	
	}

	//Try from disk
	if((bStandAloneMode || !bGameMode) && !actorFinded && bAllowLoad)
	{
		if(cloneindex == -1)
		{
			actorFinded = Actor::Load((actorName + ".0").getCharBuf(), bExplicitlyCreateActor); //Get first
		}
		
		if(!actorFinded) 
		{
			//Try even if there is not a clone name
			//solve parentView 2 (ball bug).ged
			actorFinded = Actor::Load(actorName.getCharBuf(), bExplicitlyCreateActor);
		}
			
	}

	if(actorFinded)
		actorFinded->UpdateRegionLoad();

	return actorFinded;
#endif
}

GlDynArray<void *> ActorMessage::freeList;

void GameControl::PostMessage(Actor *from, Actor *to, Uint32 messageId, Uint32 message)
{
	/*
	Post message from actor in queue message
	to actor 'to'
	The messages will be processed one at once on main message loop
	*/

	if(!to)
		return;

	MuteEx mutex(queueMessageMutEx);

	ActorMessage *msg = new ActorMessage(from, to, messageId, message, to->getActorName());		
	queueMessage.PushBack(msg);	
}


void GameControl::ProcessMessage()
{
	//Process all message
	
	MuteEx mutex(queueMessageMutEx);

#ifndef STAND_ALONE_GAME
	bool bRemoveSelectedActor = false;
#endif

	
	while(!queueMessage.Empty())
    {
        ActorMessage *msg = queueMessage.Front();
        queueMessage.PopFront();

		if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == DELETE_ME)
		{
			Actor *actor = msg->getActorTo();
			
			if(IS_VALID_ACTOR1(actor) && actor->getDeleteConfirmation())
			{
				delete actor;
			}			
		}
#ifndef STAND_ALONE_GAME
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == ADD_TO_PANEL_ACTORS)
		{
			Actor *actor = msg->getActorTo();
			
			if(IS_VALID_ACTOR1(actor))
			{
				AddToPanelActors(actor, false);
			}			
		}
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == REMOVE_SELECTED_ACTOR)
		{
			bRemoveSelectedActor = true;			
		}
#endif
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == ADD_ME_TO_HANDLED_COLLISION_MAP)
		{
			handledCollisionMap.Add(msg->getActorTo(), 1);
		}
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == REMOVE_ME_FROM_HANDLED_COLLISION_MAP)
		{
			handledCollisionMap.Remove(msg->getActorTo());
		}
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == ADD_ME_TO_WORKING_MAP)
		{
			Actor *actor = msg->getActorTo();	

			if(IS_VALID_ACTOR1(actor))
			{
				ListActor *listActor = mapWorkingActors.FindString(actor->getActorName());
				if(listActor)
				{
					listActor->PushBack(actor);
				}
				else
					mapWorkingActors.FindOrCreate(actor->getActorName())->PushBack(actor);	
			}
		}
		else if(msg->getMessageId() == msg->getMessage() && msg->getMessage() == REMOVE_ME_FROM_WORKING_MAP)
		{
			//Allow in not game mode too, to provide an clean up after SetGameMode(false)
			//Solve physical3.ged bug (execute -> ok, execute -> error) (from 1.2.7 to 1.2.8 version)

			Actor *actor = msg->getActorTo();			

			//if(IS_VALID_ACTOR1(actor)) //Don't need if get the actor name from msg
			{
				ListActor *listActor = mapWorkingActors[msg->getNameActorTo()];
				if(listActor)
				{
					listActor->FindAndDelete(actor);
					
					if(listActor->size() == 0)
					{
						mapWorkingActors.Remove(msg->getNameActorTo());
					}			
				}
			}
		}
		else if(msg->getMessage() == EXECUTE_PATH_FINDER)
		{
			Actor *actor = msg->getActorTo();

			if(getGameMode() && IS_VALID_ACTOR1(actor))
			{
				actor->SetPathFinder((Path *)msg->getMessageId());
			}
			else
			{
				delete (Path *)msg->getMessageId();
			}
		}
		else
		{
			//Only in game mode
			Actor *actor = msg->getActorTo();

			if(IS_VALID_ACTOR1(actor))
			{
				actor->OnMessage(msg->getActorFrom(), msg->getMessageId(), msg->getMessage());
			}
		}
		
		delete msg;
    }

#ifndef STAND_ALONE_GAME
	if(bRemoveSelectedActor)
	{
		RemoveSelectedActor();
	}
#endif
}


void GameControl::AddActor(Actor *actor)
{
	ListActor *listActor = mapActors.FindString(actor->getActorName());
	if(listActor)
	{
		/*Actor *lastActor = listActor->Back();
		if(lastActor && (lastActor->getRunning() || !bGameMode)) //Not increase edit actors in game mode
		{
			nextCloneIndex = lastActor->getCloneIndex() + 1;
		}*/		

		listActor->PushBack(actor);
	}
	else
		mapActors.FindOrCreate(actor->getActorName())->PushBack(actor);

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	globalMapActor.Add(actor, 1);
#endif

	AddEditActor(actor);
}

void GameControl::AddActorToHandledCollisionMap(Actor *actor)
{
	//Use messages to avoid destroy an actor, post remove from map message
	//add a new actor reusing the address and executing the message for remotion
	//http://game-editor.com/forum/viewtopic.php?p=22787 (Sometimes, ff an actor is created in a collision, it will not receive collision events)
	actor->PostMessage(actor, ADD_ME_TO_HANDLED_COLLISION_MAP, ADD_ME_TO_HANDLED_COLLISION_MAP);
}

void GameControl::RemoveActorFromHandledCollisionMap(Actor *actor)
{
	//Use messages to avoid invalid iterators crash
	actor->PostMessage(actor, REMOVE_ME_FROM_HANDLED_COLLISION_MAP, REMOVE_ME_FROM_HANDLED_COLLISION_MAP);	
}

void GameControl::AddWorkingActor(Actor *actor)
{
	//Use messages to avoid invalid iterators crash
	actor->PostMessage(actor, ADD_ME_TO_WORKING_MAP, ADD_ME_TO_WORKING_MAP);	
}

void GameControl::RemoveWorkingActor(Actor *actor)
{
	//Use messages to avoid invalid iterators crash	
	actor->PostMessage(actor, REMOVE_ME_FROM_WORKING_MAP, (Uint32)REMOVE_ME_FROM_WORKING_MAP);	
}

void GameControl::CheckCollisions()
{
	/*

	Collision detection:

		- Walk through all actors that handle collisions
		- For each actor, check collision only with actors in your mapCollision

	

	*/

	if(!bGameMode || !axis || axis->getScale() != 1.0) return;

	mapActorsInCollisionInActualFrame.Clear(false); //Don't release memory to avoid memory allocations

#ifdef USE_WALK
	engine->Tree()->Walk(); //Don't need with Walk1() optimization
#endif

	for(Actor ***actor = handledCollisionMap.GetKeyArray(); *actor; actor++)
	{
		(**actor)->getAction()->CheckCollisions(**actor);		
	}	
}

void GameControl::AddActorsInCollisionInActualFrame(stActorsInCollision2 &actorsInCollision)
{
	//If actor a collides with actor b, and a has moved
	//When b does tested will receive collision event

	if(mapActorsInCollisionInActualFrame[actorsInCollision] == NULL)
	{
		mapActorsInCollisionInActualFrame.Add(actorsInCollision, 1);
	}
}

bool GameControl::IsActorsInCollisionInActualFrame(stActorsInCollision2 &actorsInCollision)
{
	return mapActorsInCollisionInActualFrame[actorsInCollision] != NULL;
}

void GameControl::SetAnteriorPositions()
{
	//Called after all frame processing

#ifndef USE_RTREE
	for(ListActor **pList = mapWorkingActors.GetValueArray(); *pList; pList++)
	{
		ListActor *listActor = *pList;
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(IS_VALID_ACTOR1(actor)) //avoid crash when destroy an actor
				actor->SetAnteriorPositions();
		}
	}
#else
	for(int iActor = 0; iActor < visibleActors.Count(); iActor++)
	{
		Actor *actor = visibleActors[iActor];
		if(IS_VALID_ACTOR1(actor)) //avoid crash when destroy an actor
		{
			actor->SetAnteriorPositions();	
		}
	}
#endif

	if(IS_VALID_ACTOR1(viewActor))
	{
		viewActor->SetAnteriorPositions();
	}
}

void GameControl::RemoveActor(Actor *actor, bool bNotifyActors)
{
	if(!actor) return;
	
#ifdef DEBUG
	if(bGameMode || bDestroingGame) GLOUTPUT("--- Removing actor: %s, %s, (%X)\n", actor->getActorName(), actor->getCloneName(), actor);
#endif

	if(bNotifyActors)
	{
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *act = (*listActor)[il];
				if(act->EditMode() && act->getParent() == actor)
				{
					act->RemoveParent();
				}
			}
		}
	}

	if(actorModal)
	{
		modal.FindAndDelete(actor);
		if(modal.Count())
		{
			actorModal = modal.Back();				
		}
		else
		{			
			actorModal = NULL;

			//Push undo
#ifndef STAND_ALONE_GAME
			Actor *p = actorModal;

			if( !bGameMode && bInitialCheck && 
				p != LoadSaveGame::getLastPanel() &&
				p != PanelInfo::getLastPanel() 
				) 
			{
				//Don't push if is a LoadSaveGame panel or a PanelInfo
				UndoControl::Get()->PushCurrentState();
			}
#endif			
		}
	}

	if(actor == currentActor)
	{
		currentActor = NULL;
	}

	if(actor == actorDrag)
	{
		actorDrag = NULL;
	}

	ListActor *listActor = mapActors.FindString(actor->getActorName());
	bool bMoreActors = false;
	if(listActor)
	{
		listActor->FindAndDelete(actor);
		
		if(listActor->size() == 0)
		{
			if(!AddToActorCache(actor))
			{
				//Keep the list to optimize Action::CheckCollisions
				//Will impact on performance of mapActors iterators?
				mapActors.Remove(actor->getActorName(), false);
			}
		}
		else
		{
			bMoreActors = true;
		}
	}

	//Remove from working map
	//Already use the REMOVE_ME_FROM_WORKING_MAP message in Actor destructor
	/*ListActor *listWorkingActor = mapWorkingActors.FindString(actor->getActorName());
	if(listWorkingActor)
	{
		listWorkingActor->FindAndDelete(actor);
		
		if(listWorkingActor->size() == 0)
		{
			mapWorkingActors.Remove(actor->getActorName());
		}			
	}*/

	cacheActor.Remove(actor);

#ifdef ACTOR_USES_VIRTUAL_FUNCTIONS
	globalMapActor.Remove(actor);
#endif

	//Remove from mapRunningActor
	Actor **running = mapRunningActor.FindString(actor->getCloneName());
	if(running && *running == actor)
	{
		mapRunningActor.Remove(actor->getCloneName(), false);			
	}
	
	if(bMoreActors)
	{
		//Update default actor with first edit actor
		Actor *firstActor = NULL;

		if(listActor)
		{			
			for(int il = 0; il < listActor->Count(); il++)
			{
				firstActor = (*listActor)[il];
				if(firstActor->getRunning() /*&& engine->Tree()->FindNodeById((int)firstActor->getImage())*/)
				{	
					break;
				}
			}
		}
		
		if(firstActor)
		{
			mapRunningActor.Add(actor->getActorName(), firstActor);
		}
		else
		{
			mapRunningActor.Remove(actor->getActorName(), false);
		}
	}
	else
	{
		mapRunningActor.Remove(actor->getActorName(), false);
	}

	//Remove from mapEditActor
	Actor **edit = mapEditActor.FindString(actor->getCloneName());
	if(edit && *edit == actor)
	{
		mapEditActor.Remove(actor->getCloneName(), false);			
	}
	
	if(bMoreActors)
	{
		//Update default actor with first edit actor
		Actor *firstActor = NULL;
		
		if(listActor)
		{			
			for(int il = 0; il < listActor->Count(); il++)
			{
				firstActor = (*listActor)[il];
				if(!firstActor->getRunning() /*&& engine->Tree()->FindNodeById((int)firstActor->getImage())*/)
				{	
					break;
				}
			}
		}
		
		if(firstActor)
		{
			mapEditActor.Add(actor->getActorName(), firstActor);
		}
		else
		{
			mapEditActor.Remove(actor->getActorName(), false);
		}
	}
	else
	{
		mapEditActor.Remove(actor->getActorName(), false);
	}
}

void GameControl::AddRunningActor(Actor *actor)
{
	//To speed up GetActor

	if(actor->getRunning())
	{
		//Not trouble with many actors with save clone name
		mapRunningActor.Add(actor->getCloneName(), actor);

		//Default actor
		mapRunningActor.Add(actor->getActorName(), actor);

		//Remove name from mapEditActor only if the save actor
		Actor **edit = mapEditActor.FindString(actor->getCloneName());
		if(edit && *edit == actor)
		{
			mapEditActor.Remove(actor->getCloneName(), false);
		}

		edit = mapEditActor.FindString(actor->getActorName());
		if(edit && *edit == actor)
		{
			mapEditActor.Remove(actor->getActorName(), false);

			//Update default actor with first edit actor
			ListActor *listActor = mapActors.FindString(actor->getActorName());
			Actor *firstActor = NULL;

			
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				firstActor = (*listActor)[il];
				if(!firstActor->getRunning() /*&& engine->Tree()->FindNodeById((int)firstActor->getImage())*/)
				{	
					break;
				}
			}

			if(firstActor)
			{
				mapEditActor.Add(actor->getActorName(), firstActor);
			}
		}	
	}
}

void GameControl::AddEditActor(Actor *actor)
{
	//To speed up GetActor

	if(!actor->getRunning())
	{
		//Not trouble with many actors with save clone name
		mapEditActor.Add(actor->getCloneName(), actor);

		//Default actor
		mapEditActor.Add(actor->getActorName(), actor);
	}
}

Actor *GameControl::FindEditActor(gedString &name) 
{
	Actor **actor = mapEditActor[name];
	if(actor)
	{
		return *actor;
	}

	return NULL;
}

void GameControl::RemoveEditActor(gedString &name) 
{
	mapEditActor.Remove(name, false);
}

bool GameControl::AddToActorCache(Actor *actor)
{
	//Crash in 1945.ged ?
	//Don't use now. 
	//Causes the bug in view_change_bug.ged in stand alone version	
	return false; 

	if(!bStandAloneMode || !bGameMode || bCacheClean)
	{
		return false;
	}

	Actor *cache = actor->NewActor(true, false, 0, 0, true);
	if(cache)
	{
		cacheActor.Add(cache, SDL_GetTicks());
		return true;
	}

	return false;
}

void GameControl::RemoveOldActorsFromCache(bool bForce)
{
	bCacheClean = true;
	U32 now = SDL_GetTicks();
	if(bForce) now += 100000;

	CacheMapActorIterator it(cacheActor);
	for(it.Begin(); !it.Done(); it.Next())
	{
		Actor *cache = *it.Key();
		U32 cacheTime = *it.Value();

		if((now - cacheTime) > 30000)
		{
			//Remove if more 30s old
			delete cache;
			it.Begin();
		}
	}

	bCacheClean = false;
}

Actor *GameControl::getDragActor()
{
	return actorDrag;
}



KrFontResource *GameControl::GetFont(const gedString& fontName, int iniASCIIChar, int nChars)
{
	KrFontResource *fontResource = engine->Vault()->GetFontResource(fontName);
	
	if(fontResource)
	{
		return fontResource;
	}

	SDL_ClearError();

#ifndef STAND_ALONE_GAME

	//Adjust ttf font
	gedString fileName(fontName), s(fontName);
	s.lower();

	if(s.substr(s.length() - (16 + 4), 4) == ".ttf")
	{
		int i_style, i_size, ui_r, ui_g, ui_b, i_antialias;
		fileName = fileName.substr(0, fileName.length() - 16);

		i_style	= atoi(fontName.substr(fontName.length() - 16, 3).c_str());
		i_size	= atoi(fontName.substr(fontName.length() - 13, 3).c_str());
		ui_r	= atoi(fontName.substr(fontName.length() - 10, 3).c_str());
		ui_g	= atoi(fontName.substr(fontName.length() - 7, 3).c_str());
		ui_b	= atoi(fontName.substr(fontName.length() - 4, 3).c_str());
		i_antialias	= atoi(fontName.substr(fontName.length() - 1, 1).c_str());
		
		AddTrueTypeFont(fileName.c_str(), i_style, i_size, ui_r, ui_g, ui_b, i_antialias);

		return engine->Vault()->GetFontResource(fontName);
	}

	SDL_Surface *surface = LoadSurface(fileName.c_str());
	

	if(!surface)
	{
		return NULL;
	}

	KrPaintInfo info( surface );

	//Set transparent color
	KrPainter painter( surface );
	KrRGBA transRgba;
	painter.BreakPixel( 0, 0, &transRgba );	
	info.SetTranparentColor(transRgba);

	fontResource = new KrFontResource(	fontName,
													&info,
													iniASCIIChar,
													0,
													KrFontResource::FIXED,
													nChars );

	engine->Vault()->AddResource(fontResource);

	SDL_FreeSurface( surface );
	return fontResource;
#else
	return NULL;
#endif
}

int GameControl::Height()
{
	return engine->ScreenBounds().Height();	
}

int GameControl::Width()
{
	return engine->ScreenBounds().Width();	
}

void GameControl::SetModal(Actor *actor)
{
	actorModal = actor;
	modal.PushBack(actor); //If this actor open other modal actor...
}

Actor *GameControl::GetModal()
{
	return actorModal;
}



bool GameControl::InModal(Actor *actor)
{
	if(!actorModal)
		return true;

	while(actor > COLLIDE_ACTOR)
	{
		if(actor == actorModal)
			return true;

		actor = actor->getParent();
	}

	return false;
}

void GameControl::SetPathMode(bool _bPathMode)
{
	bPathMode = _bPathMode;

	if(!bPathMode && pathPoint)
	{
		delete pathPoint->getRootPath();
		pathPoint = NULL;
	}
}

PathPoint *GameControl::GetPathRoot()
{
	if(pathPoint)
	{
		return pathPoint->getRootPath();
	}
	
	return NULL;
}

bool GameControl::PathMode()
{
	return bPathMode;
}



void GameControl::ChangePathPoint(PathPoint *point)
{
	pathPoint = point;
}



void GameControl::SetTileMode(Tile *tile)
{
	if(currentTile)
	{
		currentTile->SetVisibleCursor(false);

#if !defined(STAND_ALONE_GAME)
		if(!currentTile->NumTiles())
		{
			currentTile->getParentActor()->RemoveTile();
		}
#endif
	}

	if(tile) tile->SetVisibleCursor(true);

	currentTile = tile;
}



void GameControl::AddPath(const gedString& pathName, int pathLenght)
{
	PathPoint *root = GetPathRoot();
	if(!root)
		return;

	Path *path = new Path(pathName, root->TotalKeys(), pathLenght);
	
	int oriX = root->getX(), oriY = root->getY();
	path->SetOri(oriX, oriY);
	path->AddKey(0, 0, root->getLinear());
	root = root->getNext();

	while(root)
	{
		path->AddKey(root->getX(), root->getY(), root->getLinear());
		root = root->getNext();
	} 

	path->ConstantVelocity();
	paths.Add(pathName, path);		
}



void GameControl::RemovePath(const gedString& pathName)
{
	Path **p = paths[pathName], *path;
	
	if(p)
	{
		path = *p;

		//Notify actors
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *act = (*listActor)[il];
				if(act->EditMode())
				{
					if(act->getPathX() == path)
					{
						act->SetPathX(NULL);
					}
					
					if(act->getPathY() == path)
					{
						act->SetPathY(NULL);
					}
				}
			}
		}
		
		//Delete
		paths.Remove(pathName);
		delete path;
	}
	
}

bool GameControl::ExistsPath(const gedString& pathName)
{
	return paths[pathName] != NULL;	
}

int GameControl::NumPaths()
{
	return paths.size();
}

gedString GameControl::PathName(int i)
{	
	int n = 0;

	MapPathIterator it(paths);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(n == i)
		{
			return *it.Key();
		}
		n++;
	}

	return "";
}

int GameControl::NumActors(bool bRunning)
{
	int nActors = 0;
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if((bRunning && actor->getRunning()) || (!bRunning && actor->EditMode()))
			{
				nActors++;
			}
		}
	}

	return nActors;
}

gedString GameControl::ActorName(int i)
{
	int n = 0;
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode())
			{
				if(n == i)
				{
					return actor->getCloneIndex()?actor->getCloneName():actor->getActorName();
				}
				n++;
			}			
		}
	}

	return "";
}

Path *GameControl::GetPath(const gedString &pathName)
{
	Path **path = NULL;

	if(pathName == RANDOM_PATH)
	{
		//Load all paths
		MapGameFileIndexIterator itPath(pathIndex);
		for(itPath.Begin(); !itPath.Done(); itPath.Next())
		{
			if(!paths[*itPath.Key()])
			{
				LoadPath(*itPath.Key());		
			}
		}

		//Choose
		if(paths.size())
		{
			gedString randomPath(PathName((int)round(rand(paths.size()))));
			
			path = paths[randomPath];
			if(path)
				return *path;
			
			//Try load from disk
			return LoadPath(randomPath);
		}
	}
	
	path = paths[pathName];
	if(path)
		return *path;

	//Try load from disk
	return LoadPath(pathName);
}



int GameControl::GetNumTimers()
{
	return mapTimerNameDefinition.size();
}

stTimerDefinition *GameControl::GetTimer(gedString timerName)
{
	int pos = timerName.find(" (");
	if(pos >= 0) timerName = timerName.substr(0, pos);

	return mapTimerNameDefinition[timerName];
}

bool GameControl::AddTimerDefinition(const gedString& timerName, Uint32 interval, int count, int type, Uint32 minRandomInterval)
{
	if(interval <= 0 || (type == RANDOM_TIMER && minRandomInterval > interval) || (count != REPEATE_FOREVER && count <= 0) || mapTimerNameDefinition[timerName])
	{
		return false;
	}

	mapTimerNameDefinition.Add(timerName, stTimerDefinition(interval, minRandomInterval, count, type));
	return true;
}



bool GameControl::SetGameMode(bool _bGameMode, bool bSwitchResolution) 
{	
	SetTileMode(NULL);
	PauseGame(false);
	bCheckOutOfVision = false;
	bFastFile = false;
	bMouseButtonDown = false;
	bRestartNetworkAfterPause = false;
	GeoPathFinder::Remove();

	if((!bGameMode && !_bGameMode) ||
	   (bGameMode && _bGameMode))
	{
		return true;
	}

	
	EiC_ENV->lsp = 0; //maks: only reset lsp here (avoid crash in ALPHA_1_1_4.ged exported game (to script in script execution))

#ifndef STAND_ALONE_GAME
	static double scaleAnt = 1.0;
#endif

	static int xAxisAnt, yAxisAnt;	
	bGameMode = _bGameMode;


#if  !defined(STAND_ALONE_GAME) && defined(WIN32)

	/*if(bGameMode != bOldMode) //Don't use now. Keyboard don't works without directx in some machines
	{
		//Reset video to allow the system use DirectX only in game mode (see the DX5_Available() function)
		//http://game-editor.com/forum/viewtopic.php?p=21253
		SDL_VideoQuit();
		SDL_VideoInit(NULL, 0);
	}*/

	//Get window position
	static RECT rcEditorWindow;
#endif

	mapKeyDown.Clear();
	lastKeys.Clear();

	RemoveOldActorsFromCache(true);

	while(lastKeys.Count() < MAX_KEY_SEQUENCE) lastKeys.PushBack(0);

	if(!bStandAloneMode) //solve the TheGameFile.ged stand alone bug
	{
		handledCollisionMap.Clear();
	}


	//Adjust scale and position
	if(axis)
	{
		if(bGameMode)
		{
#ifndef STAND_ALONE_GAME
			SaveEditorCanvas();
			
			scaleAnt = axis->getScale();
			axis->SetScale(1.0);
#endif //#ifndef STAND_ALONE_GAME
			
			xAxisAnt = axis->getImage()->X();
			yAxisAnt = axis->getImage()->Y();
			
			
			//axis->SetPos(resX/2, resY/2);
			axis->getImage()->Invalidate();
			
			if(gamePath.length()) chdir(gamePath.c_str());
			
			lastKey = SDLK_UNKNOWN;
			userKeys.Clear();
		}
		else
		{
			axis->getImage()->Invalidate();
		}
	}

	//Show/Hide panels and edit actors
	if(!bStandAloneMode)
	{
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(actor->getPanel() || actor->EditMode())
				{
					actor->getImage()->SetVisible(!bGameMode);
				}
			}
		}
	}

	Actor::RestartFrameSequences();
	if(bGameMode)
	{
		bLoadGameUsedInGameMode = false;
		Script::ClearError();
		ResetVars(); //Before call the first action (view OnCreate)

#ifndef STAND_ALONE_GAME
		ClearGameGraph();
#else 
		//Create view actor here to ensure OnCreate execution
		if(!viewActor)
		{
			Actor *view = GetActor(VIEW_NAME);
			if(view) 
			{
				view->AdjustView(resX, resY, false);			
			}
		}
#endif
		

#if  !defined(STAND_ALONE_GAME) && defined(WIN32)
		//Get window position
		GetWindowRect((HWND)GetMainWindow(), &rcEditorWindow);
#endif

		

		PrepareRegions();

#ifndef STAND_ALONE_GAME
		RegionLoad::DisableMouse();
#endif

		//Create game actors
		if(!bStandAloneMode)		
		{
			Action::SetExpressionPointers();

			//Set actor state
			actorIndex.Clear();
			MapActorIterator it(mapActors);
			for(it.Begin(); !it.Done(); it.Next())
			{
				Actor::ResetMaxCloneIndex(it.Key()->getCharBuf());

				ListActor *listActor = it.Value();
				for(int il = 0; listActor && il < listActor->Count(); il++)
				{
					Actor *actor = (*listActor)[il];
					if(actor->EditMode())
					{
						actorIndex.Add(actor->getCloneName(), stActorState(1));				
						actorIndex.Add(actor->getActorName(), stActorState(1)); //Speed up GetActor

						if(actor->getAction())
						{
							//Call here only in editor
							actor->getAction()->SetInheritedKeyEvents();
						}
					}			
				}
			}			
		}

		//Don't start the network here
		//Only on demand (RequestNetworkStart)
		//if(bAutoStartNetwork) InitRakNet(gameNetworkPort);
		

#if !defined(GAME_EDITOR_PROFESSIONAL) && defined(STAND_ALONE_GAME)
		gameEditorInformation = new Text(decodeCipherText(geInfo), 10, resY - 40);		
		gameEditorInformation->getImage()->SetZDepth(CURSOR_DEPTH - 1);
#endif

		//Reset game frames
		gameFrame = 0;

		//Set game title
		if(gameTitle.length())
		{
			SetCaption(gameTitle.c_str(), NULL);
		}
#ifndef STAND_ALONE_GAME
		else
		{
			SetCaption("Game Editor", NULL);
		}
#endif		

		if(!SwitchResolution(engine->Surface(), resX, resY, bFullScreen))
		{
#if !defined(STAND_ALONE_GAME)
			//Stop execution
			Script::SetError("The selected resolution is not supported by this machine.\nPlease, select other resolution in Game Properties panel.");

			SDL_Event event;
			memset(&event, 0, sizeof(SDL_Event));
			event.type = SDL_KEYUP;
			event.key.keysym.sym = SDLK_ESCAPE;
			SDL_PushEvent(&event);
#endif
			
			return false;
		}

		if(!bShowMouse) SDL_ShowCursor(SDL_DISABLE);

		
#if !defined(STAND_ALONE_GAME) && defined(WIN32)
		//Hide main window if using wx
		if(SDL_Window != GetMainWindow())
		{
			SetMainWindowGameMode(true);
		}
#endif

		
#ifndef STAND_ALONE_GAME
		//Force view load
		Actor *actor = GetEditView();
		if(actor)
		{
			Actor *_viewActor = actor;
			if(!bStandAloneMode) _viewActor = actor->NewActor(false);
			
			////////////////////////////////////////////////////////////////
			//This code make the LOADING message in checkers.ged hide in next frame
			//Call view OnCreate when call UpdateView in GameTick
			/*//Solve the bug: "Don't show LOADING message in checkers.ged"
			MoveView();

			//Initial region update (fix the viewRegionBug.ged and checkers.ged)
			engine->Tree()->Walk();
			RegionLoad::UpdateView();			
			engine->Draw();
						
			//Force call view OnCreate in edit mode
			if(!_viewActor->getCallOnCreate() && _viewActor->getAction()) 
				_viewActor->getAction()->OnCreate(_viewActor);*/				
			////////////////////////////////////////////////////////////////
		}
#endif

		SetTimer();
		SDL_EnableKeyRepeat(0, 0);


		//Set EiC reset point
		//Don't use until solve the Script::Reset()
		//EiCp_initiateResetUser(&u_ENV_bot, &u_lut_bot, &u_pp_bot, &u_xalloc_bot);

#if defined(WIN32) && !defined(_WIN32_WCE) && !defined(DEBUG)
		//More smooth render (look fluidez.ged)
		//Don't play the music in the Pocket PC
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#endif
	}
	else
	{
#if defined(WIN32) && !defined(_WIN32_WCE)
		//More smooth render (look fluidez.ged)
		//Don't play the music in the Pocket PC
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

		//Move to the last user position
		GetWindowRect(SDL_Window, &rectGameWindow);		
#endif

#ifndef STAND_ALONE_GAME
		GenerateGameGraph();
		RegionLoad::EnableMouse();
#endif


		//Destroy game actors
		bDestroingGame = true; //Don't call OnDestroy actions
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];

				//Use IS_VALID_ACTOR1(actor) to try solve a crash
				//In this location when exit game mode
				if(IS_VALID_ACTOR1(actor) && actor->getRunning())
				{
					//RemoveActor(actor);
					delete actor;
					it.Begin();					
				}			
			}
		}

		DestroyRakNet();

		bDestroingGame = false;

		//Restore max clone index
		for(it.Begin(); !it.Done(); it.Next())
		{
			Actor::ResetMaxCloneIndex(it.Key()->getCharBuf());

			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(IS_VALID_ACTOR(actor) && actor->EditMode())
				{
					actor->FindMaxCloneIndex();			
				}
			}
		}

#if !defined(GAME_EDITOR_PROFESSIONAL) && defined(STAND_ALONE_GAME)
		delete gameEditorInformation;
		gameEditorInformation = NULL;
#endif

		//Set actor destroyed state
		MapActorFileIndexIterator itActor(actorIndex);
		for(itActor.Begin(); !itActor.Done(); itActor.Next())
		{
			stActorState *state = itActor.Value();	
			if(state) state->bDestroyed = false;
		}

		//Delete sounds
		MapSoundIterator itSound(mapSound);	
		if(bAudioOpened)
		{
			for(itSound.Begin(); !itSound.Done(); itSound.Next())
			{
				Mix_FreeChunk(*itSound.Value());
			}
		}
		mapSound.Clear();

		//Delete music
		if(music)
		{
			if(bAudioOpened) Mix_FreeMusic(music);
			music = NULL;
		}

		//Restore channels
		int n;

		if(bAudioOpened) 
		{
			Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
			for(n = 0; n < maximumSounds; n++)
			{
				Mix_Volume(n, SDL_MIX_MAXVOLUME);
				Mix_SetPanning(n, 255, 255);
			}
		}

		
#ifndef STAND_ALONE_GAME

		
		//No resolution changes when exit game mode in stand alone games
		if(bSwitchResolution)
		{
			SDL_ShowCursor(SDL_ENABLE);
			SwitchResolution(engine->Surface(), screenEditWidth, screenEditHeight, false);			
		}

#ifdef WIN32
		//Show main window if using wx
		if(SDL_Window != GetMainWindow())
		{
			SetMainWindowGameMode(false);
		}
#endif


		//Restore title
		if(gameName.empty())
		{
			SetCaption("Game Editor", NULL);
		}
		else
		{
			SetCaption((sGameEditorCaption + gameName).c_str(), NULL);
		}

		
		//Change scale
		RestoreEditorCanvas();

		axis->SetPos(xAxisAnt, yAxisAnt);
		axis->SetScale(scaleAnt);				
#endif //#ifndef STAND_ALONE_GAME



		//Clean-up
		RegionLoad::ClearRegions();
		Action::ClearActorsInCollision();
		ProcessMessage(); //Delete all pending messages, call after bGameMode = false
		ClearPathFinder();

		//In edit mode
		SetTimer(EDITOR_FRAME_RATE); 
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		//Restore view
		if(!bStandAloneMode)
		{
			SetViewActor(GetEditView());
		}

		//Cleanup opened files in EiC
		_ffexit();

		//Reset EiC to release memory
		//Don't use until solve the Script::Reset()
		//EiCp_ResetUser(u_ENV_bot, u_lut_bot, u_pp_bot, u_xalloc_bot);

#if  !defined(STAND_ALONE_GAME)

	#if defined(WIN32)
		//Set editor window position
		SetWindowPos((HWND)GetMainWindow(), HWND_NOTOPMOST, 
			rcEditorWindow.left, 
			rcEditorWindow.top, 
			rcEditorWindow.right - rcEditorWindow.left, 
			rcEditorWindow.bottom - rcEditorWindow.top, 
			SWP_NOSIZE);
	#endif
		
		if(Script::GetError().length())
		{
			new PanelInfo(Script::GetError(), "Runtime Error", ALIGN_LEFT);
		}
#endif		
	}

	#if !defined(STAND_ALONE_GAME) && defined(__linux__)
	SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
	bCanResize = false;
	#endif

	//Update lastick after long operation
	lastTick = SDL_GetTicks();

	return true;
}

void GameControl::CreateAxis()
{
	if(!axis) axis = new Axis();
}




bool GameControl::Load(SDL_RWops *src, bool bLoadCursor, bool bMergeGames, Uint32 offsetKyraDat, bool bShowErrorMessage)
{
#if defined(WIN32) && !defined(_WIN32_WCE)
	NormalPriority normalPriority;
#endif

#ifdef RENDER_THREAD
	MuteEx mutex(renderMutEx);
#endif

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOBEGIN
#endif

	gedString fingerPrintError;
	SDL_ClearError();

	GED_TRY
	{

	//Clear collision handles (solve the stand alone TheGameFile.ged bug)
	handledCollisionMap.Clear();

	//Go to file begin
	SDL_RWseek( src, offsetKyraDat, SEEK_SET );
	
	//Load version
	indexedGameVersion = 0;
	Uint32 version = GameControl::Read32(src);
	lastGameFileVersion = version;

	
	if(version > 14) 
	{
		SetNewReadString(true);
		
		if(version > 15) 
			gedString::SetReadEncriptString(true);
		else 
			gedString::SetReadEncriptString(false);
	}
	else
	{
		SetNewReadString(false);
		gedString::SetReadEncriptString(false);
	}
	

	if(version > GAME_FILE_VERSION)
	{
		#ifndef STAND_ALONE_GAME
		if(bShowErrorMessage) new PanelInfo("This is not a valid file for this Game Editor version\nPlease, download last version in http://game-editor.com");
		#endif //#ifndef STAND_ALONE_GAME

		NewGame(false);
		return false;
	}
	else if(version <= 8)
	{
		return LoadV8(src, bLoadCursor, bMergeGames, version);
	}

	

	Uint32 gameEditorVersion = GameControl::Read32(src);
	if(gameEditorVersion > GAME_EDITOR_VERSION)
	{
		#ifndef STAND_ALONE_GAME
		if(bShowErrorMessage) new PanelInfo("This game require the newest Game Editor version\nPlease, download last version in http://game-editor.com");
		#endif //#ifndef STAND_ALONE_GAME

		return false;
	}

	
	//Load fast file flag
	if(version > 37)
	{
		GameControl::Read(src, &bFastFile, sizeof(Uint8), 1);
	}


#ifndef STAND_ALONE_GAME
	WaitCursor wait(bLoadCursor);
#endif

	if(!bMergeGames) ClearGame();

	//////////////////////////////////////////////////////////////////////
	//Load index for actions, paths, scripts and actors

	if(indexedGame) 
	{
		SDL_RWclose(indexedGame);
		indexedGame = NULL;
	}

	if(!bMergeGames)
	{
		actionIndex.Clear();
		pathIndex.Clear();
		scriptIndex.Clear();
		actorIndex.Clear();
	}

	gedString name;
	Uint32 count, i;

	//Load actions index
	count = GameControl::Read32( src );	
	
	for(i = 0; i < count; i++)
	{
		ReadString(src, &name);
		actionIndex.Add(name, GameControl::Read32( src ) + offsetKyraDat);
	}

	
	//Load path index
	count = GameControl::Read32( src );
	
	for(i = 0; i < count; i++)
	{
		ReadString(src, &name);
		pathIndex.Add(name, GameControl::Read32( src ) + offsetKyraDat);
	}

	//Load script index
	count = GameControl::Read32( src );
	
	for(i = 0; i < count; i++)
	{
		ReadString(src, &name);
		scriptIndex.Add(name, GameControl::Read32( src ) + offsetKyraDat);
	}

	
	//Load actor index
	count = GameControl::Read32( src );
	
	for(i = 0; i < count; i++)
	{
		ReadString(src, &name);
		U32 index = GameControl::Read32( src ) + offsetKyraDat;
		actorIndex.Add(name, stActorState(index));

		//Speed up GetActor
		int i1 = name.find('.');
		long cloneindex = atol(name.substr(i1+1).c_str());
		
		//Always add the actorName (solve parentView 2 (ball bug).ged)
		if(i1 >= 0)//if(cloneindex == 0)
		{
			gedString actorName(name.substr(0,i1));			
			actorIndex.Add(actorName, stActorState(index));
		}
	}


	//Store game file pointer
	indexedGame = src;
	indexedGameVersion = version;
	//////////////////////////////////////////////////////////////////////

	if(version > 11) Script::LoadLocalUserVars(src, bMergeGames);	
	RegionLoad::LoadRegions(src);
	if(version > 12) 
	{
		//Create fake actors (to all script codes)
		MapActorFileIndexIterator itActor(actorIndex);
		for(itActor.Begin(); !itActor.Done(); itActor.Next())
		{
			gedString actorName(itActor.Key()->substr(0, itActor.Key()->find('.')));
			if(!isSymbolInEic(actorName.c_str()))
			{
				Script::AddActor(NULL, actorName.c_str());
			}
		}

		if(version < 22) 
		{
			//Load generic scripts before
			GenericScript::Load(src, bMergeGames);
		}
	}

	if(version > 38) 
	{
		//Load animation data
		gedString animKey;
		mapActorAnimation.Clear();		

		while(1)
		{
			KrAction tempAction("");
			animKey.Read(src);

			if(animKey == "ged_E N D")
			{
				break;
			}

			tempAction.Load(src, version);
			mapActorAnimation.Add(animKey, tempAction);
		}
	}

	//Load global vars
	Uint16 nVars = GameControl::Read16(src);

	for(i = 0; i < nVars; i++)
	{
		//Read vars
		if(version > 23)
		{
			stVarInfo var;
			var.Load(src);

			CreateUserVar(var.name, var.type, var.arrayLenght, var.saveGroup);
		}
		else if(version > 1)
		{
			//Load name
			gedString _name;
			_name.Read(src);

			Uint8 type;
			GameControl::Read(src, &type, sizeof(type), 1);
			CreateUserVar(_name, type, 0, "");
		}
		else
		{
			//Load name
			gedString _name;
			_name.Read(src);

			double value;
			GameControl::Read(src, &value, sizeof(value), 1);
			CreateUserVar(_name, REAL_VAR, 0, "");
		}
	}

	if(version >= 22) 
	{
		//Load generic scripts after global vars
		GenericScript::Load(src, bMergeGames);
	}

	//Load timers
	Uint16 nTimers = GameControl::Read16(src);

	for(i = 0; i < nTimers; i++)
	{
		//Load name
		gedString name;
		name.Read(src);		

		stTimerDefinition timer;

		//Read definition
		if(version < 11)
		{
			timer.interval = GameControl::Read16(src);
			timer.minRandomInterval = 0;
			timer.count = REPEATE_FOREVER;
			timer.type = PERIODIC_TIMER;

			AddTimerDefinition(name, timer.interval, timer.count, timer.type, timer.minRandomInterval); 
		}
		else
		{			
			timer.interval = GameControl::Read32(src);
			timer.minRandomInterval = GameControl::Read32(src);
			timer.count = GameControl::Read32(src);
			timer.type = GameControl::Read32(src);

			AddTimerDefinition(name, timer.interval, timer.count, timer.type, timer.minRandomInterval); 
		}
	}

	if(version > 20)
	{
		//Load base zdepth
		Actor::setBaseZDpth(GameControl::Read32(src));
	}

	
	if(version > 39)
	{
/*#ifdef DEBUG
		if(bLoadCursor && !bMergeGames && !offsetKyraDat && bShowErrorMessage && !bFastFile)
		{
			//There are a string change in the version 55
			//The game will be corrupted if load a version prior to the 55
			RemoveFingerPrint(src); 
		}
#endif*/
		gedString loaded_fingerprint(LoadFingerPrint(src)); 

		//Allow games saved by demo be loaded in other machines
/*#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(_DEBUG)
		//Can be load in undo file
		if(!bFastFile && loaded_fingerprint != pro_fingerprint && loaded_fingerprint != GetMachineFingerprint())
		{
			//Can't load this game
			fingerPrintError = "This game was saved using Game Editor's Demo Version.\nIt can't be read or played on this computer.\nThe game must be saved with Game Editor Professional\nto be read/played on other computers.";

#ifndef GAME_EDITOR_PROFESSIONAL
			fingerPrintError += "\n\nVisit www.game-editor.com to purchase the full version";
#endif
		}
#endif*/
	}

	//Load game properties
	U16 _resX = resX, _resY = resY;

/*#if defined(STAND_ALONE_GAME) && defined(_WIN32_WCE)
	//Only native resolution on PocketPC (resX and resY seted in GameControl())

	U16 rx, ry;
	rx = GameControl::Read16(src);
	ry = GameControl::Read16(src);

	if(rx == 320 && ry == 240)
	{
		_resX = rx;
		_resY = ry;
	}
#else*/
	_resX = GameControl::Read16(src);
	_resY = GameControl::Read16(src);
//#endif

	bool _bStereo, _bFullScreen, _bShowMouse;
	GameControl::Read(src, &_bFullScreen, sizeof(Uint8), 1);
	Uint16 _fps = GameControl::Read16(src);
	Uint16 _audioSamplerRate = GameControl::Read16(src);
	GameControl::Read(src, &_bStereo, sizeof(Uint8), 1);
	Uint16 _maximumSounds = GameControl::Read16(src);
	GameControl::Read(src, &_bShowMouse, sizeof(Uint8), 1);

	bool _bSuspendGameIfLostFocus = true, _bUseESCKeyToExit = true;
	if(version > 26)
	{
		GameControl::Read(src, &_bSuspendGameIfLostFocus, sizeof(Uint8), 1);

#ifdef _WIN32_WCE
		if(IsSmartphoneDevice())
		{
			//Aways suspend with system events in Smartphones
			_bSuspendGameIfLostFocus = true;
		}
#endif
	}

	if(version > 35)
	{
		GameControl::Read(src, &_bUseESCKeyToExit, sizeof(Uint8), 1);
	}

	if(version > 50)
	{
		gedString gameServer;
		gameServer.Read(src);
		gameID.Read(src);
		gameVersion = GameControl::Read32(src);

		bUseGameEditorServer = !gameServer.empty();
	}

	if(version > 51)
	{
		U8 tmp;
		GameControl::Read(src, &tmp, sizeof(Uint8), 1);
		bAutoStartNetwork = tmp;

		gameNetworkPort = GameControl::Read32(src);
	}

	if(version > 52)
	{
		connectionLostTimeOut = GameControl::Read32(src);
	}

	//Load PocketPC keys
	if(version > 17)
	{
		Uint16 nKeys = Read16(src);
		for(int i = 0; i < nKeys; i++)
		{
			SDLKey pocketKey, pcKey;
			pocketKey = (SDLKey)Read16(src);
			pcKey = (SDLKey)Read16(src);

			pocketKeys.Add(pocketKey, pcKey);
		}
	}

	//Load grid settings
	bool bGridShow, bGridSnap;
	Uint16 gridx, gridy;
	Uint32 zDepth = 0;
	
	GameControl::Read(src, &bGridShow, sizeof(Uint8), 1);
	GameControl::Read(src, &bGridSnap, sizeof(Uint8), 1);
	gridx = GameControl::Read16(src);
	gridy = GameControl::Read16(src);

	if(version > 46)
	{
		zDepth = GameControl::Read32(src);
	}
	
	if(!bMergeGames)
	{
		GetAxis()->setGridShow(bGridShow);
		GetAxis()->setGridSnap(bGridSnap);
		GetAxis()->setGridX(gridx);
		GetAxis()->setGridY(gridy);

#ifndef STAND_ALONE_GAME
		GetAxis()->SetGridZDepth(zDepth);
#endif
	}

	KrRGBA _backgroundColor;
	_backgroundColor.all = 0;
	if(version > 24)
	{
		KrRGBA mainColor, resolutionColor;

		mainColor.all = GameControl::Read32(src);
		resolutionColor.all = GameControl::Read32(src);
		_backgroundColor.all = GameControl::Read32(src);

		if(!bMergeGames)
		{
			GetAxis()->setMainColor(mainColor);
			GetAxis()->setResolutionColor(resolutionColor);
		}
	}

	U32 _viewSafeMargin = 0;
	if(version > 27)
	{
		_viewSafeMargin = GameControl::Read32(src);
	}


	if(version > 28)
	{
		GameControl::Read(src, &bFlipPocketPCScreen, sizeof(Uint8), 1);
	}
	else
	{
		bFlipPocketPCScreen = false;
	}

	if(version > 41)
	{
		GameControl::Read(src, &bUseMotionCorrection, sizeof(Uint8), 1);
	}
	else
	{
		bUseMotionCorrection = false;
	}

	if(version > 31)
	{
		GameControl::Read(src, &bSaveCache, sizeof(Uint8), 1);
	}
	else
	{
		bSaveCache = false;
	}

	//Read icon path
	gedString _iconPath;
	_iconPath.Read(src);
	
	//Read game title
	gedString _gameTitle;
	_gameTitle.Read(src);

	if(version > 34)
	{
		int xAxis, yAxis;
		double scale;

		xAxis = GameControl::Read32(src);
		yAxis = GameControl::Read32(src);

		GameControl::Read(src, &scale, sizeof(double), 1);

#if !defined(STAND_ALONE_GAME)
		
		axis->SetScale(scale);
		axis->SetPos(xAxis, yAxis);
#endif
	}
	
	if(!bMergeGames)
	{
		SetGameProperties(_resX, _resY, _fps, _bFullScreen, _audioSamplerRate, _bStereo, _maximumSounds, _bShowMouse, _iconPath, _gameTitle, false, _backgroundColor, _bSuspendGameIfLostFocus, _viewSafeMargin, _bUseESCKeyToExit);
	}

	} 
	GED_CATCH
	{
		ClearGame();	
	}	

	if(*SDL_GetError() != 0)
	{
		NewGame();

		#ifndef STAND_ALONE_GAME
		if(bShowErrorMessage) new PanelInfo(GetErrorMsg());
		#endif //#ifndef STAND_ALONE_GAME

		return false;
	}
	else if(!fingerPrintError.empty())
	{
		NewGame();

		#ifndef STAND_ALONE_GAME
		if(bShowErrorMessage) new PanelInfo(fingerPrintError, "Error", ALIGN_LEFT);
		#endif //#ifndef STAND_ALONE_GAME

		return false;
	}


	
#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	NANOEND
#endif

	//Update lastick after long operation
	lastTick = SDL_GetTicks();

	return true;
}

bool GameControl::LoadV8(SDL_RWops *src, bool bLoadCursor, bool bMergeGames, Uint32 version)
{
	GED_TRY
	{
#ifndef STAND_ALONE_GAME
	WaitCursor wait(bLoadCursor);
#endif //#ifndef STAND_ALONE_GAME

	if(!bMergeGames) ClearGame();

	//Load paths
	Uint16 nPaths = GameControl::Read16(src);

	int i;
	for(i = 0; i < nPaths; i++)
	{
		Path *path = new Path();
		path->Load(src);
		paths.Add(path->getName(), path);
	}

	//Load expressions
	Uint16 nExpressions = GameControl::Read16(src);
	
	for(i = 0; i < nExpressions; i++)
	{
		Script *pScript = new Script();
		pScript->Load(src, version);
		expressions.Add(pScript->getFunctionName(), pScript);

#ifndef STAND_ALONE_GAME
		bUpdateScriptPanel = true;
#endif
	}

	//Load global vars
	Uint16 nVars = GameControl::Read16(src);

	for(i = 0; i < nVars; i++)
	{
		//Load name
		gedString name;
		name.Read(src);

		//Read vars
		if(version > 1)
		{
			Uint8 type;
			GameControl::Read(src, &type, sizeof(type), 1);
			CreateUserVar(name, type, 0, "");
		}
		else
		{
			double value;
			GameControl::Read(src, &value, sizeof(value), 1);
			CreateUserVar(name, REAL_VAR, 0, "");
		}
	}

	//Load actions
	Action::LoadActions(src);

	//Load timers
	Uint16 nTimers = GameControl::Read16(src);

	for(i = 0; i < nTimers; i++)
	{
		//Load name
		gedString name;
		name.Read(src);

		//Read duration
		stTimerDefinition timer;

		timer.interval = GameControl::Read16(src);
		timer.minRandomInterval = 0;
		timer.count = REPEATE_FOREVER;
		timer.type = PERIODIC_TIMER;

		AddTimerDefinition(name, timer.interval, timer.count, timer.type, timer.minRandomInterval); 
	}

	//Load actors
	Uint16 nActors = GameControl::Read16(src);

	for(i = 0; i < nActors; i++)
	{
		Actor::NewActor(src, bMergeGames, version);
	}

	//Load game properties
	bool _bStereo, _bFullScreen, _bShowMouse; 
	U16 _resX = GameControl::Read16(src);
	U16 _resY = GameControl::Read16(src);
	GameControl::Read(src, &_bFullScreen, sizeof(Uint8), 1);
	U16 _fps = GameControl::Read16(src);
	U16 _audioSamplerRate = GameControl::Read16(src);
	GameControl::Read(src, &_bStereo, sizeof(Uint8), 1);
	U16 _maximumSounds = GameControl::Read16(src);
	GameControl::Read(src, &_bShowMouse, sizeof(Uint8), 1);

	//Load grid settings
	bool bGridShow, bGridSnap;
	Uint16 gridx, gridy;

	GameControl::Read(src, &bGridShow, sizeof(Uint8), 1);
	GameControl::Read(src, &bGridSnap, sizeof(Uint8), 1);
	gridx = GameControl::Read16(src);
	gridy = GameControl::Read16(src);

	if(!bMergeGames)
	{
		GetAxis()->setGridShow(bGridShow);
		GetAxis()->setGridSnap(bGridSnap);
		GetAxis()->setGridX(gridx);
		GetAxis()->setGridY(gridy);
	}

	gedString _iconPath, _gameTitle;
	if(version < 5)
	{
		//Final version 4 expansion indicator
		Uint8 expansion = 0;
		GameControl::Read(src, &expansion, sizeof(expansion), 1);
	}
	else
	{
		//Read icon path
		_iconPath.Read(src);

		//Read game title
		_gameTitle.Read(src);		

		//Final expansion indicator
		Uint8 expansion = 0;
		GameControl::Read(src, &expansion, sizeof(expansion), 1);
	}

	

	if(!bMergeGames)
	{
		backgroundColor.all = 0;
		SetGameProperties(_resX, _resY, _fps, _bFullScreen, _audioSamplerRate, _bStereo, _maximumSounds, _bShowMouse, _iconPath, _gameTitle, false, backgroundColor, true, 0, true);
	}

	} 
	GED_CATCH
	{
		ClearGame();	
	}	

	
	if(*SDL_GetError() != 0)
	{
		NewGame();

		#ifndef STAND_ALONE_GAME
		new PanelInfo(GetErrorMsg());
		#endif //#ifndef STAND_ALONE_GAME

		return false;
	}

	//Parser expressions	
	MapExpressionIterator itExpression(expressions);	
	for(itExpression.Begin(); !itExpression.Done(); itExpression.Next())
	{
		(*itExpression.Value())->Parser(false);
	}

	return true;
}

Uint32 GameControl::GetActionIndex(const gedString& actionName)
{
	Uint32 *index = actionIndex[actionName];
	if(!index) return 0;

	return *index;
}

Uint32 GameControl::GetScriptIndex(const gedString& scriptName)
{
	Uint32 *index = scriptIndex[scriptName];
	if(!index) return 0;

	return *index;
}

Uint32 GameControl::GetActorIndex(const char *actorName, bool bExplicitlyCreateActor)
{
	//Allow recreate destroyed actors only in execCreateActor

	stActorState *actorState = actorIndex.FindString(actorName);
	if(!actorState || (actorState->bDestroyed && !bExplicitlyCreateActor)) 
		return 0;

	return actorState->index;
}

void GameControl::SetDestroyedActor(Actor *deletedActor)
{
	stActorState *actorState = actorIndex[deletedActor->getCloneName()];
	if(actorState)
	{
		actorState->bDestroyed = true;

		//The clone is the only player?
		//Solve the bug in DodgeBall\db_s_01.exe (always recreate the player after destroyed in stand alone game)
		Actor **actor = mapRunningActor[deletedActor->getActorName()];
		if(actor && *actor) 
		{
			if(*actor == deletedActor)
			{
				actorState = actorIndex[deletedActor->getActorName()];
				if(actorState)
				{
					//Set the actorName destroyed too					
					actorState->bDestroyed = true;
				}
			}
		}
	}
}

/*void GameControl::SetDestroyedActor(const gedString& actorName)
{
	SetDestroyedActor(actorName.getCharBuf());
}*/

Path *GameControl::LoadPath(const gedString& name)
{
	Uint32 *index = pathIndex[name];
	if(!index || !indexedGame) return NULL;

	//Move file pointer
	Uint32 pos = SDL_RWtell( indexedGame ); //Save actual position
	SDL_RWseek( indexedGame, *index, SEEK_SET );

	//Load
	Path *path = new Path();
	path->Load(indexedGame);
	paths.Add(path->getName(), path);

	//Restore file pointer
	SDL_RWseek( indexedGame, pos, SEEK_SET );

	return path;
}

bool GameControl::UpdateLoadedActors(SDL_RWops *src, bool bClose)
{
	///////////////////////////////////////////////////////
	//Load indexed paths, scripts and actors (actors loads actions, actions loads scripts)
	//In editor mode only
	if(!bStandAloneMode)
	{
		int baseZDepth = Actor::getBaseZDpth();

		GED_TRY
		{
			//Load paths
			MapGameFileIndexIterator itPath(pathIndex);
			for(itPath.Begin(); !itPath.Done(); itPath.Next())
			{
				if(!paths[*itPath.Key()])
				{
					LoadPath(*itPath.Key());		
				}
			}

			//Load actors
			MapActorFileIndexIterator itActor(actorIndex);
			for(itActor.Begin(); !itActor.Done(); itActor.Next())
			{
				GetActor(*itActor.Key());	
			}
			
			//Update old versions
			if(indexedGameVersion <= 8)
			{
				//Re parser scripts, now with all actors (may be got a error before)
				MapGameFileIndexIterator itScript(scriptIndex);
				for(itScript.Begin(); !itScript.Done(); itScript.Next())
				{
					Script **p = expressions[*itScript.Key()], *pScript;
					if(p)
					{
						pScript = *p;
						if(!pScript->getParserOk()) 
							pScript->Parser(true);	
					}
				}
			}
			
		} 
		GED_CATCH
		{
			ClearGame();	
		}
		
		if(bClose) SDL_RWclose(src);
		indexedGame = NULL;

		if(*SDL_GetError() != 0)
		{
			return false;
		}

		Actor::setBaseZDpth(baseZDepth);

		//Don't erase in stand alone mode
		mapActorAnimation.Clear();
	}
	///////////////////////////////////////////////////////

	return true;
}

bool GameControl::Load(const gedString& gameName, bool bLoadCursor, bool bMergeGames, bool bShowErrorMessage)
{
	SDL_ClearError();
	tmpGamePath.clear();

	gedString aux(gameName);

#ifdef DEBUG
	SDL_RWops *src = ged_SDL_RWFromFile(aux.c_str(), "r+b"); //For fingerprint remotion in RemoveFingerPrint
	if(!src)
	{
		src = ged_SDL_RWFromFile(aux.c_str(), "rb");
		bCanRemoveFingerPrint = false;
	}
	else
	{
		bCanRemoveFingerPrint = true;
	}
#else
	SDL_RWops *src = ged_SDL_RWFromFile(aux.c_str(), "rb");
#endif

	if(!src)
	{
		return false;
	}

	char bufPath[PATH_LIMIT];
	ged_getcwd(bufPath, PATH_LIMIT);
	tmpGamePath = bufPath;

	mapActorAnimation.Clear();
	bool bRes = Load(src, bLoadCursor, bMergeGames, 0, bShowErrorMessage);

	if(bRes)
	{
		if(!UpdateLoadedActors(src, true)) 
		{
			if(!bMergeGames) NewGame();

			#ifndef STAND_ALONE_GAME
			if(bShowErrorMessage) new PanelInfo(GetErrorMsg());
			#endif //#ifndef STAND_ALONE_GAME

			tmpGamePath.clear();
			return false;
		}
	}

	if(bRes && !bMergeGames)
	{
		if(gameName.substr(0, 7) != "gedTmp_") this->gameName = gameName;
		else this->gameName = gameName.substr(8);

		gamePath = bufPath;

#ifndef STAND_ALONE_GAME
		SetCaption((sGameEditorCaption + gameName).c_str(), NULL);
#endif
	}

	tmpGamePath.clear();
	return bRes;
}


void GameControl::NewGame(bool bResetMainPanel)
{
	ClearGame();
	lastGameFileVersion = 0;

	//Solve crash when load a game saved in a demo version in other machine,
	//and try add an actor with same name in the not loaded game
	indexedGame = NULL; 

	new ActorEdit(VIEW_NAME, NULL, true);
	if(bResetMainPanel) if(mainActor) mainActor->SetPos(0, 0);
}


void GameControl::ClearGame()
{
	ReleaseMemory(256*1024*1024);

#ifndef STAND_ALONE_GAME
	ActorEdit::DoActivationEvent(false);
	RemoveAllPanelActors();
	if(!bFastFile) gameID = GenerateUniqueID();
#endif

	if(!bFastFile)
	{
		gameName = "";
		oldGamePath = gamePath = "";
		gameTitle = "";
		bUseGameEditorServer = true;
		bAutoStartNetwork = true;
		gameNetworkPort = DEFAULT_GAME_PORT;
		connectionLostTimeOut = DEFAULT_CONNECTION_LOST_TIMEOUT;
		gameVersion = 0;
		bModified = false;  //Odilon
	}

	iconPath = "";		
	bFlipPocketPCScreen = false;
	bUseMotionCorrection = false;
	bSaveCache = false;	
	Actor::setBaseZDpth(0);

	
	

	//Delete paths
	if(!paths.empty())
	{
		MapPathIterator it(paths);
		it.Begin();
		while(!it.Done())
		{
			Path *path = *it.Value();
			delete path;
			paths.Remove(*it.Key());				

			it.Begin();
		}
	}


	//Delete actors (too slow...)
	bDestroingGame = true;

	MapActorIterator it(mapActors);

	//for(int i = 0; i < 3; i++) //solve the LoadGame crash in 1945.ged (solved with two iterations)
	bool bRemoved;

	do
	{
		bRemoved = false;
	
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(IS_VALID_ACTOR(actor))
				{
					if(actor->EditMode() || actor->getRunning())
					{
						delete actor;
						//it.Begin();
						bRemoved = true;
						break;
					}
				}
				else
				{
					(*it.Value()).FindAndDelete(actor);
					if((*it.Value()).size() == 0)
					{
						mapActors.Remove(*it.Key()); //Release the memory too
					}

					bRemoved = true;
					break;
				}
			}

			if(bRemoved) break;
		}
	} while(bRemoved);

	bDestroingGame = false;

	//Remove symbols created in actorIndex load 
	//Solve the crash when load Issue088.ged without data files, and load again with data files
	MapActorFileIndexIterator itActor(actorIndex);
	for(itActor.Begin(); !itActor.Done(); itActor.Next())
	{
		gedString actorName(itActor.Key()->substr(0, itActor.Key()->find('.')));
		if(isSymbolInEic(actorName.c_str()))
		{
			Script::RemoveSymbol(actorName.c_str());
		}
	}

	//Delete expressions
	if(!expressions.empty())
	{
		MapExpressionIterator it(expressions);
		it.Begin();
		while(!it.Done())
		{
			Script *exp = *it.Value();
			delete exp;
			expressions.Remove(*it.Key());	
#ifndef STAND_ALONE_GAME
			bUpdateScriptPanel = true;
#endif

			it.Begin();
		}
	}


	

	//Delete sounds
	MapSoundIterator itSound(mapSound);	
	if(bAudioOpened)
	{
		for(itSound.Begin(); !itSound.Done(); itSound.Next())
		{
			Mix_FreeChunk(*itSound.Value());
		}
	}
	mapSound.Clear();

	//Delete music
	if(music)
	{
		if(bAudioOpened) Mix_FreeMusic(music);
		music = NULL;
	}
	
	//Delete global user vars
	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		Script::RemoveSymbol(itVar.Key()->c_str());
	}

	globalUserVars.Clear();

	//Delete timers
	mapTimerNameDefinition.Clear();

	RegionLoad::DestroyRegions();
	ActivationEventsCanvas::Clear();
	Script::DestroyLocalUserVars();
	GenericScript::ClearGenericScripts();
	
	InitPocketPCKeys();

#ifndef STAND_ALONE_GAME
	if(!bFastFile)
	{
		SetCaption((sGameEditorCaption + "Untitled").c_str(), NULL);
	}
#endif

#ifndef STAND_ALONE_GAME
	ActorText::DestroyTextCache();
#endif

	Script::Reset();

#ifndef STAND_ALONE_GAME
	ClearGameGraph();
#endif

	GLASSERT(Action::getGlobalMapActions().size() == 0);
}




bool GameControl::CreateUserVar(gedString varName, Uint8 _type, int _arrayLenght, gedString _saveGroup)
{
	if(varName == S_VAR_X || 
		varName == S_VAR_Y ||
		varName == S_VAR_XPREVIOUS ||
		varName == S_VAR_YPREVIOUS ||
		varName == S_VAR_WIDTH ||
		varName == S_VAR_HEIGHT ||
		varName == S_VAR_R ||
		varName == S_VAR_G ||
		varName == S_VAR_B ||
		varName == S_VAR_TRANSP ||
		varName == S_VAR_PATHPOS ||
		varName == S_VAR_ANIMPOS ||
		varName == S_VAR_ANIMINDEX ||
		varName == S_VAR_NFRAMES ||
		varName == S_VAR_ANGLE ||
		varName == S_VAR_DIRETIONAL_VELOCITY ||
		varName == S_VAR_XVELOCITY ||
		varName == S_VAR_YVELOCITY ||
		varName == S_VAR_XSCREEN ||
		varName == S_VAR_YSCREEN ||
		varName == S_VAR_TEXT ||
		varName == S_VAR_FRAMES ||
		varName == S_VAR_XMOUSE ||
		varName == S_VAR_YMOUSE ||
		varName == S_VAR_MUSIC_VOLUME)
	{
		return false;
	}

	if(varName.empty())
	{
		return false;
	}
	
	EiC_interpON = 1;
	
	long addr = 0, size = 0;
	EiC_parseString("long _gedAddr @ %ld;\
					 long _gedSize @ %ld;", (long)&addr, (long)&size);
	
	if(_type == INTEGER_VAR)
	{
		if(_arrayLenght > 0)
		{
			EiC_parseString("long %s[%ld];", varName.c_str(), _arrayLenght);
		}
		else
		{
			EiC_parseString("long %s;", varName.c_str());
		}
	}
	else if(_type == REAL_VAR)
	{
		if(_arrayLenght > 0)
		{
			EiC_parseString("double %s[%ld];", varName.c_str(), _arrayLenght);
		}
		else
		{
			EiC_parseString("double %s;", varName.c_str());
		}
	}
	else if(_type == STRING_VAR)
	{
		if(_arrayLenght > 0)
		{
			EiC_parseString("char %s[%ld][MAX_SCRIPTTEXT];", varName.c_str(), _arrayLenght);
		}
		else
		{
			EiC_parseString("char %s[MAX_SCRIPTTEXT];", varName.c_str());
		}
		
	}
	else
	{
		Script::RemoveSymbol("_gedAddr");
		Script::RemoveSymbol("_gedSize");
		EiC_interpON = 0;
		return false;
	}

	EiC_parseString("_gedAddr = (long)&%s; _gedSize = sizeof(%s);", varName.c_str(), varName.c_str(), varName.c_str());

	Script::RemoveSymbol("_gedAddr");
	Script::RemoveSymbol("_gedSize");
	EiC_interpON = 0;

	globalUserVars.Add(varName, stVarInfo(varName, _type, _arrayLenght, _saveGroup, (void *)addr, size) );

	return true;
}

void GameControl::ResetVars()
{
	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		memset(itVar.Value()->addr, 0, itVar.Value()->size);
	}
}

void stVarInfo::Load(SDL_RWops* src)
{
	name.Read(src, GameControl::Get()->getGameMode());
	GameControl::Read(src, &type, sizeof(type), 1);
	arrayLenght = GameControl::Read32(src);
	size = GameControl::Read32(src);
	saveGroup.Read(src, GameControl::Get()->getGameMode());
}

void stVarInfo::Save(SDL_RWops* src)
{
	name.Write(src);
	SDL_RWwrite(src, &type, sizeof(type), 1);
	SDL_WriteLE32(src, arrayLenght);
	SDL_WriteLE32(src, size);
	saveGroup.Write(src);
}

void stVarInfo::LoadValue(SDL_RWops* src)
{
	//Load var to script memory

	if(!addr)
	{
		addr = malloc(size);
		if(addr) bInternalAddr = true;
	}

	if(addr && size)
	{
		GameControl::Read(src, addr, size, 1);
	}
}

void stVarInfo::SaveValue(SDL_RWops* src)
{
	//Save var from script memory

	if(addr && size)
	{
		SDL_RWwrite(src, addr, size, 1);
	}
}

extern "C" const char *getSavePath(const char *filename)
{
	static gedString filePath;

	filePath.clear();

#ifdef STAND_ALONE_GAME
	filePath = GameControl::Get()->getHomePath();
#else
	if(!GameControl::Get()->getGamePath().empty())
	{
		filePath += GameControl::Get()->getGamePath();
	}
	else
	{
		filePath += GameControl::Get()->getHomePath();
	}
#endif

	filePath += gedString(DIR_SEP) + gedString(filename).GetFileName();

	return filePath.c_str();
}

void GameControl::LoadVars(char *file, char *group)
{
	SDL_ClearError();
	gedString::SetReadEncriptString(true);

	gedString filePath(getSavePath(file));

	U32 fileCrc = FileCrc32(filePath.c_str(), 8);

	SDL_RWops* src = ged_SDL_RWFromFile(filePath.c_str(), "rb");
	if(!src) return;

	//Load file version
	Uint32 version = GameControl::Read32(src);
	if(version != GAME_FILE_VERSION)
	{
		SDL_RWclose(src);
		return;
	}

	//Load crc
	Uint32 crc = GameControl::Read32(src);
	if(crc != fileCrc)
	{
		SDL_RWclose(src);
		return;
	}


	Uint16 nVars = GameControl::Read16(src);

	//Load only the vars in this ged for this group
	for(int i = 0; i < nVars; i++)
	{
		stVarInfo var, *pVar;
		var.Load(src);
		int dataSize = GameControl::Read32(src);

		if( (pVar = globalUserVars[var.name]) != NULL && 
			*pVar == var &&
			pVar->saveGroup == group)
		{
			pVar->LoadValue(src);
		}
		else
		{
			//Skip value
			SDL_RWseek(src, dataSize, SEEK_CUR);
		}
	}

	SDL_RWclose(src);
}

void GameControl::SaveVars(char *file, char *group)
{
	SDL_ClearError();

	gedString filePath(getSavePath(file));

	MapVars	savedVars;

	U32 fileCrc = FileCrc32(filePath.c_str(), 8);

	//Load the actual saved vars
	SDL_RWops* src = ged_SDL_RWFromFile(filePath.c_str(), "rb");
	if(src)
	{		
		//Load file version
		Uint32 version = GameControl::Read32(src);
		if(version == GAME_FILE_VERSION)
		{
			Uint32 crc = GameControl::Read32(src);
			if(crc == fileCrc)
			{				
				Uint16 nVars = GameControl::Read16(src);
				
				//Load all vars
				for(int i = 0; i < nVars; i++)
				{
					stVarInfo var;
					var.Load(src);
					/*int dataSize =*/ GameControl::Read32(src);
					
					var.LoadValue(src);
					
					savedVars.Add(var.name, var);
				}
			}
		}
		
		SDL_RWclose(src);
	}

	//Update game vars
	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		if(itVar.Value()->saveGroup == group)
		{
			stVarInfo *pSaveVar;
			if((pSaveVar = savedVars[itVar.Value()->name]) != NULL)
			{
				//Update the existing var
				*pSaveVar = *itVar.Value();
			}
			else
			{
				//Add new var
				savedVars.Add(itVar.Value()->name, *itVar.Value());
			}	
		}
	}

	//Save
	U32 crc = 0;
	if(savedVars.size())
	{
		src = ged_SDL_RWFromFile(filePath.c_str(), "wb");
		if(!src) return;
		
		//Save file version
		SDL_WriteLE32(src, GAME_FILE_VERSION);

		//Reserve crc space
		SDL_WriteLE32(src, crc);
		
		
		//Save n vars
		SDL_WriteLE16(src, savedVars.size());
		
		//Save vars
		MapVarsIterator it(savedVars);	
		for(it.Begin(); !it.Done(); it.Next())
		{
			//Save var info
			it.Value()->Save(src);
			
			//Save value
			SDL_WriteLE32(src, it.Value()->size);
			it.Value()->SaveValue(src);
		}
				
		SDL_RWclose(src);

		//Get CRC
		crc = FileCrc32(filePath.c_str(), 8);

		//Save CRC
		src = ged_SDL_RWFromFile(filePath.c_str(), "r+b");
		if(!src) return;

		SDL_RWseek( src, 4, SEEK_SET );
		SDL_WriteLE32(src, crc);

		SDL_RWclose(src);
	}
}

gedString GameControl::AddExpression(Script *pScript)
{
	expressions.Add(pScript->getFunctionName(), pScript);
#ifndef STAND_ALONE_GAME
	bUpdateScriptPanel = true;
#endif
	return pScript->getFunctionName();
}

Script *GameControl::GetExpression(const char *expressionName)
{
	Script **pScript = expressions.FindString(expressionName);
	if(pScript)
	{
		return *pScript; 
	}
	else 
	{
		//Backward compatibility
		pScript = expressions[gedString("ex")+expressionName]; 
		if(pScript)
		{
			return *pScript; 
		}
	}

	return NULL;
}

void GameControl::RemoveExpression(const gedString& expressionName)
{
	Script **p = expressions[expressionName], *pScript;
	if(p)
	{
		 pScript = *p; 
		 delete pScript;

		 expressions.Remove(expressionName);
#ifndef STAND_ALONE_GAME
		bUpdateScriptPanel = true;
#endif
	}
}

class stMusicInfo
{
public:
	gedString path;
	int volume;
	int loop;
	int priority;
};

SDL_mutex *musicMutEx = SDL_CreateMutex();
int GameControl::LoadMusicThread( void *pParam )
{
#ifndef linux
	MuteEx mutex(musicMutEx);

	stMusicInfo *info = (stMusicInfo *)pParam;
	Mix_Music *music = GameControl::Get()->music;

	if(info->priority == LOW_PRIORITY_MUSIC)
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

	if(music) Mix_FreeMusic(music);

	music = ged_Mix_LoadMUS(info->path.c_str());
	GameControl::Get()->music = music;

	Mix_PlayMusic(music, info->loop-1);
	Mix_VolumeMusic(info->volume);		

	delete info;
#endif

    return 0;
}

int GameControl::PlaySound(bool bMusic, const gedString& path, int volume, int loop, int priority, Sint16 pan)
{
	/*
	Channel table:
		0:	error
		1:	music
		>=2:sounds
	*/


	if(!bAudioOpened) return 0;

#ifdef DEBUG
		if(bMusic) GLOUTPUT("PlayMusic: %s\n", path.getCharBuf());
		else GLOUTPUT("PlaySound: %s\n", path.getCharBuf());
#endif

	int channel = 0;

	if(loop <= 0) loop = 1000000; //infinity!

	if(bMusic)
	{
#ifndef linux
		if(priority == MEDIUM_PRIORITY_MUSIC || priority == LOW_PRIORITY_MUSIC)
		{
			//Load music in other thread
			stMusicInfo *info = new stMusicInfo;
			info->path = path;
			info->volume = volume;
			info->loop = loop;
			info->priority = priority;
			
			SDL_CreateThread(LoadMusicThread, info);

			channel = 1;
		}
		else
#endif
		{			
			//Without thread
			if(music) Mix_FreeMusic(music);
			music = NULL;
			
			gedString aux(path);
			music = ged_Mix_LoadMUS(aux.c_str());
			if(music)
			{
				Mix_PlayMusic(music, loop-1);
				Mix_VolumeMusic(volume);

				channel = 1;
			}
		}
	}
	else
	{
		Mix_Chunk *sound = NULL, **p = mapSound[path];
		if(p)
		{
			sound = *p;			
		}
		else
		{
			gedString aux(path);
			sound = ged_Mix_LoadWAV(aux.c_str());
			if(sound) mapSound.Add(path, sound);
		}

		if(sound)
		{
			Mix_VolumeChunk(sound, volume);
			int ch = Mix_PlayChannel(-1, sound, loop-1);
			if(ch >= 0) 
			{
				Mix_SetPanning(ch, (pan <= 0)?255:(255-pan), (pan < 0)?(255+pan):255);
				channel = ch + 2;
			}
		}
	}

	return channel;
}

void GameControl::Delete()
{
#ifndef STAND_ALONE_GAME
	ActorText::DestroyTextCache();
#endif

	DestroyRakNet();

	delete gameControl;	
	gameControl = NULL;
}


void GameControl::setBackGroundColor(KrRGBA _backgroundColor)
{
	backgroundColor = _backgroundColor;
	if(engine) engine->FillBackground(&backgroundColor);
}

void GameControl::SetGameProperties(Uint16 resX, Uint16 resY, Uint16 fps, bool  bFullScreen,
		Uint16  audioSamplerRate, bool bStereo, Uint16  maximumSounds, bool bShowMouse, 
		gedString iconPath, gedString gameTitle, bool bSetViewPos, KrRGBA _backgroundColor,
		bool _bSuspendGameIfLostFocus, U32 _viewSafeMargin, bool _bUseESCKeyToExit)
{
	//Adjusts
	
	//Sound
#if defined(_WIN32_WCE) && defined(_X86_)
	audioSamplerRate = 22050; //PocketPC Emulator
#endif

#ifdef GP2X
	bShowMouse = false;
	audioSamplerRate = 22050;
#endif


	if(audioSamplerRate != this->audioSamplerRate || bStereo != this->bStereo || maximumSounds != this->maximumSounds)
	{
		if(bAudioOpened) Mix_CloseAudio(); 
		
		//maks: pode não responder em stand alone mode 
		//A lentidão da abertura é observada usando windib. A criação da thread de áudio é lenta
		//SDL_OpenAudio -> audio->thread = SDL_CreateThread(SDL_RunAudio, audio) -> SDL_SYS_CreateThread
		if(Mix_OpenAudio(audioSamplerRate, AUDIO_S16, bStereo?2:1, 512) == 0)
		{
			bAudioOpened = true;
			Mix_AllocateChannels(maximumSounds);
		}
		else bAudioOpened = false;
			
		SDL_ClearError(); //Ignore sound errors
	}


	this->resX = resX;
	this->resY = resY;
	this->fps  = fps;
	this->bFullScreen = bFullScreen;
	this->audioSamplerRate = audioSamplerRate;
	this->bStereo = bStereo;
	this->maximumSounds = maximumSounds;
	this->bShowMouse = bShowMouse;
	this->bSuspendGameIfLostFocus = _bSuspendGameIfLostFocus;
	this->bUseESCKeyToExit = _bUseESCKeyToExit;
	this->iconPath = iconPath;
	this->gameTitle = gameTitle;
	this->viewSafeMargin = _viewSafeMargin;

	setBackGroundColor(_backgroundColor);	

	//Axis
	if(axis) axis->GameResolution(resX, resY);

#ifndef STAND_ALONE_GAME //maks: Only create the view after game mode to make sure execute OnCreate action
	//View
	if(gameControl)
	{
		Actor *view = GetActor(VIEW_NAME);
		if(view) 
		{
			view->AdjustView(resX, resY, bSetViewPos);			
		}
	}
#endif
}

void GameControl::SetViewActor(Actor *view)
{
	viewActor = view;	
}

void GameControl::SetTimer(int Fps)
{
	if(!Fps) Fps = fps;

	//Fps *= 1.1; //Adjust factor (need this?)

	frameTimeInterval = 1000 / Fps;
	if(frameTimeInterval < 2) frameTimeInterval = 2; //max 500 fps

	

#ifdef APPLICATION_THREAD_TIMERS
	SDL_KillThread(timerThread);
	timerThread = SDL_CreateThread(TimerCallback, NULL);
#else
	if(timerId) SDL_RemoveTimer(timerId);
	timerId = SDL_AddTimer(frameTimeInterval, TimerCallback, NULL);
#endif
}

bool GameControl::SwitchResolution(SDL_Surface* screen, int width, int height, bool bFullScreen)
{
#ifdef RENDER_THREAD
	MuteEx mutex(renderMutEx);
#endif

#ifdef __SYMBIAN32__ //Dont change resolution
	return;
#endif

	Uint32 flags = VIDEO_FLAGS;


#if defined(_WIN32_WCE)
	bFullScreen = true;
	char *sDevice = "device";
#else
	char *sDevice = "machine";
#endif

#if !defined(_DEBUG) || defined(_WIN32_WCE)
	if(bFullScreen) flags |= SDL_FULLSCREEN;
#endif

	static bool lastMode = false, lastFlipPocketPCScreen = bFlipPocketPCScreen;

	if(!bGameMode && (flags & SDL_OPENGL))
	{
		//Only in game mode
		flags &= ~SDL_OPENGL;
	}

	if(bGameMode
#ifndef STAND_ALONE_GAME
		|| !Tutorial::IsOff() //Don't resize window when tutorial is playing
#endif
		) 
		flags &= ~SDL_RESIZABLE;

#if defined(WIN32) && !defined(_WIN32_WCE)
	if(flags & SDL_FULLSCREEN)
	{
		//Use shadow with directx to avoid flickering
		flags |= SDL_DOUBLEBUF | SDL_CREATE_SHADOW;
	}
#endif

	bool bSwitch = width != screen->w || 
		height != screen->h ||
		(bFullScreen && !(screen->flags & SDL_FULLSCREEN)) ||
		(!bFullScreen && (screen->flags & SDL_FULLSCREEN)) ||
#ifndef STAND_ALONE_GAME
		((screen->flags & SDL_RESIZABLE) != (flags & SDL_RESIZABLE)) ||		
#endif
		lastFlipPocketPCScreen != bFlipPocketPCScreen ||
		bGameMode != lastMode;

#if !defined(STAND_ALONE_GAME) && defined(WIN32)
	if(bGameMode != lastMode && (bFullScreen || SDL_Window != GetSDLPanelWindow()) && SDL_Window != GetMainWindow()) 
	{
		//Need use a different window in full screen game mode	when use wx
		SDL_VideoQuit();

		char windowID[64];
		sprintf(windowID, "SDL_WINDOWID="); //Create a new top level window
		
		if(!bGameMode)
		{
			//Use the current window
			void *window = GetSDLPanelWindow();
			if(window)
			{
				sprintf(windowID, "SDL_WINDOWID=%ld", (unsigned long)window);
			}
		}

		putenv(windowID);		
		SDL_VideoInit(NULL, 0);
	}
#endif

	if(bSwitch)
	{
#ifdef USE_COMPILED_SPRITES
		screen = SetVideoMode(width, height, 16, flags);
#else
		screen = SetVideoMode(width, height, 0, flags); 
#endif

		if(!screen ||
			screen->w != width ||
			screen->h != height
			) 
		{
#ifdef STAND_ALONE_GAME
			SDL_SetError("This game requires a screen resolution (%dx%d).\nPlease, change the %s resolution or contact the game vendor.", width, height, sDevice);
#endif
			return false;
		}
		
		KrRect bounds;
		bounds.Set( 0, 0, screen->w - 1, screen->h - 1 );
		engine->Init(screen, &bounds, 0);
		
		GetAxis()->ScreenResolution(screen->w, screen->h);
		engine->FillBackground(&backgroundColor);
		engine->InvalidateScreen();
	}

	lastMode = bGameMode;
	lastFlipPocketPCScreen = bFlipPocketPCScreen;

#if defined(WIN32) && !defined(_WIN32_WCE)
	if(bGameMode && !bFullScreen)
	{
		//Avoid hop when click in the windows title bar or alternate windows
		ShowWindow(SDL_Window, SW_RESTORE);

		//Retore position from the last game execution
		if(rectGameWindow.left && rectGameWindow.top) 
			MoveWindow(SDL_Window, rectGameWindow.left, rectGameWindow.top, screen->w, screen->h, TRUE);
	}
#endif

	return true;
}


void GameControl::CheckSurfaceLost()
{
	SDL_Surface* screen = engine->Surface();
	if(SDL_SurfaceLost() && screen)
	{
		U32 flags = screen->flags;
		int w = screen->w, h = screen->h, bpp = 0;

#if defined(WIN32) && !defined(_WIN32_WCE)
	if(GetMainWindow() != GetForegroundWindow())
	{
		//Without this don't allow the user alternate windows
		return;
	}

	if(flags & SDL_FULLSCREEN)
	{
		flags |= SDL_DOUBLEBUF | SDL_CREATE_SHADOW;
	}

		BOOL bMaximize = IsZoomed((HWND)GetMainWindow()) || IsIconic((HWND)GetMainWindow());
#endif

#ifdef USE_COMPILED_SPRITES
		bpp = 16;
#endif

#ifdef DEBUG
		GLOUTPUT("Surface Lost\n");
#endif

		
		
		screen = SetVideoMode(w, h, bpp, flags);
		if(!screen)
		{
			//Lost the hardware, try again
			SDL_VideoQuit();
			SDL_VideoInit(NULL, 0);

			
			screen = SetVideoMode(w, h, bpp, flags);
			
#if defined(WIN32) && !defined(_WIN32_WCE)
			//Set Window icon
			HICON hIcon = ::LoadIcon(SDL_Instance, MAKEINTRESOURCE(IDI_ICON1));
			::SendMessage(SDL_Window, WM_SETICON, 1, (LPARAM)hIcon); //big
			::SendMessage(SDL_Window, WM_SETICON, 0, (LPARAM)hIcon); //small

			if(bMaximize)
			{
				ShowWindow((HWND)GetMainWindow(), SW_MAXIMIZE);
			}
#endif
		}
		
		
		KrRect bounds;
		bounds.Set( 0, 0, screen->w - 1, screen->h - 1 );
		engine->Init(screen, &bounds, 0);
		
		GetAxis()->ScreenResolution(screen->w, screen->h);
		engine->FillBackground(&backgroundColor);
		engine->InvalidateScreen();

		SDL_ClearError();
	}
}

char *GameControl::GetErrorMsg()
{
	return SDL_GetError();
}

bool GameControl::Write(SDL_RWops *context, const void *ptr, int size, int num)
{
	bool res = SDL_RWwrite(context, ptr, size, num) == num;
	
#ifndef STAND_ALONE_GAME
	if(*SDL_GetError() != 0)
	{
		GED_THROW
			return false;
	}
#endif
	

	return res;
}

bool GameControl::Read(SDL_RWops *context, void *ptr, int size, int num)
{
	bool res = SDL_RWread(context, ptr, size, num) == num;

#ifndef STAND_ALONE_GAME

#ifdef _DEBUG
	char *error = SDL_GetError();
#endif

	if(*SDL_GetError() != 0)
	{
		GED_THROW
	}
#endif

	return res;
}

Uint16 GameControl::Read16(SDL_RWops *context)
{
	Uint16 res = SDL_ReadLE16(context);

#ifndef STAND_ALONE_GAME

#ifdef _DEBUG
	char *error = SDL_GetError();
#endif

	if(*SDL_GetError() != 0)
	{
		GED_THROW
	}
#endif

	return res;
}

Uint32 GameControl::Read32(SDL_RWops *context)
{
	Uint32 res = SDL_ReadLE32(context);

#ifndef STAND_ALONE_GAME

#ifdef _DEBUG
	char *error = SDL_GetError();
#endif

	if(*SDL_GetError() != 0)
	{
		GED_THROW
	}
#endif

	return res;
}

void GameControl::LoadGameEvent(char *gamePath)
{
	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));
	event.quit.type = SDL_LOAD_GAME__EVENT;
	SDL_PushEvent(&event);

	loadGameEvent = gamePath;
}

GameControl::~GameControl()
{
	//WriteEditorResources();
	//CreateEngines();

#ifndef STAND_ALONE_GAME
	UndoControl::Destroy();
#endif

	bGameEngineIsRunning = false;

#ifdef APPLICATION_THREAD_TIMERS
	SDL_KillThread(timerThread);
#else
	SDL_RemoveTimer(timerId);
#endif

	
	//SDL_RemoveTimer(timerRealFpsCalcId);

	if(mainActor)
		delete mainActor;

	ClearGame();	

	if(axis)
		delete axis;	

	/*if(indexedGame) //Closed in Kyra KrResourceVault::~KrResourceVault()
		SDL_RWclose(indexedGame);*/ 
}

bool GameControl::CheckStandAloneMode(gedString executableName)
{
#ifdef STAND_ALONE_GAME 
	static gedString firstLevel;
	SDL_ClearError();

	if(executableName == "gameEditor.exe" || executableName == "gameEditorPocket.exe" || executableName == "gameEditorWindows.exe") executableName = "game1"; //maks

	int i = 0;
	gedString ext, gameFile;
	if((i = executableName.rfind('.')) != gedString::npos)
	{
		ext = executableName.substr(i);
		executableName = executableName.substr(0, i);		
	}	

	executableName = homePath + DIR_SEP + executableName;		
	
	SDL_RWops* exeFile;

	//Try .dat
	gameFile = executableName + ".dat";
	exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");
	
	//Try .exe (first level)
	if(!exeFile) 
	{
		gameFile = executableName + ".exe";
		exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");		
		SDL_ClearError();
	}

	//Try .gedx (first level)
	if(!exeFile) 
	{
		gameFile = executableName + ".gedx";
		exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");		
		SDL_ClearError();
	}

	//Try .gdx (first level)
	if(!exeFile) 
	{
		gameFile = executableName + ".gdx";
		exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");		
		SDL_ClearError();
	}

	//Try without extension (Linux)
	if(!exeFile) 
	{
		gameFile = executableName;
		exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");		
		SDL_ClearError();
	}

	//Try with otiginal extension (for use custom extensions)
	if(!exeFile) 
	{
		gameFile = executableName + ext;
		exeFile = ged_SDL_RWMemoryMapFile(gameFile.c_str(), "rb");		
		SDL_ClearError();
	}

	//Try the first loaded file (may be the game are triyng reload the .exe file, but the user change the file name)
	if(!exeFile && !firstLevel.empty())
	{
		exeFile = ged_SDL_RWMemoryMapFile(firstLevel.c_str(), "rb");		
		SDL_ClearError();
	}


	if(exeFile)
	{
#ifdef DEBUG
		GLOUTPUT("----------- Loaded: %s\n", gameFile.c_str());
#endif
		lastGameFileVersion = 0; //Solve crash when export a game and this game load a old .dat

		//Is GEDX?
		long gedxOffSet = 4;
		bool bIsGedX = false;

		//Test 64KB from end of file to deal with signed executables
		for(gedxOffSet = 4; gedxOffSet < 64*1024; gedxOffSet++)
		{
			SDL_RWseek( exeFile, -gedxOffSet, SEEK_END ); 
			Uint32 magic = Read32(exeFile);
			
			if(memcmp(&magic, "GEDX", 4) == 0)
			{
				bIsGedX = true;
				break;
			}
		}

		if(!bIsGedX)
		{
			SDL_RWclose(exeFile);
			return false;
		}

		

		bStandAloneMode = true;

		//Read offsets
		SDL_RWseek( exeFile, -(12 + gedxOffSet), SEEK_END ); 

		Uint32 kyraDatPos		= Read32(exeFile);
		Uint32 audioBase		= Read32(exeFile);
		Uint32 audioIndexBase	= Read32(exeFile);

		
		//Read dat
		SDL_RWseek( exeFile, kyraDatPos, SEEK_SET );
		if(!engine->Vault()->LoadDatFile(exeFile)) 
		{
			SDL_RWclose(exeFile);
			return false;
		}

		//Read ged
		SDL_ClearError();

#ifdef DEBUG
			SDL_Delay(100); //Due to delay when debug without use directx
#endif

			
		if(Load(exeFile, false, false, SDL_RWtell( exeFile )))
		{
			

#ifdef DEBUG
			SDL_Delay(100); //Due to delay when debug without use directx
#endif
			/*if(!SetGameMode(true))
			{
				return false;
			}*/
			
			LoadIcon();			
			
			SDL_ClearError(); 
		}
		else
		{
			SDL_RWclose(exeFile);
			return false;
		}

		//Read audio index
		SDL_RWseek( exeFile, audioIndexBase, SEEK_SET );
		Uint32 nAudioFiles = Read32(exeFile);

		if(nAudioFiles)
		{	
			SDL_RWIndex *index = NULL;
			sdlAudioIndex = NULL; //Deleted in ClearMultipleArchiveFile when close file archive

			for(Uint32 i = 0; i < nAudioFiles; i++)
			{
				SDL_RWIndex *newIndex = (SDL_RWIndex *)malloc(sizeof(SDL_RWIndex));
				
				if(index) index->next = newIndex;

				index = newIndex;
				index->next = NULL;

				if(!sdlAudioIndex) sdlAudioIndex = index;

				//File name
				gedString fileName;
				ReadString(exeFile, &fileName);

				index->fileName = (char *)malloc(fileName.length() + 1);
				strcpy(index->fileName, fileName.c_str());
				
				//Offset relative to audioBase
				index->offset = Read32(exeFile);
				
				//Size
				index->size = Read32(exeFile);				
			}

			//Set SDL to multiple archive file mode
			SDL_RWSetMultipleArchiveFile(exeFile, audioBase, sdlAudioIndex);
		}	

		if(firstLevel.empty())
		{
			//Store first level name
			firstLevel = gameFile;
		}

		//Only set game mode after load the audio index, to ensure sound play in the view creation
		if(!SetGameMode(true))
		{
			return false;
		}
	}

	

#endif
	
	return true;
}

bool GameControl::ImageSnap(KrImage *image, KrImage *root, int *pMoveX, int *pMoveY)
{
	if(!bActorSnap || bGameMode || !image || !root) return false;

	int snap = image->Bounds().Width()/2 + 1;

	//For redraw and position considerations
	engine->InvalidateRectangle(image->Bounds());
	image->CalcTransform();

	//Hit test
	KrRect	imageRect = image->Bounds(),
			imageRectExpanded = imageRect;

	imageRectExpanded.EdgeAdd(snap);


	GlDynArray<KrImage*> hittest;
	engine->Tree()->HitTestIntersect(root, imageRectExpanded, KrImageTree::GET_ALL_HITS, &hittest);
	
	int n = hittest.Count();
	int dx = resX, dy = resY;

	
	for(int i = 0; i < n; i++)
	{						
		KrImage *hit = hittest.Item(i);
		if(hit && hit != image)
		{
			KrRect rect = hit->Bounds(),
				   rect1 = rect;
			
			rect1.EdgeAdd(snap);

			if(imageRectExpanded.Intersect(rect1))
			{				
				//Min dx
				int dxLeft1, dxLeft2, dxRight1, dxRight2;

				dxLeft1 = rect.xmin - imageRect.xmin;
				dxLeft2 = rect.xmax - imageRect.xmin;
				
				dxRight1 = rect.xmin - imageRect.xmax;
				dxRight2 = rect.xmax - imageRect.xmax;
				
				if(abs(dx) > abs(dxLeft1)) dx = dxLeft1;
				if(abs(dx) > abs(dxLeft2)) dx = dxLeft2 + 1;
				if(abs(dx) > abs(dxRight1)) dx = dxRight1 - 1;
				if(abs(dx) > abs(dxRight2)) dx = dxRight2;
				
				
				//Min dy		
				int dyTop1, dyTop2, dyBotton1, dyBotton2;

				dyTop1 = rect.ymin - imageRect.ymin;
				dyTop2 = rect.ymax - imageRect.ymin;
				
				dyBotton1 = rect.ymin - imageRect.ymax;
				dyBotton2 = rect.ymax - imageRect.ymax;
				
				if(abs(dy) > abs(dyTop1)) dy = dyTop1;
				if(abs(dy) > abs(dyTop2)) dy = dyTop2 + 1;
				if(abs(dy) > abs(dyBotton1)) dy = dyBotton1 -1;
				if(abs(dy) > abs(dyBotton2)) dy = dyBotton2;
			}
		}
	}

	//Move
	int moveX = 0, moveY = 0;

	if(abs(dx) < snap) moveX = (int)(dx/GetAxis()->getScale());
	if(abs(dy) < snap) moveY = (int)(dy/GetAxis()->getScale());

	if(pMoveX) *pMoveX = moveX;
	if(pMoveY) *pMoveY = moveY;

	if(moveX || moveY)
	{
		image->SetPos(image->X() + moveX, image->Y() + moveY);
		return true;
	}

	return false;
}

void GameControl::ToggleSnap()
{
	if(bGameMode) return;

	bActorSnap = !bActorSnap;
}

void GameControl::MoveView()
{
	if(!viewActor) return;

	 //Don't use this code (shake moondefender and BattlestarFGC_130.dat)
	/*{int x, y;
	viewActor->getImage()->CalcTransform();
	
	if(viewActor->getParent() == GetAxis())
	{
		x = viewActor->getImage()->X();
		y = viewActor->getImage()->Y();
	}
	else
	{
		//To axis coordinates
		GetAxis()->getImage()->CalcTransform();
		
		x = viewActor->getImage()->CompositeXForm().x.ToInt();
		y = viewActor->getImage()->CompositeXForm().y.ToInt();
		
		KrVector2T< GlFixed > object;
		GetAxis()->getImage()->ScreenToObject( x, y, &object, 0 );
		

		x = object.x.ToIntRound();
		y = object.y.ToIntRound();
	}	
	
	GetAxis()->SetPos(-x, -y);
	}return;*/

	//Solve the moondefender and BattlestarFGC_130.dat shake
	
	GlFixed x, y;
	KrImage *imView = viewActor->getImage();
	KrImage *imAxis = GetAxis()->getImage();
		
	if(viewActor->getParent() == GetAxis())
	{
		imView->CalcTransform();

		//Round to solve the shake in 'shake bug.ged'
		x = imView->XTransform().x.ToIntRound();
		y = imView->XTransform().y.ToIntRound();
	}
	else
	{
		//To axis coordinates		
		imView->CalcCompositeTransform();				 
		
		x = imView->CompositeXForm().x;
		y = imView->CompositeXForm().y;
		
		KrVector2T< GlFixed > object;
		imAxis->ScreenToObject( x, y, &object );
		
		//Shake the CollisionTile3.ged if not round
		//x = object.x;
		//y = object.y;
		x = object.x.ToIntRound();
		y = object.y.ToIntRound();
	}	
	
	GetAxis()->SetPos(-x.ToDouble(), -y.ToDouble());
		
	///////////////////////////////////////////////////
	//Some times BattlestarFGC_130.dat shake yet
	//To avoid this, the follow code could be used to fix the view position
	//The problem, is couses strange moves on moondefender rocks

	//Check the view pos (the composite coordinate must be (0, 0))
	/*GlFixed xscreen, yscreen;

	imView->CalcCompositeTransform();	

	xscreen = imView->CompositeXForm().x;
	yscreen = imView->CompositeXForm().y;

	if(xscreen.ToDouble() != 0 || yscreen.ToDouble() != 0)
	{ 
		GLOUTPUT("  View: %3f, %3f\n", xscreen.ToDouble(), yscreen.ToDouble());

		//if((xscreen.ToInt() != 0 && abs(xscreen.ToInt()) < 10) || (yscreen.ToInt() != 0 && abs(yscreen.ToInt()) < 10))
		{
			//imAxis->SetPos(imAxis->X() - xscreen, imAxis->Y() - yscreen);
			GetAxis()->SetPos(-x.ToDouble() - xscreen.ToDouble(), -y.ToDouble() - yscreen.ToDouble());	
	
			imView->CalcCompositeTransform();

			xscreen = imView->CompositeXForm().x;
			yscreen = imView->CompositeXForm().y;
			
			GLOUTPUT("      : %3f, %3f\n", xscreen.ToDouble(), yscreen.ToDouble());
		}
	}*/

	///////////////////////////////////////////////////

}

#ifdef _WIN32_WCE
extern "C" int GAPI_ShowTaskBar();
extern "C" int GAPI_HideTaskBar();
extern "C" HWND menuBar;
#endif

void GameControl::SDL_Pause(int bPause)
{
	//Control the SDL, network and sound system
	//Sound bug in Pocket PC:
	/*
	"There is a sound issue on some PocketPC devices (particularly Windows Mobile based devices).
     If the game is interrupted by an alarm, other event, or incoming phone call, 
	 the sound in the game will go silent and not return until you restart." (Richard, rpowell@magearts.com)
	*/

	//But if the sound system is closed, the music don't will continue after pause

	PauseNetwork(bPause);

	if(bPause)
	{		
		SDL_PauseOn();
	}
	else
	{
		SDL_PauseOff();
	}
}

void GameControl::PauseNetwork(int bPause)
{
	if(bPause)
	{		
		if(rakPeer)
		{
			//Stop the network
			DestroyRakNet();
			bRestartNetworkAfterPause = true;
		}
	}
	else
	{
		//Try reconnect again
		if(bRestartNetworkAfterPause)
		{
			bRestartNetworkAfterPause = false;

			//Save connections before restart
			GlMap<SystemAddress, U32, GlSystemAddressHash> mapConnectionsRetry = mapConnections;
			GlMapIterator<SystemAddress, U32, GlSystemAddressHash> it(mapConnectionsRetry);
			SystemAddress *pAddr;

			//Restart
			InitRakNet(currentListenPort);

			//Try the old connections if any
			for(it.Begin(); !it.Done(); it.Next())
			{
				pAddr = it.Key();

				if(pAddr)
				{
					Connect(pAddr->ToString(false), pAddr->port);
				}
			}
		}
	}
}

void ForceSuspendGame()
{
	if(GameControl::Get()->IsGameSuspended()) 
		GameControl::Get()->SuspendGame(0);
	
	GameControl::Get()->SuspendGame(1);
}

void GameControl::SuspendGame(int bSuspendOn)
{
	/*
	Suspend on: 
		Pause the game
		Don't receive any events
		Show task bar on PocketPC
		Off only if get focus
	*/


	if(bSuspendOn)
	{
		if(!bSuspendGame)
		{
#ifndef _WIN32_WCE
			SDL_Pause(true);
			bSuspendGame = true;			
#else
			if(GAPI_ShowTaskBar())
			{
				SDL_Pause(true);
				bSuspendGame = true;
			}
#endif	
			
			if(bSuspendGame)
			{
				//Keep all pressed keys at this time stored at mapKeyDown
				
				//Clear last keys
				lastKeys.Clear();
			}
		}
	}
	else
	{
		if(bSuspendGame)
		{
#ifndef _WIN32_WCE
			if(!bPauseGame) SDL_Pause(false);
			bSuspendGame = false;
#else
			if(GAPI_HideTaskBar())
			{
				engine->InvalidateScreen();
				if(!bPauseGame) SDL_Pause(false);
				bSuspendGame = false;
			}
#endif		

			if(!bSuspendGame)
			{
				//Send Key Up event for keys pressed when suspension begins

				MapKeyDownIterator itKeys(mapKeyDown);
				//int repeatLoopCount = 1;
				for(itKeys.Begin(); !itKeys.Done(); itKeys.Next())
				{
					SDL_Event event;
					memset(&event, 0, sizeof(event));

					event.type = SDL_KEYUP;
					event.key.keysym.sym = *itKeys.Key();
					SDL_PushEvent(&event);
				}

				//Clear map keys
				mapKeyDown.Clear();
				lastKeys.Clear();
			}			
		}
	}

#ifdef _DEBUG
	if(bSuspendGame) GLOUTPUT("Suspend On\n");
	else GLOUTPUT("Suspend Off\n");
#endif
}

void GameControl::PauseGame(int bPauseOn)
{
	/*
	Pause on: 
		Pause the game
		Continue receiving keyboard and mouse events
		Off if call PauseGameOff
	*/

	if(bPauseOn)
	{
		if(!bPauseGame)
		{
			SDL_Pause(true);
			bPauseGame = true;
		}
	}
	else if(!bSuspendGame) 
	{
		if(bPauseGame)
		{
			SDL_Pause(false);
			bPauseGame = false;
		}
	}
}


void GameControl::DoKeyDownRepeat(Actor *actor)
{
	if(bGameMode && actor->getAction() && actor->getAction()->HaveKeyDown(true))
	{
		MapKeyDownIterator itKeys(mapKeyDown);
		int repeatLoopCount = 1;
		for(itKeys.Begin(); !itKeys.Done(); itKeys.Next())
		{
			actor->OnKeyDown(*itKeys.Key(), repeatLoopCount++);
		}
	}
}

void GameControl::ProcessKeyDownRepeat()
{
	//Search actions with key down event only
	//Solves the delay in Key Down - Draw Actor Delay.ged

	if(bGameMode)
	{		
		GlobalMapActionsIterator itAction(Action::getActionsKeyDown());
		
		for(itAction.Begin(); !itAction.Done(); itAction.Next())
		{
			//i < nActorsInList: solve the loop when create the same actor in the OnKeyDown event

			ListActor *listActor = mapActors[(*itAction.Value())->getActionName()];
			if(listActor)
			{
				int nActorsInList = listActor->Count(), i = 0;

				for(int il = 0; listActor && il < listActor->Count() && i < nActorsInList; il++)
				{				
					Actor *actor = (*listActor)[il];
					if(actor->getRunning() && actor->isWorkingActor())
					{
						DoKeyDownRepeat(actor);
					}

					i++;
				}
			}
		}
	}
}

void GameControl::PushActionInGlobalQueue(		    
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional
			)
{
	//Get timestamp of request
	RakNetTime timeStamp = RakNet::GetTime();

	//Get the current machine queue
	QueueAction *pQueue = mapActionQueue.FindOrCreate(currentOwnerMachine);

#ifdef _DEBUG
	if(!pQueue->Empty() && pQueue->Back())
	{
		GLASSERT(timeStamp >= pQueue->Back()->timeStampOfRequest);
	}
#endif

	ActionInfo *action = new ActionInfo(timeStamp,
			actionNumber, actionName, 
			eventActorName, eventActorCloneIndex, eventActorOwnershipTimestamp,
			collideActorName, collideActorCloneIndex,
			bExecuteConditional);		


	pQueue->PushBack(action);
}

RakNetTime GameControl::GetNextGVT()
{
	//GVT algorithm from the book Massively Multiplayer Game Development 2 (Chapter 2.11)
	
	//Initialize with a large value
	RakNetTime nextGVT = (RakNetTime) - 1; 

	//Checks the lowest timestamp
	GlMapIterator<SystemAddress, QueueAction, GlSystemAddressHash> it(mapActionQueue);
	for(it.Begin(); !it.Done(); it.Next())
	{
		QueueAction *p = it.Value();
		if(p->Empty())
		{
			//Need more information about this machine
			//Use the SendRequestGVT or wait a disconnection?
			nextGVT = 0;
			break;
		}
		
		if(p->Front()->timeStampOfRequest < nextGVT) 
		{
			nextGVT = p->Front()->timeStampOfRequest;
		}
	}

	return nextGVT;
}

void GameControl::ProcessActions()
{
	if(mapActionQueue.empty() || !bGameMode) return;

	//Use the GVT algorithm
	RakNetTime nextGVT, t, currentTime;	
	SystemAddress owner;
	QueueAction *pMin = NULL;
	bool bExecuted = true;

	GlMapIterator<SystemAddress, QueueAction, GlSystemAddressHash> it(mapActionQueue);

	while(bExecuted && (nextGVT = GetNextGVT()) != 0) //Get the GVT after each execution to avoid long queue!
	{
		bExecuted = false;
		t = (RakNetTime) - 1;
		pMin = NULL;

		for(it.Begin(); !it.Done(); it.Next())
		{
			QueueAction *p = it.Value();
			if(!p->Empty())
			{
				currentTime = p->Front()->timeStampOfRequest;
				if(currentTime <= nextGVT && currentTime < t) 
				{
					t = currentTime;
					pMin = p;
					owner = *it.Key();
				}
			}
		}

		if(pMin)
		{
			ActionInfo *pActionInfo = pMin->Front();
			pMin->PopFront();

			ExecuteAction(owner,
				pActionInfo->actionNumber, *(pActionInfo->actionName), 
				*(pActionInfo->eventActorName), pActionInfo->eventActorCloneIndex, pActionInfo->eventActorOwnershipTimestamp,
				*(pActionInfo->collideActorName), pActionInfo->collideActorCloneIndex,
				pActionInfo->bExecuteConditional, true,
				false,
				0);

			delete pActionInfo;
			bExecuted = true;
		}		
	}
}

bool GameControl::ExecuteAction(const SystemAddress& addr,
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional, bool bForceExecute,
			bool bRemote,
			unsigned int seed)
{
	//Try get the event actor (don't allow load?)
	Actor *eventActor = GetNetActor(addr, eventActorName.getCharBuf(), eventActorCloneIndex);
	if(!eventActor) 
	{
		//Can't execute the action
		//Can try again later
		return false; 
	}
	
	//Get the action
	Action *action = Action::Call(actionName);

	if(action)
	{
		Actor *collideActor = NULL;

		if(collideActorCloneIndex >= 0)
		{
			//Try get the collide actor (don't allow load?)
			gedString collideActorCloneName(collideActorName);
			collideActorCloneName += '.';
			collideActorCloneName += gedString(collideActorCloneIndex);

			collideActor = GetActor(collideActorCloneName, true, false, false);
		}

		//Don't let be deleted by other actors
		action->AttachAction();

		//Set the owner machine for this action
		//New net actors created now will get this owner machine
		SystemAddress currenOwner = currentOwnerMachine;
		currentOwnerMachine = addr;
		bool bCanExecute = true;

		if(bRemote && eventActor->getNetActor() && eventActor->GetOwner() != addr)
		{
			//Some thing is wrong		
			//If the remote machine are send this action the ownership must be the remote machine
			bCanExecute = eventActor->RequestActorOwnership(addr, eventActorOwnershipTimestamp);
		}

		if(bCanExecute)
		{
			//Execute		
			action->Execute(actionNumber, eventActor, collideActor, bExecuteConditional, bForceExecute, bRemote, seed);
		}
		
		//Restore owner machine
		currentOwnerMachine = currenOwner;

		//Delete here if not used by other actors
		action->DetachAction();
	}

	return true;
}

void GameControl::ProcessActor(Actor *actor)
{
	if(actor->getRunning() || (actor->editAnim() && !bGameMode))
	{
		//Repeat key down
		//Don't put repeat key here in game mode, causes the delay in Key Down - Draw Actor Delay.ged
		//Call in ProcessKeyDownRepeat
		//DoKeyDownRepeat(actor);

		
		//Update path
		Path *pathX = actor->getPathX(),
			 *pathY = actor->getPathY();

		bool bCanExecuteLocally = actor->CanExecuteActionLocally();

		if(!actor->getNetOptimizedPrecision() || bCanExecuteLocally)
		{
			if(pathX && pathY)
			{
				//Solve the bug in pathChange_bug.ged

				int x, y;

				switch(actor->getPathDirectionX())
				{
				case FORWARD:
					actor->setFramePathX(actor->getFramePathX() + 1);
					actor->setFramePathY(actor->getFramePathY() + 1);
					break;
				case BACKWARD:
					actor->setFramePathX(actor->getFramePathX() - 1);
					actor->setFramePathY(actor->getFramePathY() - 1);
					break;
				}

				if(actor->getFramePathX() >= pathX->GetTotalFrames())
				{
					if(bGameMode) actor->OnPathFinish();
					actor->setFramePathX(0);								
					actor->setFramePathY(0);
				}

				if(actor->getFramePathX() < 0)
				{
					if(bGameMode) actor->OnPathFinish();
					actor->setFramePathX(pathX->GetTotalFrames() - 1);
					actor->setFramePathY(pathY->GetTotalFrames() - 1);
				}

				if(actor->getPathX() && actor->getPathY()) //Check again to avoid crash when call OnPathFinish with Move To Path Finder
				{				
					pathX->GetX(x, actor->getFramePathX());
					pathY->GetY(y, actor->getFramePathY());
					x += actor->getPathIniX();
					y += actor->getPathIniY();

					//Set actor position by path
					actor->SetPos(x, y);
				}
			}
			else
			{
				if(pathX)
				{							
					int x;

					switch(actor->getPathDirectionX())
					{
					case FORWARD:
						actor->setFramePathX(actor->getFramePathX() + 1);
						break;
					case BACKWARD:
						actor->setFramePathX(actor->getFramePathX() - 1);
						break;
					}

					if(actor->getFramePathX() >= pathX->GetTotalFrames())
					{
						if(bGameMode) actor->OnPathFinish();
						actor->setFramePathX(0);								
					}

					if(actor->getFramePathX() < 0)
					{
						if(bGameMode) actor->OnPathFinish();
						actor->setFramePathX(pathX->GetTotalFrames() - 1);
					}

					if(actor->getPathX()) //Check again to avoid crash when call OnPathFinish with Move To Path Finder
					{
						pathX->GetX(x, actor->getFramePathX());
						x += actor->getPathIniX();

						//Set actor position by path
						actor->SetPos(x, actor->getImage()->Y());
					}
				}
				else if(actor->getRunning())
				{
					//Move by velocity
					if(actor->getVelocityX() != 0.0)
					{
						actor->SetPos(actor->getX() + actor->getVelocityX()*motionCorrection, actor->getY());
					}
				}


				if(pathY)
				{							
					int y;

					switch(actor->getPathDirectionY())
					{
					case FORWARD:
						actor->setFramePathY(actor->getFramePathY() + 1);
						break;
					case BACKWARD:
						actor->setFramePathY(actor->getFramePathY() - 1);
						break;
					}

					if(actor->getFramePathY() >= pathY->GetTotalFrames())
					{
						if(bGameMode && pathY != pathX) actor->OnPathFinish(); //Avoid call twice
						actor->setFramePathY(0);								
					}

					if(actor->getFramePathY() < 0)
					{
						if(bGameMode && pathY != pathX) actor->OnPathFinish();
						actor->setFramePathY(pathY->GetTotalFrames() - 1);
					}

					if(actor->getPathY()) //Check again to avoid crash when call OnPathFinish with Move To Path Finder
					{
						pathY->GetY(y, actor->getFramePathY());
						y += actor->getPathIniY();

						//Set actor position by path
						actor->SetPos(actor->getImage()->X(), y);
					}
				}
				else if(actor->getRunning())
				{
					//Move by velocity
					if(actor->getVelocityY() != 0.0)
					{
						actor->SetPos(actor->getX(), actor->getY() + actor->getVelocityY()*motionCorrection);
					}
				}
			}
		}
		
		//Follow mouse
		if(actor->getRunning() && actor->getFollowMouseAxis() != NONE_AXIS && bCanExecuteLocally)
		{
			KrVector2T< GlFixed > object;									
			actor->getParent()->getImage()->ScreenToObject( mouseX, mouseY, &object );
			
			//Use ToIntRound (solve the 'View Position Bug.ged')
			switch(actor->getFollowMouseAxis())
			{
			case BOTH_AXIS:
				actor->SetPos(object.x.ToIntRound(), object.y.ToIntRound(), true);
				break;
			case X_AXIS:
				actor->SetPos(object.x.ToIntRound(), actor->getImage()->Y(), true);
				break;
			case Y_AXIS:
				actor->SetPos(actor->getImage()->X(), object.y.ToIntRound(), true);
				break;
			}
		}														
		
		
		if(bGameMode)
		{				
#ifndef USE_RTREE
			//Actor out of vision?
			if(bCheckOutOfVision)
			{
				actor->CheckOutOfVision();
			}
#else
			//Actor into view
			if(bCheckOutOfVision)
			{
				actor->SetActorInView(true);
			}
#endif
		
			//Notify actor
			//Call OnAnimation after call CheckOutOfVision to solve the bug "Receive events if out of vision not working when the actor starts out of the view"
			//(OutOfVision Draw Actor.ged, http://game-editor.com/forum/viewtopic.php?p=24443)
			if(actor->isWorkingActor()) actor->OnAnimation();
			
			//Keep move to?
			actor->CheckMoveTo();
		}
		
		//Do animation
		if(actor->isWorkingActor() || !bGameMode)
		{
			long framesToAnim = actor->getFramesToAnim();
			bool bForward = actor->getForwardAnimation();
			bool bSendToRemoveMachines = actor->getNetOptimizedPrecision() && actor->CanExecuteActionLocally();

			if(framesToAnim != 0)
			{
				KrSprite *sprite = actor->getSprite();
				if(sprite)
				{
					Sequence *seq = actor->GetSequence();	
					double animposAnt = actor->getScriptVars()->animpos;
					
					if(seq && seq->getType() == DEFINED_SEQUENCE)
					{
						int iFrame;

						if(seq->DoStep(bForward, iFrame))
						{
							//Adjust animpos before action (solve submarine.ged bug)
							//The Fairy.ged fail due use of the absolute frame position.
							//The right code is to use the frame of the desired sequence
							actor->getScriptVars()->animpos = iFrame;
							if(bGameMode) actor->OnAnimationFinish();
						}
						else
						{
							actor->getScriptVars()->animpos = iFrame;
						}

						sprite->SetFrame(seq->GetFrame(iFrame));
					}
					else
					{
						if(sprite->DoStep(bForward, realFrameRate/*getFrameRate()*/))
						{
							//Adjust animpos before action (solve submarine.ged bug)
							actor->getScriptVars()->animpos = sprite->Frame();
							if(bGameMode) actor->OnAnimationFinish();
						}
						else
						{
							actor->getScriptVars()->animpos = sprite->Frame();
						}
					}

					//actor->getScriptVars()->animpos = sprite->Frame();

					if(bSendToRemoveMachines && actor->getScriptVars()->animpos != animposAnt)
					{
						SendRemoteFrame(actor, true);
					}
				}

				if(framesToAnim > 0)
					actor->setFramesToAnim(framesToAnim - 1);			
			}
		}
		
		if(actor->HaveQueueAction()) actor->ProcessQueueActions();						
	
	}
#ifndef STAND_ALONE_GAME
	else if(!getGameMode() && actor->getPanel())
	{
		actor->OnAnimation();
	}
#endif
}

#ifdef USE_RTREE
bool GameControl::RTreeSearchCallback(int id, void* arg) 
{  
	gameControl->visibleActors.PushBack((Actor *)id);	

	return true; 
}
#endif

void GameControl::HandleMouseMotion()
{
#ifndef STAND_ALONE_GAME

#if !defined(STAND_ALONE_GAME) && defined(__linux__)
	if(!bCanResize && !bGameMode) 
	{
		bCanResize = true;
		SDL_EventState(SDL_VIDEORESIZE, SDL_ENABLE);
	}
#endif

	if(!bGameMode)
	{
		if(mainActor) 
		{
			if(!actorDrag) 
				mainActor->OnMouseMove(mouseX, mouseY);	

			//Update actor coordinates
			((MainPanel *)mainActor)->ShowActorCoordinates(actorDrag);
		}

		Actor::NotifyMouseCoord(mouseX, mouseY);
		ActivationEventsCanvas::NotifyMouseCoord(mouseX, mouseY);
		ReleaseActorTip(false);
	}
	if(currentTile) currentTile->OnMouseMove(mouseX, mouseY);
#endif //#ifndef STAND_ALONE_GAME

	//Handle drag
	if(actorDrag)
	{
		if(IS_VALID_ACTOR1(actorDrag)) //actorDrag still valid?
		{
			//Drag (shakes if the view move, Mouse Enter Leave.ged)
			if(actorDrag->EditMode() || PathPoint::getNPaths()) GetAxis()->GridSnap(mouseX, mouseY);
			if(actorDrag->getParent())
			{
				KrVector2T< GlFixed > object;

				actorDrag->getParent()->getImage()->ScreenToObject( mouseX, mouseY, &object );
				actorDrag->SetPos(object.x.ToIntRound() - mouseActorDownX, object.y.ToIntRound() - mouseActorDownY, true);						
			}
			else
			{
				actorDrag->SetPos(mouseX - mouseActorDownX, mouseY - mouseActorDownY, true);
			}

			actorDrag->OnMouseMove(mouseX, mouseY);
		}

		if(!bGameMode)
		{
			if(GetPathRoot() || ActivationEventsCanvas::HasActivationEventLines(actorDrag->getCloneName())) 
			{
				//Slow redraw
				GetAxis()->getImage()->Invalidate();
			}		
		}
	}
	else if(actorButtonDown) //Send mouse move to actor which receive button down
	{
		if(IS_VALID_ACTOR1(actorButtonDown)) actorButtonDown->OnMouseMove(mouseX, mouseY);
	}


	//Handle Mouse Enter and Mouse Leave
	//In the Pocket PC only generates mouse enter or leave if the stylus is down
	bool bHit = false;

#ifdef WIN32_WCE
	if(bMouseButtonDown)
#endif
	{
		GlDynArray<KrImage*> hittest;				
		engine->Tree()->HitTest(mouseX, mouseY, 0, &hittest);

		if(	hittest.Count() > 0)
		{
			//Notify actor
			KrImage *p = hittest.Item(0);
			Actor *actor = (Actor *)p->getParentActor();
			bHit = true;

			if(!InModal(actor))
			{
				actor = NULL;
				currentActor = NULL;
			}

			if(actor)
			{			
				if(actor != currentActor)
				{
					if(IS_VALID_ACTOR1(currentActor))
					{
						currentActor->OnMouseLeave(mouseX, mouseY);
					}

					actor->OnMouseEnter(mouseX, mouseY);
					currentActor = actor;				
				}						

				if(!actorButtonDown) actor->OnMouseMove(mouseX, mouseY);
			}

		}
	}


	if(!bHit)
	{
		if(IS_VALID_ACTOR1(currentActor))
		{
			currentActor->OnMouseLeave(mouseX, mouseY);
		}						

		currentActor = NULL;
	}
}

bool GameControl::GameTick(SDL_Event &event)
{
	static bool bQuitEvent = false;	

	
#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME)
	//Update days check
	totalFrames++;
#endif

#ifndef STAND_ALONE_GAME
	if(bUpdateScriptPanel)
	{
		UpdatePanelScript();
		bUpdateScriptPanel = false;
	}
#endif
	
	if (event.type == SDL_POCKET_HIBERNATE)
	{
		//Try release some memory
		//If fail will exit the application

		if(MemoryAllocationError(512*1024))
		{
			//Don't need exit and show the low memory message now,
			//but some other application or the system can want more memory.
			//
			//"An application should attempt to release as many resources as possible when sent this message"
			//
			//So, release all memory now (256MB)
			
			ReleaseMemory(256*1024*1024);

			return true;
		}
		else
		{
			return false;
		}		
	}
	else if (event.type == SDL_QUIT && !bQuitEvent)
	{
		bQuitEvent = true;
		
		if(getStandAloneMode())
		{
			return false;
		}

#ifndef STAND_ALONE_GAME
		/*<Odilon>
		Verifica se nao foi feito nada, ainda, para
		pedir confirmacao apenas se necessario.
		*/
		
		if(!bGameMode)
		{
			if(Modified())
			{
				//<Odilon> Pequena correcao de Ingles...
				PanelQuestion *panel = new PanelQuestion("This will discard the current game.\nProceed anyway?");
				
				if(panel->Wait() == OK_BUTTON)
				{
					delete panel;
		
					/*#ifndef GAME_EDITOR_PROFESSIONAL
					//Final message				
					if(Config::Get()->getShowForumReminder())
					{
						if(mainActor) mainActor->getImage()->SetVisible(false);
		
						SwitchResolution(engine->Surface(), 477, 240, false);
						PanelQuestion *finalMessage = new PanelQuestion("\n\nGame Editor was made for you\n\n\n\n\nIf you have any doubts or suggestions, post your comments in our forum:\n\ngame-editor.com/forum\n\nYour opinion is very important for us\nThank you\n", "Invitation", "Close", "Don't show me anymore");				
		
						Actor *logo = new Actor("logo", finalMessage);
						logo->AddAnimation("maksiconActor.gif", "maksiconActor.gif", 1, 1, 0, 0);
						logo->SetPos(40, 25);
		
						if(finalMessage->Wait() == CANCEL_BUTTON)
						{
							Config::Get()->setShowForumReminder(false);						
						}
						
						delete logo;
						delete finalMessage;
					}			
					#endif*/
		
					bQuitEvent = false; //May be in a Wait() panel
					return false;
				}
		
				delete panel;
			}
			else
			{
				bQuitEvent = false; //May be in a Wait() panel
				return false;
			}
		}
		else
		{
			SetGameMode(false);
			ActorEdit::DoActivationEvent(false);
			RestoreGame();

			//No crashs in game mode, remove emergency warning
			/*if(Config::Get()->getEmergencySave().length())
			{
				remove(Config::Get()->getEmergencySave().c_str());
				Config::Get()->setEmergencySave("");
			}

			Config::Destroy();*/
		}
		//else  //nao ha necessidade de confirmacao...
		//	return false;
#endif //#ifndef STAND_ALONE_GAME


		bQuitEvent = false;
	}
#if !defined(STAND_ALONE_GAME) && defined(WIN32)
	else if (event.type == SDL_DROPFILES)
	{
		if(!bGameMode)
		{
			bool bLoad = true;
			if(Modified())
			{
				PanelQuestion *panel = new PanelQuestion("This will discard the current game.\nProceed anyway?");
				if(panel->Wait() != OK_BUTTON)
				{
					bLoad = false;
				}
				
				delete panel;
			}
			
			if(bLoad)
			{
				MainPanel::Get()->LoadGame((char *)event.user.data1, true);
			}
		}

		return true;
	}
#endif
	else if (event.type == SDL_LOAD_GAME__EVENT)
	{
		char oldpath[PATH_LIMIT];
		ged_getcwd(oldpath, PATH_LIMIT);
		chdir(getGamePath().c_str());
		
		if(getStandAloneMode())
		{
			indexedGame = NULL; //Don't close here (Closed in KrResourceVault::~KrResourceVault())
			
			SetGameMode(false, false); //after indexedGame = NULL

			engine->ReinitVault();
			
			if(!CheckStandAloneMode(getLoadGameEvent()))
			{
				return false;
			}
		}
		else
		{
			SetGameMode(false, false);
			if(!Load(getLoadGameEvent(), false, false, false))
			{
				//Show error message after retore
				#ifndef STAND_ALONE_GAME
				new PanelInfo(gedString("LoadGame error. The file \'") + getLoadGameEvent().GetFileName() + "\' is not a valid game");
				#endif //#ifndef STAND_ALONE_GAME

				//Exit game
				SDL_Event event;
				memset(&event, 0, sizeof(SDL_Event));
				event.quit.type = SDL_QUIT;
				SDL_PushEvent(&event);				
			}

			SetGameMode(true);
			bLoadGameUsedInGameMode = true;
		}			
		
		chdir(oldpath);
		
		mapKeyDown.Clear();
		lastKeys.Clear();
		while(lastKeys.Count() < MAX_KEY_SEQUENCE) lastKeys.PushBack(0);

		bCanAddFrameRateEvent = true;		
		return true;
	}
	else if (event.type == SDL_ACTIVEEVENT)
	{
#if !defined(STAND_ALONE_GAME)
		if(bGameMode && Tutorial::IsOff())
#else
		if(bGameMode)
#endif
		{
			if(event.active.state & SDL_APPINPUTFOCUS)
			{
				if(event.active.gain)
				{


#ifndef _WIN32_WCE
					//Exit suspend on desktop if loose the focus
					SuspendGame(false);	

					#ifdef __WIN32__
					//Solve the bug "When minimized does not restore in game mode unless you right click the task icon and close it."
					ShowWindow((HWND)GetMainWindow(), SW_RESTORE);
					#endif
#else
					if(!menuBar) 
					{
						//If menu bar is present, activate when press Continue, only
						SuspendGame(false);					
					}
#endif
				}
				else if(bSuspendGameIfLostFocus)
				{
					if(gameFrame > fps) 
					{
						//Don't suspend at first message
						//Solve the bug: "In some devices game always start in suspend mode"
						SuspendGame(true);
					}
				}
			}
		}

	}
	else if (event.type == SDL_POCKET_CONTINUE)
	{
		//Exit suspend on PocketPC only if press continue buttom
		SuspendGame(false);
	}
	

	//Get Actor Text input
	KrEventManager::Instance()->HandleEvent( event, engine );

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME) && !defined(_DEBUG)
	if(event.type)
	{
		NANOBEGIN

		//Update days check
		if(totalFrames > executeUpToFrame)
		{
			return false;
		}

		NANOEND
	}
#endif
	
#ifndef STAND_ALONE_GAME
	//Allow ESC in editor if game is in suspension state
	if(!bSuspendGame || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
	{
		if(bSuspendGame)
		{
			//ESC, leave suspension mode
			SuspendGame(false);
		}
#else
	if(!bSuspendGame)
	{
#endif
	
	switch(event.type)
	{
	case SDL_VIDEORESIZE:
		if(!bPauseGame)
		{				
			if(!getStandAloneMode()) 
			{
				#if !defined(STAND_ALONE_GAME) 
				
				#ifdef __linux__
					if(bCanResize) //Solve the resize bug when leave game mode
				#elif WIN32
					if(IsWindowVisible(SDL_Window)) //Solve bug when select Script or Schematic panel after game mode (show the stage)
				#endif

				#endif

				{
					SwitchResolution(engine->Surface(), event.resize.w, event.resize.h);
					
					//Reset position, if is the initial resize
					if(GetAxis()->getImage()->X() == resX/2 && GetAxis()->getImage()->Y() == resY/2)
					{
						GetAxis()->getImage()->SetPos(event.resize.w/2, event.resize.h/2);
					}
				}


				#if !defined(STAND_ALONE_GAME)
				static bool bStartInitialCheck = false;
				if(!bInitialCheck && !bStartInitialCheck)
				{
					bStartInitialCheck = true;

					//Start message after initial resize
					if(UndoControl::Get()->HaveEmergencyData())
					{
						//gedString recoveryName(Config::Get()->getEmergencySave().GetFileName());
						ActorProperty::Destroy();
						PanelQuestion *panel = new PanelQuestion("A failure happened on last session,\nbut Game Editor can recover your work\nDo you want reload your last game changes?", "Emergency Recovery");
						
						if(panel->Wait() == OK_BUTTON)
						{
							//LoadSaveGame::Load(Config::Get()->getEmergencySave(), true);
							bool bRetry = true;
							
							do
							{
								if(!UndoControl::Get()->LoadEmergencyData())
								{
									gedString error("A failure happened in the recovery");
									
									if(*SDL_GetError() != 0)
									{
										error += ":\n\n";
										error += SDL_GetError();
										error += "\n\n";
									}
									
									error += "Do you want to try again?";
									
									PanelQuestion *panel2 = new PanelQuestion(error, "Emergency Recovery", "Yes", "No", ALIGN_LEFT);
									
									if(panel2->Wait() != OK_BUTTON)
									{
										bRetry = false;
									}
									
									delete panel2;
								}
								else
								{
									bRetry = false;
								}

							} while(bRetry);

							bModified = true;
						}
						else
						{					
							remove(Config::Get()->getEmergencySave().c_str());
							Config::Get()->setEmergencySave("");
							Config::Destroy();
						}
						
						delete panel;
					}
					
					
					#if defined(__WIN32__) && !defined(GAME_EDITOR_HOME_EDITION)
					else if(!TutorialReminder::getShow() && Config::Get()->getShowTutorialReminder())
					{
						new TutorialReminder();
					}	
					#endif

					//Make true here (used in the undo function too)
					bInitialCheck = true;
				}
				#endif
			}
		}
		break;
	case SDL_KEYDOWN: //Works even in pause mode
		{			
			event.key.keysym.sym = UserDefinedKeys(event.key.keysym.sym);
			event.key.keysym.sym = PocketPCKey(event.key.keysym.sym);

			if(!bSuspendGame)
			{
				mapKeyDown.Add(event.key.keysym.sym, 1);
				lastKey = event.key.keysym.sym;
			}



			lastKeys.PushFront(lastKey);

			if(lastKeys.Count() > MAX_KEY_SEQUENCE)
			{
				//Maximum MAX_KEY_SEQUENCE more keys
				lastKeys.PopBack();
			}

			if(bGameMode)
			{
				#ifdef STAND_ALONE_GAME
				if(event.key.keysym.sym == SDLK_ESCAPE && bUseESCKeyToExit)
				{
					SDL_Event event;
					memset(&event, 0, sizeof(SDL_Event));
					event.quit.type = SDL_QUIT;
					SDL_PushEvent(&event);					
				}
				#endif

				//Search actions with key down event only
				GlobalMapActionsIterator itAction(Action::getActionsKeyDown());
				for(itAction.Begin(); !itAction.Done(); itAction.Next())
				{
					//i < nActorsInList: solve the loop when create the same actor in the OnKeyDown event

					ListActor *listActor = mapActors[(*itAction.Value())->getActionName()];
					if(listActor)
					{
						int nActorsInList = listActor->Count(), i = 0;

						for(int il = 0; listActor && il < listActor->Count() && i < nActorsInList; il++)
						{				
							Actor *actor = (*listActor)[il];

							if(InModal(actor) && actor->isWorkingActor())
							{
								actor->OnKeyDown(event.key.keysym.sym, 0);
							}

							i++;
						}
					}
				}

				if(mainActor) mainActor->OnKeyDown(event.key.keysym.sym, 0);			
			}
			else
			{
				#ifndef STAND_ALONE_GAME
				//After a key press disable tips
				//Reenable only into a mouse event
				ReleaseActorTip(true);
				#endif

				if(currentTile)
				{
					currentTile->SetVisibleCursor(event.key.keysym.sym != SDLK_RCTRL);
					currentTile->SetDrawMode(event.key.keysym.sym != SDLK_RSHIFT);
				}
				
				
				MapActorIterator it(mapActors);
				for(it.Begin(); !it.Done(); it.Next())
				{
					ListActor *listActor = it.Value();
					for(int il = 0; listActor && il < listActor->Count(); il++)
					{
						Actor *actor = (*listActor)[il];
						if(InModal(actor))
							actor->OnKeyDown(event.key.keysym.sym, 0);
					}
				}
			}
			
		}
		break;
		
	case SDL_KEYUP: //Works even in pause mode
		{
			event.key.keysym.sym = UserDefinedKeys(event.key.keysym.sym);
			event.key.keysym.sym = PocketPCKey(event.key.keysym.sym);
			if(!bSuspendGame) mapKeyDown.Remove(event.key.keysym.sym);
			itemSelect = 0;
			
			if(bGameMode)
			{
				//Search actions with key up event only
				GlobalMapActionsIterator itAction(Action::getActionsKeyUp());
				for(itAction.Begin(); !itAction.Done(); itAction.Next())
				{					
					//i < nActorsInList: solve the loop when create the same actor in the OnKeyUp event

					ListActor *listActor = mapActors[(*itAction.Value())->getActionName()];
					if(listActor)
					{
						int nActorsInList = listActor->Count(), i = 0;

						for(int il = 0; listActor && il < listActor->Count() && i < nActorsInList; il++)
						{				
							Actor *actor = (*listActor)[il];

							if(InModal(actor) && actor->isWorkingActor())
							{
								actor->OnKeyUp(event.key.keysym.sym);
							}

							i++;
						}
					}
				}

				if(mainActor) mainActor->OnKeyUp(event.key.keysym.sym);
			}
			else
			{
				if(currentTile)
				{
					currentTile->SetVisibleCursor(true);
					currentTile->SetDrawMode(true);
				}
				
				MapActorIterator it(mapActors);
				for(it.Begin(); !it.Done(); it.Next())
				{
					ListActor *listActor = it.Value();
					for(int il = 0; listActor && il < listActor->Count(); il++)
					{
						Actor *actor = (*listActor)[il];
						if(InModal(actor))
							actor->OnKeyUp(event.key.keysym.sym);
					}
				}
			}
		}
		break;
		
		
	case SDL_MOUSEBUTTONDOWN: //Works even in pause mode
		{	
			mouseX = event.button.x; 
			mouseY = event.button.y;
			bMouseButtonDown = true;
			
			bool bAddPathPoint = false, bScreenScroll = false, bActorSelected = false;
			SDLMod keyMod = SDL_GetModState();
			actorButtonDown = NULL;
			
			GlDynArray<KrImage*> hittest;

			engine->Tree()->HitTest(event.button.x, event.button.y, (getGameMode() || (keyMod & KMOD_LCTRL))?KrImageTree::GET_ALL_HITS:0, &hittest); //use event.button.x for posted messages

			if(!getGameMode() && (keyMod & KMOD_RCTRL)) hittest.Clear();
			
			#ifndef STAND_ALONE_GAME
			if(currentTile && currentTile->IsVisible() && InModal(currentTile->getParentActor()))
			{
				//To add tranparent tiles
				if(currentTile->AddTile())
				{
					currentTile->setAdd(true);						
				}				
			} 
			else
			#endif
			if(hittest.Count() > 0)
			{
				if(!getGameMode() && (keyMod & KMOD_LCTRL)) itemSelect = (itemSelect + 1) % hittest.Count();
				else itemSelect = 0;
				
				//Notify actor
				KrImage *p = hittest.Item(itemSelect);
				Actor *actor = (Actor *)p->getParentActor();

				if(getGameMode())
				{
					//Don't catch clicks for actors with mouse button down event disable
					
					while(actor && !(actor->getEventControl() & EVENTMOUSEBUTTONDOWN))
					{
						itemSelect++;

						if(itemSelect >= hittest.Count())
						{
							actor = NULL;
							break;
						}

						p = hittest.Item(itemSelect);
						actor = (Actor *)p->getParentActor();
					}
				}
				
				if(actor && PathMode() && actor->EditMode())
				{
					bAddPathPoint = true;
				}
				else
				{						
					if(!InModal(actor))
					{
						actor = NULL;
					}
					
					if(actor)
					{						
						int x = mouseX, y = mouseY;
						if(actor->EditMode() || PathPoint::getNPaths()) GetAxis()->GridSnap(x, y);
						
						KrVector2T< GlFixed > object;
						actor->getImage()->ScreenToObject( x, y, &object );
						
						mouseActorDownX = object.x.ToIntRound();
						mouseActorDownY = object.y.ToIntRound();
						
						actorButtonDown = actor;
						bActorSelected = true;
						if(actor->OnMouseButtonDown(mouseX, mouseY, event.button.button))
						{
							//Drag enable
							actorDrag = actor;
						}
						
						if(!getGameMode() && actor->IsLocked())							
						{
							bScreenScroll = true;
						}
						
					}
				}
			}
			else if(PathMode() && event.button.button == 1)
			{					
				bAddPathPoint = true;
			}
			else
			{
				bScreenScroll = true;
			}
			
			if(bScreenScroll && !getGameMode() && !GetModal())
			{
				//Enable screen scroll
				actorDrag = GetAxis();
				
				KrVector2T< GlFixed > object;
				actorDrag->getImage()->ScreenToObject( mouseX, mouseY, &object );
				
				mouseActorDownX = (object.x * actorDrag->getImage()->XScale()).ToInt();
				mouseActorDownY = (object.y * actorDrag->getImage()->YScale()).ToInt();					

				if(!bActorSelected)
				{
					//Notify grid selection
					actorDrag->OnMouseButtonDown(mouseX, mouseY, event.button.button);
				}

			}
			
			if(bAddPathPoint && event.button.button == 1) 
			{
				//In path addition mode	
				GetAxis()->GridSnap(mouseX, mouseY);
				
				if(!pathPoint)
				{
					//Root path
					KrVector2T< GlFixed > object;
					GetAxis()->getImage()->ScreenToObject( mouseX, mouseY, &object );
					
					pathPoint = new PathPoint(object.x.ToInt(), object.y.ToInt());
				}
				else
				{
					KrVector2T< GlFixed > object;
					pathPoint->getRootPath()->getImage()->ScreenToObject( mouseX, mouseY, &object );
					
					int parentX = object.x.ToIntRound();
					int parentY = object.y.ToIntRound();
					pathPoint = pathPoint->AddPoint(parentX, parentY);
				}
				
				GetAxis()->getImage()->Invalidate();
			}

			#ifndef STAND_ALONE_GAME
			//Show actor coordinates
			if(mainActor) ((MainPanel *)mainActor)->ShowActorCoordinates(actorDrag);
			ReleaseActorTip(false);	
			if( !bGameMode && 
				actorButtonDown && 
				!actorButtonDown->getPanel() && 
				!Button::getHaveButtonPressed() &&
				!actorButtonDown->IsLocked()) 
			{
				//Don't push if is a button or panel
				UndoControl::Get()->PushCurrentState();
			}
			#endif //#ifndef STAND_ALONE_GAME
		}
		break;
		
	case SDL_MOUSEBUTTONUP: //Works even in pause mode
		{
			mouseX = event.button.x;
			mouseY = event.button.y;
			bMouseButtonDown = false;

			//bool bPushUndo = actorButtonDown != NULL;

			Actor::NotifyMouseButtonUp();

			#ifndef STAND_ALONE_GAME
			if(currentTile && currentTile->IsVisible() && InModal(currentTile->getParentActor()))
			{
				//To add tranparent tiles
				currentTile->setAdd(false);														
			} 
			else
			#endif
			if(actorDrag)
			{
				if(IS_VALID_ACTOR1(actorDrag)) actorDrag->OnMouseButtonUp(mouseX, mouseY, event.button.button);
				actorDrag = NULL; //Disable drag
				actorButtonDown = NULL;
			}
			else if(actorButtonDown /*&& bGameMode*/) //Send button up to actor which receive button down (On editor too)
			{
				if(IS_VALID_ACTOR1(actorButtonDown)) actorButtonDown->OnMouseButtonUp(mouseX, mouseY, event.button.button);
				actorButtonDown = NULL; 
			}
			else
			{
				
				GlDynArray<KrImage*> hittest;
				engine->Tree()->HitTest(event.button.x, event.button.y, 0, &hittest);
				
				if(hittest.Count() > 0)
				{
					//Notify actor
					KrImage *p = hittest.Item(0);
					Actor *actor = (Actor *)p->getParentActor();
					
					if(!InModal(actor))
					{
						actor = GetModal();
					}
					
					if(actor)
					{
						actor->OnMouseButtonUp(mouseX, mouseY, event.button.button);
					}
				}
				else
				{
					Actor *actor = GetModal();
					if(actor)
						actor->OnMouseButtonUp(mouseX, mouseY, event.button.button);
				}
			}

			#ifndef STAND_ALONE_GAME
			//Hide actor coordinates
			if(mainActor) ((MainPanel *)mainActor)->ShowActorCoordinates(actorDrag);
			#endif //#ifndef STAND_ALONE_GAME
		}
		break;
		
	case SDL_MOUSEMOTION:
		if(!bPauseGame)
		{
			mouseX = event.motion.x; 
			mouseY = event.motion.y;

			HandleMouseMotion();							
		}
		break;
#ifndef APPLICATION_THREAD_TIMERS
	case SDL_ACTOR_TIMER__EVENT:
		if(!bPauseGame)
		{
			//No multithread actor timer event
			MuteEx mutex(timerMutEx);

			Actor *actor = (Actor *)event.user.data2;
			if(IS_VALID_ACTOR1(actor)) //Valid actor?
			{
				stTimer *timer = (stTimer *)event.user.data1;
				
				if((*actor->getMapTimer())[timer]) //Valid timer?
				{
					Uint32 newInterval = 0;					
					newInterval = timer->actor->OnTimer(timer->timerID);					
					
					if(newInterval == 0)
					{
						//Remove timer
						SDL_RemoveTimer(timer->timerID);						
						timer->actor->RemoveTimer(timer);
					}					
				}
			}
		}
		break;
#endif

	case SDL_TIMER_EVENT:
		if(!bPauseGame)
		{
			//MuteEx mutex(renderMutEx);
			bCanAddFrameRateEvent = false; //Don't add other SDL_TIMER_EVENT event while process
			bCallGameTick = false;
						
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			ProcessNetwork(true); //It's not enought call only at frame tick, slow down the receives. But the call here too
			ProcessMessage();
			RemoveOldActorsFromCache();
			ProcessKeyDownRepeat(); 	
			ProcessActions();


			{
				//Real FPS calculation
				U32 currentTick = SDL_GetTicks();

				realFrameTimeInterval = currentTick - lastTick;
				realFrameRate = 1000.0/realFrameTimeInterval;

							
				if(bUseMotionCorrection)
				{
					//Sometimes, the ball get outside of a box in BumperGirls007.1.ged	
					//Tested with dropout bug.ged
					motionCorrection = realFrameTimeInterval / frameTimeInterval;

					//Don't use motion correction if realFrameTimeInterval < frameTimeInterval
					//Solve the bad look in asteroids.ged
					if(motionCorrection < 0.9) motionCorrection = 0.9;
				}
				else
				{
					motionCorrection = 1.0;
				}

				


#ifdef DEBUG
				/*{ 
				    //Fps debug
					static int n = 0;

					if(n == 0) remove("fps.txt");

					if(n > 2*realFrameRate)
					{
						FILE *arq = fopen("fps.txt", "a+");

						if(arq)
						{
							fprintf(arq, "fps: %ld (%ld ms)\n", realFrameRate, currentTick - lastTick);
							fclose(arq);
						}

						n = 0;
					}

					n++;
				}*/
#endif


				if(bGameMode && realFrameRate > fps) 
				{
					//To avoid flickering with animations with lower fps
					//Fix the MoonDefender.ged gun flick
					//Don't set in editor mode (EDITOR_FRAME_RATE may be > fps)
					realFrameRate = fps;
				}
				
				//Last 10 results average
				/*#define MAX_FPS_AVERAGE 90
				static int iBuf = 0;
				static int realFps[MAX_FPS_AVERAGE];

				realFps[iBuf] = 1000.0/(currentTick - tick);
				iBuf = (iBuf + 1) % MAX_FPS_AVERAGE;
				
				realFrameRate = 0;
				for(int i = 0; i < MAX_FPS_AVERAGE; i++)
				{
					realFrameRate += realFps[i];
				}				
				realFrameRate /= MAX_FPS_AVERAGE;*/

				lastTick = currentTick;

			}
			

			
			/*if(viewActor) //Bug in some games
			{
				//Get bounds: must be get here (can change along the game)
				rectView = viewActor->getImage()->Bounds();
				
				//Expand the rect to avoid flick when use working actors
				rectView.EdgeAdd(getViewSafeMargin());
			}*/

#ifndef USE_RTREE	
			for(ListActor **pList = mapActors.GetValueArray(); *pList; pList++)
			{
				ListActor *listActor = *pList;
				int nActorsInList = listActor->Count(), i = 0;
				//i < nActorsInList: solve the loop when create the same actor in the OnKeyDown event
				
				for(int il = 0; listActor && il < listActor->Count() && i < nActorsInList; il++)
				{				
					Actor *actor = (*listActor)[il];

#ifdef DEBUG
					GLASSERT(IS_VALID_ACTOR(actor));
#endif
					
					ProcessActor(actor);	
					i++;
				}					
			}	
#else
			/////////////////////////////////////////////////////////////////////
			//RTree search

			KrRect	rectView = GetViewActor()->getImage()->Bounds();
			
			int rMin[2], rMax[2];
			
			rMin[0] = rectView.xmin;
			rMin[1] = rectView.ymin;
			
			rMax[0] = rectView.xmax;
			rMax[1] = rectView.ymax;

			//Clean handledCollisionMap with visible actors only
			

			
			//Check out of vision for last visible actors
			if(bCheckOutOfVision)
			{
				for(int iActor = 0; iActor < visibleActors.Count(); iActor++)
				{
					Actor *actor = visibleActors[iActor];
					if(IS_VALID_ACTOR1(actor))
					{
						actor->CheckOutOfVision();
					}
				}
			}

			//Get visible actors
			visibleActors.ClearCount();
			rtree.Search(rMin, rMax, RTreeSearchCallback, NULL);

			//Add not optimized out of view actors

			//Process visible actors
			for(int iActor = 0; iActor < visibleActors.Count(); iActor++)
			{
				ProcessActor(visibleActors[iActor]);
			}
			/////////////////////////////////////////////////////////////////////
#endif

				//Handle the situation when the actor moves relative to the mouse (Mouse Enter Leave.ged)
				if(getGameMode())
				{
					HandleMouseMotion();
				}
								
				//The correct sequence is: move -> test collisions -> Draw?
				CheckCollisions(); //Flick when call PhysicalBounce
				
				

				//Move view after all events
				if(getGameMode() && viewActor)
				{
					MoveView();					
				}

				SetAnteriorPositions();
				
				if(!bGameMode || getShowMouse())
				{
					const stCursor *cursor = NULL;

					#ifndef STAND_ALONE_GAME
					if(WaitCursor::getWaitCursor())
					{
						cursor = WaitCursor::getWaitCursor();
					}
					else 
					#endif //#ifndef STAND_ALONE_GAME						
					if(currentActor && InModal(currentActor))
					{
						cursor = currentActor->OnGetCursor();

						//Actor Tip
						if(!bGameMode && !actorButtonDown) UpdateActorTip(currentActor);						
					}
					
					DrawCursor(engine, cursor, mouseX, mouseY);
				}

								
				if(bGameMode && bCheckOutOfVision) //Call only after the bCheckOutOfVision (fix the viewRegionBug.ged)
				{
					RegionLoad::UpdateView();

					//Ok with checkers.ged, but dont show the child actor in viewRegionBug.ged
					if(viewActor && !viewActor->getOnCreateCalled() && !viewActor->getCallOnCreate() && viewActor->getAction()) 
					{
						viewActor->getAction()->OnCreate(viewActor);						
					}
				}

				
				//Invalidate by position: problems with CollisionFree (calls Walk())
				/*GlInsideNodeIt<KrImNode*> itActor = engine->Tree()->Root()->ChildTreeIterator();
				itActor.Begin(); 
				while( !itActor.Done() )
				{
					itActor.CurrentData()->CheckInvalidPos();
					itActor.Next();
				}*/
				
#if !defined(GAME_EDITOR_PROFESSIONAL) && defined(STAND_ALONE_GAME)
		if(gameEditorInformation)
		{
			static int r = 0, g = 0, b = 0, rdir = 1, gdir = 1, bdir = 1;
			gameEditorInformation->SetColor(r, g, b);

			r += rdir*4;
			if(r > 255) {rdir *= -1; r = 255;}
			else if(r < 0) {rdir *= -1; r = 0;}

			g += gdir*2;
			if(g > 255) {gdir *= -1; g = 255;}
			else if(g < 0) {gdir *= -1; g = 0;}

			b += bdir*1;
			if(b > 255) {bdir *= -1; b = 255;}
			else if(b < 0) {bdir *= -1; b = 0;}

		}
#endif		


					engine->Draw();	
					CheckSurfaceLost();


#ifdef DEBUG_FPS
				fpsTotal += realFrameRate;
				fpsCount++;

				if(fpsCount > DEBUG_FPS)
				{
					FILE *arq = fopen("fps.txt", "a+");
					if(arq)
					{
						fprintf(arq, "fps: %ld\n", fpsTotal/fpsCount);
						fclose(arq);
					}

					fpsTotal = 0;
					fpsCount = 0;
				}
#endif

				//The tree is valid, can call CheckOutOfVision in the next loop
				bCheckOutOfVision = true; 
			
				
				
				//CheckCollisions();
				//SetAnteriorPositions();
				
				incGameFrame();
				SetMouse(mouseX, mouseY);

				

#ifndef STAND_ALONE_GAME
				//Kyra LGPL liscence
				if(startKyraSplash)
				{
					Uint32 now = SDL_GetTicks();
					engine->UpdateSplash(now);
					
					if(now - startKyraSplash >= 2000)
					{
						engine->EndSplash();
						startKyraSplash = 0;
					}
				}
#endif				
			}
			break;		
			
		default:
			break;
		}
		}
		else
		{
			//Avoid CPU consume when in minimized stand alone mode
			SDL_Delay(10);
		}

#if defined(GAME_EDITOR_PROFESSIONAL) && defined(WIN32) && !defined(STAND_ALONE_GAME) && !defined(_DEBUG)
	if(event.type)
	{
		NANOBEGIN

		//Update days check
		if(totalFrames > checkFrame)
		{
			Inf2();
		}

		NANOEND
	}
#endif


	if(bGameMode && engine && engine->Tree()->IsTreeInvalid())
	{
		//Solve Text Region.ged bug:
		//Destroy child of child of view after change the region
		engine->Tree()->Walk();
	}
	
	//Can add other time event
	//Put here to avoid missing frames due to crashs on draw cycle
	bCanAddFrameRateEvent = true; 	
	return true;
}

bool GameControl::CanDraw()
{
	////////////////////////////////////////////
	//Use this if use the TimerCallBack thread	
	if(!bCallGameTick) return false; 
	else return true;
	////////////////////////////////////////////

	//Use this without TimerCallBack
	//But don't get the user input in slow machines

	/*if(!bCanAddFrameRateEvent) return false;

	//Look the article "Real Time Game Loop Models for Single-Player Computer Games"
	//at http://guff.tigris.org/docs/SBGames05-en.pdf
	//or in the "Real Time Game Loop Models.pdf" local file

	static int next_time = 0, last_time = 0; 
	int cur_time = SDL_GetTicks();
	
	//From: http://www.mvps.org/directx/articles/writing_the_game_loop.htm
	int delta = cur_time - next_time;
	if(delta > 0)
	{
		if(!next_time) delta = 0;
		next_time += frameTimeInterval - delta;


		if (next_time < cur_time)
			next_time = cur_time + frameTimeInterval - delta;

		return true;
	}

	return false;*/
}

Actor * GameControl::ExistsClone(char *actorName, int cloneIndex)
{
	ListActor *listActor = mapActors.FindString(actorName);
	if(listActor)
	{
		for(int il = 0; il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->getCloneIndex() == cloneIndex)
			{
				return actor;
			}
		}	
	}

	return NULL;
}

bool GameControl::RequestNetworkStart() 
{
	if(!rakPeer && bAutoStartNetwork) InitRakNet(gameNetworkPort);
	return rakPeer != NULL;
}

int GameControl::InitRakNet(int port)
{	
#if GAME_EDITOR_VERSION < VERSION_NETWORK
	return -1;
#endif

	if(rakPeer) return currentListenPort;
	int requestedPort = port;

	rakPeer = RakNetworkFactory::GetRakPeerInterface();
	mapRemoteMousePos.Clear();
	mapRemoteLastKey.Clear();
	mapRequestPing.Clear();
	mapPendingConnection.Clear();
	mapConnections.Clear(); //Only clear the connections here
	mapConnectionLost.Clear();
	mapUnlinkedActors.Clear();
	mapActionQueue.Clear();
	bWaitingForConnectionComplete = false;
	
	// This is a global command which will cause sends of NetworkID to use the full SystemAddress / localSystemAddress component.
	// This way NetworkIDs can be created locally.
	NetworkID::SetPeerToPeerMode(true);	

	//To time syncronization
	rakPeer->SetOccasionalPing(true);

	//NAT
	//rakPeer->AttachPlugin(&natPunchthrough);

	/*
	// You have to attach ReplicaManager for it to work, as it is one of the RakNet plugins
	rakPeer->AttachPlugin(&replicaManager);

	// Anytime we get a new connection, call AddParticipant() on that connection
	replicaManager.SetAutoParticipateNewConnections(true);

	// Anytime we get a new participant, automatically call Construct() for them with all known objects
	replicaManager.SetAutoConstructToNewParticipants(true);

	// When an object goes in scope, serialize it
	replicaManager.SetAutoSerializeInScope(true);

	replicaManager.SetReceiveConstructionCB(NULL, Actor::ConstructionCB);

	// By default all objects are not in scope, meaning we won't serialize the data automatically when they are constructed
	// Calling this eliminates the need to call replicaManager.SetScope(this, true, systemAddress); in Replica::SendConstruction.
	replicaManager.SetDefaultScope(true);
	*/

	// The network ID manager maps numbers with pointers for object lookups
	rakPeer->SetNetworkIDManager(&networkIDManager);	

	//Current owner (will be the external address after the first connection)
	currentOwnerMachine = UNASSIGNED_SYSTEM_ADDRESS;
	//maks:tesste networkIDManager.SetExternalSystemAddress(currentOwnerMachine);

	//Start server
	//May be, only after check if the game need network

	int maxConnections = 32; //Will be controlled by the P2P topology
	
	SocketDescriptor socketDescriptor(port, 0); 

	//5 ms sleep to avoid 100% CPU usage
	bool bConnectionOk = false;
	bConnectionOk = rakPeer->Startup(maxConnections, 5, &socketDescriptor, 1);

	//rakPeer->SetTimeoutTime(30000, UNASSIGNED_SYSTEM_ADDRESS); //Only works with a specify address

	if(!bConnectionOk)
	{
		//Fail, try the surrounding ports		
		for(int i = 1; i < 65535; i++)
		{
			//Left
			port = requestedPort - i;

			if(port >= 1024 && port < 65535)
			{
				socketDescriptor.port = port;
				bConnectionOk = rakPeer->Startup(maxConnections, 5, &socketDescriptor, 1);
				if(bConnectionOk) break;
			}

			//Right
			port = requestedPort + i;

			if(port >= 1024 && port < 65535)
			{
				socketDescriptor.port = port;
				bConnectionOk = rakPeer->Startup(maxConnections, 5, &socketDescriptor, 1);
				if(bConnectionOk) break;
			}
		}
	}

	if(bConnectionOk)
	{
		currentListenPort = port;
		rakPeer->SetMaximumIncomingConnections(maxConnections); 

		#define BORADCAST_ADDR "255.255.255.255"

#ifdef _DEBUG
		//rakPeer->ApplyNetworkSimulator(4000, 500, 500);		
#endif

		//Try find to other peer in the local network		

		//In the assigned port
		rakPeer->Ping(BORADCAST_ADDR, port, true);

		//In the requested port
		if(port != requestedPort) rakPeer->Ping(BORADCAST_ADDR, requestedPort, true);

		//In the surrounding ports
		for(int i = requestedPort - 2; i <= requestedPort + 2; i++)
		{
			if(i != port && i != requestedPort)
			{
				rakPeer->Ping(BORADCAST_ADDR, i, true);
			}
		}

		CreateStringTable();

		if(bUseGameEditorServer) PublishGameSession();
	}
	else
	{
		DestroyRakNet();

#if !defined(STAND_ALONE_GAME)
		//Stop execution
		Script::SetError("Can't start the network.\nPlease, try choose other port in Game Properties panel.");

		SDL_Event event;
		memset(&event, 0, sizeof(SDL_Event));
		event.type = SDL_KEYUP;
		event.key.keysym.sym = SDLK_ESCAPE;
		SDL_PushEvent(&event);
#endif
	}

	return bConnectionOk?currentListenPort:-1;
}


bool GameControl::Connect( const char *host, unsigned short remotePort, bool bForceDirectConnection)
{
	bool res = false;

	/*NAT punch through

		- http://www.brynosaurus.com/pub/net/draft-ford-midcom-p2p-01.txt
		- http://www.mindcontrol.org/~hplus/nat-punch.html															
		- http://www.rakkarsoft.com/raknet/forum/index.php?topic=976.0
		- http://www.rakkarsoft.com/raknet/manual/Doxygen/classNatPunchthrough.html

		If use the class NatPunchthrough look the messages:
			ID_NAT_TARGET_NOT_CONNECTED
			ID_NAT_TARGET_CONNECTION_LOST
	*/

	SystemAddress addr;

	if(bForceDirectConnection)
	{
		//Just connect
		//The Connect function don't allow connect twice to the same remote system
		res = rakPeer->Connect(host, remotePort, 0, 0);

		//Remove ping request		
		addr.SetBinaryAddress(host);
		addr.port = remotePort;

		mapRequestPing.Remove(addr);

		if(!mapPendingConnection[addr]) //Test here to avoid endless tries
		{
			mapPendingConnection.Add(addr, SDL_GetTicks());
		}
	}
	else
	{
		//Connect indirectly by remote pong (NAT solution)	
		
		addr.SetBinaryAddress(host);
		addr.port = remotePort;

		mapRequestPing.Add(addr, SDL_GetTicks());
		
		//First ping now
		rakPeer->Ping(addr);

		res = true;
	}

	bWaitingForConnectionComplete = true;
	return res; 
}

void GameControl::PingThroughGameServer(const SystemAddress& addr)
{
	//Request to the game server send a command to the host telling the host to send a
	//ID_PONG to me (nat punch through)
}

void GameControl::SendIDToServer()
{
	//Build the message
	//Max: 512 bytes
	RakNet::BitStream out(512);	
	
	sprintf((char *)out.GetData(), "version=%ld&msgid=%ld&gameid=%s&clientid=%s", GAME_NET_VERSION, ID_SERVER_REGISTER, gameID.c_str(), clientID.c_str());
	out.SetWriteOffset(8*(strlen((char *)out.GetData())+1));

	//TODO: Send a UDP package here
	//rakPeer->SendTo(gameEditorServer, &out);
}

void GameControl::PublishGameSession()
{
	//Tell to the game server the current game address
	/*
	Return file layout:

		Signature:	4 bytes
		n hots:		4 bytes
		IP:			4 bytes
		Port:		2 bytes
	*/

	return; //maks:teste

	//Qual é meu IP?
	//Qual é minha porta externa?

	//Get IP address for game-editor.com
	if(gameEditorServer == UNASSIGNED_SYSTEM_ADDRESS)
	{
		const char *serverIP = SocketLayer::Instance()->DomainNameToIP("game-editor.com");

		if ( serverIP )
		{
			gameEditorServer.SetBinaryAddress(serverIP);
			gameEditorServer.port = 2055;
		}
		else
		{
			//Can't find the server
			return;
		}
	}

	if(clientID.empty())
	{
		//Generate now (session id ?)
		clientID = GenerateUniqueID();
	}

	if(clientID.size() > MAX_CLIENT_ID)
	{
		clientID = clientID.substr(0, MAX_CLIENT_ID);
	}

	//Send UDP
	SendIDToServer();
	

	/*long bytesRead;
	U32 nHosts = 0;
	char *pBuf = NULL;
	gedString query("http://game-editor.com/server/login.php?");

	query += "version=";
	query += gedString(GAME_NET_VERSION);
	query += "1&";
	query += "game="; query += gameID; query += "&";	
	query += "port="; query += gedString((long)port); query += "&";

	for(int i = 0; i < 30; i++)
	{
		pBuf = download(query.c_str(), &bytesRead); 

		if(pBuf)
		{
			if(bytesRead > 4 && memcmp(pBuf, "HLST", 4) == 0)
			{
				break;
			}
			else
			{
				free(pBuf);
				pBuf = NULL;
			}
		}
		
		//Wait before retry
		SDL_Delay(1000);		
	}

	//Read the host list
	SDL_RWops *src = SDL_RWFromMem(pBuf, bytesRead);

	if(src)
	{
		//magic
		Read32(src); 

		//n hosts
		nHosts = Read32(src);

		for(int i = 0; i < nHosts; i++)
		{
			U32 ip = Read32(src);
			U16 port = Read16(src);
			SystemAddress addr;

			addr.binaryAddress = ip;
			addr.port = port;

			Connect(addr.ToString(), port);
		}

		SDL_RWclose(src);
	}

	free(pBuf);*/
	
}

void GameControl::CreateStringTable()
{
	//Create the actor name string table to speed up the communications
	return; //False duplicated assert in the RakNet. Uses when get a version fix
	if(rakPeer)
	{
		MapActorFileIndexIterator itActor(actorIndex);
		int n = 0, maxStrings = (StringTableType)-1;
		for(itActor.Begin(); !itActor.Done(); itActor.Next())
		{
			gedString *pName = itActor.Key();
			if(!pName->hasPoint() && n < maxStrings)
			{
				//Only add actor's name
				RakNet::StringTable::Instance()->AddString(pName->getCharBuf(), true);	
				n++;
			}
		}		
	}
}

void GameControl::DestroyRakNet()
{
	if(rakPeer)
	{
		rakPeer->Shutdown(2000, 0);	
		RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
		rakPeer = NULL;
		bWaitingForConnectionComplete = false;
	}

	for(int i = 0; i < pendingActions.Count(); i++)
	{			
		delete pendingActions[i];		
	}

	pendingActions.Clear();
}



void GameControl::SendRemoteMousePosition(bool bReliableSend)
{
	//Send the local mouse position to all connected machines
	//Relative to the axis
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	RakNetTime timeStamp = RakNet::GetTime();

	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_MOUSE_POSITION);

	//Position in axis coordinates
	KrVector2T< GlFixed > object;
	GetAxis()->getImage()->ScreenToObject( getMouseX(), getMouseY(), &object );

	out.Write(object.x.ToInt());
	out.Write(object.y.ToInt());

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteMousePosition: %ld, %ld\n", object.x.ToInt(), object.y.ToInt());
#endif
	
	//Send the message
	//Can ignore old messages
	//Use a different channel to ordering
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
    //rakPeer->Send(&out, MEDIUM_PRIORITY, bReliableSend?RELIABLE_SEQUENCED:UNRELIABLE_SEQUENCED, NET_CHANNEL_MOUSE, networkIDManager.GetExternalSystemAddress(), true);
}

bool GameControl::ExecuteRemoteMousePosition(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute the position based on a remote machine
	//x, y in axis coordinates

	int x, y;

	///////////////////////////////////////////////////
	//Read the message
				
	//Position
	in.Read(x);
	if(!in.Read(y))
	{
		//Read error
		//Only need to test the last read
		//Return true to discard this packet
		return true;
	}

	///////////////////////////////////////////////////
	//Execute

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteRemoteMousePosition: %ld, %ld (%s)\n", x, y, addr.ToString(true));
#endif

	//To local screen coordinate
	KrVector2T< GlFixed > screen;
	GetAxis()->getImage()->ObjectToScreen( x, y, &screen );
	KrVector2 pos;
	pos.Set(screen.x.ToInt(), screen.y.ToInt());

	mapRemoteMousePos.Add(addr, pos);
	return true;
}

void GameControl::GetMousePosition(int &mousex, int &mousey, const SystemAddress& owner)
{
	//Get the mouse position in the owner machine
	if(!rakPeer || owner == networkIDManager.GetExternalSystemAddress() || owner == UNASSIGNED_SYSTEM_ADDRESS)
	{
		//Local control
		mousex = getMouseX();
		mousey = getMouseY();

		//Update the remote machines
		static int localMouseX = -1, localMouseY = -1;
		static bool bSendReliable = false;

		if(mousex != localMouseX || mousey != localMouseY)
		{
			localMouseX = mousex;
			localMouseY = mousey;

			SendRemoteMousePosition(false);
			bSendReliable = true;
		}
		else if(bSendReliable)
		{
			//Mouse stopped, so send the last position reliable
			SendRemoteMousePosition(true);
			bSendReliable = false;
		}
	}
	else
	{
		//Get remote mouse position
		KrVector2 *pPos = mapRemoteMousePos[owner];
		if(pPos)
		{
			mousex = pPos->x;
			mousey = pPos->y;
		}
		else
		{
			//Unknow owner, get local position
			mousex = getMouseX();
			mousey = getMouseY();
		}
	}
}

void GameControl::SendRemoteLastKey()
{
	//Send the local mouse position to all connected machines
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	RakNetTime timeStamp = RakNet::GetTime();

	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_LAST_KEY);

	//Key
	out.Write((int)lastKey);

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteLastKey: %ld\n", lastKey);
#endif
	
	//Send the message
	//Can ignore old messages
	//Use the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
    //rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_SEQUENCED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
}

bool GameControl::ExecuteRemoteLastKey(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute the position based on a remote machine

	int key;

	///////////////////////////////////////////////////
	//Read the message
				
	//Key	
	if(!in.Read(key))
	{
		//Read error
		//Only need to test the last read
		//Return true to discard this packet
		return true;
	}

	///////////////////////////////////////////////////
	//Execute

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteRemoteLastKey: %ld (%s)\n", key, addr.ToString(true));
#endif

	mapRemoteLastKey.Add(addr, (SDLKey)key);
	return true;
}


SDLKey GameControl::GetLastKey(const SystemAddress& owner) 
{
	//Get the lastkey in the owner machine
	if(!rakPeer || owner == networkIDManager.GetExternalSystemAddress() || owner == UNASSIGNED_SYSTEM_ADDRESS)
	{
		//Local control
		//Update the remote machines
		static SDLKey localLastKey = SDLK_UNKNOWN;
		
		if(lastKey != localLastKey)
		{
			localLastKey = lastKey;
			SendRemoteLastKey();
		}
	}
	else
	{
		//Get remote lastkey
		SDLKey *pKey = mapRemoteLastKey[owner];
		if(pKey)
		{
			return *pKey;
		}	
	}

	return lastKey;
}



void GameControl::SendRemotePosition(Actor *actor, float x, float y, bool bReliableSend)
{
	//Send the position to all connected machines
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}	

	RakNetTime timeStamp = RakNet::GetTime();
	
	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_SET_POSITION);

	//Actor
	RakNet::StringTable::Instance()->EncodeString(actor->getActorName(), ACTOR_NAME, &out);
	out.Write(actor->getCloneIndex());

	//Position
	out.Write(x);
	out.Write(y);

#ifdef DEBUG
	GLOUTPUT("+++ SendRemotePosition: %s, %.2f, %f.2\n", actor->getCloneName(), x, y);
#endif
	
	
	//Always send in the action channel to avoid erros in some actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
	//rakPeer->Send(&out, HIGH_PRIORITY, bReliableSend?RELIABLE_ORDERED:UNRELIABLE_SEQUENCED, bReliableSend?NET_CHANNEL_ACTIONS:NET_CHANNEL_POSITION, networkIDManager.GetExternalSystemAddress(), true);
}

bool GameControl::ExecuteRemotePosition(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute the position based on a remote machine

	char eventActorName[ACTOR_NAME];
	long eventActorCloneIndex;
	float x, y;

	///////////////////////////////////////////////////
	//Read the message
	
	//Actor
	RakNet::StringTable::Instance()->DecodeString(eventActorName, ACTOR_NAME, &in);
	in.Read(eventActorCloneIndex);

			
	//Position
	in.Read(x);
	if(!in.Read(y))
	{
		//Read error
		//Only need to test the last read
		//Return true to discard this packet
		return true;
	}

	///////////////////////////////////////////////////
	//Execute

	//Try get the event actor (don't allow load?)
	Actor *actor = GetNetActor(addr, eventActorName, eventActorCloneIndex);
	if(!actor) 
	{
		//Can't execute the action
		//Can try again later
		return false; 
	}

	if(!actor->getNetActor() || actor->GetOwner() == addr)
	{
#ifdef DEBUG
		GLOUTPUT("+++ ExecuteRemotePosition: %s, %.2f, %f.2 (%s)\n", actor->getCloneName(), x, y, addr.ToString(true));
#endif

		actor->SetPos(x, y);
	}

	return true;
}

void GameControl::SendRemoteFrame(Actor *actor, bool bReliableSend)
{
	//Send the animpos to all connected machines
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}	

	RakNetTime timeStamp = RakNet::GetTime();
	
	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_SET_FRAME);

	//Actor
	RakNet::StringTable::Instance()->EncodeString(actor->getActorName(), ACTOR_NAME, &out);
	out.Write(actor->getCloneIndex());

	//Frame
	out.Write((float)actor->getScriptVars()->animpos);
	

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteFrame: %s, %.2f\n", actor->getCloneName(), actor->getScriptVars()->animpos);
#endif
	
	//Send the message
	//Always send in the action channel to avoid erros in some actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
	//rakPeer->Send(&out, HIGH_PRIORITY, bReliableSend?RELIABLE_ORDERED:UNRELIABLE_SEQUENCED, NET_CHANNEL_FRAME, networkIDManager.GetExternalSystemAddress(), true);
}

bool GameControl::ExecuteRemoteFrame(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute the frame change based on a remote machine

	char eventActorName[ACTOR_NAME];
	long eventActorCloneIndex;
	float animpos;

	///////////////////////////////////////////////////
	//Read the message
	
	//Actor
	RakNet::StringTable::Instance()->DecodeString(eventActorName, ACTOR_NAME, &in);
	in.Read(eventActorCloneIndex);

			
	//Frame	
	if(!in.Read(animpos))
	{
		//Read error
		//Only need to test the last read
		//Return true to discard this packet
		return true;
	}

	///////////////////////////////////////////////////
	//Execute

	//Try get the event actor (don't allow load?)
	Actor *actor = GetNetActor(addr, eventActorName, eventActorCloneIndex);
	if(!actor) 
	{
		//Can't execute the action
		//Can try again later
		return false; 
	}

	
	if(!actor->getNetActor() || actor->GetOwner() == addr)
	{		
#ifdef DEBUG
		GLOUTPUT("+++ ExecuteRemoteFrame: %s, %.2f, (%s)\n", actor->getCloneName(), animpos, addr.ToString(true));
#endif

		actor->SetFrame(animpos);
	}
	
	return true;
}

void GameControl::SendRemoteAction(
		    int sendOrder,
			U16 actionNumber, const gedString& actionName, 
			const gedString& eventActorName, long eventActorCloneIndex, const RakNetTime &eventActorOwnershipTimestamp,
			const gedString& collideActorName, long collideActorCloneIndex,
			bool bExecuteConditional, bool bForceExecute,
			unsigned int seed
			)
{
	//Send the action to all connected machines
	//If is a Inherited Action, eventActorName will be different from actionName
	//So, send both here
	//sendOrder = 0 -> send first

	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	RakNetTime timeStamp = RakNet::GetTime();

	//Build the message
	RakNet::BitStream *out = new RakNet::BitStream;
	if(!out) return;

	//Timestamp
	out->Write((unsigned char)ID_TIMESTAMP);
	out->Write(timeStamp);

	//Message type
	out->Write((unsigned char)ID_USER_EXECUTE_ACTION);

	bool bSendRand = seed != 0?true:false;	

	//Action number
	out->Write(actionNumber);

	//Action Name
	RakNet::StringTable::Instance()->EncodeString(actionName.getCharBuf(), ACTOR_NAME, out);

	//Event Actor
	RakNet::StringTable::Instance()->EncodeString(eventActorName.getCharBuf(), ACTOR_NAME, out);
	out->Write(eventActorCloneIndex);
	out->Write(eventActorOwnershipTimestamp);

	//Collide Actor
	if(collideActorCloneIndex >= 0)
	{
		out->Write(collideActorCloneIndex);
		RakNet::StringTable::Instance()->EncodeString(collideActorName.getCharBuf(), ACTOR_NAME, out);		
	}
	else
	{
		out->Write(collideActorCloneIndex);
	}

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteAction: %s (%ld), %s.%ld\n", actionName.getCharBuf(), actionNumber, eventActorName.getCharBuf(), eventActorCloneIndex);
#endif
	

	//Flags
	out->Write(bExecuteConditional);
	out->Write(bForceExecute);
	out->Write(bSendRand);
	if(bSendRand) out->Write(seed);
	
	//Store to send
	pendingActions[sendOrder] = out;

	//Can send now?
	if(sendOrder == 0)
	{
		//Send all messages
		RakNet::BitStream *p;

		for(int i = 0; i < pendingActions.Count(); i++)
		{			
			p = pendingActions[i];

			if(p)
			{
				rakPeer->Send(p, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
				delete p;
				pendingActions[i] = NULL;
			}
		}

		pendingActions.ClearCount();
	}
}


bool GameControl::ExecuteRemoteAction(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute action from a remote machine

	U16 actionNumber;
	char actionName[ACTOR_NAME], eventActorName[ACTOR_NAME], collideActorName[ACTOR_NAME];
	long eventActorCloneIndex, collideActorCloneIndex;
	RakNetTime eventActorOwnershipTimestamp;
	bool bExecuteConditional, bForceExecute, bReceiveSeed;
	unsigned int seed = 0;

	///////////////////////////////////////////////////
	//Read the message
	
	//Action number
	in.Read(actionNumber);

	//Action Name
	RakNet::StringTable::Instance()->DecodeString(actionName, ACTOR_NAME, &in);

	//Event Actor
	RakNet::StringTable::Instance()->DecodeString(eventActorName, ACTOR_NAME, &in);
	in.Read(eventActorCloneIndex);
	in.Read(eventActorOwnershipTimestamp);

	//Collide Actor
	in.Read(collideActorCloneIndex);
	if(collideActorCloneIndex >= 0) RakNet::StringTable::Instance()->DecodeString(collideActorName, ACTOR_NAME, &in);
		
	//Flags
	in.Read(bExecuteConditional);
	in.Read(bForceExecute);
	if(!in.Read(bReceiveSeed))
	{
		//Read error
		//Only need to test the last read
		//Return true to discard this packet
		return true;
	}

	if(bReceiveSeed) in.Read(seed);

	///////////////////////////////////////////////////
	//Execute	

#ifdef _DEBUG
	GLOUTPUT("+++ ExecuteRemoteAction: %s (%ld), %s.%ld (%s)\n", actionName, actionNumber, eventActorName, eventActorCloneIndex, addr.ToString(true));
#endif

	return ExecuteAction(addr,
			actionNumber, actionName, 
			eventActorName, eventActorCloneIndex, eventActorOwnershipTimestamp,
			collideActorName, collideActorCloneIndex,
			bExecuteConditional, bForceExecute,
			true,
			seed);
}

Actor *GameControl::GetNetActor(const SystemAddress &owner, const char *actorName, long cloneIndex)
{
	gedString actorCloneName(actorName);
	actorCloneName += '.';
	actorCloneName += gedString(cloneIndex);

	Actor *actor = GetActor(actorCloneName, true, false, false);

	if(!actor)
	{
		while(!actor)
		{
			//Try from the unlinked list
			actor = GetActorFromUnlinkedList(owner, actorName);
			if(!actor) 
			{
				//No actor in unlinked list for this owner
				return NULL;
			}

			if(IS_VALID_ACTOR1(actor))
			{
				//Actor is valid. Set the same index from the owner (link)				
				actor->SetCloneIndex(cloneIndex);
				break;
			}
		}
	}	

	if(actor && actor->getUnlinked())
	{
		//Clear unlinked
		ListActor *listActor = mapUnlinkedActors[owner];

		if(listActor)
		{
			listActor->FindAndDelete(actor);

			if(listActor->size() == 0)
			{
				mapUnlinkedActors.Remove(owner);
			}
		}

		actor->setUnlinked(false);
	}

	return actor;
}

Actor *GameControl::GetActorFromUnlinkedList(const SystemAddress &owner, const char *actorName)
{
	ListActor *listActor = mapUnlinkedActors[owner];
	Actor *actor = NULL, *p = NULL;

	if(listActor)
	{
		for(int i = 0; i < listActor->Count(); i++)
		{
			p = (*listActor)[i];
			if(IS_VALID_ACTOR1(p) && strcmp(p->getActorName(), actorName) == 0)
			{
				actor = p;
				listActor->FindAndDelete(actor);
				break;
			}
		}

		if(listActor->Count() == 0)
		{
			mapUnlinkedActors.Remove(owner);
		}
	}

	return actor;
}

void GameControl::RemoveActorFromUnlinkedList(Actor *actor)
{
	GlMapIterator<SystemAddress, ListActor, GlSystemAddressHash> it(mapUnlinkedActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		while(listActor->FindAndDelete(actor));
	}
}

void GameControl::AddToUnlinkedList(const SystemAddress &owner, Actor *actor)
{
	//This actor has been created by the remote machine, but I don't know
	//the right clone name now. So, put on the unlinked list and wait for the first
	//remote command

	if(!actor->getUnlinked())
	{
		actor->setUnlinked(true);
		mapUnlinkedActors.FindOrCreate(owner)->PushBack(actor);
	}
}




SystemAddress GameControl::GetOwnerMachine()
{
	return currentOwnerMachine;
}


void GameControl::SetExternalSystemAddress(const SystemAddress& addr)
{
	SystemAddress oldAddr = networkIDManager.GetExternalSystemAddress();

	if(addr != oldAddr || oldAddr == UNASSIGNED_SYSTEM_ADDRESS)
	{
		//The local system have your adress changed
		//Update running actors

#ifdef DEBUG
		GLOUTPUT("+++ SetExternalSystemAddress: %s to %s\n", oldAddr.ToString(true), addr.ToString(true));
#endif

		//Set new address
		networkIDManager.SetExternalSystemAddress(addr);
		currentOwnerMachine = addr;

		//Update actors
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];

				if(IS_VALID_ACTOR1(actor) && actor->getRunning())
				{
					if(actor->UpdateOwner(oldAddr, addr))
					{
						//Notify the remote machines about onership of this actor
						RequestActorOwnership(actor->getCloneName());
					}
				}			
			}
		}

		//Update queue actions
		QueueAction *pQueue = mapActionQueue[oldAddr];
		if(pQueue)
		{
			QueueAction copy = *pQueue;
			mapActionQueue.Remove(oldAddr);
			mapActionQueue.Add(addr, copy);
		}

		//Notify the remote machines about the address change
		if(oldAddr != UNASSIGNED_SYSTEM_ADDRESS) SendRemoteAdressUpdate(oldAddr);
	}
}

void GameControl::SendRemoteAdressUpdate(const SystemAddress &oldAddr)
{
	//Send the address update all connected machines

	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	RakNetTime timeStamp = RakNet::GetTime();

	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_ADDRESS_CHANGE);

	//Old address
	out.Write(oldAddr);

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteAdressUpdate: %s to %s\n", oldAddr.ToString(true), currentOwnerMachine.ToString(true));
#endif
	
	//Send the message
	//Same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);	
}

bool GameControl::ExecuteRemoteAdressUpdate(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute address update from a remote machine
	SystemAddress oldAddr;	

	///////////////////////////////////////////////////
	//Read the message
	
	//Old address	
	in.Read(oldAddr);
	
	///////////////////////////////////////////////////
	//Execute

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteRemoteAdressUpdate: %s to %s\n", oldAddr.ToString(true), addr.ToString(true));
#endif

	//Update running actors
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];

			if(IS_VALID_ACTOR1(actor) && actor->getRunning())
			{
				actor->UpdateOwner(oldAddr, addr);					
			}			
		}
	}

	//Update queue actions
	QueueAction *pQueue = mapActionQueue[oldAddr];
	if(pQueue)
	{
		QueueAction copy = *pQueue;
		mapActionQueue.Remove(oldAddr);
		mapActionQueue.Add(addr, copy);
	}

	return true;
}


bool GameControl::ReleaseActorOwnership(const char *cloneName)
{
	/*
	Release the current owner of the network actor, so other machine can request the ownership.
	If the local machine is not the owner, the function will be ignored.
				
	cloneName: nameactor.cloneindex (Ex.: ship.1, ship.2, ...), "Event Actor", "Collide Actor", "Parent Actor" or "Creator Actor"
	Return 1 if success, 0 on error.
	*/

	//Let the user release the ownership even if is not connected

	Actor *actor = GetActor(cloneName, true, false, false);
	if(actor <= COLLIDE_ACTOR) return false; 
	
	if(actor->ReleaseOwnership())
	{
		//Send to the remote machines
		SendRemoteReleaseActorOwnership(actor->getActorName(), actor->getCloneIndex());

		return true;
	}

	return false;
}

bool GameControl::RequestActorOwnership(const char *cloneName)
{
	/*
	Request ownership for the specified network actor.   
	If the network actor already has a owner, the request will fail.
				  
	cloneName: nameactor.cloneindex (Ex.: ship.1, ship.2, ...), "Event Actor", "Collide Actor", "Parent Actor" or "Creator Actor"
	Return 1 if success, 0 on error.
	*/

	//Let the user reques the ownership even if is not connected

	Actor *actor = GetActor(cloneName, true, false, false);
	if(actor <= COLLIDE_ACTOR) return false; 

	
	if(actor->RequestActorOwnership(networkIDManager.GetExternalSystemAddress(), RakNet::GetTime()))
	{
		//Send to the remote machines
		SendRemoteRequestActorOwnership(actor->getActorName(), actor->getCloneIndex(), actor->GetOwnershipTimestamp());

		return true;
	}

	return false;
}

void GameControl::SendRemoteReleaseActorOwnership(const char *actorName, long cloneIndex)
{
	//Send the ownership release to all connected machines

	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	RakNetTime timeStamp = RakNet::GetTime();

	//Build the message
	RakNet::BitStream out;

	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_RELEASE_OWNERSHIP);


	//Actor
	RakNet::StringTable::Instance()->EncodeString(actorName, ACTOR_NAME, &out);
	out.Write(cloneIndex);

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteReleaseActorOwnership: %s.%ld\n", actorName, cloneIndex);
#endif
	
	//Send the message
	//Same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);	
}

bool GameControl::ExecuteRemoteReleaseActorOwnership(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute ownership release from a remote machine
	char actorName[ACTOR_NAME];
	long cloneIndex;	

	///////////////////////////////////////////////////
	//Read the message
	
	//Actor
	RakNet::StringTable::Instance()->DecodeString(actorName, ACTOR_NAME, &in);
	in.Read(cloneIndex);
	
	///////////////////////////////////////////////////
	//Execute

	//Try get the actor
	gedString actorCloneName(actorName);
	actorCloneName += '.';
	actorCloneName += gedString(cloneIndex);

	Actor *actor = GetActor(actorCloneName, true, false, false);
	if(!actor)  
	{
		//Can't execute the action
		//Can try again later
		return false; 
	}
		
	//Set the owner machine for this action
	SystemAddress currenOwner = currentOwnerMachine;
	currentOwnerMachine = addr;

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteRemoteReleaseActorOwnership: %s.%ld (%s)\n", actorName, cloneIndex, addr.ToString(true));
#endif

	//Release
	actor->ReleaseOwnership();

	//Restore owner machine
	currentOwnerMachine = currenOwner;

	return true;
}


void GameControl::SendRemoteRequestActorOwnership(const char *actorName, long cloneIndex, const RakNetTime &timeStamp, SystemAddress addr)
{
	//Send the ownership request to all connected machines

	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return;
	}

	//Build the message
	RakNet::BitStream out;
	
	//Timestamp
	out.Write((unsigned char)ID_TIMESTAMP);
	out.Write(timeStamp);

	//Message type
	out.Write((unsigned char)ID_USER_REQUEST_OWNERSHIP);


	//Actor
	RakNet::StringTable::Instance()->EncodeString(actorName, ACTOR_NAME, &out);
	out.Write(cloneIndex);

#ifdef DEBUG
	GLOUTPUT("+++ SendRemoteRequestActorOwnership: %s.%ld (to %s)\n", actorName, cloneIndex, addr.ToString(true));
#endif
	
	//Send the message
	//Same channel of the actions
	if(addr == UNASSIGNED_SYSTEM_ADDRESS)
	{
		//Broadcast
		rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, networkIDManager.GetExternalSystemAddress(), true);
	}
	else
	{
		//Send only to the specified machine
		rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, addr, false);
	}
}

bool GameControl::ExecuteRemoteRequestActorOwnership(RakNet::BitStream &in, RakNetTime &timeStamp, SystemAddress &addr)
{
	//Execute ownership request from a remote machine
	char actorName[ACTOR_NAME];
	long cloneIndex;	

	///////////////////////////////////////////////////
	//Read the message

	//Actor
	RakNet::StringTable::Instance()->DecodeString(actorName, ACTOR_NAME, &in);
	in.Read(cloneIndex);
	
	///////////////////////////////////////////////////
	//Execute

	//Try get the actor
	gedString actorCloneName(actorName);
	actorCloneName += '.';
	actorCloneName += gedString(cloneIndex);

	Actor *actor = GetActor(actorCloneName, true, false, false);
	if(!actor) 
	{
		//Can't execute the action
		//Can try again later
		return false; 
	}

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteRemoteRequestActorOwnership: %s.%ld (%s)\n", actorName, cloneIndex, addr.ToString(true));
#endif
		
	if(!actor->RequestActorOwnership(addr, timeStamp) && actor->GetOwner() == GetOwnerMachine())
	{
		//The local machine is the oldest owner of the actor
		//so, send a owner request back to the remote machine

		SendRemoteRequestActorOwnership(actor->getActorName(), actor->getCloneIndex(), actor->GetOwnershipTimestamp(), addr);
	}

	return true;
}

bool GameControl::ExecuteTimeStampedMessages(Packet *packet)
{
	//Time stamped messages
	unsigned char msgType;
	RakNetTime timeStamp;
	bool bProcessed = true;

	///////////////////////////////////////////////////
	//Read the message
	RakNet::BitStream in(packet->data, packet->length, false);

	//Already decoded (ID_TIMESTAMP)
	//Read here to increase data pointer	
	in.Read(msgType);

	//Read timestamp
	in.Read(timeStamp);

	//Read msg type
	in.Read(msgType);

	switch(msgType)
	{
	case ID_USER_REQUEST_OWNERSHIP:
		bProcessed = ExecuteRemoteRequestActorOwnership(in, timeStamp, packet->systemAddress);
		break;

	case ID_USER_MOUSE_POSITION:
		bProcessed = ExecuteRemoteMousePosition(in, timeStamp, packet->systemAddress);
		break;

	case ID_USER_LAST_KEY:
		bProcessed = ExecuteRemoteLastKey(in, timeStamp, packet->systemAddress);
		break;

	case ID_USER_SET_POSITION:
		bProcessed = ExecuteRemotePosition(in, timeStamp, packet->systemAddress);
		break;

	case ID_USER_SET_FRAME:
		bProcessed = ExecuteRemoteFrame(in, timeStamp, packet->systemAddress);
		break;

	case ID_USER_EXECUTE_ACTION:
		bProcessed = ExecuteRemoteAction(in, timeStamp, packet->systemAddress);			
		break;

	case ID_USER_ADDRESS_CHANGE:
		bProcessed = ExecuteRemoteAdressUpdate(in, timeStamp, packet->systemAddress);						
		break;

	case ID_USER_RELEASE_OWNERSHIP:
		bProcessed = ExecuteRemoteReleaseActorOwnership(in, timeStamp, packet->systemAddress);
		break;	

	default:
		{
#ifdef DEBUG
			GLOUTPUT("+++ Unknow timestamp message from %s failed.\n", packet->systemAddress.ToString(true));
#endif
		}
		break;
	}

	return bProcessed;
}

////////////////////////////////////////////////////////////////////
//Handshake
bool GameControl::HandleNewConnection(SystemAddress &addr)
{
	//1. Remote machine wants connect
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return false;
	}

	//Build the message
	RakNet::BitStream out;

	//Message type
	out.Write((unsigned char)ID_USER_ENVIRONMENT_INFO_REQUEST);


	//Send only to the specified machine
	//In the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, addr, false);

#ifdef DEBUG
	GLOUTPUT("+++ HandleNewConnection: %s\n", addr.ToString(true));
#endif

	return true;
}

bool GameControl::HandleEnvironmentInfoRequest(SystemAddress &addr)
{
	//2. Send the environment to the remote machine
	if(!rakPeer || networkIDManager.GetExternalSystemAddress() == UNASSIGNED_SYSTEM_ADDRESS) 
	{
		//System not initialized or no connections present
		return false;
	}

	//Build the message
	RakNet::BitStream out;

	//Message type
	out.Write((unsigned char)ID_USER_ENVIRONMENT_INFO);

	//Info
	RakNet::StringTable::Instance()->EncodeString(getGameID().c_str(), 0, &out);
	out.Write(getGameVersion());


	//Send only to the specified machine
	//In the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, addr, false);

#ifdef DEBUG
	GLOUTPUT("+++ HandleEnvironmentInfoRequest: %s\n", addr.ToString(true));
#endif

	return true;
}


bool GameControl::HandleEnvironmentInfo(Packet *packet)
{	
	unsigned char msgType;
	char remoteGameID[MAX_GAME_ID+1];
	U32 remoteGameVersion;

	///////////////////////////////////////////////////
	//Read the message
	RakNet::BitStream in(packet->data, packet->length, false);
	
	//Read here to increase data pointer	
	in.Read(msgType);

	//Read Info
	RakNet::StringTable::Instance()->DecodeString(remoteGameID, MAX_GAME_ID+1, &in);
	in.Read(remoteGameVersion);

	if(gedString(remoteGameID) != getGameID())
	{
		//Different games, disconnect
		rakPeer->CloseConnection(packet->systemAddress, true);
		return true;
	}

	if(remoteGameVersion > getGameVersion())
	{
		//Remote machine have a new version, just reject.
		rakPeer->CloseConnection(packet->systemAddress, true);

#ifdef DEBUG
		GLOUTPUT("+++ HandleEnvironmentInfo, CloseConnection: %s\n", packet->systemAddress.ToString(true));
#endif

		return true;
	}
	else if(remoteGameVersion < getGameVersion())
	{
		//Remote machine have a old version, send notification

		//Build the message
		RakNet::BitStream out;

		//Message type
		out.Write((unsigned char)ID_USER_OLD_GAME_VERSION);


		//Send only to the specified machine
		//In the same channel of the actions
		rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, packet->systemAddress, false);
		rakPeer->CloseConnection(packet->systemAddress, true);

#ifdef DEBUG
		GLOUTPUT("+++ HandleEnvironmentInfo, Old Game: %s\n", packet->systemAddress.ToString(true));
#endif

		return true;
	}

	//Ok, send the green flag!
	
	//Build the message
	RakNet::BitStream out;

	//Message type
	out.Write((unsigned char)ID_USER_HANDSHAKE_FINISHED);

	//Send only to the specified machine
	//In the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, packet->systemAddress, false);

	//Connection done!
	mapPendingConnection.Remove(packet->systemAddress);
	mapConnections.Add(packet->systemAddress, 1);

#ifdef DEBUG
	GLOUTPUT("+++ HandleEnvironmentInfo: %s\n", packet->systemAddress.ToString(true));
#endif

	return true;
}

bool GameControl::HandleHandShakeFinished(Packet *packet)
{
	//Hand shake is ok
	//Request game state?

	unsigned char msgType;

	

	///////////////////////////////////////////////////
	//Read the message
	RakNet::BitStream in(packet->data, packet->length, false);
	
	//Read here to increase data pointer	
	in.Read(msgType);

	if(rakPeer->NumberOfConnections() <= 1) 
	{
		//First connection, request the game state
		
		RakNet::BitStream out;

		//Message type
		out.Write((unsigned char)ID_USER_REQUEST_GAME_STATE);

		//Send only to the specified machine
		//In the same channel of the actions
		rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, packet->systemAddress, false);

#ifdef DEBUG
	GLOUTPUT("+++ HandleHandShakeFinished, Request Game State: %s\n", packet->systemAddress.ToString(true));
#endif
	}
	else
	{
		//Connection done!
		mapPendingConnection.Remove(packet->systemAddress);
		mapConnections.Add(packet->systemAddress, 1);
	}

#ifdef DEBUG
	GLOUTPUT("+++ HandleHandShakeFinished: %s\n", packet->systemAddress.ToString(true));
#endif

	return true;
}

void GameControl::SendGameState(SystemAddress &addr)
{
	//Build the message
	RakNet::BitStream out;

	//Message type
	out.Write((unsigned char)ID_USER_GAME_STATE);

	//Write State
	WriteActorIndex(out);
	WriteGlobalVars(out);
	WriteLiveActors(out, GetAxis()->getImage(), false);


	//Send only to the specified machine
	//In the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, addr, false);

#ifdef DEBUG
	GLOUTPUT("+++ SendGameState: %s\n", addr.ToString(true));
#endif
}

bool GameControl::ExecuteGameStateUpdate(Packet *packet)
{
	//Update the local state based on remote machine state
	
	unsigned char msgType;

	///////////////////////////////////////////////////
	//Read the message
	RakNet::BitStream in(packet->data, packet->length, false);

	//Already decoded by ProcessNetwork
	//Read here to increase data pointer	
	in.Read(msgType);
	if(msgType != ID_USER_GAME_STATE) return true;

	//Read State
	ReadActorIndex(in);
	ReadGlobalVars(in);
	ReadLiveActors(in, true, packet->systemAddress);

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteGameStateUpdate: %s\n", packet->systemAddress.ToString(true));
#endif

	//Connection done!
	mapPendingConnection.Remove(packet->systemAddress);
	mapConnections.Add(packet->systemAddress, 1);

	return true;
}

void GameControl::SendOnlyStateOfActorsControlledLlocaly(SystemAddress &addr)
{
	//Send only the actors controlled by this machine
	
	//Build the message
	RakNet::BitStream out;

	//Message type
	out.Write((unsigned char)ID_USER_LOCAL_ACTORS);

	//Write State		
	WriteLiveActors(out, GetAxis()->getImage(), true);


	//Send only to the specified machine
	//In the same channel of the actions
	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, NET_CHANNEL_ACTIONS, addr, false);

#ifdef DEBUG
	GLOUTPUT("+++ SendOnlyStateOfActorsControlledLlocaly: %s\n", addr.ToString(true));
#endif
}

bool GameControl::ExecuteStateUpdateOfRemoteControlledActors(Packet *packet)
{
	//Update the local state based on remote machine state
	
	unsigned char msgType;

	///////////////////////////////////////////////////
	//Read the message
	RakNet::BitStream in(packet->data, packet->length, false);

	//Already decoded by ProcessNetwork
	//Read here to increase data pointer	
	in.Read(msgType);
	if(msgType != ID_USER_LOCAL_ACTORS) return true;

	//Read State	
	ReadLiveActors(in, false, packet->systemAddress);

#ifdef DEBUG
	GLOUTPUT("+++ ExecuteStateUpdateOfRemoteControlledActors: %s\n", packet->systemAddress.ToString(true));
#endif

	return true;
}

void GameControl::WriteActorIndex(RakNet::BitStream& out)
{	
	gedString name, actorName;
	out.Write((int)actorIndex.size());

	MapActorFileIndexIterator itActor(actorIndex);
	for(itActor.Begin(); !itActor.Done(); itActor.Next())
	{
		actorName = *itActor.Key();
		int i = actorName.find(".");
		long cloneindex = -1;

		name = actorName.substr(0,i);

		if(i > 0) cloneindex = atol(actorName.substr(i+1).c_str());

		RakNet::StringTable::Instance()->EncodeString(name.c_str(), ACTOR_NAME, &out);
		out.Write(cloneindex);
		out.Write(itActor.Value()->bDestroyed);			
	}
}

void GameControl::ReadActorIndex(RakNet::BitStream& in)
{
	int n;
	in.Read(n);
	char cloneName[CLONE_NAME];
	char actorName[ACTOR_NAME];
	bool bDestroyed;
	long cloneindex;


	for(int i = 0; i < n; i++)
	{
		RakNet::StringTable::Instance()->DecodeString(actorName, ACTOR_NAME, &in);
		in.Read(cloneindex);
		in.Read(bDestroyed);

		if(cloneindex >= 0)
		{
			sprintf(cloneName, "%s.%ld", actorName, cloneindex);
			
		}
		else
		{
			strcpy(cloneName, actorName);
		}

		stActorState *actorState = actorIndex.FindString(cloneName);
		if(actorState)
		{
			if(bDestroyed && !actorState->bDestroyed && cloneindex >= 0)
			{
				//This actor aren't valid anymore
				Actor *actor = GetActor(cloneName, true, false, false);

				if(actor && actor->getRunning())
				{
					actor->setCallOnDestroy(false);
					actor->PostMessage(actor, DELETE_ME, DELETE_ME);
				}
			}

			actorState->bDestroyed = bDestroyed;			
		}
	}
}

void GameControl::WriteGlobalVars(RakNet::BitStream& out)
{
	//Must send all global allocated vars
	//Global pointers and dynamic allocated memory aren't synchronized.

	symentry_t *sym;
	int i;
	    
    for (i = 0; i < HSIZE; i++)
	{
		for(sym = EiC_HTAB[i]; sym; sym = sym->next) 
		{
			type_expr *pType = sym->type;
			int bType = 0, bPointer = 0, bConst = 0, bFunction = 0, bStruct = 0, bTypedef = 0, bInternal = 0;			

			if(sym->sclass == c_typedef)
			{
				bTypedef = 1;
			}

			while(pType)
			{
				int t = EiC_gettype(pType);
				
				if((isconst(pType) || isconstp(pType))) 
				{
					bConst = 1;
				}

				if(t == t_pointer)
				{
					bPointer = 1;
				}

				if(t == t_eic)
				{
					bInternal = 1;
				}
	
				if(t == t_func || t == t_funcdec || t == t_builtin)
				{
					bFunction = 1;
				}	

				if(t == t_struct || t == t_union)
				{
					bStruct = 1;
				}				

				pType = nextType(pType);
			}

			if(!bFunction && !bTypedef && !bPointer && !bConst && !bInternal)
			{			
				int size = 0;
				void *addr = NULL;

				if(sym->val.ival >= 0 && sym->val.ival < EiC_ENV->ARsize)
				{
					ptr_t *p = &(EiC_ENV->AR[sym->val.ival].v.p);
					if(p)
					{						
						if(p->sp != p->ep && p->sp == p->p)
						{
							//Array, struct, ...
							addr = p->sp;
							size = (char *)p->ep - (char *)p->sp;
						}
						else
						{
							//Simple type
							addr = p;
							size = EiC_get_sizeof(sym->type);
						}
					}
				}

				if(addr && size)
				{
					out.Write(size);
					RakNet::StringTable::Instance()->EncodeString(sym->id, ACTOR_NAME, &out);
					out.Write((const char *)addr, size);	
				}
			}
		}
	}

	//End of data
	out.Write((int)0);
}

void GameControl::ReadGlobalVars(RakNet::BitStream& in)
{	
	int sizeSrc = 0, sizeDst = 0, oldSizeSrc = 0;
	void *src = NULL, *dst = NULL;

	in.Read(sizeSrc);
	char symID[ACTOR_NAME];

	while(sizeSrc)
	{
		if(oldSizeSrc < sizeSrc)
		{
			src = realloc(src, sizeSrc);
			oldSizeSrc = sizeSrc;
		}

		RakNet::StringTable::Instance()->DecodeString(symID, ACTOR_NAME, &in);
		if(in.Read((char *)src, sizeSrc))
		{
			symentry_t *sym = (symentry_t *)isSymbolInEic(symID);
			if(sym && sym->val.ival >= 0 && sym->val.ival < EiC_ENV->ARsize)
			{
				ptr_t *p = &(EiC_ENV->AR[sym->val.ival].v.p);
				if(p)
				{						
					if(p->sp != p->ep && p->sp == p->p)
					{
						//Array, struct, ...
						dst = p->sp;
						sizeDst = (char *)p->ep - (char *)p->sp;
					}
					else
					{
						//Simple type
						dst = p;
						sizeDst = EiC_get_sizeof(sym->type);
					}

					if(sizeDst == sizeSrc)
					{
						memcpy(dst, src, sizeDst);
					}
				}
			}
		}

		in.Read(sizeSrc);
	}

	free(src);
}

void GameControl::WriteLiveActors(RakNet::BitStream& out, KrImNode *pNode, bool bOnlyLocallyControlledActors)
{
	//Walk the tree and write the parents first
	static int recursion = 0;
	static GlobalMapActor alreadySend; //Avoid write twice text actors and several times tile actors

	if(recursion == 0) alreadySend.Clear();


	recursion++;

	GlInsideNodeIt<KrImNode*> it = pNode->ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		KrImNode *node = it.CurrentData();
		Actor *childActor = (Actor *)node->getParentActor();

		if(childActor && childActor->getRunning() && (!bOnlyLocallyControlledActors || (childActor->CanExecuteActionLocally() && childActor->getNetActor())) && !alreadySend[childActor])
		{
			alreadySend.Add(childActor, 1);

			//Actor
			out.Write(childActor->getCloneIndex());
			RakNet::StringTable::Instance()->EncodeString(childActor->getActorName(), ACTOR_NAME, &out);				

			//Write the actor state
			childActor->WriteState(out);

			//Look the children
			WriteLiveActors(out, node, bOnlyLocallyControlledActors);
		}

		it.Next();
	}

	recursion--;

	if(recursion == 0) out.Write((int)-1);	
}


void GameControl::ReadLiveActors(RakNet::BitStream& in, bool bDeleteAll, const SystemAddress &addr)
{
	//Update local actors based on remote machine
	MapRunningActor removeIfNotExists;

	if(bDeleteAll)
	{
		//Delete actors that aren't my
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];

				if(IS_VALID_ACTOR1(actor) && actor->getRunning())
				{
					if(!actor->CanExecuteActionLocally())
					{
						removeIfNotExists.Add(actor->getCloneName(), actor);						
					}
				}			
			}
		}
	}


	//Create the actors
	bool bHaveActor = false;
	char actorName[ACTOR_NAME];
	long cloneIndex;

	in.Read(cloneIndex);

	while(cloneIndex != -1)
	{
		RakNet::StringTable::Instance()->DecodeString(actorName, ACTOR_NAME, &in);

		//Create here and update the state later
		gedString cloneName(actorName);
		cloneName += '.';
		cloneName += gedString(cloneIndex);

		//Get an exiting actor or create a new actor
		Actor::setInContructionCallBack(true); //Avoid call InitNetActor if create a new actor here

		//Try get the actor (from unlinked list too)
		Actor *newActor = GetNetActor(addr, actorName, cloneIndex);

		//Try from load
		if(!newActor) newActor = GetActor(cloneName, true, true);
		
		//Try create
		if(!newActor || !newActor->getRunning())
		{			
			newActor = execCreateActor(actorName, "icon", NO_SELECTION, NO_SELECTION, 0, 0, true);
			Action::setNetActorCreated(false); //Avoid send this creation to the remote machines
		}
		Actor::setInContructionCallBack(false);

		if(newActor)
		{			
			bool bRemoveFromUnlinked = newActor->getUnlinked();
			newActor->SetCloneIndex(cloneIndex);
			newActor->ReadState(in);

			if(bRemoveFromUnlinked)
			{
				GetActorFromUnlinkedList(newActor->GetOwner(), newActor->getActorName());
			}

			//This actor still running on remote machines, so, don't remove
			removeIfNotExists.Remove(newActor->getCloneName());
		}		
		
	
		//Next actor
		in.Read(cloneIndex);
	}

	//Remove the actors that aren't running anymore
	if(removeIfNotExists.size())
	{
		for(Actor ***pActor = removeIfNotExists.GetValueArray(); *pActor; pActor++)
		{
			(**pActor)->setCallOnDestroy(false);
			(**pActor)->PostMessage(**pActor, DELETE_ME, DELETE_ME);
		}
	}
}


////////////////////////////////////////////////////////////////////

void GameControl::ProcessPingRequests()
{
	//Process ping requests
	GlDynArray<SystemAddress> removeAddr;
	GlMapIterator<SystemAddress, U32, GlSystemAddressHash> it(mapRequestPing);
	U32 now = SDL_GetTicks();

	for(it.Begin(); !it.Done(); it.Next())
	{
		SystemAddress *pAddr = it.Key();
		U32 *firstTime = it.Value();

		rakPeer->Ping(*pAddr);
		
		if(bUseGameEditorServer && (now - *firstTime) > 5*1000)
		{
			//May be we can't contact the remote host directly
			//Ask Game Server for help
			PingThroughGameServer(*pAddr);
		}

		

		if(now - *firstTime > 30*1000)
		{
			removeAddr.PushBack(*pAddr);
		}
	}

	//Remove old requests
	for(int i = 0; i < removeAddr.Count(); i++)
	{
		mapRequestPing.Remove(removeAddr[i]);
	}
}

void GameControl::ProcessLostConnections()
{
	//Remove orphan actors after connectionLostTimeOut seconds

	GlDynArray<SystemAddress> removeAddr;
	GlMapIterator<SystemAddress, U32, GlSystemAddressHash> it(mapConnectionLost);
	U32 now = SDL_GetTicks();

	for(it.Begin(); !it.Done(); it.Next())
	{
		SystemAddress *pAddr = it.Key();
		U32 *lostTime = it.Value();

		if(now - *lostTime > connectionLostTimeOut*1000)
		{
			removeAddr.PushBack(*pAddr);
		}
	}

	//Remove orphan actors
	for(int i = 0; i < removeAddr.Count(); i++)
	{
		SystemAddress addr = removeAddr[i];

		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];

				if(IS_VALID_ACTOR1(actor) && actor->getRunning())
				{
					if(actor->GetOwner() == addr)
					{
						actor->setCallOnDestroy(false);
						actor->PostMessage(actor, DELETE_ME, DELETE_ME);
					}
				}			
			}
		}

		mapConnectionLost.Remove(addr);
		mapUnlinkedActors.Remove(addr);
	}
}

void GameControl::ProcessPendingConnections()
{
	//Remove old peding connections
	//Wait only 30 seconds

	GlDynArray<SystemAddress> removeAddr;
	GlMapIterator<SystemAddress, U32, GlSystemAddressHash> it(mapPendingConnection);
	U32 now = SDL_GetTicks();

	for(it.Begin(); !it.Done(); it.Next())
	{
		SystemAddress *pAddr = it.Key();
		U32 *requestTime = it.Value();

		if(now - *requestTime > 30*1000)
		{
			removeAddr.PushBack(*pAddr);
		}
	}

	//Remove old connection requests
	for(int i = 0; i < removeAddr.Count(); i++)
	{
		mapPendingConnection.Remove(removeAddr[i]);
	}
}

bool GameControl::IsRemoteAddress(const SystemAddress& addr)
{
	if(!rakPeer) return false;

	bool bCanConnect = true;
	int n = rakPeer->GetNumberOfAddresses(), i;
	const char *sAddr = addr.ToString(false);

	for (i=0; i < n; i++)
	{
		if(strcmp(rakPeer->GetLocalIP(i), sAddr) == 0)
		{
			//My address
			//Is the same process?

			if(addr.port == currentListenPort)
			{
				return false;
			}
		}
	}

	return true;
}

bool GameControl::CanAcceptPacket(Packet *p)
{
	if(!mapPendingConnection.empty() && mapPendingConnection[p->systemAddress])
	{
		//Connection with this sistem was not completed yet
		switch(p->data[0])
		{
			case ID_USER_ENVIRONMENT_INFO_REQUEST:
			case ID_USER_ENVIRONMENT_INFO:
			case ID_USER_OLD_GAME_VERSION:
			case ID_USER_HANDSHAKE_FINISHED:
			case ID_USER_GAME_STATE:
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			case ID_INVALID_PASSWORD:
			case ID_CONNECTION_REQUEST_ACCEPTED:
			case ID_CONNECTION_ATTEMPT_FAILED:
				return true;
		}

#ifdef DEBUG
		GLOUTPUT("+++ Discard packet %ld due imcomplete connection (%s)\n", (unsigned char)p->data[0], p->systemAddress.ToString(true));
#endif
		return false;
	}

	return true;
}

void GameControl::ProcessNetwork(bool bFromFrameTick)
{
	//Process more than one message in one call, but limit to allow frame redraw.
	//How can make this limit adaptative?
	
	if(!rakPeer || !bCheckOutOfVision) return;	
	//U32 start = SDL_GetTicks();

	static int count = 0, maxPacket = 4;
	count++;


	if(bFromFrameTick || (bWaitingForConnectionComplete && count > 10))
	{
		ProcessPingRequests();
		ProcessLostConnections();		
		ProcessPendingConnections();
		count = 0;
	}

	//Update connection request status (in and out)
	//The problem that this will make the game wait event for incoming connections
	bWaitingForConnectionComplete = !mapPendingConnection.empty() || !mapRequestPing.empty();

	//Can process more packets?
	if(realFrameTimeInterval < frameTimeInterval)
	{
		maxPacket++;
	}
	else if(maxPacket > 4)
	{
		maxPacket--;
	}
	
	//Process packets
	for(int i = 0; i < maxPacket; i++)
	{
		Packet *p = rakPeer->Receive();
		bool bProcessed = true;
		if(!p) return;


		if(CanAcceptPacket(p))
		{
			switch(p->data[0])
			{
			case ID_TIMESTAMP:
				bProcessed = ExecuteTimeStampedMessages(p);
				break;			

			case ID_NEW_INCOMING_CONNECTION:
				{
#ifdef DEBUG
					GLOUTPUT("+++ New connection from %s\n", p->systemAddress.ToString(true));
#endif
					// We need our own unique IP address to go along with the NetworkID.  Our externalSystemAddress should be unique.
					// The internalSystemAddress, returned by rakPeer->GetLocalIP, won't be unique if we are behind a NAT machine

					mapConnectionLost.Remove(p->systemAddress);
					mapConnections.Remove(p->systemAddress); //Trying connection again?
					mapPendingConnection.Add(p->systemAddress, SDL_GetTicks());

					SetExternalSystemAddress(rakPeer->GetExternalID(p->systemAddress));
					bProcessed = HandleNewConnection(p->systemAddress);
				}
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
#ifdef DEBUG
					GLOUTPUT("+++ Connection accepted %s\n", p->systemAddress.ToString(true));
#endif

					mapConnectionLost.Remove(p->systemAddress);
					mapConnections.Remove(p->systemAddress);
					mapPendingConnection.Add(p->systemAddress, SDL_GetTicks());

					// We need our own unique IP address to go along with the NetworkID.  Our externalSystemAddress should be unique.
					// The internalSystemAddress, returned by rakPeer->GetLocalIP, won't be unique if we are behind a NAT machine
					SetExternalSystemAddress(rakPeer->GetExternalID(p->systemAddress));
				}
				break;

			case ID_USER_ENVIRONMENT_INFO_REQUEST:
				bProcessed = HandleEnvironmentInfoRequest(p->systemAddress);			
				break;

			case ID_USER_ENVIRONMENT_INFO:
				bProcessed = HandleEnvironmentInfo(p);
				break;

			case ID_USER_OLD_GAME_VERSION:
				WrongNetworkGameVersion();			
				break;

			case ID_USER_HANDSHAKE_FINISHED:				
				bProcessed = HandleHandShakeFinished(p);
				break;

			case ID_USER_REQUEST_GAME_STATE:
				//Send the current game state to the remote machine
				SendGameState(p->systemAddress);
				break;

			case ID_USER_GAME_STATE:
				//Receive the request game state from the remote machine
				bProcessed = ExecuteGameStateUpdate(p);

				//Send my actors
				SendOnlyStateOfActorsControlledLlocaly(p->systemAddress);			
				break;

			case ID_USER_LOCAL_ACTORS:
				bProcessed = ExecuteStateUpdateOfRemoteControlledActors(p);
				break;

			

			case ID_PONG:
				if(IsRemoteAddress(p->systemAddress))
				{
					//LAN discovery connection
#ifdef DEBUG
					RakNetTime time;
					memcpy((char*)&time, p->data+1, sizeof(RakNetTime));				
					GLOUTPUT("+++ Pong from %s with time %i\n", p->systemAddress.ToString(), RakNet::GetTime() - time);
#endif

					if(p->systemAddress.binaryAddress != gameEditorServer.binaryAddress)
					{
						//Connect to this peer
						//Must check if is the same game first and if can accept more connections
						//Don't matter remote is not public. Just Connect (already a entry in the remote NAT table)
						Connect(p->systemAddress.ToString(false), p->systemAddress.port, true); 				
					}
					else
					{
						//Game Editor server command or response
						//Look the first byte after the message id
						int command = p->data[1] - '0'; //ASCII value

						command;
					}
				}
				break;


			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			case ID_INVALID_PASSWORD:
				{
#ifdef DEBUG
					GLOUTPUT("+++ Connection lost to %s\n", p->systemAddress.ToString(true));
#endif

					if(p->systemAddress != UNASSIGNED_SYSTEM_ADDRESS && !mapPendingConnection.Remove(p->systemAddress))
					{
						//Remove actors from the remote machine
						mapConnectionLost.Add(p->systemAddress, SDL_GetTicks());						
					}

					mapConnections.Remove(p->systemAddress);

					//The address for this system still valid?
					SetExternalSystemAddress(rakPeer->GetExternalID(p->systemAddress));
				}
				break;

			case ID_CONNECTION_ATTEMPT_FAILED:
				{
#ifdef DEBUG
					GLOUTPUT("+++ Connection attempt to %s failed.\n", p->systemAddress.ToString(true));
#endif
					if(mapPendingConnection[p->systemAddress])
					{
						//Try again (ping)
						Connect(p->systemAddress.ToString(false), p->systemAddress.port, true);
					}

					mapConnections.Remove(p->systemAddress);
				}
				break;

			default:
				{
					//Discard packet					
#ifdef DEBUG
					GLOUTPUT("+++ Unknow message from %s failed.\n", p->systemAddress.ToString(true));
#endif
				}
				break;
			}			
		}

		if(bProcessed)
		{
			//Packet process ok. Release it now.
			rakPeer->DeallocatePacket(p);	
		}
		/*else
		{
			//Packet process error. Try again later.
			//30s max
			U32 now = SDL_GetTicks();

			if(!p->retry) 
			{
				//First insertion on retry list
				p->retry = now;
			}

			if(now - p->retry < 30000) //Avoid infinite loop
			{					
				rakPeer->PushBackPacket(p, false);
			}
			else
			{
#ifdef DEBUG
				GLOUTPUT("+++ Packet %ld discarded from %s\n", (unsigned char)p->data[0], p->systemAddress.ToString(true));
#endif
				rakPeer->DeallocatePacket(p);	
			}

			return;
		}*/

		/*if(SDL_GetTicks() - start > (realFrameTimeInterval >> 1))
		{
			return;
		}*/
	}
}

void GameControl::InitPocketPCKeys()
{
	pocketKeys.Clear();

	//Standard keys
	pocketKeys.Add(SDLK_POCKET_UP,		SDLK_UP);
	pocketKeys.Add(SDLK_POCKET_DOWN,	SDLK_DOWN);
	pocketKeys.Add(SDLK_POCKET_LEFT,	SDLK_LEFT);
	pocketKeys.Add(SDLK_POCKET_RIGHT,	SDLK_RIGHT);

	pocketKeys.Add(SDLK_POCKET_START,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_A,		SDLK_a);
	pocketKeys.Add(SDLK_POCKET_B,		SDLK_b);
	pocketKeys.Add(SDLK_POCKET_C,		SDLK_c);

	//Non standard keys
	pocketKeys.Add(SDLK_POCKET_AUX1,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX2,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX3,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX4,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX5,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX6,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX7,	SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_POCKET_AUX8,	SDLK_UNKNOWN);

	//GP2X keys
	pocketKeys.Add(SDLK_GP2X_BUTTON_UP,			SDLK_UP);
	pocketKeys.Add(SDLK_GP2X_BUTTON_DOWN,		SDLK_DOWN);
	pocketKeys.Add(SDLK_GP2X_BUTTON_LEFT,		SDLK_LEFT);
	pocketKeys.Add(SDLK_GP2X_BUTTON_RIGHT,		SDLK_RIGHT);

	pocketKeys.Add(SDLK_GP2X_BUTTON_UPLEFT,		SDLK_HOME);
	pocketKeys.Add(SDLK_GP2X_BUTTON_UPRIGHT,	SDLK_PAGEUP);
	pocketKeys.Add(SDLK_GP2X_BUTTON_DOWNLEFT,	SDLK_END);
	pocketKeys.Add(SDLK_GP2X_BUTTON_DOWNRIGHT,	SDLK_PAGEDOWN);

	pocketKeys.Add(SDLK_GP2X_BUTTON_CLICK,		SDLK_CLEAR);

	pocketKeys.Add(SDLK_GP2X_BUTTON_A,			SDLK_a);
	pocketKeys.Add(SDLK_GP2X_BUTTON_B,			SDLK_b);
	pocketKeys.Add(SDLK_GP2X_BUTTON_X,			SDLK_x);
	pocketKeys.Add(SDLK_GP2X_BUTTON_Y,			SDLK_y);

	pocketKeys.Add(SDLK_GP2X_BUTTON_L,			SDLK_l);
	pocketKeys.Add(SDLK_GP2X_BUTTON_R,			SDLK_r);

	pocketKeys.Add(SDLK_GP2X_BUTTON_START,		SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_GP2X_BUTTON_SELECT,		SDLK_UNKNOWN);
	pocketKeys.Add(SDLK_GP2X_BUTTON_VOLUP,		SDLK_KP_PLUS);
	pocketKeys.Add(SDLK_GP2X_BUTTON_VOLDOWN,	SDLK_KP_MINUS);
}

SDLKey GameControl::PocketPCKey(SDLKey from)
{
	//PocketPC keys mapper

	if(bGameMode)
	{
		SDLKey *key = pocketKeys[from];
		
		if(key && *key != SDLK_UNKNOWN)
		{
			return *key;
		}
	}

	return from;
}

SDLKey GameControl::UserDefinedKeys(SDLKey from)
{
	//User keys mapper

	if(bGameMode)
	{
		SDLKey *key = userKeys[from];
		
		if(key && *key != SDLK_UNKNOWN)
		{
			return *key;
		}
	}

	return from;
}



Actor *GameControl::GetEditView()
{
	Actor *editView = NULL;

	ListActor *listActor = mapActors[VIEW_NAME];
	if(listActor)
	{
		for(int il = 0; il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode() && actor->isView())
			{	
				editView = actor;
				break;
			}			
		}
		
	}

	return editView;
}

void GameControl::PrepareRegions()
{
	//Used when no regions has been defined
	//Call when enter in game mode

	if(RegionLoad::Count() > 0)
		return;

	RegionLoad::CreateRegionTemp();
	
	if(bStandAloneMode)
	{
		MapActorFileIndexIterator itActor(actorIndex);
		for(itActor.Begin(); !itActor.Done(); itActor.Next())
		{
			gedString *pName = itActor.Key();
			if(pName->hasPoint())
			{
				//Only clone names
				RegionLoad::AddActorInRegionTemp(*pName);	
			}
		}
	}
	else
	{
		RegionLoad::UpdateRegions();
	}
}

void GameControl::ReleaseActorTip(bool bDisableTips)
{
#ifndef STAND_ALONE_GAME
	//Destroy actor tip, if any
	ActorTip::Release();
	if(bDisableTips) tipTime = -1000000;
	else tipTime = 0;
#endif //#ifndef STAND_ALONE_GAME
}

void GameControl::UpdateActorTip(Actor *actor)
{
#ifndef STAND_ALONE_GAME
	//Show actor tip
	//Called in frame update (EDITOR_FRAME_RATE fps in editor mode)

	if(actor && tipTime > EDITOR_FRAME_RATE && (actor->EditMode() || (Config::Get()->getEnableToolTips() && Tutorial::IsOff())))
	{
		ActorTip::Create(actor);
		tipTime = 0;
	}

	tipTime++; 
#endif //#ifndef STAND_ALONE_GAME
}

#ifndef STAND_ALONE_GAME
void GameControl::UpdateToolTip(gedString tip)
{	
	if(tip.length())
	{
		ActorTip::Create(&tip);
	}
	
	tipTime = 0;
}
#endif //#ifndef STAND_ALONE_GAME



void GameControl::LoadIcon()
{
	KrBinaryDataResource* binData = engine->Vault()->GetBinaryDataResource("ged Game Icon");
	if(!binData)
		return;
	
	SDL_RWops *src = SDL_RWFromMem((void *)binData->Data(), binData->Length());

	SDL_Surface *surfaceIcon = SDL_LoadBMP_RW(src, 1);				
	if(surfaceIcon)
	{					
		SDL_WM_SetIcon(surfaceIcon, NULL);						
	}
}

void GameControl::ClearIcon()
{
	engine->Vault()->RemoveBinaryDataResource(engine->Vault()->GetBinaryDataResource("ged Game Icon"));
}

void GameControl::MapUserKey(SDLKey from, SDLKey to)
{
#ifdef DEBUG
		GLOUTPUT("MapUserKey: %s (%ld) to %s (%ld)\n", SDL_GetKeyName((SDLKey)from), from, SDL_GetKeyName((SDLKey)to), to);
#endif

	if(from != to)
	{
		userKeys.Add(from, to);
	}
	else
	{
		//Unmap
		userKeys.Remove(from);
	}
}

#ifndef STAND_ALONE_GAME
gedString GameControl::AdjustFileName(gedString filePath)
{
	if(bFastFile)
	{
		return filePath;
	}

	/*
	Set path relative to game path
	Move files to game directory, if necessary
	*/
	gedString newPath = "data", fileName, fromPath, name, aux(filePath);
	ged_mkdir("data");


	//Adjust ttf font
	gedString s(aux);
	s.lower();
	if(s.substr(s.length() - (16 + 4), 4) == ".ttf")
	{
		aux = aux.substr(0, aux.length() - 16);
	}

	if(aux.substr(0, 4) == "data")
	{
		//Relative to old game directory
		gedString gamePath(getOldGamePath());

		if(gamePath.empty())
		{
			//Try from new game directory
			gamePath = getGamePath();
		}

		if(!gamePath.empty())
		{
			aux = gamePath + DIR_SEP + aux;
		}
		else
		{
			aux = gamePath + aux;
		}
	}
	
	int i = filePath.rfind('\\');
	if(i == gedString::npos)
	{
		i = filePath.rfind('/');
		if(i == gedString::npos)
		{
			i = 0;//return filePath; //all resources must be in data directory
			
			//Solve the gravPPC dataextended font.bmp bug
			gedString dirSep;
			dirSep += DIR_SEP;
			filePath = dirSep + filePath; 
		}
	}

	newPath = newPath + filePath.substr(i);

	SplitPath(aux, fileName, fromPath);

	if(!copyfile(aux.c_str(), "data", fromPath.c_str())) return filePath;

	return newPath;
}

void GameControl::PopulateGlobalUserVars(ListPop *list)
{
	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		char buf[128];
		

		gedString sType, sSave;

		switch(itVar.Value()->type)
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

		if(itVar.Value()->saveGroup.length())
		{
			sSave = ", save in ";
			sSave += itVar.Value()->saveGroup;
		}

		if(itVar.Value()->arrayLenght > 0)
		{
			sprintf(buf, "%s[%ld] (Global, %s%s)", itVar.Key()->c_str(), itVar.Value()->arrayLenght, sType.c_str(), sSave.c_str());
		}
		else
		{
			sprintf(buf, "%s (Global, %s%s)", itVar.Key()->c_str(), sType.c_str(), sSave.c_str());
		}

		int i = list->AddText(buf);
		list->SetItemData(i, (void *)GLOBAL_VAR);
	}
}

void GameControl::PopulateGlobalUserVarsGroups(ListPop *list)
{
	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		if(itVar.Value()->saveGroup.length())
		{
			list->AddText(itVar.Value()->saveGroup);
		}		
	}
}

void GameControl::PopulateActors(ListString &list, bool bGetClones, bool bNewInterface)
{
	if(bNewInterface)
	{
		/*int nActors = NumActors();
		
		for(int i = 0; i < nActors; i++)
		{
			gedString cloneName(ActorName(i)), actorName(cloneName.substr(0, cloneName.find('.')));
			if(bGetClones) 
			{
				cloneName = actorName + "," + cloneName;
			}
			else
			{
				cloneName = actorName;
			}
			
			list.PushBack(cloneName);
			list.PushBack(actorName); //Solve the bug "remove the first clone, and not show the single actor on lists"
		}*/

		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(actor->EditMode())
				{
					gedString name(actor->getActorName());
					if(bGetClones && it.Value()->Count() > 1)
					{
						name = name + "," + actor->getCloneName();					
					}
					
					list.PushBack(name);
				}
			}
		}
	}
	else if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		gedString plus("+ ");
		MapActorIterator it(mapActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				Actor *actor = (*listActor)[il];
				if(actor->EditMode())
				{
					gedString name(actor->getActorName());
					if(bGetClones && it.Value()->Count() > 1)
					{
						name = plus + name;					
					}
					
					list.PushBack(name);
				}
			}
		}
	}
	else
	{
		int nActors = NumActors();
		
		for(int i = 0; i < nActors; i++)
		{
			gedString cloneName(ActorName(i)), actorName(cloneName.substr(0, cloneName.find('.')));
			if(!bGetClones) cloneName = actorName;
			
			list.PushBack(cloneName);
			list.PushBack(actorName); //Solve the bug "remove the first clone, and not show the single actor on lists"
		}
	}
}

void GameControl::PopulateActors(ListPop *list, bool bGetClones)
{
	list->RemoveAll();

	ListString listString;
	PopulateActors(listString, bGetClones);	

	for(int i = 0; i < listString.Count(); i++)
	{
		list->AddText(listString[i]);
	}

	list->Sort();
}

void GameControl::PopulatePaths(ListString &list, bool bNoPaths)
{
	int nPaths = NumPaths();

	for(int i = 0; i < nPaths; i++)
	{
		list.PushBack(PathName(i));
	}

	if(bNoPaths)
	{
		list.PushBack(NO_PATH);
		list.PushBack(RANDOM_PATH);
	}
}

void GameControl::PopulatePaths(ListPop *list, bool bNoPaths)
{
	list->RemoveAll();

	ListString listString;
	PopulatePaths(listString, bNoPaths);	

	for(int i = 0; i < listString.Count(); i++)
	{
		list->AddText(listString[i]);
	}	
}

void GameControl::PopulateAnimations(Actor *actor, ListPop *list, bool bAddSequences)
{
	if(actor <= COLLIDE_ACTOR) return;

	list->RemoveAll();

	ListString listString;
	PopulateAnimations(actor, listString, bAddSequences);


	if(listString.Count())
	{
		for(int i = 0; i < listString.Count(); i++)
		{
			list->AddText(listString[i]);
		}

		list->SetItem(actor->GetAnimation());
	}
	else list->SetItem(-1);

	//Why put the text here?
	//if(actor->getTextActor()) list->AddText(ACTOR_GED_TEXT);
}

void GameControl::PopulateAnimations(Actor *actor, ListString &list, bool bAddSequences)
{
	if(actor <= COLLIDE_ACTOR) return;

	int nAnimations = actor->NumAnimations();
	Sequence *seq;

	if(nAnimations)
	{
		for(int i = 2; i < nAnimations+2; i++)
		{
			gedString name = actor->AnimationName(i, &seq);

			if(bAddSequences || (seq && seq->getType() == ANIMATION_SEQUENCE))
			{
				list.PushBack(name);
			}
		}
	}
}

void GameControl::PopulateTimers(ListPop *list, bool bShowParam)
{
	list->RemoveAll();

	ListString listString;
	PopulateTimers(listString, bShowParam);
	
	for(int i = 0; i < listString.Count(); i++)
	{
		list->AddText(listString[i]);
	}

	list->Sort();
}

void GameControl::PopulateTimers(ListString &list, bool bShowParam)
{
	char buf[256];
	
	MapTimerNameDefinitionIterator it(mapTimerNameDefinition);
	for(it.Begin(); !it.Done(); it.Next())
	{
		stTimerDefinition *timer = it.Value();
		memset(buf, 0, 256);
		
		if(bShowParam)
		{
			switch(timer->type)
			{
			case PERIODIC_TIMER:
				{
					if(timer->count == REPEATE_FOREVER)
						sprintf(buf, "%s (Periodic, %ld ms)", it.Key()->c_str(), timer->interval);
					else
						sprintf(buf, "%s (Periodic, %ld ms, %ld times)", it.Key()->c_str(), timer->interval, timer->count);
				}
				break;
			case RANDOM_TIMER:
				{
					if(timer->count == REPEATE_FOREVER)
						sprintf(buf, "%s (Random, %ld to %ld ms)", it.Key()->c_str(), timer->minRandomInterval, timer->interval);
					else
						sprintf(buf, "%s (Random, %ld to %ld ms, %ld times)", it.Key()->c_str(), timer->minRandomInterval, timer->interval, timer->count);
				}
				break;
			}
		}
		else
		{
			sprintf(buf, "%s", it.Key()->c_str());
		}
		
		list.PushBack(buf);
	}
}

bool GameControl::Save(bool bExport)
{
	if(gameName.length() == 0) return false;

	char oldpath[PATH_LIMIT];
	ged_getcwd(oldpath, PATH_LIMIT);
	chdir(gamePath.c_str());

	gedString name(gameName);

	if(bExport)
	{
		name = gedString("gedTmp_") + name;
	}

	bExportMode = bExport;
	bool res = Save(name);
	bExportMode = false;

	chdir(oldpath);

	return res;
}



bool GameControl::ExportGame(const gedString& exportName, int exportType)
{
#ifdef RENDER_THREAD
	MuteEx mutex(renderMutEx);
#endif

#if /*defined(GAME_EDITOR_PROFESSIONAL) &&*/ !defined(STAND_ALONE_GAME)
	
	//Data and game.ged in same file
	bExportMode = true;


	WaitCursor wait;
	bool bIconResourceInserted = false;

	gedString fileName = exportName, sourceName;

	int i;
	if((i = fileName.find_no_case(".ged")) != gedString::npos)
	{
		fileName = fileName.substr(0, i);				
	}
	
	switch(exportType)
	{
	case GAME_DATA_ONLY:
	case POCKETPC_DATA:
		{			
			if((i = fileName.find_no_case(".dat")) != gedString::npos)
			{
				fileName = fileName.substr(0, i);				
			}

			fileName += ".dat";
		}
		break;
	case WINDOWS_EXECUTABLE:
		{
			if((i = fileName.find_no_case(".exe")) != gedString::npos)
			{
				fileName = fileName.substr(0, i);				
			}

			fileName += ".exe";
			sourceName = "bin\\windows.bin";
		}
		break;
	case POCKETPC_EXECUTABLE:
		{
			if((i = fileName.find_no_case(".exe")) != gedString::npos)
			{
				fileName = fileName.substr(0, i);				
			}

			fileName += ".exe";
			sourceName = "bin\\pocketpc.bin";
		}
		break;
	case LINUX_EXECUTABLE:
		sourceName = "bin\\linux.bin";
		break;
	case GP2X_EXECUTABLE:
		{
			if((i = fileName.find_no_case(".gpe")) != gedString::npos)
			{
				fileName = fileName.substr(0, i);				
			}

			fileName += ".gpe";
			sourceName = "bin\\gp2x.bin";
		}
		break;
	}


	//Create executable
	SDL_RWops *exeFile = ged_SDL_RWFromFile(fileName.c_str(), "wb");
	if(!exeFile)
	{
		bExportMode = false;
		return false;
	}

	//Put game engine into executable
	if(exportType != GAME_DATA_ONLY && exportType != POCKETPC_DATA)
	{
		/*if(!copy((editorPath+DIR_SEP+sourceName).c_str(), exeFile))
		{
			SDL_RWclose(exeFile);
			remove(fileName.c_str());
			bExportMode = false;
			return false;
		}*/

		int len;
		Compression comp;
		U8 *buf = NULL;

		{
			EditorDirectory editDir;
			buf = comp.Decompression(sourceName.c_str(), len);
		}

		if(!buf)
		{
			SDL_RWclose(exeFile);
			remove(fileName.c_str());
			bExportMode = false;
			return false;
		}

		
		if(exportType == WINDOWS_EXECUTABLE || exportType == POCKETPC_EXECUTABLE)
		{
			//Insert icon resource
			if(InsertResourceIcon(&buf, len))
			{
				bIconResourceInserted = true;
			}

			//Set VGA
			if(exportType == POCKETPC_EXECUTABLE)
			{
				//Will use VGA mode if resolution is:
				// 480x480
				// 480x640
				// 640x480

				
				if(resX == 480 || resY == 480)
				{
					//Only put the HI_RES_AWARE if the game is VGA
					//If put HI_RES_AWARE with value 0, don't will works
					//on Dell Axim x50v (DarkYeti)
					SetPocketPC_VGA(&buf, len, true);
				}

				
			}
		}

		//Write file
		SDL_RWwrite(exeFile, buf, len, 1);
		delete [] buf;
	}

	//Get engine lenght
	SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
	Uint32 kyraDatPos = SDL_RWtell( exeFile );
	 

	//Put icon in kyra resources
	if(!bIconResourceInserted)
	{
		SaveIcon();
	}

	IgnoreEditorResources();

	if(exportType == POCKETPC_EXECUTABLE || exportType == POCKETPC_DATA || exportType == GP2X_EXECUTABLE)
	{
		//Convert all 24 bit images to 16 bits (save memory and speedup PocketPC)
		KrRle::setConvertImage(true); 
	}

	//Put Kyra dat file into executable (modified indexed mode)
	engine->Vault()->SaveDatFile(exeFile, true, false);

	KrRle::setConvertImage(false);

	//Remove icon from kyra resources
	ClearIcon();


	//Put ged into executable
	gedString tmpGedName(gamePath+DIR_SEP+"gedTmp_"+gameName);
	if(!copy(tmpGedName.c_str(), exeFile))
	{
		SDL_RWclose(exeFile);
		remove(fileName.c_str());
		remove(tmpGedName.c_str());
		bExportMode = false;
		return false;
	}

	remove(tmpGedName.c_str());

	//Get engine + dat + ged + icon offset
	SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
	Uint32 audioBase = SDL_RWtell( exeFile );

	//Put audio files into executable
	MapAudioFileIndex audioIndex;
	Action::SaveSoundTo(exeFile, audioIndex);
	GenericScript::SaveSoundTo(exeFile, audioIndex);


	//Get engine + dat + ged + icon + audio offset
	SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
	Uint32 audioIndexBase = SDL_RWtell( exeFile );


	//Put audio index into executable
	SDL_WriteLE32(exeFile, audioIndex.size());

	MapAudioFileIndexIterator itAudioIndex(audioIndex);	
	for(itAudioIndex.Begin(); !itAudioIndex.Done(); itAudioIndex.Next())
	{
		//File name
		WriteString(exeFile, *itAudioIndex.Key());

		//Offset relative to audioBase
		SDL_WriteLE32(exeFile, itAudioIndex.Value()->offset - audioBase);

		//Size
		SDL_WriteLE32(exeFile, itAudioIndex.Value()->size);
	}

	//Save offsets
	SDL_WriteLE32(exeFile, kyraDatPos);
	SDL_WriteLE32(exeFile, audioBase);
	SDL_WriteLE32(exeFile, audioIndexBase);

	//GEDX signature
	Uint32 magic;
	memcpy(&magic, "GEDX", 4);

	SDL_WriteLE32(exeFile, magic);

	//Finish
	SDL_RWclose(exeFile);

	if(exportType == LINUX_EXECUTABLE)
	{
		//Maks: Set file executable permission
	}
	

	SDL_ClearError();
#endif

	bExportMode = false;
	return true;
}


bool GameControl::Save(const gedString& gameName, SDL_RWops *srcFile)
{
#ifdef RENDER_THREAD
	MuteEx mutex(renderMutEx);
#endif

#ifndef STAND_ALONE_GAME
	WaitCursor wait;
#endif

	SDL_ClearError();

	gedString aux(gameName);	

	//Create a memory file
	SDL_RWops *src = SDL_RWFromMem(NULL, 0);
	if(!src || !src->hidden.mem.base)
	{
		#ifndef STAND_ALONE_GAME
		if(!bFastFile) new PanelInfo("Not enough memory to save the game");
		#endif 

		return false;
	}

	GED_TRY
	{	

	RegionLoad::UpdateRegions();

	//Set game directory
	if(gameName.substr(0, 7) != "gedTmp_" && gameName.substr(0, 14) != "EmergencySave_") 
	{
		char buf[PATH_LIMIT];

		this->gameName = gameName;
		oldGamePath = gamePath;
		gamePath = ged_getcwd(buf, PATH_LIMIT);
	}

	//Save file version
	SDL_WriteLE32(src, GAME_FILE_VERSION);

	//Save Game Editor version
	SDL_WriteLE32(src, GAME_EDITOR_VERSION);

	//Save fast file flag
	SDL_RWwrite(src, &bFastFile, sizeof(Uint8), 1);

	//////////////////////////////////////////////////////////////////////
	//Reserve index space for actions, paths, scripts and actors

	//Reserve action index space
	int n = 0;
	int lenghtIndex = 4; //Save number of actions
	GlobalMapActionsIterator itAction(Action::getGlobalMapActions());
	for(itAction.Begin(); !itAction.Done(); itAction.Next())
	{
		//Action name + 32 bit index
		lenghtIndex += (*itAction.Value())->getActionName().GetWriteLenght()  + 4; 
		n++;
	}	
	
	SDL_RWseek( src, 0, SEEK_END ); //tell error without this
	int indexPos = SDL_RWtell( src );
	Uint32 *indexAction = new Uint32[n];
	
	SDL_RWseek( src, lenghtIndex, SEEK_CUR );


	//Reserve path index space
	n = 0;
	lenghtIndex = 4; //Save number of paths
	MapPathIterator itPath(paths);	
	for(itPath.Begin(); !itPath.Done(); itPath.Next())
	{
		//Path name + 32 bit index
		lenghtIndex += (*itPath.Value())->getName().GetWriteLenght() + 4; 
		n++;
	}	

	Uint32 *indexPath = new Uint32[n];
	
	SDL_RWseek( src, lenghtIndex, SEEK_CUR );


	//Reserve script index space
	n = 0;
	lenghtIndex = 4; //Save number of scripts
	MapExpressionIterator itExpression(expressions);	
	for(itExpression.Begin(); !itExpression.Done(); itExpression.Next())
	{
		//Script name + 32 bit index
		lenghtIndex += itExpression.Key()->GetWriteLenght() + 4; 
		n++;
	}	

	Uint32 *indexScript = new Uint32[n];
	
	SDL_RWseek( src, lenghtIndex, SEEK_CUR );

	//Reserve actor index space
	n = 0;
	lenghtIndex = 4; //Save number of actors
	
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode())
			{
				//Actor clone name + 32 bit index
				gedString actorCloneName(actor->getCloneName());
				lenghtIndex += actorCloneName.GetWriteLenght() + 4;
				n++;
			}
		}
	}

	Uint32 *indexActor = new Uint32[n];
	
	SDL_RWseek( src, lenghtIndex, SEEK_CUR );
	//////////////////////////////////////////////////////////////////////

	Script::SaveLocalUserVars(src);
	RegionLoad::SaveRegions(src);

	//Save animation data
	for(it.Begin(); !it.Done(); it.Next())
	{		
		Actor *actor = (*it.Value())[0];
		if(actor->EditMode())
		{
			actor->SaveAnimationData(src);
		}		
	}

	gedString endAnimations("ged_E N D");
	endAnimations.Write(src);
	
	//Save global vars
	SDL_WriteLE16(src, globalUserVars.size());

	MapVarsIterator itVar(globalUserVars);	
	for(itVar.Begin(); !itVar.Done(); itVar.Next())
	{
		itVar.Value()->Save(src);
	}	

	//After global vars
	GenericScript::Save(src);
	


	//Save timers
	Action::CleanUpUnusedTimers(mapTimerNameDefinition);
	SDL_WriteLE16(src, mapTimerNameDefinition.size());

	MapTimerNameDefinitionIterator itTimer(mapTimerNameDefinition);
	for(itTimer.Begin(); !itTimer.Done(); itTimer.Next())
	{
		//Save name
		itTimer.Key()->Write(src);

		//Save definition
		stTimerDefinition *timer = itTimer.Value();
		SDL_WriteLE32(src, timer->interval);
		SDL_WriteLE32(src, timer->minRandomInterval);
		SDL_WriteLE32(src, timer->count);
		SDL_WriteLE32(src, timer->type);
	}

	//Save base zdepth
	SDL_WriteLE32(src, Actor::getBaseZDpth());

	//Fingerprint
	SaveFingerPrint(src);


	//Save game properties
	SDL_WriteLE16(src, resX);
	SDL_WriteLE16(src, resY);
	SDL_RWwrite(src, &bFullScreen, sizeof(Uint8), 1);
	SDL_WriteLE16(src, fps);
	SDL_WriteLE16(src, audioSamplerRate);
	SDL_RWwrite(src, &bStereo, sizeof(Uint8), 1);
	SDL_WriteLE16(src, maximumSounds);
	SDL_RWwrite(src, &bShowMouse, sizeof(Uint8), 1);
	SDL_RWwrite(src, &bSuspendGameIfLostFocus, sizeof(Uint8), 1);
	SDL_RWwrite(src, &bUseESCKeyToExit, sizeof(Uint8), 1);

	//VERSION_NETWORK
	gedString gameServer;
	if(bUseGameEditorServer)
	{
		gameServer = "use server"; //Any string for now
	}	

	gameServer.Write(src);
	if(gameID.empty()) gameID = GenerateUniqueID();
	gameID.Write(src);
	if(!bFastFile && !bExportMode) gameVersion++; //Automatic version increment
	SDL_WriteLE32(src, gameVersion);

	U8 tmp = bAutoStartNetwork;
	SDL_RWwrite(src, &tmp, sizeof(U8), 1);

	SDL_WriteLE32(src, gameNetworkPort);
	SDL_WriteLE32(src, connectionLostTimeOut);
	

	//Save PocketPC keys
	SDL_WriteLE16(src, pocketKeys.size());

	MapRemapKeysIterator itKey(pocketKeys);	
	for(itKey.Begin(); !itKey.Done(); itKey.Next())
	{
		SDL_WriteLE16(src, *itKey.Key());
		SDL_WriteLE16(src, *itKey.Value());
	}

	//Save grid settings
	bool bGridShow = GetAxis()->getGridShow();
	bool bGridSnap = GetAxis()->getGridSnap();
	Uint16 gridx = GetAxis()->getGridX();
	Uint16 gridy = GetAxis()->getGridY();

	SDL_RWwrite(src, &bGridShow, sizeof(Uint8), 1);
	SDL_RWwrite(src, &bGridSnap, sizeof(Uint8), 1);
	SDL_WriteLE16(src, gridx);
	SDL_WriteLE16(src, gridy);

	//Save grid zdepth
	Uint32 zDepth = GetAxis()->GetGridZDepth();
	SDL_WriteLE32(src, zDepth);

	SDL_WriteLE32(src, GetAxis()->getMainColor().all);
	SDL_WriteLE32(src, GetAxis()->getResolutionColor().all);

	SDL_WriteLE32(src, backgroundColor.all);

	SDL_WriteLE32(src, viewSafeMargin);

	SDL_RWwrite(src, &bFlipPocketPCScreen, sizeof(Uint8), 1);
	SDL_RWwrite(src, &bUseMotionCorrection, sizeof(Uint8), 1);

	SDL_RWwrite(src, &bSaveCache, sizeof(Uint8), 1);	

	//Save icon
	if(iconPath.length() > 1) iconPath = AdjustFileName(iconPath);
	iconPath.Write(src);

	//Save game title
	gameTitle.Write(src);

	//Save axis position
	double scale = axis->getScale();
	SDL_WriteLE32(src, axis->getImage()->X());
	SDL_WriteLE32(src, axis->getImage()->Y());
	SDL_RWwrite(src, &scale, sizeof(double), 1);

	//Save indexed actions
	Action::SaveActions(src, indexAction);

	//Save indexed paths
	n = 0;
	for(itPath.Begin(); !itPath.Done(); itPath.Next())
	{
		//Store index
		SDL_RWseek( src, 0, SEEK_END ); //tell error without this
		indexPath[n++] = SDL_RWtell( src );

		//Save path
		(*itPath.Value())->Save(src);
	}

	//Save indexed scripts	
	n = 0;
	for(itExpression.Begin(); !itExpression.Done(); itExpression.Next())
	{
		//Store index
		SDL_RWseek( src, 0, SEEK_END ); //tell error without this
		indexScript[n++] = SDL_RWtell( src );

		//Save script
		(*itExpression.Value())->Save(src);
	}

	//Save indexed actors
	n = 0;
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode())
			{
				//Store index
				SDL_RWseek( src, 0, SEEK_END ); //tell error without this
				indexActor[n++] = SDL_RWtell( src );
				
				//Save actor
				actor->Save(src);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////
	//Save index

	//Save action index
	SDL_RWseek( src, indexPos, SEEK_SET ); //Goto index start
	SDL_WriteLE32(src, Action::getGlobalMapActions().size());
	
	n = 0;
	for( itAction.Begin(); !itAction.Done(); itAction.Next() )
	{
		WriteString(src, (*itAction.Value())->getActionName());
		SDL_WriteLE32(src, indexAction[n++]);			
	}

	delete indexAction;


	//Save path index
	SDL_WriteLE32(src, paths.size());
	
	n = 0;
	for( itPath.Begin(); !itPath.Done(); itPath.Next() )
	{
		WriteString(src, (*itPath.Value())->getName());
		SDL_WriteLE32(src, indexPath[n++]);			
	}

	delete indexPath;


	//Save script index
	SDL_WriteLE32(src, expressions.size());
	
	n = 0;
	for( itExpression.Begin(); !itExpression.Done(); itExpression.Next() )
	{
		WriteString(src, *itExpression.Key());
		SDL_WriteLE32(src, indexScript[n++]);			
	}

	delete indexScript;


	//Save actor index
	SDL_WriteLE32(src, NumActors());

	n = 0;
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{
			Actor *actor = (*listActor)[il];
			if(actor->EditMode())
			{
				WriteString(src, actor->getCloneName());
				SDL_WriteLE32(src, indexActor[n++]);
			}
		}
	}

	delete indexActor;
	//////////////////////////////////////////////////////////////////////

	} 
	GED_CATCH
	{
		if(*SDL_GetError() != 0)
		{
			SDL_SetError("Unexpected error occurred while saving the game.\nPlease, contact support@game-editor.com");
		}
	}


	if(*SDL_GetError() != 0 && gameName.substr(0, 7) != "gedTmp_" && gameName.substr(0, 14) != "EmergencySave_")
	{
		#ifndef STAND_ALONE_GAME
		if(!bFastFile) new PanelInfo(GetErrorMsg());
		#endif //#ifndef STAND_ALONE_GAME

		//Close memory file
		free(src->hidden.mem.base);
		SDL_RWclose(src);

		return false;
	}

	//Save ok, copy to file
	bool bCloseFile = true;

	if(!srcFile) 
	{
		srcFile = ged_SDL_RWFromFile(aux.c_str(), "wb");
	}
	else
	{
		//From mem, don't close
		bCloseFile = false;
	}

	if(!srcFile)
	{
		//Close memory file
		free(src->hidden.mem.base);
		SDL_RWclose(src);

		#ifndef STAND_ALONE_GAME
		if(!bFastFile) new PanelInfo("Can't create the file to save");
		#endif 

		return false;
	}

	//Write to file
	SDL_ClearError();

	int size = src->hidden.mem.stop - src->hidden.mem.base;
	char *memBase = (char *)src->hidden.mem.base;
	
	SDL_RWwrite(srcFile, memBase, size, 1);
	
	
	//Close file
	if(bCloseFile) SDL_RWclose(srcFile);

	//Close memory file
	free(src->hidden.mem.base);
	SDL_RWclose(src);

	if(*SDL_GetError() != 0)
	{
		#ifndef STAND_ALONE_GAME
		if(!bFastFile) new PanelInfo("Can't write final file");
		#endif 	

		return false;
	}

	/*<Odilon>
	Reset the 'Modified' flag.
	Note: It is not working as expected... the confirmation
	is always shown...*/

	if(gameName.substr(0, 7) != "gedTmp_" && gameName.substr(0, 14) != "EmergencySave_")
	{
		bModified = false;
	}

#ifndef STAND_ALONE_GAME
	if(!bFastFile)
	{
		SetCaption((sGameEditorCaption + this->gameName).c_str(), NULL);
	}
#endif

	return true;
}

void GameControl::SaveIcon()
{
	if(iconPath.length() > 1)
	{
		gedString iconExt(iconPath);
		iconExt.lower();

		if(iconExt.substr(iconExt.length() - 4, 4) != ".ico")
		{
			//No SDL_image in stand alone
			//Save icon as BMP and load it with SDL_LoadBMP_RW
			
			SDL_Surface *surface = LoadSurface((gamePath+DIR_SEP+iconPath).c_str());
			SDL_SaveBMP(surface, "tmp");
			SDL_FreeSurface( surface );
			
			
			//Put in kyra resources
			KrBinaryDataResource *binData = new KrBinaryDataResource("ged Game Icon");
			if(binData->LoadFile("tmp"))
			{
				engine->Vault()->AddResource(binData);
			}
			
			remove("tmp");
		}
	}	
}

bool GameControl::InsertResourceIcon(U8 **peFile, int &peSize)
{
#ifdef WIN32
	
	//Insert a .ico icon on PE executable files (Windows and PocketPC)

	gedString iconExt(iconPath), path(gamePath+DIR_SEP+iconPath);
	iconExt.lower();

	if(iconPath.length() < 4 || iconExt.substr(iconExt.length() - 4, 4) != ".ico") return false;

	CResourceEditor re(*peFile, peSize);
	if(!re.InitOk()) return false;

	if(replace_icon(&re, IDI_ICON1, path.getCharBuf()) != 0) return false;


	unsigned long newSize;
	newSize = re.Save(NULL, newSize);

	U8 *newFile = new U8[newSize];
	if(!newFile) return false;

	if(re.Save(newFile, newSize) != 0) 
	{
		delete [] newFile;
		return false;
	}

	peSize = newSize;
	delete [] *peFile;
	*peFile = newFile;

	return true;

#else
	return false;
#endif
}

bool GameControl::SetPocketPC_VGA(U8 **peFile, int &peSize, bool bUseVGA)
{
#ifdef WIN32
	
	//Set HI_RES_AWARE to avoid legacy emulation
	//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnppcgen/html/dpi_awareness.asp
	
	
	CResourceEditor re(*peFile, peSize);
	if(!re.InitOk()) 
		return false;

	WORD disableEmulation = bUseVGA?1:0;
	re.UpdateResource("CEUX", "HI_RES_AWARE", MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (unsigned char *)&disableEmulation, sizeof(WORD));

	unsigned long newSize;
	newSize = re.Save(NULL, newSize);

	U8 *newFile = new U8[newSize];
	if(!newFile) return false;

	if(re.Save(newFile, newSize) != 0) 
	{
		delete [] newFile;
		return false;
	}

	peSize = newSize;
	delete [] *peFile;
	*peFile = newFile;

	return true;

#else
	return false;
#endif
}


void GameControl::PocketPCKeyDesc(ListPop *list, SDLKey key)
{
	char buf[256];

	SDLKey *keyTo = pocketKeys[key];
	sprintf(buf, "%s -> %s", SDL_GetKeyName(key), (*keyTo != SDLK_UNKNOWN)?SDL_GetKeyName(*keyTo):"none");

	//Humm...
	MapRemapKeysIterator it(pocketKeys);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(*it.Key() == key)
			list->SetItemData(list->AddText(buf), it.Key());
	}	
}

void GameControl::PopulatePocketPCKeys(ListPop *list)
{
	list->RemoveAll();

	/*char buf[256];

	MapRemapKeysIterator it(pocketKeys);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		sprintf(buf, "%s -> %s", SDL_GetKeyName(*it.Key()), (*it.Value() != SDLK_UNKNOWN)?SDL_GetKeyName(*it.Value()):"none");
		list->AddText(buf);
	}*/

	//Pocket PC
	PocketPCKeyDesc(list, SDLK_POCKET_UP);
	PocketPCKeyDesc(list, SDLK_POCKET_DOWN);
	PocketPCKeyDesc(list, SDLK_POCKET_LEFT);
	PocketPCKeyDesc(list, SDLK_POCKET_RIGHT);

	PocketPCKeyDesc(list, SDLK_POCKET_START);
	PocketPCKeyDesc(list, SDLK_POCKET_A);
	PocketPCKeyDesc(list, SDLK_POCKET_B);
	PocketPCKeyDesc(list, SDLK_POCKET_C);

	PocketPCKeyDesc(list, SDLK_POCKET_AUX1);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX2);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX3);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX4);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX5);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX6);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX7);
	PocketPCKeyDesc(list, SDLK_POCKET_AUX8);

	//GP2X
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_UP);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_DOWN);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_LEFT);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_RIGHT);

	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_UPLEFT);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_UPRIGHT);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_DOWNLEFT);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_DOWNRIGHT);

	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_CLICK);

	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_A);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_B);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_X);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_Y);

	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_L);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_R);

	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_START);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_SELECT);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_VOLUP);
	PocketPCKeyDesc(list, SDLK_GP2X_BUTTON_VOLDOWN);
}

void GameControl::MapPocketPCKey(SDLKey pocketKey, SDLKey pcKey)
{
	pocketKeys.Add(pocketKey, pcKey);
}



void GameControl::IgnoreEditorResources()
{
	//Tell Kyra to ignore this editor resources
	KrResourceVault::IgnoreResource("maksbutton.bmp");
	KrResourceVault::IgnoreResource("makscursorMao.bmp");
	KrResourceVault::IgnoreResource("makscursorResize1.bmp");
	KrResourceVault::IgnoreResource("makscursorResize2.bmp");
	KrResourceVault::IgnoreResource("makskyra1.jpg");
	KrResourceVault::IgnoreResource("makslistPop.bmp");
	KrResourceVault::IgnoreResource("makslistPopBig.bmp");
	KrResourceVault::IgnoreResource("maksSDL_now.gif");
	KrResourceVault::IgnoreResource("makstileSample.bmp");
	//KrResourceVault::IgnoreResource("makswaitCursor.bmp");
	KrResourceVault::IgnoreResource("gedIconCoordinate");
	KrResourceVault::IgnoreResource("ged_TutorialReminder_logo");
	KrResourceVault::IgnoreResource("ged_VariableSelector_icon");

#ifdef GAME_EDITOR_PROFESSIONAL
	KrResourceVault::IgnoreResource("maksfont.bmp"); //Keep in demo to text information
#endif
}

void GameControl::SaveEditorCanvas()
{
	MapResNamePointerIterator it(engine->Vault()->GetResourceMap());

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if((*it.Value())->Type() == KYRATAG_CANVAS)
		{
			if(((KrCanvasResource *)*it.Value())->HasClients())
			{
				KrCanvas *canvas = ((KrCanvasResource *)*it.Value())->GetFirstClient();
				if(canvas)
				{
					Actor *actor = (Actor *)canvas->getParentActor();
					
					if(actor && actor->EditMode())
					{
						((KrCanvasResource *)*it.Value())->SaveBuffer(0);
					}
				}
			}
		}
	}
}

void GameControl::RestoreEditorCanvas()
{
	MapResNamePointerIterator it(engine->Vault()->GetResourceMap());

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if((*it.Value())->Type() == KYRATAG_CANVAS)
		{
			if(((KrCanvasResource *)*it.Value())->HasClients())
			{
				KrCanvas *canvas = ((KrCanvasResource *)*it.Value())->GetFirstClient();
				if(canvas)
				{
					Actor *actor = (Actor *)canvas->getParentActor();
					
					if(actor && actor->EditMode())
					{
						((KrCanvasResource *)*it.Value())->RestoreBuffer(0);
					}
				}
			}
		}
	}
}

long CountFiles(char *prefix, long &iMin, long &iMax, char *nameOldestFile)
{
	long nFiles = 0;
	gedString file(prefix);
	file += "*.*";

	long handle = ged_findfirst(file.c_str(), A_ANY);
	iMin = 1000000, iMax = 0;

	if(handle != GED_NOTFOUND)
	{	
		do
		{
			if(!(ged_attrib(handle) & A_DIREC))
			{
				//Get min and max index
				{
					gedString name(ged_filename(handle));
					name = name.substr(strlen(prefix));
					int index = atol(name.c_str());
					
					if(index < iMin)
					{
						strcpy(nameOldestFile, ged_filename(handle));
						iMin = index;
					}
					
					iMax = max(iMax, index);
				}

				//Count
				nFiles++;
			}
			

		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);

		ged_findclose(handle);
	}

	return nFiles;
}

void GameControl::SaveEmergencyRecovery()
{
	//After crash game save
	setFastFile(true);

	if(gameName.length() != 0)
	{
		char oldpath[PATH_LIMIT];
		ged_getcwd(oldpath, PATH_LIMIT);
		chdir(gamePath.c_str());

		char prefix[32], nameOldestFile[256];
		nameOldestFile[0] = 0;
		
		//Find other emergency files
		long nEmergencyCount = 0, iMin, iMax; 
		nEmergencyCount = CountFiles("EmergencySave_", iMin, iMax, nameOldestFile);

		if(nEmergencyCount >= 10)
		{
			//Remove oldest banner
			remove(nameOldestFile);
		}

		//Don't overwrite other emergency files
		sprintf(prefix, "EmergencySave_%06ld", iMax + 1);

		gedString sufix(gameName);

		if(gameName.substr(0, 14) == "EmergencySave_")
		{
			//Remove EmergencySave_Number_
			int i = gameName.find('_', 15);
			if(i == gedString::npos) i = 14;

			sufix = gameName.substr(i+1);
		}

		gedString emergencyFile(gedString(prefix) + gedString("_") + sufix);
		if(Save(emergencyFile))
		{
			Config::Get()->setEmergencySave(gamePath + DIR_SEP + emergencyFile);
			Config::Destroy();
		}

		SDL_ClearError(); //Ignore file errors
		chdir(oldpath);
	}

	setFastFile(false);
}

void GameControl::StoreScreenSize() 
{
	screenEditWidth  = Width();
	screenEditHeight = Height();
}

void GameControl::RestoreGame()
{
	//Solve the bug:
	//When execute a LoadGame command the current game will be lost
	//Exit from game mode will restore the game even if LoadGame have been used

	if(bLoadGameUsedInGameMode)
	{
		UndoControl::Get()->LoadEmergencyData();
	}
}


int HasShadows(void *actor)
{
	if(actor && ((Actor *)actor)->getHasShadows())
	{
		return SHADOW_THICKNESS;
	}

	return 0;
}

#endif //#ifndef STAND_ALONE_GAME














