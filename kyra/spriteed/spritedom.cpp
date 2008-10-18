#include "spritedom.h"
#include "../engine/debug.h"

/*
SpriteXmlElement* SpriteXmlElement::First( const char* tagvalue )
{
	TiXmlNode* node = 0;

	for( node = FirstChild(); node; node = node->NextSibling() )
	{
		if ( node->Value() == tagvalue )
		{
			if ( node->Type() == SPRITE_ELEMENT )
				return (SpriteXmlElement*) node;
			IOASSERT( 0 );	// it should be a sprite element
		}
	}
	return 0;
}


SpriteXmlElement* SpriteXmlElement::Sibling( const char* tagvalue )
{
	TiXmlNode* node = 0;

	for( node = NextSibling(); node; node = node->NextSibling() )
	{
		if ( node->Value() == tagvalue )
		{
			if ( node->Type() == SPRITE_ELEMENT )
				return (SpriteXmlElement*) node;
			IOASSERT( 0 );	// it should be a sprite element
		}
	}
	return 0;
}
*/

