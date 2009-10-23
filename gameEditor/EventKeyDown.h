/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


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