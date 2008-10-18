// SetTile.h: interface for the SetTile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_SETTILE_H__0206885C_960E_423B_94B0_290B6CAEFFB3__INCLUDED_)
#define AFX_SETTILE_H__0206885C_960E_423B_94B0_290B6CAEFFB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class SetTile : public Panel  
{
public:
	void AddSample();
	void OnButton(Button *button, int buttonId);
	static void Call(Actor *actor);
	SetTile(Actor *actor);
	virtual ~SetTile();
	

private:
	Actor *actor, *tileSample;
	static SetTile *setTileDialog;
	ListPop *listAnimation;
};

#endif // !defined(AFX_SETTILE_H__0206885C_960E_423B_94B0_290B6CAEFFB3__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME
