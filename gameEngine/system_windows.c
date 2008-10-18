/*******************************************************************************
system_windows.c: Implementation of system.c for MS Windows
Author: Odilon Nelson
Last modified: 02/05/03
Note: Do note compile this file directly. Compile system.c instead.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>


#include "system.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wininet.lib")

#include "dlmalloc.h" //maks

int CanShowStringW(TCHAR *s);

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

void openUrl(const char *url) 
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

char *ged_getcwd( char *buffer, int maxlen ) 
{
	TCHAR uBuf[512];	

	DWORD res = GetCurrentDirectory(maxlen, uBuf);

	if(!res || CanShowStringW(uBuf))
	{
		//Not unicode
		return getcwd(buffer, maxlen);
	}
	else
	{
		//Get the file in old 8.3 format
		if(!GetShortPathName(uBuf, buffer, maxlen))
		{
			return getcwd(buffer, maxlen);
		}
	}

	return buffer;
}

/*char* download(const char *server, const char *file, long *bytesRead)
{
	//server site: www.server site.com without http
	//file: /index.html

    HINTERNET hSession, hConnection, hRequest;

	DWORD dwNumberOfBytesAvailable = 0, dwBytesRead = 0;
	char* pBuffer = NULL;
	*bytesRead = 0;


	// Create a WinInet session
    hSession = InternetOpen("Game Editor",
                                        INTERNET_OPEN_TYPE_PRECONFIG,
                                        0,
                                        0,
                                        0);

	if(!hSession) return NULL;

    // Create an HTTP connection to the server (technically the connection is
    // established only when the first request is made).

    hConnection = InternetConnect(hSession,
                                              server,
                                              INTERNET_DEFAULT_HTTP_PORT,
                                              "",
                                              "",
                                              INTERNET_SERVICE_HTTP,
                                              0,
                                              0);

	if(!hConnection)
	{
		InternetCloseHandle(hSession);
		return NULL;
	}


    // Prepare a request for the web page
    hRequest = HttpOpenRequest(hConnection,
                                           0,
                                           file,
                                           0,
                                           "http://game-editor.com",
                                           0,
                                           0,
                                           0);

	if(!hRequest)
	{
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		return NULL;
	}

    // Send the HTTP request

    if(!HttpSendRequest(hRequest,
                      0,
                      0,
                      0,
                      0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		return NULL;
	}

    // Read the response data
    do
    {
        InternetQueryDataAvailable(hRequest,
                                     &dwNumberOfBytesAvailable,
                                     0,
                                     0);

        if (0 < dwNumberOfBytesAvailable)
        {
            pBuffer = (char *)realloc(pBuffer, *bytesRead + dwNumberOfBytesAvailable + 1);
            

            dwBytesRead = 0;

            InternetReadFile(hRequest,
                               pBuffer + *bytesRead,
                               dwNumberOfBytesAvailable,
                               &dwBytesRead);

			*bytesRead += dwBytesRead;
        }

    } while (0 < dwNumberOfBytesAvailable);


    // Close the request handle
    InternetCloseHandle(hRequest);

    // Close the HTTP connection handle
    InternetCloseHandle(hConnection);

    // Close the WinInet session
    InternetCloseHandle(hSession);

    return pBuffer;
}*/

int isInternetConnected()
{
	char szHostName[129];
	
	int bPrivateAdr = 0; // Private Address area
	int bClassA = 0;     // Class A definition
	int bClassB = 0;     // Class B definition
	int bClassC = 0;     // Class C definition
	int bAutoNet = 0;    // AutoNet definition
	char str[128], addr[16];

	WORD wVersionRequested = MAKEWORD(1, 1);  
	WSADATA wsaData; 
	/*int err =*/ WSAStartup(wVersionRequested, &wsaData);
	
	if (gethostname(szHostName, 128) == 0 )
	{
		// Get host adresses
		struct hostent * pHost;
		int i;
		UINT ipb;
		pHost = gethostbyname(szHostName); 
		
		for (i = 0; 
		     pHost!= NULL && pHost->h_addr_list[i]!= NULL; 
		     i++ )
		{
			int j;
			str[0] = 0;
			bClassA = bClassB = bClassC = 0;
			for( j = 0; j < pHost->h_length; j++ )
			{
				if( j > 0 )	strcat(str, ".");
				ipb = (unsigned int)((unsigned char*)pHost->h_addr_list[i])[j];
				
				// Define the IP range for exclusion
				if(j==0)
				{
					if(bClassA = (ipb == 10)) break; // Class A defined
					bClassB = (ipb == 172); 
					bClassC = (ipb == 192);
					bAutoNet = (ipb == 169);
				}
				else if (j==1)
				{
					// Class B defined
					if(bClassB = (bClassB && ipb >= 16 && ipb <= 31)) break;
					
					// Class C defined
					if(bClassC = (bClassC && ipb == 168)) break;
					
					//AutoNet pasibility defined
					if(bAutoNet = (bAutoNet && ipb == 254)) break;
				}
				
				
				sprintf(addr, "%u", ipb);
				strcat(str, addr);
			}
			// If any address of Private Address 
			// area has been found bPrivateAdr = 1
			bPrivateAdr = bPrivateAdr || bClassA || bClassB || bClassC;
			
			// If any address of Internet Address area 
			// has been found returns TRUE
			if (!bClassA 
			&& !bClassB 
			&& !bClassC 
			&& !bAutoNet 
			&& strcmp(str, "127.0.0.1") != 0 
			&& str[0] != 0) 
			{
				//In Windows 98, after connection, IP continues valid
				//So, make sure the connection status

				int res = gethostbyname("game-editor.com") != NULL;
				WSACleanup();

				return res;
			}
		}
	}
	
	if (bPrivateAdr)
	{
		// The system has IP address from Private Address 
		// area,only. Internet from the computer can be accessable 
		// via Proxy. 
		//Try resolve game-editor IP address		

		int res =  gethostbyname("game-editor.com") != NULL;		
		WSACleanup();

		return res;
	}
	
	WSACleanup();
	return 0;
}


long ged_findfirst(const char *path, int attrib)
{
	ged_fileinfo_t *fi;

	char *pathname = (char *)malloc(strlen(path) + 1);
	correctDirectoryName(path, pathname);
	
	fi = (ged_fileinfo_t *) malloc(sizeof(ged_fileinfo_t));
	if (fi == NULL) return GED_NOTFOUND;
	
	fi->hnd = FindFirstFile(pathname, &fi->fdata);
	fi->attrib = attrib;

	free(pathname);

	if(fi->attrib == A_DIREC || fi->attrib == (A_DIREC | A_HIDDEN))
	{
		while(fi->hnd != INVALID_HANDLE_VALUE && !(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if(FindNextFile(fi->hnd, &fi->fdata) == 0)
			{
				free(fi);
				return GED_NOTFOUND;
			}
		}
	}
	
	if (fi->hnd == INVALID_HANDLE_VALUE)
	{
		free(fi);
		return GED_NOTFOUND;
	}
	else
		return (long) fi;
	//falta tratar os atributos...
}

//---------------------------------------------------------------------------

long ged_findnext(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;  
	
	if (fi == NULL) return GED_NOTFOUND;
	if (FindNextFile(fi->hnd, &fi->fdata) == 0)
	{
		ged_findclose(handle);
		return GED_NOTFOUND;
	}
	else
	{
		if(fi->attrib == A_DIREC || fi->attrib == (A_DIREC | A_HIDDEN))
		{
			while(!(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if(FindNextFile(fi->hnd, &fi->fdata) == 0)
				{
					ged_findclose(handle);
					return GED_NOTFOUND;
				}
			}
		}

		return (long) fi;
	}
}

//---------------------------------------------------------------------------

void ged_findclose(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL && (long)fi != GED_NOTFOUND)
	{
		FindClose(fi->hnd);	
		free(fi);
	}
}

//---------------------------------------------------------------------------
// Functions to obtain data from handle

char *ged_filename(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if(fi != NULL)
	{
		if(fi->fdata.cFileName != NULL && CanShowStringW(fi->fdata.cFileName))
		{
			return fi->fdata.cFileName;
		}
		else if(fi->fdata.cAlternateFileName)
		{
			return fi->fdata.cAlternateFileName;
		}		
	}
	
	return NULL;
}

long ged_writetime(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL)
	{
		return fi->fdata.ftLastWriteTime.dwLowDateTime;
	}
	else
		return 0;
}

long ged_filesize(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL)
	{
		return fi->fdata.nFileSizeLow;
	}
	else
		return 0;
}

unsigned char ged_attrib(long handle)
{
	ged_fileinfo_t *fi = (ged_fileinfo_t *) handle;
	
	if (fi != NULL)
	{
		unsigned char attr = 0;

		if(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			attr |= A_DIREC;
		}

		if(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		{
			attr |= A_HIDDEN;
		}

		if(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		{
			attr |= A_RDONLY;
		}

		if(fi->fdata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		{
			attr |= A_SYSTEM;
		}

		return attr;
	}
	else
		return 0x00;
}
