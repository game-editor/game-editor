// EditSequenceDialog.h: interface for the EditSequenceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_)
#define AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "Sequence.h"

class EditSequenceDialog : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);
	EditSequenceDialog(Actor *actor, Sequence *seq);
	virtual ~EditSequenceDialog();

private:
	EditBox *frames, *editFps;
	Sequence *sequence;
	Actor *actor;
};

#endif // !defined(AFX_EDITSEQUENCEDIALOG_H__2A2E2233_3578_4C69_BCFE_DD50F1E7ED56__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME