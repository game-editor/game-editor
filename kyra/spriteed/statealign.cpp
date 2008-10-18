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


#include "states.h"
#include "statemachine.h"
#include "../engine/painter.h"
#include "../engine/parser.h"
#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/spriteresource.h"
#include "../engine/textbox.h"
#include "../engine/box.h"
#include "dom.h"


EdStateAlign::EdStateAlign( Editor* _machine ) : EdState( _machine )
{
	alignNode = 0;
}


void EdStateAlign::StateOpening()
{
	shared->ConsoleNode()->SetVisible( false );	
	shared->SetInfoBox( false );

	GLASSERT( shared->CurrentAction() );

	spriteResource = 0;
	alignNode = new KrImNode();
	shared->Engine()->Tree()->AddNode( shared->ImNode(), alignNode );
	spriteParent = new KrImNode();
	shared->Engine()->Tree()->AddNode( alignNode, spriteParent );

	// Create the action, put it in a sprite resource.
	KrAction* action = shared->CreateAnimationAction();

	if ( action )
	{
		spriteResource = new KrSpriteResource(	"AlignerSprite" );
		spriteResource->AddAction( action );
		
		// Note that the first and last sprite is a duplicate at
		// different locations.
		numFrames        = spriteResource->GetActionByIndex( 0 )->NumFrames();
		currentFrame   = 0;
		adjustingFrame = ( numFrames + numFrames - 1 ) % numFrames;
		
		for( int i=0; i <= numFrames; i++ )
		{
			KrSprite* sprite = new KrSprite( spriteResource );
			sprite->SetFrame( i % numFrames );
			sprite->SetNodeId( i + ID_OFFSET );
			shared->Engine()->Tree()->AddNode( spriteParent, 
											   sprite );
		}
	}

	// Set up an output box:
	KrFontResource* res = shared->Engine()->Vault()->GetFontResource( "CONSOLE" );
	alignInfo = new KrTextBox( res, SCREEN_X, SCREEN_Y, 2 );
	shared->Engine()->Tree()->AddNode( alignNode, alignInfo );

	PositionSprites();
}


void EdStateAlign::StateClosing()
{
	shared->Engine()->Tree()->DeleteNode( alignNode );
	delete spriteResource;
	spriteResource = 0;
	alignNode = 0;
	shared->ConsoleNode()->SetVisible( true );
}


void EdStateAlign::MotionKey( int key )
{
	int i;
	// We only every use the first action in the editor, so
	// it is safe to use GetAction( 0 )
	if ( key == SDLK_PAGEDOWN )
	{
		currentFrame = ( currentFrame + 1 ) % numFrames;
		adjustingFrame = ( currentFrame + numFrames - 1 ) % numFrames;
	}
	else if ( key == SDLK_PAGEUP )
	{
		currentFrame = ( currentFrame + numFrames - 1 ) % numFrames;
		adjustingFrame = ( currentFrame + numFrames - 1 ) % numFrames;
	}
	else
	{
		GlSListIterator< EdWidget* > it( shared->CurrentAction()->children );

		for( i = 0, it.Begin(); 
			 !it.Done(); 
			 ++i, it.Next() )
		{
			if ( i == adjustingFrame )
			{
				EdWidgetFrame* widget = it.Current()->ToFrame();
				GLASSERT( widget );
				KrDom::Frame frameData = widget->GetFrameData();

				switch ( key )
				{
					// The directions are reversed because we are
					// adjusting the delta from the previous frame
					// TO the current frame.
					case SDLK_UP:
						frameData.deltaY -= 1;
						break;
					case SDLK_DOWN:
						frameData.deltaY += 1;
						break;
					case SDLK_LEFT:
						frameData.deltaX -= 1;
						break;
					case SDLK_RIGHT:
						frameData.deltaX += 1;
						break;
				}
				widget->SetFrameData( frameData );
				break;
			}
		}
	}
	PositionSprites();
}


void EdStateAlign::PositionSprites()
{
	int x, y, i;
	KrImNode* krimNode;
	KrImage*  image;
	KrColorTransform	currentFrameColor;
	KrColorTransform	adjustingFrameColor;
	KrColorTransform	frameColor;

	currentFrameColor.Brighten( 20 );
	adjustingFrameColor.SetAlpha( 200 );
	frameColor.SetAlpha( 50 );

	// Walk the DOM and get the step information. Use this to set
	// the sprite locations.

	KrRect rect = shared->Engine()->ScreenBounds();
	x = rect.Width()  / 2;
	y = rect.Height() / 2;
	spriteParent->SetPos( x, y );
	x = y = 0;

	GlSListIterator< EdWidget* > it( shared->CurrentAction()->children );

	for( i = 0, it.Begin(); 
		 !it.Done(); 
		 ++i, it.Next() )
	{
		krimNode = shared->Engine()->Tree()->FindNodeById( i + ID_OFFSET );
		GLASSERT( krimNode );
		image = krimNode->ToImage();
		GLASSERT( image );
		
		image->SetPos( x, y );
		// the i not 0 case will be picked up by the extra frame.
		if ( i != 0 && currentFrame == i )
		{
			image->SetColor( currentFrameColor );
			image->SetZDepth( 3 );
		}
		else if ( i == adjustingFrame )
		{
			image->SetColor( adjustingFrameColor );
			image->SetZDepth( 1 );
		}
		else 
		{
			image->SetColor( frameColor );
			image->SetZDepth( 0 );
		}
		
		x += it.Current()->ToFrame()->GetFrameData().deltaX;
		y += it.Current()->ToFrame()->GetFrameData().deltaY;
	}
	// The one extra sprite
	krimNode = shared->Engine()->Tree()->FindNodeById( i + ID_OFFSET );
	GLASSERT( krimNode );
	image = krimNode->ToImage();
	GLASSERT( image );
	image->SetPos( x, y );

	if ( currentFrame == i-numFrames )
	{
		image->SetColor( currentFrameColor );
		image->SetZDepth( 3 );
	}
	else if ( i == adjustingFrame )
	{
		image->SetColor( adjustingFrameColor );
		image->SetZDepth( 1 );
	}
	else 
	{
		image->SetColor( frameColor );
		image->SetZDepth( 0 );
	}

	// Draw frame information (slap it on top of the engine output.)
// 	x = 0;
// 	y = 0;
	int dx = 0;
	int dy = 0;
	char buf[ 64 ];

	// Get the current and previous elements.
	for( i = 0, it.Begin(); 
		 !it.Done(); 
		 ++i, it.Next() )
	{
		KrDom::Frame f;
		f = it.Current()->ToFrame()->GetFrameData();

		sprintf( buf, "%2d-->%2d  dx=%3d dy=%3d %s\n",
					  i,
					  (i+1 ) % numFrames,
				      f.deltaX, f.deltaY,
					  i == adjustingFrame ? "*" : "" );
	
		alignInfo->SetTextChar( buf, i );
 		dx += f.deltaX;
 		dy += f.deltaY;
	}
	sprintf( buf, "total  dx=%2d dy=%2d\n", dx, dy );
	alignInfo->SetTextChar( buf, i );
}


void EdStateAlign::ZoomIn()
{
	GlFixed scale = spriteParent->XScale();
	if ( scale < 4 )
	{
		scale *= 2;
		spriteParent->SetScale( scale, scale );
	}	
}


void EdStateAlign::ZoomOut()
{
	GlFixed scale = spriteParent->XScale();
	if ( scale.v > GlFixed_1 / 4 )
	{
		scale /= 2;
		spriteParent->SetScale( scale, scale );
	}	
}

