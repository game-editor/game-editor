// ChangeAnimation.h: interface for the ChangeAnimation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CHANGEANIMATION_H__DAD5B57C_1613_47A8_AB07_AAABAF4DD3F1__INCLUDED_)
#define AFX_CHANGEANIMATION_H__DAD5B57C_1613_47A8_AB07_AAABAF4DD3F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ChangeAnimation : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	ChangeAnimation(Actor *actor);
	virtual ~ChangeAnimation();

private:
	ListPop *listActor, *listAnimation, *listAnimationState;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_CHANGEANIMATION_H__DAD5B57C_1613_47A8_AB07_AAABAF4DD3F1__INCLUDED_)

#endif
