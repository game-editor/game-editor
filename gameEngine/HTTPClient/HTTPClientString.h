
#ifndef _HTTP_CLIENT_STRING
#define _HTTP_CLIENT_STRING

#include "HTTPClientWrapper.h" // Cross platform support
#include "HTTPClient.h" 

///////////////////////////////////////////////////////////////////////////////
//
// Section      : HTTP Api global definitions
// Last updated : 01/09/2005
//
///////////////////////////////////////////////////////////////////////////////

BOOL                    HTTPStrInsensitiveCompare   (CHAR *pSrc, CHAR* pDest, Uint32 nLength);
BOOL                    HTTPStrSearch               (CHAR *pSrc, CHAR *pSearched, Uint32 nOffset, Uint32 nScope,HTTP_PARAM *HttpParam);
CHAR                    HTTPStrExtract              (CHAR *pParam,Uint32 nOffset,CHAR Restore);
CHAR*                   HTTPStrCaseStr              (const char *pSrc, Uint32 nSrcLength, const char *pFind);  //maks
CHAR*                   HTTPStrGetToken             (CHAR *pSrc, Uint32 nSrcLength, CHAR *pDest, Uint32 *nDestLength);
Uint32                  HTTPStrGetDigestToken       (HTTP_PARAM pParamSrc, CHAR *pSearched, HTTP_PARAM *pParamDest);
Uint32                  HTTPStrHToL                 (CHAR * s); 
CHAR*                   HTTPStrLToH                 (CHAR * dest,Uint32 nSrc);        
#endif
