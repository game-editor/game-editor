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


#include "engine.h"
#include "../util/glperformance.h"
#ifdef KYRA_SUPPORT_OPENGL
	#include "SDL_opengl.h"
	#include "ogltexture.h"
	//#pragma comment(lib,"opengl32.lib") //maks
#endif
#include "splash.h"
#include "canvasresource.h"
#include "canvas.h"

#include "../../gameEngine/MuteEx.h" //maks

extern "C" int GetGameMode();

gedString sNONE("NONE"); //maks: avoid mallocs


// Yes, we use 'this' in a member initialization list. Quote from
// Microsoft help: "This is a level-1 warning when Microsoft extensions 
// are enabled (/Ze) and a level-4 warning otherwise." I'm sure that
// made sense to someone. -lee
#pragma warning ( disable : 4355 )

// Check some SDL stuff here:
#if !( SDL_VERSION_ATLEAST(1, 2, 0) )
	#error Kyra requires SDL 1.2.0 or greater.
#endif


/*static*/ int KrEngine::maxOglTextureSize = 0;

KrEngine* engine = NULL; //maks

KrEngine::KrEngine( SDL_Surface* _screen )
	: paintInfo( _screen )
{
	KrRect bounds;
	bounds.Set( 0, 0, _screen->w - 1, _screen->h - 1 );
	vault = NULL; //maks
	tree = NULL; //maks
	::engine = this; //maks

	Init( _screen, /*1,*/ &bounds, 0 );
}

KrEngine::~KrEngine()	//maks						
{	
#ifdef COUNT_PERFORMANCE 
	FILE *fp = fopen("performance.txt", "w");
	if(fp)
	{
		GlPerformance::Dump(fp);
		fclose(fp);
	}
#endif
	
	delete tree;
	// Note that the tree must be deleted before the vault.											
	delete vault; 
}


KrEngine::KrEngine( SDL_Surface* _screen, const KrRect& bounds, const KrRGBA* extra )
	: paintInfo( _screen )
{
	Init( _screen, /*1,*/ &bounds, extra );
}


KrEngine::KrEngine( SDL_Surface* _screen,/* int _nWindows,*/ const KrRect* bounds, const KrRGBA* extra )
	  : paintInfo( _screen )
{
	Init( _screen, /*_nWindows,*/ bounds, extra );
}


void KrEngine::Init(	SDL_Surface* _screen,
						/*int _nWindows, */
						const KrRect* bounds, 
						const KrRGBA* extra )
{
	paintInfo.Init(_screen); //maks
	screen = _screen;
	//nWindows = _nWindows;

#ifndef USE_WALK
	bNewNode = false; //maks
#endif

	splashStart = 0;
	splashVault = 0;
	splash = splashText = 0;

	windowBounds.Set( 0, 0, screen->w-1, screen->h-1 );
	extraBackground.Set( 0, 0, 0, 255 );

	// If this assert is thrown, increase KR_MAX_WINDOWS to an
	// appropriate value and re-compile.
//	GLASSERT( nWindows <= KR_MAX_WINDOWS );

	// fullScreenUpdate draws *outside* of the windows.
	needFullScreenUpdate = ( extra != 0 );

	if ( extra )
	{
		extraBackground = *extra;

		if ( !paintInfo.OpenGL() )
		{
			U32 color = SDL_MapRGB( screen->format, extra->c.red, extra->c.green, extra->c.blue );
			SDL_FillRect( screen, 0, color ); 
		}	
	}

	//int i;
	//for( i=0; i<nWindows; ++i )
	{
		// Default to filling the background to black.
		fillBackground/*[i]*/ = true;
		backgroundColor/*[i]*/.Set( 0, 0, 0, 255 );

		// Set the screenbounds to a window.
		screenBounds/*[i]*/ = *bounds/*[i]*/;

		GLASSERT( bounds/*[i]*/->xmin >= 0 );
		GLASSERT( bounds/*[i]*/->ymin >= 0 );
		GLASSERT( bounds/*[i]*/->xmax < screen->w );
		GLASSERT( bounds/*[i]*/->ymax < screen->h );

		// Really aweful bugs result if the indivual windows
		// aren't clipped to the screen.
		screenBounds/*[i]*/.DoClip( windowBounds );

		// Initialize the DR to repaint everything and clip to the screen
		// IMPORTANT: Set clip before adding any rectangles.

		if(tree) dirtyRectangle/*[i]*/.Clear();//maks
		dirtyRectangle/*[i]*/.SetClipping( screenBounds/*[i]*/ );
		dirtyRectangle/*[i]*/.AddRectangle( screenBounds/*[i]*/ );
	}

	// Check that none overlap.
	/*#ifdef DEBUG
		int j;
		for( i=0; i<nWindows; ++i )
		{
			for( j=i+1; j<nWindows; ++j )
			{
				GLASSERT( !bounds[i].Intersect( bounds[j] ) );
			}
		}
	#endif*/

	if ( paintInfo.openGL )
		InitOpenGL(windowBounds.Width(), windowBounds.Height(), extraBackground.Redf(), extraBackground.Greenf(), extraBackground.Bluef());

	// Initialization of stuff that has "this" usage.
	if(!vault) vault = new KrResourceVault; //maks
	if(!tree) tree = new KrImageTree( this ); //maks


#ifdef USE_COLLISION_SECTOR //maks
	KrImNode::InitSectors(screen->w, screen->h);
#endif
}

void KrEngine::ReinitVault() //maks
{
	delete vault;
	vault = new KrResourceVault;
}

void KrEngine::InitOpenGL(int w, int h, float r, float g, float b) //maks
{
	#ifdef KYRA_SUPPORT_OPENGL
	
	KrTextureManager::RemoveTextures(); //maks
	GLASSERT( KrTextureManager::TextureIndex() == 0 );

	
	SDL_CurrentVideo->glViewport( 0, 0, w, h );
	

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glClearColor(	r, 
										g, 
										b, 
										255.0 );

	SDL_CurrentVideo->glClearDepth(1.0);
#else
	SDL_CurrentVideo->glClearColorx(	GlES_DoubleToFixed(r), 
										GlES_DoubleToFixed(g), 
										GlES_DoubleToFixed(b), 
										GlES_DoubleToFixed(255.0) );

	SDL_CurrentVideo->glClearDepthx(GlES_Fixed_1);
#endif

	// The depth buffer isn't actually used. This only occured to me at the end:
	// Kyra sorts everything and draws in order...so you don't need the depth testing.
	// Oddly...if you turn this on, it creates a bug in 16bit mode. I'de really
	// like to understand that.  -- lee
	//	SDL_CurrentVideo->glDepthFunc(GL_LESS);
	//  SDL_CurrentVideo->glEnable(GL_DEPTH_TEST);	// disabled by default. //maks: depth error in fps benchmark.ged

	// TODO: after reading the above comment...we don't need the openGLZ parameter...
	//		 maybe. I suspect you could implement openGL without the depth buffer
	//		 in which case you need the z depths.
	
	SDL_CurrentVideo->glShadeModel(GL_FLAT);		// Don't need smooth for 2D.

	// Create and reset the projection matrix
	SDL_CurrentVideo->glMatrixMode(GL_PROJECTION);
	
	// This matrix is set up to map pixels to screen the same way the bitbuffer does.
	// Trick from the OpenGL docs.

	SDL_CurrentVideo->glLoadIdentity();

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glOrtho( 0, w,
			 h, 0, 
			 -kKrOglDistance, kKrOglDistance );
#else
	SDL_CurrentVideo->glOrthox( 0, GlES_IntToFixed(w),
			 GlES_IntToFixed(h), 0, 
			 GlES_IntToFixed(-kKrOglDistance), GlES_IntToFixed(kKrOglDistance) );
#endif

	SDL_CurrentVideo->glMatrixMode(GL_MODELVIEW);
	SDL_CurrentVideo->glLoadIdentity();

#ifndef GL_OES_VERSION_1_1 //maks
	SDL_CurrentVideo->glTranslatef(0.375, 0.375, 0.0);
#else
	SDL_CurrentVideo->glTranslatex(GlES_DoubleToFixed(0.375), GlES_DoubleToFixed(0.375), 0);
#endif

	// A more reasonable mode to start in.
	SDL_CurrentVideo->glMatrixMode(GL_MODELVIEW);

	SDL_CurrentVideo->glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Enable the texturing and the blending mode needed.
	
	SDL_CurrentVideo->glEnable( GL_TEXTURE_2D );
	SDL_CurrentVideo->glEnable( GL_BLEND );
	SDL_CurrentVideo->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//maks: we need the fastest rendering as possible
	SDL_CurrentVideo->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//SDL_CurrentVideo->glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);
	SDL_CurrentVideo->glDisable(GL_DITHER);

#ifdef GL_OES_VERSION_1_1
	SDL_CurrentVideo->glDisable(GL_MULTISAMPLE);
#endif


#ifdef DEBUG
	GLenum error;
	GLASSERT( (error = SDL_CurrentVideo->glGetError()) == GL_NO_ERROR );
#endif

	#endif
}


void KrEngine::FillBackground( const KrRGBA* fillColor )
{
	//for( int i=0; i<nWindows; ++i )
	//{
		FillBackgroundWindow( /*i,*/ fillColor );
	//}
}


void KrEngine::FillBackgroundWindow( /*int i,*/ const KrRGBA* fillColor )
{
	if ( fillColor )
	{
		fillBackground/*[i]*/   = true;
		backgroundColor/*[i]*/	= *fillColor;
		//SDL_MapRGB( screen->format, fillColor->c.red, fillColor->c.green, fillColor->c.blue );
	}
	else
	{
		fillBackground/*[i]*/ = false;
		backgroundColor/*[i]*/.Set( 0, 0, 0 );
	}
	dirtyRectangle/*[i]*/.AddRectangle( screenBounds/*[i]*/ );
}


//void KrEngine::ClearScreen( int red, int green, int blue )
//{
//	U32 color = SDL_MapRGB( screen->format, red, green, blue );
//	SDL_FillRect( screen, 0, color ); 
//
//	SDL_UpdateRect( screen, 0, 0, 0, 0 );
//	Tree()->Root()->Invalidate( KR_ALL_WINDOWS );
//}



#if defined( DRAWDEBUG_RLE ) || defined( DRAWDEBUG_BLTRECTS )
	extern int debugFrameCounter;
#endif


#if defined(WIN32) && !defined(_WIN32_WCE)
extern "C"int DX5_WaitForVerticalBlank();
extern SDL_mutex *renderMutEx;
#endif


void KrEngine::UpdateScreen( GlDynArray< KrRect >* rectArray )
{
	if ( paintInfo.openGL )
	{
		SDL_GL_SwapBuffers();
	}
	else
	{
		if ( rectArray->Count() == 0 )
			return;

		////////////////////////////////////////////////
		//maks: use static to avoid memory allocations
		static SDL_Rect* sdlrect = NULL;
		static int rectSize = 0;

		if(rectArray->Count() > rectSize)
		{
			rectSize = rectArray->Count();
			delete sdlrect;
			sdlrect = new SDL_Rect[rectSize];			
		}
		////////////////////////////////////////////////

		for( int i=0; i<rectArray->Count(); i++ )
		{
			sdlrect[i].x = rectArray->Item( i ).xmin;
			sdlrect[i].y = rectArray->Item( i ).ymin;
			sdlrect[i].w = rectArray->Item( i ).Width();
			sdlrect[i].h = rectArray->Item( i ).Height();
			
			/*GLASSERT( sdlrect[i].x >= 0 );
			GLASSERT( sdlrect[i].y >= 0 );
			GLASSERT( sdlrect[i].w > 0 && sdlrect[i].w <= screen->w );
			GLASSERT( sdlrect[i].h > 0 && sdlrect[i].h <= screen->h );
			GLASSERT( sdlrect[i].x + sdlrect[i].w <= screen->w );
			GLASSERT( sdlrect[i].y + sdlrect[i].h <= screen->h );*/
		}

//		GLOUTPUT( "Updating %d rects\n", rectArray->Count() );


		//SDL_UpdateRects velocity
		//vsync240x320.ged: 3ms
		//vsync640x480.ged: 8ms

		/*
		Windows with DirectX: Write in the backbuffer(shadow) copy and flip to video buffer
		Pocket PC: Write to video buffer and update after vsync (Use only flip without double buffer. You can use flip double buffer only if screen have whole update, not only some parts)
		GP2X: Flip or wait for vsync causes the color bar don't draw right in the res_GP2X_320x240.ged
		Linux and DIB: Write in the backbuffer and copy to video
		*/


#if (defined(WIN32) || defined(GP2X)) && !defined(_WIN32_WCE)
		if((screen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN)
		{
			//Vsync done by flip function			
			SDL_Flip(screen);
		}
#ifndef GP2X
		else
		{
			if(GetGameMode()) //Avoid CPU consume
			{
				DX5_WaitForVerticalBlank();
			}

			SDL_UpdateRects( screen, rectArray->Count(), sdlrect );
		}
#endif

		//Copy shadow to video (define RENDER_THREAD in GameControl.cpp)
		/*MuteEx mutex(renderMutEx);

		if((screen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN)
		{
			//Vsync done by flip function
			SDL_FlipCopy(screen);
		}
		else
		{
			SDL_UpdateRectsCopy( screen, rectArray->Count(), sdlrect );
		}*/
#else
		SDL_UpdateRects( screen, rectArray->Count(), sdlrect );
#endif	

		//delete [] sdlrect; //maks
	}
}









/*void gedAsm(void *screen, int x, int y)
{
__asm {
        push    eax
        push    ebx
        push    ecx
        push    edi
        mov edi, screen
        mov eax, x
        mov ebx, y
        mov ecx, ebx
        shl ecx, 0x4
        add edi, ecx
        mov ecx, ebx
        shl ecx, 0x5
        add edi, ecx
        mov ecx, ebx
        shl ecx, 0x6
        add edi, ecx
        mov ecx, ebx
        shl ecx, 0x7
        add edi, ecx
        add     edi,eax
        mov     ebx,10000
        
        mov     dword ptr [edi+0x3c],0xb2a9cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x40],0xcbedb2a9
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x218],0xd4d0cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21c],0xe5afe5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x220],0xa985d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x224],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x3f6],0xffb2e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fa],0xd4d0ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fe],0xcbedd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x402],0xcbedd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x406],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x5d2],0xe5afd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5d6],0xe5afffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5da],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5de],0xa985b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e2],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e6],0xa985d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ea],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ee],0xcbeda985
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x7ac],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7b0],0xee91d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7b4],0xd4d0ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7b8],0xdf9de5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7bc],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7c0],0xdedbefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7c4],0x9861b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7c8],0xffd5e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7cc],0xd4d0ee91
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7d0],0x9861b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7d4],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x988],0xcbedcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x98c],0xffb2e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x990],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x994],0xbdf7e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x998],0xe7dfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x99c],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9a0],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9a4],0xb2a9efdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9a8],0xee919861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9ac],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9b0],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9b4],0xb2a9d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x9b8],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0xb64],0xe5afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb68],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb6c],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb70],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb74],0xefdfd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb78],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb7c],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb80],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb84],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb88],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb8c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb90],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb94],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb98],0xb2a9f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xb9c],0x98619861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0xd40],0xe5afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd44],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd48],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd4c],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd50],0xcbedffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd54],0xe7dff71c
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd58],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd5c],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd60],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd64],0xffffefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd68],0x9861e5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd6c],0xffd5e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd70],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd74],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd78],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd7c],0xd3c5f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xd80],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0xf1e],0xee91e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf22],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf26],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf2a],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf2e],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf32],0xefdfcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf36],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf3a],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf3e],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf42],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf46],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf4a],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf4e],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf52],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf56],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf5a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf5e],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0xf62],0x9861a985
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x10fa],0xe5afd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x10fe],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1102],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1106],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x110a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x110e],0xcbedffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1112],0xefdfdedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1116],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x111a],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x111e],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1122],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1126],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x112a],0x9861e5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x112e],0xffd8ee91
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1132],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1136],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x113a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x113e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1142],0xb2a9ffd5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x1146],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x12d8],0xee91d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12dc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12e0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12e4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12e8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12ec],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12f0],0xe7dfcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12f4],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12f8],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x12fc],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1300],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1304],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1308],0xefdfffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x130c],0xd4d09861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1310],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1314],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1318],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x131c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1320],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1324],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1328],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x14b6],0xf7afe5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14ba],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14be],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14c2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14c6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14ca],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14ce],0xcbede5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14d2],0xe7dfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14d6],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14da],0xffffefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14de],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14e2],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14e6],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14ea],0xa985ffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14ee],0xffd8cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14f2],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14f6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14fa],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x14fe],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1502],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1506],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x150a],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1694],0xf7afd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1698],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x169c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16a0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16a4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16a8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16ac],0xb2a9ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16b0],0xefdfcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16b4],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16b8],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16bc],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16c0],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16c4],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16c8],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16cc],0xb2a9cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16d0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16d4],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16d8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16dc],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16e0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16e4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16e8],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x16ec],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1872],0xf7afe5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1876],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x187a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x187e],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1882],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1886],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x188a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x188e],0xd4d0b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1892],0xe7dfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1896],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x189a],0xffffefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x189e],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18a2],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18a6],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18aa],0xd4d0ffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18ae],0xffd8a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18b2],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18b6],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18ba],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18be],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18c2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18c6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18ca],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x18ce],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1a50],0xee91d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a54],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a58],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a5c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a60],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a64],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a68],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a6c],0xa985ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a70],0xefdfbdf7
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a74],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a78],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a7c],0xffffefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a80],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a84],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a88],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a8c],0xb2a9dedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a90],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a94],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a98],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1a9c],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1aa0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1aa4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1aa8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1aac],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1ab0],0xcbed9861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1c30],0xffb2ee91
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c34],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c38],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c3c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c40],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c44],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c48],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c4c],0xa985ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c50],0xe7dfbdf7
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c54],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c58],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c5c],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c60],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c64],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c68],0xffffefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c6c],0xb2a9dedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c70],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c74],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c78],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c7c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c80],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c84],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c88],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c8c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1c90],0x9861cbed
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1e0e],0xf7afe5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e12],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e16],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e1a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e1e],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e22],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e26],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e2a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e2e],0xbdf7a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e32],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e36],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e3a],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e3e],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e42],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e46],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e4a],0xdedbefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e4e],0xffd8b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e52],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e56],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e5a],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e5e],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e62],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e66],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e6a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e6e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1e72],0x9861b2a9
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x1fec],0xf7afd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1ff0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1ff4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1ff8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x1ffc],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2000],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2004],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2008],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x200c],0xa985ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2010],0xe7dfd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2014],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2018],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x201c],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2020],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2024],0xbdf7ffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2028],0xffffdedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x202c],0xb2a9dedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2030],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2034],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2038],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x203c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2040],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2044],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2048],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x204c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2050],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x2054],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x21cc],0xffb2e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21d0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21d4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21d8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21dc],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21e0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21e4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21e8],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21ec],0xa985ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21f0],0xe7dfcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21f4],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21f8],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x21fc],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2200],0xefdfefdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2204],0xbdf7dedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2208],0xefdfb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x220c],0xb2a9e5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2210],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2214],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2218],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x221c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2220],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2224],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2228],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x222c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2230],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2234],0x9861e5af
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x23aa],0xf7afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ae],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23b2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23b6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ba],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23be],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23c2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23c6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ca],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ce],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23d2],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23d6],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23da],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23de],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23e2],0xb2a9efdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23e6],0x18a1dedb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ea],0xd4d0bdf7
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23ee],0xfffdcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23f2],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23f6],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23fa],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x23fe],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2402],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2406],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x240a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x240e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2412],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x2416],0xa985
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x258a],0xf7aff6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x258e],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2592],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2596],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x259a],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x259e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25a2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25a6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25aa],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ae],0xa985cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25b2],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25b6],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ba],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25be],0xefdfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25c2],0x18a1df9d
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25c6],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ca],0xcbedb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ce],0xfffdd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25d2],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25d6],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25da],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25de],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25e2],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25e6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ea],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25ee],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25f2],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x25f6],0x9861e5af
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x2768],0xf7afb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x276c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2770],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2774],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2778],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x277c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2780],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2784],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2788],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x278c],0xf714ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2790],0xdedba985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2794],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2798],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x279c],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27a0],0xdedbe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27a4],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27a8],0xb2a918a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27ac],0xf6bab2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27b0],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27b4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27b8],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27bc],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27c0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27c4],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27c8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27cc],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27d0],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27d4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x27d8],0x986189c3
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x2948],0xf7afed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x294c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2950],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2954],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2958],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x295c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2960],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2964],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2968],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x296c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2970],0xb2a9a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2974],0xdf9ddf9d
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2978],0xe7dfdf9d
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x297c],0xe7dfe7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2980],0xdf9de7df
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2984],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2988],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x298c],0xfffdd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2990],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2994],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2998],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x299c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29a0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29a4],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29a8],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29ac],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29b0],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29b4],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x29b8],0x9861d4d0
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x2b26],0xf7afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b2a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b2e],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b32],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b36],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b3a],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b3e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b42],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b46],0xd4d0f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b4a],0xb2a9cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b4e],0xa985b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b52],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b56],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b5a],0xcbedb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b5e],0xe7dfbdf7
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b62],0xb2a9efdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b66],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b6a],0xf6bab2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b6e],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b72],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b76],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b7a],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b7e],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b82],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b86],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b8a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b8e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b92],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b96],0xee91ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2b9a],0xa9859861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x2d06],0xf7afd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d0a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d0e],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d12],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d16],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d1a],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d1e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d22],0xcbedffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d26],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d2a],0xe5afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d2e],0xffd8f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d32],0xf6baffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d36],0xd4d0f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d3a],0xa985b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d3e],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d42],0xbdf7bdf7
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d46],0x18a118a1
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d4a],0xffffcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d4e],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d52],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d56],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d5a],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d5e],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d62],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d66],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d6a],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d6e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d72],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d76],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2d7a],0x9861d3c5
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x2ee4],0xed65cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2ee8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2eec],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2ef0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2ef4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2ef8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2efc],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f00],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f04],0xffd5ee91
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f08],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f0c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f10],0xfffbffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f14],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f18],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f1c],0xf6bafffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f20],0x9861b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f24],0xbdf7a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f28],0xf71cd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f2c],0xfffdffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f30],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f34],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f38],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f3c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f40],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f44],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f48],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f4c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f50],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f54],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x2f58],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x2f5c],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x30c4],0xf6c9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30c8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30cc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30d0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30d4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30d8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30dc],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30e0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30e4],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30e8],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30ec],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30f0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30f4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30f8],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x30fc],0xfffffffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3100],0xe5d6ffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3104],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3108],0xffffd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x310c],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3110],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3114],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3118],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x311c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3120],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3124],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3128],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x312c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3130],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3134],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3138],0xcbede5af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x313c],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x32a4],0xf7afb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32a8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32ac],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32b0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32b4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32b8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32bc],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32c0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32c4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32c8],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32cc],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32d0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32d4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32d8],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32dc],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32e0],0xfffffffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32e4],0xb2a9efdf
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32e8],0xffffe5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32ec],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32f0],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32f4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32f8],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x32fc],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3300],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3304],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3308],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x330c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3310],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3314],0x89c3cbed
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x3484],0xf7afd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3488],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x348c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3490],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3494],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3498],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x349c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34a0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34a4],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34a8],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34ac],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34b0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34b4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34b8],0xfffdfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34bc],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34c0],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34c4],0xffffffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34c8],0xfffdffff
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34cc],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34d0],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34d4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34d8],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34dc],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34e0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34e4],0xffd5ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34e8],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x34ec],0xcbedf714
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x34f0],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x3664],0xf7afd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3668],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x366c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3670],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3674],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3678],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x367c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3680],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3684],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3688],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x368c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3690],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3694],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3698],0xf71cfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x369c],0xfffdf6ba
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36a0],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36a4],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36a8],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36ac],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36b0],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36b4],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36b8],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36bc],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36c0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36c4],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x36c8],0xcbedee91
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x36cc],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x3844],0xf7afed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3848],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x384c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3850],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3854],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3858],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x385c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3860],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3864],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3868],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x386c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3870],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3874],0xf6bafffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3878],0xb2a9d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x387c],0xfffde5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3880],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3884],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3888],0xfffdfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x388c],0xfffbfffd
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3890],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3894],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3898],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x389c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x38a0],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x38a4],0xcbedf714
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x3a24],0xf7afed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a28],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a2c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a30],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a34],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a38],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a3c],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a40],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a44],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a48],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a4c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a50],0xfffbffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a54],0xcbedd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a58],0xffd8e5d6
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a5c],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a60],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a64],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a68],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a6c],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a70],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a74],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a78],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a7c],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3a80],0xcbedf714
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x3a84],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x3c02],0xed65b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c06],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c0a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c0e],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c12],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c16],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c1a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c1e],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c22],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c26],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c2a],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c2e],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c32],0xd4d0d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c36],0xfffbffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c3a],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c3e],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c42],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c46],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c4a],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c4e],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c52],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c56],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c5a],0xf714fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3c5e],0xd4d0d4d0
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x3de2],0xed65b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3de6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3dea],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3dee],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3df2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3df6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3dfa],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3dfe],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e02],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e06],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e0a],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e0e],0xd4d0ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e12],0xfffbd4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e16],0xffd8ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e1a],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e1e],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e22],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e26],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e2a],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e2e],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e32],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e36],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3e3a],0xcbede5af
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x3fc2],0xed65b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fc6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fca],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fce],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fd2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fd6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fda],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fde],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fe2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fe6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fea],0xffd8ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3fee],0xa985e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3ff2],0xffd8f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3ff6],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3ffa],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x3ffe],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4002],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4006],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x400a],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x400e],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4012],0xffd8fffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4016],0xcbede5d6
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x41a2],0xed65b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41a6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41aa],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ae],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41b2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41b6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ba],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41be],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41c2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41c6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ca],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ce],0xa985b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41d2],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41d6],0xe5afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41da],0xfffbf714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41de],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41e2],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41e6],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ea],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41ee],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x41f2],0xd4d0ee91
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x4382],0xed65b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4386],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x438a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x438e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4392],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4396],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x439a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x439e],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43a2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43a6],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43aa],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43ae],0xe5afcbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43b2],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43b6],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43ba],0xcbed9861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43be],0xf714e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43c2],0xfffbffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43c6],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43ca],0xfffbfffb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x43ce],0xd4d0f714
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x43d2],0xcbed
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x4564],0xf7afd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4568],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x456c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4570],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4574],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4578],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x457c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4580],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4584],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4588],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x458c],0xd4d0d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4590],0x9861ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4594],0xfb33d9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4598],0xd9ebfb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x459c],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x45a0],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x45a4],0xcbedb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x45a8],0xd4d0e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x45ac],0xd4d0cbed
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x4744],0xf7afb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4748],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x474c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4750],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4754],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4758],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x475c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4760],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4764],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4768],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x476c],0xe5afb2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4770],0xcbedffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4774],0xfb339861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4778],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x477c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4780],0xd9ebd9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4784],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4788],0x9861a985
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x478c],0xa985
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x4924],0xff8ba985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4928],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x492c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4930],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4934],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4938],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x493c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4940],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4944],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4948],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x494c],0xee91a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4950],0xee91ffd8
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4954],0xd9eb9861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4958],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x495c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4960],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4964],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4968],0xd9ebfb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x496c],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x4b04],0xed659861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b08],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b0c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b10],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b14],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b18],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b1c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b20],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b24],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b28],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b2c],0xf7149861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b30],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b34],0x9861b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b38],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b3c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b40],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b44],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b48],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4b4c],0x9861fb33
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x4b50],0xa985
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x4ce4],0xed65a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ce8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4cec],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4cf0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4cf4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4cf8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4cfc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d00],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d04],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d08],0xf714ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d0c],0xffd5d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d10],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d14],0x9861ee91
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d18],0xfb33d9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d1c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d20],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d24],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d28],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d2c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4d30],0xb2a99861
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x4ec4],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ec8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ecc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ed0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ed4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ed8],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4edc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ee0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ee4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ee8],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4eec],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ef0],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ef4],0xb2a9ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4ef8],0xfb339861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4efc],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4f00],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4f04],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4f08],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4f0c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x4f10],0x9861d9eb
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x4f14],0xcbed
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x50a6],0xff8b9861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50aa],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ae],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50b2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50b6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ba],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50be],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50c2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50c6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ca],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ce],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50d2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50d6],0xa985f714
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50da],0xfb339861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50de],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50e2],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50e6],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ea],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50ee],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x50f2],0x9861d9eb
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x5286],0xed65a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x528a],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x528e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5292],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5296],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x529a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x529e],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52a2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52a6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52aa],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52ae],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52b2],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52b6],0xe5afffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52ba],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52be],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52c2],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52c6],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52ca],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52ce],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x52d2],0x9861fb33
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x52d6],0xb2a9
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x5466],0xa985b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x546a],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x546e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5472],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5476],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x547a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x547e],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5482],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5486],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x548a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x548e],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5492],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5496],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x549a],0x9861cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x549e],0xfb33d9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x54a2],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x54a6],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x54aa],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x54ae],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x54b2],0xd9ebfb33
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x54b6],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x5648],0xed659861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x564c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5650],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5654],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5658],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x565c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5660],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5664],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5668],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x566c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5670],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5674],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5678],0xffd5ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x567c],0x9861cbed
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5680],0xfb33a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5684],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5688],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x568c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5690],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5694],0x9861fb33
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x5828],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x582c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5830],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5834],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5838],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x583c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5840],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5844],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5848],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x584c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5850],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5854],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5858],0xffd5ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x585c],0xd4d0ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5860],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5864],0xfb33d9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5868],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x586c],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5870],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5874],0xd9ebfb33
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x5878],0xa985
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x5a0a],0xed659861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a0e],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a12],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a16],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a1a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a1e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a22],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a26],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a2a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a2e],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a32],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a36],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a3a],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a3e],0xee91ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a42],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a46],0xfb33a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a4a],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a4e],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a52],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5a56],0x9861fb33
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x5bec],0xff8ba985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5bf0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5bf4],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5bf8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5bfc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c00],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c04],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c08],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c0c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c10],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c14],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c18],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c1c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c20],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c24],0xa985e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c28],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c2c],0xfb33d9eb
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c30],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5c34],0xfb33fb33
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x5c38],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x5dcc],0xd3c59861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5dd0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5dd4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5dd8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ddc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5de0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5de4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5de8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5dec],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5df0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5df4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5df8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5dfc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e00],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e04],0xffb2ffd5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e08],0xb2a9d4d0
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e0c],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e10],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5e14],0xd9ebd9eb
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x5e18],0x9861
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x5fae],0xed659861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fb2],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fb6],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fba],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fbe],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fc2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fc6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fca],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fce],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fd2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fd6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fda],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fde],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fe2],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fe6],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fea],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5fee],0xd3c5e5af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ff2],0x89c3b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ff6],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ffa],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x5ffe],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6002],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6006],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x600a],0xb2a9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x600e],0xf6c9d3c5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x6012],0xd3c5
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6190],0xf6c99861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6194],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6198],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x619c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61a0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61a4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61a8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61ac],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61b0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61b4],0xffb2f7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61b8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61bc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61c0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61c4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61c8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61cc],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61d0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61d4],0xf7afffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61d8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61dc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61e0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61e4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61e8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61ec],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x61f0],0x18a1f6c9
		sub ebx, 2
		jz Exit

        mov     dword ptr [edi+0x6370],0x89c39861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6374],0xff8bf6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6378],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x637c],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6380],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6384],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6388],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x638c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6390],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6394],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6398],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x639c],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63a0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63a4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63a8],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63ac],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63b0],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63b4],0xffb2ffb2
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63b8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63bc],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63c0],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63c4],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63c8],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x63cc],0xff8bf7af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x63d0],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6552],0x89c39861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6556],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x655a],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x655e],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6562],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6566],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x656a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x656e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6572],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6576],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x657a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x657e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6582],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6586],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x658a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x658e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6592],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6596],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x659a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x659e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x65a2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x65a6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x65aa],0xff8bf7af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x65ae],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6734],0xb2a99861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6738],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x673c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6740],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6744],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6748],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x674c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6750],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6754],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6758],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x675c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6760],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6764],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6768],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x676c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6770],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6774],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6778],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x677c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6780],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6784],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6788],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x678c],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6916],0x89c39861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x691a],0xff8bf6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x691e],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6922],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6926],0xf7afff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x692a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x692e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6932],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6936],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x693a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x693e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6942],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6946],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x694a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x694e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6952],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6956],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x695a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x695e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6962],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6966],0xff8bf7af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x696a],0x89c3
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6af8],0xa9859861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6afc],0xff8bf6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b00],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b04],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b08],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b0c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b10],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b14],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b18],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b1c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b20],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b24],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b28],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b2c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b30],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b34],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b38],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b3c],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b40],0xff8bf7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6b44],0xf6c9f7af
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x6b48],0xb2a9
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6cda],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cde],0xff8bd3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ce2],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ce6],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cea],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cee],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cf2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cf6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cfa],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6cfe],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d02],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d06],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d0a],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d0e],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d12],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d16],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d1a],0xff8bf7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d1e],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6d22],0xed65ff8b
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x6d26],0xd3c5
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x6ebe],0x89c39861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ec2],0xff8bed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ec6],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6eca],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ece],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ed2],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ed6],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6eda],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ede],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ee2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ee6],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6eea],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6eee],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ef2],0xf7aff7af
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6ef6],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6efa],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x6efe],0xed65ff8b
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x6f02],0xd4d0
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x70a0],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70a4],0xf6c9d3c5
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70a8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70ac],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70b0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70b4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70b8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70bc],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70c0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70c4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70c8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70cc],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70d0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70d4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70d8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x70dc],0xed65f6c9
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x70e0],0xd4d0
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x7284],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7288],0xf6c9b2a9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x728c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7290],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7294],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7298],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x729c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72a0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72a4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72a8],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72ac],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72b0],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72b4],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x72b8],0xd3c5f6c9
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x72bc],0xcbed
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x7468],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x746c],0xd3c589c3
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7470],0xff8bed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7474],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7478],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x747c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7480],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7484],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7488],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x748c],0xff8bff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7490],0xed65ff8b
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7494],0xd3c5d3c5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x7498],0xcbed
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x764e],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7652],0xd3c5a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7656],0xed65ed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x765a],0xf6c9f6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x765e],0xf6c9f6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7662],0xf6c9f6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7666],0xf6c9f6c9
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x766a],0xed65ed65
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x766e],0xb2a9d3c5
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x7672],0xb2a9
		dec ebx
		jz Exit

        mov     dword ptr [edi+0x7836],0x9861a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x783a],0x98619861
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x783e],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7842],0xa985a985
		sub ebx, 2
		jz Exit
        mov     dword ptr [edi+0x7846],0xa985a985
		sub ebx, 2
		jz Exit
        mov     word ptr [edi+0x784a],0xcbed
		dec ebx
		jz Exit


Exit:
        pop     edi
        pop     ecx
        pop     ebx
        pop     eax

}
}



void memfpu( void *d, const void *s, int _size ) {
	_asm {
		mov edi,d;
		mov esi,s;
		mov ecx,_size;
		shr ecx,3;
lx:
		fld double ptr [esi];
		lea esi,[esi+8];
		fstp double ptr [edi];
		lea edi,[edi+8];
		dec ecx;
		jnz lx;
	}
}


unsigned short spriteData[] = 
 { 
1,15,31,
4,30,52205,45737,45737,52205,
1,45,31,
1,255,31,
8,268,52205,54480,58799,58799,54480,43397,39009,45737,
1,285,31,
1,495,31,
10,507,58799,65458,65458,54480,54213,52205,54480,52205,39009,45737,
1,525,31,
1,735,31,
16,745,54480,58799,65458,58799,45737,45737,45737,43397,39009,43397,54480,43397,39009,39009,43397,52205,
1,765,31,
1,975,31,
22,982,45737,45737,54480,61073,65493,54480,58838,57245,61407,61407,61407,57051,45737,39009,58799,65493,61073,54480,45737,39009,39009,43397,
1,1005,31,
1,1215,31,
26,1220,52205,52205,58799,65458,65493,65493,58799,48631,61407,59359,59359,59359,61407,61407,61407,45737,39009,61073,65493,65493,65493,63252,54480,45737,39009,31,
1,1455,31,
30,1458,52205,58799,65458,65493,65493,65458,65493,61073,54480,61407,59359,59359,59359,61407,61407,61407,61407,61407,43397,43397,65493,65493,65493,65493,65493,65493,63252,31,39009,39009,
35,1695,31,52205,58799,65458,65458,65458,65458,65458,65493,65458,52205,63260,59359,59359,61407,61407,61407,61407,61407,61407,65535,58838,39009,58799,65493,65493,65493,65493,65493,65493,31,63252,54213,39009,43397,
36,1935,31,61073,65458,65458,65458,65458,65458,65493,65493,58799,52205,61407,59359,61407,61407,61407,61407,61407,65535,65535,65535,65535,45737,45737,65496,65493,65493,65493,65493,65493,31,65493,65493,58799,43397,39009,
39,2173,54480,58799,31,65458,65458,65458,65458,65458,65493,65493,65493,52205,57051,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,58838,39009,61073,65496,65493,65493,65493,65493,31,65493,65493,65493,65493,45737,39009,
42,2412,54480,61073,65458,31,65458,65458,65458,65458,65493,65493,65493,63252,52205,59359,59359,59359,61407,61407,65535,65535,65535,65535,65535,65535,65535,61407,39009,54480,65496,65493,65493,65493,65493,31,65493,65493,65493,65493,65493,58799,39009,43397,
44,2651,58799,63407,65458,65458,31,65458,65458,65458,65493,65493,65493,65493,58799,52205,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,43397,52205,65496,65496,65496,65493,65493,31,65493,65493,65493,65493,65493,65493,58799,39009,45737,
46,2890,54480,63407,65458,65458,65458,31,65458,65458,65493,65493,65493,65493,65493,45737,52205,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,52205,45737,65496,65496,65496,65496,65493,31,65493,65493,65493,65493,65493,65493,65493,61073,39009,43397,
48,3129,58799,63407,65458,65458,65458,65458,31,65458,65458,65493,65493,65493,65493,65493,45737,54480,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,54480,43397,65496,65496,65496,65496,65496,31,65493,65493,65493,65493,65493,65493,65493,65493,61073,39009,45737,
50,3368,54480,61073,65458,65458,65458,65458,65458,31,65458,65493,65493,65493,65493,65493,65493,43397,48631,61407,59359,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,57051,45737,65496,65496,65496,65496,65496,31,65496,65493,65493,65493,65493,65493,65493,65493,65493,58799,39009,52205,
65,3600,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
52,3847,58799,63407,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,65493,43397,48631,59359,59359,59359,59359,61407,61407,61407,61407,61407,61407,65535,65535,61407,57051,45737,65496,65496,65496,65496,65496,31,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,45737,39009,
53,4086,54480,63407,65458,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,65493,43397,54480,59359,59359,59359,59359,59359,61407,61407,61407,61407,65535,48631,57051,65535,57051,45737,65531,65531,65496,65496,65496,31,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,61073,39009,
54,4326,58799,65458,65458,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65496,65496,43397,52205,59359,59359,59359,59359,59359,59359,59359,61407,61407,57051,48631,45737,61407,58838,45737,65531,65531,65531,65496,65496,31,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65493,58799,39009,
31,4565,52205,63407,63407,65458,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65496,65496,45737,45737,59359,59359,59359,59359,59359,59359,59359,59359,61407,45737,57051,
23,4597,48631,54480,52205,65533,65531,65531,65531,65496,31,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,65458,43397,
29,4805,63177,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65493,65496,65496,52205,43397,59359,59359,59359,59359,59359,59359,59359,61407,57245,
24,4837,45737,52205,54480,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65493,58799,39009,
30,5044,45737,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65493,65496,65496,63252,43397,57051,59359,59359,59359,59359,59359,59359,59359,57051,
25,5077,45737,45737,63162,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65458,35267,39009,
30,5284,60773,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65496,65496,65496,65496,65496,43397,45737,57245,57245,57245,59359,59359,59359,59359,57245,
24,5318,54480,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65493,54480,39009,
32,5523,52205,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,63252,54480,52205,45737,45737,43397,39009,43397,43397,43397,45737,52205,48631,59359,61407,45737,
26,5557,45737,63162,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,65458,61073,39009,43397,
32,5763,54213,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,52205,45737,45737,52205,58799,63252,65496,65496,63162,63252,54480,45737,43397,39009,43397,48631,48631,
26,5797,52205,65535,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,65458,65493,54213,39009,
61,6002,52205,60773,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,63252,61073,65493,65496,65496,65496,65496,65496,65531,65531,65531,65533,65533,65531,63162,45737,39009,43397,48631,54480,63260,65535,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65493,58799,39009,
61,6242,45737,63177,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65496,65496,65496,65496,65496,65496,65496,65531,65531,65531,65531,65533,65535,65535,58838,43397,39009,54480,65535,65533,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,58799,52205,39009,
58,6482,45737,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65531,65531,65533,65533,65533,65535,61407,45737,58838,65535,65533,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,61073,52205,35267,
55,6722,54213,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65531,65533,65533,65533,65533,65533,65535,65535,65535,65533,65533,65533,65533,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65493,65496,65496,63252,52205,35267,
53,6962,54213,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65533,63260,63162,65533,65533,65533,65533,65533,65533,65533,65533,65533,65531,65531,65531,65531,65531,31,65496,65496,65496,65496,65496,65496,61073,52205,35267,
50,7202,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65531,63162,54480,45737,58838,65533,65533,65533,65533,65533,65533,65533,65533,65531,65531,65531,65531,65531,65531,31,65496,65496,65496,65496,63252,52205,
49,7442,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,31,65493,65493,65493,65493,65493,65496,65496,65496,65496,65531,54480,52205,58838,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,31,65496,65496,63252,52205,35267,
48,7681,45737,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65496,65496,65496,65496,54480,54480,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,65496,31,63252,54480,54480,
46,7921,45737,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,65496,65496,54480,54480,65531,65496,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,31,52205,
45,8161,45737,60773,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,65496,58799,43397,63252,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,58838,52205,31,
42,8401,45737,60773,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,65493,45737,43397,39009,43397,52205,58799,63252,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,61073,54480,
1,8445,31,
41,8641,45737,60773,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,31,65493,65493,65493,65493,65493,65493,61073,52205,58799,39009,43397,39009,39009,39009,52205,58799,63252,65496,65531,65531,65531,65531,65531,63252,54480,52205,
1,8685,31,
38,8882,54213,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,31,65458,65493,65493,65493,65493,65493,54480,54480,65493,39009,55787,64307,64307,55787,39009,39009,39009,43397,45737,52205,58799,54480,52205,54480,
1,8925,31,
37,9122,45737,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,31,65458,65458,65493,65493,65493,65493,45737,58799,65496,52205,39009,64307,64307,64307,64307,64307,55787,55787,43397,39009,43397,39009,43397,
1,9165,31,
38,9362,43397,65419,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,31,65458,65458,65458,65493,65493,63252,43397,61073,65496,61073,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,45737,
1,9405,31,
39,9602,39009,60773,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,31,65458,65458,65458,65493,65493,63252,39009,63252,65493,65493,45737,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,43397,
1,9645,31,
40,9842,43397,60773,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,31,65458,65458,65458,65458,65493,63252,54480,65493,65493,65493,61073,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,45737,
1,9885,31,
41,10082,45737,45737,65419,65419,63407,63407,63407,63407,63407,63407,63407,65458,65458,31,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,45737,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,52205,
1,10125,31,
40,10323,39009,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,65458,31,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,63252,43397,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,
1,10365,31,
41,10563,43397,60773,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,31,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,58799,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,45737,
1,10605,31,
65,10800,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
40,11044,39009,60773,65419,65419,63407,63407,63407,63407,63407,63407,63407,31,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,52205,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,
1,11085,31,
42,11284,45737,45737,65419,65419,65419,63407,63407,63407,63407,63407,63407,31,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,54480,39009,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,55787,43397,31,
41,11525,39009,60773,65419,65419,65419,63407,63407,63407,63407,63407,31,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,61073,43397,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,39009,31,
40,11766,43397,65419,65419,65419,65419,63407,63407,63407,63407,31,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65458,58799,43397,39009,39009,55787,64307,64307,64307,64307,64307,39009,31,
40,12006,39009,54213,65419,65419,65419,65419,63407,63407,63407,31,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65458,54480,45737,43397,39009,39009,43397,55787,55787,39009,31,
51,12247,39009,60773,65419,65419,65419,65419,63407,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,58799,54213,45737,35267,43397,43397,31,39009,39009,43397,39009,43397,43397,43397,45737,45737,54213,63177,54213,
49,12488,39009,63177,65419,65419,65419,65419,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,63407,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63177,
49,12728,39009,35267,63177,65419,65419,65419,65419,31,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,35267,
47,12969,39009,35267,65419,65419,65419,65419,31,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,65419,35267,
45,13210,39009,45737,65419,65419,65419,31,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,31,63407,63407,63407,63407,63407,63407,63407,63407,35267,
43,13451,39009,35267,63177,65419,31,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,31,63407,63407,63407,63407,63407,63407,65419,35267,
41,13692,39009,43397,63177,31,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,31,63407,63407,63407,65419,63407,63177,45737,
39,13933,43397,39009,31,65419,65419,65419,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,31,65419,65419,65419,65419,60773,54213,
35,14175,31,35267,60773,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,65419,31,65419,65419,60773,54480,
34,14415,31,43397,39009,54213,63177,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,31,63177,60773,54480,
1,14655,31,
29,14658,39009,39009,45737,63177,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,63177,31,52205,
1,14895,31,
26,14900,43397,39009,35267,54213,60773,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,60773,54213,54213,52205,31,
1,15135,31,
19,15143,39009,39009,43397,54213,60773,60773,63177,63177,63177,63177,63177,63177,63177,63177,60773,60773,54213,45737,45737,
1,15165,31,
1,15375,31,
11,15387,43397,39009,39009,39009,43397,43397,43397,43397,43397,43397,52205,
1,15405,31,
1,15615,31,
1,15645,31,
0
};

void ged(char *screen, int x, int y)
{
	//Fast blit 16bit sprite without clipping
	//Sgment Format: len, segOffSet, data 

	unsigned short *addr, *start = (unsigned short *)(screen + y*240 + x), *sprite = spriteData;
	int len = *(sprite++);//, lenr;
	//__int64 *d, *s;
	//char *d, *s;


	while(len)
	{
		addr = start + *(sprite++);
		memcpy(addr, sprite, len << 1);
		
		
	
				
		sprite += len;		

		len = *(sprite++);
	}

}

unsigned short spriteData1[] = 
 { 
4,52205,45737,45737,52205,
8,52205,54480,58799,58799,54480,43397,39009,45737,
10,58799,65458,65458,54480,54213,52205,54480,52205,39009,45737,
16,54480,58799,65458,58799,45737,45737,45737,43397,39009,43397,54480,43397,39009,39009,43397,52205,
22,45737,45737,54480,61073,65493,54480,58838,57245,61407,61407,61407,57051,45737,39009,58799,65493,61073,54480,45737,39009,39009,43397,
26,52205,52205,58799,65458,65493,65493,58799,48631,61407,59359,59359,59359,61407,61407,61407,45737,39009,61073,65493,65493,65493,63252,54480,45737,39009,43397,
30,52205,58799,65458,65493,65493,65458,65493,61073,54480,61407,59359,59359,59359,61407,61407,61407,61407,61407,43397,43397,65493,65493,65493,65493,65493,65493,63252,45737,39009,39009,
34,52205,58799,65458,65458,65458,65458,65458,65493,65458,52205,63260,59359,59359,61407,61407,61407,61407,61407,61407,65535,58838,39009,58799,65493,65493,65493,65493,65493,65493,65493,63252,54213,39009,43397,
36,58799,61073,65458,65458,65458,65458,65458,65493,65493,58799,52205,61407,59359,61407,61407,61407,61407,61407,65535,65535,65535,65535,45737,45737,65496,65493,65493,65493,65493,65493,65493,65493,65493,58799,43397,39009,
39,54480,58799,65458,65458,65458,65458,65458,65458,65493,65493,65493,52205,57051,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,58838,39009,61073,65496,65493,65493,65493,65493,65493,65493,65493,65493,65493,45737,39009,
42,54480,61073,65458,65458,65458,65458,65458,65458,65493,65493,65493,63252,52205,59359,59359,59359,61407,61407,65535,65535,65535,65535,65535,65535,65535,61407,39009,54480,65496,65493,65493,65493,65493,65493,65493,65493,65493,65493,65493,58799,39009,43397,
44,58799,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,58799,52205,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,43397,52205,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,65493,58799,39009,45737,
46,54480,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,45737,52205,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,52205,45737,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,65493,61073,39009,43397,
48,58799,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,45737,54480,61407,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,65535,54480,43397,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,61073,39009,45737,
50,54480,61073,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,43397,48631,61407,59359,59359,61407,61407,61407,65535,65535,65535,65535,65535,65535,65535,57051,45737,65496,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,58799,39009,52205,
50,61073,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,43397,48631,59359,59359,59359,61407,61407,61407,61407,65535,65535,65535,65535,61407,65535,57051,45737,65496,65496,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,52205,39009,
52,58799,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,43397,48631,59359,59359,59359,59359,61407,61407,61407,61407,61407,61407,65535,65535,61407,57051,45737,65496,65496,65496,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,45737,39009,
53,54480,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65493,43397,54480,59359,59359,59359,59359,59359,61407,61407,61407,61407,65535,48631,57051,65535,57051,45737,65531,65531,65496,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,61073,39009,
54,58799,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,43397,52205,59359,59359,59359,59359,59359,59359,59359,61407,61407,57051,48631,45737,61407,58838,45737,65531,65531,65531,65496,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65493,58799,39009,
31,52205,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,45737,45737,59359,59359,59359,59359,59359,59359,59359,59359,61407,45737,57051,
23,48631,54480,52205,65533,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65493,65458,43397,
29,63177,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65496,65496,52205,43397,59359,59359,59359,59359,59359,59359,59359,61407,57245,
24,45737,52205,54480,65533,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65493,58799,39009,
30,45737,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65496,65496,63252,43397,57051,59359,59359,59359,59359,59359,59359,59359,57051,
25,45737,45737,63162,65533,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65458,35267,39009,
30,60773,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,43397,45737,57245,57245,57245,59359,59359,59359,59359,57245,
24,54480,65533,65533,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65493,54480,39009,
32,52205,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,63252,54480,52205,45737,45737,43397,39009,43397,43397,43397,45737,52205,48631,59359,61407,45737,
26,45737,63162,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,65458,61073,39009,43397,
32,54213,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,52205,45737,45737,52205,58799,63252,65496,65496,63162,63252,54480,45737,43397,39009,43397,48631,48631,
26,52205,65535,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,65458,65493,54213,39009,
61,52205,60773,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,63252,61073,65493,65496,65496,65496,65496,65496,65531,65531,65531,65533,65533,65531,63162,45737,39009,43397,48631,54480,63260,65535,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65458,65458,65493,58799,39009,
61,45737,63177,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65496,65531,65531,65531,65531,65533,65535,65535,58838,43397,39009,54480,65535,65533,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,65493,65458,58799,52205,39009,
58,45737,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65531,65531,65533,65533,65533,65535,61407,45737,58838,65535,65533,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65493,65493,65493,65493,65493,65493,61073,52205,35267,
55,54213,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65531,65533,65533,65533,65533,65533,65535,65535,65535,65533,65533,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65493,65496,65496,63252,52205,35267,
53,54213,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65496,65531,65531,65533,63260,63162,65533,65533,65533,65533,65533,65533,65533,65533,65533,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,65496,65496,61073,52205,35267,
50,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,65496,65531,63162,54480,45737,58838,65533,65533,65533,65533,65533,65533,65533,65533,65531,65531,65531,65531,65531,65531,65496,65496,65496,65496,65496,63252,52205,
49,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,65531,54480,52205,58838,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,65496,65496,65496,63252,52205,35267,
48,45737,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,65496,65496,65496,54480,54480,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,65496,65531,63252,54480,54480,
46,45737,60773,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,65496,65496,54480,54480,65531,65496,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,58799,52205,
44,45737,60773,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65496,58799,43397,63252,65496,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65496,58838,52205,
42,45737,60773,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,65493,45737,43397,39009,43397,52205,58799,63252,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,65531,61073,54480,
41,45737,60773,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,61073,52205,58799,39009,43397,39009,39009,39009,52205,58799,63252,65496,65531,65531,65531,65531,65531,63252,54480,52205,
38,54213,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,54480,54480,65493,39009,55787,64307,64307,55787,39009,39009,39009,43397,45737,52205,58799,54480,52205,54480,
37,45737,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,45737,58799,65496,52205,39009,64307,64307,64307,64307,64307,55787,55787,43397,39009,43397,39009,43397,
38,43397,65419,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,63252,43397,61073,65496,61073,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,45737,
39,39009,60773,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65493,65493,63252,39009,63252,65493,65493,45737,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,43397,
40,43397,60773,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65493,63252,54480,65493,65493,65493,61073,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,45737,
41,45737,45737,65419,65419,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,65493,45737,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,52205,
40,39009,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,65493,63252,43397,39009,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,
41,43397,60773,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,65493,58799,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,45737,
41,45737,43397,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,65493,52205,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,55787,39009,
40,39009,60773,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,52205,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,64307,64307,39009,
41,45737,45737,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65493,54480,39009,39009,55787,64307,64307,64307,64307,64307,64307,64307,64307,55787,43397,
40,39009,60773,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,61073,43397,39009,43397,64307,64307,64307,64307,64307,64307,64307,64307,39009,
39,43397,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65458,58799,43397,39009,39009,55787,64307,64307,64307,64307,64307,39009,
39,39009,54213,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65493,65458,54480,45737,43397,39009,39009,43397,55787,55787,39009,
51,39009,60773,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,58799,54213,45737,35267,43397,43397,39009,39009,39009,43397,39009,43397,43397,43397,45737,45737,54213,63177,54213,
50,39009,63177,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63177,6305,
49,39009,35267,63177,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,65458,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,35267,
47,39009,35267,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,35267,
45,39009,45737,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,35267,
43,39009,35267,63177,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,35267,
41,39009,43397,63177,65419,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,63407,63177,45737,
39,43397,39009,54213,65419,65419,65419,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,65419,65419,65419,60773,54213,
35,39009,35267,60773,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,63407,65419,65419,65419,65419,65419,60773,54480,
33,43397,39009,54213,63177,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,63177,60773,54480,
29,39009,39009,45737,63177,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,63177,54213,52205,
25,43397,39009,35267,54213,60773,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,65419,60773,54213,54213,52205,
19,39009,39009,43397,54213,60773,60773,63177,63177,63177,63177,63177,63177,63177,63177,60773,60773,54213,45737,45737,
11,43397,39009,39009,39009,43397,43397,43397,43397,43397,43397,52205,
0
};

short spriteLine[] = 
 { 
0,5,14,25,42,65,92,123,158,195,235,278,323,370,419,470,521,574,628,683,739,794,851,907,967,1027,1089,1151,1210,1266,1320,1371,1421,1470,1517,1562,1605,1647,1686,1724,1763,1803,1844,1886,1927,1969,2011,2052,2094,2135,2175,2215,2267,2318,2368,2416,2462,2506,2548,2588,2624,2658,2688,2714,2734,2734,2734
};

unsigned long spriteOffSet[] = 
 { 
30,268,507,745,982,1220,1458,1696,1935,2173,2412,2651,2890,3129,3368,3608,3847,4086,4326,4565,4597,4805,4837,5044,5077,5284,5318,5523,5557,5763,5797,6002,6242,6482,6722,6962,7202,7442,7681,7921,8161,8401,8641,8882,9122,9362,9602,9842,10082,10323,10563,10803,11044,11284,11525,11766,12006,12247,12488,12728,12969,13210,13451,13692,13933,14175,14416,14658,14900,15143,15387,0
};



void gedCl(char *screen, int x, int y, int i, int j, int width, int height)
{
	//Fast blit 16bit sprite with clipping
	//Sgment Format: line, len, segOffSet, data 

	unsigned short *addr, *start = (unsigned short *)(screen + y*240 + x), *leftClip;
	register unsigned short *sprite = spriteData1 + spriteLine[j], *spriteFin = spriteData1 + spriteLine[j + height];
	int len = *(sprite++), i0 = j, dx, w = width;
	short *nextLineIndex = spriteLine + i0 + 1;
	unsigned short *nextLine = spriteData1 + *nextLineIndex;
	unsigned long *offset = spriteOffSet;

	leftClip = (unsigned short *)(start + i0*240 + i);


	while(sprite < spriteFin)
	{		
		addr = start + *(offset++);
		

		dx = leftClip - addr;
		if(dx > 0)
		{
			sprite += dx;
			len -= dx;
			
			addr = leftClip;
		}
		

		w = leftClip + width - addr;		


		if(len > 0) 
		{
			memcpy(addr, sprite, ((len < w)?len:w) << 1);			
		}

				
		sprite += len;	
		len = *(sprite++);

		if(sprite > nextLine) 
		{
			i0++;
			*nextLineIndex++;

			nextLine = spriteData1 + *nextLineIndex;
			leftClip = (unsigned short *)(start + i0*240 + i);
		}
	}
}*/


void Clear16(SDL_Surface *screen, U16 color )
{
	//Clear a 16 bpp screen with specified color
    // We operate on two pixels at a time. The count
    // thus represents the number of pixel pairs.
    unsigned  int count    = screen->h * screen->pitch >> 2;
    U32* pixels   = (U32*)screen->pixels;
    U32  color32  = (color << 16) | (color);

    while (count--)
        *pixels++ = color32;
}

void ForceSuspendGame();

bool bFast = false;
KrImage *gedImage = NULL;
void KrEngine::Draw( bool updateTree, GlDynArray< KrRect >* _rectangles ) //maks
{	
	bool updateRect = false; 
	GlDynArray< KrRect >* rectArray = 0;
	static GlDynArray< KrRect > rectArrayOnStack; //maks: use static to avoid memory allocations
	
	{ //lock surface
		SurfaceLock lock(&paintInfo); //maks: to GAPI and direct draw
		
		
		
		
#ifdef _WIN32_WCE
		if(!screen || (!screen->pixels && !(screen->flags & SDL_OPENGL))) 
		{
			//After a suspension. Must execute in next draw
			//Will crash if continue
			ForceSuspendGame();
			return; 
		}
#endif
		
		
		
#if defined( DRAWDEBUG_RLE ) || defined( DRAWDEBUG_BLTRECTS )
		debugFrameCounter++;
#endif
		
		//	GLOUTPUT( "Engine::Draw Walk\n" );
		
		if(updateTree) //maks
		{
#ifdef USE_WALK
			tree->Walk();
#else
			if(bNewNode) 
			{
				bNewNode = false;
				tree->Walk();		
			}
#endif
		}


		//return; //For logic performance tests
		


		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Draw full	
	/*U32 sdlColor = SDL_MapRGB( screen->format,	backgroundColor[0].c.red,
		backgroundColor[0].c.green,
		backgroundColor[0].c.blue );

	SDL_FillRect( screen, NULL, sdlColor );

	tree->DrawWalk( windowBounds, &paintInfo, 0 );

	//fps benchmark.ged
	if(windowBounds.Width() == 240 && windowBounds.Height() == 320) 
	{
		int x = 0, y = 0;
		for(int i = 0; i < 10000; i ++)
		{
			//gedCl((char *)screen->pixels, x, y, 0, 0, 65, 66);
			bFast = true;
			gedImage->Draw( &paintInfo, windowBounds, 0 );
			bFast = false;
			//break;

			x += 64;
			if(x > 240 - 64)
			{
				x = 0;
				y += 64;
			}

			if(y > 320 - 64) y = 0;
		}
	}

	SDL_UpdateRect(screen, 0, 0, 0, 0);
	
	return;*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	if ( !paintInfo.openGL )
	{
		// Queue up the rectangles that will be used to blit to screen:

		// We either use the passed in rectangles,
		// or the one here on the stack. Set the pointer
		// rectArray to the right thing.
		if ( _rectangles )
		{
			rectArray = _rectangles;
			rectArray->Clear();
		}
		else
		{
			rectArray = &rectArrayOnStack;
			rectArray->ClearCount(); //maks: avoid memory allocations
		}
	
		

		if ( needFullScreenUpdate )
		{
			needFullScreenUpdate = false;			
			KrRect rect;
			rect.Set( 0, 0, screen->w-1, screen->h-1 );
			rectArray->PushBack( rect );
		}
		else
		{
			//for ( int i=0; i<nWindows; ++i )
			{
				for ( int j=0; j<dirtyRectangle/*[i]*/.NumRect(); ++j )
				{
					rectArray->PushBack( dirtyRectangle/*[i]*/.Rect(j) );
				}
			}
		}	
	}
	
	if ( paintInfo.openGL )
	{		
		#ifdef KYRA_SUPPORT_OPENGL

		// OpenGL drawing
		SDL_CurrentVideo->glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//InitNew2DStrip(); //opengl:teste

		
		//for( int j=0; j<nWindows; ++j )
		{
			if ( fillBackground/*[j]*/ )
			{
				SDL_CurrentVideo->glBindTexture( GL_TEXTURE_2D, 0 );
				

#ifndef GL_OES_VERSION_1_1 //maks
				SDL_CurrentVideo->glColor4f( 
					backgroundColor/*[j]*/.Redf(), 
					backgroundColor/*[j]*/.Greenf(), 
					backgroundColor/*[j]*/.Bluef(), 
					1.0 );

				SDL_CurrentVideo->glBegin( GL_QUADS );
				{
					SDL_CurrentVideo->glVertex3i( screenBounds/*[j]*/.xmin, screenBounds/*[j]*/.ymin, 0 );
					SDL_CurrentVideo->glVertex3i( screenBounds/*[j]*/.xmin + screenBounds/*[j]*/.Width(), screenBounds/*[j]*/.ymin, 0 );
					SDL_CurrentVideo->glVertex3i( screenBounds/*[j]*/.xmin + screenBounds/*[j]*/.Width(), screenBounds/*[j]*/.ymin + screenBounds/*[j]*/.Height(), 0 );
					SDL_CurrentVideo->glVertex3i( screenBounds/*[j]*/.xmin, screenBounds/*[j]*/.ymin + screenBounds/*[j]*/.Height(), 0 );
				}
				SDL_CurrentVideo->glEnd();
#else
				SDL_CurrentVideo->glColor4x( 
					GlES_DoubleToFixed(backgroundColor/*[j]*/.Redf()), 
					GlES_DoubleToFixed(backgroundColor/*[j]*/.Greenf()), 
					GlES_DoubleToFixed(backgroundColor/*[j]*/.Bluef()), 
					GlES_Fixed_1 );

				GLshort vertices[] = 
				{
					screenBounds/*[j]*/.xmin, screenBounds/*[j]*/.ymin, 0,  
					screenBounds/*[j]*/.xmin + screenBounds/*[j]*/.Width(), screenBounds/*[j]*/.ymin, 0,  
					screenBounds/*[j]*/.xmin + screenBounds/*[j]*/.Width(), screenBounds/*[j]*/.ymin + screenBounds/*[j]*/.Height(), 0, 
					screenBounds/*[j]*/.xmin, screenBounds/*[j]*/.ymin + screenBounds/*[j]*/.Height(), 0						
				};

				DrawOGLQuad(GL_TRIANGLE_STRIP, vertices, 3); //opengl:teste comment if using Draw2DStrip
#endif
			}

			bool clipping = ( screenBounds/*[j]*/ != windowBounds );

			if ( clipping )
			{
				SDL_CurrentVideo->glEnable(GL_CLIP_PLANE0);
				SDL_CurrentVideo->glEnable(GL_CLIP_PLANE1);
				SDL_CurrentVideo->glEnable(GL_CLIP_PLANE2);
				SDL_CurrentVideo->glEnable(GL_CLIP_PLANE3);

				
#ifndef GL_OES_VERSION_1_1 //maks
				double plane0[4] = { 1.0, 0.0, 0.0, -screenBounds/*[j]*/.xmin };
				double plane1[4] = { -1.0, 0.0, 0.0, (screenBounds/*[j]*/.xmin + screenBounds/*[j]*/.Width() ) };
				double plane2[4] = { 0.0, 1.0, 0.0, -screenBounds/*[j]*/.ymin };
				double plane3[4] = { 0.0, -1.0, 0.0, (screenBounds/*[j]*/.ymin + screenBounds/*[j]*/.Height() ) };

				SDL_CurrentVideo->glClipPlane( GL_CLIP_PLANE0, plane0 );
				SDL_CurrentVideo->glClipPlane( GL_CLIP_PLANE1, plane1 );
				SDL_CurrentVideo->glClipPlane( GL_CLIP_PLANE2, plane2 );
				SDL_CurrentVideo->glClipPlane( GL_CLIP_PLANE3, plane3 );
#else
				//maks don't exists in OpenGL ES Common-Lite profile
				/*float plane0[4] = { 1.0, 0.0, 0.0, -screenBounds.xmin };
				float plane1[4] = { -1.0, 0.0, 0.0, (screenBounds.xmin + screenBounds.Width() ) };
				float plane2[4] = { 0.0, 1.0, 0.0, -screenBounds.ymin };
				float plane3[4] = { 0.0, -1.0, 0.0, (screenBounds.ymin + screenBounds.Height() ) };

				SDL_CurrentVideo->glClipPlanef( GL_CLIP_PLANE0, plane0 );
				SDL_CurrentVideo->glClipPlanef( GL_CLIP_PLANE1, plane1 );
				SDL_CurrentVideo->glClipPlanef( GL_CLIP_PLANE2, plane2 );
				SDL_CurrentVideo->glClipPlanef( GL_CLIP_PLANE3, plane3 );*/
#endif
			}

			tree->DrawWalk( screenBounds/*[j]*/, &paintInfo/*, j*/ );

			if ( clipping )
			{
				SDL_CurrentVideo->glDisable(GL_CLIP_PLANE0);
				SDL_CurrentVideo->glDisable(GL_CLIP_PLANE1);
				SDL_CurrentVideo->glDisable(GL_CLIP_PLANE2);
				SDL_CurrentVideo->glDisable(GL_CLIP_PLANE3);
			}
			dirtyRectangle/*[j]*/.Clear();
		}

		//Draw2DStrip(); //opengl:teste
		
		updateRect = true; 

		#else
			// No openGl support, but openGl surface used
			GLASSERT( 0 );
		#endif
	}
	else
	{
		// Bitmap drawing.

		// Draw the background, if necessary. Then
		// do a draw walk for every DR.
		updateRect = 0; //maks
		SDL_PixelFormat *format = screen->format;

		//for( int win=0; win<nWindows; ++win  )
		{
			int nDirty = dirtyRectangle/*[win]*/.NumRect();

			for( int i=0; i< nDirty; ++i )
			{
				const KrRect& rect = dirtyRectangle/*[win]*/.Rect( i );

				// Draw the background.
				
				if ( fillBackground/*[win]*/ )
				{
					if(
						nDirty == 1 &&											
						rect == screenBounds/*[win]*/
					  ) 
					{
						if(backgroundColor/*[win]*/.c.red == 0 &&
							backgroundColor/*[win]*/.c.green == 0 &&
							backgroundColor/*[win]*/.c.blue == 0)
						{
							//Black full screen
							memset(screen->pixels, 0, screen->h * screen->pitch);
						}
						else if(format->BytesPerPixel == 2)
						{
							Clear16(screen, ( ( backgroundColor/*[win]*/.c.red >> format->Rloss )     << format->Rshift )
								| ( ( backgroundColor/*[win]*/.c.green >> format->Gloss ) << format->Gshift ) 
								| ( ( backgroundColor/*[win]*/.c.blue >> format->Bloss )   << format->Bshift));
						}
						else
						{
							SDL_Rect sdlrect = { rect.xmin, rect.ymin, rect.Width(), rect.Height() };
							U32 sdlColor = SDL_MapRGB( screen->format,	backgroundColor/*[win]*/.c.red,
								backgroundColor/*[win]*/.c.green,
								backgroundColor/*[win]*/.c.blue );
							//GLASSERT( sdlColor == 0 );
							SDL_FillRect( screen, &sdlrect, sdlColor );
						}
					}
					else
					{
						SDL_Rect sdlrect = { rect.xmin, rect.ymin, rect.Width(), rect.Height() };
						U32 sdlColor = SDL_MapRGB( screen->format,	backgroundColor/*[win]*/.c.red,
							backgroundColor/*[win]*/.c.green,
							backgroundColor/*[win]*/.c.blue );
						//GLASSERT( sdlColor == 0 );
						SDL_FillRect( screen, &sdlrect, sdlColor );
					}
				}
				tree->DrawWalk( rect, &paintInfo/*, win*/ );
				updateRect = 1;

				/*
				#ifdef DRAWDEBUG_BLTRECTS
					KrPainter painter( &paintInfo );
					painter.DrawBox( rect.xmin, rect.ymin, rect.Width(), rect.Height(), 200, 0, 0 );
				#endif
				*/
			}

			#ifdef DRAWDEBUG_BLTRECTS
				dirtyRectangle/*[win]*/.DrawRects( screen );
			#endif
			dirtyRectangle/*[win]*/.Clear();
		}		
	}

	} //unlock surface

	
	// The windows and DRs have been walked. Now transfer to physical screen.
	//Update here after unlock the surface
	if ( updateRect )
	{
		// Use the composite list of rectangles.
		UpdateScreen( rectArray );
	}	
}


void KrEngine::QueryRenderDesc( gedString* desc )
{
	QueryRenderDesc( screen, desc );
}


/* static */ void KrEngine::QueryRenderDesc( SDL_Surface* screen, gedString* desc )
{
	char buf[ 256 ];
	sprintf( buf, "v%d.%d.%d %dbbp Fullscreen=%d %s ",
					KyraVersionMajor, KyraVersionMinor, KyraVersionBuild,
					screen->format->BitsPerPixel,
					( screen->flags & SDL_FULLSCREEN ) ? 1 : 0,
					#ifdef WIN32
						"Win32"
					#elif defined ( linux )
						"Linux"
					#else
						"UnknownPlatform"
					#endif
		);

	char render[256];
	#ifdef KYRA_SUPPORT_OPENGL
	if ( screen->flags & SDL_OPENGL )
	{
		const unsigned char* vendor   = SDL_CurrentVideo->glGetString( GL_VENDOR );
		const unsigned char* renderer = SDL_CurrentVideo->glGetString( GL_RENDERER );
		const unsigned char* version  = SDL_CurrentVideo->glGetString( GL_VERSION );
		sprintf( render, "OpenGL render: Vendor: '%s'  Renderer: '%s'  Version: '%s'",
					vendor, renderer, version );
	} else
	#endif
	{
		sprintf( render, "Software render" );
	}
	*desc = buf;
	desc->append( render );
}


int KrEngine::GetWindowFromPoint( int x, int y )
{
	//for( int i=0; i<nWindows; ++i )
	{
		if ( screenBounds/*[i]*/.Intersect( x, y ) )
			return 0;
	}
	return -1;
}


void KrEngine::StartSplash( U32 msec )
{
#ifndef STAND_ALONE_GAME //maks
	splashStart = msec;

	// Use the bitmap functionality, so we can compile into the source
	// file with maximum reliable compression. This is really complex.
	// The only reason to do this is because of file size minimization.
	//

	GLASSERT( splashVault == 0 );

	splashVault = new KrResourceVault();
	splashVault->LoadDatFileFromMemory( splash_DAT, splash_SIZE );

	KrSpriteResource* splashRes     = splashVault->GetSpriteResource( "splash" );
	KrSpriteResource* splashTextRes = splashVault->GetSpriteResource( "splashText" );

	splash = new KrSprite( splashRes );
	splashText = new KrSprite( splashTextRes );

	KrRect bounds, boundsText;
	splash->QueryBoundingBox( &bounds );
	splashText->QueryBoundingBox( &boundsText );

	tree->AddNode( 0, splash );
	tree->AddNode( 0, splashText );

	splash->SetPos( screenBounds.Width() / 2  - bounds.Width() / 2,
					screenBounds.Height() / 4 - bounds.Height() / 2 );
	splash->SetZDepth( 63000 ); //maks

	splashText->SetPos( screenBounds.Width() / 2  - boundsText.Width() / 2,
						splash->Y() + bounds.Height() + 20 );

	splashText->SetZDepth( 63000 ); //maks

#endif //#ifndef STAND_ALONE_GAME
}


bool KrEngine::UpdateSplash( U32 msec )
{
#ifndef STAND_ALONE_GAME //maks
	U32 delta = msec - splashStart;

	KrColorTransform xcolor;

	if ( delta < 1000 )
	{
		xcolor.SetAlpha( 255 * delta / 1000 );
	}
	splash->SetColor( xcolor );
	splashText->SetColor( xcolor );

	return ( delta >= 2000 );
#else
	return true;
#endif
}


void KrEngine::EndSplash()
{
#ifndef STAND_ALONE_GAME //maks
	tree->DeleteNode( splash );
	tree->DeleteNode( splashText );
	delete splashVault;

	splash = 0;
	splashText = 0;
	splashVault = 0;
#endif
}


