/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/




#ifndef GeoPathfinder_H_
#define GeoPathfinder_H_

//#define _USE_STL_

#ifndef _USE_STL_ 
#	include "../../kyra/util/gllist.h" //maks
#	include "../gedString.h"
#	include "../fib/fibonacci.h"
#	include "../../kyra/engine/RTree.h"
#	include "SDL_mutex.h"
#else
#	include <vector>
#	define GlDynArray vector
#	define gedString string
#	define Back back
#include "RTree.h"
using namespace std;
#endif

#include "2dmath/vector2d.h"
#include "2dmath/Line2D.h"
#include "2dmath/Box2D.h"






#define CW   1
#define CCW  2
#define BOTH 3
#define DIRECTION 4

#define MAX_SEARCH_DISTANCE 100000
#define MAX_ITERATIONS 2500
#define MAX_STATES 5000 // I think it should be double of MAX_ITERATIONS to make sure it won't have overflow

// position on contours of bounding boxes
// L left
// R right
// T top
// B bottom
// M in the middle
#define LT 0
#define RT 1
#define RB 2
#define LB 3
#define LTMRT 4
#define RTMRB 5
#define RBMLB 6
#define LBMLT 7
#define FREE 8

#define TINY_EPSILON 0.001
#define EPSILON      0.01

class GPFBoxStates
{
public:
	int cornerStateIDs[4];
	GPFBoxStates () { Clear(); }
	void Clear() {for (int n=0; n<4; n++) cornerStateIDs[n] = -1;}
};

class Box : public Box2f
{
public:
	GPFBoxStates boxStates;
};

class GPFState
{
public:
	Vec2f pos;
	int ID;
	Box *boxID;
	unsigned char cornerID; 
	double bestCost;
	double distanceFromGoal;
	int bestAncestorID;
	int enabledMovements; // CW, CCW, BOTH, FREE

	GPFState ()
	{
		bestCost = 0;
		bestAncestorID = -1; // has no ancestor by default
		distanceFromGoal = MAX_SEARCH_DISTANCE;
		enabledMovements = FREE; 
		boxID = NULL;
		// teoreticaly.. only first and finishing state is free... 
		// all other states slide over contours of bounding boxes
		// and as soon as they glue off the surface they get stuck
		// in another surface or destination point finishing the algo
	}

#ifdef _USE_STL_
	bool operator<( const GPFState &a ) const 
	{
		return distanceFromGoal+bestCost > a.distanceFromGoal+a.bestCost;
	} 
#else
	bool operator>( const GPFState &a ) const 
	{
		return distanceFromGoal+bestCost > a.distanceFromGoal+a.bestCost;
	}

	bool operator<( const GPFState &a ) const 
	{
		return distanceFromGoal+bestCost < a.distanceFromGoal+a.bestCost;
	}


	bool operator==( const GPFState &a ) const 
	{
		return distanceFromGoal+bestCost == a.distanceFromGoal+a.bestCost;
	}
#endif
};


#ifdef _USE_STL_
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class PQI : public priority_queue<GPFState> 
{
public:   
	vector<GPFState>& impl() {return c;}
};
#else
class PQI : public FibonacciHeap<GPFState> 
{
public:  

};
#endif




class GeoPathFinder
{
	// objects with bounding boxes
	// needs to be initialized before pathfinding function

	//vector <GPFState> intersectionStates;
	PQI queue; // all live states (live and competiting)
	// state can be "brought from dead" if new bestCost is better then the old one
	// recycling vector of states by pushing them in queue again

	GPFState activeState;

	bool expandToDestination ();
	void expandToCW ();
	void expandToCCW ();
	


	GeoPathFinder (char *_name = NULL);

	
	void FromCornerToCenterCoordinates(Box &box);
	bool BoxReduction1(GlDynArray <Box*> &boxArray, RTree<Box*, float, 2> &rtree);
	bool BoxReduction2(Box *testBox, GlDynArray <Box*> &boxArray, RTree<Box*, float, 2> &rtree);
	static bool RTreeSearchCallback(Box* box, void* arg);
	static void RTReleaseCallback(Box* box);

public:

	virtual ~GeoPathFinder();
	static GeoPathFinder *Get(char *_name = NULL);
	static void Remove();

	void Clear();
	void AddBox(Box &box, bool bCornerCoordinate = false);
	bool BoxReduction();

	// shouldn't be public but I wanted to debug those from glsample.cpp visualy so I needed direct access
	//GlDynArray <GPFBoxStates> boxStates;
	//GPFState states[MAX_STATES]; // all passed states 
	GlDynArray <GPFState> states;
	int numStates;
	//GlDynArray <Box> boxes; 
	RTree<Box*, float, 2> originalBoxes; // unmodified dimension for reusing if possible
	Vec2f pos;
	Vec2f destination;
	Vec2f dimensions;
	GlDynArray <Vec2f> smoothedPath;


	bool findGeoAStar (Vec2f start, Vec2f end, GlDynArray <Vec2f> &controlPoints);

	//   bool expandToDestinationFromCorner (); // this is just conditional expandToDirection

	// those operate from activeState toward goal or one of neighbouring corners on same box

	// path smoothing interface
	bool getNearestIntersectionWithBoxes (Vec2f start, Vec2f dest, int excludingBox, Vec2f &nearest, double &minDistance, Box **minBoxID, int &minEdgeID);
	void smoothPath (GlDynArray <Vec2f> &smoothedPath);

	SDL_mutex *GetMutex() {return mutEx;}

#ifdef _DEBUG
	void Save(char *fileName);
#endif

#ifndef _USE_STL_
	//For thread
	SDL_mutex *mutEx;
#endif

	bool bNeedReduceBoxes;

	//For reuse
	gedString obstacle;
	double positionSum;
};



#endif
