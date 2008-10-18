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