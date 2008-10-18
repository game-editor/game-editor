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
#include "sprite.h"
#include "engine.h"

KrSprite::KrSprite( KrSpriteResource* res, void *parent ) //maks
	: resource( res ),
	  frame( 0 )
{
	// Get the first action as a default.
	action = resource->GetActionByIndex( 0 );
	this->parentActor = parent; //maks

//	for( int i=0; i<KR_MAX_WINDOWS; ++i )
//		oldBounds[i] = bounds[i];

	infinite = NO_INFINITE; //maks
	frameInc = 0.0; //maks
}


void KrSprite::SetFrame( int i )
{
	//GLASSERT( i>=0 && i<action->NumFrames() ); //maks
	if ( i != frame )
	{
		frame = i;
		frame = GlClamp( frame, 0, action->NumFrames()-1 );

		Invalidate( /*KR_ALL_WINDOWS*/ ); //maks
	}
}


void KrSprite::SetAction( const gedString& actionName )
{ 
	KrAction* newaction = resource->GetAction( actionName ); 
	GLASSERT( newaction );
	if ( newaction && newaction != action )
	{
		Invalidate( /*KR_ALL_WINDOWS*/ ); //maks: erase old bound box

		action = newaction;
		frame = GlClamp( frame, 0, action->NumFrames()-1 );

#ifndef USE_WALK
		//maks: erase new bound box (need with Walk1())
		//solve the image cut when chage animation
		Invalidate( /*KR_ALL_WINDOWS*/ ); 
#endif
	}
}


void KrSprite::SetActionRotated( const gedString& actionName, int rotation )
{
	char buf[ 16 ];
	while( rotation < 0 )
		rotation += 360;
	while( rotation >= 360 )
		rotation -= 360;
	
	sprintf( buf, ".ROT%03d", rotation );
	gedString str = actionName + buf;

	SetAction( str );
}


void KrSprite::SetAction( U32 id )
{ 
	KrAction* newaction = resource->GetAction( id ); 
	if ( newaction != action )
	{
		Invalidate( /*KR_ALL_WINDOWS*/ );
		action = newaction;
		frame = GlClamp( frame, 0, action->NumFrames()-1 );
	}
}

bool IsTileActor(KrImNode *node); //maks
void KrSprite::Draw(	KrPaintInfo* paintInfo, 
						const KrRect& clip/*,
						int win*/ )
{	
	if(IsTileActor(this) && !IsVisible(/*win,*/ true)) return; //maks: solve the bug "Visible state "Don't draw, but allow events" don't works with tiles" (test only with tiles to avoid bugs in other games (alpha14.ged))
	GLASSERT( action );
	// This happens if an empty action (often "NONE" gets exported.)
	if ( visible/*[win]*/ != ENABLE || action->NumFrames() == 0 ) return; //maks

	/*action->Draw(	paintInfo, //maks
					frame, 
					CompositeXForm(win),
					CompositeCForm(win),
					clip,
					TreeDepth() );*/
	
	if(infinite == NO_INFINITE) //maks
	{
		action->Draw(	paintInfo,
					frame,
					CompositeXForm(/*win*/),
					CompositeCForm(/*win*/),
					clip,
					TreeDepth() );
	}
	else
	{
		KrMatrix2 xf = CompositeXForm(/*win*/);
		int x = xf.x.ToInt(), y = xf.y.ToInt();
		
		KrRect rect;
		action->CalculateBounds( frame, xf, &rect);
		
		int w = rect.Width(),
			h = rect.Height();

		//int count = 0;
		
		if(w > 0 && h > 0)
		{
			int yini, yfin;
			if(infinite == BOTH_INFINITE || infinite == Y_INFINITE) 
			{
				yini = (clip.ymin/h)*h + (y % h) - h;
				yfin = clip.ymax+h;
			}
			else
			{
				yini = y;
				yfin = yini + 1;
			}
			
			for(int j = yini; j < yfin; j += h)
			{
				xf.y = j;
				
				int xini, xfin;
				if(infinite == BOTH_INFINITE || infinite == X_INFINITE) 
				{
					xini = (clip.xmin/w)*w + (x % w) - w;
					xfin = clip.xmax+w;
				}
				else
				{
					xini = x;
					xfin = xini + 1;
				}
				
				for(int i = xini; i < xfin; i += w)
				{
					xf.x = i;
					
					action->Draw(paintInfo,
						frame, 
						xf,
						CompositeCForm(/*win*/),
						clip,
						TreeDepth() );
					
					//count++;
				}
			}			

			//GLOUTPUT("count: %ld\n", count);
		}
	}
}



bool KrSprite::DoStep(bool bForward, int gameFps) //maks
{
	bool bAnimationFinish = false;
	GLASSERT( action );
	GLASSERT( Engine() );
	if ( action )
	{
		if(!action->CanAnim(gameFps, frameInc)) return false;

		
		
		/*int count = KR_MAX_WINDOWS;
		if ( Engine() ) count = Engine()->NumWindows();*/

		

		//KrVector2 delta = action->Frame( frame ).StepSize();

		if(bForward) 
		{
			//maks: don't use step. If needed, use GlFixed instead int (solve shake bug.ged)
			//maks for( int i=0; i<count; ++i )
			/*{
#ifndef STAND_ALONE_GAME //maks
				if ( XTransform(0).IsScaled() )
				{
					int x = X(0) + ( XTransform(0).xScale * delta.x ).ToIntRound();
					int y = Y(0) + ( XTransform(0).yScale * delta.y ).ToIntRound();
					SetPos( x, y, 0 );
				}
				else
#endif
				{
					SetPos( X(0) + delta.x, Y(0) + delta.y, 0 );
				}
			}*/

			if(frame+1 == action->NumFrames()-1) bAnimationFinish = true;
			SetFrame( ( frame + 1 ) % action->NumFrames() );
		}
		else
		{
			//maks: don't use step. If needed, use GlFixed instead int (solve shake bug.ged)
			//maks for( int i=0; i<count; ++i )
			/*{
#ifndef STAND_ALONE_GAME //maks
				if ( XTransform(0).IsScaled() )
				{
					int x = X(0) - ( XTransform(0).xScale * delta.x ).ToIntRound();
					int y = Y(0) - ( XTransform(0).yScale * delta.y ).ToIntRound();
					SetPos( x, y, 0 );
				}
				else
#endif
				{
					SetPos( X(0) - delta.x, Y(0) - delta.y, 0 );
				}
			}*/
			
			if(frame-1 == 0) bAnimationFinish = true;
			else if(frame == 0) frame = action->NumFrames();
			SetFrame(frame - 1);
		}
	}

	return bAnimationFinish;
}


KrVector2 KrSprite::Stride()
{
	KrVector2 stride;
	stride.x = 0;
	stride.y = 0;

	GLASSERT( action );
	if ( action )
	{
		for( int i=0; i<action->NumFrames(); i++ )
		{
			stride += action->Frame( i ).StepSize();
		}
	}
	return stride;
}


void KrSprite::CalcTransform( /*int win*/ )
{
//	GLASSERT( oldBounds[win] == bounds[win] );
	// Calculate our new coordinates, and then bounding info.
	KrImNode::CalcTransform( /*win*/ );

	// Remember are coordinates are based on the hotspot, but
	// are bounds are based on the sprite.
	action->CalculateBounds( frame, CompositeXForm(/*win*/), &bounds/*[win]*/ );
//	oldBounds[win] = bounds[win];

	
	if(infinite == X_INFINITE || infinite == BOTH_INFINITE) //maks
	{
		bounds/*[win]*/.xmin = Engine()->ScreenBounds().xmin;
		bounds/*[win]*/.xmax = Engine()->ScreenBounds().xmax;
	}

	if(infinite == Y_INFINITE || infinite == BOTH_INFINITE) //maks
	{
		bounds/*[win]*/.ymin = Engine()->ScreenBounds().ymin;
		bounds/*[win]*/.ymax = Engine()->ScreenBounds().ymax;
	}

#ifdef USE_RTREE //maks
	UpdateRTree(/*win*/);
#endif
}

void KrSprite::CalcTransformInfinite( /*int win,*/ bool bCalcRealBounds ) //maks
{
	if(!bCalcRealBounds)
	{
		KrImNode::CalcTransform( /*win*/ );
	}

	// Remember are coordinates are based on the hotspot, but
	// are bounds are based on the sprite.
	action->CalculateBounds( frame, CompositeXForm(/*win*/), &bounds/*[win]*/ );

	if(!bCalcRealBounds)
	{
		if(infinite == X_INFINITE || infinite == BOTH_INFINITE)
		{
			bounds/*[win]*/.xmin = Engine()->ScreenBounds().xmin;
			bounds/*[win]*/.xmax = Engine()->ScreenBounds().xmax;
		}
		
		if(infinite == Y_INFINITE || infinite == BOTH_INFINITE)
		{
			bounds/*[win]*/.ymin = Engine()->ScreenBounds().ymin;
			bounds/*[win]*/.ymax = Engine()->ScreenBounds().ymax;
		}
	}
}

void KrSprite::AdjustInfinite(int &xo, int &yo/*, int window*/) //maks
{
	if(infinite != NO_INFINITE && action)
	{
		//To real actor coordinate
		KrMatrix2 xf = CompositeXForm(/*window*/);
		
		KrRect rect;
		action->CalculateBounds( frame, xf, &rect);				
		
		int w = rect.Width()/xf.xScale.ToDouble(),
			h = rect.Height()/xf.yScale.ToDouble();
		
		//int signx = 1, signy = 1;
		
		if(infinite == X_INFINITE || infinite == BOTH_INFINITE && w > 0)
		{
			int deltax = action->GetFrame( frame )->Hotspot().x;
			
			xo += deltax;
			
			if(xo < 0)
			{
				xo += (abs(xo)/w + 1)*w;
			}
			
			xo = xo % w;				
			xo -= deltax;
			
			/*if(xo < 0) signx = -1;
			xo = abs(xo);
			xo += w/2;
			xo = xo % w;
			xo -= w/2;
			xo *= signx;*/
		}
		
		if(infinite == Y_INFINITE || infinite == BOTH_INFINITE && h > 0)
		{
			int deltay = action->GetFrame( frame )->Hotspot().y;
			
			yo += deltay;
			
			if(yo < 0)
			{
				yo += (abs(yo)/h + 1)*h;
			}
			
			yo = yo % h;				
			yo -= deltay;
			
			/*if(yo < 0) signy = -1;
			yo = abs(yo);
			yo += h/2;
			yo = yo % h;
			yo -= h/2;
			yo *= signy;*/
		}
	}
}

bool KrSprite::HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results/*, int window*/ )
{
	if ( action && bEnableHit ) //maks
	{
		//int i = window;
		if (    IsVisible( /*i*/ )
			 && CompositeCForm(/*i*/).Alpha() != 0 )
		{
			KrVector2T< GlFixed > object;
			ScreenToObject( x, y, &object/*, i*/ );

			int xo = object.x.ToIntRound(),
				yo = object.y.ToIntRound();

			AdjustInfinite(xo, yo/*, window*/);

			if ( action->HitTestTransformed( frame, 
											 xo, yo, //maks
											 flags ) )
			{	
				if(results) //maks
					results->PushBack( this );
				return true;
			}
		}
	}
	return false;
}


void KrSprite::QueryBoundingBox( KrRect* rect/*, int win*/ )
{
	rect->SetInvalid();
	if ( action )
	{
		KrRle* rle = action->GetFrame( frame );
		rle->CalculateBounds( CompositeXForm( /*win*/ ), rect );
	}
}


KrImNode* KrSprite::Clone()
{
	KrSprite* sprite = new KrSprite( resource );
	sprite->SetAction( action->Id() );
	sprite->SetFrame( frame );
	return sprite;
}

