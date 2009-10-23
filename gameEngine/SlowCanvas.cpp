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


// SlowCanvas.cpp: implementation of the SlowCanvas class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)
#include "SlowCanvas.h"
#include "Actor.h"
#include "GameControl.h"
#include <math.h>

#define ABS(a)		(((a)<0) ? -(a) : (a))
#define SGN(a)		(((a)<0) ? -1 : 1)

/*Uint32 approx_distance( Sint32 dx, Sint32 dy )
{
	//http://www.flipcode.com/articles/article_fastdistance.shtml
   Uint32 min, max, approx;

   if ( dx < 0 ) dx = -dx;
   if ( dy < 0 ) dy = -dy;

   if ( dx < dy )
   {
      min = dx;
      max = dy;
   } else {
      min = dy;
      max = dx;
   }

   approx = ( max * 1007 ) + ( min * 441 );
   if ( max < ( min << 4 ))
      approx -= ( max * 40 );

   // add 512 for proper rounding
   return (( approx + 512 ) >> 10 );
}

Uint32 approx_distance2( Sint32 dx, Sint32 dy )
{
	//http://www.flipcode.com/articles/article_fastdistance.shtml
   Uint32 min, max;

   if ( dx < 0 ) dx = -dx;
   if ( dy < 0 ) dy = -dy;

   if ( dx < dy )
   {
      min = dx;
      max = dy;
   } else {
      min = dy;
      max = dx;
   }

   // coefficients equivalent to ( 123/128 * max ) and ( 51/128 * min )
   return ((( max << 8 ) + ( max << 3 ) - ( max << 4 ) - ( max << 1 ) +
            ( min << 7 ) - ( min << 5 ) + ( min << 3 ) - ( min << 1 )) >> 8 );
}*/

/*void testExecution()
{
	//sqrt is faster !!! How?

	int dx = -23, dy = 10;

	//sqrt
	DWORD t1 = GetTickCount();
	for(long n = 0; n < 10000000; n++)
	{
		sqrt(dx*dx + dy*dy);	
	}
	DWORD tSqrt = GetTickCount() - t1; //438 ms

	
	//approx_distance
	DWORD t2 = GetTickCount();
	for(n = 0; n < 10000000; n++)
	{
		approx_distance(dx, dy);
	}
	DWORD tApprox_distance = GetTickCount() - t2; //468 ms
	
	//approx_distance2
	DWORD t3 = GetTickCount();
	for(n = 0; n < 10000000; n++)
	{
		approx_distance2(dx, dy);
	}
	DWORD tApprox_distance2 = GetTickCount() - t3; //485 ms
}*/


double fast_distance(double x1, double y1, double x2, double y2)
{
	/*GLOUTPUT( "*** sqrt: %ld\n", (int)sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)) );
	GLOUTPUT( "***fast2: %ld\n", approx_distance2(x2-x1, y2-y1) );
	GLOUTPUT( "*** fast: %ld\n\n", approx_distance(x2-x1, y2-y1) );*/

	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	//return approx_distance2(x2-x1, y2-y1);
}



/*
 * define the coding of end points
 */
enum {TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8};


SlowCanvas::SlowCanvas(void *parent, int width, int height)
{
	this->parentActor = parent;
	this->width =  width;
	this->height = height;
	screenWidth = GameControl::Get()->Width();
	screenHeight = GameControl::Get()->Height();
	bInfinite = false;
	bCustomHitTest = false;
	img = NULL;

	SetHitTest(false);
	bEntireBox = true;

	thickness = 2;
	minWidth =  minHeight = 0;
}

SlowCanvas::~SlowCanvas()
{
	if(img) SDL_FreeSurface(img);
}	

void SlowCanvas::Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip )
{
#ifndef STAND_ALONE_GAME
	if ( bounds.Intersect( clip ) )
	{
		KrRect rect(bounds);
		rect.DoIntersection(clip);
		
		//KrVector2T< GlFixed > object;
		//ScreenToObject( x, y, &object );
		if(img)
		{
			SDL_Rect sdlrect = { rect.xmin, rect.ymin, rect.Width(), rect.Height() };
			DrawBackground(img, paintInfo->surface, sdlrect, -X(), -Y());
		}

		if(parentActor)
			((Actor *)parentActor)->OnSlowCanvasPaint(paintInfo, paintInfo->surface, rect, TreeDepth());				
	}
#endif
}


void SlowCanvas::CalcTransform()
{
	// Calculate our new coordinates, and then bounding info.
	KrImNode::CalcTransform();

	if(!bInfinite)
	{
		GlFixed x = CompositeXForm().x/* - width/2*/ /*- X()*/,
			y = CompositeXForm().y/* - height/2*/ /*- Y()*/,
			scale = CompositeXForm().xScale; //maks
		
		bounds.xmin = x.ToIntRound();
		bounds.ymin = y.ToIntRound();
		bounds.xmax = ( x +  scale*width  ).ToIntRound() - 1;
		bounds.ymax = ( y + scale*height ).ToIntRound() - 1;
	}
	else
	{
		bounds.xmin = 0;
		bounds.ymin = 0;
		bounds.xmax = screenWidth-1;
		bounds.ymax = screenHeight-1;
	}

#ifdef USE_RTREE
	UpdateRTree();
#endif
}


bool SlowCanvas::HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results )
{
	if(bEnableHit && IsVisible() && CompositeCForm().Alpha())
	{
		if(bEntireBox)
		{
			if(bounds.Intersect( x, y ))
			{
				results->PushBack( this );
				return true;
			}
		}
		else if(bCustomHitTest && parentActor)
		{
			if(((Actor *)parentActor)->OnSlowCanvasHitTest(x, y))
			{
				results->PushBack( this );
				return true;
			}
		}
		else
		{
			if(((abs(y - bounds.ymin) <= thickness  || abs(y - bounds.ymax) <= thickness) && x >= bounds.xmin && x <= bounds.xmax) ||	 //Top, down
			   ((abs(x - bounds.xmin) <= thickness   || abs(x - bounds.xmax) <= thickness) && y >= bounds.ymin && y <= bounds.ymax))	 //Left, right
			{
				results->PushBack( this );
				return true;
			}

			if(((y == bounds.ymin+1  || y == bounds.ymax+1) && x >= bounds.xmin && x <= bounds.xmax) ||	 //Top, down
			   ((x == bounds.xmin+1  || x == bounds.xmax+1) && y >= bounds.ymin && y <= bounds.ymax))	 //Left, right
			{
				results->PushBack( this );
				return true;
			}
			  
		}
	}
	
	return false;
}


void SlowCanvas::QueryBoundingBox( KrRect* boundingBox )
{
	KrMatrix2 xForm = CompositeXForm();

	boundingBox->xmin = xForm.x.ToIntRound();
	boundingBox->ymin = xForm.y.ToIntRound();
	boundingBox->xmax = ( xForm.xScale * width + xForm.x ).ToIntRound() - 1;
	boundingBox->ymax = ( xForm.yScale * height + xForm.y ).ToIntRound() - 1;
}

KrResource* SlowCanvas::Resource()
{
	return NULL;
}

void SlowCanvas::SetDimensions(int x, int y, int width, int height)
{
	int newX = X(), newY = Y();
	

	if(width >= minWidth)
	{
		this->width = width;
		newX = x;
	}
	if(height >= minHeight)
	{
		this->height = height;
		newY = y;
	}

	SetPos(newX, newY);

	CalcTransform();

	if(parentActor)
	{
		((Actor *)parentActor)->getScriptVars()->width = Width();
		((Actor *)parentActor)->getScriptVars()->height = Height();
	}
}

void SlowCanvas::SetDimensions(int width, int height)
{
	if(width >= minWidth)
		this->width = width;
	if(height >= minHeight)
		this->height = height;

	CalcTransform();

	if(parentActor)
	{
		((Actor *)parentActor)->getScriptVars()->width = Width();
		((Actor *)parentActor)->getScriptVars()->height = Height();
	}
}


void SlowCanvas::SetTileImage(const gedString& image)
{
	if(img)
	{
		SDL_FreeSurface(img);
		img = NULL;
	}

	gedString aux(image);
	SDL_Surface *temp = ged_IMG_Load(aux.c_str());
	if(temp)
	{
		img = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}
}


void SlowCanvas::ScreenResolution(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	CalcTransform();
}

static void DrawPoint(KrPainter& painter, int x, int y, KrRGBA &color, const KrRect &clip, int pensize = 1)
{
	int start = -(pensize >> 1), xi, yj;
	int end = start + pensize;
	for(int i = start; i < end; i++)
	{
		xi = x + i;

		for(int j = start; j < end; j++)
		{
			yj = y + j;

			if( xi >= clip.xmin && xi < clip.xmax &&
				yj >= clip.ymin && yj < clip.ymax )
			{
				painter.SetPixel(xi, yj, color);				
			}
		}
	}

	/*if(x >= clip.xmin && x < clip.xmax && y >= clip.ymin && y < clip.ymax) painter.SetPixel(x, y, color);
	else return;
	
	if(pensize > 1)  	
	{		
		if(x+1 < clip.xmax)  { x++; painter.SetPixel(x, y, color); } else return;
		if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
		if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
		
		if (pensize > 2) 
		{
			if(x+2 < clip.xmax)  { x+=2; painter.SetPixel(x, y, color); } else return;
			if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
			if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
			if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			
			if (pensize > 3) 
			{	// expecting 4 ... 9	
				
				if(x+3 < clip.xmax)  { x+=3; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			}
			
			if (pensize > 7) 
			{	// who knows	
				
				if(x+4 < clip.xmax)  { x+=4; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			}

			if (pensize > 12) 
			{	// who knows	
				
				if(x+5 < clip.xmax)  { x+=5; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			}

			if (pensize > 15) 
			{	// who knows	
				
				if(x+6 < clip.xmax)  { x+=6; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(y+1 < clip.ymax)  { y++; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
				if(x-1 >= clip.xmin) { x--; painter.SetPixel(x, y, color); } else return;
			}
			
		}
	}*/
}

/*
 * compute the code of a point relative to a rectangle
 */
unsigned int SlowCanvas::ComputeCode (int x, int y, const KrRect &r)
{
	unsigned int c = 0;
	if (y > r.ymax)
		c |= TOP;
	else if (y < r.ymin)
		c |= BOTTOM;
	if (x > r.xmax)
		c |= RIGHT;
	else if (x < r.xmin)
		c |= LEFT;
	return c;
}

/*
 *	clip line P0(x0,y0)-P1(x1,y1) against a rectangle
 */
void SlowCanvas::CohenSutherlandClip (KrPainter& painter, int x0, int y0, int x1, int y1, KrRGBA color1, KrRGBA color2, const KrRect &clip, int pensize)
{
	unsigned int C0, C1, C;
	int x, y, xi = x0, yi = y0;
	double lenght = 1.0;	

	if(color1 != color2)
		lenght = fast_distance(x0, y0, x1, y1);
	
	C0 = ComputeCode (x0, y0, clip);
	C1 = ComputeCode (x1, y1, clip);
	
	for (;;) {
		/* trivial accept: both ends in rectangle */
		if ((C0 | C1) == 0) 
		{
			if(color1 != color2)
			{
				double	ini = fast_distance(x0, y0, xi, yi),
						fin = fast_distance(x1, y1, xi, yi);
				
				color1 = MixColors(color1, color2, ini/lenght);
				color2 = MixColors(color1, color2, fin/lenght);
			}

			line(painter, x0,y0,x1,y1, color1, color2, clip, pensize);
			return;
		}
		
		/* trivial reject: both ends on the external side of the rectangle */
		if ((C0 & C1) != 0)
			return;
		
		/* normal case: clip end outside rectangle */
		C = C0 ? C0 : C1;
		if (C & TOP) {
			x = x0 + (x1 - x0) * (clip.ymax - y0) / (y1 - y0);
			y = clip.ymax;
		} else if (C & BOTTOM) {
			x = x0 + (x1 - x0) * (clip.ymin - y0) / (y1 - y0);
			y = clip.ymin;
		} else if (C & RIGHT) {
			x = clip.xmax;
			y = y0 + (y1 - y0) * (clip.xmax - x0) / (x1 - x0);
		} else {
			x = clip.xmin;
			y = y0 + (y1 - y0) * (clip.xmin - x0) / (x1 - x0);
		}
		
		/* set new end point and iterate */
		if (C == C0) {
			x0 = x; y0 = y;
			C0 = ComputeCode (x0, y0, clip);
		} else {
			x1 = x; y1 = y;
			C1 = ComputeCode (x1, y1, clip);
		}
	}
	
	/* notreached */
}

bool SlowCanvas::LineInRect(int x0, int y0, int x1, int y1, const KrRect &clip)
{
	unsigned int C0, C1, C;
	int x, y;//, xi = x0, yi = y0;
	
	C0 = ComputeCode (x0, y0, clip);
	C1 = ComputeCode (x1, y1, clip);
	
	for (;;) {
		/* trivial accept: both ends in rectangle */
		if ((C0 | C1) == 0) 
			return true;
		
		/* trivial reject: both ends on the external side of the rectangle */
		if ((C0 & C1) != 0)
			return false;
		
		/* normal case: clip end outside rectangle */
		C = C0 ? C0 : C1;
		if (C & TOP) {
			x = x0 + (x1 - x0) * (clip.ymax - y0) / (y1 - y0);
			y = clip.ymax;
		} else if (C & BOTTOM) {
			x = x0 + (x1 - x0) * (clip.ymin - y0) / (y1 - y0);
			y = clip.ymin;
		} else if (C & RIGHT) {
			x = clip.xmax;
			y = y0 + (y1 - y0) * (clip.xmax - x0) / (x1 - x0);
		} else {
			x = clip.xmin;
			y = y0 + (y1 - y0) * (clip.xmin - x0) / (x1 - x0);
		}
		
		/* set new end point and iterate */
		if (C == C0) {
			x0 = x; y0 = y;
			C0 = ComputeCode (x0, y0, clip);
		} else {
			x1 = x; y1 = y;
			C1 = ComputeCode (x1, y1, clip);
		}
	}
	
	/* notreached */
}




KrRGBA SlowCanvas::MixColors(const KrRGBA &color1, const KrRGBA &color2, double amount)
{
	/*
	amount = 0 => color = color1
	amount = 1 => color = color2
	*/

	KrRGBA color;

	color.c.red		= color1.c.red*(1.0 - amount) + color2.c.red*amount;
	color.c.green	= color1.c.green*(1.0 - amount) + color2.c.green*amount;
	color.c.blue	= color1.c.blue*(1.0 - amount) + color2.c.blue*amount;
	color.c.alpha	= 255;

	return color;
}

void SlowCanvas::line(KrPainter& painter, int x1, int y1, int x2, int y2, const KrRGBA& color1, const KrRGBA& color2, const KrRect &clip, int pensize)
{
	/*
 * Digital Line Drawing
 * by Paul Heckbert
 * from "Graphics Gems", Academic Press, 1990
 */

/*
 * digline: draw digital line from (x1,y1) to (x2,y2),
 * calling a user-supplied procedure at each pixel.
 * Does no clipping.  Uses Bresenham's algorithm.
 *
 * Paul Heckbert	3 Sep 85
 */

	KrRGBA color = color1;
	

    int d, x, y, ax, ay, sx, sy, dx, dy;
	double lenght = 1.0;

	if(color1 != color2)
		lenght = fast_distance(x1, y1, x2, y2);
	
    dx = x2-x1;  ax = ABS(dx)<<1;  sx = SGN(dx);
    dy = y2-y1;  ay = ABS(dy)<<1;  sy = SGN(dy);
	
    x = x1;
    y = y1;
    if (ax>ay) 
	{		/* x dominant */
		d = ay-(ax>>1);
		for (;;) 
		{
			if(color1 != color2)
				color = MixColors(color1, color2, fast_distance(x1, y1, x, y)/lenght);

			DrawPoint(painter, x, y, color, clip, pensize);
			
			if (x==x2) return;
			if (d>=0) {
				y += sy;
				d -= ax;
			}
			x += sx;
			d += ay;
		}
    }
    else 
	{			/* y dominant */
		d = ax-(ay>>1);
		for (;;) 
		{
			if(color1 != color2)
				color = MixColors(color1, color2, fast_distance(x1, y1, x, y)/lenght);

			DrawPoint(painter, x, y, color, clip, pensize);

			if (y==y2) return;
			if (d>=0) {
				x += sx;
				d -= ay;
			}
			y += sy;
			d += ax;
		}
    }
}


void SlowCanvas::Line(KrPaintInfo *paintInfo, const KrRect &clip, int x1, int y1, int x2, int y2, const KrRGBA& color1, const KrRGBA& color2, int pensize)
{
	//Draw a gradient line

	KrPainter painter(paintInfo);
	CohenSutherlandClip(painter, x1, y1, x2, y2, color1, color2, clip, pensize);	
}



#ifndef STAND_ALONE_GAME
void SlowCanvas::DrawBackground(SDL_Surface *bg, SDL_Surface *screen, SDL_Rect &clip, int x, int y)
{
	if(!bg) return;

	SDL_Rect sr, dr, cl = clip;
	int xIni = cl.x, 
		xmax = cl.x + cl.w,
		ymax = cl.y + cl.h;

	do
	{		
		do
		{
			sr.x = cl.x - (cl.x/bg->w)*bg->w;
			sr.y = cl.y - (cl.y/bg->h)*bg->h;

			/////////////////////////////////
			//Pan
			sr.x = (sr.x + x) % bg->w;
			sr.y = (sr.y + y) % bg->h;

			if(sr.x < 0) sr.x = bg->w + sr.x;
			if(sr.y < 0)sr.y = bg->h + sr.y;
			/////////////////////////////////
			
			sr.w = bg->w - sr.x;
			sr.h = bg->h - sr.y;
			
			dr.x = cl.x;
			dr.y = cl.y;
			
			dr.w = sr.w;
			dr.h = sr.h;

			if(dr.w > cl.w) dr.w = sr.w = cl.w;
			if(dr.h > cl.h) dr.h = sr.h = cl.h;			

			if((dr.x + dr.w) >= xmax)
			{
				sr.w = dr.w = xmax - dr.x;
			}

			if((dr.y + dr.h) >= ymax)
			{
				sr.h = dr.h = ymax - dr.y;
			}

			SDL_BlitSurface(bg, &sr, screen, &dr);
			
			cl.x += dr.w;
			
		} while(xmax > cl.x && dr.w > 0);

		cl.x = xIni;
		cl.y += dr.h;
	} while(ymax > cl.y && dr.h > 0);
}









#endif //#ifndef STAND_ALONE_GAME
