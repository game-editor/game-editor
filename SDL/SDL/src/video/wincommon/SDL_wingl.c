/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/




#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_wingl.c,v 1.14 2004/01/04 16:49:26 slouken Exp $";
#endif

#include <string.h>

/* WGL implementation of SDL OpenGL support */

#ifdef HAVE_OPENGL
#include "SDL_opengl.h"
#endif
#include "SDL_error.h"
#include "SDL_lowvideo.h"
#include "SDL_wingl_c.h"

#ifdef HAVE_OPENGL
#define DEFAULT_GL_DRIVER_PATH "OPENGL32.DLL"
#endif


/* If setting the HDC fails, we may need to recreate the window (MSDN) */
static int WIN_GL_ResetWindow(_THIS)
{
	int status = 0;
	int can_reset = 1;

	/* If we were passed a window, then we can't create a new one */
	if ( SDL_windowid ) {
		can_reset = 0;
	}
#ifndef _WIN32_WCE /* FIXME WinCE needs the UNICODE version of CreateWindow() */
	if ( can_reset ) {
		/* Save the existing window attributes */
		LONG style;
		RECT rect = { 0, 0, 0, 0 };
		style = GetWindowLong(SDL_Window, GWL_STYLE);
		GetWindowRect(SDL_Window, &rect);
		DestroyWindow(SDL_Window);
		SDL_Window = CreateWindow(SDL_Appname, SDL_Appname,
		                          style,
					  rect.left, rect.top,
                                          (rect.right-rect.left)+1,
                                          (rect.top-rect.bottom)+1,
		                          NULL, NULL, SDL_Instance, NULL);
		if ( SDL_Window ) {
			this->SetCaption(this, this->wm_title, this->wm_icon);
		} else {
			SDL_SetError("Couldn't create window");
			status = -1;
		}
	} else
#endif /* !_WIN32_WCE */
	{
		SDL_SetError("Unable to reset window for OpenGL context");
		status = -1;
	}
	return(status);
}

#if defined(HAVE_OPENGL) && !defined(GL_OES_VERSION_1_1) //maks
static void Init_WGL_ARB_extensions(_THIS)
{
	HWND hwnd;
	HDC hdc;
	HGLRC hglrc;
	int pformat;
	const char * (GL_WINAPI *wglGetExtensionsStringARB)(HDC) = 0;
	
	hwnd = CreateWindow(SDL_Appname, SDL_Appname, WS_POPUP | WS_DISABLED,
	                    0, 0, 10, 10,
	                    NULL, NULL, SDL_Instance,NULL);
	hdc = GetDC(hwnd);

	pformat = ChoosePixelFormat(hdc, &GL_pfd);
	SetPixelFormat(hdc, pformat, &GL_pfd);

	hglrc = this->gl_data->wglCreateContext(hdc);
	if ( hglrc ) {
		this->gl_data->wglMakeCurrent(hdc, hglrc);
	}

	wglGetExtensionsStringARB = (const char * (GL_WINAPI *)(HDC))
		this->gl_data->wglGetProcAddress("wglGetExtensionsStringARB");

	if(wglGetExtensionsStringARB && strstr(wglGetExtensionsStringARB(hdc),"WGL_ARB_pixel_format")) {
		this->gl_data->wglChoosePixelFormatARB =
			(BOOL (GL_WINAPI *)(HDC, const int *, const FLOAT *, UINT, int *, UINT *))
			this->gl_data->wglGetProcAddress("wglChoosePixelFormatARB");
		this->gl_data->wglGetPixelFormatAttribivARB =
			(BOOL (GL_WINAPI *)(HDC, int, int, UINT, const int *, int *))
			this->gl_data->wglGetProcAddress("wglGetPixelFormatAttribivARB");

		if( (this->gl_data->wglChoosePixelFormatARB != NULL) &&
		    (this->gl_data->wglGetPixelFormatAttribivARB != NULL) )
			this->gl_data->wgl_arb_pixel_format = 1;
		else 
			this->gl_data->wgl_arb_pixel_format = 0;
	} else {
		this->gl_data->wgl_arb_pixel_format = 0;
	}
	
	if ( hglrc ) {
		this->gl_data->wglMakeCurrent(NULL, NULL);
		this->gl_data->wglDeleteContext(hglrc);
	}
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
}
#endif /* !HAVE_OPENGL */

#ifdef GL_OES_VERSION_1_1 //maks
/*
** GLW_InitDriver
**
** - get a DC if one doesn't exist
** - create an HGLRC if one doesn't exist
*/
int GLW_InitDriver(_THIS)
{	
	EGLConfig configs[10];
	EGLint matchingConfigs; 
	EGLint attribList12[] = { EGL_RED_SIZE,4,EGL_GREEN_SIZE,4,EGL_BLUE_SIZE,4, EGL_DEPTH_SIZE,16, EGL_NONE};
	EGLint attribList15[] = { EGL_RED_SIZE,5,EGL_GREEN_SIZE,5,EGL_BLUE_SIZE,5, EGL_DEPTH_SIZE,16, EGL_NONE};
	EGLint attribList16[] = { EGL_RED_SIZE,5,EGL_GREEN_SIZE,6,EGL_BLUE_SIZE,5, EGL_DEPTH_SIZE,16, EGL_NONE};
	EGLint attribList24[] = { EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8, EGL_DEPTH_SIZE,16, EGL_NONE};
	EGLint attribList32[] = { EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8, EGL_DEPTH_SIZE,16, EGL_NONE};
	EGLint noAttribList[] = { EGL_NONE };
	EGLint *pAttribList;
	EGLConfig config;
	
	int desktopBitsPixel;
	HDC hDC;	
  
	GL_hdc = 0;

	//Get bpp	
	hDC = GetDC( NULL );
	desktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
	ReleaseDC( NULL, hDC );

	if(desktopBitsPixel==12)
	{
		pAttribList=attribList12;
	}
	if(desktopBitsPixel==15)
	{
		pAttribList=attribList15;
	}
	else if(desktopBitsPixel==16)
	{
		pAttribList=attribList16;
	}
	else if(desktopBitsPixel==24)
	{
		pAttribList=attribList24;
	}
	else if(desktopBitsPixel==32)
	{
		pAttribList=attribList32;
	}
	else
	{
		return 0;
	}


	//Init OpenGL
	this->gl_data->glesDisplay=this->gl_data->eglGetDisplay(EGL_DEFAULT_DISPLAY);	
	
	if(!this->gl_data->glesDisplay) this->gl_data->glesDisplay=this->gl_data->eglGetDisplay(NULL);
	
	if(!this->gl_data->glesDisplay)
	{
		return 0;
	}
	
	this->gl_data->eglInitialize(this->gl_data->glesDisplay,NULL,NULL);	
	
	
	if(this->gl_data->eglChooseConfig(this->gl_data->glesDisplay,pAttribList,&(configs[0]),10,&matchingConfigs)==EGL_FALSE)
	{
		return 0;
	}
	
	if (matchingConfigs < 1) {
		return 0;
	}


	config = configs[0];	// pick any

	this->gl_data->glesSurface=this->gl_data->eglCreateWindowSurface(this->gl_data->glesDisplay,config,SDL_Window,NULL);
	if(this->gl_data->glesSurface==NULL)
	{
		return 0;
	}

	this->gl_data->glesContext=this->gl_data->eglCreateContext(this->gl_data->glesDisplay,config,EGL_NO_CONTEXT,&(noAttribList[0]));
	if(this->gl_data->glesContext==NULL)
	{
		return 0;
	}

	
	if(this->gl_data->eglMakeCurrent(this->gl_data->glesDisplay,this->gl_data->glesSurface,this->gl_data->glesSurface,this->gl_data->glesContext)!=EGL_TRUE)
	{	
		return 0;
	}

	return 1;
}
#endif


int WIN_GL_SetupWindow(_THIS)
{
	int retval;
#ifdef HAVE_OPENGL

	if(!this->gl_data || !gl_active) //maks
	{

#ifndef GL_OES_VERSION_1_1 //maks
	int i;
	unsigned int matching;
	int iAttribs[64];
	int *iAttr;
	float fAttribs[1] = { 0 };

	/* load the gl driver from a default path */
	if ( ! this->gl_config.driver_loaded ) {
		/* no driver has been loaded, use default (ourselves) */
		if ( WIN_GL_LoadLibrary(this, NULL) < 0 ) {
			return(-1);
		}
	}

	for ( i=0; ; ++i ) {
		/* Get the window device context for our OpenGL drawing */
		GL_hdc = GetDC(SDL_Window);
		if ( GL_hdc == NULL ) {
			SDL_SetError("Unable to get DC for SDL_Window");
			return(-1);
		}

		/* Set up the pixel format descriptor with our needed format */
		memset(&GL_pfd, 0, sizeof(GL_pfd));
		GL_pfd.nSize = sizeof(GL_pfd);
		GL_pfd.nVersion = 1;
		GL_pfd.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL);
		if ( this->gl_config.double_buffer ) {
			GL_pfd.dwFlags |= PFD_DOUBLEBUFFER;
		}
		if ( this->gl_config.stereo ) {
			GL_pfd.dwFlags |= PFD_STEREO;
		}
		GL_pfd.iPixelType = PFD_TYPE_RGBA;
		GL_pfd.cColorBits = this->gl_config.buffer_size;
		GL_pfd.cRedBits = this->gl_config.red_size;
		GL_pfd.cGreenBits = this->gl_config.green_size;
		GL_pfd.cBlueBits = this->gl_config.blue_size;
		GL_pfd.cAlphaBits = this->gl_config.alpha_size;
		GL_pfd.cAccumRedBits = this->gl_config.accum_red_size;
		GL_pfd.cAccumGreenBits = this->gl_config.accum_green_size;
		GL_pfd.cAccumBlueBits = this->gl_config.accum_blue_size;
		GL_pfd.cAccumAlphaBits = this->gl_config.accum_alpha_size;
		GL_pfd.cAccumBits =
			(GL_pfd.cAccumRedBits + GL_pfd.cAccumGreenBits +
			 GL_pfd.cAccumBlueBits + GL_pfd.cAccumAlphaBits);
		GL_pfd.cDepthBits = this->gl_config.depth_size;
		GL_pfd.cStencilBits = this->gl_config.stencil_size;

		/* initialize WGL_ARB_pixel_format */
		Init_WGL_ARB_extensions(this);

		/* setup WGL_ARB_pixel_format attribs */
		iAttr = &iAttribs[0];

		*iAttr++ = WGL_DRAW_TO_WINDOW_ARB;
		*iAttr++ = GL_TRUE;
		*iAttr++ = WGL_ACCELERATION_ARB;
		*iAttr++ = WGL_FULL_ACCELERATION_ARB;
		*iAttr++ = WGL_RED_BITS_ARB;
		*iAttr++ = this->gl_config.red_size;
		*iAttr++ = WGL_GREEN_BITS_ARB;
		*iAttr++ = this->gl_config.green_size;
		*iAttr++ = WGL_BLUE_BITS_ARB;
		*iAttr++ = this->gl_config.blue_size;
		
		if ( this->gl_config.alpha_size ) {
			*iAttr++ = WGL_ALPHA_BITS_ARB;
			*iAttr++ = this->gl_config.alpha_size;
		}

		if ( this->gl_config.double_buffer ) {
			*iAttr ++ = WGL_DOUBLE_BUFFER_ARB;
			*iAttr ++ = GL_TRUE;
		}

		*iAttr++ = WGL_DEPTH_BITS_ARB;
		*iAttr++ = this->gl_config.depth_size;

		if ( this->gl_config.stencil_size ) {
			*iAttr++ = WGL_STENCIL_BITS_ARB;
			*iAttr++ = this->gl_config.stencil_size;
		}

		if ( this->gl_config.accum_red_size ) {
			*iAttr++ = WGL_ACCUM_RED_BITS_ARB;
			*iAttr++ = this->gl_config.accum_red_size;
		}

		if ( this->gl_config.accum_green_size ) {
			*iAttr++ = WGL_ACCUM_GREEN_BITS_ARB;
			*iAttr++ = this->gl_config.accum_green_size;
		}

		if ( this->gl_config.accum_blue_size ) {
			*iAttr++ = WGL_ACCUM_BLUE_BITS_ARB;
			*iAttr++ = this->gl_config.accum_blue_size;
		}

		if ( this->gl_config.accum_alpha_size ) {
			*iAttr++ = WGL_ACCUM_ALPHA_BITS_ARB;
			*iAttr++ = this->gl_config.accum_alpha_size;
		}

		if ( this->gl_config.stereo ) {
			*iAttr++ = WGL_STEREO_ARB;
			*iAttr++ = this->gl_config.stereo;
		}

		if ( this->gl_config.multisamplebuffers ) {
			*iAttr++ = WGL_SAMPLE_BUFFERS_ARB;
			*iAttr++ = this->gl_config.multisamplebuffers;
		}

		if ( this->gl_config.multisamplesamples ) {
			*iAttr++ = WGL_SAMPLES_ARB;
			*iAttr++ = this->gl_config.multisamplesamples;
		}

		*iAttr = 0;

		/* Choose and set the closest available pixel format */
		if ( !this->gl_data->wgl_arb_pixel_format ||
		     !this->gl_data->wglChoosePixelFormatARB(GL_hdc, iAttribs, fAttribs, 1, &pixel_format, &matching) ||
		     !matching ) {
			pixel_format = ChoosePixelFormat(GL_hdc, &GL_pfd);
			this->gl_data->wgl_arb_pixel_format = 0;
		}
		if ( !pixel_format ) {
			SDL_SetError("No matching GL pixel format available");
			return(-1);
		}
		if ( !SetPixelFormat(GL_hdc, pixel_format, &GL_pfd) ) {
			if ( i == 0 ) {
				/* First time through, try resetting the window */
				if ( WIN_GL_ResetWindow(this) < 0 ) {
					return(-1);
				}
				continue;
			}
			SDL_SetError("Unable to set HDC pixel format");
			return(-1);
		}
		/* We either succeeded or failed by this point */
		break;
	}
	DescribePixelFormat(GL_hdc, pixel_format, sizeof(GL_pfd), &GL_pfd);

	GL_hrc = this->gl_data->wglCreateContext(GL_hdc);
	if ( GL_hrc == NULL ) {
		SDL_SetError("Unable to create GL context");
		return(-1);
	}
	gl_active = 1;

#else

	//OpenGL ES

  /* load the gl driver from a default path */
	if ( ! this->gl_config.driver_loaded ) {
		/* no driver has been loaded, use default (ourselves) */
		if ( WIN_GL_LoadLibrary(this, NULL) < 0 ) {
			return(-1);
		}
	}

  gl_active = GLW_InitDriver(this);

#endif

#else
	SDL_SetError("WIN driver not configured with OpenGL");
#endif

	}

	if ( gl_active ) {
		retval = 0;
	} else {
		retval = -1;
	}

	return(retval);
}

void WIN_GL_ShutDown(_THIS)
{
#ifdef HAVE_OPENGL

#ifndef GL_OES_VERSION_1_1 //maks
	/* Clean up OpenGL */
	if ( GL_hrc ) {
		this->gl_data->wglMakeCurrent(NULL, NULL);
		this->gl_data->wglDeleteContext(GL_hrc);
		GL_hrc = NULL;
	}
	
#else

	if(this->gl_data->glesDisplay)
	{
		this->gl_data->eglMakeCurrent(this->gl_data->glesDisplay, NULL, NULL, NULL);  
		if(this->gl_data->glesContext && this->gl_data->eglDestroyContext) this->gl_data->eglDestroyContext(this->gl_data->glesDisplay, this->gl_data->glesContext);
		if(this->gl_data->glesSurface) this->gl_data->eglDestroySurface(this->gl_data->glesDisplay, this->gl_data->glesSurface);
		if(this->gl_data->eglTerminate) this->gl_data->eglTerminate(this->gl_data->glesDisplay);
	} 

#endif

	if ( GL_hdc ) 
	{
		ReleaseDC(SDL_Window, GL_hdc);
		GL_hdc = NULL;
	}

	gl_active = 0;

	WIN_GL_UnloadLibrary(this);
#endif /* HAVE_OPENGL */
}

#ifdef HAVE_OPENGL

/* Make the current context active */
int WIN_GL_MakeCurrent(_THIS)
{
	int retval;

	retval = 0;

#ifdef GL_OES_VERSION_1_1 //maks
	if(!this->gl_data->eglMakeCurrent(this->gl_data->glesDisplay, this->gl_data->glesSurface, this->gl_data->glesSurface, this->gl_data->glesContext))
#else
	if ( ! this->gl_data->wglMakeCurrent(GL_hdc, GL_hrc) ) 
#endif
	{
		SDL_SetError("Unable to make GL context current");
		retval = -1;
	}

	return(retval);
}

/* Get attribute data from glX. */
int WIN_GL_GetAttribute(_THIS, SDL_GLattr attrib, int* value)
{
#ifndef GL_OES_VERSION_1_1 //maks
	int retval;
	
	if ( this->gl_data->wgl_arb_pixel_format ) {
		int wgl_attrib;

		switch(attrib) {
		    case SDL_GL_RED_SIZE:
			wgl_attrib = WGL_RED_BITS_ARB;
			break;
		    case SDL_GL_GREEN_SIZE:
			wgl_attrib = WGL_GREEN_BITS_ARB;
			break;
		    case SDL_GL_BLUE_SIZE:
			wgl_attrib = WGL_BLUE_BITS_ARB;
			break;
		    case SDL_GL_ALPHA_SIZE:
			wgl_attrib = WGL_ALPHA_BITS_ARB;
			break;
		    case SDL_GL_DOUBLEBUFFER:
			wgl_attrib = WGL_DOUBLE_BUFFER_ARB;
			break;
		    case SDL_GL_BUFFER_SIZE:
			wgl_attrib = WGL_COLOR_BITS_ARB;
			break;
		    case SDL_GL_DEPTH_SIZE:
			wgl_attrib = WGL_DEPTH_BITS_ARB;
			break;
		    case SDL_GL_STENCIL_SIZE:
			wgl_attrib = WGL_STENCIL_BITS_ARB;
			break;
		    case SDL_GL_ACCUM_RED_SIZE:
			wgl_attrib = WGL_ACCUM_RED_BITS_ARB;
			break;
		    case SDL_GL_ACCUM_GREEN_SIZE:
			wgl_attrib = WGL_ACCUM_GREEN_BITS_ARB;
			break;
		    case SDL_GL_ACCUM_BLUE_SIZE:
			wgl_attrib = WGL_ACCUM_BLUE_BITS_ARB;
			break;
		    case SDL_GL_ACCUM_ALPHA_SIZE:
			wgl_attrib = WGL_ACCUM_ALPHA_BITS_ARB;
			break;
		    case SDL_GL_STEREO:
			wgl_attrib = WGL_STEREO_ARB;
			break;
		    case SDL_GL_MULTISAMPLEBUFFERS:
			wgl_attrib = WGL_SAMPLE_BUFFERS_ARB;
			break;
		    case SDL_GL_MULTISAMPLESAMPLES:
			wgl_attrib = WGL_SAMPLES_ARB;
			break;
		    default:
			return(-1);
		}
		this->gl_data->wglGetPixelFormatAttribivARB(GL_hdc, pixel_format, 0, 1, &wgl_attrib, value);

		return 0;
	}

	retval = 0;
	switch ( attrib ) {
	    case SDL_GL_RED_SIZE:
		*value = GL_pfd.cRedBits;
		break;
	    case SDL_GL_GREEN_SIZE:
		*value = GL_pfd.cGreenBits;
		break;
	    case SDL_GL_BLUE_SIZE:
		*value = GL_pfd.cBlueBits;
		break;
	    case SDL_GL_ALPHA_SIZE:
		*value = GL_pfd.cAlphaBits;
		break;
	    case SDL_GL_DOUBLEBUFFER:
		if ( GL_pfd.dwFlags & PFD_DOUBLEBUFFER ) {
			*value = 1;
		} else {
			*value = 0;
		}
		break;
	    case SDL_GL_BUFFER_SIZE:
		*value = GL_pfd.cColorBits;
		break;
	    case SDL_GL_DEPTH_SIZE:
		*value = GL_pfd.cDepthBits;
		break;
	    case SDL_GL_STENCIL_SIZE:
		*value = GL_pfd.cStencilBits;
		break;
	    case SDL_GL_ACCUM_RED_SIZE:
		*value = GL_pfd.cAccumRedBits;
		break;
	    case SDL_GL_ACCUM_GREEN_SIZE:
		*value = GL_pfd.cAccumGreenBits;
		break;
	    case SDL_GL_ACCUM_BLUE_SIZE:
		*value = GL_pfd.cAccumBlueBits;
		break;
	    case SDL_GL_ACCUM_ALPHA_SIZE:
		*value = GL_pfd.cAccumAlphaBits;
		break;
	    case SDL_GL_STEREO:
		if ( GL_pfd.dwFlags & PFD_STEREO ) {
			*value = 1;
		} else {
			*value = 0;
		}
		break;
	    default:
		retval = -1;
		break;
	}
	return retval;
#else
	return -1;
#endif
}

extern int bSurfaceLost; //maks
void WIN_GL_SwapBuffers(_THIS)
{
	
#ifndef GL_OES_VERSION_1_1 //maks
	SwapBuffers(GL_hdc);
#else
	int ret = this->gl_data->eglSwapBuffers(this->gl_data->glesDisplay, this->gl_data->glesSurface);


	if(ret != EGL_TRUE) 
	{
		EGLint error = this->gl_data->eglGetError();

		if(error == 0x300E/*EGL_CONTEXT_LOST_IMG*/)
		{
			//Intel 2700G Power Management Extension
			//Destroy contexts
			bSurfaceLost = 1; //maks
		}
	}

#endif
}

void WIN_GL_UnloadLibrary(_THIS)
{
	if ( this->gl_config.driver_loaded ) 
	{
		FreeLibrary((HMODULE)this->gl_config.dll_handle);

		this->gl_data->wglGetProcAddress = NULL;

#ifndef GL_OES_VERSION_1_1 //maks		
		this->gl_data->wglCreateContext = NULL;
		this->gl_data->wglDeleteContext = NULL;
		this->gl_data->wglMakeCurrent = NULL;
		this->gl_data->wglChoosePixelFormatARB = NULL;
		this->gl_data->wglGetPixelFormatAttribivARB = NULL;
#endif

		this->gl_config.dll_handle = NULL;
		this->gl_config.driver_loaded = 0;
	}
}

/* Passing a NULL path means load pointers from the application */
int WIN_GL_LoadLibrary(_THIS, const char* path) 
{
	HMODULE handle;

	

 	if (this->gl_data && gl_active) { //maks
 		SDL_SetError("OpenGL context already created");
 		return -1;
 	}

#ifndef GL_OES_VERSION_1_1 //maks
	if ( path == NULL ) 
	{
		path = DEFAULT_GL_DRIVER_PATH;
	}

	handle = LoadLibrary(path);
#else

	//Try OpenGL ES currently installed (may be in hardware)
	handle = LoadLibrary(TEXT("libGLES_CM.dll"));

    if(!handle) handle = LoadLibrary(TEXT("libGLES_CL.dll"));


	//Try the vincent
	if(!handle) handle = LoadLibrary(TEXT("v_libGLES_CM.dll"));

#endif

	if ( handle == NULL ) 
	{
		SDL_SetError("Could not load OpenGL library");
		return -1;
	}

	/* Unload the old driver and reset the pointers */
	WIN_GL_UnloadLibrary(this);

	/* Load new function pointers */
	memset(this->gl_data, 0, sizeof(*this->gl_data));
	
#ifndef GL_OES_VERSION_1_1 //maks
	this->gl_data->wglGetProcAddress = (void * (GL_WINAPI *)(const char *))
		GetProcAddress(handle, "wglGetProcAddress");
	this->gl_data->wglCreateContext = (HGLRC (GL_WINAPI *)(HDC))
		GetProcAddress(handle, "wglCreateContext");
	this->gl_data->wglDeleteContext = (BOOL (GL_WINAPI *)(HGLRC))
		GetProcAddress(handle, "wglDeleteContext");
	this->gl_data->wglMakeCurrent = (BOOL (GL_WINAPI *)(HDC, HGLRC))
		GetProcAddress(handle, "wglMakeCurrent");	

#else
	this->gl_data->wglGetProcAddress = (void * (GL_WINAPI *)(const char *))
		GetProcAddress(handle, TEXT("eglGetProcAddress"));

	this->gl_data->eglMakeCurrent = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLSurface, EGLSurface, EGLContext))
		GetProcAddress(handle, TEXT("eglMakeCurrent"));

	this->gl_data->eglGetDisplay = (EGLDisplay (GL_WINAPI *)(NativeDisplayType))
		GetProcAddress(handle, TEXT("eglGetDisplay"));

	this->gl_data->eglCreateWindowSurface = (EGLSurface (GL_WINAPI *)(EGLDisplay, EGLConfig, NativeWindowType, const EGLint *))
		GetProcAddress(handle, TEXT("eglCreateWindowSurface"));

	this->gl_data->eglCreateContext = (EGLContext (GL_WINAPI *)(EGLDisplay, EGLConfig, EGLContext, const EGLint *))
		GetProcAddress(handle, TEXT("eglCreateContext"));

	this->gl_data->eglDestroyContext = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLContext))
		GetProcAddress(handle, TEXT("eglDestroyContext"));

	this->gl_data->eglDestroySurface = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLSurface))
		GetProcAddress(handle, TEXT("eglDestroySurface"));

	this->gl_data->eglTerminate = (EGLBoolean (GL_WINAPI *)(EGLDisplay))
		GetProcAddress(handle, TEXT("eglTerminate"));

	this->gl_data->eglSwapBuffers = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLSurface))
		GetProcAddress(handle, TEXT("eglSwapBuffers"));

	this->gl_data->eglCopyBuffers = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLSurface, NativePixmapType))
		GetProcAddress(handle, TEXT("eglCopyBuffers"));

	this->gl_data->eglInitialize = (EGLBoolean (GL_WINAPI *)(EGLDisplay, EGLint *, EGLint *))
		GetProcAddress(handle, TEXT("eglInitialize"));

	this->gl_data->eglChooseConfig = (EGLBoolean (GL_WINAPI *)(EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *))
		GetProcAddress(handle, TEXT("eglChooseConfig"));

	this->gl_data->eglGetError = (EGLint (GL_WINAPI *)(void))
		GetProcAddress(handle, TEXT("eglGetError"));

#endif

#ifndef GL_OES_VERSION_1_1 //maks
	if ( /*(this->gl_data->wglGetProcAddress == NULL) || */
		 (this->gl_data->wglCreateContext == NULL) ||
	     (this->gl_data->wglDeleteContext == NULL) ||
	     (this->gl_data->wglMakeCurrent == NULL) )
#else
	if ( /*(this->gl_data->wglGetProcAddress == NULL) ||*/ 
		 (this->gl_data->eglMakeCurrent == NULL) ||
	     (this->gl_data->eglGetDisplay == NULL) ||
	     (this->gl_data->eglCreateWindowSurface == NULL) ||
		 (this->gl_data->eglCreateContext == NULL) ||
		 /*(this->gl_data->eglDestroyContext == NULL) ||*/
		 (this->gl_data->eglDestroySurface == NULL) ||
		 /*(this->gl_data->eglTerminate == NULL) ||*/
		 (this->gl_data->eglSwapBuffers == NULL) ||
		 (this->gl_data->eglInitialize == NULL) /*||
		 (this->gl_data->eglChooseConfig == NULL)*/ )
#endif
		  
	{
		SDL_SetError("Could not retrieve OpenGL functions");
		FreeLibrary(handle);
		return -1;
	}

	this->gl_config.dll_handle = handle;

#ifndef GL_OES_VERSION_1_1 //maks
	strcpy(this->gl_config.driver_path, path);
#endif

	this->gl_config.driver_loaded = 1;
	return 0;
}

void *WIN_GL_GetProcAddress(_THIS, const char* proc)
{
	void *func = NULL;

	/* This is to pick up extensions */
	if(this->gl_data->wglGetProcAddress) func = this->gl_data->wglGetProcAddress(proc); //maks
	if ( ! func ) 
	{
		/* This is probably a normal GL function */

#ifndef _WIN32_WCE //maks
		func = GetProcAddress(this->gl_config.dll_handle, proc);

#else
		TCHAR uBuf[256];
		MultiByteToWideChar(CP_ACP, 0, proc, -1, uBuf, 255);		
		
		func = GetProcAddress(this->gl_config.dll_handle, uBuf);
#endif
	}	

	return func;
}

#ifdef GL_OES_VERSION_1_1 //maks
void DrawOGLTexQuad(GLenum mode, GLshort *vertices, GLfixed *texCoords, int nVerticeCoords)
{
	SDL_VideoDevice *this  = current_video;
	static GLubyte indices[] = {0,1,3,2};
	
	this->glEnableClientState(GL_VERTEX_ARRAY);
	this->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	this->glTexCoordPointer(2, GL_FIXED, 0, texCoords);
	this->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	this->glDrawElements(mode, 4, GL_UNSIGNED_BYTE, indices);
	
	this->glDisableClientState(GL_VERTEX_ARRAY); 
	this->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void DrawOGLQuad(GLenum mode, GLshort *vertices, int nVerticeCoords)
{
	SDL_VideoDevice *this  = current_video;
	static GLubyte indices[] = {0,1,3,2};
	
	this->glEnableClientState(GL_VERTEX_ARRAY);
	
	this->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	this->glDrawElements(mode, 4, GL_UNSIGNED_BYTE, indices);
	
	this->glDisableClientState(GL_VERTEX_ARRAY);  
}

void DrawOGLLine(GLshort *vertices, int nVerticeCoords)
{
	SDL_VideoDevice *this  = current_video;
	static GLubyte indices[] = {0,1};
	
	this->glEnableClientState(GL_VERTEX_ARRAY);
	
	this->glVertexPointer(nVerticeCoords, GL_SHORT, 0, vertices);
	this->glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, indices);
	
	this->glDisableClientState(GL_VERTEX_ARRAY);  
}


//Big strips for better performance

#include "../../../../../gameEngine/dlmalloc.h" 
#define MAX_SPRITES		1000 //opengl:teste
#define MAX_TEXTURES	100 //opengl:teste
#define MAX_VETEX_COUNT (MAX_SPRITES*4)

typedef struct 
{
	GLshort *stripIndex;
	GLshort *stripVertice;
	GLfixed *stripTexCoord;
	int vCount;

} StripCache;

GLuint nTexture = 0;
StripCache *stripCache = NULL;

void InitNew2DStrip()
{
	GLuint i;

	if(stripCache)
	{
		//Reset old values
		for(i = 0; i <= nTexture; i++)
		{
			stripCache[i].vCount = 0;
		}
	}
	else
	{
		stripCache = (StripCache *)dlmalloc(MAX_TEXTURES*sizeof(StripCache));
		memset(stripCache, 0, MAX_TEXTURES*sizeof(StripCache));
	}

	nTexture = 0;	
}

void Push2DQuadStrip(GLuint textureId, GLshort *vertices, GLfixed *texCoords)
{
	//Push a new quad strip
	//
	//                   0---1   4---5
	//                   |  /|  /|  /|
	//  tri-strip #1 ->  | / | / | / |  <- tri-strip #2
	//                   |/  |/  |/  |
	//                   3---2   7---6


	static GLshort indices[] = {0,3,1,2};
	int i;
	StripCache *pStripCache = &stripCache[textureId];

	if(nTexture < textureId) nTexture = textureId;
	
	//Need to alloc new texture cache?
	if(!pStripCache->stripIndex)
	{
		pStripCache->stripIndex = (GLshort *)dlmalloc(MAX_VETEX_COUNT*sizeof(GLshort));
	}
	
	if(!pStripCache->stripVertice)
	{
		pStripCache->stripVertice = (GLshort *)dlmalloc(3*MAX_VETEX_COUNT*sizeof(GLshort));
	}
	
	if(!pStripCache->stripTexCoord)
	{
		pStripCache->stripTexCoord = (GLfixed *)dlmalloc(2*MAX_VETEX_COUNT*sizeof(GLfixed));
		pStripCache->vCount = 0;
	}


	//No loop

	if(pStripCache->vCount != 0)
	{
		//Push degenerate triangle 2 4 4 7
		static GLshort indicesD[] = {2,4,4,7};
		int ib = pStripCache->vCount - 4;

		//Push 4 index
		i = pStripCache->vCount;

		pStripCache->stripIndex[0 + i] = indicesD[0] + ib;
		pStripCache->stripIndex[1 + i] = indicesD[1] + ib;
		pStripCache->stripIndex[2 + i] = indicesD[2] + ib;
		pStripCache->stripIndex[3 + i] = indicesD[3] + ib;

		//Push 4 vertices
		i = 3*pStripCache->vCount;

		//From vertex 2 of last strip
		pStripCache->stripVertice[0 + i] = pStripCache->stripVertice[(0 + i) - 2*3];
		pStripCache->stripVertice[1 + i] = pStripCache->stripVertice[(1 + i) - 2*3];
		pStripCache->stripVertice[2 + i] = pStripCache->stripVertice[(2 + i) - 2*3];

		//From first vertext of new strip
		pStripCache->stripVertice[3 + i] = vertices[0];
		pStripCache->stripVertice[4 + i] = vertices[1];
		pStripCache->stripVertice[5 + i] = vertices[2];

		//From first vertext of new strip
		pStripCache->stripVertice[6 + i] = vertices[0];
		pStripCache->stripVertice[7 + i] = vertices[1];
		pStripCache->stripVertice[8 + i] = vertices[2];

		//From first vertext of new strip
		pStripCache->stripVertice[9 + i] = vertices[0];
		pStripCache->stripVertice[10 + i] = vertices[1];
		pStripCache->stripVertice[11 + i] = vertices[2];

		/*//From last vertext of new strip
		pStripCache->stripVertice[9 + i] = vertices[9];
		pStripCache->stripVertice[10+ i] = vertices[10];
		pStripCache->stripVertice[11+ i] = vertices[11];*/


		//Push 4 texture coords
		i = 2*pStripCache->vCount;

		//From vertex 2 of last strip
		pStripCache->stripTexCoord[0 + i] = pStripCache->stripTexCoord[(0 + i) - 2*2];
		pStripCache->stripTexCoord[1 + i] = pStripCache->stripTexCoord[(0 + i) - 2*2];

		//From first vertext of new strip
		pStripCache->stripTexCoord[2 + i] = texCoords[0];
		pStripCache->stripTexCoord[3 + i] = texCoords[1];

		//From first vertext of new strip
		pStripCache->stripTexCoord[4 + i] = texCoords[0];
		pStripCache->stripTexCoord[5 + i] = texCoords[1];

		//From first vertext of new strip
		pStripCache->stripTexCoord[6 + i] = texCoords[0];
		pStripCache->stripTexCoord[7 + i] = texCoords[1];

		//From last vertext of new strip
		/*pStripCache->stripTexCoord[6 + i] = texCoords[6];
		pStripCache->stripTexCoord[7 + i] = texCoords[7];*/

		pStripCache->vCount += 4;
	}

	//Push 4 index
	i = pStripCache->vCount;

	pStripCache->stripIndex[0 + i] = indices[0] + pStripCache->vCount;
	pStripCache->stripIndex[1 + i] = indices[1] + pStripCache->vCount;
	pStripCache->stripIndex[2 + i] = indices[2] + pStripCache->vCount;
	pStripCache->stripIndex[3 + i] = indices[3] + pStripCache->vCount;

	//Push 4 vertices
	i = 3*pStripCache->vCount;

	pStripCache->stripVertice[0 + i] = vertices[0];
	pStripCache->stripVertice[1 + i] = vertices[1];
	pStripCache->stripVertice[2 + i] = vertices[2];

	pStripCache->stripVertice[3 + i] = vertices[3];
	pStripCache->stripVertice[4 + i] = vertices[4];
	pStripCache->stripVertice[5 + i] = vertices[5];

	pStripCache->stripVertice[6 + i] = vertices[6];
	pStripCache->stripVertice[7 + i] = vertices[7];
	pStripCache->stripVertice[8 + i] = vertices[8];

	pStripCache->stripVertice[9 + i] = vertices[9];
	pStripCache->stripVertice[10+ i] = vertices[10];
	pStripCache->stripVertice[11+ i] = vertices[11];

	//Push 4 texture coords
	i = 2*pStripCache->vCount;

	pStripCache->stripTexCoord[0 + i] = texCoords[0];
	pStripCache->stripTexCoord[1 + i] = texCoords[1];

	pStripCache->stripTexCoord[2 + i] = texCoords[2];
	pStripCache->stripTexCoord[3 + i] = texCoords[3];

	pStripCache->stripTexCoord[4 + i] = texCoords[4];
	pStripCache->stripTexCoord[5 + i] = texCoords[5];

	pStripCache->stripTexCoord[6 + i] = texCoords[6];
	pStripCache->stripTexCoord[7 + i] = texCoords[7];	

	//Update vertex count
	pStripCache->vCount += 4;
}

#define GlES_Fixed_1 (1<<16)
void Draw2DStrip()
{
	GLuint i;
	static GLfixed environmentColor[4] = { 0, 0, 0, GlES_Fixed_1 };
	SDL_VideoDevice *this  = current_video;	
	
	this->glEnableClientState(GL_VERTEX_ARRAY);
	this->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for(i = 0; i <= nTexture; i++)
	{
		StripCache *pStripCache = &stripCache[i];

		if(pStripCache->vCount)
		{
			//Draw whole strip	
			this->glBindTexture( GL_TEXTURE_2D, i );
			
			this->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			this->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			
			this->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Without this is 3x more slow
			this->glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			
		

			
			this->glTexEnvxv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, environmentColor );			
			this->glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			this->glColor4x( GlES_Fixed_1, GlES_Fixed_1, GlES_Fixed_1, GlES_Fixed_1 );
			
			this->glTexCoordPointer(2, GL_FIXED, 0, pStripCache->stripTexCoord);
			this->glVertexPointer(3, GL_SHORT, 0, pStripCache->stripVertice);
			this->glDrawElements(GL_TRIANGLE_STRIP, pStripCache->vCount, GL_UNSIGNED_SHORT, pStripCache->stripIndex);			
		}
	}

	this->glDisableClientState(GL_VERTEX_ARRAY); 
	this->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
}


#endif

#endif /* HAVE_OPENGL */

