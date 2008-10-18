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

#ifndef GL_RANDOM_INCLUDED
#define GL_RANDOM_INCLUDED

#include "gltypes.h"
#include "gldebug.h"
#include <stdlib.h>

/**	Random number generators (including the rand() in C) are often frustratingly
	non-random in their lower bits. This is a fast random number that is random
	in its lower bits.
*/

class GlRandom
{
  public:
	/// Constructor, with optional seed value.
	GlRandom( U32 _seed = 0 );

	/** The current seed can be set at any time. This does 
		put the random number generator in a consistent state,
		even though the random numbers are actually partially
		cached.
	*/
	void SetSeed( U32 _seed );

	/// Returns a 16bit random number.
	U16 Rand()						{ return Randomize();	}
	/** Returns a random number greater than or equal to 0, and less 
		that 'upperBound'.
	*/	
	U16 Rand( U16 upperBound )		{ return Randomize() % upperBound; }

	/** "Roll two dice." Return a result from [0,upperBound).
		This has the same bell curve distribution as 2 dice.
	*/
	U16 RandD2( U16 upperBound )	{ U16 d1 = upperBound / 2 + 1;
									  U16 d2 = upperBound - d1 + 1;
									  U16 r  = Rand( d1 ) + Rand( d2 );
									  GLASSERT( r < upperBound );
									  return r;
									}

	/** "Roll three dice." Return a result from [0,upperBound).
		This has the same bell curve distribution as 3 dice.
	*/
	U16 RandD3( U16 upperBound )	{ U16 d1 = upperBound / 3 + 2;
									  U16 d2 = upperBound / 3 + 2;
									  U16 d3 = upperBound - d1 - d2 + 2;
									  U16 r = Rand( d1 ) + Rand( d2 ) + Rand( d3 );
									  GLASSERT( r < upperBound );
									  return r;
									}

	/// Return a random number from 0 to upper: [0,upper].
	double DRand( double upper )	{ return upper * double( Randomize() ) / 65535.0; }

	/// Return a random number from 0 to upper: [0,upper].
	float FRand( float upper )		{ return upper * float( Randomize() ) / 65535.0f; }

	/// Return a random boolean.
	bool Boolean()					{ return Rand( 100 ) >= 50; }

  private:
	U32 seed;
	inline void CalcSeed()		{ seed = seed * 39421 + 1; }
	U16 Randomize();

	enum {
		TABLESIZE = 16,
		SLOTWIDTH = 0x10000 / TABLESIZE
	};

	U16 seedTable[ TABLESIZE ];
};


#endif
