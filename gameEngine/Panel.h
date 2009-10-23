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


// Panel.h: interface for the Panel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_PANEL_H__188A67EC_0321_4226_9E0D_9050963EA2F4__INCLUDED_)
#define AFX_PANEL_H__188A67EC_0321_4226_9E0D_9050963EA2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"
#include "Text.h"

#define SHADOW_THICKNESS 8
#define SHADOW_FADE		 3
#define SHADOW_MAX		 80

class Panel;

typedef GlDynArray<Panel *> ListPanel;  

enum
{
	OK_BUTTON = 1,
	CANCEL_BUTTON
};

class Panel : public Actor  
{
public:
	int Wait();
	void RestoreColor();
	void SetColorFocus();
	static void ReleaseFocus(Panel *panel);
	static void GetFocus(Panel *panel);

	void SendButtonDownMessage(Button *button);
	void SetConfirmButton(Button *button);
	void SetCancelButton(Button *button);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void SetFocus(int i);

	int DrawHLine(int y, int xi = 0, int width = 0);
	Panel(const gedString spriteName, int x, int y, int width, int height,
		   Actor *parent = NULL, bool _bInvertBevel = false, bool bDrag = true, bool bPanel = true, bool _bShadows = true);
	virtual ~Panel();

	virtual bool OnMouseButtonDown(int x, int y, Uint8 button);
	virtual const stCursor *OnGetCursor();

	void Draw();

#ifndef STAND_ALONE_GAME
	void SetToolTip(const gedString& tip) {actorTip = tip;}
	const gedString *OnActorTip() {return &actorTip;}
#endif

	static KrRGBA getColorBack() {return colorBack;}
	static KrRGBA getColorUp() {return colorUp;}
	static KrRGBA getColorDown() {return colorDown;}
	static KrRGBA getColorHighLight() {return colorHighLight;}

protected:
	static KrRGBA colorBack, colorHighLight, colorUp, colorDown;
	KrColorTransform  panelColor;
	bool bDrag;
	int result; //Wait loop result

	bool bInvertBevel;

private:
	static long depth;

	Button *confirmButton, *cancelButton;
	int firstHorizontalLine;
	static ListPanel listPanel;
	static ListPanel listFocus;
	static Panel *actualFocus;


#ifndef STAND_ALONE_GAME
	gedString			actorTip;
#endif
};

#endif // !defined(AFX_PANEL_H__188A67EC_0321_4226_9E0D_9050963EA2F4__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME