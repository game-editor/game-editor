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


// WaitForFrame.cpp: implementation of the WaitForFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "WaitForFrame.h"
#include "GameControl.h"
#include "Sequence.h"
#include "ActorProperty.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_WAIT_FOR_FRAME_ANIMATION			"\
Select animation to wait"

#define TIP_WAIT_FOR_FRAME_LAST			"\
Choose yes to execute only the last action on the queue"
//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_ADD,
	BT_CLOSE,
	LS_ANIMATION,
	LS_LASTACTION
};

#define WIDTH	370
#define HEIGHT	185


WaitForFrame::WaitForFrame(Actor *actor, stAction *action, BaseAction *_actionPanel)
	: Panel("WaitForFrame", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	this->action = action;
	eventActor = actor;
	actionPanel = _actionPanel;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("Wait for frame", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Animation: ", 10, y);
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_WAIT_FOR_FRAME_ANIMATION);

	
	text = AddText("Frames: ", 10, listAnimation->Down() + 2);
	sequence = AddEditBox(text->Right(), text->Top(), 300);
	text = AddText("Note: The action will happen when the animation\narrives in one of the frames above.\nFrame sequence must be separate by ';'\nMaximum number of frames: 10", sequence->Left(), sequence->Down() + 4);	

	text = AddText("Execute only the last action in the queue: ", 10, text->Down() + 8);
	listLastAction = AddListPop(text->Right(), text->Top(), 64, 0, LS_LASTACTION); listLastAction->SetToolTip(TIP_WAIT_FOR_FRAME_LAST);

	
	//Close
	y = DrawHLine(listLastAction->Down() + 2);
	button = AddButton("Ok", (WIDTH - 135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);

	GameControl::Get()->PopulateAnimations(eventActor, listAnimation);	
	listAnimation->AddText(ANY_ANIMATION);
	listAnimation->SetItem(ANY_ANIMATION);

	listLastAction->AddText("Yes");
	listLastAction->AddText("No");
	listLastAction->SetItem("No");

	UpdateEdition();
}

void WaitForFrame::UpdateEdition()
{
	if(action->executionType != WAIT_FOR_FRAME_EXECUTION) return;
	
	if(*action->name) listAnimation->SetItem(action->name);

	if(action->bLastActionInQueueOnly) listLastAction->SetItem("Yes");
	
	gedString str;
	char buf[64];

	for(int i = 0; i < action->nFrames; i++)
	{
		sprintf(buf, "%ld;", action->frames[i]);
		str += buf;
	}

	sequence->SetText(str);
}

WaitForFrame::~WaitForFrame()
{

}




void WaitForFrame::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:
		{
			if(listAnimation->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a animation");
				return;
			}			

			if(!Sequence::IsValideFrameSequence(eventActor, listAnimation->GetText(), sequence->GetText(), true, 10))
			{
				new PanelInfo("Frame sequence must contain only numbers and ';'\nFrame can't be smaller than zero, or greater or equal than total animation frames");
				return;
			}

			action->executionType = WAIT_FOR_FRAME_EXECUTION;

			Sequence seq(DEFINED_SEQUENCE);
			seq.SetFrameSequence(sequence->GetText(), 0);
			
			if(listAnimation->GetText() != ANY_ANIMATION)
			{
				strcpy(action->name, listAnimation->GetText().c_str());
			}
			else
			{
				*action->name = 0;
			}

			if(listLastAction->GetText() == "Yes") action->bLastActionInQueueOnly = 1;
			else action->bLastActionInQueueOnly = 0;

			action->nFrames = seq.getNFrames();
			memcpy(action->frames, seq.getFrames(), action->nFrames*sizeof(Uint16));

			delete actionPanel;
			delete this;			
		}	
		break;

	case BT_CLOSE:
		{
			//Remove the action if is not edit action
			PostMessage(actionPanel, WAITFORFRAME_CLOSED);

			delete this;
		}
		break;
	}
}



#endif //#ifndef STAND_ALONE_GAME