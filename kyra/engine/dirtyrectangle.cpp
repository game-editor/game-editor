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
#include "dirtyrectangle.h"
#include "painter.h"
//#include <algorithm> //maks


const U8 RectMap[] = 
	{   1,   3,   7,  15,  31,  63, 127, 255,
	    0,   2,   6,  14,  30,  62, 126, 254,
	    0,   0,   4,  12,  28,  60, 124, 252,
	    0,   0,   0,   8,  24,  56, 120, 248,
	    0,   0,   0,   0,  16,  48, 112, 240,
	    0,   0,   0,   0,   0,  32,  96, 224,
	    0,   0,   0,   0,   0,   0,  64, 192,
	    0,   0,   0,   0,   0,   0,   0, 128  };


void KrMappedRectInfo::Set( const KrRect& bounds )
{
	xmin = bounds.xmin;
	ymin = bounds.ymin;

	hPixelsPerBit = ( bounds.Width() + 7 ) / 8;
	GLASSERT( hPixelsPerBit > 0 );

	vPixelsPerRow = ( bounds.Height() + 3 ) / 4;
	GLASSERT( vPixelsPerRow > 0 );
}


void KrMappedRect::CalcMap( const KrMappedRectInfo& info )
{
	GLASSERT( IsValid() );
	GLASSERT( xmin >= info.xmin );
	GLASSERT( ymin >= info.ymin );

	int minbit = ( xmin - info.xmin ) / info.hPixelsPerBit;
	int maxbit = ( xmax - info.xmin ) / info.hPixelsPerBit;

	GLASSERT( minbit >= 0 && minbit < 8 );
	GLASSERT( maxbit >= minbit && maxbit < 8 );

	U8 val = RectMap[ minbit*8 + maxbit ];

	int minrow = ( ymin - info.ymin ) / info.vPixelsPerRow;
	int maxrow = ( ymax - info.ymin ) / info.vPixelsPerRow;

	GLASSERT( minrow >= 0 && minrow < 4 );
	GLASSERT( maxrow >= minrow && maxrow < 4 );

	map = 0;
	for ( int i=minrow; i<=maxrow; ++i )
	{
		map |= val << ( i * 8 );
	}
}



///////////////////////////////////////////////////
KrDirtyRectangle::KrDirtyRectangle()
{
	nRect = 0;
	//nBlitRect = 0;
	clipping = false;

	KrRect fake;
	fake.Set( 0, 0, 80, 80 );
	mappedInfo.Set( fake );
}


KrDirtyRectangle::~KrDirtyRectangle()
{}


/*
void KrDirtyRectangle::UpdateScreen( SDL_Surface* screen )
{
	SDL_Rect sdlrect[ MAX_DIRTY_RECTANGLES ];

	for( int i=0; i<nRect; i++ )
	{
		sdlrect[i].x = rectArray[i].xmin;
		sdlrect[i].y = rectArray[i].ymin;
		sdlrect[i].w = rectArray[i].Width();
		sdlrect[i].h = rectArray[i].Height();
		
		GLASSERT( sdlrect[i].x >= 0 );
		GLASSERT( sdlrect[i].y >= 0 );
		GLASSERT( sdlrect[i].w <= screen->w );
		GLASSERT( sdlrect[i].h <= screen->h );
	}

	SDL_UpdateRects( screen, nRect, sdlrect );
}
*/


void KrDirtyRectangle::Remove( int index )
{
	// Take out the current rect by swapping it with the last.
	if ( nRect > 1 )
	{
		rectArray[index] = rectArray[ nRect-1 ];
	}
	nRect--;
}

/*
void KrDirtyRectangle::RemoveBlt( int index )
{
	// Take out the current rect by swapping it with the last.
	if ( nBlitRect > 1 )
	{
		blitArray[index].rect = blitArray[ nBlitRect-1 ].rect;
	}
	nBlitRect--;
}
*/

void KrDirtyRectangle::AddRectangle( const KrRect& r )
{
	// These limits that are checked are arbitrary - there
	// is nothing wrong with having rects out of these limits...but
	// it is a good way to 'reality check' the inputs.
	/*GLASSERT( r.xmin > -100000 );
	GLASSERT( r.ymin > -100000 );
	GLASSERT( r.xmax < 100000 );
	GLASSERT( r.ymax < 100000 );
	GLASSERT( clipping );*/

	KrRect rtemp = r;
	if ( clipping )
	{
		rtemp.DoIntersection( clippingRect );
	}

	if ( !rtemp.IsValid() )
		return;

	KrMappedRect rect( rtemp, mappedInfo );

	//
	//}
	//
	//
	//void KrDirtyRectangle::AddDRRect( const KrRect& r )
	//{
	//	KrRect rect = r;

	//   	GlSList<KrRect> rectStack;
	//   	rectStack.PushFront( rect );
	//		RectStack rectStack( rect );

	//   	while( !rectStack.Empty() )
	//   	{
	//   		rect = rectStack.Front();
	//   		rectStack.PopFront();

   	int i = 0;
   	bool added = false;

   	while( i < nRect )
   	{
   		if ( rectArray[i].Intersect( rect ) )
   		{
   			if ( rectArray[i].HasInside( rect ) )
   			{
   				// We can safely toss the rectangle added.
   				added = true;
   				break;
   			}

   			// Do we create a union or band?
   			// First case checks for a union
   			if (    rect.HasInside( rectArray[i] )
   				 || ( rect.xmin == rectArray[i].xmin && rect.xmax == rectArray[i].xmax )
   				 || ( rect.ymin == rectArray[i].ymin && rect.ymax == rectArray[i].ymax ) )
   			{
   				// Merge the 2 rectangles, remove the current
   				// rectangle, and do the add again.
   				rect.DoUnion( rectArray[i] );
				rect.CalcMap( mappedInfo );
   				Remove( i );
   				i = 0;
   			}
   			else
   			{
   				// Band.
   				// The if cases for this are overwhelming. The
   				// case is slower, but you can sort of see
   				// what is going on.

   				const int XMINLOWER   = 1<<0;
   				const int XMAXGREATER = 1<<1;
   				const int YMINLOWER   = 1<<2;
   				const int YMAXGREATER = 1<<3;
   				int geom = 0;

   				if ( rect.xmin < rectArray[i].xmin )
   					geom |= XMINLOWER;
   				if ( rect.xmax > rectArray[i].xmax )
   					geom |= XMAXGREATER;
   				if ( rect.ymin < rectArray[i].ymin )
   					geom |= YMINLOWER;
   				if ( rect.ymax > rectArray[i].ymax )
   					geom |= YMAXGREATER;

   				switch ( geom )
   				{
   					case XMINLOWER | XMAXGREATER | YMINLOWER | YMAXGREATER:
   					case 0:
   					{
   						// should have been merged or tossed.
   						GLASSERT( 0 );
   					}
   					break;
						
   					// Cases where the existing rectArray entry is mostly covered
   					// by the new rectangle. We trim the rectArray and keep looking.
   					case XMINLOWER | XMAXGREATER | YMAXGREATER:
   					{
   						rectArray[i].ymax = rect.ymin - 1;
   						GLASSERT( rectArray[i].IsValid() );
						rectArray[i].CalcMap( mappedInfo );
   					}
   					break;

   					case XMINLOWER | XMAXGREATER | YMINLOWER:
   					{
   						rectArray[i].ymin = rect.ymax + 1;
   						GLASSERT( rectArray[i].IsValid() );
						rectArray[i].CalcMap( mappedInfo );
   					}
   					break;

   					case XMINLOWER | YMINLOWER | YMAXGREATER:
   					{
   						rectArray[i].xmin = rect.xmax + 1;
   						GLASSERT( rectArray[i].IsValid() );
						rectArray[i].CalcMap( mappedInfo );
   					}
   					break;

   					case XMAXGREATER | YMINLOWER | YMAXGREATER:
   					{
   						rectArray[i].xmax = rect.xmin - 1;
   						GLASSERT( rectArray[i].IsValid() );
						rectArray[i].CalcMap( mappedInfo );
   					}
   					break;

   					// Cases where the added rect is a panhandle
   					// to the existing rectArray.
   					// We trim the rect and keep looking.

					// Note: this can lead to some small panhandles
					// in common cases. May need to test and 
					// union. (see the text test case.)
   					case XMINLOWER:
   					{
   						rect.xmax = rectArray[i].xmin - 1;
   						GLASSERT( rect.IsValid() );
						rect.CalcMap( mappedInfo );
   					}
   					break;

   					case XMAXGREATER:
   					{
   						rect.xmin = rectArray[i].xmax + 1;
   						GLASSERT( rect.IsValid() );
						rect.CalcMap( mappedInfo );
   					}
   					break;

   					case YMINLOWER:
   					{
   						rect.ymax = rectArray[i].ymin - 1;
   						GLASSERT( rect.IsValid() );
						rect.CalcMap( mappedInfo );
   					}
   					break;

   					case YMAXGREATER:
   					{
   						rect.ymin = rectArray[i].ymax + 1;
   						GLASSERT( rect.IsValid() );
						rect.CalcMap( mappedInfo );
   					}
   					break;

   					/*
   					// These may not speed it up -- I get different results for different tests.
   					// Cases where the rect sticks out the sides. We keep going
   					// with one chunk and add the other chunk to the stack.
   					case YMINLOWER | YMAXGREATER:
   					{
   						// The bottom chunk is 'extra'
   						KrRect extra = rect;
   						extra.ymin = rectArray[i].rect.ymax + 1;
   						GLASSERT( extra.IsValid() );
   						rectStack.PushFront( extra );

   						// And we trim the piece we are trying to add
   						rect.ymax = rectArray[i].rect.ymin - 1;
   						GLASSERT( rect.IsValid() );
   					}
   					break;
						
   					case XMINLOWER | XMAXGREATER:
   					{
   						KrRect extra = rect;
   						extra.xmin = rectArray[i].rect.xmax + 1;
   						GLASSERT( extra.IsValid() );
   						rectStack.PushFront( extra );
							
   						rect.xmax = rectArray[i].rect.xmin - 1;
   						GLASSERT( rect.IsValid() );
   					}
   					break;
						
   					// The 3 bands -- aka the complex cases. Changes both
   					// the existing code and the rectangle.
   					case XMINLOWER | YMINLOWER:
   					{
   						// Note that it is possible for band#3 to be empty, if
   						// ymax == rect.ymax
							
   						KrRect	r2 = rect,
   								r1 = rect;
   						r1.ymax = rectArray[i].rect.ymax - 1;
   						r2.ymin = rectArray[i].rect.ymin;
   						r2.xmax = rectArray[i].rect.xmax;
   						rectArray[i].rect.ymin = rect.ymax + 1;
							
   						if ( !rectArray[i].rect.IsValid() )
   							Remove( i );
								
   						GLASSERT( r1.IsValid() );
   						GLASSERT( r2.IsValid() );
							
   						rectStack.PushFront( r1 );
   						rectStack.PushFront( r2 );
   					}
   					break;
							
   					case XMINLOWER | YMAXGREATER:
   					{
   						KrRect 	r2 = rect,
   						 		r3 = rect;
   						r2.ymax = rectArray[i].rect.ymax;
   						r2.xmax = rectArray[i].rect.xmax;
   						r3.ymin = rectArray[i].rect.ymax + 1;
   						rectArray[i].rect.ymax = rect.ymin - 1;
							
   						if ( !rectArray[i].rect.IsValid() )
   							Remove( i );
								
   						GLASSERT( r2.IsValid() );
   						GLASSERT( r3.IsValid() );
							
   						rectStack.PushFront( r2 );
   						rectStack.PushFront( r3 );							 	
   					}
   					break;
						
   					case XMAXGREATER | YMINLOWER:
   					{
   						KrRect 	r1 = rect,
   						 		r2 = rect;
   						r1.ymax = rectArray[i].rect.ymin - 1;
   						r2.ymin = rectArray[i].rect.ymin;
   						r2.xmin = rectArray[i].rect.xmin;
   						rectArray[i].rect.ymin = rect.ymax + 1;
							
   						if ( !rectArray[i].rect.IsValid() )
   							Remove( i );
								
   						GLASSERT( r1.IsValid() );
   						GLASSERT( r2.IsValid() );
							
   						rectStack.PushFront( r1 );
   						rectStack.PushFront( r2 );							 	
   					}
   					break;
						
   					case XMAXGREATER | YMAXGREATER:
   					{
   						KrRect 	r2 = rect,
   						 		r3 = rect;
   						r2.ymax = rectArray[i].rect.ymax;
   						r2.xmin = rectArray[i].rect.xmin;
   						r3.ymin = rectArray[i].rect.ymax + 1;
   						rectArray[i].rect.ymax = rect.ymin - 1;
							
   						if ( !rectArray[i].rect.IsValid() )
   							Remove( i );
								
   						GLASSERT( r2.IsValid() );
   						GLASSERT( r3.IsValid() );
							
   						rectStack.PushFront( r2 );
   						rectStack.PushFront( r3 );							 	
   					}
   					break;
   					*/
												
   					default:
					{
        				rect.DoUnion( rectArray[i] );
						rect.CalcMap( mappedInfo );
        				Remove( i );
        				i = 0;
					}
   					break;
   				}
   			}
   		}
   		else
   		{
   			++i;
   		}
   	}		// end while

   	// Is there still a rectangle to be added?
   	if ( !added )
   	{
   		if ( nRect < MAX_DIRTY_RECTANGLES )
   		{
   			rectArray[nRect] = rect;
   			nRect++;
   		}
   		else
   		{
   			// We're out of Rectangles!
   			// Start merging. There is a *very* subtle problem that can arise.
   			// If a rectangle is split, it will be merged back to itself, then
   			// can be split again. To avoid this, one more merge must happen than
   			// the potential # of rects that can be created.
				
//   			for( int j=0; j<nRect; j++ )
//			{
//   				rect.DoUnion( rectArray[j] );
//			}
//   			nRect = 1;
//   			rectArray[0] = rect;
//			rectArray[0].CalcMap( mappedInfo );

			#ifdef DEBUG
				//GLOUTPUT( "NOTE: ------ DR Flush ------------\n" );
			#endif
			HandleOutOfRect( rect );
   		}
   	}
	#ifdef DEBUG
		// Check that nothing overlaps and is correct.
		// Very useful...and very slow.
		/*
		for ( i=0; i<nRect; ++i )
		{
			U32 map = rectArray[i].Map();
			rectArray[i].CalcMap( mappedInfo );
			GLASSERT( map == rectArray[i].Map() );

			KrRect& slice = rectArray[i];

			GLASSERT( slice.IsValid() );

			for( int j=i+1; j<nRect; ++j )
			{
				KrRect& slice2 = rectArray[j];
				// If this fires, some sort of strange internal error.
				GLASSERT( !slice.Intersect( slice2 ) );
			}
		}
		*/
	#endif

//		if (    rectArray[nRect-1].xmin == 0 && rectArray[nRect-1].ymin == 499
//		     && rectArray[nRect-1].xmax == 32 && rectArray[nRect-1].ymax == 531 )
//			int debug = 2;
}

void KrDirtyRectangle::HandleOutOfRect( const KrMappedRect& r )
{
	#ifdef OLDWAY

		KrMappedRect rect = r;

   		for( int j=0; j<nRect; j++ )
   			rect.DoUnion( rectArray[j] );

   		nRect = 1;
   		rectArray[0] = rect;
		rectArray[0].CalcMap( mappedInfo );

	#else

	if (    MAX_DIRTY_RECTANGLES <= 32
		 ||	mappedInfo.hPixelsPerBit <= 32
		 || mappedInfo.vPixelsPerRow <= 32 )
	{
		KrMappedRect rect = r;

   		for( int j=0; j<nRect; j++ )
   			rect.DoUnion( rectArray[j] );

   		nRect = 1;
   		rectArray[0] = rect;
		rectArray[0].CalcMap( mappedInfo );
	}
	else
	{
		int i;

		U32 map = r.Map();
   		for( i=0; i<nRect; i++ )
   			map |= rectArray[i].Map();

		Clear();

		// Now pull out rectangles.
		if ( map == 0xffffffff )
		{
			// All one big rectangle.
			rectArray[0].Set( clippingRect, mappedInfo );
			nRect = 1;
			map = 0;
		}
		else
		{
			// Pull out the vertical parts, then
			// the individual bits.
			for( i=0; i<4; ++i )
			{
				U32 mask = 0xff << (i*8);
				if ( ( map & mask ) == mask )
				{
					KrRect r;
					r.Set(	mappedInfo.xmin,
							mappedInfo.ymin + mappedInfo.vPixelsPerRow * i,
							clippingRect.xmax,
							mappedInfo.ymin + mappedInfo.vPixelsPerRow * (i+1) - 1 );

					r.DoIntersection( clippingRect );	// it can be out of bounds from above
					rectArray[nRect].Set( r, mappedInfo );
					++nRect;
					map &= ~mask;
				}
			}

			// Now individual parts:
			for( i=0; i<32; ++i )
			{
				U32 mask = ( 1 << i );
				if ( mask & map )
				{
					int x = i & 7;
					int y = i / 8;

					KrRect r;
					r.Set(	mappedInfo.xmin + x * mappedInfo.hPixelsPerBit,
							mappedInfo.ymin + y * mappedInfo.vPixelsPerRow,
							mappedInfo.xmin + (x+1) * mappedInfo.hPixelsPerBit - 1,
							mappedInfo.ymin + (y+1) * mappedInfo.vPixelsPerRow - 1 );

					r.DoIntersection( clippingRect );	// it can be out of bounds from above
					rectArray[nRect].Set( r, mappedInfo );
					++nRect;
				}
			}
		}
	}
	#endif
}


// #ifdef DEBUG
// void KrDirtyRectangle::DrawAllRects( SDL_Surface* surface )
// {
// 	KrPainter painter( surface );
// 
// 	for ( unsigned i=0; i<allRects.Count(); i++ )
// 	{
// 		painter.DrawBox( allRects[i].xmin, allRects[i].ymin,
// 						 allRects[i].Width(), allRects[i].Height(), 
// 						 200, 0, 0 );
// 	}
// }
// 

#if defined( DRAWDEBUG_RLE ) || defined( DRAWDEBUG_BLTRECTS )
	extern int debugFrameCounter;

	void KrDirtyRectangle::DrawRects( SDL_Surface* surface )
	{
		KrPainter painter( surface );

		for( int i=0; i<nRect; i++ )
		{
			KrRGBA color;
			if ( debugFrameCounter & 0x01 )
				color.Set( 0, 200, 50 );
			else
				color.Set( 0, 50, 200 );
			painter.DrawBox( rectArray[i].xmin,
							 rectArray[i].ymin,
							 rectArray[i].Width(),
							 rectArray[i].Height(),
							 color.c.red, color.c.green, color.c.blue );
		}
	}

#endif



// void KrDirtyRectangle::DrawBlitRects( SDL_Surface* surface )
// {
// 	KrPainter painter( surface );
// 
// 	for( int i=0; i<nBlitRect; i++ )
// 	{
// 		painter.DrawBox( blitArray[i].rect.xmin,
// 						 blitArray[i].rect.ymin,
// 						 blitArray[i].rect.Width(),
// 						 blitArray[i].rect.Height(),
// 						 0, 0, 255 );
// 	}
// }
// #endif


//void KrDirtyRectangle::Clear()
//{
//// 	#ifdef DEBUG
//// 		allRects.Clear();
//// 	#endif
//	nRect = 0;
//	//nBlitRect = 0;
//}


// void KrDRIterator::NextNode()
// {
// 	for ( ; node; node = node->next )
// 	{
// 		if ( node->rect.ymin > y1 )
// 		{
// 			// too far!
// 			node = 0;
// 			break;
// 		}
// 		else if ( node->rect.ymax < y0 )
// 				  // || node->rect.ymin > y1 ) -- checked in previous case.
// 		{
// 			// node does not intersect
// 			continue;
// 		}
// 		// If we did not go to far or did not intersect,
// 		// we found a node.
// 		break;
// 	}
// // 	#ifdef DEBUG
// // 		if ( node ) GLASSERT( node->rect.ymax >= y0 ||
// // 							  node->rect.ymin <= y1 );
// // 	#endif
// }


// #ifdef DEBUG
// void KrDRIterator::DebugDump()
// {
// 	Begin();
// 	while ( !Done() )
// 	{
// 		GLOUTPUT( "  rect (%d,%d) - (%d,%d)\n",
// 				  Rectangle().xmin,
// 				  Rectangle().ymin,
// 				  Rectangle().xmax,
// 				  Rectangle().ymax );
// 		Next();
// 	}
// }
// #endif


/*
void KrDirtyRectangle::AddBlitRect( const KrRect& r )
{
	KrRect rect = r;

   	int i = 0;
   	GlSList<KrRect> rectStack;
   	rectStack.PushFront( rect );

   	while( !rectStack.Empty() )
   	{
   		rect = rectStack.Front();
   		rectStack.PopFront();
   		i = 0;
   		bool added = false;

   		while( i < nBlitRect )
   		{
   			if ( blitArray[i].rect.Intersect( rect ) )
   			{
   				if ( blitArray[i].rect.HasInside( rect ) )
   				{
   					// We can safely toss the rectangle added.
   					added = true;
   					break;
   				}

   				// Do we create a union or band?
   				// First case checks for a union
   				if (    rect.HasInside( blitArray[i].rect )
   					 || ( rect.xmin == blitArray[i].rect.xmin && rect.xmax == blitArray[i].rect.xmax )
   					 || ( rect.ymin == blitArray[i].rect.ymin && rect.ymax == blitArray[i].rect.ymax ) )
   				{
   					// Merge the 2 rectangles, remove the current
   					// rectangle, and do the add again.
   					rect.DoUnion( blitArray[i].rect );
   					RemoveBlt( i );
   					i = 0;
   				}
   				else
   				{
   					// Band.
   					// The if cases for this are overwhelming. The
   					// case is slower, but you can sort of see
   					// what is going on.

   					const int XMINLOWER   = 1<<0;
   					const int XMAXGREATER = 1<<1;
   					const int YMINLOWER   = 1<<2;
   					const int YMAXGREATER = 1<<3;
   					int geom = 0;

   					if ( rect.xmin < blitArray[i].rect.xmin )
   						geom |= XMINLOWER;
   					if ( rect.xmax > blitArray[i].rect.xmax )
   						geom |= XMAXGREATER;
   					if ( rect.ymin < blitArray[i].rect.ymin )
   						geom |= YMINLOWER;
   					if ( rect.ymax > blitArray[i].rect.ymax )
   						geom |= YMAXGREATER;

   					switch ( geom )
   					{
   						case XMINLOWER | XMAXGREATER | YMINLOWER | YMAXGREATER:
   						case 0:
   						{
   							// should have been merged or tossed.
   							GLASSERT( 0 );
   						}
   						break;

   						// Cases where the existing blitArray entry is mostly covered
   						// by the new rectangle. We trim the blitArray and keep looking.
   						case XMINLOWER | XMAXGREATER | YMAXGREATER:
   						{
   							blitArray[i].rect.ymax = rect.ymin - 1;
   							GLASSERT( blitArray[i].rect.IsValid() );
   						}
   						break;

   						case XMINLOWER | XMAXGREATER | YMINLOWER:
   						{
   							blitArray[i].rect.ymin = rect.ymax + 1;
   							GLASSERT( blitArray[i].rect.IsValid() );
   						}
   						break;

   						case XMINLOWER | YMINLOWER | YMAXGREATER:
   						{
   							blitArray[i].rect.xmin = rect.xmax + 1;
   							GLASSERT( blitArray[i].rect.IsValid() );
   						}
   						break;

   						case XMAXGREATER | YMINLOWER | YMAXGREATER:
   						{
   							blitArray[i].rect.xmax = rect.xmin - 1;
   							GLASSERT( blitArray[i].rect.IsValid() );
   						}
   						break;

   						// Cases where the added rect is a panhandle
   						// to the existing blitArray.
   						// We trim the rect and keep looking.
   						case XMINLOWER:
   						{
   							rect.xmax = blitArray[i].rect.xmin - 1;
   							GLASSERT( rect.IsValid() );
   						}
   						break;

   						case XMAXGREATER:
   						{
   							rect.xmin = blitArray[i].rect.xmax + 1;
   							GLASSERT( rect.IsValid() );
   						}
   						break;

   						case YMINLOWER:
   						{
   							rect.ymax = blitArray[i].rect.ymin - 1;
   							GLASSERT( rect.IsValid() );
   						}
   						break;

   						case YMAXGREATER:
   						{
   							rect.ymin = blitArray[i].rect.ymax + 1;
   							GLASSERT( rect.IsValid() );
   						}
   						break;

   						// These cases don't seem to work for blit rects:
   						case YMINLOWER | YMAXGREATER:
   						case XMINLOWER | XMAXGREATER:
   						case XMINLOWER | YMINLOWER:
   						case XMINLOWER | YMAXGREATER:
   						case XMAXGREATER | YMINLOWER:
   						case XMAXGREATER | YMAXGREATER:
   						{
        					// Merge the 2 rectangles, remove the current
        					// rectangle, and do the add again.
        					rect.DoUnion( blitArray[i].rect );
        					RemoveBlt( i );
        					i = 0;
        				}
   						break;

   						default:
   							GLASSERT( 0 );
   						break;
   					}
   				}
   			}
   			else
   			{
   				i++;
   			}
   		}		// end while

   		// Is there still a rectangle to be added?
   		if ( !added )
   		{
   			if ( nBlitRect < MAX_BLIT_RECTANGLES )
   			{
   				blitArray[nBlitRect].rect = rect;
   				nBlitRect++;
   			}
   			else
   			{
   				// We're out of Rectangles!
   				// Start merging. There is a *very* subtLe problem that can arise.
   				// If a rectangle is split, it will be merged back to itself, then
   				// can be split again. To avoid this, one more merge must happen than
   				// the potential # of rects that can be created.
   				//
   				// Risky -- so the entire list is merged. The best thing is to make sure
   				// that there is enough space in the array for all the rects.

   				for( int j=0; j<nBlitRect; j++ )
   					rect.DoUnion( blitArray[j].rect );

   				#ifdef DEBUG
   					GLOUTPUT( "Blit Rectangles Merged!\n" );
   				#endif

   				nBlitRect = 1;
   				blitArray[0].rect = rect;
   			}
   		}
   	}
}
*/


#ifdef DEBUG
void KrDirtyRectangle::PrintRects( const char* message )
{
	GLOUTPUT( "DR Print. '%s'\n", message );
	for ( int i=0; i<nRect; ++i )
	{
		GLOUTPUT( "  %d,%d -- %d,%d\n", rectArray[i].xmin, rectArray[i].ymin, rectArray[i].xmax, rectArray[i].ymax );

	}
}
#endif
