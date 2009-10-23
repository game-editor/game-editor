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


// Panel.cpp: implementation of the Panel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#pragma warning(disable:4786)

#include "Panel.h"
#include "GameControl.h"

#if !defined(STAND_ALONE_GAME)
#	include "../gameEditor/Tutorial.h"
#endif

int SDL_GE_WaitEvent(SDL_Event *event);



long Panel::depth = GUI_DEPTH;

ListPanel Panel::listPanel;
ListPanel Panel::listFocus;
Panel *Panel::actualFocus = NULL;

/*KrRGBA Panel::colorBack		(239, 166, 61, 255); //Orange
KrRGBA Panel::colorUp		(255, 202, 117,255);
KrRGBA Panel::colorDown		(176, 99, 36, 255);
KrRGBA Panel::colorHighLight(255, 174, 52, 255);/**/

//Silver

KrRGBA Panel::colorUp		(255, 247, 220, 255);
KrRGBA Panel::colorDown		(132, 124, 110, 255);
KrRGBA Panel::colorHighLight(239, 166,  61, 200);
KrRGBA Panel::colorBack		(Panel::colorUp);/**/



Panel::Panel(const gedString spriteName, int x, int y, int width, int height, Actor *parent, bool _bInvertBevel, bool bDrag, bool bPanel, bool _bShadows)
: Actor(spriteName, parent, CANVAS, _bShadows?(width + SHADOW_THICKNESS):width, _bShadows?(height + SHADOW_THICKNESS):height, bPanel)
{
	flags.Clear(FLAG_ENABLECOLLISIONDETECTION);
	this->bDrag = bDrag;
	confirmButton = cancelButton = NULL;
	firstHorizontalLine = -1;
	result = -1;
	flags.Set(FLAG_HASSHADOWS, _bShadows);
	bInvertBevel = _bInvertBevel;

	getImage()->SetPos(x,y);
	getImage()->SetZDepth(++depth);



	Draw();
	SetTransparency(.1);

	panelColor = getImage()->CTransform();

	listPanel.PushBack(this);
	if(flags.IsSet(FLAG_HASSHADOWS)) 
	{
		//Only get focus if have shadows
		//Avoid get focus for slides...
		GetFocus(this);
	}
	
	//Don't call SetFocus here, there is no edit box yet
}

void Panel::Draw() 
{
	//Draw panel
	KrRGBA *pixels = getCanvasResource()->Pixels();
	int width = Width(), height = Height();

	if(flags.IsSet(FLAG_HASSHADOWS))
	{
		  width -= SHADOW_THICKNESS;
		  height -= SHADOW_THICKNESS;
	}

	if(bInvertBevel)
	{
		KrRGBA aux;
		aux = colorUp;
		colorUp = colorDown;
		colorDown = aux;
	}
		

	//Fill background
	int i, j;
	
	for(j = 0; j < height; j++)
	{
		KrRGBA color(SlowCanvas::MixColors(Panel::getColorUp(), Panel::getColorDown(), j/(double)height));
		for(i = 0; i < width; i++)
		{
			pixels[ j*Width() + i ] = color;
		}
	}

	
	//Glass effect
	if(height < 55)
	{
		KrRGBA white(255, 255, 255);
		int hm = height/2;
		for(j = 0; j < hm; j++)
		{
			for(i = 0; i < width; i++)
			{
				pixels[ j*Width() + i ] = SlowCanvas::MixColors(pixels[ j*Width() + i ], white, .8*j/(double)hm);
			}
		}
	}

	KrRGBA colorBorder(104, 104, 104);	

		//Horizontal lines
		for(i = 0; i < width; i++)
		{
			pixels[  i ] = colorBorder;	
			pixels[ (height-1)*Width() + i ] = colorBorder;
		}
		
		//Vertical lines
		for(j = 0; j < height; j++)
		{
			pixels[ j*Width() ] = colorBorder;		
			pixels[ j*Width() + (width-1) ] = colorBorder;
		}

	//Corner pixels
	pixels[ 1 + 1*Width() ] = colorBorder;
	pixels[ (width - 2) + 1*Width() ] = colorBorder;
	pixels[ (width - 2) + (height - 2)*Width() ] = colorBorder;
	pixels[ 1 + (height - 2)*Width() ] = colorBorder;


	

	//Restore colors
	if(bInvertBevel)
	{
		KrRGBA aux;
		aux = colorUp;
		colorUp = colorDown;
		colorDown = aux;
	}

	//Make trasparent corners
	KrRGBA transp(0, 0, 0, 0);

	pixels[ 0 + 0*Width() ] = transp;
	pixels[ 1 + 0*Width() ] = transp;
	pixels[ 0 + 1*Width() ] = transp;

	pixels[ (width - 1) + 0*Width() ] = transp;
	pixels[ (width - 2) + 0*Width() ] = transp;
	pixels[ (width - 1) + 1*Width() ] = transp;

	if(!flags.IsSet(FLAG_HASSHADOWS))
	{
		pixels[ (width - 1) + (height - 1)*Width() ] = transp;
		pixels[ (width - 2) + (height - 1)*Width() ] = transp;
		pixels[ (width - 1) + (height - 2)*Width() ] = transp;
	}

	pixels[ 0 + (height - 1)*Width() ] = transp;
	pixels[ 1 + (height - 1)*Width() ] = transp;
	pixels[ 0 + (height - 2)*Width() ] = transp;

	if(flags.IsSet(FLAG_HASSHADOWS))
	{
		KrRGBA colorShadow;	
		double shadowFactor;
						
		//Orange
		colorShadow.c.red	= 0;
		colorShadow.c.green	= 0;
		colorShadow.c.blue	= 0;
		colorShadow.c.alpha	= SHADOW_MAX;

		//Corner
		pixels[ (width - 1) + (height - 1)*Width() ] = colorShadow;
		pixels[ (width - 2) + (height - 1)*Width() ] = colorShadow;
		pixels[ (width - 1) + (height - 2)*Width() ] = colorShadow;
		
		//Vertical shadow
		for(i = width; i < Width(); i++)
		{
			for(int j = SHADOW_THICKNESS; j < Height(); j++)
			{
				shadowFactor = 1.0 - (i - width)/(double)SHADOW_THICKNESS;
				shadowFactor *= (j >= height)?(1.0 - (j - height)/(double)SHADOW_THICKNESS):1.0;
				shadowFactor *= (j - SHADOW_THICKNESS < SHADOW_FADE)?(j - SHADOW_THICKNESS)/(double)SHADOW_FADE:1.0;

				colorShadow.c.alpha	= SHADOW_MAX*shadowFactor;

				pixels[ j*Width() + i ] = colorShadow;
			}
		}
		
		//Horizontal shadow
		for(i = SHADOW_THICKNESS; i < Width() - SHADOW_THICKNESS; i++)
		{
			for(int j = height; j < Height(); j++)
			{
				shadowFactor = 1.0 - (j - height)/(double)SHADOW_THICKNESS;
				shadowFactor *= (i - SHADOW_THICKNESS < SHADOW_FADE)?(i - SHADOW_THICKNESS)/(double)SHADOW_FADE:1.0;

				colorShadow.c.alpha	= SHADOW_MAX*shadowFactor;

				pixels[ j*Width() + i ] = colorShadow;
			}
		}
	}

	

	getCanvasResource()->Refresh();
}

Panel::~Panel()
{
	ReleaseFocus(this);
	listPanel.FindAndDelete(this);

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	Actor::~Actor();
#endif
}

int Panel::Wait()
{
	//Modal loop
	/*SDL_Event event;

	while(SDL_GE_WaitEvent(&event))
	{
		if(!GameControl::Get()->GameTick(event) || result == OK_BUTTON || result == CANCEL_BUTTON)
			break;
	}*/

	SDL_Event event;
	int mouseX1 = -1, mouseY1 = -1;

	

	if(Tutorial::InPlayMode())
	{
		GameControl::EnableAddFrameRate();
	}

	while(SDL_GE_WaitEvent(&event))
	{
		if(event.type == SDL_MOUSEMOTION)
		{
			//Avoid mistake SDL_MOUSEMOTION events
			if(event.motion.x != mouseX1 || event.motion.y != mouseY1)
			{
				mouseX1 = event.motion.x;
				mouseY1 = event.motion.y;
			}
			else
				continue;
		}

		
#if !defined(STAND_ALONE_GAME)
		if(Tutorial::IsOff(event))
		{
			if(!GameControl::Get()->GameTick(event) || result == OK_BUTTON || result == CANCEL_BUTTON)
				break;
		}
		else if(Tutorial::InRecMode())
		{
			Tutorial::Get()->Store(event);

			if(!GameControl::Get()->GameTick(event) || result == OK_BUTTON || result == CANCEL_BUTTON)
				break;
		}
		else if(Tutorial::InPlayMode())
		{
			GameControl::EnableAddFrameRate();

			if(!Tutorial::Get()->ExecuteMoves(event))
				break;
			
			if(Tutorial::Get()->CanExecute(event))
			{
				if(!GameControl::Get()->GameTick(event) || result == OK_BUTTON || result == CANCEL_BUTTON)
					break;
			}
		}
#else
		if(!GameControl::Get()->GameTick(event) || result == OK_BUTTON || result == CANCEL_BUTTON)
			break;
#endif
	}

	return result;
}

bool Panel::OnMouseButtonDown(int x, int y, Uint8 button)
{
	if(getImage()->ZDepth() < depth)
	{
		getImage()->SetZDepth(++depth);
	}	

	GetFocus(this);

	return bDrag;
}

const stCursor *Panel::OnGetCursor()
{
	/*if(bDrag) //maks: no big hand cursor
	{
		cursor.imgName	= "makscursorMao.bmp";
		cursor.nFramesH	= 1;
		cursor.nFramesV	= 1;
		cursor.hotSpotX = 7;
		cursor.hotSpotY = 0;
		cursor.bAnimate = false;
		
		return &cursor;
	}
	else*/
	{
		return NULL;
	}
}

int Panel::DrawHLine(int y, int xi, int w)
{
	int height = getCanvasResource()->Height(),
		 width = getCanvasResource()->Width();

	
	
	if(y < 0 || y >= height - 4)
		return y;

	if(w <= 0) w = width;

	if(flags.IsSet(FLAG_HASSHADOWS))
	{
		w -= SHADOW_THICKNESS;		
	}

	if(firstHorizontalLine == -1) firstHorizontalLine = y;

	KrRGBA *pixels = getCanvasResource()->Pixels();

	for(int i = xi; i < w; i++)
	{
		if(i < width - 1) pixels[ y*width + i ] = colorDown;
		
		pixels[ (y+1)*width + i ] = colorDown;
		pixels[ (y+2)*width + i ] = colorUp;

		if(i > 0) pixels[ (y+3)*width + i ] = colorUp;
	}

	//Call set focus here: solve the Tutorial key loss
	SetFocus(0);

	getCanvasResource()->Refresh();
	return y + 8;
}


void Panel::SetFocus(int i)
{
	//Set focus to edit box i

	int n = 0;
	ListObjectIterator it(listEditBox);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(n == i)
		{
			if(((EditBox *)(it.Current()))->SetFocus())
			{
				break;
			}
		}
		n++;
	}
}

void Panel::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(GameControl::Get()->InModal(this) && !repeatLoopCount)
	{
		if(key == SDLK_RETURN && confirmButton) SendButtonDownMessage(confirmButton);
		else if(key == SDLK_ESCAPE && cancelButton) SendButtonDownMessage(cancelButton);
	}
}

void Panel::SendButtonDownMessage(Button *button)
{
	int x = button->getImage()->CompositeXForm().x.ToInt() + 4,
		y = button->getImage()->CompositeXForm().y.ToInt() + 4;
	
	SDL_Event event;
	
	memset(&event, 0, sizeof(SDL_Event));
	event.button.type = SDL_MOUSEBUTTONDOWN;
	event.button.button = 1;
	event.button.state = SDL_PRESSED;
	event.button.x = x;
	event.button.y = y;		
	SDL_PushEvent(&event);
	
	memset(&event, 0, sizeof(SDL_Event));
	event.button.type = SDL_MOUSEBUTTONUP;
	event.button.button = 1;
	event.button.state = SDL_RELEASED;
	event.button.x = x;
	event.button.y = y;		
	SDL_PushEvent(&event);	
}


void Panel::SetConfirmButton(Button *button)
{
	confirmButton = button;
}

void Panel::SetCancelButton(Button *button)
{
	cancelButton = button;
}



void Panel::GetFocus(Panel *panel)
{	
	if(actualFocus == panel) return;
	listFocus.FindAndDelete(panel); //Only one time in list

	panel->RestoreColor();

	for(int i = 0; i < listPanel.Count(); i++)
	{
		Panel *p = listPanel[i];
		if(p && p != panel && !Actor::IsChild(panel, p))
			p->SetColorFocus();
	}

	actualFocus = panel;
	listFocus.PushBack(panel);
}

void Panel::ReleaseFocus(Panel *panel)
{
	listFocus.FindAndDelete(panel);

	if(listFocus.Count() && actualFocus == panel)
	{
		//Switch focus to other panel
		Panel *newFocus = listFocus.Back();
		listFocus.FindAndDelete(newFocus); //Re added in GetFocus

		GetFocus(newFocus);
	}
}

void Panel::SetColorFocus()
{
	KrColorTransform color(getImage()->CTransform());

	//color.Darken(32);
	color.Brighten(40);
	color.SetAlpha(205);

	getImage()->SetColor(color);
}

void Panel::RestoreColor()
{
	getImage()->SetColor(panelColor);
}


#endif //#ifndef STAND_ALONE_GAME