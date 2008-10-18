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


#ifndef KYRA_CIRCLELIST_INCLUDED
#define KYRA_CIRCLELIST_INCLUDED

#include "../util/gldebug.h"

// OPT improvements:
// - add memory allocator
// - remove the 'data' from circle node, so the overhead isn't in
//	 the sentinels.

template <class T>
struct GlCircleNode
{
	T data;

	GlCircleNode<T>* next;
	GlCircleNode<T>* prev;
};

/*
	A circular, double linked list.
*/
template <class T>
class GlCircleList
{
  public:
	GlCircleList()		{ sentinel.next = &sentinel; sentinel.prev = &sentinel; }
	~GlCircleList()		{ Clear(); }

	bool	Empty() const	{ return sentinel.next == &sentinel; }
	T&		Front() const	{ return sentinel.next->data; }
	T&		Back()  const	{ return sentinel.prev->data; }
	GlCircleNode<T>* FrontNode() const	{ return sentinel.next; }
	GlCircleNode<T>* BackNode()  const	{ return sentinel.prev; }

	void Clear()			{	GlCircleNode<T>* temp;
								while( sentinel.next != &sentinel )
								{
									temp = sentinel.next;
									sentinel.next = sentinel.next->next;
									delete temp;
								}
								sentinel.prev = &sentinel;
							}
	void PushFront( const T& insert ) {
								GlCircleNode<T>* node = new GlCircleNode<T>;
								node->data = insert;

								node->prev = &sentinel;
								node->next = sentinel.next;
								sentinel.next->prev = node;
								sentinel.next = node;
							}
	void PushBack( const T& insert ) {
								GlCircleNode<T>* node = new GlCircleNode<T>;
								node->data = insert;

								node->prev = sentinel.prev;
								node->next = &sentinel;
								sentinel.prev->next = node;
								sentinel.prev = node;
							}
	void PopFront()			{
								GLASSERT( sentinel.next != &sentinel );
								GlCircleNode<T>* node = sentinel.next;
// 								node->prev->next = node->next;
// 								node->next->prev = node->prev;
// 								delete node;
								Delete( node );
							}
	void PopBack()			{
								GLASSERT( sentinel.prev != &sentinel );
								GlCircleNode<T>* node = sentinel.prev;
// 								node->prev->next = node->next;
// 								node->next->prev = node->prev;
// 								delete node;
								Delete( node );
							}

	void Delete( GlCircleNode<T>* node )	{
												GLASSERT( node != &sentinel );
												node->prev->next = node->next;
												node->next->prev = node->prev;
												delete node;
											}
	GlCircleNode<T>* Find( T value )		{
												GlCircleNode<T>* node = sentinel.next;
												while ( node != &sentinel )
												{
													if ( node->data == value )
														return node;
													node = node->next;
												}
												return 0;
											}

	// Scoping problems. Pretend this is private.
	GlCircleNode<T> sentinel;
};


template <class T>
class GlCircleListIterator
{
  public:
	GlCircleListIterator( GlCircleList<T>& _list ) : current( 0 ), list( &_list )	{}

	void Begin()		{ current = list->sentinel.next; }
	void Next()			{ current = current->next; }
	void Prev()			{ current = current->prev; }
	bool Done()			{ return ( current == &(list->sentinel) ); }

	/// Return a reference to the data - will crash horribly if one does not exist.
	T& Current()					{ return (current->data); }

	void InsertBefore( const T& addMe )
	{
		GlCircleNode<T>* node = new GlCircleNode<T>;
		node->data = addMe;

		node->prev = current->prev;
		node->next = current;
		current->prev->next = node;
		current->prev = node;
	}

	void InsertAfter( const T& addMe )
	{
		GlCircleNode<T>* node = new GlCircleNode<T>;
		node->data = addMe;

		node->prev = current;
		node->next = current->next;
		current->next->prev = node;
		current->next = node;
	}

	void Remove()
	{
		GLASSERT( current != &(list->sentinel) );

		GlCircleNode<T>* temp = current;
		current = current->next;

		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
	}

  private:
	GlCircleNode<T>* current;	
	GlCircleList<T>* list;
};


#endif
