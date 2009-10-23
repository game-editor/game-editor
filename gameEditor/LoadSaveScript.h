/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


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