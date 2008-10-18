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

/* All symbols defined below should begin with aag or YY, to avoid
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
   enum aagtokentype {
     T_graph = 258,
     T_node = 259,
     T_edge = 260,
     T_digraph = 261,
     T_subgraph = 262,
     T_strict = 263,
     T_edgeop = 264,
     T_list = 265,
     T_attr = 266,
     T_atom = 267,
     T_qatom = 268
   };
#endif
#define T_graph 258
#define T_node 259
#define T_edge 260
#define T_digraph 261
#define T_subgraph 262
#define T_strict 263
#define T_edgeop 264
#define T_list 265
#define T_attr 266
#define T_atom 267
#define T_qatom 268




/* Copy the first part of user declarations.  */
#line 17 "../../lib/agraph/grammar.y"


#include <stdio.h>  /* SAFE */
#include <aghdr.h>	/* SAFE */

#ifdef _WIN32
#define gettxt(a,b)	(b)
#endif

static char Key[] = "key";

typedef union s {					/* possible items in generic list */
		Agnode_t		*n;
		Agraph_t		*subg;
		Agedge_t		*e;
		Agsym_t			*asym;	/* bound attribute */
		char			*name;	/* unbound attribute */
		struct item_s	*list;	/* list-of-lists (for edgestmt) */
} val_t;

typedef struct item_s {		/* generic list */
	int				tag;	/* T_node, T_subgraph, T_edge, T_attr */
	val_t			u;		/* primary element */
	char			*str;	/* secondary value - port or attr value */
	struct item_s	*next;
} item;

typedef struct list_s {		/* maintain head and tail ptrs for fast append */
	item			*first;
	item			*last;
} list_t;

/* functions */
static void appendnode(char *name, char *port, char *sport);
static void attrstmt(int tkind, char *macroname);
static void startgraph(char *name, int directed, int strict);
static void bufferedges(void);
static void newedge(Agnode_t *t, char *tport, Agnode_t *h, char *hport, char *key);
static void edgerhs(Agnode_t *n, char *tport, item *hlist, char *key);
static void appendattr(char *name, char *value);
static void bindattrs(int kind);
static void applyattrs(void *obj);
static void endgraph(void);
static void endnode(void);
static void endedge(void);
static char* concat(char*, char*);
static char* concatPort(char*, char*);

static void opensubg(char *name);
static void closesubg(void);

/* global */
static Agraph_t *G;



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
#line 73 "../../lib/agraph/grammar.y"
typedef union YYSTYPE {
			int				i;
			char			*str;
			struct Agnode_s	*n;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 164 "y.tab.c"
# define aagstype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 176 "y.tab.c"

#if ! defined (aagoverflow) || YYERROR_VERBOSE

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
#endif /* ! defined (aagoverflow) || YYERROR_VERBOSE */


#if (! defined (aagoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union aagalloc
{
  short int aagss;
  YYSTYPE aagvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union aagalloc) - 1)

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
	  register YYSIZE_T aagi;		\
	  for (aagi = 0; aagi < (Count); aagi++)	\
	    (To)[aagi] = (From)[aagi];		\
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
	YYSIZE_T aagnewbytes;						\
	YYCOPY (&aagptr->Stack, Stack, aagsize);				\
	Stack = &aagptr->Stack;						\
	aagnewbytes = aagstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	aagptr += aagnewbytes / sizeof (*aagptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char aagsigned_char;
#else
   typedef short int aagsigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   59

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  34
/* YYNRULES -- Number of rules. */
#define YYNRULES  61
/* YYNRULES -- Number of states. */
#define YYNSTATES  79

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? aagtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char aagtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    23,    17,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    16,
       2,    19,     2,     2,    22,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    20,     2,    21,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    14,     2,    15,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char aagprhs[] =
{
       0,     0,     3,     6,     8,     9,    13,    17,    19,    20,
      22,    23,    25,    27,    29,    30,    33,    35,    37,    38,
      41,    44,    48,    50,    52,    53,    58,    59,    61,    65,
      67,    71,    77,    81,    83,    85,    87,    89,    92,    93,
      95,    96,   101,   104,   105,   108,   110,   112,   116,   119,
     121,   122,   126,   129,   131,   132,   134,   136,   137,   139,
     141,   143
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const aagsigned_char aagrhs[] =
{
      25,     0,    -1,    27,    26,    -1,     1,    -1,    -1,    14,
      31,    15,    -1,    29,    30,    28,    -1,    56,    -1,    -1,
       8,    -1,    -1,     3,    -1,     6,    -1,    32,    -1,    -1,
      32,    34,    -1,    34,    -1,    16,    -1,    -1,    41,    33,
      -1,    35,    33,    -1,    36,    37,    44,    -1,    39,    -1,
      52,    -1,    -1,     9,    38,    36,    37,    -1,    -1,    40,
      -1,    39,    17,    40,    -1,    56,    -1,    56,    18,    56,
      -1,    56,    18,    56,    18,    56,    -1,    42,    43,    45,
      -1,    51,    -1,     3,    -1,     4,    -1,     5,    -1,    56,
      19,    -1,    -1,    45,    -1,    -1,    44,    20,    46,    21,
      -1,    46,    47,    -1,    -1,    48,    55,    -1,    49,    -1,
      50,    -1,    56,    19,    56,    -1,    22,    56,    -1,    49,
      -1,    -1,    54,    53,    26,    -1,     7,    56,    -1,     7,
      -1,    -1,    16,    -1,    17,    -1,    -1,    12,    -1,    57,
      -1,    13,    -1,    57,    23,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char aagrline[] =
{
       0,    90,    90,    91,    92,    95,    97,   100,   100,   102,
     102,   104,   104,   106,   106,   108,   108,   110,   110,   112,
     113,   116,   120,   120,   122,   122,   123,   127,   127,   129,
     130,   131,   134,   135,   138,   139,   140,   143,   144,   147,
     147,   149,   151,   152,   154,   157,   157,   159,   162,   165,
     168,   168,   171,   172,   173,   176,   176,   176,   178,   179,
     182,   183
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const aagtname[] =
{
  "$end", "error", "$undefined", "T_graph", "T_node", "T_edge",
  "T_digraph", "T_subgraph", "T_strict", "T_edgeop", "T_list", "T_attr",
  "T_atom", "T_qatom", "'{'", "'}'", "';'", "','", "':'", "'='", "'['",
  "']'", "'@'", "'+'", "$accept", "graph", "body", "hdr", "optgraphname",
  "optstrict", "graphtype", "optstmtlist", "stmtlist", "optsemi", "stmt",
  "compound", "simple", "rcompound", "@1", "nodelist", "node", "attrstmt",
  "attrtype", "optmacroname", "optattr", "attrlist", "optattrdefs",
  "attrdefs", "attritem", "attrassignment", "attrmacro", "graphattrdefs",
  "subgraph", "@2", "optsubghdr", "optseparator", "atom", "qatom", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int aagtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   123,   125,    59,    44,    58,    61,
      91,    93,    64,    43
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char aagr1[] =
{
       0,    24,    25,    25,    25,    26,    27,    28,    28,    29,
      29,    30,    30,    31,    31,    32,    32,    33,    33,    34,
      34,    35,    36,    36,    38,    37,    37,    39,    39,    40,
      40,    40,    41,    41,    42,    42,    42,    43,    43,    44,
      44,    45,    46,    46,    47,    48,    48,    49,    50,    51,
      53,    52,    54,    54,    54,    55,    55,    55,    56,    56,
      57,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char aagr2[] =
{
       0,     2,     2,     1,     0,     3,     3,     1,     0,     1,
       0,     1,     1,     1,     0,     2,     1,     1,     0,     2,
       2,     3,     1,     1,     0,     4,     0,     1,     3,     1,
       3,     5,     3,     1,     1,     1,     1,     2,     0,     1,
       0,     4,     2,     0,     2,     1,     1,     3,     2,     1,
       0,     3,     2,     1,     0,     1,     1,     0,     1,     1,
       1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char aagdefact[] =
{
       0,     3,     9,     0,     0,     0,     1,    14,     2,    11,
      12,     8,    34,    35,    36,    53,    58,    60,     0,    13,
      16,    18,    26,    22,    27,    18,    38,    49,    33,    23,
      50,    29,    59,     6,     7,    52,     5,    15,    17,    20,
      24,    40,     0,    19,    40,     0,     0,     0,     0,     0,
      54,    21,    39,    28,    29,     0,    32,    37,    51,    30,
      47,    61,    26,    43,     0,    25,     0,    31,    41,     0,
      42,    57,    45,    46,     0,    48,    55,    56,    44
};

/* YYDEFGOTO[NTERM-NUM]. */
static const aagsigned_char aagdefgoto[] =
{
      -1,     3,     8,     4,    33,     5,    11,    18,    19,    39,
      20,    21,    22,    41,    50,    23,    24,    25,    26,    44,
      51,    52,    66,    70,    71,    27,    73,    28,    29,    46,
      30,    78,    31,    32
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -15
static const aagsigned_char aagpact[] =
{
      18,   -15,   -15,    20,     9,     3,   -15,    -2,   -15,   -15,
     -15,     1,   -15,   -15,   -15,     1,   -15,   -15,    10,    -2,
     -15,    19,    25,    21,   -15,    19,     1,   -15,   -15,   -15,
     -15,    11,    17,   -15,   -15,   -15,   -15,   -15,   -15,   -15,
     -15,   -15,     1,   -15,   -15,    22,     9,     1,     1,    29,
      15,    23,   -15,   -15,    26,    23,    27,   -15,   -15,    28,
     -15,   -15,    25,   -15,     1,   -15,    -5,   -15,   -15,     1,
     -15,    16,   -15,   -15,    30,   -15,   -15,   -15,   -15
};

/* YYPGOTO[NTERM-NUM].  */
static const aagsigned_char aagpgoto[] =
{
     -15,   -15,    -1,   -15,   -15,   -15,   -15,   -15,   -15,    31,
      32,   -15,     0,   -14,   -15,   -15,    12,   -15,   -15,   -15,
       8,    13,   -15,   -15,   -15,    -7,   -15,   -15,   -15,   -15,
     -15,   -15,   -11,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -55
static const aagsigned_char aagtable[] =
{
      34,    12,    13,    14,    35,    15,     9,    16,    17,    10,
      16,    17,   -54,    16,    17,    45,    68,    69,    -4,     1,
       6,   -10,    15,     7,   -10,    36,     2,    16,    17,    47,
      48,    54,    76,    77,    40,    38,    59,    60,    42,    54,
      49,    57,    61,    63,    47,    58,    64,   -39,    65,    48,
      62,    37,    55,    67,    53,    74,    43,    56,    75,    72
};

static const unsigned char aagcheck[] =
{
      11,     3,     4,     5,    15,     7,     3,    12,    13,     6,
      12,    13,    14,    12,    13,    26,    21,    22,     0,     1,
       0,     3,     7,    14,     6,    15,     8,    12,    13,    18,
      19,    42,    16,    17,     9,    16,    47,    48,    17,    50,
      23,    19,    13,    20,    18,    46,    18,    20,    62,    19,
      50,    19,    44,    64,    42,    66,    25,    44,    69,    66
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char aagstos[] =
{
       0,     1,     8,    25,    27,    29,     0,    14,    26,     3,
       6,    30,     3,     4,     5,     7,    12,    13,    31,    32,
      34,    35,    36,    39,    40,    41,    42,    49,    51,    52,
      54,    56,    57,    28,    56,    56,    15,    34,    16,    33,
       9,    37,    17,    33,    43,    56,    53,    18,    19,    23,
      38,    44,    45,    40,    56,    44,    45,    19,    26,    56,
      56,    13,    36,    20,    18,    37,    46,    56,    21,    22,
      47,    48,    49,    50,    56,    56,    16,    17,    55
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

#define aagerrok		(aagerrstatus = 0)
#define aagclearin	(aagchar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto aagacceptlab
#define YYABORT		goto aagabortlab
#define YYERROR		goto aagerrorlab


/* Like YYERROR except do call aagerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto aagerrlab

#define YYRECOVERING()  (!!aagerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (aagchar == YYEMPTY && aaglen == 1)				\
    {								\
      aagchar = (Token);						\
      aaglval = (Value);						\
      aagtoken = YYTRANSLATE (aagchar);				\
      YYPOPSTACK;						\
      goto aagbackup;						\
    }								\
  else								\
    { 								\
      aagerror ("syntax error: cannot back up");\
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


/* YYLEX -- calling `aaglex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX aaglex (YYLEX_PARAM)
#else
# define YYLEX aaglex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (aagdebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (aagdebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      aagsymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| aag_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
aag_stack_print (short int *bottom, short int *top)
#else
static void
aag_stack_print (bottom, top)
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
  if (aagdebug)							\
    aag_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
aag_reduce_print (int aagrule)
#else
static void
aag_reduce_print (aagrule)
    int aagrule;
#endif
{
  int aagi;
  unsigned int aaglno = aagrline[aagrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             aagrule - 1, aaglno);
  /* Print the symbols being reduced, and their result.  */
  for (aagi = aagprhs[aagrule]; 0 <= aagrhs[aagi]; aagi++)
    YYFPRINTF (stderr, "%s ", aagtname [aagrhs[aagi]]);
  YYFPRINTF (stderr, "-> %s\n", aagtname [aagr1[aagrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (aagdebug)				\
    aag_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int aagdebug;
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

# ifndef aagstrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define aagstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
aagstrlen (const char *aagstr)
#   else
aagstrlen (aagstr)
     const char *aagstr;
#   endif
{
  register const char *aags = aagstr;

  while (*aags++ != '\0')
    continue;

  return aags - aagstr - 1;
}
#  endif
# endif

# ifndef aagstpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define aagstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
aagstpcpy (char *aagdest, const char *aagsrc)
#   else
aagstpcpy (aagdest, aagsrc)
     char *aagdest;
     const char *aagsrc;
#   endif
{
  register char *aagd = aagdest;
  register const char *aags = aagsrc;

  while ((*aagd++ = *aags++) != '\0')
    continue;

  return aagd - 1;
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
aagsymprint (FILE *aagoutput, int aagtype, YYSTYPE *aagvaluep)
#else
static void
aagsymprint (aagoutput, aagtype, aagvaluep)
    FILE *aagoutput;
    int aagtype;
    YYSTYPE *aagvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) aagvaluep;

  if (aagtype < YYNTOKENS)
    YYFPRINTF (aagoutput, "token %s (", aagtname[aagtype]);
  else
    YYFPRINTF (aagoutput, "nterm %s (", aagtname[aagtype]);


# ifdef YYPRINT
  if (aagtype < YYNTOKENS)
    YYPRINT (aagoutput, aagtoknum[aagtype], *aagvaluep);
# endif
  switch (aagtype)
    {
      default:
        break;
    }
  YYFPRINTF (aagoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
aagdestruct (const char *aagmsg, int aagtype, YYSTYPE *aagvaluep)
#else
static void
aagdestruct (aagmsg, aagtype, aagvaluep)
    const char *aagmsg;
    int aagtype;
    YYSTYPE *aagvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) aagvaluep;

  if (!aagmsg)
    aagmsg = "Deleting";
  YY_SYMBOL_PRINT (aagmsg, aagtype, aagvaluep, aaglocationp);

  switch (aagtype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int aagparse (void *YYPARSE_PARAM);
# else
int aagparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int aagparse (void);
#else
int aagparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int aagchar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE aaglval;

/* Number of syntax errors so far.  */
int aagnerrs;



/*----------.
| aagparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int aagparse (void *YYPARSE_PARAM)
# else
int aagparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
aagparse (void)
#else
int
aagparse ()

#endif
#endif
{
  
  register int aagstate;
  register int aagn;
  int aagresult;
  /* Number of tokens to shift before error messages enabled.  */
  int aagerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int aagtoken = 0;

  /* Three stacks and their tools:
     `aagss': related to states,
     `aagvs': related to semantic values,
     `aagls': related to locations.

     Refer to the stacks thru separate pointers, to allow aagoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int aagssa[YYINITDEPTH];
  short int *aagss = aagssa;
  register short int *aagssp;

  /* The semantic value stack.  */
  YYSTYPE aagvsa[YYINITDEPTH];
  YYSTYPE *aagvs = aagvsa;
  register YYSTYPE *aagvsp;



#define YYPOPSTACK   (aagvsp--, aagssp--)

  YYSIZE_T aagstacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE aagval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int aaglen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  aagstate = 0;
  aagerrstatus = 0;
  aagnerrs = 0;
  aagchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  aagssp = aagss;
  aagvsp = aagvs;


  aagvsp[0] = aaglval;

  goto aagsetstate;

/*------------------------------------------------------------.
| aagnewstate -- Push a new state, which is found in aagstate.  |
`------------------------------------------------------------*/
 aagnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  aagssp++;

 aagsetstate:
  *aagssp = aagstate;

  if (aagss + aagstacksize - 1 <= aagssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T aagsize = aagssp - aagss + 1;

#ifdef aagoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *aagvs1 = aagvs;
	short int *aagss1 = aagss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if aagoverflow is a macro.  */
	aagoverflow ("parser stack overflow",
		    &aagss1, aagsize * sizeof (*aagssp),
		    &aagvs1, aagsize * sizeof (*aagvsp),

		    &aagstacksize);

	aagss = aagss1;
	aagvs = aagvs1;
      }
#else /* no aagoverflow */
# ifndef YYSTACK_RELOCATE
      goto aagoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= aagstacksize)
	goto aagoverflowlab;
      aagstacksize *= 2;
      if (YYMAXDEPTH < aagstacksize)
	aagstacksize = YYMAXDEPTH;

      {
	short int *aagss1 = aagss;
	union aagalloc *aagptr =
	  (union aagalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (aagstacksize));
	if (! aagptr)
	  goto aagoverflowlab;
	YYSTACK_RELOCATE (aagss);
	YYSTACK_RELOCATE (aagvs);

#  undef YYSTACK_RELOCATE
	if (aagss1 != aagssa)
	  YYSTACK_FREE (aagss1);
      }
# endif
#endif /* no aagoverflow */

      aagssp = aagss + aagsize - 1;
      aagvsp = aagvs + aagsize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) aagstacksize));

      if (aagss + aagstacksize - 1 <= aagssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", aagstate));

  goto aagbackup;

/*-----------.
| aagbackup.  |
`-----------*/
aagbackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* aagresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  aagn = aagpact[aagstate];
  if (aagn == YYPACT_NINF)
    goto aagdefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (aagchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      aagchar = YYLEX;
    }

  if (aagchar <= YYEOF)
    {
      aagchar = aagtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      aagtoken = YYTRANSLATE (aagchar);
      YY_SYMBOL_PRINT ("Next token is", aagtoken, &aaglval, &aaglloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  aagn += aagtoken;
  if (aagn < 0 || YYLAST < aagn || aagcheck[aagn] != aagtoken)
    goto aagdefault;
  aagn = aagtable[aagn];
  if (aagn <= 0)
    {
      if (aagn == 0 || aagn == YYTABLE_NINF)
	goto aagerrlab;
      aagn = -aagn;
      goto aagreduce;
    }

  if (aagn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", aagtoken, &aaglval, &aaglloc);

  /* Discard the token being shifted unless it is eof.  */
  if (aagchar != YYEOF)
    aagchar = YYEMPTY;

  *++aagvsp = aaglval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (aagerrstatus)
    aagerrstatus--;

  aagstate = aagn;
  goto aagnewstate;


/*-----------------------------------------------------------.
| aagdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
aagdefault:
  aagn = aagdefact[aagstate];
  if (aagn == 0)
    goto aagerrlab;
  goto aagreduce;


/*-----------------------------.
| aagreduce -- Do a reduction.  |
`-----------------------------*/
aagreduce:
  /* aagn is the number of a rule to reduce with.  */
  aaglen = aagr2[aagn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  aagval = aagvsp[1-aaglen];


  YY_REDUCE_PRINT (aagn);
  switch (aagn)
    {
        case 2:
#line 90 "../../lib/agraph/grammar.y"
    {endgraph();}
    break;

  case 3:
#line 91 "../../lib/agraph/grammar.y"
    {if (G) {agclose(G); G = Ag_G_global = NIL(Agraph_t*);}}
    break;

  case 6:
#line 97 "../../lib/agraph/grammar.y"
    {startgraph((aagvsp[0].str),(aagvsp[-1].i),(aagvsp[-2].i));}
    break;

  case 7:
#line 100 "../../lib/agraph/grammar.y"
    {(aagval.str)=(aagvsp[0].str);}
    break;

  case 8:
#line 100 "../../lib/agraph/grammar.y"
    {(aagval.str)=0;}
    break;

  case 9:
#line 102 "../../lib/agraph/grammar.y"
    {(aagval.i)=1;}
    break;

  case 10:
#line 102 "../../lib/agraph/grammar.y"
    {(aagval.i)=0;}
    break;

  case 11:
#line 104 "../../lib/agraph/grammar.y"
    {(aagval.i) = 0;}
    break;

  case 12:
#line 104 "../../lib/agraph/grammar.y"
    {(aagval.i) = 1;}
    break;

  case 21:
#line 117 "../../lib/agraph/grammar.y"
    {if ((aagvsp[-1].i)) endedge(); else endnode();}
    break;

  case 24:
#line 122 "../../lib/agraph/grammar.y"
    {bufferedges();}
    break;

  case 25:
#line 122 "../../lib/agraph/grammar.y"
    {(aagval.i) = 1;}
    break;

  case 26:
#line 123 "../../lib/agraph/grammar.y"
    {(aagval.i) = 0;}
    break;

  case 29:
#line 129 "../../lib/agraph/grammar.y"
    {appendnode((aagvsp[0].str),NIL(char*),NIL(char*));}
    break;

  case 30:
#line 130 "../../lib/agraph/grammar.y"
    {appendnode((aagvsp[-2].str),(aagvsp[0].str),NIL(char*));}
    break;

  case 31:
#line 131 "../../lib/agraph/grammar.y"
    {appendnode((aagvsp[-4].str),(aagvsp[-2].str),(aagvsp[0].str));}
    break;

  case 32:
#line 134 "../../lib/agraph/grammar.y"
    {attrstmt((aagvsp[-2].i),(aagvsp[-1].str));}
    break;

  case 33:
#line 135 "../../lib/agraph/grammar.y"
    {attrstmt(T_graph,NIL(char*));}
    break;

  case 34:
#line 138 "../../lib/agraph/grammar.y"
    {(aagval.i) = T_graph;}
    break;

  case 35:
#line 139 "../../lib/agraph/grammar.y"
    {(aagval.i) = T_node;}
    break;

  case 36:
#line 140 "../../lib/agraph/grammar.y"
    {(aagval.i) = T_edge;}
    break;

  case 37:
#line 143 "../../lib/agraph/grammar.y"
    {(aagval.str) = (aagvsp[-1].str);}
    break;

  case 38:
#line 144 "../../lib/agraph/grammar.y"
    {(aagval.str) = NIL(char*); }
    break;

  case 47:
#line 159 "../../lib/agraph/grammar.y"
    {appendattr((aagvsp[-2].str),(aagvsp[0].str));}
    break;

  case 48:
#line 162 "../../lib/agraph/grammar.y"
    {appendattr((aagvsp[0].str),NIL(char*));}
    break;

  case 50:
#line 168 "../../lib/agraph/grammar.y"
    {opensubg((aagvsp[0].str));}
    break;

  case 51:
#line 168 "../../lib/agraph/grammar.y"
    {closesubg();}
    break;

  case 52:
#line 171 "../../lib/agraph/grammar.y"
    {(aagval.str)=(aagvsp[0].str);}
    break;

  case 53:
#line 172 "../../lib/agraph/grammar.y"
    {(aagval.str)=NIL(char*);}
    break;

  case 54:
#line 173 "../../lib/agraph/grammar.y"
    {(aagval.str)=NIL(char*);}
    break;

  case 58:
#line 178 "../../lib/agraph/grammar.y"
    {(aagval.str) = (aagvsp[0].str);}
    break;

  case 59:
#line 179 "../../lib/agraph/grammar.y"
    {(aagval.str) = (aagvsp[0].str);}
    break;

  case 60:
#line 182 "../../lib/agraph/grammar.y"
    {(aagval.str) = (aagvsp[0].str);}
    break;

  case 61:
#line 183 "../../lib/agraph/grammar.y"
    {(aagval.str) = concat((aagvsp[-2].str),(aagvsp[0].str));}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 1325 "y.tab.c"

  aagvsp -= aaglen;
  aagssp -= aaglen;


  YY_STACK_PRINT (aagss, aagssp);

  *++aagvsp = aagval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  aagn = aagr1[aagn];

  aagstate = aagpgoto[aagn - YYNTOKENS] + *aagssp;
  if (0 <= aagstate && aagstate <= YYLAST && aagcheck[aagstate] == *aagssp)
    aagstate = aagtable[aagstate];
  else
    aagstate = aagdefgoto[aagn - YYNTOKENS];

  goto aagnewstate;


/*------------------------------------.
| aagerrlab -- here on detecting error |
`------------------------------------*/
aagerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!aagerrstatus)
    {
      ++aagnerrs;
#if YYERROR_VERBOSE
      aagn = aagpact[aagstate];

      if (YYPACT_NINF < aagn && aagn < YYLAST)
	{
	  YYSIZE_T aagsize = 0;
	  int aagtype = YYTRANSLATE (aagchar);
	  const char* aagprefix;
	  char *aagmsg;
	  int aagx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int aagxbegin = aagn < 0 ? -aagn : 0;

	  /* Stay within bounds of both aagcheck and aagtname.  */
	  int aagchecklim = YYLAST - aagn;
	  int aagxend = aagchecklim < YYNTOKENS ? aagchecklim : YYNTOKENS;
	  int aagcount = 0;

	  aagprefix = ", expecting ";
	  for (aagx = aagxbegin; aagx < aagxend; ++aagx)
	    if (aagcheck[aagx + aagn] == aagx && aagx != YYTERROR)
	      {
		aagsize += aagstrlen (aagprefix) + aagstrlen (aagtname [aagx]);
		aagcount += 1;
		if (aagcount == 5)
		  {
		    aagsize = 0;
		    break;
		  }
	      }
	  aagsize += (sizeof ("syntax error, unexpected ")
		     + aagstrlen (aagtname[aagtype]));
	  aagmsg = (char *) YYSTACK_ALLOC (aagsize);
	  if (aagmsg != 0)
	    {
	      char *aagp = aagstpcpy (aagmsg, "syntax error, unexpected ");
	      aagp = aagstpcpy (aagp, aagtname[aagtype]);

	      if (aagcount < 5)
		{
		  aagprefix = ", expecting ";
		  for (aagx = aagxbegin; aagx < aagxend; ++aagx)
		    if (aagcheck[aagx + aagn] == aagx && aagx != YYTERROR)
		      {
			aagp = aagstpcpy (aagp, aagprefix);
			aagp = aagstpcpy (aagp, aagtname[aagx]);
			aagprefix = " or ";
		      }
		}
	      aagerror (aagmsg);
	      YYSTACK_FREE (aagmsg);
	    }
	  else
	    aagerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	aagerror ("syntax error");
    }



  if (aagerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (aagchar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (aagchar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (aagssp == aagss)
		   YYABORT;
		 aagdestruct ("Error: popping",
                             aagstos[*aagssp], aagvsp);
	       }
        }
      else
	{
	  aagdestruct ("Error: discarding", aagtoken, &aaglval);
	  aagchar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto aagerrlab1;


/*---------------------------------------------------.
| aagerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
aagerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     aagerrorlab therefore never appears in user code.  */
  if (0)
     goto aagerrorlab;
#endif

aagvsp -= aaglen;
  aagssp -= aaglen;
  aagstate = *aagssp;
  goto aagerrlab1;


/*-------------------------------------------------------------.
| aagerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
aagerrlab1:
  aagerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      aagn = aagpact[aagstate];
      if (aagn != YYPACT_NINF)
	{
	  aagn += YYTERROR;
	  if (0 <= aagn && aagn <= YYLAST && aagcheck[aagn] == YYTERROR)
	    {
	      aagn = aagtable[aagn];
	      if (0 < aagn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (aagssp == aagss)
	YYABORT;


      aagdestruct ("Error: popping", aagstos[aagstate], aagvsp);
      YYPOPSTACK;
      aagstate = *aagssp;
      YY_STACK_PRINT (aagss, aagssp);
    }

  if (aagn == YYFINAL)
    YYACCEPT;

  *++aagvsp = aaglval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", aagstos[aagn], aagvsp, aaglsp);

  aagstate = aagn;
  goto aagnewstate;


/*-------------------------------------.
| aagacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
aagacceptlab:
  aagresult = 0;
  goto aagreturn;

/*-----------------------------------.
| aagabortlab -- YYABORT comes here.  |
`-----------------------------------*/
aagabortlab:
  aagdestruct ("Error: discarding lookahead",
              aagtoken, &aaglval);
  aagchar = YYEMPTY;
  aagresult = 1;
  goto aagreturn;

#ifndef aagoverflow
/*----------------------------------------------.
| aagoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
aagoverflowlab:
  aagerror ("parser stack overflow");
  aagresult = 2;
  /* Fall through.  */
#endif

aagreturn:
#ifndef aagoverflow
  if (aagss != aagssa)
    YYSTACK_FREE (aagss);
#endif
  return aagresult;
}


#line 185 "../../lib/agraph/grammar.y"


#define NILitem  NIL(item*)

/* globals */
static	Agraph_t	*Subgraph;	/* most recent subgraph that was opened */
static	Agdisc_t	*Disc;		/* discipline passed to agread or agconcat */
static	list_t	Nodelist,Edgelist,Attrlist;

static item *newitem(int tag, void *p0, char *p1)
{
	item	*rv = agalloc(G,sizeof(item));
	rv->tag = tag; rv->u.name = (char*)p0; rv->str = p1;
	return rv;
}

static item *cons_node(Agnode_t *n, char *port)
	{ return newitem(T_node,n,port); }

static item *cons_attr(char *name, char *value)
	{ return newitem(T_atom,name,value); }

static item *cons_list(item *list)
	{ return newitem(T_list,list,NIL(char*)); }

static item *cons_subg(Agraph_t *subg)
	{ return newitem(T_subgraph,subg,NIL(char*)); }

#ifdef NOTDEF
static item *cons_edge(Agedge_t *e)
	{ return newitem(T_edge,e,NIL(char*)); }
#endif

static void delete_items(item *ilist)
{
	item	*p,*pn;

	for (p = ilist; p; p = pn) {
		pn = p->next;
		switch(p->tag) {
			case T_list: delete_items(p->u.list); break;
			case T_atom: case T_attr: agstrfree(G,p->str); break;
		}
		agfree(G,p);
	}
}

static void deletelist(list_t *list)
{
	delete_items(list->first);
	list->first = list->last = NILitem;
}

#ifdef NOTDEF
static void listins(list_t *list, item *v)
{
	v->next = list->first;
	list->first = v;
	if (list->last == NILitem) list->last = v;
}
#endif

static void listapp(list_t *list, item *v)
{
	if (list->last) list->last->next = v;
	list->last = v;
	if (list->first == NILitem) list->first = v;
}


/* attrs */
static void appendattr(char *name, char *value)
{
	item		*v;

	assert(value != NIL(char*));
	v = cons_attr(name,value);
	listapp(&Attrlist,v);
}

static void bindattrs(int kind)
{
	item		*aptr;
	char		*name;

	for (aptr = Attrlist.first; aptr; aptr = aptr->next) {
		assert(aptr->tag == T_atom);	/* signifies unbound attr */
		name = aptr->u.name;
		if ((kind == AGEDGE) && streq(name,Key)) continue;
		if ((aptr->u.asym = agattr(G,kind,name,NIL(char*))) == NILsym)
			aptr->u.asym = agattr(G,kind,name,"");
		aptr->tag = T_attr;				/* signifies bound attr */
		agstrfree(G,name);
	}
}

static void applyattrs(void *obj)
{
	item		*aptr;

	for (aptr = Attrlist.first; aptr; aptr = aptr->next) {
		if (aptr->tag == T_attr) {
			if (aptr->u.asym) {
				agxset(obj,aptr->u.asym,aptr->str);
			}
		}
		else {
			assert(AGTYPE(obj) == AGEDGE);
			assert(aptr->tag == T_atom);
			assert(streq(aptr->u.name,Key));
		}
	}
}

static void nomacros(void)
{
	agerror(AGERROR_UNIMPL,"attribute macros");
}

static void attrstmt(int tkind, char *macroname)
{
	item			*aptr;
	int				kind;

		/* creating a macro def */
	if (macroname) nomacros();
		/* invoking a macro def */
	for (aptr = Attrlist.first; aptr; aptr = aptr->next)
		if (aptr->str == NIL(char*)) nomacros();

	switch(tkind) {
		case T_graph: kind = AGRAPH; break;
		case T_node: kind = AGNODE; break;
		case T_edge: kind = AGEDGE; break;
		default : abort();
	}
	bindattrs(kind);	/* set up defaults for new attributes */
	for (aptr = Attrlist.first; aptr; aptr = aptr->next)
		agattr(G,kind,aptr->u.asym->name,aptr->str);
	deletelist(&Attrlist);
}

/* nodes */

static void appendnode(char *name, char *port, char *sport)
{
	item		*elt;

	if (sport) {
		port = concatPort (port, sport);
	}
	elt = cons_node(agnode(G,name,TRUE),port);
	listapp(&Nodelist,elt);
	agstrfree(G,name);
}

	/* apply current optional attrs to Nodelist and clean up lists */
static void endnode()
{
	item	*ptr;

	bindattrs(AGNODE);
	for (ptr = Nodelist.first; ptr; ptr = ptr->next)
		applyattrs(ptr->u.n);
	deletelist(&Nodelist);
	deletelist(&Attrlist);
}

/* edges - store up node/subg lists until optional edge key can be seen */

static void bufferedges()
{
	item	*v;

	if (Nodelist.first) {
		v = cons_list(Nodelist.first);
		Nodelist.first = Nodelist.last = NILitem;
	}
	else {v = cons_subg(Subgraph); Subgraph = NIL(Agraph_t*);}
	listapp(&Edgelist,v);
}

static void endedge(void)
{
	char			*key;
	item			*aptr,*tptr,*p;

	Agnode_t		*t;
	Agraph_t		*subg;

	bufferedges();	/* pick up the terminal nodelist or subg */
	bindattrs(AGEDGE);

	/* look for "key" pseudo-attribute */
	key = NIL(char*);
	for (aptr = Attrlist.first; aptr; aptr = aptr->next) {
		if ((aptr->tag == T_atom) && streq(aptr->u.name,Key))
			key = aptr->str;
	}

	/* can make edges with node lists or subgraphs */
	for (p = Edgelist.first; p->next; p = p->next) {
		if (p->tag == T_subgraph) {
			subg = p->u.subg;
			for (t = agfstnode(subg); t; t = agnxtnode(t))
				edgerhs(agsubnode(G,t,FALSE),NIL(char*),p->next,key);
		}
		else {
			for (tptr = p->u.list; tptr; tptr = tptr->next)
				edgerhs(tptr->u.n,tptr->str,p->next,key);
		}
	}
	deletelist(&Edgelist);
	deletelist(&Attrlist);
}

/* concat:
 */
static char*
concat (char* s1, char* s2)
{
  char*  s;
  char   buf[BUFSIZ];
  char*  sym;
  int    len = strlen(s1) + strlen(s2) + 1;

  if (len <= BUFSIZ) sym = buf;
  else sym = (char*)malloc(len);
  strcpy(sym,s1);
  strcat(sym,s2);
  s = agstrdup (G,sym);
  agstrfree (G,s1);
  agstrfree (G,s2);
  if (sym != buf) free (sym);
  return s;
}

/* concatPort:
 */
static char*
concatPort (char* s1, char* s2)
{
  char*  s;
  char   buf[BUFSIZ];
  char*  sym;
  int    len = strlen(s1) + strlen(s2) + 2;  /* one more for ':' */

  if (len <= BUFSIZ) sym = buf;
  else sym = (char*)malloc(len);
  sprintf (sym, "%s:%s", s1, s2);
  s = agstrdup (G,sym);
  agstrfree (G,s1);
  agstrfree (G,s2);
  if (sym != buf) free (sym);
  return s;
}


static void edgerhs(Agnode_t *tail, char *tport, item *hlist, char *key)
{
	Agnode_t		*head;
	Agraph_t		*subg;
	item			*hptr;

	if (hlist->tag == T_subgraph) {
		subg = hlist->u.subg;
		for (head = agfstnode(subg); head; head = agnxtnode(head))
			newedge(tail,tport,agsubnode(G,head,FALSE),NIL(char*),key);
	}
	else {
		for (hptr = hlist->u.list; hptr; hptr = hptr->next)
			newedge(tail,tport,agsubnode(G,hptr->u.n,FALSE),hptr->str,key);
	}
}

static void mkport(Agedge_t *e, char *name, char *val)
{
	Agsym_t *attr;
	if (val) {
		if ((attr = agattr(G,AGEDGE,name,NIL(char*))) == NILsym)
			attr = agattr(G,AGEDGE,name,"");
		agxset(e,attr,val);
	}
}

static void newedge(Agnode_t *t, char *tport, Agnode_t *h, char *hport, char *key)
{
	Agedge_t 	*e;

	e = agedge(t,h,key,TRUE);
	if (e) {		/* can fail if graph is strict and t==h */
		char    *tp = tport;
		char    *hp = hport;
		if ((agtail(e) != aghead(e)) && (aghead(e) == t)) {
			/* could happen with an undirected edge */
			char    *temp;
			temp = tp; tp = hp; hp = temp;
		}
		mkport(e,TAIL_ID,tp);
		mkport(e,HEAD_ID,hp);
		applyattrs(e);
	}
}

/* graphs and subgraphs */


static void startgraph(char *name, int directed, int strict)
{
	static Agdesc_t	req;	/* get rid of warnings */

	if (G == NILgraph) {
		req.directed = directed;
		req.strict = strict;
		req.flatlock = FALSE;
		req.maingraph = TRUE;
		Ag_G_global = G = agopen(name,req,Disc);
	}
	else {
		Ag_G_global = G;
	}
	agstrfree(NIL(Agraph_t*),name);
}

static void endgraph()
{
	aglexeof();
	aginternalmapclearlocalnames(G);
}

static void opensubg(char *name)
{
	G = agsubg(G,name,TRUE);
	agstrfree(G,name);
}

static void closesubg()
{
	Subgraph = G;
	if ((G = agparent(G)) == NIL(Agraph_t*))
		aagerror("libgraph: parser lost root graph\n");
}

extern void *aagin;
Agraph_t *agconcat(Agraph_t *g, void *chan, Agdisc_t *disc)
{
	aagin = chan;
	G = g;
	Ag_G_global = NILgraph;
	Disc = (disc? disc :  &AgDefaultDisc);
	aglexinit(Disc, chan);
	aagparse();
	return Ag_G_global;
}

Agraph_t *agread(void *fp, Agdisc_t *disc) {return agconcat(NILgraph,fp,disc); }

