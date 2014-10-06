/* symbol.c
 *
 *	(C) Copyright Apr 15 1995, Edmond J. Breen.
 *		   ALL RIGHTS RESERVED.
 * This code may be copied for personal, non-profit use only.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typemod.h"
#include "MachSet.h"
#include  "global.h"
#include "lexer.h"
#include "xalloc.h"
#include "preproc.h"
#include "symbol.h"
#include "parser.h"

#include "../../dlmalloc.h" //maks
#include "../../../kyra/gui/colorscheme.h"

extern keyword_t cwords[]; //maks
extern keyword_t pwords[]; //maks
extern int s_cwords;
extern int s_pwords;

int EiC_isReservedWord(const char *id) //maks
{
	if(EiC_iskeyword(pwords, id, s_pwords) != 0)
		return 1;

    if(EiC_iskeyword(cwords, id, s_cwords) != 0)
		return 1;	

    return 0;
}

void getTokenColor(const char *str, int lastToken, int *currentToken, int *r, int *g, int *b)
{
  ColorScheme* color_scheme = get_color_scheme();

  if(lastToken == '.' && *currentToken == FUNCTION)
  {
    //Solve the color error in 'min' member of Time struct
    *currentToken = ID;
  }

  if(*currentToken >= autosym && *currentToken <= whilesym)
  {
    //Keyword
    *r = color_scheme->keyword_r;
    *g = color_scheme->keyword_g;
    *b = color_scheme->keyword_b;
  }
  else if((*currentToken >= INC && *currentToken <= RSHT) ||
	  (*currentToken >= RELOP && *currentToken <= XOREQ) ||
	  *currentToken == '+' ||
	  *currentToken == '-' ||
	  *currentToken == '*' ||
	  *currentToken == '/' ||
	  *currentToken == '%')
  {
    //Operators
    *r = color_scheme->operator_r;
    *g = color_scheme->operator_g;
    *b = color_scheme->operator_b;
  }
  else
  {
    switch(*currentToken)
    {
      case ID:
      {
	if(EiC_isReservedWord(str))
	{
	  //Is a preprocessor keyword
	  *r = color_scheme->preprocess_r;
	  *g = color_scheme->preprocess_g;
	  *b = color_scheme->preprocess_b;
	}
	else
	{
	  // normal letters
	  *r = color_scheme->normal_r;
	  *g = color_scheme->normal_g;
	  *b = color_scheme->normal_b;
	}					
      }
      break;

      case TYPENAME:
	*r = color_scheme->type_r;
	*g = color_scheme->type_g;
	*b = color_scheme->type_b;
	break;

      case FUNCTION:
	*r = color_scheme->function_r;
	*g = color_scheme->function_g;
	*b = color_scheme->function_b;
	break;

      case TOKEN_COMMENT:
	*r = color_scheme->comment_r;
	*g = color_scheme->comment_g;
	*b = color_scheme->comment_b;
	break;


      case STR:
      case TOKEN_INT:
      case TOKEN_UINT:
      case TOKEN_LONG:
      case TOKEN_ULONG:
      case TOKEN_FLOAT:
      case TOKEN_DOUBLE:
	//Literal
	*r = color_scheme->literal_r;
	*g = color_scheme->literal_g;
	*b = color_scheme->literal_b;				
	break;

      default:
	*r = color_scheme->default_r;
	*g = color_scheme->default_g;
	*b = color_scheme->default_b;				
	break;
    }
  }
}

int EiC_iskeyword(keyword_t *keywords, const char *id,int n) //maks
{
    int i;
    for(i=0;i<n;i++)
	if(strcmp(keywords[i].id,id) == 0)
	    return keywords[i].token;
    return 0;
}

#define MoD 2
void EiC_eicpush(eicstack_t *s, val_t v)
{
    if(!(s->n%MoD)) {
	if(!s->n)
	    s->val = (val_t*)xcalloc(sizeof(val_t),MoD);
	else
	    s->val = (val_t*)xrealloc(s->val,(s->n+MoD)*sizeof(val_t));
    }
    s->val[s->n] = v;
    s->n++;
}

int EiC_eicpop(eicstack_t *s, val_t *pop)
{
    if(s->n == 0)
		return 0;

    s->n--;
    *pop = s->val[s->n];

    if(!(s->n%MoD)) 
	{		
		if(!s->n)
		{
			xfree(s->val);
			s->val = NULL; //maks
		}
		else
			s->val = (val_t*)xrealloc(s->val,s->n*sizeof(val_t));
    }
    return 1;
}


/* LOOK UP TABLE ROUTINES
   --------------------------*/

static size_t _EnTrY_No = 0;
int hashsmc(char * s,int mod);
symentry_t *EiC_HTAB[HSIZE];


/* code for generating tempories */
unsigned int NumTemps=0, CurTemp =0;

symentry_t * EiC_nxtTemp(int obj, int level)
{
    symentry_t *sym;
    char tmpName[50];
    sprintf(tmpName,"%s%d","__TeMp",NumTemps);

    /* There should be no need to watch out for change of level !!!!
     * It will be assumed that the compound statement routine will
     * handle it.
     */


    sym = EiC_insertLUT(EiC_work_tab,tmpName,obj);
    NumTemps++;

    CurTemp++;

    if(sym->val.ival == -1) /* needs a home */ 
	EiC_stackit(sym,level);

    /* Setting up the token information is left
     * to the caller of this routine.
     */

    sym->level = level;
    return sym;
}    



size_t EiC_lut_NextEntryNum(void)
{
    return _EnTrY_No;
}
                             /*CUT EiChashFunc*/
int hashsmc(char * s,int mod)
{
    register unsigned int h, c;
    h = 0;
    while(  (c = (int) *s++) > 0)
	h =   (h << 1) + c;
    return ( h % mod);
}
                           /*END CUT*/

                            /*CUT lutLookUp*/
int Pclash;

char *EiC_getClashedfname(char nspace,char *id)
{
    symentry_t *sym;
    for(sym = EiC_HTAB[hashsmc(id,HSIZE)]; sym != NULL; sym = sym->next)
	if(sym->nspace == nspace && strcmp(id,sym->id) == 0)
	    if((sym->sclass & c_private) &&
	       sym->fname != CurrentFileName()) {
		return sym->fname;
	    }
    return NULL;
}

symentry_t * EiC_lookup(char nspace, char *id)
{
    symentry_t *sym;
    Pclash = 0;
    for(sym = EiC_HTAB[hashsmc(id,HSIZE)]; sym != NULL; sym = sym->next)
	if(sym->nspace == nspace && strcmp(id,sym->id) == 0) {
	    if((sym->sclass & c_private) &&
	       sym->fname != CurrentFileName()) {
		Pclash = 1;
		continue;
	    }
	    else
		break;
	}

    return(sym);
}
                             /*END CUT*/


void EiC_parseStringSingle(char *); //maks
void EiCp_Reset(int flag); //maks
void EiCp_initiateReset(void); //maks
extern code_t firstInst; //maks

static void (*EiC_VisibleVariable)(char *, int) = NULL; //maks
static int EiC_VisibleVariableType = 0;
static int EiC_VisibleVariableGetPointer = 0;
static int EiC_VisibleVariableIncludeConst = 0;
static type_expr *EiC_VisibleVariableTestType = NULL;

void GetVisibleVars(char *code, int type, int bGetPointer, int bIncludeConst, void (*VisibleVariableCallBack)(char *, int), type_expr *pRequestedType) //maks
{
	if(!code) return;

	EiC_VisibleVariableType = type;
	EiC_VisibleVariableGetPointer = bGetPointer;
	EiC_VisibleVariableIncludeConst = bIncludeConst;
	EiC_VisibleVariable = VisibleVariableCallBack;
	EiC_VisibleVariableTestType = pRequestedType;

	//Check point
	EiCp_initiateReset();

	EiC_parseStringSingle(code);

	//Remove added symblos
	EiCp_Reset(1);
	firstInst.inst = NULL; //maks: the code is not valid anymore

	EiC_VisibleVariable = NULL;
}

static void VisibleVars() //maks
{
	symentry_t *sym;
	int i;

	if(!EiC_VisibleVariable) return;
    
    for (i = 0; i < HSIZE; i++)
	{
		for(sym = EiC_HTAB[i]; sym; sym = sym->next) 
		{
			type_expr *pType = sym->type;
			int bType = 0, bPointer = 0, bConst = 0, bFunction = 0, bStruct = 0, bTypedef = 0;			

			if(sym->sclass == c_typedef)
			{
				bTypedef = 1;
			}

			while(pType)
			{
				int t = EiC_gettype(pType);
				
				if((isconst(pType) || isconstp(pType))) 
				{
					bConst = 1;
				}

				if(t == t_pointer)
				{
					bPointer = 1;
				}
				
				if(!EiC_VisibleVariableTestType)
				{
					if(EiC_VisibleVariableType != -1)
					{
						if(t == EiC_VisibleVariableType)
						{
							bType = 1;
						}
					}
					else
					{
						bType = 1;
					}
				}
					

				if(t == t_func || t == t_funcdec || t == t_builtin)
				{
					bFunction = 1;
				}	

				if(t == t_struct || t == t_union)
				{
					bStruct = 1;
				}
				

				pType = nextType(pType);
			}

			

			if( (
				(bType && !bStruct) || 
				(EiC_VisibleVariableTestType != sym->type && EiC_sametypes(EiC_VisibleVariableTestType, sym->type))
				) &&

				!bFunction && !bTypedef &&
				((!EiC_VisibleVariableGetPointer && !bPointer) || (EiC_VisibleVariableGetPointer && bPointer)) && //Pointer Ok
				(!bConst || EiC_VisibleVariableIncludeConst) //Const Ok
				)
			{					
				EiC_VisibleVariable(sym->id, bPointer);
			}
		}
	}
}

                           /*CUT lutInsert*/
symentry_t * EiC_insertLUT(char nspace,char *id,int type)
{
    symentry_t *sym;
    auto int  hashval;

    sym = (symentry_t *) xcalloc(1,sizeof(symentry_t));
    if(sym == NULL)
	return(NULL);
    if( (sym->id = EiC_strsave(id)) == NULL) 
	{
		xfree(sym);
		sym = NULL; //maks

		return(NULL);
    }
    sym->entry = _EnTrY_No++;
    hashval = hashsmc(sym->id,HSIZE);
    sym->next = EiC_HTAB[hashval];
    EiC_HTAB[hashval] = sym;
    sym->nspace = nspace;
    sym->val.ival = -1;		/* indicates  unused */
    sym->type = EiC_addtype(type,NULL);
    sym->fname =  CurrentFileName();

	if(EiC_VisibleVariable && strcmp(sym->id, "ged_ProbeValidVars") == 0) //maks
	{
		VisibleVars();
	}

    return(sym);
}
                           /*END CUT*/



void delete(symentry_t *sym)
{
    auto symentry_t * this;
    auto int idx;
    
    idx = hashsmc(sym->id,HSIZE);
    this = EiC_HTAB[idx];
    if(this == sym)
		EiC_HTAB[idx] = sym->next;
    else 
	{			/* find and unlink  */
		while(this && this->next != sym)
			this = this->next;

		if(this) this->next = sym->next; //maks
    }
}

void EiC_UpdateSymPos(symentry_t * sym)
{

    if(sym->next && sym->next->level > sym->level) {
	symentry_t * p = EiC_HTAB[hashsmc(sym->id,HSIZE)];
	delete(sym);
	while(p->next != sym)
	    if(p->next) {
		if(p->next->level > sym->level)
		    p=p->next;
		else {
		    sym->next = p->next;
		    p->next = sym;
		}
	    } else {
		p->next = sym;
		sym->next = NULL;
	    }
    }
}

void EiC_remsym(symentry_t *sym)
{

  delete(sym);
  free_sym(sym);

}
/*END CUT*/

#if 1
void remBuiltin(symentry_t *sym)
{
    /* removes the prototype of the builtin function only */
    EiC_freetype(sym->type);
    sym->type = EiC_addtype(t_builtin,NULL);
    sym->fname = "::script::"; //maks
}
#endif


/*CUT lutRemLevel*/
void EiC_remlevel(int level)
{
  int i;
  symentry_t * sym, *symh;

  for(i=0;i<HSIZE;i++) {
    sym = EiC_HTAB[i];
    while(sym && sym->level >= level) {
      symh = sym->next;
      free_sym(sym);
      sym = symh;
    }
    EiC_HTAB[i] = sym;
  }
}
/*END CUT*/
/*CUT lutRemTempories*/
void EiC_remTempories(void)
{
  int i;
  symentry_t * sym, *symh;

  for(i=0;i<HSIZE;i++) {
    sym = EiC_HTAB[i];
    while(sym && IsTemp(sym->type)) {
      symh = sym->next;
      free_sym(sym);
      sym = symh;
    }
    EiC_HTAB[i] = sym;
  }
}
/*END CUT*/



int  EiC_lutClearFileEntries(char *FileName)
{

    int i;
    symentry_t * sym, *p, *t;

    for(i=0;i<HSIZE;i++) {
	t = sym = EiC_HTAB[i];
	p = NULL;
	while(sym){
	    if(strcmp(sym->fname,FileName) == 0) {
		if(p)
		    p->next = sym->next;
		else
		    t = sym->next;
		free_sym(sym);
		if(!p) {
		    sym = t;
		    continue;
		}
	    } else
		p = sym;
	    sym=p->next;
	}
	EiC_HTAB[i] = t;
    }
    return 1;

}
                    /*END CUT*/
                     /*CUT lutCleanUp*/

void EiC_lut_CleanUp(size_t bot)
{
    int i;
    symentry_t * sym, *p, *t;
	
    for(i=0;i<HSIZE;i++) 
	{
		t = sym = EiC_HTAB[i];
		p = NULL;
		
		while(sym)
		{
			if(sym->entry >= bot) 
			{
				if(EiC_gettype(sym->type) == t_builtin) 
				{
					remBuiltin(sym);
					p = sym;
				} 
				else 
				{
					if(p)
						p->next = sym->next;
					else
						t = sym->next;
					
					free_sym(sym);
					if(!p) 
					{
						sym = t;
						continue;
					}
				}
			} 
			else
			{
				p = sym;
			}
			
			sym=p->next;
		}
		
		EiC_HTAB[i] = t;
    }

}
                     /*END CUT*/
                     /*CUT lutFreeSym*/

//maks
#define freeAg(X)   do {\
			    symentry_t *x = X; \
			    if(!isconst(x->type)) \
					{if(EiC_ENV->AR[x->val.ival].v.p.p) {xfree(EiC_ENV->AR[x->val.ival].v.p.p);EiC_ENV->AR[x->val.ival].v.p.p = NULL;}}\
					else {if(x->val.p.p) {xfree(x->val.p.p); x->val.p.p = NULL;}}\
		    } while (0)


#define freeAg1(X)  if(EiC_ENV->AR[X->val.ival].v.p.p) {xfree(EiC_ENV->AR[X->val.ival].v.p.p); EiC_ENV->AR[X->val.ival].v.p.p = NULL;} //maks


static void free_sym(symentry_t *sym)
{

#if 0    
      printf("Freeing [%s] %d  %d [%d]\n",
  	sym->id, sym->entry, sym->val.ival,EiC_gettype(sym->type));
    
#endif    
    
    if(EiC_gettype(sym->type) == t_func) {
	EiC_killcode(sym);
    } else if(sym->level == 1)  { /* global value */
	int t;
	if((t=EiC_gettype(sym->type)) == t_array && sym->val.ival >=0) 
	    freeAg(sym);
	else if ((t== t_union || t == t_struct) && sym->val.ival >=0 ) 
	    freeAg(sym);
		
    }


    /*
     * the conditions for pushing  onto the ARgar stack
     * must be the same as those for stacking as found in
     * function establish_id
     * Except for ParseError
     */
    if( sym->val.ival >=0
       && sym->level == 1
       && EiC_gettype(sym->type) != t_builtin ) {
	if(! isconst(sym->type)
	   && sym->nspace == stand_tab
	   && sym->sclass != c_typedef) {
	    EiC_eicpush(&EiC_ENV->ARgar,sym->val);
	}
    }
    EiC_freetype(sym->type);
    xfree(sym->id); sym->id = NULL; //maks
    xfree(sym); sym = NULL; //maks
}

                        /*END CUT*/

void EiC_UpdateEntry(symentry_t *sym)
{
    int t = EiC_gettype(sym->type);
    if(CurrentFileName() != sym->fname &&
       t != t_func &&
       t != t_funcdec &&
       t != t_builtin)
	return;
    sym->entry =  _EnTrY_No++;
    sym->pname = sym->fname;
    sym->fname = CurrentFileName();
}

void EiC_addoffsettolevel(char nspace,int level,int off)
{
    int i;
    symentry_t * sym;

    for(i=0;i<HSIZE;i++) {
	sym = EiC_HTAB[i];
	while(sym && sym->level == level && sym->nspace == nspace) {
	    sym->val.ival = -(sym->val.ival + off);
	    sym = sym->next;
	}
    }
}

void EiC_marktype(type_expr *type, char mark)
{
    int i;
    struct_t *s;
    void EiC_markFunc(type_expr *t,int mark);
    
    while(type) {
	xmark(type,mark);
	switch(EiC_gettype(type)) {
	  case t_builtin:
	    if(EiC_getInf(type) == NULL)
		break;
	  case t_funcdec:
	  case t_func:
	    EiC_markFunc(type,mark);
	    break;
	  case t_union:
	  case t_struct:
	    if(type->alias)
		break;
	    s = EiC_getInf(type);
	    /*
	     * REM must allow for incomplete
	     * types.
	     */
	    if(!s)
		break;
	    xmark(s,mark);
	    if(!s->cl)
		break;
	    xmark(s->offset,mark);
	    xmark(s->id,mark);
	    xmark(s->type,mark);
	    for(i=0;i<s->n;i++) {
		xmark(s->id[i],mark);
		EiC_marktype(s->type[i],mark);
	    }
	    if(s->ntags) {
		xmark(s->tag,mark);
		for(i=0;i<s->ntags;++i)
		    EiC_marktype(s->tag[i],mark);
	    }
	    break;
	}
	type = nextType(type);
    }
}

static void marklabel(Label_t *l,char mark) 
{
    while(l) {
	xmark(l,mark);
	xmark(l->name,mark);
	l = l->nxt;
    }
}

static void markcode(symentry_t *sym,char mark)
{
    int i;
    InsT_t *inst;
    code_t * code;
    code = EiC_ENV->AR[sym->val.ival].v.p.p;
    xmark(code,mark);
    xmark(code->inst,mark);
    marklabel(code->labels,mark);
    marklabel(code->gotos,mark);

    inst = code->inst;
    for(i=0;i<code->nextinst;i++,inst++)
	if(inst->opcode == jmptab) {
	    eicstack_t * s;
	    s = inst->val.p.p;
	    xmark(s->val,mark);
	    xmark(s,mark);
	} else if(inst->opcode == assigntype)
	    EiC_marktype(inst->val.p.p,mark);
}

#define GARBAGE 0

#if GARBAGE != 0

void EiC_markarray(type_expr *type,ptr_t *p, char mark);
void EiC_markstruct(type_expr *type,ptr_t *p, char mark);

void EiC_markpointer(type_expr *type,ptr_t *p, char mark)
{
    if(issafe(type) && p && p->p) {
	switch(EiC_gettype(nextType(type))) {
	case t_array: EiC_markarray(type,p,mark); return;
	case t_pointer:
	    {
		size_t i,t;
		t = ((char*)p->ep - (char*)p->sp)/sizeof(ptr_t);
		for(i=0;i<t;++i)
		    EiC_markpointer(nextType(type),
				    &((ptr_t*)p->p)[i],mark);
	    }
	    break;

	    /*
	case t_struct:
	case t_union:
	EiC_markstruct(
	    */
	}
	xmark(p->sp,mark);
	
    }
}

void EiC_markarray(type_expr *type,ptr_t *p, char mark)
{
    if(p) {
	int t;
	type_expr *h;
	h = type->nxt;
	while(h->nxt) {
	    if(EiC_gettype(h) == t_pointer && issafe(h)) {
		size_t i,sz = EiC_get_sizeof(type)/sizeof(ptr_t);
		ptr_t *pt = (ptr_t*)p->sp;
		for(i=0;i<sz;++i,++pt)
		    EiC_markpointer(h,pt,mark);

		break;
	    }
	    h = h->nxt;
	}
	if((t = EiC_gettype(h)) == t_struct || t == t_union) {
	    int inc = EiC_get_sizeof(h);
	    size_t i,sz = EiC_get_sizeof(type)/inc;
	    char *pt = p->sp;
	    for(i=0;i<sz;++i,pt += inc)
		EiC_markstruct(h,(ptr_t*)pt,mark);
	}
	xmark(p->sp,mark);
    }
}
	
void EiC_markstruct(type_expr *type,ptr_t *p, char mark)
{
    if(p->p) {
	int i;
	struct_t *S = EiC_getInf(type);
	char *data = p->p;
	for (i = 0; i < S->n; i++) {
	    switch(EiC_gettype(S->type[i])) {
	    case t_pointer:
		EiC_markpointer(S->type[i],
				(ptr_t*)&data[S->offset[i]],
				mark);
		break;
	    case t_array:
		{
		    ptr_t v;
		    v.p = v.sp = &data[S->offset[i]];
		    v.ep = (char*)v.sp + EiC_get_sizeof(S->type[i]);
		    EiC_markarray(S->type[i],
				  &v,
				  mark);
		}
		break;
	    }
	}
	xmark(p->p,mark);
    }
}

#endif

void EiC_marksyms(char mark)
{
    void EiC_markmacros(char);
    void EiC_markENV(char);
    
    int i;
    symentry_t * sym;

    EiC_markmacros(mark);
    
    if(EiC_ENV->AR)
	xmark(EiC_ENV->AR,mark);
    if(EiC_ENV->ARgar.n) 
	xmark(EiC_ENV->ARgar.val,mark);
    if(EiC_ENV->LAR)
	xmark(EiC_ENV->LAR,mark);
    if(EiC_ENV->CODE.nextinst)
	xmark(EiC_ENV->CODE.inst,mark);
    xmark(EiC_ENV,mark);

    EiC_markENV(mark);
    
    for(i=0;i<HSIZE;i++) {
	sym = EiC_HTAB[i];
	while(sym) {
	    /*printf("marking %s\n",sym->id);*/


	    xmark(sym,mark);
	    xmark(sym->id,mark);	    
	    EiC_marktype(sym->type,mark);	    
	    if(sym->nspace != tag_tab)


#if GARBAGE == 0

                switch(EiC_gettype(sym->type)) {
                  case t_func: markcode(sym,mark); break;
                  case t_array:
                  case t_union:
                  case t_struct:
                    if(isconst(sym->type))
                       xmark(sym->val.p.p,mark);
                    else
                        if(sym->sclass != c_typedef && sym->val.ival >= 0)
                            xmark(EiC_ENV->AR[sym->val.ival].v.p.p,mark);
                    break;
                }

#else		    
		switch(EiC_gettype(sym->type)) {
		case t_func: markcode(sym,mark); break;
		case t_array:

		    if(isconst(sym->type))
			EiC_markarray(sym->type,&sym->val.p,mark);
		    else
			EiC_markarray(sym->type,
				      &EiC_ENV->AR[sym->val.ival].v.p,
				      mark);
		    break;

		case t_union:
		case t_struct:
		    
		    if(isconst(sym->type)) 
			EiC_markstruct(sym->type,&sym->val.p, mark);
		    /*xmark(sym->val.p.p,mark);*/
		    else
			if(sym->sclass != c_typedef
			   && sym->val.ival>= 0)
			    EiC_markstruct(sym->type,
					   &EiC_ENV->AR[sym->val.ival].v.p,
					   mark);
		    /*xmark(EiC_ENV->AR[sym->val.ival].v.p.p,mark);*/
		    break;
		case t_pointer:
		    EiC_markpointer(sym->type,
				    &EiC_ENV->AR[sym->val.ival].v.p,
				    mark);
		    /******
			   if(issafe(sym->type) &&
			   EiC_ENV->AR[sym->val.ival].v.p.sp) {
			   xmark(EiC_ENV->AR[sym->val.ival].v.p.sp,mark);
			   }
		    *******/
		    
		}
#endif		
	    sym = sym->next;
	}
    }
}






char * EiC_strsave(char *s)
{
    char *p;
    int n;

    for(n = 0,p =s; *p != '\0';++p,++n)
	;
    n++;
    p = xcalloc(n,sizeof(char));

    if(p)  //maks
	{
		while((*p++ = *s++));
		
		return p - n;
	}

	return NULL; //maks
}

void EiC_newsymtype(symentry_t *sym, type_expr *t)
{
    if(sym) {
	if(sym->type && sym->type != t)
	    EiC_freetype(sym->type);
	sym->type = t;
    }
}

int nextstackitem(int level)
{
    if(level == 1) {		/* static variables */
	val_t v;
	if(!EiC_eicpop(&EiC_ENV->ARgar,&v))  { /* check for spare slots */
	    if(EiC_ENV->sp == EiC_ENV->ARsize) {
		if(!EiC_ENV->ARsize)
		    EiC_ENV->AR = (AR_t*)xcalloc(sizeof(AR_t),1);
		else
		    EiC_ENV->AR = (AR_t*)xrealloc(EiC_ENV->AR,
					      (EiC_ENV->sp+1)*sizeof(AR_t));
		EiC_ENV->ARsize++;
	    }
	    v.ival = EiC_ENV->sp;
	    EiC_ENV->sp++;
	}
	return v.ival;
    } else {			/* automatic variables */
	if(EiC_ENV->lsp == EiC_ENV->LARsize) {
	    if(!EiC_ENV->LARsize)
		EiC_ENV->LAR = (AR_t*)xcalloc(sizeof(AR_t),1);
	    else
		EiC_ENV->LAR = (AR_t*)xrealloc(EiC_ENV->LAR,(EiC_ENV->lsp+1)*
					   sizeof(AR_t));
	    EiC_ENV->LARsize++;
	}
	EiC_ENV->lsp++;
	return EiC_ENV->lsp - 1;
    }
}

int EiC_stackit(symentry_t * sym,int level)
{
    int i;
    AR_t * ar;
    
    i = nextstackitem(level);
    if(level == 1)		/* static variables */
	ar = EiC_ENV->AR;
    else 			/* local variable */	
	ar = EiC_ENV->LAR;

    sym->val.ival = i;

    ar[i].v.dval = 0; /* NULL it */
    ar[i].type = sym->type;
    return i;
}


/*------------------------------------------------*/
void EiC_inittoken(token_t * e1)
{
    e1->Pflag = 0;
    e1->Code.binst = e1->Code.nextinst = 0;
	e1->Code.inst = NULL; //maks
    e1->Code.labels = e1->Code.gotos = NULL;
    e1->Typequal = e1->Sclass = 0;
    e1->Sym = NULL;
    e1->Val.sym = NULL;
    e1->Type = NULL;
}
void EiC_freetoken(token_t * e1)
{
    EiC_freetype(e1->Type);
    e1->Type = NULL;
}
void initcode(code_t * code)
{
    code->binst = code->nextinst = 0;
}


void EiC_cleancode(code_t * code)
{
    unsigned int i;
    InsT_t *inst;

    if(!code)
	return;
    inst = code->inst;
    /* printf("Next instr: %d\n", code->nextinst); */
    /* rem free up other info also */
    for(i=0;i<code->nextinst;i++,inst++)
	if(inst->opcode == jmptab) 
	{
	    eicstack_t *s;
	    s = inst->val.p.p;
	    xfree(s->val); s->val = NULL; //maks
	    xfree(s); s = NULL; //maks
		inst->val.p.p = NULL; //maks
	} 
	else if(inst->opcode == assigntype)
	    EiC_freetype(inst->val.p.p);

}    

static void freeCodeLabels(code_t *code)
{
    void EiCp_freeLabels(Label_t *lab);
    if(code->labels) {
	EiCp_freeLabels(code->labels);
	code->labels = NULL;
    }
    if(code->gotos) {
	EiCp_freeLabels(code->gotos);
	code->gotos = NULL;
    }
}

void EiC_killcode(symentry_t *sym) //maks
{
    code_t **code;

    code = &(EiC_ENV->AR[sym->val.ival].v.p.p);
    if(!code || !*code) return;

    EiC_cleancode(*code);
    freeCodeLabels(*code);
    xfree((*code)->inst); 
    xfree(*code); 

	*code = NULL;
}

extern code_t firstInst; //maks: direct execution
extern int storeFirstInst;

void EiC_freecode(code_t * code)
{

    if(code && code->binst > 0) 
	{
		if(code->inst == firstInst.inst)
		{
			firstInst.inst = NULL;
		}

		xfree(code->inst); code->inst = NULL; //maks
		code->nextinst = code->binst = 0;
		freeCodeLabels(code);
    }
}

#define ModSize 5



int isPush(int opcode)
{
	switch(opcode)
	{
	case pushdouble:	
	case pushint:
	case pushuint:
	case pushlong:
	case pushulong:
	case pushllong:
	case pushptr:
	case pushval:
		return 1;

	default:
		return 0;
	}

	return 0;
}

void CrateBigInstruction(InsT_t * inst, int iCurrent, int priority) //maks
{
	/*
	Group some instructions (super operators) to spped up interpretation

	Results with 1945.ged main screen (30s execution) 
		- 370 cycles in EiC_interpret wihout big instructions
		- 291 cycles in EiC_interpret with big instructions
	*/

	//If orders by instruction group frequency
	
	if(priority == 3 && iCurrent >= 2)
	{
		//Three instructions group
		//Second end third instruction must be ignorated on interpretation

		if(inst[iCurrent-2].opcode == bump)
		{
			//bump...

			if(isPush(inst[iCurrent-1].opcode))
			{
				//push...
				int opcode = 0;

				switch(inst[iCurrent].opcode)
				{

					//add...
					case addint:	opcode = bump_push_addint;		break;
					case adduint:	opcode = bump_push_adduint;		break;			
					case addlong:	opcode = bump_push_addlong;		break;
					case addulong:	opcode = bump_push_addulong;	break;	
					case addllong:	opcode = bump_push_addllong;	break;	
					case adddouble:	opcode = bump_push_adddouble;	break;						

					//sub...
					case subint:	opcode = bump_push_subint;		break;
					case subuint:	opcode = bump_push_subuint;		break;			
					case sublong:	opcode = bump_push_sublong;		break;
					case subulong:	opcode = bump_push_subulong;	break;	
					case subllong:	opcode = bump_push_subllong;	break;	
					case subdouble:	opcode = bump_push_subdouble;	break;

					//div...
					case divint:	opcode = bump_push_divint;		break;
					case divuint:	opcode = bump_push_divuint;		break;			
					case divlong:	opcode = bump_push_divlong;		break;
					case divulong:	opcode = bump_push_divulong;	break;	
					case divllong:	opcode = bump_push_divllong;	break;	
					case divdouble:	opcode = bump_push_divdouble;	break;

					//mult...
					case multint:	opcode = bump_push_multint;		break;
					case multuint:	opcode = bump_push_multuint;	break;			
					case multlong:	opcode = bump_push_multlong;	break;
					case multulong:	opcode = bump_push_multulong;	break;	
					case multllong:	opcode = bump_push_multllong;	break;	
					case multdouble:opcode = bump_push_multdouble;	break;

					//mod...
					case modint:	opcode = bump_push_modint;		break;
					case moduint:	opcode = bump_push_moduint;		break;			
					case modlong:	opcode = bump_push_modlong;		break;
					case modulong:	opcode = bump_push_modulong;	break;	
					case modllong:	opcode = bump_push_modllong;	break;	

					//lsht...
					case lshtint:	opcode = bump_push_lshtint;		break;
					case lshtuint:	opcode = bump_push_lshtuint;	break;			
					case lshtlong:	opcode = bump_push_lshtlong;	break;
					case lshtulong:	opcode = bump_push_lshtulong;	break;	
					case lshtllong:	opcode = bump_push_lshtllong;	break;

					//rsht...
					case rshtint:	opcode = bump_push_rshtint;		break;
					case rshtuint:	opcode = bump_push_rshtuint;	break;			
					case rshtlong:	opcode = bump_push_rshtlong;	break;
					case rshtulong:	opcode = bump_push_rshtulong;	break;	
					case rshtllong:	opcode = bump_push_rshtllong;	break;

					//lt...
					case ltint:		opcode = bump_push_ltint;		break;
					case ltuint:	opcode = bump_push_ltuint;		break;			
					case ltlong:	opcode = bump_push_ltlong;		break;
					case ltulong:	opcode = bump_push_ltulong;		break;	
					case ltllong:	opcode = bump_push_ltllong;		break;	
					case ltdouble:	opcode = bump_push_ltdouble;	break;	
					case ltptr:		opcode = bump_push_ltptr;		break;
					
					//le...
					case leint:		opcode = bump_push_leint;		break;
					case leuint:	opcode = bump_push_leuint;		break;			
					case lelong:	opcode = bump_push_lelong;		break;
					case leulong:	opcode = bump_push_leulong;		break;	
					case lellong:	opcode = bump_push_lellong;		break;	
					case ledouble:	opcode = bump_push_ledouble;	break;	
					case leptr:		opcode = bump_push_leptr;		break;						

					//eq...
					case eqint:		opcode = bump_push_eqint;		break;
					case equint:	opcode = bump_push_equint;		break;			
					case eqlong:	opcode = bump_push_eqlong;		break;
					case equlong:	opcode = bump_push_equlong;		break;	
					case eqllong:	opcode = bump_push_eqllong;		break;	
					case eqdouble:	opcode = bump_push_eqdouble;	break;	
					case eqptr:		opcode = bump_push_eqptr;		break;

					//ne...
					case neint:		opcode = bump_push_neint;		break;
					case neuint:	opcode = bump_push_neuint;		break;			
					case nelong:	opcode = bump_push_nelong;		break;
					case neulong:	opcode = bump_push_neulong;		break;	
					case nellong:	opcode = bump_push_nellong;		break;	
					case nedouble:	opcode = bump_push_nedouble;	break;	
					case neptr:		opcode = bump_push_neptr;		break;

					//gt...
					case gtint:		opcode = bump_push_gtint;		break;
					case gtuint:	opcode = bump_push_gtuint;		break;			
					case gtlong:	opcode = bump_push_gtlong;		break;
					case gtulong:	opcode = bump_push_gtulong;		break;	
					case gtllong:	opcode = bump_push_gtllong;		break;	
					case gtdouble:	opcode = bump_push_gtdouble;	break;	
					case gtptr:		opcode = bump_push_gtptr;		break;

					//ge...
					case geint:		opcode = bump_push_geint;		break;
					case geuint:	opcode = bump_push_geuint;		break;			
					case gelong:	opcode = bump_push_gelong;		break;
					case geulong:	opcode = bump_push_geulong;		break;	
					case gellong:	opcode = bump_push_gellong;		break;	
					case gedouble:	opcode = bump_push_gedouble;	break;	
					case geptr:		opcode = bump_push_geptr;		break;

					//lor...
					case lorint:	opcode = bump_push_lorint;		break;
					case loruint:	opcode = bump_push_loruint;		break;			
					case lorlong:	opcode = bump_push_lorlong;		break;
					case lorulong:	opcode = bump_push_lorulong;	break;	
					case lorllong:	opcode = bump_push_lorllong;	break;	
					case lordouble:	opcode = bump_push_lordouble;	break;	
					case lorptr:	opcode = bump_push_lorptr;		break;

					//bor...
					case borint:	opcode = bump_push_borint;		break;
					case boruint:	opcode = bump_push_boruint;		break;			
					case borlong:	opcode = bump_push_borlong;		break;
					case borulong:	opcode = bump_push_borulong;	break;	
					case borllong:	opcode = bump_push_borllong;	break;

					//xor...
					case xorint:	opcode = bump_push_xorint;		break;
					case xoruint:	opcode = bump_push_xoruint;		break;			
					case xorlong:	opcode = bump_push_xorlong;		break;
					case xorulong:	opcode = bump_push_xorulong;	break;	
					case xorllong:	opcode = bump_push_xorllong;	break;

					//and...
					case andint:	opcode = bump_push_andint;		break;
					case anduint:	opcode = bump_push_anduint;		break;			
					case andlong:	opcode = bump_push_andlong;		break;
					case andulong:	opcode = bump_push_andulong;	break;	
					case andllong:	opcode = bump_push_andllong;	break;

					//ref...
					case reffloat:	opcode = bump_push_reffloat;	break;
					case refshort:	opcode = bump_push_refshort;	break;			
					case refchar:	opcode = bump_push_refchar;		break;
					case refint:	opcode = bump_push_refint;		break;	
					case refuint:	opcode = bump_push_refuint;		break;
					case reflong:	opcode = bump_push_reflong;		break;
					case refulong:	opcode = bump_push_refulong;	break;
					case refllong:	opcode = bump_push_refllong;	break;
					case refdouble:	opcode = bump_push_refdouble;	break;
					case refptr:	opcode = bump_push_refptr;		break;
					case refuptr:	opcode = bump_push_refuptr;		break;

					//ptr...
					case addptr2int:	opcode = bump_push_addptr2int;	break;
					case addint2ptr:	opcode = bump_push_addint2ptr;	break;			
					case subptrint:		opcode = bump_push_subptrint;	break;
					case subptr:		opcode = bump_push_subptr;		break;
				}


				if(opcode)
				{
					//Subst first instruction with new
					inst[iCurrent-2].opcode = opcode;
					inst[iCurrent-2].val = inst[iCurrent-1].val;
					
					//Invalidate next instructions
					inst[iCurrent-1].opcode = inst[iCurrent].opcode = 0;
				}
			}
		}
		/*else if(inst[iCurrent-2].opcode == pushlong && inst[iCurrent-1].opcode == lelong && inst[iCurrent].opcode == jmpFint)
		{
			//Create pushlong_lelong_jmpFint
			//pushlong uses:	ip->val.lval
			//lelong uses:		STK
			//jmpFint uses:		ip->val.ival
			//Don't reach here
			
			
			
			//Subst first instruction with new				
			inst[iCurrent-2].opcode = pushlong_lelong_jmpFint;			

			//Invalidate next instruction
			inst[iCurrent-1].opcode = inst[iCurrent].opcode = 0;
		}*/
		/*else if(inst[iCurrent-2].opcode == rvaluptr && inst[iCurrent-1].opcode == dreflong && inst[iCurrent].opcode == bump)
		{
			//Create rvaluptr_dreflong_bump
			//rvaluptr uses:	ip->val.ival
			//dreflong uses:	STK[ToP]
			//bump uses:		ip->val.ival
			//Uses +7 cycles: don't work
			
			int ival_bump		= inst[iCurrent].val.ival;
			
			//Subst first instruction with new
			
			inst[iCurrent-2].opcode = rvaluptr_dreflong_bump;
			inst[iCurrent-2].val1 = ival_bump;

			//Invalidate next instruction
			inst[iCurrent-1].opcode = inst[iCurrent].opcode = 0;
		}*/
	}

	
	

	if(priority == 2 && iCurrent >= 1)
	{
		//Two instructions group
		//Second instruction must be ignorated on interpretation
		
		if(isPush(inst[iCurrent-1].opcode))
		{
			//push...
			int opcode = 0;
			
			switch(inst[iCurrent].opcode)
			{				
				//add...
				case addint:	opcode = push_addint;		break;
				case adduint:	opcode = push_adduint;		break;			
				case addlong:	opcode = push_addlong;		break;
				case addulong:	opcode = push_addulong;	break;	
				case addllong:	opcode = push_addllong;	break;	
				case adddouble:	opcode = push_adddouble;	break;						
					
					//sub...
				case subint:	opcode = push_subint;		break;
				case subuint:	opcode = push_subuint;		break;			
				case sublong:	opcode = push_sublong;		break;
				case subulong:	opcode = push_subulong;	break;	
				case subllong:	opcode = push_subllong;	break;	
				case subdouble:	opcode = push_subdouble;	break;
					
					//div...
				case divint:	opcode = push_divint;		break;
				case divuint:	opcode = push_divuint;		break;			
				case divlong:	opcode = push_divlong;		break;
				case divulong:	opcode = push_divulong;	break;	
				case divllong:	opcode = push_divllong;	break;	
				case divdouble:	opcode = push_divdouble;	break;
					
					//mult...
				case multint:	opcode = push_multint;		break;
				case multuint:	opcode = push_multuint;	break;			
				case multlong:	opcode = push_multlong;	break;
				case multulong:	opcode = push_multulong;	break;	
				case multllong:	opcode = push_multllong;	break;	
				case multdouble:opcode = push_multdouble;	break;
					
					//mod...
				case modint:	opcode = push_modint;		break;
				case moduint:	opcode = push_moduint;		break;			
				case modlong:	opcode = push_modlong;		break;
				case modulong:	opcode = push_modulong;	break;	
				case modllong:	opcode = push_modllong;	break;	
					
					//lsht...
				case lshtint:	opcode = push_lshtint;		break;
				case lshtuint:	opcode = push_lshtuint;	break;			
				case lshtlong:	opcode = push_lshtlong;	break;
				case lshtulong:	opcode = push_lshtulong;	break;	
				case lshtllong:	opcode = push_lshtllong;	break;
					
					//rsht...
				case rshtint:	opcode = push_rshtint;		break;
				case rshtuint:	opcode = push_rshtuint;	break;			
				case rshtlong:	opcode = push_rshtlong;	break;
				case rshtulong:	opcode = push_rshtulong;	break;	
				case rshtllong:	opcode = push_rshtllong;	break;
					
					//lt...
				case ltint:		opcode = push_ltint;		break;
				case ltuint:	opcode = push_ltuint;		break;			
				case ltlong:	opcode = push_ltlong;		break;
				case ltulong:	opcode = push_ltulong;		break;	
				case ltllong:	opcode = push_ltllong;		break;	
				case ltdouble:	opcode = push_ltdouble;	break;	
				case ltptr:		opcode = push_ltptr;		break;
					
					//le...
				case leint:		opcode = push_leint;		break;
				case leuint:	opcode = push_leuint;		break;			
				case lelong:	opcode = push_lelong;		break;
				case leulong:	opcode = push_leulong;		break;	
				case lellong:	opcode = push_lellong;		break;	
				case ledouble:	opcode = push_ledouble;	break;	
				case leptr:		opcode = push_leptr;		break;						
					
					//eq...
				case eqint:		opcode = push_eqint;		break;
				case equint:	opcode = push_equint;		break;			
				case eqlong:	opcode = push_eqlong;		break;
				case equlong:	opcode = push_equlong;		break;	
				case eqllong:	opcode = push_eqllong;		break;	
				case eqdouble:	opcode = push_eqdouble;	break;	
				case eqptr:		opcode = push_eqptr;		break;
					
					//ne...
				case neint:		opcode = push_neint;		break;
				case neuint:	opcode = push_neuint;		break;			
				case nelong:	opcode = push_nelong;		break;
				case neulong:	opcode = push_neulong;		break;	
				case nellong:	opcode = push_nellong;		break;	
				case nedouble:	opcode = push_nedouble;	break;	
				case neptr:		opcode = push_neptr;		break;
					
					//gt...
				case gtint:		opcode = push_gtint;		break;
				case gtuint:	opcode = push_gtuint;		break;			
				case gtlong:	opcode = push_gtlong;		break;
				case gtulong:	opcode = push_gtulong;		break;	
				case gtllong:	opcode = push_gtllong;		break;	
				case gtdouble:	opcode = push_gtdouble;	break;	
				case gtptr:		opcode = push_gtptr;		break;
					
					//ge...
				case geint:		opcode = push_geint;		break;
				case geuint:	opcode = push_geuint;		break;			
				case gelong:	opcode = push_gelong;		break;
				case geulong:	opcode = push_geulong;		break;	
				case gellong:	opcode = push_gellong;		break;	
				case gedouble:	opcode = push_gedouble;	break;	
				case geptr:		opcode = push_geptr;		break;
					
					//lor...
				case lorint:	opcode = push_lorint;		break;
				case loruint:	opcode = push_loruint;		break;			
				case lorlong:	opcode = push_lorlong;		break;
				case lorulong:	opcode = push_lorulong;	break;	
				case lorllong:	opcode = push_lorllong;	break;	
				case lordouble:	opcode = push_lordouble;	break;	
				case lorptr:	opcode = push_lorptr;		break;
					
					//bor...
				case borint:	opcode = push_borint;		break;
				case boruint:	opcode = push_boruint;		break;			
				case borlong:	opcode = push_borlong;		break;
				case borulong:	opcode = push_borulong;	break;	
				case borllong:	opcode = push_borllong;	break;
					
					//xor...
				case xorint:	opcode = push_xorint;		break;
				case xoruint:	opcode = push_xoruint;		break;			
				case xorlong:	opcode = push_xorlong;		break;
				case xorulong:	opcode = push_xorulong;	break;	
				case xorllong:	opcode = push_xorllong;	break;
					
					//and...
				case andint:	opcode = push_andint;		break;
				case anduint:	opcode = push_anduint;		break;			
				case andlong:	opcode = push_andlong;		break;
				case andulong:	opcode = push_andulong;	break;	
				case andllong:	opcode = push_andllong;	break;
					
					//ref...
				case reffloat:	opcode = push_reffloat;	break;
				case refshort:	opcode = push_refshort;	break;			
				case refchar:	opcode = push_refchar;		break;
				case refint:	opcode = push_refint;		break;	
				case refuint:	opcode = push_refuint;		break;
				case reflong:	opcode = push_reflong;		break;
				case refulong:	opcode = push_refulong;	break;
				case refllong:	opcode = push_refllong;	break;
				case refdouble:	opcode = push_refdouble;	break;
				case refptr:	opcode = push_refptr;		break;
				case refuptr:	opcode = push_refuptr;		break;
					
					//ptr...
				case addptr2int:	opcode = push_addptr2int;	break;
				case addint2ptr:	opcode = push_addint2ptr;	break;			
				case subptrint:		opcode = push_subptrint;	break;
				case subptr:		opcode = push_subptr;		break;
			}
				
				
			if(opcode)
			{
				//Subst first instruction with new
				inst[iCurrent-1].opcode = opcode;
								
				//Invalidate next instruction
				inst[iCurrent].opcode = 0;
			}
		}
		else if(inst[iCurrent-1].opcode == bump && inst[iCurrent].opcode == pushlong)
		{
			//Create bump_pushlong
			//bump uses:		ip->val.ival
			//pushlong uses:	ip->val.lval
			//save 7 cycles
			
			int ival_bump		= inst[iCurrent-1].val.ival;
			long lval_pushlong	= inst[iCurrent].val.lval;
			
			//Subst first instruction with new	
			
			inst[iCurrent-1].opcode = bump_pushlong;
			inst[iCurrent-1].val.lval = lval_pushlong;
			inst[iCurrent-1].val1 = ival_bump;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == rvaluptr && inst[iCurrent].opcode == dreflong)
		{
			//Create rvaluptr_dreflong
			//rvaluptr uses:	ip->val.ival
			//dreflong uses:	STK[ToP]
			//save 15 cycles
			
			
			//Subst first instruction with new
			
			inst[iCurrent-1].opcode = rvaluptr_dreflong;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == lelong && inst[iCurrent].opcode == jmpFint)
		{
			//Create lelong_jmpFint
			//lelong uses:		STK
			//jmpFint uses:		ival
			//save 3 cycles
			
			
			//Subst first instruction with new	
			
			inst[iCurrent-1].opcode = lelong_jmpFint;
			inst[iCurrent-1].val.ival = inst[iCurrent].val.ival;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == rvaluptr && inst[iCurrent].opcode == bump)
		{
			//rvaluptr uses:	ip->val.ival
			//bump uses:		ip->val.ival
			//save 5 cycles
			
			int ival_bump = inst[iCurrent].val.ival,
				ival_rvaluptr = inst[iCurrent-1].val.ival;
			
			//Subst first instruction with new		
			
			inst[iCurrent-1].opcode = rvaluptr_bump;
			inst[iCurrent-1].val.ival = ival_rvaluptr;
			inst[iCurrent-1].val1 = ival_bump;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == bump && inst[iCurrent].opcode == pushint)
		{
			//Create bump_pushint
			//bump uses:		ip->val.ival
			//pushint uses:		ip->val.ival
			//save 0 cycles?
			
			int ival_bump		= inst[iCurrent-1].val.ival;
			int ival_pushint	= inst[iCurrent].val.ival;
			
			//Subst first instruction with new	
			
			inst[iCurrent-1].opcode = bump_pushint;
			inst[iCurrent-1].val.ival = ival_pushint;
			inst[iCurrent-1].val1 = ival_bump;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == drefdouble && inst[iCurrent].opcode == stoval)
		{
			//Create drefdouble_stoval
			//drefdouble uses:		STK
			//stoval uses:			ip->ext
			//save 6 cycles
			
			
			
			//Subst first instruction with new	
			
			inst[iCurrent-1].opcode = drefdouble_stoval;
			inst[iCurrent-1].ext = inst[iCurrent].ext;		

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		else if(inst[iCurrent-1].opcode == rvalint && inst[iCurrent].opcode == bump)
		{
			//Create rvalint_bump
			//rvalint uses:		ip->val.ival
			//bump uses:		ip->val.ival
			//save 6 cycles
			
			
			int ival_bump = inst[iCurrent].val.ival,
				ival_rvalint = inst[iCurrent-1].val.ival;
			
			//Subst first instruction with new		
			
			inst[iCurrent-1].opcode = rvalint_bump;
			inst[iCurrent-1].val.ival = ival_rvalint;
			inst[iCurrent-1].val1 = ival_bump;

			//Invalidate next instruction
			inst[iCurrent].opcode = 0;
		}
		/*else if(IsSto_op(inst[iCurrent-1].opcode) && IsGet_op(inst[iCurrent].opcode))
		{
			//Don't use this optimization (causes bug in checkers.ged - move the board)
			//remove sto, rval
			//save 1 cycles
			

			if( (inst[iCurrent-1].opcode == stochar && inst[iCurrent].opcode == rvalchar) ||
				(inst[iCurrent-1].opcode == stoshort && inst[iCurrent].opcode == rvalshort) ||
				(inst[iCurrent-1].opcode == stoint && inst[iCurrent].opcode == rvalint) ||
				(inst[iCurrent-1].opcode == stouint && inst[iCurrent].opcode == rvaluint) ||
				(inst[iCurrent-1].opcode == stolong && inst[iCurrent].opcode == rvallong) ||
				(inst[iCurrent-1].opcode == stoulong && inst[iCurrent].opcode == rvalulong) ||
				(inst[iCurrent-1].opcode == stollong && inst[iCurrent].opcode == rvalllong) ||
				(inst[iCurrent-1].opcode == stouptr && inst[iCurrent].opcode == rvaluptr) ||
				(inst[iCurrent-1].opcode == stoptr && inst[iCurrent].opcode == rvalptr) ||
				(inst[iCurrent-1].opcode == stofloat && inst[iCurrent].opcode == rvalfloat) ||
				(inst[iCurrent-1].opcode == stodouble && inst[iCurrent].opcode == rvaldouble) )
			{
				//inst[iCurrent-1].opcode = 0; //Don't invalidate sto (causes bug in 1945.ged cannon rotation)
				inst[iCurrent].opcode = 0;
			}
		}*/
	}
}

void OptimizeCode(code_t *c) //maks
{
	int i, priority;

	for(priority = 3; priority > 1; priority--)
	{
		for(i = 0; i < c->nextinst; i++)
		{
			CrateBigInstruction(c->inst, i, priority);
		}
	}
}

void EiC_generate(code_t * code, int opcode,val_t *val,int ext)
{
    InsT_t * inst;
    inst = code->inst;
    if(code->nextinst == code->binst)
	if(!(code->binst%ModSize)) {
	    if(!code->binst)
		inst = (InsT_t*)xcalloc(1,sizeof(InsT_t)*ModSize);
	    else
		{
			int reallocFirstInst = 0; //maks: direct execution
			if(firstInst.inst && firstInst.inst == inst)
				reallocFirstInst = 1;
			
			inst = (InsT_t*)xrealloc(inst,(code->binst+ModSize)*sizeof(InsT_t));
			
			if(reallocFirstInst) //maks: direct execution
				firstInst.inst = inst;
		}

	    code->binst += ModSize;
	}
    code->inst = inst;
    inst[code->nextinst].opcode = opcode;
    inst[code->nextinst].val = *val;
    inst[code->nextinst].ext = ext;

#ifndef STAND_ALONE_GAME
    inst[code->nextinst].line = CurrentLineNo();
#endif

	if(storeFirstInst && !firstInst.inst) //maks: direct execution
		firstInst = *code;

	/*if(storeFirstInst && firstInst.inst) //maks
	{
		//Don't catch all instructions
		CrateBigInstruction(firstInst.inst, code->nextinst, 3); 
		CrateBigInstruction(firstInst.inst, code->nextinst, 2); 
	}*/

	code->nextinst++;
}

#if 0
void copycode(code_t * c1, code_t * c2)
{
    /* this function needs to handle label and gotos */
    unsigned int i;
    InsT_t *inst;
    inst = c2->inst;
    for(i=0;i<c2->nextinst;++i,++inst) {
	EiC_generate(c1,inst->opcode,&inst->val,inst->ext);
	c1->inst[c1->nextinst-1].line = inst->line;
    }
}
#else
void copycode(code_t * c1, code_t * c2)
{
    Label_t * EiCp_addLabel(Label_t *, char *, int, int);
    InsT_t *inst;
    int h, Tsize;
    Label_t *lab;
	int reallocFirstInst = 0; //maks: direct execution

    if(!c2->nextinst)
	return;
    /* concatenate labels*/
    if(c2->labels) {
	lab = c2->labels;
	while(lab) {
	    c1->labels = EiCp_addLabel(c1->labels,lab->name,lab->loc+c1->nextinst,1);
	    lab = lab->nxt;
	}
    }
    /* conatenate gotos */
    if(c2->gotos) {
	lab = c2->gotos;
	while(lab) {
	    c1->gotos = EiCp_addLabel(c1->gotos,lab->name,lab->loc+c1->nextinst,0);
	    lab = lab->nxt;
	}
    }

    Tsize = c1->nextinst + c2->nextinst;
    /* make memory size a multiple of ModSize */
    h = Tsize;
    if(Tsize%ModSize) 
	Tsize += ModSize - (Tsize%ModSize);
	
	if(firstInst.inst && firstInst.inst == c1->inst) //maks: direct execution
		reallocFirstInst = 1;

    if(c1->binst)
	c1->inst = xrealloc(c1->inst,Tsize*sizeof(InsT_t));
    else
	c1->inst = xcalloc(Tsize,sizeof(InsT_t));
    inst = c1->inst;
    memcpy(&inst[c1->nextinst], c2->inst, c2->nextinst * sizeof(InsT_t));
    c1->binst = Tsize;
    c1->nextinst = h;

	if(reallocFirstInst) //maks: direct execution
		firstInst = *c1;
}

#endif


void EiC_concode(code_t * c1, code_t * c2)
{
	copycode(c1,c2);

	if(firstInst.inst && firstInst.inst == c2->inst) //maks: direct execution
		firstInst = *c1;

    EiC_freecode(c2);
}
void EiC_contoken(token_t * e1, token_t * e2)
{
    EiC_concode(&e1->Code,&e2->Code);
    e1->Type = EiC_getcontype(e1->Type,e2->Type);
    EiC_freetoken(e2);
}

void EiC_swaptokens(token_t *e1, token_t * e2)
{
    token_t e3;
    e3 = *e2;
    *e2 = *e1;
    *e1 = e3;
}
    

#if 0

typedef struct list_t {
    char * fname;
    struct list_t *nxt;
}list_t;


typdef struct llist_t {
    list_t **list;
    int n;
} llist_t;

static llist_t ** includes = NULL, ** included = NULL;

static int inllist(llist_t **list, char *fname)
{
    /* returns -1 if not in list */
    int i;
    for(i=0;i<list->n,++i) {
	if(fname == list->list[i]->fname == 0)
	    return i;
    }
    return -1;
}

static list_t * add2list(list_t *list, char *fname)
{
   list_t n = calloc(sizeof(list_t),1);
   n->fname = fname;
   n->nxt = list;
   return n;
}


static void add2includes(char *file, char * includes)
{
    int i = inlist(includes,file);

    if(i < 0) {
	i = includes->n;
	includes->list = realloc(includes->list, 
				 (includes->n + 1) * sizeof(*includes->list));
	
	includes->list = 
	includes





#endif

















