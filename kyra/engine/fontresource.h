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


#ifndef KYRA_FONTRESOURCE_INCLUDED
#define KYRA_FONTRESOURCE_INCLUDED

#include "spriteresource.h"
#include "../util/gllist.h" //maks


/** A font is what text is created from.

	A font resource is a child of the sprite resource, because a Font
	Resource is a multi-frame Sprite Resource with some 
	additional capability. (On the other hand, a Font
	is not a Sprite.)

	That said, the Sprite API is almost certainly confusing. When
	using KrFontResource, you should probably focus on the
	KrFontResource class rather than the parent KrSpriteResource.
*/


typedef GlMap< U16, U16, GlNumberHash<U16> >		 MapGlyphFrame; //maks

typedef GlSList<U16> ListGlyph;   //maks
typedef GlSListIterator<U16> ListGlyphIterator; //maks


class KrFontResource : public KrSpriteResource
{
  public:
	int getFontType() {return fontType;} //maks
	// Create by reading from a .dat file
	KrFontResource( U32 size, SDL_RWops* data );

#ifndef STAND_ALONE_GAME //maks
	KrFontResource( const gedString& name,
					KrPaintInfo* info,
					int startingGlyph,
					int addSpaceGlyph,
					int type,
					int numGlyphs );	// only need for fixed fonts
#endif

	virtual ~KrFontResource();

	virtual	U32 Type()								{ return KYRATAG_FONT; }
	virtual const gedString&	TypeName()			{ return fontName; }
	virtual KrFontResource* ToFontResource()		{ return this; }

	enum {
		FIXED,
		SFONT
	};

	/// Return the height of this font.
	int FontHeight()	{ return actionArr[0]->Frame( 0 ).Delta().y; }
	bool FontOk() {return actionArr[0]->NumFrames() > 0;} //maks
	int NumberOfChars() {return actionArr[0]->NumFrames();} //maks
	int InitialChar() {return startIndex;} //maks

	int FontWidth( const char* str ); //maks
	int FontWidth1( U16 glyphCode );	///< Return the width of a single glyph.
	int FontWidth( const U16* str );	///< Return the width of a string.
	int FontWidthN( const U16* str, int nChars );	///< Return the width of the string up to nChars characters.
	int FontWidthN( const char* str, int n ); //maks
	
	/** Return true if the given glyphcode is in this font set.
		A space is not in the font, but it will have a width.
	*/
	bool GlyphInFont( U16 glyphCode, bool bReturnSpaceIfNotFound = true ); //maks
	int  GlyphToFrame(  U16 glyphCode );

	/*  Draw a character in the font.
		@param  paintInfo	Information about the target surface for drawing (optimizing).
		@param	glyphCode	The glyph code: ascii, latin-1, whatever
		@param	x			X location in pixels.
		@param	y			Y location in pixels.
		@param  cForm		Color transformation applied to the drawing.
		@param	clip		A clipping rectangle, which can be null.
	*/
	void Draw( KrPaintInfo* paintInfo,
			   U16 glyphCode,
			   const KrMatrix2& matrix,
			   const KrColorTransform& cForm,
			   const KrRect& clipping,
			   int openGLZ );

#ifndef STAND_ALONE_GAME //maks
	virtual void Save( KrEncoder* encoder );
#endif

  private:
	// The font (and text) somewhat abuses the sprite system. Fonts
	// are single action sprites, with a number of frames equal to
	// the length of the font. The delta is interpreted as the 
	// bounding box of the letter.

	void CalcSpaceWidth();

	const static gedString fontName;

	U32			startIndex;
	U32			space;
	int			spaceWidth;
	int			fontType;
//	KrAction*	action;		// A font is only one action.

	
	MapGlyphFrame	mapGlyphFrame; //maks: mapGlyphFrame[unicode] = frame; 
	ListGlyph		listGlyph; //Use for make sure the glyph order on save

	void AddGlyphCode(U16 glyphCode, int frame); //maks
	KrRle *GetGlyph(U16 glyphCode, bool bReturnSpaceIfNotFound = true); //maks

#ifndef STAND_ALONE_GAME //maks
	SDL_Surface *CreateGlyphSurface(U16 glyphCode); //maks
#endif
};


#endif
