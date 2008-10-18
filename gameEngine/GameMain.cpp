#if defined(WIN32) && !defined(_WIN32_WCE)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#endif

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdlib.h>
#include "engine/kyra.h"
#include "Actor.h"
#include "GameControl.h"
#include "PathPoint.h"
#include "Path.h"
#include "Action.h"
#include "Sequence.h"
#include <math.h>
#include "system.h"
#include "PathFinder/GeoPathfinder.h"

#ifndef STAND_ALONE_GAME
#include "../gameEditor/UndoControl.h"
#endif 

#ifdef _WIN32_WCE
#include "libce/include/time.h"
#else
#include <time.h>
#endif

#if !defined(linux)
#include "SDL_opengl.h"
#endif

#ifdef GP2X

void InitGP2X()
{
	//Generic GP2X initializations here
}

void ReturnGP2X()
{
	// if GP2X is defined return to menu
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);	
}

#endif

//#defined DISABLE_WINDOWS_KEYS //Don't works in DirectX


//If APPLICATION_THREAD_TIMERS is defined, use the application thread to call timers
//This define give priority to user input
//So, the user input works fine even when have a lot timers
//Avoid the critical sections too
//Defined in SDL_timer.h


#ifdef APPLICATION_THREAD_TIMERS 
extern "C" {
#include "../SDL/SDL/src/timer/SDL_timer_c.h"
}
#endif

/*#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
#	include "../gameEditor/AdsControl.h"
#endif*/

#if !defined(STAND_ALONE_GAME)
#	include "../gameEditor/Tutorial.h"
#	include "../gameEditor/MainPanel.h"
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
#	include "../gameEditor/pocketpc_resource.h"
extern "C" HWND SDL_Window;
extern "C" HINSTANCE SDL_Instance;
#endif

#ifndef STAND_ALONE_GAME
extern Actor *Game_Main();
#include "../gameEditor/Config.h"
#elif defined(WIN32)
void *GetMainWindow() {return SDL_Window;}
#endif //#ifndef STAND_ALONE_GAME


Uint32 startKyraSplash;
bool bShutDown = false;

#include "dlmalloc.h"
#if !defined(DISABLE_DLMALLOC) && defined(USE_MALLOC_LOCK)
extern "C" SDL_mutex *mallocMutEx;
#endif

extern "C" void *SDL_GetModuleHandle();




///////////////////////////////////////////////////////////////////////////////
//Small size linker definitions
#if defined(NDEBUG) && defined(WIN32_oi)
// /Og (global optimizations), /Os (favor small code), /Oy (no frame pointers)
#pragma optimize("gsy",on)

#pragma comment(linker,"/RELEASE")

// Note that merging the .rdata section will result in LARGER exe's if you using
// MFC (esp. static link). If this is desirable, define _MERGE_RDATA_ in your project.
#ifdef _MERGE_RDATA_
#pragma comment(linker,"/merge:.rdata=.data")
#endif // _MERGE_RDATA_

#pragma comment(linker,"/merge:.text=.data")
#pragma comment(linker,"/merge:.reloc=.data")

#if _MSC_VER >= 1000
// Only supported/needed with VC6; VC5 already does 0x200 for release builds.
// Totally undocumented! And if you set it lower than 512 bytes, the program crashes.
// Either leave at 0x200 or 0x1000
#pragma comment(linker,"/FILEALIGN:0x200")
#endif // _MSC_VER >= 1000

#pragma comment(linker,"/OPT:REF")
//#pragma comment(linker,"/FIXED:NO")
//#pragma comment(linker,"/ALIGN:0x200")
//#pragma comment(linker,"/NODEFAULTLIB")
//#pragma comment(lib,"LIBCTINY.LIB")

#endif // NDEBUG
///////////////////////////////////////////////////////////////////////////////


#ifdef _0_
/* This is a way of telling whether or not to use hardware surfaces */
Uint32 FastestFlags(Uint32 flags, int width, int height, int bpp)
{
	const SDL_VideoInfo *info;

	/* Hardware acceleration is only used in fullscreen mode */
	flags |= SDL_FULLSCREEN;

	/* Check for various video capabilities */
	info = SDL_GetVideoInfo();
	if ( info->blit_hw_CC && info->blit_fill ) {
		/* We use accelerated colorkeying and color filling */
		flags |= SDL_HWSURFACE;
	}
	/* If we have enough video memory, and will use accelerated
	   blits directly to it, then use page flipping.
	 */
	if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
		/* Direct hardware blitting without double-buffering
		   causes really bad flickering.
		 */
		if ( info->video_mem*1024 > (height*width*bpp/8) ) {
			flags |= SDL_DOUBLEBUF;
		} else {
			flags &= ~SDL_HWSURFACE;
		}
	}

	/* Return the flags */
	return(flags);
}
#endif







#if !defined(STAND_ALONE_GAME) && defined(WIN32)

typedef BOOL (CALLBACK *LPGETLOGFILE) (LPVOID lpvState);
typedef LPVOID (*FUNC_INSTALL)(LPGETLOGFILE pfn, LPCTSTR lpcszTo, LPCTSTR lpcszSubject);
typedef LONG (*FUNC_FILTER)(PEXCEPTION_POINTERS pExInfo);
FUNC_FILTER CustomExceptionFilter = NULL;


bool InstallCrashReportSystem()
{
	//Install crash report system
	//Don't works with Armadillo
	HINSTANCE crashHandle = LoadLibrary(TEXT("CrashRpt.dll"));
	if(crashHandle)
	{		
		FUNC_INSTALL Install	= (FUNC_INSTALL)GetProcAddress(crashHandle, TEXT("Install"));
		if(Install)
		{
			CustomExceptionFilter = (FUNC_FILTER)GetProcAddress(crashHandle, TEXT("CustomExceptionFilter"));
			Install(NULL, TEXT("support@game-editor.com"), TEXT("[CRASH] Game Editor Report"));
			return true;
		}
	}

	return false;
}

// unhandled exception callback set with SetUnhandledExceptionFilter()
LONG WINAPI SaveUnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
	//Ok, Game Editor is in a crash
	//Try save the work
	
	//GameControl::Get()->SaveEmergencyRecovery();
	
	//Pass crash to system
	return EXCEPTION_CONTINUE_SEARCH;
}

#endif


#ifdef APPLICATION_THREAD_TIMERS
extern volatile bool bCallGameTick;
#endif



extern "C" int CheckGameModeAbort() //maks
{
#ifndef STAND_ALONE_GAME

	static int count = 0; //Don't delay script execution

	if(count > 1000)
	{
		//If get only one event or event buffer is greater than 16
		//the ESC key don't will be catched
		//So, empty the first events

		SDL_Event event[16];
		SDL_PumpEvents();
		int n = 0;
		if((n = SDL_PeepEvents(event, 16, SDL_PEEKEVENT, SDL_KEYUPMASK)) > 0) 
		{
			for(int i = 0; i < n; i++)
			{
				if(event[i].key.keysym.sym == SDLK_ESCAPE)
				{
					return 1;
				}
			}
		}

		//Keep only the last MAX_KEY_SEQUENCE keys on buffer
		//To avoid full event buffers
		if(n >= 16)
		{
			SDL_PeepEvents(event, 16 - MAX_KEY_SEQUENCE, SDL_GETEVENT, SDL_KEYUPMASK);
		}

		count = 0;
	}

	count++;

#endif
	
	return 0;
}

int SDL_GE_WaitEvent(SDL_Event *event)
{
	//Manage timers
	while ( 1 ) 
	{
		//Check network
		//Need it here to speed up network processing
		GameControl::Get()->ProcessNetwork(false); 

		//Pump the system events
		SDL_PumpEvents();

		//There is pending conections?
		/*if(GameControl::Get()->WaitingForConnectionComplete())
		{
			//Wait the requested connections to complete
			event->type = SDL_WAIT_FOR_CONNECT;
			SDL_Delay(30);
			return 1;
		}*/

		//Check other events
		switch(SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS)) 
		{
		    case -1: return 0;
		    case 1: return 1;
		    case 0: 
				{
#ifdef APPLICATION_THREAD_TIMERS
					//Call only if not other events
					//Give priority to user input
					if(!SDL_ThreadedTimerCheck())
					{
						SDL_Delay(TIMER_RESOLUTION/*10*/);
					}
#else
					SDL_Delay(TIMER_RESOLUTION/*10*/);
#endif
				}
		}

		
#ifdef APPLICATION_THREAD_TIMERS		
		//Can draw a frame?	
		if(GameControl::CanDraw())
		{
			event->type = SDL_TIMER_EVENT;
			return 1;
		}
#endif
	}	
}

class stMem
{
public:
	stMem() {mem = NULL; size = 0; used = 0; surface = NULL;}
	stMem(void **_mem, int _size) {mem = _mem; size = _size; used = 1; surface = NULL;}
	void **mem;
	SDL_Surface *surface; //Used for SDL blitter
	int size;
	int used;
};

GlSList<stMem *> memQueue; 
int totalQueueMem = 0;

extern "C" void free_gc(void *mem)
{
	if(mem)
	{
		void *control = ((U8 *)mem) - 4;
		stMem *pMem = (stMem *)(*((U32 *)control));

		if(pMem)
		{
			if(pMem->mem) *(pMem->mem) = NULL;
			pMem->mem = NULL;
			pMem->used = 0;
			*((U32 *)control) = 0;

			if(pMem->surface) 
			{
				SDL_FreeSurface(pMem->surface);
				pMem->surface = NULL;
			}
		}

		dlfree(control);
	}
}

extern "C" void using_mem_gc(void *mem)
{
	if(mem)
	{
		void *control = ((U8 *)mem) - 4;
		stMem *pMem = (stMem *)(*((U32 *)control));
		pMem->used = 1;
	}
}

extern "C" void available_mem_gc(void *mem)
{
	if(mem)
	{
		void *control = ((U8 *)mem) - 4;
		stMem *pMem = (stMem *)(*((U32 *)control));
		pMem->used = 0;
	}
}

extern "C" SDL_Surface **getsurface_mem_gc(void *mem)
{
	if(mem)
	{
		void *control = ((U8 *)mem) - 4;
		stMem *pMem = (stMem *)(*((U32 *)control));
		return &(pMem->surface);
	}

	return NULL;
}

extern "C" int ReleaseMemory(int bytes)
{
	int deallocated = 0;
	GeoPathFinder::Remove();
	ActorMessage::Remove();
	if(GameControl::Get()) GameControl::Get()->RemoveOldActorsFromCache(true);


	while(!memQueue.Empty() && deallocated < 2*bytes)
	{
		stMem *mem = memQueue.Front();

		if(!mem->used)
		{			
			if(mem->mem && *(mem->mem))
			{
				free_gc(*(mem->mem));
				deallocated += mem->size;
			}

			if(mem->surface) 
			{
				SDL_FreeSurface(mem->surface);
				mem->surface = NULL;
			}
			
			totalQueueMem -= mem->size;
			
			dlfree(mem);
			memQueue.PopFront();
		}
		else
		{
			break;
		}
	}

	return (deallocated >= bytes)?1:0;
}

extern "C" void malloc_gc(void **mem, int bytes)
{
	//Very simple garbage collector
	//Put the address on queue to remove in low memory conditions
	//Must call free_gc to free mem or when mem is not more a valid pointer

#if defined(_WIN32_WCE) || defined(GP2X)

	while(totalQueueMem > 4*1024*1024)
	{
		//More than 4MB allocated
		//Try release 1MB
		ReleaseMemory(1024*1024);
	}

#endif
	
	*mem = NULL;
	bytes += 4; //control + mem

	//control pointer to pMem (must be 32 bit aligned)
	U32 *control = (U32 *)dlmalloc(bytes); 

	if(control)
	{
		*mem = ((U8 *)control) + 4;
		stMem *pMem = (stMem *)dlmalloc(sizeof(stMem));

		if(pMem)
		{
			*control = (U32)pMem;

			pMem->mem = mem;
			pMem->size = bytes;
			pMem->used = 1;
			pMem->surface = NULL;

			totalQueueMem += bytes;
			
			memQueue.PushBack(pMem);
		}
		else
		{
			dlfree(*mem);
			*mem = NULL;
		}
	}
}

bool bShowExitErrorMessage = true;
extern "C" void ExitError()
{
	if(bShowExitErrorMessage)
	{
		Mix_CloseAudio();
		SDL_Quit();

#ifdef GP2X
		ReturnGP2X();
#endif

#ifdef WIN32
		MessageBox(GetForegroundWindow(), TEXT("Not Enough Memory to execute the game"), TEXT("Error 2"), MB_OK | MB_ICONERROR); 
#else
		printf("Error 2: Not Enough Memory to execute the game\n");
#endif

		bShowExitErrorMessage = false;
	}
}

extern "C" int MemoryAllocationError(int bytes)
{
	//Called if has memory errors
	//If returns 1, try allocate again

	//Is game exiting?
	if(bShutDown) 
		return 0;

	//Try release memory
	if(ReleaseMemory(bytes)) return 1;
	
	//No memory, exit
	bShowExitErrorMessage = false;

	Mix_CloseAudio();
	SDL_Quit();

#ifdef GP2X
		ReturnGP2X();
#endif
	
#ifdef WIN32
	MessageBox(GetForegroundWindow(), TEXT("Not Enough Memory to execute the game"), TEXT("Error"), MB_OK | MB_ICONERROR); 
#else
	printf("Error: Not Enough Memory to execute the game\n");
#endif
	
#ifdef _WIN32_WCE
	TerminateProcess(GetModuleHandle(NULL), 5);
#else
	exit(5);
#endif

	return 0;
}

void WrongNetworkGameVersion()
{
	//Called if the current game is older than other games on the network

#define GAME_VERSION_ERROR "In order to execute this game over the network, you need the latest version of the game.\nPlease, contact the game vendor to get the newest version.\n"
	
#ifdef STAND_ALONE_GAME
	//Shutdown the game
	Mix_CloseAudio();
	SDL_Quit();


#ifdef GP2X
		ReturnGP2X();
#endif
	
#ifdef WIN32
	MessageBox(GetForegroundWindow(), TEXT(GAME_VERSION_ERROR), TEXT("Error"), MB_OK | MB_ICONERROR); 
#else
	printf(GAME_VERSION_ERROR);
#endif
	
#ifdef _WIN32_WCE
	TerminateProcess(GetModuleHandle(NULL), 10);
#else
	exit(10);
#endif	
#else
	//Exit game mode
	Script::SetError(GAME_VERSION_ERROR);

	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));
	event.type = SDL_KEYUP;
	event.key.keysym.sym = SDLK_ESCAPE;
	SDL_PushEvent(&event);
#endif
}

//#define USE_SANDBOX 32

#if defined(USE_SANDBOX) && defined(DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
void CreateSandBox()
{
	//Limits
	//Use the 256MB limit in debug mode to avoid system crash
	DWORD dwMemoryLimit = USE_SANDBOX*1024*1024;

	// Create a job kernel object.
	HANDLE hjob = ::CreateJobObject(NULL, NULL);
	
	// Place some restrictions on processes in the job.

	// First, set some basic restrictions.
	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };// First, set some basic restrictions.

	// The process always runs in the idle priority class.
	//jobli.PriorityClass = IDLE_PRIORITY_CLASS;
  

	// set some extended restrictions.
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobeli = { 0 };

	
    jobli.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	
    ::memcpy(&jobeli.BasicLimitInformation, &jobli, sizeof(jobli));
	
    jobeli.JobMemoryLimit = dwMemoryLimit;
	
    if(!::SetInformationJobObject(hjob, JobObjectExtendedLimitInformation, &jobeli, sizeof(jobeli)))
	{
		::CloseHandle(hjob);
		return;
	}
	
	// Place this process in the job.
	HANDLE hprocess = GetCurrentProcess();
	if(!::AssignProcessToJobObject(hjob, hprocess))
	{
		DWORD error = GetLastError();
		::CloseHandle(hjob);
	}
}
#endif







gedString sGameEditorCaption("Game Editor -");
SDL_Joystick *joystick = NULL;

static int BenchMark(SDL_Surface *screen)
{
	U32 ts = 0, te = 0;
	int i, x, y;

	//Init
	SDL_ClearError();
	engine = new KrEngine( screen );

	//Create sprite
	KrCanvasResource *canvasResource = new KrCanvasResource("ged_BenchMark", 
		screen->w,
		screen->h,
		true );
	
	//Create a tansparent actor
	KrRGBA *pixels = canvasResource->Pixels(), color1, color2;
	
	color1.Set(1, 1, 1, 255);
	color2.Set(1, 1, 1, 0);
	
	//Fill
	i = 0;
	for(y = 0; y < canvasResource->Height(); y++)
	{
		for(x = 0; x < canvasResource->Width(); x++)
		{
			if(x % 2 && y % 2) pixels[i] = color1;
			else pixels[i] = color2;

			i++;
		}
	}
	
	//Add
	engine->Vault()->AddResource(canvasResource);
	
	
	KrCanvas *canvas1 = new KrCanvas( canvasResource, NULL );
	canvas1->SetNodeId((int)canvas1);
	engine->Tree()->AddNode( NULL, canvas1 );

	KrCanvas *canvas2 = new KrCanvas( canvasResource, NULL );
	canvas2->SetNodeId((int)canvas2);
	engine->Tree()->AddNode( NULL, canvas2 );
	


	//Test
	x = 0;
	ts = SDL_GetTicks();

	for(i = 0; i < 5; i++)
	{
		engine->InvalidateScreen();

		canvas1->SetPos(x, 0);
		canvas2->SetPos(-x, 0);

		x++;

		engine->Draw();
	}
		
		
	te = SDL_GetTicks();


	//Cleanup
	engine->Tree()->DeleteNode(canvas1);
	engine->Tree()->DeleteNode(canvas2);

	
	engine->Vault()->RemoveCanvasResource(canvasResource);	


	delete engine;
	engine = NULL;
	SDL_ClearError();


	return te - ts;
}

SDL_Surface *SetVideoMode(int width, int height, int bpp, Uint32 flags)
{
	//If opengl is requested, allow only if pass on benchmark test
	static bool bOpenGLTested = false;
	static bool bCanUseOpenGL = true;

#ifdef GP2X
	bpp = 16;
#endif


	if(
		//bOpenGLTested && !bCanUseOpenGL && //opengl:teste: remove comment to enable OpenGL 
		(flags & SDL_OPENGL)) 
	{
		flags &= ~SDL_OPENGL;
	}

	SDL_Surface *screen = SDL_SetVideoMode(width, height, bpp, flags);

	if((flags & SDL_OPENGL) && (!screen || !(screen->flags & SDL_OPENGL)))
	{
		//Can't open OpenGL
		//Open again without OpenGL to avoid crash
		screen = SDL_SetVideoMode(width, height, bpp, flags & ~SDL_OPENGL);
	}

	if(!screen) return NULL;	

	if(!bOpenGLTested)
	{		
		bOpenGLTested = true;
		
		if(flags & SDL_OPENGL)
		{
			//OpenGL requested
			
			if(0) //opengl:teste //for force opengl if(screen->flags & SDL_OPENGL)
			{
				//OpenGL loaded
				//Can use it? (Is hardware or software?)
				
				//Don's test vendor (some day Microsoft can make a better work)
				//const char *vendorName = (const char *)SDL_CurrentVideo->glGetString(GL_VENDOR);
				//if(vendorName)
				{
					/*gedString vendor(vendorName);
					
					vendor.lower();
					if(vendor.find("microsoft") != gedString::npos)
					{
						bCanUseOpenGL = false;
					}
					else*/
					{
						//Make a little banchmark
						//No benchmarks with OpenGL ES

#ifndef GL_OES_VERSION_1_1
						int timeGL = BenchMark(screen);

						//Without opengl
						screen = SDL_SetVideoMode(width, height, bpp, flags & ~SDL_OPENGL);

						int timeBlitter = BenchMark(screen);

						if(timeGL > timeBlitter)
						{
							bCanUseOpenGL = false;
						}
						else
						{
							//Restore OpenGL							
							screen = SDL_SetVideoMode(width, height, bpp, flags);
						}
#endif
					}
				}
			}
			
			if(!bCanUseOpenGL && screen->flags & SDL_OPENGL)
			{
				screen = SDL_SetVideoMode(width, height, bpp, flags & ~SDL_OPENGL);

				//Can show a message here, if is not stand alone game
			}
		}
	}

	return screen;
}

bool EngineTick()
{
	SDL_Event event;
	static int mouseX1 = -1, mouseY1 = -1;
	
	if(SDL_GE_WaitEvent(&event))
	{
		if(event.type == SDL_MOUSEMOTION)
		{
			//Avoid mistake SDL_MOUSEMOTION events
			if(event.motion.x != mouseX1 || event.motion.y != mouseY1)
			{
				mouseX1 = event.motion.x;
				mouseY1 = event.motion.y;
			}
			else
				return true;
		}
		
#if !defined(STAND_ALONE_GAME)
		if(Tutorial::IsOff(event))
		{
			if(!GameControl::Get()->GameTick(event))
				return false;
		}
		else if(Tutorial::InRecMode())
		{
			Tutorial::Get()->Store(event);
			
			if(!GameControl::Get()->GameTick(event))
				return false;
		}
		else if(Tutorial::InPlayMode())
		{
			if(!Tutorial::Get()->ExecuteMoves(event))
				return false;
			
			if(Tutorial::Get()->CanExecute(event))
			{
				if(!GameControl::Get()->GameTick(event))
					return false;
			}
		}
#else
		if(!GameControl::Get()->GameTick(event))
			return false;
#endif
		
		return true;
	}
	
	return false;
}

void EngineShutDown()
{
	bShutDown = true;

	// Close if opened
	if(joystick && SDL_JoystickOpened(0))
	{
		SDL_JoystickClose(joystick);
		joystick = NULL;
	}

	
	GameControl::Get()->SetGameMode(false);
	GameControl::Get()->Delete();

#ifndef STAND_ALONE_GAME
	Config::Destroy();
#endif

	
	delete engine;


	Mix_CloseAudio();

	

/*#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	//Wait for banners
	SDL_AudioQuit();
	SDL_VideoQuit();

	if(!AdsControl::Get()->getBannerFromNet())
	{
		SDL_WaitThread(AdsControl::Get()->GetThread(), NULL);		
	}
#endif*/
	

		
	SDL_Quit();

	
#ifdef GP2X
		ReturnGP2X();
#endif	

}


void EngineStart()
{
	//Initialize SDL engine
	SDL_Surface* screen;

	
#if !defined(DISABLE_DLMALLOC) && defined(USE_MALLOC_LOCK)
	mallocMutEx = SDL_CreateMutex();
#endif



	const SDL_version* sdlVersion = SDL_Linked_Version();
	if ( sdlVersion->minor < 2 )
	{
		printf( "SDL version must be at least 1.2.0" );
		exit( 254 );
	}

	/* Initialize the SDL library */
	U32 initFlags = SDL_INIT_FLAGS;

#ifndef APPLICATION_THREAD_TIMERS
	initFlags |= SDL_INIT_TIMER;
#endif

	if ( SDL_Init(initFlags) < 0 )
	{
		printf( "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(255);
	}

	if((initFlags & SDL_INIT_JOYSTICK) && SDL_NumJoysticks() > 0)
	{
		//Open first joystick
		joystick = SDL_JoystickOpen(0);
	}

	
	new GameControl();	

	/* Create a display for the image */
	screen = SetVideoMode(GameControl::Get()->getGameWidth(), GameControl::Get()->getGameHeight(), 0, 
		(VIDEO_FLAGS & SDL_OPENGL)?(VIDEO_FLAGS & ~SDL_OPENGL):VIDEO_FLAGS //Always starts without opengl to avoid startup delays
		);	

	if ( screen == NULL )  
	{
		exit(3);
	}

	/*if((VIDEO_FLAGS & SDL_OPENGL) && !(screen->flags & SDL_OPENGL)) //opengl:teste 
	{
#ifdef WIN32
	MessageBox(GetForegroundWindow(), TEXT("Can't open OpenGL"), TEXT("Error"), MB_OK | MB_ICONERROR); 
#endif
	}*/

	//Caption
	if(!(screen->flags & SDL_OPENGL))
	{
		//Place a '-' more to indicate not acceleration
		sGameEditorCaption += '-';
	}

	sGameEditorCaption += ' ';

	//new GameControl();

	//Initialize Kyra engine
	SDL_ClearError();
	engine = new KrEngine( screen );

	GameControl::Get()->CreateAxis();	

	// Start timing!
	//GameControl::Get()->SetTimer();

	//Init random seed
	int mouseX, mouseY;
	time_t timer;	

	SDL_GetMouseState(&mouseX, &mouseY);
	time( &timer );
	
	int seed = SDL_GetTicks() + mouseX + mouseY + timer;
	srand(seed);
}

void EngineLoad(const char *gamePath)
{
	if(!GameControl::Get()->getStandAloneMode())
	{
		//Load editor resources
		{
		EditorDirectory editor;
		GameControl::Get()->ReadEditorResources();
		}
		
		//Window dressing crap
		SetCaption((sGameEditorCaption + "Untitled").c_str(), NULL);
		//SDL_WM_SetIcon(LoadSurfaceEditor("maksiconActor.gif"), NULL); //Don't set icon here (icon already in file)

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);


		#ifndef STAND_ALONE_GAME
		//Main panel
		GameControl::Get()->SetMainActor(Game_Main());

		#ifdef __WIN32__
		//Install report
		if(!InstallCrashReportSystem()) 
		{
			//Install recovery system
			//InstallCrashReportSystem don't works with recovery system
			SetUnhandledExceptionFilter(SaveUnhandledExceptionFilter);
		}

		DragAcceptFiles((HWND)GetMainWindow(), TRUE);
		#endif

			
		
		/******************************************
		<Odilon>
		Para abrir passando o nome do jogo na linha de comando.
		NOTA: Eu quis fazer o mais simples possivel...
		******************************************/
		if (gamePath)
		{
			gedString pathJogo = gamePath;
			int p;
			
			p = pathJogo.rfind(DIR_SEP);
			if (p != gedString::npos) //sucesso
			{
				/*gedString nomeJogo = pathJogo.substr(p + 1);
				gedString dirJogo  = pathJogo.substr(0, p);
				
				chdir(dirJogo.c_str());
				int x = GameControl::Get()->Width() / 2;
				int y = GameControl::Get()->Height() / 2;
				GameControl::Get()->SetMouse(x, y);
				if (!GameControl::Get()->Load(nomeJogo, true))
				{
					return 1;
				}*/

				//Open editor if not load the game
				MainPanel::Get()->LoadGame(pathJogo, false);
			}
		}
		/*****************************************/

		#endif //#ifndef STAND_ALONE_GAME
	}
	
	GameControl::Get()->StartGameRender();
}

/*extern "C" */int SDL_main1( int argc, char *argv[] )
{
#ifndef STAND_ALONE_GAME 
	if(argc == 2 && argv[1] && strlen(argv[1]) > 5)
	{
		//Trying open the undo file?
		gedString sUndo(argv[1]);
		sUndo.lower();

		if(sUndo.substr(sUndo.length() - 5, 5) == ".undo")
		{
			//Extract the ged files and exit
			int n = UndoControl::Get()->ExtractAllFiles(argv[1]);

			if(n > 0)
			{
				printf("Extracted %ld ged files.\n");
			}
			else
			{
				printf("Can't recover ged files from undo.\n");
			}

			return 0;
		}
	}
#endif

	EngineStart();	
	
	

	//Load stand alone game
	{
	gedString file, dir;

#ifdef STAND_ALONE_GAME 
	if(argc == 2)
	{
		//Use second parm to open		
		SplitPath(argv[1], file, dir);		
	}

	if(dir.empty())
	{
		//Try first
		SplitPath(argv[0], file, dir);
	}

#ifdef WIN32
	if(dir.empty())
	{
		//May be an execution from the command prompt
		char buf[PATH_LIMIT];
		dir = ged_getcwd(buf, PATH_LIMIT);
	}
#endif

#else
	SplitPath(argv[0], file, dir);
#endif



	GameControl::Get()->setHomePath(dir);

	if(!GameControl::Get()->CheckStandAloneMode(file)) 
	{
		Mix_CloseAudio();
		SDL_Quit();
	
#ifdef GP2X
		ReturnGP2X();
#endif
		
#if defined(STAND_ALONE_GAME)
		if(*SDL_GetError() != 0)
		{
			#ifdef _WIN32_WCE

			TCHAR uBuf[256];	
			
			MultiByteToWideChar(CP_ACP, 0, SDL_GetError(), -1, uBuf, 255);
			MessageBox(GetForegroundWindow(), uBuf, _T("Error"), MB_OK | MB_ICONERROR); 

			#elif WIN32	
			MessageBox(NULL, SDL_GetError(), "Error", MB_OK | MB_ICONERROR); 
			#else
			printf("%s\n", SDL_GetError());
			#endif
		}
#endif
		GameControl::Get()->Delete();
		return 1;
	}
	}

#ifdef WIN32
	//Set Window icon
	HICON hIcon = ::LoadIcon(SDL_Instance, MAKEINTRESOURCE(IDI_ICON1));
	::SendMessage((HWND)GetMainWindow(), WM_SETICON, 1, (LPARAM)hIcon); //big
	::SendMessage((HWND)GetMainWindow(), WM_SETICON, 0, (LPARAM)hIcon); //small
#endif

	char *gamePath = NULL;
	if(argc == 2 && argv[1])
	{
		gamePath = argv[1];
	}
	
	EngineLoad(gamePath);


/*#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	//Start ad control
	AdsControl::Get();
#endif*/
	
	
	


	


#ifdef __WIN32__
	//Maximize after start the render
	if(!GameControl::Get()->getStandAloneMode())
	{
		//Maximize editor on Windows	
		ShowWindow((HWND)GetMainWindow(), SW_MAXIMIZE);
	}
#endif


	//Main loop (See Panel::Wait() too)
	while(EngineTick());	
	EngineShutDown();

	return 0;
}



#if defined(DISABLE_WINDOWS_KEYS) && defined(WIN32) && !defined(_WIN32_WCE)
//Disable the Windows keys
//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/TechnicalArticles/DisablingShortcutKeys.asp



HHOOK g_hKeyboardHook = NULL; 
extern "C" BOOL g_bWindowActive;

LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if( nCode < 0 || nCode != HC_ACTION ||
		!GameControl::Get() || !GameControl::Get()->getGameMode() )  // do not process message 
        return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam); 
 
    bool bEatKeystroke = false, g_bFullscreen = false;
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;	

	//g_bFullscreen = GameControl::Get()->getFullScreen();

    switch (wParam) 
    {
        case WM_KEYDOWN:  
        case WM_KEYUP:    
        {
            bEatKeystroke = (/*g_bFullscreen &&*/ /*g_bWindowActive &&*/ ((p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN)));
            break;
        }
    }


    if( bEatKeystroke )
        return 1;
    else
        return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam );
}


#endif




////////////////////////////////////////////////////////////////////////
//Single instance
//Without, when open a Game Editor, edit, and open other, will show the failure message

#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(_WIN32_WCE)
#include <windows.h>

//This code is from Q243953 in case you lose the article and wonder
//where this code came from.
class CLimitSingleInstance
{
protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;
	
public:
	CLimitSingleInstance(TCHAR *strMutexName)
	{
		//Make sure that you use a name that is unique for this application otherwise
		//two apps may think they are the same if they are using same name for
		//3rd parm to CreateMutex
		m_hMutex = CreateMutex(NULL, FALSE, strMutexName); //do early
		m_dwLastError = GetLastError(); //save for use later...
	}
	
	~CLimitSingleInstance()
	{
		if (m_hMutex)  //Do not forget to close handles.
		{
			CloseHandle(m_hMutex); //Do as late as possible.
			m_hMutex = NULL; //Good habit to be in.
		}
	}
	
	BOOL IsAnotherInstanceRunning()
	{
		return (ERROR_ALREADY_EXISTS == m_dwLastError);
	}
};

// The one and only CLimitSingleInstance object.
// Change what is passed to constructor. GUIDGEN Tool may be of help.
CLimitSingleInstance g_SingleInstanceObj(TEXT("ged_Single_Instance_Mutex"));

extern "C" TCHAR *RemovePath(TCHAR *file);
#endif
////////////////////////////////////////////////////////////////////////



extern "C" int SDL_main( int argc, char *argv[] )
{
#if defined(USE_SANDBOX) && defined(DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
	CreateSandBox();
#endif

#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(_WIN32_WCE)

	//Check single instance
	if(0) //no single instance now if (g_SingleInstanceObj.IsAnotherInstanceRunning())
	{
		//This is a second instance. 

		//Get the windows class name
		char lpFilename[256], *appName = NULL;
		int len = GetModuleFileName((HINSTANCE)SDL_GetModuleHandle(), lpFilename, 256);
		HWND hWnd = NULL;

		if(len)
		{
			if(len > 4 && lpFilename[len - 4] == '.') lpFilename[len - 4] = 0;
			appName = RemovePath(lpFilename);
		}
		
		if(appName)
		{
			//Bring the original instance to the top.
			hWnd = FindWindow(appName, NULL);
			if(hWnd) SetForegroundWindow(hWnd);

			if(appName != lpFilename)
			{
				free(appName);
			}
		}
		
		if(hWnd && argc == 2 && argv[1] && strlen(argv[1]) != 0)
		{
			//User may be try open a game in explorer
			//Send the game path to first instance

			COPYDATASTRUCT cds;
			cds.cbData = strlen(argv[1]);
			cds.lpData = argv[1];
			
			//argv[1] still valid with SendMessage
			SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cds);
		}
		
		return 0;
	}

#endif


#if defined(DISABLE_WINDOWS_KEYS) && defined(WIN32) && !defined(_WIN32_WCE)
		//Disable the Windows keys
		//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/TechnicalArticles/DisablingShortcutKeys.asp

		//Always disable Windows keys, even if not in game mode
		g_hKeyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL,  LowLevelKeyboardProc, (HINSTANCE)SDL_GetModuleHandle(), 0 );					
#endif

#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(LLVM)
	EXCEPTION_POINTERS *xp;	
    __try {
#endif

	atexit(ExitError);
	SDL_main1( argc, argv );
	bShowExitErrorMessage = false;

#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(LLVM)

	
	//} __except(CustomExceptionFilter(xp = GetExceptionInformation())) 
	} __except((xp = GetExceptionInformation()), (CustomExceptionFilter?CustomExceptionFilter(xp):EXCEPTION_CONTINUE_SEARCH))
	{
		
    }

#endif



#if defined(DISABLE_WINDOWS_KEYS) && defined(WIN32) && !defined(_WIN32_WCE)
		//Disable the Windows keys
		//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/TechnicalArticles/DisablingShortcutKeys.asp
		if(g_hKeyboardHook) 
		{			
			UnhookWindowsHookEx( g_hKeyboardHook );
			g_hKeyboardHook = NULL;
		}
#endif

	return 0;
}


#if defined(GP2X) || defined(LLVM)
#include "SDL_main.h"
#ifdef main
#undef main
#endif


extern "C" int main(int argc, char *argv[])
{
#ifndef LLVM
	InitGP2X();
#endif

	return(SDL_main(argc, argv));
}

#endif