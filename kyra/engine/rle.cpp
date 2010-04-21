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
	#include "SDL_opengles.h" //AKR
#else
	#include "SDL_opengl.h" //AKR

#endif
#endif
#include "rle.h"
#include "SDL_rwops.h"
#include "SDL_endian.h"
#include "painter.h"
#include "../util/glutil.h"
#include <string.h>
#include "sdlutil.h"
#include "kyrabuild.h"
#include "canvasresource.h"
#include "engine.h"
#include "ogltexture.h"
#include "painter.h"
#include "../../gameEngine/compression.h" //maks
#include "../util/glperformance.h" //maks
#include "FastSprite.h" //maks
#ifdef __APPLE__
#include "SDL_sysvideo.h"
#endif
#include "../../gameEngine/dlmalloc.h" //maks
extern "C" void free_gc(void *mem);
extern "C" void using_mem_gc(void *mem);
extern "C" void available_mem_gc(void *mem);
extern "C" void malloc_gc(void **mem, int bytes);
extern "C" SDL_Surface **getsurface_mem_gc(void *mem);

int SDL_SavePNG(SDL_Surface *surface, const char *file);

#ifdef GP2X
extern "C" void SDL_GP2X_WaitForBlitter();
extern "C" void flushcache();
#endif

class CMemControl
{
public:
	CMemControl(void *_mem, bool _bControl)
	{
		mem = _mem;
		bControl = _bControl;

		if(mem && bControl)
		{
			using_mem_gc(mem);
		}
	}

	~CMemControl()
	{
		if(mem && bControl)
		{
			available_mem_gc(mem);
		}
	}

private:
	void *mem;
	bool bControl;
};

#ifdef USE_COMPILED_SPRITES
U8 *compile_16b_Segment(U16 *seg, int len, long startOffSet, int& asmLen)
{
	int totalAsm = len*10 + 1;
	U8 *asmBuf = (U8 *)malloc(totalAsm);
	int n = len;
	
	asmLen = 0;

	FILE *arq = NULL; //fopen("csprite.cpp", "w");

	while(n >= 2)
	{
		int i = len - n;
		long offSet = i*2 + startOffSet;

		unsigned char *pixel1 = (unsigned char *)&seg[i];
		unsigned char *pixel2 = (unsigned char *)&seg[i + 1];
		
		// BEST CASE!  Write a double-word:					
		if(arq)
		{
			fprintf(arq,"        mov     dword ptr [edi+0x%x],0x%02x%02x%02x%02x\n",
				offSet, pixel2[1], pixel2[0], pixel1[1], pixel1[0]);	
		}

		//Machine code
		asmBuf[asmLen++] = 0xC7;
		asmBuf[asmLen++] = 0x87;

		memcpy(&asmBuf[asmLen], &offSet, 4);
		asmLen += 4;

		asmBuf[asmLen++] = pixel1[0];
		asmBuf[asmLen++] = pixel1[1];
		asmBuf[asmLen++] = pixel2[0];
		asmBuf[asmLen++] = pixel2[1];

		n -= 2;
	}

	while(n > 0)
	{
		int i = len - n;
		long offSet = i*2 + startOffSet;

		unsigned char *pixel1 = (unsigned char *)&seg[i];

		// Write a word
		if(arq)
		{
			fprintf(arq,"        mov     word ptr [edi+0x%x],0x%02x%02x\n", 
				offSet, pixel1[1], pixel1[0]);
		}

		//Machine code
		asmBuf[asmLen++] = 0x66;
		asmBuf[asmLen++] = 0xC7;
		asmBuf[asmLen++] = 0x87;

		memcpy(&asmBuf[asmLen], &offSet, 4);
		asmLen += 4;

		asmBuf[asmLen++] = pixel1[0];
		asmBuf[asmLen++] = pixel1[1];		

		n--;
	}

	if(arq) fclose(arq);

	//while(asmLen < totalAsm) asmBuf[asmLen++] = 0xC3; //ret
	
	//__asm
	//{
	//	mov     dword ptr [edi+0x123],0x50607080   
	//		
	//	//C7 87		78 56 34 12		81 71 61 51
	//	mov     dword ptr [edi+0x12345678],0x51617181
	//
	//	//66 C7	87	23 01 00 00		34 12
	//	mov     word ptr [edi+0x123],0x1234
	//
	//	//C3
	//	ret
	//}

	return asmBuf;
}

long startOffSet = 0, codeLen = 0;
int lastX = 0, lastY = 0, screenWidth = 0;
U8 *codeBuf = NULL;
extern KrEngine *engine;

void compile_newCode()
{
	startOffSet = 0;
	codeLen = 0;
	lastX = 0;
	lastY = 0;
	screenWidth = engine->ScreenBounds().Width();
	codeBuf = NULL;
}

U8 *compile_endCode()
{
	codeBuf = (U8 *)realloc(codeBuf, codeLen + 1);
	codeBuf[codeLen] = 0xC3; //ret

	return codeBuf;
}

void compile_addSegmentToCode(int x, int y, U16 *seg, int len)
{
	int asmLen = 0;

	if(y != lastY)
	{
		//Jump line
		startOffSet += 2*(y - lastY)*screenWidth;
	}

	if(x != lastX)
	{
		//Jump segment
		startOffSet += 2*(x - lastX);
	}

	U8 *buf = compile_16b_Segment(seg, len, startOffSet, asmLen);

	
	codeBuf = (U8 *)realloc(codeBuf, codeLen + asmLen);
	memcpy(codeBuf + codeLen, buf, asmLen);
	free(buf);
	
	codeLen += asmLen;
	startOffSet += 2*len;

	lastX = x + len + 1;
	lastY = y;
}
#endif


#if defined( DRAWDEBUG_RLE ) || defined( DRAWDEBUG_BLTRECTS )
	int debugFrameCounter = 0;
#endif

#ifdef DEBUG
	U32 KrRleSegment::numRGBA = 0;
#endif


void KrRleLine::DrawScaled(	U8* target, 
							KrPaintInfo* paintInfo,
							const KrRect& bounds,
							const KrRect& isect,
							const KrColorTransform& cForm,
							U32 stRatio )	// source / target
{
	GLASSERT( !paintInfo->OpenGL() );

	// Walk through all the segments, figure out where
	// they are and if they need to be drawn.

	int tx = bounds.xmin;
	S32 sErrorF = 0;
	int i = 0;

	// Draw each skip and run if in bounds.
	int nSegments = NumSegments(); //maks
	while ( tx <= isect.xmax && i < nSegments )
	{
		while ( (sErrorF>>16) < segment[i].Start() )
		{
			sErrorF += stRatio;
			++tx;
		}

		// Get a per-segment blitter
		KrPaintFunc blitter = paintInfo->GetBlitter( segment[i].getFlagBit( ALPHA ), cForm ); //maks
		GLASSERT( blitter );

		// Draw the run
		// Fixme: optimize. Pull things out of inner loop.
		while (    (sErrorF>>16) <= segment[i].End()
				&& tx <= isect.xmax )
		{
			if ( tx >= isect.xmin )
			{
				GLASSERT( tx >= bounds.xmin && tx <= bounds.xmax );
				GLASSERT( tx >= isect.xmin  && tx <= isect.xmax );

				blitter(	paintInfo, //maks
							target + tx * paintInfo->bytesPerPixel, 
							segment[i].RGBA() + ((sErrorF>>16) - segment[i].Start())*segment[i].getBytesPerPixel(), 
							segment[i].getBytesPerPixel(),
							1, 
							cForm );
			}
			sErrorF += stRatio;
			++tx;
		}
		++i;
	}
}





// -------------- Segment ------------------
KrRleSegment::~KrRleSegment()
{ 
	if (rgba && !(getFlagBit(MEMORYPOOL)))
		delete [] rgba; 
}









// -------------- Line ------------------






// -------------- Rle ------------------

GlLinearMemoryPool*	KrRle::memoryPoolRGBA    = 0;
GlLinearMemoryPool*	KrRle::memoryPoolLine    = 0;
GlLinearMemoryPool*	KrRle::memoryPoolSegment = 0;

bool KrRle::bConvertImage = false; //maks

KrRle::~KrRle()
{
	Clear(); //maks	
}

void KrRle::Clear() //maks
{
	if(bigData) //maks
	{
		if(bigDataUsingGC) free_gc(bigData);
		else free(bigData);
	}
	else if(!frameIndex)
	{
		if ( !flags.IsSet( MEMORYPOOL ) )
			delete [] line; 
	}

#ifndef STAND_ALONE_GAME
	if(surface) //maks
	{
		SDL_FreeSurface(surface); 
		surface = NULL;
	}
#endif

	RemoveTexture(); //maks

	delete collisionMap;
	
	line = NULL;
	texture = NULL;
	collisionMap = NULL;
	bitMapArea = NULL;
	bigData = NULL;
	bigDataUsingGC = false;
	frameIndex = 0;
	//refCount = 0;
	deltaHotToOrigin.Set(); 
	size.Set(); 
	delta.Set();

#ifndef STAND_ALONE_GAME //maks
	originalFrameSize.Set();
	originalHotSpot.Set();
	bHasTransparentColor = false;
#endif
}

void KrRle::RemoveTexture() //maks
{
#ifdef KYRA_SUPPORT_OPENGL
	if(texture)
	{
		KrTextureManager::RemoveTextureOwner(&texture);
		delete texture; 
		texture = NULL;
	}
	
#endif
}


bool KrRle::LoadIndexedFrame() //maks
{
	if(!frameIndex) return false;

	SDL_RWops *data = ::engine->Vault()->getIndexedData();
	if(!data) return false;

	Uint32 pos = SDL_RWtell( data ); //Save actual position
	SDL_RWseek( data, frameIndex, SEEK_SET );

	bool res = ReadFrameData(data);

	SDL_RWseek( data, pos, SEEK_SET ); //Restore position
	return res;
}

bool KrRle::ReadFrameData( SDL_RWops* data ) //maks
{
	U32  lenght = SDL_ReadLE32( data );
	U32  lenghtComp = SDL_ReadLE32( data );

	int sizeofKrRleLine = sizeof(KrRleLine);


	
	if(lenght)
	{
		if(!bigData)
		{
			if(frameIndex)
			{
				malloc_gc((void **)&bigData, lenght);
				if(bigData) bigDataUsingGC = true;
			}
			else
			{
				bigData = (U8*)dlmalloc(lenght);
				bigDataUsingGC = false;
			}

			if(!bigData) return false;
		}

		if(lenght == lenghtComp)
		{			
			SDL_RWread(data, bigData, lenght, 1);
		}
		else
		{
			//Read compressed data
			Compression comp;			
			if(!comp.Decompression(data, lenghtComp, bigData, lenght))
			{
				if(bigDataUsingGC) free_gc(bigData);
				else dlfree(bigData);

				bigDataUsingGC = false;
				bigData = NULL;

				return false;
			}
		}
		
		
		//Set line pointer
		line = (KrRleLine *)bigData;

				
		//Set all segment pointers
		U8 *seg = bigData + size.y*sizeofKrRleLine;
		int i;
		for(i=0; i<size.y; i++)
		{
			line[i].segment = (KrRleSegment *)seg;
			seg += line[i].NumSegments()*sizeof(KrRleSegment); //maks
		}

				
		//Set all segment buffers
		//KrRleSegment *lastSegment = NULL;
		for(i=0; i<size.y; i++)
		{
			int nSegments = line[i].NumSegments();
			for(int is = 0; is < nSegments; is++) //maks
			{
			/*if(lastSegment)
			{
			lastSegment->next = &(line[i].segment[is]);
			}*/
				
				line[i].segment[is].rgba = seg;
				//line[i].segment[is].next = lastSegment;
				//lastSegment = &(line[i].segment[is]);
				
				seg += line[i].segment[is].Len()*line[i].segment[is].getBytesPerPixel();					
			}
			
			/*if(lastSegment) 
			{
			if(line[i].nSegments) lastSegment->bEnd = 1;
			else lastSegment->bEnd++;
		}*/
		}
	}

	
	if(bigDataUsingGC) available_mem_gc(bigData);
	return true;
}

bool KrRle::Read( SDL_RWops* data ) //maks
{
#ifdef COUNT_PERFORMANCE //maks
	GlPerformance perf( "KrRle::Read" );
#endif 

	//if(!memory) memory = new U8[2*1024*1024];

//	#ifdef DEBUG
//	int minx = 100;	// just for error checking - should go to 0.
//	#endif

	
	if(KrResourceVault::getVersionDat() >= 2) SDL_RWread(data, &flags, sizeof(U8), 1); 
	else flags.FromU32( SDL_ReadLE16( data ) );

	deltaHotToOrigin.x = SDL_ReadLE32( data );
	deltaHotToOrigin.y = SDL_ReadLE32( data );
	size.x = SDL_ReadLE32( data );
	size.y = SDL_ReadLE32( data );
	delta.x = SDL_ReadLE32( data );
	delta.y = SDL_ReadLE32( data );

/*	GLASSERT( GlInRange(  deltaHotToOrigin.x, -4000, 4000 ) );	// sanity
	GLASSERT( GlInRange(  deltaHotToOrigin.y, -4000, 4000 ) );	// sanity
	GLASSERT( GlInRange(  size.x, 0, 4000 ) );				// sanity
	GLASSERT( GlInRange(  size.y, 0, 4000 ) );				// sanity*/

	bigData = NULL;
	bigDataUsingGC = false;
	frameIndex = 0;

	

	if(KrResourceVault::getVersionDat() >= 2) 
	{
		//Read a big data block with all lines and segments
		//Line1Line2Line3...Seg1Seg2...

		if(flags.IsSet(INDEXED_FRAME))
		{
			frameIndex = SDL_RWtell( data );

			//Skip data for next read
			U32  lenght = SDL_ReadLE32( data );
			U32  lenghtComp = SDL_ReadLE32( data );

			if(lenght)
			{
				if(lenght == lenghtComp)
				{			
					SDL_RWseek(data, lenght, SEEK_CUR);
				}
				else
				{
					SDL_RWseek(data, lenghtComp, SEEK_CUR);
				}
			}
		}
		else
		{
			ReadFrameData(data);			
		}
	}
#ifndef STAND_ALONE_GAME //maks
	else
	{
		KrRGBA minColor, bits;
		minColor.c.red = ReadByte( data );
		bits.c.red = ReadByte( data );
		minColor.c.green = ReadByte( data );
		bits.c.green = ReadByte( data );
		minColor.c.blue = ReadByte( data );
		bits.c.blue = ReadByte( data );
		minColor.c.alpha = ReadByte( data );
		bits.c.alpha = ReadByte( data );
		
		GlReadBitStream reader( data );
		
		// We get 0 sizes for fonts, so be careful.
		if ( size.y > 0 )
		{
			if ( memoryPoolLine )
			{
				line = (KrRleLine*) memoryPoolLine->Alloc( sizeof( KrRleLine ) * size.y );
				flags.Set( MEMORYPOOL );
			}
			else
			{
				line = new KrRleLine[ size.y ];
			}
			
			//KrRleSegment *lastSegment = NULL;
			for ( int i=0; i<size.y; i++ )
			{
				line[i].Clear();
				/*lastSegment =*/ line[i].Read( &reader, minColor, bits/*, lastSegment*/ );
				//			#ifdef DEBUG
				//			if ( line[i].NumSegments() > 0 && line[i].Segment( 0 )->Skip() < minx )
				//				minx = line[i].Segment( 0 )->Skip();
				//			#endif
				GLASSERT( line[i].CalcSize() <= size.x );
			}
			//		GLASSERT( minx == 0 );
		}
		else
		{
			line = 0;
		}
		
		reader.Flush();
	}
#endif

	//GLOUTPUT( "RLE: alpha=%d\n", Alpha() );
	return ( size.y > 0 );
}


void KrRle::CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const
{
	GLASSERT( xForm.xScale >= 0 );
	//GLASSERT( xForm.xScale < 1000 );	// sanity check //maks
	GLASSERT( xForm.yScale >= 0 );
	//GLASSERT( xForm.yScale < 1000 );	// sanity check //maks

	if ( xForm.IsScaled() )
	{
		S32 xmin = GlFixedMult( xForm.xScale.v, GlIntToFixed( deltaHotToOrigin.x ) );
		S32 ymin = GlFixedMult( xForm.yScale.v, GlIntToFixed( deltaHotToOrigin.y ) );

		bounds->xmin = GlFixedToIntRound( xmin + xForm.x.v );
		bounds->ymin = GlFixedToIntRound( ymin + xForm.y.v );
//		bounds->xmax = bounds->xmin + GlFixedToIntRound( GlFixedMult( xForm.xScale.v, GlIntToFixed( size.x )) ) - 1;
//		bounds->ymax = bounds->ymin + GlFixedToIntRound( GlFixedMult( xForm.yScale.v, GlIntToFixed( size.y )) ) - 1;

		bounds->xmax = GlFixedToIntRound( xForm.x.v + xmin
                                          + GlFixedMult( xForm.xScale.v, GlIntToFixed( size.x ))) - 1;
		bounds->ymax = GlFixedToIntRound( xForm.y.v + ymin
                                          + GlFixedMult( xForm.yScale.v, GlIntToFixed( size.y ))) - 1;

    }
	else
	{
		bounds->xmin = deltaHotToOrigin.x + /*GlFixedToInt/**/ GlFixedToIntRound/**/( xForm.x.v ); //maks: more smooth moves
		bounds->ymin = deltaHotToOrigin.y + /*GlFixedToInt/**/ GlFixedToIntRound/**/( xForm.y.v ); //maks
		bounds->xmax = bounds->xmin + size.x - 1;
		bounds->ymax = bounds->ymin + size.y - 1;
	}

	#ifdef DEBUG
		// Can be invalid in the encoder.
		//GLASSERT( bounds->IsValid() );

		if ( xForm.xScale == 1 && xForm.yScale == 1 )
		{
			GLASSERT( bounds->Height() == size.y );
			GLASSERT( bounds->Width()  == size.x );
		}
		/*else
		{
			GLASSERT( bounds->Width() <= ( xForm.xScale * size.x ).ToIntRound() );
			GLASSERT( bounds->Width() >= ( xForm.xScale * size.x ).ToInt() );
			GLASSERT( bounds->Height()<= ( xForm.yScale * size.y ).ToIntRound() );
			GLASSERT( bounds->Height()>= ( xForm.yScale * size.y ).ToInt() );
		}*/
		//GLASSERT( bounds->xmin > -10000 );	// sanity //maks
		//GLASSERT( bounds->ymin > -10000 );	// sanity //maks
		//GLASSERT( bounds->xmax < 10000 );	// sanity //maks
		//GLASSERT( bounds->ymax < 10000 );	// sanity //maks

	#endif
}



void KrRle::DrawScaled( KrPaintInfo* paintInfo,
						const KrMatrix2& xForm,
						const KrColorTransform& cForm,
						const KrRect& clipping )
{
	GLASSERT( !paintInfo->OpenGL() );

// 	#ifdef ASSERT_IF_NOT_CACHED
// 		GLASSERT( 0 );
// 	#endif

#ifdef STAND_ALONE_GAME //maks
	if(flags.IsSet(INDEXED_FRAME) && !bigData)
	{
		//On demmand frame loading
		if(!LoadIndexedFrame())
		{
			return;
		}
	}
#endif
	
	{
		CMemControl memControl(bigData, bigDataUsingGC);
		KrRect bounds, isect;
		
		CalculateBounds( xForm, &bounds );
		isect = bounds;
		isect.DoIntersection( clipping );
		
		if ( isect.IsValid() )
		{
			U32 xInc = 0x10000 * Width()  / bounds.Width();
			U32 yInc = 0x10000 * Height() / bounds.Height();
			
			U32 yError = ( (isect.ymin - bounds.ymin) * yInc );
			U32 syOffset = yError >> 16;
			yError &= 0xffff;
			
			
			
			// Now set up the target:
			U8* target = (U8*) paintInfo->pixels
				+ paintInfo->pitch * isect.ymin;
			
			KrRleLine* currentLine = &line[ syOffset ];
			
#ifdef DEBUG
			int yadvance = 0;
#endif
			
			for( int j=0; j<isect.Height(); ++j )
			{
				GLASSERT( currentLine < &line[ size.y ] );
				GLASSERT( target >= (U8*) paintInfo->pixels + paintInfo->pitch * isect.ymin );
				GLASSERT( target <= (U8*) paintInfo->pixels + paintInfo->pitch * isect.ymax );
				
				currentLine->DrawScaled(	target, paintInfo,
					bounds,
					isect,
					cForm,
					xInc );
				
				target += paintInfo->pitch;
				yError += yInc;			
				while ( yError & 0xffff0000 )
				{
					yError -= 0x10000;
					++currentLine;	// also pitch, for canvases
#ifdef DEBUG
					++yadvance;
#endif
				}
			}
		}
	}
	
/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
	if(flags.IsSet(INDEXED_FRAME))
	{
		//Release memory
		delete [] bigData;
		bigData = NULL;
	}
#endif*/
}
				  
//extern bool bFast;


void KrRle::DrawOpenGL( KrPaintInfo* paintInfo,
						const KrMatrix2& xForm,
						const KrColorTransform& cForm,
						const KrRect& clipping,
						int openGLZ )
{
	#ifdef KYRA_SUPPORT_OPENGL
	if ( !texture )
	{
		KrTextureManager* manager = KrTextureManager::Instance();
		if ( manager )
		{
			// Create a canvas resource with the data we need.
			int hotx, hoty;
			KrCanvasResource* resource = CreateCanvasResource( &hotx, &hoty, true );
			texture = manager->CreateTexture( resource->Pixels(), resource->Width(), resource->Height(), true/*AKR*/ );
			KrTextureManager::AddTextureOwner(&texture); //maks
			delete resource;
		}
	}

	GLASSERT( texture );
	if ( !texture ) return;

	KrRect bounds;
	CalculateBounds( xForm, &bounds );
	bounds.Translate(-OGL_OFFSET, -OGL_OFFSET); //maks

	SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, texture->Id() );
	paintInfo->SetOpenGLTextureMode( Alpha(), cForm, xForm.IsScaled(), texture );


	GLASSERT( openGLZ >= 1 );
	GLASSERT( openGLZ <= kKrOglDistance );

	

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glBegin( GL_QUADS );
	{
		SDL_CurrentVideo->glTexCoord2f( texture->Bounds().xmin,	texture->Bounds().ymin );
		SDL_CurrentVideo->glVertex3i( bounds.xmin,				bounds.ymin,					openGLZ );

		SDL_CurrentVideo->glTexCoord2f( texture->Bounds().xmax,	texture->Bounds().ymin );
		SDL_CurrentVideo->glVertex3i( bounds.xmin+bounds.Width(), bounds.ymin,					openGLZ );

		SDL_CurrentVideo->glTexCoord2f( texture->Bounds().xmax,	texture->Bounds().ymax );
		SDL_CurrentVideo->glVertex3i( bounds.xmin+bounds.Width(), bounds.ymin+bounds.Height(),	openGLZ );

		SDL_CurrentVideo->glTexCoord2f( texture->Bounds().xmin,	texture->Bounds().ymax );
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
		GlES_DoubleToFixed(texture->Bounds().xmin),	GlES_DoubleToFixed(texture->Bounds().ymin),   
		GlES_DoubleToFixed(texture->Bounds().xmax),	GlES_DoubleToFixed(texture->Bounds().ymin),   
		GlES_DoubleToFixed(texture->Bounds().xmax),	GlES_DoubleToFixed(texture->Bounds().ymax),   
		GlES_DoubleToFixed(texture->Bounds().xmin),	GlES_DoubleToFixed(texture->Bounds().ymax)
	};
	
	DrawOGLTexQuad(GL_TRIANGLE_STRIP, vertices, texCoords, 3); 
#endif


	#endif

	
}



void KrRle::Draw( KrPaintInfo* paintInfo,
				  const KrMatrix2& xForm,
				  const KrColorTransform& cForm,
				  const KrRect& clipping,
				  int openGLZ,
				  bool bUseFastSprite,
				  bool bCratingSurface //maks
				  ) //maks
{
#ifdef GP2X_HARDWARE_BLITTER //maks: no use hw blitter now. (bad repaint in some games)
	//Need to wait hardware blitter finishes even if don't use the hardware blitter in this draw
	SDL_GP2X_WaitForBlitter();
#endif

	if ( xForm.IsScaled() && !paintInfo->OpenGL() )
	{
		DrawScaled( paintInfo, xForm, cForm, clipping );
		return;
	}

#ifdef STAND_ALONE_GAME //maks
	if(flags.IsSet(INDEXED_FRAME) && !bigData)
	{
		//On demmand frame loading
		if(!LoadIndexedFrame())
		{
			return;
		}
	}
#endif


	{
	CMemControl memControl(bigData, bigDataUsingGC);

#ifdef KYRA_SUPPORT_OPENGL //maks
	if ( paintInfo->OpenGL() )
	{
		DrawOpenGL( paintInfo, xForm, cForm, clipping, openGLZ );
		return;
	}
#endif



	#ifdef DRAWDEBUG_RLE
		KrPainter painter( paintInfo );
		painter.DrawBox( xForm.x.ToIntRound(), xForm.y.ToIntRound(), 
						 ( xForm.xScale * size.x ).ToIntRound(), 
						 ( xForm.yScale * size.y ).ToIntRound(), 
						 0, 150*(debugFrameCounter&0x01), 200 );
		KrRGBA red = { 255, 150*(debugFrameCounter&0x01), 0, 0 };
	#endif

	// -------- Unscaled drawing -------------------
	// The bounds min is the local origin of the RLE.
	KrRect bounds;
	CalculateBounds( xForm, &bounds );

	KrRect isect = bounds;
	isect.DoIntersection( clipping );

	if ( !isect.IsValid() )
		return;					// No overlap.

	int bpp = paintInfo->bytesPerPixel;
	/*if(bpp == 2 && cForm.IsIdentity() && !Alpha() && bUseFastSprite)
	{
		//Draw Error in Fire top Montain
		//Without SWAP_VSYNC
		//	37 = 40 fps em 'fps benchmark.ged'
		fastSprite.Draw(this, bounds.xmin, bounds.ymin, isect);		
		return;
	}/**/

	// Set up variables that apply to rle < clip and rle >= clip

	// Note that the target starts on the first scan line of the
	// intersection, at the beginning of the rle bounding box.
	// This could put it off screen which technically is illegal,
	// but you're not running this on a segmented architecture.


	int pitch = paintInfo->pitch;
	U8* targetLine = (U8*)   paintInfo->pixels
						   + bpp * bounds.xmin
						   + pitch * isect.ymin;


#ifdef GP2X_HARDWARE_BLITTER //maks: no use hw blitter now. (bad repaint in some games)
	if(
		!cForm.HasColor() && //No color transformations
		!Alpha() && //No alpha channel
		!cForm.Alpha() && //No alpha in the surface
		!bCratingSurface
	  ) 
	{
		//maks: SDL blitter
		#ifdef STAND_ALONE_GAME
		if(bigData && bigDataUsingGC) {
		
		SDL_Surface **pSurface = getsurface_mem_gc(bigData);
		SDL_Surface *surface = *pSurface;
		#endif
		
		if(!surface)
		{
			//Same format of video device

			if(!Alpha())
			{
				surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
					size.x,
					size.y,
					paintInfo->bytesPerPixel << 3,
					paintInfo->redMask,
					paintInfo->greenMask,
					paintInfo->blueMask,
					paintInfo->alphaMask);
			}
			else
			{
				surface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,
					size.x,
					size.y,
					32,
					0x00ff0000,
					0x0000ff00,
					0x000000ff,
					0xff000000);
			}


			if(surface)
			{
				SDL_FillRect(surface, 0, colorKey.all); 
				SDL_SetColorKey(surface, SDL_SRCCOLORKEY /*| *SDL_RLEACCEL*/, colorKey.all);

				KrPaintInfo paint(surface);
				KrMatrix2 matrix;
				KrRect clip;		
				KrVector2 hotSpot(Hotspot());

				matrix.Set(hotSpot.x, hotSpot.y);
				clip.Set(0, 0, surface->w-1, surface->h-1);

				Draw(&paint, matrix, cForm, clip, 0, true, true);	

				#ifdef STAND_ALONE_GAME
				*pSurface = surface;
				#endif
			}
		}

		if(surface)
		{
			//Set the current alpha
			if(cForm.Alpha() < 255) SDL_SetAlpha(surface, SDL_SRCALPHA /*| *SDL_RLEACCEL*/, cForm.Alpha());
			else SDL_SetAlpha(surface, 0, 0); //To enable hardware blitter

			//Set rects
			SDL_Rect src, dst;

			src.x = isect.xmin - bounds.xmin;
			src.y = isect.ymin - bounds.ymin;
			src.w = isect.Width();
			src.h = isect.Height();

			dst.x = isect.xmin;
			dst.y = isect.ymin;
			dst.w = src.w;
			dst.h = src.h;

			//Flush the data cache before blitter due to other direct pixel writes
			//http://wiki.gp2x.org/index.php?title=Using_the_hardware_blitter&redirect=no
			//flushcache(); //Don't solve the GP2X problem

			SDL_BlitSurface(surface, &src, paintInfo->surface, &dst); 
		}

		return;

		#ifdef STAND_ALONE_GAME
		}
		#endif
	}
#endif


#ifdef USE_COMPILED_SPRITES
	if(spriteCode && clipping.HasInside(bounds))
	{
		//Execute compiled sprite
		U8 *addr = spriteCode;
		
		try {
		__asm 
		{
			push edi
				
			mov  edi, targetLine
			call addr
			
			pop  edi	
		}	
		} catch(...) {}

		return;
	}
#endif

	KrRleSegment *seg, *segEnd;
	int dx, width;

	int count = isect.Height();
	KrRleLine* pLine = line +  isect.ymin - bounds.ymin;

/*#ifdef _WIN32_WCE
	//Don't use in now
	//Some strange draw in desktop 16 bits (cursor)
	//and in 1945.ged (in 32 bits)
	//Error on first actor of oggLoop.ged
	if(bpp == paintInfo->bytesPerPixel && !flags.IsSet( ALPHA ) && cForm.IsIdentity())
	{
		//maks: 27fps in fps benchmark.ged
		
		if ( bounds.xmin < clipping.xmin )
		{
			// LEFT HAND CASE. 
			// The rle is less than the intersection box.
			
			// The starting draw pixel in object coordinates.
			U16 objectStart = isect.xmin - bounds.xmin;
			// The last pixel drawn in object coordinates.
			U16 objectEnd   = isect.xmax - bounds.xmin;

			
			for (	; 
			count; 
			--count, targetLine += pitch, ++pLine )
			{
				
				seg	 = pLine->segment;
				segEnd = pLine->segment + pLine->nSegments;
				
				// First group: toss out everything left of the clipping box.
				while (    seg < segEnd 
					&& seg->getEnd() < objectStart )
					++seg;
				
				if ( seg == segEnd )
					continue;
				
				// Second pass: might clip left, might clip right.
				// Might have complete jumped over the clipping box.
				if ( seg->getStart() > objectEnd )
					continue;
				
				dx = 0;
				if ( seg->getStart() < objectStart )
					dx = objectStart - seg->getStart();
				
				width = GlMin( seg->getEnd(), objectEnd ) - ( seg->getStart() + dx ) + 1;

				GLASSERT( !seg->Alpha() );
				
#ifdef _WIN32_WCE
				memcpy( targetLine + (seg->getStart() + dx) * bpp, seg->RGBA() + (dx * seg->getBytesPerPixel()), width << 1 );			
#else
				switch(bpp)
				{
				case 2:
					memcpy( targetLine + (seg->getStart() + dx) * bpp, seg->RGBA() + (dx * seg->getBytesPerPixel()), width << 1 );			
					break;

				case 3:
					memcpy( targetLine + (seg->getStart() + dx) * bpp, seg->RGBA() + (dx * seg->getBytesPerPixel()), width * 3 );			
					break;

				case 4:
					memcpy( targetLine + (seg->getStart() + dx) * bpp, seg->RGBA() + (dx * seg->getBytesPerPixel()), width << 2 );			
					break;
				}
#endif
								
				++seg;
				
				// Third pass: won't clip left, won't clip right.
				// SKIPPED
				
				// Fourth pass: won't clip left, might clip right.			
				while (    seg < segEnd 
					&& seg->getStart() <= objectEnd )
				{
					width = GlMin( seg->getEnd(), objectEnd ) - seg->getStart() + 1;
					
					GLASSERT( !seg->Alpha() );
					
#ifdef _WIN32_WCE
				memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 1 );
#else
				switch(bpp)
				{
				case 2:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 1 );					
					break;

				case 3:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width * 3 );					
					break;

				case 4:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 2 );										
					break;
				}
#endif
										
					++seg;
				}
			}
		}
		else
		{
			// RIGHT HAND CASE. 
			// The rle is greater than or equal to the clip box.
			U16 objectEnd   = isect.xmax - bounds.xmin;
			
			for ( ; count; --count )
			{
				seg	 = pLine->segment;
				segEnd = pLine->segment + pLine->nSegments;
				
				// Only pass: won't clip left, might clip right.			
				while (    seg < segEnd 
					&& seg->getStart() <= objectEnd )
				{
					width = GlMin( seg->getEnd(), objectEnd ) - seg->getStart() + 1;
					
					GLASSERT( !seg->Alpha() );
					
#ifdef _WIN32_WCE
				memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 1 );
#else
				switch(bpp)
				{
				case 2:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 1 );			
					break;

				case 3:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width * 3 );									
					break;

				case 4:
					memcpy( targetLine + (seg->getStart()) * bpp, seg->RGBA(), width << 2 );										
					break;
				}
#endif
					
					++seg;
				}
				targetLine += pitch;
				++pLine;
			}
		}
	
	}
	else
#endif*/
	{		
		U32 currentAlpha = 0;		// It's temping to get this from the first line, but
								// it's not guarenteed to exists. #(*#$ fonts.
		KrPaintFunc blitter = paintInfo->GetBlitter(false, cForm ); //maks
		
		
		
		/*if(clipping.HasInside(bounds)) //maks
		{
			KrRleSegment *seg = line->segment;
			
			do
			{
				if ( currentAlpha != seg->Alpha() )
				{
					currentAlpha = seg->Alpha();
					blitter = paintInfo->GetBlitter( ( currentAlpha != 0 ), cForm );
				}
				
				blitter(	paintInfo,
					targetLine + seg->getStart()*bpp, 
					seg->RGBA(), 
					seg->getBytesPerPixel(),
					seg->Len(), 
					cForm );
				
				if(seg->bEnd)
				{
					targetLine += pitch*seg->bEnd;
				}
				
				seg = seg->next;
			} while(seg);
			
			return;
		}*/
		
		
		
		
		
		
		//	{	
		//		int dcount = rand() % 5;
		//		
		//		KrPainter painter( paintInfo );
		//		painter.DrawFilledBox( isect.xmin, isect.ymin, isect.Width(), isect.Height(),
		//							   50 * dcount, 0, 200 - 50 * dcount );
		//	}
		
		
		
		if ( bounds.xmin < clipping.xmin )
		{
			// LEFT HAND CASE. 
			// The rle is less than the intersection box.
			
			// The starting draw pixel in object coordinates.
			U16 objectStart = isect.xmin - bounds.xmin;
			// The last pixel drawn in object coordinates.
			U16 objectEnd   = isect.xmax - bounds.xmin;
			
			GLASSERT( objectStart >= 0 && objectStart <= objectEnd );
			GLASSERT( objectEnd   >= 0 && objectEnd < bounds.Width() );
			
			for (	; 
			count; 
			--count, targetLine += pitch, ++pLine )
			{
				GLASSERT( pLine >= &line[0] );
				GLASSERT( pLine <= &line[ size.y - 1 ] );
				
				seg	 = pLine->segment;
				segEnd = pLine->segment + pLine->NumSegments(); //maks
				
				// First group: toss out everything left of the clipping box.
				while (    seg < segEnd 
					&& seg->getEnd() < objectStart )
					++seg;
				
				if ( seg == segEnd )
					continue;
				
				// Second pass: might clip left, might clip right.
				// Might have complete jumped over the clipping box.
				if ( seg->getStart() > objectEnd )
					continue;
				
				dx = 0;
				if ( seg->getStart() < objectStart )
					dx = objectStart - seg->getStart();
				
				width = GlMin( seg->getEnd(), objectEnd ) - ( seg->getStart() + dx ) + 1;
				
				GLASSERT( width > 0 );
				
				if ( currentAlpha != seg->Alpha() )
				{
					currentAlpha = seg->Alpha();
					blitter = paintInfo->GetBlitter( ( currentAlpha != 0 ), cForm );
				}
				
				GLASSERT( seg->getStart() + dx + width - 1 <= seg->getEnd() );
				GLASSERT( width > 0 );
				
				blitter(	paintInfo, //maks
						targetLine + (seg->getStart() + dx) * bpp, 
						seg->RGBA() + (dx * seg->getBytesPerPixel()), 
						seg->getBytesPerPixel(), 
						width, 
						cForm );
			
				++seg;
				
				// Third pass: won't clip left, won't clip right.
				// SKIPPED
				
				// Fourth pass: won't clip left, might clip right.			
				while (    seg < segEnd 
					&& seg->getStart() <= objectEnd )
				{
					width = GlMin( seg->getEnd(), objectEnd ) - seg->getStart() + 1;
					GLASSERT( width > 0 );
					
					if ( currentAlpha != seg->Alpha() )
					{
						currentAlpha = seg->Alpha();
						blitter = paintInfo->GetBlitter( ( currentAlpha != 0 ), cForm );
					}
					
					GLASSERT( seg->getStart() + width - 1 <= seg->getEnd() );
					GLASSERT( width > 0 );
					
					blitter(	paintInfo, //maks
							targetLine + (seg->getStart()) * bpp, 
							seg->RGBA(), 
							seg->getBytesPerPixel(),
							width, 
							cForm );
					
					
					
					++seg;
				}
			}
		}
		else
		{
			// RIGHT HAND CASE. 
			// The rle is greater than or equal to the clip box.
			U16 objectEnd   = isect.xmax - bounds.xmin;
			
			GLASSERT( objectEnd >= 0 && objectEnd < bounds.Width() );
			
			for ( ; count; --count )
			{
				GLASSERT( pLine >= &line[0] );
				GLASSERT( pLine <= &line[ size.y - 1 ] );
				
				seg	 = pLine->segment;
				segEnd = pLine->segment + pLine->NumSegments(); //maks
				
				// Only pass: won't clip left, might clip right.			
				while (    seg < segEnd 
					&& seg->getStart() <= objectEnd )
				{
					width = GlMin( seg->getEnd(), objectEnd ) - seg->getStart() + 1;
					
					if ( currentAlpha != seg->Alpha() )
					{
						currentAlpha = seg->Alpha();
						blitter = paintInfo->GetBlitter( ( currentAlpha != 0 ), cForm );
					}
					
					GLASSERT( seg->getStart() + width - 1 <= seg->getEnd() );
					GLASSERT( width > 0 );
					
					blitter(	paintInfo, //maks
							targetLine + (seg->getStart()) * bpp, 
							seg->RGBA(), 
							seg->getBytesPerPixel(),
							width, 
							cForm );
					
					
					++seg;
				}
				targetLine += pitch;
				++pLine;
			}
		}
	}


	#ifdef DRAWDEBUG_RLE
		if ( xForm.x.ToIntRound() >=0 && xForm.y.ToIntRound() >= 0 && xForm.x.ToIntRound() < paintInfo->width && xForm.y.ToIntRound() < paintInfo->height )
 			painter.SetPixel( xForm.x.ToIntRound(), xForm.y.ToIntRound(), red );
	#endif

	}

/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
	if(flags.IsSet(INDEXED_FRAME))
	{
		//Release memory
		delete [] bigData;
		bigData = NULL;
	}
#endif*/
}


bool KrRle::HitTestTransformed( int _x, int _y, int hitFlags)
{
#ifdef STAND_ALONE_GAME //maks
	if(flags.IsSet(INDEXED_FRAME) && !bigData)
	{
		//On demmand frame loading
		if(!LoadIndexedFrame())
		{
			return false;
		}		
	}
#endif

	{
	CMemControl memControl(bigData, bigDataUsingGC);
	int i;

	// We are in untransformed coordinates, so check the bounds
	// against the identity matrix
	KrMatrix2 matrix;
	matrix.Set();
	KrRect b;
	CalculateBounds( matrix, &b );
	if ( !b.Intersect( _x, _y ) )
	{
/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
		if(flags.IsSet(INDEXED_FRAME))
		{
			//Release memory
			delete [] bigData;
			bigData = NULL;
		}
#endif*/
		return false;
	}

	int x = _x - deltaHotToOrigin.x;
	int y = _y - deltaHotToOrigin.y;

	if ( y >= 0 && y < size.y )
	{
		// Get a pixel in the RLE.
		for ( i=0; i<line[y].NumSegments(); i++ )
		{
			KrRleSegment* segment = line[y].Segment( i );
			if ( x >= segment->Start() && x <= segment->End() )
			{
				if(segment->getBytesPerPixel() == 4) //maks
				{
					KrRGBA* rgba = (KrRGBA *)(segment->RGBA() + (x - segment->Start())*segment->getBytesPerPixel()); //maks

					bool res = ( rgba->c.alpha != KrRGBA::KR_TRANSPARENT );
					
/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
					if(flags.IsSet(INDEXED_FRAME))
					{
						//Release memory
						delete [] bigData;
						bigData = NULL;
					}
#endif*/

					return res;
				}
				else
				{
/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
					if(flags.IsSet(INDEXED_FRAME))
					{
						//Release memory
						delete [] bigData;
						bigData = NULL;
					}
#endif*/
					return true; //Opaque pixels
				}
			}
		}
	}		
	
/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
	if(flags.IsSet(INDEXED_FRAME))
	{
		//Release memory
		delete [] bigData;
		bigData = NULL;
	}
#endif*/
	}

	return false;
}








KrCollisionMap* KrRle::GetCollisionMap( GlFixed xScale, GlFixed yScale )
{
	if ( collisionMap )
	{
		GLASSERT( collisionMap->XScale() == xScale && collisionMap->YScale() == yScale );
	}
	else
	{
#ifdef STAND_ALONE_GAME //maks
		if(flags.IsSet(INDEXED_FRAME) && !bigData)
		{
			//On demmand frame loading
			if(!LoadIndexedFrame())
			{
				return NULL;
			}
		}
#endif

		{
			CMemControl memControl(bigData, bigDataUsingGC);
			collisionMap = new KrCollisionMap( xScale, yScale, size.x, size.y );
			bitMapArea = 0; //maks
			
			for( int j=0; j<size.y; ++j )
			{
				GlHighBitWriter<U32> writer( collisionMap->GetRow( j ) );
				
				// Walk the line.
				int prevX = 0;
				for( int i=0; i<line[j].NumSegments(); ++i )
				{
					KrRleSegment* segment = line[j].Segment( i );
					writer.Skip_N( segment->Start() - prevX );
					
					//maks
					int bits1 = segment->End() - segment->Start() + 1;
					writer.Push_1N( bits1 );
					bitMapArea += bits1;
					
					prevX = segment->End() + 1;
				}
			}
		}

/*#ifdef STAND_ALONE_GAME //maks: don't need this with malloc_gc
	if(flags.IsSet(INDEXED_FRAME))
	{
		//Release memory
		delete [] bigData;
		bigData = NULL;
	}
#endif*/
	}
	
	return collisionMap;
}



/*bool KrRle::operator==( const KrRle& other ) const
{
	if( size != other.size ||
		deltaHotToOrigin != other.deltaHotToOrigin ||
		flags.ToU8() != other.flags.ToU8() ||
		delta != other.delta)
		return false;

	//Compare lines and segments
	for(int i=0; i<size.y; i++)
	{
		if(line[i].nSegments != other.line[i].nSegments)
			return false;

		for(int is = 0; is < line[i].nSegments; is++)
		{
			if(line[i].segment[is].getBytesPerPixel() != other.line[i].segment[is].getBytesPerPixel())
				return false;
			
			if(line[i].segment[is].Len() != other.line[i].segment[is].Len())
				return false;

			if(memcmp(line[i].segment[is].rgba, other.line[i].segment[is].rgba, 
				line[i].segment[is].Len()*line[i].segment[is].getBytesPerPixel()) != 0)
				return false;
		}
	}

	return true;
}*/

/*Compression comp;

U8 workBuf[4096];
U8 *KrRleSegment::RGBA() //maks
{
	if(rgba) return rgba;

	//Decompression (slow)
	comp.Decompression(segComp, lenComp, workBuf, Len()*getBytesPerPixel());

	return workBuf;
}*/


KrCanvasResource* KrRle::CreateCanvasResource( int* hotx, int* hoty, bool bUseBorder ) //maks
{
	KrMatrix2 xForm;
	xForm.Set();
	KrRect bounds;

 	CalculateBounds( xForm, &bounds );


	*hotx = -bounds.xmin;
	*hoty = -bounds.ymin;
	
	//Use a border and OGL_OFFSET to solve artifacts in nvidia cards
	KrCanvasResource* canvasRes = new KrCanvasResource( "fromSprite",
		bounds.Width() + (bUseBorder?2:0), //maks
		bounds.Height() + (bUseBorder?2:0), //maks
														true );
	KrPaintInfo info(	canvasRes->Pixels(), 
						canvasRes->Width(), 
						canvasRes->Height() );

	KrMatrix2 paintForm;
	paintForm.Set( -bounds.xmin + (bUseBorder?OGL_OFFSET:0), -bounds.ymin + (bUseBorder?OGL_OFFSET:0) );

	KrRect clip;
	clip.Set( 0, 0, canvasRes->Width()-1 , canvasRes->Height()-1 );
	KrColorTransform cform;

	Draw( &info, paintForm, cform, clip, 0 );
	return canvasRes;	
}

//#include "C:/Downloads/SDL_gfx-2.0.13/SDL_rotozoom.c" //maks
KrRle* KrRle::CreateScaledRle( GlFixed xScale, GlFixed yScale, int* hotx, int* hoty )
{
	int normalx, normaly;
	KrCanvasResource* normal = CreateCanvasResource( &normalx, &normaly );
	GLASSERT( normal );

	// Now we need something to draw the scaled canvas to.
	// First get the size and then make a pixel block.
	KrMatrix2 scaledXForm;
	scaledXForm.Set( 0, 0, xScale, yScale );
	KrRect scaledBounds;
	normal->CalculateBounds( scaledXForm, &scaledBounds );

	KrPixelBlock pixelBlock;
	pixelBlock.Create( scaledBounds.Width(), scaledBounds.Height(), true );

	// Compute a new hotspot and rle.
	GlFixed scaledHotX   = -xScale * deltaHotToOrigin.x;
	GlFixed scaledHotY   = -yScale * deltaHotToOrigin.y;
	GlFixed scaledDeltaX = xScale * delta.x;
	GlFixed scaledDeltaY = yScale * delta.y;

	// Draw to the pixel block from the canvas.
	KrPaintInfo info( pixelBlock.Pixels(), pixelBlock.Width(), pixelBlock.Height() );

	KrColorTransform color;
	KrRect clip;
	clip.Set( 0, 0, pixelBlock.Width()-1, pixelBlock.Height()-1 );
	normal->Draw( &info, scaledXForm, color, clip, 
		KrQualityAdaptive, 
		0 );

	// Encode it!
	KrRle* rle = new KrRle();
	rle->Create( &info, 0, 0, 
				 pixelBlock.Width(),        pixelBlock.Height(),
				 scaledHotX.ToIntRound(),   scaledHotY.ToIntRound(),
				 scaledDeltaX.ToIntRound(), scaledDeltaY.ToIntRound() );


	// Clean up and return.
	delete normal;
	return rle;

	//maks rotozoom
	//Create a SDL_Surface from current rle
	/*SDL_Surface *src = SDL_CreateRGBSurface(SDL_SWSURFACE,
				Width(),
				Height(),
				32,
				0x00ff0000,	
				0x0000ff00,
				0x000000ff,
				0xff000000 );

	KrPaintInfo info(src);

	KrMatrix2 xForm;
	xForm.Set();
	KrRect bounds;

 	CalculateBounds( xForm, &bounds );

	KrMatrix2 paintForm;
	paintForm.Set( -bounds.xmin, -bounds.ymin );
	KrRect clip;

	clip.Set( 0, 0, Width() - 1, Height() - 1 );

	KrColorTransform cform;
	Draw( &info, paintForm, cform, clip, 0 );


	//Scale
	if(src->w > 0 && src->h > 0)
	{
		SDL_Surface *dst = rotozoomSurface(src, 12, xScale.ToDouble(), 0);
		SDL_FreeSurface(src);
		
		// Compute a new hotspot and rle.
		GlFixed scaledHotX   = -xScale * deltaHotToOrigin.x;
		GlFixed scaledHotY   = -yScale * deltaHotToOrigin.y;
		GlFixed scaledDeltaX = xScale * delta.x;
		GlFixed scaledDeltaY = yScale * delta.y;
		
		
		// Encode it!
		KrPaintInfo infoDst(dst);
		
		KrRle* rle = new KrRle();
		rle->Create( &infoDst, 0, 0, 
			dst->w,        dst->h,
			scaledHotX.ToIntRound(),   scaledHotY.ToIntRound(),
			scaledDeltaX.ToIntRound(), scaledDeltaY.ToIntRound() );
		
		// Clean up and return.
		SDL_FreeSurface(dst);

		return rle;
	}
	else
	{
		//Empty rle
		KrRle* rle = new KrRle();

		rle->deltaHotToOrigin = deltaHotToOrigin;
		rle->delta = delta;
		rle->size = size;

		return rle;
	}

	return NULL;*/
}

bool KrRle::Create(	KrPaintInfo* surface, 
					int x, int y, 
					int width, int height,
					int hotspotX, int hotspotY,
					int deltax, int deltay )
{
	KrPainter painter( surface );

	if(painter.getColorKey()) //maks
		colorKey = *painter.getColorKey();

	#ifdef DEBUG
		// Check our inputs:
		GLASSERT( x >=0 );
		GLASSERT( x + width - 1 < surface->width );
		GLASSERT( y >=0 );
		GLASSERT( y + height - 1 < surface->height );
	#endif

	int j;
	GLASSERT( line == 0 );
	delta.x = deltax;
	delta.y = deltay;

	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	int calcWidth = 0, calcHeight = 0;
	bool empty = true;

#ifndef STAND_ALONE_GAME
	originalFrameSize.Set(surface->width, surface->height);
	originalHotSpot.Set(hotspotX, hotspotY);
	if(surface->transparentColor)
	{
		bHasTransparentColor = true;
		originalTransparentColor = *(surface->transparentColor);
	}
	else
	{
		bHasTransparentColor = false;
	}
#endif
	
	// Start by finding the bounding box for this Rle.
	for( minX = x; minX < x + width; ++minX )
	{
		// If the column is not transparent, stop looking.
		if ( height != painter.CalcTransparentColumn( y, y+height-1, minX )	 )
			break;
	}

	if ( minX != x + width )
	{
		// Not empty. (Happens -- often -- with fonts.)
		empty = false;
		for( maxX = x + width - 1; maxX >= minX; --maxX )
		{
			if ( height != painter.CalcTransparentColumn( y, y+height-1, maxX )	 )
				break;
		}
		GLASSERT( maxX >= minX );
		calcWidth = maxX - minX + 1;

		for( minY = y; minY < y + height; ++minY )
		{
			if ( calcWidth != painter.CalcTransparentRun( minX, maxX, minY ) )
				break;
		}
		GLASSERT( minY < y + height );

		for( maxY = y + height - 1; maxY >= minY; --maxY )
		{
			if ( calcWidth != painter.CalcTransparentRun( minX, maxX, maxY ) )
				break;
		}
		GLASSERT( maxY >= minY );
		calcHeight = maxY - minY + 1;
	}

	if ( !empty )
	{
		// Allocate a line for each y pixel. We may
		// not use them all.
		//KrRleSegment *lastSegment = NULL;
		line = new KrRleLine[ calcHeight ];

		#ifdef USE_COMPILED_SPRITES
		compile_newCode();
		#endif

		for ( j=0; j<calcHeight; j++ )
		{
			// We have an RLE line.
			/*lastSegment =*/ line[j].Create( surface, minX, minY + j, calcWidth/*, lastSegment*/ );

			// Set our affected flags:
			if ( line[j].Alpha() )
				flags.Set( ALPHA );
		}

		#ifdef USE_COMPILED_SPRITES
		spriteCode = compile_endCode();
		#endif
	}

	
	// We have the delta from the hotspot to the user origin -
	// adjust for the caculated origin.
	deltaHotToOrigin.x = minX - hotspotX;
	deltaHotToOrigin.y = minY - hotspotY;

/*	GLASSERT( deltaHotToOrigin.x > -4000 );	// sanity
	GLASSERT( deltaHotToOrigin.y > -4000 );	// sanity
	GLASSERT( deltaHotToOrigin.x < 4000 );	// sanity
	GLASSERT( deltaHotToOrigin.y < 4000 );	// sanity*/

	if ( empty )
	{
		size.x = 0;
		size.y = 0;
	}
	else
	{
		size.x = calcWidth;
		size.y = calcHeight;

		#ifdef DEBUG

		int mins, maxs;
		for( int j=0; j<calcHeight; ++j )
		{
			int nSegments = line[j].NumSegments(); //maks
			for( int i=0; i<nSegments; ++i )
			{
				if ( i == 0 && j == 0 )
				{
					mins = line[j].segment[i].getStart();
					maxs = line[j].segment[i].getEnd();
				}
				else
				{
					mins = GlMin( mins, (int) line[j].segment[i].getStart() );
					maxs = GlMax( maxs, (int) line[j].segment[i].getEnd() );
				}
			}
		}
		GLASSERT( maxs - mins + 1 == calcWidth );

		#endif
	}

	return ( !empty );
}

/*KrRleSegment **/ void KrRleLine::Create( KrPaintInfo* surface, int x, int y, int w/*, KrRleSegment *lastSegment*/ ) //maks
{
	GLASSERT( NumSegments() == 0 );
	KrPainter painter( surface );

	//Count segments
	{ //maks
		int currentX = x;
		int xmax = x + w - 1;
		
		// Check for non-transparent pixels, and create a line if needed.
		while ( currentX < x + w )
		{
			int skip = painter.CalcTransparentRun( currentX, xmax, y );
			currentX += skip;
			
			if ( currentX < x + w )
			{
				KrRleSegment seg;
				seg.Create( surface, currentX, x+w, y, x );
				
				if ( seg.Alpha() )
				{
					setFlags(getFlags() | ALPHA); //maks flags.Set( ALPHA );
				}
				
				currentX += seg.Len();
				setNumSegments(NumSegments() + 1); //maks //nSegments++;
			}
		}
	}

	//Allocate
	segment = new KrRleSegment[ NumSegments() ]; //maks
	setNumSegments(0); //nSegments = 0; //maks
	int currentX = x;
	int xmax = x + w - 1;

	// Check for non-transparent pixels, and create a line if needed.
	while ( currentX < x + w )
	{
		int skip = painter.CalcTransparentRun( currentX, xmax, y );
		currentX += skip;

		if ( currentX < x + w )
		{
			segment[NumSegments()].Create( surface, currentX, x+w, y, x ); //maks

			/*if(lastSegment)
			{
				lastSegment->next = &segment[nSegments];				
			}*/

			if ( segment[NumSegments()].Alpha() ) //maks
			{
				setFlags(getFlags() | ALPHA); //maks flags.Set( ALPHA );
			}

			currentX += segment[NumSegments()].Len();
			//lastSegment = &segment[nSegments];
			setNumSegments(NumSegments() + 1); //maks //nSegments++;
		}
	}

	/*if(lastSegment) 
	{
		if(nSegments) lastSegment->bEnd = 1;
		else lastSegment->bEnd++;
	}

	return lastSegment;*/
}

bool KrRleSegment::Create( KrPaintInfo* surface, 
						   int x, int xMax, int y, int objectOriginX )
{
	// Use surface coordinates until the very end, where the
	// objectOriginX is used to adjest to object coords.

	KrPainter painter( surface );
	bool alpha = false;
	U8 r, g, b, a;

	//start = end = x;
	setStart(x);
	setEnd(x);

	// calculate the length
	int opaque = -1; //maks
	while ( getEnd() < xMax )
	{
		painter.BreakPixel( getEnd(), y, &r, &g, &b, &a );

		if(opaque == -1)
		{
			//Set opaque flag for this segment
			opaque = (a == 255);
		}

		if ( a == 0)
			break;

		if ( a < 255 )
			alpha = true;

		if((a == 255) != opaque)
			break; //maks: break opaque and semitransparent segments

		//if(Len() == 32)
		//	break;

		setEnd(getEnd() + 1); //++end;
	}
	// End is an index, not a length. The above loop will
	// always go by once, after all:
	setEnd(getEnd() - 1);//--end;

	if ( alpha )
	{
		setFlagBit( ALPHA );
	}
	
	if(surface->bytesPerPixel == 4 && alpha) //maks
	{
		KrRGBA *rgba = new KrRGBA[ getEnd() - getStart() + 1 ];
		
		setStart(getStart() - objectOriginX); //start -= objectOriginX;
		setEnd(getEnd() - objectOriginX); //end   -= objectOriginX;
		
		for ( int i=0; i<Len(); ++i )
		{
			painter.BreakPixel( x + i, y, &r, &g, &b, &a );
			rgba[i].c.alpha = a;
			rgba[i].c.red   = r;
			rgba[i].c.green = g;
			rgba[i].c.blue  = b;
		}

		this->rgba = (U8 *)rgba;
		setBytesPerPixel(4);
	}
	else if(surface->bytesPerPixel == 4 || surface->bytesPerPixel == 3) 
	{
		U8 *rgb = new U8[ (getEnd() - getStart() + 1)*3 ];
		rgba = rgb;
		
		setStart(getStart() - objectOriginX); //start -= objectOriginX;
		setEnd(getEnd() - objectOriginX); //end   -= objectOriginX;
		

		for ( int i=0; i<Len(); ++i )
		{
			painter.BreakPixel( x + i, y, &r, &g, &b, &a );

			//24 bits source image always has this mask:
			//source rgb =	0x00ff0000,	
			//				0x0000ff00,
			//				0x000000ff

			rgb[2] = r;
			rgb[1] = g;
			rgb[0] = b;

			/*U32 color = SDL_MapRGB( surface->surface->format, r, g, b );
			memcpy(rgb, &color, 3);*/

			rgb += 3;
		}

		setBytesPerPixel(3);
	}
	else if(surface->bytesPerPixel == 2) 
	{
		U16 *rgb16 = new U16[ (getEnd() - getStart() + 1) ];
		rgba = (U8 *)rgb16;
		
		setStart(getStart() - objectOriginX); //start -= objectOriginX;
		setEnd(getEnd() - objectOriginX); //end   -= objectOriginX;
		
		for ( int i=0; i<Len(); ++i )
		{
			painter.BreakPixel( x + i, y, &r, &g, &b, &a );
			*rgb16 = ( ( r >> surface->redLoss )   << surface->redShift ) 
				   | ( ( g >> surface->greenLoss ) << surface->greenShift ) 
				   | ( ( b >> surface->blueLoss )  << surface->blueShift );

			rgb16++;
		}

		setBytesPerPixel(2);

		#ifdef USE_COMPILED_SPRITES
		compile_addSegmentToCode(x, y, (U16*)rgba, Len());
		#endif
	}

	//Compress();
	return true;
}

#ifndef STAND_ALONE_GAME //maks

void KrRleLine::CalcRange( KrRGBA* minColor, KrRGBA* maxColor )
{
	for( int i=0; i<NumSegments(); ++i )
	{
		segment[i].CalcRange( minColor, maxColor );
	}
}




/*void KrRleSegment::Compress() //maks
{
	int lenght = Len()*getBytesPerPixel();
	segComp = (U8 *)malloc(lenght + lenght / 8 + 256);
	if(!segComp) return;
	
	lenComp = comp.DoCompression(rgba, lenght, segComp, lenght + lenght / 8 + 256);

	if(lenComp >= lenght)
	{
		//No compression
		lenComp = 0;
		free(segComp);
		segComp = NULL;
	}
	else
	{
		segComp = (U8 *)realloc(segComp, lenComp);
		if(!KrRle::memoryPoolRGBA) delete [] rgba;
		rgba = NULL;
	}
}*/



bool KrRleSegment::Write(	GlWriteBitStream* writer, 
							KrRGBA minColor,
							KrRGBA bits ) //maks
{
	writer->WriteBits( getFlags(), BITS_USED );
	
	int bitsInRange = GlBitStream::BitsNeeded( GlMax( getStart(), getEnd() ) );
	GLASSERT( bitsInRange < 0x10000 );

	// The range is from 0 - 64k. bitsInRange will be from 0-15. 
	// Therefore 4 bits are needed to store the range.
	writer->WriteBits( bitsInRange, 4 ); 
	writer->WriteBits( getStart(), bitsInRange );
	writer->WriteBits( getEnd(), bitsInRange );


	if(getBytesPerPixel() == 3 && KrRle::getConvertImage()) 
		writer->WriteBits( 2, 3 ); //Save 16 bit segment
	else
		writer->WriteBits( getBytesPerPixel(), 3 ); 
	
	//Write compressed
	writer->Flush();


	if(getBytesPerPixel() == 3 && KrRle::getConvertImage()) 
	{
		U16 *buf16 = Convert24To16();
		SDL_RWwrite(writer->getFp(), buf16, Len()*2, 1);
		delete [] buf16;
	}
	else
		SDL_RWwrite(writer->getFp(), rgba, Len()*getBytesPerPixel(), 1);
	

	//Too slow on PocketPC
	/*if(bytesPerPixel == 4) 
	{
		for ( int i=0; i<Len(); ++i )
		{
			KrRGBA *rgba = (KrRGBA *)this->rgba;

			writer->WriteBits( rgba[i].c.red   - minColor.c.red,   bits.c.red );
			writer->WriteBits( rgba[i].c.green - minColor.c.green, bits.c.green );
			writer->WriteBits( rgba[i].c.blue  - minColor.c.blue,  bits.c.blue );
			writer->WriteBits( rgba[i].c.alpha - minColor.c.alpha, bits.c.alpha );
		}
	}
	else if(bytesPerPixel == 3) 
	{
		U8 *rgb = rgba;
		for ( int i=0; i<Len(); ++i )
		{
			writer->WriteBits( rgb[0]  - minColor.c.red,   bits.c.red );
			writer->WriteBits( rgb[1]  - minColor.c.green, bits.c.green );
			writer->WriteBits( rgb[2]  - minColor.c.blue,  bits.c.blue );

			rgb += 3;
		}
	}
	else if(bytesPerPixel == 2) 
	{
		U16 *rgb16 = (U16 *)rgba;
		for ( int i=0; i<Len(); ++i )
		{
			writer->WriteBits(*rgb16, 16);
			rgb16++;
		}
	}*/

	return true;
}

bool KrRleSegment::Read(	GlReadBitStream* reader, 
							KrRGBA minColor,
							KrRGBA bits ) //maks
{
	setFlags( reader->ReadBits( BITS_USED ) );

	int bitsInRange = reader->ReadBits( 4 );
	setStart(reader->ReadBits( bitsInRange ));
	setEnd(reader->ReadBits( bitsInRange ));

	GLASSERT( Len() > 0 );
	GLASSERT( Len() < 2000 );	// not true, but good reality check

	if(KrResourceVault::getVersionDat() >= 2) 
	{
		setBytesPerPixel(reader->ReadBits( 3 ));
	}
	else
		setBytesPerPixel(4);

	// Allocate the RGBA. Use a memory pool, if available.
	rgba = 0;
	if ( KrRle::memoryPoolRGBA )
	{
		rgba = (U8*) KrRle::memoryPoolRGBA->Alloc( Len() * getBytesPerPixel() ); 
		setFlagBit( MEMORYPOOL );
	}

	if(!rgba ) //maks
	{
		rgba = new U8[ Len()*getBytesPerPixel() ];
	}

	#ifdef DEBUG
		numRGBA += Len();
	#endif

	if(KrResourceVault::getVersionDat() >= 2)
	{
		//Read compressed
		reader->Flush();

		SDL_RWread(reader->getFp(), rgba, Len()*getBytesPerPixel(), 1);
	}
	else
	{		
		if(getBytesPerPixel() == 4) //maks
		{
			for ( int i=0; i<Len(); ++i )
			{
				KrRGBA *rgba = (KrRGBA *)this->rgba;
				
				rgba[i].c.red   = minColor.c.red   + reader->ReadBits( bits.c.red );
				rgba[i].c.green = minColor.c.green + reader->ReadBits( bits.c.green );
				rgba[i].c.blue  = minColor.c.blue  + reader->ReadBits( bits.c.blue );
				rgba[i].c.alpha = minColor.c.alpha + reader->ReadBits( bits.c.alpha );
			}
		}
		else if(getBytesPerPixel() == 3) 
		{
			U8 *rgb = rgba;
			for ( int i=0; i<Len(); ++i )
			{
				rgb[0]  = minColor.c.red   + reader->ReadBits( bits.c.red );
				rgb[1]  = minColor.c.green + reader->ReadBits( bits.c.green );
				rgb[2]  = minColor.c.blue  + reader->ReadBits( bits.c.blue );
				
				rgb += 3;
			}
		}
		else if(getBytesPerPixel() == 2) 
		{
			U16 *rgb16 = (U16 *)rgba;
			for ( int i=0; i<Len(); ++i )
			{
				*rgb16 = reader->ReadBits(16);
				rgb16++;
			}
		}
	}

	return true;
}

void KrRleSegment::CalcRange( KrRGBA* minColor, KrRGBA* maxColor )
{
	int length = Len();
	for( int i=0; i<length; ++i )
	{
		if(getBytesPerPixel() == 4)
		{
			KrRGBA *rgba = (KrRGBA *)this->rgba;

			for( int j=0; j<4; ++j )
			{
				minColor->array[j] = GlMin( minColor->array[j], rgba[i].array[j] );
				maxColor->array[j] = GlMax( maxColor->array[j], rgba[i].array[j] );
			}
		}
		else if(getBytesPerPixel() == 3)
		{
			U8 *rgb = rgba + 3*i;

			for( int j=0; j<3; ++j )
			{
				minColor->array[j] = GlMin( minColor->array[j], rgb[j] );
				maxColor->array[j] = GlMax( maxColor->array[j], rgb[j] );
			}

			minColor->array[3] = GlMin( minColor->array[3], (U8)255 );
			maxColor->array[3] = GlMax( maxColor->array[3], (U8)255 );
		}
		else if(getBytesPerPixel() == 2)
		{
			U16 *rgb16 = (U16 *)(rgba + 2*i);
			
			U8 r = GET_RED_16(rgb16);
			U8 g = GET_GREEN_16(rgb16);
			U8 b = GET_BLUE_16(rgb16);
			
			minColor->array[0] = GlMin( minColor->array[0], r );
			maxColor->array[0] = GlMax( maxColor->array[0], r );

			minColor->array[1] = GlMin( minColor->array[1], g );
			maxColor->array[1] = GlMax( maxColor->array[1], g );

			minColor->array[2] = GlMin( minColor->array[2], b );
			maxColor->array[2] = GlMax( maxColor->array[2], b );

			minColor->array[3] = GlMin( minColor->array[3], (U8)255 );
			maxColor->array[3] = GlMax( maxColor->array[3], (U8)255 );
		}
	}
}


bool KrRleLine::Write( GlWriteBitStream* writer, KrRGBA minColor, KrRGBA bits )
{
	GLASSERT( NumSegments() >= 0 );

	writer->WriteBits( (U32)getFlags(), BITS_USED ); //maks
	
	int bitsNeeded = GlBitStream::BitsNeeded( NumSegments() );
	// There can be 64k segments. 0-15 bits. We need 4 bits to represent that range.
	writer->WriteBits( bitsNeeded, 4 );
	writer->WriteBits( NumSegments(), bitsNeeded );

	for ( int i=0; i<NumSegments(); i++ )
	{
		segment[i].Write( writer, minColor, bits );
	}
	return true;
}

/*KrRleSegment **/ void KrRleLine::Read( GlReadBitStream* reader, KrRGBA minColor, KrRGBA bits/*, KrRleSegment *lastSegment*/ )
{
	setFlags(reader->ReadBits( BITS_USED )); //maks //flags.FromU32( reader->ReadBits( BITS_USED ) );
	int bitsNeeded = reader->ReadBits( 4 );
	setNumSegments(reader->ReadBits( bitsNeeded )); //maks
	int nSegments = NumSegments();

	// Lines can have no segments: any blank line.
	GLASSERT( nSegments >= 0 );
	GLASSERT( nSegments < 2000 );	// sanity check

	if ( nSegments > 0 )
	{
		if ( KrRle::memoryPoolSegment )
		{
			segment = (KrRleSegment*) KrRle::memoryPoolSegment->Alloc( nSegments * sizeof( KrRleSegment ) );
			setFlags(getFlags() | MEMORYPOOL); //maks //flags.Set( MEMORYPOOL );
		}
		else		
		{
			segment = new KrRleSegment[ nSegments ];
		}

		for ( int i=0; i<nSegments; i++ )
		{
			segment[i].Clear();
			segment[i].Read( reader, minColor, bits );

			/*if(lastSegment)
			{
				lastSegment->next = &segment[i];				
			}

			lastSegment = &segment[i];*/
		}
	}

	/*if(lastSegment) 
	{
		if(nSegments) lastSegment->bEnd = 1;
		else lastSegment->bEnd++;
	}

	return lastSegment;*/
}




U32 KrRle::GetTotalSize()
{
	CMemControl memControl(bigData, bigDataUsingGC);
	Uint32 lenght = size.y*sizeof(KrRleLine);
	
	for(int i=0; i<size.y; i++)
	{
		for(int is = 0; is < line[i].NumSegments(); is++)
		{
			if(line[i].segment[is].getBytesPerPixel() == 3 && KrRle::getConvertImage())
				lenght += line[i].segment[is].Len()*2;
			else
				lenght += line[i].segment[is].Len()*line[i].segment[is].getBytesPerPixel();
		}
		
		lenght += line[i].NumSegments()*sizeof(KrRleSegment);
	}

	return lenght;
}

bool KrRle::Write( SDL_RWops* fp, bool bOnDemandFrameLoad ) //maks
{
	#ifdef DEBUG
		GLOUTPUT( "Writing RLE: ("  );
		if ( flags.IsSet( ALPHA ) )
			GLOUTPUT( "ALPHA " );
	#endif

/*	GLASSERT( deltaHotToOrigin.x > -4000 );	// sanity
	GLASSERT( deltaHotToOrigin.y > -4000 );	// sanity
	GLASSERT( deltaHotToOrigin.x < 4000 );	// sanity
	GLASSERT( deltaHotToOrigin.y < 4000 );	// sanity*/

	if(bOnDemandFrameLoad)
	{
		flags.Set(INDEXED_FRAME);
	}
	else
	{
		flags.Clear(INDEXED_FRAME);
	}

	SDL_RWwrite(fp, &flags, sizeof(U8), 1); //SDL_WriteLE16( fp, flags.ToU32() );
	SDL_WriteLE32( fp, deltaHotToOrigin.x );
	SDL_WriteLE32( fp, deltaHotToOrigin.y );
	SDL_WriteLE32( fp, size.x );
	SDL_WriteLE32( fp, size.y );
	SDL_WriteLE32( fp, delta.x );
	SDL_WriteLE32( fp, delta.y );

	/////////////////////////////////////////////////////////////
	//Write a big data block with all lines and segments
	//Line1Line2Line3...Seg1Seg2...bufSeg1bufSeg2...

	//Calculate lenght
	Uint32 lenght = GetTotalSize();
	
	if(lenght)
	{
		SDL_RWops *memFp = fp;
		U8 *tmp = NULL;

		//Write data lenght 
		SDL_WriteLE32( fp, lenght );

		if(flags.IsSet(INDEXED_FRAME))
		{
			//Save in memory
			tmp = new U8[lenght];
			memFp = SDL_RWFromMem(tmp, lenght);
		}
		else
		{
			//No compressed
			SDL_WriteLE32( fp, lenght );
		}
		
		
		//Write lines
		int i;
		for(i=0; i<size.y; i++ )
		{
			line[i].WriteLine(memFp);
		}		
		
		//Write segments
		for(i=0; i<size.y; i++ )
		{
			line[i].WriteSegments(memFp);
		}
		
		//Write segment buffers
		for(i=0; i<size.y; i++ )
		{
			line[i].WriteSegmentBuffers(memFp);
		}

		if(tmp)
		{
			//Compress
			U8 *out = new U8[lenght + lenght / 8 + 256];

			Compression comp;
			Uint32 lenghtComp = comp.DoCompression(tmp, lenght, out, lenght + lenght / 8 + 256);

			if(lenghtComp < lenght)
			{
				//Compression ok
				SDL_WriteLE32( fp, lenghtComp );
				SDL_RWwrite(fp, out, lenghtComp, 1);
			}
			else
			{
				//Save uncompressed data
				SDL_WriteLE32( fp, lenght );
				SDL_RWwrite(fp, tmp, lenght, 1);
			}
	

			SDL_RWclose(memFp);
			delete [] tmp;
			delete [] out;
		}
	}
	else
	{
		SDL_WriteLE32( fp, 0 );	
		SDL_WriteLE32( fp, 0 );	
	}

	/////////////////////////////////////////////////////////////

	// Get the ranges of the image data:
	/*KrRGBA minColor, maxColor, bits;
	int i;
	if ( size.y > 0 )
	{
		minColor.Set( 255, 255, 255, 255 );
		maxColor.Set( 0, 0, 0, 0 );
		for( i=0; i<size.y; ++i )
		{
			line[i].CalcRange( &minColor, &maxColor );
		}
	}
	else
	{
		// (#*%(@ empty font sprites
		minColor.Set( 0, 0, 0, 0 );
		maxColor.Set( 0, 0, 0, 0 );
	}

	GLASSERT( minColor.c.red <= maxColor.c.red );
	GLASSERT( minColor.c.green <= maxColor.c.green );
	GLASSERT( minColor.c.blue <= maxColor.c.blue );
	GLASSERT( minColor.c.alpha <= maxColor.c.alpha );

	// Write min and bits needed for each one:
	WriteByte( fp, minColor.c.red );
	bits.c.red = GlBitStream::BitsNeeded( maxColor.c.red - minColor.c.red );
	WriteByte( fp, bits.c.red );

	WriteByte( fp, minColor.c.green );
	bits.c.green = GlBitStream::BitsNeeded( maxColor.c.green - minColor.c.green );
	WriteByte( fp, bits.c.green );

	WriteByte( fp, minColor.c.blue );
	bits.c.blue = GlBitStream::BitsNeeded( maxColor.c.blue - minColor.c.blue );
	WriteByte( fp, bits.c.blue );

	WriteByte( fp, minColor.c.alpha );
	bits.c.alpha = GlBitStream::BitsNeeded( maxColor.c.alpha - minColor.c.alpha );
	WriteByte( fp, bits.c.alpha );

	GlWriteBitStream writer( fp );

	for ( i=0; i<size.y; i++ )
	{
		line[i].Write( &writer, minColor, bits );
	}

	writer.Flush();

	#ifdef DEBUG
		GLOUTPUT( ")\n size cx=%d cy=%d\n",
				  size.x, size.y );
	#endif*/

	return true;
}


bool KrRle::SaveImage(gedString& fileName) //maks
{
	//Save the rle in a BMP file if there is no transparent segments
	//Save the rle in a PNG file if have transparent segments
	//Append the extension to filename

	//Note: don't use BMP now
	//      To use BMP, must test solid images (always BMP)


	int j;
	bool bHasTransparency = false;
	
	/*if(size.x > 0 && size.y > 0)
	{
		for( j=0; j < size.y; ++j )
		{
			if(line[j].Alpha())
			{
				bHasTransparency = true;
				break;
			}
		}
	}

	if(!bHasTransparency && !bHasTransparentColor)
	{
		//Don't create a background color
		//Make a transparent image
		bHasTransparency = true;
	}*/

	//Always use PNG (solid image must be always PNG)
	bHasTransparency = true; 

	
	

	//Create surface
	SDL_Surface* surface;

	if(bHasTransparency)
	{
		//32 bits
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			originalFrameSize.x,
			originalFrameSize.y,
			32,
			0x00ff0000,	
			0x0000ff00,
			0x000000ff,
			0xff000000 );
	}
	else
	{
		//24 bits
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			originalFrameSize.x,
			originalFrameSize.y,
			24,
			0x00ff0000,	
			0x0000ff00,
			0x000000ff,
			0 );
	}

	if(!surface) return false;

	//Fill the background
	if(!bHasTransparency)
	{
		//Get transparent color
		KrRGBA transp;
		transp.Set(0, 255, 0);

		if(bHasTransparentColor)
		{
			transp = originalTransparentColor;
		}
		else
		{			
			//Slow!
			/*for( j=0; j < size.y; ++j )
			{
				KrRleLine *line = &(line[j]);
				
				for ( i=0; i<line->NumSegments(); i++ )
				{
					KrRleSegment* segment = line->Segment( i );
					int n = segment->Len();				
					
					for(int i0 = 0; i0 < n; i0++)
					{
					}
				}
			}*/
		}

		//Fill
		SDL_Rect sdlrect = {0, 0, surface->w, surface->h};
		U32 sdlColor = SDL_MapRGB( surface->format,	
			transp.c.red,
			transp.c.green,
			transp.c.blue);

		SDL_FillRect( surface, &sdlrect, sdlColor );
	}

	//Draw
	KrPaintInfo info(surface, true);

	KrMatrix2 xForm, paintForm;		
	KrColorTransform cform;
	KrRect clip;

	paintForm.Set(originalHotSpot.x, originalHotSpot.y);
	clip.Set( 0, 0, surface->w - 1, surface->h - 1 );	

	Draw( &info, paintForm, cform, clip, 0 );

	//Save
	bool res = false;
	if(bHasTransparency) 
	{
		fileName += ".png";
		res = SDL_SavePNG(surface, fileName.c_str()) == 0;
	}
	else
	{
		fileName += ".bmp";
		res = SDL_SaveBMP(surface, fileName.c_str()) == 0;
	}

	SDL_ClearError();

	SDL_FreeSurface( surface );

	return res;
}

void KrRle::CountComponents( U32* numLines, U32* numSegments, U32* numRGBA )
{
	int i, j;
	CMemControl memControl(bigData, bigDataUsingGC);

	*numLines    = size.y;
	*numSegments = 0;
	*numRGBA     = 0;

	for( i=0; i<size.y; ++i )
	{
		*numSegments += line[i].NumSegments();

		for( j=0; j<line[i].NumSegments(); ++j )
		{
			*numRGBA += line[i].Segment( j )->Len();
		}
	}
	GLASSERT( *numSegments <= *numRGBA );
	//GLASSERT( *numLines <= *numSegments );	// Freakin fonts.
}



/*static*/ void KrRle::SetMemoryPools(	GlLinearMemoryPool*	_memoryPoolRGBA,
										GlLinearMemoryPool*	_memoryPoolLine,
										GlLinearMemoryPool*	_memoryPoolSegment )
{
	memoryPoolRGBA = _memoryPoolRGBA;
	memoryPoolLine = _memoryPoolLine;
	memoryPoolSegment = _memoryPoolSegment;	
}


/*static*/ bool KrRle::PoolOut()
{
	GLASSERT( memoryPoolRGBA->OutOfMemory() );
	GLASSERT( memoryPoolLine->OutOfMemory() );
	GLASSERT( memoryPoolSegment->OutOfMemory() );

	return memoryPoolRGBA->OutOfMemory() && memoryPoolLine->OutOfMemory() && memoryPoolSegment->OutOfMemory();
}

U16 *KrRleSegment::Convert24To16()
{
	//Convert internal 24 bit buffer to 16 bit 5-6-5
	if(getBytesPerPixel() != 3) return NULL;

	U16* target   = (U16*) new U16[Len()];
	U16* buf16   = target;
	U8 *source = rgba;
	int nPixel = Len();

	while ( nPixel )
	{	
		//source rgb =	0x00ff0000,	
		//				0x0000ff00,
		//				0x000000ff

		*target = ( ( source[2] >> 3 )     << 11 ) 
				| ( ( source[1] >> 2 ) << 5 ) 
				| ( source[0] >> 3 );
		
		nPixel--;
		target++;
		source += 3;
	}

	return buf16; //Must be deleted
}

void KrRleLine::WriteSegments(SDL_RWops *fp) //maks
{
	//Write all segments
	for (int i=0; i<NumSegments(); i++)
	{
		segment[i].Write(fp);
	}
}

void KrRleLine::WriteSegmentBuffers(SDL_RWops *fp) //maks
{
	//Write all segment buffers
	for (int i=0; i<NumSegments(); i++)
	{
		segment[i].WriteBuffer(fp);
	}
}

void KrRleSegment::Write(SDL_RWops *fp) //maks
{
	U8 *tmp_rgba = rgba;
	/*KrRleSegment *tmp_next = next;
	U8 tmp_bEnd = bEnd;*/

	rgba = NULL; //More compression
	/*next = NULL;
	bEnd = 0;*/

	if(getBytesPerPixel() == 3 && KrRle::getConvertImage()) 
	{
		setBytesPerPixel(2);

		SDL_RWwrite(fp, this, sizeof(KrRleSegment), 1);

		setBytesPerPixel(3);
	}
	else
	{
		SDL_RWwrite(fp, this, sizeof(KrRleSegment), 1);
	}

	rgba = tmp_rgba;
	/*next = tmp_next;
	bEnd = tmp_bEnd;*/
}

void KrRleSegment::WriteBuffer(SDL_RWops *fp)
{
	if(getBytesPerPixel() == 3 && KrRle::getConvertImage()) 
	{
		U16 *buf16 = Convert24To16();
		SDL_RWwrite(fp, buf16, Len()*2, 1);
		delete [] buf16;
	}
	else
		SDL_RWwrite(fp, rgba, Len()*getBytesPerPixel(), 1);
}

void KrRleLine::WriteLine(SDL_RWops *fp)
{
	KrRleSegment *tmp = segment;
	segment = NULL; //More compression

	SDL_RWwrite(fp, this, sizeof(KrRleLine), 1);

	segment = tmp;
}


#endif


