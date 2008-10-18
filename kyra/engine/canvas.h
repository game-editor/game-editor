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



#ifndef KYRA_CANVAS_INCLUDED
#define KYRA_CANVAS_INCLUDED

#include "image.h"
#include "canvasresource.h"

extern void OnSlowCanvasPaint(void *actor, KrPaintInfo *paintInfo, SDL_Surface* surface, KrRect &rect, int openGLZ); //maks

struct KrPaintInfo;
class  KrCanvasResource;
union  KrRGBA;


/**	A Canvas is a pixel area that a client program can draw to.
	A Canvas has the following advantages over just drawing to
	the framebuffer after the engine:

	- automates dirty rectangle handling
	- applies color and alpha transforms like all other Kyra objects
	- can have multiple instances
	- depth
*/

class KrCanvas : public KrImage
{
  public:

	/// Construct from a resource - if the resource changes ALL the instances change.
	KrCanvas( KrCanvasResource* resource, void *parent = NULL ); //maks
	virtual ~KrCanvas();

	virtual void Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip/*,
						int window*/ );

	int Width()			{ return resource->Width(); }	///< Width
	int Height()		{ return resource->Height(); }	///< Height

	/** Note that this method is a pass through to a Canvas Resource; 
		if you change the pixel data, all instances of the 
		Canvas Resource will change.
		@sa KrCanvasResource
	*/
	KrRGBA* Pixels()	{ return resource->Pixels(); }

	/** Note that this method is a pass through to a Canvas Resource; 
		if you change the Refresh data, all instances of the 
		Canvas Resource will change.
		@sa KrCanvasResource
	*/
	void Refresh()		{ resource->Refresh(); }

	virtual KrImNode* Clone();

	virtual void QueryBoundingBox( KrRect* boundingBox/*, int window = 0*/ );
	virtual bool HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results/*, int window*/ );
	virtual KrCanvas* ToCanvas()	{ return this; }
	virtual void CalcTransform( /*int window*/ );
	virtual KrResource* Resource()			{ return resource; }

	virtual void SetDimensions(int x, int y, int width, int height); //maks
	virtual void SetDimensions(int width, int height); //maks

	void Load(SDL_RWops *src) {resource->Load(src);}; //maks

#ifndef STAND_ALONE_GAME //maks
	void Save(SDL_RWops *src) {resource->Save(src);}; //maks
#endif
	
  protected:

  private:
	KrCanvasResource* resource;
};


#endif


