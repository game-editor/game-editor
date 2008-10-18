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

#include "glbitstream.h"
#include "gldebug.h"


int GlBitStream::BitsNeeded( U32 max )
{
	if ( max == 0 ) return 0;

	int bits = 0;
	while ( max >= U32( 1 << bits ) )
		++bits;

	return bits;
}


GlWriteBitStream::GlWriteBitStream( SDL_RWops* _fp )
{
	fp = _fp;
	accum = 0;
	bitsLeft = 8;
}


GlWriteBitStream::~GlWriteBitStream()
{
	Flush();
}


void GlWriteBitStream::WriteBits( U32 data, int nBitsInData )
{
	if ( nBitsInData == 0 )
		return;

	GLASSERT( U32( 1 << ( nBitsInData ) ) > data );

	while ( nBitsInData )
	{
		if ( nBitsInData <= bitsLeft )
		{
			accum |= data << ( bitsLeft - nBitsInData );
			bitsLeft -= nBitsInData;
			nBitsInData = 0;
			data = 0;
		}
		else
		{
			accum |= data >> ( nBitsInData - bitsLeft );

			nBitsInData -= bitsLeft;
			bitsLeft = 0;

			// Lop off the top of data
			U32 mask = 0xffffffff;
			mask >>= ( 32 - nBitsInData );
			data &= mask;
		}

		if ( bitsLeft == 0 )
		{
			//fputc( (int) accum, fp );
			SDL_RWwrite( fp, &accum, 1, 1 );
			accum = 0;
			bitsLeft = 8;
		}
	}
	GLASSERT( data == 0 );
}


void GlWriteBitStream::Flush()
{
	if ( bitsLeft != 8 )
	{
		//fputc( (int) accum, fp );
		SDL_RWwrite( fp, &accum, 1, 1 );
		accum = 0;
		bitsLeft = 8;
	}
}


GlReadBitStream::GlReadBitStream( SDL_RWops* _fp )
{
	fp = _fp;

	bitsLeft = 0;
	accum = 0;
}


U32 GlReadBitStream::ReadBits( int nBitsInData )
{
	if ( nBitsInData == 0 )
		return 0;

	U32 val = 0;

#ifndef STAND_ALONE_GAME //maks

	while ( nBitsInData )
	{
		if ( bitsLeft == 0 )
		{
			bitsLeft = 8;
			//accum = fgetc( fp );
			SDL_RWread( fp, &accum, 1, 1 );
		}

		if ( nBitsInData <= bitsLeft )
		{
			val |= accum >> ( bitsLeft - nBitsInData );

			bitsLeft -= nBitsInData;
			nBitsInData = 0;

			// Trim the accumulator
			U32 mask = 0xff;
			mask >>= ( 8 - bitsLeft ); //don't compile in eVC4
			accum &= mask;

		}
		else
		{
			// There are more bits needed than what is in
			// the accumulator

			val |= accum << ( nBitsInData - bitsLeft );

			nBitsInData -= bitsLeft;
			bitsLeft = 0;
		}
	}
#endif

	return val;
}

void GlReadBitStream::Flush()
{
	bitsLeft = 0;
	accum = 0;
}

