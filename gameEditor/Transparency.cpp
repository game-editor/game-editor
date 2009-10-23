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


// Transparency.cpp: implementation of the Transparency class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "Transparency.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGE_TRANSPARENCY			"\
Use this action to change the selected actor's transparency"

#define TIP_CHANGE_TRANSPARENCY_ACTOR			"\
Select the actor that will have its transparency changed\n"

#define TIP_CHANGE_TRANSPARENCY_AMOUNT			"\
Set transparency amount"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	SL_TRANS,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	235
#define HEIGHT	105

Transparency::Transparency(Actor *actor, bool bOnCollision)
	: BaseAction("Set Transparency", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGE_TRANSPARENCY);
	eventActor = actionActor = actor;
	

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_TRANSPARENCY, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("       Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(gedString(TIP_CHANGE_TRANSPARENCY_ACTOR) + TIP_ACTION_ACTOR);
	text = AddText("Transparency: ", 10, listActor->Down() + 2);
	slideTrans = AddSlide(text->Right(), text->Top(), 100, 14, SL_TRANS); slideTrans->SetToolTip(TIP_CHANGE_TRANSPARENCY_AMOUNT);
	
	
	//Close
	y = DrawHLine(slideTrans->Down() + 2);
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

	oriTrans = actionActor->GetTransparency();

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void Transparency::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);	
	slideTrans->SetPos(editAction->data.transparency.data.fixed);	
	oriTrans = editAction->data.transparency.data.fixed;
}

Transparency::~Transparency()
{

}

bool Transparency::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			if(actionActor) actionActor->SetTransparency(oriTrans);


			actionActor = GameControl::Get()->GetActor(listActor->GetText());
			if(actionActor == EVENT_ACTOR) actionActor = eventActor;
			else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
			
		
			if(actionActor > COLLIDE_ACTOR) 
			{
				oriTrans = actionActor->GetTransparency();
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

void Transparency::OnSlide(Slide *slide, int slideId, double pos)
{
	if(actionActor) actionActor->SetTransparency(pos);
}

void Transparency::OnButton(Button *button, int buttonId)
{
	if(actionActor) actionActor->SetTransparency(oriTrans);

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
				sprintf(buf, "ChangeTransparency(\"%s\", %f);", 
					listActor->GetText().c_str(),
					slideTrans->GetPos());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetTransparency(listActor->GetText(), slideTrans->GetPos()));
			}			
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME