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
#include "SDL_endian.h"
#include "dataresource.h"
#include "sdlutil.h"
#include "encoder.h"

const gedString KrTextDataResource::textName = "TextData";
const gedString KrBinaryDataResource::binaryName = "BinaryData";


KrTextDataResource::KrTextDataResource( U32 size, SDL_RWops* data )
{
	gedString name;
	ReadString( data, &name );
	U32 id = SDL_ReadLE32( data );
	SetNameAndId( name, id );
	
	ReadString( data, &text );	
}


KrTextDataResource::KrTextDataResource( const gedString& resourceName )
{
	SetNameAndId( resourceName, 0 );
}


bool KrTextDataResource::LoadTextFile( const char* fname )
{
	FILE* fp = fopen( fname, "r" );
	text = "";

	if ( fp )
	{
		char buf[ 1024 ];
		while( fgets( buf, 1024, fp ) )
		{
			text += buf;		
		}
		fclose( fp );
		return true;
	}
	return false;
}


void KrTextDataResource::Text( GlDynArray< gedString >* strings )
{
	gedString buffer;

	for( const char* p = text.c_str(); *p; ++p )
	{
		if ( *p == '\n' )
		{
			strings->PushBack( buffer );
			buffer = "";
		}
		else
		{
			buffer += *p;
		}
	}
	if ( buffer.length() > 0 )
		strings->PushBack( buffer );
}

#ifndef STAND_ALONE_GAME //maks
void KrTextDataResource::Save( KrEncoder* encoder )
{
	encoder->StartTag( KYRATAG_TEXTDATA );

	WriteString( encoder->Stream(), ResourceName() );
	encoder->WriteCached( ResourceName() );
	
	WriteString( encoder->Stream(), text );

	encoder->EndTag();
}
#endif

/////////////////////////////////////////////////////////////////

KrBinaryDataResource::KrBinaryDataResource( U32 size, SDL_RWops* stream )
{
	gedString name;
	ReadString( stream, &name );
	U32 id = SDL_ReadLE32( stream );
	SetNameAndId( name, id );
	
	length = SDL_ReadLE32( stream );
	data = new U8[ length ];

	SDL_RWread( stream, data, length, 1 );
}


KrBinaryDataResource::KrBinaryDataResource( const gedString& resourceName )
{
	SetNameAndId( resourceName, 0 );
	length = 0;
	data = 0;
}

#ifndef STAND_ALONE_GAME //maks
void KrBinaryDataResource::Save( KrEncoder* encoder )
{
	encoder->StartTag( KYRATAG_BINARYDATA );

	WriteString( encoder->Stream(), ResourceName() );
	encoder->WriteCached( ResourceName() );
	
	SDL_WriteLE32( encoder->Stream(), length );
	SDL_RWwrite( encoder->Stream(), data, length, 1 );

	encoder->EndTag();
}
#endif


bool KrBinaryDataResource::LoadFile( const char* fname )
{
	if ( !data )
	{
		SDL_RWops* stream = SDL_RWFromFile( fname, "rb" );
		if ( stream )
		{
			SDL_RWseek( stream, 0, SEEK_END );
			length = SDL_RWtell( stream );
			SDL_RWseek( stream, 0, SEEK_SET );

			data = new U8[ length ];
			SDL_RWread( stream, data, length, 1 );
			SDL_RWclose( stream );
			return true;
		}
	}
	return false;
}
