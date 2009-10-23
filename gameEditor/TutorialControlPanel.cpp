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


// TutorialControlPanel.cpp: implementation of the TutorialControlPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "TutorialControlPanel.h"
#include "GameControl.h"
#include "Tutorial.h"
#include "MainPanel.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_TUTORIAL_RESTART			"\
Play the current tutorial from beginning"

#define TIP_TUTORIAL_CONTINUE			"\
Continue the current tutorial"

#define TIP_TUTORIAL_STOP			"\
Close the tutorial"



//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_RESTART,
	BT_CONTINUE,
	BT_STOP,
	BT_REC
};



#define WIDTH	250
#define HEIGHT	78

TutorialPlayControl *TutorialPlayControl::tutorialPlayControl = NULL;

TutorialPlayControl::TutorialPlayControl(bool bTutorialEnd)
	: Panel("TutorialPlayControl", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	tutorialPlayControl = this;

	//Store current key focus
	keyFocus = KrEventManager::Instance()->GetKeyFocus();

	SetModal();

	Text *text;
	Button *button;
	int y;

	//Title
	text = AddText("Tutorial", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText(bTutorialEnd?"End of tutorial":"The current tutorial is paused", CENTER_TEXT, y, ALIGN_CENTER);
			
	//Close
	y = DrawHLine(text->Down() + 5);
	
	if(bTutorialEnd)
	{
		button = AddButton("Restart", 58, y, 0, 0, BT_RESTART); button->SetToolTip(TIP_TUTORIAL_RESTART);	
		button = AddButton("Close", button->Right() + 10, y, 0, 0, BT_STOP); button->SetToolTip(TIP_TUTORIAL_STOP);

		Tutorial::Get()->RemovePauseDelay();
	}
	else
	{
		button = AddButton("Restart", 15, y, 0, 0, BT_RESTART); button->SetToolTip(TIP_TUTORIAL_RESTART);	
		button = AddButton("Continue", button->Right() + 10, y, 0, 0, BT_CONTINUE); button->SetToolTip(TIP_TUTORIAL_CONTINUE);	
		button = AddButton("Stop", button->Right() + 10, y, 0, 0, BT_STOP); button->SetToolTip(TIP_TUTORIAL_STOP);
	}

#ifdef _DEBUG
	AddButton("Rec", 10, 10, 0, 0, BT_REC);
#endif

	if(GameControl::Get()->getGameMode() && !GameControl::Get()->getShowMouse())
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}

TutorialPlayControl::~TutorialPlayControl()
{
	tutorialPlayControl = NULL;

	if(GameControl::Get()->getGameMode() && !GameControl::Get()->getShowMouse())
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void TutorialPlayControl::Call(bool bTutorialEnd)
{
	if(!tutorialPlayControl)
	{
		new TutorialPlayControl(bTutorialEnd);
	}
}

void TutorialPlayControl::OnButton(Button *button, int buttonId)
{
	//Restore current key focus
	KrEventManager::Instance()->ChangeKeyFocus(keyFocus);

	switch(buttonId)
	{
	case BT_RESTART:
		Tutorial::Get()->Play();
		break;

#ifdef _DEBUG
	case BT_REC:
		Tutorial::Get()->Rec();
		break;
#endif

	case BT_CONTINUE:
		Tutorial::Get()->Pause(PAUSED_BY_USER);
		Tutorial::Get()->ResetPauseDelay();
		break;

	case BT_STOP:
		{
			GameControl::Get()->SetGameMode(false);	
			Tutorial::Destroy();			

			gedString aux(GameControl::getEditorPath());
			chdir(aux.c_str());
			MainPanel::Get()->FileSetup();
		}
		break;
	}	

	PostMessage(this, DELETE_ME, DELETE_ME);
}


#endif //STAND_ALONE_GAME


