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
