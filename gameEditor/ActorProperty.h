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