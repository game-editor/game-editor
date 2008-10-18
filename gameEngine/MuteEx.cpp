// MuteEx.cpp: implementation of the MuteEx class.
//
//////////////////////////////////////////////////////////////////////

#include "MuteEx.h"
#include "SDL.h"

/* Use: 
SDL_CreateMutex();
SDL_DestroyMutex(SDL_mutex *mutex);
*/

MuteEx::MuteEx(SDL_mutex *mutex)
{
	SDL_mutexP(mutex);
	this->mutex = mutex;
}

MuteEx::~MuteEx()
{
	SDL_mutexV(mutex);
}
