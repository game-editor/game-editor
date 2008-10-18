// TutorialTextEditor.cpp: implementation of the TutorialTextEditor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "TutorialTextEditor.h"
#include "GameControl.h"
#include "Tutorial.h"
#include "TutorialTextPanel.h"
#include "TutorialPaint.h"


enum
{
	BT_CLOSE,
	BT_ADD
};

#define WIDTH	520
#define HEIGHT	400

TutorialTextEditor *TutorialTextEditor::tutorialTextEditor = NULL;

TutorialTextEditor::TutorialTextEditor()
	: Panel("TutorialTextEditor", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	TutorialPaint::Hide();
	TutorialTextPanel::Hide();

	//if(TutorialTextPanel::Count() == 0) Tutorial::Get()->Pause(PAUSED_BY_EXPLANATION_EDITOR);

	tutorialTextEditor = this;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("Explanation Text", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	edit = AddEditBox(10, y, WIDTH-20, HEIGHT-65, true);
	
	//Close
	y = DrawHLine(edit->Down() + 3);

	button = AddButton("Add", (WIDTH-135)/2, y, 0, 0, BT_ADD); 
	button = AddButton("Close", button->Right()+8, y, 0, 0, BT_CLOSE); SetCancelButton(button);	
}


TutorialTextEditor::~TutorialTextEditor()
{
	TutorialPaint::Show();
	TutorialTextPanel::Show();
	tutorialTextEditor = NULL;
}

void TutorialTextEditor::Call()
{
	if(!tutorialTextEditor)
	{
		new TutorialTextEditor();
	}
}

void TutorialTextEditor::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CLOSE:
		{
			if(TutorialTextPanel::Count() == 0) Tutorial::Get()->Pause(PAUSED_BY_EXPLANATION_EDITOR);
			delete this;
		}
		break;
	case BT_ADD:
		{
			gedString explanationText(edit->GetText(ALL_LINES));

			while(explanationText.length() > 1 && explanationText[explanationText.length() - 1] == '\n')
			{
				explanationText = explanationText.substr(0, explanationText.length() - 1);
			}


			if(explanationText.length() > 0)
			{
				new TutorialTextPanel(explanationText);
			}

			delete this;			
		}
		break;
	}
}



#endif //STAND_ALONE_GAME


