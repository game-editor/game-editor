/* eic.h
 *
 *	(C) Copyright May  7 1995, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */
#ifndef EICH_
#define EICH_


#if !defined(_eic_ptr) && !defined(_EiC)
#define _eic_ptr
typedef struct {void *p, *sp, *ep;} ptr_t;
#endif

#ifdef _WIN32_WCE //maks
//#	define CHAR char
//#	define UCHAR	unsigned char
#	define HANDLE char
#	define LPCRECT char
#	define DWORD long
//#	include <types.h>
	typedef long off_t;
#ifndef WIN32
#	define WIN32
#endif
#else
#	include <sys/types.h>
#endif

#include "eicval.h"

#if defined(__cplusplus) && __cplusplus //maks
 extern "C" {
#endif

void *EiC_add_builtinfunc(char *name, val_t(*vfunc)(void));
void EiC_startEiC(int argc, char ** argv);
void EiC_init_EiC();
int  EiC_run(int argc, char **argv);
void EiC_parseString(char *, ...);
void EiC_parseStringSingle(char *); //maks
void EiC_parseStringFile(const char *, const char *); //maks
void EiC_callBack(void *code);

void EiC_setMessageDisplay(void (*)(char *));

extern void (*EiC_messageDisplay)(char *);


#define setArg(x,C,type,v)    do { code_t *c = C; \
                              *(type*)(&c->inst[c->nextinst - 5 - 2 * x].val) = v;} while(0)

extern val_t EiC_STaCK_VaLuE;

#define  EiC_ReturnValue(type)    (*(type*)&EiC_STaCK_VaLuE)


extern AR_t * AR[3];
extern size_t ARGC;

#define arg_list                  AR_t *
#define getargc()                 ARGC
#define getargs()                 AR[2]
#define nextarg(x,type)           (*((type*)&(--x)->v))
#define arg(i,x,type)             (*((type*)&x[-(i+1)].v))

#define malloc(x)	xmalloc(x)
#define calloc(x,y)	xcalloc(x,y)
#define free(x)		xfree(x)
#define realloc(x,y)    xrealloc(x,y)

#if defined(__cplusplus) && __cplusplus //maks
 }
#endif

#endif /* EICH_ */














