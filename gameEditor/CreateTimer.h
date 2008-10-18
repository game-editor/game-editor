// CreateTimer.h: interface for the CreateTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CREATETIMER_H__B26D29F5_A804_484B_BA7C_84D583F61B3F__INCLUDED_)
#define AFX_CREATETIMER_H__B26D29F5_A804_484B_BA7C_84D583F61B3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class CreateTimer : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	CreateTimer(Actor *actor, bool bOnCollision);
	virtual ~CreateTimer();

	void AddTimerDefinition(const gedString& timerName, Uint32 interval, int count, int type, Uint32 minRandomInterval);

private:
	ListPop *listActor, *listTimer;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_CREATETIMER_H__B26D29F5_A804_484B_BA7C_84D583F61B3F__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME