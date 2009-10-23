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


// ChangeAnimationDirection.cpp: implementation of the ChangeAnimationDirection class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangeAnimationDirection.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings


#define TIP_CHANGEANIMATION_DIRECTION_ACTOR			"\
Select the actor that will have its animation direction changed\n"


#define TIP_CHANGEANIMATION_DIRECTION_DIRECTION			"\
Select the animation direction:\n\
\n\
     Forward: play the animation from beginning to end\n\
    Backward: play the animation from end to beginning\n\
     Stopped: keep the animation stopped"


//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_ANIMATION_DIRECTION,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	105

ChangeAnimationDirection::ChangeAnimationDirection(Actor *actor, bool bOnCollision)
	: BaseAction("Change Animation Direction", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGEANIMATION_DIRECTION);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_ANIMATION_DIRECTION, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGEANIMATION_DIRECTION_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("Direction: ", 10, listActor->Down());
	listAnimationState = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION_DIRECTION); listAnimationState->SetToolTip(TIP_CHANGEANIMATION_DIRECTION_DIRECTION);
	
	
	//Close
	y = DrawHLine(listAnimationState->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 45, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
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
	
	listAnimationState->AddText("Forward");
	listAnimationState->AddText("Backward");
	listAnimationState->AddText("Stopped");
	listAnimationState->SetItem("Forward");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ChangeAnimationDirection::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);

	if(editAction->data.animation.state == FORWARD) listAnimationState->SetItem("Forward");
	else if(editAction->data.animation.state == BACKWARD) listAnimationState->SetItem("Backward");
	else listAnimationState->SetItem("Stopped");
}

ChangeAnimationDirection::~ChangeAnimationDirection()
{

}


void ChangeAnimationDirection::OnButton(Button *button, int buttonId)
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
			gedString stateStr;

			if(listAnimationState->GetText() == "Forward")
			{
				state = FORWARD;
				stateStr = "FORWARD";
			}
			else if(listAnimationState->GetText() == "Backward")
			{
				state = BACKWARD;
				stateStr = "BACKWARD";
			}
			else
			{
				state = STOPPED;
				stateStr = "STOPPED";
			}

			if(IsEditingScript())
			{
				//Auto complete script
				char buf[256];
				sprintf(buf, "ChangeAnimationDirection(\"%s\", %s);", 
					listActor->GetText().c_str(),
					stateStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetAnimationDirection(listActor->GetText(), state));
			}			
		}
		break;
	}
}

#endif //STAND_ALONE_GAME