// Text.h: interface for the Text class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__8FC2EFC5_8DAD_4C91_BFBF_7812EA70D81E__INCLUDED_)
#define AFX_TEXT_H__8FC2EFC5_8DAD_4C91_BFBF_7812EA70D81E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

#define CENTER_TEXT		-2
#define ALIGN_LEFT      0
#define ALIGN_CENTER    1
#define ALIGN_RIGHT     2
#define ALIGN_AUTOMATIC 3
#define ALL_LINES		-1

class Text  
{
public:

//#ifndef STAND_ALONE_GAME
	static void GetDimensions(const gedString& text, int &width, int &height, const gedString &fontName = "maksfont.bmp");
	static KrRect GetDimensions(const gedString& text, const gedString &fontName = "maksfont.bmp");
	KrRect GetDimensionsFont(const gedString& text);
//#endif
	//const KrRect& Bounds(int window) {return textBox->CalculatedBounds(window);}
	gedString GetText(int line = -1);
	const gedString& getFontName() {return fontName;}
	int getiniASCIIChar() {return iniASCIIChar;}
	int getnChars() {return nChars;}
	KrFontResource *getFontResource() {return fontResource;}
	KrTextWidget *getImage() {return textBox;}
	
	int Left();
	int Width();
	int Top();
	int Right();
	int Down();
	int Height();
	Text(const gedString &text, int x = 0, int y = 0, int align = ALIGN_LEFT, Actor *parent = NULL,
		 int red = 0, int green = 0, int blue = 0,
		 const gedString &fontName = "maksfont.bmp",
		 int iniASCIIChar = 32, int nChars = 94,
		 bool _bEditable = false, bool _hasBorder = false, bool _hasBackGround = false, bool _getFocus = false,
		 KrRGBA _borderColor = 0, KrRGBA _cursorColor = 0, KrRGBA _backGroundColor = 0);

	virtual ~Text();

	void SetText(gedString text);
	void SetColor(int r, int g, int b);


	bool IsEditable() {return bEditable;}
	bool HasBorder() {return bDrawBorder;}
	bool HasBackground() {return bDrawBackground;}
	bool HasGetFocus() {return bGetFocus;}

	KrRGBA getBorderColor() {return borderColor;}
	KrRGBA getCursorColor() {return cursorColor;}
	KrRGBA getBackGroundColor() {return backGroundColor;}

private:
	KrFontResource *fontResource;
	KrTextWidget *textBox;
	gedString fontName, text;
	int iniASCIIChar, nChars;

	bool bEditable, bDrawBorder, bDrawBackground, bGetFocus;
	KrRGBA borderColor, cursorColor, backGroundColor;
};

#endif // !defined(AFX_TEXT_H__8FC2EFC5_8DAD_4C91_BFBF_7812EA70D81E__INCLUDED_)
