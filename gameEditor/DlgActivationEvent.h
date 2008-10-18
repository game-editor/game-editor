// DlgActivationEvent.h: interface for the DlgActivationEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGACTIVATIONEVENT_H__CE8EBB26_A306_49D2_A030_B9AC94C177B0__INCLUDED_)
#define AFX_DLGACTIVATIONEVENT_H__CE8EBB26_A306_49D2_A030_B9AC94C177B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgActivationEvent : public Panel  
{
public:
	void UpdateEdition();

	void DoAction();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	DlgActivationEvent(Actor *actor);
	virtual ~DlgActivationEvent();

private:
	ListPop *listActor;
	Actor *eventActor, *actionActor;

};

#endif // !defined(AFX_DLGACTIVATIONEVENT_H__CE8EBB26_A306_49D2_A030_B9AC94C177B0__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME