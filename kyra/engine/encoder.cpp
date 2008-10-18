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

#include "encoder.h"
#include "SDL_image.h"
#include "../engine/painter.h"
#include "../util/gldebug.h"
#include "../engine/parser.h"
#include "SDL_rwops.h"
#include "SDL_endian.h"
#include "../engine/rle.h"
#include "../engine/sdlutil.h"
#include "../engine/pixelblock.h"
#include "../engine/canvasresource.h"
#include "../engine/fontresource.h"
#include "../engine/tileresource.h"
#include "../engine/dataresource.h"
#include "../engine/engine.h"
#include "../gui/console.h"
#include "../../gameEngine/compression.h" //maks

ImageLoaderFunc KrEncoder::ImageLoader = 0;


KrEncoder::KrEncoder( SDL_RWops* _stream, bool _bSaveCompressed ) //maks
	: cachedWrite( _stream ), stream( _stream )
{
//	idShift[0] = 0;
//	idShift[1] = 12;	//0x1000;
//	idShift[2] = 10;	//0x400000;
//
	output = 0;
	numRGBA = 0;
	numLine = 0;
	numSegment = 0;

	bSaveCompressed = _bSaveCompressed; //maks
	bCompressThisResource = false;
}


SDL_Surface* KrEncoder::Load32Surface(	const char* filename,
										Transparent* trans,
										int nTrans,
										gedString* error )
{
	// Make sure SetImageLoader has been called!
	GLASSERT( ImageLoader );
	if ( !ImageLoader )
	{
		return 0;
	}

	if ( !filename )
	{
		if ( error ) *error = "No filename for a surface specified";
		#ifdef DEBUG
			GLOUTPUT( "No filename for a surface specified\n" );
		#endif
		return 0;
	}

	// Try to load the file.
	SDL_Surface* surface = ImageLoader( filename );
	if ( !surface )
	{
		char buf[256];
		sprintf( buf, "Failed to load surface '%s'.", filename );

		if ( error ) *error = buf;
		#ifdef DEBUG
			GLOUTPUT( "No filename for a surface specified\n" );
		#endif
		return 0;
	}

	// The image can be 32 bits or less. A NON-32 bit image has
	// color(s) that are marked transparent. A 32 bit image
	// simply uses the alpha channel. To simplify things,
	// images are converted to 32 bit before they
	// are returned.
	// 
	// Oddly, SDL_Image will sometimes return a 32 bit image,
	// when it wasn't. This will *really* screw up kyra.

	if ( surface->format->BytesPerPixel < 4 )
	{
		SDL_Surface* s32 = SDL_CreateRGBSurface(	SDL_SWSURFACE,
													surface->w,
													surface->h,
													32,
													0xff000000,
													0x00ff0000,
													0x0000ff00,
													0x000000ff );
		GLASSERT( s32 );
		#ifdef DEBUG
			GLOUTPUT( "Creating 32 bit SDL surface.\n" );
		#endif

		// Now copy one surface to the other, 
		// set transparency as needed afterwards.
		SDL_BlitSurface( surface, 0, s32, 0 );
		
		int i;
		KrPainter painter( s32 );

		// Covert color keys to RGB values.
		for ( i=0; i<nTrans; i++ )
		{
			if ( trans[i].type != RGBA )
			{
				switch( trans[i].type )
				{
					case LowerLeft:
						painter.BreakPixel( 0, surface->h - 1, &trans[i].rgba );
						break;
					case UpperLeft:
						painter.BreakPixel( 0, 0, &trans[i].rgba );
						break;
					case LowerRight:
						painter.BreakPixel( surface->w - 1, surface->h - 1, &trans[i].rgba );
						break;
					case UpperRight:
						painter.BreakPixel( surface->w - 1, 0, &trans[i].rgba );
						break;
					default:
						GLASSERT( 0 );
				}
			}
		}

		// Now set the transparency. 
		int x, y;
		int nTransPixels = 0;

		for( x=0; x<surface->w; x++ )
		{
			for( y=0; y<surface->h; y++ )
			{
				KrRGBA rgba;
				painter.BreakPixel( x, y, &rgba );

				for ( i=0; i<nTrans; i++ )
				{
					if (	rgba.c.red   == trans[i].rgba.c.red 
						 && rgba.c.green == trans[i].rgba.c.green
						 && rgba.c.blue  == trans[i].rgba.c.blue )
					{
						rgba.c.alpha = KrRGBA::KR_TRANSPARENT;

						// Set the surface alpha to transparent.
						painter.SetPixel( x, y, rgba );
						nTransPixels++;
						break;
					}
				}
			}
		}

		#ifdef DEBUG
			GLOUTPUT( "Transparency converted=%d\n", nTransPixels );
		#endif
		SDL_FreeSurface( surface );
		return s32;
	}
	return surface;
}


KrCanvasResource* KrEncoder::Load32Canvas(	const char* filename,
											const KrRGBA* transparent,
											int nTrans,
											gedString* error )
{
	if ( !filename )
	{
		if ( error ) *error = "No filename for a surface specified";
		return 0;
	}

	// Try to load the file.
	SDL_Surface* surface = ImageLoader( filename );
	if ( !surface )
	{
		char buf[256];
		sprintf( buf, "Failed to load surface '%s'.", filename );

		if ( error ) *error = buf;
		return 0;
	}

	// The image can be 32 bits or less. A NON-32 bit image has
	// color(s) that are marked transparent. A 32 bit image
	// simply uses the alpha channel. Canvas's are always 32 bit,
	// and we always use a canvas that supports alpha.

	KrCanvasResource* canvas = new KrCanvasResource(	"encoder", 
														surface->w,
														surface->h,
														true );

	if ( !canvas )
	{
		if ( error ) *error = "Failed to create canvas.";
		return 0;
	}

	// Copy from the surface to the canvas.
	int x, y;
	int i;
	
	KrPaintInfo canvasPaintInfo( canvas->Pixels(), canvas->Width(), canvas->Height() );
	KrPainter	canvasPainter( &canvasPaintInfo );
	KrPainter	surfacePainter( surface );

	for( x=0; x<surface->w; x++ )
	{
		for( y=0; y<surface->h; y++ )
		{
			KrRGBA rgba;
			surfacePainter.BreakPixel( x, y, &rgba );	

			for ( i=0; i<nTrans; i++ )
			{
				if (	rgba.c.red   == transparent[i].c.red
					 && rgba.c.green == transparent[i].c.green
					 && rgba.c.blue  == transparent[i].c.blue )
				{
					// Set the surface alpha to transparent.
					rgba.c.alpha = KrRGBA::KR_TRANSPARENT;
					break;
				}
			}
			canvasPainter.SetPixel( x, y, rgba );
		}
	}
	return canvas;
}


SDL_Surface* KrEncoder::LoadSurface( TiXmlElement* def, gedString* error )
{
	const gedString* surfaceName = def->Attribute( "filename" );
	if ( !surfaceName )
	{
		*error = "No filename for a surface specified.";
		return 0;
	}

	// Get the pixel colors that mark the background.
	int i;
	const int MAX_TRANS = 4;
	int nTrans = 0;
	Transparent trans[MAX_TRANS];

	for ( i=0; i<MAX_TRANS; i++ )
	{
		char buf[64];
		sprintf( buf, "Transparent%d", i );
		gedString attrib = buf;

		if ( def->Attribute( attrib ) )
		{
			const gedString* colorString = def->Attribute( attrib );
			if ( !colorString->empty() )
			{
				if ( (*colorString)[0] == '#' )
				{
					gedString aux(*colorString);
					trans[ nTrans ].type = RGBA;
					trans[ nTrans ].rgba.FromString( aux.c_str() );
				}
				else if ( colorString->compare( "LowerLeft" ) == 0 )
				{
					trans[ nTrans ].type = LowerLeft;
				}
				else if ( colorString->compare( "UpperLeft" ) == 0 )
				{
					trans[ nTrans ].type = UpperLeft;
				}
				else if ( colorString->compare( "LowerRight" ) == 0 )
				{
					trans[ nTrans ].type = LowerRight;
				}
				else if ( colorString->compare( "UpperRight" ) == 0 )
				{
					trans[ nTrans ].type = UpperRight;
				}
			}
			nTrans++;
		}
	}
	
	gedString aux(*surfaceName);
	SDL_Surface* surface = Load32Surface( aux.c_str(),
										  trans,
										  nTrans,
										  error );
	GLASSERT( surface || !error->empty() );

	/*#ifdef DEBUG
		GLOUTPUT( "Surface %s loaded.\n", surfaceName->c_str() );
	#endif*/
	return surface;
}


bool KrEncoder::EndDat()
{
	StartTag( KYRATAG_END );
	EndTag();

	SDL_RWseek( stream, numRlePos, SEEK_SET );
	GLASSERT( numRGBA >= numSegment );
	//GLASSERT( numSegment >= numLine ); //maks: Is be possible (GE_N.bmp)
	SDL_WriteLE32( stream, numRGBA );
	SDL_WriteLE32( stream, numLine );
	SDL_WriteLE32( stream, numSegment );

	#ifdef DEBUG
		GLOUTPUT( "Tally count: rgba=%d line=%d segment=%d\n", numRGBA, numLine, numSegment );
	#endif

	cachedWrite.Flush();
	return true;
}


void KrEncoder::Save() //maks
{
	MapResNamePointerIterator it(vault.GetResourceMap());
	
	for( it.Begin(); !it.Done(); it.Next() )
	{
		(*it.Value())->Save( this );
	}
}


bool KrEncoder::StartDat(const char* magic) //maks
{
	numRGBA = 0;

	// The header:
	char version[16];
	sprintf( version, "%d.%d.%d", KyraVersionMajor, KyraVersionMinor, KyraVersionBuild );

	SDL_RWwrite( stream, magic, 4, 1 );
	WriteString( stream, version, false );

	numRlePos   = SDL_RWtell( stream );
	SDL_WriteLE32( stream, 0 );		// placeholder for number of RGBAs
	SDL_WriteLE32( stream, 0 );		// placeholder for number of lines
	SDL_WriteLE32( stream, 0 );		// placeholder for number of segments

	return true;
}


bool KrEncoder::ProcessDoc( const TiXmlDocument& doc, KrEngine* engine, KrConsole* console )
{
	TiXmlElement* root = 0;
	TiXmlElement* rootChild = 0;
	TiXmlElement* action = 0;
	TiXmlElement* frame = 0;
	TiXmlElement* file = 0;
	TiXmlElement* child = 0;

	if ( ( root = doc.FirstChildElement( "Definition" ) ) != 0 )
	{
		mode = DEFINITION;
	}
	else if ( ( root = doc.FirstChildElement( "Direct" ) ) != 0 )
	{
		mode = DIRECT;
	}
	else
	{
		console->Print( "ERROR: 'Definition' or 'Direct' root element not found.\n" );
		return false;
	}

	SDL_Surface* surface = 0;
	gedString error;

	if ( mode == DEFINITION )
	{
		// The surface.
		surface = LoadSurface( root, &error );
		if ( !surface )	
		{
			console->Print( "Error loading surface: '%s'\n", error.c_str() );
			return false;
		}

		// Walk the tree, and process.
		for( rootChild = root->FirstChildElement();
			 rootChild;
			 rootChild = rootChild->NextSiblingElement() )
		{
			AllInfo allInfo;
			if ( rootChild->Value() == "Sprite" )
			{
				for( action = rootChild->FirstChildElement( "Action" );
					 action;
					 action = action->NextSiblingElement( "Action" ) )
				{
					for( frame = action->FirstChildElement( "Frame" );
						 frame;
						 frame = frame->NextSiblingElement( "Frame" ) )
					{
							CalcAllInfo( frame, &allInfo, surface );
							EncodeSprite( surface, allInfo, console );
					}
				}
			}
			else if ( rootChild->Value() == "Tile" )
			{
				CalcAllInfo( rootChild, &allInfo, surface );
				EncodeTile( surface, allInfo, console );
			}
			else if ( rootChild->Value() == "Font" )
			{
				CalcAllInfo( rootChild, &allInfo, surface );
				EncodeFont( surface, allInfo, console );
			}
			else
			{
				console->Print( "ERROR: Unrecognized element name. (Not Sprite, Tile, or Font.).\n" );
				return false;
			}
			engine->Draw();
		}
		return true;
	}
	else
	{
		for( file = root->FirstChildElement( "File" );
			 file;
			 file = file->NextSiblingElement( "File" ) )
		{
			surface = LoadSurface( file, &error );
			if ( !surface )	
			{
				console->Print( "Error loading surface: '%s'\n", error.c_str() );
				return false;
			}
			scan.Init();

			for( child = file->FirstChildElement();
				 child;
				 child = child->NextSiblingElement() )
			{
				AllInfo allInfo;
				if ( child->Value() == "ColorKey" )
				{
					CalcAllInfo( child, &allInfo, surface );
					EncodeColorKey( surface, allInfo, console );
				}
				else if ( child->Value() == "Image" )
				{
					CalcAllInfo( child, &allInfo, surface );
					if ( allInfo.type == TYPE_SPRITE )
						EncodeSprite( surface, allInfo, console );
					else if ( allInfo.type == TYPE_TILE )
						EncodeTile( surface, allInfo, console );
					else
						console->Print( "ERROR: Direct encoding can not identify whether Sprite or Tile.\n" );
				}
				else
				{
					gedString aux(child->Value());
					console->Print( "ERROR: Unrecognized element name '%s'. (Not ColorKey or Image.).\n", aux.c_str() );
					return false;
				}
				engine->Draw();
			}
		}
		for (	file = root->FirstChildElement( "BinaryFile" );
				file;
				file = file->NextSiblingElement( "BinaryFile" ) )
		{
			EncodeBinary( file, console );
		}
		for (	file = root->FirstChildElement( "TextFile" );
				file;
				file = file->NextSiblingElement( "TextFile" ) )
		{
			EncodeText( file, console );
		}
		return false;
	}
}


/*
bool KrEncoder::EncodeSpriteOrTile(	KrPaintInfo* info, 
									const AllInfo& allInfo, 
									KrConsole* console,
									int x, int y,
									int width, int height,
									int hotx, int hoty )
{
	int deltax = 0, deltay = 0;
	int isotile = 0;

	gedString spriteName = "NONE";
	gedString actionName = "NONE";
	gedString tileName   = "NONE";

	if ( e->Attribute( "sprite" ) )
	{
		spriteName = *( e->Attribute( "sprite" ) );

		if ( e->Attribute( "action" ) )
			actionName = *( e->Attribute( "action" ) );

		if ( e->Attribute( "hotspotx" ) )
			e->Attribute( "hotspotx", &hotx );

		if ( e->Attribute( "hotspoty" ) )
			e->Attribute( "hotspoty", &hoty );

		if ( e->Attribute( "deltax" ) )
			e->Attribute( "deltax", &deltax );

		if ( e->Attribute( "deltay" ) )
			e->Attribute( "deltay", &deltay );

		if ( e->Attribute( "isotile" ) )
		{
			e->Attribute( "isotile", &isotile );
			if ( isotile == 0 )
				isotile = width;
		}

		// Create or locate the sprite.
		if ( !vault.GetSpriteResource( spriteName ) )
		{
			vault.AddResource( new KrSpriteResource( spriteName ) );
		}
		KrSpriteResource* spriteResource = vault.GetSpriteResource( spriteName );
		GLASSERT( spriteResource );

		// Get or create the action.
		if ( !spriteResource->GetAction( actionName ) )
		{
			spriteResource->AddAction( new KrAction( actionName ) );
		}
		KrAction* actionRes = spriteResource->GetAction( actionName );
		GLASSERT( actionRes );

		actionRes->AddFrame();
		KrRle* rle = actionRes->GetFrame( actionRes->NumFrames() - 1 );

		if ( isotile )
		{
			CreateIsoTile( info, console, x, y, width, height, rle, isotile );
		}
		else
		{
			rle->Create( info,
						 x, y,
						 width, height,
						 hotx, hoty,
						 deltax, deltay );
		}

		PrintSprite( console, spriteName, actionName, actionRes->NumFrames() - 1, rle );

		return true;
	}
	else if ( e->Attribute( "tile" ) )
	{
		if ( width != height )
		{
			console->Print( "ERROR: Image encoding tile. Width and height of source image are not equal.\n" );
			return false;
		}

		tileName = *( e->Attribute( "tile" ) );

		if ( vault.GetTileResource( tileName ) )
		{
			console->Print( "ERROR: Tile resource '%s' already created.\n" );
			return false;
		}

		KrTileResource* tileRes = new KrTileResource( tileName, info, x, y, width );
		vault.AddResource( tileRes );

		PrintTile( console, tileName, tileRes );
		return true;
	}

	console->Print( "ERROR: 'sprite' or 'tile' attribute not found.\n" );
	return false;
}
*/


//bool KrEncoder::EncodeImage( SDL_Surface* surface, TiXmlElement* e, KrConsole* console )
//{
//	GLASSERT( e->Value() == "Image" );
//
//	gedString spriteName = "NONE";
//	gedString actionName = "NONE";
//
//	KrPaintInfo info( surface );
//
//	return EncodeSpriteOrTile( &info, e, console, 0, 0, info.width, info.height, 0, 0 );
//}


bool KrEncoder::EncodeColorKey( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console )
{
	//GLASSERT( e->Value() == "ColorKey" );

//	KrRGBA color;
//	color.Set( 255, 255, 0 );
//	int frameCount = 1024 * 1024;		// really big

//	if ( e->Attribute( "color" ) )
//	{
//		gedString colorName = *( e->Attribute( "color" ) );
//		color.FromString( colorName.c_str() );
//	}
//
//	if ( e->Attribute( "frameCount" ) )
//		e->Attribute( "frameCount", &frameCount );

	KrPaintInfo info( surface );
	KrPainter painter( surface );
	// ignore alpha channel
	// color.c.alpha = 0;
	int frames = 0;

	// Go scan by scan, looking for color keys.
	for( ; scan.y<info.height - 2;	/* no increment */ )
	{
		if ( scan.x >= info.width - 2 )
		{
			scan.x = 0;
			++scan.y;
			continue;
		}

		for( ; scan.x<info.width - 2; ++scan.x )
		{
			KrRGBA	rgba00, rgba10, rgba01, rgba11, 
					rgba21, rgba12;

			painter.BreakPixel(	scan.x, scan.y, &rgba00 );
			painter.BreakPixel(	scan.x+1, scan.y, &rgba10 );
			painter.BreakPixel(	scan.x, scan.y+1, &rgba01 );
			painter.BreakPixel(	scan.x+1, scan.y+1, &rgba11 );

			painter.BreakPixel(	scan.x+2, scan.y+1, &rgba21 );
			painter.BreakPixel(	scan.x+1, scan.y+2, &rgba12 );

			rgba00.c.alpha = KrRGBA::KR_OPAQUE;
			rgba10.c.alpha = KrRGBA::KR_OPAQUE;
			rgba01.c.alpha = KrRGBA::KR_OPAQUE;
			rgba11.c.alpha = KrRGBA::KR_OPAQUE;
			rgba21.c.alpha = KrRGBA::KR_OPAQUE;
			rgba12.c.alpha = KrRGBA::KR_OPAQUE;

			if (	rgba00 == allInfo.keyColor 
			     && rgba10 == allInfo.keyColor 
				 && rgba01 == allInfo.keyColor
				 && rgba11 != allInfo.keyColor		// don't want an empty one.
				 && rgba21 != allInfo.keyColor  	// don't be fooled by a hittest nock (x)
				 && rgba12 != allInfo.keyColor		// don't be fooled by a hittest nock (y)
				)
			{
				// Find the width and height of the image inside the color block.
				int width, height;
				int run;

				run = painter.FindPixel( scan.x+1, scan.y+1, 1, 0, allInfo.keyColor, false, false );

				if ( run < 0 )
				{
					console->Print( "ERROR: ColorKey box not closed on right side.\n" );
					return false;
				}
				width = run;

				run = painter.FindPixel( scan.x+1, scan.y+1, 0, 1, allInfo.keyColor, false, false );

				if ( run < 0 )
				{
					console->Print( "ERROR: ColorKey box not closed on bottom side.\n" );
					return false;
				}
				height = run;

				// find the hotspot markers
				int hotx = 0, hoty = 0;

				run = painter.FindPixel( scan.x+1, scan.y, 1, 0, allInfo.keyColor, false, true );
				if ( run > 0 && run < width )
				{
					hotx = run;
				}
				
				run = painter.FindPixel( scan.x, scan.y+1, 0, 1, allInfo.keyColor, false, true );
				if ( run > 0 && run < height )
				{
					hoty = run;
				}

				bool ret = false;

				{
					AllInfo temp = allInfo;
					temp.x = scan.x+1;
					temp.y = scan.y+1;
					temp.width = width;
					temp.height = height;
					temp.hotx = scan.x + 1 + hotx;
					temp.hoty = scan.y + 1 + hoty;
					
					if ( temp.type == TYPE_SPRITE )
						ret = EncodeSprite( surface, temp, console );
					else if ( temp.type == TYPE_TILE )
						ret = EncodeTile( surface, temp, console );
					else
						console->Print( "ERROR: Color Key encoding can not identify whether Sprite or Tile.\n" );
	//				bool ret = EncodeSpriteOrTile(  &info, e, console, 
	//												scan.x+1, scan.y+1,
	//												width, height,
	//												scan.x + 1 + hotx, scan.y + hoty + 1);
				}
				if ( !ret ) 
				{
					console->Print( "ERROR: Sprite or Tile encoding failed.\n" );
					return false;
				}

				++frames;
				scan.x += width + 1;		// the for loop will add one as well.
				if ( allInfo.frameCount != 0 && frames == allInfo.frameCount )
					return true;
			}
		}
	}
	return true;
}


void KrEncoder::PrintSprite( KrConsole* console, const gedString& spriteName, const gedString& actionName,
							int frameNum, KrRle* rle )
{
	gedString aux1(spriteName);
	gedString aux2(actionName);

	console->Print( "Sprite: %s :: %s :: %d  [hot:%d,%d delta:%d,%d]\n", 
					aux1.c_str(),
					aux2.c_str(),
					frameNum,
					rle->Hotspot().x, rle->Hotspot().y,
					rle->Delta().x, rle->Delta().y );
}

void KrEncoder::PrintTile( KrConsole* console, const gedString& tileName, KrTileResource* tile )
{
	gedString aux(tileName);
	console->Print( "Tile: %s  [size:%d]\n", aux.c_str(), tile->Size() );
}


bool KrEncoder::EncodeSprite( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console )
{
//	gedString spriteName, actionName;
//	TiXmlElement* action;
//	TiXmlElement* frame;
//
//	spriteName = "no_sprite_name";
//	if ( sprite->Attribute( "name" ) )
//	{
//		spriteName = *(sprite->Attribute( "name" ));
//	}

	// Create or locate the sprite.
	if ( !vault.GetSpriteResource( allInfo.name ) )
	{
		vault.AddResource( new KrSpriteResource( allInfo.name ) );
	}
	KrSpriteResource* spriteResource = vault.GetSpriteResource( allInfo.name );
	GLASSERT( spriteResource );

	GlDynArray< int > rotation;
	if ( allInfo.rotation.Count() )
		rotation = allInfo.rotation;
	else
		rotation.PushBack( 0 );

	for( int i=0; i<(int) rotation.Count(); ++i )
	{
		// Get or create the action.
		// If using rotation, append to the name.
		gedString action = allInfo.action;
		if ( rotation.Count() > 1 )
		{
			char buf[16];
			sprintf( buf, ".ROT%03d", rotation[i] );
			action += buf;
		}

		if ( !spriteResource->GetAction( action ) )
		{
			spriteResource->AddAction( new KrAction( action ) );
		}
		KrAction* actionRes = spriteResource->GetAction( action );
		GLASSERT( actionRes );

		int index = actionRes->NumFrames();
		actionRes->AddFrame();

		KrPaintInfo info( surface );
		KrRle* rle = actionRes->GetFrame( index );

		if ( allInfo.isoTargetWidth > 0 )
		{
			CreateIsoTile(  &info, console, 
							allInfo.x, allInfo.y, allInfo.width, allInfo.height,
							rle, allInfo.isoTargetWidth,
							rotation[i] );
		}
		else
		{
			rle->Create( &info, allInfo.x, allInfo.y, allInfo.width, allInfo.height, allInfo.hotx, allInfo.hoty, allInfo.deltax, allInfo.deltay );
		}
		PrintSprite( console, allInfo.name, action, index, actionRes->GetFrame( index ) );
	}

	return true;
}


bool KrEncoder::EncodeFont( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console )
{
	GLASSERT( surface->format->BytesPerPixel == 4 );

	// Create or locate the font.
	if ( vault.GetFontResource( allInfo.name ) )
	{
		gedString aux(allInfo.name);
		console->Print( "ERROR: Font %s encoded twice.\n", aux.c_str() );
		return false;
	}

	int type = KrFontResource::FIXED;
	if ( allInfo.subType == SUBTYPE_SFONT )
		type = KrFontResource::SFONT;

	if ( type == KrFontResource::FIXED && allInfo.fontLength == 0 )
	{
		console->Print( "ERROR: Fixed font created without the 'length' attribute.\n" );
		return false;
	}

	KrPaintInfo info( surface );
	KrFontResource* fontRes = new KrFontResource(	allInfo.name,
													&info,
													allInfo.fontStart,
													allInfo.space,
													type,
													allInfo.fontLength );
	vault.AddResource( fontRes );

	gedString aux(allInfo.name);
	console->Print( "Font: %s\n", 
					aux.c_str() );

	return true;
}


bool KrEncoder::EncodeText( TiXmlElement* e, KrConsole* console )
{
	gedString filename = "no_file";
	gedString resname	 = "no_text_name";

	if ( e->Attribute( "filename" ) )
	{
		filename = *( e->Attribute( "filename" ) );
	}
	else
	{
		console->Print( "ERROR: Encoding text data has no filename attribute.\n" );
		return false;
	}
	if ( e->Attribute( "name" ) )
	{
		resname = *( e->Attribute( "name" ) );
	}
	else
	{
		console->Print( "ERROR: Encoding text data has no name attribute.\n" );
		return false;
	}

	KrTextDataResource* res = new KrTextDataResource( resname );
	if ( !res || !res->LoadTextFile( filename.c_str() ) )
	{
		console->Print( "ERROR: Encoding text failed to load '%s'.\n", filename.c_str() );
		return false;
	}

	vault.AddResource( res );

	console->Print( "TextData: %s\n", 
					resname.c_str() );

	return true;
}


bool KrEncoder::EncodeBinary( TiXmlElement* e, KrConsole* console )
{
	gedString filename = "no_file";
	gedString resname	 = "no_binary_name";

	if ( e->Attribute( "filename" ) )
	{
		filename = *( e->Attribute( "filename" ) );
	}
	else
	{
		console->Print( "ERROR: Encoding binary data has no filename attribute.\n" );
		return false;
	}
	if ( e->Attribute( "name" ) )
	{
		resname = *( e->Attribute( "name" ) );
	}
	else
	{
		console->Print( "ERROR: Encoding binary data has no name attribute.\n" );
		return false;
	}

	KrBinaryDataResource* res = new KrBinaryDataResource( resname );
	if ( !res || !res->LoadFile( filename.c_str() ) )
	{
		console->Print( "ERROR: Encoding binary failed to load '%s'.\n", filename.c_str() );
		return false;
	}

	vault.AddResource( res );

	console->Print( "BinaryData: %s\n", 
					resname.c_str() );
					
	return true;
}


bool KrEncoder::EncodeTile( SDL_Surface* surface, const AllInfo& allInfo, KrConsole* console )
{
//	gedString name;
//	int x = 0, y = 0, size = 0;
//
//	name = "no_tile_name";
//	if ( tile->Attribute( "name" ) )
//	{
//		name = *( tile->Attribute( "name" ) );
//	}
	if ( vault.GetTileResource( allInfo.name ) )
	{
		gedString aux(allInfo.name);
		console->Print( "ERROR: Tile '%s' created multiple times.\n",
						aux.c_str() );
		return false;
	}

//	tile->Attribute( "x", &x );
//	tile->Attribute( "y", &y );
//	tile->Attribute( "size", &size );
//
	// Do not write an empty tile.
	if ( allInfo.width <= 0 )
	{
		console->Print( "ERROR: Tile size not specified, or 0.\n" );
		return false;
	}

	KrPaintInfo info( surface );
	KrTileResource* tileRes = new KrTileResource( allInfo.name, &info, allInfo.x, allInfo.y, allInfo.width );
	vault.AddResource( tileRes );

	PrintTile( console, allInfo.name, tileRes );

	return true;
}


void KrEncoder::StartTag( U32 tag, bool _bCompressThisResource )
{
	SDL_WriteLE32( stream, tag );
	tagpos = SDL_RWtell( stream );

	if(bSaveCompressed) SDL_WriteLE32( stream, 0 ); //Compressed size
	SDL_WriteLE32( stream, 0 ); //Real size

	realStream = stream;

	//Redirect to temp file
	if(bSaveCompressed) stream = SDL_RWFromFile( "gedTmp", "wb" );
	bCompressThisResource = _bCompressThisResource;
}


void KrEncoder::EndTag()
{
	U32 lenght = 0;
	if(bSaveCompressed)
	{
		lenght = SDL_RWtell( stream );
		unsigned int lenghtComp = 0;
		
		
		//Get data	
		U8 *in = new U8[lenght];
		U8 *out = new U8[lenght + lenght / 8 + 256];
		
		SDL_RWclose(stream);
		stream = SDL_RWFromFile( "gedTmp", "rb" );
		SDL_RWread(stream, in, lenght, 1);
		SDL_RWclose(stream);
		
#ifndef STAND_ALONE_GAME
		remove("gedTmp");
#endif
		
		//Restore real stream
		stream = realStream;
		
		//Compress
		Compression comp;

		if(bCompressThisResource)
		{
			lenghtComp = comp.DoCompression(in, lenght, out, lenght + lenght / 8 + 256);
		}
		else
		{
			lenghtComp = lenght;
		}
		
		if(lenghtComp < lenght)
			SDL_RWwrite(stream, out, lenghtComp, 1);
		else
		{
			SDL_RWwrite(stream, in, lenght, 1);
			lenghtComp = lenght;
		}
		
		delete [] out;
		delete [] in;
	}
	
	

	//End
	U32 current = SDL_RWtell( stream );
	
	// Write the jump.
	GLASSERT( current > tagpos );
	U32 jump = current - tagpos - 8;	// Add the 8 to skip over the offset value itself

	SDL_RWseek( stream, tagpos, SEEK_SET );
	SDL_WriteLE32( stream, jump ); //Compressed size
	if(bSaveCompressed) SDL_WriteLE32( stream, lenght ); //Real size
	SDL_RWseek( stream, current, SEEK_SET );
	tagpos = 0;
}


void KrEncoder::WriteHeader(	const char* name, 
								FILE* file, 
								const char* prefix )
{
	gedString defName;
	for( const char* p = name; p && *p; ++p )
	{
		if (    ( *p >= 'a' && *p <= 'z' )
			 || ( *p >= 'A' && *p <= 'Z' )
			 || ( *p >= '0' && *p <= '9' ) )
		{
			defName += *p;
		}
		else
		{
			defName += '_';
		} 
	}

	fprintf( file, "#ifndef KYRA_%s_INCLUDED\n", defName.c_str() );
	fprintf( file, "#define KYRA_%s_INCLUDED\n\n", defName.c_str() );
	fprintf( file, "// Machine generated Kyra header file.\n" );
	fprintf( file, "// Will be overwritten by the encoder.\n" );
	fprintf( file, "// Generated by kyra version %d.%d.%d\n\n", KyraVersionMajor, KyraVersionMinor, KyraVersionBuild ); 

	cachedWrite.WriteHeader( file, prefix );
	fprintf( file, "#endif\n" );
}


void KrEncoder::CreateIsoTile(	KrPaintInfo* info, 
								KrConsole* console,
								int x, int y,
								int width, int height,
								KrRle* rle,
								int isoWidth,
								int rotation )
{
	if ( isoWidth % 4 )
	{
		console->Print( "ERROR: Isometric tile created with non-multiplo of 4 width (%d).\n", isoWidth );
		return;
	}

	int isoHeight = isoWidth / 2;
	KrPainter painter( info );
	KrRGBA rgba;

	// Create a memory buffer to hold the tile:
	KrRGBA* isoMemory = new KrRGBA[ isoWidth * isoHeight ];
	memset( isoMemory, 0, isoWidth * isoHeight * sizeof( KrRGBA ) );

	for( int iy = 0; iy < isoHeight; ++iy )
	{
		int rowwidth = 0;
		if ( iy < isoHeight / 2 )
			rowwidth = 2 + 4 * iy;
		else
			rowwidth = 2 + 4 * ( isoHeight - iy - 1 );

		const int QUALITY = 4;		// 2, 4
		const int QUALITYAREA = 16;	// 4, 16
		const int QUALITYBIAS = 7;	// 1, 7
		const GlFixed increment = GlFixed( 1  ) / GlFixed( QUALITY );

		for( int ix =  isoWidth / 2 - rowwidth / 2;
		         ix <  isoWidth / 2 + rowwidth / 2;
				 ++ix )
		{
			int red = 0, green = 0, blue = 0, alpha = 0;

			for ( int i=0; i<QUALITY; ++i )
			{
				for ( int j=0; j<QUALITY; ++j )
				{
					GlFixed fsx, fsy;
					int sx, sy;

					IsoToSource( ix + increment*i,   
								 iy - isoHeight / 2 + increment*j,   
								 isoWidth, width, height, &fsx, &fsy,
								 rotation, increment );
					
					//sx = fsx.ToInt();
					//sy = fsy.ToInt();

					sx = GlClamp( fsx.ToIntRound(), 0, width - 1 );
					sy = GlClamp( fsy.ToIntRound(), 0, height - 1 );					

					painter.BreakPixel( sx + x, sy + y, &rgba );
					
					red   += rgba.c.red;
					green += rgba.c.green;
					blue  += rgba.c.blue;
					alpha += rgba.c.alpha;
				}
			}

			// Whew! all that for one pixel.
			// Use rounding on the colors, to gamma-correct
			rgba.Set(	( red + QUALITYBIAS ) / QUALITYAREA, 
						( green + QUALITYBIAS ) / QUALITYAREA, 
						( blue + QUALITYBIAS ) / QUALITYAREA, 
						( alpha + QUALITYBIAS ) / QUALITYAREA );
			GLASSERT( iy >= 0 && iy < isoHeight );
			GLASSERT( ix >= 0 && ix < isoWidth );
			isoMemory[ iy * isoWidth + ix ] = rgba;
		}
	}
	KrPaintInfo isoInfo( isoMemory, isoWidth, isoHeight );
	rle->Create( &isoInfo, 0, 0, 
				 isoWidth, isoHeight, 
				 ( isoWidth - 1 ) / 2, ( isoHeight - 1 ) / 2, 
				 isoWidth, isoHeight );
	delete [] isoMemory;
}


void KrEncoder::IsoToSource(	GlFixed x, GlFixed y, GlFixed isoWidth, 
								GlFixed sourceW, GlFixed  sourceH,
								GlFixed* sourceX, GlFixed* sourceY,
								int rotation,
								GlFixed increment )
{
	// x, y are in iso coordinates.
	GlFixed isoHeight  = isoWidth / 2;

	switch( rotation )
	{
		case 90:
		{
			*sourceX = (sourceW - increment) - x * sourceW / ( isoWidth ) - y * sourceH / ( isoHeight );
			*sourceY =						   x * sourceW / ( isoWidth ) - y * sourceH / ( isoHeight );
		}
		break;

		case 180:
		{
			*sourceX = (sourceW - increment) - x * sourceW / ( isoWidth ) + y * sourceH / ( isoHeight );
			*sourceY = (sourceH - increment) - x * sourceW / ( isoWidth ) - y * sourceH / ( isoHeight );
		}
		break;

		case 270:
		{
			*sourceX =						 x * sourceW / ( isoWidth ) + y * sourceH / ( isoHeight );
			*sourceY = (sourceH-increment) - x * sourceW / ( isoWidth ) + y * sourceH / ( isoHeight );
		}
		break;

		default:
		{
			GLASSERT( x <= isoWidth );
			GLASSERT( y <= isoHeight );
			*sourceX = x * sourceW / ( isoWidth ) - y * sourceH / ( isoHeight );
			*sourceY = x * sourceW / ( isoWidth ) + y * sourceH / ( isoHeight );
		}
		break;
	}
}


void KrEncoder::CalcAllInfo( TiXmlNode* node, 
							 AllInfo* i,
							 SDL_Surface* surface )
{
	TiXmlElement* ele = node->ToElement();
	if ( !ele )
	{
		GLASSERT( 0 );
		return;
	}	

	// Walk up the tree, get information as we go.
	TiXmlNode* parent = ele->Parent();
	while( parent )
	{
		TiXmlElement* parentEle = parent->ToElement();
		if ( parentEle )
		{
			if ( parentEle->Value() == "Definition" )
			{
//				i->format = FORMAT_DEF;
//
//				if ( parentEle->Attribute( "filename" ) )
//					i->filename = *parentEle->Attribute( "filename" );

				// We need go no higher.
				break;
			}
			else if ( parentEle->Value() == "Sprite" )
			{
				i->type = TYPE_SPRITE;

				if ( parentEle->Attribute( "name" ) )
					i->name = *parentEle->Attribute( "name" );
			}
			else if ( parentEle->Value() == "Action" )
			{
				if ( parentEle->Attribute( "name" ) )
					i->action = *parentEle->Attribute( "name" );
			}
			else if ( parentEle->Value() ==  "File" )
			{
//				if ( parentEle->Attribute( "filename" ) )
//					i->filename = *parentEle->Attribute( "filename" );
			}
			else if ( parentEle->Value() ==  "Direct" )
			{
				//i->format = FORMAT_DIRECT;
				// Go no higher.
				break;
			}
		}
		parent = parent->Parent();
	}

	// Now interpret the element itself:
	if ( ele->Value() == "Image" )
	{
		// Could be sprite or tile.
		i->useEntireImage = true;
	}
	else if ( ele->Value() == "ColorKey" )
	{
		// Could be sprite on tile.
		//i->useEntireImage = false;
	}
	else if ( ele->Value() == "Frame" )
	{
		i->type = TYPE_SPRITE;
	}
	else if ( ele->Value() == "Font" )
	{
		i->type = TYPE_FONT;
	}
	else if ( ele->Value() == "Tile" )
	{
		i->type = TYPE_TILE;
	}
	
	// And its attributes. They don't have different meanings in different
	// tags, so they can all be read in together.

	// ColorKey and Image attributes:
	if ( ele->Attribute( "tile" ) )
	{
		GLASSERT( i->type == TYPE_NONE );
		i->name = *ele->Attribute( "tile" );
		i->type = TYPE_TILE;
	}
	if ( ele->Attribute( "sprite" ) )
	{
		GLASSERT( i->type == TYPE_NONE );
		i->name = *ele->Attribute( "sprite" );
		i->type = TYPE_SPRITE;
	}
	if ( ele->Attribute( "color" ) )
	{
		gedString c = *ele->Attribute( "color" );
		i->keyColor.FromString( c.c_str() );
		i->keyColor.c.alpha = KrRGBA::KR_OPAQUE;	// alpha not used
	}
	if ( ele->Attribute( "frameCount" ) )
	{
		ele->Attribute( "frameCount", &i->frameCount );
	}
	if ( ele->Attribute( "action" ) )
	{
		i->action = *ele->Attribute( "action" );
	}
	if ( ele->Attribute( "sprite" ) )
	{
		i->name = *ele->Attribute( "sprite" );
	}

	// Used by tile and font:
	if ( ele->Attribute( "name" ) )
	{
		i->name = *ele->Attribute( "name" );
	}

	// Font attributes:
	if ( i->type == TYPE_FONT )
	{
		if ( ele->Attribute( "start" ) )
		{
			ele->Attribute( "start", &i->fontStart );
		}
		if ( ele->Attribute( "space" ) )
		{
			ele->Attribute( "space", &i->space );
		}
		if ( ele->Attribute( "type" ) )
		{
			if ( *ele->Attribute( "type" ) == "sfont" )
				i->subType = SUBTYPE_SFONT;
		}
	}

	// Generic attributes:
	if ( ele->Attribute( "x" ) )
	{
		ele->Attribute( "x", &i->x );
	}
	if ( ele->Attribute( "y" ) )
	{
		ele->Attribute( "y", &i->y );
	}
	if ( ele->Attribute( "size" ) )
	{
		GLASSERT( i->type == TYPE_TILE );
		ele->Attribute( "size", &i->width );
		i->height = i->width;					// size is height and width for tiles.
	}
	if ( ele->Attribute( "width" ) )
	{
		ele->Attribute( "width", &i->width );
	}
	if ( ele->Attribute( "height" ) )
	{
		ele->Attribute( "height", &i->height );
	}
	if ( ele->Attribute( "hotspotx" ) )
	{
		//i->hasHotspot = true;
		ele->Attribute( "hotspotx", &i->hotx);
	}
	if ( ele->Attribute( "hotspoty" ) )
	{
		//i->hasHotspot = true;
		ele->Attribute( "hotspoty", &i->hoty );
	}
	if ( ele->Attribute( "deltax" ) )
	{
		//i->hasDelta = true;
		ele->Attribute( "deltax", &i->deltax );
	}
	if ( ele->Attribute( "deltay" ) )
	{
		//i->hasDelta = true;
		ele->Attribute( "deltay", &i->deltay );
	}
	if ( ele->Attribute( "isotile" ) )
	{
		i->isoTargetWidth = 0;
		ele->Attribute( "isotile", &i->isoTargetWidth );
	}
	if ( ele->Attribute( "length" ) )
	{
		ele->Attribute( "length", &i->fontLength );
	}
	if ( ele->Attribute( "rotation" ) )
	{
		gedString r = *ele->Attribute( "rotation" );
		GlString::RemoveWhiteSpace( &r );

		GlDynArray< gedString > strArray;
		GlString::Split( &strArray, r, ",", false );

		for( int k=0; k<(int)strArray.Count(); ++k )
		{
			if ( strArray[k].length() > 1 && strArray[k].at( 0 ) == 'd' )
			{
				int division = strArray[k].at( 1 ) - '0'; //maks
				if ( division > 0 )
				{
					int increment = 360 / division;
					if ( increment > 0 )
					{
						for( int theta = 0; theta < 360; theta += increment )
						{
							i->rotation.PushBack( theta );
						}
					}
				}
			}
			else
			{
				int theta = atoi( strArray[k].c_str() );
				i->rotation.PushBack( theta );
			}
		}
		i->rotation.Sort();
	}

	if ( i->useEntireImage )
	{
		i->x = 0;
		i->y = 0;
		i->width = surface->w;
		i->height = surface->h;
	}
}


/*static*/ KrFontResource* KrEncoder::CreateFixedFontResource(	const char* name,
																const U8* buffer,
																int bufferSize )
{
	SDL_RWops* rw = SDL_RWFromMem( (void*) buffer, bufferSize );
	SDL_Surface* surfaceBMP = SDL_LoadBMP_RW( rw, false );
	GLASSERT( surfaceBMP );
	SDL_FreeRW( rw );

	SDL_Surface* s32 = SDL_CreateRGBSurface(	SDL_SWSURFACE,
												surfaceBMP->w,
												surfaceBMP->h,
												32,
												0xff << ( KrRGBA::RED * 8 ),
												0xff << ( KrRGBA::GREEN * 8 ),
												0xff << ( KrRGBA::BLUE * 8 ),
												0xff << ( KrRGBA::ALPHA * 8 ) );
	GLASSERT( s32 );

	// Now copy one surface to the other, 
	// set transparency as needed afterwards.
	SDL_BlitSurface( surfaceBMP, 0, s32, 0 );

	KrPaintInfo info( s32 );

	KrRGBA* rgba = (KrRGBA*) s32->pixels;
	KrRGBA  tr = *rgba;

	int count = info.width * info.height;
	for( int i=0; i<count; ++i )
	{
		if (	rgba[i].c.red == tr.c.red 
			 && rgba[i].c.green == tr.c.green 
			 && rgba[i].c.blue == tr.c.blue )

			rgba[i].all = 0;
	}

	KrFontResource* fontRes = new KrFontResource( name, &info, 
												  0, 0, 
												  KrFontResource::FIXED, 256 );	

	SDL_FreeSurface( surfaceBMP );
	SDL_FreeSurface( s32 );
	return fontRes;
}


#endif //#ifndef STAND_ALONE_GAME //maks
