
#ifndef HTTP_CLIENT_SAMPLE
#define HTTP_CLIENT_SAMPLE

#define HTTP_CLIENT_BUFFER_SIZE     8192

#include <stdio.h>
#include "API/HTTPClient.h"

typedef struct _HTTPParameters
{
    CHAR                    Uri[1024];        
    CHAR                    ProxyHost[1024];  
    Uint32                  UseProxy ;  
    Uint32                  ProxyPort;
    Uint32                  Verbose;
    CHAR                    UserName[64];
    CHAR                    Password[64];
    HTTP_AUTH_SCHEMA        AuthType;

} HTTPParameters;





#endif // HTTP_CLIENT_SAMPLE


