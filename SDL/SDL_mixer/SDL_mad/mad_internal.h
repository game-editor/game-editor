/***************************************************************************
                          mad_internal.h  -  description
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
#ifndef __MAD_INTERNAL_H__
#define __MAD_INTERNAL_H__

extern const char *PhaseNames[];

typedef enum MAD_phase_t
{
  MAD_PHASE_STREAM=0,
  MAD_PHASE_FRAME,
  MAD_PHASE_CONVERT,
  MAD_PHASE_DONE
} MAD_phase_t;

typedef struct MAD_decoder
{
  struct mad_stream   Stream;
  struct mad_frame    Frame;
  struct mad_synth    Synth;
  mad_timer_t  Timer;
  enum   MAD_phase_t  Phase;
  int                 FrameCount;

  SDL_RWops          *Source;
  int                 FreeRW;
  Buffer             *buf;
  int                 FrameSamplesUsed;

  int playing; //maks: play status
  int volume; //maks
} MAD_decoder;


extern MAD_phase_t MAD_PushStream(MAD_decoder *dec);
extern MAD_phase_t MAD_GetFrame(MAD_decoder *dec);
extern MAD_phase_t MAD_Convert(MAD_decoder *dec, Sint16 *buffer,
                                int *startpos, int size, int channels);

#endif/*__MAD_INTERNAL_H__*/
