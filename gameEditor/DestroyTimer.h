// DestroyTimer.h: interface for the DestroyTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DESTROYTIMER_H__D7156295_CDFF_42CC_A9E9_FCA0509C2C8E__INCLUDED_)
#define AFX_DESTROYTIMER_H__D7156295_CDFF_42CC_A9E9_FCA0509C2C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class DestroyTimer : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	DestroyTimer(Actor *actor);
	virtual ~DestroyTimer();

private:
	ListPop *listTimer;
	Actor *eventActor, *actionActor;

};

#endif // !defined(AFX_DESTROYTIMER_H__D7156295_CDFF_42CC_A9E9_FCA0509C2C8E__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME