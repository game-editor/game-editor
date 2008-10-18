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


#ifndef KYRA_ENGINE_INCLUDED
#define KYRA_ENGINE_INCLUDED

#include "SDL.h"
#include "vault.h"
#include "imagetree.h"
#include "dirtyrectangle.h"
#include "kyrabuild.h"

const int KyraVersionMajor = 2; //maks 2.0.4 > 2.1.0
const int KyraVersionMinor = 1;

//maks: 3 = new font save
//maks: 4 = new string save
//maks: 5 = new on demand frame loading
const int KyraVersionBuild = 5; //maks: 3 = new font save

/** @mainpage
	A Sprite Engine...from a Slightly Different Point of View

    Kyra is a simple, fully featured, industrial strength Sprite engine written in C++.
	It is built on top of SDL for cross platform compatibility.
	It is Open Source and provided under the GPL.

	The API that is documented here is intended to be accessible and
	easy to use. It is created with you the user in mind.

    Note, however, that the API documented here is the details of how the engine
    works. For the big picture, you should start with the HTML documentation,
    available with the source or online at www.grinninglizard.com/kyra. 
	
	The web page generally has the most up to date documentation (sometimes even
	current with the Beta version) which can be useful or somewhat confusing,
	if it discusses features that aren't in the version you are using. The
	documentation is always brought current before release, so the html in
	docs/api is current with the distribution you are using.

	The important exception is the release notes, which describe issues or 
	problems identified after release. Be sure to check there if you have any
	problems.

	<b>Brief API overview:</b>

	The KrEngine is a class that aggregates important components
	as well as providing some API calls as well. Kyra applications
	begin by creating a engine. The KrEngine::Draw
	methods renders the current sprites to the screen and updates
	the screen. The Tree method returns a pointer to the KrTree, a
	data structure where all the objects (drawable and otherwise)
	are stored. The Vault method returns a pointer to the engine's
	dictionary, where resources live.

	The tutorial and tests files give an overview as to how these
	peices fit together.

	<b>Memory Management</b>

	Generally speaking, objects are handed over to kyra components
	by ownership: that is, the client program 'new's an object,
	and then it is handed to the kyra object which will later delete
	it at the appropriate time. (An obvious exception is the engine
	itself, which you must create and destroy.)

	For example, you would create a canvas resource
	by calling 'new KrCanvasResource' with the appropriate parameters.
	Rather than memory manage this yourself, you should add it to
	the Vault, which can be retrieved via the KrEngine::Vault()
	method. You should not, in the future, delete the canvas resource
	since it is now owned by the vault. Likewise, when you create
	canvases by calling 'new KrCanvas', these will be added to the
	Tree (accessed via KrEngine::Tree() ) and will be deleted by the
	tree when appopriate.

	<b>Method and Parameter Names</b>

	Method and parameter are not fully standardized and have errors. 
	(Regrettably.) The desired convention is as follows:

	For methods that return pointers:

		- Create		Allocates memory. Returns something to you 
						that you should delete or delete [].
		- Get			Returns something that is owned by the object.
						Do not delete. 
		- To			A conversion method. Will return an address if
						the object can be downcast to "ToThing", null if
						not. This is a cast, so nothing is created by
						calling this method.

	Other methods:

		Generally no prefix is used for queries that return a simple value.

		- Calc			Implies more work is involved. A slower operation
						that a typical accessor, and should be used accordingly.
		- Query			Used when memory is passed in and it is written to.
						No memory is new'd or deleted by the callee.

	<b>Organization</b>

	Kyra uses the following directories:

		- spriteed		Sprite Editor files.
		- encoder		Encoder files.
		- tests			Test and demo files.
		- engine		The files for the main engine and library.
		- util			Utility and container classes.
		- gui			User interface widgets.
		- guiExtended	User interface widgets by external authors.

	And the following file prefixes:

		- kr			Kyra engine files.
		- gl			Generic utility files. (Grinning Lizard.)

	Similarly, the following class prefixes:

		- Kr			Kyra classes.
		- Gl			Generic utility classes.

	<b>The Not Standard Library </b>

	One of the joys of doing a project like this, is that there
	isn't a time line, and you can choose particular courses of action
	on a fun factor as well as prudence.

	Hence the lack of use of STL (except string).

	Kyra has its own container, utility, and iterator classes.
	They were fun to write, but have no real advantage over the STL.
	(Well, portability. STL is still a bit flakey.) As a Kyra user,
	you will probably never even notice them.

	But under the hood they lurk.
*/


enum
{
	KrQualityNone,		// Use the parent quality, if available.
	KrQualityFast,		// Nearest neighbor algorithm.
	KrQualityLinear,	// Bi-linear interpolation
	KrQualityAdaptive,	// Bi-linear or oversample
};

/** The core of the Kyra system. Instantiate an Engine
	to start using the sprite engine.
*/
class KrEngine
{
  public:
	/** Creates an engine that renders to the entire screen.
		@param screen	The SDL surface to use. If the surface
						is an OpenGL surface, Kyra will use the
						OpenGL library to draw with.
	*/
	KrEngine( SDL_Surface* screen );
	
	/**	Sets up an engine that renders to a rectangle on
		the screen, instead of the entire screen.
		@param screen		The SDL surface to use. 
		@param bounds		The area the engine will draw to.
		@param extraFill	If this is not null, the screen not covered
							by the bounds will be filled with this color.
							This gives a background color to "extra" screen.
	*/
	KrEngine( SDL_Surface* screen, const KrRect& bounds, const KrRGBA* extraFill );

	/** Sets up an engine that renders to multiple "windows"
		on the same surface. No window may overlap another.
		Each window is oriented with 0,0 as its upper left hand
		coordinate.

		@param screen		The SDL surface to use. 
		@param nWindows		The number of Kyra "windows" (viewports) to render to.
		@param bounds		An array of rectangles that define the windows.
		@param extraFill	If this is not null, the screen not covered
							by the bounds will be filled with this color.
							This gives a background color to "extra" screen.
	*/
	KrEngine( SDL_Surface* screen,/* int nWindows*/ const KrRect* bounds, const KrRGBA* extraFill );
	
	~KrEngine(); //maks

	/** Fetch the ImageTree. Even though it lives in the
		Engine, the Tree will be managed by the user.

		The KrImageTree returned is owned by the engine. It should not be deleted
		by the client code.
	*/
	KrImageTree*		Tree()			{ return tree; }

	/** Fetch a vault. Every engine needs a vault and there
		is nothing special about this one, except that it 
		is managed with the engine and will be created and 
		deleted at the correct time.

		The KrResourceVault is owned by the engine. It should not be deleted
		by the client code.
	*/
	KrResourceVault*	Vault()			{ return vault; }

	/// How many windows does this engine have?
/*	int NumWindows()					{	GLASSERT( nWindows <= KR_MAX_WINDOWS );
											return nWindows; }*/

	/// Given a point (in screen coordinates), what window is it in? -1 if none.
	int GetWindowFromPoint( int x, int y );
	
	/**	Once everything is ready to go, sprites and such
		are updated, call this method to put it all on
		the screen.

		@param updateRect	If true, SDL_UpdateRects will automatically
							be called in the Draw() method. Normally,
							this is the desired behavior. Only set to
							false if you will call SDL_UpdateRects,
							SDL_Flip, or something equivalent after
							you post process the buffer.
		@param rectangles	If wish the update rectangles to be returned
							to the calling application, pass in a
							DynArray.
	*/
	void Draw( bool updateTree = true, GlDynArray< KrRect >* rectangles = 0 ); //maks
	
	/** Return the bounds we are drawing to. (Not necessarily 
		the entire screen.) Set by the constructor.
	*/
	const KrRect& ScreenBounds( /*int window=0*/ )	{ return screenBounds/*[window]*/; }

	/** Retrun the bounds of the entire screen surface.
	*/
	const KrRect& FullScreenBounds()	{ return windowBounds; }

	/** Pass in a rectangle that should be invalidated. Only used 
		for unusual circumstances - primarily to clear a user-draw.
	*/
	void InvalidateRectangle( const KrRect& rect/*, int window=0*/ )	{ dirtyRectangle/*[window]*/.AddRectangle( rect ); }


	/// Query the surface pointer used by this engine.
	SDL_Surface*	Surface()	{ return screen; }

	/// Invalidate everything and mark for repaint. Follow with call to Draw()
	void InvalidateScreen()	{ Tree()->Root()->Invalidate( /*KR_ALL_WINDOWS*/ ); }

	/// Get a text description of the current rendering mode.
	void QueryRenderDesc( gedString* desc );

	static void QueryRenderDesc( SDL_Surface* surface, gedString* desc );

	// ------------- The splash screen. ---------- //
	/**	StartSplash displays the Kyra Splash screen. Pass in the current time
		in msec (can be queried from SDL_GetTicks) that is used by future calls
		to control the timeline.

		FillBackground() needs to be on (the default) for the splash screen,
		unless the client provides a background screen.
	*/
	void StartSplash( U32 msec );

	/**	Updates the splash animation. The Splash is one second long, but can
		be displayed for any amount of time.
	*/
	bool UpdateSplash( U32 msec );

	/**	Removes the splash screen and clears the data and resources it is using.
	*/
	void EndSplash();


	// ------------- Engine Options ----------- //
	/**	Will fill the background with the specified color before
		drawing Images. If the Images let no background screen show
		through, NULL should be passed in to turn this feature off
		for a performance improvement.
	*/
	void FillBackground( const KrRGBA* fillColor );

	/// Set the fill of a particular backrgound window.
	void FillBackgroundWindow( /*int window,*/ const KrRGBA* fillColor );

	/// Get the major, minor, and patch version of the engine.
	void static Version( int* major, int* minor, int* patch )	
	{ 
		*major = KyraVersionMajor;
		*minor = KyraVersionMinor;
		*patch = KyraVersionBuild;
	}

	/**	Set the maximize size of an OpenGL texture. This is the max size of either
		the x or y axis, so this limits memory usage to effectively: 

			max texture memory used = size * size * 4

		Useful to contain memory usage of large sprites. Quality, of course, will
		suffer if the size is constrained. Set to 0 (the default) to limit the largest
		size to that allowed by the driver.

		Note: This is static. The effect is global.
	*/
	static void SetMaxOglTextureSize( int size )	{ maxOglTextureSize = size; }

	/// Query the current max texture size. (0 is no Kyra imposed limit.)
	static int MaxOglTextureSize()					{ return maxOglTextureSize; }

	//----------------------------------
	// For use by the engine components:
 	KrDirtyRectangle*	DirtyRectangle( /*int window*/ )	{ return &dirtyRectangle/*[window]*/; }

	// Call SDL_Rect for each rectangle.
	void UpdateScreen( GlDynArray< KrRect >* rects );

	// Debugging: try to see if this is corrupted
	void Validate()	{   /*GLASSERT( nWindows >= 0 );
						GLASSERT( nWindows <= KR_MAX_WINDOWS );*/
						GLASSERT( windowBounds.IsValid() );
						//GLOUTPUT( "Engine Validated (nWindows=%d)\n", nWindows );
      				}

	void /*maks: public*/ Init(	SDL_Surface* _screen,		// The SDL surface
				/*int _nWindows,*/				// number of windows
				const KrRect* bounds,		// rect for each window
				const KrRGBA* extra );		// The color for non-window areas, if specified.

	void ReinitVault(); //maks

#ifndef USE_WALK
	void NotifyNewNode() {bNewNode = true;} //maks
#endif

	void setTree(KrImageTree* _tree) { tree = _tree;} //maks
	static void InitOpenGL(int w, int h, float r = 0.0, float g = 0.0, float b = 0.0); //maks

  private:
	

	

	static int maxOglTextureSize;

	SDL_Surface* screen;
	//int nWindows;

	KrDirtyRectangle	dirtyRectangle/*[ KR_MAX_WINDOWS ]*/;
	KrRect				screenBounds/*[ KR_MAX_WINDOWS ]*/;
	KrRect				windowBounds;

	KrImageTree*		tree;
	KrResourceVault*	vault;

	KrPaintInfo			paintInfo;
	bool				fillBackground/*[ KR_MAX_WINDOWS ]*/;
	KrRGBA				backgroundColor/*[ KR_MAX_WINDOWS ]*/;	// The background color of each window
	KrRGBA				extraBackground;					// The background color outside of the windows
	bool				needFullScreenUpdate;

	U32					splashStart;
	KrResourceVault		*splashVault;
	KrSprite			*splash, *splashText;

#ifndef USE_WALK
	bool bNewNode; //maks
#endif
};

extern gedString sNONE; //maks: avoid mallocs

#endif
