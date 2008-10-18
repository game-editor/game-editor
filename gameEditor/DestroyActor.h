// DestroyActor.h: interface for the DestroyActor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DESTROYACTOR_H__DD422447_101B_47D7_B8CE_AEC901975FA0__INCLUDED_)
#define AFX_DESTROYACTOR_H__DD422447_101B_47D7_B8CE_AEC901975FA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class DestroyActor : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	DestroyActor(Actor *actor, bool bOnCollision);
	virtual ~DestroyActor();

private:
	ListPop *listActor;
	Actor *eventActor, *actionActor;

};

#endif // !defined(AFX_DESTROYACTOR_H__DD422447_101B_47D7_B8CE_AEC901975FA0__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME