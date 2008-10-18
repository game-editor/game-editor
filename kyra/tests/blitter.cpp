/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

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
*/

#include "demos.h"
#include "../engine/kyra.h"


TestBlitter::TestBlitter( SDL_Surface* screen )
{
	engine = new KrEngine( screen );

	surface[0] = SDL_CreateRGBSurface(	SDL_SWSURFACE,
										16, 16, 16,
										0x1f,
										0x1f << 5,
										0x1f << 10,
										0 );
	surface[1] = SDL_CreateRGBSurface(	SDL_SWSURFACE,
										16, 16, 24,
										0xff,
										0xff << 8,
										0xff << 16,
										0 );
	surface[2] = SDL_CreateRGBSurface(	SDL_SWSURFACE,
										16, 16, 32,
										0xff,
										0xff << 8,
										0xff << 16,
										0xff << 24 );
	numTest = 0;

	KrFontResource* font = fontVault->GetFontResource( "CONSOLE" );
	GLASSERT( font );
	textBox = new KrTextBox( font,
							engine->ScreenBounds().Width(),
							engine->ScreenBounds().Height(),
							0 );
	engine->Tree()->AddNode( 0, textBox );
}


TestBlitter::~TestBlitter()
{
	delete engine;
	for( int i=0; i<NUM_SURFACE; i++ )
		SDL_FreeSurface( surface[i] );
}


void TestBlitter::CalcColorChannelFixed(	U8 newColor,
											U8 newAlpha,
											U8 surfaceColor,
											U8 cformM,
											U8 cformB,
											U8 cformAlpha,
											GlFixed* c )
{
	GlFixed fix1 = 1;
	GlFixed fix255 = 255;
	GlFixed alpha = newAlpha / fix255 * cformAlpha / fix255;

	GLASSERT( alpha >= 0 );
	GLASSERT( alpha <= 1 );

	GlFixed fNewColor = newColor;
	GlFixed fSurfaceColor = surfaceColor;
	GlFixed fCformM = cformM;
	GlFixed fCformB = cformB;

	*c = alpha * ( fNewColor / fix255 * fCformM + fCformB )
	     + ( fix1 - alpha ) * fSurfaceColor;
}


void TestBlitter::Run(		KrRGBA newColor,
							KrRGBA surfaceColor,
							KrColorTransform cform,
							bool sourceAlpha  )
{
	for( int i=0; i<NUM_SURFACE; i++ )
	{
		KrPainter painter( surface[i] );
		painter.SetPixel( 0, 0, surfaceColor );

		KrPaintInfo info( surface[i] );
		KrPaintFunc func = info.GetBlitter( sourceAlpha, cform );

		func( &info, surface[i]->pixels, &newColor, 1, cform );

		test[ numTest ].surfaceColor = surfaceColor;
		test[ numTest ].newColor = newColor;
		
		painter.BreakPixel( 0, 0, &test[ numTest ].resultColor );
		info.GetBlitterName( func, &test[ numTest ].name );

		
		CalcColorChannelFixed(	newColor.c.red,
								newColor.c.alpha,
								surfaceColor.c.red,
								cform.m.c.red,
								cform.b.c.red,
								cform.b.c.alpha,
								&test[ numTest ].r );

		CalcColorChannelFixed( newColor.c.green,
								newColor.c.alpha,
								surfaceColor.c.green,
								cform.m.c.green,
								cform.b.c.green,
								cform.b.c.alpha,
								&test[ numTest ].g );

		CalcColorChannelFixed( newColor.c.blue,
								newColor.c.alpha,
								surfaceColor.c.blue,
								cform.m.c.blue,
								cform.b.c.blue,
								cform.b.c.alpha,
								&test[ numTest ].b );
		++numTest;
	}
}


void TestBlitter::SetUp(	KrRGBA newColorBase,
							KrRGBA surfaceColorBase,
							KrColorTransform cformBase  )
{
	KrRGBA newColor;
	KrRGBA surfaceColor;
	KrColorTransform cform;

	// NoAlpha in source
	// -- Simple transform
	cform.Set( 255, 0, 255, 0, 255, 0, 255 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	newColor.c.alpha = 255;
	Run( surfaceColor, newColor, cform, false );

	// -- Color transform
	cform = cformBase;
	cform.SetAlpha( 255 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	newColor.c.alpha = 255;
	Run( surfaceColor, newColor, cform, false );

	// -- Alpha transform
	cform = cformBase;
	cform.SetRed( 255, 0 );
	cform.SetGreen( 255, 0 );
	cform.SetBlue( 255, 0 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	newColor.c.alpha = 255;
	Run( surfaceColor, newColor, cform, false );

	// -- Full transform
	cform = cformBase;
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	newColor.c.alpha = 255;
	Run( surfaceColor, newColor, cform, false );


	// Alpha in source
	// -- Simple transform
	cform.Set( 255, 0, 255, 0, 255, 0, 255 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	Run( surfaceColor, newColor, cform, true );

	// -- Color transform
	cform = cformBase;
	cform.SetAlpha( 255 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	Run( surfaceColor, newColor, cform, true );

	// -- Alpha transform
	cform = cformBase;
	cform.SetRed( 255, 0 );
	cform.SetGreen( 255, 0 );
	cform.SetBlue( 255, 0 );
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	Run( surfaceColor, newColor, cform, true );

	// -- Full transform
	cform = cformBase;
	surfaceColor = surfaceColorBase;
	newColor     = newColorBase;
	Run( surfaceColor, newColor, cform, true );
}


void TestBlitter::DrawFrame()
{
	numTest = 0;

	KrRGBA newColor;
	KrRGBA baseColor;
	KrColorTransform cform;
	
	newColor.Set( 160, 110, 40, 80 );
	baseColor.Set( 20, 80, 200 );
	cform.Set( 40, 80, 50, 90, 30, 40, 200 );

	SetUp( newColor, baseColor, cform );

	char buf[ 256 ];
	sprintf( buf, "                      color             surface           resulting" );
	textBox->SetTextChar( buf, 0 );

	int line = 1;
	for( int i=0; i<numTest; i++ )
	{
		if ( i%3 == 0 )
		{
// 			++line;
			sprintf( buf, "                                                 Optimal %3d %3d %3d",
					 test[i].r.ToIntRoundUp(), 
					 test[i].g.ToIntRoundUp(), 
					 test[i].b.ToIntRoundUp() );
			textBox->SetTextChar( buf, line );
			++line;
		}

		bool ok =    (( test[i].r.ToIntRoundUp() - test[i].resultColor.c.red ) < 10 )
		          && (( test[i].g.ToIntRoundUp() - test[i].resultColor.c.green ) < 10 )
				  && (( test[i].b.ToIntRoundUp() - test[i].resultColor.c.blue ) < 10 );
		          
		sprintf( buf, "%20s %3d %3d %3d %3d   %3d %3d %3d %3d   %3d %3d %3d  %s",
				 test[i].name.c_str(),
				 test[i].newColor.c.red, test[i].newColor.c.green, test[i].newColor.c.blue, test[i].newColor.c.alpha,
				 test[i].surfaceColor.c.red, test[i].surfaceColor.c.green, test[i].surfaceColor.c.blue, test[i].surfaceColor.c.alpha,
				 test[i].resultColor.c.red, test[i].resultColor.c.green, test[i].resultColor.c.blue,
				 ok ? "[ok]" : "[error]" );

		textBox->SetTextChar( buf, line );
		++line;
	}
	
	engine->Draw();
}

