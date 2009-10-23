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


// BaseAction.cpp: implementation of the BaseAction class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "BaseAction.h"
#include "GameControl.h"
#include "ConditionalAction.h"
#include "WaitForFrame.h"
#include "ActorProperty.h"
#include "ExpressionEditor.h"


BaseAction::BaseAction(const gedString spriteName, int x, int y, int width, int height, Actor *parent, bool bInvertBevel, bool bDrag, bool bPanel)
	: Panel(spriteName, x, y, width, height, parent, bInvertBevel, bDrag, bPanel)
{
	action = NULL;
	eventActor = NULL;
	listSelect = NULL;

	if(Action::getEditAction())
	{
		memcpy(&copyEditAction, Action::getEditAction(), sizeof(stAction));
	}
}

BaseAction::~BaseAction()
{
	if(!ConditionalAction::HasConditionalDialog() &&

		//Solve duplicate code when edit an script and call a Action in
		//list completation
		!ExpressionEditor::getExpressionEditor()) 
	{
		Action::clearEditAction();
	}
}

bool BaseAction::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(list == listSelect)
	{
		switch(index)
		{
		case 0: //Immediate action
			{
				if(action) action->executionType = IMMEDIATE_EXECUTION;
				delete this;
			}
			break;
		case 1: //Wait for frame action
			{
				new WaitForFrame(eventActor, action, this);				
			}
			break;
		}

		return true;
	}

	return false;
}

void BaseAction::SelectActionType(Actor *eventActor, stAction *action)
{
	if(ConditionalAction::HasConditionalDialog())
	{
		delete this;
		return;
	}

	this->eventActor = eventActor;
	this->action = action;

	if(!listSelect)
	{
		listSelect = AddListPop(0, 0, 1, 1);
		listSelect->AddText("Immediate action");
		listSelect->AddText("Wait for frame action");
		//listSelect->DisableHideOnClickOutList();
	}
	
	listSelect->OnMouseButtonDown(GameControl::Get()->getMouseX(), 
		GameControl::Get()->getMouseY(), SDL_BUTTON_LEFT);
}

void BaseAction::OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	if( (messageId == LISTPOPSELECT_HIDED && listSelect == (ListPop *)message) ||
		messageId == WAITFORFRAME_CLOSED )
	{
		//User cancelation
		//Remove the action if is not edit action

		if(action && action->executionType == NO_ACTION_MODE && eventActor && eventActor->getAction())
		{
			eventActor->getAction()->RemoveAction(action);
			delete action;

			ActorProperty::SetEditEvents();

			action = NULL;
		}
		else
		{
			//Restore the editAction

			if(action && copyEditAction.executionType != NO_ACTION_MODE)
			{
				//Solve the bug "modify the action after the user modify, ok, cancel immediate, close"
				memcpy(action, &copyEditAction, sizeof(stAction));
			}

			//Solve duplicate code when cancel the immediate mode twice
			Action::setEditAction(action);
		}

		eventActor = NULL;
	}
}

#endif //STAND_ALONE_GAME