// ChangeParent.h: interface for the ChangeParent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CHANGEPARENT_H__C9B91222_D49E_42B1_9057_E5839DE9BBD8__INCLUDED_)
#define AFX_CHANGEPARENT_H__C9B91222_D49E_42B1_9057_E5839DE9BBD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ChangeParent  : public BaseAction 
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	ChangeParent(Actor *actor, bool bOnCollision);
	virtual ~ChangeParent();

private:
	ListPop *listActor, *listParent;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_CHANGEPARENT_H__C9B91222_D49E_42B1_9057_E5839DE9BBD8__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME