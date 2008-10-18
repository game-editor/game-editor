#ifndef EICVALH_
#define EICVALH_

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

#ifdef __SYMBIAN32__ //maks
typedef long ssize_t;
#endif

#if !defined(WIN32) && !defined(_WIN32_WCE)
#include <unistd.h>
#endif

#if !defined(_eic_ptr)
#define _eic_ptr
typedef struct {void *p, *sp, *ep; unsigned short cnt;} ptr_t;
#endif


#ifndef NO_LONG_LONG
typedef long long eic_llong;
#else
typedef long eic_llong;
#endif


union VaL {
  char cval;   	          /* char value */
  unsigned char ucval;
  short sval;             /* short integer val */
  unsigned short usval;
  int ival;    	          /* integer value */
  unsigned  uival;
  long lval;              /* long integer */
  unsigned long ulval;

  /* long longs are not yet supported by ANSI C*/

  eic_llong llval;        /* long long value */

  float fval;  	         /* float value */
  double dval;           /* double float value */

  ptr_t p;              /* safe pointer */
  void *up;             /* unsafe pointer */

  div_t divval;
  ldiv_t ldivval;
        
  int (*func)(); 	     /* function pointer */
  union VaL (*vfunc)();

  struct symentry_t * sym;/* pointer into symbol table */

#ifndef WIN32
  pid_t pid;              /* process ID value */
#endif
  size_t szval;           /* generic size value */
#ifndef WIN32
  ssize_t sszval;         /* POSIX.1 byte count value */
#endif

#ifndef WIN32
  mode_t mval;            /* mode_t value */
#endif

#ifdef _NETBSD
  long offval;		  /* file offset position */
#else
  off_t offval;           /* file offset position */
#endif
};

typedef union VaL val_t;


typedef struct Label_t {
    char *name;            /* label name */
    int loc;               /* label location */
    struct Label_t *nxt;   /* link to next label in list */
} Label_t;

typedef struct {
    unsigned short opcode; //maks
	short  val1; //maks
	int ext;		
    val_t  val;	   

#ifndef STAND_ALONE_GAME
    unsigned short line;
#endif
}InsT_t;


typedef struct {
    unsigned int nextinst;  /* next instruction */
    unsigned int binst;     /* physical size */
    InsT_t * inst;          /* instructions */
    char * Filename;        /* file with source code */
    Label_t * labels;
    Label_t * gotos;
    void * parent;         /* used for callbacks */
    void * prev;           /* used for chaining during reentry and callbacks*/
}code_t;

/* methods for code */
#define opcode(C,i)   ((C)->inst[i].opcode)
#define setopcode(C,i,y) (opcode(C,i) = y)
#define ivalcode(C,i) ((C)->inst[i].val.ival)
#define pvalcode(C,i) ((C)->inst[i].val.p.p)
#define nextinst(C)   ((C)->nextinst)

#define codeName(C)   (C)->Filename

#ifndef STAND_ALONE_GAME
#define instline(C,i) ((C)->inst[i].line)
#else
#define instline(C,i) 0
#endif


#ifndef EICH_
typedef struct AR_t {
    val_t v;
    type_expr * type;
}AR_t;

#else
typedef struct AR_t {
    val_t v;
    void * type;
}AR_t;
#endif

#endif



