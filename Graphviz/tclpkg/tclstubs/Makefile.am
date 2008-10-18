## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = @TCL_CFLAGS@ @TCL_INCLUDES@

LIBS = @TCL_STUB_LIB_SPEC@

if WITH_TCL
noinst_LTLIBRARIES = libtclstubs.la
endif
noinst_HEADERS = tclInt.h

# tclStubLib.c is a dummy file just to fool automake
libtclstubs_la_SOURCES = tclStubLib.c

EXTRA_DIST = $(libtclstubs_la_SOURCES) Makefile.old
