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


// ChangeParent.cpp: implementation of the ChangeParent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ChangeParent.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CHANGEPARENT			"\
Use this action to change the actor's parent\n\
\n\
The parent can be any actor in game, included view actor, or no parent.\n\
The child actor will be moved with parent\n\
The (x, y) coordinates of a child actor are relative to the parent\n\
If the parent changes your transparency or color the childs will inherit the changes"

#define TIP_CHANGEPARENT_CHILD			"\
Select the child actor\n"

#define TIP_CHANGEPARENT_PARENT			"\
Select the parent actor\n\
Select \"no parent\" to remove the selected child parent"
//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_PARENT,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	105

ChangeParent::ChangeParent(Actor *actor, bool bOnCollision)
	: BaseAction("Change Parent", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CHANGEPARENT);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CHANGE_PARENT, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Child: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR);  listActor->SetToolTip(gedString(TIP_CHANGEPARENT_CHILD) + TIP_ACTION_ACTOR);
	text = AddText("   Parent: ", 10, listActor->Down());
	listParent = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_PARENT); listParent->SetToolTip(TIP_CHANGEPARENT_PARENT);
	
	
	//Close
	y = DrawHLine(listParent->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", 45, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);	


	GameControl::Get()->PopulateActors(listParent);
	listParent->AddText(NO_PARENT);
	if(actionActor && actionActor->getParent())
	{
		listParent->SetItem(actionActor->getCloneName());
	}
	else
	{
		listParent->SetItem(NO_PARENT);
	}
	
	GameControl::Get()->PopulateActors(listActor);
	
	listActor->AddText(S_EVENT_ACTOR);
	listParent->AddText(S_EVENT_ACTOR);

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listActor->AddText(S_PARENT_ACTOR);
		listActor->AddText(S_CREATOR_ACTOR);

		listParent->AddText(S_PARENT_ACTOR);
		listParent->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listParent->AddText(S_COLLIDE_ACTOR);
	if(bOnCollision) listActor->AddText(S_COLLIDE_ACTOR);

	listActor->SetItem(S_EVENT_ACTOR);

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void ChangeParent::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	listActor->SetItem(editAction->actorName);
	if(listParent->SetItem(editAction->data.name) == -1)
	{
		listParent->SetItem(NO_PARENT);
	}
}

ChangeParent::~ChangeParent()
{

}

bool ChangeParent::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			/*actionActor = GameControl::Get()->GetActor(text);
			if(actionActor > COLLIDE_ACTOR && actionActor->getParent())
			{
				listParent->SetItem(actionActor->getCloneName():actionActor->getActorName());
			}
			else
			{
				listParent->SetItem(NO_PARENT);
			}*/

			listParent->SetItem(NO_PARENT);
		}
		break;
	}

	return true;
}

void ChangeParent::OnButton(Button *button, int buttonId)
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
				new PanelInfo("Please, select a child");
				return;
			}

			if(listParent->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a parent");
				return;
			}

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "ChangeParent(\"%s\", \"%s\");",
					listActor->GetText().c_str(),
					listParent->GetText().c_str());
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetParent(listActor->GetText(), listParent->GetText()));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME