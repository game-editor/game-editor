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



#ifndef KYRA_PIXELBLOCK_INCLUDED
#define KYRA_PIXELBLOCK_INCLUDED


#include "../util/gltypes.h"
#include "../util/gldebug.h"
#include "../engine/krmath.h"
#include "SDL.h"
#include "painter.h"
#include "ogltexture.h"


class GlLinearMemoryPool;


/*	A block of memory that is a framebuffer. Used by the canvas
	resource and tiles. 
*/

class KrPixelBlock
{
  public:

	void RestoreBuffer(int iBuffer); //maks
	void SaveBuffer(int iBuffer); //maks
	void RemoveTexture(); //maks

	  
	KrPixelBlock();
	~KrPixelBlock();

	/*  The draw function is modified to accomidate both
		the tile and the canvas.

		@param paintInfo	The target surface information.
		@param x			The target x, ignoring clipping.
		@param y			The target y, ignoring clipping.
		@param invert		If true, the PixelBlock will draw upside down.
							Used for the tile.
		@param clipping		The clipping rectangle. Can be null.
	*/
	void Draw( KrPaintInfo* paintInfo,
			   const KrMatrix2& matrix,
			   bool invert,
			   const KrColorTransform& cForm,
			   const KrRect& clipping,

			   int quality,

			   int openGLZ );	

	// Reads the block from a stream.
	bool Read( SDL_RWops* data );

	// The encoder uses this to create the block.
	bool Create( KrPaintInfo* surface, 
				 int x, int y, int width, int height );

	// Creates an uninitialized pixel block.
	bool Create( int width, int height, bool alphaSupport );

	void SetDimensions(int width, int height); //maks

#ifndef STAND_ALONE_GAME //maks
	// Writes the block to the stream.
	bool Write( SDL_RWops* stream );
#endif

	int Width()	const	{ return size.x; }	///< Width, in pixels.
	int Height() const 	{ return size.y; }	///< Height, in pixels.
	bool Alpha() const	{ return (flags & ALPHA) != 0; }

	/*  Can be used to read or write the pixel block. The Tile uses
		Pixels to do specialized drawing. The Canvas uses this to 
		write to the pixel buffer.
	*/
	KrRGBA* Pixels()	{ return block; }

	// Convenience access for the tiles:
	KrRGBA* LowerLeftPixels()	{ return block + (size.y-1) * size.x; }
	KrRGBA* LowerRightPixels()	{ return block + (size.y-1) * size.x + size.x - 1; }
	KrRGBA* UpperRightPixels()	{ return block + size.x - 1; }

	// Count all the parts of this object. Used by the encoder.
	void CountComponents( U32* numRGBA );

	void CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const;

//	static void SetMemoryPool( GlLinearMemoryPool* _memoryPool )	{ memoryPool = _memoryPool; }

	#ifdef DEBUG
		static U32 numRGBA;
	#endif
	
	// Needs to be called by Refresh, or when the pixel block data changes.
	void LoadNewTexture();

	void DrawOpenGL(		KrPaintInfo* paintInfo,
							const KrMatrix2& matrix,
							const KrColorTransform& cForm,
							const KrRect& clipping,
							int rotation,
							int openGLZ );

	// Normally, you would never call these directly, they are
	// called through "Draw". However, sometimes it's nice to 
	// be able to get to a specific one.
	void DrawScaled(		KrPaintInfo* paintInfo,
							const KrMatrix2& xForm,
							const KrColorTransform& cForm,
							const KrRect& clipping,

							int quality,

							bool invert );
	void DrawScaledFast(	KrPaintInfo* paintInfo,
							const KrMatrix2& xForm,
							const KrColorTransform& cForm,
							const KrRect& clipping,
							bool invert );
	void DrawScaledDown(	KrPaintInfo* paintInfo,
							const KrMatrix2& xForm,
							const KrColorTransform& cForm,
							const KrRect& clipping );
	void DrawScaledLinear(	KrPaintInfo* paintInfo,
							const KrMatrix2& xForm,
							const KrColorTransform& cForm,
							const KrRect& clipping );

  protected:
	enum {
		ALPHA		= 0x01,
		MEMORYPOOL	= 0x02,
	};

//	static GlLinearMemoryPool* memoryPool;
	U32			flags;				// U32 LE
	KrVector2	size;				// S32 x 2
	KrRGBA      *block, *undo1, *undo2; //maks: two undo buffers

	KrTexture*	texture;
};


#endif


