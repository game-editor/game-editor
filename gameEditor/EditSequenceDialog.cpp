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


// EditSequenceDialog.cpp: implementation of the EditSequenceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "EditSequenceDialog.h"
#include "GameControl.h"


enum
{
	BT_ADD,
	BT_CLOSE
};

#define WIDTH	370
#define HEIGHT	100


EditSequenceDialog::EditSequenceDialog(Actor *actor, Sequence *seq)
	: Panel("EditSequenceDialog", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	sequence = seq;
	this->actor = actor;

	Text *text;
	Button *button;	
	int y;

	text = AddText("Edit Frame Sequence", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Frames: ", 10, text->Down() + 8);
	frames = AddEditBox(text->Right(), text->Top(), 300);

	text = AddText("Frame rate: ", 10, frames->Down() + 2);
	editFps = AddEditBox(text->Right(), text->Top(), 40);  editFps->SetNumeric(1, max(GameControl::Get()->getFrameRate(), sequence->getFrameRate()), 1);
	text = AddText("fps", editFps->Right() + 2, editFps->Top());
	editFps->SetText(sequence->getFrameRate());

	
	//Close
	y = DrawHLine(editFps->Down() + 2);
	button = AddButton("Ok", (WIDTH-135)/2, y, 0, 0, BT_ADD); SetConfirmButton(button);
	button = AddButton("Close", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);

	frames->SetText(sequence->GetFrameString());
}

EditSequenceDialog::~EditSequenceDialog()
{

}

void EditSequenceDialog::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADD:
		{
			if(!Sequence::IsValideFrameSequence(actor, sequence->getAnimationName(), frames->GetText()))
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

			sequence->SetFrameSequence(frames->GetText(), fps);
			delete this;
			
		}	
		break;
	case BT_CLOSE:
		{
			delete this;
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME