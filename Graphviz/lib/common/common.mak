# Microsoft Developer Studio Generated NMAKE File, Based on common.dsp
!IF "$(CFG)" == ""
CFG=common - Win32 Debug
!MESSAGE No configuration specified. Defaulting to common - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "common - Win32 Release" && "$(CFG)" != "common - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common.mak" CFG="common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "common - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "common - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\common.lib"


CLEAN :
	-@erase "$(INTDIR)\args.obj"
	-@erase "$(INTDIR)\arrows.obj"
	-@erase "$(INTDIR)\colxlate.obj"
	-@erase "$(INTDIR)\diagen.obj"
	-@erase "$(INTDIR)\emit.obj"
	-@erase "$(INTDIR)\figgen.obj"
	-@erase "$(INTDIR)\fontmetrics.obj"
	-@erase "$(INTDIR)\gdgen.obj"
	-@erase "$(INTDIR)\gdtextsize.obj"
	-@erase "$(INTDIR)\gdusershape.obj"
	-@erase "$(INTDIR)\geom.obj"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\hpglgen.obj"
	-@erase "$(INTDIR)\htmllex.obj"
	-@erase "$(INTDIR)\htmlparse.obj"
	-@erase "$(INTDIR)\htmltable.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\labels.obj"
	-@erase "$(INTDIR)\mapgen.obj"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\mifgen.obj"
	-@erase "$(INTDIR)\mpgen.obj"
	-@erase "$(INTDIR)\ns.obj"
	-@erase "$(INTDIR)\output.obj"
	-@erase "$(INTDIR)\picgen.obj"
	-@erase "$(INTDIR)\pointset.obj"
	-@erase "$(INTDIR)\postproc.obj"
	-@erase "$(INTDIR)\psgen.obj"
	-@erase "$(INTDIR)\psusershape.obj"
	-@erase "$(INTDIR)\routespl.obj"
	-@erase "$(INTDIR)\shapes.obj"
	-@erase "$(INTDIR)\splines.obj"
	-@erase "$(INTDIR)\strcasecmp.obj"
	-@erase "$(INTDIR)\strncasecmp.obj"
	-@erase "$(INTDIR)\svggen.obj"
	-@erase "$(INTDIR)\svgusershape.obj"
	-@erase "$(INTDIR)\timing.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vrmlgen.obj"
	-@erase "$(INTDIR)\vtxgen.obj"
	-@erase "$(INTDIR)\xdgen.obj"
	-@erase "$(OUTDIR)\common.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\cdt" /I "..\fdpgen" /I "..\pathplan" /I "..\gd" /I "..\graph" /I "..\gvc" /I "..\..\third-party\include" /D "NDEBUG" /D "HAVE_SETMODE" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\common.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\common.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\common.lib" 
LIB32_OBJS= \
	"$(INTDIR)\args.obj" \
	"$(INTDIR)\arrows.obj" \
	"$(INTDIR)\colxlate.obj" \
	"$(INTDIR)\diagen.obj" \
	"$(INTDIR)\emit.obj" \
	"$(INTDIR)\figgen.obj" \
	"$(INTDIR)\fontmetrics.obj" \
	"$(INTDIR)\gdgen.obj" \
	"$(INTDIR)\gdtextsize.obj" \
	"$(INTDIR)\gdusershape.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\hpglgen.obj" \
	"$(INTDIR)\htmllex.obj" \
	"$(INTDIR)\htmlparse.obj" \
	"$(INTDIR)\htmltable.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\labels.obj" \
	"$(INTDIR)\mapgen.obj" \
	"$(INTDIR)\mifgen.obj" \
	"$(INTDIR)\mpgen.obj" \
	"$(INTDIR)\ns.obj" \
	"$(INTDIR)\output.obj" \
	"$(INTDIR)\picgen.obj" \
	"$(INTDIR)\pointset.obj" \
	"$(INTDIR)\postproc.obj" \
	"$(INTDIR)\psgen.obj" \
	"$(INTDIR)\psusershape.obj" \
	"$(INTDIR)\routespl.obj" \
	"$(INTDIR)\shapes.obj" \
	"$(INTDIR)\splines.obj" \
	"$(INTDIR)\strcasecmp.obj" \
	"$(INTDIR)\strncasecmp.obj" \
	"$(INTDIR)\svggen.obj" \
	"$(INTDIR)\svgusershape.obj" \
	"$(INTDIR)\timing.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\vrmlgen.obj" \
	"$(INTDIR)\vtxgen.obj" \
	"$(INTDIR)\xdgen.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\geom.obj"

"$(OUTDIR)\common.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\common.lib"
   copy .\Release\common.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\common.lib"


CLEAN :
	-@erase "$(INTDIR)\args.obj"
	-@erase "$(INTDIR)\arrows.obj"
	-@erase "$(INTDIR)\colxlate.obj"
	-@erase "$(INTDIR)\diagen.obj"
	-@erase "$(INTDIR)\emit.obj"
	-@erase "$(INTDIR)\figgen.obj"
	-@erase "$(INTDIR)\fontmetrics.obj"
	-@erase "$(INTDIR)\gdgen.obj"
	-@erase "$(INTDIR)\gdtextsize.obj"
	-@erase "$(INTDIR)\gdusershape.obj"
	-@erase "$(INTDIR)\geom.obj"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\hpglgen.obj"
	-@erase "$(INTDIR)\htmllex.obj"
	-@erase "$(INTDIR)\htmlparse.obj"
	-@erase "$(INTDIR)\htmltable.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\labels.obj"
	-@erase "$(INTDIR)\mapgen.obj"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\mifgen.obj"
	-@erase "$(INTDIR)\mpgen.obj"
	-@erase "$(INTDIR)\ns.obj"
	-@erase "$(INTDIR)\output.obj"
	-@erase "$(INTDIR)\picgen.obj"
	-@erase "$(INTDIR)\pointset.obj"
	-@erase "$(INTDIR)\postproc.obj"
	-@erase "$(INTDIR)\psgen.obj"
	-@erase "$(INTDIR)\psusershape.obj"
	-@erase "$(INTDIR)\routespl.obj"
	-@erase "$(INTDIR)\shapes.obj"
	-@erase "$(INTDIR)\splines.obj"
	-@erase "$(INTDIR)\strcasecmp.obj"
	-@erase "$(INTDIR)\strncasecmp.obj"
	-@erase "$(INTDIR)\svggen.obj"
	-@erase "$(INTDIR)\svgusershape.obj"
	-@erase "$(INTDIR)\timing.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vrmlgen.obj"
	-@erase "$(INTDIR)\vtxgen.obj"
	-@erase "$(INTDIR)\xdgen.obj"
	-@erase "$(OUTDIR)\common.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\cdt" /I "..\fdpgen" /I "..\pathplan" /I "..\gd" /I "..\graph" /I "..\gvc" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /D "HAVE_SETMODE" /Fp"$(INTDIR)\common.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\common.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\common.lib" 
LIB32_OBJS= \
	"$(INTDIR)\args.obj" \
	"$(INTDIR)\arrows.obj" \
	"$(INTDIR)\colxlate.obj" \
	"$(INTDIR)\diagen.obj" \
	"$(INTDIR)\emit.obj" \
	"$(INTDIR)\figgen.obj" \
	"$(INTDIR)\fontmetrics.obj" \
	"$(INTDIR)\gdgen.obj" \
	"$(INTDIR)\gdtextsize.obj" \
	"$(INTDIR)\gdusershape.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\hpglgen.obj" \
	"$(INTDIR)\htmllex.obj" \
	"$(INTDIR)\htmlparse.obj" \
	"$(INTDIR)\htmltable.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\labels.obj" \
	"$(INTDIR)\mapgen.obj" \
	"$(INTDIR)\mifgen.obj" \
	"$(INTDIR)\mpgen.obj" \
	"$(INTDIR)\ns.obj" \
	"$(INTDIR)\output.obj" \
	"$(INTDIR)\picgen.obj" \
	"$(INTDIR)\pointset.obj" \
	"$(INTDIR)\postproc.obj" \
	"$(INTDIR)\psgen.obj" \
	"$(INTDIR)\psusershape.obj" \
	"$(INTDIR)\routespl.obj" \
	"$(INTDIR)\shapes.obj" \
	"$(INTDIR)\splines.obj" \
	"$(INTDIR)\strcasecmp.obj" \
	"$(INTDIR)\strncasecmp.obj" \
	"$(INTDIR)\svggen.obj" \
	"$(INTDIR)\svgusershape.obj" \
	"$(INTDIR)\timing.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\vrmlgen.obj" \
	"$(INTDIR)\vtxgen.obj" \
	"$(INTDIR)\xdgen.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\geom.obj"

"$(OUTDIR)\common.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\common.lib"
   copy .\Debug\common.lib ..\lib\Debug
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
!IF EXISTS("common.dep")
!INCLUDE "common.dep"
!ELSE 
!MESSAGE Warning: cannot find "common.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "common - Win32 Release" || "$(CFG)" == "common - Win32 Debug"
SOURCE=.\args.c

"$(INTDIR)\args.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\arrows.c

"$(INTDIR)\arrows.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\colxlate.c

"$(INTDIR)\colxlate.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\diagen.c

"$(INTDIR)\diagen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\emit.c

"$(INTDIR)\emit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\figgen.c

"$(INTDIR)\figgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fontmetrics.c

"$(INTDIR)\fontmetrics.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gdgen.c

"$(INTDIR)\gdgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gdtextsize.c

"$(INTDIR)\gdtextsize.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gdusershape.c

"$(INTDIR)\gdusershape.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\geom.c

"$(INTDIR)\geom.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\globals.c

"$(INTDIR)\globals.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hpglgen.c

"$(INTDIR)\hpglgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\htmllex.c

"$(INTDIR)\htmllex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\htmlparse.c

"$(INTDIR)\htmlparse.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\htmltable.c

"$(INTDIR)\htmltable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\input.c

!IF  "$(CFG)" == "common - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\cdt" /I "..\fdpgen" /I "..\pathplan" /I "..\gd" /I "..\graph" /I "..\gvc" /I "..\..\third-party\include" /D "NDEBUG" /D "HAVE_SETMODE" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\common.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\input.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "common - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W4 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\cdt" /I "..\fdpgen" /I "..\pathplan" /I "..\gd" /I "..\graph" /I "..\gvc" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /D "HAVE_SETMODE" /Fp"$(INTDIR)\common.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\input.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\labels.c

"$(INTDIR)\labels.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mapgen.c

"$(INTDIR)\mapgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\memory.c

"$(INTDIR)\memory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mifgen.c

"$(INTDIR)\mifgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mpgen.c

"$(INTDIR)\mpgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ns.c

"$(INTDIR)\ns.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\output.c

"$(INTDIR)\output.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\picgen.c

"$(INTDIR)\picgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pointset.c

"$(INTDIR)\pointset.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\postproc.c

"$(INTDIR)\postproc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\psgen.c

"$(INTDIR)\psgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\psusershape.c

"$(INTDIR)\psusershape.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\routespl.c

"$(INTDIR)\routespl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\shapes.c

"$(INTDIR)\shapes.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\splines.c

"$(INTDIR)\splines.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\strcasecmp.c

"$(INTDIR)\strcasecmp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\strncasecmp.c

"$(INTDIR)\strncasecmp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\svggen.c

"$(INTDIR)\svggen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\svgusershape.c

"$(INTDIR)\svgusershape.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\timing.c

"$(INTDIR)\timing.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\utils.c

"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\vrmlgen.c

"$(INTDIR)\vrmlgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\vtxgen.c

"$(INTDIR)\vtxgen.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xdgen.c

"$(INTDIR)\xdgen.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

