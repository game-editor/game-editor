/* stdio.c
 *
 *	(C) Copyright Dec 19 1998, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */

/* This file is broken into 2 parts
 * the first part defines the interface routines
 * and the 2nd part adds the interface routine
 * to EiC's look up tables.
 */


#ifndef  _USE_POSIX
#define  _USE_POSIX
#endif

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif

#ifndef __EXTENSIONS__
#define __EXTENSIONS__
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif


#include <stdio.h>

#if 0
#ifdef _IRIX
extern FILE *popen(const char *, const char *);
extern int pclose(FILE *);
#endif
#endif


#include <stdlib.h>
#include <string.h>
#if !defined(WIN32) && !defined(_WIN32_WCE) //maks
#include <unistd.h>
#endif
#include <limits.h>
#include <ctype.h>
#include <math.h>

#include "eic.h"
#include "stdliblocal.h"

int checkEp(ptr_t P, int s); //maks

#ifdef PPCLIB
void outbyte(int ch);
#endif

#ifdef WIN32
#include "xalloc.h"
#endif

/* The following code provides for an open FILE cleanup
mechanism.  EiC will call _ffexit, cleaning up all open
FILEs which were opened in the side effects being discarded.
	*/
static size_t NextFopenEntry = 4;

#ifdef _WIN32_WCE //maks
FILE *freopen( const char *path, const char *mode, FILE *stream )
{
	fclose(stream);
	return fopen(path, mode);
}


int rename( const char *oldname, const char *newname )
{
	return -1;
}

#define FOPEN_MAX 10
#endif

typedef struct stFileInfo
{
	FILE *fp;
	char name[256];
#ifdef WIN32
	void *buffer; //maks: solve http://code.game-editor.com/ticket/92
#endif
} FILE_INFO;

static size_t book1[FOPEN_MAX]; //maks = {1,2,3};
static FILE_INFO book2[FOPEN_MAX];


void InitFileControl() //maks
{
	//Clear books on first run
	static int bBookInit = 0;

	if(!bBookInit) 
	{
		memset(book1, 0, FOPEN_MAX*sizeof(size_t));
		memset(book2, 0, FOPEN_MAX*sizeof(FILE_INFO));
		bBookInit = 1;
	}
}

static int isValidFilePointer(FILE * ptr) //maks
{
	int i = 0;
	if(!ptr) return 0;
	
	for(i=0;i<FOPEN_MAX;i++)
	{
		if(ptr == book2[i].fp)
			break;
	}
	
	if(i<FOPEN_MAX) 
	{
		return 1;
	}
	
	return 0;
}


size_t fopen_NextEntryNum(void) {
	return NextFopenEntry;
}

/* We assume that eic_fopen() is always closed by eic_fclose()
and eic_popen() is aways closed by eic_pclose() so that book[i]
being non-zero implies that the associated FILE is still open.
This ignores the possibility that a file opened in interpreted
code can be closed in compiled code, but such a situation is
ill advised in any event.
	*/


void _ffexit(/*size_t Entry*/) //maks 
{
	int i;

	/*if(Entry < 0) //maks
		Entry = fopen_NextEntryNum();

	for(i = 0; i < FOPEN_MAX; i += 1) 
	{
		if(book1[i] >= Entry) 
		{
			fclose(stdin + i);
			book1[i] = 0;
			book2[i] = NULL;
		}
	}

	return;*/

	for(i = 0; i < FOPEN_MAX; i += 1) 
	{
		if(book2[i].fp) 
		{
			fclose(book2[i].fp);
			book1[i] = 0;
			book2[i].fp = NULL;
		}
	}
}


/*  INTERFACE FUNCTIONS */

int charin_(char **buf)
{
    int ch;
    ch = **buf;
    *buf += 1;
    if(!ch)
	ch = EOF;
    return ch;
}

int charback_(int ch, char **buf)
{
    *buf -= 1;
    return ch;
}

int charout_(int c, char **buf, char *endBuf) //maks
{
    char *s;

	if(*buf < (endBuf-1) || !endBuf) //maks: solve the crash when write beyond sprintf buf
	{
		s = *buf;
		*s++ = c;
		*s = 0;
		*buf = s;

		return 1;
	}

    return 0;
}

int _eicUprintf(int (*output)(), void *arg, char *fmt, arg_list ap, char *endStr) //maks
{
    /* copy of _UPRINTF that works with EiC's run time stack.
     * Orginally developed from James Hendrix's Small C tools.
     *
     * (c) Edmond J.Breen (March, 1995).
     *           ALL RIGHTS RESERVED.
     *
     * Purpose: To perform formatted printing to a function.
     *          It performs a superset of printf type
     *          operations.
     *
     * 		  _UPRINTF   RECOGNISES:
     * FLAGS:
     *         -, +, 0 and #.
     *    plus ! and |  (see below).
     *
     * CONVERSION CHARACTERS:
     *         d, i, o, x, X, u, c, s,
     *         f, e, E, g, G, p, n, %
     *    plus b   (see below).
     *
     * LENGTH MODIFIES:
     *          l, L, and h.
     *
     * The WIDTH and or the PRECISION can be set indirectly
     *  via * as specified by K&R.
     *
     *        _UPRINTF EXTRA FEATURES.
     * NEW FLAGS:
     *        FLAG     WHAT IT SPECIFIES
     *         |      Centre justification.
     *         !      Used with floating point numbers.
     *                   It specifies, if possible, that the
     *                   number will be centred with respect
     *                   to the decimal point.
     *                   If used with non floating point
     *                   numbers or the floating point number
     *                   does not contain a decimal point,
     *                   the ! flag is equivalent to |.
     * NEW CONVERSION CHARACTER:
     *      CHARACTER	WHAT IT SPECIFIES
     *         b      int, unsigned binary notation.
     */
  
    int v;

    static char str[128];
    char *sptr, lseen,Lseen;
  
    int  left, right, centre, maxchr,
    pad, len, width, sign, dprec,
    mod, prec,dot,gotfloat;
  
    v = 0;

    while(*fmt) {
		if(*fmt != '%') {
			if(!(*output)(*fmt,arg, endStr)) return v; //maks
			++fmt;++v;
			continue;
		} else if(*++fmt =='%') {
			if(!(*output)(*fmt++,arg, endStr)) return v; //maks
			++v;
			continue;
		}
		pad = ' ';
		centre = len = right = left = 0;
		gotfloat = dot = sign = mod = 0;
		Lseen = lseen  = dprec = 0;
		while(*fmt)  {		/* collect in any order */
			if(*fmt == '-')  		left = 1;
			else if(*fmt == '0')  	pad  = '0';
			else if(*fmt == '+') 	sign = 1;
			else if(*fmt == '#') 	mod = 1;
			else if(*fmt == '|')        centre = 1;
			else if(*fmt == '!')        centre = 1, dot = 1;
			else
				break;
			++fmt;
		}
		if(isdigit(*fmt)) {
			width = atoi(fmt);
			while(isdigit(*fmt)) ++fmt;
		} else  if(*fmt == '*') {
			width = nextarg(ap,int);
			fmt++;
		} else
			width = -1;
		if(*fmt == '.')  {
			if(*++fmt == '*')  {
				maxchr = nextarg(ap,int);
				fmt++;
			} else {
				maxchr = atoi(fmt);
				while(isdigit(*fmt)) ++fmt;
			}
		} else
			maxchr = -1;
		
		switch(*fmt) {		/* check for length modifier*/
		case 'h': fmt++;break;
		case 'l': lseen = 1; fmt++;break;
		case 'L': Lseen = 1; fmt++;break;
		}
		sptr = str;
		switch(*fmt++) {
		case 'c': sptr[0] = (char)nextarg(ap,int);
			sptr[1] = 0;
			break;
		case 'b':
			if(lseen) ultoa(nextarg(ap,long),sptr,2);
			else utoa(nextarg(ap,int),sptr,2);
			break;
		case 'i':
		case 'd': if(lseen) ltoa(nextarg(ap,long),sptr,10);
			else itoa(nextarg(ap,int),sptr,10);
			dprec=1;
			break;
		case 'u': if(lseen) ultoa(nextarg(ap,unsigned long),sptr,10);
			else utoa(nextarg(ap,unsigned),sptr,10);
			dprec=1;
			break;
		case 'o':	if(mod) *sptr = '0';
			if(lseen)ltoa(nextarg(ap,long),&sptr[mod],8);
			else itoa(nextarg(ap,int),&sptr[mod],8);
			dprec = 1;
			break;
		case 's': sptr = nextarg(ap,ptr_t).p; break;
		case 'x': case 'X':
			if(mod) {
				*sptr = '0', sptr[1] = *(fmt-1);
				mod++;
			}
			if(lseen)ultoa(nextarg(ap,long),&sptr[mod],16);
			else utoa(nextarg(ap,int),&sptr[mod],16);
			if(*(fmt-1) == 'X') {
				while(*sptr) {
					*sptr = toupper(*sptr);
					sptr++;
				}
				sptr = str;
			}
			dprec = 1;
			break;
			
		case 'e': case 'E': case 'g': case 'G':
		case 'f': {
			int trunc = 0;
			char type = *(fmt-1),c;
			double d;
			
			gotfloat = 1;
			if(maxchr < 0) prec = 6;
			else prec = maxchr,maxchr = -1;
			
			if(Lseen)
				d = nextarg(ap,double);
			else
				d = nextarg(ap,double);
			
			if(type == 'g' || type == 'G') {
				double ex;
				if(d !=0)
					ex = log10(d < 0? -d:d);
				else
					ex = 1;
				if(ex < -4 || ex >= prec)
					c = type - 2;
				else
					c = 'f';
				trunc = 1;
			} else
				c = type;
			if(mod) {
				if(!prec) prec = 1;
				trunc = 0;
			}
			sptr = fftoa(d,str, prec, c,trunc);
				  }
			break;
		case 'n': *(int*)nextarg(ap,ptr_t).p = v; continue;
		case 'p':
			ultoa((long)nextarg(ap,ptr_t).p,sptr,16);
			break;
		default: if(!(*output)(*(fmt-1),arg, endStr)) return v; //maks
			while(*fmt) 
			{
				if(!(*output)(*fmt++,arg, endStr)) return v; //maks
				++v;
			}
			
			return v;
		}
		
		
		if(!len && sptr) len = strlen(sptr);
		if(sign && sptr) len += (*sptr == '-' ? 0: 1); //maks
		
		if(dprec && maxchr >=0) {
			dprec = maxchr;
			pad = '0';
			
		}else
			dprec = 0;
		
		
		if(maxchr > -1 && maxchr<len)
			len = maxchr;
		if(width>len)
			width = width -len;
		else if(dprec > len)
			width = dprec - len;
		else
			width = 0;
		
		if(centre) 
		{
			left = (right = width >> 1) + (width%2);
			if(dot && gotfloat && sptr)  //maks
			{
				int d = 0,c;
				
				while(sptr[d] && sptr[d] != '.')
					d++;
				
				if(sptr[d] == '.') 
				{
					c = (width + len)/2;
					if(sign && sptr[0] != '-')
						d++;
					
					if(c-d > 0) right = c-d;
					else right = 0;
					
					if(width - right > 0)
						left = width - right;
					else
						left = 0;
				}
			}
		} else 
			if(!left)
				right = width;
			else
				left = width;
			
			if(sign && !left && pad == '0') 
			{
				if(sptr && *sptr != '-') //maks
				{
					if(!(*output)('+',arg, endStr)) return v; //maks
					++v;len--;
				}
				
				sign = 0;
			}
			
			while(right--) {if(!(*output)(pad,arg, endStr)) return v; ++v;} //maks

			if(sign && sptr && *sptr != '-') {if(!(*output)('+',arg, endStr)) return v; len--;++v;} //maks

			while(len-- && sptr) //maks
			{
				if(!(*output)(*sptr++,arg, endStr)) return v;++v;
			}

			while(left--) {if(!(*output)(pad,arg, endStr)) return v;++v;} //maks
    }
    return v;
}



int _eicUscanf(int (*input)(), int (*uget)(),
	    void *arg, const char *fmt, arg_list ap)
{
    /*_USCANF
     *
     * (C) May 10 1995 Edmond J.Breen.
     *           ALL RIGHTS RESERVED.
     *
     * Purpose: To perform formatted reading from a function.
     *          _Uscanf performs a superset of scanf type
     *          operations.
     *
     * 		  _USCANF   RECOGNISES:
     *    
     *     %[*] [width] [h|l|L] [b|d|i|o|x|X|u|c|s|f|e|E|g|G|p|n|%] 
     *     %[*] [width] [scan-set]
     *
     * CONVERSION CHARACTERS:
     *         d, i, o, x, X, u, c, s
     *         f, e, E, g, G, p, n, %
     *    plus b   (see below).
     *
     * LENGTH MODIFIES:
     *          l, L, and h.
     *
     *     *    optional assignment-suppression character
     * WIDTH    optional integer width of field specifier
     *
     * SCAN-SET
     *      [...]   matches the longest non-emtpy string
     *              of input from the set between the brackects.
     *      [^...]  matches the longest non-emtpy string
     *              of input NOT from the set between the brackects.
     *      []...] and [^]...]
     *                  includes ] as part of the scan-set.
     *
     * NEW CONVERSION CHARACTER:
     *      CHARACTER	WHAT IT SPECIFIES
     *         b      int, unsigned binary notation.
     *
     * STANDARD C AUXILIARY FUNCTIONS REQUIRED 
     *    strtol, strtod, strtoul
     *
     * LIMITATIONS:
     *       (1) long double handled as double
     *       (2) error checking could be improved
     */

    static char field[256], *p;
    
    char *carg, sbuf;
    int type;
    int  wast, width,cwidth, ch,  base,cnt;
    void *vptr;
    int v;

    
    cnt = v = 0;
    
    while(*fmt != '\0') {
	if(isspace(*fmt)) {  /* match white space */
	    while(*fmt && isspace(*fmt)) ++fmt;
	    if(*fmt) {
		while(isspace((ch=(*input)(arg))))
		    ++cnt;
		(*uget)(ch,arg);
	    }
	    continue;
	}
	if(*fmt != '%') { /* match literal text */
	    while(*fmt && !isspace(*fmt) && *fmt != '%')
		if((ch=(*input)(arg)) == *fmt) 
		    ++fmt,++cnt;
		else
		    return v;
	    continue;
	}
	    
	if(*++fmt == '*') {
	    vptr = &sbuf;
	    wast = 1;
	    ++fmt;
	} else if(*fmt == '%') {
	    cnt++;
	    fmt++;
	    continue;
	} else  {
	    wast = 0;
	    vptr = nextarg(ap,ptr_t).p;
	}

	for(width = 0;isdigit(*fmt);fmt++)
	    width = *fmt - '0' + width * 10;
	if(!width)
	    width = INT_MAX, cwidth = 1;
	else
	    cwidth = width;

	if(*fmt != 'c' && *fmt != '[' && *fmt != 'n') {
	    /* strip leading white space */
	    while(isspace(ch = (*input)(arg)))
		++cnt;
	    if(ch == EOF) {
		if(v)
		    break;
		else
		    return EOF;
	    }
	    (*uget)(ch,arg);
	}

	
	switch(*fmt) {
	  case 'h': type = 'h'; ++fmt; break;
	  case 'l': type = 'l'; ++fmt; break;
	  case 'L': type = 'L'; ++fmt; break;
	  default:  type = 0;
	}
	    
	switch(*fmt) {
	  case 'c': carg = vptr;
	    while(cwidth--) {
		*carg = (*input)(arg);
		++cnt;
		if(carg != &sbuf)
		    ++carg;
	    }
	    break;
	  case 's':
	    carg = vptr;
	    while(width--) {
		if((*carg = (*input)(arg)) == EOF)
		    break;
		++cnt;
		if(isspace(*carg)) {
		    (*uget)(*carg,arg);
		    --cnt;
		    break;
		} if(carg != &sbuf)
		    ++carg;
	    }
	    *carg = '\0';
	    break;
	  case '[':
	    ++fmt;
	    
	    if(*fmt == '^') {++fmt; type = 0;}
	    else type = 1;
	    
	    p = (char*)fmt;
	    if(*p == ']') ++p;
	    
	    while(*p && *p != ']') ++p;
	    cwidth = p - fmt;
	    if(cwidth == 0) return EOF;
	    carg = vptr;
	    while((ch = (*input)(arg)) != EOF) 
		if(type) {
		    if(memchr(fmt,ch,cwidth)) {
			if(!wast)
			    *carg++ = ch;
			++cnt;
		    }else {
			(*uget)(ch,arg);
			break;
		    }
		} else 
		    if(!memchr(fmt,ch,cwidth)) {
			if(!wast)
			    *carg++ = ch;
			++cnt;
		    }else {
			(*uget)(ch,arg);
			break;
		    }
	    *carg = '\0';
	    fmt += cwidth;
	    break;	    
	  case 'e': case 'f': case 'g':
	  case 'E': case 'G':
	    p = field;
	    ch = (*input)(arg);
	    if(ch == '-' || ch == '+') {
		++cnt;
		*p++ = ch;
	    } else
		(*uget)(ch,arg);
	    while(width-- && isdigit(ch=(*input)(arg))) {
		++cnt; *p++ = ch;
	    }
	    if(ch == '.' && width-- > 0) {
		*p++ = '.';
		++cnt;
		while(isdigit(ch=(*input)(arg)) && width--) {
		    ++cnt;
		    *p++ = ch;
		}
	    }
	    if((ch == 'e' || ch == 'E') && width--) {
		*p++ = ch;
		ch = (*input)(arg);
		if(ch != '+' && ch != '-' && !isdigit(ch) &&
		   ch != EOF && !isspace(ch)) {
		    (*uget)(ch,arg);
		    ch = *--p;
		} else
		    cnt++;
		    
	    }
	    if((ch == '+' || ch == '-') && width--) {
		*p++ = ch;
		++cnt;
		ch = (*input)(arg);
	    }		
	    while(isdigit(ch) && width--) {
		*p++ = ch;
		++cnt;
		ch = (*input)(arg);
	    }
	    (*uget)(ch,arg);
	    if(p == field)
		return v;
	    *p = '\0';
	    if(!wast){
		double d = strtod(field,NULL);
		if(!type || type == 'h')
		    *(float*)vptr = (float)d;
		else if(type == 'l' || type == 'L')
		    *(double*)vptr = d;
	    }
	    break;
	  case 'n':
	    if(type == 'l')
		*(long*)vptr = cnt;
	    else if(type == 'h')
		*(short*)vptr = cnt;
	    else
		*(int*)vptr = cnt;
	    v--;
	    break;	    
	  default:
	    switch(*fmt) {
	    case 'b': base = 2; break;
	    case 'i':
	    case 'd':
	    case 'u':
		ch = (*input)(arg);
		if(ch == '0') base = 8;
		else  base = 10;

		(*uget)(ch,arg);
		break;
	    case 'o': base = 8;  break;
	    case 'x':
	    case 'X': 
	    case 'p': base = 16; break;
	    default:
		return v;
	    }
	    p = field;
	    while(width-- && !isspace(ch=(*input)(arg)) && ch != EOF) {
		*p++ = ch;
		++cnt;
	    }
	    if(width > 0)
		(*uget)(ch,arg);	
	    if(wast)
		break;
	    if(p == field)
		return v;
	    *p = '\0';
	{
	    char *endptr;
	    if(*fmt == 'd' || *fmt == 'i') { /* signed conversion */
		long lval = strtol(field,&endptr,base);
		if(type) {
		    if(type == 'h')
			*(short *)vptr =(short) lval;
		    else 
			*(long *)vptr = lval;
		} else
		    *(int *)vptr = (int) lval;
			
	    } else {
		unsigned long ulval = strtoul(field,&endptr,base);
		if(type) {
		    if(type == 'h')
			*(unsigned short *)vptr =(unsigned short) ulval;
		    else
			*(unsigned long *)vptr = ulval;
		} else
		    *(unsigned *)vptr = (unsigned)ulval;
	    }
	    if(endptr == field) /* failed */
		return v;
	    while(--p >= endptr)
			(*uget)(*p,arg);
	}
	    break;
	}
	++fmt;
	++v;
    }
    return v;
}

#ifndef NO_PIPE
val_t eic_popen()
{
    val_t v;
    arg_list ap = getargs();

#ifdef WIN32
    v.p.sp = v.p.p = _popen(arg(0,ap,ptr_t).p,arg(1,ap,ptr_t).p);
#else
    v.p.sp = v.p.p = popen(arg(0,ap,ptr_t).p,arg(1,ap,ptr_t).p);
#endif

    if((FILE *)(v.p.p) != NULL) {
      int i;
      for(i=1;i<FOPEN_MAX;i++)
	if(book1[i] == 0)
	  break;
      book1[i] = NextFopenEntry++;
      book2[i].fp = (FILE *)(v.p.p);
    }
    setEp( v.p, sizeof(FILE) );
    
    return  v;
}
val_t eic_pclose()
{
  FILE *ptr;
  val_t v;
  int i;

  arg_list ap = getargs();
  
  ptr = arg(0,ap,ptr_t).p;

#ifdef WIN32
  v.ival = _pclose(ptr);
#else
  v.ival = pclose(ptr);
#endif
  
  for(i=0;i<FOPEN_MAX;i++)
    if(ptr == book2[i].fp)
      break;
  if(i<FOPEN_MAX) {
    book1[i] = 0;
    book2[i].fp = NULL;
  }
  return  v;
}
#endif




/* STDIO.C STUFF */
val_t _get_stdin(void)
{
    val_t v;
    /*
     * This function exists so that EiC can get the address stdin.
     */
#if !defined(_WIN32_WCE) //maks
    v.p.sp = v.p.p = stdin;
    setEp( v.p, sizeof(*stdin) );
#else
	v.p.sp = v.p.p = 0;
#endif
    return v;
}
val_t _get_stdout(void)
{
    val_t v;
    /*
     * This function exists so that EiC can get the address stdout.
     */
#if !defined(_WIN32_WCE) //maks
    v.p.sp = v.p.p = stdout;
    setEp( v.p, sizeof(*stdout) );
#else
	v.p.sp = v.p.p = 0;
#endif
    return v;
}
val_t _get_stderr(void)
{
    val_t v;
    /*
     * This function exists so that EiC can get the address stderr.
     */

#if !defined(_WIN32_WCE) //maks
    v.p.sp = v.p.p = stderr;
    setEp( v.p, sizeof(*stderr) );
#else
	v.p.sp = v.p.p = 0;
#endif

    return v;
}


#ifndef NO_FSEEK
val_t eic_ftell(void)
{
    val_t v;

	if(!isValidFilePointer(arg(0,getargs(),ptr_t).p)) {v.ival = -1; return v;} //maks

    v.lval = ftell(arg(0,getargs(),ptr_t).p);
    return v;
}

val_t eic_fseek(void)
{
    val_t v;

	if(!isValidFilePointer(arg(0,getargs(),ptr_t).p)) {v.ival = -1; return v;} //maks

    v.ival = fseek(arg(0,getargs(),ptr_t).p,
		   arg(1,getargs(),long),
		   arg(2,getargs(),int));
    return v;
}
#endif


val_t eic_printf(void)
{
    val_t v;
    arg_list ap = getargs();
    char *fmt;
    
    fmt = nextarg(ap,ptr_t).p;
    #ifdef PPCLIB
    v.ival = _eicUprintf(outbyte,stdout,fmt,ap, NULL);
    #else
    v.ival = _eicUprintf(fputc,stdout,fmt,ap, NULL);
    #endif
    return v;
}

val_t eic_fprintf(void)
{
    val_t v;
    arg_list ap = getargs();
    FILE *fp;
    char *fmt;
    
    fp = nextarg(ap,ptr_t).p;
    fmt = nextarg(ap,ptr_t).p;

	if(!isValidFilePointer(fp)) {v.ival = 0; return v;} //maks

    v.ival = _eicUprintf(fputc,fp,fmt,ap, NULL);
    return v;
}

val_t eic_vfprintf(void)
{
    val_t v;
    arg_list ap = getargs();
    arg_list ags;
    FILE *fp;
    char *fmt;
    
    fp = nextarg(ap,ptr_t).p;
    fmt = nextarg(ap,ptr_t).p;
    ags = nextarg(ap,arg_list);

	if(!isValidFilePointer(fp)) {v.ival = 0; return v;} //maks

    v.ival = _eicUprintf(fputc,fp,fmt,ags, NULL);
    return v;
}

val_t eic_sprintf(void)
{
    val_t v;
    arg_list ap = getargs();
    char *fmt, *str, *endStr = NULL;  //maks

    str = nextarg(ap,ptr_t).p;

	if(((*((ptr_t*)&(ap)->v))).ep > ((*((ptr_t*)&(ap)->v))).sp) //maks
	{
		//Safe
		endStr = ((*((ptr_t*)&(ap)->v))).ep;
	}
	else
	{
		//May be a text in a actor struct
		//So, limit to 255 characters
		endStr = str + 255;
	}
	

    fmt = nextarg(ap,ptr_t).p;    
    v.ival = _eicUprintf(charout_,&str,fmt,ap, endStr); //maks

	if(v.ival == 0) //maks
	{
		//EiC_messageDisplay("EiC sprintf creating an empty string"); //maks: this message will break some created games (http://game-editor.com/forum/viewtopic.php?f=2&t=8648&p=60032)
	}

    return v;
}


val_t eic_vsprintf(void)
{
    val_t v;
    arg_list ap = getargs();
    arg_list ags;
    char * str, *endStr = NULL;  //maks
    char *fmt;
    
    str = nextarg(ap,ptr_t).p;

	if(((*((ptr_t*)&(ap)->v))).ep > ((*((ptr_t*)&(ap)->v))).sp) //maks
	{
		endStr = ((*((ptr_t*)&(ap)->v))).ep;
	}

    fmt = nextarg(ap,ptr_t).p;
    ags = nextarg(ap,arg_list);
    v.ival = _eicUprintf(charout_,&str,fmt,ags, endStr);

    return v;
}

#ifndef NO_FILEIO



val_t eic_freopen(void) //maks
{
    val_t v;
	FILE *fp;
    arg_list ap = getargs();

	fp = arg(2,ap,ptr_t).p;

	if(!isValidFilePointer(fp)) {v.p.sp = v.p.p = NULL; return v;}

    v.p.sp = v.p.p = freopen(arg(0,ap,ptr_t).p,
			     arg(1,ap,ptr_t).p,
			     fp);
    
    setEp( v.p, sizeof(FILE) );
    return v;
}

extern const char *getSavePath(const char *filename); //maks
val_t eic_fopen(void) //maks
{
    val_t v;
    arg_list ap = getargs();
	int i;
	char *name = arg(0,ap,ptr_t).p;

	if(!name) 
	{
		v.p.sp = v.p.p = NULL; 
		return v;
	} 

	for(i=0;i<FOPEN_MAX;i++)
	{
		if(book2[i].fp && strcmp(name, book2[i].name) == 0)
		{
			//File already open
			v.p.sp = v.p.p = book2[i].fp;
			setEp( v.p, sizeof(FILE) );
			return v;
		}
	}

    v.p.sp = v.p.p = fopen(getSavePath(name), arg(1,ap,ptr_t).p);
	
    if((FILE *)(v.p.p) != NULL) 
	{
		int i;
		for(i=1;i<FOPEN_MAX;i++)
		{
			if(book1[i] == 0)
				break;
		}
		
		if(i < FOPEN_MAX)
		{
			book1[i] = NextFopenEntry++;
			book2[i].fp = (FILE *)(v.p.p);
			strcpy(book2[i].name, name);
			book2[i].buffer = NULL;

#ifdef WIN32
	{
	//maks: set the buffer size
	//solve http://code.game-editor.com/ticket/92
	long len;
	fseek(v.p.p, 0, SEEK_END);
	len = ftell(v.p.p);
	fseek(v.p.p, 0, SEEK_SET);
	book2[i].buffer = malloc(len);
	setvbuf(v.p.p, book2[i].buffer, _IOFBF, len);
	}
#endif
		}
		else
		{
			//maks: can't open more files
			fclose((FILE *)(v.p.p));
			v.p.sp = v.p.p = NULL; 
			return v;
		}
    }



    setEp( v.p, sizeof(FILE) );
    return v;
}

val_t eic_ungetc(void)
{
    val_t v;
    arg_list ap = getargs();
    v.ival = ungetc(arg(0,ap,int),
		    arg(1,ap,ptr_t).p);
    return v;
}
    
val_t eic_fgetc(void)
{
    val_t v;
	FILE *fp = nextarg(getargs(),ptr_t).p;

	if(!isValidFilePointer(fp)) {v.ival = EOF; return v;} //maks

    v.ival = fgetc(fp);
    return v;
}
val_t eic_fclose(void)
{
  FILE *ptr;
  val_t v;
  int i;

  v.ival = EOF; //maks

  ptr = nextarg(getargs(),ptr_t).p;

  if(!ptr) return v; //maks

  for(i=0;i<FOPEN_MAX;i++)
  {
    if(ptr == book2[i].fp)
      break;
  }

  if(i<FOPEN_MAX) 
  {
    book1[i] = 0;
    book2[i].fp = NULL;
	if(book2[i].buffer) free(book2[i].buffer);
  }
  else
  {
	  //maks: invalid pointer
	  return v;
  }

  v.ival = fclose(ptr);
  return v;
}
val_t eic_fflush(void)
{
    val_t v;
	FILE *fp = nextarg(getargs(),ptr_t).p; //maks

	if(!isValidFilePointer(fp)) {v.ival = EOF; return v;} //maks

    v.ival = fflush(fp);
    return v;
}

val_t eic_fputc(void) //maks
{
    val_t v;
    arg_list ap = getargs();
	FILE *fp = arg(1,ap,ptr_t).p; 

	if(!isValidFilePointer(fp)) {v.ival = EOF; return v;} //maks

    v.ival = fputc(arg(0,ap,int), fp);
    return v;
}

val_t eic_puts(void)
{
    val_t v;
    arg_list ap = getargs();
    v.ival = puts(arg(0,ap,ptr_t).p);
    return v;
}

val_t eic_fputs(void) //maks
{
    val_t v;
    arg_list ap = getargs();

	FILE *fp = arg(1,ap,ptr_t).p; 

	if(!isValidFilePointer(fp)) {v.ival = EOF; return v;} 

    v.ival = fputs(arg(0,ap,ptr_t).p, fp);
    return v;
}

val_t eic_fgets(void)
{
    val_t v;
    int n, s1; //maks
    arg_list ap = getargs();
	FILE *fp = arg(2,ap,ptr_t).p; //maks

	if(!isValidFilePointer(fp)) {v.p.p = NULL; return v;} //maks

    getptrarg(0,v.p);

    n = arg(1,ap,int);
    s1 = checkEp(v.p,n);

	if(s1 > 0)
	{
		n -= s1;		
	}

    v.p.p = fgets(v.p.p,n, fp);
    return v;
}

val_t eic_gets(void)
{
    int n;
    val_t v;
    getptrarg(0,v.p);

    n = (char*)v.p.ep - (char*)v.p.sp;
    v.p.p = fgets(v.p.p,n,stdin);
    return v;
}

val_t eic_fread(void) //maks
{
    val_t v;
    arg_list ap = getargs();
	FILE *fp = arg(3,ap,ptr_t).p; 

	if(!isValidFilePointer(fp)) {v.szval = 0; return v;} 

    v.szval = fread(arg(0,ap,ptr_t).p,
		   arg(1,ap,size_t),
		   arg(2,ap,size_t),
		   fp);

    return v;
}

val_t eic_fwrite(void) //maks
{
    val_t v;
    arg_list ap;
	FILE *fp;
    ap = getargs();
	fp = arg(3,ap,ptr_t).p; 

	if(!isValidFilePointer(fp)) {v.szval = 0; return v;} 


    v.szval = fwrite(arg(0,ap,ptr_t).p,
		   arg(1,ap,size_t),
		   arg(2,ap,size_t),
		   fp);
    return v;
}

val_t eic_fscanf(void) //maks
{
    val_t v;
    arg_list ap = getargs();
	FILE *fp = arg(0,ap,ptr_t).p; 

	if(!isValidFilePointer(fp)) {v.ival = EOF; return v;} //maks


    v.ival = _eicUscanf(fgetc,ungetc,
		     fp,
		     arg(1,ap,ptr_t).p,ap-2);


    return v;
}

val_t eic_feof(void)
{
    val_t v;
    getptrarg(0,v.p);

	if(!isValidFilePointer((FILE *)(v.p.p))) {v.ival = 1; return v;} //maks

    v.ival = feof((FILE *)(v.p.p));
    return v;
}

val_t eic_ferror(void)
{
    val_t v;
    getptrarg(0,v.p);

	if(!isValidFilePointer((FILE *)(v.p.p))) {v.ival = 1; return v;} //maks

    v.ival = ferror((FILE *)(v.p.p));
    return v;
}

#ifndef _WIN32_WCE //maks
val_t eic_rewind(void)
{
    val_t v;
    getptrarg(0,v.p);

	if(!isValidFilePointer((FILE *)(v.p.p))) {return v;} //maks

    rewind((FILE *)(v.p.p));
    return v;
}
#endif

val_t eic_fsetpos(void)
{
    val_t v;
    arg_list ap = getargs();

	if(!isValidFilePointer(arg(0,ap,ptr_t).p)) {v.ival = 1; return v;} //maks

    v.ival = fsetpos(arg(0,ap,ptr_t).p,
		   arg(1,ap,ptr_t).p);

    return v;
}

val_t eic_fgetpos(void)
{
    val_t v;
    arg_list ap = getargs();

	if(!isValidFilePointer(arg(0,ap,ptr_t).p)) {v.ival = 1; return v;} //maks

    v.ival = fgetpos(arg(0,ap,ptr_t).p,
		   arg(1,ap,ptr_t).p);
    return v;
}

#endif

val_t eic_scanf(void)
{
    val_t v;
    arg_list ap = getargs();
    v.ival = _eicUscanf(fgetc,ungetc,
		     stdin,
		     arg(0,ap,ptr_t).p,ap-1);
    return v;
}

val_t eic_sscanf(void)
{
    val_t v;
    char * str;
    arg_list ap = getargs();
    str = arg(0,ap,ptr_t).p;
    v.ival = _eicUscanf(charin_,charback_,
		     &str,
		     arg(1,ap,ptr_t).p,ap-2);
    return v;
}

#if !defined(_WIN32_WCE) //maks
val_t eic_setvbuf(void)
{
    val_t v;
    arg_list ap = getargs();

	if(!isValidFilePointer(arg(0,ap,ptr_t).p)) {v.ival = 1; return v;} //maks

    v.ival = setvbuf(arg(0,ap,ptr_t).p,
		     arg(1,ap,ptr_t).p,
		     arg(2,ap,int),
		     arg(3,ap,size_t));
    return v;
}
#endif

val_t eic_rename(void)
{
    val_t v;
#if defined(WIN32) || defined(__SYMBIAN32__)
    v.ival = rename(arg(0,getargs(),ptr_t).p,
		  arg(1,getargs(),ptr_t).p);
#else
    v.ival = link(arg(0,getargs(),ptr_t).p,
		  arg(1,getargs(),ptr_t).p);
    if(v.ival == 0)
	v.ival =  unlink(arg(0,getargs(),ptr_t).p);
#endif
    return v;
}

#ifndef NO_TMPFILE
val_t eic_tmpnam(void)
{
    val_t v;
    v.p.sp = v.p.p = tmpnam(arg(0,getargs(),ptr_t).p);
    setEp( v.p, strlen(v.p.p) + 1 );
    return v;
}
val_t eic_tmpfile(void)
{
    val_t v;
    v.p.sp = v.p.p = tmpfile();
    setEp( v.p, strlen(v.p.p) + 1 );
    return v;
}




val_t eic_clearerr(void)
{
    val_t v;
	v.ival = 0; //maks

	if(!isValidFilePointer(arg(0,getargs(),ptr_t).p)) {return v;} //maks

    clearerr((FILE *)(arg(0,getargs(),ptr_t).p));
    return v;
}

val_t eic_perror(void)
{
    val_t v;
	v.ival = 0; //maks

    perror(arg(0,getargs(),ptr_t).p);
    return v;
}
#endif

/**********************************************************************************/

void module_stdio(void) //maks
{
    /*book2[0] = stdin;
    book2[1] = stdout;
    book2[2] = stderr;*/ //maks

    /* stdio.h stuff that were macros */
    //EiC_add_builtinfunc("puts", eic_puts);

    EiC_add_builtinfunc("feof", eic_feof);

    /*EiC_add_builtinfunc("ferror", eic_ferror);
    EiC_add_builtinfunc("rewind", eic_rewind);
    EiC_add_builtinfunc("fsetpos", eic_fsetpos);
    EiC_add_builtinfunc("fgetpos", eic_fgetpos);*/

    /* stdio.h stuff */
    /*EiC_add_builtinfunc("_get_stdin",_get_stdin);
    EiC_add_builtinfunc("_get_stdout",_get_stdout);
    EiC_add_builtinfunc("_get_stderr",_get_stderr);*/


    EiC_add_builtinfunc("ftell", eic_ftell);
    EiC_add_builtinfunc("freopen",eic_freopen);	
    EiC_add_builtinfunc("fopen",eic_fopen);	


    //EiC_add_builtinfunc("printf", eic_printf);

    EiC_add_builtinfunc("fprintf", eic_fprintf);


    EiC_add_builtinfunc("sprintf", eic_sprintf);
    //EiC_add_builtinfunc("vfprintf", eic_vfprintf);
    //EiC_add_builtinfunc("vsprintf", eic_vsprintf);
    //EiC_add_builtinfunc("ungetc", eic_ungetc);

    EiC_add_builtinfunc("fgetc", eic_fgetc);
    EiC_add_builtinfunc("fputc", eic_fputc);
    EiC_add_builtinfunc("fputs", eic_fputs);
    EiC_add_builtinfunc("fgets", eic_fgets);

    //EiC_add_builtinfunc("gets", eic_gets);

    EiC_add_builtinfunc("fread", eic_fread);
    EiC_add_builtinfunc("fwrite", eic_fwrite);

    //EiC_add_builtinfunc("fflush", eic_fflush);

    EiC_add_builtinfunc("fclose", eic_fclose);
    EiC_add_builtinfunc("fscanf", eic_fscanf);

    EiC_add_builtinfunc("sscanf", eic_sscanf);
    //EiC_add_builtinfunc("scanf", eic_scanf);
    //EiC_add_builtinfunc("setvbuf", eic_setvbuf);
    //EiC_add_builtinfunc("tmpnam", eic_tmpnam);
    //EiC_add_builtinfunc("tmpfile", eic_tmpfile);

    EiC_add_builtinfunc("rename",eic_rename);
    EiC_add_builtinfunc("fseek", eic_fseek);

    //EiC_add_builtinfunc("clearerr", eic_clearerr);
    //EiC_add_builtinfunc("perror", eic_perror);
   

#ifndef NO_PIPE
    EiC_add_builtinfunc("popen", eic_popen);
    EiC_add_builtinfunc("pclose", eic_pclose);
#endif

}

