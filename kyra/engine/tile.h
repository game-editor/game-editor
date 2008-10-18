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

#ifndef KYRA_TILE_INCLUDED
#define KYRA_TILE_INCLUDED

#include "image.h"
#include "tileresource.h"


struct KrPaintInfo;
class  KrTileResource;
union KrRGBA;


/**	Tiles are optimized for background tiling. The have
	to be square, and can be rotated and flipped into all 8
	configurations. 
	
	They support alpha but are not optimized for it.

	The rotational properties of Tiles are very useful for 
	some applications that use the Tiles for backgrounds. Tiles
	are good for creating backgrounds and can be very efficient.
	However, if you find yourself thinking that you could use 
	a Sprite or a Tile, use the Sprite. They are much more
	flexible.

	Tiles don't collide, and only scale if cached.
*/

class KrTile : public KrImage
{
  public:
	KrTile( KrTileResource* resource );
	virtual ~KrTile();

	/** A tile has 8 rotations, as follows:
		0:	no rotation
		1:	90 CW
		2:	180 CW
		3:	270 CW
		4:  no rotation then flipped around a horizontal line
		5:  90 CW then flipped around a horizontal line
		6:  180 CW then flipped around a horizontal line
		7:  270 CW then flipped around a horizontal line
	*/
	void SetRotation( int rotation );
	int  GetRotation()						{ return rotation; }

	/// Tiles are always square. Returns the size of any edge.
	int Size() const	{ return resource->Size(); }


	virtual void Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip/*,
						int window*/ );

	virtual KrImNode* Clone();

	virtual bool HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results/*, int window*/ );

	virtual void CalcTransform( /*int window*/ );
	virtual void QueryBoundingBox( KrRect* boundingBox/*, int window = 0*/ );

	virtual KrTile*  ToTile()				{ return this; }
	virtual KrResource* Resource()			{ return resource; }
	KrTileResource* TileResource()			{ return resource; }
	
  protected:

  private:
	int rotation;
	KrTileResource* resource;
};


#endif


#endif