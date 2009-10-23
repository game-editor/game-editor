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


// ActorEdit.h: interface for the ActorEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTOREDIT_H__0BF73824_6482_422D_B09D_E5F4DFB23CD6__INCLUDED_)
#define AFX_ACTOREDIT_H__0BF73824_6482_422D_B09D_E5F4DFB23CD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class ActorEdit : public Actor  
{
public:

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	//Overload the memory alocators to avoid crash when delete the class derived from Actor
	void *operator new(size_t size)	{return dlmalloc(size);}
	void operator delete(void * mem) {dlfree(mem);}
#endif

	const gedString *OnActorTip();

#ifndef STAND_ALONE_GAME
	static ActorEdit *getActivationEventActor() {return activationEventActor;};
	static bool inActivationEventMode() {return bActivationEventMode;}
#endif
	
	
	void OnCollision(Actor *collideActor);
	Uint32 OnTimer(SDL_TimerID timerID);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnKeyUp(SDLKey key);
	void OnAnimationFinish();
	void OnPathFinish();
	void OnAnimation();
	void OnMouseLeave(int x, int y);
	void OnMouseEnter(int x, int y);

	void OnMouseButtonUp(int x, int y, Uint8 button);
	const stCursor * OnGetCursor();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	ActorEdit(const gedString& spriteName, Actor *_parent = NULL, bool _bView = false, ActorType _type = SPRITE, int canvasWidth = 0, int canvasHeight = 0);
	virtual ~ActorEdit();

#ifndef STAND_ALONE_GAME
	static void DoActivationEvent(bool bActivate);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void ListMenu(int x, int y, Uint8 button);
#endif //#ifndef STAND_ALONE_GAME
		
private:

#ifndef STAND_ALONE_GAME
	static ActorEdit *activationEventActor;
	static bool bActivationEventMode;
	static MapString mapActivationEventActors;


	gedString			actorTip;
	ListPop *listMenu;
	static ListPop *listActivationEvents;	
#endif //#ifndef STAND_ALONE_GAME
};

#endif // !defined(AFX_ACTOREDIT_H__0BF73824_6482_422D_B09D_E5F4DFB23CD6__INCLUDED_)
