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



#include "widget.h"
#include "../engine/engine.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "eventmanager.h"


KrScheme::KrScheme( KrFontResource* _font )
{
	primary.Set( 170, 170, 170 );
	cursor.Set( 255, 255, 255 );

	/*secondary.Set(	200, 0, 
					255, 0, 
					200, 0, 
					255 );*/

	secondary.Set(	255, 0, //maks: no edit highlights 
					255, 0, 
					255, 0, 
					255 );
	font = _font;
//	bright = 60;
//	dark   = 60;
}


KrRGBA KrScheme::CalcBrightLine() const
{
	KrRGBA lightGrey = primary;

	KrColorTransform xform;
	xform.Brighten( BRIGHT );
	xform.ApplyTransform( &lightGrey );

	return lightGrey;		
}


KrRGBA KrScheme::CalcShadowLine() const
{
	KrRGBA darkGrey = primary;

	KrColorTransform xform;
	xform.Darken( DARK );
	xform.ApplyTransform( &darkGrey );

	return darkGrey;		
}


KrColorTransform KrScheme::CalcHiPrimary() const
{
	KrColorTransform light;
	light.Brighten( BRIGHT );
	return light;
}


KrColorTransform KrScheme::CalcHiSec() const
{
	return secondary;
}


KrColorTransform KrScheme::CalcDark() const
{
	KrColorTransform dark;
	dark.Darken( DARK );
	return dark;	
}


KrColorTransform KrScheme::CalcDarkSec() const
{
	KrColorTransform xform = secondary;
	KrColorTransform dark;
	dark.Darken( DARK );

	xform.Composite( dark );
	return xform;	
}


KrBevelElement::KrBevelElement( int w, int h, const KrScheme& scheme, void *_parentActor)
{
	width = w;
	height = h;
	
	horD = horL = vertD = vertL = 0;

	KrRGBA light = scheme.CalcBrightLine();
	KrRGBA dark  = scheme.CalcShadowLine();

	horDR	= new KrBoxResource( "KrBevelElement",	width, 1,	&dark, 1,  KrBoxResource::FILL );
	horLR	= new KrBoxResource( "KrBevelElement",  width, 1,	&light,	1, KrBoxResource::FILL );
	vertDR	= new KrBoxResource( "KrBevelElement",	1, height,	&dark, 1,  KrBoxResource::FILL );
	vertLR	= new KrBoxResource( "KrBevelElement",	1, height,	&light,	1, KrBoxResource::FILL );

	parentActor = _parentActor;
};


KrBevelElement::~KrBevelElement()
{
	// Children should be deleted from the tree.
	delete horDR;
	delete horLR;
	delete vertDR;
	delete vertLR;
}


void KrBevelElement::AddToTree( KrEngine* e, KrImNode* p )
{
	horD = new KrBox( horDR );
	e->Tree()->AddNode( p, horD );
	horL = new KrBox( horLR );
	e->Tree()->AddNode( p, horL );
	vertD = new KrBox( vertDR );
	e->Tree()->AddNode( p, vertD );
	vertL = new KrBox( vertLR );
	e->Tree()->AddNode( p, vertL );

	//maks
	horD->setParentActor(parentActor);
	horL->setParentActor(parentActor);
	vertD->setParentActor(parentActor);
	vertL->setParentActor(parentActor);
}


void KrBevelElement::DrawIn()
{
	horD->SetPos( 0, 0 );
	horL->SetPos( 0, height-1 );
	vertD->SetPos( 0, 0 );
	vertL->SetPos( width-1, 0 );
}

void KrBevelElement::DrawOut()
{
	horD->SetPos( 0, height-1 );
	horL->SetPos( 0, 0 );
	vertD->SetPos( width-1, 0 );
	vertL->SetPos( 0, 0 );
}


KrWidget::KrWidget( const KrScheme& _scheme ) : scheme( _scheme )
{
	groupId = 0;
}


KrWidget::~KrWidget()
{
	KrEventManager::Instance()->RemoveListener( this );
}


void KrWidget::AddListener( IKrWidgetListener* listener )
{
	if ( eventList.Find( listener ) == eventList.NotFound() )
		eventList.PushBack( listener );
}


void KrWidget::RemoveListener( IKrWidgetListener* listener )
{
	// Don't actually change the list, in case this is recursively
	// called! The Publish can clean the list of extras later.

	unsigned i = eventList.Find( listener );
	if ( i != eventList.NotFound() )
		eventList[ i ] = 0;
}


void KrWidget::PublishEvent( U32 event, U32 data, const SDL_Event* sdlEvent, const char* command, const char* arg )
{
	bool foundNull = false;

	for( unsigned currentNode = 0; currentNode < eventList.Count(); ++currentNode )
	{
		if ( eventList[ currentNode ] )
		{
			// Note: this may decrement currentNode if we self-delete
			eventList[currentNode]->HandleWidgetEvent( this, event, data, sdlEvent, command, arg );
		}
		else
		{
			foundNull = true;
		}
	}

	if ( foundNull )
	{
		unsigned i=0;
		while ( i < eventList.Count() )
		{
			if ( eventList[i] )
				++i;
			else
				eventList.Remove( i );
		}
	}
}


void KrWidget::PublishTaggedEvent( U32 event, const SDL_Event* sdlEvent, const char* command, const char* arg, IKrWidgetListener* special )
{

	for( unsigned currentNode = 0; currentNode < eventList.Count(); ++currentNode )
	{
		if ( eventList[ currentNode ] )
		{
			// Note: this may decrement currentNode if we self-delete
			eventList[currentNode]->HandleWidgetEvent(	this,
														event, 
														special == eventList[currentNode] ? 1 : 0,
														sdlEvent, 
														command, arg );
		}
	}
}


void KrWidget::SetAccelerator( int keymod, int keysym )
{
	KrEventManager::Instance()->SetAccelerator( keymod, keysym, this );
}


KrWidget* KrWidget::ParentWidget()
{
	KrImNode* node;
	for( node = this->Parent(); node; node=node->Parent() )
	{
		if ( node->ToWidget() )
			return node->ToWidget();
	}
	return 0;
}



