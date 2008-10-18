// VariableEditor.h: interface for the VariableEditor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_VARIABLEEDITOR_H__F78AB399_B105_4EC2_9BFD_D35A5A9DE4E0__INCLUDED_)
#define AFX_VARIABLEEDITOR_H__F78AB399_B105_4EC2_9BFD_D35A5A9DE4E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class ExpressionEditor;
class ScriptGlobals;

class VariableEditor : public Panel  
{
public:
	VariableEditor(ExpressionEditor *_editor, ScriptGlobals *_globals = NULL);
	virtual ~VariableEditor();

	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);

	void UpdateVars(const gedString item = "");

private:
	ListPop *listVars;
	ExpressionEditor *editor;
	ScriptGlobals *globals;

	
};

#endif // !defined(AFX_VARIABLEEDITOR_H__F78AB399_B105_4EC2_9BFD_D35A5A9DE4E0__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME