// EnableDisable.h: interface for the EnableDisable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ENABLEDISABLE_H__B5F7D8D0_BF6B_4E0A_9E17_386FD07C462E__INCLUDED_)
#define AFX_ENABLEDISABLE_H__B5F7D8D0_BF6B_4E0A_9E17_386FD07C462E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class EnableDisable : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	EnableDisable(Actor *actor, const gedString& actionName, bool bOnCollision);
	virtual ~EnableDisable();

private:
	ListPop *listActor, *listState;
	Actor *eventActor, *actionActor;
	gedString actionName;
};

#endif // !defined(AFX_ENABLEDISABLE_H__B5F7D8D0_BF6B_4E0A_9E17_386FD07C462E__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME