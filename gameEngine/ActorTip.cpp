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


// ActorTip.cpp: implementation of the ActorTip class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "ActorTip.h"
#include "GameControl.h"
#include "ActivationEventsCanvas.h"

ActorTip *ActorTip::actorTip = NULL;
Actor *ActorTip::lastActor = NULL;

ActorTip::ActorTip(const gedString *tip)
	: Actor(ACTOR_TIP_NAME, NULL, CANVAS, Text::GetDimensions(*tip).Width() + 6, Text::GetDimensions(*tip).Height() + 4, true)
{
	getImage()->SetZDepth(CURSOR_DEPTH - 1);

	int x = GameControl::Get()->getMouseX() + 20, 
		y = GameControl::Get()->getMouseY() + 20, 
		width = Width(), 
		height = Height();

	if(x + width > GameControl::Get()->Width())
		x -= x + width - GameControl::Get()->Width();

	if(y + height > GameControl::Get()->Height())
		y -= y + height - GameControl::Get()->Height();

	getImage()->SetPos(x, y);

	
	//Draw
	KrRGBA *pixels = getCanvasResource()->Pixels(), colorBack, colorBlack;

	colorBack.c.red		= 255;
	colorBack.c.green	= 255;
	colorBack.c.blue	= 225;
	colorBack.c.alpha	= 255;

	colorBlack.Set(0, 0, 0);

	//Fill background
	int i;
	for(i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			if(i == 0 || j == 0 || i == width-1 || j == height - 1)
				pixels[ j*width + i ] = colorBlack;
			else
				pixels[ j*width + i ] = colorBack;
		}
	}

	//Text
	AddText(*tip, 3, 2);

	SetTransparency(.1);
}

ActorTip::~ActorTip()
{

}

void ActorTip::Create(gedString *tip)
{
	delete actorTip;
	actorTip = new ActorTip(tip);
	lastActor = NULL;
}

void ActorTip::Create(Actor *actor)
{
	//Don't repeat tip in a panel
	double aspectRatio = actor->Width()/(double)actor->Height();

	if(actor != lastActor || aspectRatio > 2 || actor->EditMode() || actor == ActivationEventsCanvas::Get())
	{
		const gedString *tip = actor->OnActorTip();
		if(tip && tip->length())
		{
			delete actorTip;
	
			actorTip = new ActorTip(tip);
			lastActor = actor;
		}
	}	
}

void ActorTip::Release()
{
	if(actorTip)
	{
		delete actorTip;
		actorTip = NULL;
	}
}

#endif //#ifndef STAND_ALONE_GAME