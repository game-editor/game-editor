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


// TutorialPaint.cpp: implementation of the TutorialPaint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "TutorialPaint.h"
#include "GameControl.h"
#include "Tutorial.h"
#include "TutorialTextPanel.h"

TutorialPaint *TutorialPaint::paint = NULL;

TutorialPaint::TutorialPaint()
: Actor("TutorialPaint", NULL, CANVAS, GameControl::Get()->Width(), GameControl::Get()->Height(), true)
{
	SetModal();
	SetPos(0, 0);
	getImage()->SetZDepth(CURSOR_DEPTH - 500);

	

	bLine = bErase = bPaint = false;
	bOnDestructor = false;
	xAnt = yAnt = -1;

	//Draw
	width = Width();
	height = Height();

	KrRGBA *pixels = getCanvasResource()->Pixels(), color;

	color.Set(0, 0, 0, 1);

	//Fill background
	int i;
	for(i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			pixels[ j*width + i ] = color;
		}
	}


	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->Pause(PAUSED_BY_EXPLANATION_EDITOR);

		//Add event
		SDL_Event event;
		event.type = SDL_TUTORIAL_PAINT_START_EVENT;
		
		Tutorial::Get()->SaveEvent(event, SDL_GetTicks());
	}
}

TutorialPaint::~TutorialPaint()
{
	bOnDestructor = true;
	TutorialTextPanel::DestroyAll();
	

	paint = NULL;

	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->Pause(PAUSED_BY_EXPLANATION_EDITOR);

		//Add event
		SDL_Event event;
		event.type = SDL_TUTORIAL_PAINT_END_EVENT;
		
		Tutorial::Get()->SaveEvent(event, 0);
	}
}

TutorialPaint *TutorialPaint::Call()
{
	if(!paint) paint = new TutorialPaint();
	
	return paint;
}

void TutorialPaint::Destroy()
{
	if(paint && !paint->bOnDestructor)
	{
		delete paint;		
	}
}

void TutorialPaint::Show()
{
	if(paint) paint->getImage()->SetVisible(true);
}

void TutorialPaint::Hide()
{
	if(paint) paint->getImage()->SetVisible(false);
}

void TutorialPaint::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(key == SDLK_LSHIFT || key == SDLK_RSHIFT)
	{
		bErase = true;
	}

	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->SaveActualEvent();
	}
}

void TutorialPaint::OnKeyUp(SDLKey key)
{
	if(key == SDLK_LSHIFT || key == SDLK_RSHIFT)
	{
		bErase = false;
	}

	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->SaveActualEvent();
	}
}

bool TutorialPaint::OnMouseButtonDown(int x, int y, Uint8 button)
{
	bPaint = true;
	this->button = button;

	if(button == SDL_BUTTON_RIGHT)
	{
		bLine = true;

		KrVector2T< GlFixed > object;
		getImage()->ScreenToObject(x, y, &object);
		
		xAnt = object.x.ToInt();
		yAnt = object.y.ToInt();
	}

	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->SaveActualEvent();
	}

	return false;
}

void TutorialPaint::OnMouseButtonUp(int x, int y, Uint8 button)
{
	if(bLine)
	{
		KrVector2T< GlFixed > object;
		getImage()->ScreenToObject(x, y, &object);

		if(object.x < 0) object.x = 0;
		if(object.y < 0) object.y = 0;
		if(object.x >= Width()) object.x = Width()-1;
		if(object.y >= Height()) object.y = Height()-1;
			
		
		int x1 = object.x.ToInt();
		int y1 = object.y.ToInt();
		
		if(xAnt != -1 && yAnt != -1)
		{
			//Draw
			KrRGBA *pixels = getCanvasResource()->Pixels(), color;
			int penSize;

			if(bErase)
			{
				color.Set(0, 0, 0, 1);
				penSize = 6;
			}
			else
			{
				color.Set(255, 0, 0);
				penSize = 3;
			}

			KrPaintInfo paintInfo(pixels, width, height );
			KrRect clip;
			clip.Set(0, 0, width, height);

			SlowCanvas::Line(&paintInfo, clip, x1, y1, xAnt, yAnt, color, color, penSize);

			KrRect r;
			r.Set(min(x1, xAnt), min(y1, yAnt), max(x1, xAnt), max(y1, yAnt));
			r.EdgeAdd(10);

			getCanvasResource()->Refresh();
			engine->InvalidateRectangle(r);
		}
	}

	bLine = bErase = bPaint = false;
	xAnt = yAnt = -1;	

	if(Tutorial::InRecMode())
	{
		Tutorial::Get()->SaveActualEvent();
	}
}


void TutorialPaint::OnMouseMove(int x, int y)
{
	if(bPaint && !bLine)
	{
		KrVector2T< GlFixed > object;
		getImage()->ScreenToObject(x, y, &object);
		
		if(object.x >= 0 && object.y >= 0 && object.x < Width() && object.y < Height())
		{
			int x1 = object.x.ToInt();
			int y1 = object.y.ToInt();
			
			if(xAnt != -1 && yAnt != -1)
			{
				//Draw
				KrRGBA *pixels = getCanvasResource()->Pixels(), color;
				int penSize;
				
				if(bErase)
				{
					color.Set(0, 0, 0, 1);
					penSize = 6;
				}
				else
				{
					color.Set(255, 0, 0);
					penSize = 3;
				}
				
				KrPaintInfo paintInfo(pixels, width, height );
				KrRect clip;
				clip.Set(0, 0, width, height);
				
				SlowCanvas::Line(&paintInfo, clip, x1, y1, xAnt, yAnt, color, color, penSize);
				
				KrRect r;
				r.Set(min(x1, xAnt), min(y1, yAnt), max(x1, xAnt), max(y1, yAnt));
				r.EdgeAdd(10);

				getCanvasResource()->Refresh();
				engine->InvalidateRectangle(r);
			}
			
			xAnt = x1;
			yAnt = y1;
			
			if(Tutorial::InRecMode())
			{
				Tutorial::Get()->SaveActualEvent();
			}
		}
	}
}

const stCursor *TutorialPaint::OnGetCursor()
{
	//Maks: use after put this images on editor.dat
	/*if(bErase)
	{
		cursor.imgName	= "makscursorErase.bmp";
		cursor.nFramesH	= 1;
		cursor.nFramesV	= 1;
		cursor.hotSpotX = 3;
		cursor.hotSpotY = 4;
		cursor.bAnimate = false;
	}
	else
	{
		cursor.imgName	= "makscursorPen.bmp";
		cursor.nFramesH	= 1;
		cursor.nFramesV	= 1;
		cursor.hotSpotX = 0;
		cursor.hotSpotY = 16;
		cursor.bAnimate = false;
	}
	
	return &cursor;	*/

	return NULL;
}

void TutorialPaint::Execute(SDL_Event &event)
{
	switch(event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		OnMouseButtonDown(event.button.x, event.button.y, event.button.button);
		break;

	case SDL_MOUSEBUTTONUP:
		OnMouseButtonUp(event.button.x, event.button.y, event.button.button);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMove(event.button.x, event.button.y);
		break;

	case SDL_KEYDOWN:
		OnKeyDown(event.key.keysym.sym, false);
		break;

	case SDL_KEYUP:
		OnKeyUp(event.key.keysym.sym);
		break;

	case SDL_TUTORIAL_TEXT_EVENT:
		{
			//Add a new text
			gedString text(Tutorial::Get()->GetText(event.user.code));
			if(text.length())
			{				
				TutorialTextPanel *p = new TutorialTextPanel(text);
				p->SetPos((int)event.user.data1, (int)event.user.data2);
			}
		}
		break;
	}
}


#endif //STAND_ALONE_GAME






