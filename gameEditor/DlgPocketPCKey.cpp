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
