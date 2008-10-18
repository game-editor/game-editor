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


#include "sharedstate.h"
#include "../engine/action.h"
#include "../engine/parser.h"
#include "../engine/encoder.h"
#include "../engine/fontresource.h"
#include "../engine/canvasresource.h"
#include "../engine/imnode.h"
#include "../engine/engine.h"
#include "../engine/textbox.h"
#include "../engine/box.h"
#include "../engine/boxresource.h"
#include "../gui/console.h"
#include "dom.h"
#include "CONSOLEFONT.h"


SharedStateData::SharedStateData( SDL_Surface* screen )
{
	currentSprite = 0;
	currentFrame = 0;
	currentAction = 0;
	currentTile = 0;
	currentObject = SPRITE;

	// Use the sprite engine for the editor. It's a way to
	// test everything.
	engine = new KrEngine( screen );
	GLASSERT( engine );

	// Make the backgrounds reasonably sized so we don't spend
	// all our time sorting.
	backgroundRes = new KrCanvasResource( "backgroundRes", 100, 100, false );
	for ( int i=0; i<100; i++ )
	{
		for( int j=0; j<100; j++ )
		{
			KrRGBA* pixel = backgroundRes->Pixels() + i + j*backgroundRes->Width();
			if ( ( ( i / 10 ) & 0x01 ) == ( ( j / 10 ) & 0x01 ) ) 
				pixel->c.red = pixel->c.green = pixel->c.blue = 30;
			else
				pixel->c.red = pixel->c.green = pixel->c.blue = 60;
		}
	}
	engine->Vault()->AddResource( backgroundRes );

//	engine->Vault()->LoadDatFileFromMemory( fontDat, fontDatSize );
// 	engine->Vault()->LoadDatFile( "consolefont.dat" );
	KrFontResource* fontRes = KrEncoder::CreateFixedFontResource( "CONSOLE", CONSOLEFONT_DAT, CONSOLEFONT_SIZE );
	engine->Vault()->AddResource( fontRes );
	fontResource = engine->Vault()->GetFontResource( "CONSOLE" );
	GLASSERT( fontResource );

	consoleHolder = new KrImNode();
	consoleHolder->SetZDepth( CONSOLE_DEPTH );
	engine->Tree()->AddNode( 0, consoleHolder );

	int consoleHeight = fontResource->FontHeight() * 6;
	int consoleWidth  = screen->w * 9 / 12;
	consoleHolder->SetPos( 0, screen->h - consoleHeight );

	KrScheme scheme( fontResource );
	console = new KrConsole( consoleWidth, consoleHeight, 0, scheme );
	engine->Tree()->AddNode( consoleHolder, console );

	KrRGBA blue;
	blue.Set( 0, 0, 80 );
	console->SetBackgroundColor( blue );

	int a, b, c;
	engine->Version( &a, &b, &c );
	console->Print( "Welcome to the Sprite Editor! [version %d.%d.%d]\nThis area is the input console.\n",
					a, b, c  );

	// A text box for output, with a background.
	// And toss it background for good measure.
	KrRGBA infoColor[2];
	infoColor[0].Set( 0, 120, 0 );
	infoColor[1].Set( 0, 70,  0 );
	KrBoxResource* infoResource = new KrBoxResource( "", 
													 screen->w - consoleWidth, 
													 consoleHeight,
													 infoColor, 2,
													 KrBoxResource::FILL );
	engine->Vault()->AddResource( infoResource );

	infoBoxBack = new KrBox( infoResource );
	engine->Tree()->AddNode( 0, infoBoxBack );
	infoBoxBack->SetZDepth( INFO_BACKGROUND_DEPTH );
	infoBoxBack->SetPos( consoleWidth, screen->h - consoleHeight );

	infoBox = new KrTextBox(	fontResource,
								screen->w - consoleWidth, 
								consoleHeight, 0 );

	engine->Tree()->AddNode( infoBoxBack, infoBox );
	infoBox->SetZDepth( INFO_DEPTH );
	//infoBox->SetPos( consoleWidth, screen->h - consoleHeight );

	// Create a new node for the view state. Set
	// it to a depth of one, so that the image
	// (loaded later) won't cover up the widgets.
	widgetNode = new KrImNode();
	widgetNode->SetZDepth( WIDGET_DEPTH );
	engine->Tree()->AddNode( 0, widgetNode );
	
	widget = new EdWidget( engine->Tree(), widgetNode, 0 );

	imnode = new KrImNode();
	imnode->SetZDepth( MAIN_IMNODE_DEPTH );
	engine->Tree()->AddNode( 0, imnode );

// 	spriteData = new TiXmlDocument;
	defFileName = "";
	animAction = 0;
	canvasResource = 0;
	nTrans = 0;
}


SharedStateData::~SharedStateData()
{
	delete widget;				// Delete first -- will delete things from tree.
	delete engine;
	delete canvasResource;		// Not in a vault -- delete explicitly.
}


void SharedStateData::Clear()
{
	SetCurrentTile( 0 );
	SetCurrentSprite( 0 );

	delete widget;
	widget = new EdWidget( engine->Tree(), widgetNode, 0 );

	delete canvasResource;
	canvasResource = 0;

	defFileName = "";
	surfaceFileName = "";
}


void SharedStateData::SetCurrentTile( EdWidget* tile )
{
	if ( currentFrame )		currentFrame->Select( false );
	if ( currentAction )	currentAction->Select( false );
	if ( currentSprite )	currentSprite->Select( false );
	if ( currentTile )		currentTile->Select( false );

	currentObject = TILE;
	if ( tile )
	{
		GLASSERT( tile->Type() == EdWidget::TILE );
		currentTile = (EdWidgetTile*) tile;
		currentTile->Select( true );
	}
	else
	{
		currentTile = 0;
	}
	currentFrame = 0;
	currentAction = 0;
	currentSprite = 0;

	SetInfoBox( true );
}


void SharedStateData::SetCurrentFrame( EdWidget* frame )
{
	if ( currentFrame )		currentFrame->Select( false );
	if ( currentAction )	currentAction->Select( false );
	if ( currentSprite )	currentSprite->Select( false );
	if ( currentTile )		currentTile->Select( false );

	currentObject = SPRITE;
	currentTile = 0;
	if ( frame )
	{
		GLASSERT( frame->Type() == EdWidget::FRAME );
		GLASSERT( currentAction );
		GLASSERT( frame->Parent() == currentAction );

		if ( frame->Type() == EdWidget::FRAME)
		{
			currentFrame = (EdWidgetFrame*) frame;
			currentFrame->Select( true );
		}
	}
	else
	{
		currentFrame = 0;
	}
	SetInfoBox( true );
}


void SharedStateData::SetCurrentAction( EdWidget* action )
{
	if ( currentFrame )		currentFrame->Select( false );
	if ( currentAction )	currentAction->Select( false );
	if ( currentSprite )	currentSprite->Select( false );
	if ( currentTile )		currentTile->Select( false );

	currentObject = SPRITE;
	currentTile = 0;
	if ( action )
	{
		GLASSERT( action->Type() == EdWidget::ACTION );
		GLASSERT( currentSprite );
		GLASSERT( action->Parent() == currentSprite );

		if ( action->Type() == EdWidget::ACTION )
		{
			currentAction = (EdWidgetAction*) action;
		}
		currentFrame = 0;
	}
	else
	{
		currentAction = 0;
		currentFrame = 0;
	}
	SetInfoBox( true );
}


void SharedStateData::SetCurrentSprite( EdWidget* sprite )
{
	if ( currentFrame )		currentFrame->Select( false );
	if ( currentAction )	currentAction->Select( false );
	if ( currentSprite )	currentSprite->Select( false );
	if ( currentTile )		currentTile->Select( false );

	currentObject = SPRITE;
	currentTile = 0;
	if ( sprite )
	{
		GLASSERT( sprite->Type() == EdWidget::SPRITE );

		if ( sprite->Type() == EdWidget::SPRITE )
		{
			currentSprite = ( EdWidgetSprite* ) sprite;
		}
		currentFrame = 0;
		currentAction = 0;
	}
	else
	{
		currentSprite = 0;
		currentAction = 0;
		currentFrame = 0;
	}

// 	ClearInfo();
	SetInfoBox( true );
}


const gedString& SharedStateData::DefFileName()
{
	return defFileName;
}


const gedString& SharedStateData::SurfaceFileName()
{
	return surfaceFileName;
}


bool SharedStateData::LoadSurface( int nTrans, const KrRGBA* trans )
{
	const char* filename = SurfaceFileName().c_str();
	if ( filename && *filename )
	{
		// We are borrowing functionality from the encoder - 
		// it does all the surface loading processing 
		// that we would like to do.
		canvasResource = KrEncoder::Load32Canvas(	filename,
													trans,
													nTrans,
													0 );
		return canvasResource != 0;
	}

	#ifdef DEBUG
		GLOUTPUT( "Error in SharedStateData::LoadSurface\n" );
	#endif
	return false;
}


// void SharedStateData::AddTransparency( const KrRGBA& rgba )
// {
// 	if ( nTrans < MAX_TRANS )
// 	{
// 		trans[nTrans] = rgba;
// 		nTrans++;
// 	}
// }
// 

// bool SharedStateData::InitFromDef( const char* deffile )
// {
// // 	TiXmlDocument doc;
// // 	doc.SetValue( gedString( deffile ) );
// // 	bool success = doc.LoadFile();
// // 
// 	TiXmlElement*	element;
// 
// 	if ( success && doc.FirstChild( "Definition" ) )
// 	{
// 		// Set the surface name, if we have it.
// 		element = doc.FirstChildElement( "Definition" );
// 		GLASSERT( element );
// 		if ( element )
// 		{
// 			// Get the surface name.
// 			if ( element->Attribute( "filename" ) )
// 			{
// 				surfaceFileName = *( element->Attribute( "filename" ) );
// 			}
// 			else
// 			{
// 				console->Print( "No surface name in XML file.\n" );
// 				return false;
// 			}
// 
// 			// Get the transparency info.
// 			nTrans = 0;
// 			int i;
// 			for ( i=0; i<MAX_TRANS; i++ )
// 			{
// 				char buf[64];
// 				sprintf( buf, "Transparent%d", i );
// 				gedString attrib = buf;
// 
// 				if ( element->Attribute( attrib ) )
// 				{
// 					const gedString* colorString = element->Attribute( attrib );
// 					trans[ nTrans ].FromString( colorString->c_str() );
// 					nTrans++;
// 				}
// 			}
// 			#ifdef DEBUG
// 				GLOUTPUT( "Coverted to 32 bit color, nTrans=%d\n", nTrans );
// 			#endif
// 		}
// 	}
// 	return success;
// }
// 
// 
// bool SharedStateData::InitFromImage( const char* imagefile )
// {
// 	Clear();
// 
// 	char buf[ 256 ];
// 	char buf2[256];
// 
// 	sprintf( buf, "<Definition filename='%s'", imagefile );
// 	for( int i=0; i<nTrans; i++ )
// 	{
// 		sprintf( buf2, "Transparent%d='#%02x%02x%02x' ",
// 				 i,
// 				 trans[i].c.red,
// 				 trans[i].c.green,
// 				 trans[i].c.blue );
// 		strcat( buf, buf2 );
// 	}
// 	strcat( buf, "/>" );
// 
// 	gedString name( imagefile );
// 	GlString::SetExtension( &name, "xml" );
// 	spriteData->SetValue( gedString( name.c_str() ) );
// 
// 	return ( spriteData->Parse( buf ) != 0 );
// }

// This is a rather tricky function. The (human) sprite editor has
// created a documented describing a sprite action. We look at this
// document, extract the relevent information, and create KrRle objects
// that are frames of an action. These frames are assembled into an 
// action which is returned.
//

KrAction* SharedStateData::CreateAnimationAction()
{
	KrAction* action = 0;

	// If there is a current action, write the frame actions
	// and encode them.
	if ( currentAction )
	{
		int nFrames = currentAction->children.Count();
		int i = 0;
		if ( nFrames > 0 )
		{
			action = new KrAction( "no_action_name" );

			GlSListIterator< EdWidget* > it( currentAction->children );
			for( i = 0, it.Begin(); 
				 !it.Done(); 
				 ++i, it.Next() )
			{
				EdWidgetFrame* frame = it.Current()->ToFrame();
				GLASSERT( frame );
				KrDom::Frame frameData = frame->GetFrameData();

				KrPaintInfo info(	canvasResource->Pixels(), 
									canvasResource->Width(), 
									canvasResource->Height() );

				action->AddFrame();
				GLASSERT( action->NumFrames() == i+1 );
				action->GetFrame( i )->Create(	&info, 
												frameData.x,
												frameData.y,
												frameData.width,
												frameData.height,
												frameData.hotspotX,
												frameData.hotspotY,
												frameData.deltaX,
												frameData.deltaY );
			}			
		}
	}
	return action;
}

void SharedStateData::SetInfoBox( bool on )
{
	if ( on )
	{
		infoBoxBack->SetVisible( true );
	}
	else
	{
		infoBoxBack->SetVisible( false );
		return;
	}

	char buf[256];

	for( int i=INFO_CLEAR_START; i<=INFO_CLEAR_END; ++i )
	{
		infoBox->SetText16( 0, i );
	}

	if ( currentTile )
	{
		infoBox->SetTextChar( currentTile->Name().c_str(), INFO_TILE );
		sprintf( buf, "%d x %d", currentTile->Size(), currentTile->Size() );
		infoBox->SetTextChar( buf, INFO_TILE_SIZE );

		if ( currentTile->Size() == 0 )
			infoBox->SetTextChar( "Creating new tile", INFO_CREATING );
		else
			infoBox->SetText16( 0, INFO_CREATING );
	}
	else if ( currentFrame || currentAction || currentSprite )
	{
		if ( currentFrame )
		{
			// Which frame is this?
			int frameNum = 0;
			int frameCount = 0;
			GlSListIterator<EdWidget*> it( currentFrame->Parent()->children );

			for( it.Begin(); !it.Done(); it.Next() )
			{
				if ( it.Current() == currentFrame )
					frameNum = frameCount;
				++frameCount;
			}
			char buf[64];
			
			KrDom::Frame frameData;
			frameData = currentFrame->GetFrameData();

			//if ( frameData.isotile > 0 )
			//	sprintf( buf, "%d of %d Iso=%d", frameNum, frameCount, frameData.isotile );
			//else

			sprintf( buf, "%d of %d", frameNum, frameCount );
	
			infoBox->SetTextChar( buf, INFO_FRAME );
			infoBox->SetText16( 0, INFO_CREATING );

			sprintf( buf, "%dx%d hotspot %d,%d",
						  frameData.width, frameData.height,
						  frameData.hotspotX, frameData.hotspotY );
			infoBox->SetTextChar( buf, INFO_FRAME_SIZE );
		}

		if ( currentAction )
		{
			sprintf( buf, "%s", currentAction->Name().c_str() );

			infoBox->SetTextChar( buf, INFO_ACTION );

			if ( currentAction->children.Count() == 0 )
				infoBox->SetTextChar( "Creating new action.", INFO_CREATING );
		}			

		if ( currentSprite )
		{
			infoBox->SetTextChar( currentSprite->Name().c_str(),
								  INFO_SPRITE );
			if ( currentSprite->children.Count() == 0 )
				infoBox->SetTextChar( "Creating new sprite.", INFO_CREATING );
		}
	}
	else
	{
		infoBox->SetTextChar( "Info about selection", 0 );
		infoBox->SetTextChar( "is shown here.", 1 );
	}
}


void SharedStateData::SetInfoBoxMouse( int x, int y )
{
	char buf[64];
	sprintf( buf, "pos=(%d,%d)", x, y );
	infoBox->SetTextChar( buf, INFO_MOUSE );
}
