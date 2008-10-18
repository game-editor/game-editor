// ActorTip.h: interface for the ActorTip class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_)
#define AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class ActorTip : public Actor  
{
public:
	static void Release();
	static void Create(gedString *tip);
	static void Create(Actor *actor);
	static void ResetLast() {lastActor = NULL;}
	ActorTip(const gedString *tip);
	virtual ~ActorTip();

private:
	static ActorTip *actorTip;
	static Actor *lastActor;
};

#endif // !defined(AFX_ACTORTIP_H__4B8EF10D_3478_463F_BD87_A4E290864054__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME