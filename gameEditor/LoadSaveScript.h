// LoadSaveScript.h: interface for the LoadSaveScript class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_LOADSAVESCRIPT_H__07A1603A_B50C_4177_8F39_7C816040A081__INCLUDED_)
#define AFX_LOADSAVESCRIPT_H__07A1603A_B50C_4177_8F39_7C816040A081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadSave.h"
#include "ExpressionEditor.h"
#include "ScriptGlobals.h"
#include "ActorText.h"

class LoadSaveScript : public LoadSave  
{
public:
	void OnButton(Button *button, int buttonId);

	LoadSaveScript(int command, ExpressionEditor *scriptEditor, ScriptGlobals *globalsEditor = NULL, ActorText *actorText = NULL);
	virtual ~LoadSaveScript();
	static void ResetPath();

private:
	ExpressionEditor *scriptEditor;
	ScriptGlobals *globalsEditor;
	ActorText *actorText;
};

#endif // !defined(AFX_LOADSAVESCRIPT_H__07A1603A_B50C_4177_8F39_7C816040A081__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME