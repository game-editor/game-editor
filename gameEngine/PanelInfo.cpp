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