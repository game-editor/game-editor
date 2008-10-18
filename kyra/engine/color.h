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


#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED

#include "SDL.h"
#include "../util/gltypes.h"

// This is a guess, on what the byte ordering for 32 bit should be:
#if !defined( SDL_BYTEORDER )
	#error Need byte order!
#endif

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	#define KYRA_FLIP_COLORS
#endif

/** The basic color and pixel structure. This may be the greatest
	number of interpretations of the least amount of data.

	Color is one byte each of red, green, blue, and alpha. Sometimes
	its useful to think of these as bytes (the components), sometimes
	as an array, and sometimes as one 32 bit value. This is
	accomplisted, of course, with a union.

	Nothing is guarenteed about the order of
	red, green, blue, and alpha, and #ifdefs may be used
	to change this on various systems. 
	
	In order to access the color that you want via 'array',
	an enumeration is defined.
	
	Addtionally, assuming no system will ever 
	have the alpha in between the other
	colors, you can iterate through 'array' 
	from START to < END to 
	get all the colors without alpha.
	
	@note 
	There is no constructor -- it is oppressive
	in debug mode to have the constructor getting called on 
	huge blocks of memory. Use 'Set' for an initializer.

	@note
	A constructor will screw up memory pool allocation
	in KrRleSegment.
*/
union KrRGBA
{
	enum  
	{
		KR_TRANSPARENT = 0,
		KR_OPAQUE      = 255,
	};

	
	enum {	
			
// It will work regardless of the endian, or whether the colors
// are aligned. But it is much faster if the colors are aligned.
#ifndef KYRA_FLIP_COLORS
		RED,
		GREEN,
		BLUE,
#else
		BLUE,
		GREEN,
		RED,
#endif
		ALPHA,

		START = 0,
		END = 3			// r, g, b...alpha not in this.
	};

	struct 
	{
#ifndef KYRA_FLIP_COLORS
		U8 red;			///< Red component.
		U8 green;		///< Green component.
		U8 blue;		///< Blue component.
#else
		U8 blue;
		U8 green;
		U8 red;
#endif
		U8 alpha;		///< Alpha component.
	} c;

	U8		array[4];
	U32		all;

	KrRGBA() {all = 0;}; //maks
	KrRGBA( U32 value ) {all = value;}; //maks
	KrRGBA( U8 _red, U8 _green, U8 _blue, U8 _alpha = 255 ) {Set(_red, _green, _blue, _alpha);}; //maks

	/// Color accessor, for OpenGL
	float Redf()	const { return float( c.red )   / 255.0f; }
	float Greenf()	const { return float( c.green ) / 255.0f; }
	float Bluef()	const { return float( c.blue )  / 255.0f; }
	float Alphaf()	const { return float( c.alpha ) / 255.0f; }

	/// Set all the members at once - a convenience method.
	void Set( U8 _red, U8 _green, U8 _blue, U8 _alpha = 255 )	
			{	c.red = _red; 
				c.green = _green; 
				c.blue = _blue; 
				c.alpha = _alpha; 
			}

	bool operator==( const KrRGBA& rhs ) const	{ return ( all == rhs.all ); } //maks
	bool operator!=( const KrRGBA& rhs ) const	{ return ( all != rhs.all ); } //maks

	/** Reads a color in the #rrggbbaa or #rrggbb format.
		Used for parsing the XML files.
	*/
	void FromString( const char* str );
};



/** A class that defines a color transformation. Unlike vectors
	and rectangles, it has many invalid states and is better ecapselated.
	It does have a constructor, and it's private data can not be
	directly accessed.

		c' = m * c / 255  +  b / 255

	where:	
		- c is the original color component, either
		  the red, green, blue, or alpha component.
		- c' is the transformed color component.

	c will always be in the range of 0-255. m and b must be 
	specified so that c' will always be in the range of
	0-255 as well. In other words, this is an unbounded transform.
	If colors out of range occur, strange things will happen.

	Using the "Friendly API" this will never be a problem: values
	will be kept in range for you. When using Set* directly, however,
	be cautious.
*/
class KrColorTransform
{
  public:
	KrColorTransform()	: 	identity( true ),
					  		hasAlpha( false ),
							hasColor( false )
						{
							m.Set( 255, 255, 255, 0 );
							b.Set( 0, 0, 0, 255 );
						}
	// --------- Friendly API ---------- //
	/// Clear all color transformations.
	void SetIdentity()					{ m.Set( 255, 255, 255, 0 );
										  b.Set( 0, 0, 0, 255 );
										}
	void SetAlpha( U8 a )				{ b.c.alpha = a; CalcState(); }		///< Set the alpha. 0 is transparent and 255 is opaque.

 	void TintRed( U8 tint )				{ SetRed( 255 - tint, tint ); }		///< Tint but leave the other channels unchanged.
 	void TintGreen( U8 tint )			{ SetGreen( 255 - tint, tint ); }	///< Tint but leave the other channels unchanged.
 	void TintBlue( U8 tint )			{ SetBlue( 255 - tint, tint ); }	///< Tint but leave the other channels unchanged.
	void TintAlpha( U8 tint )			{ SetAlpha( 255 - tint ); }			///< Only sets alpha, but in inverse: 0 is opaque.

	/// Approximate brightening -- not symmetric with darken. val==0 is no change.
	void Brighten( U8 val )			{ m.c.red   = 255-val; b.c.red   = val; 
									  m.c.green = 255-val; b.c.green = val; 
									  m.c.blue  = 255-val; b.c.blue  = val; 
									  CalcState();
									}

	/// Approximate darkening -- not symmetric with brighten. val==0 is no change.
	void Darken( U8 val  )			{ m.c.red   = 255-val; b.c.red = 0;
									  m.c.green = 255-val; b.c.green = 0;
									  m.c.blue  = 255-val; b.c.blue = 0;
									  CalcState();
									}

	// --------- Advanced API ------- //
	/**	Arbitrary set of the components. Gives
	    lots of control over the color
		transformation. The color is transformed 
		by the equation:

			c' = m * c / 255  +  b / 255

		where:	
			- c is the original color component, either
			  the red, green, blue, or alpha component.
			- c' is the transformed color component.

		c will always be in the range of 0-255. m and b must be 
		specified so that c' will always be in the range of
		0-255 as well. In other words, this is an unbounded transform.
		If colors out of range occur, strange things will happen.

		The "friendly api", above, will always guarentee color
		transformations are correctly bounded.
	*/
	void Set( U8 mRed, U8 bRed, U8 mGreen, U8 bGreen, U8 mBlue, U8 bBlue, U8 alpha );

	/// Arbitrary red set (other channel will not be affected.)
	void SetRed( U8 _m, U8 _b )		{	m.c.red = _m; b.c.red = _b; CalcState(); }
	/// Arbitrary green set (other channel will not be affected.)
	void SetGreen( U8 _m, U8 _b )	{	m.c.green = _m; b.c.green = _b; CalcState(); }
	/// Arbitrary blue set (other channel will not be affected.)
	void SetBlue( U8 _m, U8 _b )	{	m.c.blue = _m; b.c.blue = _b; CalcState(); }

	bool IsIdentity() const	{ return identity; }
	bool HasAlpha()	const	{ return hasAlpha; }
	bool HasColor() const	{ return hasColor; }

	U8   Alpha() const		{ return b.c.alpha; }

	// Apply 'color' to this transformation. Composite assumes 'color' PRE transforms this.
	void Composite( const KrColorTransform& color );

	// Note this transforms color but not alpha
	inline U8 TransformRed( U8 red ) const		{ return (( red*m.c.red ) >> 8 ) + b.c.red; }
	// Note this transforms color but not alpha
	inline U8 TransformGreen( U8 green ) const	{ return (( green*m.c.green ) >> 8 ) + b.c.green; }
	// Note this transforms color but not alpha
	inline U8 TransformBlue( U8 blue ) const	{ return (( blue*m.c.blue ) >> 8 ) + b.c.blue; }

	// Transform the color (all channels) no alpha is used.
	void ApplyTransform( KrRGBA* ) const;

	bool operator==( const KrColorTransform& rhs ) const	{ return  ( m.all == rhs.m.all && b.all == rhs.b.all ); }
	bool operator!=( const KrColorTransform& rhs ) const	{ return !( m.all == rhs.m.all && b.all == rhs.b.all ); }

  private:
	void CalcState();
	
	bool identity;		// no color transform?
	bool hasAlpha;		// alpha transform?
	bool hasColor;		// rgb transform?

  public:
	// These are public so the blit macros can get to them without
	// showing up when the code is profiled. DO NOT change these
	// directly -- use the API above.
	
	// For consistency with loops and types, we use RGBAs here as
	// well. Note this results in an extra alpha: alpha is an 
	// absolute, not slope/intercept like the other components.
	// (At least in Kyra...)
	// So: m.c.alpha must always be 0.
	//     b.c.alpha is the alpha channel used.

	KrRGBA	m,	// multiplication of color (255 == 1.0)
			b;	// addtion of color (0 == no color change)
};



#endif
