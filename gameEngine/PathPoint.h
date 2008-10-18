// PathPoint.h: interface for the PathPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHPOINT_H__6792EAD5_9360_4498_AD96_3065891C4324__INCLUDED_)
#define AFX_PATHPOINT_H__6792EAD5_9360_4498_AD96_3065891C4324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class PathPoint : public Actor  
{
public:

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	//Overload the memory alocators to avoid crash when delete the class derived from Actor
	void *operator new(size_t size)	{return dlmalloc(size);}
	void operator delete(void * mem) {dlfree(mem);}
#endif

#ifndef STAND_ALONE_GAME
	bool OnList(ListPop *list, int index, gedString &text, int listId);
#endif //#ifndef STAND_ALONE_GAME

	void Remove();
	
	int TotalKeys();
	PathPoint *AddPoint(int x, int y, bool bEdit = true, Path *path = NULL, int iKey = 0);
	const stCursor * OnGetCursor();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseMove(int x, int y);
	PathPoint *getRootPath();
	PathPoint(int x, int y, PathPoint *prev = NULL, bool bEdit = true, Path *path = NULL, int iKey = 0);
	virtual ~PathPoint();

	PathPoint *getNext() {return next;};

	int getX() {return getImage()->X();};
	int getY() {return getImage()->Y();};
	
	bool getLinear() {return bLinear;}
	static int getNPaths() {return nPaths;};
	

private:
	PathPoint *prev, *next;
	bool bEdit;
	Path *path;
	int iKey;
	bool bLinear;
	static int nPaths;

#ifndef STAND_ALONE_GAME
	ListPop *list;
#endif //#ifndef STAND_ALONE_GAME
};

#endif // !defined(AFX_PATHPOINT_H__6792EAD5_9360_4498_AD96_3065891C4324__INCLUDED_)
