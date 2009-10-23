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


// ToAnteriorPosition.cpp: implementation of the ToAnteriorPosition class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ToAnteriorPosition.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_TO_ANTERIOR_POSITION			"\
Use this action to move the selected actor to the position in the previous frame"

#define TIP_TO_ANTERIOR_POSITION_ACTOR			"\
Select the actor to be moved\n"


//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_CLOSE,
	BT_ADD,
	LS_ACTOR
};

#define WIDTH	220
#define HEIGHT	85

ToAnteriorPosition::ToAnteriorPosition(Actor *actor, bool bOnCollision)
	: BaseAction("To Anterior Position", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_TO_ANTERIOR_POSITION);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(TO_ANTERIOR_POSITION, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_TO_ANTERIOR_POSITION_ACTOR) + TIP_ACTION_ACTOR);	
	
	//Close
	y = DrawHLine(listActor->Down() + 2);
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

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ToAnteriorPosition::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
}

ToAnteriorPosition::~ToAnteriorPosition()
{

}


void ToAnteriorPosition::OnButton(Button *button, int buttonId)
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
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(IsEditingScript())
			{
				//Auto complete script
				char buf[256];
				sprintf(buf, "ToAnteriorPosition(\"%s\");",
					listActor->GetText().c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetToAnteriorPosition(listActor->GetText()));
			}
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME