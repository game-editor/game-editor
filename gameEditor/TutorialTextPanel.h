/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// TutorialTextPanel.h: interface for the TutorialTextPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_)
#define AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "GameControl.h"



class TutorialTextPanel : public Actor  
{
public:
	void Save();
	static void DestroyAll();
	static int Count();
	static void Hide();
	static void Show();
	TutorialTextPanel(const gedString& _explanationText);
	virtual ~TutorialTextPanel();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	const stCursor *OnGetCursor();
	void OnButton(Button *button, int buttonId);
	

private:
	gedString explanationText;	

	static MapPointer mapTextPanel;
	static long depth;
};

#endif // !defined(AFX_TUTORIALTEXTPANEL_H__EF1DADB6_04D4_4911_8536_19FC4B87DB68__INCLUDED_)

#endif //STAND_ALONE_GAME
