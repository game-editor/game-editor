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


// CreateActor.cpp: implementation of the CreateActor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "CreateActor.h"
#include "GameControl.h"
#include "ExpressionEditor.h"
#include "Tutorial.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_CREATEACTOR			"\
Use this action to put a new actor in game"

#define TIP_CREATEACTOR_ACTOR			"\
Select the actor to be created"

#define TIP_CREATEACTOR_ANIMATION			"\
Select the animation"

#define TIP_CREATEACTOR_PARENT			"\
Select the parent actor or \"no parent\""

#define TIP_CREATEACTOR_PATH			"\
Select the path or \"no path\""

#define TIP_CREATEACTOR_RELATIVE			"\
Choose yes to create the actor at position (x, y) relative to the parent actor"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_ANIMATION,
	LS_PARENT,
	LS_PATH,
	LS_RELATIVEPOS,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	220
#define HEIGHT	200

CreateActor::CreateActor(Actor *actor, bool bOnCollision)
	: BaseAction("Create Actor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_CREATEACTOR);
	this->eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(CREATE_ACTOR, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("    Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(TIP_CREATEACTOR_ACTOR);

	text = AddText("Animation: ", 10, listActor->Down());
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_CREATEACTOR_ANIMATION);

	text = AddText("   Parent: ", 10, listAnimation->Down());
	listParent = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_PARENT); listParent->SetToolTip(TIP_CREATEACTOR_PARENT);

	text = AddText("     Path: ", 10, listParent->Down());
	listPath = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_PATH); listPath->SetToolTip(TIP_CREATEACTOR_PATH);

	text = AddText("Initial position: ", 10, listPath->Down() + 4);
	
	text = AddText("x", 10, text->Down());
	posX = AddEditBox(text->Right() + 2, text->Top(), 50); posX->SetNumeric(-650000, 650000, 1);

	text = AddText("y", posX->Right()+5, posX->Top());
	posY = AddEditBox(text->Right() + 4, text->Top(), 50); posY->SetNumeric(-650000, 650000, 1);

	text = AddText("Relative to creator: ", 10, posY->Down() + 2);
	listRelativePos = AddListPop(text->Right() + 2, text->Top(), 40, 0, LS_RELATIVEPOS); listRelativePos->SetToolTip(TIP_CREATEACTOR_RELATIVE);
	
	
	//Close
	y = DrawHLine(listRelativePos->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);

	posX->SetText(0);
	posY->SetText(0);

	GameControl::Get()->PopulateActors(listActor, false);
	listActor->RemoveText(VIEW_NAME);
	GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
	GameControl::Get()->PopulateActors(listParent);
	listParent->AddText(NO_PARENT);
	listParent->AddText(S_EVENT_ACTOR);	

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		listParent->AddText(S_PARENT_ACTOR);
		listParent->AddText(S_CREATOR_ACTOR);
	}

	if(bOnCollision) listParent->AddText(S_COLLIDE_ACTOR);

	GameControl::Get()->PopulatePaths(listPath);

	if(!Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		//Set default only in old tutorials
		listActor->SetItem(actionActor->getActorName());
	}
	else
	{
		//Don't select animations in new version
		listAnimation->RemoveAll();
		listAnimation->SetItem(-1);
	}

	listParent->SetItem(NO_PARENT);
	listPath->SetItem(NO_PATH);

	listRelativePos->AddText("Yes");
	listRelativePos->AddText("No");
	listRelativePos->SetItem("Yes");

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void CreateActor::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	Actor *actor = GameControl::Get()->GetActor(editAction->data.createActor.name);
	
	if(actor == EVENT_ACTOR) actor = eventActor;
	else if(actor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();

	if(actor > COLLIDE_ACTOR)
	{
		GameControl::Get()->PopulateAnimations(actor, listAnimation);
	}


	listActor->SetItem(editAction->data.createActor.name);
	listAnimation->SetItem(editAction->data.createActor.animationName);
	if(listParent->SetItem(editAction->data.createActor.parentName) == -1)
	{
		listParent->SetItem(NO_PARENT);
	}

	if(listPath->SetItem(editAction->data.createActor.pathName) == -1)
	{
		listPath->SetItem(NO_PATH);
	}

	if(editAction->bAbsolutePosition) listRelativePos->SetItem("No");
	else listRelativePos->SetItem("Yes");

	posX->SetText(editAction->x);
	posY->SetText(editAction->y);
}

CreateActor::~CreateActor()
{

}

bool CreateActor::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(BaseAction::OnList(list, index, text, listId)) return true;

	switch(listId)
	{
	case LS_ACTOR:
		{
			actionActor = GameControl::Get()->GetActor(text);
			
			if(actionActor == EVENT_ACTOR) actionActor = eventActor;
			else if(actionActor == PARENT_ACTOR && eventActor->getParent() && eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
			
			if(actionActor > COLLIDE_ACTOR)
			{
				GameControl::Get()->PopulateAnimations(actionActor, listAnimation);
			}	
		}
		break;
	}

	return true;
}

void CreateActor::OnButton(Button *button, int buttonId)
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
			gedString animation = listAnimation->GetText();
			if(!animation.size())
			{
				/*new PanelInfo("The actor has no animations"); 
				return;*/
				animation = ACTOR_GED_ICON;
			}

			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			if(listParent->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a parent");
				return;
			}

			//Check create actor infinit loop
			stAction *currentAction = Action::getEditAction(), tmp;
			if(!currentAction)
			{
				tmp.eventData = Action::getActualEvent();
				currentAction = &tmp;
			}

			if( currentAction->eventData.eventType == EVENT_CREATE &&
				listActor->GetText() == eventActor->getActorName())
			{
				new PanelInfo("You can't create an actor in your own Create Actor event");
				return;
			}

			int x = atol(posX->GetText().c_str()), y = atol(posY->GetText().c_str());			
			bool bAbsolutePosition = true;
			if(listRelativePos->GetText() == "Yes") bAbsolutePosition = false;

			Actor *parent = GameControl::Get()->GetActor(listParent->GetText());
			Path *path    = GameControl::Get()->GetPath(listPath->GetText());

			if(IsEditingScript())
			{
				//Auto complete script

				char buf[256];
				sprintf(buf, "CreateActor(\"%s\", \"%s\", \"%s\", \"%s\", %ld, %ld, %s);",
					listActor->GetText().c_str(),
					animation.c_str(),
					listParent->GetText().c_str(),
					listPath->GetText().c_str(),
					x, y, bAbsolutePosition?"true":"false");
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetCreateActor(listActor->GetText(), 
													 animation,
													 parent, path, x, y, bAbsolutePosition));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME