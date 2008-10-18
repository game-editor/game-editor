// EventControl.h: interface for the EventControl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EVENTCONTROL_H__A4BA7BEA_4010_4B73_85FD_3C357FA03850__INCLUDED_)
#define AFX_EVENTCONTROL_H__A4BA7BEA_4010_4B73_85FD_3C357FA03850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class EventControl : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	EventControl(Actor *actor, bool bOnCollision, bool bEventEnable);
	virtual ~EventControl();

private:
	ListPop *listActor, *listEvent;
	Actor *eventActor, *actionActor;
	bool bEventEnable;
};

#endif // !defined(AFX_EVENTCONTROL_H__A4BA7BEA_4010_4B73_85FD_3C357FA03850__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME