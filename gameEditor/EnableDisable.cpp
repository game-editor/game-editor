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


// EnableDisable.cpp: implementation of the EnableDisable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "EnableDisable.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_VISIBILITY			"\
Use this action to show or hide the selected actor"

#define TIP_VISIBILITY_ACTOR			"\
Select the actor that will have its visibility state changed\n"

#define TIP_VISIBILITY_STATE			"\
You can select:\n\
\n\
   Enable: show the selected actor\n\
   Disable: hide the selected actor\n\
   Don't draw, but allow events: use for speed up collision map actors"



#define TIP_COLLISION_STATE			"\
Use this action to enable or disable collisions on selected actor"

#define TIP_COLLISION_STATE_ACTOR			"\
Select the actor that will have its collision state changed\n"

#define TIP_COLLISION_STATE_STATE			"\
Select the state"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_STATE,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	270
#define HEIGHT	105

EnableDisable::EnableDisable(Actor *actor, const gedString& actionName, bool bOnCollision)
	: BaseAction("Enable Disable", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	eventActor = actionActor = actor;
	this->actionName = actionName;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(actionName, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); 
	text = AddText("    State: ", 10, listActor->Down());
	listState = AddListPop(text->Right() + 2, text->Top(), 184, 0, LS_STATE);
	
	
	//Close
	y = DrawHLine(listState->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	GameControl::Get()->PopulateActors(listActor);
	
	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);
	listActor->SetItem(S_EVENT_ACTOR);
	
	listState->AddText("Enable");
	listState->AddText("Disable");
	if(actionName == VISIBILITY_STATE) listState->AddText("Don't draw, but allow events");

	if(actionName == VISIBILITY_STATE)
	{
		SetToolTip(TIP_VISIBILITY);
		listActor->SetToolTip(gedString(TIP_VISIBILITY_ACTOR) + TIP_ACTION_ACTOR);
		listState->SetToolTip(TIP_VISIBILITY_STATE);
	}
	else if(actionName == COLLISION_STATE)
	{
		SetToolTip(TIP_COLLISION_STATE);
		listActor->SetToolTip(gedString(TIP_COLLISION_STATE_ACTOR) + TIP_ACTION_ACTOR);
		listState->SetToolTip(TIP_COLLISION_STATE_STATE);
	}

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void EnableDisable::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);

	if(editAction->data.bEnable == ENABLE) listState->SetItem("Enable");
	else if(editAction->data.bEnable == DISABLE) listState->SetItem("Disable");
	else if(editAction->data.bEnable == DONT_DRAW_ONLY) listState->SetItem("Don't draw, but allow events");
}

EnableDisable::~EnableDisable()
{

}

void EnableDisable::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	case BT_ADD:
		{
			int state;
			gedString value = "ENABLE";

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(listState->GetText() == "Enable") state = ENABLE;
			else if(listState->GetText() == "Disable")
			{
				state = DISABLE;
				value = "DISABLE";
			}
			else if(listState->GetText() == "Don't draw, but allow events") 
			{
				state = DONT_DRAW_ONLY;
				value = "DONT_DRAW_ONLY";
			}
			else 
			{
				new PanelInfo("Please, select a state");
				return;
			}

			if(IsEditingScript())
			{
				//Auto complete script
				gedString function = "VisibilityState";
				if(actionName == COLLISION_STATE) function = "CollisionState";				

				char buf[256];
				sprintf(buf, "%s(\"%s\", %s);",
					function.c_str(),
					listActor->GetText().c_str(),
					value.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				if(actionName == COLLISION_STATE) SelectActionType(eventActor, Action::Call(eventActor)->SetCollisionEnable(listActor->GetText(), (bool)state));
				else if(actionName == VISIBILITY_STATE) SelectActionType(eventActor, Action::Call(eventActor)->SetVisibilityEnable(listActor->GetText(), state));
			}			
		}
		break;
	}
}



#endif //#ifndef STAND_ALONE_GAME