// MoveTo.h: interface for the MoveTo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_MOVETO_H__32813FA9_4082_45DD_AF64_427E88FDEE06__INCLUDED_)
#define AFX_MOVETO_H__32813FA9_4082_45DD_AF64_427E88FDEE06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class MoveTo : public BaseAction  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	MoveTo(Actor *actor, bool bOnCollision);
	virtual ~MoveTo();

private:
	ListPop *listActor, *listRelative, *listAvoid;
	Actor *eventActor, *actionActor;
	EditBox *posX, *posY, *velocity;
};

#endif // !defined(AFX_MOVETO_H__32813FA9_4082_45DD_AF64_427E88FDEE06__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME