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



#include "demos.h"
#include "kyra.h"
#include <math.h>


SinWaveTest::SinWaveTest( SDL_Surface* screen )
{
	// The usual beginning: create the engine, load the dat file.
	engine = new KrEngine( screen );
	GLASSERT( engine );
	engine->FillBackground( 0 );

	rand.SetSeed( 0 );

	if ( !engine->Vault()->LoadDatFile( "space.dat" ) )
	{
		#ifdef DEBUG
			GLOUTPUT( "Error loading 'space.dat'\n" );
		#endif
		exit(100);
	}

	// Get the resource from the dat file: we use the small
	// ship and the train ship.
	KrSpriteResource* smallRes = engine->Vault()->GetSpriteResource( "SMALL" );
	GLASSERT( smallRes );
	KrSpriteResource* trainRes = engine->Vault()->GetSpriteResource( "TRAIN" );
	GLASSERT( trainRes );

	// A background starfield - same as the BEM one.
	AddStarfield();


	// Set up the horizontal ships.
	int i = 0, j = 0;

	for( i=0; i<MAX_HORIZONTAL_SHIP; ++i )
	{
		horizontal[0][i] = new KrSprite( smallRes );
		horizontal[0][i]->SetPos( -PADDING + i * ( engine->ScreenBounds().Width() + PADDING * 2 ) / MAX_HORIZONTAL_SHIP,
								  1 * engine->ScreenBounds().Height() / 4 );
		engine->Tree()->AddNode( 0, horizontal[0][i] );


		horizontal[1][i] = new KrSprite( smallRes );
		horizontal[1][i]->SetPos( -PADDING + i * ( engine->ScreenBounds().Width() + PADDING * 2 ) / MAX_HORIZONTAL_SHIP,
								  3 * engine->ScreenBounds().Height() / 4 );
		engine->Tree()->AddNode( 0, horizontal[1][i] );
	}


	// Add the trains
	memset( vertical, 0, ROWS * (MAX_CARS + 2) * sizeof( KrSprite* ) );
	const int SPACE = 52;
	for ( i=0; i<ROWS; ++i )
	{
		// Engine
		int x = GetX( i );
		int y = rand.Rand( engine->ScreenBounds().Height() );
		vertical[i][0] = new KrSprite( trainRes );
		vertical[i][0]->SetAction( "ENGINE" );
		vertical[i][0]->SetPos( x, y );							 
		engine->Tree()->AddNode( 0, vertical[i][0] );
	
		// Cars
		for ( j=0; j<MAX_CARS; ++j )
		{
			KrSprite* car = new KrSprite( trainRes );
			car->SetAction( "CAR" );
			car->SetPos( x, y + SPACE * ( j + 1 ) );
			engine->Tree()->AddNode( 0, car );
			vertical[i][1+j] = car;
		}

		// Caboose
		KrSprite* caboose = new KrSprite( trainRes );
		caboose->SetAction( "CABOOSE" );
		caboose->SetPos( x, y + SPACE * ( MAX_CARS + 1 ) );
		engine->Tree()->AddNode( 0, caboose );
		vertical[i][1+MAX_CARS] = caboose;
	}
	AddText( engine );
}


SinWaveTest::~SinWaveTest()
{
	delete engine;
}


void SinWaveTest::AddStarfield()
{
	int x, y;
	KrTileResource* starRes = engine->Vault()->GetTileResource( "STARS" );
	GLASSERT( starRes );
	KrRect field = engine->ScreenBounds();

 	for( x = field.xmin; x <= field.xmax; x += starRes->Size() )
	{
	 	for( y = field.ymin; y <= field.ymax; y += starRes->Size() )
		{
			KrTile* tile = new KrTile( starRes );
			GLASSERT( tile );

			tile->SetRotation( rand.Rand( 8 ) );
			tile->SetPos( x, y );
			engine->Tree()->AddNode( 0, tile );
		}
	}
}


double SinWaveTest::CalculateScale( int _y )
{
	const double PI = 3.1415926535897932384626433832795;
	double y       = double( _y );
	double screenH = double( engine->ScreenBounds().Height() );
	double thetaRadians = ( y / screenH ) * PI;
	
	double dScale = ( cos( thetaRadians ) * 0.5 + 1.0 ) * 0.8;
	return dScale;
}


int SinWaveTest::CalculateTintFromScale( double scale )
{
	const double lowPoint  = ( 0.4 * 0.8 );
	const double highPoint = ( 1.5 * 0.8 );

	int tint = int ( 180.0 + ( scale - lowPoint ) / ( highPoint - lowPoint ) * 75.0 );
	GlClamp( tint, 0, 255 );
	return tint;
}



void SinWaveTest::DrawFrame()
{
	// Move the ships (based on current depth), set the new depth,
	// and set the scaling factor.
	KrSprite* sprite;
    int i, j;

	// The horizontal rows of ships.
	for( i=0; i<2; ++i )
	{
		for ( j=0; j<MAX_HORIZONTAL_SHIP; j++ )
		{
			sprite = horizontal[i][j];
			if ( sprite )
			{
				double dScale = ( i == 1 ) ? 1.2 : 0.4;
				int deltaX    = ( i == 0 ) ? 2 : 3;

				sprite->SetPos( sprite->X() - deltaX, sprite->Y() );
				if ( sprite->X() < -PADDING ) sprite->SetPos( engine->ScreenBounds().Width() + PADDING, sprite->Y() );

				GlFixed scale = dScale;
				sprite->SetScale( scale, scale );
				sprite->SetZDepth( scale.v );

				KrColorTransform color;
				int tint = CalculateTintFromScale( dScale );

				color.Set( tint, 0, tint, 0, tint, 0, 255 );
				sprite->SetColor( color );
			}
		}
	}


	// The trains.
	for( i=0; i<ROWS; ++i )
	{
		for ( j=0; j<MAX_CARS + 2; j++ )
		{
			sprite = vertical[i][j];
			if ( sprite )
			{
				int deltaY = 3;

				sprite->SetPos( sprite->X(), sprite->Y() - deltaY );
				if ( j == MAX_CARS + 1 && sprite->Y() < 0 )
				{
					for( int k=0; k<MAX_CARS+2; ++k )
					{
						vertical[i][k]->SetPos( vertical[i][k]->X(),
												vertical[i][k]->Y() + engine->ScreenBounds().Height() * 2 );
					}
				}

				// Recalc the scale based on the new position.
				double dScale = CalculateScale( sprite->Y() );
	
				GlFixed scale = dScale;
				sprite->SetScale( scale, scale );
				sprite->SetZDepth( scale.v );

				KrColorTransform color;
				int tint = CalculateTintFromScale( dScale );

				color.Set( tint, 0, tint, 0, tint, 0, 255 );
				sprite->SetColor( color );
			}
		}
	}

	engine->Draw();
}


int SinWaveTest::GetY( int hIndex )	
{	
	return ( hIndex + 1 ) * engine->ScreenBounds().Height() / ( ROWS + 1 );
}


int SinWaveTest::GetX( int vIndex )	
{	
	return ( vIndex + 1 ) * engine->ScreenBounds().Width() / ( ROWS + 1 );
}

