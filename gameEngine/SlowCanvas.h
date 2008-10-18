// SlowCanvas.h: interface for the SlowCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLOWCANVAS_H__06F2298B_6DA9_421C_9825_6EB7CEA3A551__INCLUDED_)
#define AFX_SLOWCANVAS_H__06F2298B_6DA9_421C_9825_6EB7CEA3A551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "engine/engine.h"

class SlowCanvas : public KrImage  
{
public:
	static bool LineInRect(int x0, int y0, int x1, int y1, const KrRect &clip);
	static KrRGBA MixColors(const KrRGBA& color1, const KrRGBA& color2, double amount);
	static void Line(KrPaintInfo* paintInfo, const KrRect& clip, int x1, int y1, int x2, int y2, const KrRGBA& color1, const KrRGBA& color2, int pensize = 1);
	
	void ScreenResolution(int width, int height);
	void DrawBackground(SDL_Surface *bg, SDL_Surface *screen, SDL_Rect &clip, int x, int y);
	void SetTileImage(const gedString& image);
	void SetDimensions(int x, int y, int width, int height);
	void SetDimensions(int width, int height);
	SlowCanvas(void *parent, int width = 100, int height = 100);
	virtual ~SlowCanvas();

	virtual void Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip);

	int Width()			{ return width; }
	int Height()		{ return height; }


	virtual void QueryBoundingBox( KrRect* boundingBox );
	virtual bool HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results );
	virtual void CalcTransform();

	virtual KrResource* Resource();

	void setInfinite() {bInfinite = true;};
	void setCustomHitTest(bool hit) {bCustomHitTest = hit;};	
	void setEntireBoxHit(bool entire) {bEntireBox = entire;};
	void setThickness(int d) {thickness = d;}
	void setMinDimentions(int w, int h) {minWidth = w; minHeight = h;}


private:
	//Line clipping
	static unsigned int ComputeCode (int x, int y, const KrRect &r);
	static void CohenSutherlandClip (KrPainter& painter, int x0, int y0, int x1, int y1, KrRGBA color1, KrRGBA color2, const KrRect &clip, int pensize = 1);

	//Line
	static void line(KrPainter& painter, int x1, int y1, int x2, int y2, const KrRGBA& color1, const KrRGBA& color2, const KrRect &clip, int pensize = 1);

	int width;
	int height;
	int screenWidth;
	int screenHeight;
	bool bInfinite;
	SDL_Surface *img;
	bool bEntireBox;
	bool bCustomHitTest;
	int thickness;
	int minWidth, minHeight;
};

#endif // !defined(AFX_SLOWCANVAS_H__06F2298B_6DA9_421C_9825_6EB7CEA3A551__INCLUDED_)
