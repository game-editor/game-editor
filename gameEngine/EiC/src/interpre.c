/* interpre.c
 *
 *	(C) Copyright Apr 15 1995, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32_WCE //maks
#	include <signal.h>
#	include <time.h>
extern int CheckGameModeAbort(); //maks
extern int CheckBreakPoints(int line); //maks
#else
typedef int ptrdiff_t;
#endif


#include <stddef.h>
#include <string.h>

#include <limits.h>

#include "MachSet.h"
#include "global.h"
#include "xalloc.h"
#include "typemod.h"
#include "symbol.h"

#include "error.h"

#ifdef __SYMBIAN32__
int raise( int sig ) {return 0;}
#endif



/* make a safe memory block */
#define MASSIGN \


/*#define checkPtr(P,s,i) \
if(( (char *)P.p + i ) > (char *) P.ep || P.p < P.sp) {\
     if(( (char *)P.p + i ) > (char *) P.ep) {\
	 EiC_messageDisplay(s ": attempted beyond allowed access area\n"); }\
     else\
	 EiC_messageDisplay(s ": attempted before allowed access area\n");\
     raise(SIGSEGV);\
}*/

#ifdef WIN32
int gedIsBadWritePtr(void *lp, int size);
int gedIsBadReadPtr(void *lp, int size);
#endif

int checkPtr(ptr_t P, char *s, int i) //maks: no exit on error
{
#if defined(_WIN32_WCE) || defined(GP2X) //maks
	return 1; //Check only in editor mode
#elif WIN32 //maks
	if( (*s == 'R' && gedIsBadReadPtr(P.p, i)) ||
		(*s == 'W' && gedIsBadWritePtr(P.p, i)) )
	{
		char error[128];
		strcpy(error, s); 

		strcat(error, " error in invalid memory area\n"); 
		

		EiC_messageDisplay(error);
		return 0;
	}
	else
#endif
	if(( (char *)P.p + i ) > (char *) P.ep || P.p < P.sp) 
	{
		char error[128];
		strcpy(error, s); 

		if(( (char *)P.p + i ) > (char *) P.ep) 
		{
			strcat(error, " attempted beyond allowed access area\n"); 

			//(char *)P.p = (char *)P.ep - i;
			//if(P.p < P.sp) 
			//	P.p = P.sp;
		}
		else 
		{			
			strcat(error, " attempted before allowed access area\n");
			//P.p = P.sp;
		}

		EiC_messageDisplay(error);
		return 0;
	}

	return 1;
}

int checkPtrSimple(void *p, char *s, int i) //maks: no exit on error
{
#if defined(_WIN32_WCE) || defined(GP2X) //maks
	return 1; //Check only in editor mode
#elif WIN32 //maks
	if( (*s == 'R' && gedIsBadReadPtr(p, i)) ||
		(*s == 'W' && gedIsBadWritePtr(p, i)) )
	{
		char error[128];
		strcpy(error, s); 

		strcat(error, " error in invalid memory area\n"); 
		

		EiC_messageDisplay(error);
		return 0;
	}
#endif

	return 1;
}





#define FMEM         if(AR[ip->ext][ip->val.ival].v.p.sp) {xfree(AR[ip->ext][ip->val.ival].v.p.sp); AR[ip->ext][ip->val.ival].v.p.sp = NULL;} //maks: solve crash in bubble buster

#define LDA                     

#define LVAL

#define stoTYPE(x)   AR[ip->ext][ip->val.ival].v.x =STK[ToP].v.x
#define stoVAL   	 env->LAR[env->lsp].v = STK[ToP].v;env->lsp++
#define pushVAL   	 STK[ToP].v = ip->val;
#define assignTYPE	 env->LAR[env->lsp-1].type = ip->val.p.p
 
#define rvalTYPE(x) STK[ToP].v.x = AR[ip->ext][ip->val.ival].v.x
 

										   
#define drefTYPE(x,y)  {if(checkPtr(STK[ToP].v.p,"READ", sizeof(x) ))\
                       STK[ToP].v.y = *(x*)STK[ToP].v.p.p;\
else goto codeCrash;};




/*#define refMEM        ToP--;\ 
        memcpy(STK[ToP].v.p.p,STK[ToP+1].v.p.p,ip->val.ival);\
        STK[ToP].v.p.sp = STK[ToP].v.p.p;\
        STK[ToP].v.p.ep = (char*)STK[ToP].v.p.p + ip->val.ival;*/

//maks: solve the crash in 'ge fx2.ged'
#define refMEM  \
{\
	if(checkPtr(STK[ToP].v.p, "READ", ip->val.ival) && checkPtr(STK[ToP-1].v.p, "WRITE", ip->val.ival))\
	{\
        ToP--;\
        memcpy(STK[ToP].v.p.p,STK[ToP+1].v.p.p,ip->val.ival);\
        STK[ToP].v.p.sp = STK[ToP].v.p.p;\
        STK[ToP].v.p.ep = (char*)STK[ToP].v.p.p + ip->val.ival;\
	}\
	else\
	{\
		goto codeCrash;\
	}\
};



#define pushTYPE(x)  STK[ToP].v.x = ip->val.x
 
#define  castTYPES(x,y,T)  STK[ToP].v.y = (T)STK[ToP].v.x
 
#define negTYPE(x)   STK[ToP].v.x =  -STK[ToP].v.x
#define landTYPE(x)  STK[ToP].v.ival = STK[ToP].v.x != 0

#define notTYPE(x)  STK[ToP].v.ival = !STK[ToP].v.x

#define compTYPE(x) STK[ToP].v.x = ~STK[ToP].v.x

//maks: without  pop
#define addTYPEf(x)   STK[ToP].v.x += ip->val.x
#define subTYPEf(x)   STK[ToP].v.x -= ip->val.x
#define divTYPEf(x)   if(ip->val.x != 0) {STK[ToP].v.x /= ip->val.x;} else {EiC_messageDisplay("Illegal division by zero");}
#define multTYPEf(x)   STK[ToP].v.x *= ip->val.x 
#define modTYPEf(x)    if(ip->val.x != 0)  {STK[ToP].v.x %= ip->val.x;} else {EiC_messageDisplay("Illegal division by zero");}
#define lshtTYPEf(x)  STK[ToP].v.x <<= ip->val.x
#define rshtTYPEf(x)  STK[ToP].v.x >>= ip->val.x
#define ltTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x < ip->val.x
#define leTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x <= ip->val.x
#define eqTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x == ip->val.x
#define neTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x != ip->val.x
#define gtTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x > ip->val.x
#define geTYPEf(x)   STK[ToP].v.ival = STK[ToP].v.x >= ip->val.x
#define lorTYPEf(x)  STK[ToP].v.ival = STK[ToP].v.x || ip->val.x
#define borTYPEf(x)  STK[ToP].v.x = STK[ToP].v.x | ip->val.x
#define xorTYPEf(x)  STK[ToP].v.x = STK[ToP].v.x ^ ip->val.x
#define andTYPEf(x)  STK[ToP].v.x = STK[ToP].v.x & ip->val.x
#define addPTR2INTf  STK[ToP].v.p.p = (char *) STK[ToP].v.p.p + ip->val.ival
#define addINT2PTRf  STK[ToP].v.p.p = STK[ToP].v.ival + (char *) ip->val.p.p
#define subPTRINTf	 STK[ToP].v.p.p = (char *) STK[ToP].v.p.p - ip->val.ival
#define subPTRf		 STK[ToP].v.ival = (int) ((long) STK[ToP].v.p.p - (long) ip->val.p.p)

#define refTYPEf(x,y)   {if(checkPtr(STK[ToP].v.p,"WRITE", sizeof(x) ))\
	{\
		       *(x*)STK[ToP].v.p.p = ip->val.y;\
				STK[ToP].v.y = ip->val.y;\
	}\
else goto codeCrash;}


//maks: with pop
#define addTYPE(x)   ToP--; STK[ToP].v.x += STK[ToP+1].v.x
#define subTYPE(x)   ToP--; STK[ToP].v.x -= STK[ToP+1].v.x
#define divTYPE(x)   ToP--; if(STK[ToP+1].v.x != 0) {STK[ToP].v.x /= STK[ToP+1].v.x;} else {EiC_messageDisplay("Illegal division by zero");}
#define multTYPE(x)  ToP--;  STK[ToP].v.x *= STK[ToP+1].v.x 
#define modTYPE(x)   ToP--;  if(STK[ToP+1].v.x != 0)  {STK[ToP].v.x %= STK[ToP+1].v.x;} else {EiC_messageDisplay("Illegal division by zero");}
#define lshtTYPE(x)  ToP--; STK[ToP].v.x <<= STK[ToP+1].v.x
#define rshtTYPE(x)  ToP--; STK[ToP].v.x >>= STK[ToP+1].v.x
#define ltTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x < STK[ToP+1].v.x
#define leTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x <= STK[ToP+1].v.x
#define eqTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x == STK[ToP+1].v.x
#define neTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x != STK[ToP+1].v.x
#define gtTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x > STK[ToP+1].v.x
#define geTYPE(x)   ToP--; STK[ToP].v.ival = STK[ToP].v.x >= STK[ToP+1].v.x
#define lorTYPE(x)  ToP--; STK[ToP].v.ival = STK[ToP].v.x || STK[ToP+1].v.x
#define borTYPE(x)  ToP--; STK[ToP].v.x = STK[ToP].v.x | STK[ToP+1].v.x
#define xorTYPE(x)  ToP--; STK[ToP].v.x = STK[ToP].v.x ^ STK[ToP+1].v.x
#define andTYPE(x)  ToP--; STK[ToP].v.x = STK[ToP].v.x & STK[ToP+1].v.x
#define addPTR2INT  ToP--; STK[ToP].v.p.p = (char *) STK[ToP].v.p.p + STK[ToP + 1].v.ival
#define addINT2PTR  ToP--; STK[ToP].v.p.p = STK[ToP].v.ival + (char *) STK[ToP+1].v.p.p
#define subPTRINT	ToP--; STK[ToP].v.p.p = (char *) STK[ToP].v.p.p - STK[ToP + 1].v.ival
#define subPTR		ToP--; STK[ToP].v.ival = (int) ((long) STK[ToP].v.p.p - (long) STK[ToP+1].v.p.p)

#define refTYPE(x,y)   {ToP--;\
	if(checkPtr(STK[ToP].v.p,"WRITE", sizeof(x) ))\
	{\
		       *(x*)STK[ToP].v.p.p = STK[ToP+1].v.y;\
				STK[ToP].v.y = STK[ToP+1].v.y;\
	}\
else goto codeCrash;}

 
#define jmpfTYPE(x)  if(STK[ToP].v.x == 0) {\
                          STK[ToP].v.ival = 0;\
                          ip += ip->val.ival-1;\
                     } else\
                       STK[ToP].v.ival = 1

#define jmptTYPE(x)  if(STK[ToP].v.x != 0) {\
                          STK[ToP].v.ival = 1;\
                          ip += ip->val.ival-1;\
                     } else\
                       STK[ToP].v.ival = 0

#ifdef _DEBUG //maks
void AdjustingTop(int x)   
{
	//Diagnostics here!
}

#define  AdjustTop(x)   if(ToP + (x) >= stksz) {\
				AdjustingTop(x);\
			  stksz += stacksize; \
			  STK =  (STK_t *) xrealloc(STK,sizeof(STK_t)\
						    * stksz); \
			stackBuffer[stackCount - 1] = STK;} //maks 

#else
#define  AdjustTop(x)   if(ToP + (x) >= stksz) {\
			  stksz += stacksize; \
			  STK =  (STK_t *) xrealloc(STK,sizeof(STK_t)\
						    * stksz); \
			stackBuffer[stackCount - 1] = STK;} //maks 

#endif
					  

extern unsigned int EiC_CurrentLine;

#ifndef STAND_ALONE_GAME
extern char *EiC_CurrentFile;
#endif

extern int EiC_traceON;
extern int EiC_traceFunc;
extern int EiC_interActive;



#define stacksize 200
typedef AR_t STK_t;

STK_t *AR[3];
size_t ARGC;

val_t EiC_STaCK_VaLuE;

#define MAX_NESTED_STACKS 200
STK_t *stackBuffer[MAX_NESTED_STACKS];
int stackCount = 0, maxStackBuf = 0;
InsT_t **currentIp = NULL;
extern size_t realAllocatedMem; //maks
int GetGameMode(); //maks

void ClearStack() //maks
{
	//Don't erase the stack in stand alone game mode
	//Solve the crash in bangPPC/bang.ged (stand alone mode)
	if(maxStackBuf > 0 && !GetGameMode())
	{
		int i;

		for(i = 0; i < maxStackBuf; i++)
		{
			xfree(stackBuffer[i]);
			stackBuffer[i] = NULL;
		}

		stackCount = maxStackBuf = 0;
	}
}



void PushFunctionName(char *name); //maks: to runtime error
void PopFunctionName();
void ResetFunctionNameStack();
int FunctionNameStackSize();



#ifndef STAND_ALONE_GAME
int CurrentExecutionLine() 
{
	if(currentIp && *currentIp)
	{
		return (*currentIp)->line;
	}

	return -1;
}
#endif

int IsValidBuiltinFunc(void *vfunc);//maks


void EiC_interpret(environ_t * env)
{

    //int p;
    size_t argc;
    unsigned int ToP;
	InsT_t *ip;
    InsT_t *InSt;
    STK_t *STK, *hold_AR, *hold_AR1;
	//register STK_t *sp;

    /* EXPLANATION OF REGISTERS
     *  ToP          stack top;
     *  p            program counter;
     *  InSt         pointer to instruction set
     */

	
    typedef struct {
	int p;
	unsigned int top;
	int lsp;
	long offset;
	void *file;
	void *inst;
	void *ar;
    } _EiC_jmp_buf;


    extern int EiC_TIMER;
    //extern void EiC_showvalue(AR_t *);
    extern void showcode(FILE *fp, InsT_t * inst,int i);
    int stksz = stacksize;
    int lastln = -1;
    eicstack_t names = {0, NULL};
    val_t v;
    void *vp = NULL;  /* for temporay use only */
    
    unsigned int ON;
    code_t *code;
    
#ifdef _DEBUG
	FILE *fp = NULL;
	//fp = fopen("execution.dat", "a+b");
	//fprintf(fp, "********************* Start *********************\n");
	if(fp)
	{
		short op = 0;
		fwrite(&op, sizeof(op), 1, fp);
	}

#endif


    v.p.p = "::script::"; //maks
	if(stackCount <= 0) 
	{
		//maks: to runtime error
		ResetFunctionNameStack();
	}

	//////////////////////////////////////
	//maks
	if(stackCount >= maxStackBuf)
	{
		if(stackCount >= MAX_NESTED_STACKS)
		{
			//Some error in game (recurse loops, activation event send to activation event, ...)

#ifndef STAND_ALONE_GAME
			EiC_error("Some thing in your code can be caused a loop and game can't execute right");
#endif
			return;
		}

		maxStackBuf++;
		stackBuffer[stackCount] = (STK_t *) xcalloc(sizeof(STK_t) * stacksize, 1);
	}

	STK = stackBuffer[stackCount];
	stackCount++;
	//////////////////////////////////////

    AR[0] = env->AR;
    AR[1] = &env->LAR[env->lsp];
    ip = InSt = env->CODE.inst;
	currentIp = &ip; //maks
    ToP = 0;
    //p = 0;
    
    STK[ToP].v.dval = 0;
    ON = 1;


#ifndef STAND_ALONE_GAME
    EiC_CurrentFile = codeName(&env->CODE);
#endif

    while (ON) 
	{

#ifndef STAND_ALONE_GAME //maks
		if(CheckGameModeAbort() || CheckBreakPoints(ip->line)) //maks
		{
			//Go to editor mode
			goto codeCrash;
		}
#endif

	/*if(EiC_CurrentLine != ip->line)   //maks: performance
	    EiC_CurrentLine = ip->line; */  //for error reporting purposes only 

#ifdef _DEBUG
	//fprintf(fp, "%4d: %3ld ", ip - InSt, ip->opcode); showcode(fp, ip, ip - InSt);  fprintf(fp, "\n");//printf("\tSTK = (%ld, %ld), ToP = %ld", STK[ToP].v.ival, ToP>0?STK[ToP-1].v.ival:0, ToP); putchar('\n'); //maks
	if(fp)
	{
		short op = ip->opcode;
		fwrite(&op, sizeof(op), 1, fp);
	}
#endif

/*#ifdef _DEBUG
	//if(EiC_traceON) 
	{
	    ///*printf("%4d:",p); showcode(&InSt[p],p); putchar('\n');

		if(ip->opcode == eiccall) 
		{
			EiC_eicpush(&names,v);
			v.p.p = ((symentry_t *) STK[ToP - 1].v.p.p)->id;
			//EiC_formatMessage("\n[%s] ",(char*)v.p.p);
			printf("\n[%s] ",(char*)v.p.p);
		}
		
		if(!EiC_traceFunc && lastln != ip->line && ip->line) 
		{
			lastln = ip->line;
			//EiC_formatMessage("%d,",lastln);
			printf("%d,",lastln);
		}
	}
#endif*/

	//#include "processor.c"
	/*#include "processor.c"
	#include "processor.c"
	#include "processor.c"

	#include "processor.c"
	#include "processor.c"
	#include "processor.c"
	#include "processor.c"*/

	/*#include "processor.c"
	#include "processor.c"
	#include "processor.c"
	#include "processor.c"

	#include "processor.c"
	#include "processor.c"
	#include "processor.c"
	#include "processor.c"*/

	/////////////////////////////////////////////////////////////////////////////////
	//processor
	switch (ip->opcode) {
	case bump:
	      AdjustTop(ip->val.ival);
	    ToP += ip->val.ival;
	    break;
	case jmpFint: jmpfTYPE(ival); break;
	case jmpFlng: jmpfTYPE(lval); break;
	case jmpFdbl: jmpfTYPE(dval); break;
	case jmpFptr: jmpfTYPE(p.p); break;
	case jmpFllng: jmpfTYPE(llval); break;
	      
	case jmpTint: jmptTYPE(ival); break;
	case jmpTlng: jmptTYPE(lval); break;
	case jmpTdbl: jmptTYPE(dval); break;
	case jmpTptr: jmptTYPE(p.p); break;
	case jmpTllng: jmptTYPE(llval); break;

	case jmpu: ip += ip->val.ival - 1; break;
	case dupval:
	    /* should really adjustTop here !!*/
		AdjustTop(ip->val.ival + 1); //maks
	    STK[ToP + 1].v = STK[ToP].v;
	    ToP += ip->val.ival;
	    break;
	case jmptab:
	{
	      struct {
		  int n;
		  val_t *loc;
	      } *tab;
	      int i;
	      tab = ip->val.p.p;
	      for (i = 1; i < tab->n; i += 2)
		  if (tab->loc[i].ival == STK[ToP].v.ival) {
		      ip += tab->loc[i + 1].ival - 1;
		      break;
		  }
	      if (i >= tab->n)
		  ip += tab->loc[0].ival - 1;
	  }
	    break;


	  /* specific float stuff */

	case dreffloat: drefTYPE(float, dval); break;
	case reffloat: refTYPE(float, dval); break;
	case rvalfloat:
	    STK[ToP].v.dval=AR[ip->ext][ip->val.ival].v.fval;
	    break;
	case stofloat:
	    AR[ip->ext][ip->val.ival].v.fval = (float)STK[ToP].v.dval;
	    break;

	    
	  /* specific short stuff */
	case rvalshort:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.sval;
	    break;
	case rvalushort:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.usval;
	    break;
	case drefushort: drefTYPE(unsigned short, ival); break; 
	case drefshort: drefTYPE(short, ival); break;
	case refshort: refTYPE(short, ival); break;
	case stoshort:
	    AR[ip->ext][ip->val.ival].v.sval = STK[ToP].v.ival;
	    break;

	  /* specific char stuff */ 
	case rvalchar:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.cval;
	    break;
	case rvaluchar:
	    STK[ToP].v.ival=AR[ip->ext][ip->val.ival].v.ucval;
	    break;
	case stochar:
	    AR[ip->ext][ip->val.ival].v.cval = STK[ToP].v.ival;
	    break;
	case drefuchar: drefTYPE(unsigned char, ival); break;
	case drefchar: drefTYPE(char, ival); break;
	case refchar: refTYPE(char, ival); break;
	case neguchar: STK[ToP].v.uival = 256 - STK[ToP].v.uival; break;

	  /* specific int stuff */
	case incint: STK[ToP].v.ival += ip->val.ival; break;
	case decint: STK[ToP].v.ival -= ip->val.ival; break;
	case drefint: drefTYPE(int, ival); break;
	case refint: refTYPE(int, ival); break;
	case stoint: stoTYPE(ival); break;
	case rvalint: rvalTYPE(ival); break;
	case pushint: pushTYPE(ival); break;
	case negint: negTYPE(ival); break;
	case addint: addTYPE(ival); break;
	case subint: subTYPE(ival); break;
	case divint: divTYPE(ival); break;
	case multint: multTYPE(ival); break;
	case modint: modTYPE(ival); break;
	case lshtint: lshtTYPE(ival); break;
	case rshtint: rshtTYPE(ival); break;
	case ltint: ltTYPE(ival); break;
	case leint: leTYPE(ival); break;
	case eqint: eqTYPE(ival); break;
	case neint: neTYPE(ival); break;
	case gtint: gtTYPE(ival); break;
	case geint: geTYPE(ival); break;
	case lorint: lorTYPE(ival); break;
	case landint: landTYPE(ival); break;
	case notint: notTYPE(ival); break;
	case borint: borTYPE(ival); break;
	case xorint: xorTYPE(ival); break;
	case andint: andTYPE(ival); break;
	case compint: compTYPE(ival); break;
	case int2double: castTYPES(ival, dval, double); break;
	case int2ptr: castTYPES(ival, p.p, void *); break;
	case int2long: castTYPES(ival, lval, long); break;
	case int2llong: castTYPES(ival, llval, eic_llong); break;
	case int2uchar: castTYPES(ival, lval, unsigned char); break;
	case int2ushort: castTYPES(ival, lval, unsigned short); break;
	    
	  /* unsigned int stuff */
	case incuint: STK[ToP].v.uival += ip->val.uival; break;
	case decuint: STK[ToP].v.uival -= ip->val.uival; break;
	case drefuint: drefTYPE(unsigned, uival); break;
	case refuint: refTYPE(unsigned, uival); break;
	case stouint: stoTYPE(uival); break;
	case rvaluint: rvalTYPE(uival); break;
	case pushuint: pushTYPE(uival); break;
	case neguint: negTYPE(uival); break;
	case adduint: addTYPE(uival); break;
	case subuint: subTYPE(uival); break;
	case divuint: divTYPE(uival); break;
	case multuint: multTYPE(uival); break;
	case moduint: modTYPE(uival); break;
	case lshtuint: lshtTYPE(uival); break;
	case rshtuint: rshtTYPE(uival); break;
	case ltuint: ltTYPE(uival); break;
	case leuint: leTYPE(uival); break;
	case equint: eqTYPE(uival); break;
	case neuint: neTYPE(uival); break;
	case gtuint: gtTYPE(uival); break;
	case geuint: geTYPE(uival); break;
	case loruint: lorTYPE(uival); break;

	case notuint: notTYPE(uival); break;
	case boruint: borTYPE(uival); break;
	case xoruint: xorTYPE(uival); break;
	case anduint: andTYPE(uival); break;
	case compuint: compTYPE(uival); break;
	case uint2double: castTYPES(uival, dval, double); break;
	case uint2ptr: castTYPES(uival, p.p, void *); break;
	case uint2long: castTYPES(uival, lval, long); break;
	case uint2llong: castTYPES(uival, llval, eic_llong); break;

	  /* specific long stuff */
	case inclong: STK[ToP].v.lval += ip->val.ival; break;
	case declong: STK[ToP].v.lval -= ip->val.ival; break;
	case dreflong: drefTYPE(long, lval); break;
	case reflong: refTYPE(long, lval); break;
	case stolong: stoTYPE(lval); break;
	case rvallong: rvalTYPE(lval); break;
	case pushlong: pushTYPE(lval); break;
	case neglong: negTYPE(lval); break;
	case addlong: addTYPE(lval); break;
	case sublong: subTYPE(lval); break;
	case divlong: divTYPE(lval); break;
	case multlong: multTYPE(lval); break;
	case modlong: modTYPE(lval); break;
	case lshtlong: lshtTYPE(lval); break;
	case rshtlong: rshtTYPE(lval); break;
	case ltlong: ltTYPE(lval); break;
	case lelong: leTYPE(lval); break;
	case eqlong: eqTYPE(lval); break;
	case nelong: neTYPE(lval); break;
	case gtlong: gtTYPE(lval); break;
	case gelong: geTYPE(lval); break;
	case lorlong: lorTYPE(lval); break;
	case landlong: landTYPE(lval); break;
	case notlong: notTYPE(lval); break;
	case borlong: borTYPE(lval); break;
	case xorlong: xorTYPE(lval); break;
	case andlong: andTYPE(lval); break;
	case complong: compTYPE(lval); break;
	case long2double: castTYPES(lval, dval, double); break;
	case long2ptr: castTYPES(lval, p.p, void *); break;
	case long2llong: castTYPES(lval, llval, eic_llong); break;
	case long2int: castTYPES(lval, ival, int); break;

	    /* unsigned long stuff */
	case inculong: STK[ToP].v.ulval += ip->val.ival; break;
	case deculong: STK[ToP].v.ulval -= ip->val.ival; break;
	case drefulong: drefTYPE(unsigned long, ulval); break;
	case refulong: refTYPE(unsigned long, ulval); break;
	case stoulong: stoTYPE(ulval); break;
	case rvalulong: rvalTYPE(ulval); break;
	case pushulong: pushTYPE(ulval); break;
	case negulong: negTYPE(ulval); break;
	case addulong: addTYPE(ulval); break;
	case subulong: subTYPE(ulval); break;
	case divulong: divTYPE(ulval); break;
	case multulong: multTYPE(ulval); break;
	case modulong: modTYPE(ulval); break;
	case lshtulong: lshtTYPE(ulval); break;
	case rshtulong: rshtTYPE(ulval); break;
	case ltulong: ltTYPE(ulval); break;
	case leulong: leTYPE(ulval); break;
	case equlong: eqTYPE(ulval); break;
	case neulong: neTYPE(ulval); break;
	case gtulong: gtTYPE(ulval); break;
	case geulong: geTYPE(ulval); break;
	case lorulong: lorTYPE(ulval); break;

	case notulong: notTYPE(ulval); break;
	case borulong: borTYPE(ulval); break;
	case xorulong: xorTYPE(ulval); break;
	case andulong: andTYPE(ulval); break;
	case compulong: compTYPE(ulval); break;
	case ulong2double: castTYPES(ulval, dval, double); break;
	case ulong2ptr: castTYPES(ulval, p.p, void *); break;
	case ulong2int: castTYPES(ulval, ival, int); break;
	case ulong2llong: castTYPES(ulval, llval, eic_llong); break;



	  /* specific long long stuff */
	case incllong: STK[ToP].v.llval += ip->val.ival; break;
	case decllong: STK[ToP].v.llval -= ip->val.ival; break;
	case drefllong: drefTYPE(eic_llong, llval); break;
	case refllong: refTYPE(eic_llong, llval); break;
	case stollong: stoTYPE(llval); break;
	case rvalllong: rvalTYPE(llval); break;
	case pushllong: pushTYPE(llval); break;
	case negllong: negTYPE(llval); break;
	case addllong: addTYPE(llval); break;
	case subllong: subTYPE(llval); break;
	case divllong: divTYPE(llval); break;
	case multllong: multTYPE(llval); break;
	case modllong: modTYPE(llval); break;
	case lshtllong: lshtTYPE(llval); break;
	case rshtllong: rshtTYPE(llval); break;
	case ltllong: ltTYPE(llval); break;
	case lellong: leTYPE(llval); break;
	case eqllong: eqTYPE(llval); break;
	case nellong: neTYPE(llval); break;
	case gtllong: gtTYPE(llval); break;
	case gellong: geTYPE(llval); break;
	case lorllong: lorTYPE(llval); break;
	case landllong: landTYPE(llval); break;
	case notllong: notTYPE(llval); break;
	case borllong: borTYPE(llval); break;
	case xorllong: xorTYPE(llval); break;
	case andllong: andTYPE(llval); break;
	case compllong: compTYPE(llval); break;
	case llong2double: castTYPES(llval, dval, double); break;
	case llong2ptr: castTYPES(llval, p.p, void *); break;
	case llong2int: castTYPES(llval, ival, int); break;
	case llong2long: castTYPES(llval, lval, long); break;


	    /* specific double stuff */
	case incdouble:STK[ToP].v.dval += ip->val.ival; break;
	case decdouble:STK[ToP].v.dval -= ip->val.ival; break;
	case drefdouble: drefTYPE(double, dval); break;
	case refdouble: refTYPE(double, dval); break;
	case stodouble: stoTYPE(dval); break;
	case rvaldouble: rvalTYPE(dval); break;
	case pushdouble: pushTYPE(dval); break;
	case negdouble: negTYPE(dval); break;
	case adddouble: addTYPE(dval); break;
	case subdouble: subTYPE(dval); break;
	case divdouble: divTYPE(dval); break;
	case multdouble: multTYPE(dval); break;
	case ltdouble: ltTYPE(dval); break;
	case ledouble: leTYPE(dval); break;
	case eqdouble: eqTYPE(dval); break;
	case nedouble: neTYPE(dval); break;
	case gtdouble: gtTYPE(dval); break;
	case gedouble: geTYPE(dval); break;
	case lordouble: lorTYPE(dval); break;
	case landdouble: landTYPE(dval); break;
	case notdouble: notTYPE(dval); break;

	case double2int: castTYPES(dval, ival, int); break;//maks: GP2X will convert negative doubles to 0 in the code: case double2int: castTYPES(dval, uival, unsigned int); break;
	case double2long: castTYPES(dval, lval, long); break;//maks: GP2X will convert negative doubles to 0 in the code: case double2long: castTYPES(dval, ulval, unsigned long); break;
	case double2llong: castTYPES(dval, llval, eic_llong); break;
	case double2float: castTYPES(dval, fval, float); break;


	  /*specific pointer stuff */
	case incptr: STK[ToP].v.p.p = (char *) STK[ToP].v.p.p +
			 ip->val.ival; break;
	case decptr: STK[ToP].v.p.p = (char *) STK[ToP].v.p.p -
			 ip->val.ival; break;
	case lda:
#if 0
	    STK[ToP].v.p = AR[1][ip->val.ival].v.p;
	    STK[ToP].v.p.p = (char*)STK[ToP].v.p.sp + ip->ext;
	    STK[ToP].v.p.sp = STK[ToP].v.p.p;
#else

	    {
		ptr_t *q = &AR[1][ip->val.ival].v.p;
		ptr_t *a = &STK[ToP].v.p;
		
		a->p   = a->sp = (char*)q->sp + ip->ext;
		a->ep  = q->ep;

		if(vp) //maks: Solve the crash in 'morcior/stars/stars.ged'
		{ 
			ptr_t *p = (ptr_t *)vp;
			if(checkPtrSimple(p, "WRITE", sizeof(ptr_t)))
			{
				/* patch previous lda assignment */
				p->ep = (char*)a->p;
			}
			else
			{
				goto codeCrash;
			}			
		}

		/* Take advantage of the fact that the next InSt
		 * has the location of where `a' is to be stored.
		 */
		vp = &AR[1][(ip+1)->val.ival].v.p;
		
	    }
	    
#endif
	    
	    break;
	case ixa:
	    ToP--;
	    STK[ToP].v.p.p = (char *) STK[ToP].v.p.p + STK[ToP+1].v.ival*ip->val.ival;
	    break;
	case addptr2int: addPTR2INT; break;
	case addint2ptr: addINT2PTR; break;
	case subptrint:	 subPTRINT; break;
	case subptr: subPTR; break;

	case drefptr:   drefTYPE(ptr_t, p); break;

	case drefuptr:  drefTYPE(void**,p.p);
	                STK[ToP].v.p.sp = 0;
			STK[ToP].v.p.ep = (void *) ULONG_MAX;
			break;

	case refptr: refTYPE(ptr_t, p); break;
	case refuptr: refTYPE(void *, p.p); break;

	case stoptr: stoTYPE(p); break;
	case stouptr: stoTYPE(p.p); break;
	    
	case rvalptr: rvalTYPE(p); break;
	case rvaluptr: rvalTYPE(p);
	  STK[ToP].v.p.sp = 0;
	  STK[ToP].v.p.ep = (void*) ULONG_MAX; /* set to a very high value */
	    break;

	case pushptr: pushTYPE(p); break;
	case ltptr: ltTYPE(p.p); break;
	case leptr: leTYPE(p.p); break;
	case eqptr: eqTYPE(p.p); break;
	case neptr: neTYPE(p.p); break;
	case gtptr: gtTYPE(p.p); break;
	case geptr: geTYPE(p.p); break;
	case lorptr: lorTYPE(p.p); break;
	case landptr: landTYPE(p.p); break;
	case notptr: notTYPE(p.p); break;
	case ptr2int: castTYPES(p.p, ival, int); break;
	case ptr2long: castTYPES(p.p, lval, long); break;
	    
	case lval: /* on the fly safe pointer */
	    STK[ToP].v.p.p = &AR[ip->ext][ip->val.ival].v;
	    STK[ToP].v.p.ep = (char *) STK[ToP].v.p.p + (size_t) 
		ip->val.p.ep;
	    STK[ToP].v.p.sp = STK[ToP].v.p.p;
	    break;
	case assigntype: assignTYPE; break;
	case stoval:
	    switch(ip->ext) {
	      case t_char:
	      case t_uchar: STK[ToP].v.cval = STK[ToP].v.ival; break;
	      case t_short:
	      case t_ushort: STK[ToP].v.sval = STK[ToP].v.ival; break;
	      case t_float:  STK[ToP].v.fval = STK[ToP].v.dval;
/*	      case t_struct:
	      case t_union:
		printf("stoVa1l with struct/union\n");
*/
	    }

	    stoVAL; break;
	case pushval: pushVAL; break;
	    
	case eiccall:
		

	    if(!((symentry_t*)STK[ToP - 1].v.p.p)->tag) 
		{
			AdjustTop(10 /*6*/); //maks
			STK[ToP + 1].v.ival = ip; //maks: int = ip?

#ifndef STAND_ALONE_GAME
			STK[ToP + 1].type = (void*)EiC_CurrentFile; /* save file */
#endif

			STK[ToP + 2].v.p.p = InSt;
			STK[ToP + 3].v.lval = AR[1] - env->LAR;
			STK[ToP + 4].v.ival = env->lsp - STK[ToP].v.ival;
			
			AR[1] = &env->LAR[env->lsp] /* - STK[ToP].v.ival] */ ;
			
			code = ! STK[ToP - 1].v.p.p ? NULL :
			AR[0][((symentry_t *)
				STK[ToP - 1].v.p.p)->val.ival].v.p.p;
			
			if (code == NULL) 
			{
				if(STK[ToP - 1].v.p.p) 
				{
					EiC_formatMessage("Link error: undefined function :-> %s\n",
						((symentry_t *) STK[ToP - 1].v.p.p)->id);
				} 
				else
				{
					EiC_formatMessage("Link error: possible usage of a function pointer"
					" before assignment.\n");
				}

				env->lsp = 0;
				//raise(SIGINT); //maks: non exit on error
				goto codeCrash;
			}
			
			//maks: to runtime error
			PushFunctionName(((symentry_t *) STK[ToP - 1].v.p.p)->id);
			
#ifndef STAND_ALONE_GAME	
			EiC_CurrentFile = codeName(code);
#endif
			
			InSt = code->inst;
			ip = InSt - 1;
			ToP += 5;
			
			//if(EiC_traceON) lastln = -1; //maks: performance
			
			break;
		}

	case call: //maks
		{
		unsigned int ToPaux;

		/*argc = ARGC;
		hold_AR = AR[2];
		hold_AR1 = AR[1];
		ARGC = STK[ToP].v.ival;
		
		//AR[2] = &env->LAR[env->lsp - ARGC];
		AR[2] = &env->LAR[env->lsp];

		if(ip->ext)
			STK[ToP - 1].v = STK[ToP - 1].v.vfunc ();
		else
			STK[ToP - 1].v.vfunc();

		//env->lsp -= STK[ToP].v.ival;
		if(env->lsp >= STK[ToP].v.ival) //maks: direct execution
			env->lsp -= STK[ToP].v.ival;
		else
			env->lsp = 0; 

		ARGC = argc;
		AR[2] = hold_AR;
		AR[1] = hold_AR1;
		ToP--;*/
		
		//Save state into stack
		AdjustTop(4 /*2*/); //maks
		STK[ToP + 1].v.ival = ARGC;
		STK[ToP + 2].v.ival = env->lsp;

		//Save into locals
		argc = ARGC;
		hold_AR = AR[2];
		hold_AR1 = AR[1];
		ARGC = STK[ToP].v.ival;
		
		//AR[2] = &env->LAR[env->lsp - ARGC];
		AR[2] = &env->LAR[env->lsp];

		ToPaux = ToP;
		ToP += 2;

		
		if(IsValidBuiltinFunc(STK[ToPaux - 1].v.vfunc)) //maks
		{
			//Go
			if(ip->ext)
				STK[ToPaux - 1].v = STK[ToPaux - 1].v.vfunc ();
			else
				STK[ToPaux - 1].v.vfunc();
		}
		else
		{
			if(FunctionNameStackSize()) currentIp = &ip;

			EiC_formatMessage("Crash in the code.\n   This error can causes unpredictable results in your game.\n   So, is better save your game now, restart Game Editor and try again.\n   If the error persists, please contact: support@game-editor.com");
			goto codeCrash;		
		}


		//Restore state
		ToP -= 2;		// over write code pointer
		env->lsp = STK[ToP + 2].v.ival;
		argc = STK[ToP + 1].v.ival;



		//env->lsp -= STK[ToP].v.ival;
		if(env->lsp >= STK[ToP].v.ival) //maks: direct execution
			env->lsp -= STK[ToP].v.ival;
		else
			env->lsp = 0; 



		ARGC = argc;
		AR[2] = hold_AR;
		AR[1] = hold_AR1;

		ToP--;

		}
		break;

	case eicreturn:		
		

		if(ToP == 0) //maks: direct execution
		{
			//env->lsp = 0; //maks: don't reset lsp (avoid crash in ALPHA_1_1_4.ged exported game (to script in script execution))
			ON = 0;  
			EiC_STaCK_VaLuE = STK[ToP].v; 
			goto codeEnd;
			break;
		}
		else
		{
			//Don't need pop if is direct execution
			PopFunctionName(); //maks: to runtime error
		}

	    ToP -= 6;		/* over write code pointer */

	    ip = STK[ToP + 2].v.ival; //maks: int = ip?

#ifndef STAND_ALONE_GAME
	    EiC_CurrentFile = (char*)STK[ToP+2].type;
#endif
	    
	    InSt = STK[ToP + 3].v.p.p;
	
	    env->lsp = STK[ToP + 5].v.ival;
	    STK[ToP].v = STK[ToP + 6].v;

	    AR[1] = &env->LAR[STK[ToP + 4].v.ival];
	    /*if(EiC_traceON) //maks: performance
		{
			EiC_eicpop(&names,&v);
			EiC_formatMessage("\n[%s] ", (char*)v.p.p);
			lastln = -1;
	    }*/
	    break;


#if 1
	case __eiclongjmp:
	{

	    _EiC_jmp_buf * reg;

	    reg = (_EiC_jmp_buf *) ((char*)STK[ToP].v.p.p - STK[ToP+1].v.ival);

	    ip = reg->p; //maks: int = ip?

#ifndef STAND_ALONE_GAME
	    EiC_CurrentFile = reg->file;
#endif

	    InSt = reg->inst;
	    env->lsp = reg->lsp;
	    AR[1] = &env->LAR[reg->offset]; /*reg->ar; */

	    if(STK[ToP+1].v.ival == 0)
		 STK[reg->top].v.ival = 1;
	    else
		 STK[reg->top].v.ival = STK[ToP+1].v.ival;

	    ToP = reg->top;


	}

	break;
	    
	case __eicsetjmp:
	{

	    _EiC_jmp_buf * reg;

	    reg = (_EiC_jmp_buf *) STK[ToP].v.p.p;

	    reg->p = ip; //maks: int = ip?

#ifndef STAND_ALONE_GAME
	    reg->file = (void*)EiC_CurrentFile; /* save file */
#endif

	    reg->inst = InSt;
	    reg->lsp =  env->lsp;
	    reg->offset = AR[1] - env->LAR;
	    reg->ar = AR[1];
	    reg->top = ToP;

	    STK[ToP].v.ival = 0;

	}

	break;
#endif

        case massign:   
	    {
		
		val_t *v = &AR[ip->ext][ip->val.ival].v;
		v->p.sp = (void*)xcalloc(STK[ToP].v.ival,1);
		v->p.ep = (char*)v->p.sp + realAllocatedMem;//STK[ToP].v.ival; //maks
		vp = NULL;
		
	    }
	    break;
	case fmem: FMEM; break;
	case refmem: refMEM; break;
	case minit:
	    memcpy(STK[ToP].v.p.p,ip->val.p.p, ip->ext); 
	    break;
	case reducear:
	    //env->lsp -= ip->val.ival;
		if(env->lsp >= ip->val.ival) //maks: direct execution
			env->lsp -= ip->val.ival;
		else
			env->lsp = 0;
	    break;
	case checkar:
		{
			size_t d;
			ptrdiff_t d2;
			d = env->LARsize - env->lsp;

			if (d < ip->val.ival) 
			{
				/* printf("expanding AR %d\n",env->LARsize);*/
				d2 = (AR[1] - env->LAR);
				env->LARsize += ip->val.ival - d;
				env->LAR = (AR_t *) xrealloc(env->LAR, env->LARsize * sizeof(AR_t));
				AR[1] = &env->LAR[(size_t) d2];
			}
			
			if (ip->ext == 0) 
			{
				env->lsp += ip->val.ival;
				/*AR[1][0].v.p.p = NULL;*/
			}
		}
		break;
	case halt: 
		{
			STK[ToP].type = ip->val.p.p;
			ON = 0;  
			EiC_STaCK_VaLuE = STK[ToP].v; 
			goto codeEnd;
			break;
		}

	case empty: break;

	/*maks: big instructions (super operators)*/

	/*case rvaluptr_dreflong_bump:
		{
			//Execute rvaluptr			
			rvalTYPE(p);
			STK[ToP].v.p.sp = 0;
			STK[ToP].v.p.ep = (void*) ULONG_MAX;

			//Execute dreflong
			drefTYPE(long, lval);			

			//Execute bump
			AdjustTop(ip->val1);
			ToP += ip->val1;

			//Ignore next instruction
			ip += 2;
		}
		break;*/

	/*case pushlong_lelong_jmpFint:
		{
			//Execute pushlong
			pushTYPE(lval);

			//Execute lelong
			leTYPE(lval);

			//Increment ip
			ip += 2;		

			//Execute jmpFint
			jmpfTYPE(ival);						
		}
		break;*/
	
	/*bump_push_add...*/
	case bump_push_addint:		addTYPEf(ival);	ip += 2; break;
	case bump_push_adduint:		addTYPEf(uival);ip += 2; break;
	case bump_push_addlong:		addTYPEf(lval);	ip += 2; break;
	case bump_push_addulong:	addTYPEf(ulval);ip += 2; break;
	case bump_push_addllong:	addTYPEf(llval);ip += 2; break;
	case bump_push_adddouble:	addTYPEf(dval);	ip += 2; break;


	/*bump_push_sub...*/
	case bump_push_subint:		subTYPEf(ival);	ip += 2; break;
	case bump_push_subuint:		subTYPEf(uival);ip += 2; break;
	case bump_push_sublong:		subTYPEf(lval);	ip += 2; break;
	case bump_push_subulong:	subTYPEf(ulval);ip += 2; break;
	case bump_push_subllong:	subTYPEf(llval);ip += 2; break;
	case bump_push_subdouble:	subTYPEf(dval);	ip += 2; break;


	/*bump_push_div...*/
	case bump_push_divint:		divTYPEf(ival);	ip += 2; break;
	case bump_push_divuint:		divTYPEf(uival);ip += 2; break;
	case bump_push_divlong:		divTYPEf(lval);	ip += 2; break;
	case bump_push_divulong:	divTYPEf(ulval);ip += 2; break;
	case bump_push_divllong:	divTYPEf(llval);ip += 2; break;
	case bump_push_divdouble:	divTYPEf(dval);	ip += 2; break;


	/*bump_push_mult...*/
	case bump_push_multint:		multTYPEf(ival);	ip += 2; break;
	case bump_push_multuint:	multTYPEf(uival);	ip += 2; break;
	case bump_push_multlong:	multTYPEf(lval);	ip += 2; break;
	case bump_push_multulong:	multTYPEf(ulval);	ip += 2; break;
	case bump_push_multllong:	multTYPEf(llval);	ip += 2; break;
	case bump_push_multdouble:	multTYPEf(dval);	ip += 2; break;


	/*bump_push_mod...*/
	case bump_push_modint:		modTYPEf(ival);		ip += 2; break;
	case bump_push_moduint:		modTYPEf(uival);	ip += 2; break;
	case bump_push_modlong:		modTYPEf(lval);		ip += 2; break;
	case bump_push_modulong:	modTYPEf(ulval);	ip += 2; break;
	case bump_push_modllong:	modTYPEf(llval);	ip += 2; break;


	/*bump_push_lsht...*/
	case bump_push_lshtint:		lshtTYPEf(ival);	ip += 2; break;
	case bump_push_lshtuint:	lshtTYPEf(uival);	ip += 2; break;
	case bump_push_lshtlong:	lshtTYPEf(lval);	ip += 2; break;
	case bump_push_lshtulong:	lshtTYPEf(ulval);	ip += 2; break;
	case bump_push_lshtllong:	lshtTYPEf(llval);	ip += 2; break;


	/*bump_push_rsht...*/
	case bump_push_rshtint:		rshtTYPEf(ival);	ip += 2; break;
	case bump_push_rshtuint:	rshtTYPEf(uival);	ip += 2; break;
	case bump_push_rshtlong:	rshtTYPEf(lval);	ip += 2; break;
	case bump_push_rshtulong:	rshtTYPEf(ulval);	ip += 2; break;
	case bump_push_rshtllong:	rshtTYPEf(llval);	ip += 2; break;


	/*bump_push_lt...*/
	case bump_push_ltint:		ltTYPEf(ival);	ip += 2; break;
	case bump_push_ltuint:		ltTYPEf(uival); ip += 2; break;
	case bump_push_ltlong:		ltTYPEf(lval);	ip += 2; break;
	case bump_push_ltulong:		ltTYPEf(ulval);	ip += 2; break;
	case bump_push_ltllong:		ltTYPEf(llval);	ip += 2; break;
	case bump_push_ltdouble:	ltTYPEf(dval);	ip += 2; break;
	case bump_push_ltptr:		ltTYPEf(p.p);	ip += 2; break;
	

	/*bump_push_le...*/
	case bump_push_leint:		leTYPEf(ival);	ip += 2; break;
	case bump_push_leuint:		leTYPEf(uival); ip += 2; break;
	case bump_push_lelong:		leTYPEf(lval);	ip += 2; break;
	case bump_push_leulong:		leTYPEf(ulval);	ip += 2; break;
	case bump_push_lellong:		leTYPEf(llval);	ip += 2; break;
	case bump_push_ledouble:	leTYPEf(dval);	ip += 2; break;
	case bump_push_leptr:		leTYPEf(p.p);	ip += 2; break;


	/*bump_push_eq...*/
	case bump_push_eqint:		eqTYPEf(ival);	ip += 2; break;
	case bump_push_equint:		eqTYPEf(uival); ip += 2; break;
	case bump_push_eqlong:		eqTYPEf(lval);	ip += 2; break;
	case bump_push_equlong:		eqTYPEf(ulval);	ip += 2; break;
	case bump_push_eqllong:		eqTYPEf(llval);	ip += 2; break;
	case bump_push_eqdouble:	eqTYPEf(dval);	ip += 2; break;
	case bump_push_eqptr:		eqTYPEf(p.p);	ip += 2; break;


	/*bump_push_ne...*/
	case bump_push_neint:		neTYPEf(ival);	ip += 2; break;
	case bump_push_neuint:		neTYPEf(uival); ip += 2; break;
	case bump_push_nelong:		neTYPEf(lval);	ip += 2; break;
	case bump_push_neulong:		neTYPEf(ulval);	ip += 2; break;
	case bump_push_nellong:		neTYPEf(llval);	ip += 2; break;
	case bump_push_nedouble:	neTYPEf(dval);	ip += 2; break;
	case bump_push_neptr:		neTYPEf(p.p);	ip += 2; break;


	/*bump_push_gt...*/
	case bump_push_gtint:		gtTYPEf(ival);	ip += 2; break;
	case bump_push_gtuint:		gtTYPEf(uival); ip += 2; break;
	case bump_push_gtlong:		gtTYPEf(lval);	ip += 2; break;
	case bump_push_gtulong:		gtTYPEf(ulval);	ip += 2; break;
	case bump_push_gtllong:		gtTYPEf(llval);	ip += 2; break;
	case bump_push_gtdouble:	gtTYPEf(dval);	ip += 2; break;
	case bump_push_gtptr:		gtTYPEf(p.p);	ip += 2; break;


	/*bump_push_ge...*/
	case bump_push_geint:		geTYPEf(ival);	ip += 2; break;
	case bump_push_geuint:		geTYPEf(uival); ip += 2; break;
	case bump_push_gelong:		geTYPEf(lval);	ip += 2; break;
	case bump_push_geulong:		geTYPEf(ulval);	ip += 2; break;
	case bump_push_gellong:		geTYPEf(llval);	ip += 2; break;
	case bump_push_gedouble:	geTYPEf(dval);	ip += 2; break;
	case bump_push_geptr:		geTYPEf(p.p);	ip += 2; break;


	/*bump_push_lor...*/
	case bump_push_lorint:		lorTYPEf(ival);	ip += 2; break;
	case bump_push_loruint:		lorTYPEf(uival);ip += 2; break;
	case bump_push_lorlong:		lorTYPEf(lval);	ip += 2; break;
	case bump_push_lorulong:	lorTYPEf(ulval);ip += 2; break;
	case bump_push_lorllong:	lorTYPEf(llval);ip += 2; break;
	case bump_push_lordouble:	lorTYPEf(dval);	ip += 2; break;
	case bump_push_lorptr:		lorTYPEf(p.p);	ip += 2; break;


	/*bump_push_bor...*/
	case bump_push_borint:		borTYPEf(ival);	ip += 2; break;
	case bump_push_boruint:		borTYPEf(uival);ip += 2; break;
	case bump_push_borlong:		borTYPEf(lval);	ip += 2; break;
	case bump_push_borulong:	borTYPEf(ulval);ip += 2; break;
	case bump_push_borllong:	borTYPEf(llval);ip += 2; break;


	/*bump_push_xor...*/
	case bump_push_xorint:		xorTYPEf(ival);	ip += 2; break;
	case bump_push_xoruint:		xorTYPEf(uival);ip += 2; break;
	case bump_push_xorlong:		xorTYPEf(lval);	ip += 2; break;
	case bump_push_xorulong:	xorTYPEf(ulval);ip += 2; break;
	case bump_push_xorllong:	xorTYPEf(llval);ip += 2; break;


	/*bump_push_and...*/
	case bump_push_andint:		andTYPEf(ival);	ip += 2; break;
	case bump_push_anduint:		andTYPEf(uival);ip += 2; break;
	case bump_push_andlong:		andTYPEf(lval);	ip += 2; break;
	case bump_push_andulong:	andTYPEf(ulval);ip += 2; break;
	case bump_push_andllong:	andTYPEf(llval);ip += 2; break;


	/*bump_push_and...*/
	case bump_push_reffloat:	refTYPEf(float, dval);	ip += 2; break;
	case bump_push_refshort:	refTYPEf(short, ival);	ip += 2; break;
	case bump_push_refchar:		refTYPEf(char, ival);	ip += 2; break;
	case bump_push_refint:		refTYPEf(int, ival);		ip += 2; break;
	case bump_push_refuint:		refTYPEf(unsigned, uival);ip += 2; break;
	case bump_push_reflong:		refTYPEf(long, lval);	ip += 2; break;
	case bump_push_refulong:	refTYPEf(unsigned long, ulval);ip += 2; break;
	case bump_push_refllong:	refTYPEf(eic_llong, llval);	ip += 2; break;
	case bump_push_refdouble:	refTYPEf(double, dval);	ip += 2; break;
	case bump_push_refptr:		refTYPEf(ptr_t, p);		ip += 2; break;
	case bump_push_refuptr:		refTYPEf(void *, p.p);	ip += 2; break;

	/*bump_push_ptr...*/
	case bump_push_addptr2int:	addPTR2INTf;	ip += 2; break;
	case bump_push_addint2ptr:	addINT2PTRf;	ip += 2; break;
	case bump_push_subptrint:	subPTRINTf;		ip += 2; break;
	case bump_push_subptr:		subPTRf;		ip += 2; break;



	/*two instructions*/
	
	/*push_add...*/
	case push_addint:	ToP--;			addTYPEf(ival);		ip++;	 break;
	case push_adduint:	ToP--;			addTYPEf(uival);	ip++;	 break;
	case push_addlong:	ToP--;			addTYPEf(lval);		ip++;	 break;
	case push_addulong:	ToP--;		addTYPEf(ulval);	ip++;	 break;
	case push_addllong:	ToP--;		addTYPEf(llval);	ip++;	 break;
	case push_adddouble:	ToP--;		addTYPEf(dval);		ip++;	 break;


	/*push_sub...*/
	case push_subint:	ToP--;			subTYPEf(ival);		ip++;	 break;
	case push_subuint:	ToP--;			subTYPEf(uival);	ip++;	 break;
	case push_sublong:	ToP--;			subTYPEf(lval);		ip++;	 break;
	case push_subulong:	ToP--;		subTYPEf(ulval);	ip++;	 break;
	case push_subllong:	ToP--;		subTYPEf(llval);	ip++;	 break;
	case push_subdouble:	ToP--;		subTYPEf(dval);		ip++;	 break;


	/*push_div...*/
	case push_divint:	ToP--;			divTYPEf(ival);		ip++;	 break;
	case push_divuint:	ToP--;			divTYPEf(uival);	ip++;	 break;
	case push_divlong:	ToP--;			divTYPEf(lval);		ip++;	 break;
	case push_divulong:	ToP--;		divTYPEf(ulval);	ip++;	 break;
	case push_divllong:	ToP--;		divTYPEf(llval);	ip++;	 break;
	case push_divdouble:	ToP--;		divTYPEf(dval);		ip++;	 break;


	/*push_mult...*/
	case push_multint:	ToP--;			multTYPEf(ival);		ip++;	 break;
	case push_multuint:	ToP--;		multTYPEf(uival);		ip++;	 break;
	case push_multlong:	ToP--;		multTYPEf(lval);		ip++;	 break;
	case push_multulong:	ToP--;		multTYPEf(ulval);		ip++;	 break;
	case push_multllong:	ToP--;		multTYPEf(llval);		ip++;	 break;
	case push_multdouble:	ToP--;		multTYPEf(dval);		ip++;	 break;


	/*push_mod...*/
	case push_modint:	ToP--;			modTYPEf(ival);			ip++;	 break;
	case push_moduint:	ToP--;			modTYPEf(uival);		ip++;	 break;
	case push_modlong:	ToP--;			modTYPEf(lval);			ip++;	 break;
	case push_modulong:	ToP--;		modTYPEf(ulval);		ip++;	 break;
	case push_modllong:	ToP--;		modTYPEf(llval);		ip++;	 break;


	/*push_lsht...*/
	case push_lshtint:	ToP--;			lshtTYPEf(ival);		ip++;	 break;
	case push_lshtuint:	ToP--;		lshtTYPEf(uival);		ip++;	 break;
	case push_lshtlong:	ToP--;		lshtTYPEf(lval);		ip++;	 break;
	case push_lshtulong:	ToP--;		lshtTYPEf(ulval);		ip++;	 break;
	case push_lshtllong:	ToP--;		lshtTYPEf(llval);		ip++;	 break;


	/*push_rsht...*/
	case push_rshtint:	ToP--;			rshtTYPEf(ival);		ip++;	 break;
	case push_rshtuint:	ToP--;		rshtTYPEf(uival);		ip++;	 break;
	case push_rshtlong:	ToP--;		rshtTYPEf(lval);		ip++;	 break;
	case push_rshtulong:	ToP--;		rshtTYPEf(ulval);		ip++;	 break;
	case push_rshtllong:	ToP--;		rshtTYPEf(llval);		ip++;	 break;


	/*push_lt...*/
	case push_ltint:	ToP--;			ltTYPEf(ival);		ip++;	 break;
	case push_ltuint:	ToP--;			ltTYPEf(uival); 	ip++;	 break;
	case push_ltlong:	ToP--;			ltTYPEf(lval);		ip++;	 break;
	case push_ltulong:	ToP--;			ltTYPEf(ulval);		ip++;	 break;
	case push_ltllong:	ToP--;			ltTYPEf(llval);		ip++;	 break;
	case push_ltdouble:	ToP--;		ltTYPEf(dval);		ip++;	 break;
	case push_ltptr:	ToP--;			ltTYPEf(p.p);		ip++;	 break;
	

	/*push_le...*/
	case push_leint:	ToP--;			leTYPEf(ival);		ip++;	 break;
	case push_leuint:	ToP--;			leTYPEf(uival); 	ip++;	 break;
	case push_lelong:	ToP--;			leTYPEf(lval);		ip++;	 break;
	case push_leulong:	ToP--;			leTYPEf(ulval);		ip++;	 break;
	case push_lellong:	ToP--;			leTYPEf(llval);		ip++;	 break;
	case push_ledouble:	ToP--;		leTYPEf(dval);		ip++;	 break;
	case push_leptr:	ToP--;			leTYPEf(p.p);		ip++;	 break;


	/*push_eq...*/
	case push_eqint:	ToP--;			eqTYPEf(ival);		ip++;	 break;
	case push_equint:	ToP--;			eqTYPEf(uival); 	ip++;	 break;
	case push_eqlong:	ToP--;			eqTYPEf(lval);		ip++;	 break;
	case push_equlong:	ToP--;			eqTYPEf(ulval);		ip++;	 break;
	case push_eqllong:	ToP--;			eqTYPEf(llval);		ip++;	 break;
	case push_eqdouble:	ToP--;		eqTYPEf(dval);		ip++;	 break;
	case push_eqptr:	ToP--;			eqTYPEf(p.p);		ip++;	 break;


	/*push_ne...*/
	case push_neint:	ToP--;			neTYPEf(ival);		ip++;	 break;
	case push_neuint:	ToP--;			neTYPEf(uival); 	ip++;	 break;
	case push_nelong:	ToP--;			neTYPEf(lval);		ip++;	 break;
	case push_neulong:	ToP--;			neTYPEf(ulval);		ip++;	 break;
	case push_nellong:	ToP--;			neTYPEf(llval);		ip++;	 break;
	case push_nedouble:	ToP--;		neTYPEf(dval);		ip++;	 break;
	case push_neptr:	ToP--;			neTYPEf(p.p);		ip++;	 break;


	/*push_gt...*/
	case push_gtint:	ToP--;			gtTYPEf(ival);		ip++;	 break;
	case push_gtuint:	ToP--;			gtTYPEf(uival); 	ip++;	 break;
	case push_gtlong:	ToP--;			gtTYPEf(lval);		ip++;	 break;
	case push_gtulong:	ToP--;			gtTYPEf(ulval);		ip++;	 break;
	case push_gtllong:	ToP--;			gtTYPEf(llval);		ip++;	 break;
	case push_gtdouble:	ToP--;		gtTYPEf(dval);		ip++;	 break;
	case push_gtptr:	ToP--;			gtTYPEf(p.p);		ip++;	 break;


	/*push_ge...*/
	case push_geint:	ToP--;			geTYPEf(ival);		ip++;	 break;
	case push_geuint:	ToP--;			geTYPEf(uival); 	ip++;	 break;
	case push_gelong:	ToP--;			geTYPEf(lval);		ip++;	 break;
	case push_geulong:	ToP--;			geTYPEf(ulval);		ip++;	 break;
	case push_gellong:	ToP--;			geTYPEf(llval);		ip++;	 break;
	case push_gedouble:	ToP--;		geTYPEf(dval);		ip++;	 break;
	case push_geptr:	ToP--;			geTYPEf(p.p);		ip++;	 break;


	/*push_lor...*/
	case push_lorint:	ToP--;			lorTYPEf(ival);		ip++;	 break;
	case push_loruint:	ToP--;			lorTYPEf(uival);	ip++;	 break;
	case push_lorlong:	ToP--;			lorTYPEf(lval);		ip++;	 break;
	case push_lorulong:	ToP--;		lorTYPEf(ulval);	ip++;	 break;
	case push_lorllong:	ToP--;		lorTYPEf(llval);	ip++;	 break;
	case push_lordouble:	ToP--;		lorTYPEf(dval);		ip++;	 break;
	case push_lorptr:	ToP--;			lorTYPEf(p.p);		ip++;	 break;


	/*push_bor...*/
	case push_borint:	ToP--;			borTYPEf(ival);		ip++;	 break;
	case push_boruint:	ToP--;			borTYPEf(uival);	ip++;	 break;
	case push_borlong:	ToP--;			borTYPEf(lval);		ip++;	 break;
	case push_borulong:	ToP--;		borTYPEf(ulval);	ip++;	 break;
	case push_borllong:	ToP--;		borTYPEf(llval);	ip++;	 break;


	/*push_xor...*/
	case push_xorint:	ToP--;			xorTYPEf(ival);		ip++;	 break;
	case push_xoruint:	ToP--;			xorTYPEf(uival);	ip++;	 break;
	case push_xorlong:	ToP--;			xorTYPEf(lval);		ip++;	 break;
	case push_xorulong:	ToP--;		xorTYPEf(ulval);	ip++;	 break;
	case push_xorllong:	ToP--;		xorTYPEf(llval);	ip++;	 break;


	/*push_and...*/
	case push_andint:	ToP--;			andTYPEf(ival);		ip++;	 break;
	case push_anduint:	ToP--;			andTYPEf(uival);	ip++;	 break;
	case push_andlong:	ToP--;			andTYPEf(lval);		ip++;	 break;
	case push_andulong:	ToP--;		andTYPEf(ulval);	ip++;	 break;
	case push_andllong:	ToP--;		andTYPEf(llval);	ip++;	 break;


	/*push_and...*/
	case push_reffloat:	ToP--;		refTYPEf(float, dval);		ip++;	 break;
	case push_refshort:	ToP--;		refTYPEf(short, ival);		ip++;	 break;
	case push_refchar:	ToP--;			refTYPEf(char, ival);		ip++;	 break;
	case push_refint:	ToP--;			refTYPEf(int, ival);			ip++;	 break;
	case push_refuint:	ToP--;			refTYPEf(unsigned, uival);	ip++;	 break;
	case push_reflong:	ToP--;			refTYPEf(long, lval);		ip++;	 break;
	case push_refulong:	ToP--;		refTYPEf(unsigned long, ulval);	ip++;	 break;
	case push_refllong:	ToP--;		refTYPEf(eic_llong, llval);		ip++;	 break;
	case push_refdouble:	ToP--;		refTYPEf(double, dval);		ip++;	 break;
	case push_refptr:	ToP--;			refTYPEf(ptr_t, p);			ip++;	 break;
	case push_refuptr:	ToP--;			refTYPEf(void *, p.p);		ip++;	 break;

	/*push_ptr...*/
	case push_addptr2int:	ToP--;		addPTR2INTf;		ip++;	 break;
	case push_addint2ptr:	ToP--;		addINT2PTRf;		ip++;	 break;
	case push_subptrint:	ToP--;		subPTRINTf;			ip++;	 break;
	case push_subptr:	ToP--;			subPTRf;			ip++;	 break;
	
	/*two instructions*/
	case lelong_jmpFint:
		{
			//Execute lelong
			leTYPE(lval);

			//Increment ip
			ip++;		

			//Execute jmpFint
			jmpfTYPE(ival);						
		}
		break;

	case rvaluptr_dreflong:
		{
			//Execute rvaluptr			
			rvalTYPE(p); //STK[ToP].v.p = AR[ip->ext][ip->val.ival].v.p
			STK[ToP].v.p.sp = 0;
			STK[ToP].v.p.ep = (void*) ULONG_MAX; /* set to a very high value */

			//Execute dreflong
			/*if(checkPtr(STK[ToP].v.p,"READ", sizeof(long) ))
			{
				STK[ToP].v.lval = *(long*)STK[ToP].v.p.p;
			}
			else 
			{
				goto codeCrash;
			}*/
			drefTYPE(long, lval);
			

			//Ignore next instruction
			ip++;
		}
		break;

	case bump_pushlong:
		{
			//Execute bump
			AdjustTop(ip->val1);
			ToP += ip->val1;
			

			//Execute pushlong
			pushTYPE(lval);
			

			//Ignore next instruction
			ip++;
		}
		break;

	case bump_pushint:
		{
			//Execute bump
			AdjustTop(ip->val1);
			ToP += ip->val1;
			

			//Execute pushint
			pushTYPE(ival);
			

			//Ignore next instruction
			ip++;
		}
		break;

	case drefdouble_stoval:
		{
			//Execute drefdouble
			drefTYPE(double, dval);
			
			//Ignore next instruction
			ip++;

			//Execute stoval
			
			switch(ip->ext) {
			case t_char:
			case t_uchar: STK[ToP].v.cval = STK[ToP].v.ival; break;
			case t_short:
			case t_ushort: STK[ToP].v.sval = STK[ToP].v.ival; break;
			case t_float:  STK[ToP].v.fval = STK[ToP].v.dval;
			/*	      case t_struct:
			case t_union:
			printf("stoVa1l with struct/union\n");
				*/
			}
			
			stoVAL;
		}
		break;

	case rvaluptr_bump:
		{
			//Execute rvaluptr
			rvalTYPE(p);
			STK[ToP].v.p.sp = 0;
			STK[ToP].v.p.ep = (void*) ULONG_MAX; // set to a very high value 

			//Execute bump
			AdjustTop(ip->val1);
			ToP += ip->val1;

			//Ignore next instruction
			ip++;
		}
		break;

	case rvalint_bump:
		{
			//Execute rvalint
			rvalTYPE(ival);
			

			//Execute bump
			AdjustTop(ip->val1);
			ToP += ip->val1;

			//Ignore next instruction
			ip++;
		}
		break;

	default: //maks
		EiC_formatMessage("Compilation error (%ld). Please, contact support@game-editor.com\n", ip->opcode);
		goto codeCrash;
		break;
	    
	}

	ip++;

	/////////////////////////////////////////////////////////////////////////////////
	
    }



    /*if(EiC_traceON) //maks: performance
		EiC_messageDisplay("\n");*/
    
    /*if(EiC_interActive)
		EiC_showvalue(&STK[ToP]);*/
    


codeEnd:
codeCrash:

#ifdef _DEBUG
	if(fp) fclose(fp);
#endif

    //xfree(STK);
	stackCount--;

	if(stackCount <= 0)
	{
		currentIp = NULL;
		env->lsp = 0; //Solve the crash in 'msrp boss.ged' when kills the boss (memory leak)
	}
}




   
    







