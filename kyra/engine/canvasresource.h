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



#ifndef KYRA_CANVASRESOURCE_INCLUDED
#define KYRA_CANVASRESOURCE_INCLUDED

#include "kyraresource.h"
#include "../engine/krmath.h"
#include "../util/glcirclelist.h"
#include "pixelblock.h"

class KrCanvas;

/** A Canvas is a user-draw object. But like all objects in Kyra,
	there is a resource and instance (Image) component.
*/
class KrCanvasResource : public KrResource
{
  public:
	  void RestoreBuffer(int iBuffer); //maks
	  void SaveBuffer(int iBuffer); //maks
	  
	  void Load(SDL_RWops *src); //maks

#ifndef STAND_ALONE_GAME //maks
	  void Save(SDL_RWops *src); //maks  
#endif

	/** A Canvas Resource is NOT created by a dat file, but is
		created by the client program. Once a Canvas Resource is
		created (and normally placed in the vault) it is used
		like any other resource, with the exception that a Canvas
		Resource can be changed at run time. You can reach in, 
		change the pixels in the Resource, and the call Refresh().
		After Refresh() is called, the Canvas's using the 
		CanvasResource will update to reflect the new image.

		@param	name			A name for this resource.
		@param	width			Width of the CanvasResource, in pixels.
		@param	height			Height of the CanvasResource, in pixels.
		@param	alphaSupport	A canvas is made up of KrRGBAs. In order to use
								the alpha channel, the CanvasResource must be 
								created with the alphaSupport set to true. There
								is a performance penalty for using the alphaSupport
								however, so only turn it on if needed.
	*/ 
	KrCanvasResource( const gedString& name,
					  int width, 
					  int height, 
					  int alphaSupport );

	virtual ~KrCanvasResource();

	virtual	U32 Type()						{ return KYRATAG_CANVAS; }
	virtual const gedString&	TypeName()	{ return canvasName; }
	virtual KrCanvasResource* ToCanvasResource(){ return this; }

	int     Width()						{ return pixelBlock.Width(); }	///< Width
	int		Height()					{ return pixelBlock.Height(); }	///< Height
	KrRGBA* Pixels()					{ return pixelBlock.Pixels(); }	///< Base memory of the image.
	int		Alpha()						{ return pixelBlock.Alpha(); }	///< Returns true if this canvas is constructed to use an alpha channel.

	
	/*  Draw a canvas resource.
		@param  paintInfo	Information about the target surface for drawing (optimizing).
		@param	x			X location in pixels.
		@param	y			Y location in pixels.
		@param  cForm		Color transformation applied to the drawing.
		@param	clip		A clipping rectangle, which can be null.
	*/
	void Draw( KrPaintInfo* paintInfo,
			   const KrMatrix2& matrix,
			   const KrColorTransform& cForm,
			   const KrRect& clipping,

			   int quality,

			   int openGLZ );

	/**	Refresh must be called if the CanvasResource changes so that 
		the Canvas objects can be updated. This is generally called
		just after writing to the canvas.
	*/
	void Refresh();

	/*  Do a HitTest (see KrImageTree::HitTest) in transformed 
		coordinates. So the tree object that made this call
		has already transformed the x and y into local pixel coords.
	*/
	bool HitTestTransformed( int x, int y, int hitFlags );

	void CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const;

	// Canvases don't cache.
	virtual void CacheScale( GlFixed xScale, GlFixed yScale )		{}
	virtual bool IsScaleCached( GlFixed xScale, GlFixed yScale )	{ return false; }
	virtual void FreeScaleCache()									{}

	// But they do collision map.
	virtual KrCollisionMap* GetCollisionMap( KrImage* state/*, int window*/ );

 	// For use by the Canvas objects:
	void AddCanvas( KrCanvas* canvas );
	void RemoveCanvas( KrCanvas* canvas );

	void SetDimensions(int width, int height); //maks
	KrCanvas *GetFirstClient() {return clients.Front();}//maks
	bool HasClients() {return numClients > 0 && !clients.Empty();} //maks

  private:
	static const gedString canvasName;
	static int canvasId;

	KrPixelBlock		pixelBlock;
	int					numClients;
	GlCircleList<KrCanvas*>		clients;
	GlDynArray<KrCollisionMap*>	collisionMaps;	
};


#endif



