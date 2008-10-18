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

#ifndef GLGRAPH_INCLUDED
#define GLGRAPH_INCLUDED

#include "gllist.h"
#include "gldynarray.h"
#include "glinsidelist.h"
#include "glcirclelist.h"


/* 	A class to solve a graph algorith. Vertices and edges, which have a travel cost, in
	a connected system.
*/
class GlGraph
{
  public:

	GlGraph( int numVertex );
	~GlGraph();

	/** Function to add an edge which connects 2 vertices. Will add a bi-connected edge (A to B is
		the same distance as B to A). A pair of vertices can be connected only by a single edge.
	*/
	void AddBiEdge( int source, int dest, int length );

	/** Function to add an edge which connects 2 vertices. Will add a bi-connected edge where
		each edge has its own weight, either of which can be INFINITE.
	*/
	void AddBiEdge( int source, int dest, int lengthSourceToDest, int lengthDestToSource );

	/** Function to add an edge which connects 2 vertices. Will add a one way edge. 
		A pair of vertices can be connected only by a single edge.
	*/
	void AddEdge( int source, int dest, int length );

	/** Get the shortest path from source to dest.
		@param	nextVertex	The vertex to go to. It will be adjacent to 'source'
		@param	length		The distance to the next vertex.
		@param	distance	The distance to the destination vertex.
	*/
	void GetPath( int source, int dest, int *nextVertex, int* length, int *distance );

	#ifdef DEBUG
		void DumpVertex();
	#endif

  private:
	struct DataPoint
	{
		int		distance;	// from the DataPoint to the destination
		int		vertex;		// the vertex to take
	};

	struct Edge
	{
		int lengthTo;	// how long the edge is, from the source vertex to 'toVertex'
		int lengthFrom;	// how long the edge is, from the 'toVertex' back to the source
		int toVertex;	// which vertex this edge goes to

		bool operator==( const Edge& rhs )	{ return lengthTo == rhs.lengthTo && lengthFrom == rhs.lengthFrom && toVertex == rhs.toVertex; }
	};

	struct Vertex
	{
		Vertex() : destinationCalculated( false ) {}

		bool			destinationCalculated;
		GlSList< Edge > edgeList;
	};
	
	void LowerAddEdge( int v0, int v1, int length0to1, int length1to0 );
	void ShortestPathCalc( int destVertex );
	DataPoint* GetDataPoint( int source, int dest )	{ return ( dataPoint + dest * numVertex + source ); }
	int FindCheapest( GlCircleList<int>* set, int dest );

	enum {
		GL_INFINITE = 0x1000000
	};

	int					numVertex;
	Vertex				*vertex;
	DataPoint			*dataPoint;
};


#endif


