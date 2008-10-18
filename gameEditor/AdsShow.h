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