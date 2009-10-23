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


// Slide.cpp: implementation of the Slide class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "Slide.h"
#include "EditBox.h"
#include "GameControl.h"


Slide::Slide(Actor *parent, int x, int y, int width, int height, int slideId)
	: Panel(getRandomName(), x, y, width, height, parent, true, false, true, false)
{
	cursor = new SlideCursor(this, width, height);
	this->slideId = slideId;
}

Slide::~Slide()
{
	delete cursor;
}


SlideCursor::SlideCursor(Actor *parent, int width, int height)
: Panel(getRandomName(), 0, 0, ((width > height)?10:width), ((width > height)?height:10), parent, false, true, true, false)
{
	widthParent = parent->Width();
	heightParent = parent->Height();
	this->width = Width();
	this->height = Height();

	if(width > height)
		bHorizontal = true;
	else
		bHorizontal = false;

	ant = -1;
}

SlideCursor::~SlideCursor()
{

}

bool SlideCursor::OnMouseButtonDown(int x, int y, Uint8 button)
{
	return true;
}

void SlideCursor::OnAnimation()
{
	double pos;
	if(bHorizontal)
	{
		int x = getImage()->X();
		
		if(x < 0)
			x = 0;
		
		int xMax = widthParent - width;
		if(x > xMax)
			x = xMax;
		
		getImage()->SetPos(x, 0);
		
		if(x != ant && ant != -1)
		{
			//Notify slide parent (0 - 1.0)
			pos = x/(double)xMax;
			((Slide *)parent)->SetPos(pos);
			parent->getParent()->OnSlide((Slide *)parent, ((Slide *)parent)->getSlideId(), pos);
		}
		
		ant = x;
	}
	else
	{
		int y = getImage()->Y();
		
		if(y < 0)
			y = 0;
		
		int yMax = heightParent - height;
		if(y > yMax)
			y = yMax;
		
		getImage()->SetPos(0, y);
		
		if(y != ant && ant != -1)
		{
			//Notify slide parent (0 - 1.0)
			pos = 1.0 - y/(double)yMax;
			((Slide *)parent)->SetPos(pos);
			parent->getParent()->OnSlide((Slide *)parent, ((Slide *)parent)->getSlideId(), pos);
		}
		
		ant = y;
	}
}

const stCursor * SlideCursor::OnGetCursor()
{
	/*cursor.imgName	= "makscursorMao.bmp";
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
	
	return &cursor;*/

	return NULL;
}

void Slide::SetPos(double pos)
{
	cursor->SetPos(pos);
	atualPos = pos;

	char buf[128];
	sprintf(buf, "%.3f", atualPos);
	SetToolTip(buf);
	cursor->SetToolTip(buf);
}

double Slide::GetPos()
{
	return atualPos;
}

void SlideCursor::SetPos(double pos)
{
	if(bHorizontal)
	{
		int x = (int)(pos*(widthParent - Width()));
		getImage()->SetPos(x, 0);

		ant = x;
	}
	else
	{
		int y = (int)((1.0 - pos)*(heightParent - Height()));
		getImage()->SetPos(0, y);

		ant = y;
	}
}

#define SCROLL_DELAY_TIME 700

ScrollBar::ScrollBar(Actor *parent, EditBox *_edit, int _scrollId)
	: Actor(getRandomName(), parent, CANVAS, 15, _edit->Height())
{
	edit = _edit;
	scrollId = _scrollId;
	side = Width() - 2;
	bInArrow = false;

	DrawBackGround();
	DrawArrowButtons();

	scrollDown = scrollUp = scrollPageDown = scrollPageUp = 0;
	scrollRepeatTime = SCROLL_DELAY_TIME;

	scrollCursor = new ScrollBarCursor(this, edit, side, Height());
}

ScrollBar::~ScrollBar()
{
	delete scrollCursor;
}

void ScrollBar::SetScrollInfo(int totalItens, int itensPerPage)
{
	scrollCursor->SetScrollInfo(totalItens, itensPerPage);
}

void ScrollBar::SetScrollPos(int line)
{
	scrollCursor->SetScrollPos(line);
}

int ScrollBar::GetScrollPos()
{
	return scrollCursor->GetScrollPos();
}

void ScrollBar::DrawBackGround()
{
	KrRGBA *pixels = getCanvasResource()->Pixels();

	KrRGBA colorBack = Panel::getColorBack();
	
	/*colorBack.c.red		-= 6;
	colorBack.c.green	+= 18;
	colorBack.c.blue	+= 55;*/ 

	

	//Fill background
	int i;
	for(i = 0; i < Width(); i++)
	{
		for(int j = 0; j < Height(); j++)
		{
			pixels[ j*Width() + i ] = colorBack;
		}
	}

	//Horizontal lines
	for(i = 0; i < Width(); i++)
	{
		pixels[ i ] = Panel::getColorDown();			
		pixels[ (Height()-1)*Width() + i ] = Panel::getColorUp();
	}

	//Vertical lines
	for(int j = 0; j < Height(); j++)
	{
		if(j < side || j >= (Height() - side - 1))
		{
			pixels[ j*Width() ] = Panel::getColorUp();		
			pixels[ j*Width() + (Width()-1) ] = Panel::getColorDown();
		}
		else
		{
			pixels[ j*Width() ] = Panel::getColorDown();		
			pixels[ j*Width() + (Width()-1) ] = Panel::getColorUp();
		}
	}

	getCanvasResource()->Refresh();
}

void ScrollBar::DrawArrowButtons()
{
	KrRGBA *pixels = getCanvasResource()->Pixels(), colorArrow;

	colorArrow.c.red	= 0;
	colorArrow.c.green	= 0;
	colorArrow.c.blue	= 0;
	colorArrow.c.alpha	= 255;

	
	
	//Fill background
	int i, j;
	for(i = 1; i < Width() - 1; i++)
	{
		//Top
		for(j = 1; j < side; j++)
		{
			pixels[ j*Width() + i ] = Panel::getColorBack();
		}

		//Down
		for(j = Height() - side - 1; j < Height() - 1; j++)
		{
			pixels[ j*Width() + i ] = Panel::getColorBack();
		}
	}

	//Horizontal lines
	for(i = 1; i < Width() - 1; i++)
	{
		//Top
		pixels[ i ] = Panel::getColorUp();			
		pixels[ (side)*Width() + i ] = Panel::getColorDown();

		//Down
		pixels[ (Height() - side - 1)*Width() + i ] = Panel::getColorUp();			
		pixels[ (Height() - 1)*Width() + i ] = Panel::getColorDown();
	}

	//Arrow Top
	pixels[ 7 + 5*Width() ] = colorArrow;

	pixels[ 6 + 6*Width() ] = colorArrow;
	pixels[ 7 + 6*Width() ] = colorArrow;
	pixels[ 8 + 6*Width() ] = colorArrow;

	pixels[ 5 + 7*Width() ] = colorArrow;
	pixels[ 6 + 7*Width() ] = colorArrow;
	pixels[ 7 + 7*Width() ] = colorArrow;
	pixels[ 8 + 7*Width() ] = colorArrow;
	pixels[ 9 + 7*Width() ] = colorArrow;

	pixels[ 4 + 8*Width() ] = colorArrow;
	pixels[ 5 + 8*Width() ] = colorArrow;
	pixels[ 6 + 8*Width() ] = colorArrow;
	pixels[ 7 + 8*Width() ] = colorArrow;
	pixels[ 8 + 8*Width() ] = colorArrow;
	pixels[ 9 + 8*Width() ] = colorArrow;
	pixels[ 10 + 8*Width() ] = colorArrow;

	//Arrow Down
	pixels[ 7 + (Height() - 6)*Width() ] = colorArrow;

	pixels[ 6 + (Height() - 7)*Width() ] = colorArrow;
	pixels[ 7 + (Height() - 7)*Width() ] = colorArrow;
	pixels[ 8 + (Height() - 7)*Width() ] = colorArrow;

	pixels[ 5 + (Height() - 8)*Width() ] = colorArrow;
	pixels[ 6 + (Height() - 8)*Width() ] = colorArrow;
	pixels[ 7 + (Height() - 8)*Width() ] = colorArrow;
	pixels[ 8 + (Height() - 8)*Width() ] = colorArrow;
	pixels[ 9 + (Height() - 8)*Width() ] = colorArrow;

	pixels[ 4 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 5 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 6 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 7 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 8 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 9 + (Height() - 9)*Width() ] = colorArrow;
	pixels[ 10 + (Height() - 9)*Width() ] = colorArrow;

	getCanvasResource()->Refresh();	
}

bool ScrollBar::OnMouseButtonDown(int x, int y, Uint8 button)
{
	KrVector2T< GlFixed > object;
	getImage()->ScreenToObject( x, y, &object );

	if(object.y >= 1 && object.y <= side)
	{
		if(!scrollUp)
		{
			scrollRepeatTime = SCROLL_DELAY_TIME;
			scrollUp = SetTimer(scrollRepeatTime);
		}

		ScrollUp();		
	}
	else if(object.y >= Height() - side - 1 && object.y < Height() - 1)
	{
		if(!scrollDown)
		{
			scrollRepeatTime = SCROLL_DELAY_TIME;
			scrollDown = SetTimer(scrollRepeatTime);
		}

		ScrollDown();		
	}
	else 
	{
		//Page swap
		if(object.y > scrollCursor->getImage()->Y() + scrollCursor->getScrollPageSize())
		{
			if(!scrollPageDown)
			{
				scrollRepeatTime = SCROLL_DELAY_TIME;
				scrollPageDown = SetTimer(scrollRepeatTime);
			}
			
			ScrollPageDown();
		}
		else if(object.y < scrollCursor->getImage()->Y())
		{
			if(!scrollPageUp)
			{
				scrollRepeatTime = SCROLL_DELAY_TIME;
				scrollPageUp = SetTimer(scrollRepeatTime);
			}

			ScrollPageUp();
		}
	}

	return false;
}

void ScrollBar::OnMouseMove(int x, int y)
{
	KrVector2T< GlFixed > object;
	getImage()->ScreenToObject( x, y, &object );

	if((object.y >= 1 && object.y <= side) || (object.y >= Height() - side - 1 && object.y < Height() - 1))
	{
		bInArrow = true;		
	}
	else
	{
		bInArrow = false;
	}
}

void ScrollBar::OnMouseButtonUp(int x, int y, Uint8 button)
{
	RemoveTimers();
}

void ScrollBar::RemoveTimers()
{
	if(scrollUp) SDL_RemoveTimer(scrollUp);
	if(scrollDown) SDL_RemoveTimer(scrollDown);
	if(scrollPageUp) SDL_RemoveTimer(scrollPageUp);
	if(scrollPageDown) SDL_RemoveTimer(scrollPageDown);

	scrollDown = scrollUp = scrollPageDown = scrollPageUp = 0;
	scrollRepeatTime = SCROLL_DELAY_TIME;
}

Uint32 ScrollBar::OnTimer(SDL_TimerID timerID)
{
	if(timerID == scrollUp)
	{
		if(scrollRepeatTime != 100)
		{
			SDL_RemoveTimer(scrollUp);
			scrollUp = SetTimer(100);
		}

		ScrollUp();	
	}
	else if(timerID == scrollDown)
	{
		if(scrollRepeatTime != 100)
		{
			SDL_RemoveTimer(scrollDown);
			scrollDown = SetTimer(100);
		}

		ScrollDown();	
	}
	else
	{
		//Page swap
		KrVector2T< GlFixed > obj;
		getImage()->ScreenToObject( 0, GameControl::Get()->getMouseY(), &obj );

		int y = obj.y.ToInt();

		if( y > scrollCursor->getImage()->Y() + scrollCursor->getScrollPageSize() ||
			y < scrollCursor->getImage()->Y())
		{
			if(timerID == scrollPageUp)
			{
				if(scrollRepeatTime != 100)
				{
					SDL_RemoveTimer(scrollPageUp);
					scrollPageUp = SetTimer(100);
				}
				
				ScrollPageUp();	
			}
			else if(timerID == scrollPageDown)
			{
				if(scrollRepeatTime != 100)
				{
					SDL_RemoveTimer(scrollPageDown);
					scrollPageDown = SetTimer(100);
				}
				
				ScrollPageDown();	
			}	
		}
		else 
		{
			//Remove page timers
			RemoveTimers();
		}
	}

	scrollRepeatTime = 100;

	return 100;
}

void ScrollBar::ScrollUp()
{
	SetScrollPos(edit->ScrollUp());
}

void ScrollBar::ScrollDown()
{
	SetScrollPos(edit->ScrollDown());
}

void ScrollBar::ScrollPageUp()
{
	SetScrollPos(edit->ScrollPageUp());
}

void ScrollBar::ScrollPageDown()
{
	SetScrollPos(edit->ScrollPageDown());
}

const stCursor *ScrollBar::OnGetCursor()
{
	/*if(bInArrow)
	{
		cursor.imgName	= "makscursorMao.bmp";
		cursor.nFramesH	= 1;
		cursor.nFramesV	= 1;
		cursor.hotSpotX = 7;
		cursor.hotSpotY = 0;
		cursor.bAnimate = false;
		
		return &cursor;
	}*/
	
	return NULL;
}

ScrollBarCursor::ScrollBarCursor(Actor *parent, EditBox *_edit, int width, int height)
	: Actor(getRandomName(), parent, CANVAS, width, height)
{
	flags.Set(FLAG_PANEL); //To OnAnimation call
	edit = _edit;
	bMove = false;

	side = Width();
	pos = 0;
	yAnt = -1;
	totalItens = itensPerPage = 0;
	
	SetScrollInfo(100, 99);

	panelColor = getImage()->CTransform();
}

ScrollBarCursor::~ScrollBarCursor()
{

}

void ScrollBarCursor::SetScrollInfo(int _totalItens, int _itensPerPage)
{
	if(totalItens != _totalItens || itensPerPage != _itensPerPage)
	{
		int maxY = Height() - side - 1;
		int range = Height() - 2*side;
		
		scrollPageSize = range*(_itensPerPage/(float)_totalItens);
		if(scrollPageSize < 5) scrollPageSize = 5;
		
		minPos = side + 1;
		maxPos = maxY - scrollPageSize;
		
		totalItens = _totalItens;
		itensPerPage = _itensPerPage;
		
		Draw();	
	}
}

void ScrollBarCursor::Draw()
{
	KrRGBA *pixels = getCanvasResource()->Pixels(), colorTransp;

	colorTransp.Set(0, 0, 0, 0);

	

	//Fill background
	int i;
	for(i = 0; i < Width(); i++)
	{
		for(int j = 0; j < Height(); j++)
		{
			if(j < scrollPageSize)
				pixels[ j*Width() + i ] = Panel::getColorBack();
			else
				pixels[ j*Width() + i ] = colorTransp;
		}
	}

	DrawBorder(Panel::getColorUp(), Panel::getColorDown());
}

void ScrollBarCursor::DrawBorder(KrRGBA color1, KrRGBA color2)
{
	KrRGBA *pixels = getCanvasResource()->Pixels();

	//Horizontal lines
	for(int i = 0; i < Width(); i++)
	{
		pixels[ i ] = color1;			
		pixels[ (scrollPageSize-1)*Width() + i ] = color2;
	}

	//Vertical lines
	for(int j = 0; j < scrollPageSize; j++)
	{
		pixels[ j*Width() ] = color1;		
		pixels[ j*Width() + (Width()-1) ] = color2;
	}

	getImage()->Invalidate();
	getCanvasResource()->Refresh();
}

bool ScrollBarCursor::OnMouseButtonDown(int x, int y, Uint8 button)
{
	bMove = true;
	return true;
}

void ScrollBarCursor::OnMouseButtonUp(int x, int y, Uint8 button)
{
	bMove = false;
}

void ScrollBarCursor::OnMouseEnter(int x, int y)
{
	/*KrRGBA white;

	colorUp.c.red	= 255;
	colorUp.c.green	= 255;
	colorUp.c.blue	= 255;
	colorUp.c.alpha	= 255;

	DrawBorder(white, white);*/

	KrColorTransform color(getImage()->CTransform());
	color.Brighten(30);

	getImage()->SetColor(color);
}

void ScrollBarCursor::OnMouseLeave(int x, int y)
{
	//DrawBorder(colorUp, colorDown);

	getImage()->SetColor(panelColor);
}


void ScrollBarCursor::OnAnimation()
{
	int y = getImage()->Y();	
	
	if(y < minPos)
		y = minPos;
	
	if(y > maxPos)
		y = maxPos;

	
	getImage()->SetPos(1, y);

	
	if(y != yAnt && bMove)
	{
		pos = totalItens*((y - minPos)/(float)(maxPos - minPos));
		if(pos > totalItens) pos = totalItens;

		edit->ScrollTo(pos);
		
		yAnt = y;
	}
}

void ScrollBarCursor::SetScrollPos(int line)
{
	if(!bMove)
	{
		if(line < 0) line = 0;
		else if(line >= totalItens) line = totalItens - 1;
		
		pos = line;
		getImage()->SetPos(1, (maxPos - minPos)*pos/(float)totalItens + minPos);
	}
}

int ScrollBarCursor::GetScrollPos()
{
	return pos;
}

const stCursor *ScrollBarCursor::OnGetCursor()
{
	/*cursor.imgName	= "makscursorMao.bmp";
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
	
	return &cursor;*/

	return NULL;
}

#endif //#ifndef STAND_ALONE_GAME