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


///////////////////////////////////////////////////////////////////

#include "GeoPathfinder.h"

#ifdef LLVM
#	define linux
#endif

#ifndef _USE_STL_
extern "C"
{
#	include "../fib/fib.c"
}

#include "../../kyra/util/glmap.h"
typedef GlMap< gedString, GeoPathFinder *, GlStringHash >		MapPathFinder;
typedef GlMapIterator< gedString, GeoPathFinder *, GlStringHash >		MapPathFinderIterator;

MapPathFinder mapPathFinder;

#endif






GeoPathFinder::GeoPathFinder(char *_name)
{
	numStates = 0;
	positionSum = 0.0;
	bNeedReduceBoxes = false;

	originalBoxes.SetReleaseCallback(RTReleaseCallback);

#ifndef _USE_STL_ 
	states.setAutoExpand(true);
	//boxes.setAutoExpand(true);
	//boxStates.setAutoExpand(true);
	states.Resize(256); //Initial size to avoid initial allocations
#else
	states.resize(10000);
#endif

#ifndef _USE_STL_
	if(_name)
	{
		obstacle = _name;
		mapPathFinder.Add(_name, this);
	}

	mutEx = SDL_CreateMutex();
#endif
}

GeoPathFinder::~GeoPathFinder()
{
#ifndef _USE_STL_
	//Block here to avoid remotion while in calc
	//Due destruction here, don't need release the mutex. Just destroy it
	SDL_mutexP(mutEx);
	SDL_DestroyMutex(mutEx);

	mapPathFinder.Remove(obstacle);	
#endif
}

GeoPathFinder *GeoPathFinder::Get(char *_name)
{
#ifndef _USE_STL_
	GeoPathFinder **gp = mapPathFinder[_name];

	if(!gp || !*gp)
	{
		return new GeoPathFinder(_name);
	}

	return *gp;
#else
	return new GeoPathFinder(_name);
#endif
}

void GeoPathFinder::Remove()
{
#ifndef _USE_STL_
	//Remove all finders
	MapPathFinderIterator it(mapPathFinder);
	it.Begin();
	while(!it.Done())
	{
		delete *it.Value();
		it.Begin();
	}

	mapPathFinder.Clear();
#endif
}

void GeoPathFinder::Clear()
{
#ifndef _USE_STL_ 
	//boxes.ClearCount();
	originalBoxes.RemoveAll();
	//boxStates.ClearCount();
#else
	//boxes.clear();
	originalBoxes.RemoveAll();
	//boxStates.clear();
#endif
}


void GeoPathFinder::AddBox(Box &box, bool bCornerCoordinate)
{
	if(bCornerCoordinate) FromCornerToCenterCoordinates(box);

	float rMin[2], rMax[2];	

	rMin[0] = box.Left();
	rMin[1] = box.Top();

	rMax[0] = box.Right();
	rMax[1] = box.Bottom();

	originalBoxes.Insert(rMin, rMax, new Box(box));

	//Don't call BoxReduction here on every new box
	//If call here will remove more box, but it's very slow and depends of the add order
	//Takes 15s to reduce Move To obstacle.ged in the Pocket PC
	//Caller after add all boxes takes only 5s
	//BoxReduction();

	bNeedReduceBoxes = true; //Call at first findStar execution
}

void GeoPathFinder::FromCornerToCenterCoordinates(Box &box)
{
	box.setOrigin(box.getOrigin() + (box.getDimensions()/2));
}


void GeoPathFinder::RTReleaseCallback(Box* box) 
{  
	delete box; 
}

bool GeoPathFinder::RTreeSearchCallback(Box* box, void* arg) 
{  
	GlDynArray <Box*> *out = (GlDynArray <Box*> *)arg;
	out->push_back(box);

	return true; 
}


bool GeoPathFinder::BoxReduction()
{
	//Works
	bool bReduction = false, bMerged = false;;	
	float rMin[2], rMax[2];	
	GlDynArray <Box*> tmp;

	RTree<Box*, float, 2>::Iterator it;
	for(originalBoxes.GetFirst(it); it.IsNotNull();)
	{
		Box *box = *it;

		rMin[0] = box->Left();
		rMin[1] = box->Top();

		rMax[0] = box->Right();
		rMax[1] = box->Bottom();
		

		bool bReset = false;
				
		do {
			bMerged = false;
			tmp.clear();

			if(originalBoxes.Search(rMin, rMax, RTreeSearchCallback, (void*)&tmp) > 1) //Always include the test box
			{
				if(BoxReduction1(tmp, originalBoxes))
				{					
					bReset = bMerged = bReduction = true;
					box = tmp[0];

					rMin[0] = box->Left();
					rMin[1] = box->Top();

					rMax[0] = box->Right();
					rMax[1] = box->Bottom();
				}
			}
		} while(bMerged);

		if(bReset) originalBoxes.GetFirst(it);
		else ++it;
	}	

	bNeedReduceBoxes = false;
	return bReduction;
}


bool GeoPathFinder::BoxReduction1(GlDynArray <Box*> &boxArray, RTree<Box*, float, 2> &rtree)
{
	//Brute force: slow, but works
	bool bReduction = false, bMerge = true;
	Box *r1, *r2;
	float rMin[2], rMax[2];

	while(bMerge)
	{
start:

#ifdef _USE_STL_ 
		vector<Box*>::iterator i, j;

		for( i = boxArray.begin(); i != boxArray.end(); i++ )
		{
			for( j = boxArray.begin(); j != boxArray.end(); j++ )
			{
#else
		int i, j;

		for(i = 0; i != boxArray.size(); i++ )
		{
			for(j = 0; j != boxArray.size(); j++ )
			{
#endif
				if(i != j)
				{

#ifdef _USE_STL_ 
					r1 = *i;
					r2 = *j;
#else
					r1 = boxArray[i];
					r2 = boxArray[j];
#endif
					
					if(r1 != r2)
					{
						if(r1->contains(*r2))
						{
							rMin[0] = r2->Left();
							rMin[1] = r2->Top();

							rMax[0] = r2->Right();
							rMax[1] = r2->Bottom();

							rtree.Remove(rMin, rMax, r2);
#ifdef _USE_STL_
							boxArray.erase(j);	
#else
							boxArray.Remove(j);	
#endif

							bReduction = true;
							goto start;
						}

						if( (
							(r1->getDimensions().x == r2->getDimensions().x && r1->getOrigin().x == r2->getOrigin().x) 
							|| (r1->getDimensions().y == r2->getDimensions().y && r1->getOrigin().y == r2->getOrigin().y)
							) &&
							r1->intersects(*r2))
						{
							//Remove r1
							rMin[0] = r1->Left();
							rMin[1] = r1->Top();

							rMax[0] = r1->Right();
							rMax[1] = r1->Bottom();

							originalBoxes.SetReleaseCallback(NULL); //Reuse this object
							rtree.Remove(rMin, rMax, r1);
							originalBoxes.SetReleaseCallback(RTReleaseCallback);
							

							//Merge
							r1->Merge(*r2);

							//Put the new r1 back
							rMin[0] = r1->Left();
							rMin[1] = r1->Top();

							rMax[0] = r1->Right();
							rMax[1] = r1->Bottom();

							rtree.Insert(rMin, rMax, r1);
							

							//Remove r2
							rMin[0] = r2->Left();
							rMin[1] = r2->Top();

							rMax[0] = r2->Right();
							rMax[1] = r2->Bottom();
							
							rtree.Remove(rMin, rMax, r2);	

#ifdef _USE_STL_
							boxArray.erase(j);	
#else
							boxArray.Remove(j);	
#endif
	
							bReduction = true;
							goto start;
						}
					}
				}
			}
		}

		bMerge = false;
	}

	return bReduction;
}

bool GeoPathFinder::BoxReduction2(Box *testBox, GlDynArray <Box*> &boxArray, RTree<Box*, float, 2> &rtree)
{
	//Brute force: slow, but works
	bool bReduction = false, bMerge = true;
	Box *r2;
	float rMin[2], rMax[2];

	while(bMerge)
	{
start:


#ifdef _USE_STL_ 
		vector<Box*>::iterator i;

		for( i = boxArray.begin(); i != boxArray.end(); i++ )
		{

#else
		int i;

		for(i = 0; i != boxArray.size(); i++ )
		{
#endif

#ifdef _USE_STL_ 
			r2 = *i;
#else
			r2 = boxArray[i];
#endif

			if(testBox != r2)
			{
				if(testBox->contains(*r2))
				{
					rMin[0] = r2->Left();
					rMin[1] = r2->Top();

					rMax[0] = r2->Right();
					rMax[1] = r2->Bottom();

					rtree.Remove(rMin, rMax, r2);
#ifdef _USE_STL_
					boxArray.erase(i);	
#else
					boxArray.Remove(i);	
#endif

					bReduction = true;
					goto start;
				}

				if( (
					(testBox->getDimensions().x == r2->getDimensions().x && testBox->getOrigin().x == r2->getOrigin().x) 
					|| (testBox->getDimensions().y == r2->getDimensions().y && testBox->getOrigin().y == r2->getOrigin().y)
					) &&
					testBox->intersects(*r2))
				{
					//Remove r1
					rMin[0] = testBox->Left();
					rMin[1] = testBox->Top();

					rMax[0] = testBox->Right();
					rMax[1] = testBox->Bottom();

					originalBoxes.SetReleaseCallback(NULL); //Reuse this object
					rtree.Remove(rMin, rMax, testBox);
					originalBoxes.SetReleaseCallback(RTReleaseCallback);


					//Merge
					testBox->Merge(*r2);

					//Put the new r1 back
					rMin[0] = testBox->Left();
					rMin[1] = testBox->Top();

					rMax[0] = testBox->Right();
					rMax[1] = testBox->Bottom();

					rtree.Insert(rMin, rMax, testBox);


					//Remove r2
					rMin[0] = r2->Left();
					rMin[1] = r2->Top();

					rMax[0] = r2->Right();
					rMax[1] = r2->Bottom();

					rtree.Remove(rMin, rMax, r2);	

#ifdef _USE_STL_
					boxArray.erase(i);	
#else
					boxArray.Remove(i);	
#endif

					bReduction = true;
					goto start;
				}
			}

		}

		bMerge = false;
	}



	return bReduction;
}







// geometrical pathfinder
// conditions: 
//   all objects have their bounding boxes
//   bounding objects are not oriented (aligned to X and Y)

// algo:

// branching happens whenever we find new intersection point
// clockwise and counter-clockwise direction of movement
// number of branches after avoiding 8 obstacles is 256 by default strategy

// to reduce number of branching, we could mark bbox corners as we reach them
// each marker could carry it's travel cost to get there from pole position
// his pointer to another marker tells us previous checkpoint for reaching it
// marker should be encoded boxID + cornerID (0,1,2 or 3) 
// it's value should be float <- movement cost until marker


// allocate state on starting position with no object associated, zero cost
// while (not reached target)
// {
//   try traveling from intersection point to cornerID+1 // (CW direction)
//     if (found newIntersectionPoint) 
//     then try CW traveling from newIntersectionPoint to next CW corner on newObject
//     else check corner's marker and compare values, keep lesser cost and appropriate pointer
//   try traveling from intersection point to cornerID-1 // (CCW direction)
//     if (found newIntersectionPoint) 
//     then try traveling from newIntersectionPoint to next CCW corner on newObject
//     else check corner's marker and compare values, keep lesser cost and appropriate pointer
// // note that initial very high costs could be specified by some algo on top of this one
// }
///////////
// note: above was initial pseudo code... tech is a bit more advanced, controllable and optimized now
//////////////////////////////////////////////////////////////////////////////////////////////////////

bool GeoPathFinder::findGeoAStar (Vec2f start, Vec2f end, GlDynArray<Vec2f>	&controlPoints/*vector <Vec2f> &controlPoints*/)
{
	if(bNeedReduceBoxes)
	{
		BoxReduction();
	}

	bool foundPath = false;
	controlPoints.clear ();

	// flushing (reseting) queue and states array to empty state

#ifndef _USE_STL_ 
	queue.clear();
#else
	while (queue.size()) queue.pop();
#endif

	numStates = 0;
	int numBoxes = originalBoxes.Count();

	// configuring pathfinder's source and destination
	pos = start;
	destination = end;

	// this is initial state for pathfinder.... (0-th in control points or left out in result)
	GPFState newActiveState;
	activeState = newActiveState;
	activeState.pos = start;
	activeState.ID = numStates;
	activeState.bestCost = 0;
	activeState.bestAncestorID = -1;
	activeState.boxID = NULL;
	activeState.distanceFromGoal = (start - end).getLength ();

#ifdef _USE_STL_ 
	if(states.size() <= numStates) states.resize(numStates+1);
	//if(boxes.size() != originalBoxes.size()) boxes.resize(originalBoxes.size());
	//if(boxStates.size() != numBoxes) boxStates.resize(numBoxes);
#endif

	states[numStates++] = activeState;
	//	queue.push (activeState);

	// resets state references which lay on corners of boxes
	
	RTree<Box*, float, 2>::Iterator it;
	for(originalBoxes.GetFirst(it); it.IsNotNull(); ++it)
	{
		(*it)->boxStates.Clear();			
		

		//boxes[b].set(originalBoxes[b].getOrigin(), originalBoxes[b].getDimensions () /*+ dimensions*/); //maks

		//maks: Don't block the invalid start. So, the actor can leave an invalid location
		/*if (boxes[b].contains (start)) 
		{ 
			controlPoints.push_back (start);
			return false; 
		}*/
	}


	//while (activeState.pos != destination)
	int n=0;
	for (n=0; n<MAX_ITERATIONS && activeState.pos != destination; n++)
	{
		if (activeState.enabledMovements & FREE) foundPath = expandToDestination ();
		if (activeState.enabledMovements & CW) expandToCW ();
		if (activeState.enabledMovements & CCW) expandToCCW ();
		if ((activeState.enabledMovements & DIRECTION) && activeState.boxID) 
		{
			// check whether is corner facing the destination and allow to take off from the bounding box of previous object 
			// (if conditions are met) <- optimization by avoiding attempts to travel which are doomed to fail
			Vec2f bboxpos = activeState.boxID->getOrigin ();
			if ((bboxpos.y > activeState.pos.y && activeState.pos.y > destination.y)
				|| (bboxpos.y < activeState.pos.y && activeState.pos.y < destination.y)
				|| (bboxpos.x < activeState.pos.x && activeState.pos.x < destination.x)
				|| (bboxpos.x > activeState.pos.x && activeState.pos.x > destination.x))
			{
				foundPath = expandToDestination ();
			}
		}
		if (foundPath) break;
		if (!queue.empty ()) 
		{
			// pop best suited state... in current case it's heuristic function f(x) = distanceFromGoal + bestCost/4
			activeState = queue.top ();
			queue.pop ();
		}
		else 
		{
			double minDistance = MAX_SEARCH_DISTANCE;
			Line2f l1, l2;
			for (int s=0; s<numStates; s++)
			{
				if (states[s].bestAncestorID == -1) continue;
				int parentID = states[s].bestAncestorID;

				l1.setExplicit (states[parentID].pos, states[s].pos);
				Vec2f dir2 (l1.getDirection ().y, -l1.getDirection ().x);
				if (dir2 * (destination - states[parentID].pos) > 0) dir2 = -dir2;

				dir2 = dir2.getNormalized () * MAX_SEARCH_DISTANCE;
				l2.setParametric (destination, dir2);

				Vec2f intersectionPoint;
				if (getLineSegmentsIntersectionPoint (l1, l2, intersectionPoint))
				{
					double distance = (intersectionPoint - destination).getLength ();
					if (distance >= minDistance) continue;
					GPFState newState;
					newState.bestAncestorID = parentID;
					newState.bestCost = states[parentID].bestCost + (intersectionPoint - states[parentID].pos).getLength ();
					newState.distanceFromGoal = distance;
					newState.pos = intersectionPoint;
					newState.enabledMovements = 0;

					activeState = newState;
					minDistance = distance;
				}
				else
				{
					double distance = states[s].distanceFromGoal;
					if (distance < minDistance) { activeState = states[s]; minDistance = distance; }
				}
			}
			break;
		}
	}

	if (n==5000)//maks: 5000 < MAX_ITERATIONS, so this code is never executed
	{
		double minDistance = MAX_SEARCH_DISTANCE;
		Line2f l1, l2;
		for (int s=0; s<numStates; s++)
		{
			if (states[s].bestAncestorID == -1) continue;
			int parentID = states[s].bestAncestorID;

			l1.setExplicit (states[parentID].pos, states[s].pos);
			Vec2f dir2 (l1.getDirection ().y, -l1.getDirection ().x);
			if (dir2 * (destination - states[parentID].pos) > 0) dir2 = -dir2;

			dir2 = dir2.getNormalized () * MAX_SEARCH_DISTANCE;
			l2.setParametric (destination, dir2);

			Vec2f intersectionPoint;
			if (getLineSegmentsIntersectionPoint (l1, l2, intersectionPoint))
			{
				double distance = (intersectionPoint - destination).getLength ();
				if (distance >= minDistance) continue;
				GPFState newState;
				newState.bestAncestorID = parentID;
				newState.bestCost = states[parentID].bestCost + (intersectionPoint - states[parentID].pos).getLength ();
				newState.distanceFromGoal = distance;
				newState.pos = intersectionPoint;
				newState.enabledMovements = 0;

				activeState = newState;
				minDistance = distance;
			}
			else
			{
				double distance = states[s].distanceFromGoal;
				if (distance < minDistance) { activeState = states[s]; minDistance = distance; }
			}
		}
	}

	//	if (foundPath)
	{
		GPFState temp = activeState;
		while (1)
		{
			controlPoints.push_back (temp.pos);
			if (temp.bestAncestorID == -1) break;
			temp = states[temp.bestAncestorID];
		}
	}


	// get rid of garbage before next search start
 
	return true;
}

template <typename RealType>
Box getLineBoudingBox(Line2D <RealType> &line)
{
	Box box2;

	box2.setOrigin ((line.getStart() + line.getEnd ())/2);	
	Vector2D <RealType> linedims = line.getDirection();
	if (linedims.x < 0) linedims.x = -linedims.x;
	if (linedims.y < 0) linedims.y = -linedims.y;
	box2.setDimensions (linedims);

	return box2;
}

template <typename RealType>
bool getBoxLineIntersection (GeoPathFinder *gp, Box *box, Line2D <RealType> line, Vector2D <RealType> &intersection, int &side)
{
	Box box1;

#ifdef linux
	Vec2f a1(box->getOrigin()), a2(box->getDimensions() + gp->dimensions);
	box1.set(a1, a2);
#else
	box1.set(box->getOrigin(), box->getDimensions() + gp->dimensions);
#endif

	Vector2D <RealType> a, b, c, d;
	Vector2D <RealType> bpos = box1.getOrigin();
	Vector2D <RealType> bdims = box1.getDimensions();
	bdims/=2;

#ifdef linux
	Box b1 = getLineBoudingBox(line);
	if (!box1.intersects (b1)) 
		return false;
#else
	if (!box1.intersects (getLineBoudingBox(line))) 
		return false;
#endif

	//	if (box.contains (line.getStart ())) { intersection = line.getStart (); side = 8; return true; }
	Line2f lines[4];

#ifdef linux
	Vector2D <RealType> src1(bpos.x - bdims.x, bpos.y + bdims.y),
		dst1(bpos.x + bdims.x, bpos.y + bdims.y),
		dst2(bpos.x + bdims.x, bpos.y - bdims.y),
		dst3(bpos.x - bdims.x, bpos.y - bdims.y);

	lines[0].setExplicit (src1, dst1);
	lines[1].setExplicit (dst1, dst2);
	lines[2].setExplicit (dst2, dst3);
	lines[3].setExplicit (dst3, src1);
#else
	lines[0].setExplicit (Vector2D <RealType> (bpos.x - bdims.x, bpos.y + bdims.y), Vector2D <RealType> (bpos.x + bdims.x, bpos.y + bdims.y));
	lines[1].setExplicit (Vector2D <RealType> (bpos.x + bdims.x, bpos.y + bdims.y), Vector2D <RealType> (bpos.x + bdims.x, bpos.y - bdims.y));
	lines[2].setExplicit (Vector2D <RealType> (bpos.x + bdims.x, bpos.y - bdims.y), Vector2D <RealType> (bpos.x - bdims.x, bpos.y - bdims.y));
	lines[3].setExplicit (Vector2D <RealType> (bpos.x - bdims.x, bpos.y - bdims.y), Vector2D <RealType> (bpos.x - bdims.x, bpos.y + bdims.y));
#endif



	Vector2D <RealType> is[4]; 
	bool isFound[4];
	int i = 0;
	for (i=0; i<4; i++)
	{
		isFound[i] = getLineSegmentsIntersectionPoint (line, lines[i], is[i]);
		//    if (i > 0) isFound[i] = false;
	}

	////////////////////////////////////////////////////////////
	/*You don't have to initialize it.
	It will take some initial value when minIs == -1.

	basicaly... when searching for minimum you can do it on two ways.... robust or better performing

	- faster one

	float minDistance = MAX_SEARCH_DISTANCE;
	for (int n=0; n<numValues; n++)
	{
	distance = ....
	if (distance < minDistance)
	{ 
	minDistance = distance;
	....
	}
	}

	- robust one

	int minFound = -1; // traditionaly means "NO"
	float minDistance;
	for (int n=0; n<numValues; n++)
	{
	distance = .....
	if (distance < minDistance || minFound == -1)
	{
	minFound = n;
	minDistance = distance;
	....
	}
	}

	.... only difference is maximal value of minimum which does not exist in robust approach but that one is making two comparations although if first one is true, second one is not going to be checked and thus is not actually 2 times slower... anyway... it's up to you and your preference about this.


	- note that you could also make something like....

	float minDistance = _FIRST_VALUE_IF_THERE_IS_ANY;
	for (int n= 1 ; n<numValues; n++)
	{
	distance = .....
	if (distance < minDistance)
	{
	minDistance = distance;
	....
	}
	}*/
	///////////////////////////////////////////////////

	int minIs = -1;
	double minDistance = 0;
	for (i=0; i<4; i++)
	{
		if (!isFound[i]) continue;
		RealType dist = (is[i] - line.getStart ()).getLength ();
		if (dist < minDistance || minIs == -1)
		{ 
			minDistance = dist; 
			side = 4+i;
			minIs = i; 
		}
	}

	if (minIs == -1) return false;

	intersection = is[minIs];

	for (int n=0; n<4; n++)
		if (TINY_EPSILON > (lines[n].getStart () - intersection).getLength ()) 
			side = n;

	return true;
}

bool GeoPathFinder::expandToDestination ()
{
	bool foundIntersectionPoint = false;
	Vec2f nearest;
	double minDistance = MAX_SEARCH_DISTANCE;
	Box *minBoxID = NULL;
	Line2f line;
	line.setExplicit (activeState.pos, destination);
	int minEdgeID;
	Box lineBox = getLineBoudingBox(line);
	lineBox.setDimensions(lineBox.getDimensions() + dimensions);

	GlDynArray <Box*> tmp;
	float rMin[2], rMax[2];

	rMin[0] = lineBox.Left();
	rMin[1] = lineBox.Top();

	rMax[0] = lineBox.Right();
	rMax[1] = lineBox.Bottom();

	if(originalBoxes.Search(rMin, rMax, RTreeSearchCallback, (void*)&tmp))
	{

		// find nearest intersection point with all objects's bounding boxes
		int numBoxes = tmp.size();
		for (int b=0; b<numBoxes; b++)
		{
			Vec2f intersectionPoint;
			if (activeState.enabledMovements != FREE && tmp[b] == activeState.boxID) continue;


			int edgeID;
			bool res = getBoxLineIntersection (this, tmp[b], line, intersectionPoint, edgeID);
			if (!res) continue;

			double dist = (intersectionPoint - activeState.pos).getLength ();
			if (minBoxID == NULL || dist < minDistance)
			{
				minDistance = dist;
				minBoxID = tmp[b];
				minEdgeID = edgeID;
				nearest = intersectionPoint;
				foundIntersectionPoint = true;
			}
		}  
	}

	if (foundIntersectionPoint)
	{
		// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
		Vec2f offset = (nearest - minBoxID->getOrigin ()).getNormalized() * EPSILON; 
		nearest += offset;

		GPFState newState;
		newState.pos = nearest;
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.bestCost = activeState.bestCost + minDistance;
		newState.distanceFromGoal = (nearest - destination).getLength ();
		newState.boxID = minBoxID;
		newState.cornerID = minEdgeID;
		newState.enabledMovements = CW | CCW;
		queue.push (newState);

		states[numStates++] = newState;
		return false;
		//     allocate newState on min and associate minObject with it
		//     calculate cost from active state and set pointer to active one
		//     priority queue.push (newState)
		//     return false;
	}
	else
	{  
		GPFState newState;
		newState.pos = destination;
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.bestCost = activeState.bestCost + (destination-activeState.pos).getLength ();
		newState.enabledMovements = FREE;
		newState.distanceFromGoal = 0;
		states[numStates++] = newState;
		queue.push (newState);
		activeState = newState;
		return true;
		//     allocate newState on destination and associate NULL with it
		//     calculate cost from previous state and set pointer to active one
		//     newState->enabledMovements = BOTH;
		//     priority queue.push (newState)
		//     return true;
	}
}



void GeoPathFinder::expandToCW ()
{
	bool foundIntersectionPoint = false;
	Vec2f nearest;
	double minDistance = MAX_SEARCH_DISTANCE;
	Box *minObjectID = NULL;
	Line2f line;
	Vec2f dest;// = cornerPoint; 
	Vec2f bboxpos = activeState.boxID->getOrigin();
	Vec2f bboxdims = activeState.boxID->getDimensions() + dimensions; //maks
	bboxdims/=2;
	int newCornerID;
	if (activeState.cornerID == LTMRT) newCornerID = RT;
	else if (activeState.cornerID == RTMRB) newCornerID = RB;
	else if (activeState.cornerID == RBMLB) newCornerID = LB;
	else if (activeState.cornerID == LBMLT) newCornerID = LT;
	else newCornerID = (activeState.cornerID+1)%4;

	if (newCornerID == LT) dest = bboxpos - Vec2f (bboxdims.x, -bboxdims.y);
	if (newCornerID == RT) dest = bboxpos - Vec2f (-bboxdims.x, -bboxdims.y);
	if (newCornerID == RB) dest = bboxpos - Vec2f (-bboxdims.x, bboxdims.y);
	if (newCornerID == LB) dest = bboxpos - Vec2f (bboxdims.x, bboxdims.y);

	// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
	Vec2f targetOffset = (dest - activeState.boxID->getOrigin ()).getNormalized() * EPSILON; 
	dest += targetOffset;

	line.setExplicit (activeState.pos, dest);
	int minEdgeID;

	Box lineBox = getLineBoudingBox(line);
	lineBox.setDimensions(lineBox.getDimensions() + dimensions);

	GlDynArray <Box*> tmp;
	float rMin[2], rMax[2];

	rMin[0] = lineBox.Left();
	rMin[1] = lineBox.Top();

	rMax[0] = lineBox.Right();
	rMax[1] = lineBox.Bottom();

	if(originalBoxes.Search(rMin, rMax, RTreeSearchCallback, (void*)&tmp))
	{

		// find nearest intersection point with all objects's bounding boxes
		int numBoxes = tmp.size();
		for (int o=0; o<numBoxes; o++)
		{
			int edgeID;
			if (tmp[o] == activeState.boxID) continue;
			Vec2f intersectionPoint;
			bool res = getBoxLineIntersection (this, tmp[o], line, intersectionPoint, edgeID);
			if (!res) continue;
			double dist = (intersectionPoint - activeState.pos).getLength ();
			if (dist < minDistance)
			{
				minDistance = dist;
				minObjectID = tmp[o];
				minEdgeID = edgeID;
				nearest = intersectionPoint;
				foundIntersectionPoint = true;
			}
		}  
	}

	if (foundIntersectionPoint)
	{
		// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
		Vec2f offset = (nearest - minObjectID->getOrigin ()).getNormalized() * EPSILON; 
		if (minEdgeID == LTMRT) offset = Vec2f (0, EPSILON);
		if (minEdgeID == RTMRB) offset = Vec2f (EPSILON, 0);
		if (minEdgeID == RBMLB) offset = Vec2f (0, -EPSILON);
		if (minEdgeID == LBMLT) offset = Vec2f (-EPSILON, 0);

		GPFState newState;
		newState.pos = nearest + offset;
		newState.bestCost = activeState.bestCost + minDistance;
		newState.distanceFromGoal = (nearest - destination).getLength ();
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.boxID = minObjectID;
		newState.cornerID = minEdgeID;
		newState.enabledMovements = CW;
		queue.push (newState);
		states[numStates++] = newState;
	}
	else
	{  
		// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
		Vec2f offset = (dest - activeState.boxID->getOrigin ()).getNormalized() * EPSILON; 

		GPFState newState;
		newState.pos = dest + offset;
		newState.distanceFromGoal = (dest - destination).getLength ();
		newState.bestCost = activeState.bestCost + (dest - activeState.pos).getLength ();
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.cornerID = newCornerID;
		newState.boxID = activeState.boxID;
		newState.enabledMovements = CW | DIRECTION;
		int ID = newState.boxID->boxStates.cornerStateIDs[newState.cornerID];
		if (ID == -1 || (ID != -1 && states[ID].bestCost > newState.bestCost))
		{
			queue.push (newState);
			states[numStates++] = newState;
			newState.boxID->boxStates.cornerStateIDs[newState.cornerID] = newState.ID;
		}
	}
}


void GeoPathFinder::expandToCCW ()
{
	bool foundIntersectionPoint = false;
	Vec2f nearest;
	double minDistance = MAX_SEARCH_DISTANCE;
	Box *minObjectID = NULL;
	Line2f line;
	Vec2f dest;// = cornerPoint; 
	Vec2f bboxpos = activeState.boxID->getOrigin();
	Vec2f bboxdims = activeState.boxID->getDimensions() + dimensions; //maks
	bboxdims/=2;
	int newCornerID;
	if (activeState.cornerID == LTMRT) newCornerID = LT;
	else if (activeState.cornerID == RTMRB) newCornerID = RT;
	else if (activeState.cornerID == RBMLB) newCornerID = RB;
	else if (activeState.cornerID == LBMLT) newCornerID = LB;
	else newCornerID = (activeState.cornerID+3)%4;

	if (newCornerID == LT) dest = bboxpos - Vec2f (bboxdims.x, -bboxdims.y);
	if (newCornerID == RT) dest = bboxpos - Vec2f (-bboxdims.x, -bboxdims.y);
	if (newCornerID == RB) dest = bboxpos - Vec2f (-bboxdims.x, bboxdims.y);
	if (newCornerID == LB) dest = bboxpos - Vec2f (bboxdims.x, bboxdims.y);

	// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
	Vec2f targetOffset = (dest - activeState.boxID->getOrigin ()).getNormalized() * EPSILON; 
	dest += targetOffset;

	line.setExplicit (activeState.pos, dest);
	int minEdgeID;

	Box lineBox = getLineBoudingBox(line);
	lineBox.setDimensions(lineBox.getDimensions() + dimensions);

	GlDynArray <Box*> tmp;
	float rMin[2], rMax[2];

	rMin[0] = lineBox.Left();
	rMin[1] = lineBox.Top();

	rMax[0] = lineBox.Right();
	rMax[1] = lineBox.Bottom();

	if(originalBoxes.Search(rMin, rMax, RTreeSearchCallback, (void*)&tmp))
	{

		// find nearest intersection point with all objects's bounding boxes
		int numBoxes = tmp.size ();
		for (int o=0; o<numBoxes; o++)
		{
			int edgeID;
			//		if (o == activeState.boxID) continue;
			Vec2f intersectionPoint;
			bool res = getBoxLineIntersection (this, tmp[o], line, intersectionPoint, edgeID);
			if (!res) continue;

			double dist = (intersectionPoint - activeState.pos).getLength ();
			if (dist < minDistance)
			{
				minDistance = dist;
				minObjectID = tmp[o];
				minEdgeID = edgeID;
				nearest = intersectionPoint;
				foundIntersectionPoint = true;
			}
		}
	}

	if (foundIntersectionPoint)
	{
		// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
		Vec2f offset = (nearest - minObjectID->getOrigin ()).getNormalized() * EPSILON; 
		if (minEdgeID == LTMRT) offset = Vec2f (0, EPSILON);
		if (minEdgeID == RTMRB) offset = Vec2f (EPSILON, 0);
		if (minEdgeID == RBMLB) offset = Vec2f (0, -EPSILON);
		if (minEdgeID == LBMLT) offset = Vec2f (-EPSILON, 0);

		GPFState newState;
		newState.pos = nearest + offset;
		newState.bestCost = activeState.bestCost + minDistance;
		newState.distanceFromGoal = (nearest - destination).getLength ();
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.boxID = minObjectID;
		newState.cornerID = minEdgeID;
		newState.enabledMovements = CCW;
		queue.push (newState);
		states[numStates++] = newState;
	}
	else
	{  
		// gluing off the surface to avoid sudden scratching two objects (hardcase scenario)
		Vec2f offset = (dest - activeState.boxID->getOrigin ()).getNormalized() * EPSILON; 

		GPFState newState;
		newState.pos = dest + offset;
		newState.distanceFromGoal = (dest - destination).getLength ();
		newState.bestCost = activeState.bestCost + (dest - activeState.pos).getLength ();
		newState.ID = numStates;
		newState.bestAncestorID = activeState.ID;
		newState.boxID = activeState.boxID;
		newState.cornerID = newCornerID;
		newState.enabledMovements = CCW | DIRECTION;
		int ID = newState.boxID->boxStates.cornerStateIDs[newState.cornerID];
		if (ID == -1 || (ID != -1 && states[ID].bestCost > newState.bestCost))
		{
			queue.push (newState);
			states[numStates++] = newState;
			newState.boxID->boxStates.cornerStateIDs[newState.cornerID] = newState.ID;
		}
	}
}



bool GeoPathFinder::getNearestIntersectionWithBoxes (Vec2f start, Vec2f dest, int excludingBox, Vec2f &nearest, double &minDistance, Box **minBoxID, int &minEdgeID)
{
	Vec2f dir = (dest-start).getNormalized ();

	Line2f line; 

#ifdef linux
	Vec2f src(start+dir*TINY_EPSILON);
	line.setExplicit (src, dest); // fixes bug "onReachIntersectionOfTwoObstacles"
#else
	line.setExplicit (start+dir*TINY_EPSILON, dest); // fixes bug "onReachIntersectionOfTwoObstacles"
#endif

	bool foundIntersectionPoint = false;
	minDistance = MAX_SEARCH_DISTANCE;

	Box lineBox = getLineBoudingBox(line);
	lineBox.setDimensions(lineBox.getDimensions() + dimensions);

	GlDynArray <Box*> tmp;
	float rMin[2], rMax[2];

	rMin[0] = lineBox.Left();
	rMin[1] = lineBox.Top();

	rMax[0] = lineBox.Right();
	rMax[1] = lineBox.Bottom();

	if(originalBoxes.Search(rMin, rMax, RTreeSearchCallback, (void*)&tmp))
	{

		// find nearest intersection point with all objects's bounding boxes
		int numBoxes = tmp.size ();
		for (int o=0; o<numBoxes; o++)
		{
			int edgeID;
			if (o == excludingBox) continue;
			Vec2f intersectionPoint;
			bool res = getBoxLineIntersection (this, tmp[o], line, intersectionPoint, edgeID);
			if (!res) continue;
			double dist = (intersectionPoint - start).getLength ();
			if (dist < minDistance)
			{
				minDistance = dist;
				*minBoxID = tmp[o];
				minEdgeID = edgeID;
				nearest = intersectionPoint;
				foundIntersectionPoint = true;
			}
		} 
	}

	return foundIntersectionPoint;
}

void GeoPathFinder::smoothPath (GlDynArray <Vec2f> &smoothedPath)
{
	smoothedPath.clear ();

	GlDynArray <GPFState> path2;
	GPFState temp = activeState;
	int numNodes=0, n = 0;
	while (1)
	{
		numNodes++;
		path2.push_back (temp);
		if (temp.bestAncestorID == -1) break;
		temp = states[temp.bestAncestorID];
	}

	GlDynArray <GPFState> path;
	for (n=0; n<numNodes; n++)
		path.push_back (path2[numNodes-1-n]);
	path2.clear ();

	int lastSmoothedNode = 0;
	double totalGain = 0;
	for (n=0; n<numNodes-1; n++)
	{
		if(!smoothedPath.size() || smoothedPath.Back() != path[n].pos) 
		{
			//Avoid duplicated points
			smoothedPath.push_back (path[n].pos);
		}

		if (n < lastSmoothedNode) n = lastSmoothedNode;
		for (int n2=numNodes-1; n2>n+1; n2--) // example... 1234 -> 1-3, 1-4, 2-4
		{
			Vec2f nearest;
			double minDistance;
			Box *minBoxID=NULL;
			int minEdgeID;
			Vec2f dir = (path[n2].pos - path[n].pos).getNormalized ();

			bool res = getNearestIntersectionWithBoxes (path[n].pos+dir*TINY_EPSILON, path[n2].pos-dir*EPSILON, -1, nearest, minDistance, &minBoxID, minEdgeID);
			if (!res && minBoxID==NULL) // direct travel is possible (must be shorter)
			{
				double newPrice = path[n].bestCost + (path[n2].pos - path[n].pos).getLength ();
				double oldPrice = path[n2].bestCost - path[n].bestCost;
				double priceGain = newPrice - oldPrice;
				path[n2].bestAncestorID = path[n].ID;
				lastSmoothedNode = n2;

				if(!smoothedPath.size() || smoothedPath.Back() != path[n2].pos) 
				{
					//Avoid duplicated points
					smoothedPath.push_back (path[n2].pos);
				}

				n=n2-1;
				totalGain += priceGain;
				//break;
			}
		}
		//		if (n > lastSmoothedNode)
	}

	if(!smoothedPath.size() || smoothedPath.Back() != path[numNodes-1].pos) 
	{
		//Avoid duplicated points
		smoothedPath.push_back(path[numNodes-1].pos);
	}


	/*temp = path[numNodes-1]; //maks: need this?
	while (1)
	{
	///		smoothedPath.push_back (temp.pos);
	if (temp.bestAncestorID == -1) break;
	temp = states[temp.bestAncestorID];
	}*/

	path.clear ();
}

#ifdef _DEBUG
#include <stdio.h>
void GeoPathFinder::Save(char *fileName)
{
	/*FILE *arq = fopen(fileName, "w");
	if(!arq) return;

	fprintf(arq, "source: %ld %ld\n", (long)pos.x, (long)pos.y);
	fprintf(arq, "destination: %ld %ld\n", (long)destination.x, (long)destination.y);
	fprintf(arq, "boundingBoxSize: %ld %ld\n\n", (long)dimensions.x, (long)dimensions.y);

	fprintf(arq, "numObstacles: %ld\n\n", (long)originalBoxes.size());

	for(int i = 0; i < originalBoxes.size(); i++)
	{
		fprintf(arq, "position: %ld %ld\n", (long)originalBoxes[i].getOrigin().x, (long)originalBoxes[i].getOrigin().y);
		fprintf(arq, "dimensions: %ld %ld\n\n", (long)originalBoxes[i].getDimensions().x, (long)originalBoxes[i].getDimensions().y);
	}

	fclose(arq);*/
}
#endif

///////////////////////////////
//For test only
//#include "glsample.cpp"
//#include "GameApplication.cpp"
///////////////////////////////