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


// GameSettings.h: interface for the GameSettings class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_GAMESETTINGS_H__E1F0136F_1408_4BEE_8732_1D4E68C330E0__INCLUDED_)
#define AFX_GAMESETTINGS_H__E1F0136F_1408_4BEE_8732_1D4E68C330E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"
#include "ColorPicker.h"

class GameSettings : public Panel  
{
public:
	void SetPocketKeys();
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	GameSettings();
	virtual ~GameSettings();
	static GameSettings *GetGameSettings() {return gameSettings;};

private:
	EditBox *editFps, *editX, *editY, *editMaxSounds, *gameTitle, *editViewSafeMargin, *gameID, *gamePort;
	ListPop *listRes, *listFull, *listHz, *listStereo, *listMouse, 
			*listSuspend, *listPocketKeys, *listESC, *listMotion, *listGameServer, *listAutoStartNetwork;
	ListDirectory *iconPath;

	ColorSample *backgroundColor;

	static GameSettings *gameSettings;
	bool bFlipPocketPCScreen;
};

#endif // !defined(AFX_GAMESETTINGS_H__E1F0136F_1408_4BEE_8732_1D4E68C330E0__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME