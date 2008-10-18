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

#ifndef KR_RESOURCE_INCLUDED
#define KR_RESOURCE_INCLUDED

#include "../util/glstring.h"
#include "../util/gltypes.h"
#include "../util/glutil.h"
#include "../util/glfixed.h"


struct SDL_RWops;
struct KrRect;
class KrSpriteResource;
class KrTileResource;
class KrCanvasResource;
class KrFontResource;
class KrBoxResource;
class KrTextDataResource;
class KrBinaryDataResource;
class KrEncoder;
class KrImage;


/** An enumeration of the possible data types in an Io DAT file.
*/
enum
{
	KYRATAG_END    = 1,
	KYRATAG_SPRITE,
	KYRATAG_TILE,
	KYRATAG_CANVAS,		// never in a .dat file - created at run-time
	KYRATAG_FONT,
	KYRATAG_BOX,
	KYRATAG_TEXTDATA,
	KYRATAG_BINARYDATA
};


// A structure -- cached in the resource -- that describes a collision bit map.
class KrCollisionMap
{
  public:
	KrCollisionMap( GlFixed xScale, GlFixed yScale,		// the scale is information only
					int width, int height );			// pixel size of the map to create

	~KrCollisionMap()							{ delete [] map; }

	// Input: creating the map.
	U32* GetRow( int i )			{	/*GLASSERT( i>=0 && i<cy );*/ //maks
										if(i>=0 && i<cy)
											return map + cx * i;
										else 
											return NULL;
									}

	// Output: using the map
	// Check a collision of something to the right.
	int Collide( int dx, int dy, const KrRect& intersection, KrCollisionMap* ); //maks: return overlapped area
	int CollideRetangle( int offsetX, int offsetY, const KrRect& boundsIntersect); //maks

/*#ifdef DEBUG
	void Draw(void* surface, int dx, int dy); //maks
#endif*/

	// Informational
	GlFixed XScale()		{ return xScale; }
	GlFixed YScale()		{ return yScale; }

  private:
	int cx, cy;
	U32* map;
	GlFixed xScale, yScale;
};


/**	The KrImNode's children - Sprite, Canvas, etc - are instances
	of corresponding resources. KrResource is the parent class for
	all resources.
*/
class KrResource
{
  public:
	KrResource()				: resId( 0 ), refCount(0) {collisionMap = NULL;} //maks
	virtual ~KrResource()		{}
	long		refCount;	//maks

	/// The type of the resource. ( ex, KYRATAG_SPRITE )
	virtual U32					Type() = 0;
	/// The name of the type. ( ex, "Sprite" )
	virtual const gedString&	TypeName() = 0;

	/// A unique name for this particular resource. ( "Warrior" )
	const gedString&	ResourceName()		{ return resName; }
	/// A unique id for this resource. (BEM_WARRIOR).
	const U32			ResourceId()		{ return resId; }	

	virtual KrSpriteResource* ToSpriteResource()	{ return 0; }	///< Return a pointer if this is a sprite resource.
	virtual KrTileResource*   ToTileResource()		{ return 0; }	///< Return a pointer if this is a tile resource.
	virtual KrCanvasResource* ToCanvasResource()	{ return 0; }	///< Return a pointer if this is a canvas resource.
	virtual KrFontResource*	  ToFontResource()		{ return 0; }	///< Return a pointer if this is a font resource.
	virtual KrBoxResource*	  ToBoxResource()		{ return 0; }	///< Return a pointer if this is a box resource.
	virtual KrTextDataResource*	  ToTextDataResource()		{ return 0; }	///< Return a pointer if this is a text data resource.
	virtual KrBinaryDataResource* ToBinaryDataResource()	{ return 0; }	///< Return a pointer if this is a binary data resource.

	/**	Create a cached resourced. A resource can be drawn much
		more quickly if its scaled image is pre-calculated. This
		generates and scaled version of the resource and stores
		it for fast drawing. The cached version will always be
		used before a version generated on the fly.
	*/
	virtual void CacheScale( GlFixed xScale, GlFixed yScale )	{}

	/// Check the cache for a given scale.
	virtual bool IsScaleCached( GlFixed xScale, GlFixed yScale )	{ return false; }

	/// Free up the cache
	virtual void FreeScaleCache()	{}
	
	// Used by the encoder:
	virtual void Save( KrEncoder* )	{ GLASSERT( 0 ); }

	// If this is a type that supports a collision map, this will create it (if
	// necessary) and return a pointer to the map. The KrImage and window are
	// passed in to determine which map to fetch.
	virtual KrCollisionMap* GetCollisionMap( KrImage* state/*, int window*/ )		{ return 0; }

  protected:
	void SetNameAndId( const gedString& _name, U32 _id ); //maks
	KrCollisionMap* collisionMap;

  private:
	gedString resName;
	U32			resId;

};


class KrResourceFactory
{
  public:
	virtual ~KrResourceFactory();

	static KrResourceFactory*	Instance();

	virtual KrResource*	Create( U32 id, 
								U32 size, 
								SDL_RWops* file );

  private:
	KrResourceFactory();
	static KrResourceFactory* instance;
};



#endif
