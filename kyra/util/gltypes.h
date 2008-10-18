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

#ifndef GL_TYPES_INCLUDED
#define GL_TYPES_INCLUDED

// #if defined( _SDL_H )
	#include "SDL_types.h"

	typedef Uint8			U8;
	typedef Sint8		    S8;
	typedef Uint16			U16;
	typedef Sint16			S16;
	typedef Uint32			U32;
	typedef Sint32			S32;

	#ifdef SDL_HAS_64BIT_TYPE
		typedef SDL_HAS_64BIT_TYPE				S64;
		typedef unsigned SDL_HAS_64BIT_TYPE		U64;
	#else
		//#error No 64-bit integer. //maks
	#endif

// #elif defined ( GL_INT64 )
// 
// 	typedef unsigned char	U8;
// 	typedef signed char	    S8;
// 	typedef unsigned short	U16;
// 	typedef signed short	S16;
// 	typedef unsigned int	U32;
// 	typedef signed int		S32;
// 
// 	typedef GL_INT64			S64;
// 	typedef unsigned GL_INT64	U64;
// 
// #else
// 	// NOTE: This usually means that the SDL header was not 
// 	//		 included before a Kyra header, or that the standard
// 	//		 types were not defined.
// 	#error Need to get type data for this system.
// #endif


#endif
