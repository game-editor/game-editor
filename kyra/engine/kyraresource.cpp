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
#include "kyraresource.h"
#include "SDL_endian.h"
#include "spriteresource.h"
#include "tileresource.h"
#include "fontresource.h"
#include "dataresource.h"
#include "../util/gldebug.h"
#include "../util/glmemorypool.h"


void KrResource::SetNameAndId( const gedString& _name, U32 _id ) //maks	
{ 
	resName = _name.GetFileName(); 
	resId = _id; 
}

KrResourceFactory::KrResourceFactory()
{}

KrResourceFactory::~KrResourceFactory()
{
	instance = 0;
}

KrResourceFactory* KrResourceFactory::instance = 0;

KrResourceFactory* KrResourceFactory::Instance()
{
	if ( !instance )
	{
		instance = new KrResourceFactory;
	}
	return instance;
}


KrResource*	KrResourceFactory::Create(	U32 id, 
										U32 size, 
										SDL_RWops* data )
{	
	switch( id )
	{
		case KYRATAG_SPRITE:
		{
			return new KrSpriteResource( size, data );
		}
		break;

#ifndef STAND_ALONE_GAME //maks
		case KYRATAG_TILE:
		{
			return new KrTileResource( size, data );
		}
		break;
#endif

		case KYRATAG_FONT:
		{			
			return new KrFontResource( size, data );
		}

		case KYRATAG_TEXTDATA:
		{
			return new KrTextDataResource( size, data );
		}

		case KYRATAG_BINARYDATA:
		{
			return new KrBinaryDataResource( size, data );
		}
	}
	
	#ifdef DEBUG
		GLOUTPUT( "Warning: could not find resource id=%d in KrResourceFactory\n", id );
		GLASSERT( 0 );
	#endif
	return 0;
}


/////////////////////////////// KrCollisionMap


KrCollisionMap::KrCollisionMap( GlFixed _xScale, GlFixed _yScale, int width, int height )
{
	cx = (width  + 31) / 32;
	cy = height;
	map = new U32[ cx * cy ];
	memset( map, 0, sizeof( U32 ) * cx * cy );
	xScale = _xScale;
	yScale = _yScale;
}

/* (C) Donald W. Gillies, 1992.  All rights reserved.  You may reuse
   this bitcount() function anywhere you please as long as you retain
   this Copyright Notice. */
#define bitready()	register U32 tmp
#define bitcount(n) 							\
      (tmp = (n) - (((n) >> 1) & 033333333333) - (((n) >> 2) & 011111111111),	\
      tmp = ((tmp + (tmp >> 3)) & 030707070707),			\
      tmp =  (tmp + (tmp >> 6)),					\
      tmp = (tmp + (tmp >> 12) + (tmp >> 24)) & 077)
/* End of Donald W. Gillies bitcount code */


/*#include "engine.h"
extern KrEngine *engine;
void Draw(const KrRect& boundsIntersect,U32* thisWord, U32* otherWord, int bitShift, int i, int j, int ocx)
{
	
	KrPainter painter(engine->Surface());
	KrRGBA color;
	
	engine->InvalidateScreen();
	
	painter.DrawBox(boundsIntersect.xmin, boundsIntersect.ymin, boundsIntersect.Width(), boundsIntersect.Height(), 0, 255, 255);
	
	color.Set(255, 255, 255);					
	for( int n = 31; n >= 0; n--)
	{
		U32 w;
		
		if(i > 0) w = bitShift?(otherWord[ i-1 ] << ( 32 - bitShift )):0;
		else if ( i < ocx ) w = otherWord[ i ] >> ( bitShift );
		
		if(w & (1 << n))
		{
			int x, y;
			
			x = i*32 + (31 - n) + boundsIntersect.xmin;
			y = j + boundsIntersect.ymin;
			painter.SetPixel(x, y, color);
		}
	}
	
	color.Set(255, 0, 0);					
	for( n = 31; n >= 0; n--)
	{
		U32 w = thisWord[i];
		if(w & (1 << n))
		{
			int x, y;
			
			x = i*32 + (31 - n) + boundsIntersect.xmin;
			y = j + boundsIntersect.ymin;
			painter.SetPixel(x, y, color);
		}
	}
	
	
	SDL_UpdateRect(engine->Surface(), 0, 0, 0, 0);
}*/

int KrCollisionMap::Collide( int offsetX, 
							  int offsetY, 
							  const KrRect& boundsIntersect,
							  KrCollisionMap* other )
{
	GLASSERT( offsetX >= 0 );

	bitready(); //maks

	int dx = offsetX >> 5;//maks / 32;			// words to the start of other
	int bitShift = offsetX & 31; //maks % 32;	// bits to the start of other

	// Words to check is sort of tricky. It's not just a width,
	// but the span of the pixels over the bit map.
	int wordWidth =   (( offsetX + boundsIntersect.Width() - 1 ) >> 5)/*/ 32*/	// high word bound
					- (( offsetX ) >> 5/*/ 32*/	)								// low word bound == dx
					+ 1;												// make width

	// ...but the wordwidth is never larget than the width of this or other.
	wordWidth = GlMin(	wordWidth,		// the max it could be
						GlMin(	cx,					// this bit width
								other->cx + 1 ) );	// other bit width: which can span 2 words of this. The left and right checks.
						
	
	int height = boundsIntersect.Height();

	int otherStartRow = 0;
	int thisStartRow = 0;
	if ( offsetY < 0 )
	{
		otherStartRow = -offsetY;
	}
	else
	{
		thisStartRow = offsetY;
	}

	int overlapped = 0; //maks
	for( int j=0; j<height; ++j )
	{
		// The other -- since it is to the right -- always start at its right edge.
		// index 0. This starts at the dx offset.
		U32* otherWord = other->GetRow( j + otherStartRow );
		U32* thisWord  = GetRow( thisStartRow + j ) + dx;
		U32 intersect; //maks

		if(otherWord < (U32*)1000 || thisWord < (U32*)1000)
			return overlapped; //maks: invalid pointers
		
		for( int i=0; i<wordWidth; ++i ) //maks
		{
			// Check to left.
			if ( i > 0 )			// make sure we can check left.
			{
				// We are comparing the left word of other. So the left
				// lowest bits match with this' highest.

				
				intersect = thisWord[i] & ( otherWord[ i-1 ] << ( 32 - bitShift ) ); 
				if (bitShift && intersect ) //Maks: if bitShift == 0, w << 32 must be 0
					overlapped += bitcount(intersect); 				
			}

			// Check to the right. 
			if ( i < other->cx )	// make sure there is a right to check.
			{
				// The highest bits of other's right
				// will match with our lower bits, so shift the other direction.
				intersect = thisWord[i] & ( otherWord[ i ] >> ( bitShift ) );
				if ( intersect )
					overlapped += bitcount(intersect); 

			}

			//Draw(boundsIntersect, thisWord, otherWord, bitShift, i, j, other->cx);
		}			
	}

	
	return overlapped;
}



int KrCollisionMap::CollideRetangle( int offsetX, 
							  int offsetY, 
							  const KrRect& boundsIntersect) //maks
{
	GLASSERT( offsetX >= 0 );

	bitready(); 

	int dx = offsetX >> 5;/*/ 32;*/			// words to the start of other
	int bitShift = offsetX & 31;/*% 32;*/	// bits to the start of other

	// Words to check is sort of tricky. It's not just a width,
	// but the span of the pixels over the bit map.
	int wordWidth =   (( offsetX + boundsIntersect.Width() - 1 ) >> 5/*/ 32*/)	// high word bound
					- (( offsetX ) >> 5/*/ 32*/	)								// low word bound == dx
					+ 1;												// make width

		
	int height = boundsIntersect.Height();

	int thisStartRow = 0;
	if ( offsetY >= 0 )
	{
		thisStartRow = offsetY;
	}

	int overlapped = 0; 
	for( int j=0; j<height; ++j )
	{
		// The other -- since it is to the right -- always start at its right edge.
		// index 0. This starts at the dx offset.
		U32* thisWord  = GetRow( thisStartRow + j ) + dx;
		
		if(thisWord) //maks: solve the crash in 'LOS Ultimo.ged' when move the soldier to the game area, in edit mode, and go to game mode
		{
			for( int i=0; i<wordWidth; ++i ) 
			{
				overlapped += bitcount(thisWord[i]);				
			}
		}
	}

	return overlapped;
}

/*#ifdef DEBUG
#include "engine.h"
extern KrEngine *engine;
void KrCollisionMap::Draw(void* surface, int dx, int dy) //maks
{
	int x, y;
	KrPainter painter((SDL_Surface *)surface);
	KrRGBA color;
	color.Set(255, 255, 255);

	for( int j=0; j<cy; ++j )
	{
		U32* thisWord  = GetRow( j );
		
		for( int i=0; i<cx; ++i ) //maks
		{
			for( int n = 31; n >= 0; n--)
			{
				if(thisWord[i] & 1 << n)
				{
					x = i*32 + (31 - n) + dx;
					y = j + dy;
					painter.SetPixel(x, y, color);
				}
			}
		}			
	}

	engine->Draw();

}
#endif*/

