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


// DestroyTimer.cpp: implementation of the DestroyTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DestroyTimer.h"
#include "GameControl.h"
#include "NewTimer.h"
#include "ExpressionEditor.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_DESTROYTIMER			"\
Use this action to destroy the selected timer on this actor"

#define TIP_DESTROYTIMER_TIMER			"\
Select the timer to be destroyed"

//////////////////////////////////////////////////////////////////////////////

enum
{
	LS_ACTOR,
	LS_TIMER,
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	400
#define HEIGHT	85

DestroyTimer::DestroyTimer(Actor *actor)
	: BaseAction("Destroy Timer", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_DESTROYTIMER);
	eventActor = actionActor = actor;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText(DESTROY_TIMER, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("   Timer: ", 10, y);
	listTimer = AddListPop(text->Right() + 2, text->Top(), 300, 0, LS_TIMER); listTimer->SetToolTip(TIP_DESTROYTIMER_TIMER);
	
	
	//Close
	y = DrawHLine(listTimer->Down() + 2);
	button = AddButton(Action::getEditAction()?"Ok":"Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); button->SetToolTip(TIP_ACTION_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); button->SetToolTip(TIP_ACTION_CANCEL); SetCancelButton(button);

	GameControl::Get()->PopulateTimers(listTimer);			
	listTimer->SetItem(listTimer->GetText(0));

	if(!ExpressionEditor::getExpressionEditor()) UpdateEdition();
}

void DestroyTimer::UpdateEdition()
{
	stAction *editAction = Action::getEditAction();
	if(!editAction) return;

	char timerName[256];
	sprintf(timerName, "%s (", editAction->data.timer.name);
	int lenght = strlen(timerName);
	for(int i = 0; i < listTimer->Count(); i++)
	{
		if(listTimer->GetText(i).substr(0, lenght) == timerName)
		{
			listTimer->SetItem(i);
			break;
		}
	}
}

DestroyTimer::~DestroyTimer()
{

}


void DestroyTimer::OnButton(Button *button, int buttonId)
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
			if(IsEditingScript())
			{
				//Auto complete script
				char timerName[256];
				strcpy(timerName, listTimer->GetText().c_str());
				int pos = listTimer->GetText().find(" (");
				if(pos >= 0) timerName[pos] = 0;

				char buf[256];
				sprintf(buf, "DestroyTimer(\"%s\");", timerName);
				InsertIntoScript(buf);
				delete this;
			}
			else
			{
				SelectActionType(eventActor, Action::Call(eventActor)->SetDestroyTimer(S_EVENT_ACTOR, listTimer->GetText()));
			}			
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME