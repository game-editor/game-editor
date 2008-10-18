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

#include "SDL.h"
#include "sdlutil.h"
#include "SDL_endian.h"

bool newStyleString = false;

U8 ReadByte( SDL_RWops* stream )
{
	U8 byte;
	SDL_RWread( stream, &byte, 1, 1 );
	return byte;
}

void SetNewReadString(bool newStyle) //maks
{
	newStyleString = newStyle;
}

void ReadString( SDL_RWops* stream, gedString* str ) //maks
{	
	if(newStyleString)
	{
		str->Read(stream);
	}
	else
	{
		//Too slow...
		char c;	
		str->clear(); 

		for( ;; )
		{
			c = ReadByte( stream );
			if ( c )
				(*str) += c;
			else 
				break;
		}
	}
}


void WriteString( SDL_RWops* stream, const gedString& str, bool newStyle ) //maks
{
	if(newStyle)
	{
		str.Write(stream);	
	}
	else
	{
		gedString aux(str);
		const char* buf = aux.c_str();
		SDL_RWwrite( stream, buf, strlen( buf ) + 1, 1 );
	}
}


void WriteByte(  SDL_RWops* stream, U8 byte )
{
	SDL_RWwrite( stream, &byte, 1, 1 );
}


