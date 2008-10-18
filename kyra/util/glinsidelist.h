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

#ifndef INPLACE_IMAGENODE_INCLUDED
#define INPLACE_IMAGENODE_INCLUDED

#include "../util/gldebug.h"

// The type of this must be a pointer.
template <class T>
class GlInsideNode
{
  public:
	/// Constructs a sentinel node.
	GlInsideNode()					{ next = this; prev = this; data = 0; }

	/// If image is null, this will be a sentinel node.
	GlInsideNode( T _data )			{ next = this; prev = this; data = _data; }

	virtual ~GlInsideNode()			{}

	bool IsSentinel() const			{ return !data; }
	bool InList() const				{ return !(( next == this ) && ( prev == this )); }

	/// Insert addMe before this.
	void InsertBefore( GlInsideNode<T>* addMe )
	{
		GLASSERT( !addMe->IsSentinel() );
		addMe->prev = prev;
		prev->next = addMe;
		prev = addMe;
		addMe->next = this;
	}

	/// Insert addMe after this.
	void InsertAfter( GlInsideNode<T>* addMe )
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
	
	// Should be private, but I don't feel like fighting with
	// making templates friends.

	GlInsideNode<T>*	next;
	GlInsideNode<T>*	prev;
	T					data;
};


template <class T>
class GlInsideNodeIt
{
  public:
	GlInsideNodeIt( GlInsideNode<T>& _sentinel )	 
		: sentinel( &_sentinel ), current( 0 ) 
	{ 
		GLASSERT( sentinel->IsSentinel() ); 
	}

	GlInsideNode<T>*	CurrentNode()						{ return current; }
	T					CurrentData()						{ return current->data; }
	void				SetCurrent( GlInsideNode<T>* c )	{ current = c; }

	void Begin()	{ current = sentinel->next; }
	void Last()		{ current = sentinel->prev; }
	void Next()		{ current = current->next; }
	void Prev()		{ current = current->prev; }
	bool Done()		{ return current?current->IsSentinel():true; }
		
	void InsertBefore( GlInsideNode<T>& addMe )	{ current->InsertBefore( &addMe ); }
	void InsertAfter(  GlInsideNode<T>& addMe )	{ current->InsertAfter( &addMe ); }

  private:
	GlInsideNode<T>*	sentinel;
	GlInsideNode<T>*	current;
};


#endif
