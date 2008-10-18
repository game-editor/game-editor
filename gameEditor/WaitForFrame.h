// WaitForFrame.h: interface for the WaitForFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_WAITFORFRAME_H__CE921131_301F_4B9F_AE0A_0C79202653F7__INCLUDED_)
#define AFX_WAITFORFRAME_H__CE921131_301F_4B9F_AE0A_0C79202653F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "BaseAction.h"

class stAction;
class WaitForFrame : public Panel  
{
public:
	void UpdateEdition();
	void OnButton(Button *button, int buttonId);
	WaitForFrame(Actor *actor, stAction *action, BaseAction *_actionPanel);
	virtual ~WaitForFrame();	

private:
	EditBox *sequence;
	stAction *action;
	ListPop *listAnimation, *listLastAction;
	Actor *eventActor;
	BaseAction *actionPanel;
};

#endif // !defined(AFX_WAITFORFRAME_H__CE921131_301F_4B9F_AE0A_0C79202653F7__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME