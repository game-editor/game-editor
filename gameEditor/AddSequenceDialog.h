// AddSequenceDialog.h: interface for the AddSequenceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ADDSEQUENCEDIALOG_H__88B23DC7_08D6_4669_B868_11B12BD463EF__INCLUDED_)
#define AFX_ADDSEQUENCEDIALOG_H__88B23DC7_08D6_4669_B868_11B12BD463EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class ActorProperty;

class AddSequenceDialog : public Panel  
{
public:
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	AddSequenceDialog(ActorProperty *actorProperty, Actor *actor);
	virtual ~AddSequenceDialog();

private:
	Actor *actor;
	ActorProperty *actorProperty;
	ListPop *listAnimation;	
	EditBox *name, *sequence, *editFps;
};

#endif // !defined(AFX_ADDSEQUENCEDIALOG_H__88B23DC7_08D6_4669_B868_11B12BD463EF__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME