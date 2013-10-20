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


// ActorEdit.cpp: implementation of the ActorEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "ActorEdit.h"
#include "GameControl.h"

#ifndef STAND_ALONE_GAME
#include "../gameEditor/ActorProperty.h"
#include "../gameEditor/MainPanel.h"
#endif //#ifndef STAND_ALONE_GAME

enum
{
	LS_ACTOR_MENU,
	LS_ACTOR_ACTIVATION_EVENTS
};

#ifndef STAND_ALONE_GAME
//ListPop *ActorEdit::listMenu = NULL;
ListPop *ActorEdit::listActivationEvents = NULL;
void AddToPanelActors(Actor *_actor, bool bSendMesage);
void RemoveFromPanelActors(Actor *_actor);
#endif //#ifndef STAND_ALONE_GAME

#ifndef STAND_ALONE_GAME
ActorEdit *ActorEdit::activationEventActor = NULL;
bool ActorEdit::bActivationEventMode = false;
MapString ActorEdit::mapActivationEventActors;
#endif

ActorEdit::ActorEdit(const gedString& spriteName, Actor *_parent, bool _bView, ActorType _type, int canvasWidth, int canvasHeight)
:	Actor(spriteName, _parent, _bView?VIEW_ACTOR:_type, canvasWidth, canvasHeight)
{
#ifndef STAND_ALONE_GAME
	EditorDirectory editDir;
#endif

	InitEdit();

#ifndef STAND_ALONE_GAME
	listMenu = NULL;	
	AddToPanelActors(this, true);
#endif	
}

ActorEdit::~ActorEdit()
{
	if(ActivationEventsCanvas::HasActivationEventLines(getCloneName()))
	{
		GameControl::Get()->GetAxis()->getImage()->Invalidate();
	}

#ifndef STAND_ALONE_GAME
	if(listMenu)
	{
		delete listMenu;
		listMenu = NULL;
	}

	if(listActivationEvents)
	{
		delete listActivationEvents;
		listActivationEvents = NULL;
	}

	if(activationEventActor == this)
	{
		activationEventActor = NULL;
	}

	RemoveFromPanelActors(this);
#endif //#ifndef STAND_ALONE_GAME
}

bool ActorEdit::OnMouseButtonDown(int x, int y, Uint8 button)
{
  if(GameControl::Get()->getGameMode())
  {
    return Actor::OnMouseButtonDown(x, y, button);
  }

#ifndef STAND_ALONE_GAME

  GameControl::Get()->setModified();

  if(bActivationEventMode)
  {
    if(button == SDL_BUTTON_LEFT && activationEventActor != this)
    {
      //Receive activation event from activationEventActor
      gedString actorsName(activationEventActor->getCloneName());
      actorsName += getCloneName();

      if(!mapActivationEventActors[actorsName]) //No duplicates
      {
	if(!ActivationEventsCanvas::DoLoop(activationEventActor->getCloneName(), getCloneName())) //No loops
	{
	  mapActivationEventActors.Add(actorsName, 1);
	  ActivationEventsCanvas::AddLine(Action::Call(activationEventActor)->SetActivationEvent(activationEventActor->getCloneName(), getCloneName()));
	  GameControl::Get()->GetAxis()->getImage()->Invalidate();
	}
	else
	{
	  new PanelInfo("Closed loop detected.\nPlease, choose another actor.");
	}
      }
      return false;
    }
  }
  else
  {
    SDLMod keyMod = SDL_GetModState();
    Uint8 *keystate = SDL_GetKeyState(NULL);	

    // check to delete
    if(button == SDL_BUTTON_LEFT && (keyMod & KMOD_LCTRL) && (keystate[SDLK_d]))
    {
			ActorProperty::getActorProperty()->RemoveActor(this);
			return false;
    }
    else if(button == SDL_BUTTON_RIGHT)
    {
      ListMenu(x, y, button);	
      return false;
    }
    else
    {
      ActorProperty::Call(this, false);

      if(!flags.IsSet(FLAG_LOCKACTOR))
      {
	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS) 
	  return Actor::OnMouseButtonDown(x, y, button);
      }

      Tile *actualTile = GameControl::Get()->GetTile();
      if(tile && tile == actualTile)
      {
	if(tile->AddTile())
	{
	  tile->setAdd(true);
	  return false;
	}
      }
    }
	  
    if(button == SDL_BUTTON_RIGHT)
    {
      ListMenu(x, y, button);	
      return false;
    }
    else
    {
      ActorProperty::Call(this, false);

      if(!flags.IsSet(FLAG_LOCKACTOR))
      {
	if(type == REGION_ACTOR || type == REGION_ACTOR_FILLED || type == CANVAS) 
	  return Actor::OnMouseButtonDown(x, y, button);
      }

      Tile *actualTile = GameControl::Get()->GetTile();
      if(tile && tile == actualTile)
      {
	if(tile->AddTile())
	{
	  tile->setAdd(true);
	  return false;
	}
      }
    }
  }

  return !flags.IsSet(FLAG_LOCKACTOR);

#else	
  return true;
#endif //#ifndef STAND_ALONE_GAME
}

const stCursor * ActorEdit::OnGetCursor()
{
	if(GameControl::Get()->getGameMode())
	{
		return Actor::OnGetCursor();
	}
	
#ifndef STAND_ALONE_GAME
	cursor.imgName	= "makscursorMao.bmp";
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
#endif //#ifndef STAND_ALONE_GAME
	
	return &cursor;
}

const gedString *ActorEdit::OnActorTip()
{
	if(GameControl::Get()->getGameMode())
		return NULL;

#ifndef STAND_ALONE_GAME
	actorTip = getCloneName();

	Tile *actualTile = GameControl::Get()->GetTile();
	if(tile && tile == actualTile)
	{
		char buf[32];
		sprintf(buf, ", Tile: %ld", tile->GetFrame());
		actorTip += buf;
	}

	return &actorTip;	
#else
	return NULL;
#endif //#ifndef STAND_ALONE_GAME
}


void ActorEdit::OnMouseButtonUp(int x, int y, Uint8 button)
{
	if(GameControl::Get()->getGameMode())
	{
		Actor::OnMouseButtonUp(x, y, button);
	}
#ifndef STAND_ALONE_GAME
	else
	{
		//if(!pathX && !pathY) //With this if, don't save actors position with paths
		{
			SetPathIni(getImage()->X(), getImage()->Y());			
		}

		Tile *actualTile = GameControl::Get()->GetTile();
		if(tile && tile == actualTile)
		{
			tile->setAdd(false);
		}

	}
#endif //#ifndef STAND_ALONE_GAME

	if(change == this && type == CANVAS)
	{
		getCanvas()->Refresh();
	}

	change = NULL; //Region
}

void ActorEdit::OnAnimation()
{
	if(GameControl::Get()->getGameMode())
	{
		Actor::OnAnimation();
	}
}

void ActorEdit::OnAnimationFinish()
{
	if(GameControl::Get()->getGameMode())
	{
		Actor::OnAnimationFinish();
	}
}

void ActorEdit::OnPathFinish()
{
	if(GameControl::Get()->getGameMode())
	{
		Actor::OnPathFinish();
	}
}

void ActorEdit::OnKeyDown(SDLKey key, int repeatLoopCount)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		Actor::OnKeyDown(key, repeatLoopCount);
	}
#ifndef STAND_ALONE_GAME
	else
	{
		Tile *actualTile = GameControl::Get()->GetTile();
		if(tile && tile == actualTile)
		{
			tile->OnKeyDown(key);	
		}
	}
#endif
}
	
void ActorEdit::OnKeyUp(SDLKey key)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		Actor::OnKeyUp(key);
	}
}

void ActorEdit::OnMouseLeave(int x, int y)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		Actor::OnMouseLeave(x, y);
	}
}

void ActorEdit::OnMouseEnter(int x, int y)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		Actor::OnMouseEnter(x, y);
	}
}

void ActorEdit::OnCollision(Actor *collideActor)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		Actor::OnCollision(collideActor);
	}
}


Uint32 ActorEdit::OnTimer(SDL_TimerID timerID)
{
	if(GameControl::Get()->getStandAloneMode() && getRunning())
	{
		return Actor::OnTimer(timerID);
	}

	return 0;
}



#ifndef STAND_ALONE_GAME
void ActorEdit::ListMenu(int x, int y, Uint8 button)
{
	if(!listMenu)
	{
		listMenu = new ListPop(0, 0, 1, 1, LS_ACTOR_MENU, MainPanel::Get());
		listMenu->AddText("Actor Control");
		listMenu->AddText("New Activation Event");		
		listMenu->AddText("Show/Hide Activation Events");
		
		if(flags.IsSet(FLAG_LOCKACTOR)) listMenu->AddText("Unlock Actor");
		else  listMenu->AddText("Lock Actor");
		listMenu->AddText("Delete Actor");

#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL) && !defined(STAND_ALONE_GAME) && !defined(GAME_EDITOR_HOME_EDITION)
		listMenu->AddText(BUY_NOW); //Must be the last item to avoid turorial bugs
#endif
	}	

	listMenu->SetParent(this);
	listMenu->getImage()->SetVisible(true);
	listMenu->OnMouseButtonDown(x, y, button);
}

bool ActorEdit::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(GameControl::Get()->getGameMode())
	{
		return Actor::OnList(list, index, text, listId);
	}
	
	if(listMenu) 
	{
		//listMenu->getImage()->SetVisible(false);
		delete listMenu;
		listMenu = NULL;
	}

	if(listActivationEvents) listActivationEvents->getImage()->SetVisible(false);

	if(listId == LS_ACTOR_MENU)
	{
		switch(index)
		{
			case 0:
				ActorProperty::Call(this);
				break;
			case 1:
			{
				if(!listActivationEvents)
				{
					listActivationEvents = new ListPop(0, 0, 1, 1, LS_ACTOR_ACTIVATION_EVENTS, this);
					Action::PopulateEvents(listActivationEvents);
					listActivationEvents->getImage()->SetVisible(false);
				}
				
				activationEventActor = this;
				listActivationEvents->SetParent(this);
				listActivationEvents->getImage()->SetVisible(true);
				//listActivationEvents->setListName("ListPopActivationEvent");
				listActivationEvents->OnMouseButtonDown(GameControl::Get()->getMouseX(), GameControl::Get()->getMouseY(), 0);
			}
			break;

			case 2:
				ActivationEventsCanvas::ToggleVisibility();
				break;

			case 3:
				flags.Set(FLAG_LOCKACTOR, !flags.IsSet(FLAG_LOCKACTOR));
				break;

		  case 4: // remove actor
				ActorProperty::getActorProperty()->RemoveActor(this);
				break;
		}
	}
	else if(listId == LS_ACTOR_ACTIVATION_EVENTS)
	{
		Action::DoEvent(this, text, true);
	}
	
	return true;
}

void ActorEdit::DoActivationEvent(bool bActivate)
{
	bActivationEventMode = bActivate;
	mapActivationEventActors.Clear();

	if(!bActivationEventMode)	
	{
		GameControl::Get()->GetAxis()->getImage()->Invalidate();
	}
}

#endif //#ifndef STAND_ALONE_GAME
