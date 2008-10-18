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


#ifndef KYRA_RENDER_INCLUDED
#define KYRA_RENDER_INCLUDED

#include "../util/gllist.h"
#include "SDL.h"
#include "../engine/krmath.h"
#include "../util/glmap.h"

// Include the other image headers, so clients just need
// to #include "image.h"

#define USE_WALK //maks
/*
maks: Define USE_WALK to not use Walk1 optimization
Walk1 bugs:
		- Bug "ListPopSelect highlight is very slow"
		- Bug "Tutorial Trace.ged deixa uma bola na posição errada quando bate na parede"	
				
*/

#include "sprite.h"

class KrEngine;
class KrMappedRect;


/**	The ImageTree contains all the Image Nodes.
*/
class KrImageTree
{
  public:
	  bool SetParent( KrImNode* parent, KrImNode* node ); //maks
	  void InvalidateTree() {bInvalidTree = true;} //maks
	  bool IsTreeInvalid() {return bInvalidTree;} //maks

	/*  The tree has to live in an engine in order
		to get to dirty rectangles and StartLists.
	*/
	KrImageTree( KrEngine* engine );
	~KrImageTree();

	/// Return the very base node of the tree. It is a KrImNode and never a more derived type.
	KrImNode* Root()	{ return offsetRoot; }

	/** Add a ImNode to the tree. The parent can be specified; if
		not, the root will be used.
		The node added -- 'addMe' -- is handed over by ownership. It
		will be deleted when the tree is deleted or DeleteNode is
		called.

		Node children cannot directly be added. There is no
		KrImNode::AddChild function for instance; everything must
		be added through the tree.
	*/
	void	AddNode( KrImNode* parent, KrImNode* addMe );
	
	/** Call this to delete a node and remove it from the tree.

		@WARNING: DeleteNode( this ) can cause a crash.
				  It's a temptingly useful thing, but in the case
				  where parent objects hold on to pointers, those
				  pointers go dangling. It's best to avoid
				  DeleteNode( this ) usage.
	*/
	bool	DeleteNode( KrImNode* removeMe );

	///	Find a ImNode by its id.
	KrImNode*	FindNodeById( int id );
	///	Find a ImNode by its name.
	//KrImNode*	FindNodeByName( const gedString& name ); //maks

	/** Given a point -- in screen coordinates -- determine
		what ImageNodes are under that point. Note that only
		images have bounds and are drawn, so only images
		can be hit. 
		
		HitTest, by defualt, stops at the first non-transparent
		pixel it hits. There are flags to modify this behavior:

		- ALWAYS_INSIDE_BOX hits inside boxes that aren't filled
		- GET_ALL_HITS, not just the first
	*/
	void   HitTest( int x, int y, int flags,
					GlDynArray<KrImage*>* outputArray /*, int *windowIndex*/ );

	void HitTestIntersect(KrImNode* root, const KrRect &rect, int flags, GlDynArray<KrImage*>* outputArray/*, int* window*/ ); //maks

	/**	Given a node, check if it collides with any of its own siblings. Returns
		true if any collisions occur, and returns an array (outputArray)
		of all the KrImages collided with.

		See KrImage::CheckCollision for a discussion of collision issues.
	*/
	bool CheckSiblingCollision( KrImNode* checkThis,
								GlDynArray<KrImage*>* outputArray/*, int window = 0*/ );

	/**	Given a node "checkThis" and a parent, check if "checkThis"
		collides with any of the children of the parent. There
		does not need to be any relationship between "checkThis" and
		"parent".
		
		Returns	true if any collisions occur, and returns an array (outputArray)
		of all the KrImages collided with.

		See KrImage::CheckCollision for a discussion of collision issues.
	*/
	int CheckChildCollision( KrImNode* checkThis, KrImNode* parent,
							  GlDynArray<KrImage*>* outputArray/*, int window = 0*/ ); //maks

	/**	Given a node, check if it collides with anything in the Tree.
		Returns true if any collisions occur, and returns an array
		of everything collided with.

		See KrImage::CheckCollision for a discussion of collision issues.
	*/
	bool CheckAllCollision( KrImNode* checkThis, GlDynArray<KrImage*>* outputArray/*, int window = 0*/ );
	
	enum 
	{
		/** Hit the inside of a box, even if the box is not filled. */
		ALWAYS_INSIDE_BOX			= 0x01,
		/** Get every object hit, not just the first */
		GET_ALL_HITS				= 0x04,
	};

	/// Flushes the cached state of the tree and makes it ready to draw.
	void Walk();

	void DrawWalk( const KrRect& dr, KrPaintInfo* info/*, int window*/ );

	// The tree maintains a look up table to the objects in it.
	// These methods are called by the KrImNodes.
	//void AddNodeNameHash( const gedString& name, KrImNode* node ); //maks
	//void RemoveNodeNameHash( const gedString& name ); //maks
	void RemoveNodeIdHash( int id );
	void AddNodeIdHash( int id, KrImNode* node );

	#ifdef DEBUG
	void ValidateTree( KrImNode* root );
	#endif

	void CalculateCompositBounds( KrImNode* walkNode ); //maks

	// Before a draw, walk the tree and do necessary pre-processing.
	void Walk(	KrImNode* node, 
				bool invalid,	// once something in the tree is invalid, all of its children are invalid.
				bool visible/*,	// once something is invisible, all children are invisible
				int window*/ );

#ifndef USE_WALK
	void Walk1(	KrImNode* node, 
				bool invalid,	// once something in the tree is invalid, all of its children are invalid.
				bool visible/*,	// once something is invisible, all children are invisible
				int window*/ ); //maks

	void Walk1(	KrImNode* node ); //maks
#endif

	// Walk and call the draw routines.
	void DrawWalk( const KrRect& dr, KrImNode* node, KrPaintInfo* /*, int window*/ ); //maks

  // depth used by opengl
	int TreeDepth()									{ return treeDepth; } //maks

  private:
	// Used to unroll recursion.
	struct StackContext
	{
		KrImNode*				node;
		bool					invalid;
		//GlInsideNode<KrImNode>*	childNode;
	};
	
	void CheckAllCollisionWalk( bool* hit, KrImNode* parent, KrImage* checkThis, GlDynArray<KrImage*>* outputArray/*, int window*/ );

	

	

	void Clear( KrImNode* root );	// delete the entire tree

	// Recursive hit test walk.
	bool HitTestRec( KrImNode* node, int x, int y, int flags, GlDynArray<KrImage*>* outputArray/*, int windowIndex*/ );
	bool HitTestRecIntersect( KrImNode* node, const KrRect &rect, int flags, GlDynArray<KrImage*>* output/*, int window*/ ); //maks

	KrImNode*  root;			// The root to position the window
	KrImNode*  offsetRoot;		// The root as returned to the client
	KrEngine*  engine;

	GlMap< U32, KrImNode*, GlNumberHash<U32> >		idMap;
	//GlMap< gedString, KrImNode*, GlStringHash >	nameMap; //maks

	int treeDepth;				// a z-depth used for opengl drawing
	bool bInvalidTree; //maks
};


#endif
