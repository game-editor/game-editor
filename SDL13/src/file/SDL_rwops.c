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
 "@(#) $Id: SDL_rwops.c,v 1.5 2002/03/06 11:23:02 slouken Exp $";
#endif

/* This file provides a general interface for SDL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL_error.h"
#include "SDL_rwops.h"
#include "SDL_endian.h"
//#include "../../../gameEngine/dlmalloc.h" //maks


////////////////////////////////////////////////////////////////////////
//maks: memory mapped files
//Use only in PocketPC (no additional memory to map)
//Ready only functions

#if /*defined(WIN32) ||*/ defined(_WIN32_WCE)
#include <windows.h>

static int mapfile_close(SDL_RWops *context)
{
	if ( context ) 
	{
		UnmapViewOfFile(context->hidden.mem.base);
		CloseHandle(context->hidden.mem.hMapping);
		CloseHandle(context->hidden.mem.hFile);

		free(context);
	}

	return(0);
}

SDL_RWops *SDL_RWMemoryMapFile(const char *file, const char *mode)
{
	SDL_RWops *srcMapFile = NULL, *auxFile = NULL;
	HANDLE hFile = NULL, hMapping = NULL;
	char *mapBuffer = NULL;
	int fileSize = 0;

	//Adjust unicode
#ifdef UNICODE
	TCHAR uBuf[256];
	MultiByteToWideChar(CP_ACP, 0, file, -1, uBuf, 255);
#else
	const char *uBuf = file;
#endif


	//Get the file size
	auxFile = SDL_RWFromFile(file, mode);
	if(auxFile)
	{
		SDL_RWseek( auxFile, 0, SEEK_END ); //tell error without this
		fileSize = SDL_RWtell( auxFile );
		SDL_RWclose(auxFile);
		
		if(fileSize)
		{			
			//Open file for map
			
#ifdef _WIN32_WCE
			hFile = CreateFileForMapping(uBuf, GENERIC_READ,0, NULL, OPEN_EXISTING, 0, NULL);
#else
			hFile = CreateFile(uBuf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);			
#endif
			
			if(hFile && hFile != INVALID_HANDLE_VALUE )
			{
				//Create map
				
				hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
				if(hMapping && hMapping != INVALID_HANDLE_VALUE )
				{
					//Map whole file (don't allocate memory on PocketPC)
					mapBuffer = (char *)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
					
					if(mapBuffer)
					{
						srcMapFile = SDL_RWFromMem(mapBuffer, fileSize);
						if(srcMapFile)
						{
							srcMapFile->hidden.mem.hFile = hFile;
							srcMapFile->hidden.mem.hMapping = hMapping;
							srcMapFile->close = mapfile_close;
						}
						else
						{
							UnmapViewOfFile(mapBuffer);
							CloseHandle(hMapping);
							CloseHandle(hFile);
						}
					}
					else
					{			
						CloseHandle(hMapping);
						CloseHandle(hFile);		
					}			
				}
				else
				{		
					CloseHandle(hFile);		
				}
			}
		}
	}


	if(!srcMapFile)
	{
		//Open in normal way
		return SDL_RWFromFile(file, mode);
	}

	srcMapFile->hidden.mem.bAutoExpand = 0;
	return srcMapFile;
}
#else
SDL_RWops *SDL_RWMemoryMapFile(const char *file, const char *mode)
{
	//No memory mapped files in this system
	return SDL_RWFromFile(file, mode);
}
#endif

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//maks: multiple archive file
//Ready only functions
//Sequencial search

#include "SDL_mutex.h"

Uint8 multipleFile = 0; 
Uint32 baseFile = 0;
Uint32 openFileCount = 0;
Uint32 fileLenght = 0;
SDL_RWops *archive = NULL;
SDL_RWIndex *indexArchive = NULL;
SDL_mutex *multipleArchiveMutEx = NULL;
SDL_RWops *current = NULL;
int bInAMultipleFuncion = 0;


void ClearMultipleArchiveFile()
{
	if(indexArchive)
	{
		SDL_RWIndex *index = indexArchive;

		while(index)
		{
			SDL_RWIndex *del = index;
			index = (SDL_RWIndex *)index->next;

			free(del->fileName);
			free(del);
		}
	}

	indexArchive = NULL;
}

void SDL_RWSetMultipleArchiveFile(SDL_RWops *src, Uint32 base, SDL_RWIndex *index)
{
	//file ini, ......... base (multiplefiles begin) file1file2file3...
	Uint32 pos;

	ClearMultipleArchiveFile();
	
	multipleFile = 1;
	baseFile = base;
	openFileCount = 0;
	indexArchive = index;

	archive = src;


	//Get file lenght
	pos = SDL_RWtell( src ); //Save actual position
	SDL_RWseek( src, 0, SEEK_END );

	fileLenght = SDL_RWtell( src );

	//Restore file pointer
	SDL_RWseek( src, pos, SEEK_SET );
}



/* Functions to read mutiple archive file */

static int multiple_seek(SDL_RWops *context, int offset, int whence)
{
	Uint32 newpos = 0;

	switch (whence) 
	{
		case SEEK_SET:
			newpos = context->hidden.multiple.base+offset;
			break;
		case SEEK_CUR:
			newpos = context->hidden.multiple.here+offset;
			break;
		case SEEK_END:
			newpos = context->hidden.multiple.stop+offset;
			break;
		default:
			SDL_SetError("Unknown value for 'whence'");
			return(-1);
	}

	SDL_mutexP(multipleArchiveMutEx);
	bInAMultipleFuncion = 1;

	if ( newpos < context->hidden.multiple.base ) 	
	{
		newpos = context->hidden.multiple.base;
	}

	if ( newpos > context->hidden.multiple.stop ) 
	{
		newpos = context->hidden.multiple.stop;
	}

	if( newpos > fileLenght )
	{
		newpos = fileLenght;
	}

	context->hidden.multiple.here = newpos;

	
	SDL_RWseek(archive, baseFile + context->hidden.multiple.here, SEEK_SET);
	archive->filePosition = baseFile + context->hidden.multiple.here;

	if(current != context)
	{
		current = context;
	}

	bInAMultipleFuncion = 0;
	SDL_mutexV(multipleArchiveMutEx);

	return context->hidden.multiple.here - context->hidden.multiple.base;
}


static int multiple_read(SDL_RWops *context, void *ptr, int size, int maxnum)
{
	int num = maxnum, nread;

	SDL_mutexP(multipleArchiveMutEx);
	bInAMultipleFuncion = 1;

	
	if ( (context->hidden.multiple.here + (num*size)) > context->hidden.multiple.stop ) 
	{
		num = (context->hidden.multiple.stop - context->hidden.multiple.here)/size;
	}	

	//Slow
	/*if((baseFile + context->hidden.multiple.here) != (Uint32)SDL_RWtell( archive ))
	{
		SDL_RWseek(archive, baseFile + context->hidden.multiple.here, SEEK_SET);
	}*/

	//Fast	
	if(current != context || archive->filePosition != (baseFile + context->hidden.multiple.here))
	{
		SDL_RWseek(archive, baseFile + context->hidden.multiple.here, SEEK_SET);
		archive->filePosition = baseFile + context->hidden.multiple.here;
		current = context;		
	}

	nread = SDL_RWread(archive, ptr, size, num);
	context->hidden.multiple.here += nread*size;
	archive->filePosition += nread*size;

	bInAMultipleFuncion = 0;
	SDL_mutexV(multipleArchiveMutEx);

	return nread;
}

static int multiple_write(SDL_RWops *context, const void *ptr, int size, int num)
{
	//No writes
	return 0;
}

static int multiple_getc(SDL_RWops *context)
{
	//No getc
	return EOF;
}

static int multiple_putc(SDL_RWops *context, int c)
{
	//No putc
	return EOF;
}

static int multiple_close(SDL_RWops *context)
{
	if(context == current) 
		current = NULL;

	free(context);
	if(openFileCount) openFileCount--;

	return(0);
}

int equal(const char a, const char b)
{
	if(a == b) 
		return 1;

	if((a == '/' || a == '\\') &&
		(b == '/' || b == '\\'))
		return 1;

	return 0;
}

int dirStrcmp(const char *a, const char *b)
{
  const char *ra = a;
  const char *rb = b;

  while (equal(*ra, *rb)) 
  {
    if (!*ra++)
      return 0;
    else
      ++rb;
  }
  return (*ra - *rb);
}


SDL_RWops *SDL_RWFromMultipleArchive(const char *file, const char *mode)
{
	SDL_RWops *rwops;
	SDL_RWIndex *index = indexArchive;

	if(!multipleFile || !file || mode[0] != 'r' || mode[1] != 'b') //Read binary
		return NULL;

	
	//Search file in table index
	while(index)
	{
		if(dirStrcmp(index->fileName, file) == 0)
		{
			break;
		}

		index = index->next;
	}

	if(!index) return NULL;

	if(!multipleArchiveMutEx) 
	{
		multipleArchiveMutEx = SDL_CreateMutex();
	}


	rwops = SDL_AllocRW();

	if ( rwops != NULL ) 
	{
		rwops->seek = multiple_seek;
		rwops->read = multiple_read;
		rwops->write = multiple_write;
		rwops->close = multiple_close;
		rwops->Getc = multiple_getc;
		rwops->Putc = multiple_putc;
		rwops->hidden.multiple.base = index->offset;
		rwops->hidden.multiple.here = rwops->hidden.multiple.base;
		rwops->hidden.multiple.stop = rwops->hidden.multiple.base + index->size;
	}

	openFileCount++;
	return(rwops);
}
////////////////////////////////////////////////////////////////////////


/* Functions to read/write stdio file pointers */
 
static int stdio_seek(SDL_RWops *context, int offset, int whence) //maks
{
	int res = -1;
	if(!multipleArchiveMutEx) 
	{
		multipleArchiveMutEx = SDL_CreateMutex();
	}
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{
		current = context;

		if(context->stdioFilePosition != context->filePosition && whence != SEEK_SET)
		{
			//Position changed by a multiple file call
			//Restore
			fseek(context->hidden.stdio.fp, context->stdioFilePosition, SEEK_SET);
			context->filePosition = context->stdioFilePosition;
		}
	}

	if ( fseek(context->hidden.stdio.fp, offset, whence) == 0 ) 
	{
		res = ftell(context->hidden.stdio.fp);

		if(!bInAMultipleFuncion)
		{
			context->stdioFilePosition = context->filePosition = res;
		}
	} 
	else 
	{
		SDL_Error(SDL_EFSEEK);
		res = -1;
	}

	/*if(!bInAMultipleFuncion && context == archive)
	{
		SDL_mutexV(multipleArchiveMutEx);		
	}*/

	SDL_mutexV(multipleArchiveMutEx);
	return res;
}

static int stdio_read(SDL_RWops *context, void *ptr, int size, int maxnum) //maks
{
	size_t nread;
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			fseek(context->hidden.stdio.fp, context->stdioFilePosition, SEEK_SET);
			context->filePosition = context->stdioFilePosition;
		}
	}

	nread = fread(ptr, size, maxnum, context->hidden.stdio.fp); 
	if ( nread == 0 && ferror(context->hidden.stdio.fp) ) 
	{
		SDL_Error(SDL_EFREAD);
	}
	else
	{
		if(!bInAMultipleFuncion)
		{
			context->filePosition += nread*size;
			context->stdioFilePosition = context->filePosition;
		}
		
	}

	/*if(!bInAMultipleFuncion && context == archive)
	{
		SDL_mutexV(multipleArchiveMutEx);
	}*/

	SDL_mutexV(multipleArchiveMutEx);
	return(nread);
}

static int stdio_write(SDL_RWops *context, const void *ptr, int size, int num) //maks
{
	size_t nwrote;
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			fseek(context->hidden.stdio.fp, context->stdioFilePosition, SEEK_SET);
			context->filePosition = context->stdioFilePosition;
		}
	}

	nwrote = fwrite(ptr, size, num, context->hidden.stdio.fp);
	if ( nwrote == 0 && ferror(context->hidden.stdio.fp) ) {
		SDL_Error(SDL_EFWRITE);
	}
	else
	{
		//maks flush the stream to see all saved data
		//if a crash happens
		fflush( context->hidden.stdio.fp );

		if(!bInAMultipleFuncion)
		{
			context->filePosition += nwrote*size;
			context->stdioFilePosition = context->filePosition;
		}
		
	}

	/*if(!bInAMultipleFuncion && context == archive)
	{
		SDL_mutexV(multipleArchiveMutEx);
	}*/

	SDL_mutexV(multipleArchiveMutEx);
	return(nwrote);
}

static int stdio_getc(SDL_RWops *context) //maks
{
	int res;
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			fseek(context->hidden.stdio.fp, context->stdioFilePosition, SEEK_SET);
			context->filePosition = context->stdioFilePosition;
		}
	}

	res = getc(context->hidden.stdio.fp);

	if(!bInAMultipleFuncion)
	{
		context->filePosition++;
		context->stdioFilePosition = context->filePosition;
	}
	

	/*if(!bInAMultipleFuncion && context == archive)
	{
		SDL_mutexV(multipleArchiveMutEx);
	}*/

	SDL_mutexV(multipleArchiveMutEx);
	return res;
}

static int stdio_putc(SDL_RWops *context, int c) //maks
{
	int res;
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			fseek(context->hidden.stdio.fp, context->stdioFilePosition, SEEK_SET);
			context->filePosition = context->stdioFilePosition;
		}
	}

	res = putc(c, context->hidden.stdio.fp);

	if(!bInAMultipleFuncion)
	{
		context->filePosition++;
		context->stdioFilePosition = context->filePosition;
	}

	/*if(!bInAMultipleFuncion && context == archive)
	{
		SDL_mutexV(multipleArchiveMutEx);
	}*/

	SDL_mutexV(multipleArchiveMutEx);
	return res;
}

static int stdio_close(SDL_RWops *context)
{
	SDL_mutexP(multipleArchiveMutEx);

	if ( context ) 
	{
		if ( context->hidden.stdio.autoclose ) 
		{
			/* WARNING:  Check the return value here! */
			fclose(context->hidden.stdio.fp);

			if(archive == context)
			{
				archive = NULL;
			}
		}
		free(context);
	}

	SDL_mutexV(multipleArchiveMutEx);
	return(0);
}

void mem_expand(SDL_RWops *context, int newSize) //maks
{
	int pos = context->hidden.mem.here - context->hidden.mem.base;

	context->hidden.mem.base = (Uint8 *)realloc(context->hidden.mem.base, newSize);
	context->hidden.mem.here = context->hidden.mem.base + pos;
	context->hidden.mem.stop = context->hidden.mem.base + newSize;

	if(context->hidden.mem.here > context->hidden.mem.stop)
	{
		context->hidden.mem.here = context->hidden.mem.stop;
	}
}

/* Functions to read/write memory pointers */

static int mem_seek(SDL_RWops *context, int offset, int whence) //maks
{
	Uint8 *newpos;
	int pos;

	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{
		current = context;

		if(context->stdioFilePosition != context->filePosition && whence != SEEK_SET)
		{
			//Position changed by a multiple file call
			//Restore
			context->hidden.mem.here = context->hidden.mem.base + context->stdioFilePosition;
			context->filePosition = context->stdioFilePosition;
		}
	}

	switch (whence) {
		case SEEK_SET:
			newpos = context->hidden.mem.base+offset;
			break;
		case SEEK_CUR:
			newpos = context->hidden.mem.here+offset;
			break;
		case SEEK_END:
			newpos = context->hidden.mem.stop+offset;
			break;
		default:
			{
				SDL_SetError("Unknown value for 'whence'");
				SDL_mutexV(multipleArchiveMutEx);
				return(-1);
			}
	}
	if ( newpos < context->hidden.mem.base ) {
		newpos = context->hidden.mem.base;
	}
	
	if ( newpos > context->hidden.mem.stop ) //maks
	{
		if(!context->hidden.mem.bAutoExpand)
		{
			newpos = context->hidden.mem.stop;
		}
		else
		{
			int pos = newpos - context->hidden.mem.base;
			mem_expand(context, newpos - context->hidden.mem.base);
			newpos = context->hidden.mem.base + pos;
		}
	}

	context->hidden.mem.here = newpos;
	pos = context->hidden.mem.here - context->hidden.mem.base;

	if(!bInAMultipleFuncion)
	{
		context->stdioFilePosition = context->filePosition = pos;
	}

	SDL_mutexV(multipleArchiveMutEx);

	return pos;
}
static int mem_read(SDL_RWops *context, void *ptr, int size, int maxnum) //maks
{
	int num;

	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			context->hidden.mem.here = context->hidden.mem.base + context->stdioFilePosition;
			context->filePosition = context->stdioFilePosition;			
		}
	}
	

	num = maxnum;
	if ( (context->hidden.mem.here + (num*size)) > context->hidden.mem.stop ) 
	{
		num = (context->hidden.mem.stop-context->hidden.mem.here)/size;
	}

	memcpy(ptr, context->hidden.mem.here, num*size);
	context->hidden.mem.here += num*size;

	if(!bInAMultipleFuncion)
	{
		context->filePosition += num*size;
		context->stdioFilePosition = context->filePosition;
	}

	SDL_mutexV(multipleArchiveMutEx);
	return(num);
}
static int mem_write(SDL_RWops *context, const void *ptr, int size, int num) //maks
{
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			context->hidden.mem.here = context->hidden.mem.base + context->stdioFilePosition;
			context->filePosition = context->stdioFilePosition;			
		}
	}

	if ( (context->hidden.mem.here + (num*size)) > context->hidden.mem.stop ) //maks
	{
		if(!context->hidden.mem.bAutoExpand)
		{
			num = (context->hidden.mem.stop-context->hidden.mem.here)/size;
		}
		else
		{
			mem_expand(context, (context->hidden.mem.here + (num*size)) - context->hidden.mem.base);
		}
	}

	memcpy(context->hidden.mem.here, ptr, num*size);
	context->hidden.mem.here += num*size;

	if(!bInAMultipleFuncion)
	{
		context->filePosition += num*size;
		context->stdioFilePosition = context->filePosition;
	}

	SDL_mutexV(multipleArchiveMutEx);
	return(num);
}

static int mem_getc(SDL_RWops *context)
{
	int c;

	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			context->hidden.mem.here = context->hidden.mem.base + context->stdioFilePosition;
			context->filePosition = context->stdioFilePosition;			
		}
	}

	if ( (context->hidden.mem.here + 1) > context->hidden.mem.stop ) 
	{
		SDL_mutexV(multipleArchiveMutEx);
		return EOF;
	}

	c = *context->hidden.mem.here;
	context->hidden.mem.here++;

	if(!bInAMultipleFuncion)
	{
		context->filePosition++;
		context->stdioFilePosition = context->filePosition;
	}

	SDL_mutexV(multipleArchiveMutEx);
	return c;
}

static int mem_putc(SDL_RWops *context, int c)
{
	SDL_mutexP(multipleArchiveMutEx);

	if(!bInAMultipleFuncion && context == archive)
	{		
		current = context;

		if(context->stdioFilePosition != context->filePosition)
		{
			//Position changed by a multiple file call
			//Restore
			context->hidden.mem.here = context->hidden.mem.base + context->stdioFilePosition;
			context->filePosition = context->stdioFilePosition;			
		}
	}

	if ( (context->hidden.mem.here + 1) > context->hidden.mem.stop ) //maks
	{
		if(!context->hidden.mem.bAutoExpand)
		{
			SDL_mutexV(multipleArchiveMutEx);
			return EOF;
		}
		else
		{
			mem_expand(context, (context->hidden.mem.here + 1) - context->hidden.mem.base);
		}
	}

	*context->hidden.mem.here = (Uint8)c;
	context->hidden.mem.here++;

	if(!bInAMultipleFuncion)
	{
		context->filePosition++;
		context->stdioFilePosition = context->filePosition;
	}

	SDL_mutexV(multipleArchiveMutEx);
	return c;
}

static int mem_writeconst(SDL_RWops *context, const void *ptr, int size, int num)
{
	SDL_SetError("Can't write to read-only memory");
	return(-1);
}

static int mem_close(SDL_RWops *context)
{
	if ( context ) {
		free(context);
	}
	return(0);
}

/* Functions to create SDL_RWops structures from various data sources */
#ifdef WIN32
/* Aggh.  You can't (apparently) open a file in an application and
   read from it in a DLL.
*/
static int in_sdl = 0;
#endif

#ifdef macintosh
/*
 * translate unix-style slash-separated filename to mac-style colon-separated
 * name; return malloced string
 */
static char *unix_to_mac(const char *file)
{
	int flen = strlen(file);
	char *path = malloc(flen + 2);
	const char *src = file;
	char *dst = path;
	if(*src == '/') {
		/* really depends on filesystem layout, hope for the best */
		src++;
	} else {
		/* Check if this is a MacOS path to begin with */
		if(*src != ':')
			*dst++ = ':';   /* relative paths begin with ':' */
	}
	while(src < file + flen) {
		const char *end = strchr(src, '/');
		int len;
		if(!end)
			end = file + flen; /* last component */
		len = end - src;
		if(len == 0 || (len == 1 && src[0] == '.')) {
			/* remove repeated slashes and . */
		} else {
			if(len == 2 && src[0] == '.' && src[1] == '.') {
				/* replace .. with the empty string */
			} else {
				memcpy(dst, src, len);
				dst += len;
			}
			if(end < file + flen)
				*dst++ = ':';
		}
		src = end + 1;
	}
	*dst++ = '\0';
	return path;
}
#endif /* macintosh */

SDL_RWops *SDL_RWFromFile(const char *file, const char *mode)
{
	FILE *fp;
	SDL_RWops *rwops;

	rwops = SDL_RWFromMultipleArchive(file, mode); //maks
	if(rwops) return rwops; //maks

#ifdef macintosh
	{
		char *mpath = unix_to_mac(file);
		fp = fopen(mpath, mode);
		free(mpath);
	}
#else
	fp = fopen(file, mode);
#endif
	if ( fp == NULL ) {
		SDL_SetError("Couldn't open %s", file);
	} else {
#ifdef WIN32
		in_sdl = 1;
		rwops = SDL_RWFromFP(fp, 1);
		in_sdl = 0;
#else
		rwops = SDL_RWFromFP(fp, 1);
#endif
	}
	return(rwops);
}



SDL_RWops *SDL_RWFromFP(FILE *fp, int autoclose)
{
	SDL_RWops *rwops;

#ifdef WIN32
	if ( ! in_sdl ) {
		SDL_SetError("You can't pass a FILE pointer to a DLL (??)");
		/*return(NULL);*/
	}
#endif
	rwops = SDL_AllocRW();
	if ( rwops != NULL ) {
		rwops->seek = stdio_seek;
		rwops->read = stdio_read;
		rwops->write = stdio_write;
		rwops->close = stdio_close;
		rwops->Getc = stdio_getc; //maks
		rwops->Putc = stdio_putc; //maks
		rwops->hidden.stdio.fp = fp;
		rwops->hidden.stdio.autoclose = autoclose;

		//maks
		rwops->filePosition = rwops->stdioFilePosition = ftell(rwops->hidden.stdio.fp);
	}
	return(rwops);
}

SDL_RWops *SDL_RWFromMem(void *mem, int size)
{
	SDL_RWops *rwops;

	rwops = SDL_AllocRW();
	if ( rwops != NULL ) 
	{
		if(!mem && !size)
		{
			//maks: auto expand
			rwops->hidden.mem.bAutoExpand = 1;
			size = 4;
			mem = malloc(size);
		}
		else
		{
			rwops->hidden.mem.bAutoExpand = 0;
		}

		rwops->seek = mem_seek;
		rwops->read = mem_read;
		rwops->write = mem_write;
		rwops->close = mem_close;
		rwops->Getc = mem_getc; //maks
		rwops->Putc = mem_putc; //maks
		rwops->hidden.mem.base = (Uint8 *)mem;
		rwops->hidden.mem.here = rwops->hidden.mem.base;
		rwops->hidden.mem.stop = rwops->hidden.mem.base+size;

		//Maks: memory map files
		rwops->hidden.mem.hFile = NULL;
		rwops->hidden.mem.hMapping = NULL;	
		
		rwops->type = SDL_RW_TYPE_MEM; //maks
	}

	return(rwops);
}

SDL_RWops *SDL_RWFromConstMem(const void *mem, int size)
{
	SDL_RWops *rwops;

	rwops = SDL_AllocRW();
	if ( rwops != NULL ) {
		rwops->seek = mem_seek;
		rwops->read = mem_read;
		rwops->write = mem_writeconst;
		rwops->close = mem_close;
		rwops->hidden.mem.base = (Uint8 *)mem;
		rwops->hidden.mem.here = rwops->hidden.mem.base;
		rwops->hidden.mem.stop = rwops->hidden.mem.base+size;

		//Maks: memory map files
		rwops->hidden.mem.hFile = NULL;
		rwops->hidden.mem.hMapping = NULL;	
		
		rwops->hidden.mem.bAutoExpand = 0;
	}
	return(rwops);
}

SDL_RWops *SDL_AllocRW(void)
{
	SDL_RWops *area;

	area = (SDL_RWops *)malloc(sizeof *area);
	if ( area == NULL ) {
		SDL_OutOfMemory();
	}
	else //maks
	{
		memset(area, 0, sizeof *area);
	}

	return(area);
}

void SDL_FreeRW(SDL_RWops *area)
{
	free(area);
}


////////////////////////////////////////////////////////////////////////
//maks: Win32 unicode

/*#ifdef WIN32
SDL_RWops *SDL_RWFromFileUnicode(const Uint16 *file, const char *mode)
{
	FILE *fp;
	SDL_RWops *rwops;

	//rwops = SDL_RWFromMultipleArchive(file, mode); //maks
	//if(rwops) return rwops; //maks


	fp = fopen(file, mode);

	if ( fp == NULL ) 
	{
		SDL_SetError("Couldn't open %s", file);
		return NULL;
	}

	return(rwops);
}
#else
SDL_RWops *SDL_RWFromFileUnicode(const Uint16 *file, const char *mode)
{
	//Usar wfopen no linux e no mac?
	return NULL;
}
#endif*/
int
SDL_WriteLE32(SDL_RWops * dst, Uint32 value) // AKR
{
    value = SDL_SwapLE32(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}

Uint16
SDL_ReadLE16(SDL_RWops * src)
{
    Uint16 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapLE16(value));
}

Uint16
SDL_ReadBE16(SDL_RWops * src)
{
    Uint16 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapBE16(value));
}

Uint32
SDL_ReadLE32(SDL_RWops * src)
{
    Uint32 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapLE32(value));
}

Uint32
SDL_ReadBE32(SDL_RWops * src)
{
    Uint32 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapBE32(value));
}

Uint64
SDL_ReadLE64(SDL_RWops * src)
{
    Uint64 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapLE64(value));
}

Uint64
SDL_ReadBE64(SDL_RWops * src)
{
    Uint64 value;
	
    SDL_RWread(src, &value, (sizeof value), 1);
    return (SDL_SwapBE64(value));
}

int
SDL_WriteLE16(SDL_RWops * dst, Uint16 value)
{
    value = SDL_SwapLE16(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}

int
SDL_WriteBE16(SDL_RWops * dst, Uint16 value)
{
    value = SDL_SwapBE16(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}



int
SDL_WriteBE32(SDL_RWops * dst, Uint32 value)
{
    value = SDL_SwapBE32(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}

int
SDL_WriteLE64(SDL_RWops * dst, Uint64 value)
{
    value = SDL_SwapLE64(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}

int
SDL_WriteBE64(SDL_RWops * dst, Uint64 value)
{
    value = SDL_SwapBE64(value);
    return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}



////////////////////////////////////////////////////////////////////////
