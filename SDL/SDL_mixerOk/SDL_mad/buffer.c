/***************************************************************************
                          Buffer.c  -  A C-style buffer object
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
#include "buffer.h" 
#include <stdlib.h>
#include <string.h>
#include "SDL_rwops.h"

void Buffer_Clear(Buffer *buf)
{
  if(buf!=NULL) buf->pos=0;  
}

size_t Buffer_KeepOnly(Buffer *buf, size_t pos, size_t len)
{
  size_t lastpos;
  if(buf==NULL) return(-1);

  lastpos=pos+len;

  if(len<0)
  {
    pos+=len;
    len=-len;
  }

  if(lastpos>buf->pos) return(-1); /* out of bounds */
  else if(pos<0)       return(-1); /* out of bounds */

  memmove(buf->data,buf->data+pos,len);
  buf->pos=len;
  return(0);
}


size_t Buffer_ReadFromRW(Buffer *buf,SDL_RWops *rw)
{
  size_t space;
  size_t bytes_read;
  if(rw==NULL) return(-1);
  
  space=Buffer_Space(buf);
  if(space<=0) return(-1);

  bytes_read=SDL_RWread(rw,buf->data + buf->pos,1,space);
  if(bytes_read<=0) return(-1);

  else
  {
    buf->pos+=bytes_read;
    return(bytes_read);
  }
}

size_t Buffer_Space(Buffer *buf)
{
  if(buf==NULL) return(-1);
  else return(buf->size-buf->pos);
}

size_t Buffer_Write(Buffer *buf,void *data,size_t bytes)
{
  size_t space;
  if((data==NULL)||(bytes<=0)) return(-1);
  
  space=Buffer_Space(buf);
  
  if(space<=0)    return(-1);  
  if(space>bytes) space=bytes;

  memcpy(buf->data+buf->pos,data,space);
  buf->pos+=space;
  
  return(space);         
}

Buffer * Buffer_Create(size_t size)
{
  Buffer *buf=(Buffer *)malloc(sizeof(Buffer));
  if(buf==NULL) return(NULL);

  buf->data=malloc(size);
  if(buf->data==NULL)
  {
    free(buf);
    return(NULL);
  }  
  buf->size=size;
  buf->pos=0;
  return(buf);
}

void Buffer_Free(Buffer *buf)
{
  if(buf!=NULL)
  {
    if(buf->data!=NULL) free(buf->data);
    free(buf);    
  }  
}
