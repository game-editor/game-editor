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


#include <math.h>
#include "demos.h"
#include "kyra.h"


const int SHIPX = 180, SHIPY= 100;


ScalingTest::ScalingTest( SDL_Surface* screen )
{
	int i, j;
	//KrMatrix2 xForm;
	state = BOTH;

	//
	// Create the engine, load the dat files, get the resource.
	//
	engine = new KrEngine( screen );
	GLASSERT( engine );

	if ( !engine->Vault()->LoadDatFile( "space.dat" ) )
	{
		#ifdef DEBUG
			GLOUTPUT( "Error loading 'space.dat'\n" );
		#endif
		exit(100);
	}

	fontVault = new KrResourceVault();
	if ( !fontVault->LoadDatFile( "font.dat" ) )
	{
		#ifdef DEBUG
			GLOUTPUT( "Error loading 'font.dat'\n" );
		#endif
		exit(100);
	}

	KrSpriteResource* shipRes = engine->Vault()->GetSpriteResource( "MED" );
	GLASSERT( shipRes );
	KrSpriteResource* smallShipRes = engine->Vault()->GetSpriteResource( "SMALL" );
	GLASSERT( smallShipRes );
	KrFontResource*	fontConsole = fontVault->GetFontResource( "CONSOLE" );
	GLASSERT( fontConsole );

	KrRGBA white, red;
	white.Set( 255, 255, 255 );
	red.Set( 255, 0, 0 );

	AddBackground();

	// A group of travelling ships.
	travellingShip = new KrSprite( shipRes );
	travellingShip->SetAction( "BODY" );
	travellingShip->SetPos( 500, 390 );
	engine->Tree()->AddNode( 0, travellingShip );

	for ( i=0; i<4; ++i )
	{
		KrSprite* escort = new KrSprite( smallShipRes );
		escort->SetPos( 10 + i*30, -55 + i*30 );
		engine->Tree()->AddNode( travellingShip, escort );
	}

	travelText = new KrTextBox( fontConsole, 300, 300, 1 );
	engine->Tree()->AddNode( travellingShip, travelText );
	travelText->SetPos( -60, 30 );
	travelText->SetTextChar( "sin wave motion", 0 );



	//
	// Zoom 3 canvases to compare. The canvas are set up by drawing
	// a sprite to a canvas.
	//
	int hotx, hoty;
	KrCanvasResource* canvasRes = shipRes->CreateCanvasResource( "BODY", 0, &hotx, &hoty );
	GLASSERT( canvasRes );
	GLASSERT( canvasRes->Alpha() );
	engine->Vault()->AddResource( canvasRes );

	for( i=0; i<canvasRes->Width(); ++i )
	{
		for( j=0; j<canvasRes->Height(); ++j )
		{
			KrRGBA* pixel = canvasRes->Pixels() + j*canvasRes->Width() + i;
			if ( pixel->c.alpha )
				pixel->c.alpha = i * 256 / canvasRes->Width();
		}
	}

	for( i=0; i<3; ++i )
	{
		canvas[i] = new KrCanvas( canvasRes );
		canvas[i]->SetPos( 50 + i * 200, 170 );
		engine->Tree()->AddNode( 0, canvas[i] );
	}
	canvas[0]->SetQuality( KrQualityFast );
	canvas[1]->SetQuality( KrQualityLinear );
	canvas[2]->SetQuality( KrQualityAdaptive );


	// A sprite that scales in the upper left.
	// A resource to mark the center of the sprite.
	KrBoxResource* centerRes = new KrBoxResource( "Center",
												  5, 5,
												  &red, 1,
												  KrBoxResource::CROSSHAIR );
	engine->Vault()->AddResource( centerRes );

	// The ship that scales in the upper left
	ship = new KrSprite( shipRes );
	ship->SetAction( "BODY" );
	ship->SetPos( SHIPX, SHIPY );
	engine->Tree()->AddNode( 0, ship );


	// Center marker of the sprite
	center = new KrBox( centerRes );
	engine->Tree()->AddNode( 0, center );
	center->SetPos( SHIPX, SHIPY );

	// Some text info.
	textBox = new KrTextBox( fontConsole, 1024, 1024, 0 );
	textBox->SetPos( 10, 10 );
	engine->Tree()->AddNode( 0, textBox );

	scaleX.v = GlFixed_1 - GlFixed_1 / 8;
	scaleY.v = GlFixed_1 - GlFixed_1 / 8;
	AddText( engine );
}


ScalingTest::~ScalingTest()
{
	delete engine;
	delete fontVault;
}


void ScalingTest::AddBackground()
{
	int i, j, k;

	KrCanvasResource* back[3];
	for( k=0; k<3; ++k )
	{
		back[k] = new KrCanvasResource( "backgroundRes", 100, 100, false );
		engine->Vault()->AddResource( back[k] );
		for ( i=0; i<100; i++ )
		{
			for( j=0; j<100; j++ )
			{
				KrRGBA* pixel = back[k]->Pixels() + i + j*back[k]->Width();
				if ( ( ( i / 10 ) & 0x01 ) == ( ( j / 10 ) & 0x01 ) ) 
					pixel->array[KrRGBA::START + k] = 100;
				else
					pixel->array[KrRGBA::START + k] = 160;
			}
		}
		back[k]->Refresh();
	}	
	
	int w = engine->ScreenBounds().Width();
	int h = engine->ScreenBounds().Height();
	int count = 0;

	for ( i=0; i <= w / 100; i++ )
	{
		for ( j=0; j <= h / 100; j++ )
		{
			KrCanvas* canvas = new KrCanvas( back[count%3] );
			canvas->SetPos( i*canvas->Width(), j*canvas->Height() );
			GLASSERT( canvas );
			engine->Tree()->AddNode( 0, canvas );
			++count;
		}
	}


}


void ScalingTest::DrawFrame()
{
	if ( state == BOTH )		
	{	
		scaleX.v += GlFixed_1 / 64;
		scaleY.v += GlFixed_1 / 64;
		if ( scaleX > GlFixed( 1.4 ) ) 
		{
			scaleX.v = GlFixed_1 / 2;
			scaleY.v = GlFixed_1 / 2;
			state = HORIZONTAL;
		}
	}
	else if ( state == HORIZONTAL )
	{
		scaleX.v += GlFixed_1 / 64;
		if ( scaleX > GlFixed( 1.4 ) ) 
		{
			scaleX.v = GlFixed_1 / 2;
			scaleY.v = GlFixed_1 / 2;
			state = VERTICAL;
		}
	}
	else
	{
		scaleY.v += GlFixed_1 / 64;
		if ( scaleY > GlFixed( 1.4 ) ) 
		{
			scaleX.v = GlFixed_1 / 8;
			scaleY.v = GlFixed_1 / 8;
			state = BOTH;
		}
	}

	ship->SetScale( scaleX, scaleY );

	travellingShip->SetPos( travellingShip->X() - 2, travellingShip->Y() );
	if ( travellingShip->X() < 0 ) 
	{
		travellingShip->SetPos( engine->ScreenBounds().Width(), travellingShip->Y() );
	}

	GlFixed tscale;
	tscale = 1.0 + sin( double( Frame() ) / 25.0 ) / 2.0;
	travellingShip->SetScale( tscale, tscale );

	for( int i=0; i<3; ++i )
	{
		canvas[i]->SetScale( scaleX, scaleY );
	}

	char buffer[ 64 ];
	sprintf( buffer, "scale: %.3f X %.3f", scaleX.ToDouble(), scaleY.ToDouble() );
	textBox->SetTextChar( buffer, 0 );
	textBox->SetTextChar( "Sprite", 1 );
	textBox->SetTextChar( "Alpha Blended Canvas: fast -- bi-linear -- adaptive", 2 );

	KrRect bounds;
	KrMatrix2 xForm;
	xForm.Set( SHIPX, SHIPY, scaleX, scaleY );
 	ship->SpriteResource()->GetAction( "BODY" )->CalculateBounds( 0, xForm, &bounds );

	// Add the background alignment boxes.
	KrRGBA color;;
	color.Set( 255, 0, 0, 128 );

	KrBoxResource* boxRes = new KrBoxResource(	"databox",
												bounds.Width(), bounds.Height(),
												&color, 1,
												KrBoxResource::OUTLINE );
	KrBox* box = new KrBox( boxRes );
	box->SetPos( bounds.xmin, bounds.ymin );
	engine->Tree()->AddNode( 0, box );

	engine->Draw();

	engine->Tree()->DeleteNode( box );

	delete boxRes;
}
