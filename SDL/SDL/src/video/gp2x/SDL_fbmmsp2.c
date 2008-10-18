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
 "@(#) $Id: SDL_fbmmsp2.c,v 0.1 2005/12/20 23:29:33 paeryn Exp $";
#endif

#include "SDL_types.h"
#include "SDL_video.h"
#include "SDL_blit.h"
#include "SDL_fbmmsp2.h"
#include "mmsp2_mmio.h"
#include "mmsp2_regs.h"

Uint16 volatile *mmsp2_regs;
Uint32 volatile *mmsp2_blit_regs;


/* Set video addr */
void MMSP2_SetScreen(Uint32 addr)
{
  mmsp2_regs[MLC_STL_OADRL] = addr & 0xffff;
  mmsp2_regs[MLC_STL_OADRH] = addr >> 16;
  addr += 640;
  mmsp2_regs[MLC_STL_EADRL] = addr & 0xffff;
  mmsp2_regs[MLC_STL_EADRH] = addr >> 16;
}

// dummy blit to force MMSP2's blitter to flush it's cache
static void dummy_blit(void)
{
  mmsp2_blit_regs[MESGDSTCTRL] = MESG_DSTBPP_16 | MESG_DSTENB;
  mmsp2_blit_regs[MESGDSTADDR] = 0x3101000;
  mmsp2_blit_regs[MESGDSTSTRIDE] = 0;
  mmsp2_blit_regs[MESGSRCCTRL] = 0;
  mmsp2_blit_regs[MESGPATCTRL] = 0;
  mmsp2_blit_regs[MESGSIZE] = (1 << MESG_HEIGHT) | 1;
  mmsp2_blit_regs[MESGCTRL] = (MESG_XDIR_POS) | (MESG_YDIR_POS) | 0xaa;
  do {} while (mmsp2_blit_regs[MESGSTATUS] & MESG_BUSY);
  asm volatile ("" ::: "memory");
  mmsp2_blit_regs[MESGSTATUS] = 1;
}

/* Wait for vertical retrace */
static void WaitVBL(_THIS)
{
  dummy_blit();
  // Now wait for vsync to start
  do {} while (!(mmsp2_regs[GPIOB_PINLVL] & GPIOB_VSYNC));
  // Wait for it to stop
  do {} while ((mmsp2_regs[GPIOB_PINLVL] & GPIOB_VSYNC));
}

/* Wait for blitter to be free */
static void WaitIdle(_THIS)
{
  mmsp2_waitidle();
}

/* Sets video mem colorkey and accelerated blit function */
static int SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key)
{
  surface->format->colorkey = key;
  return(0);
}

/* Sets per surface hardware alpha value */
static int SetHWAlpha(_THIS, SDL_Surface *surface, Uint8 value)
{
  surface->format->alpha = value;
  return(0);
}

// RectFill via hardware
static int FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 colour)
{
  Uint32 dstctrl;
  Uint32 dest;

  /* Don't blit to the display surface when switched away */
  if ( dst == this->screen ) {
    SDL_mutexP(hw_lock);
  }
  switch (dst->format->BytesPerPixel) {
  case 1:
    dstctrl = MESG_DSTBPP_8 | ((rect->x & 0x3) << 3);
    dest = mmsp2_addr(dst->pixels) + (rect->y * dst->pitch) + rect->x;
#ifdef FBACCEL_DEBUG
    fputs("8bit fill not yet implemented\n", stderr);
#endif
    break;
  case 2:
    dstctrl = MESG_DSTBPP_16 | (rect->x & 0x1) << 4;
    dest = mmsp2_addr(dst->pixels) + (rect->y * dst->pitch) + (rect->x << 1);
#ifdef FBACCEL_DEBUG
    fprintf(stderr, "dstctrl = 0x%X, dest = 0x%X, stride =%d, colour = %4X\n",
	    dstctrl, dest, dst->pitch, colour);
#endif
    mmsp2_blit_regs[MESGDSTCTRL] = dstctrl;
    mmsp2_blit_regs[MESGDSTADDR] = dest & 0xfffffffc;
    mmsp2_blit_regs[MESGDSTSTRIDE] = dst->pitch;
    mmsp2_blit_regs[MESGPATCTRL] = MESG_PATENB | MESG_PATBPP_1;
    mmsp2_blit_regs[MESGFORCOLOR] = colour;
    mmsp2_blit_regs[MESGBACKCOLOR] = colour;
    mmsp2_blit_regs[MESGSIZE] = (rect->h << MESG_HEIGHT) | rect->w;
    mmsp2_blit_regs[MESGCTRL] = (MESG_XDIR_POS) | (MESG_YDIR_POS) | 0xf0;

    // Wait for blitter to be free, then start it.
    wait_idle(this);
    asm volatile ("":::"memory");  // Imperative that next line comes last
    mmsp2_blit_regs[MESGSTATUS] = 0x01;
    break;
  default:
#ifdef FBACCEL_DEBUG
    fprintf(stderr, "HW_Fill attempted on surface with %d bpp\n",
	    dst->format->BitsPerPixel);
#endif
    break;
  }
  
  FB_AddBusySurface(dst);
  
  if ( dst == this->screen ) {
    SDL_mutexV(hw_lock);
  }

  return(0);
}

static int HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect,
                       SDL_Surface *dst, SDL_Rect *dstrect)
{
  SDL_VideoDevice *this = current_video;
  int w, h;
  int srcX, srcY;
  int srcstride, dststride;
  int dstX, dstY;
  Uint32 control;
  Uint32 srcstart, dststart;
  Uint32 srcctrl, dstctrl;

  /* Don't blit to the display surface when switched away */
  if ( dst == this->screen ) {
    SDL_mutexP(hw_lock);
  }
  
  /* Adjust for the current blit rectangles */
  srcX = srcrect->x;
  srcY = srcrect->y;
  dstX = dstrect->x;
  dstY = dstrect->y;
  w = srcrect->w;
  h = srcrect->h;
  srcstride = src->pitch;
  dststride = dst->pitch;

  // set ROP & colourkey
  control = 0xcc;
  if (src->flags & SDL_SRCCOLORKEY)
    control |= MESG_TRANSPEN | (src->format->colorkey << MESG_TRANSPCOLOR);
  /* Set up the blit direction (in control) flags */
  if ( srcX >= dstX )
    control |= MESG_XDIR_POS;
  else {
    srcX += (w - 1);
    dstX += (w - 1);
  }
  if ( srcY >= dstY )
    control |= MESG_YDIR_POS;
  else {
    srcY += (h - 1);
    dstY += (h - 1);
    srcstride = -srcstride;
    dststride = -dststride;
  }
  
  /* Set up the blit addresses, and control (32-bit alignment pixels) */
  if (src->format->BitsPerPixel == 8) {
    srcstart = mmsp2_addr(src->pixels) + (srcY * src->pitch) + srcX;
    srcctrl = MESG_SRCBPP_8 | (srcX & 0x3) << 3;
  } else {
    srcstart = mmsp2_addr(src->pixels) + (srcY * src->pitch) + (srcX << 1);
    srcctrl = MESG_SRCBPP_16 | (srcX & 0x1) << 4;
  }
  if (dst->format->BitsPerPixel == 8) {
    dststart = mmsp2_addr(dst->pixels) + (dstY * dst->pitch) + dstX;
    dstctrl = MESG_DSTBPP_8 | (dstX & 0x3) << 3;
  } else {
    dststart = mmsp2_addr(dst->pixels) + (dstY * dst->pitch) + (dstX << 1);
    dstctrl = MESG_DSTBPP_16 | (dstX & 0x1) << 4;
  }
#ifdef FBACCEL_DEBUG
  fprintf(stderr, "srcctrl = 0x%X, (%d,%d) x (%d,%d)\n",
	  srcctrl, srcX, srcY, w, h);
  fprintf(stderr, "dstctrl = 0x%X, (%d,%d) x (%d,%d)\n",
	  dstctrl, dstX, dstY, dstrect->w, dstrect->h);
  fprintf(stderr, "control = %X\n", control);
#endif
  mmsp2_blit_regs[MESGDSTCTRL] = dstctrl;
  mmsp2_blit_regs[MESGDSTADDR] = dststart & 0xfffffffc;
  mmsp2_blit_regs[MESGDSTSTRIDE] = dststride;
  mmsp2_blit_regs[MESGSRCCTRL] = MESG_SRCENB | MESG_INVIDEO | srcctrl;
  mmsp2_blit_regs[MESGSRCADDR] = srcstart & 0xfffffffc;
  mmsp2_blit_regs[MESGSRCSTRIDE] = srcstride;
  mmsp2_blit_regs[MESGPATCTRL] = 0;
  mmsp2_blit_regs[MESGSIZE] = (h << MESG_HEIGHT) | w;
  mmsp2_blit_regs[MESGCTRL] = control;
  
  // Wait for blitter to be free, then start it.
  wait_idle(this);
  asm volatile ("":::"memory");  // Imperative that next line comes last
  mmsp2_blit_regs[MESGSTATUS] = 0x01;
  
  FB_AddBusySurface(src);
  FB_AddBusySurface(dst);
  
  if ( dst == this->screen ) {
    SDL_mutexV(hw_lock);
  }
  return(0);
}

static int CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst)
{
  int accelerated;

  /* Set initial acceleration on */
  src->flags |= SDL_HWACCEL;
  
  /* Set the surface attributes */
  if ( (src->flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
    if ( ! this->info.blit_hw_A ) {
      src->flags &= ~SDL_HWACCEL;
    }
  }
  if ( (src->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
    if ( ! this->info.blit_hw_CC ) {
      src->flags &= ~SDL_HWACCEL;
    }
  }
  
  /* Check to see if final surface blit is accelerated */
  accelerated = !!(src->flags & SDL_HWACCEL);
  if (accelerated) {
    src->map->hw_blit = HWAccelBlit;
  }
  return(accelerated);
}

void FB_MMSP2Accel(_THIS, __u32 card)
{
  /* We have hardware accelerated surface functions */
  this->CheckHWBlit = CheckHWBlit;
  wait_vbl = WaitVBL;
  wait_idle = WaitIdle;
  
  /* The MMSP2 has an accelerated color fill */
  this->info.blit_fill = 1;
  this->FillHWRect = FillHWRect;
  
  /* The MMSP2 has accelerated normal and colorkey blits. */
  this->info.blit_hw = 1;
  this->info.blit_hw_CC = 1;
  this->SetHWColorKey = SetHWColorKey;

  /* The MMSP2 doesn't have an accelerated alpha blit */
  //  this->info.blit_hw_A = 1;
  //  this->SetHWAlpha = SetHWAlpha;
}
