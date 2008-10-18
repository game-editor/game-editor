// DlgMouseButton.h: interface for the DlgMouseButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGMOUSEBUTTON_H__55F0B50C_7F07_4F49_99ED_85AE00D649F0__INCLUDED_)
#define AFX_DLGMOUSEBUTTON_H__55F0B50C_7F07_4F49_99ED_85AE00D649F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgMouseButton : public Panel  
{
public:
	void UpdateEdition();

	bool DoAction();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	DlgMouseButton(Actor *actor, bool bUp = false);
	virtual ~DlgMouseButton();

private:
	bool bUp;
	Text *textButton;
	Actor *eventActor;
	Uint8 button;
	ListPop *listDrag;
};

#endif // !defined(AFX_DLGMOUSEBUTTON_H__55F0B50C_7F07_4F49_99ED_85AE00D649F0__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME