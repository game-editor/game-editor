# Microsoft Developer Studio Generated NMAKE File, Based on dotgen.dsp
!IF "$(CFG)" == ""
CFG=dotgen - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dotgen - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dotgen - Win32 Release" && "$(CFG)" != "dotgen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dotgen.mak" CFG="dotgen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dotgen - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "dotgen - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dotgen - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\dotgen.lib"


CLEAN :
	-@erase "$(INTDIR)\acyclic.obj"
	-@erase "$(INTDIR)\class1.obj"
	-@erase "$(INTDIR)\class2.obj"
	-@erase "$(INTDIR)\cluster.obj"
	-@erase "$(INTDIR)\compound.obj"
	-@erase "$(INTDIR)\conc.obj"
	-@erase "$(INTDIR)\decomp.obj"
	-@erase "$(INTDIR)\dotinit.obj"
	-@erase "$(INTDIR)\dotsplines.obj"
	-@erase "$(INTDIR)\fastgr.obj"
	-@erase "$(INTDIR)\flat.obj"
	-@erase "$(INTDIR)\mincross.obj"
	-@erase "$(INTDIR)\position.obj"
	-@erase "$(INTDIR)\rank.obj"
	-@erase "$(INTDIR)\sameport.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dotgen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\common" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /I "..\gvc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\dotgen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dotgen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\dotgen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\acyclic.obj" \
	"$(INTDIR)\class1.obj" \
	"$(INTDIR)\class2.obj" \
	"$(INTDIR)\cluster.obj" \
	"$(INTDIR)\compound.obj" \
	"$(INTDIR)\conc.obj" \
	"$(INTDIR)\decomp.obj" \
	"$(INTDIR)\dotinit.obj" \
	"$(INTDIR)\dotsplines.obj" \
	"$(INTDIR)\fastgr.obj" \
	"$(INTDIR)\flat.obj" \
	"$(INTDIR)\mincross.obj" \
	"$(INTDIR)\position.obj" \
	"$(INTDIR)\rank.obj" \
	"$(INTDIR)\sameport.obj"

"$(OUTDIR)\dotgen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\dotgen.lib"
   copy .\Release\dotgen.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "dotgen - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\dotgen.lib"


CLEAN :
	-@erase "$(INTDIR)\acyclic.obj"
	-@erase "$(INTDIR)\class1.obj"
	-@erase "$(INTDIR)\class2.obj"
	-@erase "$(INTDIR)\cluster.obj"
	-@erase "$(INTDIR)\compound.obj"
	-@erase "$(INTDIR)\conc.obj"
	-@erase "$(INTDIR)\decomp.obj"
	-@erase "$(INTDIR)\dotinit.obj"
	-@erase "$(INTDIR)\dotsplines.obj"
	-@erase "$(INTDIR)\fastgr.obj"
	-@erase "$(INTDIR)\flat.obj"
	-@erase "$(INTDIR)\mincross.obj"
	-@erase "$(INTDIR)\position.obj"
	-@erase "$(INTDIR)\rank.obj"
	-@erase "$(INTDIR)\sameport.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dotgen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /I "..\gvc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\dotgen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dotgen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\dotgen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\acyclic.obj" \
	"$(INTDIR)\class1.obj" \
	"$(INTDIR)\class2.obj" \
	"$(INTDIR)\cluster.obj" \
	"$(INTDIR)\compound.obj" \
	"$(INTDIR)\conc.obj" \
	"$(INTDIR)\decomp.obj" \
	"$(INTDIR)\dotinit.obj" \
	"$(INTDIR)\dotsplines.obj" \
	"$(INTDIR)\fastgr.obj" \
	"$(INTDIR)\flat.obj" \
	"$(INTDIR)\mincross.obj" \
	"$(INTDIR)\position.obj" \
	"$(INTDIR)\rank.obj" \
	"$(INTDIR)\sameport.obj"

"$(OUTDIR)\dotgen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\dotgen.lib"
   copy .\Debug\dotgen.lib ..\lib\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dotgen.dep")
!INCLUDE "dotgen.dep"
!ELSE 
!MESSAGE Warning: cannot find "dotgen.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dotgen - Win32 Release" || "$(CFG)" == "dotgen - Win32 Debug"
SOURCE=.\acyclic.c

"$(INTDIR)\acyclic.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\class1.c

"$(INTDIR)\class1.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\class2.c

"$(INTDIR)\class2.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster.c

"$(INTDIR)\cluster.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\compound.c

"$(INTDIR)\compound.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\conc.c

"$(INTDIR)\conc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\decomp.c

"$(INTDIR)\decomp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dotinit.c

"$(INTDIR)\dotinit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dotsplines.c

"$(INTDIR)\dotsplines.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fastgr.c

"$(INTDIR)\fastgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\flat.c

"$(INTDIR)\flat.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mincross.c

"$(INTDIR)\mincross.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\position.c

"$(INTDIR)\position.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rank.c

"$(INTDIR)\rank.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sameport.c

"$(INTDIR)\sameport.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

