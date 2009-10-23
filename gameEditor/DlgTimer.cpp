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


// DlgTimer.cpp: implementation of the DlgTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgTimer.h"
#include "GameControl.h"
#include "ActorEdit.h"





enum
{
	LS_TIMER,
	LS_ANIMATION,
	BT_CANCEL,
	LS_ACTION
};

#define WIDTH	400
#define HEIGHT	85

DlgTimer::DlgTimer(Actor *actor)
	: Panel("DlgTimer", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_DLG_TIMER);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;
	Actor *add;
	int y;

	//Title
	text = AddText("Timer Event", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("  Timer: ", 10, y);
	listTimer = AddListPop(text->Right() + 2, text->Top(), 300, 0, LS_TIMER);
		
	//Close
	y = DrawHLine(listTimer->Down() + 5);
	if(Action::inNewActivationEvent())
	{
		add = AddButton("Choose Actor", (WIDTH-135)/2 - 15, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTIVATIONEVENT_CHOOSE);	
	}
	else if(Action::getEditAction())
	{
		add = AddButton("Edit Action", (WIDTH-135)/2 - 12, y, 0, 0, LS_ACTION); ((Button *)add)->SetToolTip(TIP_ACTION_EDIT);
	}
	else
	{
		add = AddListPop((WIDTH-135)/2 - 10, y, 85, 0, LS_ACTION, "Add Action"); ((ListPop *)add)->SetToolTip(TIP_ADDACTION);	
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

	GameControl::Get()->PopulateTimers(listTimer);		
	listTimer->Sort();
	//listTimer->SetItem(listTimer->GetText(0));

	UpdateEdition();
}

void DlgTimer::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	int n = listTimer->Count();

	for(int i = 0; 0 < n; i++)
	{
		gedString timerName(listTimer->GetText(i));

		int pos = timerName.find(" (");
		if(pos >= 0) timerName = timerName.substr(0, pos);

		if(timerName == editAction->eventData.data.name)
		{
			listTimer->SetItem(i);
			break;
		}
	}
}

DlgTimer::~DlgTimer()
{

}

bool DlgTimer::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_ACTION:
		{
			if(listTimer->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a timer");
				return true;
			}

			Action::EventTimer(listTimer->GetText());
			Action::DoAction(eventActor, text, true);	
		}
		break;
	}

	return true;
}

void DlgTimer::OnButton(Button *button, int buttonId)
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
			if(listTimer->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a timer");
				return;
			}

			//Add activation event
			Action::EventTimer(listTimer->GetText());
			
			if(Action::getEditAction())
			{
				//Edit current action
				Action::Call(eventActor)->Action::UpdateEvent(); 
				Action::DoAction(eventActor, Action::GetActionName(Action::getEditAction()), true);
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


#endif //#ifndef STAND_ALONE_GAME