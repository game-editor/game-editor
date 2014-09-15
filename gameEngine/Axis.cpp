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


// Axis.cpp: implementation of the Axis class.
//
//////////////////////////////////////////////////////////////////////

#include "Axis.h"
#include "GameControl.h"
#include "PathPoint.h"
#include "Path.h"

#ifndef STAND_ALONE_GAME
#include "../gameEditor/PathDialog.h"
#endif //#ifndef STAND_ALONE_GAME

#define DIM 1000

#ifndef STAND_ALONE_GAME
void GridSelected(Actor *_actor);
#endif

Axis *Axis::axis = NULL;

Axis::Axis()
	: Actor(AXIS_NAME, NULL, SLOW_CANVAS, DIM, DIM)
{	
	axis = this;
	getImage()->SetDimensions(GameControl::Get()->Width()/2, 
			      GameControl::Get()->Height()/2,
				  DIM, DIM
				  );
	getImage()->SetZDepth(0);
	SetTransparency(0);	
	((SlowCanvas *)getImage())->setInfinite();

	scale = 1.0;
	
	screenWidth = GameControl::Get()->Width();
	screenHeight = GameControl::Get()->Height();
	gameWidth = GameControl::Get()->getGameWidth();
	gameHeight = GameControl::Get()->getGameHeight();

	
	axisColor.Set(200, 200, 200);
	pathColor.Set(255, 255, 255);
	
	
	mainColor.Set(100, 100, 100);
	resolutionColor.Set(150, 150, 150);

	bGridSnap = false;
	bGridShow = true;

	gridX = 20;//screenWidth/32;
	gridY = 20;//screenHeight/32;

	
#ifndef STAND_ALONE_GAME
	//Create here, only	
	gridCanvas = new AxisGridCanvas(this);
	pathCanvas = new AxisPathCanvas(this);
#endif
}

Axis::~Axis()
{
#ifndef STAND_ALONE_GAME
	delete gridCanvas;
	delete pathCanvas;
#endif

	axis = NULL;
}

const stCursor *Axis::OnGetCursor()
{
	return NULL;
}

bool Axis::OnMouseButtonDown(int x, int y, Uint8 button)
{
#ifndef STAND_ALONE_GAME
  GridSelected(this);

  switch(button)
  {
    case SDL_BUTTON_WHEELDOWN:
      scale /= 2.0;
      if(scale < 1/32.0) scale = 1/32.0;
      SetScale(scale);
      break;

    case SDL_BUTTON_WHEELUP:
      scale *= 2.0;
      if(scale > 32.0) scale = 32.0;
      SetScale(scale);
      break;
  }
#endif
  return false;
}



void Axis::ScreenResolution(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	((SlowCanvas *)getImage())->ScreenResolution(width, height);
	((SlowCanvas *)drawActivateEventLines.getImage())->ScreenResolution(width, height);

#ifndef STAND_ALONE_GAME

	((SlowCanvas *)gridCanvas->getImage())->ScreenResolution(width, height);
	((SlowCanvas *)pathCanvas->getImage())->ScreenResolution(width, height);

	SetScale(scale);
#endif //#ifndef STAND_ALONE_GAME
}

void Axis::GameResolution(int width, int height)
{
	gameWidth = width;
	gameHeight = height;
	
	GameControl::Get()->GetAxis()->getImage()->Invalidate();
}

void Axis::GridSnap(int &x, int &y)
{
	if(!bGridSnap || !bGridShow) return;

	int gridXs = (int)(gridX*scale),
	    gridYs = (int)(gridY*scale);
	
	if(gridXs >= 1 && gridYs >= 1)
	{
		
		int x1 = (x/gridXs)*gridXs + (getImage()->X() % gridXs),
			y1 = (y/gridYs)*gridYs + (getImage()->Y() % gridYs);
		
		if(x < x1 + gridXs/2.0) x = x1;
		else x = x1 + gridXs;
		
		if(y < y1 + gridYs/2.0) y = y1;
		else y = y1 + gridYs;
	}
}

void Axis::SetScale(double _scale)
{
	//Center on center screen, not in axis
	scale = _scale;
	if(scale < 1/32.0) scale = 1/32.0;

	KrVector2T< GlFixed > center, center1;
	getImage()->ScreenToObject(screenWidth/2.0, screenHeight/2.0, &center);

	
	getImage()->SetScale(scale, scale);
	getImage()->CalcTransform();


	getImage()->ScreenToObject(screenWidth/2.0, screenHeight/2.0, &center1);

	double	dx = (center1.x.ToInt() - center.x.ToInt()),
			dy = (center1.y.ToInt() - center.y.ToInt());

	SetPos(getImage()->X() + scale*dx, getImage()->Y() + scale*dy);	
}

#ifndef STAND_ALONE_GAME


void Axis::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(!GameControl::Get()->getGameMode() && KrEventManager::Instance()->GetKeyFocus() == -1)
	{
		//SDLMod keyMod = SDL_GetModState();
		//if(keyMod == KMOD_NONE || keyMod == KMOD_CAPS) //Why this?
		{
			switch(key)
			{
			case SDLK_COMMA:
			case SDLK_MINUS:
			case SDLK_KP_MINUS:
			case SDLK_LESS:
				{
					scale /= 2.0;
					if(scale < 1/32.0) scale = 1/32.0;
					SetScale(scale);
				}
				break;
			case SDLK_PERIOD:
			case SDLK_EQUALS:
			case SDLK_KP_PLUS:
			case SDLK_GREATER:
				{
					scale *= 2.0;
					if(scale > 32.0) scale = 32.0;
					SetScale(scale);
				}
				break;
			}

#ifdef wxUSE_GUI
			PanelProperty::UpdateActorPosition();		
#endif

		}
	}
}

void Axis::OnKeyUp(SDLKey key)
{
	if(!GameControl::Get()->getGameMode() && KrEventManager::Instance()->GetKeyFocus() == -1 && key == SDLK_DELETE)
	{
		PostMessage(this, REMOVE_SELECTED_ACTOR, REMOVE_SELECTED_ACTOR); 
	}
}


void Axis::SetGridZDepth(int zdepth)
{
	gridCanvas->getImage()->SetZDepth(zdepth);
}

int Axis::GetGridZDepth()
{
	return gridCanvas->getImage()->ZDepth();
}






AxisGridCanvas::AxisGridCanvas(Actor *parent)
	: Actor(AXIS_GRID_CANVAS_NAME, parent, SLOW_CANVAS, DIM, DIM)
{
	getImage()->SetDimensions(GameControl::Get()->Width()/2, 
			      GameControl::Get()->Height()/2,
				  DIM, DIM
				  );
	getImage()->SetZDepth(0);
	SetTransparency(0);	
	((SlowCanvas *)getImage())->setInfinite();	
}

AxisGridCanvas::~AxisGridCanvas()
{

}

void AxisGridCanvas::DrawAxis(KrPainter *painter, KrRect &clip)
{
	int x = Axis::Get()->getImage()->X(),
		y = Axis::Get()->getImage()->Y();

	KrRect axisV;
	axisV.xmin = x;
	axisV.ymin = y - 20;
	axisV.xmax = x;
	axisV.ymax = y + 19;


	if(axisV.Intersect(clip))
	{
		axisV.DoIntersection(clip);

		//SDL_Rect dr = {axisV.xmin, axisV.ymin, axisV.Width(), axisV.Height()};
		//SDL_FillRect(surface, &dr, axisColor);

		
		painter->DrawVLine(
			axisV.xmin, axisV.ymin, 
			axisV.Height(), 
			Axis::Get()->axisColor.c.red, Axis::Get()->axisColor.c.green, Axis::Get()->axisColor.c.blue, 2);
	}

	KrRect axisH;
	axisH.xmin = x - 20;
	axisH.ymin = y;
	axisH.xmax = x + 19;
	axisH.ymax = y;

	if(axisH.Intersect(clip))
	{
		axisH.DoIntersection(clip);

		//SDL_Rect dr = {axisH.xmin, axisH.ymin, axisH.Width(), axisH.Height()};
		//SDL_FillRect(surface, &dr, axisColor);

		painter->DrawHLine(
			axisH.xmin, axisH.ymin, 
			axisH.Width(), 
			Axis::Get()->axisColor.c.red, Axis::Get()->axisColor.c.green, Axis::Get()->axisColor.c.blue, 2);
	}
}

void AxisGridCanvas::DrawGrid(KrPainter *painter, SDL_Rect &clip, int gridx, int gridy, int x_offset, int y_offset, int lineSize, const KrRGBA& color)
{
	/*
	Fails when scale < 1.0 and gridy = 15
	*/
	int xmax = clip.x + clip.w,
		ymax = clip.y + clip.h;

	gridx *= Axis::Get()->scale;
	gridy *= Axis::Get()->scale;

	
	//Vertical lines
	if(gridx > 2)
	{
		SDL_Rect dr = clip;
		int xini = (clip.x/gridx)*gridx + ((Axis::Get()->getImage()->X() + (int)(x_offset*(Axis::Get()->scale))) % gridx);
		dr.w = lineSize;
		
		for(int x = xini; x < xmax; x += gridx)
		{
			dr.x = x;
			if((dr.x + dr.w) >= xmax)
			{
				dr.w = xmax - dr.x;
			}

			if(dr.x + dr.w <= xmax && dr.x >= clip.x)
			{
				//SDL_FillRect(surface, &dr, color);
				
				painter->DrawVLine(
					dr.x, dr.y, 
					dr.h, 
					color.c.red, color.c.green, color.c.blue, 1);
			}			
		}
	}

	//Horizontal lines
	if(gridy > 2)
	{
		SDL_Rect dr = clip;
		int yini = (clip.y/gridy)*gridy + ((Axis::Get()->getImage()->Y() + (int)(y_offset*(Axis::Get()->scale))) % gridy);
		dr.h = lineSize;
		
		for(int y = yini; y < ymax; y += gridy)
		{
			dr.y = y;
			if((dr.y + dr.h) >= ymax)
			{
				dr.h = ymax - dr.y;
			}

			if(dr.y + dr.h <= ymax && dr.y >= clip.y)
			{
				//SDL_FillRect(surface, &dr, color);

				painter->DrawHLine(
					dr.x, dr.y, 
					dr.w, 
					color.c.red, color.c.green, color.c.blue, 1);
			}			
		}
	}
}

void AxisGridCanvas::OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ)
{

	if(!GameControl::Get()->getGameMode())
	{
		KrPainter painter(paintInfo);
		SDL_Rect clip = { rect.xmin, rect.ymin, rect.Width(), rect.Height() };

		
		if(Axis::Get()->bGridShow)
		{
			DrawGrid(&painter, clip, Axis::Get()->gridX, Axis::Get()->gridY, 0, 0, 1, Axis::Get()->mainColor);
			DrawGrid(&painter, clip, Axis::Get()->gameWidth, Axis::Get()->gameHeight, Axis::Get()->gameWidth/2, Axis::Get()->gameHeight/2, 1, Axis::Get()->resolutionColor);
			DrawAxis(&painter, rect);
		}
	}
}


AxisPathCanvas::AxisPathCanvas(Actor *parent)
	: Actor(AXIS_PATH_CANVAS_NAME, parent, SLOW_CANVAS, DIM, DIM)
{
	getImage()->SetDimensions(GameControl::Get()->Width()/2, 
			      GameControl::Get()->Height()/2,
				  DIM, DIM
				  );

	getImage()->SetZDepth(REGION_DEPTH);

	SetTransparency(0);	
	((SlowCanvas *)getImage())->setInfinite();	
}

AxisPathCanvas::~AxisPathCanvas()
{

}

void AxisPathCanvas::DrawPath(Path *path, KrPainter *painter, SDL_Rect &clip)
{
	SDL_Rect dr = {0, 0, 1, 1};
	double scale = Axis::Get()->scale;
	int x, y, xini = path->getXOri()*scale + Axis::Get()->getImage()->X(), yini = path->getYOri()*scale + Axis::Get()->getImage()->Y();

	KrRGBA color(Axis::Get()->pathColor);
	
	for(int i = 0; i < path->GetTotalFrames(); i++)
	{
		path->GetXY(x, y, i);
		x = x*scale + xini;
		y = y*scale + yini;
		
		if(x >= clip.x && y >= clip.y && x < clip.x + clip.w && y < clip.y + clip.h)
		{
			dr.x = x;
			dr.y = y;
			//SDL_FillRect(surface, &dr, pathColor);

			painter->DrawHLine(
				dr.x, dr.y, 
				dr.w, 
				color.c.red, color.c.green, color.c.blue, 2);
		}
	}
}

void AxisPathCanvas::DrawPaths(KrPainter *painter, SDL_Rect &clip)
{
	Path *path;
	if(!PathDialog::getPathDialog() && !PathDialog::getActualPath()) return;

	//Show created path
	if((path = PathDialog::getActualPath()) != NULL)
	{
		DrawPath(path, painter, clip);
	}
	
	if(PathDialog::getPathDialog())
	{
		//Show edit path
		PathPoint *root;
		if((root = GameControl::Get()->GetPathRoot()) != NULL && root->TotalKeys() > 1)
		{
			Path path("edit", root->TotalKeys(), 20*root->TotalKeys());
			
			int oriX = root->getX(), oriY = root->getY();
			path.SetOri(oriX, oriY);
			path.AddKey(0, 0, root->getLinear());
			root = root->getNext();
			
			while(root)
			{
				path.AddKey(root->getX(), root->getY(), root->getLinear());
				root = root->getNext();
			} 
			
			DrawPath(&path, painter, clip);
		}
	}
}

void AxisPathCanvas::OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ)
{

	if(!GameControl::Get()->getGameMode())
	{
		KrPainter painter(paintInfo);
		SDL_Rect clip = { rect.xmin, rect.ymin, rect.Width(), rect.Height() };

		DrawPaths(&painter, clip);
	}
}


#endif //#ifndef STAND_ALONE_GAME



