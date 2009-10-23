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


// NewTimer.cpp: implementation of the NewTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "NewTimer.h"
#include "GameControl.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_NEWTIMER			"\
Create a new timer definition:\n\
\n\
       Name: timer name\n\
       Time: timer interval in milliseconds (or max interval for random timers)\n\
       Type: Periodic or Random\n\
   Min time: in random timers, specify min timer interval here and max timer interval in time field\n\
     Repeat: Forever or Specify quantity\n\
      Count: if timer is not Forever, set number of repetitions here"

#define TIP_NEWTIMER_TYPE			"\
Select the Periodic or Random timer\n\
\n\
       Periodic: repeat at fixed intervals\n\
   Random timer: randomly intervals between min and max values"

#define TIP_NEWTIMER_COUNT			"\
Select Forever or Specify quantity:\n\
\n\
   Forever: use the DestroyTimer action to destroy this timer when you want\n\
   Specify quantity: the timer will be destroyed after the specified count"
//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_OK,
	BT_CANCEL,
	LS_TIMER_TYPE,
	LS_COUNT_TYPE
};

#define WIDTH	200
#define HEIGHT	200

NewTimer::NewTimer(CreateTimer *parent)
	: Panel("NewTimer", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_NEWTIMER);

	Text *text;
	Button *button;
	int y;

	text = AddText("Create new timer", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("     Name: ", 10, y);
	name = AddEditBox(text->Right(), text->Top());

	textTime = AddText("Time (ms): ", 10, name->Down() + 2);
	time = AddEditBox(textTime->Right(), textTime->Top()); time->SetNumeric(12, 60000, 1);



	y = DrawHLine(time->Down() + 2);
	text = AddText("     Type: ", 10, y);
	listType = AddListPop(text->Right() + 2, text->Top(), 100, 0, LS_TIMER_TYPE); listType->SetToolTip(TIP_NEWTIMER_TYPE);

	text = AddText(" Min time: ", 10, listType->Down() + 2);
	minRandTime = AddEditBox(text->Right(), text->Top()); minRandTime->SetNumeric(10, 60000, 1);


	y = DrawHLine(minRandTime->Down() + 2);
	text = AddText("   Repeat: ", 10, y);
	listCountType = AddListPop(text->Right() + 2, text->Top(), 100, 0, LS_COUNT_TYPE); listCountType->SetToolTip(TIP_NEWTIMER_COUNT);

	text = AddText(" 	  Count: ", 10, listCountType->Down() + 2);
	count = AddEditBox(text->Right(), text->Top()); count->SetNumeric(1, 1000000, 1);

	listType->AddText("Periodic");
	listType->AddText("Random");

	listCountType->AddText("Forever");
	listCountType->AddText("Specify quantity");

	//Default
	listType->SetItem("Periodic");
	listCountType->SetItem("Forever");
	
	minRandTime->SetText("*");
	count->SetText("*");
	
	//End dialog
	y = DrawHLine(count->Down() + 2);
	button = AddButton("Ok", (WIDTH-135)/2, y, 0, 0, BT_OK); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); SetCancelButton(button);
	
	this->parent = parent;
}

NewTimer::~NewTimer()
{

}

bool NewTimer::OnList(ListPop *list, int index, gedString &text, int listId)
{
	switch(listId)
	{
	case LS_TIMER_TYPE:
		{
			if(text == 	"Periodic")
			{
				minRandTime->SetText("*");				
				textTime->SetText("Time (ms): ");
			}
			else
			{
				minRandTime->SetText("10");
				textTime->SetText(" Max (ms): ");
			}
		}
		break;
	case LS_COUNT_TYPE:
		{
			if(text == 	"Forever")
				count->SetText("*");				
			else
				count->SetText("1");
		}
		break;
	}

	return true;
}

void NewTimer::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_OK:
		{
			if(name->GetText().size() <= 0 || name->GetText().size() > NAME_LIMIT)
			{
				new PanelInfo("Please, enter valid timer name");	
				return;
			}

			if(name->GetText().find('(') != gedString::npos)
			{
				new PanelInfo("Character '(' not valid\nPlease, enter valid timer name");	
				return;
			}

			if(GameControl::Get()->GetTimer(name->GetText()))
			{
				new PanelInfo("Timer already exists\nPlease, enter other timer name");	
				return;
			}

			stTimerDefinition timer;

			timer.interval = atol(time->GetText().c_str());
			timer.minRandomInterval = atol(minRandTime->GetText().c_str());
			
			if(listType->GetText() == "Periodic")
				timer.type = PERIODIC_TIMER;
			else
				timer.type = RANDOM_TIMER;

			if(listCountType->GetText() == "Forever")
				timer.count = REPEATE_FOREVER;
			else
				timer.count = atol(count->GetText().c_str());
			
			if(timer.interval < 12)
			{
				new PanelInfo("Time must be greater than 12 ms");	
				return;
			}

			if(timer.type == RANDOM_TIMER && timer.minRandomInterval >= timer.interval)
			{
				new PanelInfo("Minimal random time must be smaller than specified time");	
				return;
			}

			if(timer.type == RANDOM_TIMER && timer.minRandomInterval < 10)
			{
				new PanelInfo("Minimal random time greater than 10 ms");	
				return;
			}

			if(timer.count != REPEATE_FOREVER && timer.count <= 0)
			{
				new PanelInfo("Timer count must be greater than zero");	
				return;
			}

			parent->AddTimerDefinition(name->GetText(), timer.interval, timer.count, timer.type, timer.minRandomInterval);
			delete this;
		}
		break;

	case BT_CANCEL:
		{
			delete this;
		}
		break;
	}
}

#endif //#ifndef STAND_ALONE_GAME