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


// AdsShow.cpp: implementation of the AdsShow class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)

#include "AdsShow.h"
#include "GameControl.h"

#define REFRESH_TIME 120*60*1000 //120 minutes

AdsShow::AdsShow(Actor *parent, int x, int y)
	: Actor("gedAds", parent)
{
	xBanner = x;
	yBanner = y;

	bannerCount = 0;

	if(RefreshBanner())
	{
		SetTimer(REFRESH_TIME);
	}
}

AdsShow::~AdsShow()
{

}

bool AdsShow::RefreshBanner()
{
	if(AdsControl::Get()->HaveBanner())
	{
		EditorDirectory editDir;
		chdir("ads");

		if(isInternetConnected())
		{
			AdsControl::Get()->RefreshBanners();
			banner = AdsControl::Get()->GetNewestBanner();
		}
		else
		{
			banner = AdsControl::Get()->GetBanner();
		}
		
		
		char s[128];
		sprintf(s, "gedBanner%ld", bannerCount++);
		if(AddAnimation(s, banner.getImageFile(), 1, 1, -1, -1, 0, false))		
		{
			SetPos(xBanner, yBanner);
			setEditAnim(true);

			return true;
		}
	}

	return false;
}

Uint32 AdsShow::OnTimer(SDL_TimerID timerID)
{
	if(RefreshBanner())
	{
		char s[128];
		sprintf(s, "gedBanner%ld", bannerCount-1);
		RemoveAnimation(s);
	}

	return REFRESH_TIME;
}

bool AdsShow::OnMouseButtonDown(int x, int y, Uint8 button)
{
	openUrl(banner.getUrl().c_str());

	return false;
}

const stCursor *AdsShow::OnGetCursor()
{
	cursor.imgName	= "makscursorMao.bmp";
	cursor.nFramesH	= 1;
	cursor.nFramesV	= 1;
	cursor.hotSpotX = 7;
	cursor.hotSpotY = 0;
	cursor.bAnimate = false;
	
	return &cursor;
}

const gedString *AdsShow::OnActorTip()
{
	actorTip = banner.getAltText();
	return &actorTip;
}

#endif //STAND_ALONE_GAME


