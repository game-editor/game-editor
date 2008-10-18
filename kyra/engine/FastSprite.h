// FastSprite.h: interface for the FastSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTSPRITE_H__1C0B8989_4784_498D_808A_36DD9C523A5A__INCLUDED_)
#define AFX_FASTSPRITE_H__1C0B8989_4784_498D_808A_36DD9C523A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _KYRA_

#ifdef _KYRA_
class KrRle;
#else
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

	/// Turn this into the intersection.
	void DoIntersection( const KrRect& rect )
	{
		xmin = xmin > rect.xmin?xmin:rect.xmin;
		xmax = xmax < rect.xmax?xmax:rect.xmax;
		ymin = ymin > rect.ymin?ymin:rect.ymin;
		ymax = ymax < rect.ymax?ymax:rect.ymax;
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






	/// Clip this to the passed in rectangle. Will become invalid if they don't intersect.
	void DoClip( const KrRect& rect )
	{
		xmin = rect.xmin > xmin ? rect.xmin : xmin;
		xmax = rect.xmax < xmax ? rect.xmax : xmax;
		ymin = rect.ymin > ymin ? rect.ymin : ymin;
		ymax = rect.ymax < ymax ? rect.ymax : ymax;
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
#endif

class FastSprite  //maks
{
public:

#ifdef _KYRA_
	void Draw(KrRle *rle, int x, int y, const KrRect& clip);
	void Create(KrRle *rle, int _screenWidth);
#endif

	void Draw(unsigned short *screen, int x, int y, const KrRect &clip);
	void Create(unsigned short *src, int w, int h, int scrW, unsigned short trasparentColor);

	FastSprite();
	virtual ~FastSprite();

private:

	void CopyToData(unsigned short *spriteData, unsigned short *segBuf, int &dataIndex, int bufLen, int startOffset);

#ifdef _KYRA_
	SDL_Surface * Convert16(KrRle *rle);
#endif

	unsigned short *spriteData;
	long *spriteLine;
	//unsigned long *spriteOffSet;

	int width, height;
	int screenWidth;
};

typedef unsigned char * (*F_ARM) (unsigned short *);

class ArmSpriteCompiler16
{
public:

	//dst address in r0
	//Uses r3 to store color value
	//Compiler generate code for ARM 32 bits instruction set for 16 bits color buffer

	ArmSpriteCompiler16();
	~ArmSpriteCompiler16();
	
	void Clear();
	
	void Compile(unsigned short *src, int w, int h, int scrW, unsigned short trasparentColor);
	void Draw(unsigned short *screen, int x, int y);
	
	const unsigned long *getCode() {return code;}
	int getCodeSize() {return n;}
	

private:

	void MOV(unsigned short val);
	void STRH_R3_R0();
	void ADD_R0_R0(unsigned short val);
	void RTS();
	unsigned long ror(unsigned long val, char n);
	void decomposit(int val, int &val8, int &rot, int &_or);
	void emit32(unsigned long op);
	void ResizePower2( int _size );

	unsigned long *code;
	int n; //Number of instructions
	int nAlloc; //Size of buffer

	int screenWidth;
	int currentAddress; //For track misalignments
};


#endif // !defined(AFX_FASTSPRITE_H__1C0B8989_4784_498D_808A_36DD9C523A5A__INCLUDED_)
