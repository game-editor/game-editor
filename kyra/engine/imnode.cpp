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
#include "imnode.h"
#include "engine.h"
#include "../util/glperformance.h"

#pragma warning ( disable : 4355 )



#ifdef USE_RTREE //maks
RTree<int, int, 2, float> rtree;

/*#pragma comment(lib,"C:/Downloads/SpatialIndex/spatialindex 0.60b/vc_lib/Release/rtree_lib.lib")
RStarTree rtree;*/


#endif

extern KrEngine *engine;

KrImNode::KrImNode() : treeNode( this ), bEnableHit(true)	//maks
{
	isText = false; //maks
	parentActor = NULL; //maks
	parent = 0;
	//engine = 0; //maks
	depth = 0;
	nodeId = -1;
	//userData = 0; //maks

	//maks for( int i=0; i<KR_MAX_WINDOWS; ++i )
	{
		invalid/*[0]*/ = true;
		visible/*[0]*/ = ENABLE;
		xTransform/*[0]*/.Set();
		compositeXForm/*[0]*/.Set();


		quality/*[0]*/ = KrQualityNone;
		compositeQuality/*[0]*/ = KrQualityFast;


		compositeBounds/*[0]*/.SetInvalid();
		bounds/*[0]*/.SetInvalid();

		/*
		//Invalidate by position: problems with CollisionFree (calls Walk())
		compositeXFormAnt[i].Set(-1, -1, -1, -1); //maks
		xTransformAnt[i].Set(-1, -1, -1, -1); //maks
		boundsAnt[i].SetInvalid(); //maks*/
	}

#ifdef USE_COLLISION_SECTOR //maks
	InvalidateSectorPos();
#endif

#ifndef USE_WALK
	::engine->NotifyNewNode(); //maks
#endif
}


/*void KrImNode::SetNodeName( const gedString& name ) //maks
{
	if ( engine )
	{
		if ( !nodeName.empty() )
			engine->Tree()->RemoveNodeNameHash( nodeName );
		engine->Tree()->AddNodeNameHash( name, this );
	}
	nodeName = name;
}*/


void KrImNode::SetNodeId( int id )
{
	if ( engine )
	{
		engine->Tree()->RemoveNodeIdHash( nodeId );
		engine->Tree()->AddNodeIdHash( id, this );
	}
	nodeId = id;
}




/*void KrImNode::CheckInvalidPos() //maks: //Invalidate by position: problems with CollisionFree (calls Walk())
{
	int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	for( int i=start; i<end; ++i )
	{
		if ( compositeXForm[i].x != compositeXFormAnt[i].x || compositeXForm[i].y != compositeXFormAnt[i].y ||
			 xTransformAnt[i].x != xTransform[i].x || xTransformAnt[i].y != xTransform[i].y )
		{
			invalid[i] = true;

			if(bounds[i] != boundsAnt[i] && bounds[i].IsValid()) 
			{
				engine->InvalidateRectangle(boundsAnt[i]);
			}

			engine->InvalidateRectangle(bounds[i]);

			compositeXFormAnt[i].x = compositeXForm[i].x;
			compositeXFormAnt[i].y = compositeXForm[i].y;

			xTransformAnt[i].x = xTransform[i].x;
			xTransformAnt[i].y = xTransform[i].y;

			boundsAnt[i] = bounds[i];
		}
	}

	//Check childs
	GlInsideNodeIt<KrImNode*> it = ChildTreeIterator();
	it.Begin(); 
	while( !it.Done() )
	{
		it.CurrentData()->CheckInvalidPos();
		it.Next();
	}
}*/

void KrImNode::SetScale( GlFixed xScale, GlFixed yScale/*, int win*/ )
{
	#ifdef DEBUG
		GLASSERT( xScale > 0 );
		GLASSERT( yScale > 0 );
	#endif

	/*GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	if ( win != KR_ALL_WINDOWS )
	{
		start = win;
		end   = win + 1;
	}*/

	//maks for( int i=start; i<end; ++i )
	{
		if ( xScale != xTransform/*[0]*/.xScale || yScale != xTransform/*[0]*/.yScale )
		{
			xTransform/*[0]*/.xScale = xScale;
			xTransform/*[0]*/.yScale = yScale;
			invalid/*[0]*/ = true;

#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
		}
	}
}


void KrImNode::SetTransform( KrMatrix2& xForm/*, int win*/ )
{
	#ifdef DEBUG
		GLASSERT( xForm.xScale > 0 );
		GLASSERT( xForm.yScale > 0 );
	#endif

	//GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	/*int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	if ( win != KR_ALL_WINDOWS )
	{
		start = win;
		end   = win + 1;
	}*/

	//maks for( int i=start; i<end; ++i )
	{	
		if ( xForm != xTransform/*[0]*/ )
		{
			xTransform/*[0]*/ = xForm;
			invalid/*[0]*/ = true;

#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
		}
	}
}





void KrImNode::Invalidate( /*int win*/ )
{
	//GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	/*int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	if ( win != KR_ALL_WINDOWS )
	{
		start = win;
		end   = win + 1;
	}*/
	
	//maks for( int i=start; i<end; ++i )
	{
		invalid/*[0]*/ = true;
#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
	}
}


void KrImNode::SetQuality( int _quality/*, int win*/ )
{
	//GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	/*int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	if ( win != KR_ALL_WINDOWS )
	{
		start = win;
		end   = win + 1;
	}*/
	
	//maks for( int i=start; i<end; ++i )
	{
		if ( _quality != quality/*[0]*/ )
		{
			quality/*[0]*/ = _quality;
			invalid/*[0]*/ = true;

#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
		}
	}
}



void KrImNode::Resort( KrImNode* resortMe )
{
	// See if we need to move at all. If so, take ourselves out
	// of the list and scoot up or down.
	int depth = resortMe->ZDepth();
	GlInsideNodeIt< KrImNode* > it( child );

	// resortMe should always be a child of this.
	it.SetCurrent( resortMe->treeNode.next );
	#ifdef DEBUG
		if ( !it.Done() ) GLASSERT( it.CurrentData()->Parent() == this );
	#endif

	if (    !it.Done()
	     && depth > it.CurrentData()->ZDepth() )
	{
		// Resort me is removed, so we need a new current
		it.Next();
		resortMe->treeNode.Remove();

		while (    !it.Done()
				&& depth > it.CurrentData()->ZDepth() )
		{
			it.Next();
		}
		it.InsertBefore( resortMe->treeNode );
	}

	it.SetCurrent( resortMe->treeNode.prev );
	#ifdef DEBUG
		if ( !it.Done() ) GLASSERT( it.CurrentData()->Parent() == this );
	#endif

	if (    !it.Done()
	     && depth < it.CurrentData()->ZDepth() )
	{
		it.Prev();
		resortMe->treeNode.Remove();

		while (    !it.Done()
				&& depth < it.CurrentData()->ZDepth() )
		{
			it.Prev();
		}
		it.InsertAfter( resortMe->treeNode );
	}
	#ifdef VALIDATE_DEBUG
		GlInsideNodeIt< KrImNode* > testIt( child );

		testIt.Begin();
		int testZ = testIt.CurrentData()->ZDepth();

// 		GLOUTPUT( "zdepth " );
		for( ; !testIt.Done(); testIt.Next() )
		{
			GLASSERT( testIt.CurrentData()->ZDepth() >= testZ );
// 			GLOUTPUT( "%x=%d ", testIt.CurrentData(), testIt.CurrentData()->ZDepth() );
			testZ = testIt.CurrentData()->ZDepth();
		}
// 		GLOUTPUT( "\n" );
	#endif
}


void KrImNode::CalcCompositeTransform( /*int i*/ ) //maks
{
	if(parent)
	{
		parent->CalcCompositeTransform(/*i*/);
	}

	CalcTransform(/*i*/);
}

void KrImNode::CalcTransform( /*int i*/ )
{
	// Running performance on this gave about equal
	// time consumption in the 3 sections. Color, quality, position.

	// Now transform this.
	compositeXForm/*[i]*/ = xTransform/*[i]*/;
	compositeCForm/*[i]*/ = cTransform/*[i]*/;

	if ( parent )
	{
		// Spacial
		compositeXForm/*[i]*/.Composite( parent->CompositeXForm( /*i*/ ) );

		// Color
		// This can actually be a little expensive, so check that
		// we don't have an identity first.
		if ( !parent->CompositeCForm( /*i*/ ).IsIdentity() )
		{
			compositeCForm/*[i]*/.Composite( parent->CompositeCForm( /*i*/ ) );
   		}

		// Transform the quality
		if ( compositeXForm/*[i]*/.IsScaled() )
		{
			compositeQuality/*[i]*/ = quality/*[i]*/;
			if ( compositeQuality/*[i]*/ == KrQualityNone )
			{
				KrImNode* node;
				for( node = parent; node; node = node->parent )
				{
					if ( node->quality/*[i]*/ != KrQualityNone )
					{
						compositeQuality/*[i]*/ = node->quality/*[i]*/;
						break;
					}
				}
			}
			if ( compositeQuality/*[i]*/ == KrQualityNone )
			{
				compositeQuality/*[i]*/ = KrQualityFast;
			}
  		}
		else
		{
			compositeQuality/*[i]*/ = KrQualityFast;
		}
	}

	
//	GLOUTPUT( "KrImNode '%s' win=%d rel=%f,%f  composite=%f,%f\n", 
//			  NodeName().c_str(),
//			  i,
//			  xTransform/*[i]*/.x.ToDouble(),
//			  xTransform/*[i]*/.y.ToDouble(),
//			  compositeXForm/*[i]*/.x.ToDouble(),
//			  compositeXForm/*[i]*/.y.ToDouble() );


#ifdef USE_COLLISION_SECTOR //maks
	UpdateSectors();
#endif

#ifdef USE_RTREE //maks
	//maks: for rtree update
	boundsAnt/*[i]*/ = bounds/*[i]*/;	
#endif
}

#ifdef USE_COLLISION_SECTOR //maks

int sectorSizeW = 0, sectorSizeH = 0, log2SectorSizeW = 1, log2SectorSizeH = 1;

void KrImNode::UpdateSectors() 
{
	//maks: for collision test optimization
	/*Sector optimization (Only for actors smaller than sectorsize/2 and with collision detection enable):
		(http://www.gamespp.com/algorithms/collisionDetectionHow.html)

		- Get the absolute position: (x, y)
		- Translate until get all values positive in the game world
		- Calculate the sector coordinates: 

			- top-left:		sc = ( x >> kLog2SectorSize, y >> kLog2SectorSize)
			- bottom-right:	sc_br = (x+width) >> kLog2SectorSize, (y+height) >> kLog2SectorSize)

			if(sc_br_x != sc_x)
			{
				if(sc_br_y != sc_y)
				{
					//Actor in 4 sectors
				}
				else
				{
					//Actor in sc and right
				}
			}
			else if(sc_br_y != sc_y)
			{
				//Actor in sc and bottom
			}

		- If current sc != previous_sc then remove the old sc and create a new on sc map
		- Add the new sc to the sc linked list
		- Store the pointer for the current sc list and your coordinates

		- Check the colision sector by sector
		- After sector test, check the actors that handle collision with 
		  actors greater than sectorsize/2 and with collision detection enable

		  */
	
	//Assume scale = 1
	//Must invalidate anterior position if change sprite frame


	if(!parentActor) 
		return;

	KrVector2 pos;

	pos.Set(compositeXForm/*[0]*/.x.ToIntRound(), compositeXForm/*[0]*/.y.ToIntRound());

	if(pos != posAnt)
	{
		KrVector2 newSectorCoord;

		newSectorCoord.Set(
			(pos.x + MAX_KYRA_INT) >> log2SectorSizeW,
			(pos.y + MAX_KYRA_INT) >> log2SectorSizeH
			);

		if(newSectorCoord != sectorCoord)
		{
			mapSector.Remove(sectorCoord);
			mapSector.Add(newSectorCoord, parentActor);

			pNewList = mapSector.FindOrCreate(newSectorCoord);			
			pNewList->Add(parentActor, 1);

			sectorCoord = newSectorCoord;
		}
		
		posAnt = pos;
	}
}

void KrImNode::InvalidateSectorPos()
{
	posAnt.Set(MAX_KYRA_INT, MAX_KYRA_INT);
}

int RoundToPower2(int x, int &log2)
{
	int n = 2;
	log2 = 1;

	while(n < x && n > 0)
	{
		n <<= 1;
		log2++;
	}

	return n;
}

void KrImNode::InitSectors(int screenW, int screenH)
{
	//Define sector size
	sectorSizeW = screenW/10;
	sectorSizeH = screenH/10;

	//Round to next power 2
	sectorSizeW = RoundToPower2(sectorSizeW, log2SectorSizeW);
	sectorSizeH = RoundToPower2(sectorSizeH, log2SectorSizeH);

	//Delete old sectors
	mapSector.Clear();

}
#endif

#ifdef USE_RTREE //maks
void KrImNode::UpdateRTree(int i) //maks
{
	if(parentActor)
	{
		int rMin[2], rMax[2];
		
		//Remove old position
		if(boundsAnt[i].IsValid())
		{
			rMin[0] = boundsAnt[i].xmin;
			rMin[1] = boundsAnt[i].ymin;
			
			rMax[0] = boundsAnt[i].xmax;
			rMax[1] = boundsAnt[i].ymax;
			
			rtree.Remove(rMin, rMax, (int)parentActor);
		}
		
		//Add new position
		if(bounds[i].IsValid())
		{
			rMin[0] = bounds[i].xmin;
			rMin[1] = bounds[i].ymin;
			
			rMax[0] = bounds[i].xmax;
			rMax[1] = bounds[i].ymax;
			
			rtree.Insert(rMin, rMax, (int)parentActor);
		}
	}
}
#endif

KrImNode::~KrImNode()
{
#ifdef USE_RTREE //maks
	int rMin[2], rMax[2];
	
	//Remove old position
	if(boundsAnt/*[0]*/.IsValid())
	{
		rMin[0] = boundsAnt/*[0]*/.xmin;
		rMin[1] = boundsAnt/*[0]*/.ymin;
		
		rMax[0] = boundsAnt/*[0]*/.xmax;
		rMax[1] = boundsAnt/*[0]*/.ymax;
		
		rtree.Remove(rMin, rMax, (int)parentActor);
	}
	
	//Remove new position
	if(bounds/*[0]*/.IsValid())
	{
		rMin[0] = bounds/*[0]*/.xmin;
		rMin[1] = bounds/*[0]*/.ymin;
		
		rMax[0] = bounds/*[0]*/.xmax;
		rMax[1] = bounds/*[0]*/.ymax;
		
		rtree.Remove(rMin, rMax, (int)parentActor);
	}
#endif
}

void KrImNode::SetZDepth( int z )
{
	if ( z != depth )
	{
		depth   = z;

// 		int count = KR_MAX_WINDOWS;
// 		if ( Engine() ) count = Engine()->NumWindows();
// 		for( int i=0; i<count; ++i )
// 		{
// 			invalid[i] = true;
// 		}
		Invalidate( /*KR_ALL_WINDOWS*/ );

		// If there is no parent, this node is not in a tree. That's
		// fine -- we'll sort it in correctly when it gets added.
		if ( parent )
		{
			// Ask our parent to move us to the correct z-position.
			parent->Resort( this );

			//Need this?
#ifndef USE_WALK
			//::engine->Tree()->Walk1( this ); //maks
#endif
		}
	}
}


/*bool KrImNode::IsVisible( int window ) //maks
{
	KrImNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->visible[window] == DISABLE )
			return false;
	}
	return true;
}*/


void KrImNode::SetVisible( int _visible/*, int win*/ ) //maks
{
	//GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );

	//maks if ( win == KR_ALL_WINDOWS )
	/*{
		int count = KR_MAX_WINDOWS;
		if ( Engine() ) count = Engine()->NumWindows();

		for( int i=0; i<count; ++i )
		{
			if ( visible[i] != _visible )
			{
				visible[i] = _visible;
				invalid[i] = true;
			}	
		}
	}
	else*/
	{
		if ( visible/*[0]*/ != _visible )
		{
			visible/*[0]*/ = _visible;
			invalid/*[0]*/ = true;

#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
		}	
	}
}


void KrImNode::ScreenToObject( int x, int y, KrVector2T< GlFixed >* object/*, int i*/ )
{
	object->x = ( GlFixed( x ) - compositeXForm/*[i]*/.x ) / compositeXForm/*[i]*/.xScale;
	object->y = ( GlFixed( y ) - compositeXForm/*[i]*/.y ) / compositeXForm/*[i]*/.yScale;
}

void KrImNode::ScreenToObject( const GlFixed& x, const GlFixed& y, KrVector2T< GlFixed >* object/*, int i*/ ) //maks
{
	object->x = ( x - compositeXForm/*[i]*/.x ) / compositeXForm/*[i]*/.xScale;
	object->y = ( y - compositeXForm/*[i]*/.y ) / compositeXForm/*[i]*/.yScale;
}

void KrImNode::ObjectToScreen( int x, int y, KrVector2T< GlFixed >* screen/*, int i*/ ) //maks
{
	screen->x = x * compositeXForm/*[i]*/.xScale + compositeXForm/*[i]*/.x;
	screen->y = y * compositeXForm/*[i]*/.yScale + compositeXForm/*[i]*/.y;	
}


void KrImNode::AddedtoTree()
{
	GLASSERT( Engine() );
	//for( int i=0; i<Engine()->NumWindows(); ++i )
	{
		CalcTransform( /*i*/ );
		Invalidate( /*i*/ );
	}
}


void KrImNode::LeavingTree()
{
	// Nothing!
}


//KrWidget* KrImNode::TopWidget()
//{
//	KrWidget* w = 0;
//	for( KrImNode* node = Parent(); 
//		 node; 
//		 node = node->Parent() )
//	{
//		if ( node->ToWidget() )
//			w = node->ToWidget();
//		else
//			break;		// if it's not a widget, it will never recurse down to this object,
//						// so it can't be our parent.
//	}
//	return w;
//}


//KrWidget* KrImNode::GetWidget()
//{
//	if ( ToWidget() )
//		return ToWidget();
//	else if ( Parent() && Parent()->ToWidget() )
//		return Parent()->ToWidget();
//	else
//		return 0;
//}


KrImNode* KrImNode::Clone()
{
	return new KrImNode();
}
void KrImNode::SetRotation( double rotation) //akr
{

	if ( rotation != xTransform/*[0]*/.rotation )
	{
		xTransform/*[0]*/.rotation = rotation;
		invalid/*[0]*/      = true; //Make true (Invalidate by position: problems with CollisionFree (calls Walk()))
		
		::engine->Tree()->InvalidateTree(); //maks: solve Text Region.ged bug
		
#ifndef USE_WALK
		::engine->Tree()->Walk1( this ); //maks
#endif
	}
}

void KrImNode::SetPos( GlFixed x, GlFixed y/*, int win*/ ) //maks
{
	//GLASSERT(win == 0 || win == KR_ALL_WINDOWS);
	
	/*#ifdef DEBUG
	// There are no reason for these limits rather that
	// a way to check for bad input -- the engine can handle
	// inputs much larger than this.
	GLASSERT( x > -40000 && x < 40000 );
	GLASSERT( y > -40000 && y < 40000 );
	#endif
	
	  GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	  int start = 0;
	  int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;
	  
		if ( win != KR_ALL_WINDOWS )
		{
		start = win;
		end   = win + 1;
		}
		
		  for( int i=start; i<end; ++i )
		  {
		  if ( x != xTransform[i].x || y != xTransform[i].y )
		  {
		  xTransform[i].x = x;
		  xTransform[i].y = y;
		  invalid[i]      = true; //Make true (Invalidate by position: problems with CollisionFree (calls Walk()))
		  }
		}*/
	
	if ( x != xTransform/*[0]*/.x || y != xTransform/*[0]*/.y )
	{
		xTransform/*[0]*/.x = x;
		xTransform/*[0]*/.y = y;
		invalid/*[0]*/      = true; //Make true (Invalidate by position: problems with CollisionFree (calls Walk()))

		::engine->Tree()->InvalidateTree(); //maks: solve Text Region.ged bug
		
#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
	}
}

void KrImNode::SetColor( const KrColorTransform& color/*, int win*/ )
{
	//GLASSERT( win >= KR_ALL_WINDOWS && win < KR_MAX_WINDOWS );
	/*int start = 0;
	int end   = Engine() ? Engine()->NumWindows() : KR_MAX_WINDOWS;

	if ( win != KR_ALL_WINDOWS )
	{
		start = win;
		end   = win + 1;
	}*/

	//maks for( int i=start; i<end; ++i )
	{
		if ( color != cTransform/*[0]*/ )
		{
			cTransform/*[0]*/ = color;
			invalid/*[0]*/ = true;

#ifndef USE_WALK
			::engine->Tree()->Walk1( this ); //maks
#endif
		}		
	}
}

bool KrImNode::IsInvalid( /*int window,*/ bool bReturnInvalidIfParentIsInvalid ) //maks
{	
	//GLASSERT( window >= 0 && window < KR_MAX_WINDOWS );

	if(bReturnInvalidIfParentIsInvalid)
	{
		KrImNode *node = this;
		do
		{
			if(node->invalid/*[ window ]*/) 
				return true;

			node = node->parent;

		} while(node);

		return false;
	}
	else
	{
		return invalid/*[ window ]*/; 
	}
}