// ChangePath.h: interface for the ChangePath class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CHANGEPATH_H__7E23A715_1233_4639_9EE0_D48CE1437150__INCLUDED_)
#define AFX_CHANGEPATH_H__7E23A715_1233_4639_9EE0_D48CE1437150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ChangePath : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	ChangePath(Actor *actor, bool bOnCollision);
	virtual ~ChangePath();

private:
	ListPop *listActor, *listPath, *listAxis;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_CHANGEPATH_H__7E23A715_1233_4639_9EE0_D48CE1437150__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME