// SetTileSingle.cpp: implementation of the SetTileSingle class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "SetTileSingle.h"
#include "GameControl.h"
#include "ActorProperty.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_TILESINGLE			"\
Use the single tile to draw a more complex actor\n\
Each tile is a animation of selected animation"

#define TIP_TILESINGLE_ANIMATION		"Select the animation with tiles"

#define TIP_TILESINGLE_RANDOM			"\
Select yes to draw the tiles randomly"
//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_OK,
	BT_CANCEL,
	LS_ANIMATION,
	LS_RANDOM
};

#define WIDTH	350
#define HEIGHT	175

SetTileSingle *SetTileSingle::setTileDialog = NULL;

SetTileSingle::SetTileSingle(Actor *actor)
	: Panel("SetTileSingle", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	this->actor = actor;
	SetToolTip(TIP_TILESINGLE);

	Text *text, *bigText;
	Button *button;	
	int y;

	//Title
	text = AddText("Set Actor Tile", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText(" Animation: ", 10, y);
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_TILESINGLE_ANIMATION);
	text = AddText("Radom tile: ", 10, listAnimation->Down() + 2);
	listRandom = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_RANDOM); listRandom->SetToolTip(TIP_TILESINGLE_RANDOM);

	text =    AddText("Note: Use right and left keys to change animation frame", 10, listRandom->Down() + 4);
	text =    AddText("      Use right shift to erase tiles", 10, text->Down() + 4);
	bigText = AddText("      Use right ctrl to hide tile cursor", 10, text->Down() + 4);
	text =    AddText("      Use ESC to end Draw Tile mode", 10, bigText->Down() + 4);
	
	
	//End dialog
	y = DrawHLine(text->Down() + 4);
	button = AddButton("Draw Tile", (WIDTH - 135)/2, y, 0, 0, BT_OK); SetConfirmButton(button);
	button = AddButton("Close", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); SetCancelButton(button);

	GameControl::Get()->PopulateAnimations(actor, listAnimation, false);

	listRandom->AddText("Yes");
	listRandom->AddText("No");
	listRandom->SetItem("No");

	if(actor->getTile()) listAnimation->SetItem(actor->getTile()->getAnimationName());
}

SetTileSingle::~SetTileSingle()
{

}

void SetTileSingle::Call(Actor *actor)
{
	if(!setTileDialog)
	{
		setTileDialog = new SetTileSingle(actor);
	}
}

void SetTileSingle::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_CANCEL:
		{
			setTileDialog = NULL;
			delete this;
		}
		break;
	case BT_OK:
		{
			if(listAnimation->GetSelectedIndex() == -1)
			{
				new PanelInfo("Please, select a animation");
				return;
			}

			gedString animationName = listAnimation->GetText();
			bool bRandom = false;
			if(listRandom->GetText() == "Yes") bRandom = true;
			
			actor->DrawTile(animationName, true, bRandom);
			ActorProperty::Destroy();

			setTileDialog = NULL;
			delete this;
		}
		break;
	}
}


#endif //#ifndef STAND_ALONE_GAME