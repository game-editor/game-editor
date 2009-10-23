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


// ActivationEventsCanvas.cpp: implementation of the ActivationEventsCanvas class.
//
//////////////////////////////////////////////////////////////////////

#include "ActivationEventsCanvas.h"
#include "GameControl.h"
#include "ActorEdit.h"
#include "../gameEditor/ActorProperty.h"


#define DIM 1000

int ActivationEventsCanvas::x1 = 0;
int ActivationEventsCanvas::y1 = 0;
int ActivationEventsCanvas::x2 = 0;
int ActivationEventsCanvas::y2 = 0;

int ActivationEventsCanvas::x1ant = 0;
int ActivationEventsCanvas::y1ant = 0;
int ActivationEventsCanvas::x2ant = 0;
int ActivationEventsCanvas::y2ant = 0;

ListEventLine ActivationEventsCanvas::listEventLines;
MapString ActivationEventsCanvas::mapActorInLines;

bool ActivationEventsCanvas::bShowLines = true;
ActivationEventsCanvas *ActivationEventsCanvas::activationEventsCanvas = NULL;


ActivationEventsCanvas::ActivationEventsCanvas()
	: Actor(ACTIVATION_EVENTS_CANVAS_NAME, NULL, SLOW_CANVAS, DIM, DIM)
{
	activationEventsCanvas = this;

	getImage()->SetDimensions(GameControl::Get()->Width()/2, 
			      GameControl::Get()->Height()/2,
				  DIM, DIM
				  );
	
	getImage()->SetZDepth(REGION_DEPTH);
	SetTransparency(0);	
	((SlowCanvas *)getImage())->setInfinite();
	((SlowCanvas *)getImage())->setCustomHitTest(true);
	((SlowCanvas *)getImage())->SetHitTest(true);
	((SlowCanvas *)getImage())->setEntireBoxHit(false);
	
	color1.Set(0, 255, 0);
	color2.Set(255, 60, 32);
	
#ifndef STAND_ALONE_GAME
	list = NULL;
#endif //#ifndef STAND_ALONE_GAME
}

ActivationEventsCanvas::~ActivationEventsCanvas()
{
	activationEventsCanvas = NULL;
}

void ActivationEventsCanvas::OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ)
{
	if(!GameControl::Get()->getGameMode())
	{
		DrawActivationEventsLines(paintInfo, rect);
	}
}

void ActivationEventsCanvas::DrawActivationEventsLines(KrPaintInfo *paintInfo, const KrRect &clip)
{
#ifndef STAND_ALONE_GAME
	if(ActorEdit::inActivationEventMode())
	{
		//Draw interacive line		
		/*int x1 = ActorEdit::getActivationEventActor()->getImage()->CompositeXForm().x.ToInt(),
			y1 = ActorEdit::getActivationEventActor()->getImage()->CompositeXForm().y.ToInt(), 
			x2 = GameControl::Get()->getMouseX(), 
			y2 = GameControl::Get()->getMouseY();
		/**/
		((SlowCanvas *)getImage())->Line(paintInfo, clip, x1, y1, x2, y2, color1, color2);		
	}
	else
	{
		x1 = y1 = x2 = y2 = 0;
		x1ant = y1ant = x2ant = y2ant = 0;
	}

	//Draw lines
	ListEventLineIterator itList(listEventLines);	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		Actor *actorFrom = GameControl::Get()->GetActor(itList.Current().actorFrom), *actorTo;
		if(!actorFrom)
		{
			//Remove
			RemoveLine(itList.Current());			
			itList.Begin();
			continue;
		}
		
		actorTo = GameControl::Get()->GetActor(itList.Current().actorTo);
		if(actorTo)
		{
			int xi = actorFrom->getImage()->CompositeXForm().x.ToInt(),
				yi = actorFrom->getImage()->CompositeXForm().y.ToInt(),
				xf = actorTo->getImage()->CompositeXForm().x.ToInt(),
				yf = actorTo->getImage()->CompositeXForm().y.ToInt();
			
			((SlowCanvas *)getImage())->Line(paintInfo, clip, xi, yi, xf, yf, color1, color2);
		}
	}

#endif //#ifndef STAND_ALONE_GAME
}

#ifndef STAND_ALONE_GAME
void ActivationEventsCanvas::NotifyMouseCoord(int x, int y)
{
	if(ActorEdit::inActivationEventMode() && ActorEdit::getActivationEventActor())
	{
		if(!bShowLines) ToggleVisibility();

		//Update coords for interacive lines

		//Slow
		//GameControl::Get()->GetAxis()->getImage()->Invalidate();

		//Fast
		KrRect r;

		x1 = ActorEdit::getActivationEventActor()->getImage()->CompositeXForm().x.ToInt();
		y1 = ActorEdit::getActivationEventActor()->getImage()->CompositeXForm().y.ToInt();
		x2 = x;
		y2 = y;

		r.Set(min(x1ant, x2ant), min(y1ant, y2ant), max(x2ant, x1ant), max(y2ant, y1ant));
		engine->InvalidateRectangle(r);

		r.Set(min(x1, x2), min(y1, y2), max(x2, x1), max(y2, y1));
		engine->InvalidateRectangle(r);

		x1ant = x1;
		y1ant = y1;
		x2ant = x2;
		y2ant = y2;/**/
	}
}

void ActivationEventsCanvas::AddLine(stAction *newAction, bool bLoad)
{
	//fromActor: newAction->name
	//toActor:	 newAction->actorName

	listEventLines.PushBack(stLine(newAction->name, newAction->actorName, newAction));
	mapActorInLines.Add(newAction->name, 1);
	mapActorInLines.Add(newAction->actorName, 1);

	//Update actorTo connection count
	if(!bLoad && !GameControl::Get()->getGameMode())
	{
		Actor *actorTo = GameControl::Get()->GetActor(newAction->actorName);
		if(actorTo) actorTo->incrementConnectedActors();
	}
}

void ActivationEventsCanvas::RemoveLine(const stLine &line)
{
	int nAtorFrom = 0, nAtorTo = 0;

	ListEventLineIterator itList(listEventLines);	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		if(	line.actorFrom == itList.Current().actorFrom ||
			line.actorFrom == itList.Current().actorTo)
			nAtorFrom++;

		if(	line.actorTo == itList.Current().actorFrom ||
			line.actorTo == itList.Current().actorTo)
			nAtorTo++;
	}

	if(nAtorFrom == 1)
		mapActorInLines.Remove(line.actorFrom);

	if(nAtorTo == 1)
		mapActorInLines.Remove(line.actorTo);

	//Update actorTo connection count
	if(!GameControl::Get()->getGameMode())
	{
		Actor *actorTo = GameControl::Get()->GetActor(line.actorTo);
		if(actorTo) actorTo->decrementConnectedActors();
	}

	listEventLines.FindAndDelete(line);
	GameControl::Get()->GetAxis()->getImage()->Invalidate();
}
#endif

const stCursor * ActivationEventsCanvas::OnGetCursor()
{
	if(GameControl::Get()->getGameMode())
		return NULL;
	
	//Need a cursor feedback here to guide the user.
	//Hard to get the tool tip without this.

	cursor.imgName	= "makscursorMao.bmp"; 
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
	
	return &cursor;
}

stAction * ActivationEventsCanvas::HitLine(int x, int y)
{
#ifndef STAND_ALONE_GAME
	//(x, y) is over a line?
	KrRect clip;
	clip.Set(x, y, x+1, y+1);
	
	ListEventLineIterator itList(listEventLines);	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		Actor *actorFrom = GameControl::Get()->GetActor(itList.Current().actorFrom), *actorTo;
		if(!actorFrom)
			continue;
		
		actorTo = GameControl::Get()->GetActor(itList.Current().actorTo);
		if(actorTo)
		{
			int xi = actorFrom->getImage()->CompositeXForm().x.ToInt(),
				yi = actorFrom->getImage()->CompositeXForm().y.ToInt(),
				xf = actorTo->getImage()->CompositeXForm().x.ToInt(),
				yf = actorTo->getImage()->CompositeXForm().y.ToInt();
			
			if(SlowCanvas::LineInRect(xi, yi, xf, yf, clip))
			{
				return itList.Current().action;
			}
		}
	}
#endif
	
	return NULL;
}

bool ActivationEventsCanvas::OnSlowCanvasHitTest(int x, int y)
{
#ifndef STAND_ALONE_GAME
	if(GameControl::Get()->getGameMode() || ActorEdit::inActivationEventMode())
		return false;
	
	hitTip = HitLine(x, y);
	return hitTip != NULL;
#else
	return false;
#endif
}

bool ActivationEventsCanvas::OnMouseButtonDown(int x, int y, Uint8 button)
{
#ifndef STAND_ALONE_GAME
	if(button == SDL_BUTTON_RIGHT)
	{
		if(!list)
		{
			//Menu
			list = AddListPop(0, 0, 1, 1);
			list->AddText("Remove Activation Event");
		}

		list->getImage()->SetVisible(true);
		list->OnMouseButtonDown(x, y, button);
		hitAction = HitLine(x, y);
	}
#endif //#ifndef STAND_ALONE_GAME

	return false;
}


void ActivationEventsCanvas::Clear()
{
	listEventLines.Clear();
	mapActorInLines.Clear();
}

const gedString *ActivationEventsCanvas::OnActorTip()
{
	if(hitTip)
	{
		actorTip = "Send Activation Event: \n";
		actorTip += "From: ";
		actorTip += hitTip->name;
		actorTip += "\n";
		actorTip += "  To: ";
		actorTip += hitTip->actorName;
		actorTip += "\n";
		actorTip += "  On: ";
		actorTip += Action::GetDescription(hitTip, false);

		return &actorTip;	
	}

	return NULL;
}

void ActivationEventsCanvas::ToggleVisibility()
{
	bShowLines = !bShowLines;
	activationEventsCanvas->getImage()->SetVisible(bShowLines);	
}

bool ActivationEventsCanvas::DoLoop(gedString actorFrom, gedString actorTo, gedString *begin)
{
	//Loop detection
	if(!begin)
		begin = &actorFrom;

	ListEventLineIterator itList(listEventLines);	
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		if(itList.Current().actorFrom == actorTo)
		{
			if(itList.Current().actorTo == *begin)
				return true;

			if(DoLoop(itList.Current().actorFrom, itList.Current().actorTo, begin))
				return true;
		}
	}

	return false;
}


#ifndef STAND_ALONE_GAME
void ActivationEventsCanvas::ActorRemoved(const gedString& cloneName)
{
	//Remove the activation event if a removed actor
	//send or receive it
	
	bool bRemoved;

	do
	{
		bRemoved = false;
		ListEventLineIterator itList(listEventLines);
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(itList.Current().actorFrom == cloneName || itList.Current().actorTo == cloneName)
			{
				Actor *actorFrom = GameControl::Get()->GetActor(itList.Current().actorFrom);
				if(actorFrom)
				{
					actorFrom->getAction()->RemoveAction(itList.Current().action);
					RemoveLine(stLine(itList.Current().actorFrom, itList.Current().actorTo, NULL));
					
					bRemoved = true;
					break;
				}			
			}
		}
	} while(bRemoved);
}

bool ActivationEventsCanvas::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(list) list->getImage()->SetVisible(false);

	if(index == 0)
	{
		if(hitAction)
		{
			PanelQuestion *panel = new PanelQuestion("Remove Activation Event?");
			if(panel->Wait() == OK_BUTTON)
			{
				Actor *actorFrom = GameControl::Get()->GetActor(hitAction->name);
				if(actorFrom)	
				{
					actorFrom->getAction()->RemoveAction(hitAction);
					RemoveLine(stLine(hitAction->name, hitAction->actorName, NULL));

					ActorProperty::SetEditEvents();
				}
			}
			
			delete panel;
		}
	}
	
	return true;
}
#endif //#ifndef STAND_ALONE_GAME