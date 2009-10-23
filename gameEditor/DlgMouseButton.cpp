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


// DlgMouseButton.cpp: implementation of the DlgMouseButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgMouseButton.h"
#include "GameControl.h"
#include "Action.h"
#include "ActorEdit.h"





enum
{
	BT_CANCEL,
	LS_ACTION
};

#define WIDTH	215
#define HEIGHT	100

DlgMouseButton::DlgMouseButton(Actor *actor, bool bUp)
	: Panel("DlgMouseButton", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	eventActor = actor;
	this->bUp = bUp;
	button = 0;

	Text *text;
	Button *button;	
	Actor *add;
	int y;

	//Title
	text = AddText(bUp?"Mouse Button Up Event":"Mouse Button Down Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Press mouse button: ", 10, y);	
	textButton = AddText("              ", text->Right(), text->Top());	

	y = textButton->Down();
	if(!bUp)
	{
		SetToolTip(TIP_DLG_MOUSEBUTTON_DOWN);

		text = AddText("Drag: ", 10, y+2);
		listDrag = AddListPop(text->Right(), text->Top(), 128); listDrag->SetToolTip(TIP_DLG_MOUSEBUTTON_DRAG);
		listDrag->AddText("Enable");
		listDrag->AddText("Disable");
		listDrag->SetItem("Disable");

		y = listDrag->Down();
	}
	else 
	{
		SetToolTip(TIP_DLG_MOUSEBUTTON_UP);
		y += 20;
	}
	
	//Close
	y = DrawHLine(y + 5);
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
		button = AddButton("Cancel", add->Right() + 10, y, 0, 0, BT_CANCEL); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CANCEL);	
	}
	else
	{
		button = AddButton("Close", add->Right() + 10, y, 0, 0, BT_CANCEL); if(!Action::inNewActivationEvent()) button->SetToolTip(TIP_ACTION_CLOSE);	
	}

	UpdateEdition();
}

void DlgMouseButton::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	OnMouseButtonDown(0, 0, editAction->eventData.data.mouse.button);

	if(editAction->eventData.eventType == EVENT_MOUSEBUTTONDOWN)
	{
		if(editAction->eventData.data.mouse.bDrag)
		{
			listDrag->SetItem("Enable");
		}
	}
}

DlgMouseButton::~DlgMouseButton()
{

}

void DlgMouseButton::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CANCEL:
		{
			Action::clearEditAction();
			delete this;
		}
		break;
	case LS_ACTION:
		{
			//Add activation event
			if(!DoAction()) return;	
			
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


bool DlgMouseButton::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(!DoAction()) return true;

			Action::DoAction(eventActor, text);	
		}
		break;
	}

	return true;
}

bool DlgMouseButton::OnMouseButtonDown(int x, int y, Uint8 button)
{
	this->button = button;
	if(this->button == SDL_BUTTON_RIGHT) textButton->SetText("Right");
	else if(this->button == SDL_BUTTON_LEFT) textButton->SetText("Left");
	else if(this->button == SDL_BUTTON_MIDDLE) textButton->SetText("Middle");
	
	return true;
}

bool DlgMouseButton::DoAction()
{
	if(!button)
	{
		new PanelInfo("Please, press a mouse button");
		return false;
	}
	
	if(bUp) Action::EventMouseButtonUp(0, 0, button);
	else
	{
		bool bDrag = false;
		if(listDrag->GetText() == "Enable") bDrag = true;
		Action::EventMouseButtonDown(0, 0, button, bDrag);
	}

	return true;
}


#endif //#ifndef STAND_ALONE_GAME