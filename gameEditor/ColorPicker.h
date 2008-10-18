// ColorPicker.h: interface for the ColorPicker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_COLORPICKER_H__5741395A_D7B1_4C23_8135_9C6F2F48B3D1__INCLUDED_)
#define AFX_COLORPICKER_H__5741395A_D7B1_4C23_8135_9C6F2F48B3D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

#define COLOR_CHANGED 100123

class ColorPointer : public Actor
{
public:
	ColorPointer(Actor *parent);
	virtual ~ColorPointer() {}

	bool OnMouseButtonDown(int x, int y, Uint8 button);
};

class ColorCanvas : public Actor
{
public:
	ColorCanvas(Actor *parent);
	virtual ~ColorCanvas();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 button);
	void OnMouseMove(int x, int y);
	void SetPointer(int x, int y) {pointer->SetPos(x - pointer->Width()/2, y - pointer->Height()/2);}

private:
	ColorPointer *pointer;
	bool canPick;
};

class ColorSampleOfPicker : public Actor
{
public:
	ColorSampleOfPicker(Actor *parent);
	virtual ~ColorSampleOfPicker() {};

	bool OnMouseButtonDown(int x, int y, Uint8 button);
};

class ColorSample : public Actor
{
public:
	ColorSample(Actor *parent, int x, int y, int w = 16, int h = 16, bool _bEditControls = false);
	virtual ~ColorSample();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnEditChar(EditBox *edit, SDLKey key, int line);
	void OnMessage(Actor *fromActor, Uint32 messageId, Uint32 message);

	int getRed() {return r;}
	int getGreen() {return g;}
	int getBlue() {return b;}

	gedString getRedText();
	gedString getGreenText();
	gedString getBlueText();

	EditBox *getRedEdit() {return rEdit;}
	EditBox *getGreenEdit() {return gEdit;}
	EditBox *getBlueEdit() {return bEdit;}

	KrRGBA getColor() {KrRGBA color; color.Set(r, g, b); return color;}
	void setColor(KrRGBA color);
	void setColor(int _r, int _g, int _b);

	void SetToolTip(const gedString& tip) {actorTip = tip;}
	
	const gedString *OnActorTip() {return &actorTip;}

	int Width();
	int Right();

private:
	int r, g, b;
	EditBox *rEdit, *gEdit, *bEdit;

	gedString			actorTip;
};

class ColorPicker : public Panel  
{
public:
	void UpdateColorControls();
	void UpdateSample(int _r, int _g, int _b, bool bUpdateHSV = true);
	void UpdateSlideColor(int r, int g, int b, int fixedColor);
	void UpdateCanvas(int c, int fixedColor);
	ColorPicker(int _r, int _g, int _b);
	virtual ~ColorPicker();

	void OnSlide(Slide *slide, int slideId, double pos);
	void OnButton(Button *button, int buttonId);
	bool OnList(ListPop *list, int index, gedString &text, int listId);
	void OnEditChar(EditBox *edit, SDLKey key, int line);

	void SetColor(int _r, int _g, int _b);

	int getRed() {return rSample;}
	int getGreen() {return gSample;}
	int getBlue() {return bSample;}

private:
	ColorCanvas *canvas;
	ColorSampleOfPicker *oldColor, *newColor;

	ListPop *listFix;
	Slide *colorSlide;

	EditBox *rEdit, *gEdit, *bEdit, *hEdit, *sEdit, *vEdit;

	int fixedColor, r, g, b, rSample, gSample, bSample;
};

#endif // !defined(AFX_COLORPICKER_H__5741395A_D7B1_4C23_8135_9C6F2F48B3D1__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME