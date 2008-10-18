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


