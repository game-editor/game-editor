#ifndef KYRA_IMAGENODE_INCLUDED
#define KYRA_IMAGENODE_INCLUDED

#include "../util/kyralist.h"
#include "SDL.h"
#include "../util/glmath.h"

/*
class KrImage;


class KrImageNode
{
	friend class KrImageNodeIterator;

  public:
	/// Constructs a sentinel node.
	KrImageNode()					{ next = this; prev = this; image = 0; }

	/// If image is null, this will be a sentinel node.
	KrImageNode( KrImage* _image )	{ next = this; prev = this; image = _image; }

	virtual ~KrImageNode()			{}

	KrImageNode*	Next() const	{ return next; }
	KrImageNode*	Prev() const	{ return prev; }
	KrImage*		Image()	const 	{ return image; }
	bool IsSentinel() const			{ return !image; }
	bool InList() const				{ return !(( next == this ) && ( prev == this )); }

	/// Insert addMe before this.
	void InsertBefore( KrImageNode* addMe )
	{
		GLASSERT( !addMe->IsSentinel() );
		addMe->prev = prev;
		prev->next = addMe;
		prev = addMe;
		addMe->next = this;
	}

	/// Insert addMe after this.
	void InsertAfter( KrImageNode* addMe )
	{
		GLASSERT( !addMe->IsSentinel() );
		addMe->prev = this;
		addMe->next = next;
		next->prev = addMe;
		next = addMe;
	}

	/// Take this node out of the list
	void Remove()
	{
		prev->next = next;
		next->prev = prev;
		prev = next = this;		// assume sentinel, again.
	}
	
  private:
	// Adding data members really stinks -- it increases the size
	// of the sentinels, and a ScanList has a bunch of sentinels.
	KrImageNode* next;
	KrImageNode* prev;
	KrImage*	 image;
};


class KrImageNodeIterator
{
  public:
	KrImageNodeIterator( KrImageNode& _sentinel )	 : sentinel( &_sentinel ), current( 0 ) { GLASSERT( sentinel->IsSentinel() ); }

	KrImageNode*	CurrentNode()	{ return current; }
	KrImage*		CurrentImage()	{ return current->image; }
	void			SetCurrent( KrImageNode* c )	{ current = c; }

	void Begin()	{ current = sentinel->next; }
	void Next()		{ current = current->next; }
	void Prev()		{ current = current->prev; }
	bool Done()		{ return current->IsSentinel(); }
		
	void InsertBefore( KrImageNode& addMe )	{ current->InsertBefore( &addMe ); }
	void InsertAfter(  KrImageNode& addMe )	{ current->InsertAfter( &addMe ); }

  private:
	KrImageNode*	sentinel;
	KrImageNode*	current;
};
*/

#endif