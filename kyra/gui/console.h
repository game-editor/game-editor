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

#ifndef STAND_ALONE_GAME //maks

#ifndef KYRA_CONSOLE_INCLUDED
#define KYRA_CONSOLE_INCLUDED

#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"
#include <ctype.h>

#include "SDL.h"
#include "widget.h"
#include "../util/gltypes.h"
#include "../util/gldynarray.h"
#include "../util/glcirclelist.h"
#include "../util/gllist.h"


class KrTextBox;
class KrImageTree;
class KrCanvasResource;
class KrCanvas;
class KrFontResource;
class KrImNode;
union KrRGBA;
class KrBoxResource;
class KrBox;
class KrTextWidget;


/**	A GUI Widget to display a (basic) console window.

	Emulates a simple command console, with one line text 
	editing, history buffer, and command completion.

	Needs SDL_EnableUNICODE( true ) to function correctly.

	<b> Events Sent </b>
	
	- ACTIVATED when a command is entered that is recognized 
	  by the console window. If the receiving listener as
	  been marked as the special "command handler" then
	  the 'data' parameter of HandleWidgetEvent will be
	  non zero.

*/
class KrConsole : public KrWidget
{
  public:
	/** The font resource to use is passed in, as well as the size
		of the console box. The lineSpacing can be used to give
		additional space between lines. The normal behavior is zero.

		WARNING: The font is in the scheme, and must be set.
	*/
	KrConsole(	int width, int height,
				int lineSpacing,
				const KrScheme& scheme );

	~KrConsole();

	/** Set a background color
		Only works after the KrConsole has been added to the Tree. 
	*/
	void SetBackgroundColor( const KrRGBA& color );

//	Not my best idea. Better solved by simply putting color or an image
//	behind the object.
//	/** Set a background box: same options and flags as KrBoxResource. 
//		Only works after the KrConsole has been added to the Tree. 
//	*/
//	void SetBackgroundBox(	const KrRGBA* colorArray,
//							int numColors,
//							int flags );

	// Not a mouse listener.

	// Not a key listener. Odd, but its child textwidget is what gets the keys.
	// Does get keys if children don't handle.
	virtual bool KeyEvent( const SDL_Event& key );

	// Not groupable.
	// Doesn't accelerate.
	// Doesn't handle events.

	/// Prints out a null termintated text string.
	void PushText( const char* text );
	/// Prints using "printf" format. Will handle newlines.
	void Print( const char* format, ... );

	/// Get the current text in the command line.
	void GetEntryTextChar( gedString* buffer );

	/** Adds a command to the list of recognized (and auto-completed) commands.

		This will add handler as a listerner if it is not already.
	*/
	void AddCommand( const gedString&, IKrWidgetListener* handler );

	const KrTextBox* TextBox()		{ return textBox; }

	KrImNode* ToExtended( const gedString& name )		{ if ( name == "console" ) return this;
														  return 0;
														}

	virtual bool HandleWidgetEvent(	KrWidget* source, 
									U32 event, U32 data, const SDL_Event* sdlEvent,
									const char* command, const char* arg );

	// Used to initialize the console.
	virtual void AddedtoTree();

  private:
	void PositionCursor();
	void ProcessEnterKey();
	void TabCompletion();

	enum
	{
		CURSOR_WIDTH = 2,
		COMMAND_BUF_SIZE = 32,
		LINE_BUF_SIZE    = 256
	};

	enum
	{
		DEPTH_BACKGROUND = -10,
		DEPTH_TEXT
	};

	struct Command
	{
		gedString			command;
		IKrWidgetListener*	handler;
	};

	GlCircleList<gedString>	commandBuf;			// Command history
	int							commandBufSize;
	GlCircleNode<gedString>*	commandBufNode;		/// Current location in history
	GlSList<Command>			commandList;		// List of recognized commands.

	KrTextBox*			textBox;
//	KrCanvasResource*	cursorRes;
//	KrCanvas*			cursor;

//	int					cursorPos;
	int					width, height, lineSpacing;

	KrFontResource*		font;
	KrBoxResource*		backgroundRes;	
	KrBox*				background;
	KrTextWidget*		commandLine;
};


#endif

#endif //#ifndef STAND_ALONE_GAME //maks