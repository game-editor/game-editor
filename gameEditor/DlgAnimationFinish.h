// DlgAnimationFinish.h: interface for the DlgAnimationFinish class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGANIMATIONFINISH_H__C659B8F7_26EC_46E2_A523_022832961187__INCLUDED_)
#define AFX_DLGANIMATIONFINISH_H__C659B8F7_26EC_46E2_A523_022832961187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgAnimationFinish : public Panel  
{
public:
	void UpdateEdition();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	DlgAnimationFinish(Actor *actor);
	virtual ~DlgAnimationFinish();

private:
	ListPop *listAnimation;
	Actor *eventActor, *actionActor;
};

class DlgPathFinish : public Panel  
{
public:
	void UpdateEdition();
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnButton(Button *button, int buttonId);
	DlgPathFinish(Actor *actor);
	virtual ~DlgPathFinish();

private:
	ListPop *listPath;
	Actor *eventActor, *actionActor;
};

#endif // !defined(AFX_DLGANIMATIONFINISH_H__C659B8F7_26EC_46E2_A523_022832961187__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME