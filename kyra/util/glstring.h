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

#ifndef KYRA_STRING16_INCLUDED
#define KYRA_STRING16_INCLUDED

// Disable the no-exception handling warning.
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )

#include "gltypes.h"
#include "gldynarray.h"
#include "../../gameEngine/gedString.h"


class GlString
{
  public:

	static bool IsSpace( char p, const char* delimiter, bool useIsSpace );

	static const char* SkipWhiteSpace(	const char* p,
										const char* delimiter,
										bool useIsSpace );

	static bool IEqual( const gedString& s1, const gedString& s2 );
	static void AppendInt( gedString* s, int i );

	// Removes all white space in the given string.
	static void RemoveWhiteSpace( gedString* s );

	/* Creates an array of strings by splitting 'this' by
		the specified delimeters. 'this' will remain unchanged.
		The returned DynArray will need to be delete'd

		@param delimeter	An array of characters, any of which
							signals a split point.
		@param useIsSpace	Use the ctype 'isspace' call to determine
							if a character is a delimeter.
	*/
	static void Split(	GlDynArray<gedString>* output,
						const gedString& input, 
						const char* delimiter, 
						bool useIsSpace );

	/* Assuming 'this' is a filename or url, changes (or adds)
		the extension. The extension can be any number of letters,
		and should be passed in without the leading period.
	*/
	static void SetExtension( gedString*, const char* extension );

  private:
	static const char* ReadWord(	const char* p, 
									gedString* word, 
									const char* delimiter,
									bool useIsSpace );
};

inline bool StrEqual( const char* s1, const char* s2 )
{
	return ( s1 && s2 && strcmp( s1, s2 ) == 0 );
}

#endif
