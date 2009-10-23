/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// DlgPocketPCKey.cpp: implementation of the DlgPocketPCKey class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "DlgPocketPCKey.h"
#include "GameControl.h"
#include "GameSettings.h"


enum
{
	BT_CANCEL,
	BT_SET
};

#define WIDTH	215
#define HEIGHT	90

DlgPocketPCKey::DlgPocketPCKey(SDLKey _keyFrom)
	: Panel("DlgPocketPCKey", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	SetModal();
	key = SDLK_UNKNOWN;
	keyFrom = _keyFrom;

	Text *text;
	Button *button;	
	int y;

	//Title
	text = AddText("PocketPC Key Mapper", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);
	
	//Body
	text = AddText(gedString("Redirect ") + SDL_GetKeyName(keyFrom), 10, y);	
	text = AddText("Press key to redirect: ", 10, text->Down()+1);	
	textKey = AddText("              ", text->Right(), text->Top());	
	
	//Close
	y = DrawHLine(textKey->Down() + 2);
	button = AddButton("Set Key", (WIDTH-135)/2, y, 0, 0, BT_SET);		
	button = AddButton("Close", button->Right() + 10, y, 0, 0, BT_CANCEL);		
}

DlgPocketPCKey::~DlgPocketPCKey()
{

}

void DlgPocketPCKey::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_SET:
		{
			GameControl::Get()->MapPocketPCKey(keyFrom, key);
			GameSettings::GetGameSettings()->SetPocketKeys();
		}
		break;
	}

	delete this;
}

void DlgPocketPCKey::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	this->key = key;
	textKey->SetText(SDL_GetKeyName(key));
}



#endif //#ifndef STAND_ALONE_GAME
