// AboutDlg.h: interface for the AboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_ABOUTDLG_H__E313A40C_4FD1_4FB7_8379_0CF376EBC892__INCLUDED_)
#define AFX_ABOUTDLG_H__E313A40C_4FD1_4FB7_8379_0CF376EBC892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class AboutDlg : public Panel  
{
public:
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	AboutDlg();
	virtual ~AboutDlg();
	

private:
	Actor *logo;//, *kyraLogo, *sdlLogo;
};

class RegisterPanel : public Panel  
{
public:
	void OnButton(Button *button, int buttonId);

	RegisterPanel(gedString _keyCreationDate, int _purchasedDays, bool bUpdate = false);
	virtual ~RegisterPanel();
	

private:


	gedString keyCreationDate;
	//int purchasedDays;
};

extern gedString getVersion();

#endif // !defined(AFX_ABOUTDLG_H__E313A40C_4FD1_4FB7_8379_0CF376EBC892__INCLUDED_)

#endif //STAND_ALONE_GAME
