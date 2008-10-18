#ifndef SPRITEDOM_INCLUDED
#define SPRITEDOM_INCLUDED

#include "../tinyxml/tinyxml.h"
/*
class SpriteXmlElement : public TiXmlElement
{
  public:
	enum {
		SPRITE_ELEMENT = TiXmlNode::TYPECOUNT
	};

	SpriteXmlElement( TiXmlDocument* doc ) : TiXmlElement( doc ) {}

	int Type()	{ return SPRITE_ELEMENT; }

	SpriteXmlElement* FirstSprite()		{ return First( "Sprite" ); }
	SpriteXmlElement* SiblingSprite()	{ return Sibling( "Sprite" ); }

	SpriteXmlElement* FirstAction()		{ return First( "Action" ); }
	SpriteXmlElement* SiblingAction()	{ return Sibling( "Action" ); }

	SpriteXmlElement* FirstFrame()		{ return First( "Frame" ); }
	SpriteXmlElement* SiblingFrame()	{ return Sibling( "Frame" ); }

  private:
	SpriteXmlElement* First( const char* tagvalue );
	SpriteXmlElement* Sibling( const char* tagvalue );
};
*/
#endif