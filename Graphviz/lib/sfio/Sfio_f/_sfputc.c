/* $Id: _sfputc.c,v 1.1.1.1 2004/12/23 04:04:18 ellson Exp $ $Revision: 1.1.1.1 $ */
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

#include	"sfhdr.h"

#undef sfputc

#if __STD_C
int sfputc(reg Sfio_t * f, reg int c)
#else
int sfputc(f, c)
reg Sfio_t *f;
reg int c;
#endif
{
    return __sf_putc(f, c);
}
