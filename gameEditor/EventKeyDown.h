// EventKeyDown.h: interface for the EventKeyDown class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EVENTKEYDOWN_H__40EF40B6_1DD6_4559_A580_30F7D0843171__INCLUDED_)
#define AFX_EVENTKEYDOWN_H__40EF40B6_1DD6_4559_A580_30F7D0843171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "Action.h"

class Text;

class DlgEventKeyDown : public Panel  
{
public:
	void UpdateEdition();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	bool DoAction();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnButton(Button *button, int buttonId);
	DlgEventKeyDown(Actor *actor, bool bUp = false);
	virtual ~DlgEventKeyDown();

private:
	void Clear();
	void UpdateKeyText();

	bool bUp;
	Text *textKey;
	Actor *eventActor;
	SDLKey mode;
	unsigned short keySequence[MAX_KEY_SEQUENCE]; //Maximum MAX_KEY_SEQUENCE keys
	int nKeys;
	ListPop *listRepeat, *listMode;
	Panel *textPanel;
};

#endif // !defined(AFX_EVENTKEYDOWN_H__40EF40B6_1DD6_4559_A580_30F7D0843171__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME