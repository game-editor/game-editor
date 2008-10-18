
#ifndef __CodeCogs_Line2D_h
#define __CodeCogs_Line2D_h


#include "vector2d.h"


//! Class Line2D represents whole family of lines in two dimensions,
/*! 
\b Line is completely defined by \b two \b points that must lay on that line and is \b not \b limited by those two points. (it comes from infinity and goes to infinity)
\b Ray is completely defined by \b two \b points that must be contained by that Ray and is \b limited by first given point when constructing ray on one end, other side goes to infinity
\b Line \b segment is completely defined by \b two \b points that must lay on that line and is bounded on both sides by those two points

for \b suitability of vast variety of computations, we will keep our lines in parametric type data (eg. keeping position of single point and direction of some "figurative" ray which share same characteristics with other line types)
*/

template <class RealType>
class Line2D //: public Line <RealType>
{
public:
   //! return true if line's direction is degenerated
   inline bool isDegenerated(void)
   {
     if (direction.isZero())
       return true;
     else return false;
   }    

   //! returns true if line contains given point
   inline bool containsInLine (Vector2D <RealType> &point) 
   {
     bool c = (point-start).isCollinearTo (direction);
     if (c) return true;
     else return false;
   }  
   //! returns true if given point lays in our ray or line
   /*! if point lays on ray it automaticaly lays on line) */
   inline bool containsInRayAndLine (Vector2D <RealType> &point) 
   {
     bool c = (point-start).isCollinearTo (direction);
     if (c && direction * (point-start)>0) return true;
     else return false;
   }    
   //! returns true if given point lays in line segment
   /*! if point lays on line segment it automaticaly lays on line and ray) */
   inline bool containsInLineSegment (Vector2D <RealType> &point) 
   {
     bool c = (point-start).isColinearTo (direction);
     if (c) 
     {    
       RealType d = direction.getNormalized() * (point-start);
       if (d < 0) return false;
       else if (d*d > direction.getPoweredLength()) return false;
       else return true;
     }    
     else return false;
   }    
   //! returns line components in implicit form (direction and distance in direction of bisecting line)
   inline void getImplicit(Vector2D <RealType> & direction, RealType & distance)
   {
     Vector2D <RealType> a (-direction.y, direction.x); // rotation of direction for -90
     direction = this->direction;
     a.normalize ();
     distance = start * a;
   }    
   //! returns line components in explicit form (two points)
   inline void getExplicit(Vector2D <RealType> & start, Vector2D <RealType> & end)
   {
     start = this->start;
     end = start + direction;
   }    
   //! returns line componens in parametric form (one point and direction of line)
   inline void getParametric(Vector2D <RealType> & start, Vector2D <RealType> & direction)
   {
     start = this->start;
     direction = this->direction;
   } 
   //! constructs line from implicit form 
   /*! given the direction of line and assuming that second parameter is distance of zero vector from nearest point on line to zero vector, and with positive sign if zero vector is on the right of our line */
   inline void setImplicit(Vector2D <RealType> & direction, RealType distance)
   {
     Vector2D <RealType> v (direction.y, -direction.x); // rotation of direction for 90
     v.normalize(); 
     start = v * (-distance); 
     this->direction = direction;
   }    
   //! constructs line between two given points
   /*! this is natural form of line segment */
   inline void setExplicit(Vector2D <RealType> & start, Vector2D <RealType> & end)
   {
     this->start = start;
     this->direction = end - start;
   }    
   //! constructs line when given single point and direction
   /*! if we want to construct line segment between A and B, first parameter would be A and second would be (B-A) vector */
   inline void setParametric(Vector2D <RealType> & start, Vector2D <RealType> & direction)
   {
     this->start = start;
     this->direction = direction;
   }    
   //! returns normalized line direction vector
   inline Vector2D <RealType> getUnitDirection (void)
   {
     return direction.getNormalized ();
   }    


   //! this method provides construction of line which pass through given point and is perpendicular to given line
   inline void setPerpendicularAndThroughThePoint (Line2D <RealType> &line, Vector2D <RealType> &point)
   {
     Vector2D <RealType> n1 = Vector2D <RealType> (line.direction.y, -line.direction.x);
     n1.normalize ();
     if (n1 * (point - start) > 0) direction = -n1;
     else direction = n1;
     n1 *= abs((point-line.start) * n1);
     start = point;
   }    
   
   //! this method provides contstruction of equidistant line from given two points
   /*! line constructed will be bisecting line passing between two given points */
   inline void setPointsBisectingLine (Vector2D <RealType> &point1, Vector2D <RealType> &point2)
   {
     start = (point2-point1)/2;
     direction = point2 - point1;
     direction.normalize ();
     direction = Vector2D <RealType> (direction.y, -direction.x);
   }    


   //! returns unit vector that is perpendicular to line and goes in direction of given point 
   /*! resulting unit vector is constructed as normalized vector starting at calculated point on line which is nearest to given point */
   inline Vector2D <RealType> getNormalToPoint(Vector2D <RealType> & point)
   {
     Vector2D <RealType> n1 = Vector2D <RealType> (direction.y, -direction.x);
     n1.normalize ();
     if (n1 * (point - start) > 0) return n1;
     else return -n1;
   }    
   
   //! returns true if two lines are parallel, false otherwise
   inline bool isParallelWith (Line2D <RealType> & line)
   {
     if ((direction ^ line.direction) == 0) return true;
     else return false;
   }    
   //! returns true if two lines are perpendicular, false otherwise
   inline bool isPerpendicularWith(Line2D <RealType> & line)
   {
     if (direction * line.direction == 0) return true;
     else return false;
   }    
   
   inline Vector2D <RealType> getStart () { return start; }    
   inline Vector2D <RealType> getEnd () { return start+direction; }    
   inline Vector2D <RealType> getDirection () { return direction; }    
   inline void setStart (Vector2D <RealType> &start) { this->start = start; }    
   inline void setDirection (Vector2D <RealType> &direction) { this->direction = direction; }    
   inline void setEnd (Vector2D <RealType> &end) { direction = end - start; }    


   friend bool getLineSegmentsIntersectionPoint (Line2D <RealType> &line1, Line2D <RealType> &line2, Vector2D <RealType> &intersectionPoint)
   {
//	   intersectionPoint = line1.start;
     Vector2D <RealType> n1 (line1.direction.y, -line1.direction.x);
		 Vector2D <RealType> n2 (line2.direction.y, -line2.direction.x);
     Vector2D <RealType> dir2 = line2.direction.getNormalized ();
     n1.normalize ();
		 n2.normalize ();
     
		 RealType startDistanceToLine1 = (line2.start-line1.start) * n1;
     RealType endDistanceToLine1 = (line2.getEnd ()-line1.start) * n1;
		 if (startDistanceToLine1 > 0 && endDistanceToLine1 > 0) return false;
		 if (startDistanceToLine1 < 0 && endDistanceToLine1 < 0) return false;

		 RealType startDistanceToLine2 = (line1.start-line2.start) * n2;
     RealType endDistanceToLine2 = (line1.getEnd ()-line2.start) * n2;
		 if (startDistanceToLine2 > 0 && endDistanceToLine2 > 0) return false;
		 if (startDistanceToLine2 < 0 && endDistanceToLine2 < 0) return false;

		 float total = fabs (startDistanceToLine2) + fabs (endDistanceToLine2);
		 float lambda = fabs (startDistanceToLine2) / total;
		 intersectionPoint = line1.start + line1.direction * lambda;
		 return true;
   }    


  protected:
   Vector2D <RealType> start;
   Vector2D <RealType> direction;
};


typedef Line2D <float> Line2f;

#endif
