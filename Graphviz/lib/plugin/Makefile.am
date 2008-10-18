## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/graph \
	-I$(top_srcdir)/lib/cdt @GD_INCLUDES@

pkglib_LTLIBRARIES = \
	libgvplugin_neato_layout.la \
	libgvplugin_dot_layout.la \
	libgvplugin_usershape_gd.la

libgvplugin_neato_layout_la_LDFLAGS = -version-info @VERSION_INFO@ -no-undefined

libgvplugin_neato_layout_la_SOURCES = \
	gvplugin_neato_layout.c \
	gvlayout_neato_layout.c

libgvplugin_neato_layout_la_LIBADD = \
	$(top_builddir)/lib/neatogen/libneatogen.la \
	$(top_builddir)/lib/twopigen/libtwopigen.la \
	$(top_builddir)/lib/fdpgen/libfdpgen.la \
	$(top_builddir)/lib/circogen/libcircogen.la \
	$(top_builddir)/lib/pathplan/libpathplan.la \
	$(top_builddir)/lib/cdt/libcdt.la \
	$(top_builddir)/lib/pack/libpack.la \
	$(top_builddir)/lib/graph/libgraph.la \
	$(top_builddir)/lib/gvc/libgvc.la

libgvplugin_dot_layout_la_LDFLAGS = -version-info @VERSION_INFO@ -no-undefined

libgvplugin_dot_layout_la_SOURCES = \
	gvplugin_dot_layout.c \
	gvlayout_dot_layout.c

libgvplugin_dot_layout_la_LIBADD = \
	$(top_builddir)/lib/graph/libgraph.la \
	$(top_builddir)/lib/dotgen/libdotgen.la \
	$(top_builddir)/lib/pathplan/libpathplan.la \
	$(top_builddir)/lib/cdt/libcdt.la \
	$(top_builddir)/lib/gvc/libgvc.la 

libgvplugin_usershape_gd_la_LDFLAGS = -version-info @VERSION_INFO@ -no-undefined

libgvplugin_usershape_gd_la_SOURCES = \
	gvplugin_usershape_gd.c \
	gvusershape_gd.c

libgvplugin_usershape_gd_la_LIBADD = \
	$(top_builddir)/lib/common/libcommon.la \
	$(top_builddir)/lib/agraph/libagraph.la \
	$(top_builddir)/lib/cdt/libcdt.la \
	$(top_builddir)/lib/gvc/libgvc.la \
	@GD_LIBS@  @EXPAT_LIBS@ -lz -lm

EXTRA_DIST = Makefile.old
