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

#include "SDL.h"
#include "SDL_endian.h"
#include "fontresource.h"
#include "engine.h"
#include "encoder.h"
#include "sdlutil.h"


const gedString KrFontResource::fontName = "Font";

//maks: size 13 is compatible with current editor font. 16 is better for Japanese symbols
gedString defaultTTFFont("msgothic.ttf0000132552552550"); 

KrFontResource::KrFontResource( U32 size, 
								SDL_RWops* data )

	: KrSpriteResource( size, data )
{
	// After the sprite has initialized, read our startIndex and type.
	
	startIndex = SDL_ReadLE32( data );
	fontType   = SDL_ReadLE16( data );
	GLASSERT( fontType == FIXED || fontType == SFONT );
	space      = SDL_ReadLE32( data );

	#ifdef DEBUG
		GLOUTPUT( "startindex=%d type=%d\n", startIndex, fontType );
	#endif
	
	
//	actionArr = GetActionByIndex( 0 );
//	GLASSERT( action );

	KrAction *action = actionArr[0];


	if(KrResourceVault::getVersionDat() >= 3) //maks
	{		
		for(int i = 0; i < action->NumFrames(); ++i)
		{
			U16 glyphCode = SDL_ReadLE16( data );			
			AddGlyphCode(glyphCode, i);			
		}
	}
	else
	{
		for(int i = 0; i < action->NumFrames(); ++i)
		{
			AddGlyphCode(i + startIndex, i);
		}
	}

	
	CalcSpaceWidth();
	
}

#ifndef STAND_ALONE_GAME //maks
KrFontResource::KrFontResource( const gedString& name,
								KrPaintInfo* info,
								int startingGlyph,
								int addSpaceGlyph,
								int type,
								int length )
	: KrSpriteResource( name )
{
	startIndex = startingGlyph;
	fontType   = type;
	space      = addSpaceGlyph;

	KrAction* action = new KrAction( sNONE );

	if ( fontType == FIXED )
	{
		int width  = info->width / length;
		int height = info->height;

		for( int i = 0; i < length; i++ )
		{
			action->AddFrame();
			KrRle* rle = action->GetFrame( i );
			AddGlyphCode(i + startIndex, i);

			rle->Create(	info,
							i * width, 0, width, height,
							i * width, 0,	// the hotspots are absolute coordinates!
							width, height );
		}
	}
	else
	{
		KrPainter painter( info );
		int height = info->height - 1;
		int x = 0;
		int transparent = 0;

		while ( x < info->width )
		{
			x += painter.CalcNotTransparentRun( x, info->width - 1, 0 );
			if ( x < info->width )
				transparent = painter.CalcTransparentRun( x, info->width - 1, 0 );
			else
				transparent = 0;

			if ( x < info->width && transparent > 0 )
			{
				int iFrame = action->NumFrames();
				action->AddFrame();
				KrRle* rle = action->GetFrame( iFrame );
				AddGlyphCode(iFrame + startIndex, iFrame);

 				rle->Create(	info,
								x, 1, transparent, height,
								x, 1,	// the hotspots are absolute coordinates!
								transparent, height );
			}
			x += transparent;
		}
	}

	AddAction( action );

	if(action->NumFrames())
	{
		CalcSpaceWidth();
	}
}
#endif //#ifndef STAND_ALONE_GAME //maks


KrFontResource::~KrFontResource()
{
}

void KrFontResource::AddGlyphCode(U16 glyphCode, int frame) //maks
{
	mapGlyphFrame.Add(glyphCode, frame);
	listGlyph.PushBack(glyphCode);	
}


#ifndef STAND_ALONE_GAME //maks
SDL_Surface *KrFontResource::CreateGlyphSurface(U16 glyphCode) //maks
{
	//Create s surface if font is TTF

	SDL_Surface *glyphSurface = NULL;

	//Adjust ttf font
	gedString fileName(ResourceName());

	if(fileName == "maksfont.bmp")
	{
		//Extend editor font
		fileName = defaultTTFFont;
	}

	if( fileName.substr(fileName.length() - (16 + 4), 4) == ".ttf" || 
		fileName.substr(fileName.length() - (16 + 4), 4) == ".TTF")
	{
		int i_style, i_size, ui_r, ui_g, ui_b;
		bool i_antialias;
		gedString fontName(fileName);
		fileName = fileName.substr(0, fileName.length() - 16);

		i_style	= atoi(fontName.substr(fontName.length() - 16, 3).c_str());
		i_size	= atoi(fontName.substr(fontName.length() - 13, 3).c_str());
		ui_r	= atoi(fontName.substr(fontName.length() - 10, 3).c_str());
		ui_g	= atoi(fontName.substr(fontName.length() - 7, 3).c_str());
		ui_b	= atoi(fontName.substr(fontName.length() - 4, 3).c_str());
		i_antialias	= atoi(fontName.substr(fontName.length() - 1, 1).c_str()) != 0;
		
		SDL_Surface* CreateUnicodeGlyphSurface (const char* str_ttf, U16 glyphCode, int i_style, int i_size, Uint8 ui_r, Uint8 ui_g, Uint8 ui_b, bool bAntiAlias);
		glyphSurface = CreateUnicodeGlyphSurface(fileName.c_str(), glyphCode, i_style, i_size, ui_r, ui_g, ui_b, i_antialias);

		//Solve the bug when load Fraunhofer/CH/CH.ged
		SDL_ClearError();
	}

	return glyphSurface;
}
#endif

KrRle *KrFontResource::GetGlyph(U16 glyphCode, bool bReturnSpaceIfNotFound) //maks
{
	//Find or create a glyph

	U16 *frame = mapGlyphFrame.Find1(glyphCode);
	if(!frame)
	{

#ifndef STAND_ALONE_GAME //maks
		//Try create, if in editor

		if(glyphCode == '\n' || glyphCode == '\r' || glyphCode == '\t')
		{
			return NULL;
		}

		
		SDL_Surface *surface = CreateGlyphSurface(glyphCode);
		if(surface)
		{		
			KrPaintInfo info( surface );			
			
			if(surface->format->BitsPerPixel < 32)
			{
				SDL_Surface* ToSupportedSurface(SDL_Surface* surface, bool bRemoveOld = true);
				surface = ToSupportedSurface(surface);

				info.Init(surface);
				
				//Set transparent color
				KrPainter painter( surface );
				KrRGBA transRgba;
				painter.BreakPixel( 0, 0, &transRgba );	
				info.SetTranparentColor(transRgba);
			}
			
			//Paint info Ok
			KrPainter painter( &info );
			
			int iFrame = actionArr[0]->NumFrames();
			actionArr[0]->AddFrame();
			KrRle *newRle = actionArr[0]->GetFrame( iFrame );
			
			AddGlyphCode(glyphCode, iFrame);
			
			newRle->Create(	&info,
				0, 0, info.width, info.height,
				1, 1,	// the hotspots are absolute coordinates!
				info.width, info.height);
			
			SDL_FreeSurface( surface );
			return newRle;
		}
#endif

		//Not found, return the space glyph
		frame = mapGlyphFrame.Find1(' ');
		if(frame && bReturnSpaceIfNotFound)
		{
			AddGlyphCode(glyphCode, *frame);
		}
		else
		{
			return NULL;
		}
	}

	return actionArr[0]->GetFrame( *frame );
}

void KrFontResource::CalcSpaceWidth()
{
	int total = 0;
	for( int i=0; i<actionArr[0]->NumFrames(); ++i )
		total += actionArr[0]->Frame( i ).Delta().x;
	
	spaceWidth = total / actionArr[0]->NumFrames();
	spaceWidth = GlMax( 1, spaceWidth );
}


int KrFontResource::FontWidth( const U16* str )
{
	// Walk the string, check to make sure each character is in
	// the font, and add up the widths.
	const U16* p;
	int width = 0;
	//int glyph;

	for( p=str; p && *p; ++p )
	{
		if ( *p == (U16)space )
		{
			width += spaceWidth;
		}
		else //maks
		{
			/*glyph = (*p) - startIndex;
			if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
			{
				width += actionArr[0]->Frame( glyph ).Delta().x;
			}*/

			KrRle *rle = GetGlyph(*p);
			if(rle)
			{
				width += rle->Delta().x;
			}
			else width += spaceWidth;

		}
	}

	return width;
}


int KrFontResource::FontWidthN( const U16* str, int n )
{
	// Walk the string, check to make sure each character is in
	// the font, and add up the widths.
	const U16* p;
	int width = 0;
	//int glyph;
	int i;

	for( p=str, i=0; p && *p && i < n; ++p, ++i )
	{
		if ( *p == space )
		{
			width += spaceWidth;
		}
		else //maks
		{
			/*glyph = (*p) - startIndex;
			if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
			{
				width += actionArr[0]->Frame( glyph ).Delta().x;
			}*/

			KrRle *rle = GetGlyph(*p);
			if(rle)
			{
				width += rle->Delta().x;
			}
			else width += spaceWidth;
		}
	}

	return width;
}

int KrFontResource::FontWidthN( const char* str, int n ) //maks
{
	// Walk the string, check to make sure each character is in
	// the font, and add up the widths.
	const char* p;
	int width = 0;
	//int glyph;
	int i;

	for( p=str, i=0; p && *p && i < n; ++p, ++i )
	{
		if ( *p == space )
		{
			width += spaceWidth;
		}
		else //maks
		{
			/*glyph = (*p) - startIndex;
			if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
			{
				width += actionArr[0]->Frame( glyph ).Delta().x;
			}*/

			KrRle *rle = GetGlyph(*p);
			if(rle)
			{
				width += rle->Delta().x;
			}
			else width += spaceWidth;
		}
	}

	return width;
}

int KrFontResource::FontWidth( const char* str ) //maks
{
	// Walk the string, check to make sure each character is in
	// the font, and add up the widths.
	const char* p;
	int width = 0;
	//int glyph;

	for( p=str; p && *p; ++p )
	{
		if ( (U16)*p == space )
		{
			width += spaceWidth;
		}
		else //maks
		{
			/*glyph = (*p) - startIndex;
			if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
			{
				width += actionArr[0]->Frame( glyph ).Delta().x;
			}*/

			KrRle *rle = GetGlyph(*p);
			if(rle)
			{
				width += rle->Delta().x;
			}
			else width += spaceWidth;
		}
	}

	return width;
}

int KrFontResource::FontWidth1( U16 glyphCode ) //maks
{
	if ( glyphCode == space )
	{
		return spaceWidth;
	}

	/*int glyph = glyphCode - startIndex;
	if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
	{
		return actionArr[0]->Frame( glyph ).Delta().x;
	}*/

	KrRle *rle = GetGlyph(glyphCode);
	if(rle)
	{
		return rle->Delta().x;
	}

	return spaceWidth;
}


bool KrFontResource::GlyphInFont( U16 glyphCode, bool bReturnSpaceIfNotFound ) //maks
{
	/*int glyph = glyphCode - startIndex;
	if ( glyph >= 0 && glyph < actionArr[0]->NumFrames() )
	{
		return true;
	}*/

	KrRle *rle = GetGlyph(glyphCode, bReturnSpaceIfNotFound);
	if(rle)
	{
		return true;
	}

#ifdef DEBUG
	GLOUTPUT( "Glyph Code=%X not found\n", glyphCode );
#endif
	return false;	
}


int  KrFontResource::GlyphToFrame(  U16 glyphCode ) //maks
{
	/*int glyph = glyphCode - startIndex;
	GLASSERT( glyph >= 0 && glyph < actionArr[0]->NumFrames() );
	return glyph;*/

	U16 *frame = mapGlyphFrame.Find1(glyphCode);
	if(frame)
	{
		return *frame;
	}

	return 0;
}


void KrFontResource::Draw( KrPaintInfo* paintInfo,
						   U16 rawGlyphCode,
						   const KrMatrix2& matrix,
						   const KrColorTransform& cForm,
						   const KrRect& clipping,
						   int openGLZ ) //maks
{
	#ifdef ASSERT_IF_NOT_CACHED
		GLASSERT( !matrix.IsScaled() );
	#endif

	if ( rawGlyphCode == space )
		return;

	/*int glyph = rawGlyphCode - startIndex;

	if ( glyph >=0 && glyph < actionArr[0]->NumFrames() )
	{
		actionArr[0]->Draw(	paintInfo,
							glyph,
							matrix,
							cForm,
							clipping,
							openGLZ );
	}*/

	U16 *frame = mapGlyphFrame.Find1(rawGlyphCode);
	if(frame)
	{
		actionArr[0]->Draw(	paintInfo,
							*frame,
							matrix,
							cForm,
							clipping,
							openGLZ );
	}
}

#ifndef STAND_ALONE_GAME //maks
void KrFontResource::Save( KrEncoder* encoder )
{
	//maks: don't use indexed fonts (slow down in the GP2X blitter when uses)

	encoder->StartTag( KYRATAG_FONT );
	
	WriteString( encoder->Stream(), ResourceName() );
	encoder->WriteCached( ResourceName() );

	SDL_WriteLE32( encoder->Stream(), NumActions() );

	int i;
	for( i=0; i<NumActions(); ++i )
	{
		actionArr[i]->Save( encoder );
	}
	SDL_WriteLE32( encoder->Stream(), startIndex );
	SDL_WriteLE16( encoder->Stream(), fontType );
	SDL_WriteLE32( encoder->Stream(), space );

	ListGlyphIterator it(listGlyph); //maks
	for(it.Begin(); !it.Done(); it.Next())
	{
		SDL_WriteLE16( encoder->Stream(), it.Current() );
	}


	encoder->EndTag();
}
#endif

