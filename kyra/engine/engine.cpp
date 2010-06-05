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
#ifdef __APPLE__
#include "SDL_sysvideo.h"
#ifdef __iPhone__
	#include "SDL_opengles.h" // AKR
#endif
#ifdef __MACOSX__
#include "SDL_opengl.h" // AKR
#endif
#else
	#include "SDL_opengl.h" // AKR
#endif
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
#if __iPhone__	
    if (w  > h) // AKR, switch to landscape
	{
		SDL_CurrentVideo->glViewport(0, 0, h, w);
		SDL_CurrentVideo->glRotatef(-90, 0, 0, 1);
	}
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0); //AKR better performance settings
	SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING,1);
#endif	
	
	SDL_CurrentVideo->glOrthox( 0, GlES_IntToFixed(w),
			 GlES_IntToFixed(h), 0, 
			 GlES_IntToFixed(-kKrOglDistance), GlES_IntToFixed(kKrOglDistance) );
#endif

	SDL_CurrentVideo->glMatrixMode(GL_MODELVIEW);
	SDL_CurrentVideo->glLoadIdentity();
	//SDL_CurrentVideo->glRotatef(GlES_DoubleToFixed(-90.0f), GlES_DoubleToFixed(0.0f), GlES_DoubleToFixed(1.0f),GlES_DoubleToFixed( 0.0f));

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
#if defined(DEBUG) && defined(__APPLE__)
	GLint maxText;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxText);

	GLOUTPUT("OpenGL MaxTextureSize: %d\n",maxText);
#endif	
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

				
				//There is a crash here in some Windows machines. 
				//Before the execution of the glBegin call there is not wrong with the memory. 
				//After that, the memory state will be wrong and the engine will crash.
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
					#elif defined ( __iPhone__ )
						"iPhone"
					#elif defined ( __MACOSX__ )
						"Macosx"
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
#ifdef __iPhone__
void DrawOGLTexQuad(GLenum mode, GLshort *vertices, GLfixed *texCoords, int nVerticeCoords)
{
	SDL_VideoDevice *curr  = current_video;
	static GLubyte indices[] = {0,1,3,2};
	
	curr->glEnableClientState(GL_VERTEX_ARRAY);
	curr->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	curr->glTexCoordPointer(2, GL_FIXED, 0, texCoords);
	curr->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	curr->glDrawElements(mode, 4, GL_UNSIGNED_BYTE, indices);
	
	curr->glDisableClientState(GL_VERTEX_ARRAY);
	curr->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void DrawOGLQuad(GLenum mode, GLshort *vertices, int nVerticeCoords)
{
	SDL_VideoDevice *curr  = current_video;
	static GLubyte indices[] = {0,1,3,2};
	
	curr->glEnableClientState(GL_VERTEX_ARRAY);
	
	curr->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	curr->glDrawElements(mode, 4, GL_UNSIGNED_BYTE, indices);
	
	curr->glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawOGLLine(GLshort *vertices, int nVerticeCoords)
{
	SDL_VideoDevice *curr  = current_video;
	static GLubyte indices[] = {0,1};
	
	curr->glEnableClientState(GL_VERTEX_ARRAY);
	
	curr->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	curr->glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, indices);
	
	curr->glDisableClientState(GL_VERTEX_ARRAY);
}
#endif
