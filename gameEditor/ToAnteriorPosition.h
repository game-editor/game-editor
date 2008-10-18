// ToAnteriorPosition.h: interface for the ToAnteriorPosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TOANTERIORPOSITION_H__515A045D_F2A3_4116_B1E1_CF0D3A4812A9__INCLUDED_)
#define AFX_TOANTERIORPOSITION_H__515A045D_F2A3_4116_B1E1_CF0D3A4812A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ToAnteriorPosition : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	ToAnteriorPosition(Actor *actor, bool bOnCollision);
	virtual ~ToAnteriorPosition();

private:
	ListPop *listActor;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_TOANTERIORPOSITION_H__515A045D_F2A3_4116_B1E1_CF0D3A4812A9__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME