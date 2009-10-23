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


// SetTile.cpp: implementation of the SetTile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "SetTile.h"
#include "GameControl.h"
#include "ActorProperty.h"

//////////////////////////////////////////////////////////////////////////////
//Tool tip strings
#define TIP_TILEMATCH			"\
Use the tile matching to draw mazes and maps\n\
To make a animation to be used in tile match\n\
draw a little map with a 9x6 squares, like shown on the sample\n\
(the green lines are the walls and the whites are the floor)\n\
After draw, cut the numbered tiles and put side by side the\n\
frames 0, 1, 2, ... 14"

#define TIP_TILEMATCH_ANIMATION		"Select the animation with tiles"
//////////////////////////////////////////////////////////////////////////////

enum
{
	BT_OK,
	BT_CANCEL,
	LS_ANIMATION
};

#define WIDTH	440
#define HEIGHT	185

SetTile *SetTile::setTileDialog = NULL;

SetTile::SetTile(Actor *actor)
	: Panel("SetTile", (GameControl::Get()->Width() - WIDTH)/2, 
				          (GameControl::Get()->Height() - HEIGHT)/2,
						  WIDTH, HEIGHT)
{
	this->actor = actor;	
	SetToolTip(TIP_TILEMATCH);

	Text *text, *bigText;
	Button *button;	
	int y;

	//Title
	text = AddText("Set Actor Tile", CENTER_TEXT, 5);
	y = DrawHLine(text->Down() + 2);

	//Body
	text = AddText("Animation: ", 10, y);
	listAnimation = AddListPop(text->Right() + 2, text->Top(), 128, 0, LS_ANIMATION); listAnimation->SetToolTip(TIP_TILEMATCH_ANIMATION);

	text =    AddText("Note: The animation must contain\n15 squared frames that form an\nillustration as shown on the right", 10, listAnimation->Down() + 4);
	text =    AddText("      Use right shift to erase tiles", 10, text->Down() + 4);
	bigText = AddText("      Use right ctrl to hide tile cursor", 10, text->Down() + 4);
	text =    AddText("      Use ESC to end Draw Tile mode", 10, bigText->Down() + 4);

	AddSample();
	tileSample->getImage()->SetPos(bigText->Right() + 4, y);
	
	
	//End dialog
	y = DrawHLine(tileSample->Down() + 4);
	button = AddButton("Draw Tile", (WIDTH - 135)/2, y, 0, 0, BT_OK); SetConfirmButton(button);
	button = AddButton("Close", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); SetCancelButton(button);

	GameControl::Get()->PopulateAnimations(actor, listAnimation, false);

	if(actor->getTile()) listAnimation->SetItem(actor->getTile()->getAnimationName());
}

SetTile::~SetTile()
{
	delete tileSample;
}

void SetTile::Call(Actor *actor)
{
	if(!setTileDialog)
	{
		setTileDialog = new SetTile(actor);
	}
}

void SetTile::OnButton(Button *button, int buttonId)
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

			KrAction *anim = actor->getSpriteResource()->GetAction(animationName);

			if(!anim)
			{
				new PanelInfo("Please, select a valid animation");
				return;
			}

			if(anim->NumFrames() != 15)
			{
				new PanelInfo("The animation must contain 15 squared frames");
				return;
			}

			int tileSize = anim->getFrameWidth();
			if(tileSize != anim->getFrameHeight())
			{
				new PanelInfo("The animation must contain squared frames");
				return;
			}

			if(actor->getTile() && !actor->getTile()->IsSingle() && actor->getTile()->getTileSize() != tileSize)
			{
				char buf[256];
				sprintf(buf, "The actor already a tile with tile size equal to %ld\nThe new animation frame must contains same tile size", actor->getTile()->getTileSize());
				new PanelInfo(buf);
				return;
			}

			actor->DrawTile(animationName);
			ActorProperty::Destroy();

			setTileDialog = NULL;
			delete this;
		}
		break;
	}
}



void SetTile::AddSample()
{
	EditorDirectory editDir;

	tileSample = new Actor("makstileSample.bmp", this);
	tileSample->AddAnimation("makstileSample.bmp", "makstileSample.bmp", 1, 1, 0, 0);
}


#endif //#ifndef STAND_ALONE_GAME