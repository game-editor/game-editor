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


// DlgPocketPCKey.h: interface for the DlgPocketPCKey class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_DLGPOCKETPCKEY_H__0B40C010_00E8_43E1_8A02_054276662471__INCLUDED_)
#define AFX_DLGPOCKETPCKEY_H__0B40C010_00E8_43E1_8A02_054276662471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class DlgPocketPCKey : public Panel  
{
public:
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnButton(Button *button, int buttonId);
	DlgPocketPCKey(SDLKey _keyFrom);
	virtual ~DlgPocketPCKey();

private:
	Text *textKey;
	SDLKey key, keyFrom;
};

#endif // !defined(AFX_DLGPOCKETPCKEY_H__0B40C010_00E8_43E1_8A02_054276662471__INCLUDED_)

#endif //#ifndef STAND_ALONE_GAME
