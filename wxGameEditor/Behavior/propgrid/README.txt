
  ************************************************

    wxPropertyGrid

      By Jaakko Salli (jmsalli@users.sourceforge.net)

  ************************************************

    Version:   1.3.1
    Requires:  wxWidgets 2.8.0 or later
               (wxWidgets 2.6.x works too, but requires patching, see
               patch_wx26 directory)
    Platforms: Windows - OK; Linux GTK 2.0 - OK;
               Linux GTK 1.2, Mac OS X, Windows CE - builds and runs. Other platforms remain untested.
    License:   wxWidgets license

    Home Pages:     http://wxpropgrid.sourceforge.net/
                    http://www.geocities.com/jmsalli/propertygrid/index.html

    At SourceForge: http://sourceforge.net/projects/wxpropgrid/

    wxPropertyGrid is a specialized two-column grid for editing properties
    such as strings, numbers, flagsets, fonts, and colours. wxPropertySheet
    used to do the very same thing, but it hasn't been updated for a while
    and it is currently deprecated.

    wxPropertyGrid is modeled after .NET propertygrid (hence the name),
    and thus features are similar.


  HOW TO INSTALL AND BUILD
  ------------------------

    * WINDOWS

  Run installer or unpack .tar.gz source archive.

  wxWidgets 2.8.x:

  Makefiles and project files to use directly are located in propgrid/build.
  You need to tell the compiler the type of wx build to use with WX_DEBUG=0/1,
  WX_UNICODE=0/1, and WX_SHARED=0/1.
  
  For instance, this builds unicode dll release with msvc:

  nmake -f makefile.vc WX_DEBUG=0 WX_UNICODE=1 WXSHARED=1


  NOTES for wxWidgets 2.6.x and wxWidgets CVS (2.9.0):

  You must use makefiles and project files in build_wx26 or build_wx29
  directory (respectively).

  NOTES for wxWidgets 2.6.x only:
  
  Make sure your wxWidgets codebase has been patched with refcounted wxVariant
  (see patch_wx26 directory).


    * POSIX SYSTEMS (LINUX, MAC OS X, MINGW+MSYS, etc.)

  First make sure you have done 'make install' for your wxWidgets build.
  Then the usual configure && make && make install proccess can ensue.

  For instance:

  tar zxf wxpropgrid-1.3.0-src.tar.gz
  cd propgrid
  ./configure
  make
  make install

  You may also need to add lib dir to the lists of library paths (e.g. by using
  ldconfig under Linux).

  wxWidgets 2.6.x only:
  
  Make sure your wxWidgets codebase has been patched with refcounted wxVariant
  (see patch_wx26 directory).



  DOCUMENTATION
  -------------

  See docs/html/index.htm

  for documentation generated with doxygen. There should also be a shortcut
  index-propgrid.html at $WXDIR/contrib.

