/*******************************************************************************
system_windows.c: Implementation of system.c for MS PocketPC
Author: Odilon Nelson
Last modified: 02/05/03
Note: Do note compile this file directly. Compile system.c instead.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <aygshell.h>
#include "system.h"


int chdir( const char *dirname ) {return 0;}
char *ged_getcwd( char *buffer, int maxlen ) {return "";}
int ged_mkdir( const char *dirname ) {return 0;}
int open( const char *filename, int oflag) {return -1;}
int close( int handle ) {return -1;}
int read( int handle, void *buffer, unsigned int count ) {return -1;}
int write( int handle, const void *buffer, unsigned int count ) {return -1;}
int remove( const char *path ) {return -1;}
void openUrl(const char *url) 
{
	/*
	//Only works on the pocket if in window mode
	
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.nShow = SW_SHOWNORMAL;
	//sei.lpVerb = _T("open");
	sei.lpFile = _T("iexplore.exe");;
	sei.lpParameters = url;
	
	ShellExecuteEx(&sei);*/

	//Works on full screen, but returns only if user stop iexplorer on setting/memory
	extern HWND SDL_Window;
	PROCESS_INFORMATION process_info; 		
	TCHAR uBuf[256];

	SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
	ShowWindow(SDL_Window, SW_HIDE);

	
	MultiByteToWideChar(CP_ACP, 0, url, -1, uBuf, 255);
		
	if(CreateProcess(_T("iexplore.exe"), uBuf,
		NULL, 
		NULL, 
		FALSE, 0, NULL, NULL, NULL, &process_info ))
	{
		WaitForSingleObject ( process_info.hProcess, INFINITE );
	}

	ShowWindow(SDL_Window, SW_SHOW);
	SHFullScreen(SDL_Window, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | SHFS_SHOWSIPBUTTON);
}

int isInternetConnected() {return 0;}

//----------------------------------------------------------------------------
// Internal defs.

typedef struct GED_FILEINFO
{
	WIN32_FIND_DATA fdata;
	DWORD attrib;
	HANDLE hnd;
} ged_fileinfo_t;

//----------------------------------------------------------------------------
// Functions

long ged_findfirst(const char *path, int attrib)
{
	return GED_NOTFOUND;
}

//---------------------------------------------------------------------------

long ged_findnext(long handle)
{
	return GED_NOTFOUND;
}

//---------------------------------------------------------------------------

void ged_findclose(long handle)
{

}

//---------------------------------------------------------------------------
// Functions to obtain data from handle

char *ged_filename(long handle)
{
	return NULL;
}

long ged_writetime(long handle)
{
	return 0;
}

long ged_filesize(long handle)
{
	return 0;
}

unsigned char ged_attrib(long handle)
{
	return 0x00;
}
