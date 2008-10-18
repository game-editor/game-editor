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



#ifndef GL_MEMORY_POOL
#define GL_MEMORY_POOL

#include "gldebug.h"

/*	A memory pool that will dynamically grow as memory is needed.
*/
class GlMemoryPool
{
  public:
	GlMemoryPool( unsigned objectSize, unsigned blockSize = 4096 );
	~GlMemoryPool();

	void* Alloc();
	void Free( void* mem );

  private:
	struct Block;

	struct Chunk
	{
		Chunk* next;
	};

	struct BlockHeader
	{
		Block* nextBlock;	// pointer to the next block
	};

	struct Block
	{
		BlockHeader header;
		Chunk* chunk;		// treated as an array of chunks.
	};

	void NewBlock();

	unsigned chunkSize;
	unsigned blockSize;
	unsigned chunksPerBlock;
	
	unsigned numBlocks;
	unsigned numChunks;
	
	Block* rootBlock;
	Chunk* head;
};

/*	A memory pool that has a fixed allocation.
	Essentially a cross between a linked list and an array.
*/
template < class T, int COUNT >
class GlFixedMemoryPool
{
  private:
	struct Chunk
	{
		Chunk* next;
	};

  public:
	GlFixedMemoryPool()
	{
		GLASSERT( sizeof( T ) >= sizeof( Chunk* ) );
		for( int i=0; i<COUNT-1; ++i )
		{
			( (Chunk*)(&memory[i]) )->next = (Chunk*)(&memory[i+1]);
		}
		( (Chunk*)(&memory[COUNT-1]) )->next = 0;
		root = ( (Chunk*)(&memory[0]) );
		inUse = 0;
	}

	~GlFixedMemoryPool()	{}

	T* Alloc()
	{
		T* ret = 0;
		if ( root )
		{
			ret = (T*) root;
			root = root->next;
			++inUse;
		}
		return ret;
	}

	void Free( T* _mem )
	{
		if ( _mem )
		{
			Chunk* mem = (Chunk*) _mem;
			mem->next = root;
			root = mem;
			--inUse;
		}
	}

	unsigned InUse()	{	return inUse; }
	unsigned Remain()	{	return COUNT - inUse; }

  private:
	T memory[ COUNT ];
	unsigned inUse;
	Chunk* root;
};


/* 	This is memory allocation for when you know exactly how much 
	memory is going to be needed. So it's a way to pre-allocate 
	while still using the new and delete operators.
*/

class GlLinearMemoryPool
{
  public:
	/// Construct, and pass in the amount of memory needed, in bytes.
	GlLinearMemoryPool( unsigned totalMemory );

	~GlLinearMemoryPool();

	/// Allocate a chunk of memory.
	void* Alloc( unsigned allocate );

	/** Note this does nothing.
	*/
	void Free( void* mem )		{}

	/// Return true of this is out of memory.
	bool OutOfMemory()			{ return current == end; }

  public:
	char*	base;
	char*	current;
	char*	end;
};

#endif
