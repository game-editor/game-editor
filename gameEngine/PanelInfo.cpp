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


// PanelInfo.cpp: implementation of the PanelInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "PanelInfo.h"
#include "GameControl.h"


//Use getRandomName() to avoid the wrong message when load
//a file using MRU and mising a file

PanelInfo *PanelInfo::lastPanel = NULL;
PanelInfo::PanelInfo(gedString text, gedString title, int align, bool bCreateButton)
: Panel(getRandomName(), (((GameControl::Get()->Width() - (Text::GetDimensions(text).Width()+60))/2) > 0)?(GameControl::Get()->Width() - (Text::GetDimensions(text).Width()+60))/2:0, 
		 (((GameControl::Get()->Height() - (Text::GetDimensions(text).Height()+65))/2) > 0)?(GameControl::Get()->Height() - (Text::GetDimensions(text).Height()+65))/2:0,
		 ((Text::GetDimensions(text).Width()+60) < GameControl::Get()->Width())?(Text::GetDimensions(text).Width()+60):GameControl::Get()->Width(),
		 ((Text::GetDimensions(text).Height()+65) < GameControl::Get()->Height())?(Text::GetDimensions(text).Height()+65):GameControl::Get()->Height())
{
	SetModal();
	lastPanel = this;

	Text *textBox;
	int y;

	while((Text::GetDimensions(text).Height()+65) > GameControl::Get()->Height())
	{
		text = text.substr(0, text.rfind('\n'));
	}

	textBox = AddText(title, CENTER_TEXT, 5);
	y = DrawHLine(textBox->Down() + 2);
	textBox = AddText(text, (align == ALIGN_CENTER)?CENTER_TEXT:10, y, align);

	y = DrawHLine(textBox->Down() + 4);
	if(bCreateButton)
	{
		Button *button = AddButton(" Ok ", (Width() - 60) / 2, y); 
		SetConfirmButton(button); 
		SetCancelButton(button);
	}

	lastLine = y;
}

PanelInfo::~PanelInfo()
{
	
}

void PanelInfo::OnButton(Button *button, int buttonId)
{
	if(GameControl::Get()->getStandAloneMode())
	{
		SDL_Event event;
		memset(&event, 0, sizeof(SDL_Event));
		event.quit.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}

	delete this;
}



#endif //#ifndef STAND_ALONE_GAME