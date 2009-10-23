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


// SetZDepth.cpp: implementation of the SetZDepth class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "SetZDepth.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGE_ZDEPTH			"\
Use this action to change the selected actor's depth\n\
\n\
Change the actor's depth to put an actor in front or behind others.\n\
Increasing Z Depth makes the actor look as if it's closer to you."

#define TIP_CHANGE_ZDEPTH_ACTOR			"\
Select the actor that will have its depth changed\n"

#define TIP_CHANGE_ZDEPTH_AMOUNT			"\
Set depth"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	SL_TRANS,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	235
#define HEIGHT	100

SetZDepth::SetZDepth(Actor *actor, bool bOnCollision)
	: BaseAction("Set ZDepth", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGE_ZDEPTH);
	eventActor = actionActor = actor;
	

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_ZDEPTH, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("       Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGE_ZDEPTH_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("     Z Depth: ", 10, listActor->Down() + 2);
	slide = AddSlide(text->Right(), text->Top(), 100, 14, SL_TRANS); slide->SetToolTip(TIP_CHANGE_ZDEPTH_AMOUNT);
	
	
	//Close
	y = DrawHLine(slide->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 55, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
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

	oriZDepth = actionActor->getImage()->ZDepth();

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void SetZDepth::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);	
	slide->SetPos(editAction->data.zdepth/(double)MAX_ACTOR_DEPTH);	
	oriZDepth = editAction->data.zdepth;
}

SetZDepth::~SetZDepth()
{

}

bool SetZDepth::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			if(actionActor) actionActor->getImage()->SetZDepth(oriZDepth);


			actionActor = GameControl::Get()->GetActor(listActor->GetText());
			
			if(actionActor == EVENT_ACTOR) actionActor = eventActor;
			else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
			
			
			if(actionActor > COLLIDE_ACTOR) 
			{
				oriZDepth = actionActor->getImage()->ZDepth();
			}
			else
			{
				actionActor = NULL;
			}
		}
		break;
	}

	return true;
}

void SetZDepth::OnSlide(Slide *slide, int slideId, double pos)
{
	if(actionActor) actionActor->getImage()->SetZDepth(pos*MAX_ACTOR_DEPTH);
}

void SetZDepth::OnButton(Button *button, int buttonId)
{
	if(actionActor) actionActor->getImage()->SetZDepth(oriZDepth);

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
				sprintf(buf, "ChangeZDepth(\"%s\", %f);", 
					listActor->GetText().c_str(),
					slide->GetPos());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetSetZDepth(listActor->GetText(), slide->GetPos()));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME