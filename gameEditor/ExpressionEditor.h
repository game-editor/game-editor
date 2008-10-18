// ExpressionEditor.h: interface for the ExpressionEditor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_EXPRESSIONEDITOR_H__BCA7B27C_BFC5_4D0F_AC4B_1482FE14021A__INCLUDED_)
#define AFX_EXPRESSIONEDITOR_H__BCA7B27C_BFC5_4D0F_AC4B_1482FE14021A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseAction.h"
#include "Action.h"

bool IsEditingScript();
void InsertIntoScript(char *str);
gedString GetEditingScriptTextUntilCursor();
int GetNewVariableInsertionPosition();

class Script;
class ExpressionEditor : public BaseAction  
{
public:
	void Insert(char *str);
	void SetText(char *str);
	static ExpressionEditor *getExpressionEditor() {return expressionEditor;}
	void UpdateVars();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	ExpressionEditor(Actor *actor, bool bOnCollision);
	virtual ~ExpressionEditor();
	void UpdateEdition();

	gedString GetText();
	gedString GetTextUntilCursor();

	Script *getScript() {return pScript;}
	Actor *getEventActor() {return eventActor;}

private:
	stAction *editAction;
	EditBox *editExp;
	ListPop *listAutoComplete, *listActor, *listFile;
	Actor *eventActor, *actionActor;
	Script *pScript;
	Text *textLine;
	bool bDelimit, bOnCollision;
	int functionsIndex;
	static ExpressionEditor *expressionEditor;

	gedString originalScript;
};

#endif // !defined(AFX_EXPRESSIONEDITOR_H__BCA7B27C_BFC5_4D0F_AC4B_1482FE14021A__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME