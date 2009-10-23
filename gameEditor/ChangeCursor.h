/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

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
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


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