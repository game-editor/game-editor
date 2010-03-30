#if defined(WIN32) && !defined(_WIN32_WCE) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0400
#endif

#include "nedmalloc.c"

int MemoryAllocationError(int bytes){exit(5);}; //maks
extern int ReleaseMemory(int bytes); //maks

void *dlmalloc(size_t bytes) //maks
{
	void* m = NULL;
	
	do
	{
#ifdef DISABLE_DLMALLOC
		m = malloc(bytes);	
#else
		m = nedmalloc(bytes);		
#endif  
		
		if(!m) 
		{
			//Try allocate again
			
			if(!MemoryAllocationError(bytes))
			{
				//failed				
				break;
			}
		}
		
	} while(!m);
	
	return m;
}

void dlfree(void* m) //maks
{
#ifdef DISABLE_DLMALLOC
	free(m);
#else  
	nedfree(m);  
#endif
}

void* dlrealloc(void* m, size_t bytes) //maks
{
	void* newMem = NULL;

	if(bytes <= 0 && m)
	{
		dlfree(m);
		return 0;
	}
	
	do
	{
#ifdef DISABLE_DLMALLOC
		newMem = realloc(m, bytes);
#else
		newMem = nedrealloc(m, bytes);		
#endif   
		
		if(!newMem && bytes > 0) 
		{
			//Try allocate again
			if(!MemoryAllocationError(bytes))
			{
				//failed
				break;
			}
		}
		
	} while(!newMem);

  return newMem;
}

void* dlcalloc(size_t n, size_t elem_size) //maks
{
	void* m = NULL;
	
	do
	{
#ifdef DISABLE_DLMALLOC
		m = calloc(n, elem_size);
#else
		m = nedcalloc(n, elem_size);		
#endif
		
		if(!m) 
		{
			//Try allocate again
			if(!MemoryAllocationError(n*elem_size))
			{
				//failed
				break;
			}
		}
		
	} while(!m);

  return m;
}


#include <string.h>
char *strDup(const char *str) //maks
{
	char *newstr;
	
	newstr = (char *)dlmalloc(strlen(str)+1);
	if ( newstr != NULL ) {
		strcpy(newstr, str);
	}
	return(newstr);
}