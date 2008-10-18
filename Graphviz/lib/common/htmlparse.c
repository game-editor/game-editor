/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with html or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum htmltokentype {
     T_end_br = 258,
     T_end_img = 259,
     T_row = 260,
     T_end_row = 261,
     T_html = 262,
     T_end_html = 263,
     T_end_table = 264,
     T_end_cell = 265,
     T_end_font = 266,
     T_string = 267,
     T_error = 268,
     T_BR = 269,
     T_br = 270,
     T_IMG = 271,
     T_img = 272,
     T_table = 273,
     T_cell = 274,
     T_font = 275
   };
#endif
#define T_end_br 258
#define T_end_img 259
#define T_row 260
#define T_end_row 261
#define T_html 262
#define T_end_html 263
#define T_end_table 264
#define T_end_cell 265
#define T_end_font 266
#define T_string 267
#define T_error 268
#define T_BR 269
#define T_br 270
#define T_IMG 271
#define T_img 272
#define T_table 273
#define T_cell 274
#define T_font 275




/* Copy the first part of user declarations.  */
#line 17 "../../lib/common/htmlparse.y"


#include "render.h"
#include "htmltable.h"
#include "htmllex.h"

extern int htmlparse(void);

static struct {
  htmllabel_t* lbl;       /* Generated label */
  htmltbl_t*   tblstack;  /* Stack of tables maintained during parsing */
  Dt_t*        lines;     /* Dictionary for lines of text */
  agxbuf*      str;       /* Buffer for text */
} HTMLstate;

/* free_ritem:
 * Free row. This closes and frees row's list, then
 * the pitem itself is freed.
 */
static void
free_ritem(Dt_t* d, pitem* p,Dtdisc_t* ds)
{
  dtclose (p->u.rp);
  free (p);
}

/* free_ritem:
 * Free cell item after cell has been copies into final table.
 * Only the pitem is freed.
 */
static void
free_item(Dt_t* d, pitem* p,Dtdisc_t* ds)
{
  free (p);
}

/* cleanTbl:
 * Clean up table if error in parsing.
 */
static void
cleanTbl (htmltbl_t* tp)
{
  dtclose (tp->u.p.rows);
  free_html_data (&tp->data);
  free (tp);
}

/* cleanCell:
 * Clean up cell if error in parsing.
 */
static void
cleanCell (htmlcell_t* cp)
{
  if (cp->child.kind == HTML_TBL) cleanTbl (cp->child.u.tbl);
  else if (cp->child.kind == HTML_TEXT) free_html_text (cp->child.u.txt);
  free_html_data (&cp->data);
  free (cp);
}

/* free_citem:
 * Free cell item during parsing. This frees cell and pitem.
 */
static void
free_citem(Dt_t* d, pitem* p,Dtdisc_t* ds)
{
  cleanCell (p->u.cp);
  free (p);
}

static Dtdisc_t rowDisc = {
    offsetof(pitem,u),
    sizeof(void*),
    offsetof(pitem,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_ritem,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};
static Dtdisc_t cellDisc = {
    offsetof(pitem,u),
    sizeof(void*),
    offsetof(pitem,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_item,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

typedef struct {
  Dtlink_t      link;
  const char*   s;          /* line of text */
  char          c;          /* alignment of text */
} sitem;

static void
free_sitem(Dt_t* d,sitem* p,Dtdisc_t* ds)
{
  free (p);
}

static Dtdisc_t strDisc = {
    offsetof(sitem,s),
    sizeof(char*),
    offsetof(sitem,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_sitem,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

static void
appendStrList(const char* p,int v)
{
  sitem*  sp = NEW(sitem);
  sp->s = strdup(p);
  sp->c = v;
  dtinsert (HTMLstate.lines, sp);
}

/* mkText:
 * Construct htmltxt_t from list of lines in HTMLstate.lines.
 * lastl is a last, odd line with no <BR>, so we use n by default.
 */
static htmltxt_t*
mkText (const char* lastl)
{
  int         cnt;
  textline_t* lp;
  sitem*      sp;
  Dt_t*       lines = HTMLstate.lines;
  htmltxt_t* tp = NEW(htmltxt_t);

  if (lines)
    cnt = dtsize (lines);
  else
    cnt = 0;
  if (lastl) cnt++;

  tp->nlines = cnt;
  tp->line = N_NEW(cnt+1,textline_t);

  lp = tp->line;
  if (lines) {
    sp = (sitem*)dtflatten(lines);
    for (; sp; sp = (sitem*)dtlink(lines,(Dtlink_t*)sp)) {
      lp->str = (char*)(sp->s);
      lp->xshow = NULL;
      lp->just = sp->c;
      lp++;
    }
  }
  if (lastl) {
    lp->str = strdup(lastl);
    lp->just = 'n';
  }

  if (lines) dtclear (lines);

  return tp;
}

/* addRow:
 * Add new cell row to current table.
 */
static void addRow (void)
{
  Dt_t*      dp = dtopen(&cellDisc, Dtqueue);
  htmltbl_t* tbl = HTMLstate.tblstack;
  pitem*     sp = NEW(pitem);

  sp->u.rp = dp;
  dtinsert (tbl->u.p.rows, sp);
}

/* setCell:
 * Set cell body and type and attach to row
 */
static void setCell (htmlcell_t* cp, void* obj, int kind)
{
  pitem*     sp = NEW(pitem);
  htmltbl_t* tbl = HTMLstate.tblstack;
  pitem*     rp = (pitem*)dtlast (tbl->u.p.rows);
  Dt_t*      row = rp->u.rp;

  sp->u.cp = cp;
  dtinsert (row, sp);
  cp->child.kind = kind;
  if (kind == HTML_TEXT)
    cp->child.u.txt = (htmltxt_t*)obj;
  else if (kind == HTML_IMAGE)
    cp->child.u.img = (htmlimg_t*)obj;
  else
    cp->child.u.tbl = (htmltbl_t*)obj;
}

/* setFont:
 * Copy in font attributes. fp has the new attributes.
 * curf corresponds to the current font info of the object.
 * From the parser, we are moving out from the object. Since
 * the inmost value is the one used, we only use a new value
 * if the attribute has not already been assigned.
 */
static htmlfont_t* setFont (htmlfont_t* fp, htmlfont_t*  curf)
{
  if (curf) {
    if (curf->size < 0.0) curf->size = fp->size;
    if (!curf->color) curf->color = fp->color;
    else if (fp->color) free (fp->color);
    if (!curf->name) curf->name = fp->name;
    else if (fp->name) free (fp->name);
    free (fp);
    return curf;
  }
  else
    return fp;
}


/* fontText:
 * Attach font information to text.
 */
static void fontText (htmlfont_t* fp, htmltxt_t* cp)
{
  cp->font = setFont (fp, cp->font);
}

/* fontTable:
 * Attach font information to table.
 */
static void fontTable (htmlfont_t* fp, htmltbl_t* cp)
{
  cp->font = setFont (fp, cp->font);
}

/* mkLabel:
 * Create label, given body and type.
 */
static htmllabel_t* mkLabel (void* obj, int kind)
{
  htmllabel_t* lp = NEW(htmllabel_t);

  lp->kind = kind;
  if (kind == HTML_TEXT)
    lp->u.txt = (htmltxt_t*)obj;
  else
    lp->u.tbl = (htmltbl_t*)obj;
    
  return lp;
}

/* cleanup:
 * Called on error. Frees resources allocated during parsing.
 * This includes a label, plus a walk down the stack of
 * tables. Note that we use the free_citem function to actually
 * free cells.
 */
static void cleanup (void)
{
  htmltbl_t* tp = HTMLstate.tblstack;
  htmltbl_t* next;

  if (HTMLstate.lbl) {
    free_html_label (HTMLstate.lbl,1);
    HTMLstate.lbl = NULL;
  }
  cellDisc.freef = (Dtfree_f)free_citem;
  while (tp) {
    next = tp->u.p.prev;
    cleanTbl (tp);
    tp = next;
  }
  cellDisc.freef = (Dtfree_f)free_item;
}

/* nonSpace:
 * Return 1 if s contains a non-space character.
 */
static int nonSpace (char* s)
{
  char   c;

  while ((c = *s++)) {
    if (c != ' ') return 1;
  }
  return 0;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 312 "../../lib/common/htmlparse.y"
typedef union YYSTYPE {
  int    i;
  htmltxt_t*  txt;
  htmlcell_t*  cell;
  htmltbl_t*   tbl;
  htmlfont_t*  font;
  htmlimg_t*   img;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 420 "y.tab.c"
# define htmlstype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 432 "y.tab.c"

#if ! defined (htmloverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (htmloverflow) || YYERROR_VERBOSE */


#if (! defined (htmloverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union htmlalloc
{
  short int htmlss;
  YYSTYPE htmlvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union htmlalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T htmli;		\
	  for (htmli = 0; htmli < (Count); htmli++)	\
	    (To)[htmli] = (From)[htmli];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T htmlnewbytes;						\
	YYCOPY (&htmlptr->Stack, Stack, htmlsize);				\
	Stack = &htmlptr->Stack;						\
	htmlnewbytes = htmlstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	htmlptr += htmlnewbytes / sizeof (*htmlptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char htmlsigned_char;
#else
   typedef short int htmlsigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   47

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  22
/* YYNRULES -- Number of rules. */
#define YYNRULES  38
/* YYNRULES -- Number of states. */
#define YYNSTATES  56

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   275

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? htmltranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char htmltranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char htmlprhs[] =
{
       0,     0,     3,     4,     9,    10,    15,    17,    19,    23,
      25,    28,    30,    31,    34,    38,    41,    43,    45,    48,
      49,    56,    58,    62,    64,    65,    67,    70,    71,    76,
      78,    81,    82,    87,    88,    93,    94,    99,   102
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const htmlsigned_char htmlrhs[] =
{
      22,     0,    -1,    -1,     7,    25,    23,     8,    -1,    -1,
       7,    32,    24,     8,    -1,     1,    -1,    26,    -1,    20,
      26,    11,    -1,    27,    -1,    27,    29,    -1,    29,    -1,
      -1,    29,    28,    -1,    27,    29,    28,    -1,    15,     3,
      -1,    14,    -1,    12,    -1,    29,    12,    -1,    -1,    33,
      18,    31,    34,     9,    33,    -1,    30,    -1,    20,    30,
      11,    -1,    29,    -1,    -1,    35,    -1,    34,    35,    -1,
      -1,     5,    36,    37,     6,    -1,    38,    -1,    37,    38,
      -1,    -1,    19,    32,    39,    10,    -1,    -1,    19,    25,
      40,    10,    -1,    -1,    19,    42,    41,    10,    -1,    17,
       4,    -1,    16,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int htmlrline[] =
{
       0,   338,   338,   338,   339,   339,   340,   343,   344,   347,
     349,   351,   354,   357,   359,   363,   364,   367,   368,   371,
     371,   391,   392,   395,   396,   399,   400,   403,   403,   406,
     407,   410,   410,   411,   411,   412,   412,   415,   416
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const htmltname[] =
{
  "$end", "error", "$undefined", "T_end_br", "T_end_img", "T_row",
  "T_end_row", "T_html", "T_end_html", "T_end_table", "T_end_cell",
  "T_end_font", "T_string", "T_error", "T_BR", "T_br", "T_IMG", "T_img",
  "T_table", "T_cell", "T_font", "$accept", "html", "@1", "@2", "fonttext",
  "text", "lines", "br", "string", "table", "@3", "fonttable", "opt_space",
  "rows", "row", "@4", "cells", "cell", "@5", "@6", "@7", "image", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int htmltoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char htmlr1[] =
{
       0,    21,    23,    22,    24,    22,    22,    25,    25,    26,
      26,    26,    26,    27,    27,    28,    28,    29,    29,    31,
      30,    32,    32,    33,    33,    34,    34,    36,    35,    37,
      37,    39,    38,    40,    38,    41,    38,    42,    42
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char htmlr2[] =
{
       0,     2,     0,     4,     0,     4,     1,     1,     3,     1,
       2,     1,     0,     2,     3,     2,     1,     1,     2,     0,
       6,     1,     3,     1,     0,     1,     2,     0,     4,     1,
       2,     0,     4,     0,     4,     0,     4,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char htmldefact[] =
{
       0,     6,    12,     0,    17,    12,     2,     7,     9,    11,
      21,     4,     0,     1,     0,     0,     0,    10,    18,    16,
       0,    13,     0,    19,     8,    22,     3,    14,    15,     5,
       0,    27,     0,    25,     0,    24,    26,    12,     0,    29,
      23,    20,    38,     0,    33,    31,    35,    28,    30,    37,
       0,     0,     0,    34,    32,    36
};

/* YYDEFGOTO[NTERM-NUM]. */
static const htmlsigned_char htmldefgoto[] =
{
      -1,     3,    16,    22,     6,     7,     8,    21,     9,    10,
      30,    11,    12,    32,    33,    34,    38,    39,    51,    50,
      52,    46
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -11
static const htmlsigned_char htmlpact[] =
{
      11,   -11,    -9,     4,   -11,     5,   -11,   -11,    -7,     1,
     -11,   -11,     3,   -11,    19,    20,    17,    14,   -11,   -11,
      29,   -11,    25,   -11,   -11,   -11,   -11,   -11,   -11,   -11,
      30,   -11,    15,   -11,    18,    -7,   -11,   -10,    -5,   -11,
      10,   -11,   -11,    32,   -11,   -11,   -11,   -11,   -11,   -11,
      24,    28,    31,   -11,   -11,   -11
};

/* YYPGOTO[NTERM-NUM].  */
static const htmlsigned_char htmlpgoto[] =
{
     -11,   -11,   -11,   -11,     2,    35,   -11,    26,    -8,    37,
     -11,     7,    12,   -11,    13,   -11,   -11,     8,   -11,   -11,
     -11,   -11
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -25
static const htmlsigned_char htmltable[] =
{
      17,    47,     4,     4,    13,     4,    42,    43,   -24,   -24,
       5,     5,     1,    18,    37,    19,    20,     4,     2,   -23,
      31,    23,    18,   -24,    35,    26,    18,    40,    19,    20,
      24,    25,    28,    29,    53,    31,    49,    37,    54,    44,
      14,    55,    15,    27,    45,    36,    48,    41
};

static const unsigned char htmlcheck[] =
{
       8,     6,    12,    12,     0,    12,    16,    17,    18,    18,
      20,    20,     1,    12,    19,    14,    15,    12,     7,    18,
       5,    18,    12,    18,     9,     8,    12,    35,    14,    15,
      11,    11,     3,     8,    10,     5,     4,    19,    10,    37,
       5,    10,     5,    17,    37,    32,    38,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char htmlstos[] =
{
       0,     1,     7,    22,    12,    20,    25,    26,    27,    29,
      30,    32,    33,     0,    26,    30,    23,    29,    12,    14,
      15,    28,    24,    18,    11,    11,     8,    28,     3,     8,
      31,     5,    34,    35,    36,     9,    35,    19,    37,    38,
      29,    33,    16,    17,    25,    32,    42,     6,    38,     4,
      40,    39,    41,    10,    10,    10
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define htmlerrok		(htmlerrstatus = 0)
#define htmlclearin	(htmlchar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto htmlacceptlab
#define YYABORT		goto htmlabortlab
#define YYERROR		goto htmlerrorlab


/* Like YYERROR except do call htmlerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto htmlerrlab

#define YYRECOVERING()  (!!htmlerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (htmlchar == YYEMPTY && htmllen == 1)				\
    {								\
      htmlchar = (Token);						\
      htmllval = (Value);						\
      htmltoken = YYTRANSLATE (htmlchar);				\
      YYPOPSTACK;						\
      goto htmlbackup;						\
    }								\
  else								\
    { 								\
      htmlerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `htmllex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX htmllex (YYLEX_PARAM)
#else
# define YYLEX htmllex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (htmldebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (htmldebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      htmlsymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| html_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
html_stack_print (short int *bottom, short int *top)
#else
static void
html_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (htmldebug)							\
    html_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
html_reduce_print (int htmlrule)
#else
static void
html_reduce_print (htmlrule)
    int htmlrule;
#endif
{
  int htmli;
  unsigned int htmllno = htmlrline[htmlrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             htmlrule - 1, htmllno);
  /* Print the symbols being reduced, and their result.  */
  for (htmli = htmlprhs[htmlrule]; 0 <= htmlrhs[htmli]; htmli++)
    YYFPRINTF (stderr, "%s ", htmltname [htmlrhs[htmli]]);
  YYFPRINTF (stderr, "-> %s\n", htmltname [htmlr1[htmlrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (htmldebug)				\
    html_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int htmldebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef htmlstrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define htmlstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
htmlstrlen (const char *htmlstr)
#   else
htmlstrlen (htmlstr)
     const char *htmlstr;
#   endif
{
  register const char *htmls = htmlstr;

  while (*htmls++ != '\0')
    continue;

  return htmls - htmlstr - 1;
}
#  endif
# endif

# ifndef htmlstpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define htmlstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
htmlstpcpy (char *htmldest, const char *htmlsrc)
#   else
htmlstpcpy (htmldest, htmlsrc)
     char *htmldest;
     const char *htmlsrc;
#   endif
{
  register char *htmld = htmldest;
  register const char *htmls = htmlsrc;

  while ((*htmld++ = *htmls++) != '\0')
    continue;

  return htmld - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
htmlsymprint (FILE *htmloutput, int htmltype, YYSTYPE *htmlvaluep)
#else
static void
htmlsymprint (htmloutput, htmltype, htmlvaluep)
    FILE *htmloutput;
    int htmltype;
    YYSTYPE *htmlvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) htmlvaluep;

  if (htmltype < YYNTOKENS)
    YYFPRINTF (htmloutput, "token %s (", htmltname[htmltype]);
  else
    YYFPRINTF (htmloutput, "nterm %s (", htmltname[htmltype]);


# ifdef YYPRINT
  if (htmltype < YYNTOKENS)
    YYPRINT (htmloutput, htmltoknum[htmltype], *htmlvaluep);
# endif
  switch (htmltype)
    {
      default:
        break;
    }
  YYFPRINTF (htmloutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
htmldestruct (const char *htmlmsg, int htmltype, YYSTYPE *htmlvaluep)
#else
static void
htmldestruct (htmlmsg, htmltype, htmlvaluep)
    const char *htmlmsg;
    int htmltype;
    YYSTYPE *htmlvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) htmlvaluep;

  if (!htmlmsg)
    htmlmsg = "Deleting";
  YY_SYMBOL_PRINT (htmlmsg, htmltype, htmlvaluep, htmllocationp);

  switch (htmltype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int htmlparse (void *YYPARSE_PARAM);
# else
int htmlparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int htmlparse (void);
#else
int htmlparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int htmlchar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE htmllval;

/* Number of syntax errors so far.  */
int htmlnerrs;



/*----------.
| htmlparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int htmlparse (void *YYPARSE_PARAM)
# else
int htmlparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
htmlparse (void)
#else
int
htmlparse ()

#endif
#endif
{
  
  register int htmlstate;
  register int htmln;
  int htmlresult;
  /* Number of tokens to shift before error messages enabled.  */
  int htmlerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int htmltoken = 0;

  /* Three stacks and their tools:
     `htmlss': related to states,
     `htmlvs': related to semantic values,
     `htmlls': related to locations.

     Refer to the stacks thru separate pointers, to allow htmloverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int htmlssa[YYINITDEPTH];
  short int *htmlss = htmlssa;
  register short int *htmlssp;

  /* The semantic value stack.  */
  YYSTYPE htmlvsa[YYINITDEPTH];
  YYSTYPE *htmlvs = htmlvsa;
  register YYSTYPE *htmlvsp;



#define YYPOPSTACK   (htmlvsp--, htmlssp--)

  YYSIZE_T htmlstacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE htmlval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int htmllen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  htmlstate = 0;
  htmlerrstatus = 0;
  htmlnerrs = 0;
  htmlchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  htmlssp = htmlss;
  htmlvsp = htmlvs;


  htmlvsp[0] = htmllval;

  goto htmlsetstate;

/*------------------------------------------------------------.
| htmlnewstate -- Push a new state, which is found in htmlstate.  |
`------------------------------------------------------------*/
 htmlnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  htmlssp++;

 htmlsetstate:
  *htmlssp = htmlstate;

  if (htmlss + htmlstacksize - 1 <= htmlssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T htmlsize = htmlssp - htmlss + 1;

#ifdef htmloverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *htmlvs1 = htmlvs;
	short int *htmlss1 = htmlss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if htmloverflow is a macro.  */
	htmloverflow ("parser stack overflow",
		    &htmlss1, htmlsize * sizeof (*htmlssp),
		    &htmlvs1, htmlsize * sizeof (*htmlvsp),

		    &htmlstacksize);

	htmlss = htmlss1;
	htmlvs = htmlvs1;
      }
#else /* no htmloverflow */
# ifndef YYSTACK_RELOCATE
      goto htmloverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= htmlstacksize)
	goto htmloverflowlab;
      htmlstacksize *= 2;
      if (YYMAXDEPTH < htmlstacksize)
	htmlstacksize = YYMAXDEPTH;

      {
	short int *htmlss1 = htmlss;
	union htmlalloc *htmlptr =
	  (union htmlalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (htmlstacksize));
	if (! htmlptr)
	  goto htmloverflowlab;
	YYSTACK_RELOCATE (htmlss);
	YYSTACK_RELOCATE (htmlvs);

#  undef YYSTACK_RELOCATE
	if (htmlss1 != htmlssa)
	  YYSTACK_FREE (htmlss1);
      }
# endif
#endif /* no htmloverflow */

      htmlssp = htmlss + htmlsize - 1;
      htmlvsp = htmlvs + htmlsize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) htmlstacksize));

      if (htmlss + htmlstacksize - 1 <= htmlssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", htmlstate));

  goto htmlbackup;

/*-----------.
| htmlbackup.  |
`-----------*/
htmlbackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* htmlresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  htmln = htmlpact[htmlstate];
  if (htmln == YYPACT_NINF)
    goto htmldefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (htmlchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      htmlchar = YYLEX;
    }

  if (htmlchar <= YYEOF)
    {
      htmlchar = htmltoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      htmltoken = YYTRANSLATE (htmlchar);
      YY_SYMBOL_PRINT ("Next token is", htmltoken, &htmllval, &htmllloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  htmln += htmltoken;
  if (htmln < 0 || YYLAST < htmln || htmlcheck[htmln] != htmltoken)
    goto htmldefault;
  htmln = htmltable[htmln];
  if (htmln <= 0)
    {
      if (htmln == 0 || htmln == YYTABLE_NINF)
	goto htmlerrlab;
      htmln = -htmln;
      goto htmlreduce;
    }

  if (htmln == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", htmltoken, &htmllval, &htmllloc);

  /* Discard the token being shifted unless it is eof.  */
  if (htmlchar != YYEOF)
    htmlchar = YYEMPTY;

  *++htmlvsp = htmllval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (htmlerrstatus)
    htmlerrstatus--;

  htmlstate = htmln;
  goto htmlnewstate;


/*-----------------------------------------------------------.
| htmldefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
htmldefault:
  htmln = htmldefact[htmlstate];
  if (htmln == 0)
    goto htmlerrlab;
  goto htmlreduce;


/*-----------------------------.
| htmlreduce -- Do a reduction.  |
`-----------------------------*/
htmlreduce:
  /* htmln is the number of a rule to reduce with.  */
  htmllen = htmlr2[htmln];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  htmlval = htmlvsp[1-htmllen];


  YY_REDUCE_PRINT (htmln);
  switch (htmln)
    {
        case 2:
#line 338 "../../lib/common/htmlparse.y"
    { HTMLstate.lbl = mkLabel((htmlvsp[0].txt),HTML_TEXT); }
    break;

  case 4:
#line 339 "../../lib/common/htmlparse.y"
    { HTMLstate.lbl = mkLabel((htmlvsp[0].tbl),HTML_TBL); }
    break;

  case 6:
#line 340 "../../lib/common/htmlparse.y"
    { cleanup(); YYABORT; }
    break;

  case 7:
#line 343 "../../lib/common/htmlparse.y"
    { (htmlval.txt) = (htmlvsp[0].txt); }
    break;

  case 8:
#line 344 "../../lib/common/htmlparse.y"
    { fontText((htmlvsp[-2].font),(htmlvsp[-1].txt)); (htmlval.txt) = (htmlvsp[-1].txt); }
    break;

  case 9:
#line 348 "../../lib/common/htmlparse.y"
    { (htmlval.txt) = mkText (NULL); }
    break;

  case 10:
#line 350 "../../lib/common/htmlparse.y"
    { (htmlval.txt) = mkText (agxbuse(HTMLstate.str)); }
    break;

  case 11:
#line 352 "../../lib/common/htmlparse.y"
    { (htmlval.txt) = mkText (agxbuse(HTMLstate.str)); }
    break;

  case 12:
#line 354 "../../lib/common/htmlparse.y"
    { (htmlval.txt) = mkText (NULL); }
    break;

  case 13:
#line 358 "../../lib/common/htmlparse.y"
    { appendStrList (agxbuse(HTMLstate.str),(htmlvsp[0].i)); }
    break;

  case 14:
#line 360 "../../lib/common/htmlparse.y"
    { appendStrList (agxbuse(HTMLstate.str), (htmlvsp[0].i)); }
    break;

  case 15:
#line 363 "../../lib/common/htmlparse.y"
    { (htmlval.i) = (htmlvsp[-1].i); }
    break;

  case 16:
#line 364 "../../lib/common/htmlparse.y"
    { (htmlval.i) = (htmlvsp[0].i); }
    break;

  case 19:
#line 371 "../../lib/common/htmlparse.y"
    { 
          if (nonSpace(agxbuse(HTMLstate.str))) {
            htmlerror ("Syntax error: non-space string used before <TABLE>");
            cleanup(); YYABORT;
          }
          (htmlvsp[0].tbl)->u.p.prev = HTMLstate.tblstack;
          (htmlvsp[0].tbl)->u.p.rows = dtopen(&rowDisc, Dtqueue);
          HTMLstate.tblstack = (htmlvsp[0].tbl);
          (htmlval.tbl) = (htmlvsp[0].tbl);
        }
    break;

  case 20:
#line 381 "../../lib/common/htmlparse.y"
    {
          if (nonSpace(agxbuse(HTMLstate.str))) {
            htmlerror ("Syntax error: non-space string used after </TABLE>");
            cleanup(); YYABORT;
          }
          (htmlval.tbl) = HTMLstate.tblstack;
          HTMLstate.tblstack = HTMLstate.tblstack->u.p.prev;
        }
    break;

  case 21:
#line 391 "../../lib/common/htmlparse.y"
    { (htmlval.tbl) = (htmlvsp[0].tbl); }
    break;

  case 22:
#line 392 "../../lib/common/htmlparse.y"
    { fontTable((htmlvsp[-2].font),(htmlvsp[-1].tbl)); (htmlval.tbl) = (htmlvsp[-1].tbl); }
    break;

  case 27:
#line 403 "../../lib/common/htmlparse.y"
    { addRow (); }
    break;

  case 31:
#line 410 "../../lib/common/htmlparse.y"
    { setCell((htmlvsp[-1].cell),(htmlvsp[0].tbl),HTML_TBL); }
    break;

  case 33:
#line 411 "../../lib/common/htmlparse.y"
    { setCell((htmlvsp[-1].cell),(htmlvsp[0].txt),HTML_TEXT); }
    break;

  case 35:
#line 412 "../../lib/common/htmlparse.y"
    { setCell((htmlvsp[-1].cell),(htmlvsp[0].img),HTML_IMAGE); }
    break;

  case 37:
#line 415 "../../lib/common/htmlparse.y"
    { (htmlval.img) = (htmlvsp[-1].img); }
    break;

  case 38:
#line 416 "../../lib/common/htmlparse.y"
    { (htmlval.img) = (htmlvsp[0].img); }
    break;


    }

/* Line 1037 of yacc.c.  */
#line 1514 "y.tab.c"

  htmlvsp -= htmllen;
  htmlssp -= htmllen;


  YY_STACK_PRINT (htmlss, htmlssp);

  *++htmlvsp = htmlval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  htmln = htmlr1[htmln];

  htmlstate = htmlpgoto[htmln - YYNTOKENS] + *htmlssp;
  if (0 <= htmlstate && htmlstate <= YYLAST && htmlcheck[htmlstate] == *htmlssp)
    htmlstate = htmltable[htmlstate];
  else
    htmlstate = htmldefgoto[htmln - YYNTOKENS];

  goto htmlnewstate;


/*------------------------------------.
| htmlerrlab -- here on detecting error |
`------------------------------------*/
htmlerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!htmlerrstatus)
    {
      ++htmlnerrs;
#if YYERROR_VERBOSE
      htmln = htmlpact[htmlstate];

      if (YYPACT_NINF < htmln && htmln < YYLAST)
	{
	  YYSIZE_T htmlsize = 0;
	  int htmltype = YYTRANSLATE (htmlchar);
	  const char* htmlprefix;
	  char *htmlmsg;
	  int htmlx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int htmlxbegin = htmln < 0 ? -htmln : 0;

	  /* Stay within bounds of both htmlcheck and htmltname.  */
	  int htmlchecklim = YYLAST - htmln;
	  int htmlxend = htmlchecklim < YYNTOKENS ? htmlchecklim : YYNTOKENS;
	  int htmlcount = 0;

	  htmlprefix = ", expecting ";
	  for (htmlx = htmlxbegin; htmlx < htmlxend; ++htmlx)
	    if (htmlcheck[htmlx + htmln] == htmlx && htmlx != YYTERROR)
	      {
		htmlsize += htmlstrlen (htmlprefix) + htmlstrlen (htmltname [htmlx]);
		htmlcount += 1;
		if (htmlcount == 5)
		  {
		    htmlsize = 0;
		    break;
		  }
	      }
	  htmlsize += (sizeof ("syntax error, unexpected ")
		     + htmlstrlen (htmltname[htmltype]));
	  htmlmsg = (char *) YYSTACK_ALLOC (htmlsize);
	  if (htmlmsg != 0)
	    {
	      char *htmlp = htmlstpcpy (htmlmsg, "syntax error, unexpected ");
	      htmlp = htmlstpcpy (htmlp, htmltname[htmltype]);

	      if (htmlcount < 5)
		{
		  htmlprefix = ", expecting ";
		  for (htmlx = htmlxbegin; htmlx < htmlxend; ++htmlx)
		    if (htmlcheck[htmlx + htmln] == htmlx && htmlx != YYTERROR)
		      {
			htmlp = htmlstpcpy (htmlp, htmlprefix);
			htmlp = htmlstpcpy (htmlp, htmltname[htmlx]);
			htmlprefix = " or ";
		      }
		}
	      htmlerror (htmlmsg);
	      YYSTACK_FREE (htmlmsg);
	    }
	  else
	    htmlerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	htmlerror ("syntax error");
    }



  if (htmlerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (htmlchar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (htmlchar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (htmlssp == htmlss)
		   YYABORT;
		 htmldestruct ("Error: popping",
                             htmlstos[*htmlssp], htmlvsp);
	       }
        }
      else
	{
	  htmldestruct ("Error: discarding", htmltoken, &htmllval);
	  htmlchar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto htmlerrlab1;


/*---------------------------------------------------.
| htmlerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
htmlerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     htmlerrorlab therefore never appears in user code.  */
  if (0)
     goto htmlerrorlab;
#endif

htmlvsp -= htmllen;
  htmlssp -= htmllen;
  htmlstate = *htmlssp;
  goto htmlerrlab1;


/*-------------------------------------------------------------.
| htmlerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
htmlerrlab1:
  htmlerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      htmln = htmlpact[htmlstate];
      if (htmln != YYPACT_NINF)
	{
	  htmln += YYTERROR;
	  if (0 <= htmln && htmln <= YYLAST && htmlcheck[htmln] == YYTERROR)
	    {
	      htmln = htmltable[htmln];
	      if (0 < htmln)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (htmlssp == htmlss)
	YYABORT;


      htmldestruct ("Error: popping", htmlstos[htmlstate], htmlvsp);
      YYPOPSTACK;
      htmlstate = *htmlssp;
      YY_STACK_PRINT (htmlss, htmlssp);
    }

  if (htmln == YYFINAL)
    YYACCEPT;

  *++htmlvsp = htmllval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", htmlstos[htmln], htmlvsp, htmllsp);

  htmlstate = htmln;
  goto htmlnewstate;


/*-------------------------------------.
| htmlacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
htmlacceptlab:
  htmlresult = 0;
  goto htmlreturn;

/*-----------------------------------.
| htmlabortlab -- YYABORT comes here.  |
`-----------------------------------*/
htmlabortlab:
  htmldestruct ("Error: discarding lookahead",
              htmltoken, &htmllval);
  htmlchar = YYEMPTY;
  htmlresult = 1;
  goto htmlreturn;

#ifndef htmloverflow
/*----------------------------------------------.
| htmloverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
htmloverflowlab:
  htmlerror ("parser stack overflow");
  htmlresult = 2;
  /* Fall through.  */
#endif

htmlreturn:
#ifndef htmloverflow
  if (htmlss != htmlssa)
    YYSTACK_FREE (htmlss);
#endif
  return htmlresult;
}


#line 420 "../../lib/common/htmlparse.y"


htmllabel_t*
simpleHTML (char* txt)
{
  htmltxt_t*   tobj = mkText (txt);
  htmllabel_t* l = mkLabel(tobj,HTML_TEXT);
  return l;
}

/* parseHTML:
 * Return parsed label or NULL if failure.
 * Set warn to 0 on success; 1 for warning message; 2 if no expat.
 */
htmllabel_t*
parseHTML (char* txt, int* warn, int charset)
{
  unsigned char buf[SMALLBUF];
  agxbuf        str;
  htmllabel_t*  l;

  HTMLstate.tblstack = 0;
  HTMLstate.lbl = 0;
  HTMLstate.lines = dtopen(&strDisc, Dtqueue);
  agxbinit (&str, SMALLBUF, buf);
  HTMLstate.str = &str;
  
  if (initHTMLlexer (txt, &str, charset)) {/* failed: no libexpat - give up */
    *warn = 2;
    l = NULL;
  }
  else {
    htmlparse();
    *warn = clearHTMLlexer ();
    l = HTMLstate.lbl;
  }

  dtclose (HTMLstate.lines);
  HTMLstate.lines = NULL;
  agxbfree (&str);

  return l;
}


