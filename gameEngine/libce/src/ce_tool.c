
#ifndef __FILETOOL_HEADERFILE__
#define __FILETOOL_HEADERFILE__

#include <Winbase.h>
#include <malloc.h>

BOOL
isDirectory(const char* nName)
{
#ifdef _WIN32_WCE
	wchar_t str[256];
	int i, len = strlen(nName);
	for(i=0; i<len; i++)
		str[i] = nName[i];
	str[len] = 0;
#else
	const char* str = nName;
#endif //_WIN32_WCE

	return (GetFileAttributes(str) & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

static char* loc = "C";

int errno;

int __mb_cur_max = 2;

char *setlocale( int category, const char *locale )
{
	return loc;
}


DWORD
GetEnvironmentVariable(
  LPCTSTR lpName,  // environment variable name
  LPTSTR lpBuffer, // buffer for variable value
  DWORD nSize      // size of buffer
)
{
	return 0;
}


BOOL
SetEnvironmentVariable(
  LPCTSTR lpName,  // environment variable name
  LPCTSTR lpValue  // new value for variable
)
{
	return FALSE;
}

static int len0 = 0, wlen0 = 0;
static char* str0 = NULL;
static unsigned short* wstr0 = NULL;


char*
tmpCharFromShort(const unsigned short* nStr)
{
	int i, len = wcslen(nStr);

	if(len>len0)
	{
		if(str0)
			free(str0);
		len0 = len;
		str0 = str0 = (char*)malloc(len0+1);
	}

	for(i=0; i<len; i++)
		str0[i] = (char)nStr[i];
	str0[len] = 0;

	return str0;
}


unsigned short*
tmpShortFromChar(const char* nStr)
{
	int i, len = strlen(nStr);

	if(len>wlen0)
	{
		if(wstr0)
			free(wstr0);
		wlen0 = len;
		wstr0 = (unsigned short*)malloc(wlen0+1);
	}

	for(i=0; i<len; i++)
		wstr0[i] = (char)nStr[i];
	wstr0[len] = 0;

	return wstr0;
}


int mblen(const char* s, size_t n)
{
	if (s == NULL || *s == '\0')
		return 0;
	if (n == 0)
		return -1;
	return 1;
}


HMODULE GetModuleHandleA(const char* lpModuleName)
{
	unsigned short* wstr = tmpShortFromChar(lpModuleName);
	HMODULE handle = GetModuleHandleW(wstr);

	return handle;
}


HANDLE CreateFileA(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
				   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
				   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	unsigned short* wstr = tmpShortFromChar(lpFileName);
	HMODULE handle = CreateFileW(wstr, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
								 dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	return handle;
}

HRSRC FindResourceExA(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLanguage)
{
	return FindResource(hModule, lpName, lpType);
}

HRSRC FindResourceEx(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLanguage)
{
	return FindResource(hModule, lpName, lpType);
}


int stricmp( const char *string1, const char *string2 )
{
	return _stricmp(string1, string2);
}

int strnicmp( const char *string1, const char *string2, size_t count )
{
	return _strnicmp(string1, string2, count);

}

FARPROC GetProcAddressA(HMODULE hModule, LPCSTR lpProcName)
{
	unsigned short* wstr = tmpShortFromChar(lpProcName);
	FARPROC proc = GetProcAddressW(hModule, wstr);
	return proc;
}

#endif //__FILETOOL_HEADERFILE__

