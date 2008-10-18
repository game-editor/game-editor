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
#include "../engine/kyra.h"
#include "../gui/console.h"
#include "../engine/encoder.h"


void EncoderOutput( KrEngine* engine, KrResourceVault* vault )
{
	// Add a background.
	int i, j;
	KrCanvasResource* backgroundRes = new KrCanvasResource( "", 20, 20, false );
	KrRGBA grey0, grey1;
	grey0.Set( 40, 40, 40 );
	grey1.Set( 0, 0, 0 );

	for( i=0; i<20; ++i )
	{
		for( j=0; j<20; ++j )
		{
			KrRGBA color = ( i/10 + j/10 ) & 0x01 ? grey0 : grey1;
			*( backgroundRes->Pixels() + j*20 + i ) = color;
		}
	}
	backgroundRes->Refresh();

	KrImNode* backNode = new KrImNode();
	engine->Tree()->AddNode( engine->Tree()->Root(), backNode );

	for ( i=0; i<engine->ScreenBounds().Width() / backgroundRes->Width() + 1; i++ )
	{
		for( j=0; j<engine->ScreenBounds().Height() / backgroundRes->Height() + 1; j++ )
		{
			KrCanvas* backCanvas = new KrCanvas( backgroundRes );
			engine->Tree()->AddNode( backNode, backCanvas );
			backCanvas->SetZDepth( -1 );
			backCanvas->SetPos( i*backgroundRes->Width(), j*backgroundRes->Height() );
		}
	}



	GlSListIterator< KrResource* > it = vault->GetResourceIterator();
	SDL_WM_SetCaption( "Kyra Encoder: Press spacebar to continue.", 0 );

	KrRGBA color;

	GlSList<KrImage*> imageList;		// Create everything, store here, then position.

	for( it.Begin(); !it.Done(); it.Next() )
	{
		KrResource* res = it.Current();
		if ( res->ToSpriteResource() )
		{
			// Also prints fonts.
			if ( res->ToFontResource() )
				color.Set( 100, 100, 255 );
			else
				color.Set( 255, 100, 100 );

			KrSpriteResource* spriteRes = res->ToSpriteResource();

			for( int actionCount = 0; actionCount < spriteRes->NumActions(); ++actionCount )
			{
				KrAction* action = spriteRes->GetActionByIndex( actionCount );

				for( int frameCount = 0; frameCount < action->NumFrames(); ++frameCount )
				{
					KrSprite* sprite = new KrSprite( spriteRes );
					sprite->SetAction( action->Name() );
					sprite->SetFrame( frameCount );

					KrRect rect;
					sprite->QueryBoundingBox( &rect, 0 );

					// don't add empties:
					if ( rect.IsValid() )
					{
						engine->Tree()->AddNode( engine->Tree()->Root(), sprite );

						// Now add a bounding box.
						KrBoxResource* boxRes = new KrBoxResource(	"", 
																	rect.Width()+2, rect.Height()+2,
																	&color, 1,
																	KrBoxResource::OUTLINE );
						engine->Vault()->AddResource( boxRes );
						KrBox* box = new KrBox( boxRes );
						box->SetPos( -1+rect.xmin, -1+rect.ymin );	// relative to sprite

						engine->Tree()->AddNode( sprite, box );

						// Now add the hotspot.
						if ( rect.xmin != 0 || rect.ymin != 0 )
						{
							KrRGBA hotColor = color;
							hotColor.c.alpha = 120;
							KrBoxResource* hotResX = new KrBoxResource( "",
																	   rect.Width(), 1,
																	   &hotColor, 1,
																	   KrBoxResource::OUTLINE );

							KrBoxResource* hotResY = new KrBoxResource( "",
																	   1, rect.Height(),
																	   &hotColor, 1,
																	   KrBoxResource::OUTLINE );

							engine->Vault()->AddResource( hotResX );
							engine->Vault()->AddResource( hotResY );
							KrBox* hotX = new KrBox( hotResX );
							KrBox* hotY = new KrBox( hotResY );

							hotX->SetPos( rect.xmin, 0 );	// relative to sprite
							hotY->SetPos( 0, rect.ymin );	// relative to sprite

							engine->Tree()->AddNode( sprite, hotX );
							engine->Tree()->AddNode( sprite, hotY );
						}
						
						imageList.PushBack( sprite );
					}
				}
			}
		}
		else if ( res->ToTileResource() )
		{
			color.Set( 100, 255, 100 );

			KrTileResource* tileRes = res->ToTileResource();
			KrTile* tile = new KrTile( tileRes );

			KrRect rect;
			tile->QueryBoundingBox( &rect, 0 );

			engine->Tree()->AddNode( engine->Tree()->Root(), tile );

			// Now add a bounding box.
			KrBoxResource* boxRes = new KrBoxResource(	"", 
														rect.Width()+2, rect.Height()+2,
														&color, 1,
														KrBoxResource::OUTLINE );
			engine->Vault()->AddResource( boxRes );
			KrBox* box = new KrBox( boxRes );
			box->SetPos( -1+rect.xmin, -1+rect.ymin );	// relative to sprite

			engine->Tree()->AddNode( tile, box );
			
			imageList.PushBack( tile );
		}
	}


	GlSListIterator< KrImage* > imageIt( imageList );
	imageIt.Begin();
	
	bool needDraw = true;

	engine->Draw();
	SDL_Event event;

	while( true )
	{
		if ( needDraw && !imageIt.Done() )
		{
			int x = 0;
			int y = 0;
			int maxY = 0;

			// Set everything invisible. Click on visibility of what
			// is on screen.
			GlSListIterator< KrImage* > visIt( imageList );
			for( visIt.Begin(); !visIt.Done(); visIt.Next() )
				visIt.Current()->SetVisible( false );

			while ( !imageIt.Done() )
			{
				KrImage* im = imageIt.Current();
				KrRect rect;
				im->QueryBoundingBox( &rect, 0 );

				// Will we exceed the current line?
				if (    x != 0		// if at left edge...oh well.
					 && x + rect.Width() + 2 >= engine->ScreenBounds().xmax )
				{
					x = 0;
					y += maxY + 2;
					maxY = 0;
				} 

				// Will we go off the bottom?
				if (	y != 0
					 && y + rect.Height() + 2 >= engine->ScreenBounds().ymax )
				{
					y = 0;
					x = 0;
					maxY = 0;
					break;			// Don't process this image. Off to next.
				}

				im->SetPos( x-rect.xmin + 1,		// +1: leave room for bounding box.
							y-rect.ymin + 1);
				im->SetVisible( true );

				x += rect.Width() + 2;
				maxY = GlMax( rect.Height(), maxY );
				imageIt.Next();
			}

			needDraw = false;
			engine->Draw();
		}

		int haveEvent = SDL_PollEvent( 0 );
		if ( haveEvent )
		{
			SDL_WaitEvent( &event );
			if ( 	event.type == SDL_KEYDOWN
				 && event.key.keysym.sym == SDLK_SPACE )
			{
				needDraw = true;
				if ( imageIt.Done() )
					break;
			} 
			else if ( event.type == SDL_QUIT )
			{
				break;
			}
		}	
	}
	engine->Tree()->DeleteNode( backNode );
	delete backgroundRes;
}
