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
#include "SDL.h"
#include "statemachine.h"
#include "../engine/parser.h"
#include "../engine/textbox.h"
#include "../engine/box.h"


EdStateMovie::EdStateMovie( Editor* _machine ) : EdState( _machine )
{
	spriteResource = 0;
	movieNode = 0;
}


void EdStateMovie::StateOpening()
{
	movieNode = new KrImNode();
	shared->Engine()->Tree()->AddNode( shared->ImNode(), movieNode );

	// Create the action, put it in a sprite resource.
	KrAction* movieAction = shared->CreateAnimationAction();
	if ( movieAction )
	{
		spriteResource = new KrSpriteResource(	"AlignerSprite" );
		spriteResource->AddAction( movieAction );

		numFrames = spriteResource->GetActionByIndex( 0 )->NumFrames();
	
		KrSprite* sprite = new KrSprite( spriteResource );
		sprite->SetFrame( 0 );
		sprite->SetNodeId( 0 );

		KrRect rect = shared->Engine()->ScreenBounds();
		sprite->SetPos( rect.Width()  / 2,
						rect.Height() / 2 );
		shared->Engine()->Tree()->AddNode( movieNode, sprite );
	}	
	shared->ConsoleNode()->SetVisible( false );	
	shared->SetInfoBox( false );
}


void EdStateMovie::StateClosing()
{
	shared->Engine()->Tree()->DeleteNode( movieNode );	
	delete spriteResource;
	spriteResource = 0;
}


void EdStateMovie::FrameTick()
{
	KrImNode* image = shared->Engine()->Tree()->FindNodeById( 0 );
	GLASSERT( image );
	KrSprite* sprite = image->ToSprite();
	GLASSERT( sprite );

	sprite->DoStep();

	int x = sprite->CompositeXForm(0).x.ToInt();
	int y = sprite->CompositeXForm(0).y.ToInt();
	KrRect screen = shared->Engine()->ScreenBounds();
	GlFixed scale = movieNode->XScale();
	GlFixed inverse = GlFixed( 1 ) / scale;

	if ( x < 0 )
		sprite->SetPos( (inverse * screen.Width()).ToInt(), sprite->Y() );
	if ( x > screen.Width() )
		sprite->SetPos( 0, sprite->Y() );
	if ( y < 0 )
		sprite->SetPos( sprite->X(), (inverse*screen.Height()).ToInt() );
	if ( y > screen.Height() )
		sprite->SetPos( sprite->X(), 0 );
}


void EdStateMovie::ZoomIn()
{
	GlFixed scale = movieNode->XScale();
	if ( scale < 4 )
	{
		scale *= 2;
		movieNode->SetScale( scale, scale );


		KrImNode* image = shared->Engine()->Tree()->FindNodeById( 0 );
		GLASSERT( image );
		KrSprite* sprite = image->ToSprite();
		GLASSERT( sprite );

		shared->Engine()->Tree()->Walk();

		//int x = sprite->CompositeXForm(0).x.ToInt();
		//int y = sprite->CompositeXForm(0).y.ToInt();
		KrRect screen = shared->Engine()->ScreenBounds();
		GlFixed scale = movieNode->XScale();
		GlFixed inverse = GlFixed( 1 ) / scale;

		sprite->SetPos( (inverse * screen.Width()).ToInt() / 2, (inverse*screen.Height()).ToInt()/2 );
	}
}


void EdStateMovie::ZoomOut()
{
	GlFixed scale = movieNode->XScale();
	if ( scale.v > GlFixed_1 / 4 )
	{
		scale /= 2;
		movieNode->SetScale( scale, scale );


		KrImNode* image = shared->Engine()->Tree()->FindNodeById( 0 );
		GLASSERT( image );
		KrSprite* sprite = image->ToSprite();
		GLASSERT( sprite );

		shared->Engine()->Tree()->Walk();

		//int x = sprite->CompositeXForm(0).x.ToInt();
		//int y = sprite->CompositeXForm(0).y.ToInt();
		KrRect screen = shared->Engine()->ScreenBounds();
		GlFixed scale = movieNode->XScale();
		GlFixed inverse = GlFixed( 1 ) / scale;

		sprite->SetPos( (inverse * screen.Width()).ToInt() / 2, (inverse*screen.Height()).ToInt()/2 );
	}
}

