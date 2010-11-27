/***************************************************************************
                          buffer.h  -  description
                             -------------------
    begin                : Thu Jun 19 2003
    copyright            : (C) 2003 by Tyler Montbriand
    email                : tsm@accesscomm.ca
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>
#include "SDL_types.h"
#include "SDL_rwops.h"
#include "begin_code.h"

typedef struct Buffer
{
  char  *data;
  size_t size;
  size_t pos;
} Buffer;


DECLSPEC Buffer *SDLCALL Buffer_Create(size_t);
DECLSPEC size_t  SDLCALL Buffer_Write(Buffer *,void *,size_t);
DECLSPEC void    SDLCALL Buffer_Clear(Buffer *);
DECLSPEC size_t  SDLCALL Buffer_KeepOnly(Buffer *, size_t pos, size_t len);
DECLSPEC size_t  SDLCALL Buffer_Space(Buffer *);
DECLSPEC void    SDLCALL Buffer_Free(Buffer *);
DECLSPEC size_t  SDLCALL Buffer_ReadFromRW(Buffer *buf,SDL_RWops *rw);

#include "close_code.h"

#endif/*__BUFFER_H__*/
