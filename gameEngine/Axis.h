// Axis.h: interface for the Axis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXIS_H__CA2512DC_B87D_4BF1_A460_F7A8936B483E__INCLUDED_)
#define AFX_AXIS_H__CA2512DC_B87D_4BF1_A460_F7A8936B483E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

class Actor;
#include "ActivationEventsCanvas.h"

#ifndef STAND_ALONE_GAME

class Axis;

//Draw axis, grid and other special drawings
class AxisGridCanvas : public Actor  
{
public:
	
	AxisGridCanvas(Actor *parent);
	virtual ~AxisGridCanvas();

	void OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ);
	const stCursor *OnGetCursor() {return NULL;}

	void DrawAxis(KrPainter *painter, KrRect &clip);
	void DrawGrid(KrPainter *painter, SDL_Rect &clip, int gridx, int gridy, int x_offset, int y_offset, int lineSize, const KrRGBA& color);
	
private:

};

class AxisPathCanvas : public Actor  
{
public:
	
	AxisPathCanvas(Actor *parent);
	virtual ~AxisPathCanvas();

	void OnSlowCanvasPaint(KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ);
	const stCursor *OnGetCursor() {return NULL;}

	void DrawPath(Path *path, KrPainter *painter, SDL_Rect &clip);
	void DrawPaths(KrPainter *painter, SDL_Rect &clip);	
	
private:

};

#endif

//Game root
class Axis : public Actor  
{
	friend class AxisGridCanvas;
	friend class AxisPathCanvas;

public:

#ifndef ACTOR_USES_VIRTUAL_FUNCTIONS
	//Overload the memory alocators to avoid crash when delete the class derived from Actor
	void *operator new(size_t size)	{return dlmalloc(size);}
	void operator delete(void * mem) {dlfree(mem);}
#endif

	void GameResolution(int width, int height);
	void ScreenResolution(int width, int height);
	void GridSnap(int &x, int &y);
		
	bool OnMouseButtonDown(int x, int y, Uint8 button);
	const stCursor * OnGetCursor();
	
	Axis();
	virtual ~Axis();

	static Axis *Get() {return axis;}

	double getScale() {return scale;};
	bool getGridSnap() {return bGridSnap;}
	bool getGridShow() {return bGridShow;}
	int getGridX() {return gridX;};
	int getGridY() {return gridY;};
	
	KrRGBA getMainColor() {return mainColor;}
	KrRGBA getResolutionColor() {return resolutionColor;}

	void setGridSnap(bool bSet) {bGridSnap = bSet;}
	void setGridShow(bool bSet) {bGridShow = bSet;}
	void setGridX(int x) {gridX = x;};
	void setGridY(int y) {gridY = y;};

	void setMainColor(KrRGBA color) {mainColor = color;};
	void setResolutionColor(KrRGBA color) {resolutionColor = color;};

	int getScreenWidth() {return screenWidth;}
	int getScreenHeight() {return screenHeight;}

	

#ifndef STAND_ALONE_GAME

	void SetGridZDepth(int zdepth);
	int GetGridZDepth();
	void OnKeyDown(SDLKey key, int repeatLoopCount = 0);
	void OnKeyUp(SDLKey key);
	
#endif //#ifndef STAND_ALONE_GAME

	void SetScale(double scale);

private:
	double scale;
	KrRGBA pathColor, axisColor;
	int screenWidth, gameWidth;
	int screenHeight, gameHeight;

	KrRGBA mainColor, resolutionColor;
	
	bool bGridSnap;
	bool bGridShow;
	int gridX, gridY;

	static Axis *axis;

	ActivationEventsCanvas drawActivateEventLines; 

#ifndef STAND_ALONE_GAME
	//Create here, only	
	AxisGridCanvas *gridCanvas;
	AxisPathCanvas *pathCanvas;
#endif
};

#endif // !defined(AFX_AXIS_H__CA2512DC_B87D_4BF1_A460_F7A8936B483E__INCLUDED_)
