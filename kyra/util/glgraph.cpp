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

#include <stdio.h>
#include "glgraph.h"

GlGraph::GlGraph( int _numVertex )
{
	numVertex = _numVertex;
	dataPoint = 0;
	vertex = new Vertex[ numVertex ];
}


GlGraph::~GlGraph()
{
	delete [] dataPoint;
	delete [] vertex;
}


void GlGraph::LowerAddEdge( int v0, int v1, int d01, int d10 )
{
	//int v[2] = { v0, v1 };

	GLASSERT( v0 < numVertex );
	GLASSERT( v1 < numVertex );
	GLASSERT( !dataPoint );

	Edge edge;
	edge.lengthTo = d01;
	edge.lengthFrom = d10;
	edge.toVertex = v1;
	vertex[v0].edgeList.PushFront( edge );
}


void GlGraph::AddBiEdge( int v0, int v1, int distance )
{
	LowerAddEdge( v0, v1, distance, distance );	
	LowerAddEdge( v1, v0, distance, distance );	
}


void GlGraph::AddBiEdge( int v0, int v1, int d01, int d10 )
{
	LowerAddEdge( v0, v1, d01, d10 );	
	LowerAddEdge( v1, v0, d10, d01 );	
}


void GlGraph::AddEdge( int v0, int v1, int distance )
{
	LowerAddEdge( v0, v1, distance, GL_INFINITE );
	LowerAddEdge( v1, v0, GL_INFINITE, distance );
}


void GlGraph::GetPath(	int source, int dest, 
						int *nextVertex, 
 						int *length,
						int *distance )
{
	// Check if we need to calc:
	if ( !vertex[dest].destinationCalculated )
	{
		ShortestPathCalc( dest );
	}
	GLASSERT( dataPoint );
	DataPoint* dp = GetDataPoint( source, dest );

	if ( dp->distance >= GL_INFINITE )
	{
		// There is no connection.
		*nextVertex = -1;
		if ( length )	*length = GL_INFINITE;
		if ( distance )	*distance = GL_INFINITE;
	}
	else
	{
		*nextVertex = dp->vertex;
		if ( distance )
		{
			*distance   = dp->distance;
		}
		if ( length )
		{
			*length = GL_INFINITE;
			GlSListIterator< Edge > it( vertex[source].edgeList );
			for( it.Begin(); !it.Done(); it.Next() )
			{
				if (	it.Current().toVertex == dp->vertex 
					 && it.Current().lengthTo < GL_INFINITE )
				{
					*length = it.Current().lengthTo;
					break;
				}
			}
			GLASSERT( *length != GL_INFINITE );
		}
	}
}


int GlGraph::FindCheapest( GlCircleList<int>* set, int dest )
{
	int ret = -1;
	int distance = GL_INFINITE;

	GlCircleListIterator< int > it( *set );
	for( it.Begin(); !it.Done(); it.Next() )
	{
		DataPoint *dp = GetDataPoint( it.Current(), dest );
		if ( dp->distance < distance )
		{
			distance = dp->distance;
			ret = it.Current();
		}
	}
	return ret;
}


void GlGraph::ShortestPathCalc( int dest )
{
	int i;
	#ifdef DEBUG
		GLOUTPUT( "Shortest path to %d\n", dest );
	#endif

	// The 'calculated' flags and the setQ are redundant to each other. But it is slow to
	// iterate through 'calculated' and slow to fnd in 'setQ' so both are here.
	bool *calculated = new bool[ numVertex ];	// Marks whether the source vertex has been calcalated
	GlCircleList<int>	setQ;					// The set of all the vertices not yet processed

	for( i=0; i<numVertex; ++i )
	{
		calculated[i] = false;
		setQ.PushFront( i );
	}	

	if ( !dataPoint )
	{
		int count = numVertex * numVertex;
		dataPoint = new DataPoint[ count ];
		for ( int source=0; source<numVertex; ++source )
		{
			for( int dest=0; dest<numVertex; ++dest )
			{
				DataPoint* dp = GetDataPoint( source, dest );
				dp->vertex = dest;

				if ( source == dest )
					dp->distance = 0;
				else
					dp->distance = GL_INFINITE;
			}
		}
	}
	vertex[dest].destinationCalculated = true;

	
	// Now, move from set Q to S...processing as we go.
	while ( !setQ.Empty() )
	{
		int cheapest = FindCheapest( &setQ, dest );
		if ( cheapest < 0 )
		{
			// disconnected set.
			break;
		}

		// Get the closest node in Q
		GlCircleNode<int>* node = setQ.Find( cheapest );
		GLASSERT( node );
		setQ.Delete( node );

		DataPoint* cheapDp = GetDataPoint( cheapest, dest );
		calculated[cheapest] = true;

		// For each vertex adjacent to the cheapest, that is still in Q,
		// relax it. Note that edges can have an GL_INFINITE distance, if
		// they have a real distance in one direction, it can be GL_INFINITE 
		// in the other.
		// Also, we are interested in the distance from the 'nextDp' back
		// to the 'cheapest'. It's easy to get confused.

// 		GLOUTPUT( "Relaxing %d\n", cheapest );
		GlSListIterator< Edge > it( vertex[cheapest].edgeList );
		for( it.Begin(); !it.Done(); it.Next() )
		{
			int toVertex = it.Current().toVertex;
			DataPoint* nextDp = GetDataPoint( toVertex, dest );
			if ( !calculated[toVertex] )
			{
				// note we use the 'lengthFrom', which is the
				// disance from 'it.current()' back to 'cheap'
				int distance = cheapDp->distance + it.Current().lengthFrom;
				if ( distance < nextDp->distance )
				{
					nextDp->distance = distance;
					nextDp->vertex = cheapest;
// 					GLOUTPUT( "  to %d, dist=%d\n", toVertex, distance );
				}
			}
		}
	}
	delete [] calculated;
}



#ifdef DEBUG
void GlGraph::DumpVertex()
{
	int i;
	for( i=0; i<numVertex; ++i )
	{
		printf( "vertex %c\n", i + 'a' );
		GlSListIterator< Edge > it( vertex[i].edgeList );
		for( it.Begin(); !it.Done(); it.Next() )
		{
			printf( "  edge to: %c  dist: %2d\n", 
					it.Current().toVertex + 'a',
					it.Current().lengthTo );
		}
	}
}

#endif
