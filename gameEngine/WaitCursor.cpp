// WaitCursor.cpp: implementation of the WaitCursor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "WaitCursor.h"
#include "GameControl.h"

#include "../gameEditor/Tutorial.h"




int WaitCursor::waitCursorCount = 0;
//stCursor WaitCursor::cursor("makswaitCursor.bmp", 1, 1, 10, 18, false);

WaitCursor::WaitCursor(bool bLoadCursor)
{
	if(waitCursorCount == 0 && !Tutorial::IsOff())
	{
		Tutorial::Get()->Pause(PAUSED_BY_WAIT);
	}

	waitCursorCount++;	

	if(bLoadCursor && !GameControl::Get()->getGameMode())
	{
		//EditorDirectory editDir;
		//DrawCursor(engine, &cursor, GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), true);
	
		if(!SDL_waitcursor)
		{
			SDL_waitcursor = SDL_CreateCursor(wait_cdata, wait_cmask,
					WAIT_CWIDTH, WAIT_CHEIGHT,
					WAIT_CHOTX, WAIT_CHOTY);
		}

		if(SDL_waitcursor) SDL_SetCursor(SDL_waitcursor);
	}
}

WaitCursor::~WaitCursor()
{
	waitCursorCount--;

	if(waitCursorCount == 0)
	{
		if(!Tutorial::IsOff()) 
		{
			Tutorial::Get()->Pause(PAUSED_BY_WAIT);
		}
		
		if(SDL_waitcursor) SDL_FreeCursor(SDL_waitcursor);
		SDL_waitcursor = NULL;
	}
}

stCursor *WaitCursor::getWaitCursor()
{
	//if(!waitCursorCount) return NULL;
	//else return &cursor;

	return NULL;
}

#endif //#ifndef STAND_ALONE_GAME