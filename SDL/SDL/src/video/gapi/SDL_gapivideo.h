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
 "@(#) $Id: SDL_gapivideo.h,v 1.0 2003/07/29 20:31:34 slouken Exp $";
#endif

#ifndef _SDL_gapivideo_h
#define _SDL_gapivideo_h

#define _USE_POCKET_HAL_

#include "modGX.h" //Use modGX to avoid error in GXGetDisplayProperties and GXGetDefaultKeys declarations


/* Private display data */
struct SDL_PrivateVideoData 
{
    int m_nBufferStartOffset;
    int m_nBufferEndOffset;		
};

#ifdef _USE_POCKET_HAL_
extern void InitPocketPCKeys(SDLKey *keymap);
extern SDLKey VK_keymap[SDLK_LAST];
#else
extern GXKeyList defaultKeys;
#endif

//Frog code
// Known Pocket PC models
/*typedef enum _Model
{
	MODEL_NOT_DETECTED = 0,
    MODEL_UNKNOWN,
	
    // Pocket PCs
    MODEL_CASIO_E115,
    MODEL_CASIO_E125,
    MODEL_CASIO_EM500,
    MODEL_JORNADA_540,
    MODEL_JORNADA_545,
    MODEL_JORNADA_568,
    MODEL_IPAQ_3600,
    MODEL_IPAQ_3800,
    MODEL_IPAQ_3900,
    MODEL_SYMBOL_PPT_2700,
	
    // Handheld PCs
    MODEL_JORNADA_680,
    MODEL_JORNADA_720
} Model;

extern Model PocketPC_GetModel();*/

#endif /* _SDL_dibvideo_h */
