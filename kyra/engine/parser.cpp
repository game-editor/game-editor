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

#include "SDL.h"
#include "../tinyxml/tinyxml.h"
#include "parser.h"


void KrDom::ReadFrameAttributes( const TiXmlNode* node,
								 KrDom::Frame* frame )
{
	TiXmlElement* element = node->ToElement();
	
	memset( frame, 0, sizeof( Frame ) );

	if ( !element )
	{
		#ifdef DEBUG
			GLOUTPUT( "WARNING not an element in ReadFrameAttributes\n" );
			GLASSERT( 0 );
		#endif
		return;
	}

	if ( element->Value() != "Frame" )
	{
		#ifdef DEBUG
			GLOUTPUT( "WARNING not an element of type 'Frame' in ReadFrameAttributes\n" );
			GLASSERT( 0 );
		#endif
		return;
	}

	element->Attribute( "x", &frame->x );
	element->Attribute( "y", &frame->y );
	element->Attribute( "width",  &frame->width );
	element->Attribute( "height", &frame->height );

	frame->hasDelta = false;
	frame->hasHotspot = false;
	if ( element->Attribute( "hotspotx" ) && element->Attribute( "hotspoty" ) )
	{
		element->Attribute( "hotspotx", &frame->hotspotX );
		element->Attribute( "hotspoty", &frame->hotspotY );
		frame->hasHotspot = true;
	}
	if ( element->Attribute( "hotspotx" ) && element->Attribute( "hotspoty" ) )
	{
		element->Attribute( "deltax", &frame->deltaX );
		element->Attribute( "deltay", &frame->deltaY );
		frame->hasDelta = true;
	}
	if ( element->Attribute( "isotile" ) )
	{
		element->Attribute( "isotile", &frame->isotile );
	}
}


void KrDom::ReadTileAttributes(	const TiXmlNode* node,
								KrDom::Tile* frame )
{
	TiXmlElement* ele = node->ToElement();
	GLASSERT( ele );
	if ( !ele ) return;

	ele->Attribute( "x",    &frame->x );
	ele->Attribute( "y",    &frame->y );
	ele->Attribute( "size", &frame->size );
}


TiXmlNode* KrDom::ReadFrame( const TiXmlNode* action, int frameNumber )
{
	int i;
	TiXmlNode* frame;

	for ( i = 0, frame = action->FirstChild( "Frame" );
		  frame;
		  i++, frame = frame->NextSibling( "Frame" ) )
	{
		if ( i == frameNumber )
			return frame;
	}
	return 0;
}


int	KrDom::CountChildren( const TiXmlNode* node, const gedString& value )
{
	int i;
	TiXmlNode* child;

	for ( i = 0, child = node->FirstChild( value );
		  child;
		  i++, child = child->NextSibling( value ) )
	{}

	return i;
}


#endif //#ifndef STAND_ALONE_GAME //maks
