# Microsoft Developer Studio Generated NMAKE File, Based on layout.dsp
!IF "$(CFG)" == ""
CFG=layout - Win32 Debug
!MESSAGE No configuration specified. Defaulting to layout - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "layout - Win32 Release" && "$(CFG)" != "layout - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "layout.mak" CFG="layout - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "layout - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "layout - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "layout - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\layout.lib"


CLEAN :
	-@erase "$(INTDIR)\gvlayout_dot_layout.obj"
	-@erase "$(INTDIR)\gvlayout_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_dot_layout.obj"
	-@erase "$(INTDIR)\gvplugin_neato_layout.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\layout.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /I "..\..\third-party\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\layout.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\layout.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\layout.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvlayout_dot_layout.obj" \
	"$(INTDIR)\gvplugin_dot_layout.obj" \
	"$(INTDIR)\gvplugin_neato_layout.obj" \
	"$(INTDIR)\gvlayout_neato_layout.obj"

"$(OUTDIR)\layout.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\layout.lib"
   copy .\Release\layout.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "layout - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\layout.lib"


CLEAN :
	-@erase "$(INTDIR)\gvlayout_dot_layout.obj"
	-@erase "$(INTDIR)\gvlayout_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_dot_layout.obj"
	-@erase "$(INTDIR)\gvplugin_neato_layout.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\layout.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\layout.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\layout.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\layout.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvlayout_dot_layout.obj" \
	"$(INTDIR)\gvplugin_dot_layout.obj" \
	"$(INTDIR)\gvplugin_neato_layout.obj" \
	"$(INTDIR)\gvlayout_neato_layout.obj"

"$(OUTDIR)\layout.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\layout.lib"
   copy .\Debug\layout.lib ..\lib\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("layout.dep")
!INCLUDE "layout.dep"
!ELSE 
!MESSAGE Warning: cannot find "layout.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "layout - Win32 Release" || "$(CFG)" == "layout - Win32 Debug"
SOURCE=.\gvlayout_dot_layout.c

"$(INTDIR)\gvlayout_dot_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvlayout_neato_layout.c

"$(INTDIR)\gvlayout_neato_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin_dot_layout.c

"$(INTDIR)\gvplugin_dot_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin_neato_layout.c

"$(INTDIR)\gvplugin_neato_layout.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

