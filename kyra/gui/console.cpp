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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "console.h"
#include "textwidget.h"
#include "../engine/engine.h"
#include "../engine/textbox.h"
#include "../engine/canvas.h"
#include "../engine/imagetree.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "../util/glutil.h"


KrConsole::KrConsole(	int _width, int _height,
						int _lineSpacing,
						const KrScheme& scheme ) : KrWidget( scheme )
{
//	cursorPos = 0;
//	defaultHandler = 0;
	commandBufSize = 0;
	commandBufNode = 0;
	background = 0;
	backgroundRes = 0;
	font = scheme.font;
	GLASSERT( font );
	width = _width;
	height = _height;
	lineSpacing = _lineSpacing;
	commandLine = 0;

	// Create the cursor, as a child of the textbox.
//	cursorRes = new KrCanvasResource(   "cursor",
//										CURSOR_WIDTH, 
//										font->FontHeight(),
//										false );

//	int i, j;
//	KrRGBA* pixels = cursorRes->Pixels();
//	KrRGBA white;
//	white.Set( 255, 255, 255, 255 );
//
//	for( j=0; j<font->FontHeight(); j++ )
//	{
//		for ( i=0; i<CURSOR_WIDTH; i++ )
//		{
//			pixels[ i + j*cursorRes->Width() ] = white;
//		}
//	}
}


KrConsole::~KrConsole()
{
	// Our children are deleted first, so this is safe.
//	delete cursorRes;
	delete backgroundRes;
	backgroundRes = 0;
//	cursorRes = 0;
}


void KrConsole::AddedtoTree()
{
	KrWidget::AddedtoTree();

	// Create the textbox
	textBox = new KrTextBox(	font, 
								width, 
								height - lineSpacing - font->FontHeight(),	// leave space for command line
								lineSpacing );
	GLASSERT( textBox );
	if ( !textBox )
		return;

	textBox->SetZDepth( DEPTH_TEXT );
	Engine()->Tree()->AddNode( this, textBox );

	commandLine = new KrTextWidget( width, 
									font->FontHeight(),
									false,
									false,
									true,
									scheme );

	commandLine->SetPos( 0, height + lineSpacing - font->FontHeight() );
	commandLine->SetZDepth( DEPTH_TEXT );

	Engine()->Tree()->AddNode( this, commandLine );

	commandLine->AddListener( this );

//	cursor = new KrCanvas( cursorRes );
//	Engine()->Tree()->AddNode( textBox, cursor );
//
//	PositionCursor();
}


void KrConsole::SetBackgroundColor( const KrRGBA& color )
{
	if ( Engine() )
	{
		if ( background )
		{
			Engine()->Tree()->DeleteNode( background );
			background = 0;
			GLASSERT( backgroundRes );
		
			delete backgroundRes;
			backgroundRes = 0;
		}

		backgroundRes = new KrBoxResource(	"background",
											textBox->Width(), textBox->Height() + commandLine->Height(),
											&color,
											1,
											KrBoxResource::FILL );
		GLASSERT( backgroundRes );
		background = new KrBox( backgroundRes );
		background->SetZDepth( DEPTH_BACKGROUND );
		Engine()->Tree()->AddNode( this, background );
	}
}

//
//
//void KrConsole::SetBackgroundBox(	const KrRGBA* colorArray,
//									int numColors,
//									int flags )
//{
//	if ( Engine() )
//	{
//		if ( background )
//		{
//			Engine()->Tree()->DeleteNode( background );
//			background = 0;
//			GLASSERT( backgroundRes );
//		
//			delete backgroundRes;
//			backgroundRes = 0;
//		}
//		backgroundRes = new KrBoxResource(	"background",
//											textBox->Width(), textBox->Height(),
//											colorArray,
//											numColors,
//											flags );
//		GLASSERT( backgroundRes );
//		background = new KrBox( backgroundRes );
//		background->SetZDepth( DEPTH_BACKGROUND );
//		Engine()->Tree()->AddNode( this, background );
//	}
//}


//void KrConsole::PositionCursor()
//{
//	if ( Engine() )
//	{
//		// Get the position of the bottom line, and 
//		// put the cursor there.
//		int line   = textBox->NumLines() - 1;
//		int length = textBox->GetLineLength( line );
//		int y      = textBox->GetLineY( line );
//
//		// Make sure the cursor position is in range.
//		cursorPos = GlClamp( cursorPos, 
//							 0, length );
//
//		int x = textBox->FontResource()->FontWidthN( textBox->GetText16( line ), cursorPos );
//
//		//GLOUTPUT( "Cursor (%d,%d)\n", x, y );
//		cursor->SetPos( x, y );
//	}
//}


bool KrConsole::HandleWidgetEvent(	KrWidget* source, 
									U32 event, U32 data, const SDL_Event* key,
									const char* command, const char* arg )
{
	GLASSERT( source == commandLine );

	if ( event == IKrWidgetListener::ACTIVATED )
	{
		ProcessEnterKey();
	}
	return true;			// A console handles all keys.
}


bool KrConsole::KeyEvent( const SDL_Event& key )
{
	if ( key.key.keysym.sym == SDLK_UP )
	{
		if ( !commandBufNode  )
		{
			commandBufNode = commandBuf.FrontNode();
		}
		else
		{
			commandBufNode = commandBufNode->next;
		}
		commandLine->SetText( commandBufNode->data ); //maks
	}
	else if ( key.key.keysym.sym == SDLK_DOWN )
	{
		if ( !commandBufNode  )
		{
			commandBufNode = commandBuf.BackNode();
		}
		else
		{
			commandBufNode = commandBufNode->prev;
		}
		commandLine->SetText( commandBufNode->data ); //maks
	}
	else if ( key.key.keysym.sym == SDLK_ESCAPE )
	{
		commandBufNode = 0;
		commandLine->SetText( "" ); //maks
	}
	else if ( key.key.keysym.sym == SDLK_TAB )
	{
		TabCompletion();
	}
	return true;
}


void KrConsole::ProcessEnterKey()
{
	gedString buf;
	commandLine->GetText( &buf ); //maks

	// Push the command to the command buffer. Don't add duplicates.
	if ( buf == commandBuf.Front() )
	{
		if ( commandBufSize >= COMMAND_BUF_SIZE )
		{
			commandBuf.PopBack();
		}
		else
		{
			++commandBufSize;
		}
		commandBuf.PushFront( buf );
	}

	// Scroll up!
	textBox->SetText16( 0, textBox->NumLines() - 1 );
	PushText( buf.c_str() );
	commandBufNode = 0;
	commandLine->SetText( "" ); //maks

	// Scan the command list. If that fails, call the default handler.
	GlSListNode<Command>* node = 0;

	gedString compareBuf = buf;
	gedString arg;

	int spaceAt = compareBuf.find( ' ' );
	if ( spaceAt > 0 )
	{
		arg = buf.substr( spaceAt + 1, buf.length() );
		compareBuf.resize( spaceAt );
	}

	for( node = commandList.FrontNode(); node; node = node->next )
	{
		//int len = node->data.command.length();

		if ( compareBuf == node->data.command )
		{
			PublishTaggedEvent( ACTIVATED, 0, compareBuf.c_str(), arg.c_str(), node->data.handler );
			break;
		}
	}
}


//void KrConsole::AddCommandHandler( const char* command, IKrCommand* handler )
//{
//	Command c;
//	c.command = command;
//	c.handler = handler;
//
//	commandList.PushFront( c );
//}


void KrConsole::PushText( const char* text )
{
	int i;
	const U16* p;

	for( i=1; i<textBox->NumLines(); i++ )
	{
		p = textBox->GetText16( i );
		textBox->SetText16( p, i-1 );
	}
 	textBox->SetText( text, textBox->NumLines()-1 ); //maks
}


void KrConsole::TabCompletion()
{
	// Go through the command list and figure out all the commands
	// this could complete to. If only one, make the completion,
	// else push the options to the console.
	gedString buf;
	GlSListNode<Command>* node;
	GlSList<gedString*> matches;

	commandLine->GetText( &buf ); //maks

	for ( node = commandList.FrontNode(); node; node = node->next )
	{
		if ( buf == node->data.command.substr( 0, buf.length() ) ) //maks
		{
			matches.PushFront( &(node->data.command) );
		}
	}

	if ( matches.Size() > 1 )
	{
		GlSListNode<gedString*>* mNode;
		for( mNode = matches.FrontNode(); mNode; mNode = mNode->next )
		{
			PushText( mNode->data->c_str() );
		}
	}
	else if ( matches.Size() == 1 )
	{
		//strcpy( buf, matches.FrontNode()->data->c_str() );
		//strcat( buf, " " );
		buf = *matches.FrontNode()->data;
		buf += " ";
		commandLine->SetText( buf ); //maks
	}
}


void KrConsole::Print( const char* format, ... )
{
    va_list     va;
    char		buffer[1024];

    //
    //  format and output the message..
    //
    va_start( va, format );
    vsprintf( buffer, format, va );
    va_end( va );

	char* start;
	char* end;
	char* next;

	start = buffer;

	while ( start && *start )
	{
		end = strchr( start, '\n' );
		if ( end )
		{
			next  = end + 1;
			*end = 0;
			PushText( start );
			start = next;
		}
		else
		{
			PushText( start );
			start = 0;
		}			
	}
}


void KrConsole::AddCommand( const gedString& command, IKrWidgetListener* handler )
{
	AddListener( handler );

	Command c;
	c.command = command;
	c.handler = handler;

	commandList.PushFront( c );
}

void KrConsole::GetEntryTextChar( gedString* buffer )
{
	*buffer = "";
	if ( commandLine )
		commandLine->GetText( buffer ); //maks
}


#endif //#ifndef STAND_ALONE_GAME //maks
