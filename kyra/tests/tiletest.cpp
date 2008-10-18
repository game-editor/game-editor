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


#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "kyra.h"
#include "demos.h"

TileTest::TileTest( SDL_Surface* _screen )
{
	drawn = false;
	screen = _screen;
	engine = new KrEngine( screen );
	//GLOUTPUT( "TileTest::TileTest\n" );
	engine->Validate();	// check validity

	int i, j;

	// Classes to hold other objects:
	engine->Vault()->LoadDatFile( "standardtest.dat" );

	KrTileResource* noAlphaRes = engine->Vault()->GetTileResource( "NOALPHA" );
	KrTileResource* alphaRes = engine->Vault()->GetTileResource( "ALPHA" );
	GLASSERT( noAlphaRes && alphaRes );

	// Transforms:
	KrColorTransform alphaCForm;
	alphaCForm.SetAlpha( 255 * 70 / 100 );

	KrColorTransform redCForm;
	redCForm.SetRed( 255 * 50 / 100, 127 );

	KrColorTransform blueCForm;
	blueCForm.SetBlue( 255 * 50 / 100, 127 );

	KrColorTransform greenAlphaCForm;
	greenAlphaCForm.SetGreen( 255 * 50 / 100, 127 );
	greenAlphaCForm.SetAlpha( 255 * 70 / 100 );

	KrColorTransform blueAlphaCForm;
	blueAlphaCForm.SetBlue( 255 * 50 / 100, 127 );
	blueAlphaCForm.SetAlpha( 255 * 70 / 100 );

	// Containers:
	KrImNode* c0 = new KrImNode;	// background
	KrImNode* c1 = new KrImNode;	// tiles
 	KrImNode* c2 = new KrImNode;	// canvas

	engine->Tree()->AddNode( 0, c0 );
	engine->Tree()->AddNode( c0, c1 );
	engine->Tree()->AddNode( c0, c2 );

	c1->SetPos(   0,   0 );
	c2->SetPos(	420,   0 );
	c1->SetZDepth( 1 );
	c2->SetZDepth( 1 );

	// ---------- Background ----------- //
	KrSpriteResource* backSpriteRes = engine->Vault()->GetSpriteResource( "BACKGROUND" );
	GLASSERT( backSpriteRes );
	KrAction* action = backSpriteRes->GetActionByIndex( 0 );
	GLASSERT( action );
	const KrRle& rle = action->Frame( 0 );

	for ( i=0; i <= (screen->w) / rle.Width(); i++ )
	{
		for ( j=0; j <= (screen->h) / rle.Height(); j++ )
		{
			KrSprite* sprite = new KrSprite( backSpriteRes );
			sprite->SetPos( i*rle.Width(), j*rle.Height() );
			GLASSERT( sprite );
			engine->Tree()->AddNode( c0, sprite );
		}
	}

	// ---------- The "no alpha" tile.
	// no transform:
	for ( i=0; i<8; i++ )
	{
		noAlpha[i][0] = new KrTile( noAlphaRes );
		noAlpha[i][0]->SetPos( i*noAlpha[i][0]->Size(), 0 );
		noAlpha[i][0]->SetRotation( i );
		GLASSERT( noAlpha );
		engine->Tree()->AddNode( c1, noAlpha[i][0] );
	}

	// alpha:
	for ( i=0; i<8; i++ )
	{
		noAlpha[i][1] = new KrTile( noAlphaRes );
		noAlpha[i][1]->SetColor( alphaCForm );
		noAlpha[i][1]->SetPos( i*noAlpha[i][1]->Size(), 1*noAlpha[i][1]->Size() );
		noAlpha[i][1]->SetRotation( i );
		GLASSERT( noAlpha[i][1] );
		engine->Tree()->AddNode( c1, noAlpha[i][1] );
	}

	// red:
	for ( i=0; i<8; i++ )
	{
		noAlpha[i][2] = new KrTile( noAlphaRes );
		noAlpha[i][2]->SetColor( redCForm );
		noAlpha[i][2]->SetPos( i*noAlpha[i][2]->Size(), 2*noAlpha[i][2]->Size() );
		noAlpha[i][2]->SetRotation( i );
		GLASSERT( noAlpha[i][2] );
		engine->Tree()->AddNode( c1, noAlpha[i][2] );
	}

	// combination:
	for ( i=0; i<8; i++ )
	{
		noAlpha[i][3] = new KrTile( noAlphaRes );
		noAlpha[i][3]->SetColor( blueAlphaCForm );
		noAlpha[i][3]->SetPos( i*noAlpha[i][3]->Size(), 3*noAlpha[i][3]->Size() );
		noAlpha[i][3]->SetRotation( i );
		GLASSERT( noAlpha );
		engine->Tree()->AddNode( c1, noAlpha[i][3] );
	}

	// ---------- The "alpha" tile.
	// no transform:
	for ( i=0; i<8; i++ )
	{
// 		i=6;
		alpha[i][0] = new KrTile( alphaRes );
		alpha[i][0]->SetPos( i*alpha[i][0]->Size(), 4*alpha[i][0]->Size() );
		alpha[i][0]->SetRotation( i );
		GLASSERT( alpha[i][0] );
		engine->Tree()->AddNode( c1, alpha[i][0] );
	}

	// alpha:
	for ( i=0; i<8; i++ )
	{
		alpha[i][1] = new KrTile( alphaRes );
		alpha[i][1]->SetColor( alphaCForm );
		alpha[i][1]->SetPos( i*alpha[i][1]->Size(), 5*alpha[i][1]->Size() );
		alpha[i][1]->SetRotation( i );
		GLASSERT( alpha[i][1] );
		engine->Tree()->AddNode( c1, alpha[i][1] );
	}

	// red:
	for ( i=0; i<8; i++ )
	{
		alpha[i][2] = new KrTile( alphaRes );
		alpha[i][2]->SetColor( redCForm );
		alpha[i][2]->SetPos( i*alpha[i][2]->Size(), 6*alpha[i][2]->Size() );
		alpha[i][2]->SetRotation( i );
		GLASSERT( alpha[i][2] );
		engine->Tree()->AddNode( c1, alpha[i][2] );
	}

	// combination:
	for ( i=0; i<8; i++ )
	{
		alpha[i][3] = new KrTile( alphaRes );
		alpha[i][3]->SetColor( blueAlphaCForm );
		alpha[i][3]->SetPos( i*alpha[i][3]->Size(), 7*alpha[i][3]->Size() );
		alpha[i][3]->SetRotation( i );
		GLASSERT( alpha[i][3] );
		engine->Tree()->AddNode( c1, alpha[i][3] );
	}

	// ----------- A canvas ----------------- //
	KrCanvasResource* canvasResource = new KrCanvasResource( "mycanvas",
															 50, 50,
															 true );
	engine->Vault()->AddResource( canvasResource );
	KrRGBA* pixels = canvasResource->Pixels();
	KrRGBA color;
	for( i=0; i<canvasResource->Width(); i++ )
	{
		for( j=0; j<canvasResource->Height(); j++ )
		{
			color.c.red = i*4 + 50;
			color.c.green = j*4 + 50;
			color.c.blue = 0;
			color.c.alpha = 255 - i;
			pixels[ j*canvasResource->Width() + i ] = color;
		}
		// Put in a diagonal line:
		color.Set( 0, 0, 255 );
		pixels[ i*canvasResource->Width() + i ] = color;
	}

	const int NUMCANVASDIV2 = NUMCANVAS / 2;
	for ( i=0; i<NUMCANVASDIV2; i++ )
	{
		GlFixed sx;
		GlFixed sy;

		// The left canvas:
		canvas[i*2] = new KrCanvas( canvasResource );

		engine->Tree()->AddNode( c2, canvas[i*2] );
		canvas[i*2]->SetPos( 0, i * canvas[i*2]->Height() * 3 / 2 );
		sx.v = GlFixed_1 * (i+1) / 3;
		sy.v = GlFixed_1 * (i+1) / 3;

		canvas[i*2]->SetScale( sx, sy );

		// The right canvas:
		canvas[i*2+1] = new KrCanvas( canvasResource );
		engine->Tree()->AddNode( c2, canvas[i*2+1] );

		canvas[i*2+1]->SetPos( 100, i * canvas[i*2+1]->Height() * 3 / 2 );
		sx.v = GlFixed_1 * (NUMCANVASDIV2 + 1 - i) / 3;
		sy.v = GlFixed_1 * (i+1) / 3;

		canvas[i*2+1]->SetScale( sx, sy );
	}
}

TileTest::~TileTest()
{
	//GLOUTPUT( "TileTest DESTRUCT\n" );
	engine->Validate();	// check validity
	//GLOUTPUT( "TileTest DESTRUCT checked\n" );
	delete engine;
}


/*	This follows the same pattern as the blit test, but it uses
	tiles and cavases instead of sprites. Once again, it is used
	to make sure the basic blts and transformations are correct.
*/
void TileTest::DrawFrame()
{
	int index  = Frame() % NUMCANVAS;
	int quality = ( Frame() / NUMCANVAS ) % 3 + 1;

	canvas[ index ]->SetQuality( quality );
	//GLOUTPUT( "TileTest Drawframe A\n" );
	engine->Validate();	// check validity
	engine->Draw();
	//GLOUTPUT( "TileTest Drawframe B\n" );
	engine->Validate();	// check validity
	//GLOUTPUT( "TileTest Drawframe done\n" );

	if ( Frame() > 20 )
	{
		if ( Frame() > SCALEY )
		{
			int vFrame = Frame() - SCALEY + 1;
			int tFrame = TestFrames() - SCALEY;
			GlFixed scale = GlFixed( vFrame ) / GlFixed( tFrame );

			for( int i=0; i<8; ++i )
			{
				for( int j=0; j<4; ++j ) 
				{
					alpha[i][j]->SetScale( 1, scale );
					noAlpha[i][j]->SetScale( 1, scale );
				}
			}
		}
		else if ( Frame() > SCALEX )
		{
			int vFrame = Frame() - SCALEX + 1;
			int tFrame = SCALEY - SCALEX;
			GlFixed scale = GlFixed( vFrame ) / GlFixed( tFrame );
			for( int i=0; i<8; ++i )
			{
				for( int j=0; j<4; ++j )
				{
					alpha[i][j]->SetScale( scale, 1 );
					noAlpha[i][j]->SetScale( scale, 1 );
				}
			}
		}
		else if ( Frame() > SCALEBOTH )
		{
			int vFrame = Frame() - SCALEBOTH + 1;
			int tFrame = SCALEX - SCALEBOTH;
			GlFixed scale = GlFixed( vFrame ) / GlFixed( tFrame );
			for( int i=0; i<8; ++i )
			{
				for( int j=0; j<4; ++j )
				{
					alpha[i][j]->SetScale( scale, scale );
					noAlpha[i][j]->SetScale( scale, scale );
				}
			}
		}
	}
}
