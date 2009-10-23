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


#ifndef BOX2D_H_
#define BOX2D_H_

#include "vector2d.h"


template <typename RealType>
class Box2D
{


	Vec2f origin;
	Vec2f dimensions;

public:
	bool dirty;

	Box2D () { dirty=true; }
	Box2D (Vector2D <RealType> &start, Vector2D <RealType> &dims) { dirty=false; origin=start, dimensions=dims; }
	void set (Vector2D <RealType> &start, Vector2D <RealType> &dims) { dirty=false; origin=start; dimensions=dims; }
	void setOrigin (Vector2D <RealType> origin) { dirty=false; this->origin=origin; }
	void setDimensions (Vector2D <RealType> dimensions) { dirty=false; this->dimensions=dimensions; }
	//   void addPoint (Vec2f point);
	Vector2D <RealType> getOrigin (void) const { return origin; }
	Vector2D <RealType> getDimensions (void) const { return dimensions; }



	bool contains (Vector2D <RealType> point)
	{
		if (dirty) return false;
		Vector2D <RealType> delta = point - origin;
		if (fabs(delta.x) > dimensions.x/2) return false;
		if (fabs(delta.y) > dimensions.y/2) return false;

		return true;
	}

	bool contains (Box2D <RealType> &box)
	{
		Vector2D <RealType> p = box.getOrigin ();
		Vector2D <RealType> d = box.getDimensions ()/2;

		if (!contains (p + d)) 
			return false;

		if (!contains (p - d)) 
			return false;

		return true;
	}


	bool intersects(Box2D <RealType> &box)
	{
		Vector2D <RealType> diff = box.getOrigin() - origin;
		Vector2D <RealType> dimshalf = (box.getDimensions () + dimensions)/2;
		if (abs (diff.x) > dimshalf.x) return false;
		if (abs (diff.y) > dimshalf.y) return false;

		return true;
	} 

	void Merge(Box2D <RealType> &box)
	{
		RealType dMax, dMin, left, top;
		Vector2D <RealType> delta = box.origin - origin;

		delta.x = abs(delta.x);
		delta.y = abs(delta.y);

		if(box.origin.x < origin.x) left = box.Left();
		else left = Left();

		if(box.origin.y < origin.y) top = box.Top();
		else top = Top();

		//X
		dMax = (dimensions.x > box.dimensions.x)?dimensions.x:box.dimensions.x;
		dMin = (dimensions.x < box.dimensions.x)?dimensions.x:box.dimensions.x;

		if(delta.x < (dMax - dMin)/2) dimensions.x = dMax;
		else	
		{
			dimensions.x = delta.x + (dMax + dMin)/2;
			origin.x = left + dimensions.x/2;
		}

		//Y
		dMax = (dimensions.y > box.dimensions.y)?dimensions.y:box.dimensions.y;
		dMin = (dimensions.y < box.dimensions.y)?dimensions.y:box.dimensions.y;

		if(delta.y < (dMax - dMin)/2) dimensions.y = dMax;
		else	
		{
			dimensions.y = delta.y + (dMax + dMin)/2;
			origin.y = top + dimensions.y/2;
		}
	}

	RealType Left() {return origin.x - dimensions.x/2;}
	RealType Top() {return origin.y - dimensions.y/2;}
	RealType Right() {return origin.x + dimensions.x/2;}
	RealType Bottom() {return origin.y + dimensions.y/2;}
};/**/

/*enum
{
	RECTANGLE = 0,
	VERT_LINE = 1,
	HORIZ_LINE = 1
};

template <typename RealType>
class Box2D
{
protected:
	RealType xmin;	///< the lower x (inclusive)
	RealType ymin;	///< the lower y (inclusive)
	RealType xmax;	///< the upper x (inclusive)
	RealType ymax;	///< the upper y (inclusive)

	int type;

public:
	bool dirty;

	Box2D () { dirty=true; xmin = ymin = xmax = ymax = 0; type = RECTANGLE;}
	Box2D (Vector2D <RealType> &start, Vector2D <RealType> &dims) 
	{ 
		set (start, dims);
	}

	void set (Vector2D <RealType> &start, Vector2D <RealType> &dims) 
	{ 
		setOrigin(start);
		setDimensions(dims);
	}

	void setOrigin (Vector2D <RealType> &origin) 
	{ 
		RealType w = Width(), h = Height();
		dirty=false; 
		type = RECTANGLE;

		xmin=origin.x; 
		ymin=origin.y;  
		setDimensions (Vector2D <RealType> (w, h));
	}

    void setDimensions (Vector2D <RealType> &dimensions) 
	{ 
		dirty=false; 
		type = RECTANGLE;

		if(dimensions.x > 0.0) xmax=dimensions.x+xmin;//-1; 
		else if(dimensions.x < 0.0) xmax=-dimensions.x+xmin;//-1;
		else
		{
			xmax = xmin; //Can't have zero dimmension
			type = VERT_LINE;
		}

		if(dimensions.y > 0.0)  ymax=dimensions.y+ymin;//-1;
		else if(dimensions.y < 0.0) ymax=-dimensions.y+ymin;//-1;
		else
		{
			ymax = ymin; //Can't have zero dimmension
			type = HORIZ_LINE;
		}
	}

	Vector2D <RealType> getOrigin (void) { return Vector2D<RealType>(xmin, ymin); }
    Vector2D <RealType> getDimensions (void) { return Vector2D<RealType>(Width(), Height()); }

	RealType Width()	 const 	{ return xmax - xmin + 1; }		///< width of the rectangle
	RealType Height() const	{ return ymax - ymin + 1; }		///< height of the rectangle
	RealType Area()   const	{ return ( xmax - xmin + 1 ) * ( ymax - ymin + 1 );	}
	RealType Top() const 	{ return ymin; }
    RealType Left() const 	{ return xmin; }
    RealType Bottom() const 	{ return ymax; }
    RealType Right() const 	{ return xmax; }

	
	
		

	/// Return true if 'rect' is inside this.
	bool contains (Box2D <RealType> &rect) const
	{
		if (	rect.xmin >= xmin
			 && rect.xmax <= xmax
			 && rect.ymin >= ymin
			 && rect.ymax <= ymax )
		{
			return true;
		}
		return false;
	}

	/// Return true if 'rect' is inside this.
	 bool contains (Vector2D <RealType> point) const
	{
		if (	point.x >= xmin
			 && point.x <= xmax
			 && point.y >= ymin
			 && point.y <= ymax )
		{
			return true;
		}
		return false;
	}

	 /// Return true if the rectangles intersect.
	bool intersects(Box2D <RealType> &rect)
	{
		Box2D <RealType> *r1, *r2;

		if(type == RECTANGLE && rect.type == RECTANGLE)
		{
			//Both rectangles
			if ( rect.xmax < xmin
				|| rect.xmin > xmax
				|| rect.ymax < ymin
				|| rect.ymin > ymax )
			{
				return false;
			}

			return true;
		}
		else if(type != RECTANGLE) 
		{
			r1 = this;
			r2 = &rect;
		}
		else
		{
			r1 = &rect;
			r2 = this;
		}

		//Line r1 with rectangle r2

		if(r1->type == VERT_LINE)
		{
			if ( r1->xmin < r2->xmin ||
				 r1->xmin > r2->xmax ||
				 r1->ymin > r2->ymax ||
				 r1->ymax < r2->ymin )
			{
				return false;
			}
		}
		else
		{
			if ( r1->ymin < r2->ymin ||
				 r1->ymin > r2->ymax ||
				 r1->xmin > r2->xmax ||
				 r1->xmax < r2->xmin )
			{
				return false;
			}
		}

		return true;
	}


	/// Merge the rect into this.
	void Merge(Box2D <RealType> &rect)
	{
		xmin = min( xmin, rect.xmin );
		xmax = max( xmax, rect.xmax );
		ymin = min( ymin, rect.ymin );
		ymax = max( ymax, rect.ymax );
	}
};/**/


typedef Box2D <float> Box2f;


#endif /*BOX3D_H_*/
