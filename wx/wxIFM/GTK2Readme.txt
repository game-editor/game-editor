These are "ready-to-run" Linux makefiles, generated with the Linux version of MinGWStudio. They will generate the libraries in the ./lib subdirectory.

Simply extract the files inside this archive on to the directory where you extracted wxIFM-beta1.

Use "make -f Makefile.Debug.unix" and "make -f Makefile.Release.unix" for their respective library versions on the wxIFM main folder. This will create the libwx_gtk2_ifm-2.5(d).a libraries in the same directory.
Optionally, you can copy these libraries to the default wx library directory: "su" as root then "make -f Makefile.Debug.unix install" or "make Makefile.Release.unix install".
Inclusion of the wxIFM header files in your project is not handled in the makefiles yet. Either use relative paths and add the wxIFM include path to your project, or simply copy them to your project folder.

The demo also has two makefiles with the same names in its directory.

These makefiles were done in a bit of a hurry, so there's lots of irrevelevant stuff in them, but hopefully they'll help you get familiar with wxIFM quickly!

Enjoy! :-)
