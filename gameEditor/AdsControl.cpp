// AdsControl.cpp: implementation of the AdsControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STAND_ALONE_GAME) && defined(WIN32) && !defined(GAME_EDITOR_PROFESSIONAL)

#include "AdsControl.h"
#include "GameControl.h"
#include <math.h>
#include<windows.h>
#include <time.h>

#include "Tutorial.h"
#include "../gameEngine/dlmalloc.h"


#define MAX_BANNERS 2


AdsControl *AdsControl::adsControl = NULL;
long AdsControl::timeCount = 1000;
bool AdsControl::bForceRefresh = true;
	

AdsControl::AdsControl()
{
	adsControl = this;

	haveBanner = false;
	bGetBannerFromNet = false;
	iCurrentBanner = -1;
	
	
	//Create ads directory in editor directory
	EditorDirectory editDir;
	ged_mkdir("ads");

	thread = SDL_CreateThread(CheckForBannerThread, NULL);	
}

AdsControl::~AdsControl()
{
	
}

int AdsControl::CheckForBannerThread( void *pParam )
{
	while(1)
	{
		//Exit if have new banners and game editor exits or windows exit
		
		MSG  msg;
		if((!GameControl::getGameEngineIsRunning() /*&& AdsControl::Get()->getBannerFromNet()*/) || //Don't stays running, after exit
			PeekMessage(&msg, NULL, WM_ENDSESSION, WM_ENDSESSION, PM_REMOVE))
		{
			break;
		}
		
		if(timeCount >= 30 && bForceRefresh) //Execute each 5 minutes
		{			
			if(isInternetConnected())
			{
				if(bForceRefresh)
				{
					//Try ten times
					for(int i = 0; i < 10; i++)
					{
						if(AdsControl::Get()->GetBannerFromNet())
						{
							break;
						}
						
						SDL_Delay(100);
					}

					bForceRefresh = false;
				}
			}
			
			AdsControl::Get()->SetCurrentBanner();
			
			//Exit if have new banners and game editor exits
			if(!GameControl::getGameEngineIsRunning() && AdsControl::Get()->getBannerFromNet())
			{
				break;
			}

			timeCount = 0;
		}

		//Sleep for 10s
		SDL_Delay(10*1000);

		timeCount++;
	}

    return 0;
}

Banner::Banner()
{
	altText = "Click here to visit our sponsor";
}

Banner::~Banner()
{

}

bool AdsControl::HaveBanner()
{
	if(!Tutorial::IsOff()) return false;

	return haveBanner;
}

AdsControl *AdsControl::Get()
{
	if(!adsControl)
	{
		new AdsControl();
	}

	return adsControl;
}

Banner AdsControl::GetBanner()
{
	return currentBanner;
}

Banner AdsControl::GetNewestBanner()
{
	return newestBanner;
}

SDL_Thread *AdsControl::GetThread()
{
	return thread;
}

bool AdsControl::GetBannerFromNet()
{
	//Get banner from Internet and store on disk


	//Read html page with image data
	char file[128];
	sprintf(file, "focusin.ads.targetnet.com/ad/id=makslane&opt=hhn&rw=468&rh=60&cv=210&uid=%ld", (time(NULL) + SDL_GetTicks()) % (long)floor(8640000.0 - (rand()/(double)RAND_MAX)*1000.0));

	long bytesRead = 0;
	char *buf = download(file, &bytesRead);
	if(!buf) return false;

	buf[bytesRead] = 0;

	//Find <a .. /a> TAG
	gedString htmlPage(buf);
	free(buf);

	int i1 = htmlPage.find("<a ");
	if(i1 > 0 && htmlPage.find("<html>") >= 0)
	{
		char delim = '\'';
		int i2 = htmlPage.find(delim, i1);
		if(i2 < 0) 
		{
			delim = '\"';
			i2 = htmlPage.find(delim, i1);
		}

		if(i2 < 0) return false;

		//Read href
		htmlPage = htmlPage.substr(i2+1);
		int i3 = htmlPage.find(delim);
		if(i3 < 0) return false;

		gedString url(htmlPage.substr(0, i3));

		//Read img
		i1 = htmlPage.find("<img ", i3);
		if(i1 < 0) return false;

		i2 = htmlPage.find(delim, i1);
		if(i2 < 0) return false;

		htmlPage = htmlPage.substr(i2+1);
		i3 = htmlPage.find(delim);
		if(i3 < 0) return false;

		gedString img(htmlPage.substr(0, i3));
		if(img.find("http://") == -1)
		{
			if(img[0] != '/') img = gedString("/") + img;
			img = gedString("focusin.ads.targetnet.com") + img;
		}
		
		//Read alt
		gedString alt;
		i1 = htmlPage.find("alt=", i3);
		if(i1 >= 0)
		{
			i2 = htmlPage.find(delim, i1);
			htmlPage = htmlPage.substr(i2+1);
			i3 = htmlPage.find(delim);

			alt = htmlPage.substr(0, i3);
		}

		return SaveBanner(img, url, alt);
	}	

	return false;
}

static int IMG_isSupportedImage(char *magic)
{	
	//Is GIF
	if ( (strncmp(magic, "GIF", 3) == 0) &&
		((memcmp(magic + 3, "87a", 3) == 0) ||
		 (memcmp(magic + 3, "89a", 3) == 0)) ) 
	{
		return true;
	}

	//Is JPEG
	if ( (magic[0] == 0xFF) && (magic[1] == 0xD8) ) 
	{
		if ( memcmp((char *)(magic + 6), "JFIF", 4) == 0 ||
			memcmp((char *)(magic + 6), "Exif", 4) == 0 ) 
		{
			return true;
		}
	}
	
	return false;
}


bool AdsControl::SaveBanner(gedString imageFile, gedString url, gedString altText)
{
	if(imageFile.find("http://") >= 0)
	{
		//Remove http://
		imageFile = imageFile.substr(7);
	}


	//Download image file
	long bytesRead = 0;
	char *image = download(imageFile.c_str(), &bytesRead);
	if(!image) return false;

	//Is gif or jpg?
	if(!IMG_isSupportedImage(image))
	{
		free(image);
		return false;
	}

	//Get min and max banner index
	char bannerFileName[128];
	long iMin = 1000000, iMax = 0;
	long nBanners = CountBanners(&iMin, &iMax);
	

	//Remove oldest banner
	if(nBanners >= MAX_BANNERS)
	{
		sprintf(bannerFileName, "bn%010ld", iMin);

		{
		EditorDirectory editDir;
		chdir("ads");
		remove(bannerFileName);
		}

		sprintf(bannerFileName, "_bn%010ld", iMin);

		{
		EditorDirectory editDir;
		chdir("ads");
		remove(bannerFileName);
		}
	}

	//Save new banner
	
	sprintf(bannerFileName, "bn%010ld", iMax + 1);
	SDL_RWops *arq;
	
	{
	EditorDirectory editDir;
	chdir("ads");
	arq= ged_SDL_RWFromFile(bannerFileName, "wb");
	}
	
	if(arq)
	{
		SDL_RWwrite(arq, image, bytesRead, 1);
		SDL_RWclose(arq);
		
		//Save banner data
		char s[128];
		sprintf(s, "_bn%010ld", iMax + 1);

		{
		EditorDirectory editDir;
		chdir("ads");
		arq= ged_SDL_RWFromFile(s, "wb");
		}

		if(arq)
		{
			gedString data(url + "|" + altText);
			
			SDL_RWwrite(arq, data.c_str(), data.length(), 1);
			SDL_RWclose(arq);
			
			bGetBannerFromNet = true;
			
			free(image);

			{
			EditorDirectory editDir;
			chdir("ads");
			newestBanner.SetBanner(bannerFileName);
			}
			return true;
		}
	}
	

	free(image);
	return false;
}

long AdsControl::CountBanners(long *iMin, long *iMax)
{
	long nBanners = 0;
	EditorDirectory editDir;
	chdir("ads");

	long handle = ged_findfirst("bn*", A_ANY);

	if(handle != GED_NOTFOUND)
	{	
		do
		{
			if(!(ged_attrib(handle) & A_DIREC))
			{
				//Get min and max index
				if(iMin && iMax)
				{
					gedString name(ged_filename(handle));
					name = name.substr(2);
					
					*iMin = min(*iMin, atol(name.c_str()));
					*iMax = max(*iMax, atol(name.c_str()));
				}

				//Count
				nBanners++;
			}
			

		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);

		ged_findclose(handle);
	}

	return nBanners;
}

void AdsControl::SetCurrentBanner()
{
	//Read banner data from disk, if any

	EditorDirectory editDir;
	chdir("ads");

	long nBanners = CountBanners(), i = 0;
	if(nBanners == 0) return;

	iCurrentBanner = (iCurrentBanner + 1) % nBanners;

	long handle = ged_findfirst("bn*", A_ANY);

	if(handle != GED_NOTFOUND)
	{	
		do
		{
			if(!(ged_attrib(handle) & A_DIREC))
			{
				if(i == iCurrentBanner)
				{
					if(currentBanner.SetBanner(ged_filename(handle)))
					{
						haveBanner = true;
						break;
					}
					else 
					{
						iCurrentBanner = (iCurrentBanner + 1) % nBanners;
					}
				}

				i++;
			}			

		} while((handle = ged_findnext(handle)) != GED_NOTFOUND);

		ged_findclose(handle);
	}	
}

bool Banner::SetBanner(gedString bannerFile)
{
	//Read data file for this banner
	//url|alt text
	
	SDL_RWops *dataFile = ged_SDL_RWFromFile((gedString("_")+bannerFile).c_str(), "r");
	if(!dataFile) return false;

	
	char buf[2049];
	memset(buf, 0, 2048);

	SDL_RWread(dataFile, buf, 2048, 1);
	SDL_RWclose(dataFile);

	gedString data(buf);
	int i = data.find('|');
	if(i == -1) return false;

	buf[i] = 0;


	//Set data
	imageFile = bannerFile;
	url = buf;
	if(strlen(&buf[i+1]) > 0) altText = &buf[i+1];


	return true;
}



#endif //STAND_ALONE_GAME






