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
#include <ctype.h>

#include "SDL.h"
#include "SDL_image.h"
#include "../engine/spriteresource.h"
#include "../engine/textbox.h"
#include "../gui/console.h"
#include "../gui/eventmanager.h"
#include "statemachine.h"
#include "../engine/encoder.h"

#define SDL_TIMER_EVENT (SDL_USEREVENT+0)

Editor* editor = 0;

//SDL_Surface* LocalImageLoader( const char* f )
//{
//	return IMG_Load( f );
//}

extern Uint32 TimerCallback( Uint32 interval )
{
	SDL_Event event;
	event.type = SDL_TIMER_EVENT;
	SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
	return interval;
}


int main(int argc, char *argv[])
{
	SDL_Surface* screen;

	const SDL_version* sdlVersion = SDL_Linked_Version();
	if ( sdlVersion->minor < 2 )
	{
		fprintf( stderr, "SDL version must be at least 1.2.0" );
		GLASSERT( 0 );
		exit( 254 );
	}

	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE ) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		GLASSERT( 0 );
		exit(255);
	}

	U32 flags = SDL_SWSURFACE;
	if ( argc > 1 && strcmp( argv[1], "-f" ) == 0 )
	{
		flags |= SDL_FULLSCREEN;
	}

	// Create an output surface, any bit depth. (Except 8)
	screen = SDL_SetVideoMode( SCREEN_X, SCREEN_Y, 0, flags);
	if ( screen == NULL ) {
		fprintf(stderr,"Couldn't set video mode: %s\n", SDL_GetError());
		GLASSERT( 0 );
		exit(3);
	}

	KrEncoder::SetImageLoader( &IMG_Load );

	// Create the editor
	editor = new Editor( screen );

	// The main event loop:
	SDL_Event event;
	int skipEvent = 0;
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );

	// Draw the initial screen:
	editor->SharedState()->Engine()->Draw();
	SDL_SetTimer( 150, TimerCallback );

	while(SDL_WaitEvent(&event))
	{
		if ( event.type == SDL_QUIT )
			break;
			
		// When the mouse is being used to drag the screen around,
		// there can very easily be too many messages. Same thing
		// happens when putting in a hot-cross.I attempt
		// to throw out extra mouse messages. -lee

		if ( event.type == SDL_MOUSEMOTION )
		{
	   		// Check for other mouse motion events in the queue.
    		SDL_Event eventFuture;
    		int num = SDL_PeepEvents( &eventFuture, 1, SDL_PEEKEVENT, SDL_ALLEVENTS );
    		
			// If this is the same state, ignore this one
    		if (    num > 0
				 && eventFuture.type == SDL_MOUSEMOTION
    			 && eventFuture.motion.state == event.motion.state )
    		{
				#ifdef DEBUG
    				GLOUTPUT( "Tossing extra mouse motion.\n" );
				#endif
    			continue;
    		}
		}

		switch(event.type)
		{
			case SDL_KEYDOWN:
				if (	event.key.keysym.sym == SDLK_RCTRL ||
						event.key.keysym.sym == SDLK_LCTRL )
				{
					editor->CommandDown();
				}
				else if ( event.key.keysym.sym == SDLK_KP_PLUS )
				{
					editor->ZoomIn();
				}
				else if ( event.key.keysym.sym == SDLK_KP_MINUS )
				{
					editor->ZoomOut();
				}
				else if (	 event.key.keysym.sym == SDLK_DELETE 
						  && ( event.key.keysym.mod & KMOD_SHIFT ) )
				{
					editor->Command( COMMAND_DELETE_CURRENT_FRAME, 0, 0 );
				}
				else if ( event.key.keysym.sym == SDLK_F1 )
				{
					// Help
					editor->ChangeState( EdState::EMPTY );
// 					consoleEvent = true;
				}
				else if ( event.key.keysym.sym == SDLK_F10 )
				{
					// save a bitmap.
					static int count = 0;
					char buf[ 256 ];
					sprintf( buf, "kredit%d.bmp", count++ );

					if ( !(screen->flags & SDL_OPENGL ) )
						SDL_SaveBMP( screen, buf );
				}
				else if (	 event.key.keysym.sym == SDLK_F2
						  || event.key.keysym.sym == SDLK_F3
						  || event.key.keysym.sym == SDLK_F4 )
				{
					if ( editor->SharedState()->CanvasResource() )
					{
						// To access modes 2, 3, and 4 there must
						// be a canvas.
						if ( event.key.keysym.sym == SDLK_F2 )
						{
							editor->ChangeState( EdState::VIEW );
						}
						else if ( event.key.keysym.sym == SDLK_F4 )
						{
							if (    editor->SharedState()->CurrentAction()
							     && editor->SharedState()->CurrentFrame() )
							{
								editor->ChangeState( EdState::MOVIE );
							}
							else
							{
								editor->SharedState()->Console()->Print( "You must have a frame and action selected to switch to Movie mode.\n" );
							}
						}
						else if ( event.key.keysym.sym == SDLK_F3 )
						{
							if (    editor->SharedState()->CurrentAction()
							     && editor->SharedState()->CurrentFrame() )
							{
								editor->ChangeState( EdState::ALIGN );
							}
							else
							{
								editor->SharedState()->Console()->Print( "You must have a frame and action selected to switch to Align mode.\n" );
							}
						}
					}
				}
				else if (	 editor->State() == EdState::ALIGN
						  && (
								 event.key.keysym.sym == SDLK_UP
							  || event.key.keysym.sym == SDLK_DOWN
							  || event.key.keysym.sym == SDLK_LEFT
							  || event.key.keysym.sym == SDLK_RIGHT
							  || event.key.keysym.sym == SDLK_PAGEUP
							  || event.key.keysym.sym == SDLK_PAGEDOWN ) )
				{
					editor->MotionKey( event.key.keysym.sym );
				}
				else
				{
					//editor->SharedState()->Console()->KeyEvent( event );
					KrEventManager::Instance()->HandleEvent( event, editor->SharedState()->Engine() );
// 					CON_ConsoleEvents(&event);
// 					consoleEvent = true;
				}
				break;

			case SDL_KEYUP:
				if ( event.key.keysym.sym == SDLK_RCTRL ||
					 event.key.keysym.sym == SDLK_LCTRL )
				{
					editor->CommandUp();
// 					consoleEvent = true;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				editor->MouseDown(	event.button.x,
									event.button.y,
									event.button.button,
									SDL_GetModState() );
				break;

			case SDL_MOUSEBUTTONUP:
				editor->MouseUp(	event.button.x,
									event.button.y,
									event.button.button,
									SDL_GetModState() );
				break;

			case SDL_MOUSEMOTION:
				editor->MouseMove(	event.motion.x,
									event.motion.y );
				break;

			case SDL_TIMER_EVENT:
				if ( editor->State() == EdState::MOVIE )
					editor->FrameTick();
		}

		// Repainting.
		// If there are still messages in the queue, go ahead 
		// and process.
		//if ( SDL_PollEvent( 0 ) && skipEvent < MAX_SKIP_EVENT )
		//{
		//	++skipEvent;
		//}
		//else
		//{
			skipEvent = 0;
// 			if ( editor->Refresh() || consoleEvent )
// 			{
// 				editor->Draw();
// 				CON_DrawConsole();
// 
// 				SDL_Rect rect;
// 				CON_GetBounds( &rect );
// 
// 				SDL_UpdateRects( screen, 1, &rect );
// 			}
// 			consoleEvent = false;
		editor->SharedState()->Engine()->Draw();
		//}
	}

	delete editor;

// 	CON_Destroy();			
	SDL_Quit();
	return 0;
}


