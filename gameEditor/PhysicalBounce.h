// PhysicalBounce.h: interface for the PhysicalBounce class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PHYSICALBOUNCE_H__7EEFEA4E_AA1A_404B_B103_880E1A78FCCD__INCLUDED_)
#define AFX_PHYSICALBOUNCE_H__7EEFEA4E_AA1A_404B_B103_880E1A78FCCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class PhysicalBounce : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	PhysicalBounce(Actor *actor);
	virtual ~PhysicalBounce();

private:
	ListPop *listMoveType, *listMassType;
	EditBox *massEventActor, *massCollideActor;
	EditBox *eventVelocityMultiplier, *collideVelocityMultiplier;
	Text *textEventActor, *textCollideActor;

	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_PHYSICALBOUNCE_H__7EEFEA4E_AA1A_404B_B103_880E1A78FCCD__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME