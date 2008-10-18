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
#include "space.h"
#include "kyra.h"


ShooterGame::ShooterGame() 
	: BLUESCALE( 0.7 ),
	  SHIELD_X_SCALE( 10 ),
	  SHIELD_Y_SCALE( 2.5 )
{
	engine = 0;
	for( int i=0; i<NUMBLOCKS; ++i )
		blockRes[i] = 0;
}

void ShooterGame::Init()
{
	idcount = 0;
	shieldDir = 1;
	rand.SetSeed( 9 );

	bool ret = engine->Vault()->LoadDatFile( "space.dat" );
	if ( !ret )
	{
		#ifdef DEBUG
		GLOUTPUT( "Error loading space.dat\n" );
		GLASSERT( 0 );
		#endif
		return;
	}

	if ( scale != 1 )
	{
		// Wasteful, but tests functionality.
		engine->Vault()->CacheScale( scale, scale );
	}

	monsters = new KrImNode();
	engine->Tree()->AddNode( 0, monsters );
	bullets = new KrImNode();
	engine->Tree()->AddNode( 0, bullets );
	walls = new KrImNode();
	engine->Tree()->AddNode( 0, walls );

	KrSpriteResource* shooterRes = engine->Vault()->GetSpriteResource( SPACE_TRAIN );
	GLASSERT( shooterRes );

	KrSpriteResource* blueShipRes = engine->Vault()->GetSpriteResource( SPACE_MED );
	GLASSERT( blueShipRes );
	blueShipRes->CacheScale( scale*BLUESCALE, scale*BLUESCALE );

	shooter = new KrSprite( shooterRes );
	shooter->SetPos( 50, SCREENY - 40, mainWindow );
	shooter->SetAction( SPACE_ENGINE );
	engine->Tree()->AddNode( 0, shooter );
	SetName( "shooter", shooter );

	destinationX = rand.Rand() % SCREENX;

//	KrRGBA red;
//	red.Set( 255, 0, 0 );
//	KrBoxResource* boxRes = new KrBoxResource( "outline", 640, 480, &red, 1, KrBoxResource::OUTLINE );
//	engine->Vault()->AddResource( boxRes );
//	KrBox* box = new KrBox( boxRes );
//	engine->Tree()->AddNode( 0, box );
	
	AddBlocks();
	AddShield();
}


ShooterGame::~ShooterGame()
{
	delete engine;
	for( int i=0; i<NUMBLOCKS; ++i )
	{
		delete blockRes[i];
	}
}


void ShooterGame::AddBlocks()
{
	if ( !useShield ) return;

	for( int i=0; i<NUMBLOCKS; ++i )
	{
		blockRes[i] = new KrCanvasResource( "block", BLOCKWIDTH, BLOCKHEIGHT, true );
		
		int size = blockRes[i]->Width() * blockRes[i]->Height();
		KrRGBA* cp = blockRes[i]->Pixels();
		for( int j=0; j<size; ++j )
		{
			cp->c.red   = 0;
			cp->c.green = 150 + j % BLOCKHEIGHT;
			cp->c.blue  = j % 169;
			cp->c.alpha = 255;
			++cp;
		}

		blockRes[i]->Refresh();
		block[i] = new KrCanvas( blockRes[i] );
		SetName( "block", block[i] );
		block[i]->SetPos( SCREENX * (i+1) / (NUMBLOCKS+1) - blockRes[i]->Width() / 2,
						  SCREENY * 2 / 3,
						  mainWindow );

		engine->Tree()->AddNode( walls, block[i] );
	}
}


void ShooterGame::AddShield()
{
	if ( !useShield ) return;

	// The shield doesn't do much, except prove that scaled canvases work.
	shieldRes = new KrCanvasResource( "shield", SHIELDSIZE, SHIELDSIZE, true );
	engine->Vault()->AddResource( shieldRes );

	KrPaintInfo info( shieldRes->Pixels(), SHIELDSIZE, SHIELDSIZE );
	KrPainter painter( &info );

	for( int i=0; i<SHIELDSIZE/2; ++i )
	{
		painter.DrawVLine( i, i, SHIELDSIZE - i*2, 255, 0, 0 );
		painter.DrawVLine( SHIELDSIZE-1-i, i, SHIELDSIZE - i*2, 255, 0, 0 );
	}
	shieldRes->Refresh();

	shield = new KrCanvas( shieldRes );
	SetName( "shield", shield );
	engine->Tree()->AddNode( 0, shield );
	shield->SetPos( SCREENX / 2,
					SCREENY * 2 / 3- ( SHIELD_Y_SCALE * SHIELDSIZE + 5 ).ToInt(),
					mainWindow );
	shield->SetScale( SHIELD_X_SCALE, SHIELD_Y_SCALE, mainWindow );
	shield->SetQuality( KrQualityLinear );
}


void ShooterGame::MoveShield()
{
	if ( !useShield ) return;

	KrColorTransform cForm = shield->CTransform( mainWindow );

	if ( cForm.b.c.blue != 0 )
	{
		cForm.TintBlue( GlMax( cForm.b.c.blue - 20, 0 ) );
		shield->SetColor( cForm );
	}
	else
	{
		shield->SetPos( shield->X( mainWindow ) + shieldDir * SHIELDSPEED, shield->Y( mainWindow ), mainWindow );
		if ( shield->X( mainWindow ) + SHIELD_X_SCALE * SHIELDSIZE >= block[ NUMBLOCKS-1 ]->X( mainWindow ) + BLOCKWIDTH )
			shieldDir = -1;
		if ( shield->X( mainWindow ) < block[0]->X( mainWindow ) )
			shieldDir = 1;
	}
}


void ShooterGame::HitBlock( KrImage* hit, KrImage* bullet )
{
	KrResource* res = hit->Resource();
	KrCanvasResource* canRes = res->ToCanvasResource();
	GLASSERT( canRes );

	// Where was the block hit?
	KrRect rect = hit->Bounds( 0 );
	rect.DoIntersection( bullet->Bounds( 0 ) );
	rect.EdgeAdd( 8 );
	rect.DoIntersection( hit->Bounds( 0 ) );

	rect.xmax -= hit->Bounds( 0 ).xmin;
	rect.xmin -= hit->Bounds( 0 ).xmin;
	rect.ymax -= hit->Bounds( 0 ).ymin;
	rect.ymin -= hit->Bounds( 0 ).ymin;

	// Now set the resource to red. We'll fade later.
	for( int j=rect.ymin; j<=rect.ymax; ++j )
	{
		KrRGBA* pixels = canRes->Pixels() + j*canRes->Width();
		for ( int i=rect.xmin; i<=rect.xmax; ++i )
		{
//			if ( pixels[i].c.red == 0 )
//			{
//				pixels[i].c.red = pixels[i].c.green;
//				pixels[i].c.blue = pixels[i].c.green / 2;
//				pixels[i].c.green = 0;
//			}
			pixels[i].all = 0;
		}
	}
	canRes->Refresh();
}


KrSprite* ShooterGame::AddMonsters()
{
	if ( mobCount < MAXMOB && rand.Rand( 400 ) < 150 )
	{
		KrSpriteResource* mobRes = 0;
		GlFixed localScale = 1;
		int action = SPACE_NONE;
		
		if ( rand.Rand() % 3 == 0 )
		{
			mobRes = engine->Vault()->GetSpriteResource( SPACE_MED );
			localScale = BLUESCALE;
			action = SPACE_BODY;
		}
		else
		{
			mobRes = engine->Vault()->GetSpriteResource( SPACE_SMALL );
		}

		GLASSERT( mobRes );
		KrSprite* mob = new KrSprite( mobRes );
		mob->SetAction( action );
		mob->SetScale( localScale, localScale, mainWindow );
		SetName( "mob", mob );

		mob->SetPos( SCREENX + 80, 
					 50 + rand.Rand( SCREENY / 2 - ( SHIELD_Y_SCALE * SHIELDSIZE ).ToInt() ),
					 mainWindow );
		engine->Tree()->AddNode( monsters, mob );

		return mob;
	}
	return 0;
}


void ShooterGame::MoveMonsters()
{
	// How many mobs (term for a monster or enemy)
	mobCount = 0;

	GlInsideNodeIt<KrImNode*> it = monsters->ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		if ( it.CurrentData()->X( mainWindow ) < -30 )
		{
			// If we're off the screen, delete.
			KrImNode* node = it.CurrentData();
			it.Next();
			StatusRemove( node->ToImage() );
			engine->Tree()->DeleteNode( node );
			continue;
		}
		else if ( (int) it.CurrentData()->GetUserData() == GLOW )
		{
			// If we've been hit, change the color transform. Delete
			// when it whites out.
			KrColorTransform cForm = it.CurrentData()->CTransform( mainWindow );
			if ( cForm.b.c.red > 240 )
			{
				KrImNode* node = it.CurrentData();
				it.Next();
				StatusRemove( node->ToImage() );
				engine->Tree()->DeleteNode( node );
				continue;
			}
			else
			{
				cForm.Brighten( cForm.b.c.red + 5 );
				it.CurrentData()->SetColor( cForm );
			}	
		}
		++mobCount;
		it.Next();
	}

	// Move everything that hasn't been hit.
	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( (int) it.CurrentData()->GetUserData() != GLOW )
		{
			it.CurrentData()->SetPos( it.CurrentData()->X( mainWindow ) - MOBDELTA, it.CurrentData()->Y( mainWindow ), mainWindow );
		}
	}
}


void ShooterGame::MoveBullets()
{
	GlInsideNodeIt<KrImNode*> it = bullets->ChildTreeIterator();
	for( it.Begin(); !it.Done(); it.Next() )
	{
		int dir = (int) it.CurrentData()->GetUserData();
		int dx = 0;
		switch( dir )
		{
			case LEFT:
				dx = -BULLETDELTA;
				break;
			case RIGHT:
				dx = BULLETDELTA;
				break;
		}

		// Move the bullet up, unless its hit something. If its hit something,
		// color transform it.
		if ( dir != GLOW )
		{
			it.CurrentData()->SetPos(	it.CurrentData()->X( mainWindow ) + dx, 
										it.CurrentData()->Y( mainWindow ) - BULLETSPEED,
										mainWindow );
		}
		else
		{	
			KrColorTransform cForm = it.CurrentData()->CTransform( mainWindow );
			cForm.Brighten( cForm.b.c.red + 10 );
			it.CurrentData()->SetColor( cForm );
		}
	}

	// Get rid of stuff that has gone white out, or is off the screen.
	it = bullets->ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		KrColorTransform cForm = it.CurrentData()->CTransform( mainWindow );
		if ( cForm.b.c.red > 240 || it.CurrentData()->Y( mainWindow ) < -30 )
		{
			KrImNode* node = it.CurrentData();
			it.Next();
			engine->Tree()->DeleteNode( node );
		}
		else
		{
			it.Next();
		}
	}
}


void ShooterGame::MoveShooter()
{
	// Move the shooter at the bottom.
	if ( abs( shooter->X( mainWindow ) - destinationX ) <= XSPEED )
	{
		destinationX = rand.Rand() % SCREENX;

		// Shoot!
		KrSpriteResource* bulletRes = engine->Vault()->GetSpriteResource( SPACE_BULLET );
		GLASSERT( bulletRes );

		KrSprite* bulletLeft = new KrSprite( bulletRes );
		bulletLeft->SetAction( SPACE_SPHERE );
		SetName( "bulletLeft", bulletLeft );

		KrSprite* bulletCenter = new KrSprite( bulletRes );
		bulletCenter->SetAction( SPACE_OBLONG );
		SetName( "bulletCenter", bulletCenter );

		KrSprite* bulletRight = new KrSprite( bulletRes );
		bulletRight->SetAction( SPACE_SPHERE );
		SetName( "bulletRight", bulletRight );

		bulletLeft->SetPos(   shooter->X( mainWindow ) - 10, shooter->Y( mainWindow ) - 40, mainWindow );	//10, 40
		bulletCenter->SetPos( shooter->X( mainWindow ) - 0,  shooter->Y( mainWindow ) - 40, mainWindow );
		bulletRight->SetPos(  shooter->X( mainWindow ) + 10, shooter->Y( mainWindow ) - 40, mainWindow );

		bulletLeft->SetUserData( (void*) LEFT );
		bulletCenter->SetUserData( (void*) CENTER );
		bulletRight->SetUserData( (void*) RIGHT );

		bulletLeft->SetVisible( false );	bulletLeft->SetVisible( true, mainWindow );
		bulletCenter->SetVisible( false );	bulletCenter->SetVisible( true, mainWindow );
		bulletRight->SetVisible( false );	bulletRight->SetVisible( true, mainWindow );

		engine->Tree()->AddNode( bullets, bulletLeft );
		engine->Tree()->AddNode( bullets, bulletCenter );
		engine->Tree()->AddNode( bullets, bulletRight );
	}
	else
	{
		int dx = XSPEED;
		if ( destinationX < shooter->X( mainWindow ) )
			dx = -dx;
		shooter->SetPos( shooter->X( mainWindow ) + dx, shooter->Y( mainWindow ), mainWindow );
	}	
}


void ShooterGame::SetName( const char* desc, KrImNode* node )
{
	char buf[ 256 ];

	sprintf( buf, "%s.%d", desc, ++idcount );
	node->SetNodeName( buf );
}


void ShooterGame::DrawFrame()
{
	MoveMonsters();
	MoveBullets();
	MoveShooter();
	MoveShield();
	StatusMove();

	KrSprite* newMonster = AddMonsters();

	// Flush the changes to this point and then look for
	// collisions between the bullets and everything else.
	engine->Tree()->Walk();

	GlDynArray< KrImage* > hit;
	// Can we keep the monster we just added -- or did it collide?
	if ( newMonster && engine->Tree()->CheckSiblingCollision( newMonster, &hit, mainWindow ) )
	{
		engine->Tree()->DeleteNode( newMonster );
		newMonster = 0;
	}
	else if ( newMonster )
	{
		// This add will not be collision detected.
		StatusAdd( newMonster );
	}

	StatusCollide();

	// Check for collisions.
	GlInsideNodeIt<KrImNode*> it = bullets->ChildTreeIterator();
	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( (int) it.CurrentData()->GetUserData() != GLOW )
		{
			bool anyHit = false;
			KrColorTransform cForm;
			cForm.Brighten( 150 );

			// Have we hit a monster?
			bool monsterHit = engine->Tree()->CheckChildCollision(	it.CurrentData(),
																	monsters,
																	&hit,
																	mainWindow );

			if ( monsterHit )
			{
				for ( int i=0; i<(int)hit.Count(); ++i )
				{
					if ( hit[i]->GetUserDataU32() != GLOW )
					{
						anyHit = true;
						//GLOUTPUT( "Hit: %s frm=%d\n", hit[i]->NodeName().c_str(), Frame() );
						hit[i]->SetColor( cForm );
						hit[i]->SetUserData( (void*) GLOW );
					}
				}
			}

			if ( useShield )
			{
				// How about a block?
	//			GLOUTPUT( "Bullet: '%s' [%d,%d]-[%d,%d]\n", it.CurrentData()->NodeName().c_str(),
	//					   it.CurrentData()->Bounds( 0 ).xmin,
	//					   it.CurrentData()->Bounds( 0 ).ymin,
	//					   it.CurrentData()->Bounds( 0 ).xmax,
	//					   it.CurrentData()->Bounds( 0 ).ymax );
				bool blockHit = engine->Tree()->CheckChildCollision(	it.CurrentData(),
																		walls,
																		&hit,
																		mainWindow );

				if ( blockHit )
				{
					for ( int i=0; i<(int)hit.Count(); ++i )
					{
						anyHit = true;			
						HitBlock( hit[i], it.CurrentData()->ToImage() );
					}
				}

				// And finally the shield.
				bool shieldHit = shield->CheckCollision( it.CurrentData()->ToImage(), mainWindow );
				if ( shieldHit )
				{
					anyHit = true;
					KrColorTransform blue;
					blue.TintBlue( 200 );
					shield->SetColor( blue );
				}
			}

			// End the shot if it hit anything.
			if ( anyHit )
			{
				it.CurrentData()->SetColor( cForm );
				it.CurrentData()->SetUserData( (void*) GLOW );
			}
		}
	}
	engine->Draw();
}


void ShooterGame::StatusAdd( KrImage* image )
{
	if ( useStatus )
	{
		GLASSERT( image );
		image->SetPos( STATUS_X, statusArray.Count() * STATUS_DELTAY + STATUS_Y, 0 );
		statusArray.PushBack( image );
	}
}


void ShooterGame::StatusRemove( KrImage* image )
{	
	if ( useStatus )
	{
		GLASSERT( image );
		int i = statusArray.Find( image );
		GLASSERT( i >= 0 );
		statusArray.Remove( i );
	}
}


void ShooterGame::StatusMove()
{
	if ( useStatus )
	{
		for ( int i=0; i<(int)statusArray.Count(); ++i )
		{
			int yWanted = i * STATUS_DELTAY + STATUS_Y;

			if ( abs( statusArray[i]->Y() - yWanted ) <= STATUS_MOVE )
			{
				statusArray[i]->SetPos( statusArray[i]->X(), yWanted, 0 );
			}
			else
			{
				int bias = yWanted < statusArray[i]->Y() ? -1 : 1;
				statusArray[i]->SetPos( statusArray[i]->X(), statusArray[i]->Y() + bias * STATUS_MOVE, 0 );
			} 
		}
	}
}


void ShooterGame::StatusCollide()
{
	if ( useStatus )
	{
		for ( int i=0; i<(int)statusArray.Count(); ++i )
		{
			KrImage* image = statusArray[i];

			GlDynArray< KrImage* > hit;
			KrColorTransform color = image->CTransform( 0 );

			if ( engine->Tree()->CheckSiblingCollision( image, &hit, 0 ) )
				color.SetAlpha( 150 );
			else
				color.SetAlpha( 255 );

			image->SetColor( color, 0 );
		}
	}
}


SimpleShooter::SimpleShooter( SDL_Surface* screen )
	: ShooterGame()
{
	scale = 1;
	mainWindow = 0;
	useShield = true;
	useStatus = false;

	engine = new KrEngine( screen );
	Init();
}


WindowShooter::WindowShooter( SDL_Surface* screen ) 
	: ShooterGame()
{
	scale = 0.75;
	mainWindow = 1;
	useShield = false;
	useStatus = true;

	const int SCREENX1 = 140;
	const int SCREENY1 = 60;

	KrRect bounds[2];
	bounds[0].Set( 0, 0, 120, 479 );
	bounds[1].Set( SCREENX1, SCREENY1, SCREENX1 + 480, SCREENY1 + 360 );

	KrRGBA background;
	background.Set( 0, 0, 200 );

	engine = new KrEngine( screen, 2, bounds, &background );
	engine->Tree()->Root()->SetScale( scale, scale, mainWindow );

	Init();

	shooter->SetVisible( false, 0 );
}

