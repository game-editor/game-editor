/*******************************************************************************
system_linux.c: Implementation of system.c for Linux
Author: Odilon Nelson
Last modified: 02/05/03
Note: Do note compile this file directly. Compile system.c instead.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <glob.h>
#include "system.h"

#include "dlmalloc.h" //maks

//----------------------------------------------------------------------------
// Internal defs.
#define MAX_GLOBS_OPEN 32
static glob_t *GlobsOpen[MAX_GLOBS_OPEN];
 
typedef struct GED_FILEINFO
{
	unsigned char ff_attrib;
	int ff_ftime;
	int ff_fdate;
	long ff_fsize;
	char ff_name[FILENAME_MAX + 1];
	// For internal use, only.
	glob_t *GlobPtr;
	int OnFile;
	int DesiredAttributes;
} ged_fileinfo_t;


//----------------------------------------------------------------------------
// Functions
char* download(const char *url, long *bytesRead) {};
char *ged_getcwd( char *buffer, int maxlen ) {return getcwd(buffer, maxlen);}

int isInternetConnected() {return 0;}

int chdrive(int a) { return -1; }
int getdrive()     { return 0;  }


long ged_findfirst(const char *path, int attrib)
{
	ged_fileinfo_t *fi;
	char *pattern;
	int i;
	int Flag;

	char *pathname = (char *)malloc(strlen(path) + 1);
	correctDirectoryName(path, pathname);
	
	for (i = 0; i < MAX_GLOBS_OPEN; i++)
		if (GlobsOpen[i] == NULL) break;
		if (i == MAX_GLOBS_OPEN)
		{
			free(pathname);
			return GED_NOTFOUND;
		}
		
		fi = (ged_fileinfo_t *) malloc(sizeof(ged_fileinfo_t));
		if (fi == NULL)
		{
			free(pathname);
			return GED_NOTFOUND;
		}
		
		fi->OnFile = 0;
		fi->GlobPtr = GlobsOpen[i] = (glob_t *)calloc (1, sizeof (glob_t));
		if (fi->GlobPtr == NULL) 
		{
			free(pathname);
			return GED_NOTFOUND;
		}

		fi->DesiredAttributes = attrib & (A_RDONLY | A_HIDDEN | A_SYSTEM | A_DIREC);
		fi->GlobPtr->gl_offs = 0;
		Flag = 0;//Flag = GLOB_NOSORT;
#ifdef GLOB_PERIOD
		if ((A_HIDDEN & attrib) != 0) Flag |= GLOB_PERIOD;
#endif
#ifdef GLOB_ONLYDIR //AKR
		if (attrib == A_DIREC || attrib == (A_DIREC | A_HIDDEN))
			Flag |= GLOB_ONLYDIR;
#endif
			/*
			NOTE: the behaviour of *.* under UNIXes is not what
			one may expect if migrating from DOS...
			Just try at the shell: 'ls *.*' and 'ls *' to see.
		*/
		pattern = (char *)calloc(strlen(pathname)+1, sizeof(char));
		if (strcmp(pathname, "*.*") == 0)
			strcpy(pattern, "*");
		else
			strcpy(pattern, pathname);
		
		if (glob(pattern, Flag, NULL, fi->GlobPtr) != 0)
		{
			ged_findclose((long) fi);

			free(pathname);
			return GED_NOTFOUND;
		}

		free(pathname);
		return ged_findnext((long) fi);
}

//---------------------------------------------------------------------------

void globclose(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	int i;
	if (fi != NULL && (long)fi != GED_NOTFOUND && fi->GlobPtr != NULL)
	{
		for (i = 0; i < MAX_GLOBS_OPEN; i++)
			if (GlobsOpen[i] == fi->GlobPtr)
			{
				GlobsOpen[i] = NULL;
				break;
			}
			globfree (fi->GlobPtr);
			fi->GlobPtr = NULL;

	}
}

long ged_findnext(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	struct tm *TimeBuf;
	struct stat StatBuf;
	int ReturnValue = GED_NOTFOUND, attr;
	if (fi == NULL || fi->GlobPtr == NULL)
		return GED_NOTFOUND;
	for (; fi->OnFile < fi->GlobPtr->gl_pathc; fi->OnFile++)
    {
		strcpy (fi->ff_name, fi->GlobPtr->gl_pathv[fi->OnFile]);
		if (stat(fi->ff_name, &StatBuf) != 0)
			continue;
		attr = 0;
		if ((S_IWUSR & StatBuf.st_mode) == 0)
			attr |= A_RDONLY;
		if ((S_IFDIR & StatBuf.st_mode) != 0)
			attr |= A_DIREC;
		if ((S_IRUSR & StatBuf.st_mode) == 0)
			attr |= A_SYSTEM;
		if (*fi->ff_name == '.')
			attr |= A_HIDDEN;
		if ((attr & ~fi->DesiredAttributes) != 0)
			continue;
		fi->ff_attrib = attr;
		fi->ff_fsize = StatBuf.st_size;
		//Odilon: Suporte a data temporariamente desabilitado.
		///      TimeBuf = localtime (&StatBuf.st_mtime);
		///      fi->ff_fdate = (TimeBuf->tm_year - 80) << 9;
		///      fi->ff_fdate += (TimeBuf->tm_mon + 1) << 5;
		///      fi->ff_fdate += TimeBuf->tm_mday;
		///      fi->ff_ftime = TimeBuf->tm_hour << 11;
		///      fi->ff_ftime += TimeBuf->tm_min << 5;
		///      fi->ff_ftime += TimeBuf->tm_sec / 2;
		ReturnValue = (long) fi;
		fi->OnFile++;
		break;
    }
	// All done with this record.  Close out the glob, if appropriate.
	// Close outside this function
	if (fi->OnFile >= fi->GlobPtr->gl_pathc)
		globclose((long) fi);
	return ReturnValue;
}

//---------------------------------------------------------------------------

void ged_findclose(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	int i;
	if (fi != NULL && (long)fi != GED_NOTFOUND && fi->GlobPtr != NULL)
	{
		for (i = 0; i < MAX_GLOBS_OPEN; i++)
			if (GlobsOpen[i] == fi->GlobPtr)
			{
				GlobsOpen[i] = NULL;
				break;
			}
			globfree (fi->GlobPtr);
			fi->GlobPtr = NULL;
			free(fi);
	}
}

//---------------------------------------------------------------------------
// Functions to obtain data from handle

char *ged_filename(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL && fi->ff_name != NULL)
	{
		//Return file name only
		char *name = fi->ff_name + strlen(fi->ff_name);
		while(*name != '/' && name > fi->ff_name) name--;
		if(*name == '/') name++;
		
		return name;
	}
	else
		return NULL;
}

long ged_writetime(long handle)
{
	return 0;
}

long ged_filesize(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL)
	{
		return fi->ff_fsize;
	}
	else
		return 0;
}

unsigned char ged_attrib(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL)
		return fi->ff_attrib;
	else
		return 0x00;
}


