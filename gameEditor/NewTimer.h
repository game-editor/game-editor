// NewTimer.h: interface for the NewTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_NEWTIMER_H__ACC77F93_BB36_4032_996D_5EEA56E8842F__INCLUDED_)
#define AFX_NEWTIMER_H__ACC77F93_BB36_4032_996D_5EEA56E8842F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "CreateTimer.h"

class NewTimer : public Panel  
{
public:
	NewTimer(CreateTimer *parent);
	virtual ~NewTimer();

	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

private:
	EditBox *name, *time, *minRandTime, *count;
	Text *textTime;
	ListPop *listType, *listCountType;
	CreateTimer *parent;
};

#endif // !defined(AFX_NEWTIMER_H__ACC77F93_BB36_4032_996D_5EEA56E8842F__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME