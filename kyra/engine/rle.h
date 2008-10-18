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

#ifndef KYRA_RLE_INCLUDED
#define KYRA_RLE_INCLUDED

#include <stdlib.h>

#include "../util/gltypes.h"
#include "../util/gldebug.h"
#include "../engine/krmath.h"
#include "../util/glmemorypool.h"
#include "../util/glbitstream.h"
#include "SDL.h"
#include "painter.h"
#include "../../gameEngine/system.h" //maks

#include "FastSprite.h" //maks

//#define USE_COMPILED_SPRITES

class KrCanvasResource;
class KrTexture;
class KrCollisionMap;


#undef RGBA //maks
#undef RGB



class KrRleSegment
{
	friend class KrRle;
	friend class KrRleLine; //maks
  public:
	  
	  
	  
	  
	  KrRleSegment()	: info( 0 ), rgba( 0 )/*, next(0), bEnd(0)*/	{} //maks
	~KrRleSegment();

	void Clear()	{ info = 0; rgba = 0;/* next = 0; bEnd = 0;*/}
	
	U32				Alpha()		{ return getFlagBit( ALPHA ); }
	U16				Start()		{ return getStart(); }
	U16				End()		{ return getEnd(); }
	U16				Len()		{ return getEnd() - getStart() + 1; }

	inline U8*			RGBA() {return rgba;}; //maks
	//GlFlag<U8>		Flags()		{ return getFlags();	}

	// For creating the RLE
	bool Create( KrPaintInfo* surface, 
				 int x, int xMax, int y, int objectOriginX );

#ifndef STAND_ALONE_GAME
	U16 *Convert24To16();
	void Write(SDL_RWops *fp);
	void WriteBuffer(SDL_RWops *fp);

	// For creating the RLE in the reader.
	bool Read( GlReadBitStream* reader, KrRGBA minColor, KrRGBA bits );

	

	//void Compress(); //maks

	void CalcRange( KrRGBA* minColor, KrRGBA* maxColor );
	bool Write( GlWriteBitStream* writer, KrRGBA minColor, KrRGBA bits );
#endif

	
	

	#ifdef DEBUG
		static U32 numRGBA;
	#endif

		U16 getEnd() {return info & 0x1FFF;}
		void setEnd(U32 end) {info = (end & 0x1FFF) | (info & 0xFFFFE000);}

		U16 getStart() {return (info >> 13) & 0x1FFF;}
		void setStart(U32 start) {info = (start & 0x1FFF) << 13 | (info & 0xFC001FFF);}

		U8 getFlags() {return (info >> 26) & 0x7;}
		void setFlags(U32 flags) {info = (flags & 0x7) << 26 | (info & 0xE3FFFFFF);}

		U8 getBytesPerPixel() {return (info >> 29) & 0x7;}
		void setBytesPerPixel(U32 bytesPerPixel) {info = (bytesPerPixel & 0x7) << 29 | (info & 0x1FFFFFFF);}

		bool getFlagBit(U8 flag) {return ( getFlags() & flag ) != 0;}
		void setFlagBit(U8 flag) {setFlags(getFlags() | flag);}
		
public: //maks testStructSize protected:
  
//	static GlLinearMemoryPool*	memoryPool;

	enum {
		ALPHA			=	1,
		//COMPRESS8BIT =	0x02,	// Set if we can write the length and skip in 8 bits.

		BITS_USED		=   3,			// For compression.
		MEMORYPOOL		=	2		// Is the rgba out of a memory pool?		
	};

	U8*		rgba;//, *segComp; //maks
	/*KrRleSegment *next; //maks
	U8 bEnd;*/

	//int lenComp; //maks
	
	//Info format:
	// 3 bits: bytesPerPixel
	// 3 bits: flags
	//13 bits: start
	//13 bits: end
	Uint32 info;
} STRUCT_PACK;


class KrRleLine
{
	friend class KrRle;
  public:
	  
	KrRleLine()	:		flags_nSegments( 0 ), segment( 0 ) {} //maks
	~KrRleLine()		{ if (!/*flags.IsSet( MEMORYPOOL )*/(getFlags() & MEMORYPOOL)) if(segment) delete [] segment; }

	void			Clear()			{	flags_nSegments = 0; segment = 0; } //maks
	U32				Alpha()			{	return /*flags.IsSet( ALPHA )*/ getFlags() & ALPHA; } 
	int				NumSegments()	{	return /*nSegments*/ flags_nSegments & 0x0000FFFF;} //maks
	void setNumSegments(U32 n) {flags_nSegments = (n      & 0x0000FFFF)        | (flags_nSegments & 0xFFFF0000);}
	void setFlags(U32 flags)   {flags_nSegments = (flags  & 0x0000FFFF) << 16  | (flags_nSegments & 0x0000FFFF);}
	int getFlags() {	return (flags_nSegments >> 16) & 0x0000FFFF;} //maks
	
	KrRleSegment*	Segment( int i )
	{	
		GLASSERT( i < NumSegments() );
		GLASSERT( segment );
		return &segment[i]; 
	}

	int				CalcSize()		{	if ( NumSegments() )
											return ( segment[ NumSegments()-1 ].End() - segment[ 0 ].Start() + 1 );
										else
											return 0; }

	/*
	Pushed to the RLE
	void Draw( U8* target,				// pointer to beginning of scanline
			   KrPaintInfo* paintInfo,
			   const KrRect& bounds,	// bounds of the RLE
			   const KrRect& isect,		// intersection of the RLE and DirtyRectangle
			   const KrColorTransform& cForm );	// color transform to use
	*/

	void DrawScaled(U8* target,				// pointer to beginning of line (will be offset from here.)
					KrPaintInfo* paintInfo,
					const KrRect& bounds,	// bounds of the RLE
					const KrRect& isect,		// intersection of the RLE and DirtyRectangle
					const KrColorTransform& cForm,	// color transform to use
					U32 xInc );

	// For creating the RLE
	/*KrRleSegment **/ void Create( KrPaintInfo* surface, int x, int y, int w/*, KrRleSegment *lastSegment*/ ); //maks


#ifndef STAND_ALONE_GAME
	void WriteLine(SDL_RWops *fp);
	  void WriteSegments(SDL_RWops* fp);
	  void WriteSegmentBuffers(SDL_RWops* fp);

	// For creating the RLE in the reader.
	/*KrRleSegment **/ void Read( GlReadBitStream* writer, KrRGBA minColor, KrRGBA bits/*, KrRleSegment *lastSegment*/ ); //maks

	
	bool Write( GlWriteBitStream* writer, KrRGBA minColor, KrRGBA bits );

	void CalcRange( KrRGBA* minColor, KrRGBA* maxColor );
#endif //#ifndef STAND_ALONE_GAME

	
	

public: //maks testStructSize protected:

	enum {
		ALPHA =			0x01,
		MEMORYPOOL =	0x02,		// Is the semgment allocated out of a pool?

		BITS_USED = 2
	};

/*#ifdef GP2X
	GlFlag<U16>		flags;	//make total struct size = 12 in GP2X		
#else
	GlFlag<U8>		flags;	//make total struct size = 16 in GP2X			
#endif

	U16				nSegments;	*/	
	
	U32 flags_nSegments; //maks: Combine flags and nSegments to make a 8 byte size struct event in GP2X
	
	KrRleSegment*	segment;
} STRUCT_PACK;



/*	A basic drawing thing. A Run length encoded image.

	WARNING: The KrAction::GrowFrameArray copies these things
	like structures. It is careful to 0 out old memory, but if
	anything ever has a pointer *back* to the Rle, the pointers
	will be trashed. Could be fixed with a proper copy constructor,
	although that would be difficult.
*/
class KrRle
{
  public:
	  static bool getConvertImage() {return bConvertImage;} //maks
	  static void setConvertImage(bool convert) {bConvertImage = convert;} //maks
	  int getArea() {return bitMapArea;} //maks
	  void RemoveTexture(); //maks

	  void Clear(); //maks

	// The flags are never used directly by the engine code using
	// this class. However, the tile code is borrowing id's
	// for consistency.
	enum {
		ALPHA			= 1,
		MEMORYPOOL		= 2,	// is the line out of a memory pool?
		INDEXED_FRAME	= 4		//If set, load rle data on demand
	};

	KrRle()	: line( 0 ), 
		texture( 0 ), 

		#ifndef STAND_ALONE_GAME
		surface(0), //maks
		#endif
		
		#ifdef USE_COMPILED_SPRITES
		spriteCode(0),
		#endif
		collisionMap( 0 ), bitMapArea (0), bigData(0), bigDataUsingGC(false), frameIndex(0)/*, refCount(0)*/ //maks
								
	{ 
		deltaHotToOrigin.Set(); 
		size.Set(); 
		delta.Set(); 

#ifndef STAND_ALONE_GAME //maks
		originalFrameSize.Set();
		originalHotSpot.Set();
		bHasTransparentColor = false;
#endif
	}
	~KrRle();
	
	void Draw( KrPaintInfo* paintInfo,
			   const KrMatrix2& matrix,
			   const KrColorTransform& cForm,	// color transform to use
			   const KrRect& clipping,
			   int openGLZ,
			   bool bUseFastSprite = true,
			   bool bCratingSurface = false //maks
			   ); //maks
		
	// Reads the RLE from a stream. Returns true if not empty.
	bool Read( SDL_RWops* stream );
	bool LoadIndexedFrame(); //maks
	bool ReadFrameData( SDL_RWops* data ); //maks
	U32 GetTotalSize();

	KrRle*            CreateScaledRle( GlFixed xScale, GlFixed yScale, int* hotx, int* hoty );
	KrCanvasResource* CreateCanvasResource( int* hotx, int* hoty, bool bUseBorder = false ); //maks

	// The encoder uses this to create the sprite. Returns true if not empty.
	bool Create( KrPaintInfo* surface, 
				 int x, int y, int width, int height,
				 int hotx, int hoty,
				 int deltax, int deltay );

#ifndef STAND_ALONE_GAME
	

	// Writes the RLE sprite to the stream.
	bool Write( SDL_RWops* stream, bool bOnDemandFrameLoad );

	// Save the rle to an image file
	bool SaveImage(gedString& fileName); //maks



	// Count all the parts of this object. Used by the encoder.
	void CountComponents( U32* numLines, U32* numSegments, U32* numRGBA );

	static bool PoolOut();

	// Scary function: basically sets the global memory allocator.
	// The vault will set this, allocate a bunch of sprites,
	// then clear it. Will cause problems in a multi-threaded version...
	static void SetMemoryPools(		GlLinearMemoryPool*	_memoryPoolRgba,
									GlLinearMemoryPool*	_memoryPoolLine,
									GlLinearMemoryPool*	_memoryPoolSegment );

	KrVector2& OriginalFrameSize() { return originalFrameSize; } //maks
	KrVector2& OriginalHotSpot() { return originalHotSpot; }
	bool getHasTransparentColor() {return bHasTransparentColor;}
	KrRGBA getOriginalTransparentColor() {return originalTransparentColor;}

#endif

	
	// The x and y "step" for this sprite.
	const KrVector2& StepSize()	const	{ return delta; }

	int Width()	const	{ return size.x; }	///< Width, in pixels.
	int Height() const 	{ return size.y; }	///< Height, in pixels.

	bool Alpha() const	{ return flags.IsSet( ALPHA ); }
	const KrVector2& Delta() const			{ return delta; }
	KrVector2		 Hotspot() const		{	KrVector2 hot = deltaHotToOrigin;
												hot.x = -hot.x; hot.y = -hot.y;
												return hot;
											}
		
	/*	Given a hotspot x and y, return the bounding box
		for the rle.
	*/
	void CalculateBounds( const KrMatrix2& xForm,
						  KrRect* bounds ) const;

	bool HitTestTransformed( int x, int y, int hitFlags );

	

	
	
	KrCollisionMap*	  GetCollisionMap( GlFixed xScale, GlFixed yScale );

	KrRGBA getColorKey() {return colorKey;} //maks

	
	

	// Treat as private:
	// These are NOT owned by the Rle, but the vault. Set here temporarily.
	static GlLinearMemoryPool*	memoryPoolRGBA;
	static GlLinearMemoryPool*	memoryPoolLine;
	static GlLinearMemoryPool*	memoryPoolSegment;

	/*void incRefCount() {refCount++;}
	void decRefCount() {refCount--;}
	int getRefCount() {return refCount;}*/

	//bool operator==( const KrRle& other ) const; //maks

public: //maks testStructSize protected:

	  void DrawScaled(	KrPaintInfo* paintInfo,
						const KrMatrix2& xForm,
						const KrColorTransform& cForm,
						const KrRect& clipping );


	void DrawOpenGL(	KrPaintInfo* paintInfo,
						const KrMatrix2& matrix,
						const KrColorTransform& cForm,
						const KrRect& clipping,
						int openGLZ );





	GlFlag<U8>	flags;				
	KrRleLine*	line;
	KrVector2	deltaHotToOrigin;	
	KrVector2	size;				
	KrVector2   delta;

#ifndef STAND_ALONE_GAME //maks: to frame edition
	KrVector2 originalFrameSize;
	KrVector2 originalHotSpot;
	bool bHasTransparentColor;
	KrRGBA originalTransparentColor;
#endif

	KrCollisionMap* collisionMap;
	int bitMapArea; //maks

	static bool bConvertImage; //maks
	U8 *bigData;
	bool bigDataUsingGC;
	U32 frameIndex;

#ifdef USE_COMPILED_SPRITES
	U8 *spriteCode;
#endif

#ifndef STAND_ALONE_GAME
	SDL_Surface *surface; //maks
#endif

	//int refCount; //maks
	//FastSprite fastSprite;//maks
	KrRGBA colorKey; //maks

//#if defined(KYRA_SUPPORT_OPENGL) //maks: strange bug here, comment if use OPENGL (rle = &frame[i] is wrong)
	KrTexture *texture;	
//#endif
} STRUCT_PACK;



#endif
