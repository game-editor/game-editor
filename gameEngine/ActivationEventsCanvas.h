// ActivationEventsCanvas.h: interface for the ActivationEventsCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVATIONEVENTSCANVAS_H__4288C212_663F_4A8C_9915_676C07DBDAA4__INCLUDED_)
#define AFX_ACTIVATIONEVENTSCANVAS_H__4288C212_663F_4A8C_9915_676C07DBDAA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"



class stLine
{
public:
	stLine() {action = NULL;}
	stLine(const gedString& _actorFrom, const gedString& _actorTo, stAction *_action) {actorFrom = _actorFrom; actorTo = _actorTo; action = _action;}
	void operator=( const stLine& copy ) 
	{ 
		if( &copy != this )
		{
			actorFrom = copy.actorFrom;
			actorTo = copy.actorTo;
			action = copy.action;
		}
	}

	bool operator==( const stLine& copy ) 
	{ 
		if( actorFrom == copy.actorFrom &&
			actorTo == copy.actorTo)
			return true;

		return false;
	}

	gedString actorFrom, actorTo;
	stAction *action;
};

typedef GlSList<stLine> ListEventLine;  
typedef GlSListIterator<stLine> ListEventLineIterator;

class ActivationEventsCanvas : public Actor  
{
public:

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	//Overload the memory alocators to avoid crash when delete the class derived from Actor
	void *operator new(size_t size)	{return dlmalloc(size);}
	void operator delete(void * mem) {dlfree(mem);}
#endif

#ifndef STAND_ALONE_GAME
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	static void ActorRemoved(const gedString& cloneName);
#endif //#ifndef STAND_ALONE_GAME

	static bool DoLoop(gedString actorFrom, gedString actorTo, gedString *begin = NULL);
	static void ToggleVisibility();
	const gedString *OnActorTip();
	static void Clear();
	static ActivationEventsCanvas *Get() {return activationEventsCanvas;}
	
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	const stCursor *OnGetCursor();
	bool OnSlowCanvasHitTest(int x, int y);
	stAction * HitLine(int x, int y);
	static bool HasActivationEventLines(const gedString& actorCloneName) {return activationEventsCanvas->getImage()->IsVisible() && (mapActorInLines[actorCloneName] != NULL);}

#ifndef STAND_ALONE_GAME
	static void AddLine(stAction *newAction, bool bLoad = false);
	static void NotifyMouseCoord(int x, int y);
#endif

	ActivationEventsCanvas();
	virtual ~ActivationEventsCanvas();

	void OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ);

private:

#ifndef STAND_ALONE_GAME
	static void RemoveLine(const stLine& line);
#endif

	static bool bShowLines;
	static ActivationEventsCanvas *activationEventsCanvas;

	//Interactive activation lines
	void DrawActivationEventsLines(KrPaintInfo *paintInfo, const KrRect &clip);
	static int x1, y1, x2, y2;
	static int x1ant, y1ant, x2ant, y2ant;
	KrRGBA color1, color2;

	//Actor lines
	static ListEventLine listEventLines;
	static MapString mapActorInLines;
	stAction *hitAction, *hitTip;
	gedString			actorTip;

#ifndef STAND_ALONE_GAME
	ListPop *list;
#endif //#ifndef STAND_ALONE_GAME
};

#endif // !defined(AFX_ACTIVATIONEVENTSCANVAS_H__4288C212_663F_4A8C_9915_676C07DBDAA4__INCLUDED_)
