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

#ifndef ENCODER_INCLUDED
#define ENCODER_INCLUDED

#include "SDL.h"
#include "../tinyxml/tinyxml.h"
#include "../util/gltypes.h"
#include "../engine/color.h"
#include "../util/gllist.h"
#include "kyraresource.h"
#include "namefield.h"
#include "vault.h"


class KrRle;
class KrCanvasResource;
class KrPixelBlock;
class KrCachedWrite;
class KrConsole;
class KrEngine;


typedef SDL_Surface* (*ImageLoaderFunc)( const char* );


class KrEncoder
{
  public:
	KrEncoder( SDL_RWops* stream, bool _bSaveCompressed); //maks

	// Writes the header of the .dat file.
	bool StartDat(const char* magic = "KYRA"); //maks

	/*	Process an entire doc, write the result of the
		encoding to stream. Normally called between a
		'StartDat' and 'EndDat'. If the 'screen' is non-0
		then the surface will be displayed as we go.
	*/
	bool ProcessDoc( const TiXmlDocument& doc, 
					 KrEngine*  engine,
					 KrConsole* output );

	// Writes the tail of the .dat and completes the header.
	bool EndDat();

	void WriteHeader( const char* name, 
					  FILE* stream,
					  const char* prefix );

	bool getCompressed() {return bSaveCompressed;}

	/*  A utility function that loads a surface. Will be converted
		to 32 bit regardless of the source format.

		@param filename		The filename of the surface to load.
		@param transparent	An array of colors that will be interpreted	
							as transparent. (Only used for surfaces
							less than 32 bit.)
		@param nTransparent	The number of entries in the transparent array.
		@param error		If an error occurs, the despcription will go here.
	*/
	enum
	{
		RGBA,
		UpperLeft,
		LowerLeft,
		UpperRight,
		LowerRight
	};

	struct Transparent
	{
		int type;			// one of the enums, above.
		KrRGBA rgba;		// if RGBA, value is here.
	};

	/*	Very, very strange limitation. It is important to not link the engine
		library with SDL_Image...because it doesn't use it. But static linking
		is confusing the linker, so it looks for the unused IMG_Load function.
		If a tool wishes to make that function available, its address is passed
		here.
	*/
	static void SetImageLoader( ImageLoaderFunc i )		{ ImageLoader = i; }

	static SDL_Surface* Load32Surface(	const char* filename,
										Transparent* transparent,
										int nTransparent,
										gedString* error );

	/*  Exactly the same as Load32Surface, except that it loads to 
		a canvas. A transition function on the road to wrapping
		the video layer.
	*/
	static KrCanvasResource* Load32Canvas(	const char* filename,
											const KrRGBA* transparent,
											int nTransparent,
											gedString* error );

	/*  Useful, if odd, function to create a fixed font
		from a BMP file in a buffer. It assumes
		ascii: start 32, length 95.
	*/
	static KrFontResource* CreateFixedFontResource(	const char* resourceName,
													const U8* buffer,
													int bufferSize );
													

	// Used by the tags to Save() themselves.
	void StartTag( U32 tag, bool _bCompressThisResource = true );
	void EndTag();
	void WriteCached( const gedString& name )		{ static int id = 0; SDL_WriteLE32( stream, id++ );/*cachedWrite.Write( name );*/ } //maks
	SDL_RWops* Stream()								{ return stream; }
	void AddCount( U32 _numLines, U32 _numSegments, U32 _numRGBA )						
													{	
														GLASSERT( _numRGBA >= _numSegments );
//														GLASSERT( _numSegments >= _numLines );
														numLine		+= _numLines;
														numSegment	+= _numSegments;
														numRGBA		+= _numRGBA;
														GLASSERT( numRGBA >= numSegment );
//														GLASSERT( numSegment >= numLine );
													 }
	void KrEncoder::Save();

	KrResourceVault* GetVault()						{ return &vault; }

  private:
	static ImageLoaderFunc ImageLoader;
	bool bSaveCompressed, bCompressThisResource; //maks

//	void InitOutput( SDL_Surface* screen );

	enum
	{
		TYPE_NONE, 
		TYPE_SPRITE, 
		TYPE_TILE, 
		TYPE_FONT
	};

	enum
	{
		SUBTYPE_FIXEDFONT,		// The default value.
		SUBTYPE_SFONT,
	};

	struct AllInfo
	{
		AllInfo() :	type( 0 ),
					subType( 0 ),
					name( "NONE" ), action( "NONE" ), 
					useEntireImage( false ),
					frameCount( 0 ),
					x( 0 ), y( 0 ), 
					width( 0 ), height( 0 ), 
					//hasHotspot( false ),
					hotx( 0 ), hoty( 0 ),
					//hasDelta( false ),
					deltax( 0 ), deltay( 0 ),
					isoTargetWidth( 0 ),
					fontStart( 0 ),
					fontLength( 0 ),
					space( 0 )			{ keyColor.Set( 0, 0, 0, 0 ); }

		int			type;				// SPRITE, TILE, ...
		int			subType;
		//gedString filename;			// The filename that goes with this.
		gedString name;				// Sprite, Tile, Font, etc.
		gedString action;
		//int			frame;			// Automatically calculated
		bool		useEntireImage;		// If true, width, height, x, y don't matter
		int			frameCount;			// Limit for # frames to be read by color key
		int			x, y;
		int			width, height;		// Width and Height set for tiles
		int			hotx, hoty;
		int			deltax, deltay;
		int			isoTargetWidth;
		int			fontStart;			// Glyph number that starts the font
		int			fontLength;
		int			space;				// Space glyph number for a font
		KrRGBA		keyColor;			// Color key color, if used.
		GlDynArray< int > rotation;		// All the rotation angles specified.
	};
	
	void CalcAllInfo( TiXmlNode* node, AllInfo* info, SDL_Surface* );

	void CreateIsoTile(	KrPaintInfo* info, 
						KrConsole* console,
						int x, int y,
						int width, int height,
						KrRle* rle,
						int isoIdealWidth,
						int rotation );

	void IsoToSource( GlFixed x, GlFixed y, GlFixed isoWidth, 
					  GlFixed sourceW, GlFixed  sourceH,
					  GlFixed* sourceX, GlFixed* sourceY,
					  int rotation, GlFixed increment );


	/*	Process a particular frame to the stream.
		@param frame	input: node that describes the frame
		@param surface	input: the surface where the data is 
		@param rle		output: where to write the frame data
	*/
//	bool ProcessFrame(	const AllInfo& allInfo,
//						SDL_Surface* surface, 
//						KrRle* rle );

	// Font encoding
	bool EncodeSFontFrames( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );
	bool EncodeFixedFontFrames( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );

	// Image encoding
	bool EncodeSprite( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );
	bool EncodeFont( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );
	bool EncodeTile( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );

	// Direct encoding
	bool EncodeColorKey( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console );
//	bool EncodeImage( SDL_Surface* surface, TiXmlElement* e, KrConsole* console );

	bool EncodeBinary( TiXmlElement* e, KrConsole* console );
	bool EncodeText( TiXmlElement* e, KrConsole* console );

	// Printing
	void PrintSprite( KrConsole*, const gedString& spriteName, const gedString& actionName,
								  int frameNum, KrRle* rle );
	void PrintTile(   KrConsole*, const gedString& tileName, KrTileResource* tile );

	SDL_Surface* LoadSurface( TiXmlElement* definition, gedString* error );

	U32 tagpos;

	// For storing the 'constant' information used to write the
	// header file.
	enum
	{
		SPRITE,
		TILE,
		ACTION,
		NUM_NAME_BINS,
	};

	enum 
	{
		DEFINITION,
		DIRECT
	};

	struct Scan
	{
		Scan()	{ Init(); }
		void Init()		{ x = 0; y = 0; }

		int x;
		int y;
	};

	Scan scan;

	int mode;		// definiton or direct

	// Output information.
	int thisLine;
	int nextLine;
	int outputX;
	SDL_Surface* output;

	// Count for information
	U32 numRGBA, numLine, numSegment;
	U32 numRlePos;	// position to write count information.

	KrResourceVault vault;

	KrCachedWrite cachedWrite;
	SDL_RWops* stream;
	SDL_RWops* realStream; //maks
};

#endif

#endif //#ifndef STAND_ALONE_GAME //maks

