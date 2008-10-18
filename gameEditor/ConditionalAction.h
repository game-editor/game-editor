// ConditionalAction.h: interface for the ConditionalAction class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_CONDITIONALACTION_H__88758561_1A68_4D61_AB82_DA331CDA6670__INCLUDED_)
#define AFX_CONDITIONALACTION_H__88758561_1A68_4D61_AB82_DA331CDA6670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"
#include "Action.h"

class Script;
class ConditionalAction;

typedef GlSList<ConditionalAction *> ListConditionalActionDlg;  
typedef GlSListIterator<ConditionalAction *> ListConditionalActionDlgIterator;

class ConditionalAction : public BaseAction  
{
public:
	void UpdateEdition();
	void Disable();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	ConditionalAction(Actor *actor, bool bOnCollision);
	virtual ~ConditionalAction();

	static bool HasConditionalDialog() {return listConditionalActionDlg.size() > 0;}

private:
	EditBox *editExp1, *editExp2;
	ListPop *listAutoComplete1, *listActor1;
	ListPop *listAutoComplete2, *listActor2;
	ListPop *listCondition, *listActions;
	Actor *eventActor;
	Script *pScript;
	bool bDelimit1, bDelimit2;
	int userVarsIndex;
	
	static stAction *firstAction;
	static ListConditionalActionDlg listConditionalActionDlg;
	stAction *editAction;
	bool bConditionAdd;
};

#endif // !defined(AFX_CONDITIONALACTION_H__88758561_1A68_4D61_AB82_DA331CDA6670__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME