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
#include "canvas.h"
#include "engine.h"



KrCanvas::KrCanvas( KrCanvasResource* _resource, void *parent ) //maks
{
	resource = _resource;
	resource->AddCanvas( this );

	this->parentActor = parent; //maks
}


KrCanvas::~KrCanvas()
{
	resource->RemoveCanvas( this );
}


void KrCanvas::Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip/*,
						int win*/ )
{
	GLASSERT( IsVisible( /*win*/ ) );

	GLASSERT( CompositeQuality( /*win*/ ) != KrQualityNone );


	if(visible/*[win]*/ != ENABLE) return; //maks

	if ( bounds/*[win]*/.Intersect( clip ) )
	{
		resource->Draw(	paintInfo,
						CompositeXForm(/*win*/),
						CompositeCForm(/*win*/),
						clip,

						CompositeQuality(/*win*/),
						TreeDepth() );

#ifndef STAND_ALONE_GAME
		//Draw border
		KrRect rect(bounds/*[win]*/);
		rect.DoIntersection(clip);

		if(parentActor)
			OnSlowCanvasPaint(parentActor, paintInfo, paintInfo->surface, rect, TreeDepth());				
#endif
	}
}


void KrCanvas::CalcTransform( /*int win*/ )
{
	// Calculate our new coordinates, and then bounding info.
	KrImNode::CalcTransform( /*win*/ );

	// Tiles don't have hotspots, so this is easier than the sprite.
	bounds/*[win]*/.xmin = CompositeXForm(/*win*/).x.ToIntRound();
	bounds/*[win]*/.ymin = CompositeXForm(/*win*/).y.ToIntRound();
	bounds/*[win]*/.xmax = ( CompositeXForm(/*win*/).x + CompositeXForm(/*win*/).xScale * resource->Width()  ).ToIntRound() - 1;
	bounds/*[win]*/.ymax = ( CompositeXForm(/*win*/).y + CompositeXForm(/*win*/).yScale * resource->Height() ).ToIntRound() - 1;

#ifdef USE_RTREE //maks
	UpdateRTree(/*win*/);
#endif
}


bool KrCanvas::HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results/*, int window*/ )
{
	//int i = window;

	if (    IsVisible( /*i*/ )
		 && CompositeCForm(/*i*/).Alpha() != 0
		 && bounds/*[i]*/.Intersect( x, y ) 
		 && bEnableHit) //maks
	{
		KrVector2T< GlFixed > object;
		ScreenToObject( x, y, &object );

		// Transform to local, and query the resource:
		if(	resource->HitTestTransformed(	object.x.ToIntRound(), object.y.ToIntRound(),
											flags ) )
		{
			if(results) //maks
				results->PushBack( this );
//			#ifdef DEBUG
//			GLOUTPUT( "HIT: Canvas %x (size=%dx%d)\n", this, Width(), Height() );
//			#endif
			return true;
		}
	}
	return false;
}


void KrCanvas::QueryBoundingBox( KrRect* boundingBox/*, int window*/ )
{
	resource->CalculateBounds( CompositeXForm( /*window*/ ), boundingBox );
}


KrImNode* KrCanvas::Clone()
{
	return new KrCanvas( resource );
}

void KrCanvas::SetDimensions(int x, int y, int width, int height) //maks
{
	SetPos(x, y); 
	resource->SetDimensions(width, height);
} 

void KrCanvas::SetDimensions(int width, int height) //maks
{
	resource->SetDimensions(width, height);
} 

