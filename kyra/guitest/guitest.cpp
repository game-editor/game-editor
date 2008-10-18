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

#include "../engine/kyra.h"
#include "SDL.h"
#include "../spriteed/CONSOLEFONT.h"
#include <stdlib.h>

/*	This simple program demonstrates the use of all the default widgets.
	It should be build as part of your Kyra build.

	Start with "main()" below, reading through how it works. Then refer up to
	the class files that main uses as they occur.
*/


/*	This creates a subclass of the textwidget. If it receives an activation
	message, it will change the text to "activated", or "de-activated".
	When made the listener of a button, it will print "activated" or "de-activated"
	as the button is pressed or released with the mouse, or when the button
	hot key is pressed or released.
*/
class MyTextWidget : public KrTextWidget
{
  public:
	MyTextWidget(	const char* _message,
					int width, int height, 
					bool drawBorder,
					bool drawBackground,
					bool editable,
					const KrScheme& scheme
				 )
		: KrTextWidget( width, height, drawBorder, drawBackground, editable, scheme ), message( _message )	{}

	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, 
									U32 data, 
									const SDL_Event* sdlEvent,
									const char* command, 
									const char* arg )
	{
		const char* eventDesc = "none";
		if ( event == ACTIVATED )
			eventDesc = "activated";
		else if ( event == DEACTIVATED )
			eventDesc = "de-activated";

		char buf[ 256 ];
		sprintf( buf, "%s %s", message, eventDesc );
		SetTextChar( buf );
		return true;
	};										

	private:
		const char* message;
};


/*	This is a *very* useful pattern for working with widgets. ScrollButtonConnector
	"connects", with minimal logic, a widget to another. In this case, a button
	is connected to a scrolling listbox. The connector is constructed with a pointer
	to the listbox, and listens to the button. When the button message comes in,
	MoveSelection is called on the the listbox.

	In a real world system, having many small connectors, or one large widget manager
	are both valid approaches to managing widgets.
*/

class ScrollButtonConnector : public IKrWidgetListener
{
  public:	
	ScrollButtonConnector( KrListBox* _box, int _delta )
	{
		listbox = _box;
		delta = _delta;
	}

	bool HandleWidgetEvent(	KrWidget* source, 
							U32 event, 
							U32 data, 
							const SDL_Event* sdlEvent,
							const char* command, 
							const char* arg )
	{
		if ( event == ACTIVATED )
		{
			listbox->MoveSelection( delta );
			return true;
		}	
		return false;
	}

  private:
	int delta;
	KrListBox* listbox;
};


/*	A console window. It listens to all the other widgets, and prints out the
	event messages that travel through the system.
*/

class MyConsole : public KrConsole
{
  public:
  	MyConsole(	int width, int height,
				int lineSpacing,
				const KrScheme& scheme ) : KrConsole( width, height, lineSpacing, scheme )
	{}


	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, 
									U32 data, 
									const SDL_Event* sdlEvent,
									const char* command, 
									const char* arg )
	{
		char buf[ 256 ];
		const char* eventDesc[] = { "NONE", "ACTIVATED", "DEACTIVATED", "SELECTION" };
		sprintf( buf, "Source=0x%x event=%13s data=%3d sdlType=%2d command='%s' arg='%s'\n",
					  source,
					  eventDesc[ event ],
					  data,
					  ( sdlEvent ) ? sdlEvent->type : -1,
					  command ? command : "--null--",
					  arg ? arg : "--null--" );
		PushText( buf );
		return true;	
	}
};


#define SDL_TIMER_EVENT ( SDL_USEREVENT + 0 )
const int TIMER_INTERVAL = 200;

/*	A timer callback, used to test widgets on a timer.
*/
Uint32 TimerCallback(Uint32 interval)
{
	SDL_Event event;
	event.type = SDL_TIMER_EVENT;

	SDL_PeepEvents( &event, 1, SDL_ADDEVENT, 0 );
	return TIMER_INTERVAL;
}


/*	A Kyra main loop, that displays all the widgets on a test screen. Note that the
	setup code for Kyra and SDL is skimmed over -- check out tutorial1 for a
	full explanation there.
*/
int main( int argc, char *argv[] )
{
	const SDL_version* sdlVersion = SDL_Linked_Version();
	if ( sdlVersion->minor < 2 )
	{
		printf( "SDL version must be at least 1.2.0\n" );
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

	SDL_WM_SetCaption( "Kyra GuiTest", 0 );

	int screenX = 800;
	int screenY = 600;

	SDL_Surface* screen = SDL_SetVideoMode( screenX, screenY, 32, SDL_SWSURFACE );
	SDL_EnableUNICODE( true );

	/*	The 'if (screen)' is a little strange. But some Kyra classes are created
		inside the 'if' case, and it's a way to make sure their destructors are
		called before "SDL_Quit()".
	*/
	if ( screen )
	{
		/*	The console font is compiled in. CONSOLEFONT.CPP contains a font bmp file
			in a big array. CreateFixedFontResource is a simple way to load in such
			a file. Fonts are so basic it's nice to not have to load them from a dat
			when developing and prototyping, although you certainly could.
		*/
		KrFontResource* consoleFontRes =  KrEncoder::CreateFixedFontResource( "CONSOLE", CONSOLEFONT_DAT, CONSOLEFONT_SIZE );
		KrEngine* engine = new KrEngine( screen );

		/*	In order to test the icon / decal functionality of the buttons,
			grab some sprites from the test directory. In order to not
			go crazy with compile & make ordering, make the icons optional.
		*/
		bool useIcons = false;
		if ( engine->Vault()->LoadDatFile( "../tests/space.dat" ) )
		{
			useIcons = true;
		}
			
		/*	Generally, you'll want all your widgets to share a color scheme.
			This uses the default. However, you can reach in and set a primary and
			matching secondary color to customize the look of the widgets.
		*/
		KrScheme scheme( consoleFontRes );

		/*	--- Toggle Buttons ---

			2 toggle buttons in the lower left of the screen. Create them,
			with size and scheme, add to the Tree() and you're ready to go!
			Like all KrImage's, they are positioned with SetPos().

			The top button (0) responds to a control-2, and the bottom
			button (1) responds to control-3.

			Toggle buttons automatically group. Only one thing can be selected
			in a group at a time, so they default to radio button behavior.
			You can put them in different groups using the SetGroup() method.
		*/
		KrToggleButton* toggle0 = new KrToggleButton( 30, 30, scheme );
		engine->Tree()->AddNode( 0, toggle0 );
		toggle0->SetPos( 10, 150 );

		KrToggleButton* toggle1 = new KrToggleButton( 30, 30, scheme );
		engine->Tree()->AddNode( 0, toggle1 );
		toggle1->SetPos( 10, 190 );

		toggle0->SetAccelerator( KMOD_CTRL, SDLK_2 );
		toggle1->SetAccelerator( KMOD_CTRL, SDLK_3 );

		/* --- Left List Box ---

		This list box is underfilled. Each line you added is added to the bottom
		of the box. An index is returned that identifies it in the future. A SELECTION
		event will return the index, allow you to know what the user selected.

		*/
		KrListBox* listBox = new KrListBox( consoleFontRes, 150, 200, 20, scheme );
		listBox->SetPos( 330, 10 );
		engine->Tree()->AddNode( 0, listBox );
		listBox->AddTextChar( "item 0" );
		listBox->AddTextChar( "item 1" );
		listBox->AddTextChar( "item 2" );
		listBox->AddTextChar( "item 3" );

		/* --- Right List Box --

		This list box is overfilled. Which means you need to be able to scroll it
		to see all the contents. 
		
		How to do this?

		The push buttons, below, will scroll the selection up or down when they are
		pressed.
		*/

		KrListBox* listBox2 = new KrListBox( consoleFontRes, 150, 200, 20, scheme );
		listBox2->SetPos( 500, 10 );
		engine->Tree()->AddNode( 0, listBox2 );
		for( int i=0; i<20; ++i )
		{
			char buf[ 256 ];
			sprintf( buf, "item %d", i );
			listBox2->AddTextChar( buf );
		}

		/* --- Push Buttons ---

		These push buttons are subclasses of the default push button. When pressed,
		they will scroll the list box up or down.

		The buttons will respond to control-0 and control-1.

		See the ScrollButtonConnector comments for how this system works.
		*/
		KrPushButton* push0 = new KrPushButton( 50, 50, scheme );
		engine->Tree()->AddNode( 0, push0 );
		push0->SetPos( 10, 10 );

		KrPushButton* push1 = new KrPushButton( 50, 50, scheme );
		push1->SetPos( 10, 70 );
		engine->Tree()->AddNode( 0, push1 );

		push0->SetAccelerator( KMOD_CTRL, SDLK_0 );
		push1->SetAccelerator( KMOD_CTRL, SDLK_1 );

		ScrollButtonConnector connector0( listBox2, -1 );
		ScrollButtonConnector connector1( listBox2, 1 );
		push0->AddListener( &connector0 );
		push1->AddListener( &connector1 );

		/* --- Text Widgets ---

		These text widgets listen to the 2 push buttons. They are subclassed
		to set their text to "activated" or "de-activated" in response to the
		button state. There are 2 steps to this:

		- subclass the text widget to respond to the the ACTIVATED and DEACTIVATED
		  events.

		- use AddListener() to attach the text widgets to the buttons.

		*/
		MyTextWidget* text0 = new MyTextWidget( "Push Button 0",
												200, 50, true, true, true, scheme );
		text0->SetPos( 100, 10 );
		engine->Tree()->AddNode( 0, text0 );
		
		MyTextWidget* text1 = new MyTextWidget( "Push Button 1",
												200, 50, true, true, true, scheme );
		text1->SetPos( 100, 70 );
		engine->Tree()->AddNode( 0, text1 );

		push0->AddListener( text0 );
		push1->AddListener( text1 );

		/* --- Push buttons with icons ---

		Here to test the icon functionality, with oddly sized buttons. The Medium ship
		resource is a completely arbritrary choice.

		*/

		KrPushButton* iconButton0 = 0;
		KrPushButton* iconButton1 = 0;

		if ( useIcons )
		{
			KrSprite* sprite = 0;
			KrSpriteResource* medShipRes = engine->Vault()->GetSpriteResource( "MED" );
			GLASSERT( medShipRes );
	
			// Button 0
			iconButton0 = new KrPushButton( 70, 50, scheme );
			sprite = new KrSprite( medShipRes );
			sprite->SetAction( "BODY" );

			iconButton0->SetIcon( sprite );
			engine->Tree()->AddNode( 0, iconButton0 );
			iconButton0->SetPos( 10, 240 );

			// Button 1
			iconButton1 = new KrPushButton( 30, 50, scheme );
			sprite = new KrSprite( medShipRes );
			sprite->SetAction( "BODY" );

			engine->Tree()->AddNode( 0, iconButton1 );
			iconButton1->SetIcon( sprite );
			iconButton1->SetPos( 90, 240 );
		}

		/* --- Progress Bar --- */
		KrProgress* progressBar = new KrProgress( 200, 50, scheme );
		progressBar->SetPos( 10, 450 );
		progressBar->SetMaxValue( 122 );
		engine->Tree()->AddNode( 0, progressBar );

		/* --- Eavsdropping console ---

		This console window is set up as a listener of everyone, and will print 
		every event published to the system.
		*/
		MyConsole* console = new MyConsole( 600, 100, 1, scheme );
		console->SetPos( 10, 300 );
		engine->Tree()->AddNode( 0, console );

		push0->AddListener( console );
		push1->AddListener( console );
		text0->AddListener( console );
		text1->AddListener( console );
		toggle0->AddListener( console );
		toggle1->AddListener( console );
		listBox->AddListener( console );
		listBox2->AddListener( console );
		if ( useIcons )
		{
			iconButton0->AddListener( console );
			iconButton1->AddListener( console );
		}


		/*	Now the main loop logic. Stay in an event loop, listening,
			and Draw() after every event. To make sure that the EventManager
			gets all the mouse and keyboard events, all events are sent. (You
			could be more selective, but it doesn't hurt to send everything.)

			The KrEventManager is a singleton. It doesn't need to be created.
		*/ 
		engine->Draw();

		SDL_Event event;
		SDL_SetTimer( TIMER_INTERVAL, TimerCallback );

		while ( SDL_WaitEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
			{
				SDL_SetTimer( 0, 0 );
				break;
			}

			if ( event.type == SDL_TIMER_EVENT )
			{
				// The progress bar is on a timer. Move it
				// every time pulse.
				unsigned progress = progressBar->GetValue();
				progress += 3;
				if ( progress > progressBar->GetMaxValue() )
					progress = 0;
				progressBar->SetValue( progress );
			}

			KrEventManager::Instance()->HandleEvent( event, engine );
			engine->Draw();
		}	

		/*	Clean up! The only thing of interest here is how to destroy the
			event manager. Note that if you call KrEventManager::Instance()
			after the delete KrEventManager::Instance(), this will re-create
			the event manager, which would be (presumably) a memory leak.
		*/
		delete engine;
		engine = 0;
		delete consoleFontRes;
		consoleFontRes = 0;
		delete KrEventManager::Instance();
	}
	SDL_Quit();

	return 0;
}

