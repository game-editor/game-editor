/* $Id: pathbin.c,v 1.1.1.1 2004/12/23 04:04:21 ellson Exp $ $Revision: 1.1.1.1 $ */
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

/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * return current PATH
 */

#include <ast.h>

char *pathbin(void)
{
    register char *bin;

    static char *val;

    if ((!(bin = getenv("PATH")) || !*bin) && !(bin = val)) {
	bin = "/bin:/usr/bin:/usr/local/bin";
	val = bin;
    }
    return bin;
}
