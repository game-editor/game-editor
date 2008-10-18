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



#include "../tinyxml/tinyxml.h"
#include "SDL.h"
#include "SDL_image.h"
#include "../engine/kyra.h"
#include "../gui/console.h"
#include "../engine/encoder.h"
#include "../spriteed/CONSOLEFONT.h"
#include <string.h>

void EncoderOutput( KrEngine* engine, KrResourceVault* vault );

const int SCREENX = 640;
const int SCREENY = 480;

const int fontDatSize = 11153;
extern unsigned char fontDat[];


void ProcessOneXMLFile( const char* inputXML,
						KrEngine* engine,
						KrEncoder* encoder,
						KrConsole* console )
{
	const gedString inputName( inputXML );

	TiXmlDocument doc( inputName );
	doc.LoadFile();

	if ( doc.Error() )
		printf( "Error reading XML file: '%s'\n", doc.ErrorDesc().c_str() );
	else
		printf( "Processing XML file '%s'\n", inputName.c_str() );

	if ( !doc.Error() )
	{
		encoder->ProcessDoc( doc, engine, console );
	}
	else
	{
		console->Print( "ERROR from XML: '%s'. XML file '%s'.\n", doc.ErrorDesc().c_str(), doc.Value().c_str() );
	}
	engine->Draw();
}


int main( int argc, char* argv[] )
{
	SDL_Surface* screen = 0;
	int i = 0;

	const SDL_version* sdlVersion = SDL_Linked_Version();
	if ( sdlVersion->minor < 2 )
	{
		fprintf( stderr, "SDL version must be at least 1.2.0" );
		GLASSERT( 0 );
		exit( 254 );
	}

	if ( argc < 3 )
	{
		printf( "Usage: encoder [options] outputfile inputfile...\n" );
		printf( "  Options:  -pPREFIX append PREFIX before every name in the header.\n" );
		printf( "            -rFILE   read a list of xml input filenames from FILE.\n" );
		printf( "            -w       wait for 'spacebar' between screens.\n" );
		printf( "            -f       run fullscreen.\n" );
		exit( 0 );
	}

	int firstIndex = 1;
	const char* prefix = 0;
	bool waitForSpacebar = false;
	bool fullScreen = false;
	const char* fromFile = 0;

	while( *argv[firstIndex] == '-' )
	{
		char* option = argv[firstIndex] + 1;
		if ( *( option ) == 'p' )
		{
			prefix = ( option+1 );
		}
		else if ( *( option ) == 'w' )
		{
			waitForSpacebar = true;
		}
		else if ( *( option ) == 'f' )
		{
			fullScreen = true;
		}
		else if ( *( option ) == 'r' )
		{
			fromFile = (option+1 );
		}
		firstIndex++;
	}

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(255);
	}

	int flags = SDL_SWSURFACE;
	if ( fullScreen )
		flags |= SDL_FULLSCREEN;

	int depth = SDL_VideoModeOK( SCREENX, SCREENY, 0, flags );
	if ( depth < 16 )
		depth = 16;

	screen = SDL_SetVideoMode( SCREENX, SCREENY, depth, flags );

	if ( screen == NULL ) {
		fprintf(stderr,"Couldn't set video mode: %s\n", SDL_GetError());
		exit(3);
	}

	KrEncoder::SetImageLoader( &IMG_Load );

	KrEngine* engine = new KrEngine( screen );
	KrFontResource* fontRes = KrEncoder::CreateFixedFontResource( "CONSOLE", CONSOLEFONT_DAT, CONSOLEFONT_SIZE );

	KrScheme scheme( fontRes );	
	KrConsole* console = new KrConsole( SCREENX, SCREENY,
										0, scheme );
	engine->Tree()->AddNode( 0, console );

	int major, minor, patch;
	KrEngine::Version( &major, &minor, &patch );
	console->Print( "Starting encoder version %d.%d.%d.\n", major, minor, patch );
	engine->Draw();

	// Create and set the extensions for the output files
	gedString datOutputName( argv[firstIndex] );
	gedString hOutputName( argv[firstIndex] );
	GlString::SetExtension( &datOutputName, "dat" );
	GlString::SetExtension( &hOutputName, "h" );

	SDL_RWops* stream = SDL_RWFromFile( datOutputName.c_str(), "wb" );
	if ( stream )
	{
		SDL_WM_SetCaption( "Kyra Encoder", 0 );

		console->Print( "Write stream open. Beginning processing of output: %s\n", datOutputName.c_str() );
		engine->Draw();
		KrEncoder encoder ( stream );

		if ( fromFile )
		{
			FILE* file = fopen( fromFile, "r" );
			if ( file )
			{
				char buffer[512];
				char* p;
				while( fgets( buffer, sizeof( buffer ), file ) )
				{
					p = strchr( buffer, '\n' );
					if ( p ) *p = 0;
					p = strchr( buffer, '\r' );
					if ( p ) *p = 0;

					if ( buffer[0] && buffer[0] != '#' )
					{
						console->Print( "Processing '%s'\n", buffer );
						ProcessOneXMLFile( buffer, engine, &encoder, console );
					}
				}
				fclose( file );
			}	
			else
			{
				console->Print( "ERROR: could not open file list '%s'\n", fromFile );
			}
		}
		else
		{
			for ( i=firstIndex+1; i<argc; i++ )
			{
				ProcessOneXMLFile( argv[i], engine, &encoder, console );
			}
		}
		console->Print( "Writing DAT file.\n" );
		engine->Draw();
		encoder.StartDat();
		encoder.Save();
		encoder.EndDat();
		console->Print( "DAT complete. Writing header '%s'\n", hOutputName.c_str() );
		engine->Draw();
	
//		GlSListIterator< KrResource* > it = encoder.GetVault()->GetResourceIterator();
//		it.Begin();

		if ( waitForSpacebar )
		{
			console->Print( "Press spacebar to continue.\n" );
			SDL_Event event;

			while( true )
			{
				int haveEvent = SDL_PollEvent(&event);
				if ( haveEvent )
				{
					if ( 	event.type == SDL_KEYDOWN
						 && event.key.keysym.sym == SDLK_SPACE )
						break;

					if ( event.type == SDL_QUIT )
					{
						goto exit_out;	// Sad. May be the only goto I've ever used.
						break;
					}

					KrEventManager::Instance()->HandleEvent( event, engine );
				}
				engine->Draw();
			}

			console->SetVisible( false );
			EncoderOutput( engine, encoder.GetVault() );

		}

		FILE* fp = fopen( hOutputName.c_str(), "w" );
		if ( fp )
		{
			gedString headerName = hOutputName;
			unsigned i = headerName.find( '.' );
			if ( i != gedString::npos )
				headerName.resize( i );
			encoder.WriteHeader(	headerName.c_str(), fp,
									prefix );
			fclose( fp );
		}

		SDL_RWclose( stream );
	}
  exit_out:
	delete engine;
	delete fontRes;
	SDL_Quit();
	return 0;
}

