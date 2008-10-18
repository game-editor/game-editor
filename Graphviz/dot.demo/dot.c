/* $Id: dot.c,v 1.3 2005/07/27 19:55:28 ellson Exp $ $Revision: 1.3 $ */
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

#include <gvc.h>

int main(int argc, char **argv)
{
    graph_t *g, *prev = NULL;
    GVC_t *gvc;

    gvc = gvContext();
    gvParseArgs(gvc, argc, argv);

    while ((g = next_input_graph())) {
	if (prev) {
	    gvFreeLayout(gvc, prev);
	    agclose(prev);
	}
	gvLayoutJobs(gvc, g);
	gvRenderJobs(gvc, g);
	prev = g;
    }
    return (gvFreeContext(gvc));
}
