// LoadSave.cpp: implementation of the LoadSave class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "LoadSave.h"
#include "GameControl.h"


//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_LOAD			"\
Use this panel to load a previously saved game\n\
All games saved in early versions are supported"

#define TIP_LOAD_FILE		"Select the game file to be loaded"




#define TIP_MERGE			"\
Merge current game with a newly loaded game.\n\
If loaded game has actor with same name as\n\
in current game, the results can be unsatisfactory"

#define TIP_MERGE_FILE		"Select the game file to be loaded"




#define TIP_SAVE			"\
Save current game and move all used animations,\n\
fonts and sounds to data game directory\n\
(Animations, fonts and sounds loaded in scripts must be copied manually)"

#define TIP_SAVE_FILE		"Select the directory where the game will be saved"

//////////////////////////////////////////////////////////////////////////////


#define WIDTH	340
#define HEIGHT	83


LoadSave::LoadSave(bool bExtendHeight)
	: Panel("LoadSave", (GameControl::Get()->Width() - WIDTH)/2, 
	(GameControl::Get()->Height() - (bExtendHeight?HEIGHT+25:HEIGHT))/2,
						  WIDTH, (bExtendHeight?HEIGHT+25:HEIGHT))
{
	
}

LoadSave::~LoadSave()
{

}

bool LoadSave::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(list == dir && command == SAVE_DIALOG)
	{
		name->SetText(text);
	}

	return true;
}


void LoadSave::OnButton(Button *button, int buttonId)
{
	SDL_ClearError();

	bOk = false;

	switch(buttonId)
	{
	case LOADSAVE_OK:
		{
			if(command == SAVE_DIALOG)
			{
				if(name->GetText().size() <= 0 || name->GetText().size() > PATH_LIMIT)
				{
					new PanelInfo("Please, enter valid file name");
					return;
				}
			}
			else if(command == LOAD_DIALOG)
			{
				if(dir->GetSelectedIndex() == -1)
				{
					new PanelInfo("Please, select a file");
					return;
				}
			}
		}	
		break;
	}

	bOk = true;
}


void LoadSave::Init(int command, gedString title, gedString confirmButtonText)
{
	SetModal();
	this->command = command;

	Text *text;
	Button *button;		
	int y;	

	text = AddText(title, CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	if(command == SAVE_DIALOG)
	{
		SetToolTip(TIP_SAVE);

		text = AddText("File: ", 10, y);
		name = AddEditBox(text->Right(), text->Top(), 200);
		dir = AddListDirectory(name->Right() + 2, y, 80, 0, 0, "select");  dir->SetToolTip(TIP_SAVE_FILE);
		dir->SetSave();
		y = name->Down();
	}
	else if(command == LOAD_DIALOG)
	{
		SetToolTip(TIP_LOAD);

		text = AddText("File: ", 40, y);
		dir = AddListDirectory(text->Right(), y, 200, 0, 0); dir->SetToolTip(TIP_LOAD_FILE);
		y = dir->Down();
	}	
	else if(command == MERGE_DIALOG)
	{
		SetToolTip(TIP_MERGE);

		text = AddText("File: ", 40, y);
		dir = AddListDirectory(text->Right(), y, 200, 0, 0); dir->SetToolTip(TIP_MERGE_FILE);
		y = dir->Down();

		command = LOAD_DIALOG;
	}

	
	//Close
	y = DrawHLine(Height() - 40);

	button = AddButton(confirmButtonText, (WIDTH-135)/2, y, 0, 0, LOADSAVE_OK); SetConfirmButton(button);
	button = AddButton("Cancel", button->Right()+8, y, 0, 0, LOADSAVE_CLOSE); SetCancelButton(button);

	chdir(GameControl::Get()->getGamePath().c_str());
}

void LoadSave::SetFilter(const gedString &filter)
{
	dir->SetFilter(filter);
}

void LoadSave::AddFilter(const gedString &filter)
{
	dir->AddFilter(filter);
}

void LoadSave::SetText(gedString str)
{
	name->SetText(str);
	dir->SetSaveName(str);
}

gedString LoadSave::GetText()
{
	return name->GetText();
}

gedString LoadSave::GetDirectory()
{
	return dir->GetText();
}

#endif //#ifndef STAND_ALONE_GAME