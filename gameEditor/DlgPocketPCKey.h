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
