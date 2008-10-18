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

#ifndef GlFixed_MATH_INCLUDED
#define GlFixed_MATH_INCLUDED

#include <limits.h>
#include "gltypes.h"
#include "gldebug.h"

//Decimal precision is:		1/(2 ^ DECIMAL_BITS)
//World size is:		+-  2^(INTEGER_BITS - 1)

#define FP32 S32

#define DECIMAL_BITS 11	//Less then 11 bits couses error on scaled ListPops
#define INTEGER_BITS (32 - DECIMAL_BITS) 
#define INTEGER_MASK (((1 << INTEGER_BITS) - 1) << DECIMAL_BITS) //0xffff0000 for 16.16

#define MAX_KYRA_INT ((1 << (INTEGER_BITS - 1)) - 1)

#define GlFixed_0 		( 0 )		// Help the compliler - use 1<<DECIMAL_BITS instead of a const.
#define GlFixed_1 		( 1<<DECIMAL_BITS )	// Help the compliler - use 1<<DECIMAL_BITS instead of a const.

inline S32		GlIntToFixed( int x )			{ return x << DECIMAL_BITS; }
inline S32		GlDoubleToFixed( double x )		{ return S32( x * (double)(1 << DECIMAL_BITS) + 0.5); }
inline double	GlFixedToDouble( S32 f )		{ return double( f ) / (double)(1 << DECIMAL_BITS); }
inline float	GlFixedToFloat( S32 f )			{ return float( f ) / (float)(1 << DECIMAL_BITS); }
inline int      GlFixedToInt( S32 f )			{ return f >> DECIMAL_BITS; }
inline int		GlFixedToIntRound( S32 f )		{ return ((f + ((1<<(DECIMAL_BITS-1)) >> 4) ) >> DECIMAL_BITS); } //maks: '>> 4' to be compatible with original function
inline int		GlFixedToIntRoundUp( S32 f )	{ return ( f + ((1 << INTEGER_BITS) - 1) ) >> DECIMAL_BITS; }

/*****************************************************************************
Original Kyra functions
*****************************************************************************/
/*inline S32		GlIntToFixed( int x )			{ return x << 16; }
inline S32		GlDoubleToFixed( double x )		{ return S32( x * 65536.0 + 0.5); }
inline double	GlFixedToDouble( S32 f )		{ return double( f ) / 65536.0; }
inline float	GlFixedToFloat( S32 f )			{ return float( f ) / 65536.0f; }
inline int      GlFixedToInt( S32 f )			{ return f >> 16; }
inline int		GlFixedToIntRound( S32 f )		{ return ( f + 0x0800 ) >> 16; }
inline int		GlFixedToIntRoundUp( S32 f )	{ return ( f + 0xffff ) >> 16; }*/


/*****************************************************************************
DATA-TYPE CONVERSION MACROS (From Intel GPP)
*****************************************************************************/
/*#define gppInt_To_Fixed_n(x, n)		(x << n)		
#define gppInt_To_Fixed_16(x)		(x << 16)	
#define gppFixed_n_To_Int(x, n)		(x >> n)	
#define gppFixed_16_To_Int(x)		(x >> 16)

#define gppRound_Fixed_n_To_Int(x,n)    ((x + (1<<(n-1))) >> n)
#define gppRound_Fixed_16_To_Int(x)	((x + 0x8000) >> 16)

#define gppFloat_To_Fixed_n(x, n)       ((I32)(x * (float)(1 << n) + 0.5f))
#define gppFloat_To_Fixed_16(x)         ((I32)(x * (float)(1 << 16) + 0.5f)) 
#define gppFixed_n_To_Float(x, n)	((float)x / (float)(1 << n))	
#define gppFixed_16_To_Float(x)		((float)x / (float)(1 << 16))*/	


inline S32 GlFixedMult( S32 _x, S32 _y ) //maks
{
	#ifdef VALIDATE_DEBUG
		S64 x = _x;
		S64 y = _y;
		S64 ret = ( ( x * y ) >> DECIMAL_BITS );
		GLASSERT( ret <= INT_MAX && ret >= INT_MIN );
		return S32( ret );
	#else
		#ifdef SDL_HAS_64BIT_TYPE
			return (S32) ( ( S64( _x ) * S64( _y ) ) >> DECIMAL_BITS );
		#else
			double x = _x, y = _y, mult = (x*y)/(1 << DECIMAL_BITS);
			S32 ret2 =  (S32) mult;
			return ret2;
		#endif		
	#endif
}


inline S32 GlFixedDiv( S32 _x, S32 _y ) //maks
{
	#ifdef VALIDATE_DEBUG
		S64 x = ( S64( _x ) << DECIMAL_BITS );
		S64 y = _y;

		S64 ret =  x / y;
		GLASSERT( ret <= INT_MAX && ret >= INT_MIN );
		return S32( ret );
	#else
		#ifdef SDL_HAS_64BIT_TYPE
			return S32( ( S64( _x ) << DECIMAL_BITS ) / S64( _y ) );
		#else
			double x = _x, y = _y, div = (x * (1 << DECIMAL_BITS))/y;
			S32 ret2 =  (S32) div;
			return ret2;
		#endif
	#endif
}




/**	A fixed math class. Uses (32-DECIMAL_BITS).DECIMAL_BITS fixed point numbers.
*/
class GlFixed
{
  public:
	GlFixed()									{}

	GlFixed( const GlFixed &x )					{ v = x.v; }
	GlFixed( int x )							{ v = GlIntToFixed( x ); }
	GlFixed( const double x )					{ v = GlDoubleToFixed( x ); }

	#ifdef DEBUG
	void DebugDump()		{ GLOUTPUT( "fixed=%.2f\n", GlFixedToDouble( v ) ); }
	#endif


	// This could be done with overloaded operators, but I dislike overloaded
	// operators for casting because I think they are confusing when they
	// are used. -lee

	int    ToInt()	const					{ return GlFixedToInt( v ); }
	//int    ToIntRoundUp()	const			{ return GlFixedToIntRoundUp( v ); }
	int    ToIntRound() const				{ return GlFixedToIntRound( v ); }
	double ToDouble() const					{ return GlFixedToDouble( v ); }
	float  ToFloat() const					{ return GlFixedToFloat( v ); }

	GlFixed Abs()							{ GlFixed t; ( v < 0 ) ? t.v = -v : t.v = v; return t; }

	GlFixed sqrt()
	{
		if(this <= 0)
			return 0;
		
		GlFixed k;
		k.v = (v + GlFixed_1) >> 1;
		
		for(int l = 0; l < DECIMAL_BITS; l++)
		{
			k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		}

		//Repeat DECIMAL_BITS (don't change velocity)
		/*k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;
		k.v = (k.v + GlFixedDiv(v, k.v)) >> 1;*/
		
		if(k < 0)
			return 0;		
	
		return k;
	}

	GlFixed sin()
    {
		int _flt = 12 - DECIMAL_BITS;
		int PI = 12868 >> _flt;

        GlFixed j, k;
		j.v = GlFixedMult(v, GlFixedDiv(GlIntToFixed(180), PI));
        j.v %= GlIntToFixed(360);

        if(j < 0)
            j.v = GlIntToFixed(360) + j.v;

        k = j;

        if(j.v >= GlIntToFixed(90) && j.v < GlIntToFixed(270))
		{
            k.v = GlIntToFixed(180) - j.v;
		}
        else if(j.v >= GlIntToFixed(270) && j.v < GlIntToFixed(360))
		{
            k.v = -(GlIntToFixed(360) - j.v);
		}

        GlFixed l = k / 90;
        GlFixed i1 = l * l, j1;		
        j1.v = GlFixedMult(GlFixedMult(GlFixedMult(GlFixedMult(-18 >> _flt, i1.v) + (326 >> _flt), i1.v) - (2646 >> _flt), i1.v) + (6434 >> _flt), l.v);

        return j1;
    }

	GlFixed cos()
    {
		int _flt = 12 - DECIMAL_BITS;
		int PI = 12868 >> _flt;

		v = PI / 2 - v;
        return sin();
    }

	GlFixed& operator = (const GlFixed &x)		{ v = x.v;          		return *this; }
	GlFixed& operator = (const int x)			{ v = GlIntToFixed( x );    return *this; }
	GlFixed& operator = (const double x)		{ v = GlDoubleToFixed( x ); return *this; }

	GlFixed& operator +=  (const GlFixed x)		{ v += x.v;          return *this; }
	GlFixed& operator +=  (const int x)			{ v += GlIntToFixed(x);    return *this; }
	GlFixed& operator +=  (const double x)		{ v += GlDoubleToFixed(x);    return *this; }

	GlFixed& operator -=  (const GlFixed x)		{ v -= x.v;          return *this; }
	GlFixed& operator -=  (const int x)			{ v -= GlIntToFixed(x);    return *this; }
	GlFixed& operator -=  (const double x)		{ v -= GlDoubleToFixed(x);    return *this; }

	GlFixed& operator *=  (const GlFixed x)		{ v = GlFixedMult(v, x.v);       return *this; }
	GlFixed& operator *=  (const int x)			{ v *= x;                     return *this; }
	GlFixed& operator *=  (const double x)		{ v = GlDoubleToFixed(GlFixedToDouble(v) * x);  return *this; }

	GlFixed& operator /=  (const GlFixed x)		{ v = GlFixedDiv(v, x.v);						return *this; }
	GlFixed& operator /=  (const int x)			{ v /= x;										return *this; }
	GlFixed& operator /=  (const double x)		{ v = GlFixedDiv( v, GlDoubleToFixed(x) );			return *this; }

	GlFixed& operator <<= (const int x)			{ v <<= x;           return *this; }
	GlFixed& operator >>= (const int x)			{ v >>= x;           return *this; }

	GlFixed& operator ++ ()						{ v += GlFixed_1;    return *this; }
	GlFixed& operator -- ()						{ v -= GlFixed_1;    return *this; }

	GlFixed operator ++ (int)						{ GlFixed t;  t.v = v;   v += GlFixed_1;  return t; }
	GlFixed operator -- (int)						{ GlFixed t;  t.v = v;   v -= GlFixed_1;  return t; }

	// Negation.
	GlFixed operator - () const					{ GlFixed t;  t.v = -v;  return t; }

	inline friend GlFixed operator +  (const GlFixed x, const GlFixed y)    { GlFixed t;  t.v = x.v + y.v; return t; }
	inline friend GlFixed operator +  (const int x,	   const GlFixed y)		{ GlFixed t;  t.v = GlIntToFixed(x) + y.v; return t; }
	inline friend GlFixed operator +  (const GlFixed x, const int y      )  { GlFixed t;  t.v = x.v + GlIntToFixed(y); return t; }

	inline friend GlFixed operator -  (const GlFixed x, const GlFixed y)    { GlFixed t;  t.v = x.v - y.v;        return t; }
	inline friend GlFixed operator -  (const int       x, const GlFixed y)    { GlFixed t;  t.v = GlIntToFixed(x) - y.v;        return t; }
	inline friend GlFixed operator -  (const GlFixed x, const int       y)    { GlFixed t;  t.v = x.v - GlIntToFixed(y);        return t; }
	inline friend GlFixed operator -  (const double x,    const GlFixed y)    { GlFixed t;  t.v = GlDoubleToFixed(x) - y.v;     return t; }
	inline friend GlFixed operator -  (const GlFixed x, const double    y)    { GlFixed t;  t.v = x.v - GlDoubleToFixed(y);     return t; }

	inline friend GlFixed operator *  (const GlFixed x, const GlFixed y)    { GlFixed t;  t.v = GlFixedMult(x.v, y.v);        return t; }
	inline friend GlFixed operator *  (const int x,       const GlFixed y)    { GlFixed t;  t.v = GlFixedMult(GlIntToFixed(x), y.v);        return t; }
	inline friend GlFixed operator *  (const GlFixed x, const int y)			 { GlFixed t;  t.v = GlFixedMult(x.v, GlIntToFixed(y));		return t; }
	inline friend GlFixed operator *  (const double x,    const GlFixed y)    { GlFixed t;  t.v = GlFixedMult(GlDoubleToFixed(x), y.v);        return t; }
	inline friend GlFixed operator *  (const GlFixed x, const double y)		 { GlFixed t;  t.v = GlFixedMult(x.v, GlDoubleToFixed(y));		return t; }

	inline friend GlFixed operator /  (const GlFixed x, const GlFixed y)    { GlFixed t;  t.v = GlFixedDiv(x.v, y.v);           return t; }
	inline friend GlFixed operator /  (const int       x, const GlFixed y)    { GlFixed t;  t.v = GlFixedDiv(GlIntToFixed(x), y.v);           return t; }
	inline friend GlFixed operator /  (const GlFixed x, const int       y)    { GlFixed t;  t.v = GlFixedDiv(x.v, GlIntToFixed(y));           return t; }

	inline friend GlFixed operator << (const GlFixed x, const int y)    { GlFixed t;  t.v = x.v << y;    return t; }
	inline friend GlFixed operator >> (const GlFixed x, const int y)    { GlFixed t;  t.v = x.v >> y;    return t; }

	inline friend bool operator == (const GlFixed x, const GlFixed y)    { return (x.v == y.v);       }
	inline friend bool operator == (const int x,       const GlFixed y)    { return (GlIntToFixed(x) == y.v);       }
	inline friend bool operator == (const GlFixed x, const int y)			{ return (x.v == GlIntToFixed(y));       }

	inline friend bool operator != (const GlFixed x, const GlFixed y)    { return (x.v != y.v);       }
	inline friend bool operator != (const int x,		  const GlFixed y)    { return (GlIntToFixed(x) != y.v);       }
	inline friend bool operator != (const GlFixed x, const int y)			{ return (x.v !=GlIntToFixed(y));       }

	inline friend bool operator <  (const GlFixed x, const GlFixed y)   { return (x.v < y.v);        }
	inline friend bool operator <  (const int x,	 const GlFixed y)   { return (GlIntToFixed(x) < y.v);        }
	inline friend bool operator <  (const GlFixed x, const int y)		{ return (x.v < GlIntToFixed(y) );        }

	inline friend bool operator >  (const GlFixed x, const GlFixed y)   { return (x.v > y.v);        }
	inline friend bool operator >  (const int x,	 const GlFixed y)   { return (GlIntToFixed(x) > y.v);        }
	inline friend bool operator >  (const GlFixed x, const int y)		{ return (x.v > GlIntToFixed(y));        }

	inline friend bool operator <= (const GlFixed x, const GlFixed y)    { return (x.v <= y.v);       }
	inline friend bool operator <= (const int x,		  const GlFixed y)    { return (GlIntToFixed(x) <= y.v);       }
	inline friend bool operator <= (const GlFixed x, const int y)			{ return (x.v <= GlIntToFixed(y));       }

	inline friend bool operator >= (const GlFixed x, const GlFixed y)    { return (x.v >= y.v);       }
	inline friend bool operator >= (const int x,		  const GlFixed y)    { return (GlIntToFixed(x) >= y.v);       }
	inline friend bool operator >= (const GlFixed x, const int y)			{ return (x.v >= GlIntToFixed(y));       }

    S32 v;
};



#endif
