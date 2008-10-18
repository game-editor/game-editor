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
#include <ctype.h>

TitleScreen::TitleScreen( SDL_Surface* screen )
{
	engine = new KrEngine( screen );
	engine->StartSplash( SDL_GetTicks() );

//	// Pull the title into the default vault.
//	bool ret = engine->Vault()->LoadDatFile( "title.dat" );
//	if ( !ret )
//	{
//		#ifdef DEBUG
//		GLOUTPUT( "Error loading title.dat\n" );
//		GLASSERT( 0 );
//		#endif
//		return;
//	}

	// And create another vault for the text.
	vault = new KrResourceVault();
	bool ret = vault->LoadDatFile( "font.dat" );
	if ( !ret )
	{
		#ifdef DEBUG
		GLOUTPUT( "Error loading font.dat\n" );
		GLASSERT( 0 );
		#endif
		return;
	}

	// Create the sprite and text, and throw them in.
	parent = new KrImNode();
	engine->Tree()->AddNode( 0, parent );

//	KrSpriteResource* splashRes = engine->Vault()->GetSpriteResource( "SPLASH" );
//	GLASSERT( splashRes );
//	KrSprite* sprite = new KrSprite( splashRes );
//	engine->Tree()->AddNode( parent, sprite );

	// center it:
//	KrRect bounds;
//	sprite->QueryBoundingBox( &bounds );
//	parent->SetPos( engine->ScreenBounds().Width() / 2 - bounds.Width() / 2,
//					engine->ScreenBounds().Height() / 3 - bounds.Height() / 2 );

	KrFontResource* font = vault->GetFontResource( "CONSOLE" );
	GLASSERT( font );
	text = new KrTextBox( font, 640, 400, 2, KrTextBox::CENTER );
//	text->SetPos( bounds.Width() / 2 - 320, bounds.Height() + 10 );
	text->SetPos( 0, 300 );

//	text->SetTextChar( "Kyra Sprite Engine", 0 );

	int major, minor, build;
	KrEngine::Version( &major, &minor, &build );
	char buf[ 512 ];
	sprintf( buf, "%s v%d.%d.%d", 
			#ifdef DEBUG
				"Debug build",
  			#else
				"Release build",
			#endif
			major, minor, build );


	text->SetTextChar( buf, 1 );

	text->SetTextChar( "www.grinninglizard.com/kyra", 2 );
	text->SetTextChar( "www.sourceforge.net/projects/kyra", 3 );

	text->SetTextChar( "Demos will automatically advance,", 5 );
	text->SetTextChar( "or press the spacebar to skip to next.", 6 );

	GameFactory::GetHeader( screen, buf );

	// Need to split up the lines.
	int line = 8;
	char* p = buf;
	char* q = p;

	while ( *q && *p )
	{
		++q;
		while ( *q && ( q - p ) < 50 )
		{
			++q;
		}
		if ( *q )
		{
			// try to split the line.
			while ( *q )
			{
				if ( isspace( *q ) )
				{
					*q = 0;
					++q;
					break;
				}
				++q;
			}
   		}
		text->SetTextChar( p, line );
		p = q;
		++line;
    }

	engine->Tree()->AddNode( parent, text );
}

TitleScreen::~TitleScreen()
{
	delete engine;
	delete vault;
}


void TitleScreen::DrawFrame()
{
	KrColorTransform cform;

	if ( Frame() < TestFrames() - 10 )
	{
		engine->UpdateSplash( SDL_GetTicks() );
	}
	else if ( Frame() == TestFrames() - 10 )
	{
		engine->EndSplash();
		text->SetTextChar( "Kyra Sprite Engine", 0 );
	}

	// Choses to exercise all the OpenGL color transforms.
	if ( Frame() < THRESH0 )
	{
		cform.Set(	0, 0, 
					255, 0,
					0, 0,
					255 * Frame() / THRESH0 );
	}
	else if ( Frame() < THRESH1 )
	{
		int num = Frame() - THRESH0;
		int den = THRESH1 - THRESH0;

		cform.Set(	255 * num / den, 0,
					255, 0,
					255 * num / den, 0,
					255 );
	}
	else
	{
		cform.SetIdentity();
	}

	parent->SetColor( cform );

	char buf[256];
	sprintf( buf, "[%d / %d]", Frame(), TestFrames() );
	text->SetTextChar( buf, 6 );
	engine->Draw();
}
