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
#include "demos.h"
#include "kyra.h"
#include "space.h"
#include <math.h>


SpaceGame::SpaceGame( SDL_Surface* screen, bool _tracking )
{
	random.SetSeed( 0 );

	GlFixed camera = -50;
	for( int i=0; i<NUM_SCALES; ++i )
	{
		int z = ( i * MAXDEPTH / ( NUM_SCALES - 1 ) );
		GlFixed scale = GlFixed( z - camera ) / GlFixed( BIGDEPTH - camera );
		scalesToUse[i] = scale;
	}

	tracking = _tracking;
	engine = new KrEngine( screen );
	GLASSERT( engine );
 	
	if ( !engine->Vault()->LoadDatFile( "space.dat" ) )
	{
		#ifdef DEBUG
		GLOUTPUT( "Error loading 'space.dat'!\n" );
		GLASSERT( 0 );
		#endif
		exit(100);
	}

	// Calculate the size of the space field and fill
	// it with space tiles.
	field.xmin = -engine->ScreenBounds().Width() * 3;
	field.xmax =  engine->ScreenBounds().Width();
	field.ymin =  engine->ScreenBounds().ymin;
	field.ymax =  engine->ScreenBounds().ymax;

	xResetAt = -engine->ScreenBounds().Width() * 2;
	xResetTo =  engine->ScreenBounds().Width() / 2;

	// Only useful if the demo runs past the end of the screen.
	// For a "real" app, this should be set to 0 because the
	// background shouldn't show through.
	KrRGBA black;
    black.Set( 0, 0, 0, 255 );
	engine->FillBackground( &black );
	
	// We use 2 container ImNodes: 
	// - one for the starfield
	// - one for everything else.
	starfieldNode = new KrImNode;
	mainNode	  = new KrImNode;

	engine->Tree()->AddNode( 0, starfieldNode );
	engine->Tree()->AddNode( 0, mainNode );

	AddStarfield();
	AddShips();

	TrackCamera();
}


SpaceGame::~SpaceGame()
{
	delete engine;
}


void SpaceGame::TrackCamera()
{
	// Find the tracker object and put it on the right side of the screen.
	KrImNode* node;

	node = engine->Tree()->FindNodeByName( "track" );
	if ( node )
	{
		// We scoll the screen to position the node in the view.
		// We want the ship to end up at 3/4 width and 1/2 height.
		// (the height doesn't change, so we only have to play 
		// with the x)

		int desiredX = engine->ScreenBounds().Width() * 3 / 4;
		int offsetX  = node->X() - desiredX;
		engine->Tree()->Root()->SetPos( -offsetX, 0 );
	}
}


void SpaceGame::AddStarfield()
{
	int x, y;
	KrTileResource* starRes = engine->Vault()->GetTileResource( "STARS" );
	GLASSERT( starRes );
// 	GlFixed scalex = 1.3;
// 	GlFixed scaley = 1.3;
// 	starRes->CacheScale( scalex, scaley );

 	for( x = field.xmin; x <= field.xmax; x += starRes->Size() )
	{
	 	for( y = field.ymin; y <= field.ymax; y += starRes->Size()  )
		{
			KrTile* tile = new KrTile( starRes );
			GLASSERT( tile );

			tile->SetRotation( random.Rand( 8 ) );
			tile->SetPos( x, y );
			engine->Tree()->AddNode( starfieldNode, tile );
// 			tile->SetScale( scalex, scaley );
		}
	}
}


GlFixed SpaceGame::GetScale( int z )
{
	if ( z == BIGDEPTH ) return 1;

	GlFixed camera = -50;
	GlFixed scale = GlFixed( z - camera ) / GlFixed( BIGDEPTH - camera );

	int index = 0;
	int error = abs( scale.v - scalesToUse[0].v );

	for( int i=1; i<NUM_SCALES; ++i )
	{
		int thisError = abs( scale.v - scalesToUse[i].v );
		if ( thisError < error )
		{
			error = thisError;
			index = i;
		}
	}
	return scalesToUse[ index ];
}


void SpaceGame::AddShips()
{
	// Put one big ship in, some random med size, and a bunch of
	// small ones.

	KrSpriteResource* bigRes;
	KrSpriteResource* medRes;
	KrSpriteResource* smallRes;
	KrSprite* sprite;
	KrSprite* insignia;
	int i, x, y;
	 
	GlFixed scale;
	
	bigRes = engine->Vault()->GetSpriteResource( "LARGE" );
	GLASSERT( bigRes );	
	medRes = engine->Vault()->GetSpriteResource( "MED" );
	GLASSERT( medRes );	
	smallRes = engine->Vault()->GetSpriteResource( "SMALL" );
	GLASSERT( smallRes );

	// Cache scale the ships.
	for( i=0; i<NUM_SCALES; ++i )
	{
		medRes->CacheScale( scalesToUse[i], scalesToUse[i] );
		smallRes->CacheScale( scalesToUse[i], scalesToUse[i] );
	}

	// Big ship. Noting that the big ship is 2 sprites: the
	// body and the tail.
	sprite = new KrSprite( bigRes );
	sprite->SetPos( field.xmin + engine->ScreenBounds().Width() * 3 / 2,
				    engine->ScreenBounds().Height() / 2 );
	sprite->SetZDepth( BIGDEPTH );
	sprite->SetNodeName( "track" );
	sprite->SetAction( "BODY" );

	engine->Tree()->AddNode( mainNode, sprite );

	KrSprite* tail = new KrSprite( bigRes );
	tail->SetAction( "TAIL" );
	engine->Tree()->AddNode( sprite, tail );

	// Med ships. These are a little special because they
	// each have a triangle insignia on them that is a child
	// node which is color transformed.
	for( i=0; i<NUM_MED; i++ )
	{
		// The ship:
		x = random.Rand( field.Width() );
		y = random.Rand( field.Height() );
		
		sprite = new KrSprite( medRes );
		sprite->SetPos( field.xmin + x, field.ymin + y );
		sprite->SetZDepth( random.Rand( MAXDEPTH ) );
		scale = GetScale( sprite->ZDepth() );
		sprite->SetScale( scale, scale );
		sprite->SetAction( "BODY" );

		engine->Tree()->AddNode( mainNode, sprite );

		// The insignia. It will move with the ship
		// since it is a child, so we don't set coordinates.
		insignia = new KrSprite( medRes );
		insignia->SetAction( "INSIGNIA" );

		KrColorTransform color;
		switch( random.Rand( 3 ) )
		{
			case 0:	
				color.TintBlue( 200 );
				break;
			case 1:	
				color.TintRed( 200 );
				break;
			case 2:	
				color.TintGreen( 200 );
				break;
		}
		insignia->SetColor( color );
		engine->Tree()->AddNode( sprite, insignia );
	}				

	// Small ships.
	for( i=0; i<NUM_SMALL; i++ )
	{
		x = random.Rand( field.Width() );
		y = random.Rand( field.Height() );
		
		sprite = new KrSprite( smallRes );
		sprite->SetPos( field.xmin + x, field.ymin + y );
		sprite->SetZDepth( random.Rand( MAXDEPTH ) );
		scale = GetScale ( sprite->ZDepth() );
		sprite->SetScale( scale, scale );
		engine->Tree()->AddNode( mainNode, sprite );
	}				
}


void SpaceGame::DrawFrame()
{
	AddText( engine );

	// Move ships
	GlInsideNodeIt<KrImNode*> it = mainNode->ChildTreeIterator();

	for ( it.Begin(); !it.Done(); it.Next() )
	{
		KrSprite* sprite = it.CurrentData()->ToSprite();
		if ( sprite )
		{
			int bonus = ( sprite->ZDepth() > BIGDEPTH ) ? 1 : 0;
// 			bonus = 0;

			if ( sprite->SpriteResource()->ResourceName() == "LARGE" )
			{
				sprite->DeltaPos( -1, 0 );
			}
			else if ( sprite->SpriteResource()->ResourceName() == "MED" )
			{
				sprite->DeltaPos( -2 - bonus, 0 );
							}
			else if ( sprite->SpriteResource()->ResourceName() == "SMALL" )
			{
				sprite->DeltaPos( -3 - bonus , 0 );
			}

			// Reset the ships, so the demo can keep running.
			// Regrettably, different logic for tracking and not-tracking.
			if ( tracking )
			{
				if ( sprite->SpriteResource()->ResourceName() == "LARGE" )
				{
					if ( sprite->X() < xResetAt )
						sprite->SetPos( xResetTo, sprite->Y() );
				}
				else
				{
					if ( sprite->X() < xResetAt - engine->ScreenBounds().Width() )
						sprite->SetPos( xResetTo + engine->ScreenBounds().Width(), sprite->Y() );
				}
			}
			else
			{
				if ( sprite->X() < field.xmin )
					sprite->SetPos( field.xmax, sprite->Y() );
			}
		}
	}

	if ( tracking )
		TrackCamera();
		
	engine->Draw();
}

