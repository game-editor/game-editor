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

#ifndef KYRA_TILERESOURCE_INCLUDED
#define KYRA_TILERESOURCE_INCLUDED

#include "kyraresource.h"
#include "../engine/krmath.h"
#include "pixelblock.h"

/** Practically, a tile resource is used to create a tile.
	It contains a pixel block for its data.
*/
class KrTileResource : public KrResource
{
  public:
	// Create by reading from a .dat file
	KrTileResource( U32 size, SDL_RWops* data );

	KrTileResource( const gedString& name,
					KrPaintInfo* surface,
					int x, int y, int size );

	virtual ~KrTileResource();

	virtual	U32 Type()						{ return KYRATAG_TILE; }
	virtual const gedString&	TypeName()	{ return tileName; }
	virtual KrTileResource* ToTileResource(){ return this; }

	/// The width and height must be the same for a tile.
	int     Size() const					{ return pixelBlock[0]->Width(); }
	/// Return true if this Tile supports alpha.
	bool	Alpha()							{ return pixelBlock[0]->Alpha(); }

	// -- internal -- //
	void Draw( KrPaintInfo* paintInfo,
			   const KrMatrix2& matrix,
			   int rotation, 
			   const KrColorTransform& cForm,
			   const KrRect& clipping,
			   int quality,
			   int openGLZ );

	/*  Do a HitTest (see KrImageTree::HitTest) in transformed 
		coordinates. So the tree object that made this call
		has already transformed the x and y into local pixel coords.
	*/
	bool HitTestTransformed( int rotation, int x, int y, int hitFlags );

	void CalculateBounds( const KrMatrix2&, KrRect* bounds ) const;
	
	virtual void CacheScale( GlFixed xScale, GlFixed yScale );
	virtual bool IsScaleCached( GlFixed xScale, GlFixed yScale );
	virtual void FreeScaleCache();

#ifndef STAND_ALONE_GAME //maks
	virtual void Save( KrEncoder* );
#endif

  private:
	struct CachedBlock
	{
		GlFixed xScale,
				yScale;
		KrPixelBlock* pixelBlock;

		bool operator==( const CachedBlock& rhs )	{ GLASSERT( 0 ); return false; }	// be nice to buggy compilers.
	};
	GlDynArray< CachedBlock > cache;
	
	void CalcSourceAndPitch( KrPixelBlock* pblock, int rotation, KrRGBA** source, int* sPitchX, int* sPitchY );

	static const gedString tileName;
	KrPixelBlock*	pixelBlock[4];	
};


#endif

#endif //#ifndef STAND_ALONE_GAME //maks
