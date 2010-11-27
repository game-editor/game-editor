/***************************************************************************
                          mad_convert.c  -  description
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

static unsigned short MadFixedToUshort(mad_fixed_t Fixed);

MAD_phase_t MAD_Convert(MAD_decoder *dec, Sint16 *buffer, int *startpos,
                        int size_in,int channels)
{
  int pos=(*startpos)/2;  
  int sample;
  int size=size_in/2;
  
  if((dec==NULL)||(buffer==NULL)||(startpos==NULL)) return(MAD_PHASE_DONE);
  if((channels<1)||(channels>2)) return(MAD_PHASE_DONE);
  if(size<=0)
  {
    fprintf(stderr,"MAD_Convert - 0 size given\n");    
    return(MAD_PHASE_DONE);
  }

  sample=dec->FrameSamplesUsed;

  if(channels==1)
  {
    if(MAD_NCHANNELS(&dec->Frame.header)==2)
    {
      while((pos<size)&&(sample<dec->Synth.pcm.length))
      {
        int val=MadFixedToUshort(dec->Synth.pcm.samples[0][sample]);
        val+=MadFixedToUshort(dec->Synth.pcm.samples[1][sample]);
        val=val>>1;
        buffer[pos]=val;
                
        pos++;
        sample++;
      }
    }
    else
    {
      while((pos<size)&&(sample<dec->Synth.pcm.length))
      {
        buffer[pos]=MadFixedToUshort(dec->Synth.pcm.samples[0][sample]);
        pos++;
        sample++;
      }
    }
  }
  else /* Stereo output */
  {
    if(MAD_NCHANNELS(&dec->Frame.header)==2)
    {
      while((pos<size)&&(sample<dec->Synth.pcm.length))
      {
        buffer[pos]=MadFixedToUshort(dec->Synth.pcm.samples[0][sample]);
        buffer[pos+1]=MadFixedToUshort(dec->Synth.pcm.samples[1][sample]);
        pos+=2;
        sample++;
      }
    }
    else
    {
      while((pos<size)&&(sample<dec->Synth.pcm.length))
      {
        buffer[pos]=MadFixedToUshort(dec->Synth.pcm.samples[0][sample]);
        buffer[pos+1]=buffer[pos];
        pos+=2;
        sample++;
      }
    }
  }

  
  (*startpos)=pos*2;

  if(sample==dec->Synth.pcm.length)
  {
    dec->FrameSamplesUsed=0;
    return(MAD_PHASE_FRAME);
  }
  else
  {
    dec->FrameSamplesUsed=sample;
    return(MAD_PHASE_CONVERT);
  }
}


/****************************************************************************
 * Converts a sample from mad's fixed point number format to an unsigned	*
 * short (16 bits).															*
 ****************************************************************************/
static unsigned short MadFixedToUshort(mad_fixed_t Fixed)
{
	/* A fixed point number is formed of the following bit pattern:
	 *
	 * SWWWFFFFFFFFFFFFFFFFFFFFFFFFFFFF
	 * MSB                          LSB
	 * S ==> Sign (0 is positive, 1 is negative)
	 * W ==> Whole part bits
	 * F ==> Fractional part bits
	 *
	 * This pattern contains MAD_F_FRACBITS fractional bits, one
	 * should alway use this macro when working on the bits of a fixed
	 * point number. It is not guaranteed to be constant over the
	 * different platforms supported by libmad.
	 *
	 * The unsigned short value is formed by the least significant
	 * whole part bit, followed by the 15 most significant fractional
	 * part bits. Warning: this is a quick and dirty way to compute
	 * the 16-bit number, madplay includes much better algorithms.
	 */
	Fixed=Fixed>>(MAD_F_FRACBITS-15);
	return((unsigned short)Fixed);
}
