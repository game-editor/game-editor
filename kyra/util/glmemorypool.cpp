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


#include "glmemorypool.h"
#include "gldebug.h"
#include <stdlib.h>
#include <string.h>

#include "../../gameEngine/dlmalloc.h" //maks


GlMemoryPool::GlMemoryPool( unsigned _objectSize, unsigned _blockSize )
{
	chunkSize = _objectSize;
	numBlocks = 0;

	if ( chunkSize < sizeof( Chunk ) )
		chunkSize = sizeof( Chunk );
	chunkSize = ( chunkSize + 3 ) & (~0x03 );

	blockSize = _blockSize;

	chunksPerBlock = ( blockSize - sizeof( BlockHeader ) ) / chunkSize;
	GLASSERT( chunksPerBlock > 0 );

	#ifdef DEBUG
		numBlocks = 0;
		numChunks = 0;
	#endif

	rootBlock = 0;
	head = 0;
}


GlMemoryPool::~GlMemoryPool()
{
	GLASSERT( numChunks == 0 );

	Block* node = 0;

	while( rootBlock )
	{
		node = rootBlock->header.nextBlock;
		free( rootBlock );
		rootBlock = node;
	}
}


void* GlMemoryPool::Alloc()
{
	void* ret = 0;

	if ( !head )
	{
		NewBlock();
		GLASSERT( head );
	}

	ret = head;
	head = head->next;

	#ifdef DEBUG
		++numChunks;
		memset( ret, 0xaa, chunkSize );
	#endif

	return ret;
}


void GlMemoryPool::Free( void* mem )
{
	Chunk* chunk = (Chunk*) mem;

	#ifdef DEBUG
		--numChunks;
		memset( mem, 0xbb, chunkSize );
	#endif

	chunk->next = 0;
	head = chunk;
}


void GlMemoryPool::NewBlock()
{
	GLASSERT( head == 0 );

	Block* block = (Block*) malloc( blockSize );
	GLASSERT( block );

	block->header.nextBlock = rootBlock;
	rootBlock = block;

	#ifdef DEBUG
		memset( block, 0xcc, blockSize );
		++numChunks;
	#endif

	unsigned i;
	for( i=0; i<chunksPerBlock-1; i++ )
	{
		block->chunk[i].next = &block->chunk[i+1];
	}
	block->chunk[ chunksPerBlock-1 ].next = 0;

	head = &block->chunk[0];
}


GlLinearMemoryPool::GlLinearMemoryPool( unsigned totalMemory )
{
	base = (char* ) malloc( totalMemory );
	GLASSERT( base );

	current = base;
	end = base + totalMemory;

	#ifdef DEBUG
		memset( base, 0xaa, totalMemory );
	#endif
}


GlLinearMemoryPool::~GlLinearMemoryPool()
{
	free( base );
}


void* GlLinearMemoryPool::Alloc( unsigned allocate )
{
	if ( current < end )
	{
		char* ret = current;
		current += allocate;

		// Out of memory check.
		GLASSERT( current <= end );
		if ( current <= end )
			return ret;
		else
			return 0;
	}
	else
	{
		// Out of memory!
		GLASSERT( 0 );
		return 0;
	}
}
