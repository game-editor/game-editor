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


// MoveTo.cpp: implementation of the MoveTo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "MoveTo.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_MOVETO			"\
Use this action to move the selected Actor to a specified position, at a specified velocity:\n\
\n\
   Relative to: The Actor can be moved to an game center,\n\
                relative to the mouse or any Actor in a game.\n\
   x, y: An Actor's position\n\
   Velocity: An Actor's movement velocity (speed)."

#define TIP_MOVETO_ACTOR			"\
Select the Actor to move\n"

#define TIP_MOVETO_RELATIVE			"\
The Actor can be moved to an game center,\n\
relative to the mouse or any Actor in a game."

#define TIP_MOVETO_AVOID			"\
Set the actor to avoid collisions while moving.\n\
If you select the actor name, not a clone name, all clones will be considered.\n\
If you select a actor with tiles, the bound box of each tile will be considered.\n\
\n\
Only the actor bound box will be considered, not the real shape.\n\
\n\
The move path is calculated only when the action is executed.\n\
So, if you need avoid collision with other moving actors, you need to call the\n\
Move To action again.\n\
\n\
To make a more natural move, the resulting path is smoothing, so if you want to move\n\
the actor through a maze, sometimes the actor will touch the walls."

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_RELATIVE,
	LS_AVOID,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	240
#define HEIGHT	120

static int getHeight()
{
	int height = HEIGHT;

	if(Tutorial::IsCompatible(VERSION_MOVE_TO_RELATIVE))
	{
		height += 23;
	}

	if(Tutorial::IsCompatible(VERSION_MOVE_TO_PATHFINDER))
	{
		height += 23;
	}

	return height;
}

MoveTo::MoveTo(Actor *actor, bool bOnCollision)
	: BaseAction("Create Actor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - getHeight())/2,
						  WIDTH, getHeight())
{
	SetModal();
	SetToolTip(TIP_MOVETO);
	this->eventActor = actionActor = actor;
	listRelative = listAvoid = NULL;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(MOVE_TO, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("      Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR);  listActor->SetToolTip(gedString(TIP_MOVETO_ACTOR) + TIP_ACTION_ACTOR);
	y = listActor->Down();

	if(Tutorial::IsCompatible(VERSION_MOVE_TO_RELATIVE))
	{
		text = AddText("Relative to: ", 10, y);
		listRelative = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_RELATIVE);  listRelative->SetToolTip(gedString(TIP_MOVETO_RELATIVE));
		y = listRelative->Down() + 2;
		
		GameControl::Get()->PopulateActors(listRelative);

		listRelative->AddText(S_EVENT_ACTOR);
		listRelative->AddText(S_PARENT_ACTOR);
		listRelative->AddText(S_CREATOR_ACTOR);

		listRelative->AddText("Game Center");
		listRelative->AddText("Mouse Position");

		listRelative->SetItem("Game Center");
		
		if(bOnCollision) listRelative->AddText(S_COLLIDE_ACTOR);
	}

	if(Tutorial::IsCompatible(VERSION_MOVE_TO_PATHFINDER))
	{
		text = AddText("      Avoid: ", 10, y);
		listAvoid = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_AVOID);  listAvoid->SetToolTip(gedString(TIP_MOVETO_AVOID));
		y = listAvoid->Down() + 2;

		GameControl::Get()->PopulateActors(listAvoid);

		listAvoid->AddText(S_EVENT_ACTOR);
		listAvoid->AddText(S_PARENT_ACTOR);
		listAvoid->AddText(S_CREATOR_ACTOR);
		
		if(bOnCollision) listAvoid->AddText(S_COLLIDE_ACTOR);

		listAvoid->AddText(NO_SELECTION);
		listAvoid->SetItem(NO_SELECTION);
	}

	text = AddText("   Position: x", 10, y+2);
	posX = AddEditBox(text->Right() + 2, text->Top(), 50); posX->SetNumeric(-650000, 650000, 1);

	text = AddText("y", posX->Right()+5, posX->Top());
	posY = AddEditBox(text->Right() + 4, text->Top(), 50); posY->SetNumeric(-650000, 650000, 1);

	text = AddText("   Velocity:  ", 10, posY->Down() + 2);
	velocity = AddEditBox(text->Right() + 2, text->Top(), 50);  velocity->SetNumeric(0, 1000, .1);
	
	
	//Close
	y = DrawHLine(velocity->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);

	posX->SetText(0);
	posY->SetText(0);
	velocity->SetText(1);

	GameControl::Get()->PopulateActors(listActor/*, false*/);
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

void MoveTo::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);

	posX->SetText((int)editAction->data.moveto.x);
	posY->SetText((int)editAction->data.moveto.y);
	velocity->SetText(editAction->data.moveto.velocity);

	if(listRelative)
	{
		if(editAction->data.moveto.relative[0] == 0)
		{
			listRelative->SetItem("Game Center");
		}
		else
		{
			listRelative->SetItem(editAction->data.moveto.relative);
		}
	}

	if(listAvoid)
	{
		if(editAction->data.moveto.obstacle[0] == 0)
		{
			listAvoid->SetItem(NO_SELECTION);
		}
		else
		{
			listAvoid->SetItem(editAction->data.moveto.obstacle);
		}
	}
}

MoveTo::~MoveTo()
{

}


void MoveTo::OnButton(Button *button, int buttonId)
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
			double	x = atof(posX->GetText().c_str()), 
					y = atof(posY->GetText().c_str()),
					vel = atof(velocity->GetText().c_str());

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(!ExpressionEditor::getExpressionEditor() && vel <= 0.0) //Acept expressions
			{
				new PanelInfo("Please, enter a positive velocity"); 
				return;
			}

			gedString relativeActor(S_NONE);

			if(listRelative)
			{
				if(listRelative->GetSelectedIndex() == -1)
				{
					new PanelInfo("Please, select an relative option");
					return;
				}

				relativeActor = listRelative->GetText();

				//if(relativeActor == "Game Center") relativeActor = S_NONE;
				//else if(relativeActor == "Mouse Position") relativeActor = S_MOUSE; //keep the space to avoide conflit with other actor with this name
			}

			gedString avoidActor;

			if(listAvoid && listAvoid->GetText() != NO_SELECTION)
			{
				avoidActor = listAvoid->GetText();
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "MoveTo(\"%s\", %f, %f, %f, \"%s\", \"%s\");",
					listActor->GetText().c_str(),
					x, y, vel, 
					relativeActor.getCharBuf(),
					avoidActor.getCharBuf()?avoidActor.getCharBuf():"");
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetMoveTo(listActor->GetText(), x, y, vel,
					relativeActor.getCharBuf(), avoidActor.getCharBuf(), STRAIGHT_LINE));
			}			
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME