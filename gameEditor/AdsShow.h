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


// AdsShow.h: interface for the AdsShow class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)

#if !defined(AFX_ADSSHOW_H__85F9DA01_DBC8_4C39_A8C1_CFE4FB577541__INCLUDED_)
#define AFX_ADSSHOW_H__85F9DA01_DBC8_4C39_A8C1_CFE4FB577541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "AdsControl.h"

class AdsShow : public Actor  
{
public:
	Uint32 OnTimer(SDL_TimerID timerID);
	bool RefreshBanner();
	AdsShow(Actor *parent, int x, int y);
	virtual ~AdsShow();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	const stCursor *OnGetCursor();
	const gedString *OnActorTip();

private:
	Banner banner;
	int xBanner, yBanner, bannerCount;

	gedString			actorTip;
};

#endif // !defined(AFX_ADSSHOW_H__85F9DA01_DBC8_4C39_A8C1_CFE4FB577541__INCLUDED_)

#endif //STAND_ALONE_GAME