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

#ifndef KYRA_SPRITERESOURCE_INCLUDED
#define KYRA_SPRITERESOURCE_INCLUDED

#include "../util/glstring.h"
#include "../util/glmap.h"
#include "kyraresource.h"
// #include "tags.h"
#include "../engine/krmath.h"
#include "action.h"


/** Practically, a sprite resource is used to create a sprite.
	A sprite resource is a type of resource and a container for 
	actions.

	SpriteResource
		Action
			Frames ( Rle )
*/
class KrSpriteResource : public KrResource
{
  public:
	  void RemoveAction( const gedString& actionName ); //maks
	// Create by reading from a .dat file
	KrSpriteResource( U32 size, SDL_RWops* data );

	/*  Create the resource from the program. (Used by the sprite
		editor for animating and aligning sprites.) 
		After construction, SetAction is called.
	*/
	KrSpriteResource( const gedString& spriteName );
	
	virtual ~KrSpriteResource();

	// Pass in an action to the resource. Used by the sprite editor.
	// Will allocate the action, if necessary.
	void AddAction( KrAction* action );

	virtual	U32 Type()								{ return KYRATAG_SPRITE; }
	virtual const gedString&	TypeName()			{ return spriteName; }
	virtual KrSpriteResource* ToSpriteResource()	{ return this; }

	/// Return the count of actions in this sprite.
	int		  NumActions()					{ return actionArr.Count(); }
	/// Fetch an action from this Sprite Resource by the name of the action.
	KrAction* GetAction( const gedString& actionName );
	/// Fetch an action from this Sprite Resource by the id of the action.
	KrAction* GetAction( U32 actionId );
	
	

	

	// -- internal -- //
	KrAction* GetActionByIndex( int i )		{ return actionArr[i]; }
	const KrAction& ActionByIndex( int i )	{ return *actionArr[i]; }

	/*  Draw a sprite resource.
		@param	surface		Target surface.
		@param  paintInfo	Information about the target surface for drawing (optimizing).
		@param	action		The name of the action to draw.
		@param	frame		A frame # to draw.
		@param	x			X location in pixels.
		@param	y			Y location in pixels.
		@param  cForm		Color transformation applied to the drawing.
		@param	clip		A clipping rectangle, which can be null.
	*/
	void Draw( KrPaintInfo* paintInfo, 
			   const gedString& action,
			   int frame,
			   const KrMatrix2& matrix,
			   const KrColorTransform& cForm,
			   const KrRect& clip,

			   int quality,

			   int openGLZ );

	/*  Do a HitTest (see KrImageTree::HitTest) in transformed 
		coordinates. So the tree object that made this call
		has already transformed the x and y into local pixel coords.
	*/
	bool HitTestTransformed( int x, int y, int hitFlags )	{	GLASSERT( 0 ); return false; }	// never called directly. Use the action.

	
	virtual bool IsScaleCached( GlFixed xScale, GlFixed yScale );
	virtual void FreeScaleCache();
	virtual KrCollisionMap* GetCollisionMap( KrImage* state/*, int window*/ );

#ifndef STAND_ALONE_GAME //maks
	virtual void Save( KrEncoder* );

	/** It is sometimes useful to get a Canvas Resource that
		is created from a sprite. This method calculates the bounds,
		creates the Resource, and returns it. If there is an error,
		0 is returned.
	*/
	KrCanvasResource* CreateCanvasResource(	const gedString& actionName,
												int frame, int* hotx, int* hoty );

	/// Same functionality, uses the id instead of the name.
	KrCanvasResource* CreateCanvasResource(	U32 actionId,
												int frame, int* hotx, int* hoty );

	
#endif

	virtual void CacheScale( GlFixed xScale, GlFixed yScale );

  protected:
	GlDynArray< KrAction* >	actionArr;			// the actions are stored here

  private:

	const static gedString spriteName;

	GlMap< gedString, KrAction*, GlStringHash >*  actionMap;		// the map is fast access to the action
	GlMap< U32, KrAction*, GlNumberHash<U32> >*		actionIdMap;	// the map is fast access to the action
};


#endif
