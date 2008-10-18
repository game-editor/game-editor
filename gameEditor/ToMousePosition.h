// ToMousePosition.h: interface for the ToMousePosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TOMOUSEPOSITION_H__BC9E2F9D_1F63_47EB_BAAD_D47B58D97642__INCLUDED_)
#define AFX_TOMOUSEPOSITION_H__BC9E2F9D_1F63_47EB_BAAD_D47B58D97642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ToMousePosition : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	ToMousePosition(Actor *actor, bool bOnCollision);
	virtual ~ToMousePosition();

private:
	ListPop *listActor, *listAxis;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_TOMOUSEPOSITION_H__BC9E2F9D_1F63_47EB_BAAD_D47B58D97642__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME