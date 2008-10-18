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

#ifdef COUNT_PERFORMANCE //maks

#ifdef _WIN32
	#include <windows.h>
#endif
#include <stdio.h>
#include "SDL.h"
#include "gldebug.h"
#include "glperformance.h"

GlPerformance::PerfData GlPerformance::map[ MAX_DATA ];
int GlPerformance::numMap = 0;


GlPerformance::GlPerformance( const char* measure ) 
	: name( measure )
{
	// Get our name out of the map, create if it doesn't exist.
	data = 0;
	for( int i=0; i<numMap; ++i )
	{
		if ( map[i].name == name )
		{
			data = &map[i];
		}
	}
	if ( !data )
	{
		GLASSERT( numMap < MAX_DATA );
		data = &map[numMap];
		map[numMap].count = 0;
		map[numMap].name  = name;
		map[numMap].totalTime = 0;
		++numMap;
	}
	++data->count;
	#ifdef _WIN32
		QueryPerformanceCounter( (LARGE_INTEGER*) (&start) );
	#else
		start = SDL_GetTicks();
	#endif
}


GlPerformance::~GlPerformance()
{
	#ifdef _WIN32
		QueryPerformanceCounter( (LARGE_INTEGER*) (&end) );
	#else
		end = SDL_GetTicks();
	#endif

	S64 delta = ( end - start );
	GLASSERT( delta >= 0 );
	data->totalTime += delta;
}


void GlPerformance::Dump( FILE* fp )
{
	S64 freq;
	double dfreq;

	#ifdef _WIN32
		QueryPerformanceFrequency( (LARGE_INTEGER*) (&freq) );
	#else
		freq = 1000;
	#endif

	dfreq = double( freq ) / 1000.0;
	double total = 0.0;

	int i;

	for( i=0; i<numMap; ++i )
	{
		total += map[i].totalTime;
	}

	for( i=0; i<numMap; ++i )
	{
		fprintf( fp, "%25s calls=%8d time/call=%10.2f percent=%.1f%%\n",
				map[i].name.c_str(),
				map[i].count,
				double( map[i].totalTime ) / ( double( map[i].count ) * dfreq ),
				100.0 * double( map[i].totalTime ) / total );
	}
}

#endif
