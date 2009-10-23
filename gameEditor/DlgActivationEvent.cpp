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


// DlgActivationEvent.cpp: implementation of the DlgActivationEvent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgActivationEvent.h"
#include "GameControl.h"
#include "ActorEdit.h"



enum
{
	LS_ACTOR,
	BT_CLOSE,
	LS_ACTION
};

#define WIDTH	220
#define HEIGHT	85

DlgActivationEvent::DlgActivationEvent(Actor *actor)
	: Panel("DlgActivationEvent", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_DLG_ACTIVATIONEVENTS);

	eventActor = actionActor = actor;

	Text *text;
	Button *button;
	Actor *add;
	int y;

	//Title
	text = AddText(EV_ACTIVATION_EVENT, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("From Actor: ", 10, y);
	listActor = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ACTOR); listActor->SetToolTip(TIP_DLG_ACTIVATIONEVENTS_FROM);
		
	//Close
	y = DrawHLine(listActor->Down() + 5);
	if(Action::inNewActivationEvent())
	{
		add = AddButton("Choose Actor", 25, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTIVATIONEVENT_CHOOSE);	
	}
	else if(Action::getEditAction())
	{
		add = AddButton("Edit Action", (WIDTH-135)/2 - 12, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTION_EDIT);
	}
	else
	{
		add = AddListPop(30, y, 85, 0, LS_ACTION, "Add Action"); ((ListPop *)add)->SetToolTip(TIP_ADDACTION);	
		Action::Populate((ListPop *)add);
	}
	

	if(Action::getEditAction())
	{
		button = AddButton("Cancel", add->Right() + 10, y, 0, 0, BT_CLOSE); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CANCEL);	
	}
	else
	{
		button = AddButton("Close", add->Right() + 10, y, 0, 0, BT_CLOSE); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CLOSE);	
	}


	GameControl::Get()->PopulateActors(listActor);
	//listActor->SetItem(actionActor->getCloneIndex()?actionActor->getCloneName():actionActor->getActorName());	
	listActor->AddText(ALL_ACTORS);
	listActor->AddText(ANY_ACTOR);

	UpdateEdition();
}

void DlgActivationEvent::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	if(listActor->SetItem(editAction->eventData.data.name) == -1)
	{
		//Remove clone name
		gedString textCmp(editAction->eventData.data.name);
		int i = textCmp.find('.');

		if(i != gedString::npos)
		{
			textCmp = textCmp.substr(0, i);
			listActor->SetItem(textCmp);
		}
	}
}

DlgActivationEvent::~DlgActivationEvent()
{

}

bool DlgActivationEvent::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return true;
			}

			DoAction();
			Action::DoAction(eventActor, text);	
		}
		break;
	}

	return true;
}

void DlgActivationEvent::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			Action::clearEditAction();
			delete this;
		}
		break;
	case LS_ACTION:
		{
			if(listActor->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select an actor");
				return;
			}

			//Add activation event
			DoAction();
			
			if(Action::getEditAction())
			{
				//Edit current action
				Action::Call(eventActor)->Action::UpdateEvent(); 
				Action::DoAction(eventActor, Action::GetActionName(Action::getEditAction()));
			}
			else
			{
				//Add activation event
				ActorEdit::DoActivationEvent(true);
			}

			delete this;
		}
		break;
	}
}

void DlgActivationEvent::DoAction()
{
	gedString cloneName = listActor->GetText();
	Actor *actor = NULL;
	if(cloneName != ANY_ACTOR && cloneName != ALL_ACTORS && (actor = GameControl::Get()->GetActor(cloneName)) != NULL)
		cloneName = actor->getCloneName();
	
	Action::EventActivationEvent(cloneName);
}

#endif //#ifndef STAND_ALONE_GAME