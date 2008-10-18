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

#include "glstring.h"
#include <stdlib.h>
#include <ctype.h>
#include "string.h"
#include <stdio.h>


void GlString::AppendInt( gedString* s, int i )
{
	char buf[64];

	sprintf( buf, "%d", i );
	(*s) += buf;
}


bool GlString::IEqual( const gedString& s1, const gedString& s2 )
{	
	unsigned i;

	if ( s1.size() == s2.size() )
	{
		for( i=0; i<s1.size(); i++ )
		{
			if ( tolower( s1[i] ) != tolower( s2[i] ) )
				return false;
		}
		return true;
	}
	return false;
}


bool GlString::IsSpace( char p, const char* delimiter, bool useIsSpace )
{
	if ( useIsSpace && isspace( p ) )
		return true;

	const char* white;
	for( white = delimiter; white && *white; white++ )
	{
		if ( p == *white )
			return true;
	}
	return false;
}


const char* GlString::SkipWhiteSpace( const char* p,
									  const char* delimiter,
									  bool useIsSpace )
{
	for ( ;
		  p && *p;
		  p++
		 )
	{
		if ( !IsSpace( *p, delimiter, useIsSpace ) )
			break;
	}

	return p;
}


const char* GlString::ReadWord(	const char* p,
								gedString* word, 
								const char* delimeter,
								bool useIsSpace )
{
	while ( p
			&& *p
			&& !IsSpace( *p, delimeter, useIsSpace )
		  )
	{
		(*word) += *p;
		p++;
	}

	return p;
}

void GlString::Split(	GlDynArray<gedString>* output,
						const gedString& input, 
						const char* delimiter,
						bool useIsSpace )
{
	if ( output )
	{
		if ( input.empty() )
			return;

		//GlDynArray<gedString>* retArray = new GlDynArray<gedString>;
		output->Clear();

//		if ( !retArray )
//			return 0;

		gedString aux(input);
		char* array = new char[ input.size()+1 ];
		memcpy( array, aux.c_str(), input.size() );
		array[ input.size() ] = 0;

		const char* p = &array[0]; 
		
		// parse out the array, rememebering in Linux the strings are
		// NOT null-terminated.
		//
		while ( p && *p )	
		{
			p = SkipWhiteSpace( p, delimiter, useIsSpace );
			if ( p && *p )
			{
				int index = output->Count();
				output->SetCount( index+1 );
				p = ReadWord( p, output->ItemPointer( index ),
							  delimiter, useIsSpace );
			}
		}
		delete [] array;
	}
}


void GlString::SetExtension( gedString* s, const char* extension )
{
	if ( !s->empty() )
	{
		unsigned pos = s->rfind( '.' );

		if ( pos != gedString::npos )
		{	
			s->resize( pos+1 );
		}
		else
		{
			*s += '.';
		}
		*s += extension;
	}
}


void GlString::RemoveWhiteSpace( gedString* s )
{
	gedString str;

	const char* p = s->c_str();

	for( ; p && *p; ++p )
	{
		if ( !isspace( *p ) )
			str += *p;
	}

	*s = str;
}
