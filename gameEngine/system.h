/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


/*******************************************************************************
  system.h: OS-independent functions (header file)
            Author: Odilon Nelson
            Last modified: 02/05/03
*******************************************************************************/

#ifndef GEDSYSTEM_H_INCLUDED  /* to avoid recursive inclusion */
#define GEDSYSTEM_H_INCLUDED

#if defined(__cplusplus) && __cplusplus
extern "C" {
#endif


//------------------------------
// Common defs to all OSes

#define GED_NOTFOUND  -1

#define A_RDONLY	  0x01	/* Read only attribute */
#define A_HIDDEN	  0x02	/* Hidden file */
#define A_SYSTEM	  0x04	/* System file */
#define A_DIREC	      0x08	/* Directory */
/* Matches any attribute */
#define A_ANY	      (A_RDONLY|A_HIDDEN|A_SYSTEM|A_DIREC)
/* This one matches any attribute, except hidden */
#define A_ANYVISIBLE  (A_RDONLY|A_SYSTEM|A_DIREC)

long ged_findfirst(const char *, int);
long ged_findnext(long);
void ged_findclose(long);
char *ged_filename(long); //Get filename from handle
long ged_filesize(long); //Get file size from handle
long ged_writetime(long); //Get file write time from handle
unsigned char ged_attrib(long); //Get file attribute from handle

void correctDirectoryName(const char *oldName, char *newName);

void openUrl(const char *url);
char* download(const char *url, long *bytesRead);
int isInternetConnected();


//------------------------------
// Symbian specific stuff
#ifdef __SYMBIAN32__

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE)
#define VIDEO_FLAGS (SDL_SWSURFACE | SDL_RESIZABLE /*| SDL_DOUBLEBUF | SDL_HWSURFACE/**/)

#define GED_TRY
#define	GED_CATCH	if(0)
#define GED_THROW 

#define DIR_SEP     '\\'
#define DEVICE_SEP  ':'

#define ged_mkdir(pathname)  mkdir(pathname, 0755)  //create dir with default user access
char *ged_getcwd( char *buffer, int maxlen );


#define STRUCT_PACK

//------------------------------
// MS Pocket PC specific stuff
#elif _WIN32_WCE

#include <windows.h>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)
#define VIDEO_FLAGS (SDL_HWSURFACE | SDL_OPENGL |/**/ SDL_FULLSCREEN/**/) //Don't use SDL_DOUBLEBUF and use SDL_HWSURFACE to don't slow down PocketHAL

#define GED_TRY		
#define	GED_CATCH	if(0)
#define GED_THROW	

#define DIR_SEP     '\\'
#define DEVICE_SEP  ':'

#define chdrive		_chdrive
#define getdrive	_getdrive
#define ged_mkdir	mkdir


#define STRUCT_PACK

#define O_RDONLY	0
#define O_BINARY	0
#define O_CREAT		0
#define O_WRONLY	0
#define O_APPEND	0

#undef RGBA
#undef RGB

typedef long off_t;

int chdir( const char *dirname );
char *ged_getcwd( char *buffer, int maxlen );
int ged_mkdir( const char *dirname );
int open( const char *filename, int oflag);
int close( int handle );
int read( int handle, void *buffer, unsigned int count );
int write( int handle, const void *buffer, unsigned int count );
int remove( const char *path );

//------------------------------
// MSWindows specific stuff
#elif WIN32
#define ___WIN32___


#include <direct.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)
#define VIDEO_FLAGS (SDL_SWSURFACE | SDL_RESIZABLE | SDL_OPENGL/**/ /*| SDL_DOUBLEBUF | SDL_HWSURFACE/**/)

#define GED_TRY		try	
#define	GED_CATCH	catch(...)
#define GED_THROW	throw(1);

#define DIR_SEP     '\\'
#define DEVICE_SEP  ':'

#define chdrive		_chdrive
#define getdrive	_getdrive
#define ged_mkdir	mkdir
char *ged_getcwd( char *buffer, int maxlen );

#define STRUCT_PACK

//------------------------------
// GP2X specific stuff
#elif GP2X

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>

#define O_BINARY	0

#ifndef FILENAME_MAX
#define FILENAME_MAX  1024
#endif

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK)
#define VIDEO_FLAGS (SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_CREATE_SHADOW) //You can use flip double buffer only if screen have whole update or a shadow surface

#define GED_TRY		
#define	GED_CATCH	if(0)
#define GED_THROW

#define DIR_SEP     '/'
#define DEVICE_SEP  '\0'

#define ged_mkdir(pathname)  mkdir(pathname, 0755)  //create dir with default user access
char *ged_getcwd( char *buffer, int maxlen );
int chdrive(int);
int getdrive(void);

#define STRUCT_PACK __attribute__ ((aligned (8))) //use in saved structs

//------------------------------
// Linux specific stuff
#else

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>

#define O_BINARY	0

#ifndef FILENAME_MAX
#define FILENAME_MAX  1024
#endif

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)
#define VIDEO_FLAGS (SDL_SWSURFACE | SDL_RESIZABLE | SDL_OPENGL/*| SDL_DOUBLEBUF | SDL_HWSURFACE/**/)

#define GED_TRY		try
#define	GED_CATCH	catch(...)
#define GED_THROW	throw(1);

#define DIR_SEP     '/'
#define DEVICE_SEP  '\0'

#define ged_mkdir(pathname)  mkdir(pathname, 0755)  //create dir with default user access
char *ged_getcwd( char *buffer, int maxlen );
int chdrive(int);
int getdrive(void);

#define STRUCT_PACK __attribute__ ((aligned (8))) //use in saved structs

//------------------------------
#endif
//------------------------------

#if defined(__cplusplus) && __cplusplus
} //extern "C"
#endif

#endif /* GEDSYSTEM_H_INCLUDED */
