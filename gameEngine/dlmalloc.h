//#define USE_NEDMALLOC


#if defined(_WIN32_WCE) //|| defined(GP2X) //maks: uses the default gp2x implementation
#	include <stdlib.h>   /* for size_t */
#	undef linux
#else
	//maks: 
	//dlmalloc causes the bug "Asteroids, run, checkers, run, no screen update" when DirectX is active
	//dlmalloc causes crash when execute editor without directx
#	define DISABLE_DLMALLOC 
#include <stddef.h>   /* for size_t */
#endif

//Call the dlfunctions, even if dlmalloc is disable (will call the release memory functions)

#	define USE_DL_PREFIX

#	ifdef USE_NEDMALLOC

		//Use nedmalloc (with dlmalloc 2.8.4 and thread optimized)
		//No speed gains over dlmalloc in the Pocket PC tests
		//Loop in checkers.ged
		#include "nedmalloc/dlmalloc_ned.h"

#	else

		//Use dlmalloc 2.7.0
		#define USE_MALLOC_LOCK
		#include "dlmalloc/dlmalloc.h"

#	endif




