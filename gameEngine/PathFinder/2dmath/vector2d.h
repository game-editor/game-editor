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



#ifndef _CodeCogs_Vector2D_h
#define _CodeCogs_Vector2D_h

#include <math.h>

#ifdef _USE_STL_
#include <iostream>
//using std::istream;
//using std::ostream;
using namespace std;
#endif


//! makes use of Vector3D if it's included, ignoring otherwise
template <class RealType> class Vector3D;


template <class RealType>
class Vector2D
{
  public:
   //! components of 2D vector can be accessed using different synonims or through pointer to all of them
   union
   {
     struct { RealType x, y; };
     struct { RealType u, v; };
     struct { RealType a, b; };
     
     RealType xy[2];
     RealType uv[2];
     RealType ab[2];
   };

   //! 2D vector can be initialized either passing X and Y coordinates of 2D vector, where X=0 and Y=0 are defult values if X and/or Y are not specified
   Vector2D (RealType X=0, RealType Y=0) : x(X),y(Y) { }
   //! 2D vector can also be initialized passing a pointer to RealType components (2 of them)
   Vector2D (RealType coords[2])
   { x = coords[0]; y = coords[1]; }    
   
   //! operator = can be used between two 2D vectors, or 2D vector and pointer to two RealType components (describing a 2D vector)
   inline void operator = (RealType coords[2])
   { x = coords[0]; y = coords[1]; }    

   //! getLength returns single value describing distance of Vector from Zero Vector in 2D
   inline double getLength () { return sqrt(x*x+y*y); } 

//! additive translation
/*!
operators += is provided to accumulate 2D vector on the right side of operator on 2D vector on the
left side of operator refered as translation of left side vector by right side vector
\param <RealType>&op2
*/
   inline void operator += (const Vector2D <RealType>&op2) { x+=op2.x, y+=op2.y; }
//! substractive translation
/*!
operators += is provided to accumulate 2D vector on the right side of operator on 2D vector on the
left side of operator refered as translation of left side vector by right side vector
\param <RealType>&op2
*/
   inline void operator -= (const Vector2D <RealType>&op2) { x-=op2.x, y-=op2.y; }

   //! operator *= and /= are provided to modify initial length of vector
   void operator *= (double scalar) { x*=scalar; y*=scalar; } 
   void operator /= (double scalar) { x/=scalar; y/=scalar; }

   //! comparison of two 2D vectors are performed using standard == operators from C/C++
   /*! both components of first vector need to match with their pair in second vector */
   const bool operator == (const Vector2D <RealType>&V) const { return (x == V.x && y == V.y); }
   //! comparison of two 2D vectors are performed using standard != operators from C/C++
   /*! at least one component of first vector needs to be different from it's paired component in second vector */
   const bool operator != (const Vector2D <RealType>&V) const { return !(*this == V); }   
  

//! additive translation
/*!
\image vectoradd.jpg
returns 3D vector as a product of translation of first input vector by second input vector, vector
addition operator known from math
\param <RealType>&v2
*/
   Vector2D <RealType> operator + (const Vector2D <RealType>&v2)
   {
     return Vector2D <RealType> (x+v2.x, y+v2.y);   
   }    
//! substractive translation
/*!
\image vectorsub.jpg
returns 2D vector as a product of translation of first input vector by second input vector, vector
substraction operator known from math
\param <RealType>&v2
*/
   Vector2D <RealType> operator - (const Vector2D <RealType>&v2)
   {
     return Vector2D <RealType> (x-v2.x, y-v2.y);   
   }    

//! invert sign operation (rotation by 180 degrees)
/*!
\b unary \b operator \b - parsed before 2D vector at input, \b inverts his sign keeping orientation so it&#039;s
result is same as: "Zero Vector" \b - "input 3D vector at input"
*/
   Vector2D <RealType> operator - () const
   {
     return Vector2D <RealType>(-x, -y);
   } 
   
   //! returns 2D vector representing input 2D vector stretched by scalar
   /*! returns vector which length was multiplied by scalar value, Zero Vector stays Zero Vector */
   Vector2D <RealType> operator * (double scalar) 
   {
     return Vector2D <RealType>(x*scalar, y*scalar); 
   }    

//! distance of input 2D vector is divided by scalar, Zero Vector stays Zero Vector
/*! Zero Vector stays Zero Vector
can cast an exception if scalar is equal to 0
same operation can be done using * with 1/scalar value */
   Vector2D <RealType> operator / (double scalar)
   {
     return Vector2D <RealType>(x/scalar, y/scalar); 
   }    
   

   //! operator * provides dot-product for two input 2D vectors
   double operator * (const Vector2D <RealType>&v2) { return x*v2.x + y*v2.y; }    
//! operator ^ stands for cross product of two vectors returning Vector3D which is perpendicular to both input 3D vectors
/*!
\image vectorcross.jpg
returning a Z component of vector perpendicular to both input 2D vectorssince it is perpendiclar to
both 2D vectors we know X and Y will be equal to 0 so only Z component is important
\param <RealType>&v2
*/
   RealType operator ^ (const Vector2D <RealType> &v2) 
   {
     return x*v2.y-v2.x*y;
   }    


   //! normalize modifies 2D vector so it's length becomes one, keeping same direction
   void normalize () { *this /= getLength(); };
   //! getNormalized returns a 2D unit vector of same direction as input 2D vector 
   Vector2D <RealType> getNormalized () { Vector2D <RealType> norm (*this); norm.normalize(); return norm; }


   //! returns 2D vector but with components of double format
   Vector2D <double> getDouble () { return Vector2D <double> (x, y); }
   //! returns 2D vector but with components of float format
   Vector2D <float> getFloat () { return Vector2D <float> (x, y); }


#ifdef _USE_STL_
   //! output of input 2D vector on the stream in "X Y" format
   //! stream operations will work with custom data types only if << and >> are overloaded for streams
   friend ostream &operator << (ostream &os, const Vector2D <RealType> &v)
   {
     os << v.x << " " << v.y;
     return os;
   }    
   //! input of 2D vector in "X Y" format
   //! stream operations will work with custom data types only if << and >> are overloaded for streams
   friend istream &operator >> (istream &is, Vector2D <RealType> &v)
   {
     is >> v.x >> v.y;
     return is;
   }
#endif

   //! returns true if two 2D vectors are colinear, false otherwise
   bool isColinearTo(const Vector2D <RealType>& point1)
   {
     RealType d = point1.x*this->y - this->x*point1.y;
     return !d;
   }    
   
   //! return true if this 2D vector is Zero vector in 2D
   bool isZero ()
   {
     if (x==0 && y==0) return true;
     else return false;
   }    
   //! return true if this 2D vector is Zero vector in 2D
   bool isDegenerate ()
   {
     if (x==0 && y==0) return true;
     else return false;
   }    

   //! assigns 3D vector to 2D vector ignoring Z component of 3D vector
   void operator = (const Vector3D <RealType> &op2)
   {
     x = op2.x;
     y = op2.y;
   }    
   
   //! get3D method returns 2D vector converted to 3D vector assuming Z=0
   Vector3D <RealType> get3D ()
   {
     return Vector3D <RealType> (x, y, 0);
   }    

   //! returns a length of the vector on the power of 2 (eg. before using sqrt which is slow)
   inline RealType getPoweredLength () { return x*x+y*y; }
};


//! assuming that most of the people would use 2D vectors in float or double form
//! typedefs offer a hungarian notation naming types
//! Vec2f would be 2D vector with float components
typedef Vector2D <float> Vec2f;
//! Vec2d would be 2D vector with double components
typedef Vector2D <double> Vec2d;
//! Vec2i would be 2D vector with integer components
typedef Vector2D <int> Vec2i;

#endif
