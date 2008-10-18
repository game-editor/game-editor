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


#ifndef IO_UTIL_INCLUDED
#define IO_UTIL_INCLUDED

#include "gltypes.h"


//#ifdef _MSC_VER
//	#define OPTIMIZE_NO_ALIAS_ON optimize ( "w", on )
//#else
//	#define OPTIMIZE_NO_ALIAS_ON 
//#endif
//
//
//#ifdef _MSC_VER
//	#define OPTIMIZE_NO_ALIAS_OFF optimize ( "w", off )
//#else
//	#define OPTIMIZE_NO_ALIAS_OFF 
//#endif


template <class T> inline T		GlMin( T a, T b )		{ return ( a < b ) ? a : b; }
template <class T> inline T		GlMax( T a, T b )		{ return ( a > b ) ? a : b; }
template <class T> inline void	GlSwap( T& a, T& b )	{ T temp; temp = a; a = b; b = temp; }
template <class T> inline bool	GlInRange( T a, T lower, T upper )	{ return a >= lower && a <= upper; }
template <class T> inline T		GlClamp( const T& a, T lower, T upper )	
								{ 
									if ( a < lower )
										return lower;
									else if ( a > upper )
										return upper;
									return a;
								}
template <class T> inline T GlInterpolate( T rangeLow, T rangeHigh, T rangeVal,
										   T low, T high )
{
	return ( ( ( rangeVal - rangeLow ) * ( high - low ) / ( rangeHigh - rangeLow ) ) ) + low;
}


/*	A class to store a set of bit flags, and set and get them in a standard way.
	Class T must be some kind of integer type.
*/
template < class T >
class GlFlag
{
  public:
	GlFlag()							{ store = 0; }

	inline void Set( T flag, bool bSet ) //maks
	{ 
		if(bSet) Set(flag);
		else Clear(flag);
	}

	inline void Set( T flag )			{ store |= flag; }
	inline void Clear( T flag )			{ store &= ~flag; }
	inline bool IsSet( T flag ) const	{ return ( store & flag ) != 0; }

	inline U32  ToU32() const			{ return store; }
	inline U8  ToU8() const			{ return store; } //maks
	
	inline void FromU32( U32 s )		{ store = (T) s; }
	inline void FromU8( U8 s )		{ store = (T) s; } //maks

	inline void ClearAll()				{ store = 0; }

  private:
	T store;
};	

/*	A strange class: it creates bitmaps used for collision detection. Given
	an unsigned something, this is a utility class to pack bits...starting
	with the highest bit.
*/

template < class T >
class GlHighBitWriter 
{
  public:
	enum
	{
		MAXBIT = ( sizeof(T)*8-1 ),
		NUMBIT = ( sizeof(T)*8 ),
		ALLSET = T( -1 )
	};

	GlHighBitWriter( T* _data ) : data( _data ), bitPos( MAXBIT )	{}

	void Skip()
	{
		if ( bitPos == 0 )
		{
			++data;
			bitPos = MAXBIT;
		}
		else
		{
			--bitPos;
		}
	}

	void Skip_N( unsigned n )
	{
		bitPos -= n % NUMBIT;
		if ( bitPos < 0 )
		{
			bitPos += NUMBIT;
			++data;
		}
		data += n / NUMBIT;
	}
	
	void Push_1()	
	{
		*data |= ( 1 << bitPos );
		Skip();
	}

	void Push_1N( unsigned n )
	{
		// Push bits to T boundary
		while( n && bitPos != MAXBIT )
		{
			Push_1();
			--n;
		}

		// Write Full T size
		while( n >= NUMBIT )
		{
			*data = ALLSET;
			++data;
			n -= NUMBIT;
		}

		// Write the remainder
		while ( n )
		{
			Push_1();
			--n;
		}
	}

  private:
	T*	data;
	int bitPos;
};




#endif
