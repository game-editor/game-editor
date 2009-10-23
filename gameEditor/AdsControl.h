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


// AdsControl.h: interface for the AdsControl class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)

#if !defined(AFX_ADSControl_H__727EBA2C_1D10_4E79_ABEA_4ACFDDB83565__INCLUDED_)
#define AFX_ADSControl_H__727EBA2C_1D10_4E79_ABEA_4ACFDDB83565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gedString.h"
#include "SDL_thread.h"



class Banner
{
public:
	bool SetBanner(gedString bannerFile);
	Banner();
	virtual ~Banner();

	gedString getImageFile() {return imageFile;}
	gedString getUrl() {return url;}
	gedString getAltText() {return altText;}
	
	void operator=( const Banner& copy ) 
	{ 
		imageFile = copy.imageFile;
		url = copy.url;
		altText = copy.altText;
	}

private:
	

	gedString imageFile; //Full image path
	gedString url; //Full url with http://...
	gedString altText;	
};

class AdsControl  
{
public:
	void RefreshBanners() {bForceRefresh = true;}; //Force connection
	
	SDL_Thread * GetThread();
	Banner GetBanner();
	Banner GetNewestBanner();
	static AdsControl * Get();
	bool HaveBanner();
	bool getBannerFromNet() {return bGetBannerFromNet;}
		
	virtual ~AdsControl();

private:
	long CountBanners(long *iMin = NULL, long *iMax = NULL);
	void SetCurrentBanner();
	AdsControl();
	bool GetBannerFromNet();
	bool SaveBanner(gedString imageFile, gedString url, gedString altText);

	static AdsControl *adsControl;
	bool haveBanner;
	Banner currentBanner, newestBanner;
	int iCurrentBanner;

	static int CheckForBannerThread( void *pParam );
	SDL_Thread *thread;

	bool bGetBannerFromNet;
	static long timeCount;
	static bool bIsConnected, bForceRefresh;
};

#endif // !defined(AFX_ADSControl_H__727EBA2C_1D10_4E79_ABEA_4ACFDDB83565__INCLUDED_)

#endif //STAND_ALONE_GAME
