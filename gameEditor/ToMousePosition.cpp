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


// ToMousePosition.cpp: implementation of the ToMousePosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ToMousePosition.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_FOLLOW_MOUSE			"\
Use this action to make the selected actor follow the mouse"

#define TIP_FOLLOW_MOUSE_ACTOR			"\
Select the actor that will follow the mouse\n"

#define TIP_FOLLOW_MOUSE_AXIS			"\
Specify if the mouse affects x axis, y axis, both axis or none (end following)"
//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_PATH,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	105

ToMousePosition::ToMousePosition(Actor *actor, bool bOnCollision)
	: BaseAction("To Mouse Position", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_FOLLOW_MOUSE);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(FOLLOW_MOUSE, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_FOLLOW_MOUSE_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText(" Axis: ", 10, listActor->Down());
	listAxis = AddListPop(text->Right() + 2, text->Top(), 128); listAxis->SetToolTip(TIP_FOLLOW_MOUSE_AXIS);
	
	
	//Close
	y = DrawHLine(listAxis->Down() + 2);
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


	listAxis->AddText("Both");
	listAxis->AddText("x");
	listAxis->AddText("y");
	listAxis->AddText("none");
	listAxis->SetItem("Both");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ToMousePosition::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);	

	if(editAction->data.path.axis == X_AXIS) listAxis->SetItem("x");
	else if(editAction->data.path.axis == Y_AXIS) listAxis->SetItem("y");
	else if(editAction->data.path.axis == NONE_AXIS) listAxis->SetItem("none");
	else listAxis->SetItem("Both");
}

ToMousePosition::~ToMousePosition()
{

}


void ToMousePosition::OnButton(Button *button, int buttonId)
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
			int axis = BOTH_AXIS;
			gedString axisStr = "BOTH_AXIS";

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(listAxis->GetText() == "x")
			{
				axis = X_AXIS;
				axisStr = "X_AXIS";
			}
			else if(listAxis->GetText() == "y")
			{
				axis = Y_AXIS;
				axisStr = "Y_AXIS";
			}
			else if(listAxis->GetText() == "none") 
			{
				axis = NONE_AXIS;
				axisStr = "NONE_AXIS";
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "FollowMouse(\"%s\", %s);",
					listActor->GetText().c_str(),
					axisStr.c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetFollowMouse(listActor->GetText(), axis));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME