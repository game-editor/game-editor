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
#include "pixelblock.h"
#include "SDL_rwops.h"
#include "SDL_endian.h"
#include "../util/glmemorypool.h"
#include "engine.h"
#include "ogltexture.h"
#include "../../gameEngine/compression.h" //maks

#include "../../gameEngine/dlmalloc.h" //maks
#ifdef __iPhone__
#include "SDL_sysvideo.h"
#endif
#ifdef DEBUG
	U32 KrPixelBlock::numRGBA = 0;
#endif



KrPixelBlock::KrPixelBlock()
{
	flags = 0;
	undo1 = undo2 = block = NULL;
	size.x = size.y = 0;

#ifdef KYRA_SUPPORT_OPENGL
	texture = 0;
#endif
}


KrPixelBlock::~KrPixelBlock()
{
	if ( ! ( flags & MEMORYPOOL ) )
		free(block);

	if(undo1) free(undo1);
	if(undo2) free(undo2);

	RemoveTexture(); //maks
}

void KrPixelBlock::RemoveTexture()
{
#ifdef KYRA_SUPPORT_OPENGL
	if(texture) //maks
	{
		KrTextureManager::RemoveTextureOwner(&texture);
		delete texture; 
		texture = NULL;
	}
#endif
}

bool KrPixelBlock::Create(	KrPaintInfo* surface,
							int x, int y, int width, int height )
{
	int i, j;

	size.x = width;
	size.y = height;

	KrPainter painter( surface );
	GLASSERT( block == 0 );
	block = (KrRGBA *)malloc(width*height*sizeof(KrRGBA)); //maks
	flags = 0;

	for( i=0; i<width; i++ )
	{
		for( j=0; j<height; j++ )
		{
			KrRGBA rgba;
			painter.BreakPixel( x+i, y+j, &rgba );

			// Check for alpha:
			if ( rgba.c.alpha != KrRGBA::KR_OPAQUE )
			{
				flags |= ALPHA;
			}
			block[ j*width + i ] = rgba;
		}
	}
	return true;
}


bool KrPixelBlock::Create( int width, int height, bool alphaSupport )
{
	flags = 0;
	if ( alphaSupport) flags |= ALPHA;
	size.x = width;
	size.y = height;
	GLASSERT( block == 0 );

	block = (KrRGBA *)malloc(width*height*sizeof(KrRGBA)); //maks	
	//memset( block, 0, width * height * sizeof( KrRGBA ) );
	
	// Need to get alpha channel correct for openGL
	KrRGBA color;
	color.Set( 0, 0, 0, alphaSupport ? 0 : 255 );	// if alpha not supported, surface is black, else transparent
	U32 v = color.all;
	U32 count = width * height;
	U32* block32 = (U32*) block;

	for( U32 i=0; i<count; ++i )
		block32[i] = v;

	return true;
}

void KrPixelBlock::SetDimensions(int width, int height) //maks
{
	if(width < 15 || height < 15) return;

	KrRGBA color;
	color.Set(0, 0, 0, 1);


	//realloc don't works visually
	KrRGBA *tmp = (KrRGBA *)malloc(width*height*sizeof(KrRGBA));

	//Copy
	int i, j;
	for( i=0; i<width; i++ )
	{
		for( j=0; j<height; j++ )
		{
			if(i < size.x && j < size.y)
			{
				tmp[ j*width + i ] = block[ j*size.x + i ];
			}
			else
			{
				tmp[ j*width + i ] = color;
			}
		}
	}


	size.x = width;
	size.y = height;

	free(block);
	block = tmp;
}

void KrPixelBlock::SaveBuffer(int iBuffer) //maks
{
	if(iBuffer == 0)
	{
		if(undo1) free(undo1);
		
		undo1 = (KrRGBA *)malloc(size.x*size.y*sizeof(KrRGBA)); 
		memcpy(undo1, block, size.x*size.y*sizeof(KrRGBA));
	}
	else
	{
		if(undo2) free(undo2);
		
		undo2 = (KrRGBA *)malloc(size.x*size.y*sizeof(KrRGBA)); 
		memcpy(undo2, block, size.x*size.y*sizeof(KrRGBA));
	}
}

void KrPixelBlock::RestoreBuffer(int iBuffer) //maks
{
	if(iBuffer == 0)
	{
		if(!undo1) return;

		memcpy(block, undo1, size.x*size.y*sizeof(KrRGBA));
		
		free(undo1);
		undo1 = NULL;
	}
	else
	{
		if(!undo2) return;

		memcpy(block, undo2, size.x*size.y*sizeof(KrRGBA));
		
		free(undo2);
		undo2 = NULL;
	}

#ifdef KYRA_SUPPORT_OPENGL
	LoadNewTexture();
#endif
}

#ifndef STAND_ALONE_GAME //maks



bool KrPixelBlock::Write( SDL_RWops* fp ) //maks
{
	#ifdef DEBUG
		GLOUTPUT( "Writing tile size=(%d,%d) (", size.x, size.y );
		if ( flags & ALPHA ) GLOUTPUT( "ALPHA " );
		GLOUTPUT( ")\n" );
	#endif
	SDL_WriteLE32( fp, flags );
	SDL_WriteLE32( fp, size.x );
	SDL_WriteLE32( fp, size.y );

	//Compress
	int lenght = size.x * size.y * sizeof(KrRGBA);
	U8 *out = new U8[lenght + lenght / 8 + 256];
	if(!out) return false;

	Compression comp;
	int lenghtComp = comp.DoCompression(block, lenght, out, lenght + lenght / 8 + 256);

	SDL_WriteLE32( fp, lenghtComp );
	SDL_RWwrite(fp, out, lenghtComp, 1);

	/*for ( int i=0; i<( size.x * size.y ); i++ )
	{
		SDL_RWwrite( fp, &block[i].c.red,   1, 1);
		SDL_RWwrite( fp, &block[i].c.green, 1, 1);
		SDL_RWwrite( fp, &block[i].c.blue,  1, 1);
		if ( flags & ALPHA )
			SDL_RWwrite( fp, &block[i].c.alpha, 1, 1);
	}*/

	delete [] out;
	return true;
}
#endif


bool KrPixelBlock::Read( SDL_RWops* data ) //maks
{
	if(block)
	{
		free(block);
	}

	flags = SDL_ReadLE32( data );
	size.x = SDL_ReadLE32( data );
	size.y = SDL_ReadLE32( data );
	

	block = 0;
	if ( KrRle::memoryPoolRGBA )
	{
		block = (KrRGBA*) KrRle::memoryPoolRGBA->Alloc( size.x * size.y * sizeof( KrRGBA ) );
		flags |= MEMORYPOOL;
	}

	if ( !block )
	{
		block = (KrRGBA *)malloc(size.x*size.y*sizeof(KrRGBA)); //maks
	}

	#ifdef DEBUG
		numRGBA += size.x * size.y;
	#endif

	//Decompression
	int lenghtComp = SDL_ReadLE32( data );
	U8 *in = new U8[lenghtComp];
	if(!in) return false;

	SDL_RWread( data, in, lenghtComp, 1);

	Compression comp;			
	if(!comp.Decompression(in, lenghtComp, block, size.x*size.y*sizeof(KrRGBA)))
	{
		delete [] in;
		return false;
	}

	/*for ( int i=0; i<size.x*size.y; i++ )
	{
		SDL_RWread( data, &block[i].c.red,   1, 1);
		SDL_RWread( data, &block[i].c.green, 1, 1);
		SDL_RWread( data, &block[i].c.blue,  1, 1);

		if ( flags & ALPHA )
			SDL_RWread( data, &block[i].c.alpha, 1, 1);
		else
			block[i].c.alpha = 255;
	}*/


	delete [] in;

	return true;
}


void KrPixelBlock::CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const
{
	bounds->xmin = xForm.x.ToIntRound();
	bounds->ymin = xForm.y.ToIntRound();
	bounds->xmax = ( xForm.xScale * size.x + xForm.x ).ToIntRound() - 1;
	bounds->ymax = ( xForm.yScale * size.y + xForm.y ).ToIntRound() - 1;

	#ifdef DEBUG
		if ( xForm.xScale == 1 && xForm.yScale == 1 )
		{
			GLASSERT( bounds->Height() == size.y );
			GLASSERT( bounds->Width()  == size.x );
		}
		else
		{
			GLASSERT( bounds->Width() <= ( xForm.xScale * size.x ).ToIntRound() );
			GLASSERT( bounds->Width() >= ( xForm.xScale * size.x ).ToInt() );
			GLASSERT( bounds->Height()<= ( xForm.yScale * size.y ).ToIntRound() );
			GLASSERT( bounds->Height()>= ( xForm.yScale * size.y ).ToInt() );
		}
	#endif	
}


void KrPixelBlock::DrawScaledDown(	KrPaintInfo* info,
									const KrMatrix2& xForm,
									const KrColorTransform& cForm,
									const KrRect& clipping )
{
	GLASSERT( !info->OpenGL() );

	KrRect tBounds;
	CalculateBounds( xForm, &tBounds );

	KrRect isect = tBounds;
	isect.DoIntersection( clipping );

	if ( isect.IsValid() )
	{
		int txOffset = isect.xmin - tBounds.xmin;
		int tyOffset = isect.ymin - tBounds.ymin;

		// A square blit won't introduce an alpha:
		KrPaintFunc blitter = info->GetBlitter( ( flags & ALPHA ), cForm );

		// Every pixel in the target 'increments' the source
		// by a certain amount. This is the integer scaling factor
		// between source and target.
		U32 xInc = 0x10000 * Width()  / tBounds.Width();
		U32 yInc = 0x10000 * Height() / tBounds.Height();

		// The 'error' terms. Every time the exceed x10000,
		// its time to move a pixel in source. The distance
		// traveled on the target is *always* one pixel.
		U32 xError = ( txOffset * xInc );
		U32 yError = ( tyOffset * yInc );
		
		// Every new row will have the same x-error:
		U32 xErrorPerRow = xError & 0xffff;
		
		// The low part of error contains the current error;
		// the high part the # of pixels to move in source.
		KrRGBA* rowSource = block
							+ ( ( xError & 0xffff0000 ) >> 16 )
							+ ( ( yError & 0xffff0000 ) >> 16 ) * size.x;
		xError &= 0xffff;
		yError &= 0xffff;
		KrRGBA* source;



		// Now set up the target:
		U8* rowTarget = (U8*) info->pixels 
						+ isect.ymin * info->pitch
						+ isect.xmin * info->bytesPerPixel;
		U8* target;


		int iWidth  = isect.Width();
		int iHeight = isect.Height();

		for ( int j=0; j<iHeight; ++j )
		{
			target = rowTarget;
			source = rowSource;

			for( int i=0; i<iWidth; ++i )
			{
				// This one is a little different. It is always a
				// downsample, and sums all the colors over the downsample
				// rectangle.
				U32 color[4] = { 0, 0, 0, 0 };

				GLASSERT( xInc >= 0x1000 );
				GLASSERT( yInc >= 0x1000 );
				U32 dx = ( xInc + xError ) >> 16;
				U32 dy = ( yInc + yError ) >> 16;
				int subSize = 0;	// For anti-artifacting, we don't necessary use every pixel.

				KrRGBA* row = source;
				KrRGBA* scan;

				for( unsigned jj=0; jj<dy; ++jj )
				{
					scan = row;
					for( unsigned ii=0; ii<dx; ++ii )
					{
						++scan;
						if ( scan->c.alpha != 0 )
						{
							++subSize;
							for( unsigned k=0; k<4; ++k )
							{
								color[k] += scan->array[k];
							}
						}
					}
					row += size.x;	// the scanline for a canvas
				}
				if ( subSize != 0 )
				{
					for( int k=0; k<4; ++k )
					{
						color[k] /= subSize;
					}
				}
				else
				{
					color[0] = 0;
					color[1] = 0;
					color[2] = 0;
					color[3] = 0;
				}
				GLASSERT( color[0] <= 255 );
				GLASSERT( color[1] <= 255 );
				GLASSERT( color[2] <= 255 );
				GLASSERT( color[3] <= 255 );
				#ifdef DEBUG
					if ( !(flags & ALPHA) ) GLASSERT( color[ KrRGBA::ALPHA ] == 255 );
				#endif

				KrRGBA buffer;
				buffer.Set( color[ KrRGBA::RED ],
							color[ KrRGBA::GREEN ],
							color[ KrRGBA::BLUE ],
							color[ KrRGBA::ALPHA ] );

				blitter( info, target, &buffer, 4, 1, cForm ); //maks

				target += info->bytesPerPixel;
				xError += xInc;
				while ( xError & 0xffff0000 )
				{
					xError -= 0x10000;
					++source;
				}
			}
			
			rowTarget += info->pitch;
			
			yError += yInc;			
			while ( yError & 0xffff0000 )
			{
				yError -= 0x10000;
				rowSource += size.x;	// also pitch, for canvases
			}

			xError = xErrorPerRow;
		}
	}
}


void KrPixelBlock::DrawScaledLinear(	KrPaintInfo* info,
										const KrMatrix2& xForm,
										const KrColorTransform& cForm,
										const KrRect& clipping )
{
	GLASSERT( !info->OpenGL() );
	
	KrRect tBounds;
	CalculateBounds( xForm, &tBounds );

	KrRect isect = tBounds;
	isect.DoIntersection( clipping );

	if ( isect.IsValid() )
	{
		int txOffset = isect.xmin - tBounds.xmin;
		int tyOffset = isect.ymin - tBounds.ymin;

		// A square blit won't introduce an alpha:
		KrPaintFunc blitter = info->GetBlitter( ( flags & ALPHA ), cForm );

		// Every pixel in the target 'increments' the source
		// by a certain amount. This is the integer scaling factor
		// between source and target. Note however, that we need
		// one less source, since the interpolation will be between
		// the source pixel and the one to the right and down.
		U32 xInc = 0x10000 * ( Width() - 1 )  / tBounds.Width();
		U32 yInc = 0x10000 * ( Height() - 1 ) / tBounds.Height();

		// The 'error' terms. Every time the exceed x10000,
		// its time to move a pixel in source. The distance
		// traveled on the target is *always* one pixel.
		U32 xError = ( txOffset * xInc );
		U32 yError = ( tyOffset * yInc );
		
		// Every new row will have the same x-error:
		U32 xErrorPerRow = xError & 0xffff;
		
		// The low part of error contains the current error;
		// the high part the # of pixels to move in source.
		KrRGBA* rowSource = block
							+ ( ( xError & 0xffff0000 ) >> 16 )
							+ ( ( yError & 0xffff0000 ) >> 16 ) * size.x;
		xError &= 0xffff;
		yError &= 0xffff;
		KrRGBA* source0;
		KrRGBA* source1;



		// Now set up the target:
		U8* rowTarget = (U8*) info->pixels 
						+ isect.ymin * info->pitch
						+ isect.xmin * info->bytesPerPixel;
		U8* target;


		int iWidth  = isect.Width();
		int iHeight = isect.Height();

		for ( int j=0; j<iHeight; ++j )
		{
			target = rowTarget;
			source0 = rowSource;
			source1 = rowSource + size.x;

			KrRGBA color, c00, c01, c10, c11;

			for( int i=0; i<iWidth; ++i )
			{
				// We now copy and process, for anti-artifacting
				c00 = *source0;
				c10 = *(source0+1);
				c01 = *source1;
				c11 = *(source1+1);
				int k;

// 				for( k=KrRGBA::START; k<KrRGBA::END; ++k )
// 				{
// 					U32 g0, g1;
// 
// 					// interpelate on scanline, top
// 					g0 = (   (( c00.array[k] * ( 0x10000 - xError ) ) >> 16 ) * ( c00.c.alpha + 1 )
// 						   + (( c10.array[k] * ( xError ) ) >> 16 ) * ( c10.c.alpha + 1 ) ) / ( ( c00.c.alpha + c10.c.alpha ) / 2 + 1);
// 					// interpolate on scanline, bottom
// 					g1 = (   (( c01.array[k] * ( 0x10000 - xError ) ) >> 16 ) * ( c01.c.alpha + 1 )
// 						   + (( c11.array[k] * ( xError ) ) >> 16 ) * ( c11.c.alpha + 1 ) ) / ( ( c01.c.alpha + c11.c.alpha ) / 2 + 1);
// 					// interpolate between scanlines.
// 					color.array[k]  = (   (( g0 * ( 0x10000 - yError ) ) >> 16 )
// 										+ (( g1 * ( yError ) ) >> 16 ) );
// 				}
// 
// 				k = KrRGBA::ALPHA;
				for( k=0; k<4; ++k )
				{
					U32 g0, g1;

					// interpelate on scanline, top
					g0 = (   (( c00.array[k] * ( 0x10000 - xError ) ) >> 16 )
						   + (( c10.array[k] * ( xError ) ) >> 16 ) );
					// interpolate on scanline, bottom
					g1 = (   (( c01.array[k] * ( 0x10000 - xError ) ) >> 16 )
						   + (( c11.array[k] * ( xError ) ) >> 16 ) );
					// interpolate between scanlines.
					color.array[k]  = (   (( g0 * ( 0x10000 - yError ) ) >> 16 )
										+ (( g1 * ( yError ) ) >> 16 ) );
				}

				blitter( info, target, &color, 4, 1, cForm ); //maks
				target += info->bytesPerPixel;
				
				xError += xInc;
				while ( xError & 0xffff0000 )
				{
					xError -= 0x10000;
					++source0;
					++source1;
					GLASSERT( source0 < rowSource + size.x );
					GLASSERT( source1 < rowSource + size.x * size.y );
				}
			}
			
			rowTarget += info->pitch;
			
			yError += yInc;			
			while ( yError & 0xffff0000 )
			{
				yError -= 0x10000;
				rowSource += size.x;	// also pitch, for canvases
			}

			xError = xErrorPerRow;
		}
	}
}


void KrPixelBlock::DrawScaledFast(	KrPaintInfo* info,
									const KrMatrix2& xForm,
									const KrColorTransform& cForm,
									const KrRect& clipping,
									bool invert )
{
	GLASSERT( !info->OpenGL() );

	KrRect tBounds;
	CalculateBounds( xForm, &tBounds );

	KrRect isect = tBounds;
	isect.DoIntersection( clipping );

	if ( isect.IsValid() )
	{
		int txOffset = isect.xmin - tBounds.xmin;
		int tyOffset = isect.ymin - tBounds.ymin;

		// A square blit won't introduce an alpha:
		KrPaintFunc blitter = info->GetBlitter( ( flags & ALPHA ), cForm );

		// Every pixel in the target 'increments' the source
		// by a certain amount. This is the integer scaling factor
		// between source and target.
		U32 xInc = 0x10000 * Width()  / tBounds.Width();
		U32 yInc = 0x10000 * Height() / tBounds.Height();

		// The 'error' terms. Every time the exceed x10000,
		// its time to move a pixel in source. The distance
		// traveled on the target is *always* one pixel.
		U32 xError = ( txOffset * xInc );
		U32 yError = ( tyOffset * yInc );
		
		// Every new row will have the same x-error:
		U32 xErrorPerRow = xError & 0xffff;
		
		int yBias = 1;
		int yStartBias = 0;
		if ( invert )
		{
			yBias = -1;
			yStartBias = size.y - 1;
		}

		// The low part of error contains the current error;
		// the high part the # of pixels to move in source.
		KrRGBA* rowSource = block
							+ ( ( xError & 0xffff0000 ) >> 16 )
							+ ( yBias * ( ( yError & 0xffff0000 ) >> 16 ) + yStartBias ) * size.x;
		GLASSERT( rowSource >= block );
		GLASSERT( rowSource <  block + size.y * size.x );

		xError &= 0xffff;
		yError &= 0xffff;
		KrRGBA* source;



		// Now set up the target:
		U8* rowTarget = (U8*) info->pixels 
						+ isect.ymin * info->pitch
						+ isect.xmin * info->bytesPerPixel;
		U8* target;


		int iWidth  = isect.Width();
		int iHeight = isect.Height();

		for ( int j=0; j<iHeight; ++j )
		{
			target = rowTarget;
			source = rowSource;

			for( int i=0; i<iWidth; ++i )
			{
				GLASSERT( source >= block );
				GLASSERT( source <  block + size.y * size.x );
				GLASSERT( target >= (U8*) info->pixels );
				GLASSERT( target <  (U8*) info->pixels + info->pitch * info->height );

				blitter( info, target, source, 4, 1, cForm ); //maks
				target += info->bytesPerPixel;
				
				xError += xInc;
				while ( xError & 0xffff0000 )
				{
					xError -= 0x10000;
					++source;
				}
			}
			
			rowTarget += info->pitch;
			
			yError += yInc;			
			while ( yError & 0xffff0000 )
			{
				yError -= 0x10000;
				rowSource += yBias * size.x;	// also pitch, for canvases
			}

			xError = xErrorPerRow;
		}
	}
}


void KrPixelBlock::DrawScaled(	KrPaintInfo* info,
								const KrMatrix2& xForm,
								const KrColorTransform& cForm,
								const KrRect& clipping,


								int quality,

								bool invert )
{
	GLASSERT( !info->OpenGL() );

	if ( invert || quality == KrQualityFast )
	{
		DrawScaledFast( info, xForm, cForm, clipping, invert );
	}
	else if ( quality == KrQualityLinear )
	{
		DrawScaledLinear( info, xForm, cForm, clipping );
	}
	else
	{
		GLASSERT( quality == KrQualityAdaptive );
		if (    xForm.xScale.v <= GlFixed_1 / 2
		     && xForm.yScale.v <= GlFixed_1 / 2 )
		{
			DrawScaledDown( info, xForm, cForm, clipping );
		}
		else
		{
			DrawScaledLinear( info, xForm, cForm, clipping );
		}
	}

}

#ifdef KYRA_SUPPORT_OPENGL
void KrPixelBlock::LoadNewTexture()
{
	//maks: remove and let DrawOpenGL reload to avoid crash when resize canvas actor
	/*if ( texture )
	{
		texture->SetTexture( block, size.x, size.y );
	}*/

	RemoveTexture();
}
#endif


void KrPixelBlock::DrawOpenGL(		KrPaintInfo* paintInfo,
									const KrMatrix2& xForm,
									const KrColorTransform& cForm,
									const KrRect& clipping,
									int rotation,
									int openGLZ )
{
	#ifdef KYRA_SUPPORT_OPENGL
	GLASSERT( paintInfo->OpenGL() );

	if ( !texture )
	{
		KrTextureManager* manager = KrTextureManager::Instance();
		if ( manager )
		{
			// Create a canvas resource with the data we need.
			texture = manager->CreateTexture( block, size.x, size.y/*, true*/ ); //maks
			KrTextureManager::AddTextureOwner(&texture); //maks
		}
	}

	GLASSERT( texture );
	if ( !texture ) return;

	SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, texture->Id() );

	paintInfo->SetOpenGLTextureMode( Alpha(), cForm, xForm.IsScaled(), texture );

	KrRect bounds;
	CalculateBounds( xForm, &bounds );
	//bounds.Translate(-OGL_OFFSET, -OGL_OFFSET); //maks

	KrVector2T< float > texCoord[ 4 ];

	texCoord[ (rotation+0) % 4 ].x = texture->Bounds().xmin;
	texCoord[ (rotation+0) % 4 ].y = texture->Bounds().ymin;

	texCoord[ (rotation+1) % 4 ].x = texture->Bounds().xmax;
	texCoord[ (rotation+1) % 4 ].y = texture->Bounds().ymin;

	texCoord[ (rotation+2) % 4 ].x = texture->Bounds().xmax;
	texCoord[ (rotation+2) % 4 ].y = texture->Bounds().ymax;

	texCoord[ (rotation+3) % 4 ].x = texture->Bounds().xmin;
	texCoord[ (rotation+3) % 4 ].y = texture->Bounds().ymax;

	if ( rotation > 3 )
	{
		GlSwap( texCoord[0], texCoord[3] );
		GlSwap( texCoord[1], texCoord[2] );
	}

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glBegin( GL_QUADS );
	{
		SDL_CurrentVideo->glTexCoord2f( texCoord[0].x,	texCoord[0].y );
		SDL_CurrentVideo->glVertex3i( bounds.xmin,				bounds.ymin,					openGLZ );

		SDL_CurrentVideo->glTexCoord2f( texCoord[1].x,	texCoord[1].y );
		SDL_CurrentVideo->glVertex3i( bounds.xmin+bounds.Width(), bounds.ymin,					openGLZ );

		SDL_CurrentVideo->glTexCoord2f( texCoord[2].x,	texCoord[2].y );
		SDL_CurrentVideo->glVertex3i( bounds.xmin+bounds.Width(), bounds.ymin+bounds.Height(),	openGLZ );
		
		SDL_CurrentVideo->glTexCoord2f( texCoord[3].x,	texCoord[3].y );
		SDL_CurrentVideo->glVertex3i( bounds.xmin,				bounds.ymin+bounds.Height(),	openGLZ );
	}
	SDL_CurrentVideo->glEnd();
#else
	GLshort vertices[] = 
	{
		bounds.xmin,				bounds.ymin,					openGLZ,  
		bounds.xmin+bounds.Width(), bounds.ymin,					openGLZ,  
		bounds.xmin+bounds.Width(), bounds.ymin+bounds.Height(),	openGLZ, 
		bounds.xmin,				bounds.ymin+bounds.Height(),	openGLZ						
	};
	
	GLfixed texCoords[] = 
	{
		GlES_DoubleToFixed(texCoord[0].x),	GlES_DoubleToFixed(texCoord[0].y),   
		GlES_DoubleToFixed(texCoord[1].x),	GlES_DoubleToFixed(texCoord[1].y),   
		GlES_DoubleToFixed(texCoord[2].x),	GlES_DoubleToFixed(texCoord[2].y),   
		GlES_DoubleToFixed(texCoord[3].x),	GlES_DoubleToFixed(texCoord[3].y)
	};
	
	DrawOGLTexQuad(GL_TRIANGLE_STRIP, vertices, texCoords, 3);
#endif


	GLASSERT( SDL_CurrentVideo->glGetError() == GL_NO_ERROR );
	#endif
}


void KrPixelBlock::Draw(	KrPaintInfo* paintInfo,
							const KrMatrix2& xForm,
							bool invert,
							const KrColorTransform& cForm,
							const KrRect& clipping,


							int quality,


							int openGLZ )
{
	if ( paintInfo->OpenGL() )
	{
		#ifdef KYRA_SUPPORT_OPENGL
			GLASSERT( invert == false );	// This path should only come from a canvas, which does no invert.
			DrawOpenGL( paintInfo, xForm, cForm, clipping, 0, openGLZ );
		#else
			GLASSERT( 0 );
		#endif
		return;
	}

	if ( xForm.IsScaled() )
	{
		//GLASSERT( invert == false );	// we don't scale tiles.
		DrawScaled( paintInfo, xForm, cForm, clipping, 

			quality, 

			invert );
		return;
	}

	KrRect bounds;
	CalculateBounds( xForm, &bounds );

	KrRect isect = bounds;
	isect.DoIntersection( clipping );

	int j;
	int yOffset		= isect.ymin - bounds.ymin;
	int xOffset		= isect.xmin - bounds.xmin;
	int width		= isect.Width();
	int height		= isect.Height();
	int sourcePitch	= size.x;



	U8* target =   (U8*) paintInfo->pixels 
				   + isect.ymin * paintInfo->pitch
				   + isect.xmin * paintInfo->bytesPerPixel;

	// for rotations 4-7, we run the source in reverse.
	KrRGBA* source;
	if ( !invert )
	{
		source =   block
				 + yOffset * size.x
				 + xOffset;
	}
	else
	{
		source =   block
				 + ( size.y - yOffset - 1) * size.x
				 + xOffset;
		sourcePitch = -sourcePitch;
	}

	if ( width > 0 && height > 0 )
	{
		KrPaintFunc blitter = paintInfo->GetBlitter( ( flags & ALPHA ), cForm );
		if ( blitter )
		{
			for ( j=0; 
				  j<height; 
				  j++, source += sourcePitch, target += paintInfo->pitch )
			{
				blitter( paintInfo, target, source, 4, width, cForm ); //maks
			}
		}
	}
}


void KrPixelBlock::CountComponents( U32* numRGBA )
{
	*numRGBA = size.x * size.y;
}




