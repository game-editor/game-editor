#ifndef _EXPARSE_H
#define _EXPARSE_H
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

/* Tokens.  */
#ifndef EXTOKENTYPE
# define EXTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum extokentype {
     MINTOKEN = 258,
     INT = 259,
     INTEGER = 260,
     UNSIGNED = 261,
     CHAR = 262,
     FLOATING = 263,
     STRING = 264,
     VOID = 265,
     ADDRESS = 266,
     ARRAY = 267,
     BREAK = 268,
     CALL = 269,
     CASE = 270,
     CONSTANT = 271,
     CONTINUE = 272,
     DECLARE = 273,
     DEFAULT = 274,
     DYNAMIC = 275,
     ELSE = 276,
     EXIT = 277,
     FOR = 278,
     FUNCTION = 279,
     GSUB = 280,
     ITERATE = 281,
     ID = 282,
     IF = 283,
     LABEL = 284,
     MEMBER = 285,
     NAME = 286,
     POS = 287,
     PRAGMA = 288,
     PRE = 289,
     PRINT = 290,
     PRINTF = 291,
     PROCEDURE = 292,
     QUERY = 293,
     RAND = 294,
     RETURN = 295,
     SCANF = 296,
     SPRINTF = 297,
     SRAND = 298,
     SSCANF = 299,
     SUB = 300,
     SUBSTR = 301,
     SWITCH = 302,
     WHILE = 303,
     F2I = 304,
     F2S = 305,
     I2F = 306,
     I2S = 307,
     S2B = 308,
     S2F = 309,
     S2I = 310,
     F2X = 311,
     I2X = 312,
     S2X = 313,
     X2F = 314,
     X2I = 315,
     X2S = 316,
     X2X = 317,
     XPRINT = 318,
     OR = 319,
     AND = 320,
     NE = 321,
     EQ = 322,
     GE = 323,
     LE = 324,
     RS = 325,
     LS = 326,
     UNARY = 327,
     DEC = 328,
     INC = 329,
     CAST = 330,
     MAXTOKEN = 331
   };
#endif
#define MINTOKEN 258
#define INT 259
#define INTEGER 260
#define UNSIGNED 261
#define CHAR 262
#define FLOATING 263
#define STRING 264
#define VOID 265
#define ADDRESS 266
#define ARRAY 267
#define BREAK 268
#define CALL 269
#define CASE 270
#define CONSTANT 271
#define CONTINUE 272
#define DECLARE 273
#define DEFAULT 274
#define DYNAMIC 275
#define ELSE 276
#define EXIT 277
#define FOR 278
#define FUNCTION 279
#define GSUB 280
#define ITERATE 281
#define ID 282
#define IF 283
#define LABEL 284
#define MEMBER 285
#define NAME 286
#define POS 287
#define PRAGMA 288
#define PRE 289
#define PRINT 290
#define PRINTF 291
#define PROCEDURE 292
#define QUERY 293
#define RAND 294
#define RETURN 295
#define SCANF 296
#define SPRINTF 297
#define SRAND 298
#define SSCANF 299
#define SUB 300
#define SUBSTR 301
#define SWITCH 302
#define WHILE 303
#define F2I 304
#define F2S 305
#define I2F 306
#define I2S 307
#define S2B 308
#define S2F 309
#define S2I 310
#define F2X 311
#define I2X 312
#define S2X 313
#define X2F 314
#define X2I 315
#define X2S 316
#define X2X 317
#define XPRINT 318
#define OR 319
#define AND 320
#define NE 321
#define EQ 322
#define GE 323
#define LE 324
#define RS 325
#define LS 326
#define UNARY 327
#define DEC 328
#define INC 329
#define CAST 330
#define MAXTOKEN 331




#if ! defined (EXSTYPE) && ! defined (EXSTYPE_IS_DECLARED)
#line 37 "../../lib/expr/exparse.y"
typedef union EXSTYPE {
	struct Exnode_s*expr;
	double		floating;
	struct Exref_s*	reference;
	struct Exid_s*	id;
	Sflong_t	integer;
	int		op;
	char*		string;
	void*		user;
	struct Exbuf_s*	buffer;
} EXSTYPE;
/* Line 1318 of yacc.c.  */
#line 201 "y.tab.h"
# define exstype EXSTYPE /* obsolescent; will be withdrawn */
# define EXSTYPE_IS_DECLARED 1
# define EXSTYPE_IS_TRIVIAL 1
#endif

extern EXSTYPE exlval;



#endif /* _EXPARSE_H */
