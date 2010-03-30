#ifndef MALLOC_270_H
#define MALLOC_270_H

#ifdef __cplusplus
extern "C" {
#endif


void*  dlmalloc(size_t);
void   dlfree(void*);
void*  dlcalloc(size_t, size_t);
void*  dlrealloc(void*, size_t);

#ifndef _ALLOCA_H
extern void* alloca(size_t);
#endif

#define malloc	dlmalloc	//maks
#define realloc dlrealloc	//maks
#define calloc	dlcalloc	//maks
#define free	dlfree		//maks

 #define memalign nedmemalign             
 #define mallinfo nedmallinfo             
 #define mallopt nedmallopt              
 #define malloc_trim nedmalloc_trim          
 #define malloc_stats nedmalloc_stats         
 #define malloc_footprint nedmalloc_footprint     
 #define independent_calloc nedindependent_calloc   
 #define independent_comalloc nedindependent_comalloc 

#ifdef _MSC_VER
 #define msize	nedblksize
#endif


char *strDup(const char *str);

#ifdef __cplusplus
};  /* end of extern "C" */
#endif

#endif /* MALLOC_270_H */



