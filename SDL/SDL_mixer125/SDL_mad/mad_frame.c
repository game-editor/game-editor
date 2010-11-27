/***************************************************************************
                          mad_frame.c  -  description
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
#include "mad_decode.h"
#include "mad_internal.h"

MAD_phase_t MAD_GetFrame(MAD_decoder *dec)
{
  if(dec==NULL) return(-1);

  if(mad_frame_decode(&dec->Frame,&dec->Stream)) /* Error? */
  {
    if(MAD_RECOVERABLE(dec->Stream.error))
    {
      fprintf(stderr,"Error in stream, proceeding to next frame\n");
      return(MAD_PHASE_FRAME);
    }
    else if(dec->Stream.error==MAD_ERROR_BUFLEN)
    {
      return(MAD_PHASE_STREAM);
    }
    else
    {
      fprintf(stderr,"Irrecoverable error\n");
      return(MAD_PHASE_DONE);
    }
  }
  else /* Valid frame */
  {
    dec->FrameCount++;  /* Increment frame count */
    /* Keep track of timing */
    mad_timer_add(&dec->Timer,dec->Frame.header.duration);
    /* Convert the frame to sound data */
    mad_synth_frame(&dec->Synth,&dec->Frame);
    dec->FrameSamplesUsed=0;    /* Set to start of frame */
    
    return(MAD_PHASE_CONVERT);  /* Lets go next phase */
  }
}
