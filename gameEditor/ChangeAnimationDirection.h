// ChangeAnimationDirection.h: interface for the ChangeAnimationDirection class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CHANGEANIMATIONDIRECTION_H__0270ED3A_35D5_43D6_9484_4C78B28E4B68__INCLUDED_)
#define AFX_CHANGEANIMATIONDIRECTION_H__0270ED3A_35D5_43D6_9484_4C78B28E4B68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ChangeAnimationDirection : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	ChangeAnimationDirection(Actor *actor, bool bOnCollision);
	virtual ~ChangeAnimationDirection();

private:
	ListPop *listActor, *listAnimationState;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_CHANGEANIMATIONDIRECTION_H__0270ED3A_35D5_43D6_9484_4C78B28E4B68__INCLUDED_)

#endif //STAND_ALONE_GAME