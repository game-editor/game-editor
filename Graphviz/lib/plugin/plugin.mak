# Microsoft Developer Studio Generated NMAKE File, Based on plugin.dsp
!IF "$(CFG)" == ""
CFG=plugin - Win32 Debug
!MESSAGE No configuration specified. Defaulting to plugin - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "plugin - Win32 Release" && "$(CFG)" != "plugin - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plugin.mak" CFG="plugin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plugin - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "plugin - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "plugin - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\plugin.lib"


CLEAN :
	-@erase "$(INTDIR)\gvlayout_dot_layout.obj"
	-@erase "$(INTDIR)\gvlayout_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_dot_layout.obj"
	-@erase "$(INTDIR)\gvplugin_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_usershape_gd.obj"
	-@erase "$(INTDIR)\gvusershape_gd.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\plugin.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\common" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\plugin.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\plugin.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\plugin.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvusershape_gd.obj" \
	"$(INTDIR)\gvlayout_dot_layout.obj" \
	"$(INTDIR)\gvlayout_neato_layout.obj" \
	"$(INTDIR)\gvplugin_dot_layout.obj" \
	"$(INTDIR)\gvplugin_neato_layout.obj" \
	"$(INTDIR)\gvplugin_usershape_gd.obj"

"$(OUTDIR)\plugin.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\plugin.lib"
   copy .\Release\plugin.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "plugin - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\plugin.lib"


CLEAN :
	-@erase "$(INTDIR)\gvlayout_dot_layout.obj"
	-@erase "$(INTDIR)\gvlayout_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_dot_layout.obj"
	-@erase "$(INTDIR)\gvplugin_neato_layout.obj"
	-@erase "$(INTDIR)\gvplugin_usershape_gd.obj"
	-@erase "$(INTDIR)\gvusershape_gd.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\plugin.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\plugin.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\plugin.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\plugin.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gvusershape_gd.obj" \
	"$(INTDIR)\gvlayout_dot_layout.obj" \
	"$(INTDIR)\gvlayout_neato_layout.obj" \
	"$(INTDIR)\gvplugin_dot_layout.obj" \
	"$(INTDIR)\gvplugin_neato_layout.obj" \
	"$(INTDIR)\gvplugin_usershape_gd.obj"

"$(OUTDIR)\plugin.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\plugin.lib"
   copy .\Debug\plugin.lib ..\lib\Debug
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
!IF EXISTS("plugin.dep")
!INCLUDE "plugin.dep"
!ELSE 
!MESSAGE Warning: cannot find "plugin.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "plugin - Win32 Release" || "$(CFG)" == "plugin - Win32 Debug"
SOURCE=.\gvlayout_dot_layout.c

"$(INTDIR)\gvlayout_dot_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvlayout_neato_layout.c

"$(INTDIR)\gvlayout_neato_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin_dot_layout.c

"$(INTDIR)\gvplugin_dot_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin_neato_layout.c

"$(INTDIR)\gvplugin_neato_layout.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvplugin_usershape_gd.c

"$(INTDIR)\gvplugin_usershape_gd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gvusershape_gd.c

"$(INTDIR)\gvusershape_gd.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

