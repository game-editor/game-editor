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
