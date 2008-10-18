// PanelQuestion.cpp: implementation of the PanelQuestion class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "PanelQuestion.h"
#include "GameControl.h"


PanelQuestion::PanelQuestion(gedString text, gedString title, gedString confirmButton, gedString cancelButton, int align)
: PanelInfo(text, title, align, false)
{
	Button *button1, *button2 = NULL;

	button1 = AddButton(confirmButton, 0, lastLine, 0, 0, OK_BUTTON); SetConfirmButton(button1);
	if(cancelButton != NO_BUTTON)
	{
		button2 = AddButton(cancelButton, button1->Right()+8, lastLine, 0, 0, CANCEL_BUTTON); SetCancelButton(button2);
	}

	//Center
	int widthButtons = button1->Width();
	if(button2) widthButtons += button2->Width() + 8;

	button1->SetPos((Width() - widthButtons)/2, lastLine);
	if(button2) button2->SetPos(button1->Right()+8, lastLine);
}

PanelQuestion::~PanelQuestion()
{

}

void PanelQuestion::OnButton(Button *button, int buttonId)
{
	result = buttonId;
}




#endif //#ifndef STAND_ALONE_GAME