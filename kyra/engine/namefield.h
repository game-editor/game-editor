/*--License:
	Kyra Sprite Engine
	Copyright Lee Thomason (Grinning Lizard Software) 2001-2002
	www.grinninglizard.com/kyra
	www.sourceforge.net/projects/kyra

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
*/

#ifndef GLNAMEFIELD_INCLUDED
#define GLNAMEFIELD_INCLUDED

#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#include "../../gameEngine/gedString.h"

#include "../util/gltypes.h"
#include "../util/gllist.h"
#include "SDL_endian.h"




// A strange data type. Given names (strings) with dot seperators,
// these are mapped to integers, so that the names can be OR'd together
// with unique values.
//
// for example:
//		woman.walking.ne
//		woman.walking.se
// 
// may get mapped to:
//		woman	1	( 1 << 0 )
//		walking 2	( 1 << 1 )
//		ne		4	( 1 << 2 )
//		se		8	( 2 << 2 )


class GlNameField
{
  public:

	GlNameField();
	~GlNameField()		{}

	// Add woman.walking.ne
	void Add( const gedString& sentance );					

	// Adds complete, calculate the values.
	void Calc();												

	// Get a value; can only be called after calc
	bool Get( const gedString& sentance, U32* value ) const;

	// Horrible hack...that needs to be fixed to make this a "Gl" class
	void WriteHeader( FILE* text, const char* prefix );
	
  private:
	enum
	{
		MAX_BUCKET = 32
	};
	
	bool calcComplete;
	int numBucket;
	int bitWidth[ MAX_BUCKET ];		// bit width of a bucket
	int shift[ MAX_BUCKET ];		// shift to get to a bucket

	GlSList< gedString > bucket[ MAX_BUCKET ];
};


class KrCachedWrite
{
  public:

	KrCachedWrite( SDL_RWops* _stream )	{ stream = _stream; }
	~KrCachedWrite()						{}

	// Writes a dummy value to the stream, but remembers the location.
	// Flush will write the data.
	void Write( const gedString& value );

	// Goes back and writes all the values; needs the name field to look them up.
	void Flush();

	// Write all the cached names to the header file. Call after a Flush.
	void WriteHeader( FILE* text, const char* prefix );

  private:
	struct Data
	{
		U32			pos;
		gedString value;

		bool operator==( const Data& rhs )	{ return value == rhs.value && pos == rhs.pos; }
	};
	GlNameField nameField;
	GlSList< Data > list;
	SDL_RWops* stream;
};


#endif

