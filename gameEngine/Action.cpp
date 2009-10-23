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


// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

#include "Action.h"
#include "GameControl.h"
#include "Script.h"
#include "compression.h"
#include <math.h>
#include "Path.h"
#include "RegionLoad.h"

#include "../RakNet/Source/Rand.h"
#include "../RakNet/Source/GetTime.h"

#ifndef STAND_ALONE_GAME

#include "../gameEditor/ChangeAnimation.h"
#include "../gameEditor/EventKeyDown.h"
#include "../gameEditor/ChangePath.h"
#include "../gameEditor/ChangeParent.h"
#include "../gameEditor/ChangeCursor.h"
#include "../gameEditor/CreateActor.h"
#include "../gameEditor/DestroyActor.h"
#include "../gameEditor/DlgCollision.h"
#include "../gameEditor/DlgTimer.h"
#include "../gameEditor/CreateTimer.h"
#include "../gameEditor/DestroyTimer.h"
#include "../gameEditor/DlgGeneralEvent.h"
#include "../gameEditor/ChangeAnimationDirection.h"
#include "../gameEditor/DlgMouseButton.h"
#include "../gameEditor/EnableDisable.h"
#include "../gameEditor/Transparency.h"
#include "../gameEditor/ToMousePosition.h"
#include "../gameEditor/ExpressionEditor.h"
#include "../gameEditor/ActorText.h"
#include "../gameEditor/ConditionalAction.h"
#include "../gameEditor/SetZDepth.h"
#include "../gameEditor/PlaySong.h"
#include "../gameEditor/EventControl.h"
#include "../gameEditor/ActorProperty.h"
#include "../gameEditor/ToAnteriorPosition.h"
#include "../gameEditor/DlgAnimationFinish.h"
#include "../gameEditor/MoveTo.h"
#include "../gameEditor/DlgActivationEvent.h"
#include "../gameEditor/PhysicalBounce.h"
#include "../gameEditor/Tutorial.h"
#include "../gameEditor/ActorText.h"
#include "../wxGameEditor/GameGraph.h"


#endif //#ifndef STAND_ALONE_GAME

#include "dlmalloc.h"


double distance(double x1, double y1, double x2, double y2);

stEvent Action::actualEvent;
bool Action::bConditionalAction = false;
bool Action::bActivationEvent = false;
bool Action::bInCreateActorEvent = false;
bool Action::bInCreateActorAction = false;
bool Action::bNetActorCreated = false;
bool Action::bUserInput = false;
stAction *Action::editAction = NULL;

Actor *Action::actualEventActor = NULL;
Actor *Action::actualCollideActor = NULL;
//Action *Action::actualAction = NULL;

Uint32 emulatedTimeStamp = 0; //Faster than SDL_GetTicks()

/*StackActors Action::stackActors;
StackActions Action::stackActions;*/

GlDynArray<void *> ActionInfo::freeList;

MapActorsInCollision Action::mapActorsInCollision;

GlobalMapActions globalMapActions, mapActionsKeyDown, mapActionsKeyUp;

gedString ANY_ACTOR("Any Actor");
gedString ALL_ACTORS("All Actors");
gedString ANY_ANIMATION("Any Animation");
gedString ANY_PATH("Any Path");

GlobalMapActions &Action::getGlobalMapActions()
{
	return globalMapActions;
}

GlobalMapActions &Action::getActionsKeyDown()
{
	return mapActionsKeyDown;
}

GlobalMapActions &Action::getActionsKeyUp()
{
	return mapActionsKeyUp;
}


Action::Action(const gedString& name)
{
	actionName = name;
	nRef = 0;
	listActions = NULL;
	nActions = 0;
	actionAnt = NULL;

	//Default must be false
/*#ifdef STAND_ALONE_GAME
	bOutOfVisionOptimization = true;
#else*/
	/*if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		bOutOfVisionOptimization = true;
	}
	else*/
	{
		//Don't optimize old tutorials
		bOutOfVisionOptimization = false;
	}
//#endif

	bHasCollisionWithAnyActor = false;
	
	globalMapActions.Add(actionName, this);
}

Action::Action()
{
	nRef = 0;
	listActions = NULL;
	nActions = 0;
	actionAnt = NULL;

	//Default must be false
/*#ifdef STAND_ALONE_GAME
	bOutOfVisionOptimization = true;
#else*/
	/*if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		bOutOfVisionOptimization = true;
	}
	else*/
	{
		//Don't optimize old tutorials
		bOutOfVisionOptimization = false;
	}
//#endif

	bHasCollisionWithAnyActor = false;	
}

Action::~Action()
{
	stAction *aux = listActions;
	while(aux)
	{
		if(listActions) aux = listActions->next;
		else aux = NULL;

		//Don't remove action script in game mode (parser again genarate leaks)
		if(!GameControl::Get()->getGameMode())
		{
			if(listActions)
			{
				if(listActions->actionType == SET_EXPRESSION)
				{
					GameControl::Get()->RemoveExpression(listActions->data.name);
				}
				else if(listActions->actionType == SET_CONDITIONAL_ACTION)
				{
					GameControl::Get()->RemoveExpression(listActions->data.conditionalaction.expression1);
				}
			}
		}

		//Delete action
		delete listActions;
		listActions = aux;
	}

#ifdef STAND_ALONE_GAME
	//Release Inherited Events
	Action *pActionInherited = getInheritedEvents();
	if(pActionInherited)
	{
		pActionInherited->DetachAction();
	}
#endif
}

int Action::GetActionIndex(stAction *action)
{
	int i = 0;

	stAction *pAction = listActions;
	while(pAction)
	{
		if(pAction == action)
		{
			return i;
		}

		pAction = pAction->next;
		i++;
	}

	return -1;
}

stAction *Action::GetActionByIndex(int n)
{
	stAction *pAction = listActions;
	int i = 0;

	while(pAction)
	{
		if(i == n)
		{
			return pAction;
		}

		pAction = pAction->next;
		i++;
	}

	return NULL;
}

stAction *Action::newAction(gedString actionActor, int actionType, ListActions *pUpdateList)
{
	stAction *action = editAction;
	
	if(!action)
	{
		action = new stAction();		
	}

	action->eventData = actualEvent;
	action->bConditional = bConditionalAction;

	if(!actionActor.empty())
	{
		strcpy(action->actorName, actionActor.c_str());
	}
	
	action->actionType = actionType;

	if(!editAction)
	{
		if(!listActions) listActions = action;
		if(actionAnt) actionAnt->next = action;

		actionAnt = action;
		ToEvent(action);

#ifndef STAND_ALONE_GAME
		ActorProperty::SetEditEvents();
#endif //#ifndef STAND_ALONE_GAME

		nActions++;
	}	
	else if(pUpdateList)
	{
		ToEvent(action, pUpdateList);
	}

	if(action->actionType != SET_CONDITIONAL_ACTION) clearEditAction();
	return action;
}


bool Action::setInheritedEvents(gedString actorName) 
{
	//Save the name, not the pointer
	//The system can reuse a pointer and causes a error in the game
	//So, the pointer can point to a different actor, after destroy the creator

	if(actorName == actionName)
		return false;

	if(!actorName.empty())
	{
		//Verify cyclical parent
		Action **parent = globalMapActions[actorName];
		if(!parent) 
			return false;

		Action *testParent = *parent;	

		while(testParent)
		{
			if(testParent == this)
				return false;	
			
			testParent = testParent->getInheritedEvents();
		} 
	}

	InheritedEvents = actorName;

	return true;
}

bool Action::CanSetInheritedEvents(const gedString& _actionName, gedString parentName) 
{
	if(_actionName == parentName)
		return false;
	
	
	//Verify cyclical parent
	Action **parent = globalMapActions[parentName];
	if(parent) 
	{
		while(parent && *parent && !parentName.empty())
		{
			parentName = (*parent)->getInheritedEventsActorName();
			if(_actionName == parentName)
				return false;				
			
			parent = globalMapActions[parentName];
		}
	}
	
	
	return true;
}

Action *Action::getInheritedEvents() 
{
	if(!InheritedEvents.empty())
	{
		Action **p = globalMapActions[InheritedEvents];
		if(p)
			return *p;
	}

	return NULL;
}

bool Action::HaveKeyDown(bool bGetInheritedEvents) 
{
	if(mapKeyDown.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveKeyDown(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveKeyUp(bool bGetInheritedEvents) 
{
	if(mapKeyUp.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveKeyUp(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveCollisions(bool bGetInheritedEvents) 
{
	if(mapCollision.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveCollisions(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveCollisionFinish(bool bGetInheritedEvents) 
{
	if(mapCollisionFinish.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveCollisionFinish(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveCollisionWithAnyActor() 
{
	if(bHasCollisionWithAnyActor) return true;	
	
	Action *pInheritedAction = getInheritedEvents();
	if(pInheritedAction)
	{
		return pInheritedAction->HaveCollisionWithAnyActor();
	}
	
	return false;
}

bool Action::HaveActivationEvent(bool bGetInheritedEvents) 
{
	if(mapActivationEvent.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveActivationEvent(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveTimer(bool bGetInheritedEvents) 
{
	if(mapTimerNameAction.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveTimer(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveAnimation(bool bGetInheritedEvents) 
{
	if(listAnimationActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveAnimation(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveAnimationFinish(bool bGetInheritedEvents) 
{
	if(mapAnimationFinishActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveAnimationFinish(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HavePathFinish(bool bGetInheritedEvents) 
{
	if(mapPathFinishActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HavePathFinish(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveCreate(bool bGetInheritedEvents) 
{
	if(listCreateActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveCreate(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveDestroyActor(bool bGetInheritedEvents) 
{
	if(listDestroyActorActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveDestroyActor(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveMouseEnter(bool bGetInheritedEvents) 
{
	if(listMouseEnterActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveMouseEnter(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveMouseLeave(bool bGetInheritedEvents) 
{
	if(listMouseLeaveActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveMouseLeave(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveMoveFinish(bool bGetInheritedEvents) 
{
	if(listMoveFinishActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveMoveFinish(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveOutOfVision(bool bGetInheritedEvents) 
{
	if(listOutOfVisionActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveOutOfVision(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveMouseButtonDown(bool bGetInheritedEvents) 
{
	if(mapMouseButtonDownActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveMouseButtonDown(bGetInheritedEvents);
		}
	}
	
	return false;
}

bool Action::HaveMouseUpActions(bool bGetInheritedEvents) 
{
	if(mapMouseUpActions.size() > 0) return true;
	
	if(bGetInheritedEvents)
	{
		Action *pInheritedAction = getInheritedEvents();
		if(pInheritedAction)
		{
			return pInheritedAction->HaveMouseUpActions(bGetInheritedEvents);
		}
	}
	
	return false;
}

void Action::OnActivationEvent(Actor *eventActor, Actor *fromActor)
{
	//Store time stamp
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	ExecuteActivationEventActions(eventActor, fromActor->getCloneName(), timeStamp, EXECUTE_ALL);
}

void Action::ExecuteActivationEventActions(Actor *eventActor, char *cloneName, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_CLONE_NAME)
	{
		//From specified cloned actor?	
		ListActionsIterator itList(mapActivationEvent.FindString(cloneName));
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_CLONE_NAME; 
		}
	}
	
	if(execFlags & EXEC_ANY_ACTOR)
	{
		//From others actors? (Logical Or)
		ListActionsIterator itList(mapActivationEvent[ANY_ACTOR]);

		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_ANY_ACTOR; 
		}
	}

	if(execFlags & EXEC_ALL_ACTORS)
	{
		//From all actors? (Logical And)
		ListActionsIterator itList(mapActivationEvent[ALL_ACTORS]);
		
		
		//Execute only when receive Activation Events from all actors connected to him
		if(eventActor->CanExecuteAllActorsActivationEventAction(cloneName))
		{
			for(itList.Begin(); !itList.Done(); itList.Next())
			{
				stAction *action = itList.Current();
				action->timeStamp = timeStamp;
				Execute(action, eventActor);

				execFlags &= ~EXEC_ALL_ACTORS;
			}
		}
	}
	
	if((execFlags & EXEC_CLONE_NAME) || (execFlags & EXEC_ANY_ACTOR) || (execFlags & EXEC_ALL_ACTORS))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteActivationEventActions(eventActor, cloneName, timeStamp, execFlags);
		}
	}
}

Uint32 Action::OnTimer(Actor *eventActor, SDL_TimerID timerID)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	stTimerEvent *p = mapTimerIdName[timerID];
	if(p)
	{	
		int interval = p->miliseconds;
		
		ExecuteTimerActions(eventActor, p->name, timeStamp);				
		
		return interval; //p can be destroyed
	}
	
	return 0;
}

void Action::ExecuteTimerActions(Actor *eventActor, char *timerName, Uint32 timeStamp)
{
	bool bExecuted = false;

	ListActionsIterator itList(mapTimerNameAction.FindString(timerName));
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;

		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteTimerActions(eventActor, timerName, timeStamp);
		}
	}
}



void Action::OnKeyDown(Actor *eventActor, SDLKey key, int repeatLoopCount)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = true;

	ExecuteKeyDownActions(eventActor, key, repeatLoopCount, timeStamp, EXECUTE_ALL);	
}

void Action::ExecuteKeyDownActions(Actor *eventActor, SDLKey key, int repeatLoopCount, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_KEY)		
	{
		//Specified key
		ListActionsIterator itList(mapKeyDown[key]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			
			if(!repeatLoopCount || !action->eventData.data.keyEvent.bDisableRepeat) 
			{
				action->timeStamp = timeStamp;
				Execute(action, eventActor);
				
				execFlags &= ~EXEC_KEY;
				execFlags &= ~EXEC_AT_LEAST_ONE_KEY_IS_PRESSED;
			}			
		}
	}
	
	
	if(execFlags & EXEC_ANY_KEY)		
	{
		//Any key
		ListActionsIterator itList(mapKeyDown[SDLK_ANY_KEY]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			
			if(!repeatLoopCount || !action->eventData.data.keyEvent.bDisableRepeat) 
			{
				action->timeStamp = timeStamp;
				Execute(action, eventActor);

				execFlags &= ~EXEC_ANY_KEY;
			}			
		}
	}	

	if(repeatLoopCount <= 1)
	{
		//Execute only if is not in repeat loop or if the first key
		
		if(execFlags & EXEC_AT_LEAST_ONE_KEY_IS_PRESSED)
		{
			//At least one key is pressed
			ListActionsIterator itList(mapKeyDown[SDLK_AT_LEAST_ONE_KEY_IS_PRESSED]);
			for(itList.Begin(); !itList.Done(); itList.Next())
			{
				stAction *action = itList.Current();
				
				if(!repeatLoopCount || !action->eventData.data.keyEvent.bDisableRepeat) 
				{
					for(int i = 0; i < MAX_KEY_SEQUENCE; i++)
					{
						SDLKey key1 = (SDLKey)action->eventData.data.keyEvent.keySequence[i];
						if(!key1) break;
						
						if(GameControl::Get()->getMapKeyDown().Find1(key1))
						{
							action->timeStamp = timeStamp;
							Execute(action, eventActor);

							execFlags &= ~EXEC_KEY;
							execFlags &= ~EXEC_AT_LEAST_ONE_KEY_IS_PRESSED;
							break;
						}
					}
				}			
			}
		}

		
		if(execFlags & EXEC_ALL_KEYS_PRESSED)
		{
			//All keys pressed
			ListActionsIterator itList(mapKeyDown[SDLK_ALL_KEYS_PRESSED]);
			for(itList.Begin(); !itList.Done(); itList.Next())
			{
				stAction *action = itList.Current();
				
				if(!repeatLoopCount || !action->eventData.data.keyEvent.bDisableRepeat) 
				{
					bool bAllKeysPressed = true;
					
					for(int i = 0; i < MAX_KEY_SEQUENCE; i++)
					{
						SDLKey key1 = (SDLKey)action->eventData.data.keyEvent.keySequence[i];
						if(!key1) break;
						
						if(!GameControl::Get()->getMapKeyDown().Find1(key1))
						{
							bAllKeysPressed = false;
							break;
						}
					}
					
					if(bAllKeysPressed)
					{
						action->timeStamp = timeStamp;
						Execute(action, eventActor);

						execFlags &= ~EXEC_ALL_KEYS_PRESSED;
					}
				}			
			}
		}

		
		if(execFlags & EXEC_KEYS_PRESSED_IN_ORDER)
		{
			//Keys pressed in order 
			ListActionsIterator itList(mapKeyDown[SDLK_KEYS_PRESSED_IN_ORDER]);
			for(itList.Begin(); !itList.Done(); itList.Next())
			{
				stAction *action = itList.Current();
				
				if(!repeatLoopCount || !action->eventData.data.keyEvent.bDisableRepeat) 
				{
					LastKeysIterator itKeys(GameControl::Get()->getLastKeys());	
					int i = MAX_KEY_SEQUENCE - 1;
					bool bCorrectOrder = true;
					
					//Get the last valid key
					while(i >= 0 && !action->eventData.data.keyEvent.keySequence[i]) i--;
					
					//Check the sequence from back to front
					for(itKeys.Begin(); !itKeys.Done() && i >= 0; itKeys.Next())
					{
						SDLKey key1 = (SDLKey)action->eventData.data.keyEvent.keySequence[i--];
						if(!key1) break;
						
						if(key1 != itKeys.Current())
						{
							bCorrectOrder = false;
							break;
						}
					}
					
					if(bCorrectOrder)
					{
						action->timeStamp = timeStamp;
						Execute(action, eventActor);

						execFlags &= ~EXEC_KEYS_PRESSED_IN_ORDER;
					}
				}			
			}
		}
	}



	if(
		(execFlags & EXEC_KEY) || 
		(execFlags & EXEC_ANY_KEY) ||
		(execFlags & EXEC_AT_LEAST_ONE_KEY_IS_PRESSED) ||
		(execFlags & EXEC_ALL_KEYS_PRESSED) ||
		(execFlags & EXEC_KEYS_PRESSED_IN_ORDER)
	  )
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteKeyDownActions(eventActor, key, repeatLoopCount, timeStamp, execFlags);
		}
	}
}

void Action::OnKeyUp(Actor *eventActor, SDLKey key)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = true;

	ExecuteKeyUpActions(eventActor, key, timeStamp, EXECUTE_ALL);	
}

void Action::ExecuteKeyUpActions(Actor *eventActor, SDLKey key, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_KEY)		
	{
		//Specified key
		ListActionsIterator itList(mapKeyUp[key]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_KEY; 
		}
	}
	
	if(execFlags & EXEC_ANY_KEY)		
	{
		//Any key
		ListActionsIterator itList(mapKeyUp[SDLK_ANY_KEY]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_ANY_KEY; 
		}
	}	

	if((execFlags & EXEC_KEY) || (execFlags & EXEC_ANY_KEY))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteKeyUpActions(eventActor, key, timeStamp, execFlags);
		}
	}
}


bool Action::OnMouseButtonDown(Actor *eventActor, int x, int y, Uint8 button)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = true;

	return ExecuteMouseButtonDownActions(eventActor, button, timeStamp);	
}

bool Action::ExecuteMouseButtonDownActions(Actor *eventActor, Uint8 button, Uint32 timeStamp)
{
	bool bExecuted = false, bDrag = false;

	
	stAction *pAction;
	ListActionsIterator itList(mapMouseButtonDownActions[button]);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		pAction = itList.Current();
		pAction->timeStamp = timeStamp;
		Execute(pAction, eventActor);
		//if(pAction->bDrag) bDrag = true; //Causes the bug "don't save relative to creator when call Create Actor inside mouse button down event"
		if(pAction->eventData.data.mouse.bDrag && eventActor->CanExecuteActionLocally()) bDrag = true;
		bExecuted = true;
	}
		
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			return pInheritedActions->ExecuteMouseButtonDownActions(eventActor, button, timeStamp);
		}
	}

	return bDrag;
}

void Action::OnMouseButtonUp(Actor *eventActor, int x, int y, Uint8 button)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = true;
	
	ExecuteMouseButtonUpActions(eventActor, button, timeStamp);		
}

void Action::ExecuteMouseButtonUpActions(Actor *eventActor, Uint8 button, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(mapMouseUpActions[button]);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
		
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteMouseButtonUpActions(eventActor, button, timeStamp);
		}
	}
}

void Action::OnMessage(Actor *eventActor, Actor *fromActor, Uint32 messageId, Uint32 message)
{
}

void Action::OnAnimation(Actor *eventActor)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	ExecuteAnimationActions(eventActor, timeStamp);
}

void Action::ExecuteAnimationActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;

	ListActionsIterator itList(listAnimationActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteAnimationActions(eventActor, timeStamp);
		}
	}
}


void Action::OnAnimationFinish(Actor *eventActor)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	ExecuteAnimationFinishActions(eventActor, timeStamp, EXECUTE_ALL);		
}

void Action::ExecuteAnimationFinishActions(Actor *eventActor, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_ANIMATION)
	{
		ListActionsIterator itList(mapAnimationFinishActions[eventActor->GetAnimation()]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_ANIMATION; 
		}	
	}
	
	if(execFlags & EXEC_ANY_ANIMATION)
	{
		ListActionsIterator itList(mapAnimationFinishActions[ANY_ANIMATION]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_ANY_ANIMATION;
		}
	}
	
	if((execFlags & EXEC_ANIMATION) || (execFlags & EXEC_ANY_ANIMATION))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteAnimationFinishActions(eventActor, timeStamp, execFlags);
		}
	}
}

void Action::OnPathFinish(Actor *eventActor)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();	
	bUserInput = false;

	Path *path = eventActor->getPathX();
	if(!path) path = eventActor->getPathY();
	if(!path) return; //Never must happen					
	
	ExecutePathFinishActions(eventActor, path, timeStamp, EXECUTE_ALL);	
}

void Action::ExecutePathFinishActions(Actor *eventActor, Path *path, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_PATH)
	{
		ListActionsIterator itList(mapPathFinishActions[path->getName()]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_PATH;
		}
	}
	
	if(execFlags & EXEC_ANY_PATH)
	{
		ListActionsIterator itList(mapPathFinishActions[ANY_PATH]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor);

			execFlags &= ~EXEC_ANY_PATH;
		}
	}
	
	if((execFlags & EXEC_PATH) || (execFlags & EXEC_ANY_PATH))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecutePathFinishActions(eventActor, path, timeStamp, execFlags);
		}
	}
}

void Action::OnCreate(Actor *eventActor)
{
	if(
		//Don't call OnCreate twice
		//Solve created_actor_creating_actors_bug.ged
		eventActor->getOnCreateCalled() ||

		//Only call is running
		//Avoid call in the load time on the create actor action
		!eventActor->getRunning()
	  ) 
	{		
		return; 
	}

	bUserInput = false;
	bInCreateActorEvent = true;
	eventActor->setOnCreateCalled();	

	if(eventActor->getEventControl() & EVENTCREATE)
	{
		Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();

		ExecuteCreateActions(eventActor, timeStamp);	
	}

	bInCreateActorEvent = false;
}

void Action::ExecuteCreateActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;

	ListActionsIterator itList(listCreateActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteCreateActions(eventActor, timeStamp);
		}
	}
}

void Action::OnDestroyActor(Actor *eventActor)
{
	if(GameControl::Get()->getDestroingGame()) return; //End of game mode

	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	if(eventActor->getEventControl() & EVENTDESTROYACTOR)
	{
		ExecuteDestroyActorActions(eventActor, timeStamp);		
	}
}

void Action::ExecuteDestroyActorActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(listDestroyActorActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteDestroyActorActions(eventActor, timeStamp);
		}
	}
}

void Action::OnOutOfVision(Actor *eventActor)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	if(eventActor->getEventControl() & EVENTOUTOFVISION)
	{
		ExecuteOutOfVisionActions(eventActor, timeStamp);		
	}
}

void Action::ExecuteOutOfVisionActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(listOutOfVisionActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteOutOfVisionActions(eventActor, timeStamp);
		}
	}
}

void Action::OnMoveFinish(Actor *eventActor)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	ExecuteMoveFinisActions(eventActor, timeStamp);	
}

void Action::ExecuteMoveFinisActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(listMoveFinishActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteMoveFinisActions(eventActor, timeStamp);
		}
	}
}

void Action::OnMouseEnter(Actor *eventActor, int x, int y)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();	
	bUserInput = true;

	if(eventActor->getEventControl() & EVENTMOUSEENTER)
	{
		ExecuteMouseEnterActions(eventActor, timeStamp);		
	}
}

void Action::ExecuteMouseEnterActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(listMouseEnterActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);
		
		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteMouseEnterActions(eventActor, timeStamp);
		}
	}
}

void Action::OnMouseLeave(Actor *eventActor, int x, int y)
{
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = true;

	if(eventActor->getEventControl() & EVENTMOUSELEAVE)
	{
		ExecuteMouseLeaveActions(eventActor, timeStamp);		
	}
}

void Action::ExecuteMouseLeaveActions(Actor *eventActor, Uint32 timeStamp)
{
	bool bExecuted = false;
	
	ListActionsIterator itList(listMouseLeaveActions);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		stAction *action = itList.Current();
		action->timeStamp = timeStamp;
		Execute(action, eventActor);

		bExecuted = true;
	}
	
	if(!bExecuted)
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteMouseLeaveActions(eventActor, timeStamp);
		}
	}
}

void Action::OnMouseMove(Actor *eventActor, int x, int y)
{
}



Action *Action::Call(Actor *actor, bool bCreate)
{
	if(!actor->getAction())
	{
		Action *action = NULL;
		Action **p = globalMapActions.FindString(actor->getActorName());
		if(p) action = *p;

		if(!action)
		{		
			if(!bCreate)
			{
				//Try load from disk				
				action = Load(actor->getActorName());
				if(!action)
					return NULL;
			}
			else
				action = new Action(actor->getActorName());
		}


		actor->setAction(action);

		//Set clones
		ListActor *listActor = mapActors.FindString(actor->getActorName());
		if(listActor)
		{		
			for(int il = 0; il < listActor->Count(); il++)
			{
				Actor *pClone = (*listActor)[il];
				if(pClone && !pClone->getAction())
				{
					pClone->setAction(actor->getAction());
				}			
			}
		}
	}

	return actor->getAction();
}

Action *Action::Call(const gedString& name)
{
	Action *action = NULL, **p = globalMapActions[name];
	if(p) action = *p;

	if(!action)
	{		
		//Try load from disk				
		action = Load(name);	
	}

	return action;
}





void Action::ToEvent(stAction *action, ListActions *pUpdateList)
{
	ListActions *pNewList = NULL;

	switch(actualEvent.eventType)
	{
	case EVENT_KEYDOWN:
		{
			pNewList = mapKeyDown.FindOrCreate(actualEvent.data.keyEvent.key);			
			mapActionsKeyDown.Add(actionName, this);
		}
		break;
	case EVENT_KEYUP:
		{
			pNewList = mapKeyUp.FindOrCreate(actualEvent.data.keyEvent.key);
			mapActionsKeyUp.Add(actionName, this);
		}
		break;
	case EVENT_COLLISION:
		{
			pNewList = mapCollision.FindOrCreate(actualEvent.data.name);			
		}
		break;
	case EVENT_COLLISION_FINISH:
		{
			pNewList = mapCollisionFinish.FindOrCreate(actualEvent.data.name);						
		}
		break;
	case EVENT_ACTIVATION_EVENT:
		{
			pNewList = mapActivationEvent.FindOrCreate(actualEvent.data.name);
		}
		break;
	case EVENT_TIMER:
		{
			pNewList = mapTimerNameAction.FindOrCreate(actualEvent.data.name);			
		}
		break;
	case EVENT_ANIMATION:
		{
			pNewList = &listAnimationActions;
		}
		break;
	case EVENT_ANIMATION_FINISH:
		{
			if(!actualEvent.data.name)
			{
				strcpy(actualEvent.data.name, ANY_ANIMATION.getCharBuf());
			}

			pNewList = mapAnimationFinishActions.FindOrCreate(actualEvent.data.name);
		}
		break;
	case EVENT_PATH_FINISH:
		{
			if(!actualEvent.data.name || actualEvent.data.name[0] == 0)
			{
				//Anterior version compatibility
				strcpy(actualEvent.data.name, ANY_PATH.getCharBuf());
				strcpy(action->eventData.data.name, ANY_PATH.getCharBuf());
			}

			pNewList = mapPathFinishActions.FindOrCreate(actualEvent.data.name);
		}
		break;
	case EVENT_CREATE:
		{
			pNewList = &listCreateActions;
		}
		break;
	case EVENT_MOUSEBUTTONDOWN:
		{
			//action->bDrag = actualEvent.data.mouse.bDrag; //Causes the bug "don't save relative to creator when call Create Actor inside mouse button down event"
			pNewList = mapMouseButtonDownActions.FindOrCreate(actualEvent.data.mouse.button);
		}
		break;
	case EVENT_MOUSEBUTTONUP:
		{
			pNewList = mapMouseUpActions.FindOrCreate(actualEvent.data.mouse.button);
		}
		break;
	case EVENT_MOUSEENTER:
		{
			pNewList = &listMouseEnterActions;
		}
		break;
	case EVENT_MOUSELEAVE:
		{
			pNewList = &listMouseLeaveActions;
		}
		break;
	case EVENT_MOVEFINISH:
		{
			pNewList = &listMoveFinishActions;
		}
		break;
	case EVENT_DESTROY_ACTOR:
		{
			pNewList = &listDestroyActorActions;
		}
		break;
	case EVENT_OUT_OF_VISION:
		{
			pNewList = &listOutOfVisionActions;
		}
		break;
	}

	GLASSERT(pNewList != NULL);

	if(pNewList)
	{
		if(pUpdateList)
		{
			if(pNewList == pUpdateList)
			{
				//Add only if not in list
				if(!pNewList->Find(action))
				{
					pNewList->PushBack(action);
				}
			}
			else
			{
				//Remove from update list and add on new list
				pUpdateList->FindAndDelete(action);
				pNewList->PushBack(action);
			}
		}
		else
		{
			pNewList->PushBack(action);
		}
	}

	if(actualEvent.eventType == EVENT_COLLISION || actualEvent.eventType == EVENT_COLLISION_FINISH)
	{
		U8 *n = mapCollisionOrCollisionFinish.Find1(actualEvent.data.name);
		if(!n)
		{
			mapCollisionOrCollisionFinish.Add(actualEvent.data.name, 1);
		}
		else
		{
			mapCollisionOrCollisionFinish.Add(actualEvent.data.name, *n + 1);
		}

		//For speedup collisions
		bHasCollisionWithAnyActor = mapCollision[ANY_ACTOR] || mapCollisionFinish[ANY_ACTOR];
	}


}


void exec_setpen(int r, int g, int b, double transp, int pensize)
{
	/*
	Define the actual pen for the Event Actor

	r: red component (0 - 255)
	g: green component (0 - 255)
	b: blue component (0 - 255)
	transparency: (0.0 - 1.0)
	pensize: size of pen (1 - 15)
	*/

	Action::getActualEventActor()->DrawSetPen(r, g, b, transp, pensize);

#ifdef GP2X	
	SDL_Delay(1); //Solve the crash in 'gp2x setpen.ged' (don't ask me why!)
#endif
}

void exec_moveto(int x, int y)
{
	/*
	Move Event Actor pen to (x, y) coordinates
	*/

	Action::getActualEventActor()->DrawMoveTo(x, y);
}

void exec_lineto(int x, int y)
{
	/*
	Draw line on Event Actor to (x, y) coordinates using actual pen
	*/

	Action::getActualEventActor()->DrawLineTo(x, y);
}

void exec_putpixel(int x, int y)
{
	/*
	Draw a single pixel on Event Actor in (x, y) coordinates using actual pen
	*/

	Action::getActualEventActor()->DrawPutPixel(x, y);
}

void exec_screen_to_actor(int *x, int *y)
{
	//Convert from screen coordinates to actor coordinates
	if(!x || !y) return;

	KrVector2T< GlFixed > object;
	Action::getActualEventActor()->getImage()->ScreenToObject( *x, *y, &object );

	*x = object.x.ToInt();
	*y = object.y.ToInt(); 
}


void exec_actor_to_screen(int *x, int *y)
{
	//Convert from actor coordinates to screen coordinates
	if(!x || !y) return;

	KrVector2T< GlFixed > screen;
	Action::getActualEventActor()->getImage()->ObjectToScreen( *x, *y, &screen );

	*x = screen.x.ToInt();
	*y = screen.y.ToInt(); 
}

void exec_erase(int r, int g, int b, double transp)
{
	/*
	Erase all actor with specified color

	r: red component (0 - 255)
	g: green component (0 - 255)
	b: blue component (0 - 255)
	transparency: (0.0 - 1.0)	
	*/

	Action::getActualEventActor()->DrawErase(r, g, b, transp);

#ifdef GP2X	
	SDL_Delay(1); //Solve the crash in 'gp2x_erase_not_working_demo.ged' (don't ask me why!)
#endif
}

void exec_savecanvas()
{
	//Save Event Actor canvas on memory
	
	Action::getActualEventActor()->DrawSaveCanvas();
}

void exec_restorecanvas()
{
	//Restore Event Actor canvas from memory
	
	Action::getActualEventActor()->DrawLoadCanvas();
}

int execChangeAnimation(char *actorName, char *animationName, int state)
{
	/*
	Change animation and set initial state (FORWARD, BACKWARD, STOPPED, NO_CHANGE)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event,
		- Any actor in game
	*/

	int res = 0;
	if(!actorName || !animationName || (state != FORWARD && state != BACKWARD && state != STOPPED && state != NO_CHANGE)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}

	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetAnimationState(state);
		actionActor->SetAnimation(animationName, state == NO_CHANGE);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_ANIMATION);
#endif

		res = 1;
	}
	else
	{
		//Multiple
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->SetAnimationState(state);
					actionActor->SetAnimation(animationName, state == NO_CHANGE);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_ANIMATION);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetAnimationState(state);
				actionActor->SetAnimation(animationName, state == NO_CHANGE);
				
#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_ANIMATION);
#endif

				res = 1;
			}
		}
	}	

#ifndef STAND_ALONE_GAME



	//Don't stop the game when show this message
	//Can causes cunfusion when try execute an action in a inexistent actor, that can be created later
	//(see bsfgc010a.ged)
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangeAnimation(\"%s\", \"%s\", %ld) call is invalid", actorName, animationName, state);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execChangeAnimationDirection(char *actorName, int state)
{
	/*
	Change animation  state (FORWARD, BACKWARD, STOPPED)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName || (state != FORWARD && state != BACKWARD && state != STOPPED)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}

	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetAnimationState(state);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_ANIMATION_DIRECTION);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];			
				if(actionActor->getRunning())
				{
					actionActor->SetAnimationState(state);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_ANIMATION_DIRECTION);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetAnimationState(state);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_ANIMATION_DIRECTION);
#endif

				res = 1;
			}
			
		}
		
	}	

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangeAnimationDirection(\"%s\", %ld) call is invalid", actorName, state);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execChangeTransparency(char *actorName, double transp)
{
	/*
	Change actor transparency (0.0 - opaque, to 1.0 - transparent)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;		
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;		
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetTransparency(transp);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_TRANSPARENCY);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->SetTransparency(transp);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_TRANSPARENCY);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetTransparency(transp);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_TRANSPARENCY);
#endif

				res = 1;
			}
			
		}
		
	}

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangeTransparency(\"%s\", %f) call is invalid", actorName, transp);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}


int execCollisionState(char *actorName, int iState)
{
	/*
	Enable or disable collision (enable = 1)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName) return 0; 
	bool state = iState?true:false;

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->setEnableCollisionDetection(state);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_COLLISION_STATE);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->setEnableCollisionDetection(state);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_COLLISION_STATE);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->setEnableCollisionDetection(state);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_COLLISION_STATE);
#endif

				res = 1;
			}
		}
	}		

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "CollisionState(\"%s\", %ld) call is invalid", actorName, state);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execVisibilityState(char *actorName, int state)
{
	/*
	state:
		ENABLE, DISABLE and DONT_DRAW_ONLY (Don't draw, but allow events)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;	
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;	
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetVisible(state);	

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_VISIBILITY_STATE);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{			
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->SetVisible(state);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_VISIBILITY_STATE);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetVisible(state);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_VISIBILITY_STATE);
#endif

				res = 1;
			}
			
		}
		
	}	

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "VisibilityState(\"%s\", %ld) call is invalid", actorName, state);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execChangePath(char *actorName, char *pathName, int axis)
{
	/*
	Change actor path and set axis (X_AXIS, Y_AXIS, BOTH_AXIS)
	pathName: any path name (included "no path" and "random path")
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/


	int res = 0;
	if(!actorName || !pathName || (axis != X_AXIS && axis != Y_AXIS && axis != BOTH_AXIS)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetPath(pathName, axis);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_PATH);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->SetPath(pathName, axis);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_PATH);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetPath(pathName, axis);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_PATH);
#endif

				res = 1;
			}
			
		}
		
	}
	
#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangePath(\"%s\", \"%s\", %ld) call is invalid", actorName, pathName, axis);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

static double CollisionNormal(Actor *eventActor, Actor *collideActor, int moveType, bool bCalcAngleOnly = false)
{
	double	PosX1 = eventActor->getX(), 
			PosY1 = eventActor->getY(), 
			PosX2 = collideActor->getX(), 
			PosY2 = collideActor->getY();

	double PosX1ant, PosY1ant, PosX2ant, PosY2ant;

	double	Vx1 = eventActor->getRealVelocityX(),
			Vy1 = eventActor->getRealVelocityY(),
			Vx2 = collideActor->getRealVelocityX(),
			Vy2 = collideActor->getRealVelocityY();

	int eventWidth = eventActor->Width(), 
		eventHeight = eventActor->Height(),
		collideWidth = collideActor->Width(),
		collideHeight = collideActor->Height();

	bool bEventBall =	(eventWidth >= eventHeight && eventHeight/(double)eventWidth > .9) ||
						(eventHeight >= eventWidth && eventWidth/(double)eventHeight > .9),

		 bCollideBall =	(collideWidth >= collideHeight && collideHeight/(double)collideWidth > .9) ||
						(collideHeight >= collideWidth && collideWidth/(double)collideHeight > .9);


	//Normalize
	double	v1 = sqrt(Vx1*Vx1 + Vy1*Vy1),
			v2 = sqrt(Vx2*Vx2 + Vy2*Vy2);
	
	if(v1 > 0.0)
	{
		Vx1 /= v1;
		Vy1 /= v1;
	}
	
	if(v2 > 0.0)
	{
		Vx2 /= v2;
		Vy2 /= v2;
	}

	
	//Save positions
	PosX1ant = PosX1;
	PosY1ant = PosY1;
	PosX2ant = PosX2;
	PosY2ant = PosY2;


	//position exactly touching - no intersection	
	GlDynArray<KrImage*> outputArray;

#define BACK_OVER_COLLISION_ANGLE
//#define CHECK_ALL

/*#ifndef BACK_OVER_COLLISION_ANGLE
	do
	{
		PosX1ant = PosX1;
		PosY1ant = PosY1;
		PosX2ant = PosX2;
		PosY2ant = PosY2;


		if((moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx1) > 0.0 || fabs(Vy1) > 0.0))
		{
			PosX1 += -Vx1;//cos1;
			PosY1 += -Vy1;//sin1;
			
			eventActor->SetPos(PosX1, PosY1);
		}

		if((moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx2) > 0.0 || fabs(Vy2) > 0.0))
		{
			PosX2 += -Vx2;//cos2;
			PosY2 += -Vy2;//sin2;

			collideActor->SetPos(PosX2, PosY2);
		}

		outputArray.Clear();

	} while((eventActor->getImage()->CheckCollision(collideActor->getImage()) 
	//|| engine->Tree()->CheckChildCollision(eventActor->getImage(), collideActor->getImage(), &outputArray))
	);

	//Put a little collision
	PosX1 = PosX1ant;
	PosY1 = PosY1ant;
	PosX2 = PosX2ant;
	PosY2 = PosY2ant;

	if((moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx1) > 0.0 || fabs(Vy1) > 0.0))
	{
		eventActor->SetPos(PosX1, PosY1);
	}
	
	if((moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx2) > 0.0 || fabs(Vy2) > 0.0))
	{
		collideActor->SetPos(PosX2, PosY2);
	}
#endif*/

	//Get collision normal
	double collNormalAngle = 0.0/*, collNormalAngleAnt = 0.0*/;
	bool bMoved;
		
	
#ifdef CHECK_ALL
	//of all colliding actors
	engine->Tree()->CheckAllCollision(eventActor->getImage(), &outputArray);

	//do {
		collNormalAngle = 0; 
		int dx = 0, dy = 0, i= 0, n = 0, sumDxDy = 0;

	KrImage *img;
	for(i = 0; i < outputArray.Count(); i++)
	{
		img = outputArray[i];
		if(img->getParentActor() && ((Actor *)img->getParentActor())->getEnableCollisionDetection())
		{
			//Get angle
			eventActor->getImage()->Get_overlap_collision_area_gradient(img, dx, dy);

			collNormalAngle += atan2(dy, dx);
			sumDxDy += abs(dx) + abs(dy);

			n++;
		}
	}


	if(sumDxDy != 0 && n > 0)
	{
		if(n > 1)
		{
			int a;
			a=0;
		}

		collNormalAngle /= n;
	}
	else
	{
		dx = dy = 0;
		bMoved = false;

		while(dx == 0 && dy == 0)
		{
			//Try calculate in anterior position		
			
			if((moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx1) > 0.0 || fabs(Vy1) > 0.0))
			{
				PosX1 += -Vx1;
				PosY1 += -Vy1;
				bMoved = true;
				
				eventActor->SetPos(PosX1, PosY1);
			}
			
			if((moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx2) > 0.0 || fabs(Vy2) > 0.0))
			{
				PosX2 += -Vx2;
				PosY2 += -Vy2;
				bMoved = true;
				
				collideActor->SetPos(PosX2, PosY2);
			}	
			
			eventActor->getImage()->Get_overlap_collision_area_gradient(collideActor->getImage(), dx, dy);

			if(!bMoved) break;
		} 

		collNormalAngle = atan2(dy, dx);
	}

#else

	int dx = 0, dy = 0/*, i= 0, n = 0, sumDxDy = 0*/;

	//Get angle
	KrImage *normal = eventActor->getImage(), *tile = collideActor->getImage();
	bool bTile = false;

	if(eventActor->getTile())
	{
		//Swap the actors
		normal = collideActor->getImage();
		tile = eventActor->getTile()->getTileImage();
		bTile = true;
	}
	else if(collideActor->getTile())
	{
		//Swap the actors
		normal = eventActor->getImage();
		tile = collideActor->getTile()->getTileImage();
		bTile = true;
	}

	normal->Get_overlap_collision_area_gradient(tile, dx, dy, bTile);

	bMoved = false;	
	while(dx == 0 && dy == 0)
	{
		//Try calculate in anterior position
		
		if((moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx1) > 0.0 || fabs(Vy1) > 0.0))
		{
			PosX1 += -Vx1;
			PosY1 += -Vy1;
			bMoved = true;

			//Linear limit
			if( fabs(PosX1 - PosX1ant) > .7*eventWidth ||
				fabs(PosY1 - PosY1ant) > .7*eventHeight )
			{
				break;
			}
			
			eventActor->SetPos(PosX1, PosY1);
		}
		
		if((moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(Vx2) > 0.0 || fabs(Vy2) > 0.0))
		{
			PosX2 += -Vx2;
			PosY2 += -Vy2;
			bMoved = true;

			//Linear limit
			if( fabs(PosX2 - PosX2ant) > .7*collideWidth ||
				fabs(PosY2 - PosY2ant) > .7*collideHeight )
			{
				break;
			}
			
			collideActor->SetPos(PosX2, PosY2);
		}	
		
		normal->Get_overlap_collision_area_gradient(tile, dx, dy, bTile);

		if(!bMoved) break;
	} 

	collNormalAngle = atan2((double)dy, (double)dx);
#endif

	//Restore position
	PosX1 = PosX1ant;
	PosY1 = PosY1ant;
	PosX2 = PosX2ant;
	PosY2 = PosY2ant;

	if(moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) eventActor->SetPos(PosX1, PosY1);
	if(moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) collideActor->SetPos(PosX2, PosY2);

	if(bCalcAngleOnly) 
	{
		return collNormalAngle;
	}


	//Adjust angle to avoid loops
	/*if(((collNormalAngle > 0.0 && collNormalAngleAnt < 0.0) || (collNormalAngle < 0.0 && collNormalAngleAnt > 0.0)) && 
		(fabs(collNormalAngle - collNormalAngleAnt) == Script::PI ||
		(fabs(collNormalAngle + collNormalAngleAnt) == Script::PI/4.0)))
	{
		//collNormalAngle += Script::PI/2.0;
		break;
	}*/


	//position exactly touching - no intersection

#ifdef BACK_OVER_COLLISION_ANGLE
	double	cos2 = cos(collNormalAngle)/2.0,
			sin2 = sin(collNormalAngle)/2.0;

	double	cos1 = -cos2,
			sin1 = -sin2;
#else
	double	cos2 = -Vx2,
			sin2 = -Vy2;

	double	cos1 = -Vx1,
			sin1 = -Vy1;
#endif

	PosX1ant = PosX1;
	PosY1ant = PosY1;
	PosX2ant = PosX2;
	PosY2ant = PosY2;

	bMoved = false;
	//Don't test collision with children anymore
	//Very confuse concept

	while(
		eventActor->getImage()->CheckCollision(collideActor->getImage()) ||
		//engine->Tree()->CheckChildCollision(eventActor->getImage(), collideActor->getImage(), &outputArray) //Don't test collision with children anymore

		//Tiles (Need this test when don't use children collision)		
		(
			//Without this, CollisionTile3.ged don't get side collisions
			(collideActor->getTile() && engine->Tree()->CheckChildCollision(eventActor->getImage(), collideActor->getTile()->getTileImage(), &outputArray))

			//Don't check this: causes error with collisionTile.ged
			//|| (eventActor->getTile() && engine->Tree()->CheckChildCollision(collideActor->getImage(), eventActor->getImage(), &outputArray)) 
		)
		)
	{
		if((moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(cos1) > 0.0 || fabs(sin1) > 0.0))
		{
			PosX1 += cos1;
			PosY1 += sin1;
			bMoved = true;


			//Linear limit
			if( fabs(PosX1 - PosX1ant) > .7*eventWidth ||
				fabs(PosY1 - PosY1ant) > .7*eventHeight )
			{
				break;
			}

			//Ball limit
			if(bEventBall && distance(PosX1, PosY1, PosX1ant, PosY1ant) > .7*eventHeight)
			{
				break;
			}
			
			eventActor->SetPos(PosX1, PosY1);
		}

		if((moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS) && (fabs(cos2) > 0.0 || fabs(sin2) > 0.0))
		{
			PosX2 += cos2;
			PosY2 += sin2;
			bMoved = true;

			
			//Linear limit
			if( fabs(PosX2 - PosX2ant) > .7*collideWidth ||
				fabs(PosY2 - PosY2ant) > .7*collideHeight )
			{
				break;
			}

			//Ball limit
			if(bCollideBall && distance(PosX2, PosY2, PosX2ant, PosY2ant) > .7*eventHeight)
			{
				break;
			}

			collideActor->SetPos(PosX2, PosY2);			
		}		
		

		outputArray.Clear();		

		if(!bMoved) break;
	} 

	//collNormalAngleAnt = collNormalAngle; //remove comment if use collNormalAngleAnt above

	//} while(engine->Tree()->CheckAllCollision(eventActor->getImage(), &outputArray));


	///////////////////////////////////////////
	//This code was used to solve the CollisionTile3.ged, but
	//the right fix is use the option 'Repeat while in collision' on the game.
	//So, don't use this code anymore. (causes bug in captainDJ_06.ged if use)

	/*stActorsInCollision actorsInCollision(eventActor, collideActor);
	if(Action::mapActorsInCollision.Remove(actorsInCollision))
	{
		eventActor->OnCollisionFinish(collideActor);
	}*/
	///////////////////////////////////////////


	return collNormalAngle;
}

static void VelocityAfterCollision(double M1, double M2, 
			   double &Vx1, double &Vy1, double &Vx2, double &Vy2,
			   double collNormalAngle)
{
	/*
	Calculate objects velocity vector after collision

	Object 1:
		M1 = mass
		Vx1, Vy1 = velocity vector		

	Object 2:
		M2 = mass
		Vx2, Vy2 = velocity vector		

	Result:
		Vx1, Vy1, Vx2, Vy2
	*/	

	//collision response

	double	nX = cos(collNormalAngle),
			nY = sin(collNormalAngle);

	double  a1 = Vx2*nX + Vy2*nY,
			a2 = Vx1*nX + Vy1*nY;

	double optimisedP = (2.0 * (a1 - a2)) / (M2 + M1);

	//now find out the resultant vectors

	Vx1 += optimisedP * M2 * nX;
	Vy1 += optimisedP * M2 * nY;

	Vx2 -= optimisedP * M1 * nX;
	Vy2 -= optimisedP * M1 * nY;
}


int execPhysicalBounce(int moveType, int massType, double massEventActor, double massCollideActor, double eventVelocityMultiplier, double collideVelocityMultiplier)
{
	/*
	Do a physical bounce movement between actors in a collision
	Return 1 if success, 0 on error

	moveType: MOVE_EVENT_ACTOR_ONLY, MOVE_COLLIDE_ACTOR_ONLY, MOVE_BOTH_ACTORS
	massType: USE_CALCULATED_MASS, USE_SPECIFIED_MASS

	massEventActor, massCollideActor:	When massType = USE_CALCULATED_MASS, use this vars how calculated mass multiplier.
										When massType = USE_SPECIFIED_MASS, use this vars how mass.
										Values > 0.0

	eventVelocityMultiplier, collideVelocityMultiplier: final velocity multiplier
	*/

	if((moveType != MOVE_EVENT_ACTOR_ONLY && moveType != MOVE_COLLIDE_ACTOR_ONLY && moveType != MOVE_BOTH_ACTORS) ||
	   (massType != USE_CALCULATED_MASS && massType != USE_SPECIFIED_MASS) ||
	   massEventActor <= 0.0 || massCollideActor <= 0.0) 
	   return 0; 
	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();

	if(!collideActor)
		return 0;

#ifndef STAND_ALONE_GAME		
	AddToGameGraph(collideActor, SET_PHYSICAL_BOUNCE);
#endif

	int eventWidth = eventActor->Width(), 
		eventHeight = eventActor->Height(),
		collideWidth = collideActor->Width(),
		collideHeight = collideActor->Height();

	
	//Calculate move
	double	Vx1 = eventActor->getRealVelocityX(),
		Vy1 = eventActor->getRealVelocityY(),
		Vx2 = collideActor->getRealVelocityX(),
		Vy2 = collideActor->getRealVelocityY();

	double M1, M2;

	if(massType == USE_SPECIFIED_MASS)
	{
		M1 = massEventActor;
		M2 = massCollideActor;
	}
	else
	{
		M1 = eventActor->GetArea()*massEventActor;
		M2 = collideActor->GetArea()*massCollideActor;
	}


	double collNormalAngle = CollisionNormal(eventActor, collideActor, moveType);

	if(collideActor->getFrame() != collideActor->getFrameAnt())
	{
		//Dynamic animation: dont't calculate pixel velocity, but work for now
		//Get local velocity in other actor

		if(moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
		{
			eventActor->setVelocityX(0.0);
			eventActor->setVelocityY(0.0);
			
			//Estimated pixel velocity
			Vx2 += eventActor->getRealVelocityX();
			Vy2 += eventActor->getRealVelocityY();		
		}
	}

	if(eventActor->getFrame() != eventActor->getFrameAnt())
	{
		//Dynamic animation: dont't calculate pixel velocity, but work for now
		//Get local velocity in other actor

		if(moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
		{
			collideActor->setVelocityX(0.0);
			collideActor->setVelocityY(0.0);
			
			//Estimated pixel velocity
			Vx1 += collideActor->getRealVelocityX();
			Vy1 += collideActor->getRealVelocityY();		
		}
	}
	

	VelocityAfterCollision(M1, M2, 
		Vx1, Vy1, Vx2, Vy2,
		collNormalAngle);

	//GLOUTPUT( "collNormalAngle: %f, Vx1: %f, Vy1: %f\n", collNormalAngle*180.0/3.14, Vx1, Vy1);



	//Limit velocity to actor dimentions (avoid actor scape)
	if(moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
	{
		if(fabs(Vx1) > .7*eventWidth)
		{
			double scale = .7*eventWidth / fabs(Vx1);
			
			Vx1 *= scale;
			Vy1 *= scale;
		}
		
		if(fabs(Vy1) > .7*eventHeight)
		{
			double scale = .7*eventHeight / fabs(Vy1);
			
			Vx1 *= scale;
			Vy1 *= scale;
		}
		
		//Ball limit
		if( (eventWidth >= eventHeight && eventHeight/(double)eventWidth > .9) ||
			(eventHeight >= eventWidth && eventWidth/(double)eventHeight > .9) )
		{
			if(sqrt(Vx1*Vx1 + Vy1*Vy1) > .7*eventHeight)
			{
				Vx1 *= .7;
				Vy1 *= .7;
			}
		}
	}

	if(moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
	{
		if(fabs(Vx2) > .7*collideWidth)
		{
			double scale = .7*collideWidth / fabs(Vx2);
			
			Vx2 *= scale;
			Vy2 *= scale;
		}
		
		if(fabs(Vy2) > .7*collideHeight)
		{
			double scale = .7*collideHeight / fabs(Vy2);
			
			Vx2 *= scale;
			Vy2 *= scale;
		}

		//Ball limit
		if( (collideWidth >= collideHeight && collideHeight/(double)collideWidth > .9) ||
			(collideHeight >= collideWidth && collideWidth/(double)collideHeight > .9) )
		{
			if(sqrt(Vx2*Vx2 + Vy2*Vy2) > .7*collideHeight)
			{
				Vx2 *= .7;
				Vy2 *= .7;
			}
		}
	}

	
	

	//Apply multiplier
	Vx1 *= eventVelocityMultiplier;
	Vy1 *= eventVelocityMultiplier;

	Vx2 *= collideVelocityMultiplier;
	Vy2 *= collideVelocityMultiplier;

	if(moveType == MOVE_EVENT_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
	{
		eventActor->setVelocityX(Vx1);
		eventActor->setVelocityY(Vy1);
	}
	
	if(moveType == MOVE_COLLIDE_ACTOR_ONLY || moveType == MOVE_BOTH_ACTORS)
	{
		collideActor->setVelocityX(Vx2);
		collideActor->setVelocityY(Vy2);
	}
	
	return 1;
}

static bool checkCollisionSide(double angle, int side)
{
	//Return true if side match with real collision side
	
	if(side != ANY_SIDE)
	{
		switch(side)
		{
		case TOP_SIDE:
			if(angle > 45.0 && angle <= 135.0) return true;
			break;

		case LEFT_SIDE:
			if(angle > -45.0 && angle <= 45.0) return true;
			break;

		case BOTTOM_SIDE:
			if(angle > -135.0 && angle <= -45.0) return true;
			break;

		case RIGHT_SIDE:
			if(angle > 135.0 || angle <= -135.0) return true;
			break;

		case TOP_OR_BOTTOM_SIDE:
			if((angle > 45.0 && angle <= 135.0) || (angle > -135.0 && angle <= -45.0)) return true;
			break;

		case LEFT_OR_RIGHT_SIDE:
			if((angle > -45.0 && angle <= 45.0) || (angle > 135.0 || angle <= -135.0)) return true;
			break;
		}

		return false;
	}

	return true;
}

void Action::OnCollision(Actor *eventActor, Actor *collideActor)
{
	//The tree may be invalid after position change
	Actor::ResetAnyPositionChanged();

	//Store time stamp
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	stActorsInCollision actorsInCollision(eventActor, collideActor);
	stActorsInCollision2 actorsInCollision2(eventActor, collideActor);

	bool bActorsInCollision = mapActorsInCollision[actorsInCollision] != NULL;
	
	
	if(!bActorsInCollision)
		mapActorsInCollision.Add(actorsInCollision, 1);

	GameControl::Get()->AddActorsInCollisionInActualFrame(actorsInCollision2);

	//Get collision angle here
	//Solve the collisionSideBug.ged

	ExecuteCollisionActions(eventActor, collideActor, bActorsInCollision, -1.0, timeStamp, EXECUTE_ALL);	
	

#ifdef USE_WALK
	//Is the tree still valid?
	if(Actor::AnyPositionChanged()) //Don't need with Walk1() optimization
	{
		engine->Tree()->Walk();
	}
#endif
}

void Action::ExecuteCollisionActions(Actor *eventActor, Actor *collideActor, bool bActorsInCollision, double angle, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_ACTOR_NAME)			
	{
		//Collide with actor name?
		ListActionsIterator itList(mapCollision.FindString(collideActor->getActorName()));
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			
			bool bDisableRepeat = action->eventData.data.name[NAME_LIMIT-1]?true:false;
			if(!bDisableRepeat || !bActorsInCollision)
			{
				int side = action->eventData.data.name[NAME_LIMIT-2];
				if(side != ANY_SIDE && angle == -1.0)
				{
					angle = CollisionNormal(eventActor, collideActor, MOVE_BOTH_ACTORS, true) * 180/Script::PI;
				}

				if(checkCollisionSide(angle, side))
				{
					action->timeStamp = timeStamp;
					Execute(action, eventActor, collideActor);

					execFlags &= ~EXEC_ACTOR_NAME;
				}
			}
		}
	}
	
	if(execFlags & EXEC_CLONE_NAME)		
	{
		//Collide with specified cloned actor?	
		ListActionsIterator itList(mapCollision.FindString(collideActor->getCloneName()));
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			
			bool bDisableRepeat = action->eventData.data.name[NAME_LIMIT-1]?true:false;
			if(!bDisableRepeat || !bActorsInCollision)
			{
				int side = action->eventData.data.name[NAME_LIMIT-2];
				if(side != ANY_SIDE && angle == -1.0)
				{
					angle = CollisionNormal(eventActor, collideActor, MOVE_BOTH_ACTORS, true) * 180/Script::PI;
				}

				if(checkCollisionSide(angle, side))
				{
					action->timeStamp = timeStamp;
					Execute(action, eventActor, collideActor);

					execFlags &= ~EXEC_CLONE_NAME;
				}
			}
		}
	}

	//Collide with others actors?
	if(bHasCollisionWithAnyActor && (execFlags & EXEC_ANY_ACTOR))
	{
		ListActionsIterator itList(mapCollision[ANY_ACTOR]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			
			bool bDisableRepeat = action->eventData.data.name[NAME_LIMIT-1]?true:false;
			if(!bDisableRepeat || !bActorsInCollision)
			{
				int side = action->eventData.data.name[NAME_LIMIT-2];
				if(side != ANY_SIDE && angle == -1.0)
				{
					angle = CollisionNormal(eventActor, collideActor, MOVE_BOTH_ACTORS, true) * 180/Script::PI;
				}

				if(checkCollisionSide(angle, side))
				{
					action->timeStamp = timeStamp;
					Execute(action, eventActor, collideActor);

					execFlags &= ~EXEC_ANY_ACTOR;
				}
			}
		}
	}
	
	if((execFlags & EXEC_ACTOR_NAME) || (execFlags & EXEC_CLONE_NAME) || (execFlags & EXEC_ANY_ACTOR))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteCollisionActions(eventActor, collideActor, bActorsInCollision, angle, timeStamp, execFlags);
		}
	}
}


void Action::OnCollisionFinish(Actor *eventActor, Actor *collideActor)
{
	//The tree may be invalid after position change
	Actor::ResetAnyPositionChanged();

	//Store time stamp
	Uint32 timeStamp = emulatedTimeStamp++; //SDL_GetTicks();
	bUserInput = false;

	ExecuteCollisionFinishActions(eventActor, collideActor, timeStamp, EXECUTE_ALL);
	

#ifdef USE_WALK
	//Is the tree still valid?
	if(Actor::AnyPositionChanged()) //Don't need with Walk1() optimization
	{
		engine->Tree()->Walk();
	}
#endif
}

void Action::ExecuteCollisionFinishActions(Actor *eventActor, Actor *collideActor, Uint32 timeStamp, Uint32 execFlags)
{
	if(execFlags & EXEC_ACTOR_NAME)						
	{
		//Collide with actor name?
		ListActionsIterator itList(mapCollisionFinish.FindString(collideActor->getActorName()));
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor, collideActor);

			execFlags &= ~EXEC_ACTOR_NAME;
		}
	}
	
	if(execFlags & EXEC_CLONE_NAME)				
	{
		//Collide with specified cloned actor?	
		ListActionsIterator itList(mapCollisionFinish.FindString(collideActor->getCloneName()));
		
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor, collideActor);

			execFlags &= ~EXEC_CLONE_NAME;
		}
	}

	//Collide with others actors?
	if(bHasCollisionWithAnyActor && (execFlags & EXEC_ANY_ACTOR))
	{
		ListActionsIterator itList(mapCollisionFinish[ANY_ACTOR]);
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			stAction *action = itList.Current();
			action->timeStamp = timeStamp;
			Execute(action, eventActor, collideActor);

			execFlags &= ~EXEC_ANY_ACTOR;
		}
	}
	
	if((execFlags & EXEC_ACTOR_NAME) || (execFlags & EXEC_CLONE_NAME) || (execFlags & EXEC_ANY_ACTOR))
	{
		//Execute inherited events
		Action *pInheritedActions = getInheritedEvents();
		if(pInheritedActions)
		{
			pInheritedActions->ExecuteCollisionFinishActions(eventActor, collideActor, timeStamp, execFlags);
		}
	}
}


int execFollowMouse(char *actorName, int axis)
{
	/*
	Make actor follow mouse axis (X_AXIS, Y_AXIS, BOTH_AXIS, NONE_AXIS)
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName || (axis != X_AXIS && axis != Y_AXIS && axis != BOTH_AXIS && axis != NONE_AXIS)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->setFollowMouseAxis(axis);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_FOLLOW_MOUSE);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->setFollowMouseAxis(axis);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_FOLLOW_MOUSE);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->setFollowMouseAxis(axis);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_FOLLOW_MOUSE);
#endif

				res = 1;
			}
			
		}
		
	}	

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "FollowMouse(\"%s\", %ld) call is invalid", actorName, axis);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execChangeParent(char *actorName, char *parentName)
{
	/*
	Change actor parent
	parentName: any actor name (included "Event Actor", "Collide Actor" and "no parent")
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName || !parentName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{			
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;		
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		if(strcmp(parentName, S_EVENT_ACTOR) == 0)
		{
			actionActor->SetParent(eventActor);

#ifndef STAND_ALONE_GAME
			AddToGameGraph(actionActor, SET_PARENT, eventActor);
#endif
		}
		else if(strcmp(parentName, S_COLLIDE_ACTOR) == 0)
		{
			if(!collideActor) return 0;
			actionActor->SetParent(collideActor);

#ifndef STAND_ALONE_GAME
			AddToGameGraph(actionActor, SET_PARENT, collideActor);
#endif
		}
		else if(strcmp(parentName, S_PARENT_ACTOR) == 0)
		{
			if( eventActor->getParent() != GameControl::Get()->GetAxis() &&
				actionActor != eventActor->getParent() &&
				IS_VALID_ACTOR(eventActor->getParent())
				) 
			{
				actionActor->SetParent(eventActor->getParent());

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_PARENT, eventActor->getParent());
#endif
			}
		}
		else if(strcmp(parentName, S_CREATOR_ACTOR) == 0)
		{
			if(actionActor != eventActor->getCreator() && IS_VALID_ACTOR(eventActor->getCreator()))
			{
				actionActor->SetParent(eventActor->getCreator());

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_PARENT, eventActor->getCreator());
#endif
			}
		}
		/*else if(strcmp(parentName, NO_PARENT) == 0) //Removed in the SetParent function
		{
			actionActor->RemoveParent();
		}*/
		else
		{
			Actor *p = GameControl::Get()->GetActor(parentName, true, false, false);
			actionActor->SetParent(p);

#ifndef STAND_ALONE_GAME
			AddToGameGraph(actionActor, SET_PARENT, p);
#endif
		}

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					if(strcmp(parentName, S_EVENT_ACTOR) == 0)
					{
						actionActor->SetParent(eventActor);

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_PARENT, eventActor);
#endif
					}
					else if(strcmp(parentName, S_COLLIDE_ACTOR) == 0)
					{
						if(!collideActor) return 0;
						actionActor->SetParent(collideActor);

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_PARENT, collideActor);
#endif
					}
					else if(strcmp(parentName, S_PARENT_ACTOR) == 0)
					{
						if( eventActor->getParent() != GameControl::Get()->GetAxis() &&
							actionActor != eventActor->getParent() &&
							IS_VALID_ACTOR(eventActor->getParent())
							) 
						{
							actionActor->SetParent(eventActor->getParent());

#ifndef STAND_ALONE_GAME
							AddToGameGraph(actionActor, SET_PARENT, eventActor->getParent());
#endif
						}
					}
					else if(strcmp(parentName, S_CREATOR_ACTOR) == 0)
					{
						if(actionActor != eventActor->getCreator() && IS_VALID_ACTOR(eventActor->getCreator()))
						{
							actionActor->SetParent(eventActor->getCreator());

#ifndef STAND_ALONE_GAME
							AddToGameGraph(actionActor, SET_PARENT, eventActor->getCreator());
#endif
						}
					}
					/*else if(strcmp(parentName, NO_PARENT) == 0) //Removed in the SetParent function
					{
						actionActor->RemoveParent();
					}*/
					else
					{
						Actor *p = GameControl::Get()->GetActor(parentName, true, false, false);
						actionActor->SetParent(p);

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_PARENT, p);
#endif
					}

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				if(strcmp(parentName, S_EVENT_ACTOR) == 0)
				{
					actionActor->SetParent(eventActor);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_PARENT, eventActor);
#endif
				}
				else if(strcmp(parentName, S_COLLIDE_ACTOR) == 0)
				{
					if(!collideActor) return 0;
					actionActor->SetParent(collideActor);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_PARENT, collideActor);
#endif
				}
				else if(strcmp(parentName, S_PARENT_ACTOR) == 0)
				{
					if( eventActor->getParent() != GameControl::Get()->GetAxis() &&
						actionActor != eventActor->getParent() &&
						IS_VALID_ACTOR(eventActor->getParent())
						) 
					{
						actionActor->SetParent(eventActor->getParent());

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_PARENT, eventActor->getParent());
#endif
					}
				}
				else if(strcmp(parentName, S_CREATOR_ACTOR) == 0)
				{
					if(actionActor != eventActor->getCreator() && IS_VALID_ACTOR(eventActor->getCreator()))
					{
						actionActor->SetParent(eventActor->getCreator());

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_PARENT, eventActor->getCreator());
#endif
					}
				}
				/*else if(strcmp(parentName, NO_PARENT) == 0) //Removed in the SetParent function
				{
					actionActor->RemoveParent();
				}*/
				else
				{
					Actor *p = GameControl::Get()->GetActor(parentName, true, false, false);
					actionActor->SetParent(p);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_PARENT, p);
#endif
				}

				res = 1;
			}
			
		}
		
	}	

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangeParent(\"%s\", \"%s\") call is invalid", actorName, parentName);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}


int execChangeCursor(char *actorName, char *imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY)
{
	/*
	Change actor cursor
	imgName: image file path relative to game directory
	nFramesH: number of horizontal frames
	nFramesV: number of vertical frames
	hotSpotX: cursor x hot spot
	hotSpotY: cursor y hot spot
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- Any actor in game
	*/

	int res = 0;
	if(!actorName || !imgName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();	
	Actor *actionActor = NULL;

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}

	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->SetCursor(imgName,
			nFramesH,
			nFramesV,
			hotSpotX,
			hotSpotY);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_CURSOR);
#endif

		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->SetCursor(imgName,
						nFramesH,
						nFramesV,
						hotSpotX,
						hotSpotY);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_CURSOR);
#endif

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->SetCursor(imgName,
						nFramesH,
						nFramesV,
						hotSpotX,
						hotSpotY);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_CURSOR);
#endif

				res = 1;
			}			
		}		
	}	

#ifndef STAND_ALONE_GAME
	/*if(!res)
	{
		char error[1024];
		sprintf(error, "ChangeCursor(\"%s\", \"%s\", %ld, %ld, %ld, %ld) call is invalid", actorName, imgName, nFramesH, nFramesV, hotSpotX, hotSpotY);
		Script::myDisplay(error);
	}*/
#endif

	return res;
}

int execActivationEvent(char *cloneName)
{
	/*
	Send a Actication Event to actor cloneName
	cloneName: valid clone name
	
	Return 1 if success, 0 on error
	*/

	Actor *fromActor = Action::getActualEventActor();
	Actor *toActor = GameControl::Get()->GetActor(cloneName, true, false, false);


	if(toActor)
	{
#ifndef STAND_ALONE_GAME
		AddToGameGraph(toActor, SET_ACTIVATION_EVENT);
#endif
		toActor->OnActivationEvent(fromActor);
	}
	else 
	{
#ifndef STAND_ALONE_GAME
		/*char error[1024];
		sprintf(error, "SendActivationEvent(\"%s\") call is invalid", cloneName);
		Script::myDisplay(error);*/
#endif

		return 0;
	}

	return 1;
}

Actor* execCreateActor(char *_creatorName, char *_animationName, char *_parentName, char *_pathName, int xpos, int ypos, int absolutePosition)
{
	/*
	Create new actor
	creatorName: any actor name
	animationName: animation valid for actorName
	parentName: any actor name (included "Event Actor", "Collide Actor" and "no parent")
	pathName: any path name (included "no path" and "random path")
	x,y: new actor initial position
	absolutePosition: true = 1
	Return actor if success, 0 on error
	*/

	//int res = 0;
	if(!_creatorName || !_animationName || !_parentName || !_pathName) return NULL; 
	
	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();

	Actor *oriActor = NULL;

	if(Action::getInCreateActorEvent() && strcmp(_creatorName, eventActor->getActorName()) == 0)
	{
		//Avoid create actor infinit loop
		return NULL;
	}

	//Cache strings to avoid allocations
	static gedString creatorName, animationName, parentName, pathName;

	creatorName = _creatorName;
	animationName = _animationName;
	parentName = _parentName;
	pathName = _pathName;



	//The actor will be created by the event actor, 
	//so assign to the new actor the event actor region 
	//Restore the current region when return
	//Solve the alpha8_debug.ged bug? (http://game-editor.com/forum/viewtopic.php?p=7444#7444)
	//RegionLoadSetDefault region(eventActor);

	Action::setInCreateActorAction(true); //Avoid call setRunning in the load time

	bool bLoadedActor = false;
	if(GameControl::Get()->getStandAloneMode())  //Avoid creating two actors: this and other from disk
	{
		//From not running actors, without loading
		oriActor = GameControl::Get()->GetActor(creatorName, false, true, false); 
	}

	if(!oriActor)
	{
		oriActor = GameControl::Get()->GetActor(creatorName, false, true); //From not running actors, with loading
		if(oriActor && GameControl::Get()->getStandAloneMode())
		{
			bLoadedActor = true;
		}
	}

	if(!oriActor) oriActor = GameControl::Get()->GetActor(creatorName, true, true); //Try again from running actors

	Action::setInCreateActorAction(false);

	Actor *newActor = NULL;
	//Don't use GetAction to test the anim name (don't works with sequences)
	if(oriActor && !oriActor->isView())
	{
		Actor *parent = GameControl::Get()->GetActor(parentName, true, false, false);
		if(parent == EVENT_ACTOR) parent = eventActor;
		else if(parent == COLLIDE_ACTOR) parent = collideActor;	
		else if(parent == PARENT_ACTOR)
		{
			if(oriActor->getParent() != GameControl::Get()->GetAxis()) parent = oriActor->getParent();
			else parent = NULL; //Avoid crash in IonSkiff_Tournament.ged when try destroy the ship
		}
		else if(parent == CREATOR_ACTOR)
		{
			parent = oriActor->getCreator();
		}
		
		if(!bLoadedActor) 
		{
			newActor = new Actor(creatorName, parent, oriActor->getType(), 0, 0, false, NULL, true);
		}
		else
		{
			newActor = oriActor;
			newActor->getImage()->SetVisible(true);
			//Set the parent later
		}

		
		
		Action *copyAction = oriActor->getAction();
		if(copyAction) newActor->setAction(copyAction);

		//newActor->setRunning(); //Why here?
		
		if(!oriActor->ExistsAction(animationName)) animationName = ACTOR_GED_TEXT.getCharBuf();

		

		newActor->AddAnimation(animationName, "", 0, 0);
		newActor->SetAnimation(animationName, true);
		
		
		if(oriActor && oriActor->getTextActor())
		{
			Text *text = newActor->AddText(oriActor->getTextActor()->GetText(), 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
				oriActor->getTextActor()->getFontName(), 
				oriActor->getTextActor()->getiniASCIIChar(), 
				oriActor->getTextActor()->getnChars(),
				
				oriActor->getTextActor()->IsEditable(),
				oriActor->getTextActor()->HasBorder(),
				oriActor->getTextActor()->HasBackground(),
				oriActor->getTextActor()->HasGetFocus(),
				oriActor->getTextActor()->getBorderColor(),
				oriActor->getTextActor()->getCursorColor(),
				oriActor->getTextActor()->getBackGroundColor());

			newActor->setTextZeros(oriActor->getTextZeros()); //Solve the 00 bug when set a big text and set a number after
			newActor->setTextActor(text);
		}
		
		if(parent)
		{
			int x = xpos, y = ypos;

			newActor->SetParent(parent);

			newActor->SetPos(x, y);
			newActor->SetPathIni(x, y);			
		}
		else
		{
			int x = 0, y = 0;
			if(!absolutePosition)
			{
				//Initial position relative to creator
				x = eventActor->getImage()->CompositeXForm().x.ToInt();
				y = eventActor->getImage()->CompositeXForm().y.ToInt();
				
				KrVector2T< GlFixed > object;
				GameControl::Get()->GetAxis()->getImage()->ScreenToObject( x, y, &object );
				
				x = object.x.ToInt();
				y = object.y.ToInt();
			}
			
			x += xpos;
			y += ypos;

			newActor->SetPos(x, y);
			newActor->SetPathIni(x, y);
		}
		
		

		Path *path = GameControl::Get()->GetPath(pathName);
		newActor->SetPathX(path);			
		newActor->SetPathY(path);
		
//#ifdef STAND_ALONE_GAME
		//Use the same code in both versions
		//Solve the bug "For some reason the code on the AI spawn sometimes causes the player character to turn red or yellow" in ALPHA_1_1_4a.ged

		//Get from original loaded actor
		//Solve the bug "don't show myPlane smoke in 1945.ged in stand alone game"
		newActor->getImage()->SetZDepth(oriActor->getLoadedZDepth());
		newActor->SetTransparency(oriActor->getLoadedTransparency());
		newActor->SetColor(oriActor->getLoadedColor().c.red, oriActor->getLoadedColor().c.green, oriActor->getLoadedColor().c.blue);

		//Copy original loaded values
		newActor->setLoadedZDepth(oriActor->getLoadedZDepth());
		newActor->setLoadedTransparency(oriActor->getLoadedTransparency());
		newActor->setLoadedColor(oriActor->getLoadedColor());
/*#else
		newActor->getImage()->SetZDepth(oriActor->getImage()->ZDepth());
		newActor->SetTransparency(oriActor->GetTransparency());
		newActor->SetColor(oriActor->getScriptVars()->r, oriActor->getScriptVars()->g, oriActor->getScriptVars()->b);
#endif*/

		//Copy flags
		newActor->setNetActor(oriActor->getNetActor());
		newActor->setNetOptimizedPrecision(oriActor->getNetOptimizedPrecision());
				
		if(oriActor->getTile())
		{
			Tile *tile = new Tile(newActor);	
			tile->Equal(oriActor->getTile());
			newActor->setTile(tile);
		}

		if(oriActor->getType() == REGION_ACTOR || oriActor->getType() == REGION_ACTOR_FILLED)
		{
			//Set position
			newActor->getImage()->SetDimensions(oriActor->Width(), oriActor->Height());
			engine->InvalidateRectangle(newActor->getImage()->Bounds());
		}

		if(oriActor->getInfinite() != NO_INFINITE) newActor->SetInfinite(oriActor->getInfinite());

		//Solve the bug "Sometimes parents don't update the children position"
		//In alpha9.ged after click in the left door, player_base is the parent of player_graphic,
		//but player_graphic will be updated only after move the actor again		
		if(parent)
		{
			parent->getImage()->Invalidate();
		}
		
		//Assign to the new actor the event actor regions
		//Solve the alpha8_debug.ged bug? (http://game-editor.com/forum/viewtopic.php?p=7444#7444)
		//Solve the alpha14.ged bug?
		//newActor->SetRegionsFrom(eventActor);		
		
#ifndef STAND_ALONE_GAME
		AddToGameGraph(newActor, SET_CREATE_ACTOR);
#endif
		newActor->setNetActor(oriActor->getNetActor());

		newActor->setCreator(eventActor);
		newActor->setRunning(); //Only works here if not in net creation callback

		//Call OnCreate		
		if(newActor->getRunning() && newActor->getAction()) newActor->getAction()->OnCreate(newActor);

		if(newActor->getNetActor())
		{
			//Net actor created. The original event must be executed on the remote machines
			Action::setNetActorCreated(true);
		}
	}
	else
	{
#ifndef STAND_ALONE_GAME
		/*char error[1024];
		sprintf(error, "CreateActor(\"%s\", \"%s\", \"%s\", \"%s\", %ld, %ld, %ld) call is invalid", 
			creatorName, animationName, parentName, pathName, xpos, ypos, absolutePosition);
		Script::myDisplay(error);*/
#endif	
		return NULL;	
	}

	
	return newActor;
}

int execDestroyActor(char *actorName)
{
	/*
	Destroy actor
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;

	

	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		if(eventActor->isView()) return 0;
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor || collideActor->isView()) return 0;
		actionActor = collideActor;		
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis() && !eventActor->getParent()->isView()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		if(eventActor->getCreator() && !eventActor->getCreator()->isView()) actionActor = eventActor->getCreator();
	}

	//Check the running flag to avoid destroy editor actors
	//Solve ski_c1_04r.ged bug (ENV_trees destroyed in game and editor mode)
	if(IS_VALID_ACTOR(actionActor))
	{
		if(!actionActor->isView() && actionActor->getRunning())
		{
			actionActor->getImage()->SetVisible(false);
			GameControl::Get()->SetDestroyedActor(actionActor);
			actionActor->PostMessage(actionActor, DELETE_ME, DELETE_ME);


#ifndef STAND_ALONE_GAME
			AddToGameGraph(actionActor, SET_DESTROY_ACTOR);
#endif

			res = 1;
		}
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(!actionActor->isView() && actionActor->getRunning())
				{
					actionActor->getImage()->SetVisible(false);
					GameControl::Get()->SetDestroyedActor(actionActor);
					actionActor->PostMessage(actionActor, DELETE_ME, DELETE_ME);

					

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_DESTROY_ACTOR);
#endif
					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor && !actionActor->isView() && actionActor->getRunning()) 
			{
				actionActor->getImage()->SetVisible(false);
				GameControl::Get()->SetDestroyedActor(actionActor);
				actionActor->PostMessage(actionActor, DELETE_ME, DELETE_ME);


#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_DESTROY_ACTOR);
#endif

				res = 1;
			}
			
		}
		
	}

#ifndef STAND_ALONE_GAME
	if(!res)
	{
		/*char error[1024];
		sprintf(error, "DestroyActor(\"%s\") call is invalid", actorName);
		Script::myDisplay(error);*/
	}
#endif

	return res;
}

int execCreateTimer(char *actorName, char *timerName, int milliseconds)
{
	/*
	Create new timer
	Return 1 if success, 0 on error

	timerName: new timer name
	milliseconds: new timer tempo
	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	int res = 0;
	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	//Action *actualAction = Action::getActualAction(); //Removed to solve the createTimerInOtherActor.ged bug

	stTimerDefinition timer, *pTimer = GameControl::Get()->GetTimer(timerName);


	if(pTimer) //Get timer definition, if timer exists
		timer = *pTimer;
	else
	{
		timer.minRandomInterval = 0;
		timer.count = REPEATE_FOREVER;
		timer.type = PERIODIC_TIMER;
	}

	timer.interval = milliseconds;

	char buf[256];
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;		
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;		
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		Action *actualAction = actionActor->getAction(); //solve the createTimerInOtherActor.ged bug
		
		if(actualAction && (!actionActor->getNetOptimizedPrecision() || actionActor->CanExecuteActionLocally()))
		{		
			sprintf(buf, "%s%012X\0", timerName, actionActor);
			if(!actualAction->mapTimerNameId.FindString(buf)) //Not repeat timers
			{
				SDL_TimerID timerId = actionActor->SetTimer(timer.interval, timer.count, timer.type, timer.minRandomInterval);
				stTimerEvent *p = actualAction->mapTimerIdName.FindOrCreate(timerId);
				
				strcpy(p->name, timerName);
				p->miliseconds = milliseconds;
				actualAction->mapTimerNameId.Add(buf, timerId);

#ifdef DEBUG
			GLOUTPUT( "New timer: %s (%s)\n",  buf, actionActor->getCloneName());
#endif

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_CREATE_TIMER);
#endif
			}
		}
		
		res = 1;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					Action *actualAction = actionActor->getAction(); //solve the createTimerInOtherActor.ged bug
					
					if(actualAction && (!actionActor->getNetOptimizedPrecision() || actionActor->CanExecuteActionLocally()))
					{
						sprintf(buf, "%s%012X\0", timerName, actionActor);
						if(!actualAction->mapTimerNameId.FindString(buf)) //Not repeat timers
						{
							SDL_TimerID timerId = actionActor->SetTimer(timer.interval, timer.count, timer.type, timer.minRandomInterval);
							stTimerEvent *p = actualAction->mapTimerIdName.FindOrCreate(timerId);
							
							strcpy(p->name, timerName);
							p->miliseconds = milliseconds;
							actualAction->mapTimerNameId.Add(buf, timerId);

#ifdef DEBUG
							GLOUTPUT( "New timer: %s (%s)\n",  buf, actionActor->getCloneName());
#endif

#ifndef STAND_ALONE_GAME
							AddToGameGraph(actionActor, SET_CREATE_TIMER);
#endif
						}
					}

					res = 1;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				Action *actualAction = actionActor->getAction(); //solve the createTimerInOtherActor.ged bug
				
				if(actualAction && (!actionActor->getNetOptimizedPrecision() || actionActor->CanExecuteActionLocally()))
				{
					sprintf(buf, "%s%012X\0", timerName, actionActor);
					if(!actualAction->mapTimerNameId.FindString(buf)) //Not repeat timers
					{
						SDL_TimerID timerId = actionActor->SetTimer(timer.interval, timer.count, timer.type, timer.minRandomInterval);
						stTimerEvent *p = actualAction->mapTimerIdName.FindOrCreate(timerId);
						
						strcpy(p->name, timerName);
						p->miliseconds = milliseconds;
						actualAction->mapTimerNameId.Add(buf, timerId);

#ifdef DEBUG
						GLOUTPUT( "New timer: %s (%s)\n",  buf, actionActor->getCloneName());
#endif

#ifndef STAND_ALONE_GAME
						AddToGameGraph(actionActor, SET_CREATE_TIMER);
#endif
					}
				}

				res = 1;
			}
			
		}
		
	}		

#ifndef STAND_ALONE_GAME
	if(!res)
	{
		/*char error[1024];
		sprintf(error, "CreateTimer(\"%s\", \"%s\", %ld) call is invalid", actorName, timerName, milliseconds);
		Script::myDisplay(error);*/
	}
#endif

	return res;
}

int execDestroyTimer(char *timerName)
{
	/*
	Destroy timer on event actor
	Return 1 if success, 0 on error

	timerName: timer name
	*/

	if(!timerName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	//Action *actualAction = Action::getActualAction(); //Removed to solve the createTimerInOtherActor.ged bug

	char buf[256];
	Actor *actionActor = eventActor;
	Action *actualAction = actionActor->getAction(); //solve the createTimerInOtherActor.ged bug
		
	if(actualAction)
	{		
		sprintf(buf, "%s%012X\0", timerName, actionActor);
		SDL_TimerID *timerId = actualAction->mapTimerNameId.FindString(buf);
		if(timerId)
		{		
			SDL_TimerID id = *timerId;

			actionActor->RemoveTimer((stTimer *)(id->param)); //Solve the bug: Bug "Gravity Wars only shot one time / ship" (não acontece na 1.3.4, na 1.3.5 debug)

			
			SDL_RemoveTimer(id);
			actualAction->mapTimerNameId.Remove(buf);
			actualAction->mapTimerIdName.Remove(id);
			

#ifndef STAND_ALONE_GAME
			AddToGameGraph(actionActor, SET_DESTROY_TIMER);
#endif
		}
	}
	

	return 1;
}

int execChangeZDepth(char *actorName, double zdepth)
{
	/*
	Change actor z depth
	zdepth: 0.0 to 1.0
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;		
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;		
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->getImage()->SetZDepth((int)(zdepth*MAX_ACTOR_DEPTH));
		
#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_ZDEPTH);
#endif
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->getImage()->SetZDepth((int)(zdepth*MAX_ACTOR_DEPTH));
					
#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_ZDEPTH);
#endif
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->getImage()->SetZDepth((int)(zdepth*MAX_ACTOR_DEPTH));

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_ZDEPTH);
#endif
			}
			else return 0;
		}
		else return 0;
	}

	return 1;
}


int execPlaySound(char *soundPath, double volume, int loop, Sint16 pan)
{
	/*
	Play a sound file
	soundPath: relative to game directory
	volume: 0.0 to 1.0
	loop: loop count (1 to 65000 or 0 to infinite loop)
	pan: -255 (left) to 255 (right)
	Return the allocated chanel if success, 0 on error
	*/

	if(!soundPath) return 0; 

#ifndef STAND_ALONE_GAME
	AddToGameGraph(Action::getActualEventActor(), SET_PLAY_SOUND);
#endif
	
	return GameControl::Get()->PlaySound(false,
								  soundPath,
								  (int)(volume*SDL_MIX_MAXVOLUME), loop, 0, pan);	
}

int execPlayMusic(char *soundPath, double volume, int loop, int priority)
{
	/*
	Play a music file
	soundPath: relative to game directory
	volume: 0.0 to 1.0
	loop: loop count (1 to 65000 or 0 to infinite loop)
	priority: HIGH_PRIORITY_MUSIC, MEDIUM_PRIORITY_MUSIC or LOW_PRIORITY_MUSIC
	Return the allocated chanel if success, 0 on error
	*/

	if(!soundPath) return 0; 

#ifndef STAND_ALONE_GAME
	AddToGameGraph(Action::getActualEventActor(), SET_PLAY_MUSIC);
#endif
	
	return GameControl::Get()->PlaySound(true,
								  soundPath,
								  (int)(volume*SDL_MIX_MAXVOLUME), loop, priority, 0);
}

int execToAnteriorPosition(char *actorName)
{
	/*
	Send actor to anterior position
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	if(!actorName) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;	
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;			
	}	
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->ToAnteriorPosition();

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_TO_ANTERIOR_POSITION);
#endif
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->ToAnteriorPosition();	

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_TO_ANTERIOR_POSITION);
#endif
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->ToAnteriorPosition();

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_TO_ANTERIOR_POSITION);
#endif
			}
			else return 0;
		}
		else return 0;
	}	

	return 1;
}



int execEventEnable(char *actorName, unsigned long event)
{
	/*
	Enable actor event:
		EVENTMOUSEBUTTONDOWN	
		EVENTMOUSEBUTTONUP
		EVENTMOUSEENTER
		EVENTMOUSELEAVE
		EVENTANIMATION			
		EVENTANIMATIONFINISH	
		EVENTPATHFINISH	
		EVENTMOVEFINISH
		EVENTKEYDOWN			
		EVENTKEYUP				
		EVENTTIMER				
		EVENTCOLLISION	
		EVENTCOLLISIONFINISH
		EVENTCREATE	
		EVENTDESTROYACTOR
		EVENTOUTOFVISION
		EVENTACTIVATIONEVENT
		EVENTALL				
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	if(!actorName || 
	   (event != EVENTMOUSEBUTTONDOWN &&
		event != EVENTMOUSEBUTTONUP &&
		event != EVENTMOUSEENTER &&
		event != EVENTMOUSELEAVE &&
		event != EVENTANIMATION &&
		event != EVENTANIMATIONFINISH &&
		event != EVENTPATHFINISH &&
		event != EVENTMOVEFINISH &&
		event != EVENTKEYDOWN &&
		event != EVENTKEYUP &&
		event != EVENTTIMER &&
		event != EVENTCOLLISION &&
		event != EVENTCOLLISIONFINISH &&
		event != EVENTCREATE &&
		event != EVENTDESTROYACTOR &&
		event != EVENTOUTOFVISION &&
		event != EVENTACTIVATIONEVENT &&
		event != EVENTALL)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->setEventControl(event, true);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_EVENT_ENABLE);
#endif
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->setEventControl(event, true);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_EVENT_ENABLE);
#endif
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->setEventControl(event, true);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_EVENT_ENABLE);
#endif
			}
			else return 0;
		}
		else return 0;
	}	

	return 1;
}

int execEventDisable(char *actorName, unsigned long event)
{
	/*
	Disable actor event:
		EVENTMOUSEBUTTONDOWN	
		EVENTMOUSEBUTTONUP	
		EVENTMOUSEENTER
		EVENTMOUSELEAVE
		EVENTANIMATION			
		EVENTANIMATIONFINISH	
		EVENTPATHFINISH
		EVENTMOVEFINISH
		EVENTKEYDOWN			
		EVENTKEYUP				
		EVENTTIMER				
		EVENTCOLLISION	
		EVENTCOLLISIONFINISH
		EVENTCREATE	
		EVENTDESTROYACTOR
		EVENTOUTOFVISION
		EVENTACTIVATIONEVENT
		EVENTALL				
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor":	actor that is receiving the current event
		- "Collide Actor": actor that collided with the event actor
		- Any actor in game
	*/

	if(!actorName || 
	   (event != EVENTMOUSEBUTTONDOWN &&
		event != EVENTMOUSEBUTTONUP &&
		event != EVENTMOUSEENTER &&
		event != EVENTMOUSELEAVE &&
		event != EVENTANIMATION &&
		event != EVENTANIMATIONFINISH &&
		event != EVENTPATHFINISH &&
		event != EVENTMOVEFINISH &&
		event != EVENTKEYDOWN &&
		event != EVENTKEYUP &&
		event != EVENTTIMER &&
		event != EVENTCOLLISION &&
		event != EVENTCOLLISIONFINISH &&
		event != EVENTCREATE &&
		event != EVENTDESTROYACTOR &&
		event != EVENTOUTOFVISION &&
		event != EVENTACTIVATIONEVENT &&
		event != EVENTALL)) return 0; 

	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->setEventControl(event, false);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_EVENT_DISABLE);
#endif
	}	
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->setEventControl(event, false);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_EVENT_DISABLE);
#endif
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->setEventControl(event, false);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_EVENT_DISABLE);
#endif
			}
			else return 0;
		}
		else return 0;
	}	

	return 1;
}

int execExitGame()
{
	/*
	End game			
	Return 1 if success, 0 on error
	*/

	
#ifndef STAND_ALONE_GAME
	if(GameControl::Get()->GetLastKey(UNASSIGNED_SYSTEM_ADDRESS) == SDLK_ESCAPE)
	{
		//Will be processed by MainPanel
		return 1;
	}
#endif


	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));
	event.quit.type = SDL_QUIT;
	SDL_PushEvent(&event);

	return 1;
}

int execLoadGame(char *gamePath)
{
	/*
	Load a game
	gamePath: game file path relative to actual game directory		
	Return 1 if success, 0 on error
	*/

	if(!gamePath) return 0;

	GameControl::Get()->LoadGameEvent(gamePath);
	return 1;
}

int execActorCount(char *actorName)
{
	/*
	Return number of actors with actorName

	Actor name: 
		- Any actor in game
	*/

	int count = 0;
	if(!actorName) return count;	
	
	ListActor *listActor = mapActors.FindString(actorName);
	if(listActor)
	{
		for(int il = 0; il < listActor->Count(); il++)
		{			
			if((*listActor)[il]->getRunning())
			{
				count++;
			}			
		}	
	}
	else if(strchr(actorName, '.'))
	{
		//Clone specified
		Actor *actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
		if(actionActor) return 1;
		else return 0;
	}
	

	return count;
}

bool checkCollision(Actor *actor, int x, int y)
{
	int xant = actor->getImage()->X(),
		yant = actor->getImage()->Y();

	//Move to test position 
	actor->getImage()->SetPos(x, y);

#ifdef USE_WALK
	//Update tree
	engine->Tree()->Walk(); //Don't need with Walk1() optimization
#endif

	//Check collision
	MapActorIterator it(mapActors);
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActor *listActor = it.Value();
		for(int il = 0; listActor && il < listActor->Count(); il++)
		{		
			GlDynArray<KrImage*> outputArray;

			//Don't test collision with children anymore
			//Very confuse concept

			Actor *actorTest = (*listActor)[il];
			if(actorTest && 
			   actorTest != actor &&
			   actorTest->getRunning() &&			   
			   actorTest->getEnableCollisionDetection() &&

			   (
			   actor->getImage()->CheckCollision(actorTest->getImage()) || 
			   //engine->Tree()->CheckChildCollision(actor->getImage(), actorTest->getImage(), &outputArray) //Don't test collision with children anymore

			   //Tiles
			   ((actor->getTile() && engine->Tree()->CheckChildCollision(actorTest->getImage(), actor->getTile()->getTileImage(), &outputArray)) ||
					 (actorTest->getTile() && engine->Tree()->CheckChildCollision(actor->getImage(), actorTest->getTile()->getTileImage(), &outputArray))) // Need this second test when don't use children collision
			   )
			   
			  )
			{	
				actor->getImage()->SetPos(xant, yant);
				return true;	
			}
		}
	}

	actor->getImage()->SetPos(xant, yant);
	return false;
}

int execCollisionFree(char *actorName, int x, int y)
{
	/*
	Check if position x,y is collision free for actorName

	Actor name: 
		- Any actor in game

    Return 1 if not collide, 0 otherwise
	*/

	if(!actorName) return 0;	
	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		if(checkCollision(actionActor, x, y)) return 0;
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					if(checkCollision(actionActor, x, y)) return 0;
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				if(checkCollision(actionActor, x, y)) return 0;
			}
		}
	}		

	return 1;
}

int execMoveTo(char *actorName, double x, double y, double velocity, char *relativeActor, char *obstacleActor, int moveType)
{
	/*
	Move actor to specified position, at specified velocity
	Return 1 if success, 0 on error

	Actor name: 
		- "Event Actor": Actor that is receiving the current event.
		- "Parent Actor": Event Actor's parent, if exists.
		- "Creator Actor": Event Actor's creator, if Event Actor has been created in some "Create Actor" action.
		- "Collide Actor": Actor that collided with the event Actor.
		- Any Actor in game

	x, y: position to move relative to relativeActor

	relativeActor: 

		The actor can be moved relative to game center (absolute coordinate), relative to mouse or
		any actor in game.

		Valid values:

		- "Event Actor": Actor that is receiving the current event. 
		- "Parent Actor": Event Actor's parent, if exists.
		- "Creator Actor": Event Actor's creator, if Event Actor has been created in some "Create Actor" action.
		- "Collide Actor": Actor that collided with the event Actor.
		- "Mouse Position": Move the actor relative to current mouse position
		- "Game Center": Move the actor relative to game coordinates
		- Any Actor in game

		Examples:

		1) Move the Event Actor to right 10 positions (x = x + 10)
		   actorName = "Event Actor"
		   x = 10
		   y = 0
		   velocity = 1000 (use high velocity to instantaneous move)
		   relativeActor = "Event Actor"

		2) Move the view to player position (view.x = player.x; view.y = player.y;)
		   actorName = "view"
		   x = 0
		   y = 0
		   relativeActor = "player"

		3) Move the player to mouse position 
		   actorName = "player"
		   x = 0
		   y = 0
		   relativeActor = "Mouse Position"

    avoidActor:

		Set the actor to avoid collisions while moving.
		If you select the actor name, not a clone name, all clones will be considered.
		If you select a actor with tiles, the bound box of the tiles will be considered.

		Only the actor bound box will be considered, not the real shape.

		The move path is calculated only when the action is executed.
		So, if you need avoid collision with other moving actors, you need to call the
		Move To action again.

		Valid values:

		- "Event Actor": Actor that is receiving the current event. 
		- "Parent Actor": Event Actor's parent, if exists.
		- "Creator Actor": Event Actor's creator, if Event Actor has been created in some "Create Actor" action.
		- "Collide Actor": Actor that collided with the event Actor.
		- Any Actor in game
		
	*/

	if(!actorName) return 0;
	
	
	Actor *eventActor = Action::getActualEventActor();
	Actor *collideActor = Action::getActualCollideActor();
	Actor *actionActor = NULL;
	
	if(strcmp(actorName, S_EVENT_ACTOR) == 0)
	{
		actionActor = eventActor;	
	}
	else if(strcmp(actorName, S_COLLIDE_ACTOR) == 0)
	{
		if(!collideActor) return 0;
		actionActor = collideActor;			
	}
	else if(strcmp(actorName, S_PARENT_ACTOR) == 0)
	{
		if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
	}
	else if(strcmp(actorName, S_CREATOR_ACTOR) == 0)
	{
		actionActor = eventActor->getCreator();
	}
	
	if(IS_VALID_ACTOR(actionActor))
	{
		actionActor->MoveTo(x, y, velocity, relativeActor, obstacleActor, moveType);

#ifndef STAND_ALONE_GAME
		AddToGameGraph(actionActor, SET_MOVE_TO);
#endif
	}
	else
	{
		//Multiple actors
		ListActor *listActor = mapActors.FindString(actorName);
		if(listActor)
		{
			for(int il = 0; il < listActor->Count(); il++)
			{
				actionActor = (*listActor)[il];
				if(actionActor->getRunning())
				{
					actionActor->MoveTo(x, y, velocity, relativeActor, obstacleActor, moveType);

#ifndef STAND_ALONE_GAME
					AddToGameGraph(actionActor, SET_MOVE_TO);
#endif
				}			
			}
		}
		else if(strchr(actorName, '.'))
		{
			//Clone specified
			actionActor = GameControl::Get()->GetActor(actorName, true, false, false);
			if(actionActor)
			{
				actionActor->MoveTo(x, y, velocity, relativeActor, obstacleActor, moveType);

#ifndef STAND_ALONE_GAME
				AddToGameGraph(actionActor, SET_MOVE_TO);
#endif
			}
			else return 0;
		}
		else return 0;
	}	

	return 1;
}

void Action::Execute(U16 actionNumber, Actor *eventActor, Actor *collideActor, bool bExecuteConditional, bool bForceExecute, bool bFromRemoteMachine, unsigned int remoteSeed)
{
	//Execute by the action number
	stAction *action = GetActionByIndex(actionNumber);

	if(action)
	{
		Execute(action, eventActor, collideActor, bExecuteConditional, bForceExecute, bFromRemoteMachine, remoteSeed);
	}
}

void Action::Execute(stAction *action, Actor *eventActor, Actor *collideActor, bool bExecuteConditional, bool bForceExecute, bool bFromRemoteMachine, unsigned int remoteSeed)
{
	//Process messages
	//Execute by the action pointer

	static int recursion = 0, executionOrder = 0;	
	gedString eventActorName, collideActorName;
	int eventActorCloneIndex, collideActorCloneIndex = -1;
	RakNetTime eventActorOwnershipTimestamp;
	bool bTestCreated = false, bCanExecute = true, bNetActor = eventActor->getNetActor(), bCanExecuteRemoteActionBefore = false;
	bool bRakPeer = GameControl::Get()->GetRakPeer();

	///////////////////////////
	//For remote random sincronization
	//The right way is send the RNG state (like seed, n)
	//But I not know now
	unsigned int seed = 0;
	///////////////////////////

	if(recursion == 0) executionOrder = 0;
	else executionOrder++;

	int currentOrder = executionOrder;

	recursion++;

	//Can execute now?
	if(bRakPeer && !bForceExecute)
	{
		//Put in the global queue
		int actionNumber = GetActionIndex(action);

		if(actionNumber >= 0)
		{
			if(collideActor)
			{
				collideActorName = collideActor->getActorName();
				collideActorCloneIndex = collideActor->getCloneIndex();
			}

			//The local and remote action must be executed in the same conditions
			//The state of the memory used to execute the action must be the same (x, y, xscreen...)

			if(!bFromRemoteMachine && bNetActor && eventActor->CanExecuteActionLocally() && eventActor->CanSendRemoteAction())
			{
				//This machine is the controller
				//Send event to the remote machines if have permission
				//RequestActorOwnership and ReleaseActorOwnership are supposed to be the only commands of an action.
				//So, there is no problem in sending the remote action now

				int actionNumber = GetActionIndex(action);

				if(actionNumber >= 0)
				{
					GameControl::Get()->SendRemoteAction(
						currentOrder,
						actionNumber, actionName, 
						eventActor->getActorName(), eventActor->getCloneIndex(), eventActor->GetOwnershipTimestamp(),
						collideActorName, collideActorCloneIndex,
						bExecuteConditional, false,
						seed
						);
				}
			}

			GameControl::Get()->PushActionInGlobalQueue(
				actionNumber, actionName, 
				eventActor->getActorName(), eventActor->getCloneIndex(), eventActor->GetOwnershipTimestamp(),
				collideActorName, collideActorCloneIndex,
				bExecuteConditional
				);
		}

		recursion--;
		return;
	}

	
	if(!bForceExecute && !bFromRemoteMachine && bNetActor && !eventActor->CanExecuteActionLocally())
	{
		//Wait event from remote machine
		bCanExecute = false;
	}

	//Execute now	
	if(bCanExecute)
	{
		

#if defined(WIN32) && !defined(_WIN32_WCE)
		//Don't change the priority when processing network actions (very slow)
		NormalPriority normalPriority(!bFromRemoteMachine);
#endif



		//A normal actor or a net controller actor will execute an action

		//To script execution
		StoreExecStaticData();

		actualEventActor = eventActor;
		actualCollideActor = collideActor;
		//actualAction = this;


		//Execute action
		if(!action->bConditional || bExecuteConditional)
		{
			if(!bForceExecute && action->executionType == WAIT_FOR_FRAME_EXECUTION)
			{
				if(!bRakPeer)
				{
					//Push action in eventActor
					//Can't execute WAIT_FOR_FRAME_EXECUTION in network games
					eventActor->PushQueueAction(action);				
				}

				recursion--;
				return;				
			}

			switch(action->actionType)
			{
			case SET_ANIMATION:
				execChangeAnimation(action->actorName, action->data.animation.name, action->data.animation.state);
				break;
			case SET_ANIMATION_DIRECTION:
				execChangeAnimationDirection(action->actorName, action->data.animation.state);
				break;
			case SET_TRANSPARENCY:
				execChangeTransparency(action->actorName, action->data.transparency.data.fixed);
				break;
			case SET_COLLISION_STATE:
				execCollisionState(action->actorName, action->data.bEnable);		
				break;
			case SET_VISIBILITY_STATE:
				execVisibilityState(action->actorName, action->data.bEnable);
				break;
			case SET_PATH:			
				execChangePath(action->actorName, action->data.path.name, action->data.path.axis);
				break;
			case SET_FOLLOW_MOUSE:		
				execFollowMouse(action->actorName, action->data.path.axis);
				break;
			case SET_PHYSICAL_BOUNCE:		
				execPhysicalBounce( action->data.physicalbounce.moveType,
					action->data.physicalbounce.massType,
					action->data.physicalbounce.massEventActor,
					action->data.physicalbounce.massCollideActor,
					action->data.physicalbounce.eventVelocityMultiplier,
					action->data.physicalbounce.collideVelocityMultiplier);
				break;
			case SET_PARENT:			
				execChangeParent(action->actorName, action->data.name);
				break;
			case SET_CURSOR:			
				execChangeCursor(action->actorName, action->data.cursor.imgName,
					action->data.cursor.nFramesH,
					action->data.cursor.nFramesV,
					action->data.cursor.hotSpotX,
					action->data.cursor.hotSpotY);
				break;
			case SET_CREATE_ACTOR:			
				execCreateActor(action->data.createActor.name, 
					action->data.createActor.animationName, 
					action->data.createActor.parentName, 
					action->data.createActor.pathName, 
					action->x, action->y, action->bAbsolutePosition);
				break;
			case SET_ACTIVATION_EVENT:
				{
					if(strcmp(action->name, eventActor->getCloneName()) == 0) //Actor from must be eventActor
						execActivationEvent(action->actorName);
				}
				break;
			case SET_DESTROY_ACTOR:			
				execDestroyActor(action->data.name);
				break;
			case SET_CREATE_TIMER:			
				execCreateTimer(action->actorName, action->data.timer.name, action->data.timer.miliseconds);
				break;
			case SET_DESTROY_TIMER:			
				execDestroyTimer(action->data.timer.name);
				break;
			case SET_EXPRESSION:
				{
					//if(action->x) //Cache address on x var
					Script *pScript = ((Script *)(action->x));

					if(pScript->getUseRand())
					{
						if(bFromRemoteMachine)
						{
							//Use remote machine seed?
							seed = remoteSeed;			
						}
						else
						{
							//If use only the time value, causes error in NewBearOids137.ged
							//All bears go the same direction when running on a fast machine
							
							static int s = 0;
							s += SDL_GetTicks() % 1000;							
							seed = RakNet::GetTime() + s;
						}

						if(seed) seedMT(seed);
					}

					pScript->Execute(eventActor, collideActor
#ifndef STAND_ALONE_GAME
						, action
#endif
						);
					//else
					/*{
					Script *pScript = GameControl::Get()->GetExpression(action->data.name);
					action->x = (Sint32)pScript;
					if(pScript) pScript->Execute(eventActor, collideActor);
					}*/				
				}
				break;
			case SET_SETTEXT:
				{
					Actor *actionActor = NULL;

					if(strcmp(action->actorName, S_EVENT_ACTOR) == 0)
					{
						actionActor = eventActor;					
					}
					else if(strcmp(action->actorName, S_COLLIDE_ACTOR) == 0)
					{
						actionActor = collideActor;					
					}
					else if(strcmp(action->actorName, S_PARENT_ACTOR) == 0)
					{
						if(eventActor->getParent() != GameControl::Get()->GetAxis()) actionActor = eventActor->getParent();
					}
					else if(strcmp(action->actorName, S_CREATOR_ACTOR) == 0)
					{
						actionActor = eventActor->getCreator();
					}

					if(IS_VALID_ACTOR(actionActor))
					{
						if(!actionActor->isView() && strlen(action->data.settext.text) > 0)
						{
							//Don't create input texts on actions
							Text *text = actionActor->AddText(action->data.settext.text, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
								action->data.settext.fontPath, 
								action->data.settext.iniASCIIChar, 
								action->data.settext.nChars);

							actionActor->setTextActor(text);

#ifndef STAND_ALONE_GAME
							AddToGameGraph(actionActor, SET_SETTEXT);
#endif
						}
						else
						{
							actionActor->setTextActor(NULL);
						}
					}
					else
					{
						//Multiple actors
						ListActor *listActor = mapActors.FindString(action->actorName);
						if(listActor)
						{
							for(int il = 0; il < listActor->Count(); il++)
							{
								actionActor = (*listActor)[il];

								if(!actionActor->isView() && actionActor->getRunning())
								{
									if(strlen(action->data.settext.text) > 0)
									{
										Text *text = actionActor->AddText(action->data.settext.text, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
											action->data.settext.fontPath, 
											action->data.settext.iniASCIIChar, 
											action->data.settext.nChars);

										actionActor->setTextActor(text);

#ifndef STAND_ALONE_GAME
										AddToGameGraph(actionActor, SET_SETTEXT);
#endif
									}
									else
									{
										actionActor->setTextActor(NULL);
									}	
								}			
							}
						}
						else if(strchr(action->actorName, '.'))
						{
							//Clone specified
							actionActor = GameControl::Get()->GetActor(action->actorName, true, false, false);
							if(actionActor && !actionActor->isView())
							{
								if(strlen(action->data.settext.text) > 0)
								{
									Text *text = actionActor->AddText(action->data.settext.text, 0, 0, ALIGN_AUTOMATIC, 255, 255, 255, 
										action->data.settext.fontPath, 
										action->data.settext.iniASCIIChar, 
										action->data.settext.nChars);

									actionActor->setTextActor(text);

#ifndef STAND_ALONE_GAME
									AddToGameGraph(actionActor, SET_SETTEXT);
#endif
								}
								else
								{
									actionActor->setTextActor(NULL);
								}
							}						
						}
					}	
				}
				break;
			case SET_CONDITIONAL_ACTION:
				{
					Script *pScript = GameControl::Get()->GetExpression(action->data.conditionalaction.expression1);
					if(pScript)
					{
						pScript->Execute(eventActor, collideActor);
						if(pScript->getConditionalResult())
						{
							action = action->next; //Go to conditional action
							if(action)
							{
								//Execute conditional action until other test 
								//or finish conditional actions

								do
								{
									Execute(action, eventActor, collideActor, true);
									action = action->next;
									//} while(action && (action->actionType != SET_CONDITIONAL_ACTION || action->bConditional));
								} while(action && action->actionType != SET_CONDITIONAL_ACTION && action->bConditional);
							}
						}
					}				
				}
				break;
			case SET_ZDEPTH:
				execChangeZDepth(action->actorName, action->data.zdepth/(double)MAX_ACTOR_DEPTH);
				break;
			case SET_PLAY_SOUND:
				if(action->data.sound.bMusic) execPlayMusic(action->data.sound.path, action->data.sound.volume/((double)SDL_MIX_MAXVOLUME), action->data.sound.loop, action->data.sound.priority);
				else execPlaySound(action->data.sound.path, action->data.sound.volume/((double)SDL_MIX_MAXVOLUME), action->data.sound.loop, action->data.sound.pan);				
				break;
			case SET_EVENT_CONTROL:
				if(action->data.eventcontrol.bEnable) execEventEnable(action->actorName, action->data.eventcontrol.events);
				else execEventDisable(action->actorName, action->data.eventcontrol.events);				
				break;
			case SET_TO_ANTERIOR_POSITION:			
				execToAnteriorPosition(action->actorName);
				break;
			case SET_MOVE_TO:			
				execMoveTo(action->actorName, action->data.moveto.x, action->data.moveto.y, action->data.moveto.velocity,
					action->data.moveto.relative, action->data.moveto.obstacle, action->data.moveto.moveType);
				break;

				//New action: verify if applicable to view actor
			}
		}

		RestoreExecStaticData();

		


		/*if(!bFromRemoteMachine)
		{
			//Send to remote machines			
			if(bNetActorCreated && recursion == 0 && bTestCreated)
			{
				//A normal actor has created a net actor in a user input		
				//Execute this action in the remote machines

				int actionNumber = GetActionIndex(action);

				if(actionNumber >= 0)
				{
					GameControl::Get()->SendRemoteAction(
						currentOrder,
						actionNumber, actionName, 
						eventActorName, eventActorCloneIndex, eventActorOwnershipTimestamp,
						collideActorName, collideActorCloneIndex, 
						bExecuteConditional, bForceExecute,
						seed
						);
				}
			}
		}*/
	}

	recursion--;
}


void Action::SetInheritedKeyEvents()
{
	//Solve the 'Event Inheritance with Key Down repeat.ged'

	if(!HaveKeyDown(false) && HaveKeyDown(true))
	{
		//Don't have key down, but inherit from other action
		//So, put on mapActionsKeyDown


		mapActionsKeyDown.Add(actionName, this);
	}

	if(!HaveKeyUp(false) && HaveKeyUp(true))
	{
		//Don't have key up, but inherit from other action
		//So, put on mapActionsKeyUp


		mapActionsKeyUp.Add(actionName, this);
	}
}


Action *Action::Load(const gedString& _actionName)
{
	//Load action by name
	Action *loadedAction = NULL;

	//Get action location
	SDL_RWops *src = GameControl::Get()->GetGameFilePointer();	
	if(!src) return NULL;

	Uint32 index = GameControl::Get()->GetActionIndex(_actionName);
	if(!index) return NULL;

	//Move file pointer
	Uint32 pos = SDL_RWtell( src ); //Save actual position
	SDL_RWseek( src, index, SEEK_SET );
	
	//Load
	loadedAction = new Action();
	loadedAction->Load(src);
	
	//Restore file pointer
	SDL_RWseek( src, pos, SEEK_SET ); 

#ifdef STAND_ALONE_GAME
	if(!loadedAction->InheritedEvents.empty())
	{
		//Load Inherited Events
		Action *pActionInherited = loadedAction->getInheritedEvents();
		if(!pActionInherited)
		{	
			pActionInherited = Load(loadedAction->InheritedEvents);
		}
		
		if(pActionInherited) 
		{
			pActionInherited->AttachAction();

			loadedAction->SetInheritedKeyEvents();
		}
	}
#endif

	return loadedAction;
}

void stAction::Load(SDL_RWops *src, Uint16 actionSize)
{
	Clear();		
	
	Uint16 lenComp = 0;

		
	
	if(GameControl::Get()->GetGameFileVersion() >= 17)
	{			
		lenComp = GameControl::Read16(src);
	}	
	
	if(lenComp)
	{
		//Load compressed action

		//Alloc buffers
		U8 *in = new U8[lenComp];
		if(!in) return;

		U8 *out = new U8[sizeof(stAction)];
		if(!out) 
		{
			delete [] in;
			return;
		}
		
		//Read compressed action
		GameControl::Read(src, in, lenComp, 1); 
		
		//Inflate
		Compression comp;			
		comp.Decompression(in, lenComp, out, sizeof(stAction)); //Don't extract to this. Solve the crash with 4 bytes struct alignment
		delete [] in;		

		//Copy to this object
		memcpy(this, out, sizeof(stAction));

		delete [] out;
	}
	else
	{
		GameControl::Read(src, this, actionSize, 1); //If actual stAction grather than read action, fill with zeros
	}

	pDescription = NULL;
	
	if(GameControl::Get()->GetGameFileVersion() < 33 && eventData.eventType == EVENT_KEYDOWN)
	{
		eventData.data.keyEvent.keySequence[0] = eventData.data.keyEvent.key;
	}

	
	if(GameControl::Get()->GetGameFileVersion() < 48 && actionType == SET_CREATE_TIMER)
	{
		//Old versions save timer interval as U16
		//thats limit the timer to 65000 ms
		//So, new version use U32
		U16 oldInterval = (U16)(data.timer.miliseconds);
		data.timer.miliseconds = oldInterval;
	}


	
#ifndef STAND_ALONE_GAME
	if(GameControl::Get()->GetGameFileVersion() > 45)
	{
		//Read description (don't read in exported game)
		gedString tmp;
		tmp.Read(src);
		
		setDescription(tmp);
	}
	else
	{
		setDescription("");
	}
#endif

#ifdef GP2X
	ToGP2X();
#endif
}

void Action::Load(SDL_RWops *src)
{
	actionName.Read(src);
	globalMapActions.Add(actionName, this);

	if(GameControl::Get()->GetGameFileVersion() > 40)
	{
		InheritedEvents.Read(src);
	}

	nActions = GameControl::Read16(src);
	Uint16 actionSize = GameControl::Read16(src);

	if(GameControl::Get()->GetGameFileVersion() > 27)
	{
		Uint8 OutOfVisionOptimization;
		GameControl::Read(src, &OutOfVisionOptimization, sizeof(OutOfVisionOptimization), 1);
		bOutOfVisionOptimization = OutOfVisionOptimization?true:false;
	}
	else
	{
		//Keep old version without optimizations
		bOutOfVisionOptimization = false;				
	}
	
	stAction loadAction;
	for(int i = 0; i < nActions; i++)
	{		
		loadAction.Load(src, actionSize);				

		actualEvent = loadAction.eventData;
		
		stAction *pNewAction = new stAction();
		*pNewAction = loadAction;
		
		if(!listActions) listActions = pNewAction;
		if(actionAnt) actionAnt->next = pNewAction;
		
		actionAnt = pNewAction;
		ToEvent(pNewAction);	
		
		
		if(pNewAction->actionType == SET_EXPRESSION)
		{
			//Load action script
			gedString scriptName(pNewAction->data.name);
			if(!GameControl::Get()->GetExpression(scriptName.c_str()))
			{
				Script::Load(scriptName);				
			}

			Script *pScript = GameControl::Get()->GetExpression(scriptName.c_str());
			pNewAction->x = (Sint32)pScript;
		}
		else if(pNewAction->actionType == SET_CONDITIONAL_ACTION)
		{
			//Load action script
			gedString scriptName(pNewAction->data.conditionalaction.expression1);
			if(!GameControl::Get()->GetExpression(scriptName.c_str()))
			{
				Script::Load(scriptName);
			}
		}
#ifndef STAND_ALONE_GAME
		else if(pNewAction->actionType == SET_ACTIVATION_EVENT && !GameControl::Get()->getStandAloneMode())
		{
			//Update activation events lines
			ActivationEventsCanvas::AddLine(pNewAction, true);
		}
		else if(pNewAction->actionType == SET_SETTEXT && !GameControl::Get()->getStandAloneMode())
		{
			//Add to cache text to avoid crash in Se Text panel when select the font
			//and keep text in Resource to export
			ActorText::AddFontToCache(pNewAction->data.settext.fontPath, pNewAction->data.settext.iniASCIIChar, pNewAction->data.settext.nChars);
		}
#endif
	}

	

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}


void Action::LoadActions(SDL_RWops *src)
{
	//Load all game actions

	Uint16 nGlobalActions = GameControl::Read16(src);

	for(int i = 0; i < nGlobalActions; i++)
	{
		Action *pNewAction = new Action();
		pNewAction->Load(src);
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	GameControl::Read(src, &expansion, sizeof(expansion), 1);
}



void Action::RemoveAction(Action *actionRemove)
{
	globalMapActions.Remove(actionRemove->getActionName());
	mapActionsKeyDown.Remove(actionRemove->getActionName());
	mapActionsKeyUp.Remove(actionRemove->getActionName());

	delete actionRemove;
}

void Action::DeleteConditionalAction(stAction *actionRemove)
{
	stAction *aux = actionRemove;
	
	do
	{
		aux = actionRemove->next;
		RemoveAction(actionRemove);
		delete actionRemove;
		actionRemove = aux;
	} while(aux && aux->bConditional);

}

bool Action::RemoveAction(stAction *actionRemove)
{
	if(!actionRemove) return false;
	bool bRemoved = false;


	
	//Remove from list
	stAction *aux = listActions, *ant = NULL;
	while(aux)
	{
		if(aux == actionRemove)
		{
			if(ant)
			{
				//Middle or last action
				ant->next = aux->next;
			}
			else
			{
				//First action
				listActions = aux->next;
			}

			if(actionRemove == actionAnt)
			{
				//Last action
				actionAnt = ant;
			}

			if(nActions) nActions--;
			bRemoved = true;
			break;
		}

		ant = aux;
		aux = aux->next;		
	}

	if(!bRemoved) return false;

	//Remove script action
	//Don't remove action script in game mode (parser againt genarete leaks)
	//Don't remove now, couses crash in the ExpressionEditor when cancel an action
	if(!GameControl::Get()->getGameMode())
	{
		if(actionRemove->actionType == SET_EXPRESSION)
		{
			GameControl::Get()->RemoveExpression(actionRemove->data.name);
		}
		else if(actionRemove->actionType == SET_CONDITIONAL_ACTION)
		{
			GameControl::Get()->RemoveExpression(actionRemove->data.conditionalaction.expression1);
		}
	}


	//Remove map and list events for this action

	//mapKeyDown
	{MapKeysIterator it(mapKeyDown);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapKeyDown.Remove(*it.Key());
				if(mapKeyDown.size() == 0)
					mapActionsKeyDown.Remove(getActionName());
			}
			return true;
		}
	}}


	//mapKeyUp
	{MapKeysIterator it(mapKeyUp);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapKeyUp.Remove(*it.Key());
				if(mapKeyUp.size() == 0)
					mapActionsKeyUp.Remove(getActionName());
			}
			return true;
		}
	}}


	//mapCollision
	{MapStringActionIterator it(mapCollision);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapCollision.Remove(*it.Key());
			}

			//For speedup collisions
			bHasCollisionWithAnyActor = mapCollision[ANY_ACTOR] || mapCollisionFinish[ANY_ACTOR];

			U8 *n = mapCollisionOrCollisionFinish.Find1(actualEvent.data.name);
			if(n && *n > 1)
			{
				mapCollisionOrCollisionFinish.Add(actualEvent.data.name, *n - 1);
			}
			else
			{
				mapCollisionOrCollisionFinish.Remove(*it.Key());
			}

			return true;
		}
	}}

	//mapCollisionFinish
	{MapStringActionIterator it(mapCollisionFinish);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapCollisionFinish.Remove(*it.Key());
			}

			//For speedup collisions
			bHasCollisionWithAnyActor = mapCollision[ANY_ACTOR] || mapCollisionFinish[ANY_ACTOR];

			U8 *n = mapCollisionOrCollisionFinish.Find1(actualEvent.data.name);
			if(n && *n > 1)
			{
				mapCollisionOrCollisionFinish.Add(actualEvent.data.name, *n - 1);
			}
			else
			{
				mapCollisionOrCollisionFinish.Remove(*it.Key());
			}
			return true;
		}
	}}

	//mapActivationEvent
	{MapStringActionIterator it(mapActivationEvent);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapActivationEvent.Remove(*it.Key());
			}
			return true;
		}
	}}

	//mapAnimationFinishActions
	{MapStringActionIterator it(mapAnimationFinishActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapAnimationFinishActions.Remove(*it.Key());
			}
			return true;
		}
	}}

	//mapPathFinishActions
	{MapStringActionIterator it(mapPathFinishActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapPathFinishActions.Remove(*it.Key());
			}
			return true;
		}
	}}


	//mapTimerNameAction
	{MapStringActionIterator it(mapTimerNameAction);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapTimerNameAction.Remove(*it.Key());
			}
			return true;
		}
	}}

	//mapMouseButtonDownActions
	{MapMouseButtonsIterator it(mapMouseButtonDownActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapMouseButtonDownActions.Remove(*it.Key());
			}
			return true;
		}
	}}


	//mapMouseUpActions
	{MapMouseButtonsIterator it(mapMouseUpActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		int s1 = it.Value()->size();
		it.Value()->FindAndDelete(actionRemove);
		int s2 = it.Value()->size();
		
		if(s2 < s1)
		{
			if(it.Value()->size() == 0)
			{
				mapMouseUpActions.Remove(*it.Key());
			}
			return true;
		}
	}}

	listAnimationActions.FindAndDelete(actionRemove);
	listCreateActions.FindAndDelete(actionRemove);
	listDestroyActorActions.FindAndDelete(actionRemove);
	listOutOfVisionActions.FindAndDelete(actionRemove);
	listMouseEnterActions.FindAndDelete(actionRemove);
	listMouseLeaveActions.FindAndDelete(actionRemove);
	listMoveFinishActions.FindAndDelete(actionRemove);

	//For speedup collisions
	bHasCollisionWithAnyActor = mapCollision[ANY_ACTOR] || mapCollisionFinish[ANY_ACTOR];

	return true;
}


void Action::CheckCollisionsBetween(Actor *actor, Actor *actorTest)
{	
	//actor always is valid (GameControl::CheckCollisions())

	if( //actorTest &&  //Tested in IS_VALID_ACTOR
		actorTest != actor && 
		IS_VALID_ACTOR(actor) && //avoid crash when destroy an actor (Need this test here? Delete while in collision?)
		IS_VALID_ACTOR(actorTest) &&
		actorTest->getRunning() &&
		actorTest->getEnableCollisionDetection())
	{		
		GlDynArray<KrImage*> outputArray;
		stActorsInCollision2 actorsInCollision2(actor, actorTest);

		KrImage *actorImage = actor->getImage(),
				*actorTestImage = actorTest->getImage();

		//Don't test collision with children anymore
		//Very confuse concept

		//SDL_Delay(1); //Put this delay to simulate the Pocket PC speed
	
		if(
			(GameControl::Get()->IsActorsInCollisionInActualFrame(actorsInCollision2)) || 
			(actorImage->CheckCollision(actorTestImage)) ||
			
			//(engine->Tree()->CheckChildCollision(actorImage, actorTestImage, &outputArray)) || //Don't test collision with children anymore 
			
			//Tiles
			(
				(actor->getTile() && engine->Tree()->CheckChildCollision(actorTestImage, actor->getTile()->getTileImage(), &outputArray))
				|| (actorTest->getTile() && engine->Tree()->CheckChildCollision(actorImage, actorTest->getTile()->getTileImage(), &outputArray)) // Need this second test when don't use children collision
			) 
		  )
		{		
			actor->OnCollision(actorTest);					
		}
		else
		{
			stActorsInCollision actorsInCollision(actor, actorTest);
			if(mapActorsInCollision.Remove(actorsInCollision)) //Only call collision finish if previous call
			{
				actor->OnCollisionFinish(actorTest);
			}
		}
	}
}

void Action::CheckCollisions(Actor *actor)
{
	//Cycles mesured with 1945.ged in 30s

	if(HaveCollisionWithAnyActor())
	{
		MapActorIterator it(mapWorkingActors);
		for(it.Begin(); !it.Done(); it.Next())
		{
			ListActor *listActor = it.Value();
			for(int il = 0; listActor && il < listActor->Count(); il++)
			{
				CheckCollisionsBetween(actor, (*listActor)[il]);
			}
		}
	}
	else
	{
		CheckCollisionList(actor);
	}	
}


void Action::CheckCollisionList(Actor *actor)
{	
	//Don't need keep empty lists in mapActors
	//CheckCollisions cycles: 721
	//Use mapCollisionOrCollisionFinish (solve the bug when need collision finish without test collision)
	for(gedString **p = mapCollisionOrCollisionFinish.GetKeyArray(); *p; p++)
	{
		if((*p)->hasPoint())
		{
			//Collision with clone (solve collisionCloneBug.ged)
			CheckCollisionsBetween(actor, GameControl::Get()->GetActor(**p, true, false, false));
		}
		else
		{
			/*To do: Collision test optimization (Axis Sorting)

				An easy way to eliminate collision tests is to sort the sprites 
				in ascending order using each sprite's x- or y-axis coordinate as a key. 
				To check for collisions, simply test a given sprite with the next few on the list.
				Stop testing sprites when you reach a sprite further down the list 
				whose location has a key greater than the location of the first sprite, 
				plus its width or height. 
				
				http://www.ddj.com/documents/s=983/ddj9513a/ */


			ListActor *listActor = mapWorkingActors[**p];
			
			for(int il = 0; listActor && il < listActor->Count(); il++)			
			{
				CheckCollisionsBetween(actor, (*listActor)[il]);
			}
		}
	}

	//Check Inherited Events
	//Will duplicate the collision actions only if the repeat flag is on
	//and both actor and parent have collision check with save actor
	//Solve Event Inheritance Collision.ged

	Action *pInheritedAction = getInheritedEvents();
	if(pInheritedAction)
	{
		pInheritedAction->CheckCollisionList(actor);
	}
}


/*Script *Action::GetExpression()
{
	//Return expression for actual event (must be used on edition only)
	//Only one expression per event

	stAction *action = listActions;
	while(action)
	{
		if( action->actionType == SET_EXPRESSION && 
			action->eventData.eventType == actualEvent.eventType &&
			!bConditionalAction)
		{
			if(actualEvent.eventType == EVENT_COLLISION && 
			   strcmp(action->eventData.data.name, actualEvent.data.name) == 0)
			{
				//One expression per collide actor
				return GameControl::Get()->GetExpression(action->data.name);
			}
			else if((actualEvent.eventType == EVENT_KEYDOWN || actualEvent.eventType == EVENT_KEYUP) && 
				    action->eventData.data.keyEvent.key == actualEvent.data.keyEvent.key)
			{
				//One expression per key
				return GameControl::Get()->GetExpression(action->data.name);
			}
			else if((actualEvent.eventType == EVENT_MOUSEBUTTONDOWN || actualEvent.eventType == EVENT_MOUSEBUTTONUP) && 
				    action->eventData.data.mouse.button == actualEvent.data.mouse.button)
			{
				//One expression per mouse button
				return GameControl::Get()->GetExpression(action->data.name);
			}
			else if(actualEvent.eventType == EVENT_TIMER && 
			   strcmp(action->eventData.data.name, actualEvent.data.name) == 0)
			{
				//One expression per timer event
				return GameControl::Get()->GetExpression(action->data.name);
			}
			else if(actualEvent.eventType != EVENT_COLLISION &&
					actualEvent.eventType != EVENT_KEYDOWN &&
					actualEvent.eventType != EVENT_KEYUP &&
					actualEvent.eventType != EVENT_MOUSEBUTTONDOWN &&
					actualEvent.eventType != EVENT_MOUSEBUTTONUP &&
					actualEvent.eventType != EVENT_TIMER)
			{
				return GameControl::Get()->GetExpression(action->data.name);
			}
			 
		}

		action = action->next;
	}

	return NULL;
}*/


gedString Action::GetActionName(stAction *action)
{
	switch(action->actionType)
	{
	case SET_ANIMATION:	return CHANGE_ANIMATION;		
	case SET_ANIMATION_DIRECTION: 	return CHANGE_ANIMATION_DIRECTION;					
	case SET_TRANSPARENCY: 	return CHANGE_TRANSPARENCY;					
	case SET_COLLISION_STATE: 	return COLLISION_STATE;					
	case SET_VISIBILITY_STATE: 	return VISIBILITY_STATE;					
	case SET_PATH: 	return CHANGE_PATH;					
	case SET_FOLLOW_MOUSE: 	return FOLLOW_MOUSE;					
	case SET_PARENT: 	return CHANGE_PARENT;					
	case SET_CURSOR: 	return CURSOR_POINTER;					
	case SET_CREATE_ACTOR: 	return CREATE_ACTOR;
	case SET_ACTIVATION_EVENT: return ACTIVATION_EVENT;
	case SET_DESTROY_ACTOR:	return DESTROY_ACTOR;				
	case SET_CREATE_TIMER:	return CREATE_TIMER;				
	case SET_DESTROY_TIMER: return DESTROY_TIMER;					
	case SET_EXPRESSION: return EXPRESSION_EDITOR;					
	case SET_SETTEXT: return SET_TEXT;					
	case SET_CONDITIONAL_ACTION: return CONDITIONAL_ACTION;					
	case SET_ZDEPTH: return CHANGE_ZDEPTH;
	case SET_TO_ANTERIOR_POSITION: 	return TO_ANTERIOR_POSITION;					
	case SET_PHYSICAL_BOUNCE: 	return PHYSICAL_BOUNCE;
	case SET_MOVE_TO: 	return MOVE_TO;
	case SET_PLAY_SOUND: //sound and music	
		{
			if(action->data.sound.bMusic) return PLAY_MUSIC;
			else return PLAY_SOUND;
		}
		break;
	case SET_EVENT_CONTROL: //enable and disable
		{
			if(action->data.eventcontrol.bEnable) return EVENT_ENABLE;
			else return EVENT_DISABLE;
		}
		break;
	}
	

	return "";
}







void Action::DetachAction()
{
	nRef--;

	if(!nRef)
	{
		Action::RemoveAction(this);
	}
}

void Action::AttachAction()
{
	nRef++;
}

void Action::CleanUpTimers(Actor *actor, SDL_TimerID timerId)
{
	stTimerEvent *p = mapTimerIdName[timerId];
	if(!p) return;
	
	char buf[256];
	sprintf(buf, "%s%012X\0", p->name, actor);
	
	mapTimerNameId.Remove(buf);
	mapTimerIdName.Remove(timerId);
}

void Action::FixAnimationFinish(Actor *actor)
{
	//Fix game versions < 4

	MapStringActionIterator it(mapAnimationFinishActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(!actor->GetSequence(*it.Key()))
		{
			ListActionsIterator itList(*it.Value());
			for(itList.Begin(); !itList.Done(); itList.Next())
			{
				stAction *action = itList.Current();
				memset(&action->eventData, 0, sizeof(stEvent));
				action->eventData.eventType = EVENT_ANIMATION_FINISH;
				strcpy(action->eventData.data.name, ANY_ANIMATION.getCharBuf());

				mapAnimationFinishActions.FindOrCreate(ANY_ANIMATION)->PushBack(action);
			}

			mapAnimationFinishActions.Remove(*it.Key());
			it.Begin();
		}
	}
}

//SDL_mutex *executeMutEx = SDL_CreateMutex();

long **sStackPtr = NULL;

int spStackPtr = 0, maxStackPtr = 0;

void Action::StoreExecStaticData()
{
	//Store static data (for Execute in Execute call)
	//MuteEx mutex(executeMutEx);

	if(spStackPtr >= maxStackPtr)
	{
		maxStackPtr += 10;

		sStackPtr = (long **)realloc(sStackPtr, 3*maxStackPtr*sizeof(long *));				
	}

	sStackPtr[spStackPtr++] = (long *)actualEventActor;
	sStackPtr[spStackPtr++] = (long *)actualCollideActor;
	//sStackPtr[spStackPtr++] = (long *)actualAction;
}

void Action::RestoreExecStaticData()
{
	//MuteEx mutex(executeMutEx);

	//actualAction = (Action *)sStackPtr[--spStackPtr];
	actualCollideActor = (Actor *)sStackPtr[--spStackPtr];
	actualEventActor = (Actor *)sStackPtr[--spStackPtr];
}

#ifndef STAND_ALONE_GAME
gedString Action::GetDescriptions(Uint32 events)
{
	//Return the description for all requested events
	gedString desc;
	ListString list;

	if(events & EVENTKEYDOWN)
		PopulateKeyActions(mapKeyDown, list);

	if(events & EVENTKEYUP)
		PopulateKeyActions(mapKeyUp, list);

	if(events & EVENTCOLLISION)
		PopulateStringActions(mapCollision, list);

	if(events & EVENTCOLLISIONFINISH)
		PopulateStringActions(mapCollisionFinish, list);
	
	if(events & EVENTACTIVATIONEVENT)
		PopulateStringActions(mapActivationEvent, list);	

	if(events & EVENTTIMER)
		PopulateStringActions(mapTimerNameAction, list);	

	if(events & EVENTANIMATIONFINISH)
		PopulateStringActions(mapAnimationFinishActions, list);
	
	if(events & EVENTANIMATION)
		PopulateListActions(listAnimationActions, list);
	
	if(events & EVENTPATHFINISH)
		PopulateStringActions(mapPathFinishActions, list);
	
	if(events & EVENTCREATE)
		PopulateListActions(listCreateActions, list);

	if(events & EVENTDESTROYACTOR)
		PopulateListActions(listDestroyActorActions, list);

	if(events & EVENTOUTOFVISION)
		PopulateListActions(listOutOfVisionActions, list);

	if(events & EVENTMOUSEENTER)
		PopulateListActions(listMouseEnterActions, list);

	if(events & EVENTMOUSELEAVE)
		PopulateListActions(listMouseLeaveActions, list);

	if(events & EVENTMOVEFINISH)
		PopulateListActions(listMoveFinishActions, list);
	
	if(events & EVENTMOUSEBUTTONDOWN)
		PopulateMouseActions(mapMouseButtonDownActions, list);
	
	if(events & EVENTMOUSEBUTTONUP)
		PopulateMouseActions(mapMouseUpActions, list);

	for(int i = 0; i < list.Count(); i++)
	{	
		if(i > 0) desc += " \n";
		desc += list[i];
	}

	return desc;
}
#endif

gedString Action::GetDescription(stAction *action, bool bActionDescriptionBefore, bool bEventNameOnly, bool bActionDescriptionAfter)
{
	if(!action) return "";

	char str[512];
	str[0] = 0;

	gedString desc;
	
	if(bActionDescriptionBefore)
		desc = GetActionName(action);

	switch(action->eventData.eventType)
	{
	case EVENT_KEYDOWN:
		if(!bActionDescriptionBefore) desc = EV_KEY_DOWN;

#ifndef STAND_ALONE_GAME
		if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
		{
			if(Tutorial::IsCompatible(VERSION_GET_ANIMINDEX))
			{
				gedString keys;
				
				for(int i = 0; i < 2; i++)
				{
					if(!keys.empty()) keys += " ";
					if(!action->eventData.data.keyEvent.keySequence[i])
					{
						break;
					}

					keys += SDL_GetKeyName((SDLKey)action->eventData.data.keyEvent.keySequence[i]);
				}

				if(action->eventData.data.keyEvent.keySequence[2])
				{
					keys += "...";
				}

				
				if(action->eventData.data.keyEvent.key >= SDLK_ALL_KEYS_PRESSED)
				{
					if(action->eventData.data.keyEvent.bDisableRepeat)
					{
						sprintf(str, "%s (%s, %s)", desc.c_str(), keys.getCharBuf(), SDL_GetKeyName(action->eventData.data.keyEvent.key));					
					}
					else
					{
						sprintf(str, "%s (%s, %s, repeat)", desc.c_str(), keys.getCharBuf(), SDL_GetKeyName(action->eventData.data.keyEvent.key));	
					}
				}
				else
				{
					if(action->eventData.data.keyEvent.bDisableRepeat)
					{
						sprintf(str, "%s (%s, At least one key is pressed)", desc.c_str(), keys.getCharBuf());					
					}
					else
					{
						sprintf(str, "%s (%s, At least one key is pressed, repeat)", desc.c_str(), keys.getCharBuf());	
					}
				}
			}
			else
			{
				if(action->eventData.data.keyEvent.bDisableRepeat)
				{
					sprintf(str, "%s (%s)", desc.c_str(), SDL_GetKeyName(action->eventData.data.keyEvent.key));					
				}
				else
				{
					sprintf(str, "%s (%s, repeat)", desc.c_str(), SDL_GetKeyName(action->eventData.data.keyEvent.key));	
				}		
			}
			
		}
		else
#endif
		{
			sprintf(str, "%s (%s)", desc.c_str(), SDL_GetKeyName(action->eventData.data.keyEvent.key));		
		}
		break;
	case EVENT_KEYUP:
		if(!bActionDescriptionBefore) desc = EV_KEY_UP;
		sprintf(str, "%s (%s)", desc.c_str(), SDL_GetKeyName(action->eventData.data.keyEvent.key));		
		break;	
	case EVENT_COLLISION:
		{
			if(!bActionDescriptionBefore) desc = EV_COLLISION;
			
			gedString side("Any Side");
			switch(action->eventData.data.name[NAME_LIMIT-2])
			{
			case TOP_SIDE:
				side = "Top Side";
				break;
			case BOTTOM_SIDE:
				side = "Bottom Side";
				break;
			case LEFT_SIDE:
				side = "Left Side";
				break;
			case RIGHT_SIDE:
				side = "Right Side";
				break;
			case TOP_OR_BOTTOM_SIDE:
				side = "Top or Bottom Side";
				break;
			case LEFT_OR_RIGHT_SIDE:
				side = "Left or Right Side";
				break;
			}
			
			sprintf(str, "%s (%s of %s)", desc.c_str(), side.c_str(), action->eventData.data.name);
		}
		break;
	case EVENT_COLLISION_FINISH:
		if(!bActionDescriptionBefore) desc = EV_COLLISION_FINISH;
		sprintf(str, "%s (%s)", desc.c_str(), action->eventData.data.name);		
		break;
	case EVENT_ACTIVATION_EVENT:
		if(!bActionDescriptionBefore) desc = EV_ACTIVATION_EVENT;
		sprintf(str, "%s (%s)", desc.c_str(), action->eventData.data.name);		
		break;
	case EVENT_TIMER:
		if(!bActionDescriptionBefore) desc = EV_TIMER;
		sprintf(str, "%s (%s)", desc.c_str(), action->eventData.data.name);		
		break;
	case EVENT_ANIMATION_FINISH:
		if(!bActionDescriptionBefore) desc = EV_ANIMATION_FINISH;
		sprintf(str, "%s (%s)", desc.c_str(), action->eventData.data.name);		
		break;
	case EVENT_ANIMATION:
		if(!bActionDescriptionBefore) desc = EV_ANIMATION;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_PATH_FINISH:
		if(!bActionDescriptionBefore) desc = EV_PATH_FINISH;
		sprintf(str, "%s (%s)", desc.c_str(), action->eventData.data.name);
		break;
	case EVENT_CREATE:
		if(!bActionDescriptionBefore) desc = EV_CREATE;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_DESTROY_ACTOR:
		if(!bActionDescriptionBefore) desc = EV_DESTROY_ACTOR;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_OUT_OF_VISION:
		if(!bActionDescriptionBefore) desc = EV_OUT_OF_VISION;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_MOUSEENTER:
		if(!bActionDescriptionBefore) desc = EV_MOUSE_ENTER;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_MOUSELEAVE:
		if(!bActionDescriptionBefore) desc = EV_MOUSE_LEAVE;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_MOVEFINISH:
		if(!bActionDescriptionBefore) desc = EV_MOVE_FINISH;
		sprintf(str, "%s", desc.c_str());
		break;
	case EVENT_MOUSEBUTTONDOWN:
		{
			if(!bActionDescriptionBefore) desc = EV_MOUSE_BUTTON_DOWN;
			gedString sbutton;
			if(action->eventData.data.mouse.button == SDL_BUTTON_RIGHT) sbutton = "Right";
			else if(action->eventData.data.mouse.button == SDL_BUTTON_LEFT) sbutton = "Left";
			else if(action->eventData.data.mouse.button == SDL_BUTTON_MIDDLE) sbutton = "Middle";

#ifndef STAND_ALONE_GAME
			if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
			{
				if(action->eventData.data.mouse.bDrag)
				{
					sprintf(str, "%s (%s, drag)", desc.c_str(), sbutton.c_str());		
				}
				else
				{
					sprintf(str, "%s (%s)", desc.c_str(), sbutton.c_str());		
				}
			}
			else
#endif
			{
				sprintf(str, "%s (%s)", desc.c_str(), sbutton.c_str());		
			}
		}
		break;
	case EVENT_MOUSEBUTTONUP:
		{
			if(!bActionDescriptionBefore) desc = EV_MOUSE_BUTTON_UP;
			gedString sbutton;
			if(action->eventData.data.mouse.button == SDL_BUTTON_RIGHT) sbutton = "Right";
			else if(action->eventData.data.mouse.button == SDL_BUTTON_LEFT) sbutton = "Left";
			else if(action->eventData.data.mouse.button == SDL_BUTTON_MIDDLE) sbutton = "Middle";

			sprintf(str, "%s (%s)", desc.c_str(), sbutton.c_str());		
		}
		break;
	}	

	if(bActionDescriptionAfter)
	{
		desc = str;
		desc += " -> ";
		desc += GetActionName(action);

		return desc;
	}
	else if(bEventNameOnly && !bActionDescriptionBefore)
	{
		return desc;
	}
	else
	{
		return str;
	}
}

void Action::ClearActorsInCollision()
{
	mapActorsInCollision.Clear();
}

gedString Action::VerifyScripts(bool bVerifyScriptsAddedWithError)
{
	//Re parser all action scripts	
	//Return: Actor, Event, Action, if error

	gedString error;
	Script *pCurrentEditorScript = NULL;

#ifndef STAND_ALONE_GAME
	if(ExpressionEditor::getExpressionEditor())
	{
		pCurrentEditorScript = ExpressionEditor::getExpressionEditor()->getScript();
	}
#endif

	stAction *action = listActions;
	while(action)
	{
		gedString scriptName;
		switch(action->actionType)
		{
		case SET_EXPRESSION:
			scriptName = action->data.name;			
			break;
		case SET_CONDITIONAL_ACTION:
			scriptName = action->data.conditionalaction.expression1;
			break;		
		}

		if(scriptName.length() > 0)
		{
			Script *pScript = GameControl::Get()->GetExpression(scriptName.c_str());
			if(pScript && pScript != pCurrentEditorScript && (!pScript->getAddedWithError() || bVerifyScriptsAddedWithError))
			{
				//Use On Demand flag only if not test errors
				//Solve the crash in GE Rally error.ged (stand alone)
				pScript->Parser(!bVerifyScriptsAddedWithError, true);
				if(pScript->GetError().length() > 0 && pScript->GetError().find("Error") != gedString::npos)
				{
					if(error.length() > 0) error += "\n";

					error += getActionName();
					error += "->";
					error += GetDescription(action, false);
					error += "->";
					error += GetDescription(action, true);
					
					//Remove last (...)
					int i = error.rfind(" (");
					if(i != gedString::npos)
					{
						error = error.substr(0, i);
					}
				}
			}
		}
		
		action = action->next;
	}

	if(error.length()) error += "\n";
	return error;
}

void Action::SetExpressionPointers()
{
	//Cache address on x var

	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		stAction *action = (*it.Value())->listActions;
		
		while(action)
		{
			if(action->actionType == SET_EXPRESSION)
			{
				Script *pScript = GameControl::Get()->GetExpression(action->data.name);
				action->x = (Sint32)pScript;
			}	
			
			action = action->next;
		}
	}
}


#ifndef STAND_ALONE_GAME
gedString Action::MapScriptFunctionToAction(const gedString& scriptFunction)
{
	if(scriptFunction == "ChangeAnimation") return CHANGE_ANIMATION;
	else if(scriptFunction == "ChangeAnimationDirection") return CHANGE_ANIMATION_DIRECTION;
	else if(scriptFunction == "ChangeTransparency") return CHANGE_TRANSPARENCY;
	else if(scriptFunction == "CollisionState") return COLLISION_STATE;
	else if(scriptFunction == "VisibilityState") return VISIBILITY_STATE;
	else if(scriptFunction == "ChangePath") return CHANGE_PATH;
	else if(scriptFunction == "FollowMouse") return FOLLOW_MOUSE;
	else if(scriptFunction == "ChangeParent") return CHANGE_PARENT;
	else if(scriptFunction == "ChangeCursor") return CURSOR_POINTER;
	else if(scriptFunction == "CreateActor") return CREATE_ACTOR;
	else if(scriptFunction == "DestroyActor") return DESTROY_ACTOR;
	else if(scriptFunction == "CreateTimer") return CREATE_TIMER;
	else if(scriptFunction == "DestroyTimer") return DESTROY_TIMER;
	else if(scriptFunction == "ChangeZDepth") return CHANGE_ZDEPTH;
	else if(scriptFunction == "PlaySound" || scriptFunction == "PlaySound2") return PLAY_SOUND;
	else if(scriptFunction == "PlayMusic" || scriptFunction == "PlayMusic2") return PLAY_MUSIC;
	else if(scriptFunction == "ToAnteriorPosition") return TO_ANTERIOR_POSITION;
	else if(scriptFunction == "EventEnable") return EVENT_ENABLE;
	else if(scriptFunction == "EventDisable") return EVENT_DISABLE;
	else if(scriptFunction == "MoveTo") return MOVE_TO;
	else if(scriptFunction == "PhysicalResponse") return PHYSICAL_BOUNCE;

	return "";
}

void Action::DoAction(Actor *actor, const gedString& actionName, bool bOnCollision)
{
	if(actionName == CHANGE_ANIMATION)
	{
		new ChangeAnimation(actor);
	}
	else if(actionName == CHANGE_ANIMATION_DIRECTION)
	{
		new ChangeAnimationDirection(actor, bOnCollision);
	}
	else if(actionName == CHANGE_PATH)
	{
		if(GameControl::Get()->NumPaths())
		{
			new ChangePath(actor, bOnCollision);
		}
		else
		{
			new PanelInfo("Current project has no paths");
		}
	}
	else if(actionName == FOLLOW_MOUSE)
	{
		new ToMousePosition(actor, bOnCollision);
	}
	else if(actionName == CHANGE_PARENT)
	{
		new ChangeParent(actor, bOnCollision);	
	}
	else if(actionName == CURSOR_POINTER)
	{
		new ChangeCursor(actor);	
	}
	else if(actionName == CREATE_ACTOR)
	{
		if(GameControl::Get()->NumActors() > 1)
		{
			new CreateActor(actor, bOnCollision);
		}
		else
		{
			//Only the view in project
			new PanelInfo("Current project has no actors");
		}
	}
	else if(actionName == DESTROY_ACTOR)
	{
		new DestroyActor(actor, bOnCollision);	
	}
	else if(actionName == CREATE_TIMER)
	{
		new CreateTimer(actor, bOnCollision);	
	}
	else if(actionName == DESTROY_TIMER)
	{
		if(GameControl::Get()->GetNumTimers())
		{
			new DestroyTimer(actor);	
		}
		else
		{
			new PanelInfo("Current project has no timers");
		}		
	}
	else if(actionName == COLLISION_STATE ||
		    actionName == VISIBILITY_STATE)
	{
		new EnableDisable(actor, actionName, bOnCollision);	
	}
	else if(actionName == CHANGE_TRANSPARENCY)
	{
		new Transparency(actor, bOnCollision);	
	}
	else if(actionName == EXPRESSION_EDITOR)
	{
		new ExpressionEditor(actor, bOnCollision);
	}
	else if(actionName == SET_TEXT)
	{
		new ActorText(actor, bOnCollision, true);
	}
	else if(actionName == CONDITIONAL_ACTION)
	{
		new ConditionalAction(actor, bOnCollision);
	}
	else if(actionName == CHANGE_ZDEPTH)
	{
		new SetZDepth(actor, bOnCollision);
	}
	else if(actionName == PLAY_SOUND)
	{
		new PlaySong(actor, bOnCollision, false);	
	}
	else if(actionName == PLAY_MUSIC)
	{
		new PlaySong(actor, bOnCollision, true);	
	}
	else if(actionName == EVENT_ENABLE)
	{
		new EventControl(actor, bOnCollision, true);	
	}
	else if(actionName == EVENT_DISABLE)
	{
		new EventControl(actor, bOnCollision, false);	
	}
	else if(actionName == TO_ANTERIOR_POSITION)
	{
		new ToAnteriorPosition(actor, bOnCollision);	
	}
	else if(actionName == MOVE_TO)
	{
		new MoveTo(actor, bOnCollision);	
	}
	else if(actionName == ACTIVATION_EVENT)
	{
		//Do nothing
	}
	else if(actionName == PHYSICAL_BOUNCE)
	{
		if(actualEvent.eventType == EVENT_COLLISION)
			new PhysicalBounce(actor);
		else
			new PanelInfo("Physical Response available only in Collision event");
	}
}

void Action::Populate(ListPop *list)
{ 
	//Dont show activation event in actor control

	list->AddText(CHANGE_ANIMATION);
	list->AddText(CHANGE_ANIMATION_DIRECTION);
	list->AddText(CHANGE_PATH);	
	list->AddText(FOLLOW_MOUSE);
	list->AddText(CHANGE_TRANSPARENCY);
	list->AddText(CHANGE_PARENT);	
	list->AddText(CURSOR_POINTER);
	list->AddText(CREATE_TIMER);
	list->AddText(DESTROY_ACTOR);
	list->AddText(DESTROY_TIMER);	
	list->AddText(CREATE_ACTOR);	
	list->AddText(COLLISION_STATE);
	list->AddText(VISIBILITY_STATE);
	list->AddText(EXPRESSION_EDITOR);
	list->AddText(SET_TEXT);
	list->AddText(CONDITIONAL_ACTION);
	list->AddText(CHANGE_ZDEPTH);
	list->AddText(PLAY_SOUND);
	list->AddText(PLAY_MUSIC);
	list->AddText(EVENT_ENABLE);
	list->AddText(EVENT_DISABLE);
	list->AddText(TO_ANTERIOR_POSITION);
	list->AddText(MOVE_TO);
	list->AddText(PHYSICAL_BOUNCE);
	//list->AddText(SEND_MESSAGE);

	list->Sort();
}

void Action::PopulateConditionalActions(ListPop *listActions, stAction *condAction)
{
	listActions->RemoveAll();
	
	condAction = condAction->next; //Go to conditional action
	if(condAction)
	{
		//Search conditional action until other test 
		//or finish conditional actions
		
		do
		{
			int i = listActions->AddText(GetActionName(condAction));
			listActions->SetItemData(i, condAction);

			condAction = condAction->next;
		} while(condAction && condAction->actionType != SET_CONDITIONAL_ACTION && condAction->bConditional);
	}
}

void Action::PopulateKeyActions(MapKeys& map, ListPop *listActions)
{
	//Dont show activation events

	MapKeysIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				int i = listActions->AddText(GetDescription(itList.Current(), true), true);	
				listActions->SetItemData(i, itList.Current());
			}
		}
	}
}

void Action::PopulateKeyActions(MapKeys& map, ListString &list)
{
	//Dont show activation events

	MapKeysIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				list.PushBack(GetDescription(itList.Current(), false, false, true));	
			}
		}
	}
}

void Action::PopulateStringActions(MapStringAction& map, ListPop *listActions)
{
	//Dont show activation events

	MapStringActionIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				int i = listActions->AddText(GetDescription(itList.Current(), true), true);		
				listActions->SetItemData(i, itList.Current());
			}
		}
	}
}

void Action::PopulateStringActions(MapStringAction& map, ListString &list)
{
	//Dont show activation events

	MapStringActionIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				list.PushBack(GetDescription(itList.Current(), false, false, true));	
			}
		}
	}
}

void Action::PopulateListActions(ListActions& list, ListPop *listActions)
{
	//Dont show activation events

	ListActionsIterator itList(list);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
		{
			int i = listActions->AddText(GetDescription(itList.Current(), true), true);		
			listActions->SetItemData(i, itList.Current());
		}
	}
}

void Action::PopulateListActions(ListActions& list, ListString &listString)
{
	//Dont show activation events

	ListActionsIterator itList(list);
	for(itList.Begin(); !itList.Done(); itList.Next())
	{
		if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
		{
			listString.PushBack(GetDescription(itList.Current(), false, false, true));	
		}
	}
}

void Action::PopulateMouseActions(MapMouseButtons& map, ListPop *listActions)
{
	//Dont show activation events

	MapMouseButtonsIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				int i = listActions->AddText(GetDescription(itList.Current(), true), true);	
				listActions->SetItemData(i, itList.Current());
			}
		}
	}	
}

void Action::PopulateMouseActions(MapMouseButtons& map, ListString &list)
{
	//Dont show activation events

	MapMouseButtonsIterator it(map);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		ListActionsIterator itList(*it.Value());
		for(itList.Begin(); !itList.Done(); itList.Next())
		{
			if(!itList.Current()->bConditional && itList.Current()->actionType != SET_ACTIVATION_EVENT)
			{
				list.PushBack(GetDescription(itList.Current(), false, false, true));
			}
		}
	}	
}

void Action::PopulateEventActions(ListPop *listActions, const gedString& eventName)
{ 	
	if(eventName == EV_KEY_DOWN)
		PopulateKeyActions(mapKeyDown, listActions);

	else if(eventName == EV_KEY_UP)
		PopulateKeyActions(mapKeyUp, listActions);

	else if(eventName == EV_COLLISION)
		PopulateStringActions(mapCollision, listActions);

	else if(eventName == EV_COLLISION_FINISH)
		PopulateStringActions(mapCollisionFinish, listActions);
	
	else if(eventName == EV_ACTIVATION_EVENT)
		PopulateStringActions(mapActivationEvent, listActions);	

	else if(eventName == EV_TIMER)
		PopulateStringActions(mapTimerNameAction, listActions);	

	else if(eventName == EV_ANIMATION_FINISH)
		PopulateStringActions(mapAnimationFinishActions, listActions);
	
	else if(eventName == EV_ANIMATION)
		PopulateListActions(listAnimationActions, listActions);
	
	else if(eventName == EV_PATH_FINISH)
		PopulateStringActions(mapPathFinishActions, listActions);
	
	else if(eventName == EV_CREATE)
		PopulateListActions(listCreateActions, listActions);

	else if(eventName == EV_DESTROY_ACTOR)
		PopulateListActions(listDestroyActorActions, listActions);

	else if(eventName == EV_OUT_OF_VISION)
		PopulateListActions(listOutOfVisionActions, listActions);

	else if(eventName == EV_MOUSE_ENTER)
		PopulateListActions(listMouseEnterActions, listActions);

	else if(eventName == EV_MOUSE_LEAVE)
		PopulateListActions(listMouseLeaveActions, listActions);

	else if(eventName == EV_MOVE_FINISH)
		PopulateListActions(listMoveFinishActions, listActions);
	
	else if(eventName == EV_MOUSE_BUTTON_DOWN)
		PopulateMouseActions(mapMouseButtonDownActions, listActions);
	
	else if(eventName == EV_MOUSE_BUTTON_UP)
		PopulateMouseActions(mapMouseUpActions, listActions);	
}

void Action::DoEvent(Actor *actor, const gedString& eventName, bool _bActivationEvent)
{
	bActivationEvent = _bActivationEvent;
	if(eventName == EV_KEY_DOWN)
	{
		new DlgEventKeyDown(actor);
	}
	else if(eventName == EV_KEY_UP)
	{
		new DlgEventKeyDown(actor, true);
	}
	else if(eventName == EV_COLLISION)
	{
		new DlgCollision(actor, true);
	}
	else if(eventName == EV_COLLISION_FINISH)
	{ 
		new DlgCollision(actor, false);
	}
	else if(eventName == EV_TIMER)
	{
		if(GameControl::Get()->GetNumTimers())
		{
			new DlgTimer(actor);
		}
		else
		{
			new PanelInfo("Current project has no timers\nPlease, add a new timer on actor first");
		}
	}
	else if(eventName == EV_ANIMATION ||
			eventName == EV_CREATE ||
			eventName == EV_DESTROY_ACTOR ||
			eventName == EV_OUT_OF_VISION ||
			eventName == EV_MOUSE_ENTER ||
			eventName == EV_MOUSE_LEAVE ||
			eventName == EV_MOVE_FINISH)
	{
		new DlgGeneralEvent(actor, eventName);
	}
	else if(eventName == EV_ANIMATION_FINISH)
	{
		new DlgAnimationFinish(actor);
	}
	else if(eventName == EV_PATH_FINISH)
	{
		new DlgPathFinish(actor);
	}
	else if(eventName == EV_MOUSE_BUTTON_DOWN)
	{
		new DlgMouseButton(actor);
	}
	else if(eventName == EV_MOUSE_BUTTON_UP)
	{
		new DlgMouseButton(actor, true);
	}
	else if(eventName == EV_ACTIVATION_EVENT)
	{
		new DlgActivationEvent(actor);
	}
}


void Action::SaveSound(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex)
{
	//Save all game sound from game path to savepath

	stAction *action = listActions;
	while(action)
	{
		if(action->actionType == SET_PLAY_SOUND)
		{		
			if(!audioIndex.FindString(action->data.sound.path))
			{
				//Get start audio position
				SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
				Uint32 startAudioPos = SDL_RWtell( exeFile ), size = 0;
				
				if(action->data.sound.bMusic)
				{
					copy((GameControl::Get()->getGamePath() + DIR_SEP + action->data.sound.path).c_str(), exeFile);
				}
				else
				{
					//Compress data
					Compression comp;
					comp.DoCompression((GameControl::Get()->getGamePath() + DIR_SEP + action->data.sound.path).c_str(), exeFile);
				}
				
				//Get audio size
				SDL_RWseek( exeFile, 0, SEEK_END ); //tell error without this
				size = SDL_RWtell( exeFile ) - startAudioPos;
				
				if(size > 0)
				{
					audioIndex.Add(action->data.sound.path, stAudioIndex(startAudioPos, size));
				}
			}
		}
		else if(action->actionType == SET_EXPRESSION)
		{
			//Process sounds in script
			Script *pScript = GameControl::Get()->GetExpression(action->data.name);
			if(pScript)
			{
				SaveSoundList(pScript->GetSoundList(), exeFile, audioIndex);				
			}
		}

		action = action->next;
	}
}

void Action::SaveSoundTo(SDL_RWops *exeFile, MapAudioFileIndex &audioIndex)
{
	//Put all game sounds into executable

	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		(*it.Value())->SaveSound(exeFile, audioIndex);		
	}
}

void stAction::Save(SDL_RWops *src)
{
	if(!GameControl::Get()->getFastFile())
	{
		//Save compressed action
		stAction tmp, tmp1;
		
		Compression comp;
		int len = comp.DoCompression(this, sizeof(stAction), &tmp, sizeof(stAction));
		
		SDL_WriteLE16(src, len);
		SDL_RWwrite(src, &tmp, len, 1);
	}
	else
	{
		//Save uncompressed action			
		SDL_WriteLE16(src, 0);
		SDL_RWwrite(src, this, sizeof(stAction), 1);
	}
	
	if(!GameControl::Get()->getExportMode())
	{
		//Save description
		if(pDescription)
		{
			pDescription->Write(src);
		}
		else
		{
			//Write blank string
			gedString tmp;
			tmp.Write(src);
		}
	}
}

void Action::Save(SDL_RWops *src)
{
	actionName.Write(src);
	InheritedEvents.Write(src);

	SDL_WriteLE16(src, nActions);

	//Action size
	SDL_WriteLE16(src, sizeof(stAction));

	//Out of Vision Optimization
	Uint8 OutOfVisionOptimization = bOutOfVisionOptimization;
	SDL_RWwrite(src, &OutOfVisionOptimization, sizeof(OutOfVisionOptimization), 1);

	stAction *action = listActions;
	while(action)
	{
		switch(action->actionType)
		{
		case SET_CURSOR:
			{
				strcpy(action->data.cursor.imgName, GameControl::Get()->AdjustFileName(action->data.cursor.imgName).c_str());
				
				//Add cursor to resource for game export
				KrSpriteResource *resource;
							
				if((resource = engine->Vault()->GetSpriteResource(action->data.cursor.imgName)) == NULL)
				{		
					resource = LoadTiledSprite(action->data.cursor.imgName, 
						action->data.cursor.imgName, 
						action->data.cursor.nFramesH, 
						action->data.cursor.nFramesV, 
						NULL, NULL, 
						action->data.cursor.hotSpotX, action->data.cursor.hotSpotY);

					if(resource)
					{
						engine->Vault()->AddResource(resource);
					}
				}
			}
			break;
		case SET_SETTEXT:
			//Copy font to correct path
			strcpy(action->data.settext.fontPath, GameControl::Get()->AdjustFileName(action->data.settext.fontPath).c_str());

			//Put font on the kyra (avoid export without this font)
			GameControl::Get()->GetFont(action->data.settext.fontPath, action->data.settext.iniASCIIChar, action->data.settext.nChars);
			break;
		case SET_PLAY_SOUND:
			strcpy(action->data.sound.path, GameControl::Get()->AdjustFileName(action->data.sound.path).c_str());
			break;
		}

		action->Save(src);		
		
		action = action->next;
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);

}

void Action::SaveActions(SDL_RWops *src, Uint32 *indexAction)
{
	//Save all game actions

	SDL_WriteLE16(src, globalMapActions.size());

	int n = 0;
	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		//Store index
		SDL_RWseek( src, 0, SEEK_END ); //tell error without this
		indexAction[n++] = SDL_RWtell( src );

		//Save action
		(*it.Value())->Save(src);		
	}

	//Final expansion indicator
	Uint8 expansion = 0;
	SDL_RWwrite(src, &expansion, sizeof(expansion), 1);
}

void Action::PopulateScriptActions(ListPop *list)
{
	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		stAction *action = (*it.Value())->listActions;
		while(action)
		{
			if(action->actionType == SET_EXPRESSION)
			{
				gedString title((*it.Value())->actionName);
				title += " -> ";				
				title += Action::GetDescription(action, false);
				
				int i = list->AddText(title, true); //true to avoid hide scripts with the same description (see FortuneCookie.ged)
				list->SetItemData(i, action);
			}
			
			action = action->next;
		}		
	}
}

void Action::PopulateScriptActions(ListData &list)
{
	//To the wx interface
	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		stAction *action = (*it.Value())->listActions;
		while(action)
		{
			if(action->actionType == SET_EXPRESSION)
			{
				gedString title((*it.Value())->actionName);
				title += ",";				
				title += Action::GetDescription(action, false);
	
				list.PushBack(stListData(title, action));
			}
			
			action = action->next;
		}		
	}
}

void Action::PopulateEvents(ListString &list, Actor *actor)
{
	Action *action = NULL;
	if(actor) action = actor->getAction();

	if(!actor || (action && action->HaveMouseButtonDown(false))) list.PushBack(EV_MOUSE_BUTTON_DOWN);
	if(!actor || (action && action->HaveMouseUpActions(false))) list.PushBack(EV_MOUSE_BUTTON_UP);
	if(!actor || (action && action->HaveAnimation(false))) list.PushBack(EV_ANIMATION);
	if(!actor || (action && action->HaveAnimationFinish(false))) list.PushBack(EV_ANIMATION_FINISH);
	if(!actor || (action && action->HavePathFinish(false))) list.PushBack(EV_PATH_FINISH);
	if(!actor || (action && action->HaveKeyDown(false))) list.PushBack(EV_KEY_DOWN);
	if(!actor || (action && action->HaveKeyUp(false))) list.PushBack(EV_KEY_UP);
	if(!actor || (action && action->HaveTimer(false))) list.PushBack(EV_TIMER);
	if(!actor || (action && action->HaveCollisions(false))) list.PushBack(EV_COLLISION);
	if(!actor || (action && action->HaveCollisionFinish(false))) list.PushBack(EV_COLLISION_FINISH);
	if(!actor || (action && action->HaveCreate(false))) list.PushBack(EV_CREATE);	
	if(!actor || (action && action->HaveDestroyActor(false))) list.PushBack(EV_DESTROY_ACTOR);	
	if(!actor || (action && action->HaveOutOfVision(false))) list.PushBack(EV_OUT_OF_VISION);	
	if(!actor || (action && action->HaveActivationEvent(false))) list.PushBack(EV_ACTIVATION_EVENT);	
	
	if(Tutorial::IsCompatible(VERSION_MOUSE_ENTER_MOUSE_LEAVE))
	{
		if(!actor || (action && action->HaveMouseEnter(false))) list.PushBack(EV_MOUSE_ENTER);	
		if(!actor || (action && action->HaveMouseLeave(false))) list.PushBack(EV_MOUSE_LEAVE);
		if(!actor || (action && action->HaveMoveFinish(false))) list.PushBack(EV_MOVE_FINISH);
	}
	//if(!actor || (action && action->HaveMessage())) list.PushBack(EV_MESSAGE);
}

void Action::PopulateEvents(ListPop *list, Actor *actor)
{
	list->RemoveAll();

	ListString listString;
	PopulateEvents(listString, actor);	

	for(int i = 0; i < listString.Count(); i++)
	{
		list->AddText(listString[i]);
	}

	list->Sort();
}

void Action::ShowActionDialog(stAction *action, Actor *eventActor)
{
	setEditAction(action);
	actualEvent = action->eventData;

	DoAction(eventActor, 
		     GetActionName(action), 
			 action->eventData.eventType == EVENT_COLLISION || action->eventData.eventType == EVENT_TIMER);
}

void Action::ShowEventDialog(stAction *action, Actor *eventActor)
{
	setEditAction(action);
	actualEvent = action->eventData;

	DoEvent(eventActor, GetDescription(action, false, true), false);
}

void Action::EventAnimation()
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_ANIMATION;
}

void Action::EventAnimationFinish(const gedString& animationName)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_ANIMATION_FINISH;

	gedString aux(animationName);
	strcpy(actualEvent.data.name, aux.c_str());
}

void Action::EventPathFinish(const gedString& pathName)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_PATH_FINISH;

	gedString aux(pathName);
	strcpy(actualEvent.data.name, aux.c_str());
}

void Action::EventCreate()
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_CREATE;
}

void Action::EventDestroyActor()
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_DESTROY_ACTOR;
}

void Action::EventOutOfVision()
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_OUT_OF_VISION;
}

void Action::EventMoveFinish()
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOVEFINISH;
}

void Action::EventMouseEnter(int x, int y)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOUSEENTER;
	actualEvent.data.mouse.x = x;
	actualEvent.data.mouse.y = y;
}

void Action::EventMouseLeave(int x, int y)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOUSELEAVE;
	actualEvent.data.mouse.x = x;
	actualEvent.data.mouse.y = y;
}

void Action::EventMouseMove(int x, int y, Uint8 button)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOUSEMOVE;
	actualEvent.data.mouse.x = x;
	actualEvent.data.mouse.y = y;	
	actualEvent.data.mouse.button = button;
}

void Action::EventMouseButtonDown(int x, int y, Uint8 button, bool bDrag)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOUSEBUTTONDOWN;
	actualEvent.data.mouse.x = x;
	actualEvent.data.mouse.y = y;	
	actualEvent.data.mouse.button = button;
	actualEvent.data.mouse.bDrag = bDrag;
}

void Action::EventMouseButtonUp(int x, int y, Uint8 button)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MOUSEBUTTONUP;
	actualEvent.data.mouse.x = x;
	actualEvent.data.mouse.y = y;	
	actualEvent.data.mouse.button = button;
}

void Action::EventCollision(const gedString& collideActorName, bool bRepeatCollision, int side)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_COLLISION;

	gedString aux(collideActorName);
	strcpy(actualEvent.data.name, aux.c_str());

	//Without space in stEvent
	//Work around: use last caracter in data.name
	actualEvent.data.name[NAME_LIMIT-1] = !bRepeatCollision;//Use !bRepeatCollision to backward compatibility

	//Work around: use last caracter - 1 in data.name
	actualEvent.data.name[NAME_LIMIT-2] = side;
}

void Action::EventCollisionFinish(const gedString& collideActorName)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_COLLISION_FINISH;

	gedString aux(collideActorName);
	strcpy(actualEvent.data.name, aux.c_str());
}

void Action::EventActivationEvent(const gedString& fromActorCloneName)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_ACTIVATION_EVENT;

	gedString aux(fromActorCloneName);
	strcpy(actualEvent.data.name, aux.c_str());
}

void Action::EventTimer(const gedString& timerName)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_TIMER;

	gedString s(timerName);
	int pos = timerName.find(" (");

	if(pos != gedString::npos)
	{
		((char *)s.c_str())[pos] = 0;
	}

	strcpy(actualEvent.data.name, s.c_str());
}

void Action::EventKeyDown(SDLKey key, bool bDisableRepeat, unsigned short *keySequence)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_KEYDOWN;
	actualEvent.data.keyEvent.key = key;
	actualEvent.data.keyEvent.bDisableRepeat = bDisableRepeat;
	memcpy(actualEvent.data.keyEvent.keySequence, keySequence, MAX_KEY_SEQUENCE*sizeof(unsigned short));
}

void Action::EventKeyUp(SDLKey key)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_KEYUP;
	actualEvent.data.keyEvent.key = key;
}

void Action::EventMessage(Actor *fromActor, Uint32 messageId, Uint32 message)
{
	memset(&actualEvent, 0, sizeof(actualEvent));
	actualEvent.eventType = EVENT_MESSAGE;
	actualEvent.data.message.fromActor = fromActor;
	actualEvent.data.message.messageId = messageId;
	actualEvent.data.message.message   = message;
}

stAction * Action::SetAnimation(const gedString& actionActor, const gedString& name, int state)
{
	stAction *action = newAction(actionActor, SET_ANIMATION);

	gedString aux(name);
	strcpy(action->data.animation.name, aux.c_str());
	action->data.animation.state = state;

	return action;
}

stAction * Action::SetAnimationDirection(const gedString& actionActor, int state)
{
	stAction *action = newAction(actionActor, SET_ANIMATION_DIRECTION);

	action->data.animation.state = state;

	return action;
}

stAction * Action::SetTransparency(const gedString& actionActor, double trans)
{
	stAction *action = newAction(actionActor, SET_TRANSPARENCY);

	action->data.transparency.data.fixed = trans;

	return action;
}

stAction * Action::SetCollisionEnable(const gedString& actionActor, bool bEnable)
{
	stAction *action = newAction(actionActor, SET_COLLISION_STATE);

	action->data.bEnable = bEnable;

	return action;
}

stAction * Action::SetVisibilityEnable(const gedString& actionActor, int state)
{
	stAction *action = newAction(actionActor, SET_VISIBILITY_STATE);

	action->data.bEnable = state;

	return action;
}

stAction * Action::SetCreateTimer(const gedString& actionActor, const gedString& timerName, int miliseconds)
{
	stAction *action = newAction(actionActor, SET_CREATE_TIMER);

	gedString aux(timerName);
	strcpy(action->data.timer.name, aux.c_str());
	action->data.timer.miliseconds = miliseconds;

	int pos = timerName.find(" (");
	if(pos >= 0) action->data.timer.name[pos] = 0;

	return action;
}

stAction * Action::SetDestroyTimer(const gedString& actionActor, const gedString& timerName)
{
	stAction *action = newAction(actionActor, SET_DESTROY_TIMER);

	gedString aux(timerName);
	strcpy(action->data.timer.name, aux.c_str());

	int pos = timerName.find(" (");
	if(pos >= 0) action->data.timer.name[pos] = 0;

	return action;
}

stAction * Action::SetParent(const gedString& actionActor, const gedString& name)
{
	stAction *action = newAction(actionActor, SET_PARENT);

	gedString aux(name);
	strcpy(action->data.name, aux.c_str());

	return action;
}

stAction * Action::SetPath(const gedString& actionActor, const gedString& name, int axis)
{
	stAction *action = newAction(actionActor, SET_PATH);

	gedString aux(name);
	strcpy(action->data.path.name, aux.c_str());
	action->data.path.axis = axis;

	return action;
}

stAction * Action::SetFollowMouse(const gedString& actionActor, int axis)
{
	stAction *action = newAction(actionActor, SET_FOLLOW_MOUSE);

	action->data.path.axis = axis;

	return action;
}

stAction * Action::SetPhysicalBounce(int moveType, int massType, double massEventActor, double massCollideActor, double eventVelocityMultiplier, double collideVelocityMultiplier)
{
	stAction *action = newAction("", SET_PHYSICAL_BOUNCE);

	action->data.physicalbounce.moveType = moveType;
	action->data.physicalbounce.massType = massType;
	action->data.physicalbounce.massEventActor = massEventActor;
	action->data.physicalbounce.massCollideActor = massCollideActor;
	action->data.physicalbounce.eventVelocityMultiplier = eventVelocityMultiplier;
	action->data.physicalbounce.collideVelocityMultiplier = collideVelocityMultiplier;

	return action;
}

stAction * Action::SetDirectionPath(const gedString& actionActor, int direction)
{
	stAction *action = newAction(actionActor, SET_PATH_POSITION);

	action->data.pathPosition.dataType = SET_DIRECTION;
	action->data.pathPosition.data.direction = direction;
	
	return action;
}

stAction * Action::SetFixedPathPosition(const gedString& actionActor, int frame)
{
	stAction *action = newAction(actionActor, SET_PATH_POSITION);

	action->data.pathPosition.dataType = FIXED_VALUE;
	action->data.pathPosition.data.fixed.frame = frame;	

	return action;
}


stAction * Action::SetEnvelopePathPosition(const gedString& actionActor, Envelope *env)
{
	stAction *action = newAction(actionActor, SET_PATH_POSITION);

	action->data.pathPosition.dataType = ENVELOPE;
	action->data.pathPosition.data.envelope.env = env;	

	return action;
}

stAction * Action::SetFixedAnimationFrame(const gedString& actionActor, int frame)
{
	stAction *action = newAction(actionActor, SET_ANIMATION_FRAME);

	action->data.animationFrame.dataType = FIXED_VALUE;
	action->data.animationFrame.data.fixed.frame = frame;
	
	return action;
}


stAction * Action::SetEnvelopeAnimationFrame(const gedString& actionActor, Envelope *env)
{
	stAction *action = newAction(actionActor, SET_ANIMATION_FRAME);

	action->data.animationFrame.dataType = ENVELOPE;
	action->data.animationFrame.data.envelope.env = env;
	
	return action;
}

stAction * Action::SetFixedColor(const gedString& actionActor, int red, int green, int blue)
{
	stAction *action = newAction(actionActor, SET_COLOR);

	action->data.color.dataType = FIXED_VALUE;
	action->data.color.data.fixed.red = red;	
	action->data.color.data.fixed.green = green;	
	action->data.color.data.fixed.blue = blue;	

	return action;
}


stAction * Action::SetEnvelopeColor(const gedString& actionActor, Envelope *env)
{
	stAction *action = newAction(actionActor, SET_COLOR);

	action->data.color.dataType = ENVELOPE;
	action->data.color.data.envelope.env = env;	

	return action;
}

stAction * Action::SetCursor(const gedString& actionActor, const gedString& imgName, int nFramesH, int nFramesV, int hotSpotX, int hotSpotY)
{
	stAction *action = newAction(actionActor, SET_CURSOR);

	gedString aux(imgName);
	strcpy(action->data.cursor.imgName, aux.c_str());		
	action->data.cursor.nFramesH = nFramesH;
	action->data.cursor.nFramesV = nFramesV;
	action->data.cursor.hotSpotX = hotSpotX;
	action->data.cursor.hotSpotY = hotSpotY;

	return action;
}

stAction * Action::SetCreateActor(const gedString& actorName, const gedString& animationName, Actor *parent, Path *path, int xini, int yini, bool bAbsolutePosition)
{
	stAction *action = newAction("", SET_CREATE_ACTOR);
	
	gedString aux1(actorName);
	gedString aux2(animationName);

	strcpy(action->data.createActor.name, aux1.c_str());	
	strcpy(action->data.createActor.animationName, aux2.c_str());
	action->bAbsolutePosition = bAbsolutePosition;
	action->x = xini;
	action->y = yini;
	
	if(parent)
	{
		if(parent == EVENT_ACTOR)			strcpy(action->data.createActor.parentName,  S_EVENT_ACTOR);
		else if(parent == COLLIDE_ACTOR)	strcpy(action->data.createActor.parentName,  S_COLLIDE_ACTOR); 
		else if(parent == PARENT_ACTOR)		strcpy(action->data.createActor.parentName, S_PARENT_ACTOR);
		else if(parent == CREATOR_ACTOR)	strcpy(action->data.createActor.parentName, S_CREATOR_ACTOR);
		else strcpy(action->data.createActor.parentName,  parent->getCloneName()); //cloneName, solve the issue 119
	}
	else 
	{
		strcpy(action->data.createActor.parentName,  NO_PARENT);
	}
	
	strcpy(action->data.createActor.pathName, path?path->getName().getCharBuf():NO_PATH);

	return action;
}

stAction * Action::SetActivationEvent(const gedString& actorFromCloneName, const gedString& actorToCloneName)
{
	stAction *action = newAction("", SET_ACTIVATION_EVENT);

	gedString aux1(actorFromCloneName);
	gedString aux2(actorToCloneName);

	strcpy(action->name, aux1.c_str());
	strcpy(action->actorName, aux2.c_str());
	action->executionType = IMMEDIATE_EXECUTION;

	return action;
}

stAction * Action::SetDestroyActor(const gedString& actorName)
{
	stAction *action = newAction("", SET_DESTROY_ACTOR);
	
	gedString aux(actorName);
	strcpy(action->data.name, aux.c_str());

	return action;
}

stAction * Action::SetExpression(Script *pScript)
{
	if(pScript)
	{
		stAction *action = newAction("", SET_EXPRESSION);
		
		strcpy(action->data.name, GameControl::Get()->AddExpression(pScript).c_str());
		
		return action;
	}
	
	return NULL;
}

stAction * Action::SetSetText(const gedString& actionActor, const gedString& text, const gedString& fontPath,  int iniASCIIChar, int nChars)
{
	stAction *action = newAction(actionActor, SET_SETTEXT);

	gedString aux1(text);
	gedString aux2(fontPath);

	strcpy(action->data.settext.text, aux1.c_str());
	strcpy(action->data.settext.fontPath, aux2.c_str());
	action->data.settext.iniASCIIChar = iniASCIIChar;
	action->data.settext.nChars = nChars;

	return action;
}

stAction *Action::SetConditionalAction(Script *pScript)
{
	stAction *action = newAction("", SET_CONDITIONAL_ACTION);
	bConditionalAction = true; //First conditional test is not a conditional action
	
	strcpy(action->data.conditionalaction.expression1, GameControl::Get()->AddExpression(pScript).c_str());
	//strcpy(action->data.conditionalaction.expression2, GameControl::Get()->AddExpression(expression2, listExpressions2).c_str());
	//action->data.conditionalaction.condition = condition;

	return action;
}

stAction * Action::SetSetZDepth(const gedString& actionActor, double depth)
{
	stAction *action = newAction(actionActor, SET_ZDEPTH);

	action->data.zdepth = (Uint32)(depth*MAX_ACTOR_DEPTH);

	return action;
}

stAction * Action::SetSound(bool bMusic, const gedString& path, int volume, int loop, int priority, Sint16 pan)
{
	stAction *action = newAction("", SET_PLAY_SOUND);

	gedString aux(path);
	strcpy(action->data.sound.path, aux.c_str());
	action->data.sound.bMusic = bMusic;
	action->data.sound.volume = volume;
	action->data.sound.loop   = loop;
	action->data.sound.priority   = priority;
	action->data.sound.pan   = pan;

	return action;
}

stAction * Action::SetEventControl(const gedString& actionActor, Uint32 events, bool bEnable)
{
	stAction *action = newAction(actionActor, SET_EVENT_CONTROL);

	action->data.eventcontrol.events = events;
	action->data.eventcontrol.bEnable = bEnable;

	return action;
}

stAction * Action::SetToAnteriorPosition(const gedString& actionActor)
{
	stAction *action = newAction(actionActor, SET_TO_ANTERIOR_POSITION);

	return action;
}

stAction * Action::SetMoveTo(const gedString& actionActor, double x, double y, double velocity, 
							 char *relativeActor, char *obstacleActor, int moveType)
{
	stAction *action = newAction(actionActor, SET_MOVE_TO);

	action->data.moveto.x = x;
	action->data.moveto.y = y;
	action->data.moveto.velocity = velocity;
	strcpy(action->data.moveto.relative, relativeActor);
	if(obstacleActor) strcpy(action->data.moveto.obstacle, obstacleActor);
	action->data.moveto.moveType = moveType;

	return action;
}

void Action::CleanUpUnusedTimers(MapTimerNameDefinition &timerDefinition)
{
	MapString usedTimers;

	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		MapString timers;
		(*it.Value())->CleanUpUnusedTimers(usedTimers);
	}

	//Removed unused timer definitions
	int nTry = 10;
	while(timerDefinition.size() > usedTimers.size() && nTry--)
	{
		MapTimerNameDefinitionIterator it1(timerDefinition);
		for(it1.Begin(); !it1.Done(); it1.Next())
		{
			if(!usedTimers.FindString(it1.Key()->c_str()))
			{
				timerDefinition.Remove(*it1.Key());
				it1.Begin();
			}
		}
	}
}

void Action::CleanUpUnusedTimers(MapString& usedTimers)
{
	stAction *action = listActions;
	while(action)
	{
		switch(action->actionType)
		{
		case SET_CREATE_TIMER:			
		case SET_DESTROY_TIMER:
			usedTimers.Add(action->data.timer.name, 1);
			break;
		case SET_EXPRESSION:
			{
				//Process timers in script
				Script *pScript = GameControl::Get()->GetExpression(action->data.name);
				if(pScript)
				{
					ListString list = pScript->GetTimerList();

					for(int i = 0; i < list.Count(); i++)
					{
						usedTimers.Add(list[i], 1);
					}
				}
			}
			break;

		}

		if(action->eventData.eventType == EVENT_TIMER)
		{
			usedTimers.Add(action->eventData.data.name, 1);
		}

		action = action->next;
	}
}

ListActions *Action::FindAction(stAction *actionFind)
{
	if(!actionFind) return NULL;
	

	//Remove map and list events for this action

	//mapKeyDown
	{MapKeysIterator it(mapKeyDown);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}


	//mapKeyUp
	{MapKeysIterator it(mapKeyUp);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}


	//mapCollision
	{MapStringActionIterator it(mapCollision);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	//mapCollisionFinish
	{MapStringActionIterator it(mapCollisionFinish);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	//mapActivationEvent
	{MapStringActionIterator it(mapActivationEvent);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	//mapAnimationFinishActions
	{MapStringActionIterator it(mapAnimationFinishActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	//mapPathFinishActions
	{MapStringActionIterator it(mapPathFinishActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}


	//mapTimerNameAction
	{MapStringActionIterator it(mapTimerNameAction);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	//mapMouseButtonDownActions
	{MapMouseButtonsIterator it(mapMouseButtonDownActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}


	//mapMouseUpActions
	{MapMouseButtonsIterator it(mapMouseUpActions);	
	for(it.Begin(); !it.Done(); it.Next())
	{
		if(it.Value()->Find(actionFind)) return it.Value();
	}}

	if(listAnimationActions.Find(actionFind)) return &listAnimationActions;
	if(listCreateActions.Find(actionFind)) return &listCreateActions;
	if(listDestroyActorActions.Find(actionFind)) return &listDestroyActorActions;
	if(listOutOfVisionActions.Find(actionFind)) return &listOutOfVisionActions;
	if(listMouseEnterActions.Find(actionFind)) return &listMouseEnterActions;
	if(listMouseLeaveActions.Find(actionFind)) return &listMouseLeaveActions;
	if(listMoveFinishActions.Find(actionFind)) return &listMoveFinishActions;


	return NULL;
}

void Action::UpdateEvent()
{
	//Update only if in edit mode
	ListActions *pList = FindAction(editAction);
	if(pList)
	{
		//editAction->next = NULL;
		setEditAction(newAction(editAction->actorName, editAction->actionType, pList));
	}
}

bool Action::GiveInheritedEvents(gedString parentActorName)
{
	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if((*it.Value())->InheritedEvents == parentActorName)
		{
			return true;
		}
	}

	return false;
}

void Action::RemoveInheritedEvents(gedString parentActorName)
{
	GlobalMapActionsIterator it(globalMapActions);
	for(it.Begin(); !it.Done(); it.Next())
	{
		if((*it.Value())->InheritedEvents == parentActorName)
		{
			(*it.Value())->setInheritedEvents("");
		}
	}
}

Uint32 Action::GetEventBits(bool bGetInheritedEvents)
{
	Uint32 events = 0;

	if(HaveActivationEvent(bGetInheritedEvents))	events |= EVENTACTIVATIONEVENT;	
	if(HaveAnimationFinish(bGetInheritedEvents))	events |= EVENTANIMATIONFINISH;	
	if(HaveCollisions(bGetInheritedEvents))			events |= EVENTCOLLISION;
	if(HaveCollisionFinish(bGetInheritedEvents))	events |= EVENTCOLLISIONFINISH;	
	if(HaveCreate(bGetInheritedEvents))				events |= EVENTCREATE;	
	if(HaveDestroyActor(bGetInheritedEvents))		events |= EVENTDESTROYACTOR;
	if(HaveAnimation(bGetInheritedEvents))			events |= EVENTANIMATION;	
	if(HaveKeyDown(bGetInheritedEvents))			events |= EVENTKEYDOWN;
	if(HaveKeyUp(bGetInheritedEvents))				events |= EVENTKEYUP;	
	if(HaveMouseButtonDown(bGetInheritedEvents))	events |= EVENTMOUSEBUTTONDOWN;
	if(HaveMouseUpActions(bGetInheritedEvents))		events |= EVENTMOUSEBUTTONUP;
	if(HaveMouseEnter(bGetInheritedEvents))			events |= EVENTMOUSEENTER;
	if(HaveMouseLeave(bGetInheritedEvents))			events |= EVENTMOUSELEAVE;	
	if(HaveMoveFinish(bGetInheritedEvents))			events |= EVENTMOVEFINISH;		
	if(HaveOutOfVision(bGetInheritedEvents))		events |= EVENTOUTOFVISION;			
	if(HavePathFinish(bGetInheritedEvents))			events |= EVENTPATHFINISH;	
	if(HaveTimer(bGetInheritedEvents))				events |= EVENTTIMER;

	return events;
}

#endif //#ifndef STAND_ALONE_GAME


