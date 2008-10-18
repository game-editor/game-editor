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

#ifndef STAND_ALONE_GAME //maks

#include "SDL.h"
#include "SDL_endian.h"
#include "tileresource.h"
#include "sdlutil.h"
#include "engine.h"
#include "canvasresource.h"
#include "encoder.h"


const gedString KrTileResource::tileName = "Tile";

KrTileResource::KrTileResource( U32 dataSize, SDL_RWops* data )
{
	gedString name;
	ReadString( data, &name );
	U32 id = SDL_ReadLE32( data );
	SetNameAndId( name, id );

	#ifdef DEBUG
		GLOUTPUT( "Creating tile resource '%s' id=%d\n", name.c_str(), id );
	#endif

	memset( pixelBlock, 0, sizeof( KrPixelBlock* ) * 4 );
	pixelBlock[0] = new KrPixelBlock();
	pixelBlock[0]->Read( data );
	GLASSERT( pixelBlock[0]->Width() == pixelBlock[0]->Height() );
}


KrTileResource::KrTileResource( const gedString& name, KrPaintInfo* surface, int x, int y, int size )
{
	SetNameAndId( name, 0 );
	memset( pixelBlock, 0, sizeof( KrPixelBlock* ) * 4 );
	pixelBlock[0] = new KrPixelBlock();
	pixelBlock[0]->Create( surface, x, y, size, size );
}


KrTileResource::~KrTileResource()
{
	FreeScaleCache();
	for( int i=0; i<4; ++i )
		delete pixelBlock[i];
}


void KrTileResource::CalcSourceAndPitch(	KrPixelBlock* pblock,
											int rotation, 
											KrRGBA** source, 
											int* sPitchX, 
											int* sPitchY )
{
	// We need to figure out a source pitch for each rotation:
	// bytes to advance in source for every target x: sPitchX
	// bytes to advance in source for every target y: sPitchY
	switch ( rotation )
	{
		case 0:
		{
			*source = pblock->Pixels();
			*sPitchX = 1;
			*sPitchY = pblock->Width();
		}
		break;

		case 1:
		{
			*source  =   pblock->LowerLeftPixels();
			*sPitchX = -pblock->Width();
			*sPitchY = 1;
		}
		break;

		case 2:
		{
			*source  =   pblock->LowerRightPixels();
			*sPitchX = -1;
			*sPitchY = -pblock->Width();
		}
		break;

		case 3:
		{
			*source  =   pblock->UpperRightPixels();
			*sPitchX = pblock->Width();
			*sPitchY = -1;
		}
		break;

		case 4:
		{
			*source = pblock->LowerLeftPixels();
			*sPitchX = 1;
			*sPitchY = -pblock->Width();
		}
		break;

		case 5:
		{
			*source  =  pblock->LowerRightPixels();
			*sPitchX = -pblock->Width();
			*sPitchY = -1;
		}
		break;

		case 6:
		{
			*source  =   pblock->UpperRightPixels();
			*sPitchX = -1;
			*sPitchY = pblock->Width();
		}
		break;

		case 7:
		{
			*source  =   pblock->Pixels();
			*sPitchX = pblock->Width();
			*sPitchY = 1;
		}
		break;

		default:
			GLASSERT( 0 );
			return;
	}

}


void KrTileResource::Draw( KrPaintInfo* paintInfo,
						   const KrMatrix2& _xForm,
						   int rotation, 
						   const KrColorTransform& cForm,
						   const KrRect& clipping,
						   int quality,
						   int openGLZ )
{
	if ( paintInfo->OpenGL() ) 
	{
		#ifdef KYRA_SUPPORT_OPENGL
			pixelBlock[0]->DrawOpenGL(	paintInfo,
										_xForm,
										cForm,
										clipping,
										rotation,
										openGLZ );
		#else
			GLASSERT( 0 );
		#endif
		return;
	}
	
	GLASSERT( rotation >=0 && rotation < 8 );
	//GLASSERT( _xForm.xScale == _xForm.yScale );

	KrPixelBlock* pblock = 0;
	KrMatrix2 matrix = _xForm;
	//matrix.xScale = matrix.yScale;

	// Check for a cache hit.
	if ( matrix.IsScaled() )
	{
		for ( unsigned i=0; i<cache.Count(); ++i )
		{
			if (    cache[i].xScale == matrix.xScale
			     && cache[i].yScale == matrix.yScale )
			{
				// Take out the scaling terms, since cached, and draw.
				matrix.xScale = 1;
				matrix.yScale = 1;
				pblock = cache[i].pixelBlock;
				break;
			}
		}
	}
	// Either we aren't scaled or the tile wasn't in the cache.
	if ( !pblock )
	{
		#ifdef ASSERT_IF_NOT_CACHED
			#ifdef DEBUG
				if ( matrix.IsScaled() ) GLASSERT( 0 );
			#endif
		#endif
		
		if ( !pixelBlock[ rotation%4 ] )
		{
			int index = rotation%4;
			// Create the pixel block for this rotation
			pixelBlock[ index ] = new KrPixelBlock();
			pixelBlock[ index ]->Create( pixelBlock[0]->Width(),
										 pixelBlock[0]->Height(),
										 pixelBlock[0]->Alpha() );

			int j;

			KrPaintInfo internalInfo( pixelBlock[index]->Pixels(), pixelBlock[index]->Width(), pixelBlock[index]->Height() ); 
			U8* target =   (U8*) internalInfo.pixels;
			KrRGBA* source = 0;
			int sPitchX, sPitchY;
			CalcSourceAndPitch( pixelBlock[0], index, &source, &sPitchX, &sPitchY );

//			if ( pixelBlock[0]->Alpha() == false )
//			{
				// We can use the fast blitter.
				for ( j = 0; 
					  j < pixelBlock[0]->Height(); 
					  j++ )
				{
					internalInfo.PaintRotated_Simple_NoAlpha(	&internalInfo, //maks
																target,
																source,
																4,
																sPitchX,
																pixelBlock[0]->Width() );
					source += sPitchY;
					target += internalInfo.pitch;
				}
//			}
//			else
//			{
//				// Use the regular blitters, but call them a bunch of times.
//				KrColorTransform none;
// 				KrPaintFunc blitter = internalInfo.GetBlitter( pixelBlock[0]->Alpha(), none );
//				GLASSERT( blitter );
//
//				for ( j = 0; 
//					  j < pixelBlock[0]->Height(); 
//					  j++ )
//				{
//					U8*		subTarget = target;
//					KrRGBA* subSource = source;
//
//					for( i=0; i < pixelBlock[0]->Width(); i++ )
//					{
//						blitter(	&internalInfo,
//									subTarget,
//									subSource,
//									1,
//									none );
//						subTarget += internalInfo.bytesPerPixel;
//						subSource += sPitchX;			
//					}
//					target += internalInfo.pitch;
//					source += sPitchY;
//				}	
//			}
		}
		pblock = pixelBlock[ rotation % 4 ];
	}

	GLASSERT( pblock );

	pblock->Draw(	paintInfo, 
					matrix,
					rotation > 3,
					cForm,
					clipping,

					quality,
					openGLZ );
}


bool KrTileResource::HitTestTransformed( int rotation, int x, int y, int hitFlags )
{
	if ( x >=0 && x < Size() && y >=0 && y < Size() )
	{
		int sPitchX, sPitchY;
		KrRGBA* source;

		CalcSourceAndPitch( pixelBlock[0], rotation, &source, &sPitchX, &sPitchY );
	
		source = source + sPitchX * x + sPitchY * y;

		if ( !pixelBlock[0]->Alpha() )
		{
			return true;
		}

		if ( source->c.alpha != KrRGBA::KR_TRANSPARENT )
		{
			return true;
		}
	}
	return false;
}


void KrTileResource::CacheScale( GlFixed xScale, GlFixed yScale )
{
	GLASSERT( xScale == yScale );
	xScale = yScale;
		
	// check for pre-existing:
	unsigned i = 0;

	for( i=0; i<cache.Count(); ++i )
	{
		if ( cache[i].xScale == xScale && cache[i].yScale == yScale )
		{
			#ifdef DEBUG
				GLOUTPUT( "Scale %f %f already cached in tile resource.\n", xScale.ToDouble(), yScale.ToDouble() );
			#endif
			return;
		}
	}

	KrMatrix2 xForm;
	KrRect    scaledBounds, clip;
	KrColorTransform color;
	KrPixelBlock* pblock = new KrPixelBlock();

	// First, figure out how big we would be if we were at this 
	// scale. Then create a pixelblock of that size. Draw with 
	// the standard pixel block to the scaled one.

	xForm.Set( 0, 0, xScale, yScale );
	pixelBlock[0]->CalculateBounds( xForm, &scaledBounds );
	GLASSERT( scaledBounds.xmin == 0 && scaledBounds.ymin == 0 );

	// Create a pixel block to back this one. Alpha is never introduced
	// in a rectangular canvas.
	pblock->Create( scaledBounds.Width(), scaledBounds.Height(), Alpha() ? true : false );

	KrPaintInfo info( pblock->Pixels(), pblock->Width(), pblock->Height() );		
	clip.Set( 0, 0, pblock->Width()-1, pblock->Height()-1 );
	
	pixelBlock[0]->Draw( &info, xForm, false, color, clip, 
		KrQualityAdaptive, 
		0 );

	CachedBlock block;
	block.pixelBlock = pblock;
	block.xScale = xScale;
	block.yScale = yScale;

	cache.PushBack( block );
}


bool KrTileResource::IsScaleCached( GlFixed xScale, GlFixed yScale )
{
	for ( unsigned i=0; i<cache.Count(); ++i )
	{
		if (    cache[i].xScale == xScale 
			 && cache[i].yScale == yScale )
		{
			return true;
		}
	}
	return false;
}


void KrTileResource::FreeScaleCache()
{
	for( unsigned i=0; i<cache.Count(); ++i )
	{
		delete cache[i].pixelBlock;
	}
	cache.SetCount( 0 );
}


#ifndef STAND_ALONE_GAME //maks
void KrTileResource::Save( KrEncoder* encoder )
{
	encoder->StartTag( KYRATAG_TILE );

	WriteString( encoder->Stream(), ResourceName() );
	encoder->WriteCached( ResourceName() );

	pixelBlock[0]->Write( encoder->Stream() );

	U32 blockRGBA = 0;
	pixelBlock[0]->CountComponents( &blockRGBA );
	encoder->AddCount( 0, 0, blockRGBA );

	encoder->EndTag();
}
#endif


void KrTileResource::CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const
{
	bounds->SetInvalid();
//	if ( xForm.IsScaled() )
//	{
//		GlFixed scaleWidthF = Size();
//		GlFixed scaleHeightF = Size();
//		scaleWidthF  *= xForm.xScale;
//		scaleHeightF *= xForm.yScale;
//		int scaleW = scaleWidthF.ToIntRound();
//		int scaleH = scaleHeightF.ToIntRound();
//
//		bounds->xmin = xForm.x.ToIntRound();
//		bounds->ymin = xForm.y.ToIntRound();
//		bounds->xmax = bounds->xmin + scaleW - 1;
//		bounds->ymax = bounds->ymin + scaleH - 1;
//    }
//	else
//	{
//		bounds->xmin = xForm.x.ToIntRound();
//		bounds->ymin = xForm.y.ToIntRound();
//		bounds->xmax = bounds->xmin + Size() - 1;
//		bounds->ymax = bounds->ymin + Size() - 1;
//	}
	pixelBlock[0]->CalculateBounds( xForm, bounds );
}

#endif //#ifndef STAND_ALONE_GAME //maks
