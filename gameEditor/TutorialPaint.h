/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// TutorialPaint.h: interface for the TutorialPaint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_)
#define AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class TutorialPaint : public Actor  
{
public:
	void Execute(SDL_Event &event);
	static void Hide();
	static void Show();
	static void Destroy();
	static TutorialPaint *Call();
	static TutorialPaint *Get() {return paint;};
	static bool InPaintMode() {return paint != NULL;}
	
	virtual ~TutorialPaint();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 mouseState);
	void OnMouseMove(int x, int y);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnKeyUp(SDLKey key);
	const stCursor *OnGetCursor();

protected:
	TutorialPaint();

	static TutorialPaint *paint;
	
	bool bPaint, bErase, bLine;
	bool bOnDestructor;
	int width, height;
	int xAnt, yAnt;

	Uint8 button;
};

#endif // !defined(AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_)

#endif //STAND_ALONE_GAME
