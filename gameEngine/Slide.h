// Slide.h: interface for the Slide class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_SLIDE_H__CD2D6ED2_891F_4E7F_8222_7F2A986DD544__INCLUDED_)
#define AFX_SLIDE_H__CD2D6ED2_891F_4E7F_8222_7F2A986DD544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Panel.h"

class EditBox;

class SlideCursor : public Panel 
{
public:
	void SetPos(double pos);
	const stCursor * OnGetCursor();
	void OnAnimation();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	SlideCursor(Actor *parent, int width, int height);
	virtual ~SlideCursor();

	int ant;

protected:
	int widthParent;
	int heightParent;
	int width;
	int height;
	bool bHorizontal;	
};

class Slide : public Panel  
{
public:
	void SetPos(double pos);
	double GetPos();
	Slide(Actor *parent, int x, int y, int width = 100, int height = 14, int slideId = 0);
	virtual ~Slide();

	int getSlideId() {return slideId;};

protected:
	SlideCursor *cursor;
	int slideId;
	double atualPos;
};

class ScrollBarCursor : public Actor 
{
public:

	ScrollBarCursor(Actor *parent, EditBox *_edit, int width, int height);
	virtual ~ScrollBarCursor();

	void OnAnimation();
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 button);
	void OnMouseEnter(int x, int y);	
	void OnMouseLeave(int x, int y);

	void Draw();
	void DrawBorder(KrRGBA color1, KrRGBA color2);

	void SetScrollInfo(int _totalItens, int _itensPerPage);
	void SetScrollPos(int line);
	int GetScrollPos();

	int getScrollPageSize() {return scrollPageSize;}

	const stCursor * OnGetCursor();

protected:
	
	EditBox *edit;
	int side, minPos, maxPos, scrollPageSize, pos, yAnt;
	int totalItens, itensPerPage;
	bool bMove;

	KrColorTransform panelColor;
};

class ScrollBar : public Actor  
{
public:
	ScrollBar(Actor *parent, EditBox *_edit, int _scrollId = 0);
	virtual ~ScrollBar();

	int getScrollBarId() {return scrollId;};

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 button);
	void OnMouseMove(int x, int y);

	Uint32 OnTimer(SDL_TimerID timerID);

	void ScrollUp();
	void ScrollDown();
	void ScrollPageUp();
	void ScrollPageDown();
	void SetScrollInfo(int totalItens, int itensPerPage);
	void SetScrollPos(int line);
	int GetScrollPos();

	const stCursor * OnGetCursor();

protected:

	void DrawBackGround();
	void DrawArrowButtons();
	void RemoveTimers();

	EditBox *edit;
	int scrollId, side;
	bool bInArrow;

	SDL_TimerID scrollUp, scrollDown, scrollPageUp, scrollPageDown;
	int scrollRepeatTime;

	ScrollBarCursor *scrollCursor;
};

#endif // !defined(AFX_SLIDE_H__CD2D6ED2_891F_4E7F_8222_7F2A986DD544__INCLUDED_)


#endif //#ifndef STAND_ALONE_GAME