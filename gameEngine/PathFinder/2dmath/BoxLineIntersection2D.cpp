/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/




#include "Vector2D.h"
#include "Line2D.h"
#include "Box2D.h"


/*template <typename RealType>
bool getBoxLineIntersection (Box2D <RealType> box, Line2D <RealType> line, Vector2D <RealType> &intersection)
{
	Vector2D <RealType> a, b, c, d;
	Vector2D <RealType> bpos = box.getPosition();
	Vector2D <RealType> bdims = box.getDimensions();
  bdims/=2;
 
  Line2D lines[4];  
	lines[0].setExplicit (bpos - bdims, bpos - Vector2D <RealType> (-bdims.x, bdims.y));
	lines[1].setExplicit (bpos - Vector2D <RealType> (-bdims.x, bdims.y), bpos + bdims);
	lines[2].setExplicit (bpos + bdims, bpos + Vector2D <RealType> (-bdims.x, bdims.y));
	lines[3].setExplicit (bpos + Vector2D <RealType> (-bdims.x, bdims.y), bpos - bdims);

	Vector2D <RealType> is[4]; 
	bool isFound[4];
	for (int i=0; i<4; i++)
	  isFound[i] = getLinesIntersectionPoint (line, lines[i], is[i]);
	  
  int minIs = -1;
  for (int i=0; i<4; i++)
  {
  	if (!isFound[i]) continue;
  	RealType dist = (is[i] - line.getStartPoint ()).getLength ();
  	if (minIs == -1 || (minIs != -1 && dist < minDistance)) 
  	{ 
  		minDistance = dist; 
  		minIs = i; 
  	}
  }
  
  if (minIs == -1) return false;
  intersectionPoint = is[minIs];
	
	return true;
}*/
