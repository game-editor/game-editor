/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

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
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gapivideo.c,v 1.5 2004/02/29 21:54:11 lemure Exp $";
#endif

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@linuxgames.com). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

/* Not yet in the mingw32 cross-compile headers */
#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN	4
#endif

#ifndef WS_THICKFRAME
#define WS_THICKFRAME 0
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
#include "SDL_dibevents_c.h"
#include "SDL_gapivideo.h"

#if defined(WIN32_PLATFORM_PSPC)
#include <aygshell.h>                      // Add Pocket PC includes
#pragma comment( lib, "aygshell" )         // Link Pocket PC library
#endif

#ifdef _WIN32_WCE
extern void DIB_ShowTaskBar(BOOL taskBarShown);
#endif


/* Initialization/Query functions */
static int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int GAPI_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void GAPI_VideoQuit(_THIS);


/* Hardware surface functions */
static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface);
static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface);

/* Windows message handling functions, will not be processed */
static void GAPI_RealizePalette(_THIS);
static void GAPI_PaletteChanged(_THIS, HWND window);
static void GAPI_WinPAINT(_THIS, HDC hdc);

static void GAPI_UpdateRects(_THIS, int numrects, SDL_Rect *rects); 
/*static void GAPI_UpdateRectsMono(_THIS, int numrects, SDL_Rect *rects);*/

static int GAPI_Available(void);
static SDL_VideoDevice *GAPI_CreateDevice(int devindex);

void GAPI_GrabHardwareKeys(BOOL grab);

VideoBootStrap WINGAPI_bootstrap = {
	"wingapi", "WinCE GAPI",
	GAPI_Available, GAPI_CreateDevice
};

/* 2003 SE GAPI emulation */

#define GETRAWFRAMEBUFFER   0x00020001

#define FORMAT_565 1
#define FORMAT_555 2
#define FORMAT_OTHER 3

static void* _OzoneFrameBuffer = NULL;
static struct GXDisplayProperties _OzoneDisplayProperties;
static char _OzoneAvailable = 0;

typedef struct _RawFrameBufferInfo
{
   WORD wFormat;
   WORD wBPP;
   VOID *pFramePointer;
   int  cxStride;
   int  cyStride;
   int  cxPixels;
   int  cyPixels;
} RawFrameBufferInfo;


struct GXDisplayProperties Ozone_GetDisplayProperties(void) {
	return _OzoneDisplayProperties;
}

int Ozone_OpenDisplay(HWND window, unsigned long flag) {
	return 1;
}

int Ozone_CloseDisplay(void) {
	return 1;
}

void* Ozone_BeginDraw(void) {
	return _OzoneFrameBuffer;
}

int Ozone_EndDraw(void) {
	return 1;
}

int Ozone_Suspend(void) {
	return 1;
}

int Ozone_Resume(void) {
	return 1;
}

static HINSTANCE checkOzone(tGXDisplayProperties *gxGetDisplayProperties, tGXOpenDisplay *gxOpenDisplay,
					  tGXVoidFunction *gxCloseDisplay, tGXBeginDraw *gxBeginDraw, 
					  tGXVoidFunction *gxEndDraw, tGXVoidFunction *gxSuspend, tGXVoidFunction *gxResume) {
#ifdef ARM

	int result;
	RawFrameBufferInfo frameBufferInfo;
	HDC hdc = GetDC(NULL);
	result = ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&frameBufferInfo);
	ReleaseDC(NULL, hdc);
	if (result < 0)
		return NULL;
	OutputDebugString(TEXT("Running on Ozone\r\n"));
	_OzoneAvailable = 1;

	// Initializing global parameters
	_OzoneFrameBuffer = frameBufferInfo.pFramePointer;
	_OzoneDisplayProperties.cBPP = frameBufferInfo.wBPP;
	_OzoneDisplayProperties.cbxPitch = frameBufferInfo.cxStride;
	_OzoneDisplayProperties.cbyPitch = frameBufferInfo.cyStride;
	_OzoneDisplayProperties.cxWidth = frameBufferInfo.cxPixels;
	_OzoneDisplayProperties.cyHeight = frameBufferInfo.cyPixels;
	if (frameBufferInfo.wFormat == FORMAT_565)
		_OzoneDisplayProperties.ffFormat = kfDirect565;
	else
	if (frameBufferInfo.wFormat == FORMAT_555)
		_OzoneDisplayProperties.ffFormat = kfDirect555;
	else {
		OutputDebugString(TEXT("Ozone unknown screen format"));
		return NULL;
	}

	if (gxGetDisplayProperties)
		*gxGetDisplayProperties = Ozone_GetDisplayProperties;
	if (gxOpenDisplay)
		*gxOpenDisplay = Ozone_OpenDisplay;
	if (gxCloseDisplay)
		*gxCloseDisplay = Ozone_CloseDisplay;
	if (gxBeginDraw)
		*gxBeginDraw = Ozone_BeginDraw;
	if (gxEndDraw)
		*gxEndDraw = Ozone_EndDraw;
	if (gxSuspend)
		*gxSuspend = Ozone_Suspend;
	if (gxResume)
		*gxResume = Ozone_Resume;

	return (HINSTANCE)1;

#else

	return NULL;

#endif
}

int getScreenWidth() {
	return (_OzoneFrameBuffer ? _OzoneDisplayProperties.cxWidth : GetSystemMetrics(SM_CXSCREEN));
}

int getScreenHeight() {
	return (_OzoneFrameBuffer ? _OzoneDisplayProperties.cyHeight : GetSystemMetrics(SM_CYSCREEN));
}


/* Check GAPI library */

#define IMPORT(Handle,Variable,Type,Function, Store) \
        Variable = GetProcAddress(Handle, TEXT(Function)); \
		if (!Variable) { \
			FreeLibrary(Handle); \
			return NULL; \
		} \
		if (Store) \
			*Store = (Type)Variable;

static HINSTANCE checkGAPI(tGXDisplayProperties *gxGetDisplayProperties, tGXOpenDisplay *gxOpenDisplay,
					  tGXVoidFunction *gxCloseDisplay, tGXBeginDraw *gxBeginDraw, 
					  tGXVoidFunction *gxEndDraw, tGXVoidFunction *gxSuspend, tGXVoidFunction *gxResume,
					  BOOL bypassOzone) {
	HMODULE gapiLibrary;
	FARPROC proc;
	HINSTANCE result;
	// FIXME paletted !
	tGXDisplayProperties temp_gxGetDisplayProperties;

	// Workaround for Windows Mobile 2003 SE
	_OzoneFrameBuffer = NULL;
	if (!bypassOzone) {
		result = checkOzone(gxGetDisplayProperties, gxOpenDisplay, gxCloseDisplay, gxBeginDraw, gxEndDraw, gxSuspend, gxResume);
		if (result)
			return result;
	}

	gapiLibrary = LoadLibrary(TEXT("gx.dll"));
	if (!gapiLibrary)
		return NULL;

	IMPORT(gapiLibrary, proc, tGXDisplayProperties, "?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ", gxGetDisplayProperties)
	IMPORT(gapiLibrary, proc, tGXOpenDisplay, "?GXOpenDisplay@@YAHPAUHWND__@@K@Z", gxOpenDisplay)
	IMPORT(gapiLibrary, proc, tGXVoidFunction, "?GXCloseDisplay@@YAHXZ", gxCloseDisplay)
	IMPORT(gapiLibrary, proc, tGXBeginDraw, "?GXBeginDraw@@YAPAXXZ", gxBeginDraw)
	IMPORT(gapiLibrary, proc, tGXVoidFunction, "?GXEndDraw@@YAHXZ", gxEndDraw)
	IMPORT(gapiLibrary, proc, tGXVoidFunction, "?GXSuspend@@YAHXZ", gxSuspend)
	IMPORT(gapiLibrary, proc, tGXVoidFunction, "?GXResume@@YAHXZ", gxResume)
	
	// FIXME paletted ! for the moment we just bail out	
	if (!gxGetDisplayProperties) {
		IMPORT(gapiLibrary, proc, tGXDisplayProperties, "?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ", &temp_gxGetDisplayProperties)
		if (temp_gxGetDisplayProperties().ffFormat & kfPalette) {
			FreeLibrary(gapiLibrary);
			return NULL;
		}
		FreeLibrary(gapiLibrary);		
		gapiLibrary = (HINSTANCE)1;
	}
	
	return gapiLibrary;
}


/* GAPI driver bootstrap functions */

static int GAPI_Available(void)
{
	/* Check if the GAPI library is available */

	if (!checkGAPI(NULL, NULL, NULL, NULL, NULL, NULL, NULL, FALSE)) {
		OutputDebugString(TEXT("GAPI driver not available\r\n"));
		return 0;
	}
	else {
		OutputDebugString(TEXT("GAPI driver available\r\n"));
		return 1;
	}
}

static void GAPI_DeleteDevice(SDL_VideoDevice *device)
{
	if (device && device->hidden && device->hidden->gapiFuncs.dynamicGXCloseDisplay)
		device->hidden->gapiFuncs.dynamicGXCloseDisplay();

	if (device && device->hidden)	
		free(device->hidden);
	if (device)
		free(device);

}

static SDL_VideoDevice *GAPI_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			free(device);
		}
		return(0);
	}
	memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set GAPI pointers */

	checkGAPI(&device->hidden->gapiFuncs.dynamicGXGetDisplayProperties, 
			  &device->hidden->gapiFuncs.dynamicGXOpenDisplay, 
			  &device->hidden->gapiFuncs.dynamicGXCloseDisplay, 
			  &device->hidden->gapiFuncs.dynamicGXBeginDraw, 
			  &device->hidden->gapiFuncs.dynamicGXEndDraw, 
			  &device->hidden->gapiFuncs.dynamicGXSuspend, 
			  &device->hidden->gapiFuncs.dynamicGXResume, 
			  FALSE);
	device->hidden->displayProps = device->hidden->gapiFuncs.dynamicGXGetDisplayProperties();

	/* Set the function pointers */

	device->VideoInit = GAPI_VideoInit;
	device->ListModes = GAPI_ListModes;
	device->SetVideoMode = GAPI_SetVideoMode;
	device->UpdateMouse = WIN_UpdateMouse;
	device->SetColors = GAPI_SetColors;
	device->UpdateRects = NULL;
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
	device->InitOSKeymap = DIB_InitOSKeymap;
	device->PumpEvents = DIB_PumpEvents;

	device->SetColors = GAPI_SetColors;

	/* Set up the windows message handling functions */
	WIN_RealizePalette = GAPI_RealizePalette;
	WIN_PaletteChanged = GAPI_PaletteChanged;
	WIN_WinPAINT = GAPI_WinPAINT;
	HandleMessage = DIB_HandleMessage;

	device->free = GAPI_DeleteDevice;
	
	
	/*
	device->VideoInit = GAPI_VideoInit;
	device->ListModes = GAPI_ListModes;
	device->SetVideoMode = GAPI_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = DUMMY_SetColors;
	device->UpdateRects = DUMMY_UpdateRects;
	device->VideoQuit = DUMMY_VideoQuit;
	device->AllocHWSurface = DUMMY_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = DUMMY_LockHWSurface;
	device->UnlockHWSurface = DUMMY_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = DUMMY_FreeHWSurface;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = DUMMY_InitOSKeymap;
	device->PumpEvents = DUMMY_PumpEvents;

	device->free = DUMMY_DeleteDevice;
	*/

	return device;
}


int GAPI_VideoInit(_THIS, SDL_PixelFormat *vformat)
{

	/* Create the window */
	if ( DIB_CreateWindow(this) < 0 ) {
		return(-1);
	}

	vformat->BitsPerPixel = (unsigned char)displayProperties.cBPP;	

	// Get color mask
	if (displayProperties.ffFormat & kfDirect565) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = 0x0000f800;
		vformat->Gmask = 0x000007e0;
		vformat->Bmask = 0x0000001f;
		videoMode = GAPI_DIRECT_565;
	}
	else
	if (displayProperties.ffFormat & kfDirect555) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = 0x00007c00;
		vformat->Gmask = 0x000003e0;
		vformat->Bmask = 0x0000001f;
		videoMode = GAPI_DIRECT_555;
	}
	else
	if ((displayProperties.ffFormat & kfDirect) && (displayProperties.cBPP <= 8)) {
		// We'll perform the conversion
		vformat->BitsPerPixel = 24;
		vformat->Rmask = 0x00ff0000;
		vformat->Gmask = 0x0000ff00;
		vformat->Bmask = 0x000000ff;
		if (displayProperties.ffFormat & kfDirectInverted)
			invert = (1 << displayProperties.cBPP) - 1;
		colorscale = displayProperties.cBPP < 8 ? 8 - displayProperties.cBPP : 0;
		videoMode = GAPI_MONO;
	}
	else
	if (displayProperties.ffFormat & kfPalette) {
		videoMode = GAPI_PALETTE;
	}

	/* Set UpdateRect callback */
	// FIXME
	/*
	if (videoMode != GAPI_MONO)
		this->UpdateRects = GAPI_UpdateRects;
	else
		this->UpdateRects = GAPI_UpdateRectsMono;
	*/

	this->UpdateRects = GAPI_UpdateRects;
	
	/* We're done! */
	return(0);
}

SDL_Rect **GAPI_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

SDL_Surface *GAPI_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	SDL_Surface *video;
	Uint32 Rmask, Gmask, Bmask;
	Uint32 prev_flags;
	DWORD style;
	const DWORD directstyle =
			(WS_POPUP);
	const DWORD windowstyle = 
			(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	const DWORD resizestyle =
			(WS_THICKFRAME|WS_MAXIMIZEBOX);
	int screenWidth, screenHeight;
	BOOL was_visible;

	/* We negociate legacy GAPI if we want a screen that fits in QVGA */
	if (_OzoneAvailable && _OzoneFrameBuffer && (width <= GetSystemMetrics(SM_CXSCREEN) || width <= GetSystemMetrics(SM_CYSCREEN)) &&
						     (height <= GetSystemMetrics(SM_CXSCREEN) || height <= GetSystemMetrics(SM_CYSCREEN))) {
			OutputDebugString(TEXT("Ozone workaround, switching back to GAPI\r\n"));
			ozoneHack = 0;
			checkGAPI(&this->hidden->gapiFuncs.dynamicGXGetDisplayProperties, 
				&this->hidden->gapiFuncs.dynamicGXOpenDisplay, 
				&this->hidden->gapiFuncs.dynamicGXCloseDisplay, 
				&this->hidden->gapiFuncs.dynamicGXBeginDraw, 
				&this->hidden->gapiFuncs.dynamicGXEndDraw, 
				&this->hidden->gapiFuncs.dynamicGXSuspend, 
				&this->hidden->gapiFuncs.dynamicGXResume, 
				TRUE);
			this->hidden->displayProps = this->hidden->gapiFuncs.dynamicGXGetDisplayProperties();
	}
	/* Otherwise we'll use the new system call */
	if (_OzoneAvailable && !_OzoneFrameBuffer && (width > GetSystemMetrics(SM_CXSCREEN) && width > GetSystemMetrics(SM_CYSCREEN)) && 
		                                         (height > GetSystemMetrics(SM_CXSCREEN) && height > GetSystemMetrics(SM_CYSCREEN))) {
			OutputDebugString(TEXT("Ozone workaround, switching back to true Ozone\r\n"));
			checkGAPI(&this->hidden->gapiFuncs.dynamicGXGetDisplayProperties, 
				&this->hidden->gapiFuncs.dynamicGXOpenDisplay, 
				&this->hidden->gapiFuncs.dynamicGXCloseDisplay, 
				&this->hidden->gapiFuncs.dynamicGXBeginDraw, 
				&this->hidden->gapiFuncs.dynamicGXEndDraw, 
				&this->hidden->gapiFuncs.dynamicGXSuspend, 
				&this->hidden->gapiFuncs.dynamicGXResume, 
				FALSE);
			this->hidden->displayProps = this->hidden->gapiFuncs.dynamicGXGetDisplayProperties();
	}
	/* Which will need a tiny input hack if the original code does not have the "Hi Res" aware ressource property set */
	ozoneHack = 0;
	if (_OzoneFrameBuffer && (GetSystemMetrics(SM_CXSCREEN) != _OzoneDisplayProperties.cxWidth ||
							  GetSystemMetrics(SM_CYSCREEN) != _OzoneDisplayProperties.cyHeight)) {
				OutputDebugString(TEXT("Running true Ozone with stylus hack\r\n"));
				ozoneHack = 1;
	}

	/* See whether or not we should center the window */
	was_visible = IsWindowVisible(SDL_Window);

	/* Recalculate bitmasks if necessary */
	if (bpp == current->format->BitsPerPixel) {
		video = current;
	}
	else {
		switch(bpp) {
			case 8:
				Rmask = 0;
				Gmask = 0;
				Bmask = 0;
				break;
			case 15:				
			case 16:
				/* Default is 565 unless the display is specifically 555 */
				if (displayProperties.ffFormat & kfDirect555) {
					Rmask = 0x00007c00;
					Gmask = 0x000003e0;
					Bmask = 0x0000001f;
				}
				else {
					Rmask = 0x0000f800;
					Gmask = 0x000007e0;
					Bmask = 0x0000001f;
				}
				break;
			case 24:
			case 32:
				Rmask = 0x00ff0000;
				Gmask = 0x0000ff00;
				Bmask = 0x000000ff;
				break;
			default:
				SDL_SetError("Unsupported Bits Per Pixel format requested");
				return NULL;
		}
		video = SDL_CreateRGBSurface(SDL_SWSURFACE,
					0, 0, bpp, Rmask, Gmask, Bmask, 0);
		if ( video == NULL ) {
			SDL_OutOfMemory();
			return(NULL);
		}
	}
	
	/* Fill in part of the video surface */
	prev_flags = video->flags;
	video->flags = 0;	/* Clear flags */
	video->w = width;
	video->h = height;
	video->pitch = SDL_CalculatePitch(video);
	mainSurfaceWidth = width;
	mainSurfaceHeight = height;	

//#ifdef WIN32_PLATFORM_PSPC
	/* Hide taskbar */	
	if ( flags & SDL_FULLSCREEN ) {
		if ( !(prev_flags & SDL_FULLSCREEN) ) {
			//SHFullScreen(SDL_Window, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
			//ShowWindow(FindWindow(TEXT("HHTaskBar"),NULL),SW_HIDE);
			DIB_ShowTaskBar(FALSE);
		}
		video->flags |= SDL_FULLSCREEN;
	} else {
		if ( prev_flags & SDL_FULLSCREEN ) {
			//SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON);
			//ShowWindow(FindWindow(TEXT("HHTaskBar"),NULL),SW_SHOWNORMAL);
			DIB_ShowTaskBar(TRUE);
		}
	}
//#endif

	/* Reset the palette and create a new one if necessary */	
	if (screenPal != NULL) {
		DeleteObject(screenPal);
		screenPal = NULL;
	}

	/* See if we need to create a translation palette */
	if (convertPalette != NULL) {
		free(convertPalette);
	}
	if (bpp == 8) {
		OutputDebugString(TEXT("creating palette\r\n"));
		convertPalette = (unsigned short*)malloc(256 * sizeof(unsigned short));
	}

	if (displayProperties.ffFormat & kfPalette) {
		/* Will only be able to support 256 colors in this mode */
		// FIXME
		//screenPal = GAPI_CreatePalette();
	}

	/* Set Window style */
	style = GetWindowLong(SDL_Window, GWL_STYLE);
	if ( (video->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN ) {
		style &= ~windowstyle;
		style |= directstyle;
	} else {
		if ( flags & SDL_NOFRAME ) {
			style &= ~windowstyle;
			style |= directstyle;
			video->flags |= SDL_NOFRAME;
		} else {
			style &= ~directstyle;
			style |= windowstyle;
			if ( flags & SDL_RESIZABLE ) {
				style |= resizestyle;
				video->flags |= SDL_RESIZABLE;
			}
		}
#if WS_MAXIMIZE
		if (IsZoomed(SDL_Window)) style |= WS_MAXIMIZE;
#endif
	}

	if (!SDL_windowid)
		SetWindowLong(SDL_Window, GWL_STYLE, style);

	/* Allocate bitmap */
	if (gapiBuffer) {
		free(gapiBuffer);
		gapiBuffer = NULL;
	}
	gapiBuffer = malloc(video->h * video->pitch);
	video->pixels = gapiBuffer;

	/* See if we will rotate */
	rotation = SDL_ROTATE_NONE;
	screenWidth = getScreenWidth();
	screenHeight = getScreenHeight();
	if ((flags & SDL_FULLSCREEN) && 
		(width > screenWidth && width <= screenHeight)
		) 
	{
			rotation = SDL_ROTATE_LEFT;
	}
	/* Compute the different drawing properties */
	switch(rotation) {
		case SDL_ROTATE_NONE:
			dstPixelstep = displayProperties.cbxPitch;
			dstLinestep = displayProperties.cbyPitch;
			startOffset = 0;
			break;
		case SDL_ROTATE_LEFT:
			dstPixelstep = -displayProperties.cbyPitch;
			dstLinestep = displayProperties.cbxPitch;
			startOffset = displayProperties.cbyPitch * (displayProperties.cyHeight - 1);
			break;
		case SDL_ROTATE_RIGHT:
			dstPixelstep = displayProperties.cbyPitch;
			dstLinestep = -displayProperties.cbxPitch;
			startOffset = displayProperties.cbxPitch * (displayProperties.cxWidth - 1);
			break;
	}
	/* Compute padding */
	padWidth = 0;
	padHeight = 0;
	if (rotation == SDL_ROTATE_NONE) {
		if (getScreenWidth() > width)
			padWidth = (getScreenWidth() - width) / 2;
		if (getScreenHeight() > height)
			padHeight = (getScreenHeight() - height) / 2;
	}
	else {
		if (getScreenWidth() > height)
			padWidth = (getScreenWidth() - height) / 2;
		if (getScreenHeight() > width)
			padHeight = (getScreenHeight() - width) / 2;
	}
	srcLinestep = video->pitch;
	srcPixelstep = (bpp == 15 ? 2 : bpp / 8);
	
	MoveWindow(SDL_Window, 0, 0, getScreenWidth(), getScreenHeight(), FALSE);
	ShowWindow(SDL_Window, SW_SHOW);

	/* Resize the window */
	//if ( SDL_windowid == NULL ) {
	if (0) {
		HWND top;
		UINT swp_flags;
		RECT bounds;
		int x,y;

		SDL_resizing = 1;
		bounds.left = 0;
		bounds.top = 0;
		bounds.right = video->w;
		bounds.bottom = video->h;
		AdjustWindowRectEx(&bounds, GetWindowLong(SDL_Window, GWL_STYLE), FALSE, 0);
		width = bounds.right-bounds.left;
		height = bounds.bottom-bounds.top;
		x = (getScreenWidth()-width)/2;
		y = (getScreenHeight()-height)/2;
		if ( y < 0 ) { /* Cover up title bar for more client area */
			y -= GetSystemMetrics(SM_CYCAPTION)/2;
		}
		swp_flags = (SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		if ( was_visible && !(flags & SDL_FULLSCREEN) ) {
			swp_flags |= SWP_NOMOVE;
		}
		if ( flags & SDL_FULLSCREEN ) {
			top = HWND_TOPMOST;
		} else {
			top = HWND_NOTOPMOST;
		}

		if (flags & SDL_FULLSCREEN) {
			SetWindowPos(SDL_Window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(SDL_Window, SW_SHOW);
		}
		else
			SetWindowPos(SDL_Window, top, x, y, width, height, swp_flags);
		
		SDL_resizing = 0;
		SetForegroundWindow(SDL_Window);
	}
	
	/* Open GAPI display */
	GXOpenDisplay(SDL_Window, (flags & SDL_FULLSCREEN ? GX_FULLSCREEN : 0));

	/* Grab hardware keys if necessary */
	if (flags & SDL_FULLSCREEN)
		GAPI_GrabHardwareKeys(TRUE);

	/* Blank screen */
	memset(GXBeginDraw(), 0, getScreenWidth() * getScreenHeight() * 2);
	GXEndDraw();

	/* We're done */
	return(video);
}

/* We don't actually allow hardware surfaces other than the main one */
static int GAPI_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void GAPI_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int GAPI_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void GAPI_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void updateLine(_THIS, unsigned char *srcPointer, unsigned char *destPointer, int width) {
	// FIXME, we assume everything is in the correct format, either 16 bits 565 or 555, or 8 bits
	int i;
	for (i=0; i<width; i++) {
		if (!convertPalette) {
			*(unsigned short*)destPointer = *(unsigned short*)srcPointer;
		}
		else {
			*(unsigned short*)destPointer = convertPalette[*srcPointer];
		}
			
		destPointer += dstPixelstep;
		srcPointer += srcPixelstep;
	}
}

static void GAPI_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	int i;
	unsigned char *screenBuffer;

	screenBuffer = GXBeginDraw();

	for (i=0; i<numrects; i++) {
		unsigned char *destPointer = screenBuffer + startOffset + ((rects[i].x + padWidth) * dstPixelstep) + ((rects[i].y + padHeight) * dstLinestep);
		unsigned char *srcPointer = (unsigned char*)gapiBuffer + (rects[i].x * srcPixelstep) + (rects[i].y * srcLinestep);
		int height = rects[i].h;

		while (height) {
			updateLine(this, srcPointer, destPointer, rects[i].w);
			destPointer += dstLinestep;
			srcPointer += srcLinestep;
			height--;
		}
	}

	GXEndDraw();
}

/*
#define ADVANCE_PARTIAL(address, step) \
	bitshift += displayProperties.cBPP;             \
	if(bitshift >= 8)                  \
	{                                  \
		bitshift = 0;                  \
		bitmask = (1<<displayProperties.cBPP)-1;    \
		address += step;               \
	}                                  \
	else                               \
		bitmask <<= displayProperties.cBPP;

#define ADVANCE_REV_PARTIAL(address, step)        \
	bitshift -= gxdp.cBPP;                        \
	if(bitshift < 0)                              \
	{                                             \
		bitshift = 8-displayProperties.cBPP;                   \
		bitmask = ((1<<displayProperties.cBPP)-1)<<bitshift;   \
		address += step;                          \
	}                                             \
	else                                          \
		bitmask >>= displayProperties.cBPP;


static void GAPI_UpdateRectsMono(_THIS, int numrects, SDL_Rect *rects)
{
	int i;
	unsigned char *screenBuffer;

	screenBuffer = GXBeginDraw();

	for (i=0; i<numrects; i++) {
		unsigned char *destPointer = screenBuffer + startOffset + (rects[i].x * dstPixelstep) + (rects[i].y * dstLinestep);
		unsigned char *srcPointer = (unsigned char*)gapiBuffer + (rects[i].x * srcPixelstep) + (rects[i].y * srcLinestep);
		unsigned char bitmask;
		int bitshift;

		int height = rects[i].h;
		while (height) {
			updateLine(_THIS, srcPointer, destPointer, rects[i].w);
			destPointer += dstLinestep;
			srcPointer += srcLinestep;
			height--;
		}
	}

	GXEndDraw();
}
*/

/* -------------------------------------------------------------------------------- */
// Global fixme for paletted mode !

#define COLORCONV565(r,g,b) (((r&0xf8)<<(11-3))|((g&0xfc)<<(5-2))|((b&0xf8)>>3))

#define COLORCONV555(r,g,b) (((r&0xf8)<<(10-3))|((g&0xf8)<<(5-2))|((b&0xf8)>>3))

int GAPI_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	/* Convert colors to appropriate 565 or 555 mapping */
	for (i=0; i<ncolors; i++) 
		convertPalette[firstcolor + i] = (videoMode == GAPI_DIRECT_565 ? 
			COLORCONV565(colors[i].r, colors[i].g, colors[i].b) :
			COLORCONV555(colors[i].r, colors[i].g, colors[i].b));
	return(1);
}

static void GAPI_RealizePalette(_THIS)
{
	OutputDebugString(TEXT("GAPI_RealizePalette NOT IMPLEMENTED !\r\n"));
}

static void GAPI_PaletteChanged(_THIS, HWND window)
{
	OutputDebugString(TEXT("GAPI_PaletteChanged NOT IMPLEMENTED !\r\n"));
}

/* Exported for the windows message loop only */
static void GAPI_WinPAINT(_THIS, HDC hdc)
{
	OutputDebugString(TEXT("GAPI_WinPAINT NOT IMPLEMENTED !\r\n"));
}


/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void GAPI_VideoQuit(_THIS)
{
	/* Destroy the window and everything associated with it */
	if ( SDL_Window ) {
		/* Delete the screen bitmap (also frees screen->pixels) */
		if ( this->screen ) {
//#ifdef WIN32_PLATFORM_PSPC
			if ( this->screen->flags & SDL_FULLSCREEN ) {
				/* Unhide taskbar, etc. */
				//SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON);
				//ShowWindow(FindWindow(TEXT("HHTaskBar"),NULL),SW_SHOWNORMAL);
				DIB_ShowTaskBar(TRUE);
				GAPI_GrabHardwareKeys(FALSE);
			}
//#endif

			if (this->screen->pixels != NULL)
			{
				free(this->screen->pixels);
				this->screen->pixels = NULL;
			}

			if (GXCloseDisplay)
				GXCloseDisplay();
		}
	}
}

void GAPI_GrabHardwareKeys(BOOL grab) {
	HINSTANCE GAPI_handle;
	tGXVoidFunction GAPIActionInput;

	GAPI_handle = LoadLibrary(TEXT("gx.dll"));
	if (!GAPI_handle)
		return;
	GAPIActionInput = (tGXVoidFunction)GetProcAddress(GAPI_handle, (grab ? TEXT("?GXOpenInput@@YAHXZ") : TEXT("?GXCloseInput@@YAHXZ")));
	if (GAPIActionInput) {
		GAPIActionInput();
	}
	FreeLibrary(GAPI_handle);
}
