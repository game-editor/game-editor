# Microsoft Developer Studio Generated NMAKE File, Based on text.dsp
!IF "$(CFG)" == ""
CFG=text - Win32 Debug
!MESSAGE No configuration specified. Defaulting to text - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "text - Win32 Release" && "$(CFG)" != "text - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "text.mak" CFG="text - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "text - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "text - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "text - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\text.lib"


CLEAN :
	-@erase "$(INTDIR)\gvplugin_text.obj"
	-@erase "$(INTDIR)\gvrender_ps.obj"
	-@erase "$(INTDIR)\gvrender_svg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\text.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /I "..\..\third-party\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\text.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\text.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\text.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvplugin_text.obj" \
	"$(INTDIR)\gvrender_svg.obj" \
	"$(INTDIR)\gvrender_ps.obj"

"$(OUTDIR)\text.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\text.lib"
   copy .\Release\text.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "text - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\text.lib"


CLEAN :
	-@erase "$(INTDIR)\gvplugin_text.obj"
	-@erase "$(INTDIR)\gvrender_ps.obj"
	-@erase "$(INTDIR)\gvrender_svg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\text.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\..\lib\gvc" /I "..\..\lib\common" /I "..\..\lib\cdt" /I "..\..\lib\graph" /I "..\..\lib\pathplan" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\text.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\text.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\text.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvplugin_text.obj" \
	"$(INTDIR)\gvrender_svg.obj" \
	"$(INTDIR)\gvrender_ps.obj"

"$(OUTDIR)\text.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\text.lib"
   copy .\Debug\text.lib ..\lib\Debug
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
!IF EXISTS("text.dep")
!INCLUDE "text.dep"
!ELSE 
!MESSAGE Warning: cannot find "text.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "text - Win32 Release" || "$(CFG)" == "text - Win32 Debug"
SOURCE=.\gvplugin_text.c

"$(INTDIR)\gvplugin_text.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvrender_ps.c

"$(INTDIR)\gvrender_ps.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvrender_svg.c

"$(INTDIR)\gvrender_svg.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

