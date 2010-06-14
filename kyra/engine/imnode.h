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

#ifndef KYRA_IMAGE_NODE_INCLUDED
#define KYRA_IMAGE_NODE_INCLUDED

#include "../util/glinsidelist.h"
#include "../util/gldynarray.h"
#include "SDL.h"
#include "krmath.h"
#include "color.h"
#include "kyrabuild.h"

// Disable the no-exception handling warning.
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"

//#define USE_RTREE
//#define USE_COLLISION_SECTOR 

#ifdef USE_COLLISION_SECTOR //maks

extern int sectorSizeW, sectorSizeH, log2SectorSizeW, log2SectorSizeH;

#endif

#ifdef USE_RTREE //maks
#include "RTree.h" 
extern RTree<int, int, 2, float> rtree;


/*#include "C:/Downloads/SpatialIndex/spatialindex 0.60b/lib/SpatialIndex.h"

using namespace SpatialIndex;


typedef bool (*FUNC_RTREE_CALLBACK)(int a_data, void* a_context);

class MyVisitor : public IVisitor
{
public:
	void visitNode(const INode& n) 
	{
		//Region *r;
		//n.getShape((IShape **)&r);
	}

	void visitData(const IData& d)
	{
		//Region *r;
		//d.getShape((IShape **)&r);

		a_resultCallback(d.getIdentifier(), NULL);
	}

	FUNC_RTREE_CALLBACK a_resultCallback;
};

class RStarTree
{
public:
	RStarTree()
	{
		mem = StorageManager::createNewMemoryStorageManager();
		tree = RTree::createNewRTree(*mem, 0.7, 10, 10, 2, SpatialIndex::RTree::RV_RSTAR, indexIdentifier);
	}

	void Insert(const int a_min[2], const int a_max[2], const int& a_dataId)
	{
		double plow[2], phigh[2];
		plow[0] = a_min[0];
		plow[1] = a_min[1];

		phigh[0] = a_max[0];
		phigh[1] = a_max[1];

		Region r = Region(plow, phigh, 2);
		tree->insertData(0, 0, r, a_dataId);
	}

	void Remove(const int a_min[2], const int a_max[2], const int& a_dataId)
	{
		double plow[2], phigh[2];
		plow[0] = a_min[0];
		plow[1] = a_min[1];

		phigh[0] = a_max[0];
		phigh[1] = a_max[1];

		Region r = Region(plow, phigh, 2);
		tree->deleteData(r, a_dataId);
	}

	void Search(const int a_min[2], const int a_max[2], bool __cdecl a_resultCallback(int a_data, void* a_context), void* a_context)
	{
		vis.a_resultCallback = a_resultCallback;

		double plow[2], phigh[2];
		plow[0] = a_min[0];
		plow[1] = a_min[1];

		phigh[0] = a_max[0];
		phigh[1] = a_max[1];

		Region r = Region(plow, phigh, 2);
		tree->containsWhatQuery(r, vis);
	}

	long indexIdentifier;

	IStorageManager *mem;
	ISpatialIndex* tree;

	MyVisitor vis;
};

extern RStarTree rtree;*/

#endif



enum //maks
{
	DISABLE = 0,
	ENABLE,
	DONT_DRAW_ONLY
};

class KrEngine;
struct KrRect;
class KrImage;
class KrSprite;
class KrTile;
class KrCanvas;
class KrTextBox;
class KrBox;
class KrWidget;

extern KrEngine *engine; //maks

//const int KR_ALL_WINDOWS = -1;

/**	The base class of everything that can be in an ImageTree.
	The class in not abstract -- it can be very useful to
	have a KrImNode that is used as a container for other 
	objects.

	Children always draw on top of their parents.
*/
class KrImNode
{
	friend class KrImageTree;

  public:
	  //void CheckInvalidPos(); //maks //Invalidate by position: problems with CollisionFree (calls Walk())
	  bool getIsText() {return isText;} //maks
	  void SetHitTest(bool bHitTest) {bEnableHit = bHitTest;}; //maks
	  void *getParentActor() {return parentActor;}; //maks
	  void setParentActor(void *parentActor) {this->parentActor = parentActor;}; //maks
	  void ObjectToScreen( int x, int y, KrVector2T< GlFixed >* screen/*, int window=0*/ ); //maks

	// The nodes:
	// Not really methods, not really data members. Here
	// but not to be used.
	GlInsideNode<KrImNode*>	treeNode;	// We are in someone else's child list.

  public:
	KrImNode();
	virtual ~KrImNode();

	/**	Every node has a Z-Depth within the node that contains it.
		The lowest z-depth is the farthest from the viewer. Any 
		integer value is acceptable.

		Nodes at the same z-depth are sorted by the order they were
		added to the tree. The last one added is the closest to the
		viewer. (It is not a good idea to mix z-depth changing
		and "order sorting." Go with one or the other.

		Changing the z-depth of the root is meaningless. (It has
		no siblings.)
	*/
	int  ZDepth()					{ return depth; }

	/// Set the z-depth of this node relative to its siblings.
	void SetZDepth( int z );

	/// Return the x postion of this node, relative to its parent.
	int X( /*int window=0*/ )			{	//maks GLASSERT( ( xTransform[window].x.v & ((1 << DECIMAL_BITS) - 1) ) == 0 );
										return xTransform/*[window]*/.x.ToInt(); }
	/// Return the y position of this node, relative to its parent.
	int Y( /*int window=0*/ )			{	//maks GLASSERT( ( xTransform[window].y.v & ((1 << DECIMAL_BITS) - 1) ) == 0 );
										return xTransform/*[window]*/.y.ToInt(); }

	/// Return the x scale of this node, untransformed by parent.
	GlFixed XScale( /*int window=0*/ )				{ return xTransform/*[window]*/.xScale; }
	/// Return the x scale of this node, untransformed by parent.
	GlFixed YScale( /*int window=0*/ )				{ return xTransform/*[window]*/.yScale; }

	/// Query the entire transformation matrix.
	const KrMatrix2& Transform( /*int window=0*/ )	{ return xTransform/*[window]*/; }

	/**	Set the x and y positon of the ImNode, relative to 
		the node's parent.

		Even non-drawing nodes have position since changing the
		position moves the node's children.

		Setting the positon of the root node scrolls the screen.
	*/
	
	void SetPos( GlFixed x, GlFixed y/*, int win=KR_ALL_WINDOWS*/ );
	void SetRotation( double rotation );

	/// Set the scale of this ImNode, which changes all the children as well.
	void SetScale( GlFixed xScale, GlFixed yScale/*, int window=KR_ALL_WINDOWS*/ );

	/// Set the transformation matrix of this ImNode: combines the SetPos and SetScale functionality.
	void SetTransform( KrMatrix2& xForm/*, int window=KR_ALL_WINDOWS*/ );

	/** Set the drawing quality for scaling. Quality is inherited
		from the parent the same way other properties are, if the
		quality is set to KrQualityNone, else the set quality is
		used.
		Cached scaling is always at maximum quality. 
	*/

	void SetQuality( int quality/*, int window=KR_ALL_WINDOWS*/ );

	/// Offsets the node position.
	void DeltaPos( int deltaX, int deltaY/*, int window=KR_ALL_WINDOWS*/ )		{ SetPos( X() + deltaX, Y() + deltaY/*, window*/ ); }

	/** Get the color transformation of the node.
	*/
	inline const KrColorTransform& CTransform( /*int window=0*/ )			{ return cTransform/*[window]*/; }

	/** Set the color of a node. This color transforms all
		the node's children as well.
	*/
	virtual void SetColor( const KrColorTransform& color/*, int window=KR_ALL_WINDOWS*/ );

	virtual KrImage*  ToImage()		{ return 0; }	///< Return a pointer if this is an image.
	virtual KrSprite* ToSprite()	{ return 0; }	///< Return a pointer if this is a sprite.
	virtual KrTile*   ToTile()		{ return 0; }	///< Return a pointer if this is a tile.
	virtual KrCanvas* ToCanvas()	{ return 0; }	///< Return a pointer if this is a canvas.
	virtual KrTextBox*ToTextBox()	{ return 0; }	///< Return a pointer if this is a text box.
	virtual KrBox*	  ToBox()		{ return 0; }	///< Return a pointer if this is a box.
	virtual KrWidget* ToWidget()	{ return 0; }	///< Return a pointer if this is a widget.

	/**	A generic cast. The Kyra library is not as extensible as it could be;
		this implements a "cheap RTTI" if you add your own sub-classes. Arbitrary
		casting to any type in the string. For example,
		@verbatim
			MyClassType* myType = (MyClassType*) node->ToExtended( "MyClassType" );
		@endverbatim
		Somewhat inelegant, but a very useful hook. The build in classes will not 
		respond to this. (That is, ToExtended( "Sprite" ) does not work.)
	*/
	virtual KrImNode* ToExtended( const gedString& type )	{ return 0; }

//	/** Support for GUI Widgets.
//		A widget can interact with its immediate children on the tree.
//		So if a node's parent is a widget, then that widget is related, but
//		any non-widget parent will break the relation. This function returns
//		the highest related parent widget. It will never return 'this'.
//	*/
//	KrWidget* TopWidget();
//
//	/** Support for GUI Widgets.
//		Return this if it is a widget, or its parent if that is a widget.
//		Otherwise returns null. Effectively returns the lowest level widget
//		this node is associated with. (The opposite behavior of TopWidget() ).
//	*/
//	KrWidget* GetWidget();

	/** Any ImNode can have a name or id. They are completely 
		managed by the user -- they are not used by the engine
		in any way. They should be unique to the tree, and can
		be used as a way to keep track of nodes.

		An empty name or an id<0 indicate no name and no id,
		respectively.
	*/
	//const gedString& NodeName()		{ return nodeName; } //maks

	/** Any ImNode can have a name or id. They are completely 
		managed by the user -- they are not used by the engine
		in any way. They should be unique to the tree, and can
		be used as a way to keep track of nodes.

		An empty name or an id<0 indicate no name and no id,
		respectively.
	*/
	int                NodeId()			{ return nodeId; }

	/** Set the name of this node. This is completely user defined.
		The name of a node should be unique. In DEBUG mode, an assertion
		will fire if the same name is used twice.
	*/
	//void SetNodeName( const gedString& name ); //maks

	/** Set the id of this node. It must be > 0. This is completely user defined.
		Like the name, this should be unique. In DEBUG mode, an assertion
		will fire if an id is re-used.
	*/
	void SetNodeId( int id );	
	
	/// Fetch an iterator to walk the children with.
	GlInsideNodeIt<KrImNode*> ChildTreeIterator()	{ return GlInsideNodeIt<KrImNode*>( child ); }

	/// Hit test this object. Returns true if there was a hit, and 'this' will be added to results.
	virtual bool HitTest( int x, int y, int flags, GlDynArray<KrImage*>* results/*, int window*/ )  { return false; }
	
	/** Return the visibility of this node, taking into account	
		the state of all its parents.
	*/
	bool IsVisible( /*int window=0,*/ bool bCheckDontDrawOnly = false ) //maks
	{
		KrImNode* node;
		
		if(bCheckDontDrawOnly)
		{
			for( node = this; node; node = node->parent )
			{
				if ( node->visible/*[window]*/ != ENABLE )
					return false;
			}
		}
		else
		{
			for( node = this; node; node = node->parent )
			{
				if ( node->visible/*[window]*/ == DISABLE )
					return false;
			}
		}
		return true;
	}

	/** Return the visibility of this node, NOT taking into account	
		the state of all its parents.
	*/
	bool IsThisVisible( /*int window=0*/ )		{ return visible/*[window]*/ != DISABLE; } //maks
	int getVisible( /*int window=0*/ )		{ return visible; } //maks

	/** If visible is set to false, this node and all its children
		will not show up on the stage.

		Returns true if the visiblity was changed by this call.
	*/
	virtual void SetVisible( int visible/*, int window=KR_ALL_WINDOWS*/ ); //maks

	/** Transform screen (pixel) coordinates to local object
		coordinates. The composite Xform is used for this
		and must be initialized -- practically, the object
		must be in a Tree to call this method.
	*/
	void ScreenToObject( int x, int y, KrVector2T< GlFixed >* object/*, int window=0*/ );
	void ScreenToObject( const GlFixed& x, const GlFixed& y, KrVector2T< GlFixed >* object/*, int window=0*/ ); //maks

	/// A hook to set user defined data.
	//void SetUserData( void* data )				{ userData = data; } //maks
	///  A hook to set user defined data. Sets the *same* user data as SetUserData, just uses a convenience cast.
	//void SetUserDataU32( U32 data )				{ userData = (void*) data; } //maks

	/// Fetches the user defined data.
	//void* GetUserData()							{ return userData; } //maks
	/// Fetches the user defined data. Fetches the *same* user data as GetUserData, just uses a convenience cast.
	//U32   GetUserDataU32()						{ return (U32) userData; } //maks

	/** Return a copy if this object, not in a Tree(). It is
		return untransformed. In the case of a sprite, the action
		and frame will be set to the current value.	
	*/
	virtual KrImNode* Clone();

	// -- internal -- //
	// Will be true if this needs to be repainted.
	bool IsInvalid( /*int window,*/ bool bReturnInvalidIfParentIsInvalid = false );

	// Adds the invalid rectangles of this node the the 
	// engine's DR list and removes the node from the
	// start list. Called, with CalcTransform and
	// AddToDrawLists, by KrImageTree::Walk.
	// This relies on the current "bounds" for the invalid
	// rect, so it must be called before CalcTransform.
	//
	// If "cache" is true, no rectangle is sent, but it is stored.
	// Used to pair up overlapping rectangles.
	virtual void FlushInvalid( /*int window,*/ bool cache )	{}

	// Calculate the:
	//	- Transformation, both color and spacial.
	//	- Bounds.
	// Note that the transformation will depend on the parent
	// already having been transformed, unless transformFromRoot
	// is true.
	virtual void CalcTransform( /*int win*/ );

	void CalcCompositeTransform( /*int i*/ ); //maks

	// The node has been added to the tree: its links are valid.
	virtual void AddedtoTree();
	// The node is being removed from the tree.
	virtual void LeavingTree();

	void Invalidate( /*int window*/ );

	// Properties of the node.
	// Were protected...but are sometimes useful to get to.
	// The transformation matrix, expressed as such.
	inline const KrMatrix2&		XTransform( /*int window*/ )			{ return xTransform/*[window]*/; }
	// The composite (calculated absolute) transformation matrix.
	inline const KrMatrix2&		CompositeXForm( /*int window*/ )		{ return compositeXForm/*[window]*/; }
	inline KrMatrix2*		getCompositeXForm( /*int window*/ )		{ return &compositeXForm/*[window]*/; } //maks

	// The composite (calculated absolute) color transformation.
	inline const KrColorTransform& CompositeCForm( /*int window*/ )		{ return compositeCForm/*[window]*/; }

	// The composite (calculated absolute) quality for scaling setting.
	inline int						CompositeQuality( /*int window*/ )		{ return compositeQuality/*[window]*/; } 

	// The composite (calculated absolute) bounds. May be invalid.
	inline const KrRect&			CompositeBounds( /*int window*/ )		{ return compositeBounds/*[window]*/; }
	inline KrRect&			GetCompositeBounds( /*int window*/ )		{ return compositeBounds/*[window]*/; } //maks

	// The bounds for this object; may be invalid.
	inline const KrRect&	Bounds( /*int window*/ )						{ return bounds/*[window]*/; } //maks
	inline KrRect*	GetBounds( /*int window*/ )						{ return &bounds/*[window]*/; } //maks

	// Get the child sentinel.
	GlInsideNode<KrImNode*>* Child() { return &child; }

	/// The parent of this KrImNode
 	KrImNode* Parent()								{ return parent; }
	/// The engine this KrImNode is in.
	KrEngine* Engine()								{ return ::engine; }

	// depth used by opengl
	int TreeDepth()									{ return treeDepth; } //maks

#ifdef USE_COLLISION_SECTOR //maks
	void InvalidateSectorPos();
	static void InitSectors(int screenW, int screenH);
#endif

  protected:
	

	inline void ClearInvalid( /*int window*/ )					{ invalid/*[window]*/ = false; }

	/** Resort can be called if a node's depth has changed
		and it needs to move elsewhere in the list.
	*/
	void Resort( KrImNode* resortMe );

	// Sentinel for our child nodes
	GlInsideNode<KrImNode*> child;

	
	

	void *parentActor; //maks
	bool bEnableHit;	//maks
	bool isText;	//maks

	int		visible/*[KR_MAX_WINDOWS]*/; //maks

#ifdef USE_RTREE //maks
	void UpdateRTree(int i);
	KrRect boundsAnt/*[KR_MAX_WINDOWS]*/;
#endif

  //private:
	  //Maks data reorganized to take account the memory access

	int			depth;		// user-specified depth
	

	KrMatrix2			xTransform/*[KR_MAX_WINDOWS]*/;		// (x,y) relative to parent (set)
	KrMatrix2			compositeXForm/*[KR_MAX_WINDOWS]*/;	// (x,y) world (calculated)

	//maks //Invalidate by position: problems with CollisionFree (calls Walk())
	//KrMatrix2			xTransformAnt/*[KR_MAX_WINDOWS]*/;		// (x,y) relative to parent (set) in anterior frame //maks
	//KrMatrix2			compositeXFormAnt/*[KR_MAX_WINDOWS]*/;		// (x,y) relative to parent (set) in anterior frame //maks

	KrColorTransform	cTransform/*[KR_MAX_WINDOWS]*/;		// color relative to parent
	KrColorTransform	compositeCForm/*[KR_MAX_WINDOWS]*/;	// color world (calculated)

	KrImNode*	parent;			// The parent in the ImageTree, only null for the root.
	int			treeDepth;	// used by opengl drawing
	
	
	// Anything that can be drawn has bounds. Bounds are
	// calulated in the CalcTransform call.
	KrRect				bounds/*[KR_MAX_WINDOWS]*/;

	KrRect				compositeBounds/*[KR_MAX_WINDOWS]*/;

	
	//KrEngine*	engine;			// The engine this Image is attached to.
	bool		invalid/*[KR_MAX_WINDOWS]*/;		// Does this need be drawn?

	//gedString nodeName; //maks
	int         nodeId;
	
	//void*		userData; //maks

	int					quality/*[KR_MAX_WINDOWS]*/;
	int					compositeQuality/*[KR_MAX_WINDOWS]*/;

#ifdef USE_COLLISION_SECTOR //maks
	void UpdateSectors();
	KrVector2 posAnt, sectorCoord;
#endif
};


#endif

