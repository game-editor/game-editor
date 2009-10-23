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


// ChangeAnimation.cpp: implementation of the ChangeAnimation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangeAnimation.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"



enum
{
	LS_ACTOR,
	LS_ANIMATION,
	LS_ANIMATION_STATE,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	125

ChangeAnimation::ChangeAnimation(Actor *actor)
	: BaseAction("Change Animation", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGEANIMATION);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_ANIMATION, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGEANIMATION_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("Animation: ", 10, listActor->Down());
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_CHANGEANIMATION_ANIMATION);
	text = AddText("Direction: ", 10, listAnimation->Down());
	listAnimationState = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION_STATE); listAnimationState->SetToolTip(TIP_CHANGEANIMATION_DIRECTION);
	
	
	//Close
	y = DrawHLine(listAnimationState->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 45, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);


	GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
	GameControl::Get()->PopulateActors(listActor);
	listActor->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		//listActor->AddText(S_CREATOR_ACTOR); //Don't use: you don't know the animations of creator
	}

	listActor->SetItem(S_EVENT_ACTOR);	

	listAnimationState->AddText("Forward");
	listAnimationState->AddText("Backward");
	listAnimationState->AddText("Stopped");
	listAnimationState->AddText("No change");
	listAnimationState->SetItem("Forward");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ChangeAnimation::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
	actionActor = GameControl::Get()->GetActor(editAction->actorName);

	if(actionActor == EVENT_ACTOR) actionActor = eventActor;
	else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();

	if(actionActor > COLLIDE_ACTOR)
	{
		GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
	}

	listAnimation->SetItem(editAction->data.animation.name);

	if(editAction->data.animation.state == FORWARD) listAnimationState->SetItem("Forward");
	else if(editAction->data.animation.state == BACKWARD) listAnimationState->SetItem("Backward");
	else if(editAction->data.animation.state == NO_CHANGE) listAnimationState->SetItem("No change");
	else listAnimationState->SetItem("Stopped");
}

ChangeAnimation::~ChangeAnimation()
{
	
}

bool ChangeAnimation::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			actionActor = GameControl::Get()->GetActor(text);
			if(actionActor == EVENT_ACTOR)
			{
				actionActor = eventActor;
			}
			else if(actionActor == PARENT_ACTOR)
			{
				if(eventActor->getParent() != GameControl::Get()->GetAxis())
				{
					actionActor = eventActor->getParent();
				}
				else
				{
					new PanelInfo("This actor has no parent");
					listActor->SetItem(S_EVENT_ACTOR);
					actionActor = eventActor;
				}
			}
			/*else if(actionActor == CREATOR_ACTOR)
			{
				actionActor = eventActor->getCreator();
			}*/

			GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
		}
		break;
	}

	return true;
}

void ChangeAnimation::OnButton(Button *button, int buttonId)
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
			if(!actionActor || (actionActor > COLLIDE_ACTOR && actionActor->NumAnimations() == 0))
			{
				new PanelInfo("This actor has no animations");
				return;
			}

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
			else if(listAnimationState->GetText() == "No change")
			{
				state = NO_CHANGE;
				stateStr = "NO_CHANGE";
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
				sprintf(buf, "ChangeAnimation(\"%s\", \"%s\", %s);", 
					listActor->GetText().c_str(),
					listAnimation->GetText().c_str(),
					stateStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetAnimation(listActor->GetText(), listAnimation->GetText(), state));
			}			
		}
		break;
	}
}


#endif //STAND_ALONE_GAME