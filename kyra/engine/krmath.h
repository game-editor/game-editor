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

#ifndef IO_MATH_INCLUDED
#define IO_MATH_INCLUDED

#include "../util/glutil.h"
#include "../util/gltypes.h"
#include "../util/glfixed.h"


// A point or other 2d object.
struct KrVector2
{
	int x;		///< x
	int y;		///< y

	void operator+=( const KrVector2& rhs )	{ x += rhs.x; y += rhs.y; }
	void Set( int _x = 0, int _y = 0 )		{ x = _x; y = _y; }

	bool operator!=( const KrVector2& other ) const {return x != other.x || y != other.y;} //maks
};


template <class T>
struct KrVector2T
{
	T x;
	T y;

	void operator+=( const KrVector2T& rhs )	{ x += rhs.x; y += rhs.y; }
	void Set( T _x, T _y )						{ x = _x; y = _y; }
};


/**	A matrix class, of sorts. Describes a spacial transformation
	in Kyra: x and y positon, as well as x and y scale. All numbers
	are stored in fixed notation.
*/
class KrMatrix2
{
  public:
	GlFixed x, y; 
	GlFixed xScale, yScale;
	double rotation; //akr
	
	/// Transform this matrix.
	void Composite( const KrMatrix2& other )
	{
		// The if check helps performance a little bit. The ".v" usage
		// makes a surprising difference. The opitimizer is doing less
		// than I expected.
		if (	other.xScale.v == GlFixed_1 
			 && other.yScale.v == GlFixed_1 )
		{
			x.v = other.x.v + x.v;		// bypass the compiler opt
			y.v = other.y.v + y.v;
		}
		else
		{
			x = other.x + other.xScale * x;
			y = other.y + other.yScale * y;
			xScale = other.xScale * xScale;
			yScale = other.yScale * yScale;
   		}
	}

	void Set( GlFixed _x = 0, GlFixed _y = 0, GlFixed _xScale = 1, GlFixed _yScale = 1, double _rotation = 0)
	{
		x.v = _x.v;
		y.v = _y.v;
		xScale.v = _xScale.v;
		yScale.v = _yScale.v;
		rotation = _rotation;
	}

	// return true if there is any scaling term:
	bool IsScaled()	const { return ( xScale.v != GlFixed_1 ) || ( yScale.v != GlFixed_1 ); }

	inline friend bool operator == (const KrMatrix2& a, const KrMatrix2& b)    { return (a.x.v == b.x.v && a.y.v == b.y.v && a.xScale.v == b.xScale.v && a.yScale.v == b.yScale.v && a.rotation == b.rotation ); }
	inline friend bool operator != (const KrMatrix2& a, const KrMatrix2& b)    { return (a.x.v != b.x.v || a.y.v != b.y.v || a.xScale.v != b.xScale.v || a.yScale.v != b.yScale.v || a.rotation == b.rotation ); }
};


/** A rectangle structure.
*/
struct KrRect
{
	int xmin;	///< the lower x (inclusive)
	int ymin;	///< the lower y (inclusive)
	int xmax;	///< the upper x (inclusive)
	int ymax;	///< the upper y (inclusive)

	int Width()	 const 	{ return xmax - xmin + 1; }		///< width of the rectangle
	int Height() const	{ return ymax - ymin + 1; }		///< height of the rectangle
	int Area()   const	{ return ( xmax - xmin + 1 ) * ( ymax - ymin + 1 );	}

	/// Initialize. Convenience function.
	void Set( int _xmin, int _ymin, int _xmax, int _ymax )	{ xmin = _xmin; ymin = _ymin; xmax = _xmax; ymax = _ymax; }

	/// Initialize to an invalid rectangle.
	void SetInvalid()	{ xmin = -1; xmax = -2; ymin = -1; ymax = -2; }

	/// Set all the members to zero.
	void Zero()
	{
		xmin = ymin = xmax = ymax = 0;
	}

	/// Return true if this is potentially a valid rectangle.
	bool IsValid() const
	{
		return ( xmin <= xmax ) && ( ymin <= ymax );
	}

	/** Creates the rectangle from 2 points, which can be 
		in any relationship to each other.
	*/
	void FromPair( int x0, int y0, int x1, int y1 );

	/// Return true if the rectangles intersect.
	bool Intersect( const KrRect& rect ) const
	{
		if (	rect.xmax < xmin
			 || rect.xmin > xmax
			 || rect.ymax < ymin
			 || rect.ymin > ymax )
		{
			return false;
		}
		return true;
	}	

	bool Intersect( const KrVector2& point ) const
	{
		if (	point.x < xmin
			 || point.x > xmax
			 || point.y < ymin
			 || point.y > ymax )
		{
			return false;
		}
		return true;
	}

	bool Intersect( int x, int y ) const
	{
		if (	x < xmin
			 || x > xmax
			 || y < ymin
			 || y > ymax )
		{
			return false;
		}
		return true;
	}

	bool IntersectVertical( const KrRect& rect ) //maks
	{
		if(ymax < rect.ymin || ymin > rect.ymax)
		{
			return false;
		}		

		return true;
	}

	bool IntersectHorizontal( const KrRect& rect ) //maks
	{		
		if(xmax < rect.xmin || xmin > rect.xmax)
		{
			return false;
		}

		return true;
	}

	

	/// Return true if 'rect' is inside this.
	bool HasInside( const KrRect& rect ) const
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
	bool HasInside( const KrVector2& point ) const
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


	/// Merge the rect into this.
	void DoUnion( const KrRect& rect )
	{
		xmin = GlMin( xmin, rect.xmin );
		xmax = GlMax( xmax, rect.xmax );
		ymin = GlMin( ymin, rect.ymin );
		ymax = GlMax( ymax, rect.ymax );
	}

	/// Turn this into the intersection.
	void DoIntersection( const KrRect& rect )
	{
		xmin = GlMax( xmin, rect.xmin );
		xmax = GlMin( xmax, rect.xmax );
		ymin = GlMax( ymin, rect.ymin );
		ymax = GlMin( ymax, rect.ymax );
	}

	/// Clip this to the passed in rectangle. Will become invalid if they don't intersect.
	void DoClip( const KrRect& rect )
	{
		xmin = rect.xmin > xmin ? rect.xmin : xmin;
		xmax = rect.xmax < xmax ? rect.xmax : xmax;
		ymin = rect.ymin > ymin ? rect.ymin : ymin;
		ymax = rect.ymax < ymax ? rect.ymax : ymax;
	}


	/// Scale all coordinates by the given ratios:
	void Scale( GlFixed x, GlFixed y )
	{
		xmin = ( x * xmin ).ToInt();
		ymin = ( y * ymin ).ToInt();
		xmax = ( x * xmax ).ToInt();
		ymax = ( y * ymax ).ToInt();
	}

	// Scale then Translate using the give matrix.
	void ScaleTranslate( const KrMatrix2& matrix )
	{
		Scale( matrix.xScale, matrix.yScale );
		xmin += matrix.x.ToInt();
		xmax += matrix.x.ToInt();
		ymin += matrix.y.ToInt();
		ymax += matrix.y.ToInt();	
	}

	
	void Translate( int dx, int dy ) //maks
	{
		xmin += dx;
		xmax += dx;
		ymin += dy;
		ymax += dy;	
	}

	/// Changes the boundaries
	void EdgeAdd( int i )
	{
		xmin -= i;
		xmax += i;
		ymin -= i;
		ymax += i;
	}

	bool operator==( const KrRect& that ) const { return     ( xmin == that.xmin )
													&& ( xmax == that.xmax )
													&& ( ymin == that.ymin )
													&& ( ymax == that.ymax ); }
	bool operator!=( const KrRect& that ) const { return     ( xmin != that.xmin )
													|| ( xmax != that.xmax )
													|| ( ymin != that.ymin )
													|| ( ymax != that.ymax ); }
};


/* A rectangle structure.
*/
template <class T>
struct KrTRect
{
	T xmin;	///< the lower x (inclusive)
	T ymin;	///< the lower y (inclusive)
	T xmax;	///< the upper x (inclusive)
	T ymax;	///< the upper y (inclusive)

	T Width()  const 	{ return xmax - xmin + 1; }		///< width of the rectangle
	T Height() const	{ return ymax - ymin + 1; }		///< height of the rectangle
	T Area()   const	{ return ( xmax - xmin + 1 ) * ( ymax - ymin + 1 );	}

	/// Initialize. Convenience function.
	void Set( T _xmin, T _ymin, T _xmax, T _ymax )	{ xmin = _xmin; ymin = _ymin; xmax = _xmax; ymax = _ymax; }

	/// Return true if this is potentially a valid rectangle.
	bool IsValid() const
	{
		return ( xmin <= xmax ) && ( ymin <= ymax );
	}

	/// Return true if the rectangles intersect.
	bool Intersect( const KrTRect<T>& rect ) const
	{
		if (	rect.xmax < xmin
			 || rect.xmin > xmax
			 || rect.ymax < ymin
			 || rect.ymin > ymax )
		{
			return false;
		}
		return true;
	}	

	bool Intersect( T x, T y ) const
	{
		if (	x < xmin
			 || x > xmax
			 || y < ymin
			 || y > ymax )
		{
			return false;
		}
		return true;
	}


	/// Return true if 'rect' is inside this.
	bool HasInside( const KrTRect<T>& rect ) const
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

	/// Merge the rect into this.
	void DoUnion( const KrTRect<T>& rect )
	{
		xmin = GlMin( xmin, rect.xmin );
		xmax = GlMax( xmax, rect.xmax );
		ymin = GlMin( ymin, rect.ymin );
		ymax = GlMax( ymax, rect.ymax );
	}

	/// Turn this into the intersection.
	void DoIntersection( const KrTRect<T>& rect )
	{
		xmin = GlMax( xmin, rect.xmin );
		xmax = GlMin( xmax, rect.xmax );
		ymin = GlMax( ymin, rect.ymin );
		ymax = GlMin( ymax, rect.ymax );
	}
};


#endif
