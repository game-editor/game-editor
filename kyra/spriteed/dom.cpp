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

#include "dom.h"
#include "../engine/color.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "../engine/imagetree.h"
#include "../engine/textbox.h"

#include "../tinyxml/tinyxml.h"
#include "../gui/console.h"


bool EdWidget::colorInit = false;
KrRGBA EdWidget::DRAGGING_COLOR[ DRAGGING_NCOLORS ];
KrRGBA EdWidget::HOTCROSS_COLOR[HOTCROSS_NCOLORS];
KrRGBA EdWidget::TILE_SELECTED_COLOR[TILE_SELECTED_NCOLORS];
KrRGBA EdWidget::TILE_NOT_SELECTED_COLOR[TILE_NOT_SELECTED_NCOLORS];
KrRGBA EdWidget::SPRITE_NOT_SELECTED_COLOR[SPRITE_NOT_SELECTED_NCOLORS];
KrRGBA EdWidget::SPRITE_SELECTED_COLOR[SPRITE_SELECTED_NCOLORS];


EdWidget::EdWidget( KrImageTree* _tree,
			        KrImNode* _krimNode,
			        EdWidget* _parent)
{
    krimNode = _krimNode;
	tree = _tree;
	parent = _parent;

	if ( !colorInit )
	{	
		DRAGGING_COLOR[0].Set( 255,   0,   0, 255 );
		DRAGGING_COLOR[1].Set( 255,   0,   0, 255 );
		DRAGGING_COLOR[2].Set( 0,   0,	255, 255  );
		DRAGGING_COLOR[3].Set( 0,   0,	255, 255  );

		HOTCROSS_COLOR[0].Set( 255, 100, 100, 255 );
		HOTCROSS_COLOR[1].Set( 200, 100, 100, 255 );

		TILE_SELECTED_COLOR[0].Set(0, 255,   0, 255 );
		TILE_SELECTED_COLOR[1].Set(0, 255,   0, 255 );
		TILE_SELECTED_COLOR[2].Set(255, 0,  0, 255 );
		TILE_SELECTED_COLOR[3].Set(255, 0,  0, 255 );

		TILE_NOT_SELECTED_COLOR[0].Set( 0, 200, 0 );
		TILE_NOT_SELECTED_COLOR[1].Set( 255, 0, 0 );

		SPRITE_NOT_SELECTED_COLOR[0].Set( 0, 200, 0 );
		SPRITE_NOT_SELECTED_COLOR[1].Set( 0, 0, 200 );

		SPRITE_SELECTED_COLOR[0].Set( 0, 255, 0 );
		SPRITE_SELECTED_COLOR[1].Set( 0, 255, 0 );
		SPRITE_SELECTED_COLOR[2].Set( 0, 0, 255 );
		SPRITE_SELECTED_COLOR[3].Set( 0, 0, 255 );
		colorInit = true;
	}
};


EdWidget::~EdWidget()
{
	while ( !children.Empty() )
	{
		delete children.Front();
 		children.PopFront();
	}	
}


EdWidget* EdWidget::FindWidget( KrBox* box, int flags )
{
	GlSListIterator<EdWidget*> it( children );
	EdWidget* returnWidget = 0;
	EdWidget* widget;

	Select( false );

	if ( IsThisYours( box ) )
	{
		GLASSERT( returnWidget == 0 );
		returnWidget = this;
		if ( flags & SELECT )
			returnWidget->Select( true );
	}

	for( it.Begin(); !it.Done(); it.Next() )
	{
		widget = it.Current()->FindWidget( box, flags );
		if ( widget )
		{
			GLASSERT( returnWidget == 0 );
			returnWidget = widget;
		}
	}
	return returnWidget;
}


void EdWidget::Save(	const gedString& xmlFilename,
						KrConsole* console,
						const gedString& surfaceName,
						int nTrans,
						const KrRGBA* trans )
{
	TiXmlDocument doc;
	doc.SetValue( xmlFilename );

	TiXmlElement definition( "Definition" );
	definition.SetAttribute( "filename", surfaceName.c_str() );

	for( int i=0; i<nTrans; i++ )
	{
		char key[256];
		char value[256];
		sprintf( key, "Transparent%d", i );
		sprintf( value, "#%02x%02x%02x", trans[i].c.red, trans[i].c.green, trans[i].c.blue );

		definition.SetAttribute( key, value );
	}

	GlSListIterator< EdWidget* > it( children );
	for( it.Begin(); !it.Done(); it.Next() )
		it.Current()->BuildXML( &definition );

	doc.InsertEndChild( definition );
	doc.SaveFile();
	if ( doc.Error() )
	{
		console->Print( "Error saving XML. '%s'\n", doc.ErrorDesc().c_str() );
	}
	else
	{
		console->Print( "XML file '%s' saved.\n", xmlFilename.c_str() );
	}
}


void EdWidget::BuildXML( TiXmlNode* parent )
{
	GLASSERT( 0 );	// shouldn't get called.
}


EdWidgetSprite::EdWidgetSprite(	KrImageTree* _tree,
								KrImNode* krimNode,			
								EdWidget* _parent,
								const gedString& _name )
	: EdWidget( _tree, krimNode, _parent )
{
	name = _name;
}


void EdWidgetSprite::BuildXML( TiXmlNode* parent )
{
	// Check to see if we are empty:
	if ( children.Count() == 0 )
		return;
	if ( children.Front()->children.Count() == 0 )
		return;

	TiXmlElement sprite( "Sprite" );
	sprite.SetAttribute( "name", name );
	
	GlSListIterator< EdWidget* > it( children );
	for( it.Begin(); !it.Done(); it.Next() )
		it.Current()->BuildXML( &sprite );

	parent->InsertEndChild( sprite );
}


EdWidgetAction::EdWidgetAction(	KrImageTree* _tree,
								KrImNode* krimNode,			
								EdWidget* _parent,
								const gedString& _name )
	: EdWidget( _tree, krimNode, _parent )
{
	name = _name;
}


void EdWidgetAction::BuildXML( TiXmlNode* parent )
{
	TiXmlElement action( "Action" );
	action.SetAttribute( "name", name );
	
	GlSListIterator< EdWidget* > it( children );
	for( it.Begin(); !it.Done(); it.Next() )
		it.Current()->BuildXML( &action );

	parent->InsertEndChild( action );
}


EdWidgetFrame::EdWidgetFrame(	KrImageTree* tree,
								KrImNode* krimNode,
								EdWidget* _parent,
								KrFontResource* fontRes,
								const KrDom::Frame& _frameData  )
	: EdWidget( tree, krimNode, _parent )
{
	frameData = _frameData;

	normalRes = new KrBoxResource(	"normal",
									frameData.width,
									frameData.height,
									SPRITE_NOT_SELECTED_COLOR,
									SPRITE_NOT_SELECTED_NCOLORS,
									0 );

	box = new KrBox( normalRes );
	box->SetPos( frameData.x, frameData.y );
	tree->AddNode( krimNode, box );

	// Add the cross for the hot spot:
	crossVRes = new KrBoxResource(	"crossV",
									1,
									CROSSSIZE,
									HOTCROSS_COLOR,
									HOTCROSS_NCOLORS,
									0 );
	crossHRes = new KrBoxResource(	"crossH",
									CROSSSIZE,
									1,
									HOTCROSS_COLOR,
									HOTCROSS_NCOLORS,
									0 );

	crossV = new KrBox( crossVRes );
	crossV->SetPos( frameData.hotspotX , 
					frameData.hotspotY - CROSSSIZE/2 );
	tree->AddNode( krimNode, crossV );


	crossH = new KrBox( crossHRes );
	crossH->SetPos( frameData.hotspotX - CROSSSIZE/2, 
					frameData.hotspotY  );
	tree->AddNode( krimNode, crossH );

	text = new KrTextBox( fontRes, 1024, 20, 0 );
	text->SetPos( frameData.x, frameData.y );
	tree->AddNode( krimNode, text );

	SetFrameNumber();
}


EdWidgetFrame::~EdWidgetFrame()
{
	tree->DeleteNode( box );
	tree->DeleteNode( crossV );
	tree->DeleteNode( crossH );
	tree->DeleteNode( text );
	box = 0;

	delete normalRes;
	delete crossVRes;
	delete crossHRes;
}


void EdWidgetFrame::Select( bool select )
{
	KrColorTransform color;

	if ( select )
	{
		color.Brighten( 200 );
		box->SetColor( color );
	}
	else
	{
		box->SetColor( color );
	}
}


void EdWidgetFrame::SetFrameData( const KrDom::Frame& data )
{
	GLASSERT( data.width > 0 );
	GLASSERT( data.height > 0 );

	KrColorTransform color = box->CTransform();
	frameData = data;

	tree->DeleteNode( box );
	delete normalRes;

	normalRes = new KrBoxResource(	"normal",
									frameData.width,
									frameData.height,
									SPRITE_NOT_SELECTED_COLOR,
									SPRITE_NOT_SELECTED_NCOLORS,
									0 );

	box = new KrBox( normalRes );
	box->SetPos( frameData.x, frameData.y );
	tree->AddNode( krimNode, box );
	box->SetColor( color );

	text->SetPos( frameData.x, frameData.y );
	SetFrameNumber();

	crossV->SetPos( frameData.hotspotX , 
					frameData.hotspotY - CROSSSIZE/2 );
	crossH->SetPos( frameData.hotspotX - CROSSSIZE/2, 
					frameData.hotspotY  );
}


void EdWidgetFrame::SetFrameNumber()
{
	int frameNum = 0;
	GlSListIterator< EdWidget* > it( parent->children );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( it.Current() == this )
			break;
		++frameNum;
	}

	char buf[64];
	sprintf( buf, "%d", frameNum );
	text->SetTextChar( buf, 0 );
}


void EdWidgetFrame::BuildXML( TiXmlNode* parent )
{
	TiXmlElement frame( "Frame" );
	frame.SetAttribute( "x", frameData.x );
	frame.SetAttribute( "y", frameData.y );
	frame.SetAttribute( "width", frameData.width );
	frame.SetAttribute( "height", frameData.height );
	frame.SetAttribute( "hotspotx", frameData.hotspotX );
	frame.SetAttribute( "hotspoty", frameData.hotspotY );
	frame.SetAttribute( "deltax", frameData.deltaX );
	frame.SetAttribute( "deltay", frameData.deltaY );
	
//	if ( frameData.isotile > 0 )
//		frame.SetAttribute( "isotile", frameData.isotile );

//	EdWidgetAction* parentAction = this->parent->ToAction();
//	if ( parentAction && parentAction->IsoTile() )
//	{
//		frame.SetAttribute( "isotile", parentAction->IsoTile() );
//	}
	
	parent->InsertEndChild( frame );
}


EdWidgetTile::EdWidgetTile(	KrImageTree* tree, KrImNode* krimNode,
							EdWidget* _parent,
							const gedString& _name,
							int _x, int _y,
							int _size )
	: EdWidget( tree, krimNode, _parent )
{
	x = _x;
	y = _y;
	size = _size;
	name = _name;

	normalRes = new KrBoxResource(	"normal",
									size,
									size,
									TILE_NOT_SELECTED_COLOR,
									TILE_NOT_SELECTED_NCOLORS,
									0 );
	box = new KrBox( normalRes );
	box->SetPos( x, y );
	tree->AddNode( krimNode, box );
}


EdWidgetTile::~EdWidgetTile()
{
	tree->DeleteNode( box );
	box = 0;

	delete normalRes;
}


void EdWidgetTile::Set( int _x, int _y, int _size )
{
	if ( x != _x || y != _y || size != _size )
	{
		x = _x;
		y = _y;
		size = _size;

		KrColorTransform color = box->CTransform();

		tree->DeleteNode( box );
		delete normalRes;

		normalRes = new KrBoxResource(	"normal",
										size,
										size,
										TILE_NOT_SELECTED_COLOR,
										TILE_NOT_SELECTED_NCOLORS,
										0 );
		box = new KrBox( normalRes );
		box->SetPos( x, y );
		tree->AddNode( krimNode, box );
		box->SetColor( color );
	}
}


void EdWidgetTile::Select( bool select )
{
	KrColorTransform color;

	if ( select )
	{
		color.Brighten( 200 );
		box->SetColor( color );
	}
	else
	{
		box->SetColor( color );
	}
}


void EdWidgetTile::BuildXML( TiXmlNode* parent )
{
	if ( size > 0 )
	{
		TiXmlElement tile( "Tile" );
		tile.SetAttribute( "name", name );
		tile.SetAttribute( "x", x );	
		tile.SetAttribute( "y", y );	
		tile.SetAttribute( "size", size );	

		parent->InsertEndChild( tile );
	}
}
