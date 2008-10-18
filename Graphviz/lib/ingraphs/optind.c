/* $Id: optind.c,v 1.1 2005/01/20 19:57:38 erg Exp $ $Revision: 1.1 $ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)optind.c 1.1 90/03/23 SMI";
#endif


 /*LINTLIBRARY*/ int optind = 1;
int opterr = 1;
char *optarg;
