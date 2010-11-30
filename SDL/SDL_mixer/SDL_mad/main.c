/***************************************************************************
                          main.c  -  description
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
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "mad_decode.h"

#define BUFSIZE 8192

SDL_AudioSpec *obtained=NULL;
volatile int finished=1;
struct MAD_decoder *dec=NULL;

/* Prototype of our callback function */
void my_audio_callback(void *userdata, Uint8 *stream, int len);
int Init_Audio();
void Close_Audio();

int main(int argc, char *argv[])
{
  int n=-1;

  if(argc==1)
  {
    fprintf(stderr,"Syntax:  sdl_mad file1 file2 file3 ...\n\n");
    return(1);
  }
    
  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)<0)
  {
    fprintf(stderr,"Can't initialize audio!\n");
    return(1);
  }

  if(Init_Audio()<0)
  {
    SDL_Quit();
    return(1);
  }
  
  fprintf(stderr,"Initialized audio\n");

  for(n=1; n<argc; n++)
  {
    dec=MAD_CreateDecoder(argv[n],4096);
    
    if(dec==NULL)
    {
      fprintf(stderr,"Couldn't load %s\n",argv[n]);
      continue;
    }
    
    fprintf(stderr,"Loaded %s\n",argv[n]);
    finished=0; /* Start playing of music */
    SDL_PauseAudio(0);

    while(!finished)  SDL_Delay(10);

    fprintf(stderr,"Done playing %s\n",argv[n]);

    MAD_Free(dec);
    dec=NULL;
  }

  Close_Audio();

  SDL_Quit();

  return(0);
}

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
  if(finished||(dec==NULL))
  {
    SDL_PauseAudio(1);
    memset(stream,0,len);
  }
  else
  {
    int decoded=MAD_Decode(dec,stream,len,2);
    if(decoded<len)
    {
      memset(stream+decoded,0,len-decoded);
      finished=1;
      SDL_PauseAudio(1);
    }
  }
}

int Init_Audio()
{
  /* Allocate a desired SDL_AudioSpec */
  SDL_AudioSpec *desired = malloc(sizeof(SDL_AudioSpec));

  /* Allocate space for the obtained SDL_AudioSpec */
  obtained = malloc(sizeof(SDL_AudioSpec));

  /* 22050Hz - FM Radio quality */
  desired->freq=44100;

  /* 16-bit signed audio */
  desired->format=AUDIO_S16LSB;

  /* Mono */
  desired->channels=2;

  /* Large audio buffer reduces risk of dropouts but increases response time */
  desired->samples=8192;

  /* Our callback function */
  desired->callback=my_audio_callback;

  desired->userdata=NULL;

  /* Open the audio device */
  if ( SDL_OpenAudio(desired, obtained) < 0 ){
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
    free(obtained);
    obtained=NULL;
    free(desired);
    return(-1);
  }
  else
  {
    /* desired spec is no longer needed */
    free(desired);
    return(0);
  }
}

void Close_Audio()
{
  SDL_CloseAudio();
  free(obtained);
  obtained=NULL;
}
