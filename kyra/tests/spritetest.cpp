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


void GameTest::AddText( KrEngine* engine )
{
// 	if ( !textAdded )
// 	{
// 		KrFontResource* font = fontVault->GetFontResource( "CONSOLE" );
// 		GLASSERT( font );
// 		KrTextBox* text = new KrTextBox( font, 640, 20, 0 );
// 		text->SetZDepth( 1024 );
// 		char buf[ 512 ];
// 		int x, y, z;
// 		engine->Version( &x, &y, &z );
// 		sprintf( buf, "Kyra Sprite Engine v%d.%d.%d Demo (www.grinninglizard.com/krya)   Test: %s",
// 				 x, y, z,
// 				 Name() );
// 		text->SetTextChar( buf, 0 );
// 		engine->Tree()->AddNode( 0, text );
// 		textAdded = true;
// 	}
}



SpriteTest::SpriteTest( SDL_Surface* _screen )
{
	drawn = false;
	screen = _screen;
}


void SpriteTest::DrawFrame()
{
	if ( drawn )
		return;
	drawn = true;

	KrResourceVault vault;
	KrEngine engine( screen );

	// Classes to hold other objects:
	vault.LoadDatFile( "standardtest.dat" );
	
	//	c0
	//  |
	//  c1 ---------- c2 ----------- c3
	//  |             |				 |
	//  no xform      |				 no xform
	//  alpha		  |				 alpha
	//  red  		  alpha			 red
	//  blue,alpha    |				 green,alpha
	//				  red
	//				  |
	//				  blue,alpha

	KrResource* alphaRes = 0;
	KrResource* noAlphaRes = 0;
	KrResource* backRes = 0;

	KrSprite*   alpha = 0;
	KrSprite*	noAlpha = 0;

	int i, j;

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
	KrImNode* c0 = new KrImNode;
	KrImNode* c1 = new KrImNode;
	KrImNode* c2 = new KrImNode;
	KrImNode* c3 = new KrImNode;

	c0->SetPos(   0, 0 );

	c1->SetPos(   0,   0 );
	c2->SetPos(	300,   0 );
	c3->SetPos(   0, 200 );

	engine.Tree()->AddNode( 0, c0 );
	engine.Tree()->AddNode( c0, c1 );
	engine.Tree()->AddNode( c0, c2 );
	engine.Tree()->AddNode( c0, c3 );

	c1->SetZDepth( 1 );
	c2->SetZDepth( 1 );
	c3->SetZDepth( 1 );

	// Sprites:
	alphaRes   = vault.GetResource( "Sprite", "ALPHA" );
	noAlphaRes = vault.GetResource( "Sprite", "NOALPHA" );
	backRes	   = vault.GetResource( "Sprite", "BACKGROUND" );

	GLASSERT( alphaRes && noAlphaRes && backRes );
	GLASSERT( alphaRes->ToSpriteResource() && noAlphaRes->ToSpriteResource() );
	GLASSERT( backRes->ToSpriteResource() );
	
	KrSpriteResource* backSpriteRes = backRes->ToSpriteResource();
	KrAction* action = backSpriteRes->GetActionByIndex( 0 );
	GLASSERT( action );
	const KrRle& rle = action->Frame( 0 );

	// ---------- Background ----------- //
	for ( i=0; i <= (screen->w) / rle.Width(); i++ )
	{
		for ( j=0; j <= (screen->h) / rle.Height(); j++ )
		{
			KrSprite* sprite = new KrSprite( backSpriteRes );
			sprite->SetPos( i*rle.Width(), j*rle.Height() );
			GLASSERT( sprite );
			engine.Tree()->AddNode( c0, sprite );
		}
	}


	// ---------- The "no alpha" sprite.
	// no transform:
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	engine.Tree()->AddNode( c1, noAlpha );

	// alpha
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( alphaCForm );
	noAlpha->SetPos( 0, 50 );
	engine.Tree()->AddNode( c1, noAlpha );

	// red
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( redCForm );
	noAlpha->SetPos( 0, 100 );
	engine.Tree()->AddNode( c1, noAlpha );
	
	// blue, alpha
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( blueAlphaCForm );
	noAlpha->SetPos( 0, 150 );
	engine.Tree()->AddNode( c1, noAlpha );

	// ---------- The "no alpha" sprite in containment.
	// no transform:
	KrSprite* prev;
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	engine.Tree()->AddNode( c2, noAlpha );
	prev = noAlpha;

	// alpha
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( alphaCForm );
	noAlpha->SetPos( 0, 50 );
	engine.Tree()->AddNode( prev, noAlpha );
	prev = noAlpha;

	// red
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( redCForm );
	noAlpha->SetPos( 0, 50 );
	engine.Tree()->AddNode( prev, noAlpha );
	prev = noAlpha;
	
	// blue, alpha
	noAlpha = new KrSprite( noAlphaRes->ToSpriteResource() );
	GLASSERT( noAlpha );
	noAlpha->SetColor( blueAlphaCForm );
	noAlpha->SetPos( 0, 50 );
	engine.Tree()->AddNode( prev, noAlpha );


	// ---------- The "alpha" sprite.
	// no transform:
	alpha = new KrSprite( alphaRes->ToSpriteResource() );
	GLASSERT( alpha );
	engine.Tree()->AddNode( c3, alpha );

	// alpha
	alpha = new KrSprite( alphaRes->ToSpriteResource() );
	GLASSERT( alpha );
	alpha->SetColor( alphaCForm );
	alpha->SetPos( 0, 50 );
	engine.Tree()->AddNode( c3, alpha );
// 	alpha->SetScale( sx, sy );

	// red
	alpha = new KrSprite( alphaRes->ToSpriteResource() );
	GLASSERT( alpha );
	alpha->SetColor( redCForm );
	alpha->SetPos( 0, 100 );
	engine.Tree()->AddNode( c3, alpha );
	
	// blue, alpha
	alpha = new KrSprite( alphaRes->ToSpriteResource() );
	GLASSERT( alpha );
	alpha->SetColor( blueAlphaCForm );
	alpha->SetPos( 0, 150 );
	engine.Tree()->AddNode( c3, alpha );

	// Draw it.
	engine.Draw();

	if ( !(screen->flags & SDL_OPENGL ) )
		SDL_SaveBMP( screen, "testpatternout.bmp" ); 
}

