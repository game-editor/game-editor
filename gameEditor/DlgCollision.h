// DlgCollision.h: interface for the DlgCollision class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGCOLLISION_H__A3A743F0_F950_4414_8103_35E520729A63__INCLUDED_)
#define AFX_DLGCOLLISION_H__A3A743F0_F950_4414_8103_35E520729A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgCollision : public Panel  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	DlgCollision(Actor *actor, bool bCollision);
	virtual ~DlgCollision();

private:
	ListPop *listActor, *listType, *listSide;
	Actor *eventActor, *actionActor;
	bool bCollision;
	int side;

};

#endif // !defined(AFX_DLGCOLLISION_H__A3A743F0_F950_4414_8103_35E520729A63__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME