/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


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
#ifdef __MACOSX__
		SDL_ClearError(); //SDL 1.3 beta bug 29.3.2010 AKR
#endif
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