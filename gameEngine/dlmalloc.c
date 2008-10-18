#include "dlmalloc.h"

#if defined(_WIN32_WCE)
#	define WIN32
#	undef linux
//#	define ALWAYS_ALIGNED 32//maks: mALLOC don't returns 32 bytes align memory on Pocket PC (fail on VirtualQuery)
#endif


#	ifdef USE_NEDMALLOC
	//Use nedmalloc (with dlmalloc 2.8.4 and thread optimized)
	//Not speed gains over dlmalloc in the Pocket PC tests
#	include "nedmalloc/dlmalloc_ned.c"
#	else
	//Use dlmalloc 2.7.0
#	include "dlmalloc/dlmalloc.c"
#	endif



