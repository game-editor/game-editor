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
#include "tile.h"
#include "tileresource.h"
#include "engine.h"


KrTile::KrTile( KrTileResource* _resource )
{
	resource = _resource;
	rotation = 0;
}


KrTile::~KrTile()
{}


void KrTile::Draw(	KrPaintInfo* paintInfo, const KrRect& clip )
{
	GLASSERT( IsVisible() );
	if ( bounds.Intersect( clip ) )
	{
		resource->Draw(	paintInfo,
						CompositeXForm(),
						rotation,
						CompositeCForm(),
						clip,

						CompositeQuality(),

						TreeDepth() );
	}
}


void KrTile::SetRotation( int _r )
{
	_r = _r & 7;
	if ( _r != rotation )
	{
		rotation = _r;
		Invalidate();
	}
}


void KrTile::CalcTransform()
{
	// Calculate our new coordinates, and then bounding info.
	KrImNode::CalcTransform();

	resource->CalculateBounds( CompositeXForm(), &bounds );


}


bool KrTile::HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results )
{
	if (    IsVisible()
		 && CompositeCForm().Alpha() != 0
		 && bounds.Intersect( x, y ) )
	{
		KrVector2T< GlFixed > object;
		ScreenToObject( x, y, &object );

		// Transform to local, and query the resource:
		if(	resource->HitTestTransformed(	rotation,
											object.y.ToIntRound(), object.y.ToIntRound(),
											flags ) )
		{
			results->PushBack( this );
			return true;
		}
	}
	return false;
}


void KrTile::QueryBoundingBox( KrRect* boundingBox )
{
	// Tiles don't have hotspots, so this is easier than the sprite.
	// Make sure to look for scaling.
	resource->CalculateBounds( CompositeXForm(), boundingBox );
}


KrImNode* KrTile::Clone()
{
	KrTile* tile = new KrTile( resource );
	tile->SetRotation( rotation );
	return tile;
}


#endif //#ifndef STAND_ALONE_GAME //maks
