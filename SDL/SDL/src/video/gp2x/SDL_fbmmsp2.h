/*
	SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

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
 "@(#) $Id: SDL_fbmmsp2.h,v 0.1 2005/12/20 23:29:33 paeryn Exp $";
#endif

/* MMSP2 hardware acceleration for the SDL framebuffer console driver */

#include "SDL_fbvideo.h"

/* base for mmsp io registers */
extern Uint16 volatile *mmsp2_regs;
extern Uint32 volatile *mmsp2_blit_regs;

/* Set up the driver for MMSP2 acceleration */
extern void FB_MMSP2Accel(_THIS, __u32 card);
extern void MMSP2_SetScreen(Uint32 addr);
