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

#include "glintarrayset.h"

GlIntArraySet::GlIntArraySet( int _maxSize, bool startOn )
{
	maxSize = _maxSize;
	array = new GlSListNode<bool>[ maxSize ];
	
	if ( startOn )
	{
		for( int i=0; i<maxSize; ++i )
		{
			array[i].data = true;
			if ( i < maxSize-1 )
				array[i].next = &array[i+1];
			else
				array[i].next = 0;
		}
		root = &array[0];
	}
	else
	{
		for( int i=0; i<maxSize; ++i )
		{
			array[i].data = false;
			array[i].next = 0;
		}
		root = 0;
	}		
}


GlIntArraySet::~GlIntArraySet()
{
	root = 0;
	delete [] array;
}


void GlIntArraySet::Push( int data )
{
	if ( Find( data ) )	return;		// already on

	array[data].data = true;

	// Now, we find the previous and point it to the one just added
	int i = data-1;
	while ( i >= 0 )
	{
		if ( array[i].data )
		{
			// The next pointer of the i element points past the 'data' element.
			// Connect 'data' to the i elements pointer.
			array[data].next = array[i].next;
			array[i].next = &array[data];
			break;
		}
		--i;
	}
	if ( i < 0 )
	{
		// nothing was found!
		if ( root )
		{
			array[data].next = root;
			root = &array[data];
		}
		else
		{
			array[data].next = 0;
			root = &array[data];
		}
	}
}


	bool Pop(  int data );
