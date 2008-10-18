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

#ifndef KYRA_SPRITE_INCLUDED
#define KYRA_SPRITE_INCLUDED

#include "image.h"
#include "spriteresource.h"


/**	The most obvious class for a sprite image, describes on
	on screen image, potentially with multiple actions and 
	frames.

	Almost all pre-generated images are sprites. The only reason
	to use a tile is for rotation.

	A Sprite contains an arbitrary number of actions, each of which
	has any number of frames.

	@verbatim
	An example:
		Sprite:	"DRONE", an alien creature
			Action: "WALK.DIR7", walking NW
				Frames: 0, 1, 2, 3, 4, 5
			Action: "WALK.DIR9", walking NE
				Frames: 0, 1, 2, 3, 4, 5
			Action: "STANDING", no motion
				Frames: 0

	@endverbatim
*/

enum //maks
{
	NO_INFINITE = 0,
	X_INFINITE,
	Y_INFINITE,
	BOTH_INFINITE
};

class KrSprite : public KrImage
{
  public:
	KrSprite( KrSpriteResource* resource, void *parent = NULL ); //maks
	virtual ~KrSprite()		{}
	
	void AdjustInfinite(int &xo, int &yo/*, int window*/); //maks
	void CalcTransformInfinite( /*int win,*/ bool bCalcRealBounds ); //maks

	/// Set the current frame for the current action.
	void SetFrame( int i );

	/// Set the current action by its name. "STANDING"
	void SetAction( const gedString& actionName );

	/**	[Experimental, added 1.6]
		Rotations applied by the encoder change the action name to reflect
		the rotation. ROT000, where 000 is the rotation, in integer degrees.
		It as appended as the last segment of the name. For instance,

			"STANDING.ROT090" and STANDING | ROT090 
			
		would be the string and integer representations. This method, giving
		the leading string ("STANDING") will add the rotation automatically.
	*/
	void SetActionRotated( const gedString& actionName, int rotation );

	/** Note this is the action Id (written by the encoder: STANDING),
		NOT the index (whatever that happens to be.
	*/
	void SetAction( U32 id );

	/// Get a pointer to the current action.
	KrAction* GetAction()	{ return action; }
	/// Get the name of the current action.
	gedString ActionName() const	{ return action->Name(); }

	/// Return the number of frames in the current action.
	int  NumFrames() const		{ return action->NumFrames(); }
	/// Return the current frame of the current action.
	int  Frame() const			{ return frame; }

	/** Take one step: advance the frame and move the position of the sprite.
		Not all sprites have steps. They are configured in the Sprite
		editor.
	*/
	bool DoStep(bool bForward = true, int gameFps = 30);	//maks	
	
	/// Get the sum of distance covered by all the DoSteps
	KrVector2 Stride();		

	virtual void QueryBoundingBox( KrRect* boundingBox/*, int window = 0*/ );

	/// Get the resource for this Sprite.
	KrSpriteResource*   SpriteResource()	{ return resource; }
	virtual KrResource* Resource()			{ return resource; }


	virtual bool HitTest( int x, int y, int flags = 0, GlDynArray<KrImage*>* results = NULL/*, int window = 0*/); //maks
	virtual KrImNode* Clone();

	// -- internal -- //
	// Recursive call to draw a node and all its children.
	virtual void Draw(	KrPaintInfo* paintInfo,
						const KrRect& clip/*,
						int window*/ );

	virtual KrSprite* ToSprite()	{ return this; }
	virtual void	CalcTransform( /*int win*/ );

	void SetInfinite(int infinite) {this->infinite = infinite; Invalidate(/*0*/);} //maks
	int getInfinite() {return infinite;} //maks
  private:
	int infinite; //maks
    KrSprite();                                 // not defined and not allowed
    KrSprite& operator=( const KrSprite& );    // not defined and not allowed.

	KrSpriteResource*	resource;
	KrAction*			action;
	int					frame;
	double frameInc; //maks
};


#endif
