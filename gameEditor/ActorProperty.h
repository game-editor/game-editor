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


// ActorProperty.h: interface for the ActorProperty class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ACTORPROPERTY_H__6D61B748_CA75_415F_9553_BD6F4B22AF02__INCLUDED_)
#define AFX_ACTORPROPERTY_H__6D61B748_CA75_415F_9553_BD6F4B22AF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "GameControl.h"

class ActorEdit;

/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
class AdsShow;
#endif*/



class ActorProperty : public Panel  
{
public:
	static Actor *GetCurrentEventActor();
	static void Update(Actor *actor);

	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	static void SetEditEvents();
	static void Update();
	static void Destroy();
	void RemoveActor(Actor* act);
	void SetParent();
	static void SetPaths();
	void AddAction(const gedString &action);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnSlide(Slide *slide, int slideId, double pos);
	void OnButton(Button *button, int buttonId);
	
	static void Call(Actor *actor, bool bCreate = true);
	ActorProperty();
	virtual ~ActorProperty();

	static ActorProperty *getActorProperty() {return actorProperty;}

	Actor *getCurrentActor() {return actor;}

	static bool GiveInheritedEvents(Actor *testActor);

private:
	
	void SetActor(Actor *actor);

	static ActorProperty *actorProperty;
	ListPop *listAction, *listPath, *listParent, *listAnim, *listCreate, *listNetActor, *listNetOptimize, *listInfinite;
	ListPop *listEditEvents, *listEditActions, *listRemoveEvents, *listActor;
	ListPop *listOutOfVisionOptimization, *listInheritEventsFrom;
	Actor *actor;
	Slide *slideTrans, *slideZDepth;
	bool bRemove;

/*#if defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)
	AdsShow *adsShow;
#endif*/
};


class CloneArray : public Panel  
{
public:
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	void UpdatePreview();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	CloneArray(Actor *actor);
	virtual ~CloneArray();

private:
	void RemovePreviewClones();

	ListPop *listMode;
	EditBox *horizontalCount, *verticalCount, *horizontalDistance, *verticalDistance;	
	Actor *clonedActor;

	ArrayActor createdClones;
};

class ClonePath : public Panel  
{
public:
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	void UpdatePreview();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	ClonePath(Actor *actor);
	virtual ~ClonePath();

private:
	void RemovePreviewClones();

	ListPop *listPath;
	Path *actualPath;
	bool bControlPathShow;
	int xPathOri, yPathOri;

	EditBox *count;	
	Actor *clonedActor;

	ArrayActor createdClones;	
};

#endif // !defined(AFX_ACTORPROPERTY_H__6D61B748_CA75_415F_9553_BD6F4B22AF02__INCLUDED_)


#endif //STAND_ALONE_GAME
