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

	GAPI by Makslane Araujo Rodrigues, makslane@hotmail.com
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gapievents_c.h,v 1.0 2003/07/29 11:23:08 slouken Exp $";
#endif

#include "SDL_lowvideo.h"

/* Variables and functions exported by SDL_dibevents.c to other parts 
   of the native video subsystem (SDL_dibvideo.c)
*/
extern LONG
 GAPI_HandleMessage(_THIS, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern int GAPI_CreateWindow(_THIS);
extern void GAPI_DestroyWindow(_THIS);

extern void GAPI_PumpEvents(_THIS);
extern void GAPI_InitOSKeymap(_THIS);
