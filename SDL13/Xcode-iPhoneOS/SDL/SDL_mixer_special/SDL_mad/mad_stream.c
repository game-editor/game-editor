/***************************************************************************
                          mad_stream.c  -  description
                             -------------------
    begin                : Fri Jun 20 2003
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
#include <stdlib.h>
#include "mad_decode.h"
#include "mad_internal.h"

static int MAD_GetInputData(MAD_decoder *decoder);

MAD_phase_t MAD_PushStream(MAD_decoder *dec)
{
  if(dec==NULL) return(MAD_PHASE_DONE);

  /* Stream needs more data */
  if(dec->Stream.buffer==NULL || dec->Stream.error==MAD_ERROR_BUFLEN)
  {

    if(dec->Stream.next_frame==NULL)  /* All-new data */
    {
      Buffer_Clear(dec->buf);
            
      if(MAD_GetInputData(dec)<0)
        if(MAD_GetInputData(dec)<0)
        {
          fprintf(stderr,"MAD_PushStream - Input error\n");
          return(MAD_PHASE_DONE);
        }  
    }
    else /* Little bit left in buffer */
    {
      int Remaining=dec->Stream.bufend-dec->Stream.next_frame;
      if(Buffer_KeepOnly(dec->buf,dec->buf->pos-Remaining,Remaining)<0)
      {
        abort();
      }

      if(MAD_GetInputData(dec)<0)
        if(MAD_GetInputData(dec)<0)
        {
          fprintf(stderr,"MAD_PushStream - Input error\n");          
          return(MAD_PHASE_DONE);
        }
    }
    
    mad_stream_buffer(&dec->Stream,dec->buf->data,dec->buf->pos);    
  }

  return(MAD_PHASE_FRAME);
}

static int MAD_GetInputData(MAD_decoder *decoder)
{
  if(decoder==NULL)
    return(-1);
  else
    return(Buffer_ReadFromRW(decoder->buf,decoder->Source));
}
