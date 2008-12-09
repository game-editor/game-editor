wxXmlSerializer for wxWidgets
-----------------------------
Author: Michal Bliznak, 2007
e-mail: michal.bliznak@tiscali.cz
www: http://sourceforge.net/projects/wxxmlserializer


What is the wxXmlSerializer?
----------------------------
wxXmlSerializer (shortly wxXS) is a software library based on wxWidgets which offers a functionality needed for creation
of persistent classes/hierarchical data containers. wxXS allows user to easily serialize/deserialize hierarchicaly arragned
class instances and their datamembers to/from an XML structure. The user can create classes derived from a so called 'serializable'
base class provided by the wxXS and then define which the class members will be serialized and which not into XML structure.
The XML content can be then stored in a disk file or in any output stream. Currently supported data types serializable
by the wxXmlSerializer are: bool, char, int, long, float, double, wxString, wxPoint, wxSize, wxRealPoint, wxPen,
wxBrush, wxFont, wxColour, wxArrayString, array of wxRealPoint values, list of wxRealPoint values, and dynamic or
static instance of the serializable class itself. Moreover, new I/O handlers for custom data types can be created
via a set of macros provided by the library.

The library can be successfully used for wide range of application scenarios: it provides simple functionality
needed for saving of program settings/configuration and on the other hand it can be used as a persistent
dynamic d-ary tree-based data container with all functionality needed for comfort managing of its stored items.


Requirements:
-------------
- wxXmlSerializer uses wxWidgets (www.wxwidgets.org) and was tested with version 2.8.x on MS Windows
  and Linux (Ubuntu 7.04) target platforms.
- Code::Blocks (www.codeblocks.org) IDE with MinGW C++ compiler or MS Visual Studio 2005/2008/
  Visual C++ 2005/2008 Express are recommended for opening generated workspace/project files.


Distribution content:
---------------------

.\build		- Premake build system for creation of various project/make files
.\doc		- Doxygen configuration file and built documentation files
.\include	- Library headers files (needed for both hosting applications and
		library itself)
.\samples	- Sample projects with commented source code.
.\src		- Source code ad project files of the wxXmlSerializer library
changes.txt	- Information about library version and release changes.

.\lib		- Lib files (will be created after the successful build process)

Note: The binary distribution package contains also prebuild library files for MS Windows platform.
Currently only statically and dynamically linkable library files for MS VC++ 9.0 and MinGW (without
debuging symbols, with unicode support) are available.


Build instructions:
-------------------
For building of the library files and the sample and demo projects you must 
create a build/project files first. This task can be done via included premake build
system located in the ./build directory. Simple run one of the appropriate shell scripts 
(create_build_files.bat for MS Windows platform, create_build_files.sh for Linux and
create_build_files_mac.sh for OS X platform). These scripts will create project/make files
suitable for choosen build targets and IDEs. Currently the Code::Blocks and MS Visual
Studio 2005/Visual C++ 2005 Express project files for Windows/Linux platforms are
supported as well as standard GNU makefiles (target flag 'vs2005' is suitable also for MS Visual
Studio 2008/Visual C++ 2008). If you want to change some build target properties modify the
script files to fulfil your needs.


Related projects and information:
---------------------------------
For more inspiration you can donwload and see the source codes of wxShapeFramework library
(open source, multiplatform diagraming graphics library published at http://sourceforge.net/
projects/wxsf) which uses the wxXmlSerializer for a data persistence and also as a core part of its
built-in the clipboard and undo/redo functionality.
