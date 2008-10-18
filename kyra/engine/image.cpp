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


#include "SDL.h"
#include "image.h"
#include "engine.h"


KrImage::KrImage() 
{
	//maks for( int i=0; i<KR_MAX_WINDOWS; ++i )
	{
		wasVisibleAtLastFlush/*[0]*/ = true; //maks 
//		haveCacheRect[i] = false;
	}

	bSlowCanvasCollision = false; //maks
}


void KrImage::AddedtoTree()
{
	KrImNode::AddedtoTree();
	//GLASSERT( bounds[0].IsValid() ); //maks
}


void KrImage::LeavingTree()
{
	GLASSERT( Engine() );
	//for( int i=0; i<Engine()->NumWindows(); ++i )
	{
		if ( wasVisibleAtLastFlush/*[i]*/ )
		{
			Engine()->DirtyRectangle(/*i*/)->AddRectangle( bounds/*[i]*/ );
		}
	}
	KrImNode::LeavingTree();
}


void KrImage::FlushInvalid( /*int window,*/ bool cache )
{
	// This whole cache mess takes advantage that the 2 rectangles generated
	// by a particular sprite will almost certainly overlap and can be treated
	// as one. Cache the first. When the 2nd comes in, if they intersect, union
	// together and add both. Else add seperately. Doesn't make much of a 
	// difference, pulling for now.
	//
//	if ( cache )
//	{
//		// Make sure our bounds get re-painted.
//		if ( wasVisibleAtLastFlush[window] || IsVisible(window) )
//		{
//			GLASSERT( haveCacheRect[window] == false );
//			haveCacheRect[window] = true;
//			cacheRect[window] = bounds[window];
//		}
//	}
//	else
//	{
//		if ( haveCacheRect[window] )
//		{
//			// Make sure our bounds get re-painted.
//			if ( wasVisibleAtLastFlush[window] || IsVisible(window) )
//			{
//				if ( cacheRect[window].Intersect( bounds[window] ) )
//				{
//					cacheRect[window].DoUnion( bounds[window] );
//					Engine()->DirtyRectangle(window)->AddRectangle( cacheRect[window] );
//				}
//				else
//				{
//					Engine()->DirtyRectangle(window)->AddRectangle( cacheRect[window] );
//					Engine()->DirtyRectangle(window)->AddRectangle( bounds[window] );
//				}
//			}
//			haveCacheRect[window] = false;
//		}
//		else
//		{
			// The entirety of "the old code."
			// Make sure our bounds get re-painted.
			//GLASSERT( bounds[window].IsValid() );
			bool vis = IsVisible(/*window*/);
			//GLASSERT( !vis || bounds[window].IsValid() ); //maks: fail with totaly transparent objects

			if ( wasVisibleAtLastFlush/*[window]*/ || vis )
			{
				Engine()->DirtyRectangle(/*window*/)->AddRectangle( bounds/*[window]*/ );
				wasVisibleAtLastFlush/*[window]*/ = vis;
			}
//		}
//	}
}

class AdjustInfiniteBounds //maks
{
public:
	AdjustInfiniteBounds(KrImage *_img1, KrImage *_img2/*, int _window*/)
	{
		//window = _window;
		img1 = _img1;
		img2 = _img2;
		infinite = NULL;
		fixed = NULL;

		bool bSprite1Infinite = false, bSprite2Infinite = false;

		//Calc real bounds
		if(_img1->ToSprite())
		{
			_img1->ToSprite()->CalcTransformInfinite(/*window,*/ true);	
			bSprite1Infinite = _img1->ToSprite()->getInfinite() != NO_INFINITE;
		}

		if(_img2->ToSprite())
		{
			_img2->ToSprite()->CalcTransformInfinite(/*window,*/ true);			
			bSprite2Infinite = _img2->ToSprite()->getInfinite() != NO_INFINITE;
		}

		//Adjust infinite position
		/*if(bSprite1Infinite && bSprite2Infinite)
		{			
			//Both infinite

			int x1=0, y1=0, x2=0, y2=0;
			KrRect	r1 = _img1->Bounds(window),
					r2 = _img2->Bounds(window);
			
			//Move to zero
			r1.Translate(-r1.xmin, -r1.ymin);
			r2.Translate(-r2.xmin, -r2.ymin);
			
			_img1->ToSprite()->AdjustInfinite(x1, y1, window);
			_img2->ToSprite()->AdjustInfinite(x2, y2, window);
			
			//Translate to correct position
			r1.Translate(x1, y1);
			r2.Translate(x2, y2);
			
			_img1->Bounds(window) = r1;
			_img2->Bounds(window) = r2;
		}
		else*/ if(bSprite1Infinite)
		{
			//Sprite1 infinite, translate sprite2

			KrRect	*r = _img2->GetBounds(/*window*/);
			int xi, yi, x = r->xmin, y = r->ymin;			
			
			
			KrVector2T< GlFixed > object;
			_img1->ScreenToObject( x, y, &object/*, window*/ );

			xi = x = object.x.ToIntRound();
			yi = y = object.y.ToIntRound();
			

			_img1->ToSprite()->AdjustInfinite(x, y/*, window*/);
			
			r->Translate(x - xi, y - yi);


			infinite = _img1->ToSprite();
			fixed = _img2;
		}
		else if(bSprite2Infinite)
		{
			//Sprite2 infinite, translate sprite1

			KrRect	*r = _img1->GetBounds(/*window*/);
			int xi, yi, x = r->xmin, y = r->ymin;			
			
			
			KrVector2T< GlFixed > object;
			_img2->ScreenToObject( x, y, &object/*, window*/ );

			xi = x = object.x.ToIntRound();
			yi = y = object.y.ToIntRound();
			

			_img2->ToSprite()->AdjustInfinite(x, y/*, window*/);
			
			r->Translate(x - xi, y - yi);


			infinite = _img2->ToSprite();
			fixed = _img1;
		}
	}

	virtual ~AdjustInfiniteBounds()
	{
		//Restore bounds
		if(img1->ToSprite()) img1->ToSprite()->CalcTransformInfinite(/*window,*/ false);
		if(img2->ToSprite()) img2->ToSprite()->CalcTransformInfinite(/*window,*/ false);
	}

	KrImage *img1, *img2, *fixed;
	KrSprite *infinite;
	//int window;
};

int KrImage::CalcCollision( KrImage* other/*, int window*/ ) //maks
{
	// The entire collision thing is base on a left-right model. Very important
	// to get that correct.
	KrImage* left  = 0;
	KrImage* right = 0;

	if ( other->Bounds( /*window*/ ).xmin > this->Bounds( /*window*/ ).xmin )
	{
		right = other;
		left  = this;
	}
	else
	{
		right = this;
		left = other;
	}
	
	KrRect isect = left->Bounds( /*window*/ );
	isect.DoIntersection( right->Bounds( /*window*/ ) );
	
	if(!isect.IsValid() || isect.Area() == 0)
		return 0;
	
	
	KrCollisionMap *leftMap = NULL, *rightMap = NULL;
	
	if(left->bSlowCanvasCollision && right->bSlowCanvasCollision)
	{
		return isect.Area();
	}
	else if(right->bSlowCanvasCollision)
	{
		leftMap  = left->Resource()->GetCollisionMap( left/*, window*/ );
		if(leftMap)
			return leftMap->CollideRetangle(isect.xmin - left->Bounds(/*window*/ ).xmin,
			isect.ymin - left->Bounds( /*window*/ ).ymin,
			isect);
	}
	else if(left->bSlowCanvasCollision)
	{
		rightMap  = right->Resource()->GetCollisionMap( right/*, window*/ );
		if(rightMap)
			return rightMap->CollideRetangle(isect.xmin - right->Bounds( /*window*/ ).xmin,
			isect.ymin - right->Bounds( /*window*/ ).ymin,
			isect);
	}
	else
	{
		leftMap  = left->Resource()->GetCollisionMap( left/*, window*/ );
		rightMap = right->Resource()->GetCollisionMap( right/*, window*/ );		
		
		if ( leftMap && rightMap ) //maks
		{
			// Okay, we might actually collide after all that.
			
			int rightXmin = right->Bounds( /*window*/ ).xmin, rightYmin = right->Bounds( /*window*/ ).ymin,
				leftXmin = left->Bounds( /*window*/ ).xmin, leftYmin = left->Bounds( /*window*/ ).ymin;			
			
			
			return leftMap->Collide(rightXmin - leftXmin,
				rightYmin - leftYmin,
				isect,
				rightMap );
		}
	}

	return 0;
}

int KrImage::CheckCollision( KrImage* other/*, int window*/ ) //maks
{
	// Only sprites and canvases collide.
	if (    !( other->ToSprite() || other->ToCanvas() || other->bSlowCanvasCollision)
		 || !( this->ToSprite() || this->ToCanvas() || this->bSlowCanvasCollision  ) )
	{
		return 0;
	}

	// If the bounding boxes don't overlap, they don't collide.
	if ( !this->Bounds( /*window*/ ).Intersect( other->Bounds( /*window*/ ) ) )
	{
		return 0;
	}

	// Visiibily
	if ( !this->IsVisible( /*window*/ ) || !other->IsVisible( /*window*/ ) )
	{
		return 0;
	}

	// Transparency
	if (    this->CompositeCForm( /*window*/ ).Alpha() == 0
		 || other->CompositeCForm( /*window*/ ).Alpha() == 0 )
	{
		return 0;
	}

//	GLOUTPUT( "BoundingBoxPass: %s [%d,%d]-[%d,%d]\n",
//			  other->NodeName().c_str(),
//			  other->Bounds( 0 ).xmin,
//			  other->Bounds( 0 ).ymin,
//			  other->Bounds( 0 ).xmax,
//			  other->Bounds( 0 ).ymax );

	
	// Not colliding with text is a good idea -- but I fear I'll be chasing
	// special cases and widget behavior forever. So removing check.

	//	// Don't collide with text.
	//	if (	other->Parent() 
	//		 && other->Parent()->Parent()
	//		 && other->Parent()->Parent()->ToTextBox() )
	//	{
	//		return false;
	//	}
	//	if (	this->Parent()
	//		 &&	this->Parent()->Parent()
	//		 &&	this->Parent()->Parent()->ToTextBox() )
	//	{
	//		return false;
	//	}

	


	if((ToSprite() && ToSprite()->getInfinite() != NO_INFINITE) || (other->ToSprite() && other->ToSprite()->getInfinite() != NO_INFINITE))
	{
		//Adjust infinite sprites
		AdjustInfiniteBounds adjustInfiniteBounds(this, other/*, window*/);

		if(adjustInfiniteBounds.infinite)
		{
			int area = 0;

			//Move infinite from before  until after fixed bounding box
			KrRect	*infRect = adjustInfiniteBounds.infinite->GetBounds(/*window*/),
					*fixRect = adjustInfiniteBounds.fixed->GetBounds(/*window*/);

			if(adjustInfiniteBounds.infinite->getInfinite() == X_INFINITE)
			{
				while(infRect->IntersectHorizontal(*fixRect))
				{
					area += CalcCollision(other/*, window*/);
					infRect->Translate(infRect->Width(), 0);
				}
			}
			else if(adjustInfiniteBounds.infinite->getInfinite() == Y_INFINITE)
			{
				while(infRect->IntersectVertical(*fixRect))
				{
					area += CalcCollision(other/*, window*/);
					infRect->Translate(0, infRect->Height());
				}
			}

			return area;
		}
	}
	else
	{
		//Normal sprites
		return CalcCollision(other/*, window*/);
	}

	return 0;
}


void KrImage::Get_overlap_collision_area_gradient(KrImage *other, int &dx, int &dy, bool bCheckChildCollisions) //maks
{
	/*
"The overlap gradient is
stored in (dx,dy). This gradient represents the change in overlap
when moving the masks one pixel. The gradient vector points in the
direction which increase the overlap the most, which can be used 
for quite realistic game physics. (This is a good collision normal).
Might be a bit expensive, probably 5 * bitmask_overlap_pos or something.  

Example:
 The gradient was dx = 5, dy = -2.
 This means that the overlap will be about 5 pixels greater if the 
 xoffset is increased, and 5 pixels less if xoffset is decreased. 
 To move the sprites away from the overlap the offset should be 
 changed in the (-5,2) direction. The magnitude of the gradient may
 or may not be the actual change in overlap."


"A very robust way to determine the angle at which
the sprites collided can be had from the gradient of
the overlap area. Calculate the gradient of the overlap
area f(x, y), where x and y are the dierences in
position of the two sprites:

rf(x, y) = (f(x+1, y)-f(x-1, y), f(x, y+1)-f(x, y-1))

This gradient vector will point in the direction
where the overlap increases the most, and this can
be used as the normal vector of the collision. Overall
this method is very good at finding good looking
normals, and has very few weaknesses. It does however
require four calls to the overlap area function,
and can therefor be a bit expensive. It should only
be used when actually needed."
 */

	GlDynArray<KrImage*> outputArray;
	KrMatrix2 current =  Transform(/*0*/); //solve shake bug.ged

	SetPos(current.x + 1, current.y);
	CalcTransform(/*0*/);
	int overlapped_area1 = CheckCollision(other);
	if(bCheckChildCollisions && !overlapped_area1)
	{
		outputArray.Clear();
		overlapped_area1 = Engine()->Tree()->CheckChildCollision(this, other, &outputArray);
	}

	SetPos(current.x - 1, current.y);
	CalcTransform(/*0*/);
	int overlapped_area2 = CheckCollision(other);
	if(bCheckChildCollisions && !overlapped_area2)
	{
		outputArray.Clear();
		overlapped_area2 = Engine()->Tree()->CheckChildCollision(this, other, &outputArray);
	}

	dx = overlapped_area1 - overlapped_area2;



	SetPos(current.x, current.y + 1);
	CalcTransform(/*0*/);
	int overlapped_area3 = CheckCollision(other);
	if(bCheckChildCollisions && !overlapped_area3)
	{
		outputArray.Clear();
		overlapped_area3 = Engine()->Tree()->CheckChildCollision(this, other, &outputArray);
	}

	SetPos(current.x, current.y - 1);
	CalcTransform(/*0*/);
	int overlapped_area4 = CheckCollision(other);
	if(bCheckChildCollisions && !overlapped_area4)
	{
		outputArray.Clear();
		overlapped_area4 = Engine()->Tree()->CheckChildCollision(this, other, &outputArray);
	}

	dy = overlapped_area3 - overlapped_area4;

	//Restore position
	SetPos(current.x, current.y);
	CalcTransform(/*0*/);
}
