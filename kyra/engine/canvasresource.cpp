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
#include "canvasresource.h"
#include "canvas.h"
#include "engine.h"

const gedString KrCanvasResource::canvasName = "Canvas";
int KrCanvasResource::canvasId = 0;


KrCanvasResource::KrCanvasResource(	const gedString& _name,
									int width,
									int height,
									int alphaSupport )
{
	pixelBlock.Create( width, height, alphaSupport !=0 );
	++canvasId;
	SetNameAndId( _name, canvasId );

	numClients = 0;
}


KrCanvasResource::~KrCanvasResource()
{
	GLASSERT( numClients == 0 );
	FreeScaleCache();

	for ( int i=0; i<collisionMaps.Count(); ++i )
		delete collisionMaps[i];
}

void KrCanvasResource::SetDimensions(int width, int height) //maks
{
	pixelBlock.SetDimensions(width, height);
}


void KrCanvasResource::Draw(	KrPaintInfo* paintInfo,
								const KrMatrix2& matrix,
								const KrColorTransform& cForm,
								const KrRect& clipping,

								int quality,
								int openGLZ )
{
	pixelBlock.Draw(	paintInfo,
						matrix,
						false,
						cForm,
						clipping,

						quality,
						openGLZ );
}


void KrCanvasResource::Refresh()
{
	GlCircleListIterator<KrCanvas*> it( clients );
	for( it.Begin(); !it.Done(); it.Next() )
	{
		it.Current()->Invalidate( /*KR_ALL_WINDOWS*/ );
	}

#ifdef KYRA_SUPPORT_OPENGL
	pixelBlock.LoadNewTexture();
#endif

	// This trashes the collision maps, if alpha channel is used.
	if ( pixelBlock.Alpha() )
	{
		for ( int i=0; i<collisionMaps.Count(); ++i )
			delete collisionMaps[i];
		collisionMaps.SetCount( 0 );
	}
}


void KrCanvasResource::AddCanvas( KrCanvas* canvas )
{
	clients.PushBack( canvas );
	++numClients;
}


void KrCanvasResource::RemoveCanvas( KrCanvas* canvas )
{
	GlCircleListIterator<KrCanvas*> it( clients );

	#ifdef DEBUG	
		// check the list
		int count = 0;
		for( it.Begin(); !it.Done(); it.Next() )
			count++;
		GLASSERT( count == numClients );
	#endif
	--numClients;

	for( it.Begin(); !it.Done(); it.Next() )
	{
		if ( it.Current() == canvas )
		{
			it.Remove();
			return;
		}
	}
	// Remove failed!
	GLASSERT( 0 );
}


bool KrCanvasResource::HitTestTransformed( int x, int y, int hitFlags )
{
	if ( x >=0 && x < Width() && y >=0 && y < Height() )
	{
		KrRGBA* pixel = pixelBlock.Pixels() + x + y * pixelBlock.Width();

		if ( !pixelBlock.Alpha() )
		{
			return true;
		}

		if ( pixel->c.alpha != KrRGBA::KR_TRANSPARENT )
		{
			return true;
		}
	}
	return false;
}


void KrCanvasResource::CalculateBounds( const KrMatrix2& xForm, KrRect* bounds ) const
{
	pixelBlock.CalculateBounds( xForm, bounds );
}


KrCollisionMap* KrCanvasResource::GetCollisionMap( KrImage* state/*, int window*/ )
{
	// First, check for the existence of one that is cached already:
	int i;
	int j;

	for( i=0; i<collisionMaps.Count(); ++i )
	{
		if (	collisionMaps[i]->XScale() == state->XScale( /*window*/ )
			 && collisionMaps[i]->YScale() == state->YScale( /*window*/ ) )
		{
			// Match found.
			return collisionMaps[i];
		}
	}

	KrCollisionMap* map = 0;

	if ( pixelBlock.Alpha() )
	{
		KrRGBA *pixel	= 0;
		KrRGBA *memory  = 0;
		int     cx = 0, cy = 0;

		// Handle scaling. If not scale, then easy. Else generate the fast rendered version.
		if ( state->XScale( /*window*/ ) == 1 && state->YScale( /*window*/ ) == 1 )
		{
			pixel = pixelBlock.Pixels();
			cx = pixelBlock.Width();
			cy = pixelBlock.Height();
		}
		else
		{
			KrMatrix2 xForm;
			xForm.Set( 0, 0, state->XScale( /*window*/ ), state->YScale( /*window*/ ) );
			KrRect bounds;
			CalculateBounds( xForm, &bounds );
			KrColorTransform cForm;

			memory = new KrRGBA[ bounds.Width() * bounds.Height() ];
			memset( memory, 0, bounds.Width() * bounds.Height() * sizeof( KrRGBA ) );

			KrPaintInfo info( memory, bounds.Width(), bounds.Height() );
			KrRect clip;
			clip.Set( 0, 0, bounds.Width() - 1, bounds.Height() - 1 );

			pixelBlock.DrawScaledFast( &info, xForm, cForm, clip, false );

			pixel = memory;
			cx = bounds.Width();
			cy = bounds.Height();
		}


		map = new KrCollisionMap( state->XScale( /*window*/ ), state->YScale( /*window*/ ), cx, cy );		// For alpha, we need to check each pixel to see if it is turned

		// on in the map or not.
		for( j=0; j<cy; ++j )
		{
			GlHighBitWriter<U32> writer( map->GetRow( j ) );
			KrRGBA* row = pixel + cx * j;

			for( i=0; i< cx; ++i )
			{
				if ( row[i].c.alpha == KrRGBA::KR_TRANSPARENT )
					writer.Skip();
				else
					writer.Push_1();
			}
		}
		delete [] memory;	// clean up the tempory buffer, if we used one.
	}
	else
	{
		// For no alpha, every pixel is turned on.
		KrMatrix2 xForm;
		xForm.Set( 0, 0, state->XScale( /*window*/ ), state->YScale( /*window*/ ) );
		KrRect bounds;
		CalculateBounds( xForm, &bounds );
		map = new KrCollisionMap( state->XScale( /*window*/ ), state->YScale( /*window*/ ), bounds.Width(), bounds.Height() );

		for( j=0; j<bounds.Height(); ++j )
		{
			GlHighBitWriter<U32> writer( map->GetRow( j ) );
			writer.Push_1N( bounds.Width() );
		}
	}
	collisionMaps.PushBack( map );
	return map;
}

void KrCanvasResource::Load(SDL_RWops *src)
{
	pixelBlock.Read(src);
}

void KrCanvasResource::SaveBuffer(int iBuffer) //maks
{
	pixelBlock.SaveBuffer(iBuffer);
}

void KrCanvasResource::RestoreBuffer(int iBuffer) //maks
{
	pixelBlock.RestoreBuffer(iBuffer);
}

#ifndef STAND_ALONE_GAME //maks
void KrCanvasResource::Save(SDL_RWops *src)
{
	pixelBlock.Write(src);
}



#endif


