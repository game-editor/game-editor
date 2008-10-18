
#ifndef __FILETOOL_HEADERFILE__
#define __FILETOOL_HEADERFILE__

#include <Winbase.h>

#ifdef  __cplusplus
extern "C" {
#endif

BOOL
isDirectory(const char* nName);


DWORD
GetEnvironmentVariable(
  const char* lpName,  // environment variable name
  const char* lpBuffer, // buffer for variable value
  DWORD nSize      // size of buffer
);

BOOL
SetEnvironmentVariable(
  const char* lpName,  // environment variable name
  const char* lpValue  // new value for variable
);


#define strdup _strdup

#define _S_IFDIR

char*
tmpCharFromShort(const unsigned short* nStr);

unsigned short*
tmpShortFromChar(const char* nStr);

int mblen(const char* s, size_t n);

HMODULE GetModuleHandleA(const char* lpModuleName);

HANDLE CreateFileA(
  const char* lpFileName,                     // file name
  DWORD dwDesiredAccess,                      // access mode
  DWORD dwShareMode,                          // share mode
  LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
  DWORD dwCreationDisposition,                // how to create
  DWORD dwFlagsAndAttributes,                 // file attributes
  HANDLE hTemplateFile                        // handle to template file
);

HRSRC FindResourceExA(
  HMODULE hModule, // module handle
  LPCTSTR lpType,  // resource type
  LPCTSTR lpName,  // resource name
  WORD wLanguage   // resource language
);

HRSRC FindResourceEx(
  HMODULE hModule, // module handle
  LPCTSTR lpType,  // resource type
  LPCTSTR lpName,  // resource name
  WORD wLanguage   // resource language
);

int stricmp( const char *string1, const char *string2 );
int strnicmp( const char *string1, const char *string2, size_t count );

FARPROC GetProcAddressA(HMODULE hModule, LPCSTR lpProcName);


#ifdef  __cplusplus
}
#endif

#endif //__FILETOOL_HEADERFILE__
