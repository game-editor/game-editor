// TutorialPaint.h: interface for the TutorialPaint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#if !defined(AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_)
#define AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Actor.h"

class TutorialPaint : public Actor  
{
public:
	void Execute(SDL_Event &event);
	static void Hide();
	static void Show();
	static void Destroy();
	static TutorialPaint *Call();
	static TutorialPaint *Get() {return paint;};
	static bool InPaintMode() {return paint != NULL;}
	
	virtual ~TutorialPaint();

	bool OnMouseButtonDown(int x, int y, Uint8 button);
	void OnMouseButtonUp(int x, int y, Uint8 mouseState);
	void OnMouseMove(int x, int y);
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnKeyUp(SDLKey key);
	const stCursor *OnGetCursor();

protected:
	TutorialPaint();

	static TutorialPaint *paint;
	
	bool bPaint, bErase, bLine;
	bool bOnDestructor;
	int width, height;
	int xAnt, yAnt;

	Uint8 button;
};

#endif // !defined(AFX_TUTORIALPAINT_H__2D8E447F_7428_43E5_9461_5E3A4A0796BB__INCLUDED_)

#endif //STAND_ALONE_GAME
