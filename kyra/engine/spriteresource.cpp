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
#include "rle.h"
#include "spriteresource.h"
#include "sdlutil.h"
#include "canvasresource.h"
#include "encoder.h"
#include "image.h"
#include "sprite.h"


const gedString KrSpriteResource::spriteName = "Sprite";


KrSpriteResource::KrSpriteResource( U32 size, 
									SDL_RWops* data ) 
{
	
	gedString name;
	ReadString( data, &name );
	U32 id = SDL_ReadLE32( data );
	SetNameAndId( name, id );

	

	#ifdef DEBUG
		GLOUTPUT( "Sprite or Font resource '%s' id=%d\n", name.c_str(), id );
	#endif

	U32 nAction = SDL_ReadLE32( data );
	actionArr.SetCount( nAction );

	

	actionMap   = new GlMap< gedString, KrAction*, GlStringHash >( nAction );
	actionIdMap = new GlMap< U32, KrAction*, GlNumberHash< U32 > >( nAction );

	

	for ( U32 i=0; i<nAction; i++ )
	{
		actionArr[ i ] = new KrAction( data );

		actionMap->Add(   actionArr[i]->Name(), actionArr[i] );
		actionIdMap->Add( actionArr[i]->Id(), actionArr[i] );
	}

	
}


KrSpriteResource::KrSpriteResource( const gedString& _name )
{
	SetNameAndId( _name, 0 );

	actionMap   = new GlMap< gedString, KrAction*, GlStringHash >( 0 );
	actionIdMap = new GlMap< U32, KrAction*, GlNumberHash<U32> >( 0 );
}


KrSpriteResource::~KrSpriteResource()
{
	delete actionMap;
	delete actionIdMap;

	for ( int i=0; i<actionArr.Count(); i++ )
	{
		delete actionArr[i];
	}
//	delete [] actionArr;
}


void KrSpriteResource::Draw(	KrPaintInfo* paintInfo, 
								const gedString& actionname,
								int frame,
								const KrMatrix2& xForm,
								const KrColorTransform& cForm,
								const KrRect& clip,

								int quality,

								int openGLZ )
{
	KrAction* act = 0;
	actionMap->Find( actionname, &act );
	if ( act )
	{
		act->Draw( paintInfo, frame, xForm, cForm, clip, openGLZ );
	}
	/*#ifdef DEBUG
	else
	{
		GLOUTPUT( "WARING KrSpriteResource::Draw failed to find action %s\n", actionname.c_str() );
	}
	#endif*/
}


KrAction* KrSpriteResource::GetAction( const gedString& actionName )
{
	KrAction* ret = 0;
	actionMap->Find( actionName, &ret );
//	GLASSERT( ret );
	return ret;	
}

void KrSpriteResource::RemoveAction( const gedString& actionName ) //maks
{
	Uint32 id;
	for(int i = 0; i < actionArr.Count(); i++)
	{
		KrAction *action = actionArr.Item(i);
		if(action->Name() == actionName)
		{		
			id = action->Id();
			actionArr.Remove(i);
			
			delete action;
			
			actionMap->Remove(actionName);
			actionIdMap->Remove(id);
			break;
		}
	}	
}

KrAction* KrSpriteResource::GetAction( U32 id )
{
	KrAction* ret = 0;
	actionIdMap->Find( id, &ret );
//	GLASSERT( ret );
	
	return ret;
}


void KrSpriteResource::AddAction( KrAction* _action )
{
	actionArr.PushBack( _action );
	actionMap->Add( _action->Name(), actionArr.Back() );
	actionIdMap->Add( _action->Id(), actionArr.Back() );
}












bool KrSpriteResource::IsScaleCached( GlFixed xScale, GlFixed yScale )
{
	// All actions should have the same cache. The first
	// action is used for simplicity.
	if ( actionArr[0] )
		return actionArr[0]->IsScaleCached( xScale, yScale );
	return 0;
}


void KrSpriteResource::FreeScaleCache()
{
	for( int i=0; i<NumActions(); ++i )
	{
		actionArr[i]->FreeScaleCache();
	}
}


KrCollisionMap* KrSpriteResource::GetCollisionMap( KrImage* state/*, int window*/ )
{
	GLASSERT( state && state->ToSprite() );
	KrSprite* sprite = state->ToSprite();
	GLASSERT( sprite->SpriteResource() == this );

	if ( state->XScale( /*window*/ ) != 1 || state->YScale( /*window*/ ) != 1 )
	{
		if ( !IsScaleCached( state->CompositeXForm( /*window*/ ).xScale,
							 state->CompositeXForm( /*window*/ ).yScale ) )
		{
			// For sprites, only cached scales can be used!
			/*#ifdef DEBUG
			GLOUTPUT( "ERROR: Collision only allowed with cahed sprites!\n" );
			GLOUTPUT( "SpriteResource '%s' for Action '%s' Frame %d scale %f, %f\n",
					  ResourceName().c_str(),
					  sprite->GetAction()->Name().c_str(),
					  sprite->Frame(),
					  state->CompositeXForm( window ).xScale.ToDouble(),
					  state->CompositeXForm( window ).xScale.ToDouble() );
			#endif*/
			GLASSERT( 0 );
			return 0;
		}
	}
	KrAction* action = sprite->GetAction();
	return action->GetCollisionMap( state->CompositeXForm( /*window*/ ).xScale,
									state->CompositeXForm( /*window*/ ).yScale,
									sprite->Frame() );
}

void KrSpriteResource::CacheScale( GlFixed xScale, GlFixed yScale )
{
	for( int i=0; i<NumActions(); ++i )
	{
		actionArr[i]->CacheScale( xScale, yScale );
	}
}

#ifndef STAND_ALONE_GAME //maks
void KrSpriteResource::Save( KrEncoder* encoder )
{
	int i;
	bool bCompressedResource = false; //true;
	U32 actionSize = 0;

	//maks: always use per frame compression to take advantage of malloc_gc
	/*if(encoder->getCompressed())
	{
		//Save all actions at same compressed data?
		for(i=0; i < NumActions(); ++i )
		{
			actionSize += actionArr[i]->GetTotalSize();

			if(actionSize > 2*1024*1024)
			{
				//4MB ~ 13 frames (240x320)
				//Compress by frame (load frame on demand)
				bCompressedResource = false;
				break;
			}
		}
	}*/

	encoder->StartTag( KYRATAG_SPRITE, bCompressedResource );

	WriteString( encoder->Stream(), ResourceName() );
	encoder->WriteCached( ResourceName() );

	//maks: don't save unnecessary default icon
	Uint32 n = NumActions();
	/*bool bRemoveIcon = false; //Bug in create actor
	if(n > 2) //icon, text, other animation
	{
		for( int i=0; i < NumActions(); ++i )
		{
			if(actionArr[i]->Name() == "icon")
			{
				n--;
				bRemoveIcon = true;
				break;
			}
		}
	}*/
	
	SDL_WriteLE32( encoder->Stream(), n );

	for(i=0; i < NumActions(); ++i )
	{
		//if(actionArr[i]->Name() != "icon" || !bRemoveIcon)
			actionArr[i]->Save( encoder, !bCompressedResource );
	}

	encoder->EndTag();
}

KrCanvasResource* KrSpriteResource::CreateCanvasResource( const gedString& actionName, int frame, int* hotx, int* hoty )
{
	KrAction* action = 0;
	actionMap->Find( actionName, &action );
	if ( action )
		return action->CreateCanvasResource( frame, hotx, hoty );
	return 0;
}

KrCanvasResource* KrSpriteResource::CreateCanvasResource( U32 id, int frame, int* hotx, int* hoty )
{
	KrAction* action = 0;
	actionIdMap->Find( id, &action );
	if ( action )
		return action->CreateCanvasResource( frame, hotx, hoty );
	return 0;
}



#endif


