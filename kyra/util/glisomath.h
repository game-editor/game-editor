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

#ifndef ISOMATH_INCLUDED
#define ISOMATH_INCLUDED

#include "gldebug.h"

/*
	"Front" View, screen


	     ViewY
  offset   |
	+----------------> ScreenX
	|	   |		|
	|	   |		|
	|	   |		|
	|----------------
    v      |
   SceenY  |
		   |
  ---------+------------> ViewX
          Oview

	The offset (screen offset) is measured in view coordinates
	from the view origin to the offset point.


	"Side" View, word

	WorldZ
	|
	|
	|     ViewY
	|     /
	|    /
	|   / 
	|  /
	| /
	|/
	+---------------------WorldY
	|\_
	|  \_
	|    \_ ViewZ


	"Top" View, Map (Should be right angles)

			  WorldY
			   /|\
			  /	| \
			 /	|  \
		MapY \	|  /Mapx
			  \	| /
			   \|/
	---------------------- WorldX
*/

#include "gltypes.h"
#include "glfixed.h"


/** A utility class for performing math transformations in an
	Isometric view. An Iso view is a quasi-3D view of tiles and
	cubes in 3-D space. In screen view, the cube is exactly 
	twice as wide is it is high.

	The coordinate spaces referenced are:

	Tile: Cubic tile space, or map space. So a "10x10 map"
	      would be equivalent to 10 by 10 "tiles". Tile space
		  is true 3D and has fixed point coordinates.

	World: An intermediate coordinate system.
		   World coordinates are rotated 45 degrees from tile
		   coordinates, but have the same dimensions.
		   
	View: An intermediate coordinate system.
		  View coordinates are rotated to world, but
		  aligned with the screen.
		  
	Screen: A 2-D coordinate system. Pixel coordinates.

	The class can be used via its static functions, which
	provide general transforms. It is usually easier, however,
	to construct an isoMath object and use the "normal" methods.
*/
class GlIsoMath
{
  public:
	/**	Transform tile to screen.
	*/
	static void TileToScreen(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
								int tileWidth, int tileHeight,
								int screenOffsetX, int screenOffsetY,
								GlFixed* screenX, GlFixed* screenY );

	/** Transform from screen coordinates to tile coordinates,
		if you know the tileZ of the transform, as input.
	*/
	static void ScreenToFlatTile(	GlFixed screenX, GlFixed screenY, GlFixed tileZ,
									int tileWidth, int tileHeight,
									int screenOffsetX, int screenOffsetY,
									GlFixed* tileX, GlFixed* tileY );

	/** Tile to world. Generally an internal transformation. Note 
		that this can introduct rounding errors!
	*/
	static void TileToWorld(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
								GlFixed* worldX, GlFixed* worldY, GlFixed* worldZ );

// 	/** Tile to view.
// 	*/
// 	static void TileToView(		GlFixed tileX, GlFixed tileY, GlFixed tileZ,
// 								int tileWidth, int tileHeight,
// 								int* viewX, int* viewY, int* viewZ );
// 

	/** A more useable transformation object. Should be followed by
		SetScreen... in order to view in a useful way.
	*/
	GlIsoMath( int tileWidth, int tileHeight );

	/** Set the screen so that the bottom center of the screen
		is at tile 0,0
	*/
	void SetScreenBaseToOrigin( int screenW, int screenH );

	/**	Set the screen center so that it "looks" at the given tile. */
	void SetScreenCenterToTile( int screenW, int screenH, 
								int tileX, int tileY, int tileZ );

	/** Set an arbitrary screen point to an arbitrary tile. */
	void SetScreenToTile( int screenX, int screenY,
						  int tileX, int tileY, int tileZ );

	/// Coverts from tile coordinates to screen coordinates.
	void TileToScreen(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
						int* screenX, int* screenY ) const;

	/**	Converts from screen to map, for a known mapZ = worldZ.
		The map doesn't have to be flat, just the area of transformation.
	*/
	void ScreenToFlatTile(	int screenX, int screenY, GlFixed tileZ,
							GlFixed* tileX, GlFixed* tileY ) const;

  private:
	int tileWidth, tileHeight;
	int screenOffsetX, screenOffsetY;
};

#endif
