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


#ifndef KYRA_SHARED_STATE_INCLUDED
#define KYRA_SHARED_STATE_INCLUDED

#include "../tinyxml/tinyxml.h"
#include "SDL.h"
#include "SDL_image.h"
#include "../util/glstring.h"
#include "../engine/color.h"

class KrAction;
class KrEngine;
class KrCanvasResource;
class KrImageTree;
class KrImNode;
class KrConsole;
class EdWidget;
class EdWidgetFrame;
class EdWidgetTile;
class EdWidgetSprite;
class EdWidgetAction;
class KrFontResource;
class KrTextBox;
class KrBox;



enum
{
	INFO_CLEAR_START = 0,
	INFO_CLEAR_END = 4,
	
	INFO_SPRITE = 0,
	INFO_ACTION = 1,
	INFO_FRAME  = 2,
	INFO_FRAME_SIZE = 3,

	INFO_TILE = 0,
	INFO_TILE_SIZE = 1,

	INFO_CREATING = 4,
	INFO_MOUSE = 5,
};


class SharedStateData
{
  public:
	SharedStateData( SDL_Surface* surface );
	~SharedStateData();
	
	// Clear the widgets and filenames
	void Clear();

	void SetFilenames( const gedString& def, const gedString& surface )	{ defFileName = def; surfaceFileName = surface; }
	const gedString&	DefFileName();
	const gedString&	SurfaceFileName();
	
	/* The image that backs the screen is kept as a canvas
		resource so it can be used with the sprite engine. The 
		sprite engine is faster and it's a good test of the engine
		code.
	*/
	KrCanvasResource*	CanvasResource()		{ return canvasResource; }

	// The engine we are using:
	KrEngine* Engine()		{ return engine; }

	// The console to output to:
	KrConsole* Console()	{ return console; }

	// The console in contained in this node:
	KrImNode*  ConsoleNode()	{ return consoleHolder; }

	// The image node that states should be a child of:
	KrImNode* ImNode()		{ return imnode; }

	// The view state tosses its (considerable) widget set in
	// this node:
	KrImNode* WidgetNode()	{ return widgetNode; }

	// The base widget, once the XML stuff is parsed to widgets.
	EdWidget* Widget()		{ return widget; }

	KrFontResource*		FontResource()	{ return fontResource; }
	KrCanvasResource*	BackgroundRes()	{ return backgroundRes; }

	void SetInfoBox( bool on );
	void SetInfoBoxMouse( int x, int y );
	
	enum {
		SPRITE,
		TILE
	};

	int				CurrentObject()		{ return currentObject; }
	EdWidgetFrame*	CurrentFrame()		{ return currentFrame; }
	EdWidgetAction*	CurrentAction()		{ return currentAction; }
	EdWidgetSprite*	CurrentSprite()		{ return currentSprite; }
	EdWidgetTile*	CurrentTile()		{ return currentTile; }

	// Sets both the current tile/sprite and switches to that mode.
	void SetCurrentTile( EdWidget* tile );
	void SetCurrentFrame( EdWidget* frame );
	void SetCurrentAction( EdWidget* action );
	void SetCurrentSprite( EdWidget* sprite );

	// Converts the current action into a KrAction, and returns
	// the object. Used to feed to the engine for rendering.
	KrAction* CreateAnimationAction();

	// SetFilenames must be set before calling LoadSurface.
	bool LoadSurface( int nTransparency, const KrRGBA* rgba );

	// Transparency values for non-32 bit surfaces.
	enum { 
		MAX_TRANS = 10
	};
  private:
	enum {
		MAIN_IMNODE_DEPTH,
		WIDGET_DEPTH,
		CONSOLE_DEPTH,
		INFO_BACKGROUND_DEPTH,
		INFO_DEPTH,
	};

	KrEngine*		engine;			
	KrImNode*		imnode;		// The node where states hang there data from
	KrImNode*		widgetNode;	// Special container for the view state.
	KrImNode*		consoleHolder;
	KrConsole*		console;
	KrTextBox*		infoBox;
	KrBox*			infoBoxBack;
	EdWidget*		widget;
	KrFontResource*		fontResource;
	KrCanvasResource*	backgroundRes;

	gedString		surfaceFileName;
	gedString		defFileName;

	KrAction*		animAction;
	int				currentObject;
	EdWidgetTile*	currentTile;
	EdWidgetFrame*	currentFrame;
	EdWidgetAction*	currentAction;
	EdWidgetSprite*	currentSprite;
	KrCanvasResource* canvasResource;

 	KrRGBA			trans[MAX_TRANS];
	int				nTrans;
};


#endif
