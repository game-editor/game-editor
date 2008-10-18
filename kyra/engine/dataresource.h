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

#ifndef KYRA_DATASOURCE_INCLUDED
#define KYRA_DATASOURCE_INCLUDED

#include "kyraresource.h"


/** A data resource stores user data in the dat file. Kyra does
	not use or interact with these resources. 

	When text is loaded into the resource, it will be stored
	as one long newline seperated null terminated string.
	(The newline termination is not effected by 
	encoding or reading OS).

	IMPORTANT: This is unrelated to the KrFontResource, which
	is used to display KrTextBox's. This is a collection of
	user defined text data.

	See also class KrBinaryDataResource.

	See the HTML docs for an example of how to encode user resources.
*/
class KrTextDataResource : public KrResource
{
  public:
	// Create by reading from a .dat file
	KrTextDataResource( U32 size, SDL_RWops* data );
	// Create from file
	KrTextDataResource( const gedString& resourceName );

	virtual ~KrTextDataResource()						{}

	virtual	U32 Type()									{ return KYRATAG_TEXTDATA; }
	virtual const gedString&	TypeName()				{ return textName; }
	virtual KrTextDataResource* ToTextDataResource()	{ return this; }

	/** Get the text as one long null terminated, newline
		separated char string.
	*/
	const char* Text()									{ return text.c_str(); }
	const gedString& TextString()						{ return text; }

	/** Get the text as a group of null terminated strings.
	*/
	void Text( GlDynArray< gedString >* strings );

	// [internal]
#ifndef STAND_ALONE_GAME //maks
	virtual void Save( KrEncoder* );
#endif

	bool LoadTextFile( const char* fname );

  private:
	const static gedString textName;
	gedString text;
};


/** A data resource stores user data in the dat file. Kyra does
	not use or interact with these resources. 

	The binary data resource stores an arbitrary binary block 
	of data.

	See also class KrTextDataResource

	See the HTML docs for an example of how to encode user resources.
*/
class KrBinaryDataResource : public KrResource
{
  public:
	// Create by reading from a .dat file
	KrBinaryDataResource( U32 size, SDL_RWops* data );
	KrBinaryDataResource( const gedString& resourceName );
	virtual ~KrBinaryDataResource()						{ delete [] data; }

	virtual	U32 Type()									{ return KYRATAG_BINARYDATA; }
	virtual const gedString&	TypeName()				{ return binaryName; }
	virtual KrBinaryDataResource* ToBinaryDataResource()	{ return this; }

	/// Get a pointer to the binary data.
	const U8* Data()									{ return data; }
	/// Get the data length
	int Length()									{ return length; }

	// [internal]
#ifndef STAND_ALONE_GAME //maks
	virtual void Save( KrEncoder* );
#endif

	bool LoadFile( const char* fname );

  private:
	const static gedString binaryName;
	U8* data;
	int length;
};

#endif
