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


// BaseAction.h: interface for the BaseAction class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_)
#define AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "../gameEngine/Action.h"

//class stAction;
class BaseAction : public Panel  
{
public:
	void SelectActionType(Actor *eventActor, stAction *action);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);

	BaseAction(const gedString spriteName, int x, int y, int width, int height,
		   Actor *parent = NULL, bool bInvertBevel = false, bool bDrag = true, bool bPanel = true);

	virtual ~BaseAction();

private:
	ListPop *listSelect;
	stAction *action, copyEditAction;
	Actor *eventActor;
};

#endif // !defined(AFX_BASEACTION_H__8FC213C8_ACD3_4E85_AD7A_CA39B063AFF7__INCLUDED_)

#endif //STAND_ALONE_GAME
