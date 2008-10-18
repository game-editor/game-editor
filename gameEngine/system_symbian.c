/*******************************************************************************
system_windows.c: Implementation of system.c for Symbian
Author: Odilon Nelson
Last modified: 02/05/03
Note: Do note compile this file directly. Compile system.c instead.
*******************************************************************************/

#include "SDL_mixer.h"

void Mix_FreeMusic(Mix_Music *music) {}
void Mix_FreeChunk(Mix_Chunk *chunk) {}
int Mix_VolumeChunk(Mix_Chunk *chunk, int volume) {return 0;}
int Mix_VolumeMusic(int volume) {return 0;}
int Mix_PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks) {return 0;}
int Mix_PlayMusic(Mix_Music *music, int loops) {return 0;}
int Mix_AllocateChannels(int numchans) {return 0;}
int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize) {return 0;}
void Mix_CloseAudio(void) {}
Mix_Music *Mix_LoadMUS(const char *file) {return NULL;}
Mix_Chunk *Mix_LoadWAV_RW(SDL_RWops *src, int freesrc) {return NULL;}

long ged_findfirst(const char *a, int b) {return GED_NOTFOUND;}
long ged_findnext(long a) {return GED_NOTFOUND;}
void ged_findclose(long a) {}
char *ged_filename(long a) {return "";} 
long ged_filesize(long handle) {return 0;}
long ged_writetime(long handle) {return 0;}
unsigned char ged_attrib(long a) {return 0;} 
char *ged_getcwd( char *buffer, int maxlen ) {return getcwd(buffer, maxlen);}

void openUrl(const char *url) {}
int isInternetConnected() {return 0;}
