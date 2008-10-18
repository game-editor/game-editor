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