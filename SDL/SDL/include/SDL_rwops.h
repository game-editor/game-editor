/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

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
 "@(#) $Id: SDL_rwops.h,v 1.5 2002/04/11 14:35:13 slouken Exp $";
#endif

/* This file provides a general interface for SDL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#ifndef _SDL_RWops_h
#define _SDL_RWops_h

#include <stdio.h>

#include "SDL_types.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This is the read/write operation structure -- very basic */

#define SDL_RW_TYPE_MEM 1

typedef struct SDL_RWops {
	/* Seek to 'offset' relative to whence, one of stdio's whence values:
		SEEK_SET, SEEK_CUR, SEEK_END
	   Returns the final offset in the data source.
	 */
	int (*seek)(struct SDL_RWops *context, int offset, int whence);

	/* Read up to 'num' objects each of size 'objsize' from the data
	   source to the area pointed at by 'ptr'.
	   Returns the number of objects read, or -1 if the read failed.
	 */
	int (*read)(struct SDL_RWops *context, void *ptr, int size, int maxnum);

	/* Write exactly 'num' objects each of size 'objsize' from the area
	   pointed at by 'ptr' to data source.
	   Returns 'num', or -1 if the write failed.
	 */
	int (*write)(struct SDL_RWops *context, const void *ptr, int size, int num);

	/* Close and free an allocated SDL_FSops structure */
	int (*close)(struct SDL_RWops *context);

	int (*Getc)(struct SDL_RWops *context); //maks
	int (*Putc)(struct SDL_RWops *context, int c); //maks

	Uint32 type;

	//maks: to control the concurrent access when use multiple files and stdio
	Uint32 filePosition;

	//maks: check with filePosition to make sure the file is in the right pos (changed by stdio functions only)
	Uint32 stdioFilePosition;

	union {
	    struct {
		int autoclose;
	 	FILE *fp;
	    } stdio;

	    struct 
		{
			Uint8 *base;
	 		Uint8 *here;
			Uint8 *stop;

			//maks: memory mapped files		
			void *hFile; //HANDLE
			void *hMapping; //HANDLE
			
			//maks:auto expand mem
			Uint8 bAutoExpand;
	    } mem;

	    struct {
		void *data1;
	    } unknown;

		//maks: multiple archive file
		struct
		{
			Uint32 base; //first file -> base = 0 (relative to baseFile)
			Uint32 here; // base <= here <= stop
			Uint32 stop; //final offset (relative to baseFile)
		} multiple;		

		//maks: Win32 data to unicode
		/*struct
		{
			void *handle; //HANDLE
		} win32;*/

	} hidden;

} SDL_RWops;


//maks: multiple archive file
typedef struct SDL_RWIndex 
{
	Uint32 offset;
	Uint32 size;

	char *fileName;

	void *next;
} SDL_RWIndex;

extern DECLSPEC void SDLCALL SDL_RWSetMultipleArchiveFile(SDL_RWops *src, Uint32 base, SDL_RWIndex *index);

//maks: Memory mapped files
extern DECLSPEC SDL_RWops * SDLCALL SDL_RWMemoryMapFile(const char *file, const char *mode);




/* Functions to create SDL_RWops structures from various data sources */

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFile(const char *file, const char *mode);
//extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFileUnicode(const Uint16 *file, const char *mode); //maks

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFP(FILE *fp, int autoclose);

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromMem(void *mem, int size);
extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromConstMem(const void *mem, int size);
extern DECLSPEC SDL_RWops * SDLCALL SDL_AllocRW(void);
extern DECLSPEC void SDLCALL SDL_FreeRW(SDL_RWops *area);

extern void mem_expand(SDL_RWops *context, int newSize); //maks

/* Macros to easily read and write from an SDL_RWops structure */
#define SDL_RWseek(ctx, offset, whence)	(ctx)->seek(ctx, offset, whence)
#define SDL_RWtell(ctx)			(ctx)->seek(ctx, 0, SEEK_CUR)
#define SDL_RWread(ctx, ptr, size, n)	(ctx)->read(ctx, ptr, size, n)
#define SDL_RWwrite(ctx, ptr, size, n)	(ctx)->write(ctx, ptr, size, n)
#define SDL_RWclose(ctx)		(ctx)->close(ctx)

#define SDL_RWgetc(ctx)		(ctx)->Getc(ctx) //maks
#define SDL_RWputc(ctx, c)		(ctx)->Putc(ctx, c) //maks


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_RWops_h */
