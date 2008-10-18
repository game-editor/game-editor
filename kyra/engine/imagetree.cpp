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
#include "imagetree.h"
#include "engine.h"
#include "../util/glperformance.h"
//#include "../util/glprimitive.h"

extern "C" int GetGameMode(); //maks

#ifndef KYRA_BUILD_INCLUDED
	#error Build file missing.
#endif

#ifndef STAND_ALONE_GAME //maks
extern int HasShadows(void *actor);
#endif

KrImageTree::KrImageTree( KrEngine* _engine )
	: idMap( 64 ), 
	//nameMap( 64 ),  //maks
	bInvalidTree(false)
{
	//int i;

	engine = _engine;
	engine->setTree(this);
	root		= new KrImNode;
	offsetRoot	= new KrImNode;
	treeDepth = 1; //maks
	root->GetCompositeBounds(/*0*/).Set(-MAX_KYRA_INT, -MAX_KYRA_INT, MAX_KYRA_INT, MAX_KYRA_INT); //maks
	offsetRoot->GetCompositeBounds(/*0*/).Set(-MAX_KYRA_INT, -MAX_KYRA_INT, MAX_KYRA_INT, MAX_KYRA_INT); //maks
	
	
	AddNode( root, offsetRoot );

	//for( i=0; i<engine->NumWindows(); ++i )
	{
		root->SetPos(	engine->ScreenBounds(/*i*/).xmin, 
						engine->ScreenBounds(/*i*/).ymin/*,
						i*/ );
		root->CalcTransform( /*i*/ );
	}

	#ifdef DEBUG
		GLOUTPUT( "Root: %x  Offset: %x\n", root, offsetRoot );
	#endif
}


KrImageTree::~KrImageTree()
{
	Clear( root );
}


void KrImageTree::Clear( KrImNode* parent )
{
	// Recursively delete the tree. It is important that
	// we delete from the outside in.

	GlInsideNode< KrImNode* >* child = &(parent->child);
	child = child->next;	// skip the sentinel
	while ( !child->IsSentinel() )
	{
		GlInsideNode< KrImNode* >* temp = child;
		child = child->next;

		Clear( temp->data );
	}
	delete parent;
}


void KrImageTree::AddNode( KrImNode* parent, KrImNode* addMe )
{
	if ( !parent )
	{
		parent = offsetRoot;
	}

	addMe->parent = parent;
	//addMe->engine = engine;

	// Adds a node, keeping the depth sorted correctly.
	GlInsideNodeIt< KrImNode* > it = parent->ChildTreeIterator();

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( addMe->ZDepth() < it.CurrentData()->ZDepth() )
		{
			it.InsertBefore( addMe->treeNode );
			break;
		}
	}
	if ( it.Done() )
	{
		// Add at the end. (It's a circular list - the end
		// is just before the sentinel.)
		it.InsertBefore( addMe->treeNode );
	}

	#ifdef VALIDATE_DEBUG
		// Validate!
		for( it.Begin(); !it.Done(); it.Next() )	
		{
			GlInsideNode< KrImNode* >* prev = it.CurrentNode()->prev;
			GlInsideNode< KrImNode* >* next = it.CurrentNode()->next;

			if ( !prev->IsSentinel() )
				GLASSERT( it.CurrentData()->ZDepth() >= prev->data->ZDepth() );
			if ( !next->IsSentinel() )
				GLASSERT( it.CurrentData()->ZDepth() <= next->data->ZDepth() );
		}		 
	#endif

	addMe->Invalidate( /*KR_ALL_WINDOWS*/ );

	if ( addMe->NodeId() >= 0 )
	{
        #ifdef DEBUG
        bool okay =
        #endif

        idMap.Add( addMe->NodeId(), addMe );

		GLASSERT( okay );
		#ifdef DEBUG
			//GLOUTPUT( "Node id=%d added\n", addMe->NodeId() );
		#endif
	}
	/*if ( !addMe->NodeName().empty() && addMe->NodeName().length() > 0 ) //maks
	{
        #ifdef DEBUG
        bool okay =
        #endif

        nameMap.Add( addMe->NodeName(), addMe );

        GLASSERT( okay );
	}*/

	// WARNING: the widgets will add nodes during this call, which makes
	// this function recursive.
	addMe->AddedtoTree();

	
	#ifdef VALIDATE_DEBUG
		ValidateTree( root );
	#endif
}


bool KrImageTree::DeleteNode( KrImNode* removeMe )
{
	bool ok = true;
	if(!removeMe) return false; //maks
	// This call is recursive -- it takes out the given
	// node and all the children.
	GlInsideNode< KrImNode* >* child = &(removeMe->child);
	child = child->next;	// skip the sentinel
	while ( !child->IsSentinel() )
	{
		GlInsideNode< KrImNode* >* temp = child;
		child = child->next;

		DeleteNode( temp->data );
	}

	// Unlink the node.
	removeMe->treeNode.Remove();
	// Give it a chance to clean up.
	removeMe->LeavingTree();

	if ( removeMe->NodeId() >= 0 )
	{	
		ok = idMap.Remove( removeMe->NodeId() );
		GLASSERT( ok );
	}

	/*if ( !removeMe->NodeName().empty() ) //maks
	{	
		ok = nameMap.Remove( removeMe->NodeName() );
		GLASSERT( ok );		
	}*/

	delete removeMe;
	
	#ifdef VALIDATE_DEBUG
		ValidateTree( root );
	#endif
	return ok;
}


void KrImageTree::Walk()
{	
	//for( int i=0; i<engine->NumWindows(); ++i )
	{
		treeDepth = 1;	// reserve depth 0 for background. Note that windows don't overlap,
						// so we can save some depths doing this
		Walk( root, root->IsInvalid( /*i*/ ), root->IsThisVisible()/*, i*/ );
//		if ( i == 2 && engine->DirtyRectangle( i )->NumRect())
//			engine->DirtyRectangle( i )->PrintRects( "KrImageTree::Walk done tree 2" );
	}

	bInvalidTree = false; //maks
}


void KrImageTree::Walk( KrImNode* walkNode, bool invalid, bool visible/*, int currentWin*/ )
{
	// The child will use the parents composite transformation
	// in its calculation, so parent nodes *must* be transformed
	// before the children.
	if ( invalid || walkNode->IsInvalid(/*currentWin*/) )
	{
		walkNode->FlushInvalid( /*currentWin,*/ true );	// Adds current bounds to DR

		// Even if invisible, we need the previous FlushInvalid, because
		// the visibility could have changed between the last frame and
		// this one. However, if the walkNode is now invisible, 
		// its new position can't cause a repaint.
		//
		// This means that invisible nodes don't have correct transforms.

		if ( visible )
		{
			walkNode->CalcTransform( /*currentWin*/ );		// Calculates new bounds.
			walkNode->FlushInvalid( /*currentWin,*/ false );	// Adds new bounds to DR
		}
		invalid = true;
	}

	// Our children will be drawn on top of us. Only used for OpenGL. In normal mode,
	// we draw the tree in order and don't need an explicit z-value.
	//GLASSERT( walkNode != root || treeDepth == 1 );
	walkNode->treeDepth = ++treeDepth;

	// A check for openGL. See notes where constant is defined.
	GLASSERT( treeDepth < kKrOglDistance );

	if ( visible )
	{
		// As we walk each child node, add its composite bounds to ours.
		walkNode->compositeBounds/*[currentWin]*/ = walkNode->bounds/*[currentWin]*/;
	}

	#ifdef DEBUG
		// Text -- damn it -- can have invalid bounds.
		// if ( walkNode->ToImage() ) GLASSERT( walkNode->bounds.IsValid() );
		if ( !walkNode->ToImage() ) GLASSERT( !walkNode->bounds/*[currentWin]*/.IsValid() );
	#endif

	// Even if not visible, it is critical to walk the children, in case
	// they were visible on the last frame.
	//
	for( 	GlInsideNode< KrImNode* >* childNode = walkNode->child.next;		// start after the sentinel
			childNode != &walkNode->child && childNode;   //maks  									// look for the sentinel
			childNode = childNode->next )
	{
		KrImNode* child = childNode->data;
		Walk( child, invalid, visible/*, currentWin*/ );

		// Check the child's composite bounds and add them in.
		// Note here, we can use visibility.
		if ( visible && child->CompositeBounds(/*currentWin*/).IsValid() )
		{
			if ( walkNode->compositeBounds/*[currentWin]*/.IsValid() )
				walkNode->compositeBounds/*[currentWin]*/.DoUnion( child->CompositeBounds(/*currentWin*/) );
			else
				walkNode->compositeBounds/*[currentWin]*/ =  child->CompositeBounds(/*currentWin*/);
		}
	}

	// We are done with this node and all its children, so
	// its invalid state will be cleared. It will be re-drawn
	// based on the dirty rectangle, not the invalid state.
 	walkNode->ClearInvalid( /*currentWin*/ );
}

void KrImageTree::CalculateCompositBounds( KrImNode* walkNode ) //maks
{
	walkNode->CalcTransform( /*0*/ );		// Calculates new bounds.
	
	// As we walk each child node, add its composite bounds to ours.
	walkNode->compositeBounds/*[0]*/ = walkNode->bounds/*[0]*/;
	
	
	// Even if not visible, it is critical to walk the children, in case
	// they were visible on the last frame.
	//
	for( 	GlInsideNode< KrImNode* >* childNode = walkNode->child.next;		// start after the sentinel
	childNode != &walkNode->child && childNode;   //maks  									// look for the sentinel
	childNode = childNode->next )
	{
		KrImNode* child = childNode->data;
		
		// Check the child's composite bounds and add them in.
		// Note here, we can use visibility.
		if ( child->CompositeBounds(/*0*/).IsValid() )
		{
			if ( walkNode->compositeBounds/*[0]*/.IsValid() )
				walkNode->compositeBounds/*[0]*/.DoUnion( child->CompositeBounds(/*0*/) );
			else
				walkNode->compositeBounds/*[0]*/ =  child->CompositeBounds(/*0*/);
		}
	}
}

#ifndef USE_WALK
void KrImageTree::Walk1( KrImNode* walkNode )
{	
	treeDepth = 1;	// reserve depth 0 for background. Note that windows don't overlap
	Walk1(  walkNode, true, walkNode->IsThisVisible()/*, 0*/ );
}

void KrImageTree::Walk1( KrImNode* walkNode, bool invalid, bool visible, int currentWin )
{
	// The child will use the parents composite transformation
	// in its calculation, so parent nodes *must* be transformed
	// before the children.
	if ( invalid || walkNode->IsInvalid(currentWin) )
	{
		walkNode->FlushInvalid( currentWin, true );	// Adds current bounds to DR

		// Even if invisible, we need the previous FlushInvalid, because
		// the visibility could have changed between the last frame and
		// this one. However, if the walkNode is now invisible, 
		// its new position can't cause a repaint.
		//
		// This means that invisible nodes don't have correct transforms.

		if ( visible )
		{
			walkNode->CalcTransform( currentWin );		// Calculates new bounds.
			walkNode->FlushInvalid( currentWin, false );	// Adds new bounds to DR
		}
		invalid = true;
	}

	// Our children will be drawn on top of us. Only used for OpenGL. In normal mode,
	// we draw the tree in order and don't need an explicit z-value.
	//GLASSERT( walkNode != root || treeDepth == 1 );
	bool bAdustParent = treeDepth == 1; //maks
	walkNode->treeDepth = ++treeDepth;

	// A check for openGL. See notes where constant is defined.
	//GLASSERT( treeDepth < kKrOglDistance );

	if ( visible )
	{
		// As we walk each child node, add its composite bounds to ours.
		walkNode->compositeBounds[currentWin] = walkNode->bounds[currentWin];
	}

	#ifdef DEBUG
		// Text -- damn it -- can have invalid bounds.
		// if ( walkNode->ToImage() ) GLASSERT( walkNode->bounds.IsValid() );
		if ( !walkNode->ToImage() ) GLASSERT( !walkNode->bounds[currentWin].IsValid() );
	#endif

	// Even if not visible, it is critical to walk the children, in case
	// they were visible on the last frame.
	//
	for( 	GlInsideNode< KrImNode* >* childNode = walkNode->child.next;		// start after the sentinel
			childNode != &walkNode->child && childNode;   //maks  									// look for the sentinel
			childNode = childNode->next )
	{
		KrImNode* child = childNode->data;
		Walk1( child, invalid, visible, currentWin );

		// Check the child's composite bounds and add them in.
		// Note here, we can use visibility.
		if ( visible && child->CompositeBounds(currentWin).IsValid() )
		{
			if ( walkNode->compositeBounds[currentWin].IsValid() )
				walkNode->compositeBounds[currentWin].DoUnion( child->CompositeBounds(currentWin) );
			else
				walkNode->compositeBounds[currentWin] =  child->CompositeBounds(currentWin);
		}
	}

	// We are done with this node and all its children, so
	// its invalid state will be cleared. It will be re-drawn
	// based on the dirty rectangle, not the invalid state.
 	walkNode->ClearInvalid( currentWin );

	if(bAdustParent) //maks
	{
		KrImNode *parentNode = walkNode->parent, *childNode = walkNode;
		while(parentNode)
		{
			parentNode->compositeBounds[currentWin].DoUnion( childNode->compositeBounds[currentWin] );
			childNode = parentNode;
			parentNode = parentNode->parent;
		}
	}
}
#endif

void KrImageTree::DrawWalk( const KrRect& dr, KrPaintInfo* info/*, int win*/ )
{
	#ifdef DEBUG
 		if ( info->OpenGL() )
		{
			GLASSERT( dr == engine->ScreenBounds( /*win*/ ) );
			//GLASSERT( root->CompositeBounds(/*win*/).IsValid() ); Can be invalid for tree with no images.
			//GLASSERT( root->CompositeBounds(/*win*/).Intersect( dr ) );
		}
	#endif

	if (    root->CompositeBounds(/*win*/).IsValid()
		 && root->CompositeBounds(/*win*/).Intersect( dr ) )
	{
		DrawWalk( dr, root, info/*, win*/ );
	}
}


// Child nodes in Kyra are drawn on top of their parents. This recursive
// call needs to be organized so that this property is maintained.
// It is assumed that the caller has called with valid composite
// bounds.

void KrImageTree::DrawWalk( const KrRect& dr, KrImNode* node, KrPaintInfo* info/*, int win*/ )
{
	GLASSERT( node->CompositeBounds(/*win*/).IsValid() );
	GLASSERT( dr.Intersect( node->CompositeBounds(/*win*/) ) );

	// Draw the children first on top, so the parent draws first.
	// For opengl mode, the depth has been written to the image nodes,
	// since there is no draw order.
	if ( node->Bounds(/*win*/).IsValid() && node->IsVisible(/*win*/) )
	{
		if ( dr.Intersect( node->Bounds(/*win*/) ) )
		{
			GLASSERT( node->ToImage() );
			KrImage* image = node->ToImage();
			if ( image )
			{
				image->Draw( info, dr/*, win*/ );
			}
		}
	}

	GlInsideNodeIt< KrImNode* > it( node->child );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if (    it.CurrentData()->CompositeBounds(/*win*/).IsValid()
		     && it.CurrentData()->CompositeBounds(/*win*/).Intersect( dr )
			 && it.CurrentData()->IsVisible(/*win*/) )
		{
			DrawWalk( dr, it.CurrentData(), info/*, win*/ );
		}
	}
}


/*void KrImageTree::AddNodeNameHash( const gedString& name, KrImNode* node ) //maks
{
    #ifdef DEBUG
    bool okay =
    #endif

    nameMap.Add( name, node );

	GLASSERT( okay );
}*/


/*void KrImageTree::RemoveNodeNameHash( const gedString& name ) //maks
{
	#ifdef DEBUG
	bool ret =
	#endif
	nameMap.Remove( name );
	GLASSERT( ret );
}*/


void KrImageTree::RemoveNodeIdHash( int id )
{
	idMap.Remove( id );
}


void KrImageTree::AddNodeIdHash( int id, KrImNode* node )
{
    idMap.Add( id, node );
}

KrImNode* KrImageTree::FindNodeById( int id )
{
	KrImNode* ret = 0;
	idMap.Find( id, &ret );
	return ret;
}


/*KrImNode* KrImageTree::FindNodeByName( const gedString& name ) //maks
{
	KrImNode* ret = 0;
	nameMap.Find( name, &ret );
	return ret;
}*/

#ifdef DEBUG
void KrImageTree::ValidateTree( KrImNode* root )
{
	GlInsideNodeIt< KrImNode* > it( root->child );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		// Check the depths and the parents.
		GLASSERT( it.CurrentData()->Parent() == root );

		if ( !it.CurrentNode()->prev->IsSentinel() )
			GLASSERT( it.CurrentNode()->prev->data->ZDepth() <= it.CurrentData()->ZDepth() );
		if ( !it.CurrentNode()->next->IsSentinel() )
			GLASSERT( it.CurrentNode()->next->data->ZDepth() >= it.CurrentData()->ZDepth() );

		ValidateTree( it.CurrentData() );
	}
}
#endif


void KrImageTree::HitTest( int x, int y, int flags, GlDynArray<KrImage*>* outputArray/*, int* window*/ )
{
	outputArray->Clear();
	//*window = -1;

	// Figure out which window this hit-test is in.
	//for( int i=0; i<engine->NumWindows(); ++i )
	{
		KrRect bounds = engine->ScreenBounds( /*i*/ );
		if(GetGameMode() || bounds.Intersect( x, y )) //maks: calculates the hits in all game area when in game mode
		{
			//*window = i;
			HitTestRec( offsetRoot, x, y, flags, outputArray/*, *window*/ );
			//break;
		}
	}

	/*if ( *window >= 0 )
	{
		HitTestRec( offsetRoot, x, y, flags, outputArray, *window );	
	}*/
}




bool KrImageTree::HitTestRec( KrImNode* node, int x, int y, int flags, GlDynArray<KrImage*>* output/*, int window*/ )
{
	// Check our composite bounds, if these don't intersect, no dice.


 	if ( !node->CompositeBounds( /*window*/ ).Intersect( x, y ) )
	{
		return false;
	}
#ifndef STAND_ALONE_GAME //maks
	else
	{
		//Test panel shadows
		int shadowTickness = HasShadows(node->getParentActor());
		if(shadowTickness)
		{
			KrRect r = node->CompositeBounds( /*window*/ );
			if(x > (r.xmax - shadowTickness) || y > (r.ymax - shadowTickness))
			{
				//Don't hit at panel shadows
				return false;
			}
		}
	}
#endif
	
	// Walk the children first, since they are on top of the parent.
	// Note that we need to go in reverse order, so the object
	// closest to the user is clicked first.
	GlInsideNodeIt< KrImNode* > it( node->child );

	for( it.Last(); !it.Done(); it.Prev() )
	{
// 		HitTestRec( it.CurrentData(), x, y, flags, output, window );
		bool abort = HitTestRec( it.CurrentData(), x, y, flags, output/*, window*/ );
		if ( abort == true )
			return true;
	}

	if ( node->ToImage() )
	{
		// Ignore transparent and invisible items.
// 		if (    node->CompositeCForm().Alpha() > 0 )
// // // 			 || flags & HIT_TRANSPARENT_IMAGES )
// 		{	
// 			if ( node->IsVisible() )
// 			{ 
				bool hit = node->HitTest( x, y, flags, output/*, window*/ );
				if ( hit )
				{
					if ( flags & GET_ALL_HITS )
						return false;	// don't abort: get everything.
					else	
						return true;	// return on the first hit.
				}
// 			}
// 		}
	}
	return false;	// keep going.
}

void KrImageTree::HitTestIntersect(KrImNode* root, const KrRect &rect, int flags, GlDynArray<KrImage*>* outputArray/*, int* window*/ ) //maks
{
	outputArray->Clear();
	//*window = -1;

	// Figure out which window this hit-test is in.
	//for( int i=0; i<engine->NumWindows(); ++i )
	{
		KrRect bounds = engine->ScreenBounds( /*i*/ );
		if ( bounds.Intersect(rect) )
		{
			//*window = i;
			
			if(!root)
			{
				HitTestRecIntersect( offsetRoot, rect, flags, outputArray/*, *window*/ );	
			}
			else
			{
				// Walk the children first, since they are on top of the parent.
				// Note that we need to go in reverse order, so the object
				// closest to the user is clicked first.
				GlInsideNodeIt< KrImNode* > it( root->child );
				
				for( it.Last(); !it.Done(); it.Prev() )
				{
					bool abort = HitTestRecIntersect( it.CurrentData(), rect, flags, outputArray/*, *window*/ );
					if ( abort == true )
					{
						if(!(flags & GET_ALL_HITS))
							return;
					}
				}
			}

			//break;
		}
	}

	/*if ( *window >= 0 )
	{
		if(!root)
		{
			HitTestRecIntersect( offsetRoot, rect, flags, outputArray, *window );	
		}
		else
		{
			// Walk the children first, since they are on top of the parent.
			// Note that we need to go in reverse order, so the object
			// closest to the user is clicked first.
			GlInsideNodeIt< KrImNode* > it( root->child );
			
			for( it.Last(); !it.Done(); it.Prev() )
			{
				bool abort = HitTestRecIntersect( it.CurrentData(), rect, flags, outputArray, *window );
				if ( abort == true )
				{
					if(!(flags & GET_ALL_HITS))
						return;
				}
			}
		}
	}*/
}

bool KrImageTree::HitTestRecIntersect( KrImNode* node, const KrRect &rect, int flags, GlDynArray<KrImage*>* output/*, int window*/ ) //maks
{
	// Check our composite bounds, if these don't intersect, no dice.
 	if ( !node->CompositeBounds( /*window*/ ).Intersect(rect) )
	{
		return false;
	}
	
	// Walk the children first, since they are on top of the parent.
	// Note that we need to go in reverse order, so the object
	// closest to the user is clicked first.
	GlInsideNodeIt< KrImNode* > it( node->child );

	for( it.Last(); !it.Done(); it.Prev() )
	{
		bool abort = HitTestRecIntersect( it.CurrentData(), rect, flags, output/*, window*/ );
		if ( abort == true )
			return true;
	}
	
	output->PushBack(node->ToImage());
	
	if ( flags & GET_ALL_HITS )
		return false;	// don't abort: get everything.

	return true;	// return on the first hit.
}

bool KrImageTree::CheckSiblingCollision( KrImNode* node, GlDynArray<KrImage*>* outputArray/*, int window*/ )
{
	bool ret = false;
	outputArray->Clear();

	KrImNode* parent   = node->Parent();
	KrImage* checkThis = node->ToImage();
	if (	parent 
		 && checkThis )
	{
		GlInsideNodeIt< KrImNode* > it( parent->child );

		for( it.Begin(); !it.Done(); it.Next() )
		{
			if (    it.CurrentData() != checkThis
				 && it.CurrentData()->ToImage()
			     && checkThis->CheckCollision( it.CurrentData()->ToImage()/*, window*/ ) )
			{
				ret = true;
				outputArray->PushBack( it.CurrentData()->ToImage() );
			}
		}
	}
	return ret;
}


int KrImageTree::CheckChildCollision( KrImNode* check, KrImNode* parent, GlDynArray<KrImage*>* outputArray/*, int window*/ ) //maks
{
	int ret = 0, overlapped_area = 0;
	outputArray->Clear();

	KrImage* checkThis = check->ToImage();
	if (	checkThis
		 && parent->CompositeBounds( /*window*/ ).Intersect( checkThis->Bounds( /*window*/ ) ) )
	{
		GlInsideNodeIt< KrImNode* > it( parent->child );

		for( it.Begin(); !it.Done(); it.Next() )
		{
			if (    it.CurrentData() != checkThis
				 && it.CurrentData()->ToImage()
			     && (overlapped_area = checkThis->CheckCollision( it.CurrentData()->ToImage()/*, window*/ )) > 0 )
			{
				ret += overlapped_area;
				outputArray->PushBack( it.CurrentData()->ToImage() );
			}
		}
	}

	return ret;
}


bool KrImageTree::CheckAllCollision( KrImNode* checkThis, GlDynArray<KrImage*>* outputArray/*, int window*/ )
{
	bool ret = false;
	outputArray->Clear();

	if ( checkThis->ToImage() )
		CheckAllCollisionWalk( &ret, Root(), checkThis->ToImage(), outputArray/*, window*/ );
	return ret;
}


void KrImageTree::CheckAllCollisionWalk( bool* hit, KrImNode* node, KrImage* checkThis, GlDynArray<KrImage*>* outputArray/*, int window*/ )
{
	// check all the children:
	GlInsideNodeIt< KrImNode* > it( node->child );

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if (    it.CurrentData() != checkThis
			 && it.CurrentData()->ToImage()
			 && checkThis->CheckCollision( it.CurrentData()->ToImage()/*, window*/ ) )
		{
			*hit = true;
			outputArray->PushBack( it.CurrentData()->ToImage() );
		}

		if ( it.CurrentData()->CompositeBounds( /*window*/ ).Intersect( checkThis->Bounds( /*window*/ ) ) )
			CheckAllCollisionWalk( hit, it.CurrentData(), checkThis, outputArray/*, window*/ );
	}
}

bool KrImageTree::SetParent( KrImNode* parent, KrImNode* node ) //maks
{
	if(!parent || !node || node->parent == parent)
		return false;

	//Verify cyclical parent
	KrImNode *testParent = parent, *oldParent = node->parent;
	do
	{
		if(testParent == node)
			return false;

		testParent = testParent->parent;
	} while(testParent);

	//Remove node of old parent childs
	
	GlInsideNodeIt< KrImNode* > itOld = node->parent->ChildTreeIterator();

	for( itOld.Begin(); !itOld.Done(); itOld.Next() )
	{
		if(itOld.CurrentData() == node)
		{
			itOld.CurrentNode()->Remove();
			break;
		}
	}

	//Set new parent
	node->parent = parent;

	// Adds a node, keeping the depth sorted correctly.
	GlInsideNodeIt< KrImNode* > it = parent->ChildTreeIterator();

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( node->ZDepth() < it.CurrentData()->ZDepth() )
		{
			it.InsertBefore( node->treeNode );
			break;
		}
	}
	if ( it.Done() )
	{
		// Add at the end. (It's a circular list - the end
		// is just before the sentinel.)
		it.InsertBefore( node->treeNode );
	}


	if ( node->NodeId() >= 0 )
	{
        idMap.Add( node->NodeId(), node );
	}

	/*if ( !node->NodeName().empty() && node->NodeName().length() > 0 ) //maks
	{
        nameMap.Add( node->NodeName(), node );
 	}*/
	
#ifndef USE_WALK
	engine->Tree()->Walk1( oldParent ); //maks
	engine->Tree()->Walk1( parent ); //maks
#endif

	engine->InvalidateRectangle(engine->FullScreenBounds());
	//engine->Draw(); //maks: why need this Draw()? (causes flick when set the parent)
	return true;
}

