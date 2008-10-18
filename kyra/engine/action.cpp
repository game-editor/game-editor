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


#include "SDL_endian.h"
#include "action.h"
#include "sdlutil.h"
#include "canvasresource.h"
#include "kyrabuild.h"
#include "encoder.h"
#include <stdlib.h> //maks

//#define ALWAYS_CACHE //maks

KrAction::KrAction( SDL_RWops* data )
{	
	ReadString( data, &name );
	id = SDL_ReadLE32( data );
	
	numFrames = SDL_ReadLE32( data );
 	frame     = new KrRle[ numFrames ];

	
	#ifdef DEBUG
	GLOUTPUT( "Action '%s' id=%d numFrames=%d\n", name.c_str(), id, numFrames );
	#endif

	for( int i=0; i < numFrames; i++ )
	{
		frame[ i ].Read( data );
	}

	nFramesH = nFramesV = hotSpotX = hotSpotY = frameWidth = frameHeight = fps = 0;	
}


KrAction::KrAction( const gedString& _name, const gedString& imgName, Sint16 nFramesH, Sint16 nFramesV, Sint16 hotSpotX, Sint16 hotSpotY, Uint16 fps) //maks
{
	name = _name;
	numFrames = 0;
	frame = 0;
	id = 0;

	//maks
	this->imgName	= imgName;
	this->nFramesH	= nFramesH;
	this->nFramesV	= nFramesV;
	this->hotSpotX	= hotSpotX;
	this->hotSpotY	= hotSpotY;
	this->fps = fps;
}

void KrAction::SetAttr( const gedString& _name, const gedString& imgName, Sint16 nFramesH, Sint16 nFramesV, Sint16 hotSpotX, Sint16 hotSpotY, Uint16 fps) //maks
{
	name = _name;
	this->imgName	= imgName;
	this->nFramesH	= nFramesH;
	this->nFramesV	= nFramesV;
	this->hotSpotX	= hotSpotX;
	this->hotSpotY	= hotSpotY;
	this->fps = fps;
}

void KrAction::Save(SDL_RWops *src) //maks
{
	name.Write(src);
	imgName.Write(src);	

	SDL_WriteLE16(src, nFramesH);
	SDL_WriteLE16(src, nFramesV);
	SDL_WriteLE16(src, hotSpotX);
	SDL_WriteLE16(src, hotSpotY);	
	SDL_WriteLE16(src, frameWidth);
	SDL_WriteLE16(src, frameHeight);
	SDL_WriteLE16(src, fps);
}

void KrAction::Load(SDL_RWops *src, Uint32 version)	//maks
{
	name.Read(src);
	imgName.Read(src);
	
	nFramesH = SDL_ReadLE16(src);
	nFramesV = SDL_ReadLE16(src);
	hotSpotX = SDL_ReadLE16(src);
	hotSpotY = SDL_ReadLE16(src);
	frameWidth = SDL_ReadLE16(src);
	frameHeight = SDL_ReadLE16(src);

	if(version > 7) 
	{
		fps = SDL_ReadLE16(src);
	}
	else fps = 30;
}

bool KrAction::CanAnim(int gameFps, double &frameInc) //maks
{
	//Verifies if it can animate (depending game frame rate, and animation frame rate)
	//For animation frame rate <= game frame rate

	bool canAnim = false;

	if(fps == 0)
	{
		fps = 30;
		frameInc = 0.0;
	}

	if(frameInc == 0.0) canAnim = true;
	
	Sint16 sFps = fps;
	if(sFps > 0)
	{
		frameInc += sFps/(double)gameFps;
	}
	else
	{
		frameInc += (-sFps/10000.0)/(double)gameFps;
	}

	if(frameInc >= 1.0) frameInc = 0.0;

	//GLOUTPUT( "******* fps %ld gameFps %ld canAnim: %ld frameInc=%f img: %s\n", fps, gameFps, canAnim,  frameInc, imgName.c_str());
	return canAnim;
}

KrAction::~KrAction()
{
	FreeScaleCache();
	delete [] frame;
}


void KrAction::CalculateBounds( int iframe, const KrMatrix2& matrix, KrRect* bounds )
{
	if(!frame || iframe < 0) 
		return; //maks

	if ( matrix.IsScaled() )
	{
        KrMatrix2 m;

#ifndef ALWAYS_CACHE //maks
		for( int i=0; i<cache.Count(); ++i ) //maks
		{
			if (    cache[i].xScale == matrix.xScale
			     && cache[i].yScale == matrix.yScale )
			{
				m = matrix;
				m.xScale = 1;
				m.yScale = 1;
				cache[i].frame[iframe]->CalculateBounds( m, bounds );
				return;
			}
		}
#else
		CachedBlock *pCache = CacheScale(matrix.xScale, matrix.yScale); //maks
		if(pCache)
		{
			m = matrix;
			m.xScale = 1;
			m.yScale = 1;
			pCache->frame[iframe]->CalculateBounds( m, bounds );
			return;
		}
#endif

		#ifdef ASSERT_IF_NOT_CACHED
			GLASSERT( 0 );
		#endif
	}

	frame[iframe].CalculateBounds( matrix, bounds );
}


void KrAction::Draw(	KrPaintInfo* paintInfo,
						int iframe,
						const KrMatrix2& matrix,
					    const KrColorTransform& cForm,	// color transform to use
						const KrRect& clip,
						int openGLZ )
{
	// This happens if an empty action (often "NONE" gets exported.)
	if ( NumFrames() == 0 ) return;


	if (	matrix.IsScaled() 
		 && !paintInfo->OpenGL() )		// Scaling is trivial for openGL
	{
		KrMatrix2 m;

#ifndef ALWAYS_CACHE //maks
		for( int i=0; i<cache.Count(); ++i ) //maks
		{
			if ( cache[i].xScale == matrix.xScale && cache[i].yScale == matrix.yScale )
			{
				m = matrix;
				m.xScale = 1;
				m.yScale = 1;
				cache[i].frame[iframe]->Draw( paintInfo, m, cForm, clip, openGLZ );
				return;
			}
		}
#else
		CachedBlock *pCache = CacheScale(matrix.xScale, matrix.yScale); //maks
		if(pCache)
		{
			m = matrix;
			m.xScale = 1;
			m.yScale = 1;
			pCache->frame[iframe]->Draw( paintInfo, m, cForm, clip, openGLZ );
			return;
		}
#endif

		#ifdef ASSERT_IF_NOT_CACHED
			GLASSERT( 0 );
		#endif
	}

	frame[iframe].Draw( paintInfo, matrix, cForm, clip, openGLZ );


}


bool KrAction::HitTestTransformed( int iframe, int x, int y, int hitFlags )
{
	if(iframe >=0 && iframe < numFrames )
	{
//#ifndef ALWAYS_CACHE //maks
		return frame[ iframe ].HitTestTransformed( x, y, hitFlags );
/*#else
		CachedBlock *pCache = CacheScale(matrix.xScale, matrix.yScale); //maks
		if(pCache)
		{
			pCache->frame[iframe]->HitTestTransformed( x, y, hitFlags );
			return;
		}
#endif*/
	}
	
	return false;
}





KrCollisionMap* KrAction::GetCollisionMap( GlFixed xScale, GlFixed yScale, int index )
{
	GLASSERT( index >=0 && index<numFrames );

	int i;
	// Check for existing. We have established that this is a scale that exists.
	if ( xScale == 1 && yScale == 1 )
	{
		return frame[index].GetCollisionMap( xScale, yScale );
	}
	else
	{
		// It absolutely should be in the cache. We won't have gotten here if it
		// isn't.

#ifndef ALWAYS_CACHE //maks
		for( i=0; i<cache.Count(); ++i ) //maks
		{
			if ( cache[i].xScale == xScale && cache[i].yScale == yScale )
			{
				return cache[i].frame[index]->GetCollisionMap( xScale, yScale );
			}
		}
#else
		CachedBlock *pCache = CacheScale(xScale, yScale); //maks
		if(pCache)
		{
			return pCache->frame[index]->GetCollisionMap( xScale, yScale );			
		}
#endif
	}
	// Should have been 1:1 or in the cache.
	GLASSERT( 0 );
	return 0;
}




bool KrAction::IsScaleCached( GlFixed xScale, GlFixed yScale )
{
#ifndef ALWAYS_CACHE //maks
	for( int i=0; i<cache.Count(); ++i ) //maks
	{
		if ( cache[i].xScale == xScale && cache[i].yScale == yScale )
		{	
			return true;
		}
	}
#else
	CachedBlock *pCache = CacheScale(xScale, yScale); //maks
	if(pCache)
	{
		return true;
	}
#endif

	return false;
}




void KrAction::FreeScaleCache()
{
	for( int i=0; i<cache.Count(); ++i )
	{
		for ( int j=0; j<NumFrames(); ++j )
		{
			delete cache[i].frame[j];
		}
		delete [] cache[i].frame;
	}
	cache.SetCount( 0 );
}


void KrAction::GrowFrameArray( int newSize )
{
	GLASSERT( newSize > numFrames );

	//maks: invalidate textures to avoid crash in KrTextureManager::RemoveTextures()
	for(int i = 0; i < numFrames; i++) 
	{
		frame[i].RemoveTexture();
	}

	KrRle* newFrame = new KrRle[ newSize ];
	if ( newSize > 0 )
	{
		memset( newFrame, 0, newSize * sizeof( KrRle ) );
	}
	if ( numFrames > 0 )
	{
		memcpy( newFrame, frame, numFrames * sizeof( KrRle ) );
		memset( frame, 0, numFrames * sizeof( KrRle ) );
	}
	delete [] frame;
	frame = newFrame;
	numFrames = newSize;
}

KrAction::CachedBlock *KrAction::CacheScale( GlFixed xScale, GlFixed yScale ) //maks
{
	// check for pre-existing:
	int i;

	for( i=0; i<cache.Count(); ++i )
	{
		if ( cache[i].xScale == xScale && cache[i].yScale == yScale )
		{
			#ifdef DEBUG
				GLOUTPUT( "Scale %f %f already cached in KrAction (used by sprite resource).\n", xScale.ToDouble(), yScale.ToDouble() );
			#endif

			return &cache[i]; //maks
		}
	}

	// Push an empty one on the back:
	CachedBlock block;
	block.xScale = xScale;
	block.yScale = yScale;
	block.frame  = 0;
	cache.PushBack( block );

	CachedBlock* pblock = &cache[ cache.Count() - 1 ];
	pblock->frame = new KrRle*[ NumFrames() ];

	for( i=0; i< NumFrames(); ++i )
	{
		int hotx, hoty;
		pblock->frame[ i ] = frame[i].CreateScaledRle( xScale, yScale, &hotx, &hoty );
	}

	return pblock; //maks
}

KrCanvasResource* KrAction::CreateCanvasResource( int frameIndex, int* hotx, int* hoty )
{
	GLASSERT( frameIndex >= 0 && frameIndex < numFrames );
	return frame[ frameIndex ].CreateCanvasResource( hotx, hoty );
}

#ifndef STAND_ALONE_GAME //maks
void KrAction::Save( KrEncoder* encoder, bool bOnDemandFrameLoad )
{
	WriteString( encoder->Stream(), name );
	encoder->WriteCached( name );
	SDL_WriteLE32( encoder->Stream(), numFrames );

	for ( int j=0; j<NumFrames(); ++j )
	{
		frame[j].Write( encoder->Stream(), bOnDemandFrameLoad );

		U32 a = 0, b = 0, c = 0;
		frame[j].CountComponents( &a, &b, &c );
		encoder->AddCount( a, b, c );
	}
}





U32 KrAction::GetTotalSize() //maks
{
	U32 totalSize = 0;

	for(int i = 0; i < numFrames; i++)
	{
		totalSize += frame[i].GetTotalSize();
	}

	return totalSize;
}

#endif
