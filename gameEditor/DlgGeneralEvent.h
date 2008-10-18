// DlgGeneralEvent.h: interface for the DlgGeneralEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_)
#define AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgGeneralEvent : public Panel  
{
public:
	void UpdateEdition();
	void DoAction();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	DlgGeneralEvent(Actor *actor, const gedString& eventName);
	virtual ~DlgGeneralEvent();

private:
	bool bUp;
	Actor *eventActor;
	gedString eventName;
};

#endif // !defined(AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME