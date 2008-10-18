// ChangeCursor.h: interface for the ChangeCursor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CHANGECURSOR_H__2C40863F_A96E_404C_9706_4D7C59832D7D__INCLUDED_)
#define AFX_CHANGECURSOR_H__2C40863F_A96E_404C_9706_4D7C59832D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"

class ChangeCursor : public BaseAction  
{
public:
	void UpdateEdition();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	ChangeCursor(Actor *actor);
	virtual ~ChangeCursor();

private:
	Actor *eventActor, *actionActor;
	ListDirectory *path;
	EditBox *hframes, *vframes, *hotx, *hoty;
	ListPop *listActor;
};

#endif // !defined(AFX_CHANGECURSOR_H__2C40863F_A96E_404C_9706_4D7C59832D7D__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME