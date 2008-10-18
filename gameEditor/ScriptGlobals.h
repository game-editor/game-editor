// ScriptGlobals.h: interface for the ScriptGlobals class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_SCRIPTGLOBALS_H__05BAAC91_39DE_4966_B0D2_04CE494F46B4__INCLUDED_)
#define AFX_SCRIPTGLOBALS_H__05BAAC91_39DE_4966_B0D2_04CE494F46B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class ScriptGlobals : public Panel  
{
public:	
	void SetText(char *str);
	gedString GetText();
	gedString GetTextUntilCursor();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	ScriptGlobals();
	virtual ~ScriptGlobals();
	void Insert(char *str);
	void UpdateVars();

	static ScriptGlobals *getScriptGlobals() {return scriptGlobals;}

private:
	EditBox *editGlobal, *name;
	ListPop *listChooseGlobal, *listFile;
	ListPop *listAutoComplete, *listActor;
	Text *textLine;

	gedString originalScript;
	int functionsIndex;
	bool bDelimit;

	static ScriptGlobals *scriptGlobals;
};

#endif // !defined(AFX_SCRIPTGLOBALS_H__05BAAC91_39DE_4966_B0D2_04CE494F46B4__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME