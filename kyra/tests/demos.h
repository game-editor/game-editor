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


#ifndef KYRA_DEMOS_INCLUDED
#define KYRA_DEMOS_INCLUDED


#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"
#include "kyra.h"


struct SDL_Surface;
class KrEngine;
class KrResourceVault;
class KrImNode;
class KrSprite;
class KrCanvas;
class KrTextBox;
class KrBox;
class KrResourceVault;
class GlIsoMath;
class KrCanvasResource;
class KrConsole;


// The base class for all the tests.
class GameTest
{
  public:
	GameTest()				{ frame = 0; textAdded = false; }
	virtual ~GameTest()		{}

	void UpperDrawFrame()	{	DrawFrame();
								++frame;
							}
	int Frame()				{	return frame; }
	virtual void VideoExpose() = 0;
	
	virtual const char* Name() = 0;
	virtual void DrawFrame() = 0;
	virtual int  TestFrames() = 0;
	virtual int  FrameInterval()			{ return 80; }

//	// All the games share the fonts:
//	static void SetFontVault( KrResourceVault* vault )	{	GLASSERT( vault );
//															fontVault = vault; }
  
  protected:
//	static KrResourceVault* fontVault;
	void   AddText( KrEngine* engine );

  private:
	int frame;
	bool textAdded;
};


// Opening title.
class TitleScreen : public GameTest
{
  public:
	TitleScreen( SDL_Surface* screen );
	virtual ~TitleScreen();

	virtual const char* Name()		{ return "TitleScreen"; }

	virtual void DrawFrame();
	virtual int  TestFrames() { return 100; }

	virtual void VideoExpose() { engine->InvalidateScreen(); }
  
  private:
	enum
	{
		THRESH0 = 30,
		THRESH1 = 70
	};
	KrEngine*	engine;
	KrImNode*	parent;
	KrResourceVault*	vault;		// extra vault for the font
	KrTextBox*			text;
};


// A 2D shooter demo, primarily to test collision detection.
class ShooterGame : public GameTest
{
  public:
	ShooterGame();
	virtual ~ShooterGame();

	virtual void DrawFrame();
	virtual int  TestFrames()		{ return 400; }
	virtual int  FrameInterval()	{ return 60; }

	virtual void VideoExpose() { engine->InvalidateScreen(); }
  
  protected:
	void Init();
	void AddShield();
	void StatusAdd( KrImage* );
	void StatusRemove( KrImage* );
	void StatusCollide();

	GlDynArray< KrImage* > statusArray;
	KrSprite*	shooter;

	// Set by children:
	KrEngine*	engine;
	GlFixed		scale;
	int			mainWindow;
	bool		useShield;
	bool		useStatus;

	enum
	{
		STATUS_MOVE = 5,
		STATUS_Y = 40,
		STATUS_DELTAY = 60,
		STATUS_X = 80
	};

  private:
	KrSprite* AddMonsters();
	void MoveMonsters();
	void MoveBullets();
	void MoveShooter();
	void AddBlocks();
	void HitBlock( KrImage *hit, KrImage *bullet );
	void MoveShield();
	void StatusMove();
	//void ProcessBlocks();

	void SetName( const char* desc, KrImNode* );

	enum
	{
		LEFT,
		RIGHT,
		CENTER,
		GLOW,

		NUMBLOCKS = 3,
		BLOCKWIDTH = 100,	// Size of the blocks at the bottom of the screen
		BLOCKHEIGHT = 30,
	
		SHIELDSIZE = 10,	// The size of the shield, unscaled.
		SHIELDSPEED = 3,

		XSPEED = 7,				// speed of shooter
		MAXMOB = 20,			// max # of monsters
		MOBDELTA = 3,			// speed of monsters
		BULLETSPEED = 11,
		BULLETDELTA = 3,

		SCREENX = 640,
		SCREENY = 480
	};

	const GlFixed BLUESCALE;		// scaling of the blue ship
	const GlFixed SHIELD_X_SCALE;	// shield scaling (10x, 1.5y)
	const GlFixed SHIELD_Y_SCALE;

	KrImNode*	monsters;	// A node to contain all the target ships
	KrImNode*	bullets;
	KrImNode*	walls;
	KrCanvasResource*	blockRes[ NUMBLOCKS ];
	KrCanvasResource*	shieldRes;
	KrCanvas*			block[ NUMBLOCKS ];
	KrCanvas*			shield;

	int			destinationX;
	int			idcount;
	int			mobCount;
	int			shieldDir;

	GlRandom	rand;
};


class SimpleShooter : public ShooterGame
{
  public:
	SimpleShooter( SDL_Surface* screen );
	virtual ~SimpleShooter()										{}

	virtual const char* Name()		{ return "SimpleShooter"; }

  protected:
	virtual bool UseShield()		{ return true; }
};


class WindowShooter : public ShooterGame
{
  public:
	WindowShooter( SDL_Surface* screen );
	virtual ~WindowShooter()										{}

	virtual const char* Name()		{ return "WindowShooter"; }

  protected:
	virtual bool UseShield()		{ return false; }
};


// A demo using extra graphics from Bug Eyed Monsters. 
// (Grinning Lizard, the early years. It was a game for OS/2)
// A bunch of Drone and Brain BEM's walk around an isometric
// map. An alien thing in the middle changes colors, and
// any Drone or Brain close to it changes proportional to the
// distance from the alien thing.

class BemGame : public GameTest
{
  public:
	BemGame( SDL_Surface* screen, bool useWindows );
	virtual ~BemGame();

	virtual const char* Name() = 0;
	virtual bool UseWindows() = 0;

	virtual void DrawFrame();
	virtual int  TestFrames()		{ return 300; }
	virtual int  FrameInterval()	{ return 75; }

	virtual void VideoExpose() { engine->InvalidateScreen(); }

  private:
	enum
	{
		MAPX = 12,
		MAPY = 12,
		SUBTICK = 12,
		MAX_PARTICLE = 100,

		NORTH = 0,
		EAST,
		SOUTH,
		WEST,

		FLOOR = 1,
		GIZMO = 2
	};

	struct Actor
	{
		int	mapX, mapY;
		int dir;
		KrSprite* sprite;
	};

	struct Particle
	{
		KrSprite* sprite;
		GlFixed x, y, z;		// in tile coordinates
		GlFixed vx, vy, vz;
	};

	GlCircleList< Particle > particleList;

	// The map is upside down and backwards. This accesses it more reasonably.
	int  GetMap( int x, int y )		{	return map[y][x]; }

	void AddSpaceTiles();
	void AddFloor();
	void AddRoomObjects();
	void AddActors( bool useWindows );
	void SetupLeftWindow();
	void SetupRightWindow();
	void GetRightWindowTarget();
	void ProcessRightWindow();

	void ProcessMap( Actor* actor );
	bool NextMap( int dir, int x, int y, int* nx, int* ny );

	void DrawMiniMap();
	void DrawMiniMapBox( KrCanvasResource* cres, KrRGBA rgba, int x, int y, int w, int h );

	float DistanceFromCenter( Actor* act );

	void AddParticles( GlFixed x, GlFixed y );
	void MoveParticles();

	Actor actor[ 256 ];
	int numActors;
	int subtick, tick;

	KrEngine* engine;

	KrImNode* underTree;
	KrImNode* backgroundTree;
	KrImNode* floorTree;
	KrImNode* standingTree;
	KrImNode* overTree;

	KrSprite* gizmo;
	KrCanvas* canvas;

	int tileWidth;
	int tileHeight;
	GlRandom random;

	int teleFrame;
	KrSprite* teleSprite;

	KrBox* track;
	GlIsoMath* isoMath;

	int			rightDormant;		// frames until it scales again
	GlFixed     rightTarget;		// scaling target

	static const int map[MAPY][MAPX];
	static const int dirMap[ 4 ];
};


class BemGameNormal : public BemGame
{
  public:
  public:
	BemGameNormal( SDL_Surface* screen ) : BemGame( screen, false ) {}
	virtual ~BemGameNormal()		{}

	virtual const char* Name()	{ return "BEM demo"; }
	virtual bool UseWindows()	{ return false; }	
};


class BemGameWindowed : public BemGame
{
  public:
	BemGameWindowed( SDL_Surface* screen ) : BemGame( screen, true ) {}
	virtual ~BemGameWindowed()		{}

	virtual const char* Name()	{ return "BEM:Windowed demo"; }
	virtual bool UseWindows()	{ return true; }	
};


// A demo using a large number of sprites against a tiled space
// background. A good test of lots of things moving at once.
// Can be run in 2 modes: 1) where the camera is fixed and the
// big ship moves by, and 2) where the camera follows the big ship,
// which tests scrolling.

class SpaceGame : public GameTest
{
  public:
	SpaceGame( SDL_Surface* screen, bool trackingOn );
	virtual ~SpaceGame();

	virtual void DrawFrame();
	virtual int  TestFrames() { return 150; }
	virtual void VideoExpose() { engine->InvalidateScreen(); }

  private:
	enum {
		WIDTHFACTOR = 3,	// The width of the space field.
		MAXDEPTH = 50,		// The range of depths used.
		BIGDEPTH = 30,		// Depth of the big ship.
		NUM_MED = 20,		// Number of medium ships.
		NUM_SMALL = 70,		// Number of small ships.
		NUM_SCALES = 7,		// Number of cached scales are used, including possibly 1:1
	};

	void AddStarfield();
	void AddShips();
	void TrackCamera();
	GlFixed GetScale( int z );

	GlFixed scalesToUse[NUM_SCALES];
	
	KrEngine* engine;
	KrImNode* starfieldNode;
	KrImNode* mainNode;
	KrRect	  field;		// Size of the world
	int		  xResetAt;		// x value where the ships are moved to...
	int		  xResetTo;		// ...this value
	GlRandom  random;
	bool	  tracking;
};


class SpaceGameFixed : public SpaceGame
{
  public:
	SpaceGameFixed( SDL_Surface* screen ) : SpaceGame( screen, false ) {}
	virtual ~SpaceGameFixed()	{}
	virtual const char* Name()		{ return "Space : Fixed"; }
};
	

class SpaceGameTracking : public SpaceGame
{
  public:
	SpaceGameTracking( SDL_Surface* screen ) : SpaceGame( screen, true ) {}
	virtual ~SpaceGameTracking()	{}
	virtual const char* Name()		{ return "Space : Tracking"; }
};


// Test to make sure all the sprite blits and transforms are working.

class SpriteTest : public GameTest
{
  public:
	SpriteTest( SDL_Surface* screen );
	virtual ~SpriteTest()		{}

	virtual const char* Name()		{ return "Sprite Test"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 200; }
	virtual void VideoExpose() {}

  private:
	bool drawn;
	SDL_Surface* screen;
};


class TileTest : public GameTest
{
  public:
	TileTest( SDL_Surface* screen );
	virtual ~TileTest();

	virtual const char* Name()		{ return "Tile Test"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 240; }
	virtual void VideoExpose() {}

  private:
	enum {
		NUMCANVAS = 10,
		SCALEBOTH = 30,
		SCALEX      = 100,
		SCALEY      = 170,
	};
	KrCanvas* canvas[ NUMCANVAS ];
	bool drawn;
	SDL_Surface* screen;
	KrEngine* engine;

	KrTile* alpha[ 8 ][ 4 ];
	KrTile* noAlpha[ 8 ][ 4 ]; 
};


// Tons of text, displaying Lewis Carol's Jabberwock poem.
//
class TextTest : public GameTest
{
  public:
	TextTest( SDL_Surface* screen );
	virtual ~TextTest();

	virtual const char* Name()		{ return "Text"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 250; }
	virtual void VideoExpose() { engine->InvalidateScreen(); }

  private:
	enum {
		NUM_SHIPS = 6,
		NUM_TEXT_BOX = 5
	};
	KrEngine*			engine;
	KrResourceVault*	spaceVault;
	KrResourceVault*	dataVault;
	KrResourceVault*	fontVault;
	KrSprite*			ship[ NUM_SHIPS ];
	KrTextBox*			textBox[ NUM_TEXT_BOX ];
	KrBox*				box[NUM_TEXT_BOX];
	GlRandom			random;
	int					frame;
	int					showGlyphs;
	GlDynArray< gedString > poem;
};


// Test the scaling, consistency, "jumping"
class ScalingTest : public GameTest
{
  public:
	ScalingTest( SDL_Surface* screen );
	virtual ~ScalingTest();

	virtual const char* Name()		{ return "Scaling"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 250; }
	virtual void VideoExpose() { engine->InvalidateScreen(); }

  private:
	void AddBackground();

	KrEngine*			engine;
	KrResourceVault*	fontVault;
	KrSprite*			ship;
	KrSprite*			travellingShip;
	KrCanvas*			canvas[3];
	KrBox*				center;
	KrTextBox*			textBox;
	KrTextBox*			travelText;
	GlFixed				scaleX, scaleY;

	enum
	{
		BOTH, HORIZONTAL, VERTICAL	
	};
	int					state;
};


// Test the scaling in a more entertaining example
class SinWaveTest : public GameTest
{ 
  public:
	SinWaveTest( SDL_Surface* screen );
	virtual ~SinWaveTest();

	virtual const char* Name()		{ return "WaveScaling"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 250; }
	virtual void VideoExpose() { engine->InvalidateScreen(); }
	virtual int  FrameInterval()			{ return 60; }

  private:
	enum {
		ROWS = 3,
		MAX_HORIZONTAL_SHIP = 9,
		MAX_CARS = 5,
		PADDING = 200,
	};

	int GetY( int hIndex );
	int GetX( int vIndex );
	double CalculateScale( int y );
	int CalculateTintFromScale( double scale );
	void AddStarfield();

	GlRandom rand;

	KrEngine* engine;
	KrSprite* vertical[ ROWS ][ MAX_CARS + 2 ];
	KrSprite* horizontal[ 2 ][ MAX_HORIZONTAL_SHIP ];

};


// Test out blitters, and print results.
class TestBlitter : public GameTest
{
  public:
	TestBlitter( SDL_Surface* screen );
	virtual ~TestBlitter();

	virtual const char* Name()		{ return "Blitter"; }
	virtual void DrawFrame();
	virtual int  TestFrames() { return 1; }
	virtual void VideoExpose() { engine->InvalidateScreen(); }

  private:
	void SetUp(	KrRGBA newColorBase,
				KrRGBA surfaceColorBase,
				KrColorTransform cformBase  );

	void Run(	KrRGBA newColor,
				KrRGBA surfaceColor,
				KrColorTransform cform,
				bool sourceAlpha  );

	void CalcColorChannelFixed(	U8 newColor,
								U8 newAlpha,
								U8 surfaceColor,
								U8 cformM,
								U8 cformB,
								U8 cformAlpha,
								GlFixed* c );	

	KrEngine*			engine;
	
	enum {
		NUM_SURFACE = 3
	};
	SDL_Surface* surface[NUM_SURFACE];

	struct Test
	{
		KrRGBA surfaceColor;
		KrRGBA newColor;
		KrRGBA resultColor;
		GlFixed r, g, b;
		gedString name;
	};

	Test test[ 256 ];
	int  numTest;
	KrTextBox* textBox;
};


class GameFactory 
{
  public:
	GameFactory();

	GameTest* CreateGame( int i, SDL_Surface* screen );
	void	  DestroyGame( GameTest* );

	int       NumGames()			{ return NUM_GAMES; }
	int		  Current()				{ return current; }

	void	  SetTime( U32 ticks )	{ gameTime[current] = ticks; }
	void	  PrintTimes( SDL_Surface* screen );
	static void	  GetHeader( SDL_Surface* screen, char* buffer );

  private:
	enum {
		NUM_GAMES = 12
	};
	int current;
	U32 gameTime[ NUM_GAMES ];
	U32 gameFrames[ NUM_GAMES ];
	gedString name[ NUM_GAMES ];
};

#endif
