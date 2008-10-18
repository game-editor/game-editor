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

#ifndef KYRA_ACTION_INCLUDED
#define KYRA_ACTION_INCLUDED

#include "SDL.h"
#include "rle.h"
#include "../util/gldynarray.h"

#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"

class KrCanvasResource;

#ifndef STAND_ALONE_GAME //maks
class KrEncoder;
#endif

class KrCollisionMap;


/** An action is contained by a sprite resource and is itself a
	container for frames.
	@sa KrSprite
*/
class KrAction
{
  public:
	/*  Create an empty KrAction. Used with the AddFrame method
		to create an action.
	*/
	KrAction( const gedString& name = "", const gedString& imgName = " ", Sint16 nFramesH = 0, Sint16 nFramesV = 0, Sint16 hotSpotX = 0, Sint16 hotSpotY = 0, Uint16 fps = 0); //maks	
	bool CanAnim(int gameFps, double &frameInc); //maks
	void Save(SDL_RWops *src); //maks
	void Load(SDL_RWops *src, Uint32 version); //maks
	const gedString& getimgName() const { return imgName; } //maks
	void setimgName(const gedString& name) { imgName = name; } //maks
	const Sint16 getnFramesH() const { return nFramesH; } //maks
	const Sint16 getnFramesV() const { return nFramesV; } //maks
	const Sint16 gethotSpotX() const { return hotSpotX; } //maks
	const Sint16 gethotSpotY() const { return hotSpotY; } //maks
	const Uint16 getFrameWidth() const { return frameWidth; } //maks
	const Uint16 getFrameHeight() const { return frameHeight; } //maks
	const Uint16 getFrameRate() const { return fps; } //maks
	void setFrameRate(int fps) {this->fps = fps;} //maks
	void setFrameWidth(int w) {frameWidth = w;} //maks
	void setFrameHeight(int h) {frameHeight = h;} //maks
	void setnFramesH(int nh) {nFramesH = nh;} //maks
	void setnFramesV(int nv) {nFramesV = nv;} //maks
	void SetAttr( const gedString& name, const gedString& imgName, Sint16 nFramesH, Sint16 nFramesV, Sint16 hotSpotX, Sint16 hotSpotY, Uint16 fps); //maks	

	void operator=( const KrAction& copy )  //maks
	{ 
		name		= copy.name;
		imgName		= copy.imgName;

		numFrames	= copy.numFrames;
		id			= copy.id;		
		
		nFramesH	= copy.nFramesH;
		nFramesV	= copy.nFramesV;
		hotSpotX	= copy.hotSpotX;
		hotSpotY	= copy.hotSpotY;
		frameWidth	= copy.frameWidth;
		frameHeight = copy.frameHeight;
		fps			= copy.fps;
	}
	
	/// Create the action by reading from a file. Used by the sprite constructor.
	KrAction( SDL_RWops* data );

	~KrAction();

	/// Each action has a name (ex, "Walking")
	const gedString& Name() const		{ return name; }
	/// Each action has a unique id. (ex, WALKING)
	U32				   Id() const		{ return id; }

	/*  Draw the action to the surface.
		Normally called by the Sprite.
		@param paintInfo	Target surface information
		@param frame		The frame number to draw.
		@param x			x value to draw -- measured at hotspot.
		@param y			y value to draw -- measured at hotspot.
		@param cForm		The color transformation to use.
		@param clip			A clipping rectangle. (Can be NULL)
	*/
	void Draw( KrPaintInfo* paintInfo,	
			   int frame,				
			   const KrMatrix2& matrix,		
			   const KrColorTransform& cForm,
			   const KrRect& clip,
			   int openGLZ );
	
	/// Total number of frames.
	int   NumFrames() const 			{ return numFrames; }

	/// Get a frame.
	/*const*/ KrRle& Frame( int i ) const	{ GLASSERT( i >= 0 );
										  GLASSERT( i < numFrames );
										  return frame[ i ]; } //maks

	/*  Get a non-conts pointer to the frame. A special case call.
	*/
	KrRle* GetFrame( int i ) const	//maks	
	{ 
		KrRle *rle = NULL;

		if( i >= 0 && i < numFrames )
			rle = &frame[ i ]; 

		return rle;
	}

	bool HitTestTransformed( int frame, int x, int y, int hitFlags );

	// internal
	
	bool IsScaleCached( GlFixed xScale, GlFixed yScale );
	void FreeScaleCache();
	KrCollisionMap* GetCollisionMap( GlFixed xScale, GlFixed yScale, int frame );
	
	void AddFrame()		{ GrowFrameArray( numFrames + 1 ); }

	

	void CalculateBounds( int frame, const KrMatrix2& xForm, KrRect* bounds );

	struct CachedBlock
	{
		GlFixed xScale, 
				yScale;
		KrRle** frame;

		bool operator==( const CachedBlock& )	{ GLASSERT( 0 ); return false; }		// be nice to buggy compilers.
	};

#ifndef STAND_ALONE_GAME //maks
	void Save( KrEncoder* encoder, bool bOnDemandFrameLoad = false);
	U32 GetTotalSize(); //maks

	
#endif

	CachedBlock *CacheScale( GlFixed xScale, GlFixed yScale ); //maks

	/** A strange -- but sometimes useful function. Creates
		a canvas from this action and returns it.
	*/
	KrCanvasResource* CreateCanvasResource( int frame, int* hotx, int* hoty );

  private:
	
	void GrowFrameArray( int newSize );

	GlDynArray< CachedBlock >		cache;

	gedString		name;
	U32				id;
	KrRle*			frame;			// Can't use dynamic array since it doesn't have proper copy
	int				numFrames;

	///////////////////////////////////////
	//maks
	gedString imgName;
	Sint16 nFramesH;
	Sint16 nFramesV;
	Sint16 hotSpotX;
	Sint16 hotSpotY;
	Uint16 frameWidth;
	Uint16 frameHeight;
	Uint16 fps;	
	///////////////////////////////////////
};


#endif
