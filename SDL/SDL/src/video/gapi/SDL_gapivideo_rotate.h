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
 "@(#) $Id: SDL_gapivideo.h,v 1.1 2004/02/02 23:25:35 lemure Exp $";
#endif

#ifndef _SDL_gapivideo_h
#define _SDL_gapivideo_h

#include <windows.h>

/* -------------------------------------------------------------------------------------------- */

/* From gx.h, since it's not really C compliant */

struct GXDisplayProperties {
    DWORD cxWidth;
    DWORD cyHeight;         // notice lack of 'th' in the word height.
    long cbxPitch;          // number of bytes to move right one x pixel - can be negative.
    long cbyPitch;          // number of bytes to move down one y pixel - can be negative.
    long cBPP;              // # of bits in each pixel
    DWORD ffFormat;         // format flags.
};

struct GXKeyList {
    short vkUp;             // key for up
    POINT ptUp;             // x,y position of key/button.  Not on screen but in screen coordinates.
    short vkDown;
    POINT ptDown;
    short vkLeft;
    POINT ptLeft;
    short vkRight;
    POINT ptRight;
    short vkA;
    POINT ptA;
    short vkB;
    POINT ptB;
    short vkC;
    POINT ptC;
    short vkStart;
    POINT ptStart;
};

#define kfLandscape	0x8			// Screen is rotated 270 degrees
#define kfPalette	0x10		// Pixel values are indexes into a palette
#define kfDirect	0x20		// Pixel values contain actual level information
#define kfDirect555	0x40		// 5 bits each for red, green and blue values in a pixel.
#define kfDirect565	0x80		// 5 red bits, 6 green bits and 5 blue bits per pixel
#define kfDirect888	0x100		// 8 bits each for red, green and blue values in a pixel.
#define kfDirect444	0x200		// 4 red, 4 green, 4 blue
#define kfDirectInverted 0x400

#define GX_FULLSCREEN	0x01		// for OpenDisplay()

/* -------------------------------------------------------------------------------------------- */

/* Rotation direction */
typedef enum {
	SDL_ROTATE_NONE,
	SDL_ROTATE_LEFT,
	SDL_ROTATE_RIGHT
} SDL_RotateAttr;

/* GAPI video mode */
typedef enum {
	GAPI_NONE = 0,
	GAPI_DIRECT_565,
	GAPI_DIRECT_555,
	GAPI_MONO,
	GAPI_PALETTE
} SDL_GAPIVideoMode;


/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this

/* GAPI functions definitions */

typedef struct GXDisplayProperties (*tGXDisplayProperties)(void);
typedef int (*tGXOpenDisplay)(HWND, unsigned long);
typedef void* (*tGXBeginDraw)(void);
typedef int (*tGXVoidFunction)(void);

/* Private display data */

struct GAPI_funcs {
	tGXDisplayProperties dynamicGXGetDisplayProperties;
	tGXOpenDisplay	dynamicGXOpenDisplay;
	tGXVoidFunction	dynamicGXCloseDisplay;
	tGXBeginDraw	dynamicGXBeginDraw;
	tGXVoidFunction	dynamicGXEndDraw;
	tGXVoidFunction	dynamicGXSuspend;
	tGXVoidFunction	dynamicGXResume;
};

struct GAPI_properties {
	unsigned char invert;
	int colorscale;
	int dstPixelstep;
	int dstLinestep;
	int startOffset;
	SDL_GAPIVideoMode videoMode;
};

#define MAX_CLR         0x100

struct palette_properties {
	unsigned char *palRed;
	unsigned char *palGreen;
	unsigned char *palBlue;
	unsigned short *pal;
};


struct SDL_PrivateVideoData {
	/* --- <Hack> --- begin with DIB private structure to allow DIB events code sharing */
	HBITMAP screen_bmp;
    HPALETTE screen_pal;
	void *work_pixels; /* if the display needs to be rotated, memory allocated by the API */
	void *rotation_pixels; /* if the display needs to be rotated, memory allocated by the code */
	SDL_RotateAttr rotation;
	char ozoneHack; /* force stylus translation if running without Hi Res flag */

#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
    int SDL_nummodes[NUM_MODELISTS];
    SDL_Rect **SDL_modelist[NUM_MODELISTS];

	/* --- </Hack> --- */

    int w, h;
    void *gapiBuffer;
	HPALETTE screenPal;
	struct GAPI_funcs gapiFuncs;
	struct GAPI_properties gapiProperties;
	struct GXDisplayProperties displayProps;
	int srcLinestep;
	int srcPixelstep;

	int padWidth;
	int padHeight;

	unsigned short *convertPalette;
};

#define gapiBuffer (this->hidden->gapiBuffer)
#define mainSurfaceWidth (this->hidden->w)
#define mainSurfaceHeight (this->hidden->h)
#define rotation (this->hidden->rotation)
#define ozoneHack (this->hidden->ozoneHack)
#define displayProperties (this->hidden->displayProps)
#define screenPal (this->hidden->screenPal)
#define GXGetDisplayProperties (this->hidden->gapiFuncs.dynamicGXGetDisplayProperties)
#define GXOpenDisplay (this->hidden->gapiFuncs.dynamicGXOpenDisplay)
#define GXCloseDisplay (this->hidden->gapiFuncs.dynamicGXCloseDisplay)
#define GXBeginDraw (this->hidden->gapiFuncs.dynamicGXBeginDraw)
#define GXEndDraw (this->hidden->gapiFuncs.dynamicGXEndDraw)
#define GXSuspend (this->hidden->gapiFuncs.dynamicGXSuspend)
#define GXResume (this->hidden->gapiFuncs.dynamicGXResume)
#define invert (this->hidden->gapiProperties.invert)
#define colorscale (this->hidden->gapiProperties.colorscale)
#define videoMode (this->hidden->gapiProperties.videoMode)
#define srcPixelstep (this->hidden->srcPixelstep)
#define srcLinestep (this->hidden->srcLinestep)
#define dstPixelstep (this->hidden->gapiProperties.dstPixelstep)
#define dstLinestep (this->hidden->gapiProperties.dstLinestep)
#define startOffset (this->hidden->gapiProperties.startOffset)
#define padWidth (this->hidden->padWidth)
#define padHeight (this->hidden->padHeight)
#define convertPalette (this->hidden->convertPalette)

#endif /* _SDL_gapivideo_h */
