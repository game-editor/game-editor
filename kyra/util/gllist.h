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

#ifndef KYRA_LIST_INCLUDED
#define KYRA_LIST_INCLUDED

#include "gldebug.h"
#include "gldynarray.h" //maks

template <class T>
class GlSListIterator;

/** A node of a single linked list. Used by the GlSList template.
*/
template <class T>
struct GlSListNode
{
	GlSListNode* next;	
	T data;				///< The template data.
};


/**	A template for a singly-linked list, of the simplest sort. Uses
	GlSListNodes for members of the list, and can be walked with 
	the GlSListIterator.
*/
template <class T>
class GlSList
{
  public:
	  GlSList()		{ root = 0; last = 0; count = 0;} //maks
	virtual ~GlSList()		{ Clear(); }

	/// The number of items in the list -- O(n) performance!
	int  Size() const		{ return count; } //maks
	int  size() const		{ return count; } //maks
	/// The number of items in the list -- O(n) performance!
	int  Count() const		{	/*GlSListNode<T>* node;
								int count = 0;
								for( node = root; node; node = node->next )*/ //maks
								return count;
							}
	/// A very fast check for an empty list.
	bool Empty() const		{ return root == 0; }
	/// Returns the data at the front of the list.
	T&	 Front() const		{ return root->data; }
	T	 Back() const		{ return last?last->data:0; } //maks
	/// Get the node, not just the data, at the front of the list.
	GlSListNode<T>*	 FrontNode() const		{ return root; }

	/// Deletes all the items in the list.
	void Clear()	{	GlSListNode<T>* temp;

						while( root )
						{
							temp = root;
							root = root->next;
							delete temp;
						}

						last = 0; //maks
						count = 0; //maks
						array.Clear();
					}

	/// Adds an item (by copy) to the list. (Fast)
	void PushFront( const T& insert )
	{
		GlSListNode<T>* node = new GlSListNode<T>;

		//maks dont't clear the arrays here
		if(array.Memory())
		{
			array.SetCount(count + 2); 
			array.SetItem(count, &node->data);
			array.SetItem(count + 1, 0);
		}

		count++;
		node->data = insert;
		node->next = root;

		if(!root) last = node; //maks

		root = node;
	}

	/// Adds an item (by copy) to the list. (Slower, since it must seek to the end.)
	void PushBack( const T& insert ) //maks
	{
		/*GlSListNode<T>* end;
		for ( end=root; end && end->next; end = end->next )
		{}*/

		if ( !last )
		{
			PushFront( insert );
		}
		else
		{
			GlSListNode<T>* node = new GlSListNode<T>;

			//maks dont't clear the arrays here
			if(array.Memory())
			{
				array.SetCount(count + 2); 
				array.SetItem(count, &node->data);
				array.SetItem(count + 1, 0);
			}

			count++;
			node->data = insert;
			node->next = 0;
			last->next = node;
			last = node;
		}
	}

	/** Pull off the root entry in the list.
	*/
	void PopFront()
	{
		if ( root )
		{
			GlSListNode<T>* temp = root->next;
			delete root;
			count--; //maks
			root = temp;
			if(!root) last = 0; //maks

			array.Clear();
		}
	}

	void PopBack() //maks
	{
		//Don't use	Pop(Back()); fails if are repeated elemnts

		GlSListNode<T>* node;
		GlSListNode<T>* prev;

		for( node = root, prev = 0;
			 node;
			 prev = node, node = node->next )
		{
			if ( node == last )
			{
				if ( prev )
				{
					prev->next = node->next;
					if(last == node) last = prev; //maks
				}
				else
				{
					root = node->next;
					if(last == node) last = root; //maks
				}
								

				delete node;
				count--; //maks

				array.Clear();
				break;
			}
		}
	} 

	/**	Delete an arbitrary element. If elements are repeated,
		removes the first instance.
	*/
	void Pop( const T& thisone )
	{
		GlSListNode<T>* node;
		GlSListNode<T>* prev;

		for( node = root, prev = 0;
			 node;
			 prev = node, node = node->next )
		{
			if ( node->data == thisone )
			{
				if ( prev )
				{
					prev->next = node->next;
					if(last == node) last = prev; //maks
				}
				else
				{
					root = node->next;
					if(last == node) last = root; //maks
				}
				

				delete node;
				count--; //maks

				array.Clear();
				break;
			}
		}
	}

	/// Find the first occurance of node in the list. Linear search.
	GlSListNode<T>* Find( const T& findthis )
	{
		GlSListNode<T>* node;
		for( node=root; node; node=node->next )
		{
			if ( node->data == findthis )
				return node;
		}	
		return 0;
	}
		
	/// Find and delete instance if found. Do nothing if find fails. Return if delete.
	bool FindAndDelete( const T& findthis )
	{
		GlSListNode<T>* node;
		GlSListNode<T>* prev;

		for(	node=root, prev = 0; 
				node; 
				prev = node, node=node->next )
		{
			if ( node->data == findthis )
			{
				if ( prev )
				{
					prev->next = node->next;
					if(last == node) last = prev; //maks
				}
				else
				{
					root = node->next;
					if(last == node) last = root; //maks
				}
				delete node;
				count--;
				array.Clear();
				return true;
			}
		}	
		return false;
	}

	void operator=( const GlSList<T>& copy ) //maks	
	{ 
		Clear();
		if(copy.Empty()) return;

		GlSListIterator<T> it(copy);
		for(it.Begin(); !it.Done(); it.Next())
			PushBack(it.Current());		
	}

	GlSList( const GlSList<T>& copy ) //maks	
	{
		root = last = 0;
		count = 0;
		if(copy.Empty()) return;

		GlSListIterator<T> it(copy);
		for(it.Begin(); !it.Done(); it.Next())
			PushBack(it.Current());
	}

	T **GetArray() //maks
	{
		if(!array.Memory())
		{
			//Create array		
			int i = 0;
			array.SetCount(count + 1);
			
			if(count)
			{
				GlSListIterator<T> it(*this);
				
				for(it.Begin(); !it.Done(); it.Next())
				{
					array.SetItem(i++, &it.Current());
				}
			}
			
			//Always include the terminator
			array.SetItem(i++, 0); 
		}	
		
		return array.Memory();
	}
	
	
  private:
	GlSListNode<T>* root, *last; //maks	
	int count; //maks
	//GlSList( const GlSList& that ); 
	//GlSList operator=( const GlSList& that ); 
	
	//maks: to avoid iterators and sppedup
	GlDynArray<T *>	array; 
};


/** An iterator to walk a singly linked list.
*/
template <class T>
class GlSListIterator
{
  public:
	  GlSListIterator( const GlSList<T>& _list ) : current( 0 ), next( 0 ), list( &_list )	{} //maks
	GlSListIterator( const GlSList<T> * _list ) : current( 0 ), next( 0 ),  list( _list )	{} //maks

	void Begin()		//maks
	{ 
		if(list)
		{
			current = list->FrontNode(); 
			
			if(current)
				next = current->next;
			else
				next = 0;
		}
		else
			current = next = 0;
	} 
	void Next()			//maks
	{ 		
		current = next;
		if(current) next = current->next;		
	}
	bool Done()			{ return ( current == 0 ); }

	/// Return a reference to the data - will crash horribly if one does not exist.
	T& Current()					{ return (current->data); }

	GlSListNode<T>* CurrentNode()	{ return current; }

  private:
	GlSListNode<T>* current, *next;	//Prevent current deletion //maks
	const GlSList<T>* list;
};


#endif
