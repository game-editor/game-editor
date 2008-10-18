# Microsoft Developer Studio Generated NMAKE File, Based on cairo.dsp
!IF "$(CFG)" == ""
CFG=cairo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cairo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cairo - Win32 Release" && "$(CFG)" != "cairo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cairo.mak" CFG="cairo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cairo - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cairo - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cairo - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cairo.lib"


CLEAN :
	-@erase "$(INTDIR)\gvplugin_cairo.obj"
	-@erase "$(INTDIR)\gvrender_cairo.obj"
	-@erase "$(INTDIR)\gvtext_cairo.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cairo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\cairo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cairo.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cairo.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvplugin_cairo.obj" \
	"$(INTDIR)\gvrender_cairo.obj" \
	"$(INTDIR)\gvtext_cairo.obj"

"$(OUTDIR)\cairo.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\cairo.lib"
   copy .\Release\cairo.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "cairo - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cairo.lib"


CLEAN :
	-@erase "$(INTDIR)\gvplugin_cairo.obj"
	-@erase "$(INTDIR)\gvrender_cairo.obj"
	-@erase "$(INTDIR)\gvtext_cairo.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cairo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\cairo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cairo.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cairo.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvplugin_cairo.obj" \
	"$(INTDIR)\gvrender_cairo.obj" \
	"$(INTDIR)\gvtext_cairo.obj"

"$(OUTDIR)\cairo.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\cairo.lib"
   copy .\Debug\cairo.lib ..\lib\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cairo.dep")
!INCLUDE "cairo.dep"
!ELSE 
!MESSAGE Warning: cannot find "cairo.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cairo - Win32 Release" || "$(CFG)" == "cairo - Win32 Debug"
SOURCE=.\gvplugin_cairo.c

"$(INTDIR)\gvplugin_cairo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvrender_cairo.c

"$(INTDIR)\gvrender_cairo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvtext_cairo.c

"$(INTDIR)\gvtext_cairo.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

