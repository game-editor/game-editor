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
#include <stdlib.h>
#include <string.h>

#define SDL_TIMER_EVENT ( SDL_USEREVENT + 0 )
const int TIMER_INTERVAL = 80;
const int SCREENX = 640;
const int SCREENY = 480;

GameTest*	currentGame = 0;
GameFactory factory;
int  singleTest = -1;
int  timerInterval = TIMER_INTERVAL;


//KrResourceVault* GameTest::fontVault = 0;


Uint32 TimerCallback(Uint32 interval)
{
	SDL_Event event;
	event.type = SDL_TIMER_EVENT;

	SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
	return timerInterval;
}


void NextTest( SDL_Surface* screen )
{
	factory.DestroyGame( currentGame );
	currentGame = 0;
	if ( singleTest < 0 )
	{
		currentGame = factory.CreateGame( factory.Current() + 1,
										  screen );

		if ( currentGame )
			timerInterval = currentGame->FrameInterval();
	}
}


int main( int argc, char *argv[] )
{
	SDL_Surface* screen;
	int  depth = 0;
	bool speedtest = false;	
	U32  flags = SDL_SWSURFACE;
	int i;
	bool timerOn = true;
	bool useOpenGL = false;
// 	bool saveBMP = false;

	const SDL_version* sdlVersion = SDL_Linked_Version();
	if ( sdlVersion->minor < 2 )
	{
		#ifdef DEBUG
			GLOUTPUT( "SDL version must be at least 1.2.0" );
		#endif
		GLASSERT( 0 );
		exit( 254 );
	}

	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) < 0 ) {
		#ifdef DEBUG
			GLOUTPUT( "Couldn't initialize SDL: %s\n",SDL_GetError());
		#endif
		exit(255);
	}

	SDL_WM_SetCaption( "Kyra Demo", 0 );

	for ( i=1; i<argc; i++ )
	{
		if ( *argv[i] == '-' && *(argv[i]+1) )
		{
			const char* param = argv[i]+2;
			switch( *(argv[i]+1) )
			{
				case 'd':
				{
					depth = atoi( param );
				}
				break;

				case 's':
				{
					speedtest = true;
				}
				break;

				case 'f':
				{
					flags |= SDL_FULLSCREEN;
				}
				break;

				case '1':
				{
					timerOn = false;
				}
				break;

				case 't':
				{
					singleTest = atoi( param );
				}
				break;

				case 'o':
				{
					useOpenGL = true;
				}

// 				case 'b':
// 				{
// 					saveBMP = true;
// 				}
// 				break;

			}
		}
	}

	if ( useOpenGL )
	{
		if ( flags & SDL_FULLSCREEN )
			flags = SDL_OPENGL | SDL_FULLSCREEN;
		else
			flags = SDL_OPENGL;
	}

	/* Create a display for the image. If we are forcing the mode,
	   skip this step and potentially use a shadow surface.
	*/
	if ( depth == 0 )
	{
		depth = SDL_VideoModeOK( SCREENX, SCREENY, 32, flags );
		if ( depth < 16 )
			depth = 16;
	}

	if ( useOpenGL )
	{
		// We want *at least* 5 bits per channel.
		//SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    	//SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    	//SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depth );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	}
	screen = SDL_SetVideoMode( SCREENX, SCREENY, depth, flags );

	if ( screen == NULL ) {
		fprintf( stdout, "Can't open display (%dx%d %dbps %s)\n",
				 SCREENX, SCREENY, depth,
				 useOpenGL ? "OpenGL" : "" );		
		exit(3);
	}

	char buf[512];
 	factory.GetHeader( screen, buf );
	printf( buf );


	SDL_Event event;

	if ( speedtest )
	{
		memset( &event, 0, sizeof( event ) );
		event.type = SDL_TIMER_EVENT;
		SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
		#ifdef DEBUG
			GLOUTPUT( "Speedtest mode!\n" );
		#endif
	}
	else if ( timerOn )
	{
		SDL_SetTimer( timerInterval, TimerCallback );
		#ifdef DEBUG
			GLOUTPUT( "Demo play mode!\n" );
		#endif
	}
	U32 start = SDL_GetTicks();

	if ( singleTest < 0 )
		currentGame = factory.CreateGame( 0, screen );
	else
		currentGame = factory.CreateGame( singleTest, screen );		
	timerInterval = currentGame->FrameInterval();

	while( currentGame && SDL_WaitEvent(&event) )
	{
		if ( event.type == SDL_QUIT )
			break;

		// Cull extra timer messages:
		if ( !speedtest && timerOn )
		{
			SDL_Event nextEvent;
			if (    SDL_PeepEvents( &nextEvent, 1, SDL_PEEKEVENT, SDL_ALLEVENTS )
			     && nextEvent.type == SDL_TIMER_EVENT )
			{
				// the event is duplicated
				//GLOUTPUT( "Timer event tossed.\n" );
				continue;
			}
		}

		switch(event.type)
		{
			case SDL_KEYDOWN:
			{
				if ( event.key.keysym.sym == SDLK_F10 )
				{
					// save a bitmap.
					static int count = 0;
					char buf[ 256 ];
					sprintf( buf, "krdemo%d.bmp", count++ );

					if ( !(screen->flags & SDL_OPENGL ) )
						SDL_SaveBMP( screen, buf );
				} 
				else if ( !timerOn && event.key.keysym.sym == SDLK_SPACE )
				{
					// Simulate a timer:
					SDL_Event e;
					memset( &e, 0, sizeof( e ) );
					e.type = SDL_TIMER_EVENT;
					SDL_PeepEvents( &e, 1, SDL_ADDEVENT, 0 );
				}
				else if ( event.key.keysym.sym == SDLK_s )
				{
					timerInterval = 500;
				}
				else if ( event.key.keysym.sym == SDLK_a )
				{
					timerInterval = 10000;
				}
				else
				{
					NextTest( screen );
				}
			}
			break;

			case SDL_KEYUP:
			{
				if (    event.key.keysym.sym == SDLK_s
					 || event.key.keysym.sym == SDLK_a )
				{
					if ( currentGame )
						timerInterval = currentGame->FrameInterval();

					SDL_SetTimer( 0, TimerCallback );
					SDL_SetTimer( timerInterval, TimerCallback );
				}
			}
			break;

			case SDL_VIDEOEXPOSE:
			{
				currentGame->VideoExpose();
//				currentGame->UpperDrawFrame();
			}
			break;

			case SDL_TIMER_EVENT:
			{
				if ( currentGame )
				{
					currentGame->UpperDrawFrame();

// 					if (    speedtest 
// 					     && currentGame->Frame() == currentGame->TestFrames() )
					if ( currentGame->Frame() == currentGame->TestFrames() )
					{
						U32 end = SDL_GetTicks();
						factory.SetTime( end - start );
						NextTest( screen );
						start = SDL_GetTicks();
					}
					if ( speedtest )
					{
						SDL_Event e;
						memset( &e, 0, sizeof( e ) );
						e.type = SDL_TIMER_EVENT;
						SDL_PeepEvents( &e, 1, SDL_ADDEVENT, 0 );
					}
				}
			}
			break;
		}

	}

	if ( speedtest )
	{
		factory.PrintTimes( screen );
	}
	else
	{
		printf( "Tested at %dbbp\n", screen->format->BitsPerPixel );
	}

	FILE* fp = fopen( "perf.txt", "w" );
	if ( fp )
	{
		fprintf( fp, "Performance: \n" );
		
		GlPerformance::Dump( fp );
		fclose( fp );
	}

	SDL_Quit();	
	return 0;
}

