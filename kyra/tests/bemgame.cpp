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


#include "demos.h"
#include "bem.h"
#include <math.h>
#include "kyra.h"


const int BemGame::map[MAPY][MAPX] = 
{
	{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0	},
	{	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0	},
	{	0, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 0	},
	{	0, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 0	},
	{	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0	},
	{	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	},
	{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	},
};


const int BemGame::dirMap[4] = { BEM_WALK | BEM_DIR7, 
								 BEM_WALK | BEM_DIR9, 
								 BEM_WALK | BEM_DIR3, 
								 BEM_WALK | BEM_DIR1 };


BemGame::BemGame( SDL_Surface* screen, bool useWindows )
{
	random.SetSeed( 0 );

	KrRGBA green;
	green.Set( 0, 200, 0 );
	
	if ( useWindows )
	{
		KrRect rects[5];
		
		const int lW = 280;
		const int lH = 180;
		const int r = 200;
		const int pad = 10;
		
		rects[0].Set( 0,   0, screen->w - 1 - pad - r, r + pad - 1 );
		rects[1].Set( 0, r + pad, screen->w-1, screen->h - 1 - pad - lH );
		rects[2].Set( lW + pad, screen->h - pad - lH, screen->w-1, screen->h - 1 );

		rects[3].Set( 0, screen->h - lH, lW-1, screen->h - 1 );
		rects[4].Set( screen->w - r, 0, screen->w-1, r-1 );

		engine = new KrEngine( screen, 5, rects, &green );
	}
	else
	{
		engine = new KrEngine( screen );
	}
	GLASSERT( engine );

	// The vault contains all the resources used by this
	// demo.
	if (!engine->Vault()->LoadDatFile( "bem.dat" ) )
	{
		#ifdef DEBUG
			GLOUTPUT( "Error loading dat file: 'bem.dat'!\n" );
		#endif
		exit( 100 );
	}

	KrRGBA black;
	black.Set( 0, 0, 0 );
	if ( useWindows )
	{
		engine->FillBackgroundWindow( 0, 0 );			// main
		engine->FillBackgroundWindow( 1, 0 );			// main
		engine->FillBackgroundWindow( 2, 0 );			// main
		engine->FillBackgroundWindow( 3, &black );		// zoomed out
		engine->FillBackgroundWindow( 4, &black );		// moving zoom
	}
	else
	{
		// We have a space image so the engine should not draw
		// the background. Performance enhancement.
		engine->FillBackground( 0 );
	}

	// Set up some depth categories:
	backgroundTree = new KrImNode;
	underTree	   = new KrImNode;
	floorTree      = new KrImNode;
	standingTree   = new KrImNode;
	overTree       = new KrImNode;

	// Depth sort by order of addition. All the z-depths are
	// left at 0. In other words, when added at the same Z-depth (0
	// in this case) the object added most recently is on top.
	engine->Tree()->AddNode( 0, backgroundTree );
	engine->Tree()->AddNode( 0, underTree );
	engine->Tree()->AddNode( 0, floorTree );
	engine->Tree()->AddNode( 0, standingTree );
	engine->Tree()->AddNode( 0, overTree );

	// Store the floor size. Due to the way the sprites are defined,
	// the size in terms of positioning the tiles is slightly different
	// than the size in terms of the tile bitmap.
	KrSpriteResource* resource = engine->Vault()->GetSpriteResource( BEM_ROOM | BEM_FLOOR );
	GLASSERT( resource );
	tileWidth  = resource->GetActionByIndex( 0 )->Frame( 0 ).Width() +2;
	tileHeight = resource->GetActionByIndex( 0 )->Frame( 0 ).Height();

	isoMath = new GlIsoMath( tileWidth, tileHeight );
	isoMath->SetScreenCenterToTile( screen->w,
									screen->h,
									MAPX / 2, MAPY / 2, 0 );

	AddSpaceTiles();
	AddFloor();
	AddRoomObjects();
	AddActors( useWindows );

	// Add the mini-map
	KrCanvasResource* canvasResource =
					new KrCanvasResource(	"minimap",
											140, 140,
											true  );
	GLASSERT( canvasResource );


	// An example of a user-defined resource getting added to a vault.
	engine->Vault()->AddResource( canvasResource );

	canvas = new KrCanvas( canvasResource );
	canvas->SetPos( screen->w - canvasResource->Width(),
					screen->h - canvasResource->Height() );
	engine->Tree()->AddNode( overTree, canvas );

	subtick = 0;
	tick = 0;
	teleFrame = 0;
	teleSprite = 0;
	DrawMiniMap();

	if ( useWindows )
	{
		// Set the 3 "band" windows to be positioned
		// to the first.
 		engine->Tree()->Root()->SetPos( 0, 0, 0 );
		engine->Tree()->Root()->SetPos( -engine->ScreenBounds(1).xmin, -engine->ScreenBounds(1).ymin, 1 );
 		engine->Tree()->Root()->SetPos( -engine->ScreenBounds(2).xmin, -engine->ScreenBounds(2).ymin, 2 );

		SetupLeftWindow();
		SetupRightWindow();
	}
}


BemGame::~BemGame()
{
	delete engine;
	delete isoMath;
}


void BemGame::SetupRightWindow()
{
	// Turn off the tiles: they scale but it looks bad
	backgroundTree->SetVisible( false, 4 );
	rightDormant = 10;
}


void BemGame::SetupLeftWindow()
{
	GlFixed scale = 0.3;

	// Set window #3 to be cached scaled. Cached scaling doesn't
	// do anything good for an alpha channel, so selectively cache.
	KrResource* resource = engine->Vault()->GetResource( KYRATAG_SPRITE, BEM_ROOM | BEM_FLOOR );
	GLASSERT( resource );
	resource->CacheScale( scale, scale );

	resource = engine->Vault()->GetResource( KYRATAG_TILE, BEM_SPACE );
	GLASSERT( resource );
	resource->CacheScale( scale, scale );

	resource = engine->Vault()->GetResource( KYRATAG_SPRITE, BEM_ROOM | BEM_PLASMA_PLANT );
	GLASSERT( resource );
	resource->CacheScale( scale, scale );

	resource = engine->Vault()->GetResource( KYRATAG_SPRITE, BEM_BRAIN );
	GLASSERT( resource );
	resource->CacheScale( scale, scale );

	resource = engine->Vault()->GetResource( KYRATAG_SPRITE, BEM_DRONE );
	GLASSERT( resource );
	resource->CacheScale( scale, scale );

	canvas->SetVisible( false, 3 );
	int x, y;
	GlFixed fx, fy;
	isoMath->TileToScreen( MAPX/2, MAPY/2, 0, &x, &y );
	fx = x;
	fy = y;
	fx *= scale;
	fy *= scale;

 	engine->Tree()->Root()->SetPos( -fx.ToInt() + engine->ScreenBounds(3).Width() / 2, 
									-fy.ToInt() + engine->ScreenBounds(3).Height() / 2, 
									3 );

	engine->Tree()->Root()->SetScale( scale, scale, 3 );
}


void BemGame::ProcessRightWindow()
{
	GlFixed scale = engine->Tree()->Root()->XScale( 4 );
	const GlFixed step = 0.03;
	KrColorTransform color;

	if ( rightDormant )
	{
		--rightDormant;
		if ( rightDormant == 0 ) rightTarget = 0.3 + random.DRand( 1.2 );
	}
	else
	{
		if ( scale < rightTarget )
		{
			color.TintRed( 100 );
			scale += step;
			if ( scale > rightTarget ) scale = rightTarget;
		}
		if ( scale > rightTarget )
		{
			color.TintBlue( 100 );
			scale -= step;
			if ( scale < rightTarget ) scale = rightTarget;
		}
		if ( scale == rightTarget )
			rightDormant = 15;
	}
	KrMatrix2 matrix;
	matrix.Set( ( scale * -actor[0].sprite->X()).ToIntRound() + engine->ScreenBounds( 4 ).Width() / 2,
				( scale * (-actor[0].sprite->Y() + 100) ).ToIntRound() + engine->ScreenBounds( 4 ).Height() / 2,
				scale, scale );

	engine->Tree()->Root()->SetTransform( matrix, 4 );
	engine->Tree()->Root()->SetColor( color, 4 );
}




void BemGame::AddSpaceTiles()
{
	// This functions adds the background bitmaps. (Pictures
	// of Space.) The Space picture is a tile that is rotated
	// for variaton.

	KrResource* resource = engine->Vault()->GetResource(	KYRATAG_TILE,
															BEM_SPACE );
	GLASSERT( resource );

	KrTileResource* spaceTileRes = resource->ToTileResource();
	GLASSERT( spaceTileRes );

	int i, j;
	KrRect bounds = engine->FullScreenBounds();

	for( i=0; i<(bounds.Width() / spaceTileRes->Size() + 1); i++ )
	{
		for( j=0; j<(bounds.Height() / spaceTileRes->Size() + 1); j++ )
		{
			KrTile* spaceTile = new KrTile( spaceTileRes );
			GLASSERT( spaceTile );

			spaceTile->SetPos( i * spaceTile->Size(), j * spaceTile->Size() );
			spaceTile->SetRotation( (i+j) & 7 );

			engine->Tree()->AddNode( backgroundTree, spaceTile );
		}
	}
}


void BemGame::AddFloor()
{
	int i, j, x, y;
	KrResource* resource = engine->Vault()->GetResource( KYRATAG_SPRITE, BEM_ROOM | BEM_FLOOR );
	GLASSERT( resource );
	KrSpriteResource* spriteRes = resource->ToSpriteResource();
	GLASSERT( spriteRes );

	// The map is backwards. It is reference [y][x] and is upside
	// down to boot. *sigh*
	for( i=0; i<MAPX; i++ )
	{
		for( j=0; j<MAPY; j++ )
		{
			if ( GetMap( i, j ) == FLOOR )
			{
				KrSprite* sprite = new KrSprite( spriteRes );
				isoMath->TileToScreen( i, j, 0, &x, &y );
				sprite->SetPos( x, y );

				sprite->SetActionRotated( "NONE", ((i+j)%4) * 90 );

				// So much for abstraction.
				if ( ( j == 2 || j == 9 ) &&
					 ( i == 4 || i == 7 ) )
					sprite->SetFrame( 1 );
				else
					sprite->SetFrame( 0 );
				engine->Tree()->AddNode( floorTree, sprite );
			}
		}
	}
}


void BemGame::AddRoomObjects()
{
	// Add the alien gizmo. It's coordinates are 4.5 x 4.5,
	// so there are some sleazy tricks in this function.
	//
	// Note that in an iso tile engine, like this one,
	// the z depth is proportional to the screen y of the
	// center of the tile.
	//
	KrSpriteResource* resource = engine->Vault()->GetSpriteResource( BEM_ROOM | BEM_PLASMA_PLANT );
	GLASSERT( resource );
	
	gizmo = new KrSprite( resource );
	int x0, y0, x1, y1;

 	isoMath->TileToScreen( (MAPX-1)/2, (MAPY-1)/2, 0, &x0, &y0 );
 	isoMath->TileToScreen( MAPX/2, MAPY/2, 0, &x1, &y1 );
	
	gizmo->SetPos( (x0+x1)/2, (y0+y1)/2 );

	GlFixed tx, ty, wx, wy, wz;

	tx = MAPX-1;
	tx /= 2;
	ty = MAPY-1;
	ty /= 2;

	isoMath->TileToWorld( tx, ty, 0, &wx, &wy, &wz );
	gizmo->SetZDepth( -wy.v );
// 	gizmo->SetZDepth( (y0+y1)/2 );
	engine->Tree()->AddNode( standingTree, gizmo );
}


void BemGame::AddActors( bool useWindows )
{
	int x, y;
	numActors = 0;
	KrSpriteResource* resourceDrone = engine->Vault()->GetSpriteResource( BEM_DRONE );
	KrSpriteResource* resourceBrain = engine->Vault()->GetSpriteResource( BEM_BRAIN );

	for ( int i=0; i<MAPX; i++ )
	{
		for( int j=0; j<MAPY; j++ )
		{
			if ( GetMap( i, j ) == FLOOR && random.Rand( 5 ) == 0 )
			{
				actor[numActors].mapX = i;
				actor[numActors].mapY = j;
				
				actor[numActors].dir = -1;
				while ( actor[numActors].dir == -1 )
				{
					switch( random.Rand( 4 ) )
					{
						case 0: 
							if ( GetMap( i, j+1 ) == FLOOR )
								actor[numActors].dir = NORTH;
							break;

						case 1:
							if ( GetMap( i, j-1 ) == FLOOR )
								actor[numActors].dir = SOUTH;
							break;

						case 2:
							if ( GetMap( i+1, j ) == FLOOR )
								actor[numActors].dir = EAST;
							break;

						case 3:
							if ( GetMap( i-1, j ) == FLOOR )
								actor[numActors].dir = WEST;
							break;
					}
				}

				if ( numActors%4 )
					actor[numActors].sprite = new KrSprite( resourceDrone );
				else
					actor[numActors].sprite = new KrSprite( resourceBrain );
				
				isoMath->TileToScreen( actor[numActors].mapX, actor[numActors].mapY, 0, &x, &y );

				actor[numActors].sprite->SetAction( dirMap[ actor[numActors].dir ] );
				actor[numActors].sprite->SetPos( x, y );
				actor[numActors].sprite->SetZDepth( y );

				engine->Tree()->AddNode( standingTree, actor[numActors].sprite );
				numActors++;
			}
		}
	}
}


bool BemGame::NextMap( int dir, int x, int y, int* nx, int* ny )
{	
	*nx = x;
	*ny = y;

	switch ( dir )
	{
		case NORTH:
			*ny = y+1;
			break;
		case SOUTH:
			*ny = y-1;
			break;
		case EAST:
			*nx = x+1;
			break;
		case WEST:
			*nx = x-1;
			break;
		default:
			GLASSERT( 0 );
	}
	return map[*ny][*nx] != 0;
}


void BemGame::ProcessMap( Actor* act )
{
	// Check the current map location and calculate the next.
	// Turn if we need to.
	int nX, nY;

	// Advance the x and y to reflect current map coordinates:
	switch ( act->dir )
	{
		case NORTH:
			act->mapY++;
			break;
		case SOUTH:
			act->mapY--;
			break;
		case EAST:
			act->mapX++;
			break;
		case WEST:
			act->mapX--;
			break;
		default:
			GLASSERT( 0 );
	}

	// See where we are going next:
	int sign = 1;
	if ( tick & 0x01 ) sign = -1;
	while ( !NextMap( act->dir, act->mapX, act->mapY, &nX, &nY ) )
	{
		act->dir += sign;
		act->dir &= 3;
	}
		
	act->sprite->SetAction( dirMap[ act->dir ] );
}


float BemGame::DistanceFromCenter( Actor* act )
{
	GlFixed x, y;

	isoMath->ScreenToFlatTile( act->sprite->X(), actor->sprite->Y(),
							   0, &x, &y );
	return sqrt(   (x.ToDouble()-double(MAPX)/2.0) * (x.ToDouble()-double(MAPX)/2.0)
	             + (y.ToDouble()-double(MAPY)/2.0) * (y.ToDouble()-double(MAPY)/2.0) );
}


void BemGame::DrawMiniMapBox( KrCanvasResource* cres, KrRGBA color, int x, int y, int w, int h )
{
	for( int i=0; i<w; ++i )
	{
		for( int j=0; j<h; ++j )
		{
			*( cres->Pixels() + ( y + j ) * cres->Width() + i + x ) = color;
		}
	}
}


void BemGame::DrawMiniMap()
{
	int k, m, n;

	KrResource* res = engine->Vault()->GetResource( KYRATAG_CANVAS, "minimap" );
	KrCanvasResource* cres = res->ToCanvasResource();
	GLASSERT( cres );

	KrRGBA* target = cres->Pixels();
	int factorX = cres->Width()  / MAPX;
	int factorY = cres->Height() / MAPY;

	memset( target, 0, sizeof( KrRGBA ) * cres->Height() * cres->Width() );
	
	// Draw the floor tiles on the map.

	for( m=0; m<MAPX; m++ )
	{
		for( n=0; n<MAPY; n++ )
		{
			if ( GetMap( m, MAPY - 1 - n ) == FLOOR ) // The map is stored upside down.
			{
				KrRGBA color;
				color.Set( 0, 0, 255, 255 );

				int x = factorX * m;
				int y = factorY * n;
				
				DrawMiniMapBox( cres, color, 
								x+1, y+1, 
								factorX-1, factorY-1 );
			}
		}
	}

	// Draw a box for each actor:
	for( k=0; k<numActors; k++ )
	{
		// Draw a box for each actor.
		KrRGBA color;
		if ( actor[k].sprite->SpriteResource()->ResourceName() == "DRONE" )
			color.Set( 255, 0, 0, 180 );
		else
			color.Set( 0, 255, 0, 180 );

		int x = factorX * actor[k].mapX;
		int y = factorY * ( MAPY - actor[k].mapY - 1);	// The map is stored upside down.

		DrawMiniMapBox( cres, color,
						x+1, y+1, 
						factorX-1, factorY-1 );

	}
	// Remember we have to refresh a canvas after we draw!
	cres->Refresh();	
}


void BemGame::DrawFrame()
{
	AddText( engine );
	int i;

	// The whole demo works because it takes exactly 12 ticks for a 
	// Drone or Brain to walk one tile. So every 12 subticks
	// we calculate a new direction for the actor.
	subtick++;
	tick++;
	teleFrame++;

	if ( subtick == SUBTICK )
	{
		subtick = 0;
		for ( i=0; i<numActors; i++ )
		{
			ProcessMap( &actor[i] );
		}
	}

	// calculate the tinting, from map 4.5,4.5
 	float rad = float( tick ) / 45.0;
 	float fred   = fabs( sin( rad*1.5 ) ) * 0.5;
 	float fgreen = fabs( sin( rad*2.5 ) ) * 0.5;
 	float fblue  = fabs( sin( rad*3.5 ) ) * 0.5;
 	float falpha = fabs( sin( rad ) ) * 0.3;
	
	KrColorTransform gizmoColor;
	gizmoColor.TintRed(   U8( fred * 255.0 ));
	gizmoColor.TintGreen( U8( fgreen * 255.0 ));
	gizmoColor.TintBlue(  U8( fblue * 255.0 ));
	gizmoColor.TintAlpha( U8( falpha * 255.0 ));
	gizmo->SetColor( gizmoColor );

	// update all the actors.
	for( i=0; i<numActors; i++ )
	{
		KrColorTransform actorColor;
		float d = DistanceFromCenter( &actor[i] );
		if ( d < 3.0 )
		{
			float fraction = 1.0 - d / 3.0;
			actorColor.TintRed(   U8( fred * 255.0 * fraction ));
			actorColor.TintGreen( U8( fgreen * 255.0 * fraction ));
			actorColor.TintBlue(  U8( fblue * 255.0 * fraction ));
			actorColor.TintAlpha( U8( falpha * 255.0 * fraction ));
		}
		actor[i].sprite->SetColor( actorColor );

		actor[i].sprite->DoStep();

		// In order to sort with the particles, sort with
		// the world Y.
		GlFixed wx, wy, wz, tx, ty;

		isoMath->ScreenToFlatTile(	actor[i].sprite->X(), 
									actor[i].sprite->Y(),
									0, &tx, &ty );
		isoMath->TileToWorld( tx, ty, 0, &wx, &wy, &wz );
		actor[i].sprite->SetZDepth( -wy.v );
	}

	// The teleport in and out special effect. Done "by hand"
	// counting frames with a switch. Looks cool and tests
	// the visibility stuff.
	if ( teleFrame > 9 && numActors > 0 )
	{
		KrColorTransform cform;

		switch ( teleFrame )
		{
			case 10:
				teleSprite = actor[ random.Rand( numActors ) ].sprite;
				cform.Brighten( 128 );
				break;

			case 11:
				cform.Brighten( 200 );
				cform.SetAlpha( 220 );
				break;

			case 12:
			case 13:
				cform.Brighten( 255 );
				cform.SetAlpha( 200 );
				break;

			case 14:
				{
					GlFixed x, y;
					isoMath->ScreenToFlatTile(	teleSprite->X(),
												teleSprite->Y(),
												0, &x, &y );
					AddParticles( x, y );
					teleSprite->SetVisible( false );
				}
				break;

			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
				teleSprite->SetVisible( true );
				cform.TintBlue( 240 - 30 * ( teleFrame - 35 ) );
				cform.SetAlpha( 100 + 20 * ( teleFrame - 35 ) );
				break;
				
			case 41:
				teleFrame = 0;
				break;

			default:
				break;
		}
		teleSprite->SetColor( cform );
	}

	MoveParticles();

	// Since the map coordinates only change if the subtick rolled
	// over, we only change the mini map every 12 frames.
	if ( subtick == 0 )
		DrawMiniMap();

	if ( UseWindows() )
		ProcessRightWindow();

	engine->Draw();
}


void BemGame::AddParticles( GlFixed x, GlFixed y )
{
	KrSpriteResource* res = engine->Vault()->GetSpriteResource( BEM_ROOM | BEM_PARTICLE );
	GLASSERT( res );

	double theta = 0.0;
	const double PI = 3.1415926535897932384626433832795;
	const int N = 10;
	const int M = 4;
	const double a = 0.5;
	const double b = 0.02;
	const double FRACTION = 0.5;

	Particle particle;
		
	for(	theta = random.DRand( 2.0 * PI ) / double( 2 * N );	
			theta <= 2.0 * PI;
			theta += random.DRand( 2.0 * PI ) / double( N ) )
	{
 		particle.x = x.ToDouble() + a * sin( theta );
		particle.y = y.ToDouble() + a * cos( theta );
		particle.z = a;

		particle.vx = b * sin( theta );
		particle.vy = b * cos( theta );
		particle.vz = 0.1 + random.DRand( 0.01 );

		particle.sprite = new KrSprite( res );
		engine->Tree()->AddNode( standingTree, particle.sprite );
		particle.sprite->SetAction( BEM_ROUND );

		int sx, sy;
		isoMath->TileToScreen( particle.x, particle.y, particle.z,
							   &sx, &sy );

		particle.sprite->SetPos( sx, sy );
		particleList.PushFront( particle );
	}

	for(	theta = random.DRand( 2.0 * PI ) / double( 2 * M );	
			theta <= 2.0 * PI;
			theta += random.DRand( 2.0 * PI ) / double( M ) )
	{
 		particle.x = x.ToDouble() + a * sin( theta ) * FRACTION;
		particle.y = y.ToDouble() + a * cos( theta ) * FRACTION;
		particle.z = a / FRACTION;

		particle.vx = b * sin( theta );
		particle.vy = b * cos( theta );
		particle.vz = 0.1;

		particle.sprite = new KrSprite( res );
		engine->Tree()->AddNode( standingTree, particle.sprite );
		particle.sprite->SetAction( BEM_OBLONG );

		int sx, sy;
		isoMath->TileToScreen( particle.x, particle.y, particle.z,
							   &sx, &sy );

		particle.sprite->SetPos( sx, sy );
		particleList.PushFront( particle );
	}
}


void BemGame::MoveParticles()
{
//	const GlFixed g = -0.003;
	const int alphaDelta = 3;

	GlFixed half;
	half.v = GlFixed_1 / 2;

//	GlFixed oldz;
	
	Particle* particle;
	GlCircleListIterator<Particle> it( particleList );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		particle = &it.Current();

		particle->x += particle->vx;
		particle->y += particle->vy;

// 		oldz = particle->z;
		particle->z += particle->vz;
//  		particle->vz += g;

		GlFixed mapx = particle->x + half;
		GlFixed mapy = particle->y + half;
		int tx = mapx.ToInt();
		int ty = mapy.ToInt();

		// Check for gizmo hit:
		if ( GetMap( tx, MAPY - 1 - ty ) == GIZMO )
		{	
			engine->Tree()->DeleteNode( particle->sprite );
			it.Remove();
			it.Prev();
			continue;
		}

		int sx, sy;
		isoMath->TileToScreen( particle->x, particle->y, particle->z,
							   &sx, &sy );

		particle->sprite->SetPos( sx, sy );

		GlFixed x, y, z;
		isoMath->TileToWorld(	particle->x, particle->y, particle->z,
								&x, &y, &z );
		particle->sprite->SetZDepth( -z.v );

		KrColorTransform color;
		color = particle->sprite->CTransform();
		
		// Get rid of off screen:
		if (  color.Alpha() <= alphaDelta
			 || sx < 0 || sy < 0 
		     || sx > engine->FullScreenBounds().xmax
			 || sy > engine->FullScreenBounds().ymax )
		{
			engine->Tree()->DeleteNode( particle->sprite );
			it.Remove();
			it.Prev();
			continue;
		}
		color.SetAlpha( color.Alpha() - alphaDelta );
		particle->sprite->SetColor( color );
	}
}
