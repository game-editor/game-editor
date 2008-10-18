/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

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
*/

#ifndef GLFIXED_SET_INCLUDED
#define GLFIXED_SET_INCLUDED

#include "gllist.h"

// An odd class. A set of sequential integers starting with 0.
class GlIntArraySet
{
  public:
	GlIntArraySet( int maxSize, bool startOn );
	~GlIntArraySet();

	bool Empty()			{ return root == 0; }

	void Push( int data );
	void Pop(  int data );
	bool Find( int data )	{ return array[data].data; }

  private:
	int maxSize;
	GlSListNode<bool>* array;
	GlSListNode<bool>* root;
};

#endif

