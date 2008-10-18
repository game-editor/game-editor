// AddPathDialog.cpp: implementation of the AddPathDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "AddPathDialog.h"
#include "GameControl.h"
#include "gui.h"
#include "PathPoint.h"
#include "PathDialog.h"
#include "ActorProperty.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_ADDPATH			"\
Enter path name and path duration in frames\n\
Click in 'Draw' button to begin draw path\n\
Click in 'Close' button when finish path"

//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_ADDCLOSE	
};

#define WIDTH	180
#define HEIGHT	100

AddPathDialog *AddPathDialog::addPathDialog = NULL;

AddPathDialog::AddPathDialog()
	: Panel("AddPathDialog", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	Text *text;
	Button *button;	
	int y;

	SetToolTip(TIP_ADDPATH);

	text = AddText("Add Path", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	text = AddText("Name:   ", 10, text->Down() + 8);
	name = AddEditBox(text->Right(), text->Top());

	text = AddText("Frames: ", 10, name->Down() + 2);
	frames = AddEditBox(text->Right(), text->Top()); frames->SetNumeric(2, 16000, 1);
	
	
	//Close
	y = DrawHLine(frames->Down() + 2);
	button = AddButton("Draw", (WIDTH - 60)/2, y, 0, 0, BT_ADDCLOSE);  SetConfirmButton(button);

	bClose = false;
}

AddPathDialog::~AddPathDialog()
{

}

void AddPathDialog::Call()
{
	if(!addPathDialog)
	{
		addPathDialog = new AddPathDialog();
	}
}

void AddPathDialog::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_ADDCLOSE:
		{
			if(bClose)
			{
				if(name->GetText().size() <= 0 || name->GetText().size() > NAME_LIMIT)
				{
					new PanelInfo("Please, enter valid path name");
					return;
				}

				if(GameControl::Get()->ExistsPath(name->GetText()))
				{
					new PanelInfo("Path already exists\nEnter another path name");
					return;
				}

				int nFrames = atol(frames->GetText().c_str());
				if(nFrames < 2 || nFrames > 16000)
				{
					new PanelInfo("Please, enter valid path lenght");
					return;
				}

				if(!GameControl::Get()->GetPathRoot() || GameControl::Get()->GetPathRoot()->TotalKeys() < 2)
				{
					new PanelInfo("You must put at least two points on screen to make your path");
					return;
				}
				
				
				GameControl::Get()->AddPath(name->GetText(), nFrames);
				GameControl::Get()->SetPathMode(false);

				//Notify path dialog
				PathDialog *pathDialog = PathDialog::getPathDialog();
				if(pathDialog)
					pathDialog->SetPaths();

				//Notify actor dialog
				ActorProperty::SetPaths();

				GameControl::Get()->GetAxis()->getImage()->Invalidate();
				addPathDialog = NULL;
				delete this;
			}
			else
			{
				//Add
				button->SetText("Close");
				bClose = true;

				GameControl::Get()->SetPathMode(true);
			}
		}
	}
}


#endif //STAND_ALONE_GAME