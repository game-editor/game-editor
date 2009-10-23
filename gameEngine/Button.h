/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// Button.h: interface for the Button class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_BUTTON_H__2571861F_C2C3_40CD_B6F2_66DEC3797DBD__INCLUDED_)
#define AFX_BUTTON_H__2571861F_C2C3_40CD_B6F2_66DEC3797DBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "engine/kyra.h"
#include "Actor.h"

class Button : public Actor  
{
public:
	void Disable();
	void Enable();

	void SetPressed(bool bPressed);
	
	void SetText(gedString text);
	void OnMouseButtonUp(int x, int y, Uint8 mouseState);
	bool OnMouseButtonDown(int x, int y, Uint8 mouseState);
	void OnMouseLeave(int x, int y);
	void OnMouseEnter(int x, int y);
	Button(gedString text, 
			   int x = 0, int y = 0,
			   int width = 0, int height = 0,
			   int buttonId = -1,
			   Actor *parent = NULL,
			   int red = 0, int green = 0, int blue = 0,
			   const gedString spriteName = "maksbutton.bmp",
			   gedString fontName = "maksfont.bmp",
			   bool _bDrawArrows = false);

	virtual ~Button();

	int getButtonId() {return buttonId;}
	static bool getHaveButtonPressed() {return bHaveButtonPressed;}

#ifndef STAND_ALONE_GAME
	void SetToolTip(const gedString& tip) {actorTip = tip;}
	const gedString *OnActorTip() {return &actorTip;}
#endif

protected:
	void Draw(bool bHighLight);
	//float sx, sy;
	KrFontResource *fontResource;
	KrTextBox *textBox;
	int px;
	int py;
	int offSetText;
	int buttonId;
	bool buttonDown, bStayPressed, bDrawArrows;

	bool bEnable;
	static bool bHaveButtonPressed;

#ifndef STAND_ALONE_GAME
	gedString			actorTip;
#endif
};

#endif // !defined(AFX_BUTTON_H__2571861F_C2C3_40CD_B6F2_66DEC3797DBD__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME
