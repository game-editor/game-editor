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
