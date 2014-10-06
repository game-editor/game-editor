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


// Button.cpp: implementation of the Button class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "Button.h"
#include "GameControl.h"
#include <math.h>

extern "C" {
#include "../kyra/gui/colorscheme.h"
}

bool Button::bHaveButtonPressed = false;

static void GetButtonDimensions(gedString text, int &width, int &height, gedString& fontName)
{
	int textWidth = 0, textHeight = 0;

	//if(!text.empty())
	{
		gedString textButton(gedString("  ") + text + "  ");	
		
		Text::GetDimensions(textButton, textWidth, textHeight, fontName);
	}
	
	if(!width)
		width = max(textWidth, 62);
		
	if(!height)
		height = max(textHeight, 20);
}

static int GetButtonWidth(gedString text, int &width, int &height, gedString& fontName)
{
	GetButtonDimensions(text, width, height, fontName);

	return width;
}

static int GetButtonHeight(gedString text, int &width, int &height, gedString& fontName)
{
	GetButtonDimensions(text, width, height, fontName);

	return height;
}

Button::Button(gedString text, 
			   int x, int y,
			   int width, int height,
			   int buttonId,
			   Actor *parent,
			   int red, int green, int blue,
			   const gedString spriteName,
			   gedString fontName,
			   bool _bDrawArrows)
 : Actor(getRandomName(), parent, CANVAS, 
			   width?width:GetButtonWidth(text, width, height, fontName), 
			   height?height:GetButtonHeight(text, width, height, fontName))
{
	flags.Clear(FLAG_ENABLECOLLISIONDETECTION);
	bDrawArrows = _bDrawArrows;

	
	EditorDirectory editDir;	
	AddAnimation(spriteName, spriteName, 3, 1, 0, 0);
	
	offSetText = 0;
	
	//KrRect rect;
	//getImage()->QueryBoundingBox(&rect, 0);

	//Button text
	textBox = NULL;
	fontResource = GameControl::Get()->GetFont(fontName.c_str());
	if(fontResource)
	{
		KrScheme scheme(fontResource);
		text = gedString("  ") + text + "  ";
		int textWidth  = fontResource->FontWidth(text.c_str());
		int textHeight = fontResource->FontHeight();

		/*if(!width) width = textWidth;		
		if(!height) height = textHeight;*/

		/*if(!width)
			width = max(textWidth, rect.Width());
		
		if(!height)
			height = max(textHeight, rect.Height());*/

		//sx = width/(float)rect.Width();
		//sy = height/(float)rect.Height();

		px = (width - textWidth)/2 + offSetText - (bDrawArrows?3:0);
		py = (height - textHeight)/2;

		textBox = new KrTextBox( fontResource, 
							 width,
							 fontResource->FontHeight(), 
							 0);


		
		textBox->SetText(text, 0);
		KrColorTransform color;
		ColorScheme* cs = get_color_scheme();
		// sky change color of font
		color.Set(0, cs->button_text_r, 0, cs->button_text_g, 0, cs->button_text_b, 255); // red 255 should be red
		textBox->SetColor(color);

		//Round to avoid text cut (don't works for "Game Mode" text)
		/*double r = 1000;
		sx = floor(sx*r)/r;
		sy = floor(sy*r)/r;

		double  isx = 1/sx, isy = 1/sy;
		//isx = floor(isx*r)/r;
		//isy = floor(isy*r)/r;

		textBox->SetScale(isx, isy); //Do not scale text*/
		

		textBox->SetNodeId((int)textBox);
		engine->Tree()->AddNode( getImage(), textBox );
		textBox->SetPos(px, py);

		fontResource->refCount++;
	}	 

	//Position
	//getImage()->SetScale(sx, sy);
	getImage()->SetPos(x, y);
	flags.Clear(FLAG_ENABLECOLLISIONDETECTION);
	framesToAnim = DISABLE_ANIMATION;

	this->buttonId = buttonId;
	buttonDown = false;
	bStayPressed = false;
	bEnable = true;

	Draw(false);
}

Button::~Button()
{
	GLASSERT(textBox != NULL && fontResource != NULL);

	if(engine->Vault()->ExistsResource(fontResource))
	{
		fontResource->refCount--;
		if(fontResource->refCount == 0)
		{
			engine->Vault()->RemoveSpriteResource(fontResource);
		}		
	}	

	if(engine->Tree()->FindNodeById((int)textBox))
	{
		engine->Tree()->DeleteNode(textBox);
	}
}

void Button::Draw(bool bHighLight)
{
	//Draw panel
	KrRGBA *pixels = getCanvasResource()->Pixels(), colorUp, colorDown, colorArrow;
	KrRGBA transp(0, 0, 0, 0);
	ColorScheme * cs = get_color_scheme();
	int width = Width(), height = Height();

	if(width <= 4 && height <= 4) return;

	colorUp = Panel::getColorUp();
	colorDown = Panel::getColorDown();
	colorArrow = colorDown;


	if(buttonDown)
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
		KrRGBA color;
		
		if(bHighLight)
		{
			color = (SlowCanvas::MixColors(Panel::getColorUp(), Panel::getColorHighLight(), j/(double)height));
		}
		else
		{
			color = (SlowCanvas::MixColors(Panel::getColorUp(), Panel::getColorDown(), j/(double)height));
		}

		for(i = 0; i < width; i++)
		{
			pixels[ j*width + i ] = color;
		}
	}

	//Glass effect
	KrRGBA white(255, 255, 255);
	int hm = height/2;
	for(j = 0; j < hm; j++)
	{
		for(i = 0; i < width; i++)
		{
			pixels[ j*width + i ] = SlowCanvas::MixColors(pixels[ j*width + i ], white, .8*j/(double)hm);
		}
	}


	if(bDrawArrows && width > 11 && height > 11)
	{
		int ox = width - 10, oy = (height - 7)/2;

		pixels[ (3 + ox) + (0 + oy)*width ] = colorArrow;		

		pixels[ (2 + ox) + (1 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (1 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (1 + oy)*width ] = colorArrow;		

		pixels[ (1 + ox) + (2 + oy)*width ] = colorArrow;		
		pixels[ (2 + ox) + (2 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (2 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (2 + oy)*width ] = colorArrow;		
		pixels[ (5 + ox) + (2 + oy)*width ] = colorArrow;		

		/*pixels[ (0 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (1 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (2 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (5 + ox) + (3 + oy)*width ] = colorArrow;		
		pixels[ (6 + ox) + (3 + oy)*width ] = colorArrow;

		pixels[ (0 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (1 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (2 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (5 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (6 + ox) + (5 + oy)*width ] = colorArrow;*/

		pixels[ (1 + ox) + (4 + oy)*width ] = colorArrow;		
		pixels[ (2 + ox) + (4 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (4 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (4 + oy)*width ] = colorArrow;		
		pixels[ (5 + ox) + (4 + oy)*width ] = colorArrow;

		pixels[ (2 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (3 + ox) + (5 + oy)*width ] = colorArrow;		
		pixels[ (4 + ox) + (5 + oy)*width ] = colorArrow;

		pixels[ (3 + ox) + (6 + oy)*width ] = colorArrow;
		
	}

	if(width > 4 && height > 4)
	{
	  //sky border colors
		KrRGBA colorBorder(cs->button_border_r, cs->button_border_g, cs->button_border_b);	
		
		//Horizontal lines
		for(i = 0; i < width; i++)
		{
			pixels[  i ] = colorBorder;	
			pixels[ (height-1)*width + i ] = colorBorder;
		}
		
		//Vertical lines
		for(j = 0; j < height; j++)
		{
			pixels[ j*width ] = colorBorder;		
			pixels[ j*width + (width-1) ] = colorBorder;
		}
		
		//Corner pixels
		pixels[ 1 + 1*width ] = colorBorder;
		pixels[ (width - 2) + 1*width ] = colorBorder;
		pixels[ (width - 2) + (height - 2)*width ] = colorBorder;
		pixels[ 1 + (height - 2)*width ] = colorBorder;
		
		
		//Make trasparent corners
		pixels[ 0 + 0*width ] = transp;
		pixels[ 1 + 0*width ] = transp;
		pixels[ 0 + 1*width ] = transp;
		
		pixels[ (width - 1) + 0*width ] = transp;
		pixels[ (width - 2) + 0*width ] = transp;
		pixels[ (width - 1) + 1*width ] = transp;
		
		pixels[ (width - 1) + (height - 1)*width ] = transp;
		pixels[ (width - 2) + (height - 1)*width ] = transp;
		pixels[ (width - 1) + (height - 2)*width ] = transp;
		
		pixels[ 0 + (height - 1)*width ] = transp;
		pixels[ 1 + (height - 1)*width ] = transp;
		pixels[ 0 + (height - 2)*width ] = transp;
	}
	


	getImage()->Invalidate();
	getCanvasResource()->Refresh();
}

void Button::Disable() 
{
	if(bEnable)
	{
		SetTransparency(.5);		
		bEnable = false;
	}
}

void Button::Enable() 
{
	if(!bEnable)
	{
		SetTransparency(0.0);		
		bEnable = true;
	}
}

void Button::OnMouseEnter(int x, int y)
{
	if(bStayPressed || !bEnable)
		return;

	//getSprite()->SetFrame(1);
	Draw(true);
}

void Button::OnMouseLeave(int x, int y)
{
	if(bStayPressed || !bEnable)
		return;

	buttonDown = false;

	//getSprite()->SetFrame(0);
	textBox->SetPos(px/*/sx*/, py/*/sy*/);

	Draw(false);
}

bool Button::OnMouseButtonDown(int x, int y, Uint8 button)
{
	//Click only with left mouse
	if(!bEnable || button != SDL_BUTTON_LEFT) return false;
	bHaveButtonPressed = true;

	if(bStayPressed)
	{
		bStayPressed = false;
		buttonDown = false;
		//getSprite()->SetFrame(1);
		textBox->SetPos(px/*/sx*/, py/*/sy*/);
	}
	else
	{
		buttonDown = true;
		
		//getSprite()->SetFrame(2);
		textBox->SetPos((px+1)/*/sx*/, (py+1)/*/sy*/);
	}
	
	Draw(false);
	return false;
}

void Button::OnMouseButtonUp(int x, int y, Uint8 button)
{
	//Click only with left mouse
	bHaveButtonPressed = false;
	if(!bEnable || button != SDL_BUTTON_LEFT) return;
	
	//getSprite()->SetFrame(1);
	textBox->SetPos(px/*/sx*/, py/*/sy*/);
	
	if(buttonDown && getParent())
	{
		buttonDown = false;

		Draw(false); //Before call OnButton to avoid crash

		GameControl::Get()->setModified();
		getParent()->OnButton(this, buttonId);
	}	
	else
	{
		Draw(false);
	}

	//Don't do anymore here (this button can be deleted on parent OnButton call)
}

void Button::SetText(gedString text)
{
	getImage()->CalcTransform();

	textBox->SetText(text, 0);
	int textWidth  = fontResource->FontWidth(text.c_str());

#ifdef USE_WALK
	px = (int)((Width()/*/sx*/ - textWidth)/2 + offSetText/*/sx*/);
#else
	px = (int)((Width() - textWidth)/2 + offSetText);
#endif

	px -= bDrawArrows?3:0;

	textBox->SetPos(px/*/sx*/, py/*/sy*/);
}

void Button::SetPressed(bool bPressed)
{
	bStayPressed = bPressed;

	buttonDown = true;
	//getSprite()->SetFrame(2);
	textBox->SetPos((px+1)/*/sx*/, (py+1)/*/sy*/);

	Draw(false);
}


#endif //#ifndef STAND_ALONE_GAME
