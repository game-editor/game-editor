/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org

	GAPI by Makslane Araujo Rodrigues, makslane@hotmail.com
*/



#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gapivideo.c,v 1.0 2003/07/29 20:31:34 slouken Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include "../../../../../gameEditor/pocketpc_resource.h"

#include "../../../../../gameEngine/dlmalloc.h" //maks

//#define _USE_BACK_BUFFER_ //20 fps (fps benchmark.ged)

void halCreate(HWND hwnd, int width, int height);
void halClose();
void *halGetBuffer();
void halGetDimensions(int *width, int *height);
void halSwap(void *backBufer);
int halWidth, halHeight;
void CloseGAPI();
int IsSmartphoneDevice();
void writeDebugInfo(const char *s);


#if defined(WIN32_PLATFORM_PSPC)
#include <aygshell.h>                      // Add Pocket PC includes
#pragma comment( lib, "aygshell" )         // Link Pocket PC library
#endif



/* Not yet in the mingw32 cross-compile headers */
#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN	4
#endif

#include "SDL.h"
#include "SDL_mutex.h"
#include "SDL_syswm.h"
#include "SDL_sysvideo.h"
#include "SDL_sysevents.h"
#include "SDL_events_c.h"
#include "SDL_pixels_c.h"
#include "SDL_syswm_c.h"
#include "SDL_sysmouse_c.h"
#include "SDL_gapivideo.h"
#include "SDL_gapievents_c.h"
#include "SDL_loadso.h"
#include "SDL_wingl_c.h"

/////////////////////////////////////////////////////////////////
//maks: The declarations from regext.h and snapi.h are included here, because there is no such declaration for Pocket PC 2003
//http://code.game-editor.com/ticket/16
//http://msdn.microsoft.com/en-us/library/bb677132.aspx
//

// PhoneIncomingCall
// Gets a value indicating whether there is an incoming (ringing) call.
#define SN_PHONEINCOMINGCALL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALL_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALL_VALUE TEXT("Status")
#define SN_PHONEINCOMINGCALL_BITMASK 65536

DECLARE_HANDLE(HREGNOTIFY); // transient notification handle

typedef enum tagREG_COMPARISONTYPE 
{ 
    REG_CT_ANYCHANGE,
    REG_CT_EQUAL,
    REG_CT_NOT_EQUAL,
    REG_CT_GREATER,
    REG_CT_GREATER_OR_EQUAL,
    REG_CT_LESS,
    REG_CT_LESS_OR_EQUAL,
    REG_CT_CONTAINS,
    REG_CT_STARTS_WITH,
    REG_CT_ENDS_WITH
} REG_COMPARISONTYPE;


typedef struct tagNOTIFICATIONCONDITION
{
    REG_COMPARISONTYPE ctComparisonType;
    DWORD dwMask;
    union
    {
        LPCTSTR psz;
        DWORD dw;
    } TargetValue;
} NOTIFICATIONCONDITION;


		

typedef BOOL (*ShowFull)( HWND, DWORD );

typedef HRESULT (*tRegistryNotifyWindow)(HKEY ,
                                    LPCTSTR ,
                                    LPCTSTR ,
                                    HWND ,
                                    UINT ,
                                    DWORD ,
                                    NOTIFICATIONCONDITION *,
                                    HREGNOTIFY *);

typedef HRESULT (*tRegistryCloseNotification)(HREGNOTIFY);

HREGNOTIFY hregNotifyPhone = NULL;
void CloseIncomingCallNotification()
{
	tRegistryCloseNotification  lRegistryCloseNotification = NULL;
	HINSTANCE hInstAygShell = NULL;
	
	if(hregNotifyPhone)
	{
		hInstAygShell = LoadLibrary( _T( "AygShell.dll" ) );
		if(hInstAygShell)
		{
			lRegistryCloseNotification = GetProcAddress(hInstAygShell, _T( "RegistryCloseNotification" ) );

			if(lRegistryCloseNotification)
			{
				lRegistryCloseNotification(hregNotifyPhone);   
				hregNotifyPhone = NULL;

				writeDebugInfo("CloseIncomingCallNotification");
			}	

			FreeLibrary(hInstAygShell);
		}
	}
}

void RegisterIncomingCallNotification(HWND wnd)
{
	HRESULT hr;	
	tRegistryNotifyWindow  lRegistryNotifyWindow = NULL;
	HINSTANCE hInstAygShell = NULL;

	hInstAygShell = LoadLibrary( _T( "AygShell.dll" ) );

	if(hInstAygShell) 
	{
		lRegistryNotifyWindow = GetProcAddress(hInstAygShell, _T( "RegistryNotifyWindow" ) );
		if(lRegistryNotifyWindow)
		{
			hr = lRegistryNotifyWindow( 
				SN_PHONEINCOMINGCALL_ROOT, //defined in snapi.h
				SN_PHONEINCOMINGCALL_PATH, 
				SN_PHONEINCOMINGCALL_VALUE,
				wnd,  //handle to our window to receive msg
				WM_USER+5123, //app defined message to send
				WM_USER+5124, //app defined value
				NULL,
				&hregNotifyPhone
				); //Don’t forget to close this

			if(hregNotifyPhone && hr == S_OK) writeDebugInfo("Created RegistryNotifyWindow notification");
		}

		//Load the AygShell when was necessary (the code freezes if hold the reference)
		FreeLibrary(hInstAygShell);
	}
}



/////////////////////////////////////////////////////////////////


extern const char *GetGameTitle();

#ifdef _WIN32_WCE
#define NO_CHANGEDISPLAYSETTINGS
#endif

#ifndef WS_MAXIMIZE
#define WS_MAXIMIZE		0
#endif

#ifndef SWP_NOCOPYBITS
#define SWP_NOCOPYBITS	0
#endif

FUNC1 GXOpenDisplay = NULL;
FUNC2 GXCloseDisplay = NULL;
FUNC3 GXBeginDraw = NULL;
FUNC2 GXEndDraw = NULL;
FUNC2 GXOpenInput = NULL;
FUNC2 GXCloseInput = NULL;
FUNC4 GXGetDisplayProperties = NULL;
FUNC5 GXGetDefaultKeys = NULL;
FUNC7 GXIsDisplayDRAMBuffer = NULL;

void *gxHandle = NULL;
int bGXOpenedDisplay = 0;
WORD *backBufer = NULL;
WORD* videoBuffer = NULL;

#ifndef _USE_POCKET_HAL_
GXKeyList defaultKeys;
#endif

#ifndef NO_CHANGEDISPLAYSETTINGS
#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
int SDL_nummodes[NUM_MODELISTS];
SDL_Rect **SDL_modelist[NUM_MODELISTS];
#endif


typedef BOOL (__stdcall *UnregisterFunc1Proc)( UINT, UINT );
HINSTANCE hCoreDll = NULL;
UnregisterFunc1Proc procUndergisterFunc = NULL;

int OpenInput()
{
#ifdef _USE_POCKET_HAL_
	return 1;
#else
	/*hCoreDll = LoadLibrary(_T("coredll.dll"));

	if(hCoreDll)
	{		
		procUndergisterFunc = (UnregisterFunc1Proc)GetProcAddress(hCoreDll, _T("UnregisterFunc1"));
		
		if(procUndergisterFunc)
		{		
			int i;
			for (i=0xc1; i<=0xcf; i++) 
			{
				procUndergisterFunc(MOD_WIN, i);
				RegisterHotKey(SDL_Window, i, MOD_WIN, i);
			}

			procUndergisterFunc(MOD_WIN, defaultKeys.vkStart);
			RegisterHotKey(SDL_Window, defaultKeys.vkStart, MOD_WIN, defaultKeys.vkStart);
		}
		else
		{
			FreeLibrary(hCoreDll);
			procUndergisterFunc = NULL;
			hCoreDll = NULL;
		}
	}*/

	/*int i;
	for (i=0xc1; i<=0xcf; i++) 
	{
		SHSetAppKeyWndAssoc(i, SDL_Window);
	}
	
	SHSetAppKeyWndAssoc(defaultKeys.vkStart, SDL_Window);*/
	

	

	return GXOpenInput();
#endif
}

void CloseInput()
{
#ifndef _USE_POCKET_HAL_
	/*if(hCoreDll)
	{
		int i;
		for (i=0xc1; i<=0xcf; i++) 
		{
			UnregisterHotKey(SDL_Window, i);
		}

		UnregisterHotKey(SDL_Window, defaultKeys.vkStart);

		FreeLibrary(hCoreDll);
	}*/


	GXCloseInput();
#endif
}


/* Initialization/Query functions */
static int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int GAPI_SetColors(_THIS, int firstcolor, int ncolors,
			 SDL_Color *colors);
static void GAPI_CheckGamma(_THIS);
void GAPI_SwapGamma(_THIS);
void GAPI_QuitGamma(_THIS);
int GAPI_SetGammaRamp(_THIS, Uint16 *ramp);
int GAPI_GetGammaRamp(_THIS, Uint16 *ramp);
static void GAPI_VideoQuit(_THIS);

/* Hardware surface functions */
static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface);
static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface);

/* Windows message handling functions */
static void GAPI_RealizePalette(_THIS);
static void GAPI_PaletteChanged(_THIS, HWND window);
static void GAPI_WinPAINT(_THIS, HDC hdc);





RECT sdlWindowSize;
int backBuferSize = 0;


void GetPokcetPCScreenDimensions(int *physicalWidth, int *physicalHeight)
{
	//The Pocket sized devices and smartphones have a retangular physical screen,
	//that have the width smaller than height.
	//
	//So, consider the small dimension always as width
	//to avoid fails if the system have rotated the screen
	

	int sysScreenW = GetSystemMetrics(SM_CXSCREEN),
		sysScreenH = GetSystemMetrics(SM_CYSCREEN);

	
	//Correction: use the system screen dimensions and show a error message to the user
	//if is not the game resolution
	//if(sysScreenW < sysScreenH)
	{
		//Normal screen
		*physicalWidth	= sysScreenW;
		*physicalHeight = sysScreenH;
	}
	/*else
	{
		//Probably, the system or other software
		//have rotated the screen
		*physicalWidth	= sysScreenH;
		*physicalHeight = sysScreenW;
	}*/
}

void GetTopBottomBorder(int *top, int *bottom)
{
	//Border:
	//Height 220: (Smartphone)
	//
	//	   Top: 20
	//  Bottom: 20

	//Height 320: (Pocket PC QVGA)
	//
	//	   Top: 26
	//  Bottom: 24

	//Height 640: (Pocket PC VGA)
	//
	//	   Top: 52
	//  Bottom: 52

	int screenH = GetSystemMetrics(SM_CYSCREEN);

	switch(screenH)
	{
	case 220:
		*top = 20;
		*bottom = 20;
		break;

	case 320:
		*top = 26;
		*bottom = 24;
		break;

	case 640:
		*top = 52;
		*bottom = 52;
		break;

	default:
		*top = 26;
		*bottom = 24;
		break;
	}
}

//
// Static variables for task bar subclassing
//
static HWND g_hWndTask = NULL;
static WNDPROC g_fnProcTask = NULL;

void Suspend(HWND wnd)
{
	//Don't use:
	//	PostMessage(SDL_Window, WM_ACTIVATE , MAKEWPARAM(WA_INACTIVE, 0), (LPARAM)msgWindow);
	//	SDL_PrivateAppActive(0, appstate);
	//
	//don't works with some messages

	
	SDL_Event event;
	memset(&event, 0, sizeof(event));
	event.type = SDL_ACTIVEEVENT;
	event.active.gain = 0;
	event.active.state = SDL_APPACTIVE | SDL_APPINPUTFOCUS;
	SDL_PushEvent(&event);
	
	if(wnd) SetForegroundWindow(wnd);

	/*
	//The iPhone a approach, just exit (http://code.game-editor.com/ticket/16)
	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));
	event.quit.type = SDL_QUIT;
	SDL_PushEvent(&event);*/
}

LRESULT TaskWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER && wParam == 4)
	{
		//Navigation bar text changed
		HWND newWindow = (HWND)lParam;

		if(newWindow != SDL_Window)
		{			
			TCHAR str[32];
			str[0] = 0;	
						
			if(IsWindow(newWindow) && (IsSmartphoneDevice() || GetWindowText(newWindow, str, 32) > 0)) 
			{
				//In the smartphone phone call alerts don't put text on the title bar
				//In Pocket PC must test valid window
				writeDebugInfo("WM_USER, 4");
				Suspend(newWindow);
			}
		}
	}
	else if(msg == WM_USER+7)
	{
		HWND msgWindow = (HWND)wParam;		

		if( msgWindow != SDL_Window && 
			((lParam & SHFS_SHOWTASKBAR) || (lParam & SHFS_SHOWSTARTICON) || (lParam & SHFS_SHOWSIPBUTTON)) 
		  )
		{
			writeDebugInfo("WM_USER+7");
			Suspend(msgWindow);
		}
	}	
	/*else if(msg == WM_WINDOWPOSCHANGED)
	{
		LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam;
		if(lpwp && lpwp->flags & SWP_SHOWWINDOW)
		{
			//Some application change the task bar visibility
			PostMessage(SDL_Window, WM_CANCELMODE , 0, 0);
			{
					FILE *arq = fopen("oi.txt", "a+");
					if(arq)
					{
						fprintf(arq, "3\n");
						fclose(arq);
					}
				}
		}
	}*/

	return CallWindowProc(g_fnProcTask, hWnd, msg, wParam, lParam);
}


//
// Hook into the task bar
//
int HookTaskBar()
{
	//Some messages don't go to foreground
	//Try see if change the task bar text


	//
	// Already hooked?
	//
	if(g_fnProcTask)
		return 0;

	g_hWndTask = FindWindow(_T("HHTaskBar"), NULL);
	if(!g_hWndTask)
	{
		//Try smartphone tray
		//Betaplayer source code (http://hantor.net/~adtl/doc_beta/db/d98/win32_8c-source.html)
		g_hWndTask = FindWindow(_T("Tray"), NULL);
	}


	if(g_hWndTask)
	{
		g_fnProcTask = (WNDPROC)GetWindowLong(g_hWndTask, GWL_WNDPROC);
		SetWindowLong(g_hWndTask, GWL_WNDPROC, (LONG)TaskWndProc);
	}
	
	return g_hWndTask != NULL;
}

//
// Free the task bar
//
int FreeTaskBar()
{
	//
	// Already freed?
	//
	if(!g_fnProcTask)
		return 0;
	
	SetWindowLong(g_hWndTask, GWL_WNDPROC, (LONG)g_fnProcTask);
	g_fnProcTask = NULL;

	return 1;
}


int OpenGAPI(int fullscreen)
{
	static int  lastFullscreen = -1;

	

	
	//Crash when click on Continue Game button, due to disp == NULL (GAPI closed)
	/*if(fullscreen == -1 && !bGXOpenedDisplay)
		return 0;*/

	if(fullscreen == -1 && bGXOpenedDisplay)
	{
		CloseGAPI();
	}

	RegisterIncomingCallNotification(SDL_Window);

#ifdef _USE_POCKET_HAL_
	if(fullscreen == -1)
	{
		fullscreen = lastFullscreen;
	}

#ifdef _USE_BACK_BUFFER_
	if(backBufer) 
	{
		free(backBufer);
		backBufer = NULL;
	}

	//Create back buffer		
	backBufer = (WORD *)malloc(halWidth*halHeight*2); //PocketHAL always use 16bit screen
	if(backBufer) memset(backBufer, 0, halWidth*halHeight*2);
#endif

	//Reopen
	halCreate(SDL_Window, halWidth, halHeight);

	//Reinit keys
	InitPocketPCKeys(VK_keymap);

	bGXOpenedDisplay = 1;

#else
	//Open display
	if(bGXOpenedDisplay)
	{
		GXCloseDisplay();
		CloseInput();
		bGXOpenedDisplay = 0;

		if(backBufer) 
		{
			free(backBufer);
			backBufer = NULL;
		}
	}

	if(fullscreen == -1)
	{
		fullscreen = lastFullscreen;
	}

	
	if (!GXOpenDisplay(SDL_Window, fullscreen) || !OpenInput())
	{		
		return 0;
	}

	//Double buffer?
	if(!GXIsDisplayDRAMBuffer())
	{
		//Create back buffer		
		backBufer = (WORD *)malloc(backBuferSize);
		if(backBufer) memset(backBufer, 0, backBuferSize);
	}
	bGXOpenedDisplay = 1;
#endif

	
	lastFullscreen = fullscreen;
	return 1;
}


void CloseGAPI()
{
	CloseIncomingCallNotification();	

	if(bGXOpenedDisplay)
	{
		
#ifdef _USE_POCKET_HAL_
		halClose();
		
#ifdef _USE_BACK_BUFFER_
		if(backBufer) 
		{
			free(backBufer);
			backBufer = NULL;
		}
#endif
		
#else
		
		GXCloseDisplay();
		CloseInput();			
		
		if(backBufer) 
		{
			free(backBufer);
			backBufer = NULL;
		}
#endif

		bGXOpenedDisplay = 0;
	}
	

	//Don't care about task bar changes
	FreeTaskBar();
}


int bShowTaskBar = 1;
Uint32 callShowTaskBar = 0, callHideTaskBar = 0;
HMENU menuBar = NULL;
BITMAPINFO binfo;
SDL_Surface *videoSurface = NULL;
HBITMAP srcBitmap = NULL;




void CreateBitMap()
{
	HDC hdcSrc, hdcDest;
	HGDIOBJ hOld;

	// get te hDC of the target window    
	hdcSrc = GetWindowDC(SDL_Window);

 
    // create a device context we can copy to
    hdcDest = CreateCompatibleDC(hdcSrc);

    // create a bitmap we can copy it to,
    // using GetDeviceCaps to get the width/height
    srcBitmap = CreateCompatibleBitmap(hdcSrc, videoSurface->w, videoSurface->h); 

    // select the bitmap object
    hOld = SelectObject(hdcDest, srcBitmap);

    // bitblt over
    BitBlt(hdcDest, 0, 0, videoSurface->w, videoSurface->h, hdcSrc, 0, 0, SRCCOPY);

    // restore selection
    SelectObject(hdcDest,hOld);

    // clean up 
    DeleteDC(hdcDest);
    ReleaseDC(SDL_Window, hdcSrc);
}

int GAPI_ShowTaskBar()
{
	callShowTaskBar++;
	if(callShowTaskBar <= 1 && !IsSmartphoneDevice()) return 0; //Avoid initial activation messages on Pocket PC

	if(!bShowTaskBar)
	{
		int topBorder, bottomBorder;
		RECT rc;

		//Take screen shot
		CreateBitMap();		

		//Need close the GAPI to give key control back to the system
		CloseGAPI(); 	

		
		//ShowWindow(SDL_Window, SW_HIDE); 
		SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
		bShowTaskBar = 1;

		GetTopBottomBorder(&topBorder, &bottomBorder);
		
		//Without this call, doesn't show the incoming call controls (finish tthe call, ...), but
		//with this call, doesn't show the incoming call menu
		//SetWindowPos(SDL_Window, HWND_NOTOPMOST, /*0, 0, 1, 1*/sdlWindowSize.left, sdlWindowSize.top + topBorder, sdlWindowSize.right, sdlWindowSize.bottom - (topBorder + bottomBorder), SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		//SetWindowLong(SDL_Window, GWL_STYLE, GetWindowLong(SDL_Window, GWL_STYLE) | WS_NONAVDONEBUTTON);

		

			
		SDL_WM_SetCaption(GetGameTitle(), NULL);

		SHDoneButton(SDL_Window, SHDB_HIDE);
		
		{
			//http://forum.soft32.com/pda/SHCreateMenuBar-resource-file-ftopict47431.html
			SHMENUBARINFO cbi;
			memset(&cbi, 0, sizeof(SHMENUBARINFO));		
			menuBar = NULL;
			
			cbi.cbSize = sizeof(SHMENUBARINFO); 
			cbi.hwndParent = SDL_Window; 
			cbi.hInstRes = SDL_Instance; 
			cbi.nToolBarId = IDR_MENUBAR;

			//cbi.dwFlags = /*SHCMBF_EMPTYBAR |*/ SHCMBF_HIDESIPBUTTON | SHCMBF_HMENU; //Don't use this with the Smartphone menu
			
			if(SHCreateMenuBar(&cbi))
			{
				menuBar = (HMENU)cbi.hwndMB;				
			}
		}

		//http://msdn.microsoft.com/en-us/library/aa453694.aspx
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, FALSE);
        MoveWindow(SDL_Window, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE); //Show the game menu, but not the incoming call menu		
		SetWindowPos(GetForegroundWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); //Show incoming call window 
		SetWindowPos(SDL_Window, HWND_NOTOPMOST, /*0, 0, 1, 1*/sdlWindowSize.left, sdlWindowSize.top + topBorder, sdlWindowSize.right, sdlWindowSize.bottom - (topBorder + bottomBorder), SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_SHOWWINDOW); //Allow to show the incomming call window

		
		if(menuBar) writeDebugInfo("GAPI_ShowTaskBar Menu Ok"); 
		else
		{
			char buf[64];
			sprintf(buf, "GAPI_ShowTaskBar No Menu, Error: %ld", GetLastError());
			writeDebugInfo(buf); 
		}


		return 1;
	}

	return 0;
}

int GAPI_HideTaskBar()
{
	callHideTaskBar++;
	if(!menuBar && callHideTaskBar <= 1) return 0; //Avoid initial activation messages

	if(bShowTaskBar)
	{
		//ShowWindow(SDL_Window, SW_SHOW); 
		SHFullScreen(SDL_Window, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);
		bShowTaskBar = 0;
		
		SetWindowPos(SDL_Window, HWND_TOPMOST, sdlWindowSize.left, sdlWindowSize.top, sdlWindowSize.right, sdlWindowSize.bottom, SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		OpenGAPI(-1);

		//Go to the game, monitor task bar
		HookTaskBar();
		
		//SetWindowLong(SDL_Window, GWL_STYLE, GetWindowLong(SDL_Window, GWL_STYLE) | ~WS_EX_CAPTIONOKBTN);


		if(menuBar) 
		{
			CommandBar_Destroy((HWND)menuBar);
			menuBar = NULL;
		}

		if(srcBitmap)
		{
			DeleteObject(srcBitmap);
			srcBitmap = NULL;
		}

		return 1;
	}

	return 0;
}

static void GAPI_NormalUpdate(_THIS, int numrects, SDL_Rect *rects)
{
#ifdef _USE_POCKET_HAL_
	
#else
	if(backBufer && !bShowTaskBar)
	{
		//int i;
		//__int64  *src, *dst;
		
		videoBuffer = GXBeginDraw();

		memcpy(videoBuffer + this->hidden->m_nBufferStartOffset, backBufer, 
			(this->hidden->m_nBufferEndOffset - this->hidden->m_nBufferStartOffset + 1) * 2);	

		/*src = (__int64  *)backBufer;
		dst = (__int64  *)(videoBuffer + this->hidden->m_nBufferStartOffset);

		for(i = 0; i < 19200; i++)
			*dst++ = *src++;*/
		

		GXEndDraw();
	}
#endif
}



/* Exported for the windows message loop only */
static void GAPI_WinPAINT(_THIS, HDC hdc)
{
	if(bShowTaskBar)
	{
		//Blank

		/*RECT rect;
		HBRUSH brush;

		rect.left = 0;
		rect.top = 0;
		rect.right = sdlWindowSize.right;
		rect.bottom = sdlWindowSize.bottom - (26 + 24);

		brush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);*/
		
		//Bitmap
		if(srcBitmap)
		{
			int topBorder, bottomBorder;
			HDC mdc = CreateCompatibleDC(hdc);			
			
			SelectObject(mdc, srcBitmap);

			GetTopBottomBorder(&topBorder, &bottomBorder);

			BitBlt(hdc, 0, 0, sdlWindowSize.right, sdlWindowSize.bottom - (topBorder + bottomBorder), mdc, 0, topBorder, SRCCOPY);

			DeleteDC(mdc);
		}

		//Refresh navigation bar
		SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
		SDL_WM_SetCaption(GetGameTitle(), NULL);		
	}
	/*else if(backBufer)
	{
		videoBuffer = GXBeginDraw();

		memcpy(videoBuffer + this->hidden->m_nBufferStartOffset, backBufer, 
			(this->hidden->m_nBufferEndOffset - this->hidden->m_nBufferStartOffset + 1) * 2);

		GXEndDraw();
	}*/
}

static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface)
{
	if(bShowTaskBar) return - 1;

#ifdef _USE_POCKET_HAL_
	
#ifdef _USE_BACK_BUFFER_
	surface->pixels = backBufer;
#else
	surface->pixels = halGetBuffer();
#endif

#else
	if(backBufer) 
	{
		//Use GAPI_NormalUpdate to update display
		surface->pixels = backBufer;
		//videoBuffer = GXBeginDraw();
	}
	else
	{
		surface->pixels = GXBeginDraw();
	}
#endif

	return(0);
}
static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	if(bShowTaskBar) return;

#ifdef _USE_POCKET_HAL_
	if(!(surface->flags & SDL_OPENGL))
	{
		//Only use PocketHAL draw if not have OpenGL
		halSwap(backBufer);
	}
#else
	if(!backBufer)
	{
		GXEndDraw();
	}
	/*else
	{
		memcpy(videoBuffer + this->hidden->m_nBufferStartOffset, backBufer, 
			(this->hidden->m_nBufferEndOffset - this->hidden->m_nBufferStartOffset + 1) * 2);

		GXEndDraw();
	}*/
#endif

	surface->pixels = NULL;
}

static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

/* GAPI driver bootstrap functions */

static int GAPI_Available(void)
{
#ifndef _USE_POCKET_HAL_
	if(!gxHandle)
	{
		gxHandle = LoadLibrary(TEXT("GX.DLL"));
		if(!gxHandle) return 0;
		
		GXOpenDisplay			= (void *)GetProcAddress(gxHandle, TEXT("?GXOpenDisplay@@YAHPAUHWND__@@K@Z"));
		GXCloseDisplay			= (void *)GetProcAddress(gxHandle, TEXT("?GXCloseDisplay@@YAHXZ"));
		GXBeginDraw				= (void *)GetProcAddress(gxHandle, TEXT("?GXBeginDraw@@YAPAXXZ"));
		GXEndDraw				= (void *)GetProcAddress(gxHandle, TEXT("?GXEndDraw@@YAHXZ"));
		GXOpenInput				= (void *)GetProcAddress(gxHandle, TEXT("?GXOpenInput@@YAHXZ"));
		GXCloseInput			= (void *)GetProcAddress(gxHandle, TEXT("?GXCloseInput@@YAHXZ"));
		GXGetDisplayProperties	= (void *)GetProcAddress(gxHandle, TEXT("?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ"));
		GXGetDefaultKeys		= (void *)GetProcAddress(gxHandle, TEXT("?GXGetDefaultKeys@@YA?AUGXKeyList@@H@Z"));
		GXIsDisplayDRAMBuffer	= (void *)GetProcAddress(gxHandle, TEXT("?GXIsDisplayDRAMBuffer@@YAHXZ"));
		
		if( !GXOpenDisplay || !GXCloseDisplay || !GXBeginDraw ||
			!GXEndDraw || !GXOpenInput || !GXCloseInput ||
			!GXGetDisplayProperties || !GXGetDefaultKeys ||
			!GXIsDisplayDRAMBuffer)
		{
			FreeLibrary(gxHandle);
			gxHandle = NULL;
			return 0;
		}
	}

	//Set default keys
	defaultKeys = GXGetDefaultKeys(GX_NORMALKEYS);
#endif

	return 1;
}

static void GAPI_DeleteDevice(SDL_VideoDevice *device)
{
	if ( device ) {
		if ( device->hidden ) {
			free(device->hidden);
		}
		if ( device->gl_data ) {
			free(device->gl_data);
		}
		free(device);
	}
}

static SDL_VideoDevice *GAPI_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)malloc(sizeof(SDL_VideoDevice));

	if ( device )
	{
		memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				malloc((sizeof *device->hidden));
#ifdef HAVE_OPENGL
		device->gl_data = (struct SDL_PrivateGLData *)
				malloc((sizeof *device->gl_data));
#endif
	}

	if ( (device == NULL) || (device->hidden == NULL) 
#ifdef HAVE_OPENGL
		|| (device->gl_data == NULL)
#endif
		) 
	{
		SDL_OutOfMemory();
		GAPI_DeleteDevice(device);
		return(NULL);
	}
	memset(device->hidden, 0, (sizeof *device->hidden));

#ifdef HAVE_OPENGL
	memset(device->gl_data, 0, (sizeof *device->gl_data));
#endif

	


	/* Set the function pointers */
	device->VideoInit = GAPI_VideoInit;
	device->ListModes = GAPI_ListModes;
	device->SetVideoMode = GAPI_SetVideoMode;
	device->UpdateMouse = WIN_UpdateMouse;
	device->SetColors = GAPI_SetColors;
	device->UpdateRects = GAPI_NormalUpdate;
	device->VideoQuit = GAPI_VideoQuit;
	device->AllocHWSurface = GAPI_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = GAPI_LockHWSurface;
	device->UnlockHWSurface = GAPI_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = GAPI_FreeHWSurface;
	device->SetGammaRamp = GAPI_SetGammaRamp;
	device->GetGammaRamp = GAPI_GetGammaRamp;
#ifdef HAVE_OPENGL
	device->GL_LoadLibrary = WIN_GL_LoadLibrary;
	device->GL_GetProcAddress = WIN_GL_GetProcAddress;
	device->GL_GetAttribute = WIN_GL_GetAttribute;
	device->GL_MakeCurrent = WIN_GL_MakeCurrent;
	device->GL_SwapBuffers = WIN_GL_SwapBuffers;
#endif
	device->SetCaption = WIN_SetWMCaption;
	device->SetIcon = WIN_SetWMIcon;
	device->IconifyWindow = WIN_IconifyWindow;
	device->GrabInput = WIN_GrabInput;
	device->GetWMInfo = WIN_GetWMInfo;
	device->FreeWMCursor = WIN_FreeWMCursor;
	device->CreateWMCursor = WIN_CreateWMCursor;
	device->ShowWMCursor = WIN_ShowWMCursor;
	device->WarpWMCursor = WIN_WarpWMCursor;
	device->CheckMouseMode = WIN_CheckMouseMode;
	device->InitOSKeymap = GAPI_InitOSKeymap;
	device->PumpEvents = GAPI_PumpEvents;

	/* Set up the windows message handling functions */
	WIN_RealizePalette = GAPI_RealizePalette;
	WIN_PaletteChanged = GAPI_PaletteChanged;
	WIN_WinPAINT = GAPI_WinPAINT;
	HandleMessage = GAPI_HandleMessage;

	device->freeVideoDevice = GAPI_DeleteDevice;

	/* We're finally ready */
	return device;
}

VideoBootStrap GAPI_bootstrap = {
	"gapi", "PocketPC 2002 GAPI",
	GAPI_Available, GAPI_CreateDevice
};

#ifndef NO_CHANGEDISPLAYSETTINGS

static int cmpmodes(const void *va, const void *vb)
{
    SDL_Rect *a = *(SDL_Rect **)va;
    SDL_Rect *b = *(SDL_Rect **)vb;
    if(a->w > b->w)
        return -1;
    return b->h - a->h;
}

static int GAPI_AddMode(_THIS, int bpp, int w, int h)
{
	SDL_Rect *mode;
	int i, index;
	int next_mode;

	/* Check to see if we already have this mode */
	if ( bpp < 8 ) {  /* Not supported */
		return(0);
	}
	index = ((bpp+7)/8)-1;
	for ( i=0; i<SDL_nummodes[index]; ++i ) {
		mode = SDL_modelist[index][i];
		if ( (mode->w == w) && (mode->h == h) ) {
			return(0);
		}
	}

	/* Set up the new video mode rectangle */
	mode = (SDL_Rect *)malloc(sizeof *mode);
	if ( mode == NULL ) {
		SDL_OutOfMemory();
		return(-1);
	}
	mode->x = 0;
	mode->y = 0;
	mode->w = w;
	mode->h = h;

	/* Allocate the new list of modes, and fill in the new mode */
	next_mode = SDL_nummodes[index];
	SDL_modelist[index] = (SDL_Rect **)
	       realloc(SDL_modelist[index], (1+next_mode+1)*sizeof(SDL_Rect *));
	if ( SDL_modelist[index] == NULL ) {
		SDL_OutOfMemory();
		SDL_nummodes[index] = 0;
		free(mode);
		return(-1);
	}
	SDL_modelist[index][next_mode] = mode;
	SDL_modelist[index][next_mode+1] = NULL;
	SDL_nummodes[index]++;

	return(0);
}

#endif /* !NO_CHANGEDISPLAYSETTINGS */


int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
#ifndef NO_CHANGEDISPLAYSETTINGS
	int i;
	DEVMODE settings;
#endif

	/* Create the window */
	if ( GAPI_CreateWindow(this) < 0 ) {
		return(-1);
	}


	/* Determine the screen depth */
	vformat->BitsPerPixel = 16;
	switch (vformat->BitsPerPixel) {
		case 15:
			vformat->Rmask = 0x00007c00;
			vformat->Gmask = 0x000003e0;
			vformat->Bmask = 0x0000001f;
			vformat->BitsPerPixel = 16;
			break;
		case 16:
			vformat->Rmask = 0x0000f800;
			vformat->Gmask = 0x000007e0;
			vformat->Bmask = 0x0000001f;
			break;
		case 24:
		case 32:
			/* GDI defined as 8-8-8 */
			vformat->Rmask = 0x00ff0000;
			vformat->Gmask = 0x0000ff00;
			vformat->Bmask = 0x000000ff;
			break;
		default:
			break;
	}

	/* See if gamma is supported on this screen */
	GAPI_CheckGamma(this);

#ifndef NO_CHANGEDISPLAYSETTINGS
	/* Query for the list of available video modes */
	for ( i=0; EnumDisplaySettings(NULL, i, &settings); ++i ) {
		GAPI_AddMode(this, settings.dmBitsPerPel,
			settings.dmPelsWidth, settings.dmPelsHeight);
	}
	/* Sort the mode lists */
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( SDL_nummodes[i] > 0 ) {
			qsort(SDL_modelist[i], SDL_nummodes[i], sizeof *SDL_modelist[i], cmpmodes);
		}
	}
#endif /* !NO_CHANGEDISPLAYSETTINGS */

	

	/* Fill in some window manager capabilities */
	this->info.wm_available = 1;

	/* We're done! */
	return(0);
}

/* We support any format at any dimension */
SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
#ifdef NO_CHANGEDISPLAYSETTINGS
	return((SDL_Rect **)-1);
#else
	if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		return(SDL_modelist[((format->BitsPerPixel+7)/8)-1]);
	} else {
		return((SDL_Rect **)-1);
	}
#endif
}




/* Various screen update functions available */


SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	SDL_Surface *video;
	Uint32 prev_flags;
	DWORD style;
	const DWORD directstyle =
			(WS_POPUP);
	const DWORD windowstyle = 
			(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	const DWORD resizestyle =
			(WS_THICKFRAME|WS_MAXIMIZEBOX);

	
	int x, y;
	BOOL was_visible;
	Uint32 Rmask, Gmask, Bmask;


#ifdef HAVE_OPENGL //maks
	/* Clean up any GL context that may be hanging around */
	if ( current->flags & SDL_OPENGL ) { //maks: close only in video quit to avoid texture lost when go to game mode
		WIN_GL_ShutDown(this);
	}
#endif	
	

#ifdef _USE_POCKET_HAL_

	halWidth = width;
	halHeight = height;

	//Don't use GAPI or PocketHAL with OpenGL
	//Hybrid will show a blank screen
	OpenGAPI(1);	//Can open PocketHAL in Intel 2700G

	//Go to the game, monitor task bar
	HookTaskBar();

	/*if( flags & SDL_FULLSCREEN ) 
	{
		//maks: native resolution only		
		//Causes "Error - not enough memory to execute the game"
		halGetDimensions(&width, &height);
	}*/

	/* Recalculate the bitmasks if necessary */
	if ( bpp == current->format->BitsPerPixel ) 
	{
		video = current;
	} 
	else 
	{
		bpp = 16;		
		
		/* 5-6-5 */
		Rmask = 0x0000f800;
		Gmask = 0x000007e0;
		Bmask = 0x0000001f;	
		
		
		video = SDL_CreateRGBSurface(SDL_SWSURFACE,
					0, 0, bpp, Rmask, Gmask, Bmask, 0);

		if ( video == NULL ) 
		{
			SDL_OutOfMemory();
			return(NULL);
		}
	}
	
#else

	int m_nXPitch, m_nYPitch;
	GXDisplayProperties m_dispProps = GXGetDisplayProperties();

	m_nXPitch = m_dispProps.cbxPitch >> 1;
	m_nYPitch = m_dispProps.cbyPitch >> 1;

	if (m_nXPitch < 0)
	{
		this->hidden->m_nBufferStartOffset = m_nXPitch * (m_dispProps.cxWidth - 1);
	}
	else
	{
		this->hidden->m_nBufferEndOffset = m_nXPitch * (m_dispProps.cxWidth - 1);
	}

	if (m_nYPitch < 0)
	{
		this->hidden->m_nBufferStartOffset += m_nYPitch * (m_dispProps.cyHeight - 1);
	}
	else
	{
		this->hidden->m_nBufferEndOffset += m_nYPitch * (m_dispProps.cyHeight - 1);
	}

	

	
	

	/* Recalculate the bitmasks if necessary */
	if ( bpp == current->format->BitsPerPixel ) 
	{
		video = current;
	} 
	else 
	{
		bpp = m_dispProps.cBPP;		
		
		if(m_dispProps.ffFormat | kfDirect565)
		{
			/* 5-6-5 */
			Rmask = 0x0000f800;
			Gmask = 0x000007e0;
			Bmask = 0x0000001f;
		}
		else if(m_dispProps.ffFormat | kfDirect555)
		{
			/* 5-5-5 */
			Rmask = 0x00007c00;
			Gmask = 0x000003e0;
			Bmask = 0x0000001f;
		}
		else if(m_dispProps.ffFormat | kfDirect888)
		{
			/* 8-8-8 */
			Rmask = 0x00ff0000;
			Gmask = 0x0000ff00;
			Bmask = 0x000000ff;
		}
			
		video = SDL_CreateRGBSurface(SDL_SWSURFACE,
					0, 0, bpp, Rmask, Gmask, Bmask, 0);

		if ( video == NULL ) 
		{
			SDL_OutOfMemory();
			return(NULL);
		}
	}

	if( flags & SDL_FULLSCREEN ) 
	{
		//maks: native resolution only
		width  = m_dispProps.cxWidth;
		height = m_dispProps.cyHeight;
	}
#endif //_USE_POCKET_HAL_


	/* See whether or not we should center the window */
	was_visible = IsWindowVisible(SDL_Window);

	/* Fill in part of the video surface */
	prev_flags = video->flags;
	video->flags = 0;	/* Clear flags */
	video->w = width;
	video->h = height;
	video->pitch = SDL_CalculatePitch(video);

	 /* Stuff to hide that $#!^%#$ WinCE taskbar in fullscreen... */
	if ( flags & SDL_FULLSCREEN ) 
	{
		if ( !(prev_flags & SDL_FULLSCREEN) )
		{
			SHFullScreen(SDL_Window, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);			
			bShowTaskBar = 0;
		}
		video->flags |= SDL_FULLSCREEN;
	} 
	else 
	{
		if ( prev_flags & SDL_FULLSCREEN ) 
		{
			SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
			bShowTaskBar = 1;
		}
	}



#ifndef NO_CHANGEDISPLAYSETTINGS
	/* Set fullscreen mode if appropriate */
	if ( (flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		DEVMODE settings;

		memset(&settings, 0, sizeof(DEVMODE));
		settings.dmSize = sizeof(DEVMODE);
		settings.dmBitsPerPel = video->format->BitsPerPixel;
		settings.dmPelsWidth = width;
		settings.dmPelsHeight = height;
		settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		if ( ChangeDisplaySettings(&settings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL ) {
			video->flags |= SDL_FULLSCREEN;
			SDL_fullscreen_mode = settings;
		}
	}
#endif /* !NO_CHANGEDISPLAYSETTINGS */

	
	

	style = GetWindowLong(SDL_Window, GWL_STYLE);
	style &= ~(resizestyle|WS_MAXIMIZE);

	if ( (video->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) 
	{
		style &= ~windowstyle;
		style |= directstyle;
	} 
	else 
	{
#ifndef NO_CHANGEDISPLAYSETTINGS
		if ( (prev_flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) 
		{
			ChangeDisplaySettings(NULL, 0);
		}
#endif
		if ( flags & SDL_NOFRAME ) 
		{
			style &= ~windowstyle;
			style |= directstyle;
			video->flags |= SDL_NOFRAME;
		} 
		else 
		{
			style &= ~directstyle;
			style |= windowstyle;

			if ( flags & SDL_RESIZABLE ) 
			{
				style |= resizestyle;
				video->flags |= SDL_RESIZABLE;
			}
		}
#if WS_MAXIMIZE
		if (IsZoomed(SDL_Window)) style |= WS_MAXIMIZE;
#endif
	}

	/* DJM: Don't piss of anyone who has setup his own window */
	if (!SDL_windowid)
		SetWindowLong(SDL_Window, GWL_STYLE, style);	
	

	/* Resize the window */
	if ( SDL_windowid == NULL ) 
	{
		
		HWND top;
		UINT swp_flags;
		//RECT bounds; //Aways full screen, don't need calculate the bounds

		int realW = width, realH = height, screenW, screenH;

		GetPokcetPCScreenDimensions(&screenW, &screenH);

		if(width > height && screenW < screenH)
		{
			//Rotate
			realW = height;
			realH = width;
		}

		SDL_resizing = 1;
		/*bounds.left = 0;
		bounds.top = 0;
		bounds.right = realW;
		bounds.bottom = realH;
		AdjustWindowRectEx(&bounds, GetWindowLong(SDL_Window, GWL_STYLE), FALSE, 0); //If need to use, consider the system screen rotation
		realW = bounds.right-bounds.left;
		realH = bounds.bottom-bounds.top;*/
		x = (screenW-realW)/2;
		y = (screenH-realH)/2;


		//if(x < 0 || y < 0) //maks: to avoid crash when run a 480x640 game in a 240x320 device
		if((realW != screenW && 2*realW != screenW) ||
		   (realH != screenH && 2*realH != screenH)) 
		{
			//maks: Avoid a game running in a different system resolution,
			//but can 240x320 in a 480x640 device
			CloseGAPI();
			return NULL;
		}

	   

		if ( y < 0 ) { /* Cover up title bar for more client area */
			y -= GetSystemMetrics(SM_CYCAPTION)/2;
		}
		swp_flags = (SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		if ( was_visible && !(flags & SDL_FULLSCREEN) ) {
			swp_flags |= SWP_NOMOVE;
		}
		if ( flags & SDL_FULLSCREEN ) {
			top = HWND_TOPMOST;
		} else {
			top = HWND_NOTOPMOST;
		}

		sdlWindowSize.left = x;
		sdlWindowSize.top = y;
		sdlWindowSize.right = realW;
		sdlWindowSize.bottom = realH;

		SetWindowPos(SDL_Window, top, x, y, realW, realH, swp_flags);
		SDL_resizing = 0;
		SetForegroundWindow(SDL_Window);
	}

	/* If not fullscreen, locking is possible, but it doesn't do what 
	   the caller really expects -- if the locked surface is written to,
	   the appropriate portion of the entire screen is modified, not 
	   the application window, as we would like.
	   Note that it is still possible to write directly to display
	   memory, but the application must respect the clip list of
	   the surface.  There might be some odd timing interactions
	   involving clip list updates and background refreshing as
	   Windows moves other windows across our window.
	   We currently don't support this, even though it might be a
	   good idea since BeOS has an implementation of BDirectWindow
	   that does the same thing.  This would be most useful for
	   applications that do complete screen updates every frame.
	    -- Fixme?
	*/


	if ( (flags & SDL_FULLSCREEN) != SDL_FULLSCREEN ) {
		/* Necessary if we're going from fullscreen to window */
		#if 0
		if ( video->pixels == NULL ) {
			video->pitch = (width*video->format->BytesPerPixel);
			/* Pitch needs to be QWORD (8-byte) aligned */
			video->pitch = (video->pitch + 7) & ~7;
			video->pixels = (void *)malloc(video->h*video->pitch);
			if ( video->pixels == NULL ) {
				if ( video != current ) {
					SDL_FreeSurface(video);
				}
				SDL_OutOfMemory();
				return(NULL);
			}
		}
		#endif



		
//#if 0 /* FIXME: enable this when SDL consistently reports lost surfaces */
		if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
			video->flags |= SDL_HWSURFACE;
		} else {
			video->flags |= SDL_SWSURFACE;
		}
/*#else
		video->flags |= SDL_SWSURFACE;
#endif*/
		if ( (flags & SDL_RESIZABLE) && !(flags & SDL_NOFRAME) ) {
			video->flags |= SDL_RESIZABLE;
		}
		if ( flags & SDL_NOFRAME ) {
			video->flags |= SDL_NOFRAME;
		}
	} else {
		/* Necessary if we're going from window to fullscreen */
		if ( video->pixels != NULL ) {
			free(video->pixels);
			video->pixels = NULL;
		}
		
		video->flags |= SDL_HWSURFACE;
	}


#ifndef _USE_POCKET_HAL_
	backBuferSize = (this->hidden->m_nBufferEndOffset - this->hidden->m_nBufferStartOffset + 1)*2;
	if(!OpenGAPI((video->flags & SDL_FULLSCREEN)?1:0))
	{
		if ( video != current ) 
		{
			SDL_FreeSurface(video);
		}
		SDL_SetError("Couldn't open GAPI");
		
		return NULL;
	}

	//Go to the game, monitor task bar
	HookTaskBar();

#endif //_USE_POCKET_HAL_

#ifdef HAVE_OPENGL
	/* Set up for OpenGL */
	if ( flags & SDL_OPENGL ) 
	{
		if ( WIN_GL_SetupWindow(this) >= 0 )
		{
			video->flags |= SDL_OPENGL;
		}
	}
#endif

	/* We're live! */
	videoSurface = video;
	return(video);
}






int GAPI_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	return 1;	
}

static void GAPI_CheckGamma(_THIS)
{
	
}

void GAPI_SwapGamma(_THIS)
{

}
void GAPI_QuitGamma(_THIS)
{

}

int GAPI_SetGammaRamp(_THIS, Uint16 *ramp)
{
	SDL_SetError("SDL compiled without gamma ramp support");
	return -1;
}

int GAPI_GetGammaRamp(_THIS, Uint16 *ramp)
{
	SDL_SetError("SDL compiled without gamma ramp support");
	return -1;
}

static void FlushMessageQueue()
{
	MSG  msg;
	while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
		if ( msg.message == WM_QUIT ) break;
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void GAPI_VideoQuit(_THIS)
{
	writeDebugInfo("GAPI_VideoQuit");
	/* Destroy the window and everything associated with it */
	if ( SDL_Window ) 
	{
		/* Delete the screen bitmap (also frees screen->pixels) */
		if ( this->screen ) 
		{
#ifdef HAVE_OPENGL //maks
			if ( this->screen->flags & SDL_OPENGL ) 
			{
				WIN_GL_ShutDown(this);
			}
#endif

			if ( this->screen->flags & SDL_FULLSCREEN ) 
			{
				SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
				bShowTaskBar = 1;
			}


#ifndef NO_CHANGEDISPLAYSETTINGS
			if ( this->screen->flags & SDL_FULLSCREEN ) {
				ChangeDisplaySettings(NULL, 0);
				ShowWindow(SDL_Window, SW_HIDE);
			}
#endif


			
			this->screen->pixels = NULL;
		}
		
		GAPI_QuitGamma(this);
		GAPI_DestroyWindow(this);
		FlushMessageQueue();

		SDL_Window = NULL;

		CloseGAPI();		
	}

	if(gxHandle)
	{
		FreeLibrary(gxHandle);
		gxHandle = NULL;
	}	

	//Make sure restore task bar
	SHFullScreen(GetForegroundWindow(), SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
}

/* Exported for the windows message loop only */
static void GAPI_FocusPalette(_THIS, int foreground)
{
	
}
static void GAPI_RealizePalette(_THIS)
{
	
}
static void GAPI_PaletteChanged(_THIS, HWND window)
{
	
}

