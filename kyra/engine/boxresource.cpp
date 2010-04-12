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
#ifdef KYRA_SUPPORT_OPENGL
#ifdef __iPhone__
	#include "SDL_opengles.h"
#else
	#include "SDL_opengl.h"

#endif
#endif
#include "boxresource.h"
#include "box.h"
#include "painter.h"
#include "imagetree.h"
#ifdef __APPLE__
#include "SDL_sysvideo.h"
#endif


const gedString KrBoxResource::boxName = "Box";
int KrBoxResource::boxId = 0;


KrBoxResource::KrBoxResource(	const gedString& _name,
								int _width, 
								int _height, 
								const KrRGBA* _colorArray,
								int numColors,
								int _flags )
{
	++boxId;
	SetNameAndId( _name, boxId );
	width  = _width;
	height = _height;
	boxtype  = _flags;
	sourceAlpha = false;

	int i, j = 0;
	for( i=0; i < COLOR_ARRAY_SIZE; i++ )
	{
		colorArray[i] = _colorArray[j];
		if ( colorArray[i].c.alpha != KrRGBA::KR_OPAQUE )
		{
			sourceAlpha = true;
		}

		++j;
		if ( j == numColors )
		{
			j = 0;
		}
	}
}


void KrBoxResource::CalculateBounds(  const KrMatrix2& matrix,
									  KrRect* bounds ) const
{
	int dx = 0, dy = 0;

	if ( boxtype == CROSSHAIR )
	{
		dx = Width() / 2;
		dy = Height() / 2;
	}

	bounds->xmin = -dx;
	bounds->xmax = -dx + Width() - 1;
	bounds->ymin = -dy;
	bounds->ymax = -dy + Height() - 1;

	// Scaling factor won't change the x and y:
	// just the size.
	bounds->ScaleTranslate( matrix );
}


void KrBoxResource::DrawOpenGL(	KrPaintInfo* paintInfo,
								const KrMatrix2& matrix,
								const KrColorTransform& cForm,
								const KrRect& clipping,
								int openGLZ )
{
	#ifdef KYRA_SUPPORT_OPENGL
	// TODO: It would be good to have color array support. This would need
	//		  to be implemented by a texture.
	KrRect bounds;
	CalculateBounds( matrix, &bounds );

	KrRGBA color = colorArray[0];
	cForm.ApplyTransform( &color );

	SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glColor4f(	color.Redf(), 
									color.Greenf(), 
									color.Bluef(), 
									color.Alphaf() );	
#else
	SDL_CurrentVideo->glColor4x(	GlES_DoubleToFixed(color.Redf()), 
									GlES_DoubleToFixed(color.Greenf()), 
									GlES_DoubleToFixed(color.Bluef()), 
									GlES_DoubleToFixed(color.Alphaf()) );
#endif

	if ( boxtype == OUTLINE )
	{
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glBegin( GL_LINE_LOOP );
		{
			SDL_CurrentVideo->glVertex3f( bounds.xmin, bounds.ymin, openGLZ );
			SDL_CurrentVideo->glVertex3f( bounds.xmin + bounds.Width(), bounds.ymin, openGLZ );
			SDL_CurrentVideo->glVertex3f( bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height(), openGLZ );
			SDL_CurrentVideo->glVertex3f( bounds.xmin, bounds.ymin + bounds.Height(), openGLZ );
		}
		SDL_CurrentVideo->glEnd();
#else
		GLshort vertices[] = 
		{
			bounds.xmin, bounds.ymin, openGLZ,  
			bounds.xmin + bounds.Width(), bounds.ymin, openGLZ,  
			bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height(), openGLZ, 
			bounds.xmin, bounds.ymin + bounds.Height(), openGLZ						
		};
		
		DrawOGLQuad(GL_LINE_LOOP, vertices, 3);
#endif

		GLASSERT( SDL_CurrentVideo->glGetError() == GL_NO_ERROR );
	}
	else if ( boxtype == CROSSHAIR )
	{
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glBegin( GL_LINES );
		{
			SDL_CurrentVideo->glVertex3i( bounds.xmin + bounds.Width() / 2, bounds.ymin, openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin + bounds.Width() / 2, bounds.ymin + bounds.Height(), openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin,                  bounds.ymin + bounds.Height() / 2, openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height() / 2, openGLZ );
		}
		SDL_CurrentVideo->glEnd();
#else
		GLshort vertices[] = 
		{
			bounds.xmin + bounds.Width() / 2, bounds.ymin, openGLZ,  
			bounds.xmin + bounds.Width() / 2, bounds.ymin + bounds.Height(), openGLZ,  
			bounds.xmin,                  bounds.ymin + bounds.Height() / 2, openGLZ, 
			bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height() / 2, openGLZ						
		};
		
		DrawOGLQuad(GL_LINES, vertices, 3);
#endif
	}
	else if ( boxtype == FILL )
	{
#ifndef GL_OES_VERSION_1_1 //maks
		SDL_CurrentVideo->glBegin( GL_QUADS );
		{
			SDL_CurrentVideo->glVertex3i( bounds.xmin, bounds.ymin, openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin + bounds.Width(), bounds.ymin, openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height(), openGLZ );
			SDL_CurrentVideo->glVertex3i( bounds.xmin, bounds.ymin + bounds.Height(), openGLZ );
		}
		SDL_CurrentVideo->glEnd();
#else
		GLshort vertices[] = 
		{
			bounds.xmin, bounds.ymin, openGLZ,  
			bounds.xmin + bounds.Width(), bounds.ymin, openGLZ,  
			bounds.xmin + bounds.Width(), bounds.ymin + bounds.Height(), openGLZ, 
			bounds.xmin, bounds.ymin + bounds.Height(), openGLZ						
		};
		
		DrawOGLQuad(GL_TRIANGLE_STRIP, vertices, 3);
#endif
	}
	GLASSERT( SDL_CurrentVideo->glGetError() == GL_NO_ERROR );
	#endif
}


void KrBoxResource::Draw(	KrPaintInfo* paintInfo,
							const KrMatrix2& matrix,
							const KrColorTransform& cForm,
							const KrRect& clipping,
							int openGLZ )
{
	if ( paintInfo->OpenGL() )
	{
		#ifdef KYRA_SUPPORT_OPENGL
			DrawOpenGL( paintInfo, matrix, cForm, clipping, openGLZ );
		#else
			GLASSERT( 0 );	
		#endif
		return;
	}

	KrPaintFunc blitter = paintInfo->GetBlitter( sourceAlpha, cForm );

	KrRect isect;
	KrRect bounds;
	CalculateBounds( matrix, &bounds );

	isect = bounds;
	if ( isect.Intersect( clipping ) )
	{
		isect.DoIntersection( clipping );

		int j, xSource, run, remainingWidth, dx;

		int yOffset		= isect.ymin - bounds.ymin;
		int xOffset		= isect.xmin - bounds.xmin;
		int width		= isect.Width();
		int height		= isect.Height();
		
		// measured by the j value, not y
		int drawHLine0	= -1;
		int drawHLine1	= -1;

		// measured by the x value
		int drawVLine0	= -1;
		int drawVLine1	= -1;

		if ( boxtype == OUTLINE )
		{
			if ( isect.xmin == bounds.xmin )
				drawVLine0 = isect.xmin;
			if ( isect.xmax == bounds.xmax )
				drawVLine1 = isect.xmax;
			if ( isect.ymin == bounds.ymin )
				drawHLine0 = 0;
			if ( isect.ymax == bounds.ymax )
				drawHLine1 = height-1;
		}
		else if ( boxtype == CROSSHAIR )
		{
			KrVector2 center;
			center.x = ( bounds.xmin + bounds.xmax ) / 2;
			center.y = ( bounds.ymin + bounds.ymax ) / 2;

			if ( isect.HasInside( center ) )
			{
				drawVLine0 = center.x;
				drawHLine0 = center.y - isect.ymin;
			}
		}

		if ( width > 0 && height > 0 )
		{
			

			for( j=0; j<height; j++ )
			{
				U8* target =   (U8*) paintInfo->pixels 
									 + ( isect.ymin + j ) * paintInfo->pitch
									 + isect.xmin * paintInfo->bytesPerPixel;
				
				// First, the case where the entire line is drawn.
				//		FILL: draw all lines.
				//		always draw top and bottom lines.
				if (    ( boxtype == FILL )
					 || ( j == drawHLine0 )
					 || ( j == drawHLine1 ) )
				{

					// As we draw, remember the source buffer is
					// only COLOR_ARRAY_SIZE pixels wide.
					remainingWidth = width;
					dx = xOffset;

					while ( remainingWidth )
					{
						// Change the source offset if we are striped:
						//	 dx is measured from bounds.
						//	 j  is measured from isect.

						xSource = ( (   ( dx )
									  + ( j + yOffset )
									) & MASK );

						run = GlMin( COLOR_ARRAY_SIZE - xSource, remainingWidth );
						GLASSERT( run > 0 );

						blitter( paintInfo, target,	//maks
								 &colorArray[ xSource ],
								 4,
								 run,
								 cForm );

						remainingWidth -= run;
						dx += run;
						target += run * paintInfo->bytesPerPixel;
					}

					target += paintInfo->pitch;
				}
				else
				{
					// The case where we just draw the left or right edge

					if ( drawVLine0 >= 0 )
					{
						// Change the source offset if we are striped:
						//	 dx is measured from bounds.
						//	 j  is measured from isect.

						xSource = ( ( drawVLine0 + isect.ymin + j ) & MASK );

						blitter( paintInfo, //maks
								 target + ( drawVLine0 - isect.xmin )*paintInfo->bytesPerPixel,
								 &colorArray[ xSource ],
								 4,
								 1,
								 cForm );
					}

					if ( drawVLine1 >= 0 )
					{
						// Change the source offset if we are striped:
						//	 dx is measured from bounds.
						//	 j  is measured from isect.

						xSource = ( ( drawVLine1 + isect.ymin + j ) & MASK );

						blitter( paintInfo, //maks
								 target + ( drawVLine1 - isect.xmin )*paintInfo->bytesPerPixel,
								 &colorArray[ xSource ],
								 4,
								 1,
								 cForm );
					}
				}
			}
		}		
	}
}


bool KrBoxResource::HitTestTransformed( int x, int y, int hitFlags )
{
	// If this is a filled box, or ALWAYS_INSIDE_BOX is set,
	// then it is a simple rectangle intersection, else we
	// check to see if we are on the edge.

	if ( ( boxtype == FILL ) || ( hitFlags & KrImageTree::ALWAYS_INSIDE_BOX ) )
	{
		if ( x >= 0 && x < width && y >=0 && y < height )
		{
			return true;
		}
	}
	else
	{
		if ( x == 0 || x == ( width - 1	) || y == 0 || y == ( height - 1 ) )
		{
			return true;
		}
	}
	return false;
}

