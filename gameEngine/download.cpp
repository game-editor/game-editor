#include "gedString.h"
#include "HTTPClient/httpclient.cpp"
#include "HTTPClient/HTTPClientAuth.cpp"
#include "HTTPClient/HTTPClientString.cpp"
#include "HTTPClient/HTTPClientWrapper.cpp"



#define HTTP_CLIENT_BUFFER_SIZE 1024
extern "C" char* download(const char *url, long *bytesRead)
{
	//Without http://
	//url: www.server.com 
	//url: www.server.com/file.html
	

	char* pBuffer = NULL;
	


	Uint32                  nRetCode;
	Uint32                  nSize,nTotal = 0;	
	HTTP_SESSION_HANDLE pHTTP = 0;
	char buf[HTTP_CLIENT_BUFFER_SIZE];

	gedString sUrl(url);
	sUrl.replaceAll(" ", "%20");
	sUrl.replaceAll("//", "/");
	sUrl = gedString("http://") + sUrl;
	

	*bytesRead = 0;
	pHTTP = HTTPClientOpenRequest(0);
	

	if(!pHTTP) return NULL;


	// Send a request for the home page 
	if((nRetCode = HTTPClientSendRequest(pHTTP, sUrl.c_str(), NULL, 0, FALSE, 0, 0)) != HTTP_CLIENT_SUCCESS)
	{
		HTTPClientCloseRequest(&pHTTP);
		return NULL;
	}

	// Retrieve the the headers and analyze them
	if((nRetCode = HTTPClientRecvResponse(pHTTP,3)) != HTTP_CLIENT_SUCCESS)
	{
		HTTPClientCloseRequest(&pHTTP);
		return NULL;
	}

	
	// Get the data until we get an error or end of stream code
	// printf("Each dot represents %d bytes:\n",HTTP_BUFFER_SIZE );
	while(nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
	{
		nSize = 0;   

		// Get the data
		nRetCode = HTTPClientReadData(pHTTP, buf, HTTP_CLIENT_BUFFER_SIZE, 0, &nSize);

		if(nSize)
		{
			pBuffer = (char *)realloc(pBuffer, nTotal + nSize + 1);
			memcpy(pBuffer + nTotal, buf, nSize);
		}

		nTotal += nSize;		
	}

	*bytesRead = nTotal;
	HTTPClientCloseRequest(&pHTTP);

	return pBuffer;
}
