// DlgTimer.h: interface for the DlgTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGTIMER_H__B5A5EB33_36FD_4C6F_BA3E_D172496C9FFE__INCLUDED_)
#define AFX_DLGTIMER_H__B5A5EB33_36FD_4C6F_BA3E_D172496C9FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgTimer : public Panel  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	DlgTimer(Actor *actor);
	virtual ~DlgTimer();

private:
	ListPop *listTimer;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_DLGTIMER_H__B5A5EB33_36FD_4C6F_BA3E_D172496C9FFE__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME