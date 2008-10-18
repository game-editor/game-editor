# Microsoft Developer Studio Generated NMAKE File, Based on gvc.dsp
!IF "$(CFG)" == ""
CFG=gvc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to gvc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "gvc - Win32 Release" && "$(CFG)" != "gvc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gvc.mak" CFG="gvc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gvc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gvc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "gvc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\gvc.lib"


CLEAN :
	-@erase "$(INTDIR)\dot_builtins.obj"
	-@erase "$(INTDIR)\gvc.obj"
	-@erase "$(INTDIR)\gvconfig.obj"
	-@erase "$(INTDIR)\gvcontext.obj"
	-@erase "$(INTDIR)\gvdevice.obj"
	-@erase "$(INTDIR)\gvevent.obj"
	-@erase "$(INTDIR)\gvjobs.obj"
	-@erase "$(INTDIR)\gvlayout.obj"
	-@erase "$(INTDIR)\gvplugin.obj"
	-@erase "$(INTDIR)\gvrender.obj"
	-@erase "$(INTDIR)\gvtextlayout.obj"
	-@erase "$(INTDIR)\gvusershape.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\gvc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\..\lib\cdt" /I "..\..\lib\pathplan" /I "..\..\lib\common" /I "..\..\lib\graph" /I "..\..\third-party\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\gvc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gvc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\gvc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvc.obj" \
	"$(INTDIR)\gvconfig.obj" \
	"$(INTDIR)\gvcontext.obj" \
	"$(INTDIR)\gvdevice.obj" \
	"$(INTDIR)\gvevent.obj" \
	"$(INTDIR)\gvjobs.obj" \
	"$(INTDIR)\gvlayout.obj" \
	"$(INTDIR)\gvplugin.obj" \
	"$(INTDIR)\gvrender.obj" \
	"$(INTDIR)\gvtextlayout.obj" \
	"$(INTDIR)\gvusershape.obj" \
	"$(INTDIR)\dot_builtins.obj"

"$(OUTDIR)\gvc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\gvc.lib"
   copy .\Release\gvc.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "gvc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\gvc.lib"


CLEAN :
	-@erase "$(INTDIR)\dot_builtins.obj"
	-@erase "$(INTDIR)\gvc.obj"
	-@erase "$(INTDIR)\gvconfig.obj"
	-@erase "$(INTDIR)\gvcontext.obj"
	-@erase "$(INTDIR)\gvdevice.obj"
	-@erase "$(INTDIR)\gvevent.obj"
	-@erase "$(INTDIR)\gvjobs.obj"
	-@erase "$(INTDIR)\gvlayout.obj"
	-@erase "$(INTDIR)\gvplugin.obj"
	-@erase "$(INTDIR)\gvrender.obj"
	-@erase "$(INTDIR)\gvtextlayout.obj"
	-@erase "$(INTDIR)\gvusershape.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gvc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\..\lib\cdt" /I "..\..\lib\pathplan" /I "..\..\lib\common" /I "..\..\lib\graph" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\gvc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gvc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\gvc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvc.obj" \
	"$(INTDIR)\gvconfig.obj" \
	"$(INTDIR)\gvcontext.obj" \
	"$(INTDIR)\gvdevice.obj" \
	"$(INTDIR)\gvevent.obj" \
	"$(INTDIR)\gvjobs.obj" \
	"$(INTDIR)\gvlayout.obj" \
	"$(INTDIR)\gvplugin.obj" \
	"$(INTDIR)\gvrender.obj" \
	"$(INTDIR)\gvtextlayout.obj" \
	"$(INTDIR)\gvusershape.obj" \
	"$(INTDIR)\dot_builtins.obj"

"$(OUTDIR)\gvc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\gvc.lib"
   copy .\Debug\gvc.lib ..\lib\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("gvc.dep")
!INCLUDE "gvc.dep"
!ELSE 
!MESSAGE Warning: cannot find "gvc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "gvc - Win32 Release" || "$(CFG)" == "gvc - Win32 Debug"
SOURCE=.\dot_builtins.c

"$(INTDIR)\dot_builtins.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvc.c

"$(INTDIR)\gvc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvconfig.c

"$(INTDIR)\gvconfig.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvcontext.c

"$(INTDIR)\gvcontext.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvdevice.c

"$(INTDIR)\gvdevice.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvevent.c

"$(INTDIR)\gvevent.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvjobs.c

"$(INTDIR)\gvjobs.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvlayout.c

"$(INTDIR)\gvlayout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin.c

"$(INTDIR)\gvplugin.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvrender.c

"$(INTDIR)\gvrender.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvtextlayout.c

"$(INTDIR)\gvtextlayout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvusershape.c

"$(INTDIR)\gvusershape.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

