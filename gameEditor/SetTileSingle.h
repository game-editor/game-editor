// SetTileSingle.h: interface for the SetTileSingle class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_SETTILESINGLE_H__E3E3E324_3A80_4B48_BE7E_A200928308B2__INCLUDED_)
#define AFX_SETTILESINGLE_H__E3E3E324_3A80_4B48_BE7E_A200928308B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class SetTileSingle : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	static void Call(Actor *actor);
	SetTileSingle(Actor *actor);
	virtual ~SetTileSingle();
	

private:
	Actor *actor;
	static SetTileSingle *setTileDialog;
	ListPop *listAnimation, *listRandom;
};

#endif // !defined(AFX_SETTILESINGLE_H__E3E3E324_3A80_4B48_BE7E_A200928308B2__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME