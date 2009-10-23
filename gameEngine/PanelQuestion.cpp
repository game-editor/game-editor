/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


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