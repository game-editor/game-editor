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

#include "glisomath.h"
#include <math.h>

/*static*/
void GlIsoMath::TileToScreen(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
								int tileWidth, int tileHeight,
								int screenOffsetX, int screenOffsetY,
								GlFixed* screenX, GlFixed* screenY )
{
	// Tile to view:
	GlFixed vX =   tileX * ( tileWidth / 2 )
				 - tileY * ( tileWidth / 2 );

	GlFixed vY =   tileX * ( tileHeight / 2 )
				 + tileY * ( tileHeight / 2 )
				 + tileZ * tileHeight;

	// View to screen:
// 	GLOUTPUT( "view=%d, %d\n", vX.ToInt(), vY.ToInt() );
	*screenX =  vX + screenOffsetX;
	*screenY = -vY + screenOffsetY;
}


/*static*/
void GlIsoMath::ScreenToFlatTile(	GlFixed screenX, GlFixed screenY, GlFixed tileZ,
									int tileWidth, int tileHeight,
									int screenOffsetX, int screenOffsetY,
									GlFixed* tileX, GlFixed* tileY )
{
	// screen to view:
	GlFixed vX =  screenX - screenOffsetX;
	GlFixed vY = -screenY + screenOffsetY;

	// view to tile:
	*tileX =  ( vX / tileWidth ) + ( vY / tileHeight ) - tileZ;
	*tileY = -( vX / tileWidth ) + ( vY / tileHeight ) - tileZ;

// 	#ifdef DEBUG
// 	GLOUTPUT( "screen=%d,%d offset=%d,%d view=%d,%d tile=%d,%d\n",
// 			  screenX.ToInt(), screenY.ToInt(),
// 			  screenOffsetX, screenOffsetY,
// 			  vX.ToInt(), vY.ToInt(),
// 			  tileX->ToInt(), tileY->ToInt() );
// 	#endif
}


/*static*/
void GlIsoMath::TileToWorld(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
								GlFixed* worldX, GlFixed* worldY, GlFixed* worldZ )
{
	GlFixed csAlpha( 0.7071067811865475244008443621045 );

	*worldX = csAlpha * tileX - csAlpha * tileY;
	*worldY = csAlpha * tileX + csAlpha * tileY;
	*worldZ = tileZ;
}


// /*static*/
// void GlIsoMath::TileToView(		GlFixed tileX, GlFixed tileY, GlFixed tileZ,
// 								int tileWidth, int tileHeight,
// 								int* viewX, int* viewY, int* viewZ )
// {
// 	*viewX = ( tileX * tileWidth / 2 - tileY * tileHeight / 2 ).ToInt();
// 	*viewY = ( tileX * tileWidth / 2 - tileY * tileHeight / 2 ).ToInt();
// 	*viewZ = ( 
// }


GlIsoMath::GlIsoMath( int _tileWidth, int _tileHeight )
{
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;
	screenOffsetX = 0;
	screenOffsetY = 0;
	
}

void GlIsoMath::SetScreenBaseToOrigin( int screenW, int screenH )
{
	screenOffsetX = screenW / 2;
	screenOffsetY = screenH;
}


void GlIsoMath::SetScreenCenterToTile( int screenW, int screenH, int tileX, int tileY, int tileZ )
{
	GlFixed sx;
	GlFixed sy;

	TileToScreen( tileX, tileY, tileZ, 
				  tileWidth, tileHeight, 
				  0, 0, 
				  &sx, &sy );

	screenOffsetX = screenW / 2 - sx.ToInt();
	screenOffsetY = screenH / 2 - sy.ToInt(); 
}


void GlIsoMath::SetScreenToTile(	int screenX, int screenY,
									int tileX, int tileY, int tileZ )
{
	GlFixed sx;
	GlFixed sy;

	TileToScreen( tileX, tileY, tileZ, 
				  tileWidth, tileHeight, 
				  0, 0, 
				  &sx, &sy );

	screenOffsetX = screenX - sx.ToInt();
	screenOffsetY = screenY - sy.ToInt(); 
}
	

void GlIsoMath::TileToScreen(	GlFixed tileX, GlFixed tileY, GlFixed tileZ,
								int* screenX, int* screenY ) const
{
	GlFixed sx, sy;

	TileToScreen(	tileX, tileY, tileZ,
					tileWidth, tileHeight,
					screenOffsetX, screenOffsetY, 
					&sx, &sy );
	*screenX = sx.ToInt();
	*screenY = sy.ToInt();
}


void GlIsoMath::ScreenToFlatTile(	int screenX, int screenY, GlFixed tileZ,
									GlFixed* mapX, GlFixed* mapY ) const
{
	ScreenToFlatTile(	screenX, screenY, tileZ,
						tileWidth, tileHeight,
						screenOffsetX, screenOffsetY,
						mapX, mapY );
}
