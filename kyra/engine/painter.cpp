/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

	Kyra is provided under 2 licenses:

	- The GPL, with no additional restrictions.
	- The LGPL, provided you display the Kyra splash screen, described below.


--- GPL License --
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	The full text of the license can be found in license.txt


--- LGPL License --
  **Provided you kindly display the Kyra splash screen (details below), 
	you	may use the LGPL license:**

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	The full text of the license can be found in lgpl.txt


--- Kyra Splash Screen.

	It would be appreciate if you display the Kyra splash screen when using
	either license, however it is only required for the LGPL. All the
	resources for the splash are compiled into the library, and it can be
	accessed through the following API:

		KrEngine::StartSplash
		KrEngine::UpdateSplash
		KrEngine::EndSplash

	Full documentation is provided with the KrEngine class. The splash screen
	should be displayed for 2 seconds.

	Thank you.
*/

#include "SDL.h"
#ifdef KYRA_SUPPORT_OPENGL
#ifdef __iPhone__
	#include "SDL_opengles.h" // AKR
	#include "SDL_sysvideo.h"
#else
	#include "SDL_opengl.h" // AKR
#endif
#endif

#include "painter.h"
#include "../util/gldebug.h"
#include "../util/glperformance.h"
#include "ogltexture.h"
#include "engine.h" //maks


#ifdef _MSC_VER
#	pragma check_stack(off)
#	pragma intrinsic(memcmp)
#	pragma intrinsic(memcpy)
#	pragma intrinsic(memset)
#	pragma intrinsic(strcmp)
#	pragma intrinsic(strcpy)
#	pragma intrinsic(strlen)
#endif

extern "C" void *memcpy1( void *dest, const void *src, int count );
extern "C" void *memcpy2( void *dest, const void *src, int count );


/*#ifdef _WIN32_WCE
#	include "../../PocketHAL/PocketHAL/fastcopy.h"
#	define gedMemcpy PHAL::FastCopy //No changes in fps benchmark.ged
#else*/
#	define gedMemcpy memcpy
//#endif

//Causes misalignment on fps.ged on PocketPC
/*#define gedMemcpy(dst, src, s)\
{\
			U8* pDest8 = (U8*)(dst);\
			U8* pSrc8 = (U8*)(src);\
			int size = (s);\
\
			while(size && (((int)pDest8&0x3)!=0))\
			{\
				*pDest8++ = *pSrc8++;\
				size--;\
			}\
\
			U64* pDest = (U64*)pDest8;\
			U64* pSrc = (U64*)pSrc8;\
\
			while(size >= 16)\
			{\
				*pDest++ = *pSrc++;\
				*pDest++ = *pSrc++;\
				size -= 16;\
			}\
\
			while(size >= 8)\
			{\
				*pDest++ = *pSrc++;\
				size -= 8;\
			}\
\
			U32* pDest32 = (U32*)pDest;\
			U32* pSrc32 = (U32*)pSrc;\
			while(size >= 4)\
			{\
				*pDest32++ = *pSrc32++;\
				size -= 4;\
			}\
\
			U16* pDest16 = (U16*)pDest32;\
			U16* pSrc16 = (U16*)pSrc32;\
			while(size >= 2)\
			{\
				*pDest16++ = *pSrc16++;\
				size -= 2;\
			}\
\
			if(size&1)\
			{ \
				*(U8 *)pDest16 = *(U8 *)pSrc16;\
			}\
}*/

/*void gedMemcpy( void * dest, const void * src, size_t numBytes )
{
	//PocketPC exception: data alignment missaligned

	double *dp = (double *)dest;
	double *dq = (double *)src;
				
	while(numBytes >= 4*sizeof(double))
	{
		*dp++ = *dq++;
		*dp++ = *dq++;
		*dp++ = *dq++;
		*dp++ = *dq++;

		numBytes -= 4*sizeof(double);
	}
	
	while(numBytes >= sizeof(double))
	{
		*dp++ = *dq++;
		numBytes -= sizeof(double);
	}

	if(numBytes)
	{
		char *          pDest;
		const char *    pSrc;
		
		pDest = (char *)dp;
		pSrc  = (const char *)dq;

		while ( numBytes-- )
			*pDest++ = *pSrc++;
	}
}*/


//#define COUNT_PERFORMANCE
#define KYRA_ASM


SurfaceLock::SurfaceLock(KrPaintInfo *_painterInfo) //maks
{
	//No need lock in GP2X
	painterInfo = NULL;
	surface = NULL;

	if(_painterInfo && !(_painterInfo->surface->flags & SDL_OPENGL) && (_painterInfo->pixels == NULL || _painterInfo->surface->pixels == NULL))
	{
		if((_painterInfo->surface->flags & SDL_HWSURFACE) == SDL_HWSURFACE) 
		{
			SDL_LockSurface(_painterInfo->surface);
			
			painterInfo = _painterInfo;
			painterInfo->pixels = painterInfo->surface->pixels;
		}
	}
}

SurfaceLock::SurfaceLock(SDL_Surface *_surface) //maks
{
	painterInfo = NULL;
	surface = NULL;

	if(_surface && _surface->pixels == NULL && !(_surface->flags & SDL_OPENGL))
	{
		if((_surface->flags & SDL_HWSURFACE) == SDL_HWSURFACE) 
		{
			SDL_LockSurface(_surface);
			
			surface = _surface;
		}
	}
}

SurfaceLock::~SurfaceLock() //maks
{
	if(painterInfo)
	{
		SDL_UnlockSurface(painterInfo->surface);
		if(!painterInfo->surface->locked) painterInfo->pixels = NULL;

	}
	else if(surface)
	{
		SDL_UnlockSurface(surface);
	}
}


KrPainter::KrPainter( SDL_Surface* _surface ) 
	: surface( _surface )
{
	transparentColor = NULL;
}


KrPainter::KrPainter( KrPaintInfo* _info )
	: surface( _info->surface ), transparentColor(_info->transparentColor)
{
	if(_info->transparentColor)
	{
		transparentColor = new KrRGBA;
		*transparentColor = *_info->transparentColor;
	}
	else
		transparentColor = NULL;
}


KrPainter::~KrPainter()
{
// 	if ( mustDeletePaintInfo )
// 		delete paintInfo;
	
	if(transparentColor) delete transparentColor;
}

void KrPainter::SetPixel( int x, int y, const KrRGBA& color )
{
#ifdef KYRA_SUPPORT_OPENGL //maks
	if(surface->flags & SDL_OPENGL) 
	{
		//if(openGLZ < 0) openGLZ = engine->Tree()->TreeDepth();
		//int openGLZ = 1000;
		SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );
		
#ifndef GL_OES_VERSION_1_1
		
		SDL_CurrentVideo->glBegin(GL_POINTS);
		
		SDL_CurrentVideo->glColor3f(color.c.red/255.0, color.c.green/255.0, color.c.blue/255.0);
		
		SDL_CurrentVideo->glVertex2f((GLdouble) x, (GLdouble) y/*, openGLZ*/);
		
		SDL_CurrentVideo->glEnd();
		
#else
		GLshort vertices[] = 
		{
			x, y, //openGLZ,  
			x+1, y//, openGLZ
		};
		
		SDL_CurrentVideo->glColor4x(
			GlES_DoubleToFixed(color.Redf()), 
			GlES_DoubleToFixed(color.Greenf()), 
			GlES_DoubleToFixed(color.Bluef()), 
			GlES_Fixed_1);
		
// AKR		DrawOGLLine(vertices, 2);
#endif
		
	}
	else
#endif
	{
		
		U8* bits = (U8*) surface->pixels 
			+ y * surface->pitch
			+ x * surface->format->BytesPerPixel;
		
		SetPixel(	bits, 
			color.c.red, 
			color.c.green, 
			color.c.blue, 
			color.c.alpha );
	}
}

void KrPainter::SetPixel( void* bits, U8 red, U8 green, U8 blue, U8 alpha )
{
	if(surface->flags & SDL_OPENGL) return; //maks
	GLASSERT( surface->format->BytesPerPixel > 1 );



	U32 pixel = SDL_MapRGBA( surface->format, red, green, blue, alpha );
	switch( surface->format->BytesPerPixel ) 
	{
		case 2:
		{
			*((Uint16 *)(bits)) = (Uint16)pixel;
		}
		break;

		case 3: 
		{	
			*(( (U8*) bits)+surface->format->Rshift/8) = red; 
			*(( (U8*) bits)+surface->format->Gshift/8) = green;
			*(( (U8*) bits)+surface->format->Bshift/8) = blue;
		}
		break;

		case 4:
		{
			*((Uint32 *)(bits)) = (Uint32)pixel;
		}
		break;

		default:	
			GLASSERT( 0 );
	}
}


void KrPainter::BreakPixel( int x, int y, 
							U8* r, U8* g, U8* b, U8* a )
{
	GLASSERT( x >=0 && x < surface->w );
	GLASSERT( y >=0 && y < surface->h );



	U8* bits = ( (U8*) surface->pixels 
					 + y * surface->pitch
					 + x * surface->format->BytesPerPixel );

	U32 color;

	if ( surface->format->BytesPerPixel == 2 ) //maks
	{
		color = *((U16*) bits);
		SDL_GetRGBA( color, surface->format, r, g, b, a );

		//Set alpha chanel
		if( transparentColor &&
			*r == transparentColor->c.red &&
			*g == transparentColor->c.green &&
			*b == transparentColor->c.blue)
		{
			*a = KrRGBA::KR_TRANSPARENT;
		}

	}
	else if ( surface->format->BytesPerPixel == 3 ) //maks
	{
		*r = bits[ surface->format->Rshift / 8 ];
		*g = bits[ surface->format->Gshift / 8 ];
		*b = bits[ surface->format->Bshift / 8 ];

		//Set alpha chanel
		if( transparentColor &&
			*r == transparentColor->c.red &&
			*g == transparentColor->c.green &&
			*b == transparentColor->c.blue)
		{
			*a = KrRGBA::KR_TRANSPARENT;
		}
		else
		{
			*a = KrRGBA::KR_OPAQUE;
		}
	}
	else if ( surface->format->BytesPerPixel == 4 )
	{
		color = *((U32*) bits);
		SDL_GetRGBA( color, surface->format, r, g, b, a );
	}		
	else
	{
		GLASSERT( 0 );
	}
}

int KrPainter::CalcTransparentRun( int xmin, int xmax, int y )
{
	U8 r, g, b, a;
	int length = 0;
	int x = xmin;

	BreakPixel( x+0, y, &r, &g, &b, &a );

	while ( a == KrRGBA::KR_TRANSPARENT )
	{	
		length++;
		if ( x + length > xmax )
			break;

		BreakPixel( x+length, y, &r, &g, &b, &a );
	}
	return length;
}


int KrPainter::CalcTransparentColumn( int ymin, int ymax, int x )
{
	U8 r, g, b, a;
	int length = 0;
	int y = ymin;

	BreakPixel( x, y, &r, &g, &b, &a );

	while ( a == KrRGBA::KR_TRANSPARENT )
	{	
		++length;
		if ( y + length > ymax )
			break;

		BreakPixel( x, y+length, &r, &g, &b, &a );
	}
	return length;
}

#ifndef STAND_ALONE_GAME //maks
void KrPainter::DrawHLine(	int x, int y, int w, 
							U8 red, U8 green, U8 blue, int openGLZ ) 
{
#ifdef KYRA_SUPPORT_OPENGL //maks
	if(surface->flags & SDL_OPENGL) 
	{
		if(openGLZ < 0) openGLZ = engine->Tree()->TreeDepth();
		SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );

#ifndef GL_OES_VERSION_1_1

		SDL_CurrentVideo->glBegin(GL_LINES);

		SDL_CurrentVideo->glColor3f(red/255.0, green/255.0, blue/255.0);

		SDL_CurrentVideo->glVertex3f((GLdouble) x, (GLdouble) y, openGLZ);
		SDL_CurrentVideo->glVertex3f((GLdouble) x + w,(GLdouble) y, openGLZ);

		SDL_CurrentVideo->glEnd();

#else
		GLshort vertices[] = 
		{
			x, y, openGLZ,  
			x + w, y, openGLZ
		};

		SDL_CurrentVideo->glColor4x(
			GlES_DoubleToFixed(red/255.0), 
			GlES_DoubleToFixed(green/255.0), 
			GlES_DoubleToFixed(blue/255.0), 
			GlES_Fixed_1);
		
		DrawOGLLine(vertices, 3);
#endif

	}
	else
#endif
	{
		KrRect targetRect;
		targetRect.xmin = 0;
		targetRect.ymin = 0;
		targetRect.xmax = surface->w  - 1;
		targetRect.ymax = surface->h - 1;
		
		KrRect lineRect;
		lineRect.xmin = x;
		lineRect.ymin = y;
		lineRect.xmax = x + w - 1;
		lineRect.ymax = y;
		
		if ( targetRect.Intersect( lineRect ) )
		{
			lineRect.DoIntersection( targetRect );
			w = lineRect.xmax - lineRect.xmin + 1;
			
			U8* dst;
			int nPixel;
			
			
			
			// Draw the top and bottom h-line
			dst = (U8*) surface->pixels
				+ surface->pitch*lineRect.ymin
				+ surface->format->BytesPerPixel*lineRect.xmin;
			
			for (	nPixel = 0;
			nPixel < w;
			nPixel++ )
			{
				SetPixel( dst, red, green, blue, KrRGBA::KR_OPAQUE );
				dst += surface->format->BytesPerPixel;
			}
		}
	}
}


void KrPainter::DrawHLine(	int x, int y, int w, 
							const KrRGBA* colors, int nColors ) 
{
	if(surface->flags & SDL_OPENGL) return; //maks
	
	KrRect targetRect;
	targetRect.xmin = 0;
	targetRect.ymin = 0;
	targetRect.xmax = surface->w - 1;
	targetRect.ymax = surface->h - 1;

	KrRect lineRect;
	lineRect.xmin = x;
	lineRect.ymin = y;
	lineRect.xmax = x + w - 1;
	lineRect.ymax = y;

	if ( targetRect.Intersect( lineRect ) )
	{
		lineRect.DoIntersection( targetRect );
		w = lineRect.xmax - lineRect.xmin + 1;

		U8* dst;
		int nPixel;



		// Draw the top and bottom h-line
		dst = (U8*)   surface->pixels
		            + surface->pitch * lineRect.ymin 
					+ surface->format->BytesPerPixel * lineRect.xmin;

		for (	nPixel = 0;
				nPixel < w;
				nPixel++ )
		{
			int index = ( lineRect.xmin + nPixel + lineRect.ymin ) % nColors;
			SetPixel(	dst, 
						colors[index].c.red, 
						colors[index].c.green, 
						colors[index].c.blue, 
						colors[index].c.alpha );

			dst += surface->format->BytesPerPixel;
		}
	}
}


void KrPainter::DrawVLine(	int x, int y, int h, 
							U8 red, U8 green, U8 blue, int openGLZ ) 
{
#ifdef KYRA_SUPPORT_OPENGL //maks
	if(surface->flags & SDL_OPENGL) 
	{
		if(openGLZ < 0) openGLZ = engine->Tree()->TreeDepth();
		SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );

#ifndef GL_OES_VERSION_1_1

		SDL_CurrentVideo->glBegin(GL_LINES);
		
		SDL_CurrentVideo->glColor3f(red/255.0, green/255.0, blue/255.0);
		
		SDL_CurrentVideo->glVertex3f((GLdouble) x, (GLdouble) y, openGLZ);
		SDL_CurrentVideo->glVertex3f((GLdouble) x,(GLdouble) y + h, openGLZ);
		
		SDL_CurrentVideo->glEnd();

#else
		GLshort vertices[] = 
		{
			x, y, openGLZ,  
			x, y + h, openGLZ
		};

		SDL_CurrentVideo->glColor4x(
			GlES_DoubleToFixed(red/255.0), 
			GlES_DoubleToFixed(green/255.0), 
			GlES_DoubleToFixed(blue/255.0), 
			GlES_Fixed_1);
		
		DrawOGLLine(vertices, 3);
#endif
	}
	else
#endif
	{		
		KrRect targetRect;
		targetRect.xmin = 0;
		targetRect.ymin = 0;
		targetRect.xmax = surface->w - 1;
		targetRect.ymax = surface->h - 1;
		
		KrRect lineRect;
		lineRect.xmin = x;
		lineRect.ymin = y;
		lineRect.xmax = x;
		lineRect.ymax = y + h - 1;
		
		if ( targetRect.Intersect( lineRect ) )
		{
			lineRect.DoIntersection( targetRect );
			h = lineRect.ymax - lineRect.ymin + 1;
			
			U8* dst;
			int nPixel;
			
			
			
			// Draw the top and bottom h-line
			dst = (U8*)   surface->pixels
				+ surface->pitch*lineRect.ymin 
				+ surface->format->BytesPerPixel*lineRect.xmin;
			
			for (	nPixel = 0;
			nPixel < h;
			nPixel++ )
			{
				SetPixel( dst, red, green, blue, KrRGBA::KR_OPAQUE );
				dst += surface->pitch;
			}
		}
	}
}


void KrPainter::DrawVLine(	int x, int y, int h, 
							const KrRGBA* colors, int nColors ) 
{
	if(surface->flags & SDL_OPENGL) return; //maks
	
	KrRect targetRect;
	targetRect.xmin = 0;
	targetRect.ymin = 0;
	targetRect.xmax = surface->w - 1;
	targetRect.ymax = surface->h - 1;

	KrRect lineRect;
	lineRect.xmin = x;
	lineRect.ymin = y;
	lineRect.xmax = x;
	lineRect.ymax = y + h - 1;

	if ( targetRect.Intersect( lineRect ) )
	{
		lineRect.DoIntersection( targetRect );
		h = lineRect.ymax - lineRect.ymin + 1;

		U8* dst;
		int nPixel;



		// Draw the top and bottom h-line
		dst = (U8*)   surface->pixels 
		            + surface->pitch         * lineRect.ymin 
					+ surface->format->BytesPerPixel * lineRect.xmin;
					
		for (	nPixel = 0;
				nPixel < h;
				nPixel++ )
		{
			int index = ( lineRect.xmin + nPixel + lineRect.ymin ) % nColors;
			SetPixel(	dst, 
						colors[index].c.red, 
						colors[index].c.green, 
						colors[index].c.blue, 
						colors[index].c.alpha );
			dst += surface->pitch;
		}
	}
}


void KrPainter::DrawBox(	int x, int y, int w, int h,
							U8 red, U8 green, U8 blue )
{
	DrawHLine( x, y, w, red, green, blue );
	DrawHLine( x, y+h-1, w, red, green, blue );
	DrawVLine( x, y, h, red, green, blue );
	DrawVLine( x+w-1, y, h, red, green, blue );
}


void KrPainter::DrawBox(	int x, int y, int w, int h,
							const KrRGBA* colors, int nColors )
{
	if(surface->flags & SDL_OPENGL) return; //maks

	DrawHLine( x, y, w, colors, nColors );
	DrawHLine( x, y+h-1, w, colors, nColors );
	DrawVLine( x, y, h, colors, nColors );
	DrawVLine( x+w-1, y, h, colors, nColors );
}


void KrPainter::DrawFilledBox(	int x, int y, int w, int h,
								U8 red, U8 green, U8 blue, int openGLZ ) //maks
{	
	KrRect rect;
	rect.Set( x, y, x+w-1, y+h-1 );
		
	KrRect screenBound;
	screenBound.Set( 0, 0, surface->w - 1, surface->h - 1 );
		
	rect.DoIntersection( screenBound );

#ifdef KYRA_SUPPORT_OPENGL //maks
	if(surface->flags & SDL_OPENGL) 
	{	
		if(openGLZ < 0) openGLZ = engine->Tree()->TreeDepth();
		SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );

#ifndef GL_OES_VERSION_1_1

		SDL_CurrentVideo->glBegin(GL_POLYGON);

		SDL_CurrentVideo->glColor3f(red/255.0, green/255.0, blue/255.0);		
		
		SDL_CurrentVideo->glVertex3f (  rect.xmin, rect.ymin, openGLZ );
		SDL_CurrentVideo->glVertex3f (  rect.xmax, rect.ymin, openGLZ);
		SDL_CurrentVideo->glVertex3f (  rect.xmax, rect.ymax, openGLZ);
		SDL_CurrentVideo->glVertex3f (  rect.xmin, rect.ymax, openGLZ);

		SDL_CurrentVideo->glEnd();

#else
		GLshort vertices[] = 
		{
			rect.xmin, rect.ymin, openGLZ,
			rect.xmax, rect.ymin, openGLZ,
			rect.xmax, rect.ymax, openGLZ,
			rect.xmin, rect.ymax, openGLZ				
		};

		SDL_CurrentVideo->glColor4x(
			GlES_DoubleToFixed(red/255.0), 
			GlES_DoubleToFixed(green/255.0), 
			GlES_DoubleToFixed(blue/255.0), 
			GlES_Fixed_1);	
		
		DrawOGLQuad(GL_TRIANGLE_STRIP, vertices, 3);
#endif
	}
	else
#endif
	{		
		for( y=rect.ymin; y<=rect.ymax; ++y )
		{
			U8* target = (U8*) surface->pixels + y * surface->pitch + rect.xmin * surface->format->BytesPerPixel;
			
			for( x=rect.xmin; x<=rect.xmax; ++x )
			{
				SetPixel( target, red, green, blue, 255 );
				target += surface->format->BytesPerPixel;
			}
		}
	}
}


// -------------- Surface -----------------------









void KrPainter::CalcNumberOfTiles(int &nHorizontal, int &nVertical) //maks
{
	//Find the tile number in a image with tiles separeted by transparent color

	bool bInBlock = false;
	nHorizontal = nVertical = 0;


	//Calculate number of columns
	for(int i = 0; i < surface->w; i++)
	{
		if(CalcTransparentColumn(0, surface->h - 1, i) != surface->h)
		{
			if(!bInBlock)
			{
				nHorizontal++;
				bInBlock = true;
			}
		}
		else
		{
			bInBlock = false;
		}
	}

	bInBlock = false;

	//Calculate number of lines
	for(int j = 0; j < surface->h; j++)
	{
		if(CalcTransparentRun(0, surface->w - 1, j) != surface->w)
		{
			if(!bInBlock)
			{
				nVertical++;
				bInBlock = true;
			}
		}
		else
		{
			bInBlock = false;
		}
	}
}

int KrPainter::CalcNotTransparentRun( int xmin, int xmax, int y )
{
	U8 r, g, b, a;
	int length = 0;
	int x = xmin;

	BreakPixel( x+0, y, &r, &g, &b, &a );

	while ( a != KrRGBA::KR_TRANSPARENT )
	{	
		length++;
		if ( x + length > xmax )
			break;

		BreakPixel( x+length, y, &r, &g, &b, &a );
	}
	return length;
}


int KrPainter::CalcOpaqueRun( int xmin, int xmax, int y )
{
	U8 r, g, b, a;
	int length = 0;
	int x = xmin;

	BreakPixel( x+0, y, &r, &g, &b, &a );

	while ( a == KrRGBA::KR_OPAQUE )
	{	
		length++;
		if ( x + length > xmax )
			break;

		BreakPixel( x+length, y, &r, &g, &b, &a );
	}
	return length;
}


int KrPainter::CalcTranslucentRun( int xmin, int xmax, int y )
{
	U8 r, g, b, a;
	int length = 0;
	int x = xmin;

	BreakPixel( x+0, y, &r, &g, &b, &a );

	while ( a != KrRGBA::KR_OPAQUE && a != KrRGBA::KR_TRANSPARENT )
	{	
		length++;
		if ( x + length > xmax )
			break;

		BreakPixel( x+length, y, &r, &g, &b, &a );
	}
	return length;
}


int KrPainter::FindPixel( int x, int y, int dx, int dy, KrRGBA color, bool useAlpha, bool invert )
{
	int count = 0;
	if ( !useAlpha ) color.c.alpha = 0;

	while( x >= 0 && x < surface->w && y >= 0 && y < surface->h )
	{
		KrRGBA rgba;
		BreakPixel( x, y, &rgba );
		if ( !useAlpha ) rgba.c.alpha = 0;

		if ( !invert )
		{
			if ( rgba == color )
				return count;
		}
		else
		{
			if ( rgba != color )
				return count;
		}
		
		x += dx;
		y += dy;
		++count;
	}
	return -1;
}
#endif //maks

// void KrPainter::FillSurface( int red, int green, int blue )
// {
// 	U8* bits = (U8*) paintInfo->pixels 
// 				+ y * paintInfo->pitch
// 				+ x * paintInfo->format->BytesPerPixel;
// 
// 	GLASSERT( paintInfo->bytesPerPixel > 1 );
// 
// 	switch( surface->format->BytesPerPixel ) 
// 	{	
// 		case 2:
// 			for ( int j=0; j<paintInfo->height; j++ )
// 			{
// 				KrPaint24_Simple_NoAlpha( paintInfo, bits, KrRGBA* source, int nPixel, const KrColorTransform& cform )
// }

//#ifndef _WIN32_WCE //maks
KrPaintInfo::KrPaintInfo( KrRGBA* memory, int _width, int _height )
{
	// Make sure the KrRGBA are okay:
	GLASSERT( sizeof( KrRGBA ) == 4 );

	needToFreeSurface = true;
	openGL = false;

	int redShift   = 8 * ( ( (U8*) &memory->c.red )   - ( (U8*) memory ) );
	int greenShift = 8 * ( ( (U8*) &memory->c.green ) - ( (U8*) memory ) );
	int blueShift  = 8 * ( ( (U8*) &memory->c.blue )  - ( (U8*) memory ) );
	int alphaShift = 8 * ( ( (U8*) &memory->c.alpha ) - ( (U8*) memory ) );

	int redMask    = 0xff << redShift;
	int greenMask  = 0xff << greenShift;
	int blueMask   = 0xff << blueShift;
	int alphaMask  = 0xff << alphaShift;

	surface = SDL_CreateRGBSurfaceFrom( memory,
										_width, _height, 32,
										4 * _width,		// pitch
										redMask,
										greenMask,
										blueMask,
										alphaMask );
	
	// Can always us the fast blitter: we're copying form
	// RGBAs.
	Paint_Simple_NoAlpha = KrPaintRGBA_Simple_NoAlpha;
	Paint_Alpha_NoAlpha  = KrPaintRGBA_Alpha_NoAlpha;
	Paint_Color_NoAlpha  = KrPaintRGBA_Color_NoAlpha;
	Paint_Full_NoAlpha   = KrPaintRGBA_Full_NoAlpha;

	Paint_Simple_Alpha   = KrPaintRGBA_Simple_Alpha;
	Paint_Alpha_Alpha    = KrPaintRGBA_Alpha_Alpha;
	Paint_Color_Alpha    = KrPaintRGBA_Color_Alpha;
	Paint_Full_Alpha     = KrPaintRGBA_Full_Alpha;

	PaintRotated_Simple_NoAlpha = KrPaintRGBARotated_Simple_NoAlpha;

	InitCopies();
}
//#endif //#ifndef _WIN32_WCE //maks


KrPaintInfo::KrPaintInfo( SDL_Surface* screen, bool bForceRGBA ) //maks
{
	Init(screen, bForceRGBA); //maks
}


void KrPaintInfo::Init( SDL_Surface* screen, bool bForceRGBA ) //maks
{
	Paint_Simple_NoAlpha = 0;
	Paint_Color_NoAlpha  = 0;
	Paint_Alpha_NoAlpha  = 0;
	Paint_Full_NoAlpha   = 0;

	Paint_Simple_Alpha   = 0;
	Paint_Color_Alpha    = 0;
	Paint_Alpha_Alpha    = 0;
	Paint_Full_Alpha     = 0;

	GLASSERT( screen );

	surface = screen;
	needToFreeSurface	= false;
	openGL				= ( screen->flags & SDL_OPENGL ); //AKR
	InitCopies();

#ifndef _WIN32_WCE //maks
	if ( bytesPerPixel == 4 )
	{
		// Check for fast 32 bit blit.
		KrRGBA memory;
		int redShiftRgb   = 8 * ( ( (U8*) &memory.c.red )   - ( (U8*) &memory ) );
		int greenShiftRgb = 8 * ( ( (U8*) &memory.c.green ) - ( (U8*) &memory ) );
		int blueShiftRgb  = 8 * ( ( (U8*) &memory.c.blue )  - ( (U8*) &memory ) );

		if ( redShiftRgb == redShift && blueShiftRgb == blueShift && greenShiftRgb == greenShift )
		{
			Paint_Simple_NoAlpha = KrPaint32B_Simple_NoAlpha;
			#ifdef DEBUG
				GLOUTPUT( "Fast 32 bit blitter.\n" );	
			#endif
		}
		else
		{
			Paint_Simple_NoAlpha = KrPaint32_Simple_NoAlpha;
			#ifdef DEBUG
				GLOUTPUT( "Standard 32 bit blitter.\n" );
			#endif
		}

		if(bForceRGBA) //maks
		{
			// Can always us the fast blitter: we're copying form
			// RGBAs.
			Paint_Simple_NoAlpha = KrPaintRGBA_Simple_NoAlpha;
			Paint_Alpha_NoAlpha  = KrPaintRGBA_Alpha_NoAlpha;
			Paint_Color_NoAlpha  = KrPaintRGBA_Color_NoAlpha;
			Paint_Full_NoAlpha   = KrPaintRGBA_Full_NoAlpha;
			
			Paint_Simple_Alpha   = KrPaintRGBA_Simple_Alpha;
			Paint_Alpha_Alpha    = KrPaintRGBA_Alpha_Alpha;
			Paint_Color_Alpha    = KrPaintRGBA_Color_Alpha;
			Paint_Full_Alpha     = KrPaintRGBA_Full_Alpha;
			
			PaintRotated_Simple_NoAlpha = KrPaintRGBARotated_Simple_NoAlpha;
		}
		else
		{
			Paint_Alpha_NoAlpha  = KrPaint32_Alpha_NoAlpha;
			Paint_Color_NoAlpha  = KrPaint32_Color_NoAlpha;
			Paint_Full_NoAlpha   = KrPaint32_Full_NoAlpha;
			
			Paint_Simple_Alpha   = KrPaint32_Simple_Alpha;
			Paint_Alpha_Alpha    = KrPaint32_Alpha_Alpha;
			Paint_Color_Alpha    = KrPaint32_Color_Alpha;
			Paint_Full_Alpha     = KrPaint32_Full_Alpha;
			
			PaintRotated_Simple_NoAlpha = KrPaint32Rotated_Simple_NoAlpha;
		}
	}
	else if ( bytesPerPixel == 3 )
	{
 		GLASSERT( screen->format->Rshift % 8 == 0 );
		GLASSERT( screen->format->Gshift % 8 == 0 );
		GLASSERT( screen->format->Bshift % 8 == 0 );

		Paint_Simple_NoAlpha = KrPaint24_Simple_NoAlpha;
		Paint_Alpha_NoAlpha  = KrPaint24_Alpha_NoAlpha;
		Paint_Color_NoAlpha  = KrPaint24_Color_NoAlpha;
		Paint_Full_NoAlpha   = KrPaint24_Full_NoAlpha;

		Paint_Simple_Alpha   = KrPaint24_Simple_Alpha;
		Paint_Alpha_Alpha    = KrPaint24_Alpha_Alpha;
		Paint_Color_Alpha    = KrPaint24_Color_Alpha;
		Paint_Full_Alpha     = KrPaint24_Full_Alpha;

		PaintRotated_Simple_NoAlpha = KrPaint24Rotated_Simple_NoAlpha;
	}		
	else if ( bytesPerPixel == 2 )
#endif //#ifndef _WIN32_WCE //maks
	{
// 		GLOUTPUT( "Setting up basic 16 bit blitter.\n" );

		Paint_Simple_NoAlpha = KrPaint16_Simple_NoAlpha;
		Paint_Alpha_NoAlpha  = KrPaint16_Alpha_NoAlpha;
		Paint_Color_NoAlpha  = KrPaint16_Color_NoAlpha;
		Paint_Full_NoAlpha   = KrPaint16_Full_NoAlpha;

		Paint_Simple_Alpha   = KrPaint16_Simple_Alpha;
		Paint_Alpha_Alpha    = KrPaint16_Alpha_Alpha;
		Paint_Color_Alpha    = KrPaint16_Color_Alpha;
		Paint_Full_Alpha     = KrPaint16_Full_Alpha;

		PaintRotated_Simple_NoAlpha = KrPaint16Rotated_Simple_NoAlpha;
	}		
	
}




void KrPaintInfo::InitCopies()
{
	redShift   = surface->format->Rshift;
	greenShift = surface->format->Gshift;
	blueShift  = surface->format->Bshift;
	alphaShift = surface->format->Ashift;
	redMask    = surface->format->Rmask;
	greenMask  = surface->format->Gmask;
	blueMask   = surface->format->Bmask;
	alphaMask  = surface->format->Amask;
	redByte	   = surface->format->Rshift / 8;
	blueByte   = surface->format->Bshift / 8;
	greenByte  = surface->format->Gshift / 8;
	redLoss	   = surface->format->Rloss;
	greenLoss  = surface->format->Gloss;
	blueLoss   = surface->format->Bloss;
	width	   = surface->w;
	height     = surface->h;
	pitch      = surface->pitch;
	bytesPerPixel = surface->format->BytesPerPixel;
	pixels     = surface->pixels;

	transparentColor = NULL; //maks
}


KrPaintFunc KrPaintInfo::GetBlitter( bool alpha, const KrColorTransform& cform )
{
	if ( alpha )
		if ( cform.IsIdentity() )
			return Paint_Simple_Alpha;
		else if ( cform.HasAlpha() )
			if ( cform.HasColor() )
				return Paint_Full_Alpha;
			else 
				return Paint_Alpha_Alpha;
		else
			if ( cform.HasColor() )
				return Paint_Color_Alpha;
			else
			{
				GLASSERT( 0 );
				return 0;
			}
	else
		if ( cform.IsIdentity() )
			return Paint_Simple_NoAlpha;
		else if ( cform.HasAlpha() )
			if ( cform.HasColor() )
				return Paint_Full_NoAlpha;
			else
				return Paint_Alpha_NoAlpha;
		else
			if ( cform.HasColor() )
				return Paint_Color_NoAlpha;
			else
			{
				GLASSERT( 0 );
				return 0;
			}			
}


#ifdef KYRA_SUPPORT_OPENGL

void KrPaintInfo::SetOpenGLTextureMode( bool /*sourceAlpha*/, 
										const KrColorTransform cform,
										bool isScaled,
										KrTexture* texture )
{
	#ifdef DEBUG
		if ( cform.b.c.red == 0 && cform.b.c.green == 255 && cform.b.c.blue == 0 )
			int breakPoint = 0;
	#endif

	// Only use linear scaling if we need it. If we don't, nearest neighbor
	// looks better and is faster too.
	//

#ifndef GL_OES_VERSION_1_1 //maks
	if ( isScaled || texture->Scale() )
	{
		SDL_CurrentVideo->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		SDL_CurrentVideo->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	}
	else
	{ 
		SDL_CurrentVideo->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		SDL_CurrentVideo->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
#else
	if ( isScaled || texture->Scale() )
	{
		SDL_CurrentVideo->glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		SDL_CurrentVideo->glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_TEXTURE_MAG_FILTER is not supported in the Intel 2700G
	}
	else
	{ 
		SDL_CurrentVideo->glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		SDL_CurrentVideo->glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_TEXTURE_MAG_FILTER is not supported in the Intel 2700G
	}
#endif

/*#ifndef GL_OES_VERSION_1_1 //maks
	//Don't works in some cards
	SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#else*/

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
	SDL_CurrentVideo->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	SDL_CurrentVideo->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

#ifndef GL_OES_VERSION_1_1
	if(SDL_CurrentVideo->glGetError() != GL_NO_ERROR)
	{
		SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		SDL_CurrentVideo->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
#endif

//#endif

	//	cn	color of pixel in sprite or canvas
	//	c0  color on framebuffer
	//  c'	new color
	//
	//	an	alpha of pixel in sprite or canvas
	//	at	alpha of the transform
	//	
	//	m (multiplier) b (adder) color transforms
	//
	//	c' = cn an at m + an at b + ( 1 - an at ) c0
	//

#ifndef GL_OES_VERSION_1_1 //maks
	GLfloat environmentColor[4] = { 0.0, 0.0, 0.0, 1.0 };
	SDL_CurrentVideo->glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, environmentColor );
#else
	GLfixed environmentColor[4] = { 0, 0, 0, GlES_Fixed_1 };
	SDL_CurrentVideo->glTexEnvxv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, environmentColor );
#endif

	// There may be alpha in the individual pixels.
	// There isn't alpha in the transformation.
	if ( cform.IsIdentity() )
	{
		// c' = c
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		SDL_CurrentVideo->glColor4f( 1.0, 1.0, 1.0, 1.0 );
#else
		SDL_CurrentVideo->glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		SDL_CurrentVideo->glColor4x( GlES_Fixed_1, GlES_Fixed_1, GlES_Fixed_1, GlES_Fixed_1 );
#endif

		
	}
	else
	{
		if (	cform.b.c.red == 0
			 && cform.b.c.green == 0
			 && cform.b.c.blue == 0 )
		{
			// We can do a perfect transformation.
			// full noAlpha:	c' = a cn m + a b + (1-a) c0
			// where b == 0
			
			
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		SDL_CurrentVideo->glColor4f(	
						cform.m.Redf(),	//  + ( cform.b.Greenf() + cform.b.Bluef() ) / 2.0f, 
						cform.m.Greenf(), //+ ( cform.b.Redf()   + cform.b.Bluef() ) / 2.0f,
						cform.m.Bluef(), // + ( cform.b.Greenf() + cform.b.Redf() ) / 2.0f,
						cform.b.Alphaf() );
#else
		SDL_CurrentVideo->glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		SDL_CurrentVideo->glColor4x(	
						GlES_DoubleToFixed(cform.m.Redf()),	//  + ( cform.b.Greenf() + cform.b.Bluef() ) / 2.0f, 
						GlES_DoubleToFixed(cform.m.Greenf()), //+ ( cform.b.Redf()   + cform.b.Bluef() ) / 2.0f,
						GlES_DoubleToFixed(cform.m.Bluef()), // + ( cform.b.Greenf() + cform.b.Redf() ) / 2.0f,
						GlES_DoubleToFixed(cform.b.Alphaf()) );
#endif

			
		}
		else
		{
			/*
				Color systems, with alpha channel removed. (It works out, and just adds terms.)

				OpenGL
				Color[value] =  Color[poly] * ( 1 - Color[tex] ) + Color[env] * Color[tex]

						or		Color[tex] ( Color[env] - Color[poly] ) + Color[poly]

				Kyra
				Color[value] =    Color[texture] * m + b

				m = Color[env] - Color[poly]
				b = Color[poly]

				Color[poly] = b
				Color[env]  = m + b
			*/

			// The fragment color. (b)

			
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );	

		SDL_CurrentVideo->glColor4f(	
						cform.b.Redf(),
						cform.b.Greenf(),
						cform.b.Bluef(),
						cform.b.Alphaf() );

			environmentColor[ 0 ] = cform.m.Redf()   + cform.b.Redf();
			environmentColor[ 1 ] = cform.m.Greenf() + cform.b.Greenf();
			environmentColor[ 2 ] = cform.m.Bluef()  + cform.b.Bluef();
			environmentColor[ 3 ] = 1.0;

			SDL_CurrentVideo->glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, environmentColor );
#else
		SDL_CurrentVideo->glTexEnvx( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );	

		SDL_CurrentVideo->glColor4x(	
						GlES_DoubleToFixed(cform.b.Redf()),
						GlES_DoubleToFixed(cform.b.Greenf()),
						GlES_DoubleToFixed(cform.b.Bluef()),
						GlES_DoubleToFixed(cform.b.Alphaf()) );

		environmentColor[ 0 ] = GlES_DoubleToFixed(cform.m.Redf()   + cform.b.Redf());
			environmentColor[ 1 ] = GlES_DoubleToFixed(cform.m.Greenf() + cform.b.Greenf());
			environmentColor[ 2 ] = GlES_DoubleToFixed(cform.m.Bluef()  + cform.b.Bluef());
			environmentColor[ 3 ] = GlES_Fixed_1;

			SDL_CurrentVideo->glTexEnvxv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, environmentColor );
#endif

			

			// Use the environment color (Cc) to get the (m) term
			
			GLASSERT( SDL_CurrentVideo->glGetError() == GL_NO_ERROR );

			/*#ifdef DEBUG
				GLint mode = -1;
				glGetTexEnviv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mode );
				GLASSERT( mode == GL_BLEND );

				float test[4];
				glGetTexEnvfv(  GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, test );
				GLASSERT( test[0] == environmentColor[ 0 ] );
				GLASSERT( test[1] == environmentColor[ 1 ] );
				GLASSERT( test[2] == environmentColor[ 2 ] );
				GLASSERT( test[3] == environmentColor[ 3 ] );

			#endif*/
		}
	}
	GLASSERT( SDL_CurrentVideo->glGetError() == GL_NO_ERROR );
}

#endif

#ifdef _MSC_VER
// Tell the compiler that no aliasing is used.
#pragma optimize ( "w", on )
#endif






// ---------------- 32 Bit Blt Routines -------------------------//

// Utility inlines and macros.

// Calculate the color transform.
// params:
//		val		original color value
//		ch		channel( red, ... )
// uses:
//		cform	the color transform
//

#define CFORM( val, ch ) ((( val*cform.m.c.ch ) >> 8 ) + cform.b.c.ch )

inline U8 CFORM_I( U8 val, int ch, const KrColorTransform& cform )
{
	return ( ( ( val*cform.m.array[ch] ) >> 8 ) + cform.b.array[ch] );
}

// Caluclate the color using alpha.
// params:
//		ch		channel (red, green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U32* to the target pixel
//		info	KrPaintInfo

#define ALPHA_CH_32( ch, alpha )	(((( alpha * source->c.ch )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift ) ) ) >> 8 )
#define ALPHA_CH_32_VAL( val, ch, alpha )	(((( alpha * val )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift ) ) ) >> 8 ) //maks
#define ALPHA_CH_RGBA( ch, alpha )	(((( alpha * source->c.ch )) + (( 255-alpha ) * (( target->c.ch ) >> info->ch##Shift ) ) ) >> 8 )
#define ALPHA_CH_RGBA_VAL(val, ch, alpha )	(((( alpha * val )) + (( 255-alpha ) * (( target->c.ch ) >> info->ch##Shift ) ) ) >> 8 ) //maks


// Calculate the color using full.
// params:
//		ch		channel (red, green...)
//		Ch		channel (Red, Green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U32* to the target pixel
//		info	KrPaintInfo

#define FULL_CH_32( ch, alpha )	(((( alpha * CFORM( source->c.ch, ch ) )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift ) ) ) >> 8 )
#define FULL_CH_32_VAL(val,  ch, alpha )	(((( alpha * CFORM( val, ch ) )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift ) ) ) >> 8 ) //maks
#define FULL_CH_RGBA( ch, alpha )	(((( alpha * CFORM( source->c.ch, ch ) )) + (( 255-alpha ) * (( target->c.ch ) >> info->ch##Shift ) ) ) >> 8 )
#define FULL_CH_RGBA_VAL(val, ch, alpha )	(((( alpha * CFORM( val, ch ) )) + (( 255-alpha ) * (( target->c.ch ) >> info->ch##Shift ) ) ) >> 8 ) //maks

//#ifndef _WIN32_WCE //maks
void KrPaint32_Simple_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target =   ( source->c.red   << info->redShift ) 
					+ ( source->c.green << info->greenShift ) 
					+ ( source->c.blue  << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( source[2]   << info->redShift ) 
					+ ( source[1] << info->greenShift ) 
					+ ( source[0]  << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( GET_RED_16(source)   << info->redShift ) 
					+ ( GET_GREEN_16(source) << info->greenShift ) 
					+ ( GET_BLUE_16(source)  << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32B_Simple_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32B_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	GLASSERT( ( unsigned( _target ) & 3 ) == 0 );
	//GLASSERT( ( unsigned( source ) & 3 ) == 0 );
	

	switch(sourceBpp) //maks
	{
	case 4:
		gedMemcpy( _target, _source, nPixel << 2 );
		break;
	case 3:
		{
			U32* target = (U32*) _target;
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( source[2]   << info->redShift ) 
					+ ( source[1] << info->greenShift ) 
					+ ( source[0]  << info->blueShift );
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U32* target = (U32*) _target;
			U16 *source = (U16 *)_source;
			
			while ( nPixel )
			{	
				//227 cycles in 1945.ged
				//*target =   ( GET_RED_16(source)   << info->redShift ) 
				//	+ ( GET_GREEN_16(source) << info->greenShift ) 
				//	+ ( GET_BLUE_16(source)  << info->blueShift );

				//maks: 16 -> RGB more fast (168 cycles in 1945.ged)
				*target = (U32)*source;
				*target = (((*target&0xf800)<<8)|((*target&0x7e0)<<5)|((*target&0x1f)<<3)) | 0x030103;

				//maks: no diferences in velocity
				//*target = ((*source >> 11) << 19) +
				//		  (((*source & 2016) >> 5) << 10) +
				//		  ((*source & 31) << 3);

				nPixel--;
				target++;
				source++;
			}

			//maks: no diferences in velocity
			//If the current pixel isn't dword aligned, try write one pixel first
			/*if((int)target&0x3)
			{ 
				*target = (U32)*source;
				*target = (((*target&0xf800)<<8)|((*target&0x7e0)<<5)|((*target&0x1f)<<3)) | 0x030103;

				nPixel--;
				target++;
				source++;
			}

			// Write blocks of two pixels

			for(int i=nPixel>>1;i;i--)
			{ 
				*target = (U32)*source;
				*target = (((*target&0xf800)<<8)|((*target&0x7e0)<<5)|((*target&0x1f)<<3)) | 0x030103;

				target++;
				source++;

				*target = (U32)*source;
				*target = (((*target&0xf800)<<8)|((*target&0x7e0)<<5)|((*target&0x1f)<<3)) | 0x030103;

				target++;
				source++;
			}

			
			// Eventually, write a single odd pixel that might be left 
			if(nPixel&1)
			{ 
				*target = (U32)*source;
				*target = (((*target&0xf800)<<8)|((*target&0x7e0)<<5)|((*target&0x1f)<<3)) | 0x030103;
			}*/
		}
		break;
	}

}


void KrPaint32_Color_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Color_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				// Works, but not speed tested:
				//*target =   ( CFORM_I( source->c.red, KrRGBA::RED, cform )     << info->redShift ) 
				//		  | ( CFORM_I( source->c.green, KrRGBA::GREEN, cform ) << info->greenShift ) 
				//		  | ( CFORM_I( source->c.blue, KrRGBA::BLUE, cform )   << info->blueShift );
				
				*target =   ( CFORM( source->c.red, red )     << info->redShift ) 
					| ( CFORM( source->c.green, green ) << info->greenShift ) 
					| ( CFORM( source->c.blue, blue )   << info->blueShift );
				--nPixel;
				++target;
				++source;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( CFORM( source[2], red )     << info->redShift ) 
					| ( CFORM( source[1], green ) << info->greenShift ) 
					| ( CFORM( source[0], blue )   << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( CFORM( GET_RED_16(source), red )     << info->redShift ) 
					| ( CFORM( GET_GREEN_16(source), green ) << info->greenShift ) 
					| ( CFORM( GET_BLUE_16(source), blue )   << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32_Alpha_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Alpha_NoAlpha" );
	#endif 

	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	
	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				* target =    ( ALPHA_CH_32( red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32( green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32( blue,  cform.b.c.alpha ) << info->blueShift );
				
				--nPixel;
				++target;
				++source;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( ALPHA_CH_32_VAL( source[2], red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( source[1], green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( source[0], blue,  cform.b.c.alpha ) << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha ) << info->blueShift );

				nPixel--;
				target++;
				source++;
			}

			//maks
			/*U16 *source = (U16 *)_source;
			U32 s_pixel;

			//If the current pixel isn't dword aligned, try write one pixel first
			if((int)target&0x3)
			{ 
				//16 -> RGB
				//s_pixel = (U32)*source;
				//s_pixel = (((s_pixel&0xf800)<<8)|((s_pixel&0x7e0)<<5)|((s_pixel&0x1f)<<3)) | 0x030103;
				

				*target =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha ) << info->blueShift );

				nPixel--;
				target++;
				source++;
			}

			// Write blocks of two pixels

			for(int i=nPixel>>1;i;i--)
			{ 
				*target++ =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha ) << info->blueShift );

				
				source++;

				*target++ =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha ) << info->blueShift );

				
				source++;
			}

			
			// Eventually, write a single odd pixel that might be left 
			if(nPixel&1)
			{ 
				*target =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha ) << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha ) << info->blueShift );
			}*/
		}
		break;
	}
}


void KrPaint32_Full_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Full_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				// 		U8 alpha = ( cform.Alpha() * source->alpha ) >> 8;
				
				* target =    ( FULL_CH_32( red,   cform.b.c.alpha   )  << info->redShift )
					| ( FULL_CH_32( green, cform.b.c.alpha )  << info->greenShift )
					| ( FULL_CH_32( blue,  cform.b.c.alpha   ) << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				* target =    ( FULL_CH_32_VAL( source[2], red,   cform.b.c.alpha   )  << info->redShift )
					| ( FULL_CH_32_VAL( source[1], green, cform.b.c.alpha )  << info->greenShift )
					| ( FULL_CH_32_VAL( source[0], blue,  cform.b.c.alpha   ) << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =    ( FULL_CH_32_VAL( GET_RED_16(source), red,   cform.b.c.alpha   )  << info->redShift )
					| ( FULL_CH_32_VAL( GET_GREEN_16(source), green, cform.b.c.alpha )  << info->greenShift )
					| ( FULL_CH_32_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha   ) << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32_Simple_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Simple_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					*target =   ( source->c.red   << info->redShift ) 
						| ( source->c.green << info->greenShift ) 
						| ( source->c.blue  << info->blueShift );
				}
				else if ( source->c.alpha != 0 )
				{
					*target  =    ( ALPHA_CH_32( red, source->c.alpha )   << info->redShift )
						| ( ALPHA_CH_32( green, source->c.alpha ) << info->greenShift )
						| ( ALPHA_CH_32( blue, source->c.alpha )  << info->blueShift );
					
					// 			*target =
					// 				    (((( s->c.red   * s->c.alpha ) + ( 255 - s->c.alpha ) * (( *target & info->redMask   ) >> info->redShift   ) ) >> 8 ) << info->redShift ) 
					// 				  | (((( s->c.green * s->c.alpha ) + ( 255 - s->c.alpha ) * (( *target & info->greenMask ) >> info->greenShift ) ) >> 8 ) << info->greenShift )
					// 				  | (((( s->c.blue  * s->c.alpha ) + ( 255 - s->c.alpha ) * (( *target & info->blueMask  ) >> info->blueShift  ) ) >> 8 ) << info->blueShift );
					
					// 			U32 redo =   (( *target & info->redMask   ) >> info->redShift   );
					// 			U32 greeno = (( *target & info->greenMask ) >> info->greenShift );
					// 			U32 blueo =  (( *target & info->blueMask  ) >> info->blueShift  );
					// 			U32 red =   (((( s->c.red   * s->c.alpha ) + ( 255 - s->c.alpha ) * redo ) >> 8 ) );
					// 			U32 green = (((( s->c.green * s->c.alpha ) + ( 255 - s->c.alpha ) * greeno ) >> 8 ) );
					// 			U32 blue =  (((( s->c.blue  * s->c.alpha ) + ( 255 - s->c.alpha ) * blueo ) >> 8 ) );
					// 			*target = ( red << info->redShift ) | ( green << info->greenShift ) | ( blue << info->blueShift );
				}
				--nPixel;
				++target;
				++source;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( source[2]   << info->redShift ) 
						| ( source[1] << info->greenShift ) 
						| ( source[0]  << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( GET_RED_16(source)   << info->redShift ) 
						| ( GET_GREEN_16(source) << info->greenShift ) 
						| ( GET_BLUE_16(source)  << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32_Color_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Color_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					*target =   ( CFORM( source->c.red, red )       << info->redShift ) 
						| ( CFORM( source->c.green, green ) << info->greenShift ) 
						| ( CFORM( source->c.blue, blue )    << info->blueShift );
				}
				else if ( source->c.alpha != 0 )
				{
					* target =    ( FULL_CH_32( red,   source->c.alpha   )  << info->redShift )
						| ( FULL_CH_32( green, source->c.alpha )  << info->greenShift )
						| ( FULL_CH_32( blue,  source->c.alpha   ) << info->blueShift );
				}
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( CFORM( source[2], red )       << info->redShift ) 
						| ( CFORM( source[1], green ) << info->greenShift ) 
						| ( CFORM( source[0], blue )    << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( CFORM( GET_RED_16(source), red )       << info->redShift ) 
						| ( CFORM( GET_GREEN_16(source), green ) << info->greenShift ) 
						| ( CFORM( GET_BLUE_16(source), blue )    << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32_Alpha_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Alpha_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	
	U32* target = (U32*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				* target =    ( ALPHA_CH_32( red, alpha )   << info->redShift )
					| ( ALPHA_CH_32( green, alpha ) << info->greenShift )
					| ( ALPHA_CH_32( blue, alpha )  << info->blueShift );
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{				
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				* target =    ( ALPHA_CH_32_VAL( source[2], red, alpha )   << info->redShift )
					| ( ALPHA_CH_32_VAL( source[1], green, alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( source[0], blue, alpha )  << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				*target =    ( ALPHA_CH_32_VAL( GET_RED_16(source), red, alpha )   << info->redShift )
					| ( ALPHA_CH_32_VAL( GET_GREEN_16(source), green, alpha ) << info->greenShift )
					| ( ALPHA_CH_32_VAL( GET_BLUE_16(source), blue, alpha )  << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32_Full_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Full_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				* target =    ( FULL_CH_32( red,   alpha   )  << info->redShift )
					| ( FULL_CH_32( green, alpha )  << info->greenShift )
					| ( FULL_CH_32( blue,  alpha   ) << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{				
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( FULL_CH_32_VAL( source[2], red,   alpha   )  << info->redShift )
					| ( FULL_CH_32_VAL( source[1], green, alpha )  << info->greenShift )
					| ( FULL_CH_32_VAL( source[0], blue,  alpha   ) << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				*target =    ( FULL_CH_32_VAL( GET_RED_16(source), red,   alpha   )  << info->redShift )
					| ( FULL_CH_32_VAL( GET_GREEN_16(source), green, alpha )  << info->greenShift )
					| ( FULL_CH_32_VAL( GET_BLUE_16(source), blue,  alpha   ) << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint32Rotated_Simple_NoAlpha( KrPaintInfo* info, //maks
								    void*		_target,
									void*	_source,
									char sourceBpp,
									int sPitch, // pitch in RGBAs, not bytes
									int nPixel )
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32Rotated_Simple_NoAlpha" );
	#endif
	


	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	U32* target = (U32*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target =   ( source->c.red   << info->redShift )
					| ( source->c.green << info->greenShift )
					| ( source->c.blue  << info->blueShift );
				nPixel--;
				target++;
				source += sPitch;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{				
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( source[2]   << info->redShift )
					| ( source[1] << info->greenShift )
					| ( source[0]  << info->blueShift );

				nPixel--;
				target++;
				source += 3*sPitch;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( GET_RED_16(source)   << info->redShift )
					| ( GET_GREEN_16(source) << info->greenShift )
					| ( GET_BLUE_16(source)  << info->blueShift );

				nPixel--;
				target++;
				source += sPitch;
			}
		}
		break;
	}
}


// ---------------- RGBA Rountines. (Use alpha channel) -------- //
void KrPaintRGBA_Simple_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaintRGBA_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	KrRGBA* target = (KrRGBA*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		gedMemcpy( target, _source, sizeof( KrRGBA) * nPixel );
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = source[2];
				target->c.green = source[1];
				target->c.blue  = source[0];
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = GET_RED_16(source);
				target->c.green = GET_GREEN_16(source);
				target->c.blue  = GET_BLUE_16(source);
				target->c.alpha = 255;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Color_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Color_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target->c.red   = CFORM( source->c.red,   red );
				target->c.green = CFORM( source->c.green, green );
				target->c.blue  = CFORM( source->c.blue,  blue );
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = CFORM( source[2],   red );
				target->c.green = CFORM( source[1], green );
				target->c.blue  = CFORM( source[0],  blue );
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = CFORM( GET_RED_16(source),   red );
				target->c.green = CFORM( GET_GREEN_16(source), green );
				target->c.blue  = CFORM( GET_BLUE_16(source),  blue );
				target->c.alpha = 255;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Alpha_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Alpha_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target->c.red   = ALPHA_CH_RGBA( red,   cform.b.c.alpha );
				target->c.green = ALPHA_CH_RGBA( green, cform.b.c.alpha );
				target->c.blue  = ALPHA_CH_RGBA( blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = ALPHA_CH_RGBA_VAL( source[2], red,   cform.b.c.alpha );
				target->c.green = ALPHA_CH_RGBA_VAL( source[1], green, cform.b.c.alpha );
				target->c.blue  = ALPHA_CH_RGBA_VAL( source[0], blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = ALPHA_CH_RGBA_VAL( GET_RED_16(source), red,   cform.b.c.alpha );
				target->c.green = ALPHA_CH_RGBA_VAL( GET_GREEN_16(source), green, cform.b.c.alpha );
				target->c.blue  = ALPHA_CH_RGBA_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Full_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Full_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target->c.red   = FULL_CH_RGBA( red,   cform.b.c.alpha );
				target->c.green = FULL_CH_RGBA( green, cform.b.c.alpha );
				target->c.blue  = FULL_CH_RGBA( blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = FULL_CH_RGBA_VAL( source[2], red,   cform.b.c.alpha );
				target->c.green = FULL_CH_RGBA_VAL( source[1], green, cform.b.c.alpha );
				target->c.blue  = FULL_CH_RGBA_VAL( source[0], blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = FULL_CH_RGBA_VAL( GET_RED_16(source), red,   cform.b.c.alpha );
				target->c.green = FULL_CH_RGBA_VAL( GET_GREEN_16(source), green, cform.b.c.alpha );
				target->c.blue  = FULL_CH_RGBA_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha );
				target->c.alpha = cform.b.c.alpha;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Simple_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Simple_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					*target = *source;
				}
				else if ( source->c.alpha != 0 )
				{
					target->c.red   = ALPHA_CH_RGBA( red, source->c.alpha );
					target->c.green	= ALPHA_CH_RGBA( green, source->c.alpha );
					target->c.blue  = ALPHA_CH_RGBA( blue, source->c.alpha );
					target->c.alpha = source->c.alpha;
				}
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = source[2];
				target->c.green = source[1];
				target->c.blue  = source[0];
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = GET_RED_16(source);
				target->c.green = GET_GREEN_16(source);
				target->c.blue  = GET_BLUE_16(source);
				target->c.alpha = 255;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Color_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Color_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					target->c.red   = CFORM( source->c.red, red );
					target->c.green = CFORM( source->c.green, green );
					target->c.blue	= CFORM( source->c.blue, blue );
					target->c.alpha = 255;
				}
				else if ( source->c.alpha != 0 )
				{
					target->c.red   = FULL_CH_RGBA( red,   source->c.alpha );
					target->c.green = FULL_CH_RGBA( green, source->c.alpha );
					target->c.blue  = FULL_CH_RGBA( blue,  source->c.alpha );
					target->c.alpha = source->c.alpha;
				}
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = CFORM( source[2], red );
				target->c.green = CFORM( source[1], green );
				target->c.blue	= CFORM( source[0], blue );
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = CFORM( GET_RED_16(source), red );
				target->c.green = CFORM( GET_GREEN_16(source), green );
				target->c.blue	= CFORM( GET_BLUE_16(source), blue );
				target->c.alpha = 255;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Alpha_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Alpha_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				target->c.red   = ALPHA_CH_RGBA( red, alpha );
				target->c.green = ALPHA_CH_RGBA( green, alpha );
				target->c.blue  = ALPHA_CH_RGBA( blue, alpha );
				target->c.alpha = alpha;
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{			
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = ALPHA_CH_RGBA_VAL( source[2], red, alpha );
				target->c.green = ALPHA_CH_RGBA_VAL( source[1], green, alpha );
				target->c.blue  = ALPHA_CH_RGBA_VAL( source[0], blue, alpha );
				target->c.alpha = alpha;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				target->c.red   = ALPHA_CH_RGBA_VAL( GET_RED_16(source), red, alpha );
				target->c.green = ALPHA_CH_RGBA_VAL( GET_GREEN_16(source), green, alpha );
				target->c.blue  = ALPHA_CH_RGBA_VAL( GET_BLUE_16(source), blue, alpha );
				target->c.alpha = alpha;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBA_Full_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32_Full_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );

	KrRGBA* target = (KrRGBA*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				target->c.red   = FULL_CH_RGBA( red, alpha );
				target->c.green	= FULL_CH_RGBA( green, alpha );
				target->c.blue  = FULL_CH_RGBA( blue,  alpha   );
				target->c.alpha = alpha;
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{		
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = FULL_CH_RGBA_VAL( source[2], red, alpha );
				target->c.green	= FULL_CH_RGBA_VAL( source[1], green, alpha );
				target->c.blue  = FULL_CH_RGBA_VAL( source[0], blue,  alpha   );
				target->c.alpha = alpha;
				
				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				target->c.red   = FULL_CH_RGBA_VAL( GET_RED_16(source), red, alpha );
				target->c.green	= FULL_CH_RGBA_VAL( GET_GREEN_16(source), green, alpha );
				target->c.blue  = FULL_CH_RGBA_VAL( GET_BLUE_16(source), blue,  alpha   );
				target->c.alpha = alpha;

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaintRGBARotated_Simple_NoAlpha( KrPaintInfo* info, //maks
								    void*		_target,
									void*	_source,
									char sourceBpp,
									int sPitch, // pitch in RGBAs, not bytes
									int nPixel )
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint32Rotated_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 4 );
	GLASSERT( nPixel >= 0 );
	KrRGBA* target = (KrRGBA*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target = *source;
				
				nPixel--;
				target++;
				source += sPitch;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{		
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				target->c.red   = source[2];
				target->c.green = source[1];
				target->c.blue  = source[0];
				target->c.alpha = 255;
				
				nPixel--;
				target++;
				source += 3*sPitch;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target->c.red   = GET_RED_16(source);
				target->c.green = GET_GREEN_16(source);
				target->c.blue  = GET_BLUE_16(source);
				target->c.alpha = 255;

				nPixel--;
				target++;
				source += sPitch;
			}
		}
		break;
	}
}

// ---------------- 24 Bit Blt Routines -------------------------//

// Utility inlines and macros.

// Caluclate the color using alpha.
// params:
//		ch		channel (red, green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U24* to the target pixel
//		info	KrPaintInfo

#define ALPHA_CH_24( ch, alpha ) (((( alpha * source->c.ch )) + (( 255-alpha ) * target[ info->ch##Byte ] ) ) >> 8 )
#define ALPHA_CH_24_VAL( val, ch, alpha ) (((( alpha * val )) + (( 255-alpha ) * target[ info->ch##Byte ] ) ) >> 8 ) //maks


// Caluclate the color using full.
// params:
//		ch		channel (red, green...)
//		Ch		channel (Red, Green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U24* to the target pixel
//		info	KrPaintInfo

#define FULL_CH_24( ch, Ch, alpha ) (((( alpha * cform.Transform##Ch( source->c.ch ))) + (( 255-alpha ) * target[ info->ch##Byte ] ) ) >> 8 )
#define FULL_CH_24_VAL( val, ch, Ch, alpha ) (((( alpha * cform.Transform##Ch( val ))) + (( 255-alpha ) * target[ info->ch##Byte ] ) ) >> 8 ) //maks


void KrPaint24_Simple_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );
	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = source->c.red;
				target[ info->greenByte ] = source->c.green;
				target[ info->blueByte ]  = source->c.blue;
				
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		gedMemcpy( target, _source, 3*nPixel );
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = GET_RED_16(source);
				target[ info->greenByte ] = GET_GREEN_16(source);
				target[ info->blueByte ]  = GET_BLUE_16(source);

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24_Color_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Color_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = CFORM( source->c.red, red );
				target[ info->greenByte ] = CFORM( source->c.green, green );
				target[ info->blueByte ]  = CFORM( source->c.blue, blue );
				
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{					
				target[ info->redByte ]   = CFORM( source[info->redByte], red );
				target[ info->greenByte ] = CFORM( source[info->greenByte], green );
				target[ info->blueByte ]  = CFORM( source[info->blueByte], blue );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = CFORM( GET_RED_16(source), red );
				target[ info->greenByte ] = CFORM( GET_GREEN_16(source), green );
				target[ info->blueByte ]  = CFORM( GET_BLUE_16(source), blue );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}

}


void KrPaint24_Alpha_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Alpha_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = ALPHA_CH_24( red, cform.b.c.alpha );
				target[ info->greenByte ] = ALPHA_CH_24( green, cform.b.c.alpha );
				target[ info->blueByte ]  = ALPHA_CH_24( blue, cform.b.c.alpha );
				
				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{					
				target[ info->redByte ]   = ALPHA_CH_24_VAL( source[info->redByte], red, cform.b.c.alpha );
				target[ info->greenByte ] = ALPHA_CH_24_VAL( source[info->greenByte], green, cform.b.c.alpha );
				target[ info->blueByte ]  = ALPHA_CH_24_VAL( source[info->blueByte], blue, cform.b.c.alpha );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = ALPHA_CH_24_VAL( GET_RED_16(source), red, cform.b.c.alpha );
				target[ info->greenByte ] = ALPHA_CH_24_VAL( GET_GREEN_16(source), green, cform.b.c.alpha );
				target[ info->blueByte ]  = ALPHA_CH_24_VAL( GET_BLUE_16(source), blue, cform.b.c.alpha );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24_Full_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Full_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = FULL_CH_24( red, Red, cform.b.c.alpha );
				target[ info->greenByte ] = FULL_CH_24( green, Green, cform.b.c.alpha );
				target[ info->blueByte ]  = FULL_CH_24( blue, Blue, cform.b.c.alpha );
				
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{					
				target[ info->redByte ]   = FULL_CH_24_VAL( source[info->redByte], red, Red, cform.b.c.alpha );
				target[ info->greenByte ] = FULL_CH_24_VAL( source[info->greenByte], green, Green, cform.b.c.alpha );
				target[ info->blueByte ]  = FULL_CH_24_VAL( source[info->blueByte], blue, Blue, cform.b.c.alpha );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = FULL_CH_24_VAL( GET_RED_16(source), red, Red, cform.b.c.alpha );
				target[ info->greenByte ] = FULL_CH_24_VAL( GET_GREEN_16(source), green, Green, cform.b.c.alpha );
				target[ info->blueByte ]  = FULL_CH_24_VAL( GET_BLUE_16(source), blue, Blue, cform.b.c.alpha );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24_Simple_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Simple_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					target[ info->redByte ]   = source->c.red;
					target[ info->greenByte ] = source->c.green;
					target[ info->blueByte ]  = source->c.blue;
				}
				else if ( source->c.alpha != 0 )
				{
					target[ info->redByte ]   = ALPHA_CH_24( red, source->c.alpha );
					target[ info->greenByte ] = ALPHA_CH_24( green, source->c.alpha );
					target[ info->blueByte ]  = ALPHA_CH_24( blue, source->c.alpha );
				}
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		gedMemcpy( target, _source, 3*nPixel );
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = GET_RED_16(source);
				target[ info->greenByte ] = GET_GREEN_16(source);
				target[ info->blueByte ]  = GET_BLUE_16(source);

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24_Color_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Color_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					target[ info->redByte ]   = CFORM( source->c.red, red );
					target[ info->greenByte ] = CFORM( source->c.green, green );
					target[ info->blueByte ]  = CFORM( source->c.blue, blue );
				}
				else if ( source->c.alpha != 0 )
				{
					target[ info->redByte ]   = FULL_CH_24( red, Red, source->c.alpha );
					target[ info->greenByte ] = FULL_CH_24( green, Green, source->c.alpha );
					target[ info->blueByte ]  = FULL_CH_24( blue, Blue, source->c.alpha );
				}
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{					
				target[ info->redByte ]   = CFORM( source[info->redByte], red );
				target[ info->greenByte ] = CFORM( source[info->greenByte], green );
				target[ info->blueByte ]  = CFORM( source[info->blueByte], blue );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = CFORM( GET_RED_16(source), red );
				target[ info->greenByte ] = CFORM( GET_GREEN_16(source), green );
				target[ info->blueByte ]  = CFORM( GET_BLUE_16(source), blue );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24_Alpha_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Alpha_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				
				target[ info->redByte ]   = ALPHA_CH_24( red, alpha );
				target[ info->greenByte ] = ALPHA_CH_24( green, alpha );
				target[ info->blueByte ]  = ALPHA_CH_24( blue, alpha );
				
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{					
				target[ info->redByte ]   = ALPHA_CH_24_VAL( source[info->redByte], red, alpha );
				target[ info->greenByte ] = ALPHA_CH_24_VAL( source[info->greenByte], green, alpha );
				target[ info->blueByte ]  = ALPHA_CH_24_VAL( source[info->blueByte], blue, alpha );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				target[ info->redByte ]   = ALPHA_CH_24_VAL( GET_RED_16(source), red, alpha );
				target[ info->greenByte ] = ALPHA_CH_24_VAL( GET_GREEN_16(source), green, alpha );
				target[ info->blueByte ]  = ALPHA_CH_24_VAL( GET_BLUE_16(source), blue, alpha );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}

}


void KrPaint24_Full_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24_Full_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );

	U8* target = (U8*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				target[ info->redByte ]   = FULL_CH_24( red, Red, alpha );
				target[ info->greenByte ] = FULL_CH_24( green, Green, alpha );
				target[ info->blueByte ]  = FULL_CH_24( blue, Blue, alpha );
				
				nPixel--;
				target+=3;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{					
				target[ info->redByte ]   = FULL_CH_24_VAL( source[info->redByte], red, Red, alpha );
				target[ info->greenByte ] = FULL_CH_24_VAL( source[info->greenByte], green, Green, alpha );
				target[ info->blueByte ]  = FULL_CH_24_VAL( source[info->blueByte], blue, Blue, alpha );
				
				nPixel--;
				target += 3;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				target[ info->redByte ]   = FULL_CH_24_VAL( GET_RED_16(source), red, Red, alpha );
				target[ info->greenByte ] = FULL_CH_24_VAL( GET_GREEN_16(source), green, Green, alpha );
				target[ info->blueByte ]  = FULL_CH_24_VAL( GET_BLUE_16(source), blue, Blue, alpha );

				nPixel--;
				target += 3;
				source++;
			}
		}
		break;
	}
}


void KrPaint24Rotated_Simple_NoAlpha( KrPaintInfo* info, //maks
								    void*		_target,
									void*	_source,
									char sourceBpp,
									int sPitch, 
									int nPixel )
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint24Rotated_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 3 );
	GLASSERT( nPixel >= 0 );
	U8* target = (U8*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = source->c.red;
				target[ info->greenByte ] = source->c.green;
				target[ info->blueByte ]  = source->c.blue;
				
				nPixel--;
				target+=3;
				source+=sPitch;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{					
				target[ info->redByte ]   = source[info->redByte];
				target[ info->greenByte ] = source[info->greenByte];
				target[ info->blueByte ]  = source[info->blueByte];
				
				nPixel--;
				target += 3;
				source += 3*sPitch;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				target[ info->redByte ]   = GET_RED_16(source);
				target[ info->greenByte ] = GET_GREEN_16(source);
				target[ info->blueByte ]  = GET_BLUE_16(source);

				nPixel--;
				target += 3;
				source += sPitch;
			}
		}
		break;
	}
}

//#endif //#ifndef _WIN32_WCE //maks

// ---------------- 16 Bit Blt Routines -------------------------//


// Caluclate the color using alpha.
// params:
//		ch		channel (red, green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U32* to the target pixel
//		info	KrPaintInfo

/*int multCache[256*64]; //maks
class MultCache
{
public:
	MultCache()
	{
		for(int a = 0; a < 255; a++)
		{
			for(int b = 0; b < 64; b++)
			{
				//Red and blue
				int val = a*((b << 3) + (b >> 5));
				multCache[a | (b << 8)] = val;
			}
		}
	}
};

MultCache cacheMult;*/

#define ALPHA_CH_16( ch, alpha ) (((( alpha * source->c.ch )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) ) ) >> 8 )
#define ALPHA_CH_16_VAL( val, ch, alpha ) (((( alpha * val )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) ) ) >> 8 ) //maks

U32 PLUS64         = 64 | (64 << 16);
//#define ALPHA_CH_16_VAL_FAST( val, ch, alpha ) ((( alpha * (val - (( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) ) ) + ((( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) << 8)) >> 8 ) //maks
#define ALPHA_CH_16_VAL_FAST( val, tgt, ch, alpha ) ((multCache[alpha | (val << 8)] + (multCache[( 255-alpha ) | (tgt << 8)]  ) ) >> 8 ) //maks


// Caluclate the color using full.
// params:
//		ch		channel (red, green...)
//		Ch		channel (Red, Green...)
//		alpha	the alpha value (0-255)
// uses:
//		source	KrRGBA* 
//		target	U32* to the target pixel
//		info	KrPaintInfo

#define FULL_CH_16( ch, alpha )	(((( alpha * CFORM( source->c.ch, ch ) )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) ) ) >> 8 )
#define FULL_CH_16_VAL( val, ch, alpha )	(((( alpha * CFORM( val, ch ) )) + (( 255-alpha ) * (( *target & info->ch##Mask ) >> info->ch##Shift << info->ch##Loss ) ) ) >> 8 ) //maks



void KrPaint16_Simple_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );
	
	//U16* end  = target + nPixel;
	
	switch(sourceBpp) //maks
	{
	case 2:
		//memcpy 450 cycles in 1945.ged (40s)
		//gedMemcpy 127 cycles in 1945.ged (40s)
		gedMemcpy( _target, _source, nPixel << 1 );

		//Can we use the optimized version?
		//286 cycles in 1945.ged
        /*if (!(nPixel & 7))
        {
            // Surface is a multiple of 8, use loop unrolling
            U64* pDest = (U64*)_target;
			U64* pSrc = (U64*)_source;
            
			
			for ( int x = nPixel >> 3; x; --x )
			{
				*pDest++ = *pSrc++;
				*pDest++ = *pSrc++;
			}
            
        }
		else if (!(nPixel & 3))
        {
            // Surface is a multiple of 4, use loop unrolling
            U64* pDest = (U64*)_target;
			U64* pSrc = (U64*)_source;
            
			
			for ( int x = nPixel >> 2; x; --x )
			{
				*pDest++ = *pSrc++;				
			}
            
        }
        else
        {
            // Surface is not a multiple of 8
            gedMemcpy( _target, _source, nPixel << 1 );
        }*/
		
		//115 cycles in 1945.ged		
		/*{
			//Process single pixels until we are dword aligned
			U8* pDest8 = (U8*)_target;
			U8* pSrc8 = (U8*)_source;
			
			//while(((int)pDest8&0x3)!=0)
			//{ 
			//	*pDest8++ = *pSrc8++;	
			//} 

			U64* pDest = (U64*)pDest8;
			U64* pSrc = (U64*)pSrc8;
			
			// Write blocks of eight pixels		
			while(nPixel >= 8)
			{ 
				*pDest++ = *pSrc++;
				*pDest++ = *pSrc++;			
				nPixel -= 8;
			}
			
			// Write blocks of four pixels		
			while(nPixel >= 4)
			{ 
				*pDest++ = *pSrc++;			
				nPixel -= 4;
			}
			
			// Write blocks of two pixels	
			U32* pDest32 = (U32*)pDest;
			U32* pSrc32 = (U32*)pSrc;
			while(nPixel >= 2)
			{ 
				*pDest32++ = *pSrc32++;
				nPixel -= 2;
			}		
			
			// Eventually, write a single odd pixel that might be left 
			if(nPixel&1)
			{ 
				*(U16 *)pDest32 = *(U16 *)pSrc32;			
			}
		}*/

		break;
	case 4:
		{
			U16* target   = (U16*) _target;
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target =   ( ( source->c.red >> info->redLoss )     << info->redShift ) 
					| ( ( source->c.green >> info->greenLoss ) << info->greenShift ) 
					| ( ( source->c.blue >> info->blueLoss )   << info->blueShift );
				--nPixel;
				++target;
				++source;
			}
		}
		break;
	case 3:
		{
			U16* target   = (U16*) _target;
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( ( source[2] >> info->redLoss )     << info->redShift )
					| ( ( source[1] >> info->greenLoss ) << info->greenShift ) 
					| ( ( source[0] >> info->blueLoss )   << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;	
	}
}


void KrPaint16_Color_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Color_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target =   ( CFORM( source->c.red, red ) >> info->redLoss  << info->redShift ) 
					| ( CFORM( source->c.green, green ) >> info->greenLoss << info->greenShift ) 
					| ( CFORM( source->c.blue, blue ) >> info->blueLoss << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( CFORM( source[2], red ) >> info->redLoss  << info->redShift ) 
					| ( CFORM( source[1], green ) >> info->greenLoss << info->greenShift ) 
					| ( CFORM( source[0], blue ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( CFORM( GET_RED_16(source), red ) >> info->redLoss  << info->redShift ) 
					| ( CFORM( GET_GREEN_16(source), green ) >> info->greenLoss << info->greenShift ) 
					| ( CFORM( GET_BLUE_16(source), blue ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint16_Alpha_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Alpha_NoAlpha" );
	#endif 


	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				* target =    ( ALPHA_CH_16( red, cform.b.c.alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16( green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16( blue, cform.b.c.alpha )  >> info->blueLoss << info->blueShift );
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( ALPHA_CH_16_VAL( source[2], red, cform.b.c.alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16_VAL( source[1], green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16_VAL( source[0], blue, cform.b.c.alpha )  >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{

			/*U16 *source = (U16 *)_source;			

			while ( nPixel )
			{	
				//3848 cycles in 1945.ged
				*target =    ( ALPHA_CH_16_VAL( GET_RED_16(source), red, cform.b.c.alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16_VAL( GET_BLUE_16(source), blue, cform.b.c.alpha )  >> info->blueLoss << info->blueShift );
				
				
				nPixel--;
				target++;
				source++;
			}*/

			//maks
			//alignment == 0: source and target aligned
			//alignment == 4: source and target unaligned
			//alignment == 2: source or target unaligned
			

			//2490 cycles in 1945.ged
			int alignment = ((int)_target & 3) + ((int)_source & 3);

			if(alignment == 2)
			{				
				//2572 cycles in 1945.ged
				U32 ALPHABY4 = cform.b.c.alpha >> 2;
				U16 sTemp,dTemp;
				U16 sr,sg,sb,dr,dg,db;
				U16* lpSprite = (U16*)_source;
				U16* lpDest = (U16*)target;
				
				while(nPixel-- > 0)
				{
					sTemp = *lpSprite;				
					dTemp = *lpDest;
					
					sb = sTemp & 0x1f;
					db = dTemp & 0x1f;
					sg = (sTemp >> 5)  & 0x3f;
					dg = (dTemp >> 5)  & 0x3f;
					sr = (sTemp >> 11) & 0x1f;
					dr = (dTemp >> 11) & 0x1f;
					
					*lpDest = 
						( (((cform.b.c.alpha * ((sr + 64) - dr) >> 8) + dr) - ALPHABY4) << 11) |
						( (((cform.b.c.alpha * ((sg + 64) - dg) >> 8) + dg) - ALPHABY4) << 5) |
						( ((cform.b.c.alpha  * ((sb + 64) - db) >> 8) + db) - ALPHABY4);
					
					
					lpDest++;
					lpSprite++;					
				}
			}
			else
			{
				//1630 cycles in 1945.ged
				U32 ALPHABY4       = (cform.b.c.alpha >> 2) | ((cform.b.c.alpha >> 2) << 16);
				U32 sTemp,dTemp;
				U32 sr,sg,sb,dr,dg,db;
				U8* lpSprite = (U8*)_source;
				U8* lpDest = (U8*)target;
				int oddWidth;
				
				
				if(alignment == 4)
				{
					//Missalignment
					
					nPixel -= 1;
					sTemp = *((U16*)lpSprite);				
					dTemp = *((U16*)lpDest);
					
					sb = sTemp & 0x1f;
					db = dTemp & 0x1f;
					sg = (sTemp >> 5) & 0x3f;
					dg = (dTemp >> 5) & 0x3f;
					sr = (sTemp >> 11) & 0x1f;
					dr = (dTemp >> 11) & 0x1f;
					
					*((U16*)lpDest) = (U16)
						((cform.b.c.alpha * (sb - db) >> 8) + db |
						((cform.b.c.alpha * (sg - dg) >> 8) + dg) << 5 |
						((cform.b.c.alpha * (sr - dr) >> 8) + dr) << 11);
					
					
					lpDest   += 2;
					lpSprite += 2;
				}
				
				oddWidth = nPixel & 1;
				nPixel >>= 1;  //div by 2, processing 2 pixels at a time.			
				
				while(nPixel-- > 0)
				{
					sTemp = *((U32*)lpSprite);				
					dTemp = *((U32*)lpDest);
					
					sb = sTemp & 0x001F001F;
					db = dTemp & 0x001F001F;
					sg = (sTemp >> 5)  & 0x003F003F;
					dg = (dTemp >> 5)  & 0x003F003F;
					sr = (sTemp >> 11) & 0x001F001F;
					dr = (dTemp >> 11) & 0x001F001F;
					
					*((U32*)lpDest) = 
						(((((cform.b.c.alpha * ((sr + PLUS64) - dr)) >> 8) + dr) - ALPHABY4) & 0x001F001F) << 11 |
						(((((cform.b.c.alpha * ((sg + PLUS64) - dg)) >> 8) + dg) - ALPHABY4) & 0x003F003F) << 5 |					
						((((cform.b.c.alpha * ((sb + PLUS64) - db)) >> 8) + db) - ALPHABY4) & 0x001F001F;		
					
					lpDest    += 4;
					lpSprite  += 4;
					
				} 
				
				//Last pixel
				if(oddWidth)
				{
					sTemp = *((U16*)lpSprite);				
					dTemp = *((U16*)lpDest);
					
					sb = sTemp & 0x1f;
					db = dTemp & 0x1f;
					sg = (sTemp >> 5) & 0x3f;
					dg = (dTemp >> 5) & 0x3f;
					sr = (sTemp >> 11) & 0x1f;
					dr = (dTemp >> 11) & 0x1f;
					
					*((U16*)lpDest) = (U16)((cform.b.c.alpha * (sb - db) >> 8) + db |
						((cform.b.c.alpha * (sg - dg) >> 8) + dg) << 5 |
						((cform.b.c.alpha * (sr - dr) >> 8) + dr) << 11);
				}
			}
		}
		break;
	}
}



void KrPaint16_Full_NoAlpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Full_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				// 		U8 alpha = ( cform.Alpha() * source->alpha ) >> 8;
				
				* target =    ( FULL_CH_16( red,   cform.b.c.alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16( green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16( blue,  cform.b.c.alpha   ) >> info->blueLoss << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( FULL_CH_16_VAL( source[2], red,   cform.b.c.alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16_VAL( source[1], green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16_VAL( source[0], blue,  cform.b.c.alpha   ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =    ( FULL_CH_16_VAL( GET_RED_16(source), red,   cform.b.c.alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16_VAL( GET_GREEN_16(source), green, cform.b.c.alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16_VAL( GET_BLUE_16(source), blue,  cform.b.c.alpha   ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint16_Simple_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Simple_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					*target =   ( source->c.red   >> info->redLoss   << info->redShift ) 
						| ( source->c.green >> info->greenLoss << info->greenShift ) 
						| ( source->c.blue  >> info->blueLoss  << info->blueShift );
				}
				else if ( source->c.alpha != 0 )
				{
					* target =    ( ALPHA_CH_16( red, source->c.alpha )   >> info->redLoss   << info->redShift )
						| ( ALPHA_CH_16( green, source->c.alpha ) >> info->greenLoss << info->greenShift )
						| ( ALPHA_CH_16( blue, source->c.alpha )  >> info->blueLoss  << info->blueShift );
				}
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( source[2]   >> info->redLoss   << info->redShift ) 
						| ( source[1] >> info->greenLoss << info->greenShift ) 
						| ( source[0]  >> info->blueLoss  << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		gedMemcpy( target, _source, nPixel << 1);
		break;
	}
}


void KrPaint16_Color_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Color_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				if ( source->c.alpha == 255 )
				{
					*target =   ( CFORM( source->c.red, red )       >> info->redLoss   << info->redShift ) 
						| ( CFORM( source->c.green, green ) >> info->greenLoss << info->greenShift ) 
						| ( CFORM( source->c.blue, blue )    >> info->blueLoss  << info->blueShift );
				}
				else if ( source->c.alpha != 0 )
				{
					* target =    ( FULL_CH_16( red,   source->c.alpha   )    >> info->redLoss << info->redShift )
						| ( FULL_CH_16( green, source->c.alpha )  >> info->greenLoss << info->greenShift )
						| ( FULL_CH_16( blue,  source->c.alpha   )  >> info->blueLoss << info->blueShift );
				}
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( CFORM( source[2], red )       >> info->redLoss   << info->redShift ) 
						| ( CFORM( source[1], green ) >> info->greenLoss << info->greenShift ) 
						| ( CFORM( source[0], blue )    >> info->blueLoss  << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( CFORM( GET_RED_16(source), red )       >> info->redLoss   << info->redShift ) 
						| ( CFORM( GET_GREEN_16(source), green ) >> info->greenLoss << info->greenShift ) 
						| ( CFORM( GET_BLUE_16(source), blue )    >> info->blueLoss  << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint16_Alpha_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Alpha_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;

	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				* target =    ( ALPHA_CH_16( red, alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16( green, alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16( blue, alpha )  >> info->blueLoss << info->blueShift );
				
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( ALPHA_CH_16_VAL( source[2], red, alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16_VAL( source[1], green, alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16_VAL( source[0], blue, alpha )  >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				*target =    ( ALPHA_CH_16_VAL( GET_RED_16(source), red, alpha )   >> info->redLoss << info->redShift )
					| ( ALPHA_CH_16_VAL( GET_GREEN_16(source), green, alpha ) >> info->greenLoss << info->greenShift )
					| ( ALPHA_CH_16_VAL( GET_BLUE_16(source), blue, alpha )  >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint16_Full_Alpha(KrPaintInfo* info, void* _target, void* _source, char sourceBpp, int nPixel, const KrColorTransform& cform) //maks
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16_Full_Alpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );

	U16* target = (U16*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				U8 alpha = ( cform.b.c.alpha * source->c.alpha ) >> 8;
				
				* target =    ( FULL_CH_16( red,   alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16( green, alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16( blue,  alpha   ) >> info->blueLoss << info->blueShift );
				nPixel--;
				target++;
				source++;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =    ( FULL_CH_16_VAL( source[2], red,   alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16_VAL( source[1], green, alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16_VAL( source[0], blue,  alpha   ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source += 3;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			U8 alpha = ( cform.b.c.alpha * 255 ) >> 8;
			while ( nPixel )
			{	
				*target =    ( FULL_CH_16_VAL( GET_RED_16(source), red,   alpha   )   >> info->redLoss << info->redShift )
					| ( FULL_CH_16_VAL( GET_GREEN_16(source), green, alpha ) >> info->greenLoss << info->greenShift )
					| ( FULL_CH_16_VAL( GET_BLUE_16(source), blue,  alpha   ) >> info->blueLoss << info->blueShift );

				nPixel--;
				target++;
				source++;
			}
		}
		break;
	}
}


void KrPaint16Rotated_Simple_NoAlpha( KrPaintInfo* info, //maks
								    void*		_target,
									void*	_source,
									char sourceBpp,
									int sPitch,
									int nPixel )
{
	#ifdef COUNT_PERFORMANCE
	GlPerformance perf( "KrPaint16Rotated_Simple_NoAlpha" );
	#endif 



	GLASSERT( info->bytesPerPixel == 2 );
	GLASSERT( nPixel >= 0 );
	U16* target = (U16*) _target;
	
	switch(sourceBpp) //maks
	{
	case 4:
		{
			KrRGBA *source = (KrRGBA *)_source;
			while ( nPixel )
			{	
				*target =   ( ( source->c.red >> info->redLoss )     << info->redShift ) 
					| ( ( source->c.green >> info->greenLoss ) << info->greenShift ) 
					| ( ( source->c.blue >> info->blueLoss )   << info->blueShift );
				--nPixel;
				++target;
				source += sPitch;
			}
		}
		break;
	case 3:
		{
			U8 *source = (U8 *)_source;
			while ( nPixel )
			{	
				//source rgb =	0x00ff0000,	
				//				0x0000ff00,
				//				0x000000ff

				*target =   ( ( source[2] >> info->redLoss )     << info->redShift ) 
					| ( ( source[1] >> info->greenLoss ) << info->greenShift ) 
					| ( ( source[0] >> info->blueLoss )   << info->blueShift );

				nPixel--;
				target++;
				source += 3*sPitch;
			}
		}
		break;
	case 2:
		{
			U16 *source = (U16 *)_source;
			while ( nPixel )
			{	
				*target =   ( ( GET_RED_16(source) >> info->redLoss )     << info->redShift ) 
					| ( ( GET_GREEN_16(source) >> info->greenLoss ) << info->greenShift ) 
					| ( ( GET_BLUE_16(source) >> info->blueLoss )   << info->blueShift );

				nPixel--;
				target++;
				source += sPitch;
			}
		}
		break;
	}
}

#ifdef _MSC_VER
#pragma optimize ( "w", off )
#endif

void KrPaintInfo::GetBlitterName( KrPaintFunc func, gedString* name )
{
#ifndef _WIN32_WCE //maks
	if ( func == KrPaint32_Simple_NoAlpha ) 
	{	*name = "32_Simple_NoAlpha"; return; }
	else if ( func == KrPaint32B_Simple_NoAlpha ) 
	{	*name = "32B_Simple_NoAlpha"; return; }
	else if ( func == KrPaint32_Color_NoAlpha ) 
	{	*name = "32_Color_NoAlpha"; return; }
	else if ( func == KrPaint32_Alpha_NoAlpha )
	{	*name = "32_Alpha_NoAlpha"; return; }
	else if ( func == KrPaint32_Full_NoAlpha )
	{	*name = "32_Full_NoAlpha"; return; }
	else if ( func == KrPaint32_Simple_Alpha )
	{	*name = "32_Simple_Alpha"; return; }
	else if ( func == KrPaint32_Color_Alpha )
	{	*name = "32_Color_Alpha"; return; }
	else if ( func == KrPaint32_Alpha_Alpha )
	{	*name = "32_Alpha_Alpha"; return; }
	else if ( func == KrPaint32_Full_Alpha )
	{	*name = "32_Full_Alpha"; return; }
	else if ( func == KrPaint24_Simple_NoAlpha )
	{	*name = "24_Simple_NoAlpha"; return; }
	else if ( func == KrPaint24_Color_NoAlpha )
	{	*name = "24_Color_NoAlpha"; return; }
	else if ( func == KrPaint24_Alpha_NoAlpha )
	{	*name = "24_Alpha_NoAlpha"; return; }
	else if ( func == KrPaint24_Full_NoAlpha )
	{	*name = "24_Full_NoAlpha"; return; }
	else if ( func == KrPaint24_Simple_Alpha )
	{	*name = "24_Simple_Alpha"; return; }
	else if ( func == KrPaint24_Color_Alpha )
	{	*name = "24_Color_Alpha"; return; }
	else if ( func == KrPaint24_Alpha_Alpha )
	{	*name = "24_Alpha_Alpha"; return; }
	else if ( func == KrPaint24_Full_Alpha )
	{	*name = "24_Full_Alpha"; return; }
	else 
#endif //#ifndef _WIN32_WCE //maks
		if ( func == KrPaint16_Simple_NoAlpha )
	{	*name = "16_Simple_NoAlpha"; return; }
	else if ( func == KrPaint16_Color_NoAlpha )
	{	*name = "16_Color_NoAlpha"; return; }
	else if ( func == KrPaint16_Alpha_NoAlpha )
	{	*name = "16_Alpha_NoAlpha"; return; }
	else if ( func == KrPaint16_Full_NoAlpha )
	{	*name = "16_Full_NoAlpha"; return; }
	else if ( func == KrPaint16_Simple_Alpha )
	{	*name = "16_Simple_Alpha"; return; }
	else if ( func == KrPaint16_Color_Alpha )
	{	*name = "16_Color_Alpha"; return; }
	else if ( func == KrPaint16_Alpha_Alpha )
	{	*name = "16_Alpha_Alpha"; return; }
	else if ( func == KrPaint16_Full_Alpha )
	{	*name = "16_Full_Alpha"; return; }
	else
	{	
		*name = "error";
	}
	return;
}


void KrPaintInfo::SetTranparentColor(const KrRGBA &color) //maks
{
	//Used in 16 and 24 bit images
	
	if(!transparentColor)
		transparentColor = new KrRGBA;

	*transparentColor = color;
}


