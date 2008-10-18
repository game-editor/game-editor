# Microsoft Developer Studio Generated NMAKE File, Based on fdpgen.dsp
!IF "$(CFG)" == ""
CFG=fdpgen - Win32 Debug
!MESSAGE No configuration specified. Defaulting to fdpgen - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "fdpgen - Win32 Release" && "$(CFG)" != "fdpgen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fdpgen.mak" CFG="fdpgen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fdpgen - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fdpgen - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "fdpgen - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\fdpgen.lib"


CLEAN :
	-@erase "$(INTDIR)\clusteredges.obj"
	-@erase "$(INTDIR)\comp.obj"
	-@erase "$(INTDIR)\dbg.obj"
	-@erase "$(INTDIR)\fdpinit.obj"
	-@erase "$(INTDIR)\grid.obj"
	-@erase "$(INTDIR)\layout.obj"
	-@erase "$(INTDIR)\tlayout.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\xlayout.obj"
	-@erase "$(OUTDIR)\fdpgen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\common" /I "..\neatogen" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /I "..\pack" /I "..\gvc" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\fdpgen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fdpgen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\fdpgen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\comp.obj" \
	"$(INTDIR)\dbg.obj" \
	"$(INTDIR)\fdpinit.obj" \
	"$(INTDIR)\grid.obj" \
	"$(INTDIR)\layout.obj" \
	"$(INTDIR)\tlayout.obj" \
	"$(INTDIR)\xlayout.obj" \
	"$(INTDIR)\clusteredges.obj"

"$(OUTDIR)\fdpgen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\fdpgen.lib"
   copy .\Release\fdpgen.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "fdpgen - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\fdpgen.lib"


CLEAN :
	-@erase "$(INTDIR)\clusteredges.obj"
	-@erase "$(INTDIR)\comp.obj"
	-@erase "$(INTDIR)\dbg.obj"
	-@erase "$(INTDIR)\fdpinit.obj"
	-@erase "$(INTDIR)\grid.obj"
	-@erase "$(INTDIR)\layout.obj"
	-@erase "$(INTDIR)\tlayout.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xlayout.obj"
	-@erase "$(OUTDIR)\fdpgen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\neatogen" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /I "..\pack" /I "..\gvc" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\fdpgen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fdpgen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\fdpgen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\comp.obj" \
	"$(INTDIR)\dbg.obj" \
	"$(INTDIR)\fdpinit.obj" \
	"$(INTDIR)\grid.obj" \
	"$(INTDIR)\layout.obj" \
	"$(INTDIR)\tlayout.obj" \
	"$(INTDIR)\xlayout.obj" \
	"$(INTDIR)\clusteredges.obj"

"$(OUTDIR)\fdpgen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\fdpgen.lib"
   copy .\Debug\fdpgen.lib ..\lib\Debug
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
!IF EXISTS("fdpgen.dep")
!INCLUDE "fdpgen.dep"
!ELSE 
!MESSAGE Warning: cannot find "fdpgen.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fdpgen - Win32 Release" || "$(CFG)" == "fdpgen - Win32 Debug"
SOURCE=.\clusteredges.c

"$(INTDIR)\clusteredges.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\comp.c

"$(INTDIR)\comp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dbg.c

"$(INTDIR)\dbg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fdpinit.c

"$(INTDIR)\fdpinit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\grid.c

"$(INTDIR)\grid.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\layout.c

"$(INTDIR)\layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tlayout.c

"$(INTDIR)\tlayout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xlayout.c

"$(INTDIR)\xlayout.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

