#ifndef _ASSERTH
#define _ASSERTH

#pragma push_safeptr


void _Assert(char *msg);

#ifndef __F1LE__
#define __F1LE__	""
#endif

#undef assert
#ifdef NDEBUG
#define assert(test)
#else
#define __Str(x)  __Val(x)
#define __Val(x)  #x
#define assert(test)  ((test) ? (void) 0 : _Assert("In "__F1LE__ ", line " __Str(__LINE__)  ": " #test ))
#endif


#pragma pop_ptr

#endif /* end _ASSERTH */

