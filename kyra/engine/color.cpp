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


#include "color.h"
#include "../util/gldebug.h"
#include "../util/glutil.h"
#include "kyrabuild.h"

#include <stdlib.h>


void KrRGBA::FromString( const char* str )
{
	char* stop;
	char buf[ 4 ];

	all = 0;
// 	red = 0;
// 	green = 0;
// 	blue = 0;
// 	alpha = 0;

	if ( *str == '#' )
		str++;

	buf[0] = *(str+0);
	buf[1] = *(str+1);
	buf[2] = 0;
	c.red = (U8) strtoul( buf, &stop, 16 );

	buf[0] = *(str+2);
	buf[1] = *(str+3);
	buf[2] = 0;
	c.green = (U8) strtoul( buf, &stop, 16 );

	buf[0] = *(str+4);
	buf[1] = *(str+5);
	buf[2] = 0;
	c.blue = (U8) strtoul( buf, &stop, 16 );

	if ( *(str+6) )
	{
		buf[0] = *(str+0);
		buf[1] = *(str+1);
		buf[2] = 0;
		c.alpha = (U8) strtoul( buf, &stop, 16 );
	}
}



void KrColorTransform::Set( U8 _mRed, U8 _bRed, U8 _mGreen, U8 _bGreen, U8 _mBlue, U8 _bBlue, U8 _alpha )
{
	GLASSERT( GlInRange( _mRed,   (U8) 0, (U8) 255 ) );
	GLASSERT( GlInRange( _mGreen, (U8) 0, (U8) 255 ) );
	GLASSERT( GlInRange( _mBlue,  (U8) 0, (U8) 255 ) );
	GLASSERT( GlInRange( _alpha,  (U8) 0, (U8) 255 ) );

	GLASSERT( GlInRange( _bRed,   (U8) 0, (U8) 255 ) );
	GLASSERT( GlInRange( _bGreen, (U8) 0, (U8) 255 ) );
	GLASSERT( GlInRange( _bBlue,  (U8) 0, (U8) 255 ) );

	m.Set( _mRed, _mGreen, _mBlue, 0 );			// m.c.alpha always 0!
	b.Set( _bRed, _bGreen, _bBlue, _alpha );

	CalcState();
}


void KrColorTransform::CalcState()
{
 	if (    m.c.red == 255 && m.c.green == 255 && m.c.blue == 255 
	     && b.c.alpha == 255
 		 && b.c.red == 0 && b.c.green == 0 && b.c.blue == 0 )
	{
		identity = true;
		hasAlpha = false;
		hasColor = false;
	}
	else
	{
		identity = false;
		if ( b.c.alpha == 255 )
			hasAlpha = false;
		else
			hasAlpha = true;

		if (    m.c.red == 255 && m.c.green == 255 && m.c.blue == 255
			 && b.c.red == 0   && b.c.green == 0   && b.c.blue == 0   )
			hasColor = false;
		else
			hasColor = true;
	}		

	#ifdef DEBUG
		// Check for a bounded transform.
		int r1 = (( 255 * m.c.red   )>>8 ) + b.c.red;
		int g1 = (( 255 * m.c.green )>>8 ) + b.c.green;
		int b1 = (( 255 * m.c.blue  )>>8 ) + b.c.blue;
		GLASSERT( GlInRange( r1, 0, 255 ) );
		GLASSERT( GlInRange( g1, 0, 255 ) );
		GLASSERT( GlInRange( b1, 0, 255 ) );
	#endif
	GLASSERT( m.c.alpha == 0 );
}


void KrColorTransform::Composite( const KrColorTransform& color )
{
	GLASSERT( m.c.alpha == 0 );
	KrRGBA oldm = m;
	KrRGBA oldb = b;

	m.c.red   = oldm.c.red   * color.m.c.red   / 255;
	b.c.red   = oldm.c.red   * color.b.c.red   / 255 + b.c.red;
	m.c.green = oldm.c.green * color.m.c.green / 255;
	b.c.green = oldm.c.green * color.b.c.green / 255 + b.c.green;
	m.c.blue  = oldm.c.blue  * color.m.c.blue  / 255;
	b.c.blue  = oldm.c.blue  * color.b.c.blue  / 255 + b.c.blue;

	b.c.alpha = oldb.c.alpha * color.b.c.alpha / 255;

	CalcState();
}


void KrColorTransform::ApplyTransform( KrRGBA* c ) const
{
	for ( int i=KrRGBA::START; i<KrRGBA::END; ++i )
	{
		c->array[i] = (( c->array[i] * m.array[i] ) >> 8 ) + b.array[i];
	}
}

