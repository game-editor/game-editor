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


// AddSequenceDialog.cpp: implementation of the AddSequenceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "AddSequenceDialog.h"
#include "ActorProperty.h"
#include "GameControl.h"
#include "Sequence.h"



enum
{
	BT_OK,
	BT_CANCEL
};

#define WIDTH	370
#define HEIGHT	140

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_SEQUENCE			"\
Use the Add Sequence to define a frame sequence based on the animation.\n\
So, you can create a new animation sequence from existing animation.\n\
\n\
Sequence name: Give a name for this sequence, so you can identify on Actor Control\n\
Frame rate: The velocity of new sequence in frames per second\n\
Frames: Enter the frame sequence (Ex.: 14;2;9;10;22;2;3;4...)"

#define TIP_SEQUENCE_ANIMATION			"\
Select the animation with wanted frames"



//////////////////////////////////////////////////////////////////////////////


AddSequenceDialog::AddSequenceDialog(ActorProperty *actorProperty, Actor *actor)
: Panel("AddSequenceDialog", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	SetToolTip(TIP_SEQUENCE);

	this->actorProperty = actorProperty;
	this->actor = actor;

	Text *text;
	Button *button;	
	int y;

	text = AddText("Add Sequence", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Animation: ", 10, y);
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0); listAnimation->SetToolTip(TIP_SEQUENCE_ANIMATION);

	text = AddText("Sequence name: ", 10, listAnimation->Down() + 2);
	name = AddEditBox(text->Right(), text->Top()); 

	text = AddText("   Frame rate: ", name->Right(), name->Top()); 
	editFps = AddEditBox(text->Right(), text->Top(), 40);  editFps->SetNumeric(1, GameControl::Get()->getFrameRate(), 1);
	text = AddText("fps", editFps->Right() + 2, editFps->Top());
	

	text = AddText("Frames: ", 10, editFps->Down() + 2);
	sequence = AddEditBox(text->Right(), text->Top(), 300); 

	text = AddText("Note: Frame sequence must be separate by ';'", sequence->Left(), sequence->Down() + 4);
	
	
	//End dialog
	y = DrawHLine(text->Down() + 5);
	button = AddButton("Ok", (WIDTH-135)/2, y, 0, 0, BT_OK); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); SetCancelButton(button);	

	GameControl::Get()->PopulateAnimations(actor, listAnimation, false);

	KrAction *action = NULL;
	if(actor)
	{
		editFps->SetText(actor->GetAnimation());
		action = actor->getSpriteResource()->GetAction(actor->GetAnimation());
	}

	if(action) editFps->SetText(action->getFrameRate());
	else editFps->SetText(GameControl::Get()->getFrameRate());
}

AddSequenceDialog::~AddSequenceDialog()
{

}

bool AddSequenceDialog::OnList(ListPop *list, int index, gedString &text, int listId)
{	
	if(text == ANY_ANIMATION)
	{
		editFps->SetText(GameControl::Get()->getFrameRate());
	}
	else
	{
		KrAction *action = actor->getSpriteResource()->GetAction(text);
		if(action) editFps->SetText(action->getFrameRate());
	}	

	return true;
}

void AddSequenceDialog::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_OK:
		{
			if(listAnimation->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a animation");
				return;
			}

			if(name->GetText().size() <= 0 || name->GetText().size() > NAME_LIMIT)
			{
				new PanelInfo("Please, enter valid sequence name");
				return;
			}

			if(actor->ExistsAction(name->GetText()))
			{
				new PanelInfo("Sequence already exists\nEnter another sequence name");
				return;
			}

			if(!Sequence::IsValideFrameSequence(actor, listAnimation->GetText(), sequence->GetText()))
			{
				new PanelInfo("Frame sequence must contain only numbers and ';'\nFrame can't be smaller than zero, or greater or equal than total animation frames");
				return;
			}

			int fps = atol(editFps->GetText().c_str());
			if(fps < 1 || fps > 512)
			{
				new PanelInfo("Frame rate is invalid");
				return;
			}
			
			actor->AddSequence(name->GetText(), listAnimation->GetText(), sequence->GetText(), fps);
			actorProperty->AddAction(name->GetText());
			
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