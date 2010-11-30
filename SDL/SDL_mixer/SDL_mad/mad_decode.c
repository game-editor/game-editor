/***************************************************************************
                             mad_decode.c
      A non-braindead implementation of an MP3 decoder using libmad,
      with SDL_RWops support.
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
#include <string.h>

#include "mad_decode.h"
#include "mad_internal.h"

/* Strings to correspond to MAD_phase_t values. */
const char *PhaseNames[]={"MAD_PHASE_STREAM","MAD_PHASE_FRAME",
                          "MAD_PHASE_CONVERT","MAD_PHASE_DONE",NULL};


/* Sets dec->Phase, and prints a warning message if
   setting dec->Phase to MAD_PHASE_DONE.            */
static void SetPhase(MAD_decoder *dec, MAD_phase_t phase);   

int MAD_Decode(MAD_decoder *dec, Uint8 *buffer, size_t size,int channels)
{
  int bytes=0;  /* Number of bytes of audio output decoded */

  /* Sanity checking */        
  if((dec==NULL)||(buffer==NULL)||(size<=0)) return(-1);

  while(bytes<size) /* Loop until 'size' bytes decoded, or error */  
  switch(dec->Phase)
  {
  case MAD_PHASE_STREAM:  /* Reading raw data from file */
    SetPhase(dec,MAD_PushStream(dec));
    break;
    
  case MAD_PHASE_FRAME:   /* Reading frames from raw data */
    SetPhase(dec,MAD_GetFrame(dec));
    break;
    
  case MAD_PHASE_CONVERT: /* Converting frames to 16-bit PCM data */
    SetPhase(dec,MAD_Convert(dec,(Sint16 *)buffer,&bytes,size,channels));
    break;
    
  case MAD_PHASE_DONE:  /* Out of data, or fatal error */
    return(-1);
    
  default:  /* Should't happen */
    /*fprintf(stderr,"Unknown phase %d\n",dec->Phase);
    fprintf(stderr,"Email me at tsm@accesscomm.ca and tell me how you did this\n");
    abort();*/
    break;
  }
  
  return(bytes);  /* Return the # of bytes decoded */
}

void MAD_Free(MAD_decoder *decoder)
{
 if(decoder==NULL) return;

 /* Deinitialize MAD */
 mad_synth_finish(&(decoder->Synth));
 mad_frame_finish(&(decoder->Frame));
 mad_stream_finish(&(decoder->Stream));
 
 if(decoder->buf!=NULL) Buffer_Free(decoder->buf);

 if((decoder->FreeRW)&&(decoder->Source!=NULL))
   SDL_FreeRW(decoder->Source);

 free(decoder);  
}

MAD_decoder *MAD_CreateDecoder(const char *fname, size_t bufsize)
{
 SDL_RWops *source=SDL_RWFromFile(fname,"rb");

 if(source==NULL)
 {
   return(NULL);
 }
 else
 {
   return(MAD_CreateDecoder_RW(source,bufsize,1));
 }
}

MAD_decoder *MAD_CreateDecoder_RW(SDL_RWops *rwIn, size_t buffersize, int freerw)
{
  MAD_decoder *decoder=NULL;

  if(rwIn==NULL) return(NULL);

  decoder=(MAD_decoder *)malloc(sizeof(MAD_decoder));
  if(decoder==NULL) return(NULL);

  memset(decoder,0,sizeof(MAD_decoder));

  decoder->buf=Buffer_Create(buffersize);
  if(decoder->buf==NULL)
  {
    free(decoder);
    return(NULL);
  }
  decoder->Phase=MAD_PHASE_STREAM;
  decoder->Source=rwIn;
  decoder->FreeRW=freerw;
  decoder->FrameCount=0;
  decoder->playing = 0; //maks:not playing
  decoder->volume = 128; //maks: max volume

  /* Initialize MAD */
  mad_stream_init(&(decoder->Stream));
  mad_frame_init(&(decoder->Frame));
  mad_synth_init(&(decoder->Synth));
  mad_timer_reset((&decoder->Timer));

  return(decoder);
}

void MAD_DecoderReset(MAD_decoder *decoder) //maks
{
  if(decoder==NULL) return;

  decoder->Phase=MAD_PHASE_STREAM;
  decoder->FrameCount=0;
  //Don't reset playing and volume

  /* Initialize MAD */
  mad_stream_init(&(decoder->Stream));
  mad_frame_init(&(decoder->Frame));
  mad_synth_init(&(decoder->Synth));
  mad_timer_reset((&decoder->Timer));

  if(decoder->Source) 
  {
	  SDL_RWseek(decoder->Source, 0, SEEK_SET);
  }
}

static void SetPhase(MAD_decoder *dec, MAD_phase_t phase)
{
  if(phase==MAD_PHASE_DONE)
  {
    fprintf(stderr,"Going from %s to %s\n",
      PhaseNames[dec->Phase],PhaseNames[phase]);
  }

  dec->Phase=phase;
}
