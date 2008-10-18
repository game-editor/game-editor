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

#include "button.h"
#include "eventmanager.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"
#include "../engine/textbox.h"


KrButton::KrButton( int _width, int _height, const KrScheme& s )
	: KrWidget( s ),
	  bevel( _width, _height, s )
{
	// Like all widgets, this is essentially a KrImNode that creates children
	// in its "addedToTree" method. Little to do here.
	width = _width;
	height = _height;
	mode = 0;

	KrRGBA grey;
	//KrColorTransform xform;

	grey		= scheme.primary;
	plateRes	= new KrBoxResource( "KrButton plate", width, height,	&grey,		1, KrBoxResource::FILL );
	textBox = 0;
	icon = 0;
}


KrButton::~KrButton()
{
	// Our children are deleted first, so this is safe.
	delete plateRes;
//	delete topLineRes;
//	delete bottomLineRes;
//	delete leftLineRes;
//	delete rightLineRes;
}


void KrButton::SetTextChar( const gedString& _text )
{
	if ( Engine() && scheme.font )
	{
		if ( textBox )
			Engine()->Tree()->DeleteNode( textBox );

		textBox = 0;
		text = _text;
		PlaceText();
	}
	else
	{
		text = _text;
	}
}


void KrButton::SetIcon( KrSprite* giveSprite )
{
	if ( Engine() )
	{
		if ( icon )
			Engine()->Tree()->DeleteNode( icon );

		icon = giveSprite;
		PlaceIcon();
	}
	else
	{
		icon = giveSprite;
	}
}


void KrButton::PlaceText()
{
	GLASSERT( Engine() );
	if ( scheme.font )
	{
		textBox = new KrTextBox( scheme.font, width, scheme.font->FontHeight(), 0, KrTextBox::CENTER );
		textBox->SetPos( 0, ( height - scheme.font->FontHeight() ) / 2 );
		textBox->SetText( text, 0 ); //maks
		Engine()->Tree()->AddNode( holder, textBox );
		textBox->SetZDepth( TEXT_DEPTH );
	}
}


void KrButton::PlaceIcon()
{
	GLASSERT( Engine() );

	if ( icon )
	{
		int width  = bevel.width - 4;	// Give some border space.
		int height = bevel.height - 4;

		if ( width > 0 && height > 0 )
		{
			KrRect bounds;
			icon->QueryBoundingBox( &bounds );

			GlFixed wScale = GlFixed( width ) / GlFixed( bounds.Width() );
			GlFixed hScale = GlFixed( height ) / GlFixed( bounds.Height() );
			GlFixed scale = GlMin( wScale, hScale );
			GLASSERT( scale > 0 );

			iconX = width - ( scale * bounds.Width() ).ToInt();
			iconX = iconX / 2 - ( scale * bounds.xmin ).ToInt() + 1;

			iconY = height - ( scale * bounds.Height() ).ToInt();
			iconY = iconY / 2 - ( scale * bounds.ymin ).ToInt() + 1;

			icon->SetScale( scale, scale );
			icon->SetPos( iconX, iconY );
			Engine()->Tree()->AddNode( holder, icon );
			icon->SetZDepth( ICON_DEPTH );
		}
	}
}


void KrButton::AddedtoTree()
{
	KrWidget::AddedtoTree();

	holder = new KrImNode();
	Engine()->Tree()->AddNode( this, holder );

	plate = new KrBox( plateRes );
	Engine()->Tree()->AddNode( holder, plate );

	bevel.AddToTree( Engine(), holder );
	bevel.DrawOut();
	KrEventManager::Instance()->AddListener( this );

	if ( scheme.font )
		PlaceText();
	if ( icon )
		PlaceIcon();
}


void KrButton::SetMode( int m )
{
	if (    Engine() 
		 && m != mode )
	{
		if ( ( m & DOWN ) && !( mode & DOWN ) )			// down transition
		{
			PublishEvent( ACTIVATED, 0, 0, 0, 0 );		
			if ( IsSelectable() )
			{
				mode = m;		// Prevent from recursion.
				KrEventManager::Instance()->SelectMe( this );
			}
		}
		else if ( !( m & DOWN ) && ( mode & DOWN ) )	// up transition
		{
			PublishEvent( DEACTIVATED, 0, 0, 0, 0 );
		}
		mode = m;

		KrColorTransform color;

		if ( mode == ( OVER ) )
		{
			color = scheme.CalcHiSec();
			bevel.DrawOut();
			if ( icon )
				icon->SetPos( iconX, iconY );
		}
		else if ( mode & DOWN )
		{		
			if ( mode & OVER )
				color = scheme.CalcDarkSec();
			else
				color = scheme.CalcDark();

			bevel.DrawIn();
			if ( icon )
				icon->SetPos( iconX+1, iconY+1 );
		}
		else
		{
			bevel.DrawOut();
			if ( icon )
				icon->SetPos( iconX, iconY );
		}

		plate->SetColor( color );
	}
}


///////////////////////////////////////////////////////
// PUSH


void KrPushButton::MouseIn( bool down, bool in )
{
	if ( in )
		SetMode(   ( down ? DOWN : 0 )
				 | ( in   ? OVER : 0 ) );
	else	
		SetMode( 0 );
}


void KrPushButton::MouseMove( bool down, int x, int y )
{}


void KrPushButton::MouseClick( int down, int x, int y )
{
	if(down == KrWidget::LEFT_DOWN || down == KrWidget::LEFT_UP) //maks
	{
		SetMode( down ? DOWN | OVER : OVER );
	}
}


void KrPushButton::Accelerate( bool down, int keymod, int keysym ) //maks
{
	int m = mode & OVER;
	if ( down )
		m |= DOWN;
	SetMode( m );
}


///////////////////////////////////////////////////////
// TOGGLE


void KrToggleButton::MouseIn( bool down, bool in )
{
	int m = mode & DOWN;

	if ( in )
		m |= OVER;

	SetMode( m );
}


void KrToggleButton::MouseMove( bool down, int x, int y )
{}


void KrToggleButton::MouseClick( int down, int x, int y )
{
	if ( down == KrWidget::LEFT_DOWN) //maks
	{
		int m = ( mode & DOWN ) ? 0 : DOWN;
		m |= OVER;

		SetMode( m );
	}
}


void KrToggleButton::Accelerate( bool down, int keymod, int keysym ) //maks
{
	if ( down )
	{
		int m = mode & OVER;
		int d = ( mode & DOWN );

		if ( !d )
			m |= DOWN;

		SetMode( m );
	}
}


void KrToggleButton::Selected( bool selected )
{
	int m = mode & OVER;
	m |= selected ? DOWN : 0;
	SetMode( m );
}

#endif //#ifndef STAND_ALONE_GAME //maks
