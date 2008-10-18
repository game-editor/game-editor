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

/* All symbols defined below should begin with ag or YY, to avoid
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
   enum agtokentype {
     T_graph = 258,
     T_digraph = 259,
     T_strict = 260,
     T_node = 261,
     T_edge = 262,
     T_edgeop = 263,
     T_symbol = 264,
     T_qsymbol = 265,
     T_subgraph = 266
   };
#endif
#define T_graph 258
#define T_digraph 259
#define T_strict 260
#define T_node 261
#define T_edge 262
#define T_edgeop 263
#define T_symbol 264
#define T_qsymbol 265
#define T_subgraph 266




/* Copy the first part of user declarations.  */
#line 17 "../../lib/graph/parser.y"


#include	"libgraph.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

static char		*Port;
static char		In_decl,In_edge_stmt;
static int		Current_class,Agraph_type;
static Agsym_t		*headsubsym;
static Agsym_t		*tailsubsym;
static Agraph_t		*G;
static Agnode_t		*N;
static Agedge_t		*E;
static objstack_t	*SP;
static Agraph_t		*Gstack[32];
static int			GSP;

static void push_subg(Agraph_t *g)
{
	G = Gstack[GSP++] = g;
}

static Agraph_t *pop_subg(void)
{
	Agraph_t		*g;
	if (GSP == 0) {
		agerr (AGERR, "Gstack underflow in graph parser\n"); exit(1);
	}
	g = Gstack[--GSP];					/* graph being popped off */
	if (GSP > 0) G = Gstack[GSP - 1];	/* current graph */
	else G = 0;
	return g;
}

static objport_t pop_gobj(void)
{
	objport_t	rv;
	rv.obj = pop_subg();
	rv.port = NULL;
	return rv;
}

static void anonname(char* buf)
{
	static int		anon_id = 0;

	sprintf(buf,"_anonymous_%d",anon_id++);
}

static void begin_graph(char *name)
{
	Agraph_t		*g;
	char			buf[SMALLBUF];

	if (!name) {
		anonname(buf);
		name = buf;
    }
	g = AG.parsed_g = agopen(name,Agraph_type);
	Current_class = TAG_GRAPH;
	headsubsym = tailsubsym = NULL;
	push_subg(g);
	In_decl = TRUE;
}

static void end_graph(void)
{
	pop_subg();
}

static Agnode_t *bind_node(char *name)
{
	Agnode_t	*n = agnode(G,name);
	In_decl = FALSE;
	return n;
}

static void anonsubg(void)
{
	char			buf[SMALLBUF];
	Agraph_t			*subg;

	In_decl = FALSE;
	anonname(buf);
	subg = agsubg(G,buf);
	push_subg(subg);
}

#if 0 /* NOT USED */
static int isanonsubg(Agraph_t *g)
{
	return (strncmp("_anonymous_",g->name,11) == 0);
}
#endif

static void begin_edgestmt(objport_t objp)
{
	struct objstack_t	*new_sp;

	new_sp = NEW(objstack_t);
	new_sp->link = SP;
	SP = new_sp;
	SP->list = SP->last = NEW(objlist_t);
	SP->list->data  = objp;
	SP->list->link = NULL;
	SP->in_edge_stmt = In_edge_stmt;
	SP->subg = G;
	agpushproto(G);
	In_edge_stmt = TRUE;
}

static void mid_edgestmt(objport_t objp)
{
	SP->last->link = NEW(objlist_t);
	SP->last = SP->last->link;
	SP->last->data = objp;
	SP->last->link = NULL;
}

static void end_edgestmt(void)
{
	objstack_t	*old_SP;
	objlist_t	*tailptr,*headptr,*freeptr;
	Agraph_t		*t_graph,*h_graph;
	Agnode_t	*t_node,*h_node,*t_first,*h_first;
	Agedge_t	*e;
	char		*tport,*hport;

	for (tailptr = SP->list; tailptr->link; tailptr = tailptr->link) {
		headptr = tailptr->link;
		tport = tailptr->data.port;
		hport = headptr->data.port;
		if (TAG_OF(tailptr->data.obj) == TAG_NODE) {
			t_graph = NULL;
			t_first = (Agnode_t*)(tailptr->data.obj);
		}
		else {
			t_graph = (Agraph_t*)(tailptr->data.obj);
			t_first = agfstnode(t_graph);
		}
		if (TAG_OF(headptr->data.obj) == TAG_NODE) {
			h_graph = NULL;
			h_first = (Agnode_t*)(headptr->data.obj);
		}
		else {
			h_graph = (Agraph_t*)(headptr->data.obj);
			h_first = agfstnode(h_graph);
		}

		for (t_node = t_first; t_node; t_node = t_graph ?
		  agnxtnode(t_graph,t_node) : NULL) {
			for (h_node = h_first; h_node; h_node = h_graph ?
			  agnxtnode(h_graph,h_node) : NULL ) {
				e = agedge(G,t_node,h_node);
				if (e) {
					char	*tp = tport;
					char 	*hp = hport;
					if ((e->tail != e->head) && (e->head == t_node)) {
						/* could happen with an undirected edge */
						char 	*temp;
						temp = tp; tp = hp; hp = temp;
					}
					if (tp && tp[0]) {
						agxset(e,TAILX,tp);
						agstrfree(tp); 
					}
					if (hp && hp[0]) {
						agxset(e,HEADX,hp);
						agstrfree(hp); 
					}
				}
			}
		}
	}
	tailptr = SP->list; 
	while (tailptr) {
		freeptr = tailptr;
		tailptr = tailptr->link;
		if (TAG_OF(freeptr->data.obj) == TAG_NODE)
		free(freeptr);
	}
	if (G != SP->subg) abort();
	agpopproto(G);
	In_edge_stmt = SP->in_edge_stmt;
	old_SP = SP;
	SP = SP->link;
	In_decl = FALSE;
	free(old_SP);
	Current_class = TAG_GRAPH;
}

#if 0 /* NOT USED */
static Agraph_t *parent_of(Agraph_t *g)
{
	Agraph_t		*rv;
	rv = agusergraph(agfstin(g->meta_node->graph,g->meta_node)->tail);
	return rv;
}
#endif

static void attr_set(char *name, char *value)
{
	Agsym_t		*ap = NULL;
	char		*defval = "";

	if (In_decl && (G->root == G)) defval = value;
	switch (Current_class) {
		case TAG_NODE:
			ap = agfindattr(G->proto->n,name);
			if (ap == NULL)
				ap = agnodeattr(AG.parsed_g,name,defval);
            else if (ap->fixed && In_decl)
              return;
			agxset(N,ap->index,value);
			break;
		case TAG_EDGE:
			ap = agfindattr(G->proto->e,name);
			if (ap == NULL)
				ap = agedgeattr(AG.parsed_g,name,defval);
            else if (ap->fixed && In_decl)
              return;
			agxset(E,ap->index,value);
			break;
		case 0:		/* default */
		case TAG_GRAPH:
			ap = agfindattr(G,name);
			if (ap == NULL) 
				ap = agraphattr(AG.parsed_g,name,defval);
            else if (ap->fixed && In_decl)
              return;
			agxset(G,ap->index,value);
			break;
	}
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
  s = agstrdup (sym);
  if (sym != buf) free (sym);
  return s;
}

/* concat3:
 */
static char*
concat3 (char* s1, char* s2, char*s3)
{
  char*  s;
  char   buf[BUFSIZ];
  char*  sym;
  int    len = strlen(s1) + strlen(s2) + strlen(s3) + 1;

  if (len <= BUFSIZ) sym = buf;
  else sym = (char*)malloc(len);
  strcpy(sym,s1);
  strcat(sym,s2);
  strcat(sym,s3);
  s = agstrdup (sym);
  if (sym != buf) free (sym);
  return s;
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
#line 296 "../../lib/graph/parser.y"
typedef union YYSTYPE {
			int					i;
			char				*str;
			struct objport_t	obj;
			struct Agnode_t		*n;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 384 "y.tab.c"
# define agstype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 396 "y.tab.c"

#if ! defined (agoverflow) || YYERROR_VERBOSE

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
#endif /* ! defined (agoverflow) || YYERROR_VERBOSE */


#if (! defined (agoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union agalloc
{
  short int agss;
  YYSTYPE agvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union agalloc) - 1)

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
	  register YYSIZE_T agi;		\
	  for (agi = 0; agi < (Count); agi++)	\
	    (To)[agi] = (From)[agi];		\
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
	YYSIZE_T agnewbytes;						\
	YYCOPY (&agptr->Stack, Stack, agsize);				\
	Stack = &agptr->Stack;						\
	agnewbytes = agstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	agptr += agnewbytes / sizeof (*agptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char agsigned_char;
#else
   typedef short int agsigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   80

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  37
/* YYNRULES -- Number of rules. */
#define YYNRULES  68
/* YYNRULES -- Number of states. */
#define YYNSTATES  90

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   266

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? agtranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char agtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    20,    14,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,    18,
       2,    17,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    15,     2,    16,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    12,     2,    13,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char agprhs[] =
{
       0,     0,     3,     4,    11,    13,    14,    16,    17,    19,
      22,    24,    27,    29,    31,    33,    37,    38,    39,    41,
      45,    48,    49,    51,    55,    57,    59,    61,    62,    64,
      67,    69,    72,    74,    76,    78,    80,    82,    85,    87,
      90,    92,    93,    96,   101,   102,   106,   107,   108,   114,
     115,   116,   122,   125,   126,   131,   134,   135,   140,   145,
     146,   152,   153,   158,   160,   163,   165,   167,   169
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const agsigned_char agrhs[] =
{
      22,     0,    -1,    -1,    25,    24,    23,    12,    34,    13,
      -1,     1,    -1,    -1,    56,    -1,    -1,     3,    -1,     5,
       3,    -1,     4,    -1,     5,     4,    -1,     3,    -1,     6,
      -1,     7,    -1,    33,    28,    27,    -1,    -1,    -1,    14,
      -1,    15,    27,    16,    -1,    30,    29,    -1,    -1,    30,
      -1,    56,    17,    56,    -1,    32,    -1,    56,    -1,    35,
      -1,    -1,    36,    -1,    35,    36,    -1,    37,    -1,    37,
      18,    -1,     1,    -1,    42,    -1,    44,    -1,    38,    -1,
      52,    -1,    26,    29,    -1,    32,    -1,    40,    41,    -1,
      56,    -1,    -1,    19,    56,    -1,    19,    56,    19,    56,
      -1,    -1,    39,    43,    31,    -1,    -1,    -1,    39,    45,
      49,    46,    31,    -1,    -1,    -1,    52,    47,    49,    48,
      31,    -1,     8,    39,    -1,    -1,     8,    39,    50,    49,
      -1,     8,    52,    -1,    -1,     8,    52,    51,    49,    -1,
      55,    12,    34,    13,    -1,    -1,    11,    12,    53,    34,
      13,    -1,    -1,    12,    54,    34,    13,    -1,    55,    -1,
      11,    56,    -1,     9,    -1,    57,    -1,    10,    -1,    57,
      20,    10,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int agrline[] =
{
       0,   314,   314,   313,   317,   324,   327,   327,   330,   332,
     334,   336,   340,   342,   344,   348,   349,   352,   353,   356,
     359,   360,   363,   366,   370,   371,   375,   376,   379,   380,
     383,   384,   385,   388,   389,   390,   391,   394,   396,   400,
     410,   413,   414,   415,   419,   418,   425,   427,   424,   432,
     434,   431,   440,   442,   441,   444,   447,   446,   452,   453,
     453,   454,   454,   455,   458,   468,   469,   472,   473
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const agtname[] =
{
  "$end", "error", "$undefined", "T_graph", "T_digraph", "T_strict",
  "T_node", "T_edge", "T_edgeop", "T_symbol", "T_qsymbol", "T_subgraph",
  "'{'", "'}'", "','", "'['", "']'", "'='", "';'", "':'", "'+'", "$accept",
  "file", "@1", "optgraphname", "graph_type", "attr_class",
  "inside_attr_list", "optcomma", "attr_list", "rec_attr_list",
  "opt_attr_list", "attr_set", "iattr_set", "stmt_list", "stmt_list1",
  "stmt", "stmt1", "attr_stmt", "node_id", "node_name", "node_port",
  "node_stmt", "@2", "edge_stmt", "@3", "@4", "@5", "@6", "edgeRHS", "@7",
  "@8", "subg_stmt", "@9", "@10", "subg_hdr", "symbol", "qsymbol", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int agtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   123,   125,    44,    91,    93,    61,    59,    58,
      43
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char agr1[] =
{
       0,    21,    23,    22,    22,    22,    24,    24,    25,    25,
      25,    25,    26,    26,    26,    27,    27,    28,    28,    29,
      30,    30,    31,    32,    33,    33,    34,    34,    35,    35,
      36,    36,    36,    37,    37,    37,    37,    38,    38,    39,
      40,    41,    41,    41,    43,    42,    45,    46,    44,    47,
      48,    44,    49,    50,    49,    49,    51,    49,    52,    53,
      52,    54,    52,    52,    55,    56,    56,    57,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char agr2[] =
{
       0,     2,     0,     6,     1,     0,     1,     0,     1,     2,
       1,     2,     1,     1,     1,     3,     0,     0,     1,     3,
       2,     0,     1,     3,     1,     1,     1,     0,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     0,     2,     4,     0,     3,     0,     0,     5,     0,
       0,     5,     2,     0,     4,     2,     0,     4,     4,     0,
       5,     0,     4,     1,     2,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char agdefact[] =
{
       0,     4,     8,    10,     0,     0,     7,     9,    11,     1,
      65,    67,     2,     6,    66,     0,     0,     0,    68,    32,
      12,    13,    14,     0,    61,     0,    38,     0,     0,    28,
      30,    35,    44,    41,    33,    34,    36,    63,    40,    59,
      64,     0,    16,    37,     3,    29,    31,    21,     0,     0,
      39,     0,     0,     0,     0,     0,     0,    24,    17,    25,
      22,    45,     0,    47,    42,    50,     0,    23,     0,    62,
      19,    18,    16,    20,    52,    55,    40,    21,     0,    21,
      58,    60,    15,     0,     0,    48,    43,    51,    54,    57
};

/* YYDEFGOTO[NTERM-NUM]. */
static const agsigned_char agdefgoto[] =
{
      -1,     5,    15,    12,     6,    25,    56,    72,    43,    60,
      61,    26,    58,    27,    28,    29,    30,    31,    32,    33,
      50,    34,    47,    35,    48,    77,    51,    79,    63,    83,
      84,    36,    54,    41,    37,    38,    14
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -68
static const agsigned_char agpact[] =
{
       4,   -68,   -68,   -68,    27,     6,    44,   -68,   -68,   -68,
     -68,   -68,   -68,   -68,    -9,     8,    25,    12,   -68,   -68,
     -68,   -68,   -68,    29,   -68,    22,   -68,    31,    39,   -68,
      37,   -68,    49,    40,   -68,   -68,    50,    48,    45,   -68,
     -68,    12,    44,   -68,   -68,   -68,   -68,   -68,    53,    44,
     -68,    53,    12,    44,    12,    51,    47,   -68,    54,    45,
      22,   -68,    17,   -68,    46,   -68,    56,   -68,    57,   -68,
     -68,   -68,    44,   -68,    59,    63,   -68,   -68,    44,   -68,
     -68,   -68,   -68,    53,    53,   -68,   -68,   -68,   -68,   -68
};

/* YYPGOTO[NTERM-NUM].  */
static const agsigned_char agpgoto[] =
{
     -68,   -68,   -68,   -68,   -68,   -68,     1,   -68,    14,   -68,
     -67,   -40,   -68,   -38,   -68,    52,   -68,   -68,    13,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,   -50,   -68,
     -68,    15,   -68,   -68,   -68,    -6,   -68
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -57
static const agsigned_char agtable[] =
{
      13,    65,    57,    55,    -5,     1,     9,     2,     3,     4,
      85,    16,    87,    19,    66,    20,    68,    40,    21,    22,
      17,    10,    11,    23,    24,   -27,    10,    11,    23,    24,
       7,     8,    57,    88,    89,    18,    59,    42,    10,    11,
      19,    39,    20,    64,    44,    21,    22,    67,    10,    11,
      23,    24,   -26,    10,    11,    46,    76,   -46,   -49,    49,
      52,    62,    53,    70,    69,    78,    59,   -53,    71,    80,
      81,   -56,    86,    82,    73,    74,     0,    75,     0,     0,
      45
};

static const agsigned_char agcheck[] =
{
       6,    51,    42,    41,     0,     1,     0,     3,     4,     5,
      77,    20,    79,     1,    52,     3,    54,    23,     6,     7,
      12,     9,    10,    11,    12,    13,     9,    10,    11,    12,
       3,     4,    72,    83,    84,    10,    42,    15,     9,    10,
       1,    12,     3,    49,    13,     6,     7,    53,     9,    10,
      11,    12,    13,     9,    10,    18,    62,     8,     8,    19,
      12,     8,    17,    16,    13,    19,    72,     8,    14,    13,
      13,     8,    78,    72,    60,    62,    -1,    62,    -1,    -1,
      28
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char agstos[] =
{
       0,     1,     3,     4,     5,    22,    25,     3,     4,     0,
       9,    10,    24,    56,    57,    23,    20,    12,    10,     1,
       3,     6,     7,    11,    12,    26,    32,    34,    35,    36,
      37,    38,    39,    40,    42,    44,    52,    55,    56,    12,
      56,    54,    15,    29,    13,    36,    18,    43,    45,    19,
      41,    47,    12,    17,    53,    34,    27,    32,    33,    56,
      30,    31,     8,    49,    56,    49,    34,    56,    34,    13,
      16,    14,    28,    29,    39,    52,    56,    46,    19,    48,
      13,    13,    27,    50,    51,    31,    56,    31,    49,    49
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

#define agerrok		(agerrstatus = 0)
#define agclearin	(agchar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto agacceptlab
#define YYABORT		goto agabortlab
#define YYERROR		goto agerrorlab


/* Like YYERROR except do call agerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto agerrlab

#define YYRECOVERING()  (!!agerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (agchar == YYEMPTY && aglen == 1)				\
    {								\
      agchar = (Token);						\
      aglval = (Value);						\
      agtoken = YYTRANSLATE (agchar);				\
      YYPOPSTACK;						\
      goto agbackup;						\
    }								\
  else								\
    { 								\
      agerror ("syntax error: cannot back up");\
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


/* YYLEX -- calling `aglex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX aglex (YYLEX_PARAM)
#else
# define YYLEX aglex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (agdebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (agdebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      agsymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| ag_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ag_stack_print (short int *bottom, short int *top)
#else
static void
ag_stack_print (bottom, top)
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
  if (agdebug)							\
    ag_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ag_reduce_print (int agrule)
#else
static void
ag_reduce_print (agrule)
    int agrule;
#endif
{
  int agi;
  unsigned int aglno = agrline[agrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             agrule - 1, aglno);
  /* Print the symbols being reduced, and their result.  */
  for (agi = agprhs[agrule]; 0 <= agrhs[agi]; agi++)
    YYFPRINTF (stderr, "%s ", agtname [agrhs[agi]]);
  YYFPRINTF (stderr, "-> %s\n", agtname [agr1[agrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (agdebug)				\
    ag_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int agdebug;
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

# ifndef agstrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define agstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
agstrlen (const char *agstr)
#   else
agstrlen (agstr)
     const char *agstr;
#   endif
{
  register const char *ags = agstr;

  while (*ags++ != '\0')
    continue;

  return ags - agstr - 1;
}
#  endif
# endif

# ifndef agstpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define agstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
agstpcpy (char *agdest, const char *agsrc)
#   else
agstpcpy (agdest, agsrc)
     char *agdest;
     const char *agsrc;
#   endif
{
  register char *agd = agdest;
  register const char *ags = agsrc;

  while ((*agd++ = *ags++) != '\0')
    continue;

  return agd - 1;
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
agsymprint (FILE *agoutput, int agtype, YYSTYPE *agvaluep)
#else
static void
agsymprint (agoutput, agtype, agvaluep)
    FILE *agoutput;
    int agtype;
    YYSTYPE *agvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) agvaluep;

  if (agtype < YYNTOKENS)
    YYFPRINTF (agoutput, "token %s (", agtname[agtype]);
  else
    YYFPRINTF (agoutput, "nterm %s (", agtname[agtype]);


# ifdef YYPRINT
  if (agtype < YYNTOKENS)
    YYPRINT (agoutput, agtoknum[agtype], *agvaluep);
# endif
  switch (agtype)
    {
      default:
        break;
    }
  YYFPRINTF (agoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
agdestruct (const char *agmsg, int agtype, YYSTYPE *agvaluep)
#else
static void
agdestruct (agmsg, agtype, agvaluep)
    const char *agmsg;
    int agtype;
    YYSTYPE *agvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) agvaluep;

  if (!agmsg)
    agmsg = "Deleting";
  YY_SYMBOL_PRINT (agmsg, agtype, agvaluep, aglocationp);

  switch (agtype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int agparse (void *YYPARSE_PARAM);
# else
int agparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int agparse (void);
#else
int agparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int agchar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE aglval;

/* Number of syntax errors so far.  */
int agnerrs;



/*----------.
| agparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int agparse (void *YYPARSE_PARAM)
# else
int agparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
agparse (void)
#else
int
agparse ()

#endif
#endif
{
  
  register int agstate;
  register int agn;
  int agresult;
  /* Number of tokens to shift before error messages enabled.  */
  int agerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int agtoken = 0;

  /* Three stacks and their tools:
     `agss': related to states,
     `agvs': related to semantic values,
     `agls': related to locations.

     Refer to the stacks thru separate pointers, to allow agoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int agssa[YYINITDEPTH];
  short int *agss = agssa;
  register short int *agssp;

  /* The semantic value stack.  */
  YYSTYPE agvsa[YYINITDEPTH];
  YYSTYPE *agvs = agvsa;
  register YYSTYPE *agvsp;



#define YYPOPSTACK   (agvsp--, agssp--)

  YYSIZE_T agstacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE agval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int aglen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  agstate = 0;
  agerrstatus = 0;
  agnerrs = 0;
  agchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  agssp = agss;
  agvsp = agvs;


  agvsp[0] = aglval;

  goto agsetstate;

/*------------------------------------------------------------.
| agnewstate -- Push a new state, which is found in agstate.  |
`------------------------------------------------------------*/
 agnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  agssp++;

 agsetstate:
  *agssp = agstate;

  if (agss + agstacksize - 1 <= agssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T agsize = agssp - agss + 1;

#ifdef agoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *agvs1 = agvs;
	short int *agss1 = agss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if agoverflow is a macro.  */
	agoverflow ("parser stack overflow",
		    &agss1, agsize * sizeof (*agssp),
		    &agvs1, agsize * sizeof (*agvsp),

		    &agstacksize);

	agss = agss1;
	agvs = agvs1;
      }
#else /* no agoverflow */
# ifndef YYSTACK_RELOCATE
      goto agoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= agstacksize)
	goto agoverflowlab;
      agstacksize *= 2;
      if (YYMAXDEPTH < agstacksize)
	agstacksize = YYMAXDEPTH;

      {
	short int *agss1 = agss;
	union agalloc *agptr =
	  (union agalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (agstacksize));
	if (! agptr)
	  goto agoverflowlab;
	YYSTACK_RELOCATE (agss);
	YYSTACK_RELOCATE (agvs);

#  undef YYSTACK_RELOCATE
	if (agss1 != agssa)
	  YYSTACK_FREE (agss1);
      }
# endif
#endif /* no agoverflow */

      agssp = agss + agsize - 1;
      agvsp = agvs + agsize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) agstacksize));

      if (agss + agstacksize - 1 <= agssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", agstate));

  goto agbackup;

/*-----------.
| agbackup.  |
`-----------*/
agbackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* agresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  agn = agpact[agstate];
  if (agn == YYPACT_NINF)
    goto agdefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (agchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      agchar = YYLEX;
    }

  if (agchar <= YYEOF)
    {
      agchar = agtoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      agtoken = YYTRANSLATE (agchar);
      YY_SYMBOL_PRINT ("Next token is", agtoken, &aglval, &aglloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  agn += agtoken;
  if (agn < 0 || YYLAST < agn || agcheck[agn] != agtoken)
    goto agdefault;
  agn = agtable[agn];
  if (agn <= 0)
    {
      if (agn == 0 || agn == YYTABLE_NINF)
	goto agerrlab;
      agn = -agn;
      goto agreduce;
    }

  if (agn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", agtoken, &aglval, &aglloc);

  /* Discard the token being shifted unless it is eof.  */
  if (agchar != YYEOF)
    agchar = YYEMPTY;

  *++agvsp = aglval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (agerrstatus)
    agerrstatus--;

  agstate = agn;
  goto agnewstate;


/*-----------------------------------------------------------.
| agdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
agdefault:
  agn = agdefact[agstate];
  if (agn == 0)
    goto agerrlab;
  goto agreduce;


/*-----------------------------.
| agreduce -- Do a reduction.  |
`-----------------------------*/
agreduce:
  /* agn is the number of a rule to reduce with.  */
  aglen = agr2[agn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  agval = agvsp[1-aglen];


  YY_REDUCE_PRINT (agn);
  switch (agn)
    {
        case 2:
#line 314 "../../lib/graph/parser.y"
    {begin_graph((agvsp[0].str)); agstrfree((agvsp[0].str));}
    break;

  case 3:
#line 316 "../../lib/graph/parser.y"
    {AG.accepting_state = TRUE; end_graph();}
    break;

  case 4:
#line 318 "../../lib/graph/parser.y"
    {
					if (AG.parsed_g)
						agclose(AG.parsed_g);
					AG.parsed_g = NULL;
					/*exit(1);*/
				}
    break;

  case 5:
#line 324 "../../lib/graph/parser.y"
    {AG.parsed_g = NULL;}
    break;

  case 6:
#line 327 "../../lib/graph/parser.y"
    {(agval.str)=(agvsp[0].str);}
    break;

  case 7:
#line 327 "../../lib/graph/parser.y"
    {(agval.str)=0;}
    break;

  case 8:
#line 331 "../../lib/graph/parser.y"
    {Agraph_type = AGRAPH; AG.edge_op = "--";}
    break;

  case 9:
#line 333 "../../lib/graph/parser.y"
    {Agraph_type = AGRAPHSTRICT; AG.edge_op = "--";}
    break;

  case 10:
#line 335 "../../lib/graph/parser.y"
    {Agraph_type = AGDIGRAPH; AG.edge_op = "->";}
    break;

  case 11:
#line 337 "../../lib/graph/parser.y"
    {Agraph_type = AGDIGRAPHSTRICT; AG.edge_op = "->";}
    break;

  case 12:
#line 341 "../../lib/graph/parser.y"
    {Current_class = TAG_GRAPH;}
    break;

  case 13:
#line 343 "../../lib/graph/parser.y"
    {Current_class = TAG_NODE; N = G->proto->n;}
    break;

  case 14:
#line 345 "../../lib/graph/parser.y"
    {Current_class = TAG_EDGE; E = G->proto->e;}
    break;

  case 23:
#line 367 "../../lib/graph/parser.y"
    {attr_set((agvsp[-2].str),(agvsp[0].str)); agstrfree((agvsp[-2].str)); agstrfree((agvsp[0].str));}
    break;

  case 25:
#line 372 "../../lib/graph/parser.y"
    {attr_set((agvsp[0].str),"true"); agstrfree((agvsp[0].str)); }
    break;

  case 32:
#line 385 "../../lib/graph/parser.y"
    {agerror("syntax error, statement skipped");}
    break;

  case 36:
#line 391 "../../lib/graph/parser.y"
    {}
    break;

  case 37:
#line 395 "../../lib/graph/parser.y"
    {Current_class = TAG_GRAPH; /* reset */}
    break;

  case 38:
#line 397 "../../lib/graph/parser.y"
    {Current_class = TAG_GRAPH;}
    break;

  case 39:
#line 401 "../../lib/graph/parser.y"
    {
					objport_t		rv;
					rv.obj = (agvsp[-1].n);
					rv.port = Port;
					Port = NULL;
					(agval.obj) = rv;
				}
    break;

  case 40:
#line 410 "../../lib/graph/parser.y"
    {(agval.n) = bind_node((agvsp[0].str)); agstrfree((agvsp[0].str));}
    break;

  case 42:
#line 414 "../../lib/graph/parser.y"
    { Port=(agvsp[0].str);}
    break;

  case 43:
#line 415 "../../lib/graph/parser.y"
    {Port=concat3((agvsp[-2].str),":",(agvsp[0].str));agstrfree((agvsp[-2].str)); agstrfree((agvsp[0].str));}
    break;

  case 44:
#line 419 "../../lib/graph/parser.y"
    {Current_class = TAG_NODE; N = (Agnode_t*)((agvsp[0].obj).obj);}
    break;

  case 45:
#line 421 "../../lib/graph/parser.y"
    {free((agvsp[-2].obj).port);Current_class = TAG_GRAPH; /* reset */}
    break;

  case 46:
#line 425 "../../lib/graph/parser.y"
    {begin_edgestmt((agvsp[0].obj));}
    break;

  case 47:
#line 427 "../../lib/graph/parser.y"
    { E = SP->subg->proto->e;
				  Current_class = TAG_EDGE; }
    break;

  case 48:
#line 430 "../../lib/graph/parser.y"
    {end_edgestmt();}
    break;

  case 49:
#line 432 "../../lib/graph/parser.y"
    {begin_edgestmt((agvsp[0].obj));}
    break;

  case 50:
#line 434 "../../lib/graph/parser.y"
    { E = SP->subg->proto->e;
				  Current_class = TAG_EDGE; }
    break;

  case 51:
#line 437 "../../lib/graph/parser.y"
    {end_edgestmt();}
    break;

  case 52:
#line 440 "../../lib/graph/parser.y"
    {mid_edgestmt((agvsp[0].obj));}
    break;

  case 53:
#line 442 "../../lib/graph/parser.y"
    {mid_edgestmt((agvsp[0].obj));}
    break;

  case 55:
#line 445 "../../lib/graph/parser.y"
    {mid_edgestmt((agvsp[0].obj));}
    break;

  case 56:
#line 447 "../../lib/graph/parser.y"
    {mid_edgestmt((agvsp[0].obj));}
    break;

  case 58:
#line 452 "../../lib/graph/parser.y"
    {(agval.obj) = pop_gobj();}
    break;

  case 59:
#line 453 "../../lib/graph/parser.y"
    { anonsubg(); }
    break;

  case 60:
#line 453 "../../lib/graph/parser.y"
    {(agval.obj) = pop_gobj();}
    break;

  case 61:
#line 454 "../../lib/graph/parser.y"
    { anonsubg(); }
    break;

  case 62:
#line 454 "../../lib/graph/parser.y"
    {(agval.obj) = pop_gobj();}
    break;

  case 63:
#line 455 "../../lib/graph/parser.y"
    {(agval.obj) = pop_gobj();}
    break;

  case 64:
#line 459 "../../lib/graph/parser.y"
    { Agraph_t	 *subg;
				if ((subg = agfindsubg(AG.parsed_g,(agvsp[0].str)))) aginsert(G,subg);
				else subg = agsubg(G,(agvsp[0].str)); 
				push_subg(subg);
				In_decl = FALSE;
				agstrfree((agvsp[0].str));
				}
    break;

  case 65:
#line 468 "../../lib/graph/parser.y"
    {(agval.str) = (agvsp[0].str); }
    break;

  case 66:
#line 469 "../../lib/graph/parser.y"
    {(agval.str) = (agvsp[0].str); }
    break;

  case 67:
#line 472 "../../lib/graph/parser.y"
    {(agval.str) = (agvsp[0].str); }
    break;

  case 68:
#line 473 "../../lib/graph/parser.y"
    {(agval.str) = concat((agvsp[-2].str),(agvsp[0].str)); agstrfree((agvsp[-2].str)); agstrfree((agvsp[0].str));}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 1636 "y.tab.c"

  agvsp -= aglen;
  agssp -= aglen;


  YY_STACK_PRINT (agss, agssp);

  *++agvsp = agval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  agn = agr1[agn];

  agstate = agpgoto[agn - YYNTOKENS] + *agssp;
  if (0 <= agstate && agstate <= YYLAST && agcheck[agstate] == *agssp)
    agstate = agtable[agstate];
  else
    agstate = agdefgoto[agn - YYNTOKENS];

  goto agnewstate;


/*------------------------------------.
| agerrlab -- here on detecting error |
`------------------------------------*/
agerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!agerrstatus)
    {
      ++agnerrs;
#if YYERROR_VERBOSE
      agn = agpact[agstate];

      if (YYPACT_NINF < agn && agn < YYLAST)
	{
	  YYSIZE_T agsize = 0;
	  int agtype = YYTRANSLATE (agchar);
	  const char* agprefix;
	  char *agmsg;
	  int agx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int agxbegin = agn < 0 ? -agn : 0;

	  /* Stay within bounds of both agcheck and agtname.  */
	  int agchecklim = YYLAST - agn;
	  int agxend = agchecklim < YYNTOKENS ? agchecklim : YYNTOKENS;
	  int agcount = 0;

	  agprefix = ", expecting ";
	  for (agx = agxbegin; agx < agxend; ++agx)
	    if (agcheck[agx + agn] == agx && agx != YYTERROR)
	      {
		agsize += agstrlen (agprefix) + agstrlen (agtname [agx]);
		agcount += 1;
		if (agcount == 5)
		  {
		    agsize = 0;
		    break;
		  }
	      }
	  agsize += (sizeof ("syntax error, unexpected ")
		     + agstrlen (agtname[agtype]));
	  agmsg = (char *) YYSTACK_ALLOC (agsize);
	  if (agmsg != 0)
	    {
	      char *agp = agstpcpy (agmsg, "syntax error, unexpected ");
	      agp = agstpcpy (agp, agtname[agtype]);

	      if (agcount < 5)
		{
		  agprefix = ", expecting ";
		  for (agx = agxbegin; agx < agxend; ++agx)
		    if (agcheck[agx + agn] == agx && agx != YYTERROR)
		      {
			agp = agstpcpy (agp, agprefix);
			agp = agstpcpy (agp, agtname[agx]);
			agprefix = " or ";
		      }
		}
	      agerror (agmsg);
	      YYSTACK_FREE (agmsg);
	    }
	  else
	    agerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	agerror ("syntax error");
    }



  if (agerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (agchar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (agchar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (agssp == agss)
		   YYABORT;
		 agdestruct ("Error: popping",
                             agstos[*agssp], agvsp);
	       }
        }
      else
	{
	  agdestruct ("Error: discarding", agtoken, &aglval);
	  agchar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto agerrlab1;


/*---------------------------------------------------.
| agerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
agerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     agerrorlab therefore never appears in user code.  */
  if (0)
     goto agerrorlab;
#endif

agvsp -= aglen;
  agssp -= aglen;
  agstate = *agssp;
  goto agerrlab1;


/*-------------------------------------------------------------.
| agerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
agerrlab1:
  agerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      agn = agpact[agstate];
      if (agn != YYPACT_NINF)
	{
	  agn += YYTERROR;
	  if (0 <= agn && agn <= YYLAST && agcheck[agn] == YYTERROR)
	    {
	      agn = agtable[agn];
	      if (0 < agn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (agssp == agss)
	YYABORT;


      agdestruct ("Error: popping", agstos[agstate], agvsp);
      YYPOPSTACK;
      agstate = *agssp;
      YY_STACK_PRINT (agss, agssp);
    }

  if (agn == YYFINAL)
    YYACCEPT;

  *++agvsp = aglval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", agstos[agn], agvsp, aglsp);

  agstate = agn;
  goto agnewstate;


/*-------------------------------------.
| agacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
agacceptlab:
  agresult = 0;
  goto agreturn;

/*-----------------------------------.
| agabortlab -- YYABORT comes here.  |
`-----------------------------------*/
agabortlab:
  agdestruct ("Error: discarding lookahead",
              agtoken, &aglval);
  agchar = YYEMPTY;
  agresult = 1;
  goto agreturn;

#ifndef agoverflow
/*----------------------------------------------.
| agoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
agoverflowlab:
  agerror ("parser stack overflow");
  agresult = 2;
  /* Fall through.  */
#endif

agreturn:
#ifndef agoverflow
  if (agss != agssa)
    YYSTACK_FREE (agss);
#endif
  return agresult;
}


#line 475 "../../lib/graph/parser.y"

#ifdef UNUSED
/* grammar allowing port variants */
/* at present, these are not used, so we remove them from the grammar */
node_port	:	/* empty */
		|	port_location 
		|	port_angle 			/* undocumented */
		|	port_angle port_location 	/* undocumented */
		|	port_location port_angle 	/* undocumented */
		;

port_location	:	':' symbol {strcat(Port,":"); strcat(Port,$2);}
		|	':' '(' symbol {Symbol = strdup($3);} ',' symbol ')'
				{	char buf[SMALLBUF];
					sprintf(buf,":(%s,%s)",Symbol,$6);
					strcat(Port,buf); free(Symbol);
				}
		;

port_angle	:	'@' symbol
				{	char buf[SMALLBUF];
					sprintf(buf,"@%s",$2);
					strcat(Port,buf);
				}
		;

#endif

