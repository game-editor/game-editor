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


// DlgGeneralEvent.h: interface for the DlgGeneralEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_)
#define AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgGeneralEvent : public Panel  
{
public:
	void UpdateEdition();
	void DoAction();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	DlgGeneralEvent(Actor *actor, const gedString& eventName);
	virtual ~DlgGeneralEvent();

private:
	bool bUp;
	Actor *eventActor;
	gedString eventName;
};

#endif // !defined(AFX_DLGGENERALEVENT_H__614DEEA4_80B4_43FF_9834_829380069DA6__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME