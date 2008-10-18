# Microsoft Developer Studio Generated NMAKE File, Based on neatogen.dsp
!IF "$(CFG)" == ""
CFG=neatogen - Win32 Debug
!MESSAGE No configuration specified. Defaulting to neatogen - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "neatogen - Win32 Release" && "$(CFG)" != "neatogen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "neatogen.mak" CFG="neatogen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "neatogen - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "neatogen - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "neatogen - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\neatogen.lib"


CLEAN :
	-@erase "$(INTDIR)\adjust.obj"
	-@erase "$(INTDIR)\bfs.obj"
	-@erase "$(INTDIR)\circuit.obj"
	-@erase "$(INTDIR)\closest.obj"
	-@erase "$(INTDIR)\compute_hierarchy.obj"
	-@erase "$(INTDIR)\conjgrad.obj"
	-@erase "$(INTDIR)\constrained_majorization.obj"
	-@erase "$(INTDIR)\constraint.obj"
	-@erase "$(INTDIR)\dijkstra.obj"
	-@erase "$(INTDIR)\edges.obj"
	-@erase "$(INTDIR)\embed_graph.obj"
	-@erase "$(INTDIR)\find_ints.obj"
	-@erase "$(INTDIR)\geometry.obj"
	-@erase "$(INTDIR)\heap.obj"
	-@erase "$(INTDIR)\hedges.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\intersect.obj"
	-@erase "$(INTDIR)\kkutils.obj"
	-@erase "$(INTDIR)\legal.obj"
	-@erase "$(INTDIR)\lu.obj"
	-@erase "$(INTDIR)\matinv.obj"
	-@erase "$(INTDIR)\matrix_ops.obj"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\neatoinit.obj"
	-@erase "$(INTDIR)\neatosplines.obj"
	-@erase "$(INTDIR)\opt_arrangement.obj"
	-@erase "$(INTDIR)\pca.obj"
	-@erase "$(INTDIR)\poly.obj"
	-@erase "$(INTDIR)\printvis.obj"
	-@erase "$(INTDIR)\quad_prog_solve.obj"
	-@erase "$(INTDIR)\site.obj"
	-@erase "$(INTDIR)\smart_ini_x.obj"
	-@erase "$(INTDIR)\solve.obj"
	-@erase "$(INTDIR)\stress.obj"
	-@erase "$(INTDIR)\stuff.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\voronoi.obj"
	-@erase "$(OUTDIR)\neatogen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\neatogen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\neatogen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\neatogen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\adjust.obj" \
	"$(INTDIR)\bfs.obj" \
	"$(INTDIR)\circuit.obj" \
	"$(INTDIR)\closest.obj" \
	"$(INTDIR)\conjgrad.obj" \
	"$(INTDIR)\constraint.obj" \
	"$(INTDIR)\dijkstra.obj" \
	"$(INTDIR)\edges.obj" \
	"$(INTDIR)\embed_graph.obj" \
	"$(INTDIR)\find_ints.obj" \
	"$(INTDIR)\geometry.obj" \
	"$(INTDIR)\heap.obj" \
	"$(INTDIR)\hedges.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\intersect.obj" \
	"$(INTDIR)\kkutils.obj" \
	"$(INTDIR)\legal.obj" \
	"$(INTDIR)\lu.obj" \
	"$(INTDIR)\matinv.obj" \
	"$(INTDIR)\matrix_ops.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\neatoinit.obj" \
	"$(INTDIR)\neatosplines.obj" \
	"$(INTDIR)\pca.obj" \
	"$(INTDIR)\poly.obj" \
	"$(INTDIR)\printvis.obj" \
	"$(INTDIR)\site.obj" \
	"$(INTDIR)\solve.obj" \
	"$(INTDIR)\stress.obj" \
	"$(INTDIR)\stuff.obj" \
	"$(INTDIR)\voronoi.obj" \
	"$(INTDIR)\opt_arrangement.obj" \
	"$(INTDIR)\quad_prog_solve.obj" \
	"$(INTDIR)\smart_ini_x.obj" \
	"$(INTDIR)\compute_hierarchy.obj" \
	"$(INTDIR)\constrained_majorization.obj"

"$(OUTDIR)\neatogen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\neatogen.lib"
   copy .\Release\neatogen.lib ..\lib\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "neatogen - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\neatogen.lib"


CLEAN :
	-@erase "$(INTDIR)\adjust.obj"
	-@erase "$(INTDIR)\bfs.obj"
	-@erase "$(INTDIR)\circuit.obj"
	-@erase "$(INTDIR)\closest.obj"
	-@erase "$(INTDIR)\compute_hierarchy.obj"
	-@erase "$(INTDIR)\conjgrad.obj"
	-@erase "$(INTDIR)\constrained_majorization.obj"
	-@erase "$(INTDIR)\constraint.obj"
	-@erase "$(INTDIR)\dijkstra.obj"
	-@erase "$(INTDIR)\edges.obj"
	-@erase "$(INTDIR)\embed_graph.obj"
	-@erase "$(INTDIR)\find_ints.obj"
	-@erase "$(INTDIR)\geometry.obj"
	-@erase "$(INTDIR)\heap.obj"
	-@erase "$(INTDIR)\hedges.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\intersect.obj"
	-@erase "$(INTDIR)\kkutils.obj"
	-@erase "$(INTDIR)\legal.obj"
	-@erase "$(INTDIR)\lu.obj"
	-@erase "$(INTDIR)\matinv.obj"
	-@erase "$(INTDIR)\matrix_ops.obj"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\neatoinit.obj"
	-@erase "$(INTDIR)\neatosplines.obj"
	-@erase "$(INTDIR)\opt_arrangement.obj"
	-@erase "$(INTDIR)\pca.obj"
	-@erase "$(INTDIR)\poly.obj"
	-@erase "$(INTDIR)\printvis.obj"
	-@erase "$(INTDIR)\quad_prog_solve.obj"
	-@erase "$(INTDIR)\site.obj"
	-@erase "$(INTDIR)\smart_ini_x.obj"
	-@erase "$(INTDIR)\solve.obj"
	-@erase "$(INTDIR)\stress.obj"
	-@erase "$(INTDIR)\stuff.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\voronoi.obj"
	-@erase "$(OUTDIR)\neatogen.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\neatogen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\neatogen.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\neatogen.lib" 
LIB32_OBJS= \
	"$(INTDIR)\adjust.obj" \
	"$(INTDIR)\bfs.obj" \
	"$(INTDIR)\circuit.obj" \
	"$(INTDIR)\closest.obj" \
	"$(INTDIR)\conjgrad.obj" \
	"$(INTDIR)\constraint.obj" \
	"$(INTDIR)\dijkstra.obj" \
	"$(INTDIR)\edges.obj" \
	"$(INTDIR)\embed_graph.obj" \
	"$(INTDIR)\find_ints.obj" \
	"$(INTDIR)\geometry.obj" \
	"$(INTDIR)\heap.obj" \
	"$(INTDIR)\hedges.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\intersect.obj" \
	"$(INTDIR)\kkutils.obj" \
	"$(INTDIR)\legal.obj" \
	"$(INTDIR)\lu.obj" \
	"$(INTDIR)\matinv.obj" \
	"$(INTDIR)\matrix_ops.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\neatoinit.obj" \
	"$(INTDIR)\neatosplines.obj" \
	"$(INTDIR)\pca.obj" \
	"$(INTDIR)\poly.obj" \
	"$(INTDIR)\printvis.obj" \
	"$(INTDIR)\site.obj" \
	"$(INTDIR)\solve.obj" \
	"$(INTDIR)\stress.obj" \
	"$(INTDIR)\stuff.obj" \
	"$(INTDIR)\voronoi.obj" \
	"$(INTDIR)\opt_arrangement.obj" \
	"$(INTDIR)\quad_prog_solve.obj" \
	"$(INTDIR)\smart_ini_x.obj" \
	"$(INTDIR)\compute_hierarchy.obj" \
	"$(INTDIR)\constrained_majorization.obj"

"$(OUTDIR)\neatogen.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\neatogen.lib"
   copy .\Debug\neatogen.lib ..\lib\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("neatogen.dep")
!INCLUDE "neatogen.dep"
!ELSE 
!MESSAGE Warning: cannot find "neatogen.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "neatogen - Win32 Release" || "$(CFG)" == "neatogen - Win32 Debug"
SOURCE=.\adjust.c

"$(INTDIR)\adjust.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bfs.c

"$(INTDIR)\bfs.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\circuit.c

"$(INTDIR)\circuit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\closest.c

"$(INTDIR)\closest.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\compute_hierarchy.c

"$(INTDIR)\compute_hierarchy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\conjgrad.c

"$(INTDIR)\conjgrad.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\constrained_majorization.c

"$(INTDIR)\constrained_majorization.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\constraint.c

"$(INTDIR)\constraint.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dijkstra.c

"$(INTDIR)\dijkstra.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\edges.c

"$(INTDIR)\edges.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\embed_graph.c

"$(INTDIR)\embed_graph.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\find_ints.c

"$(INTDIR)\find_ints.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\geometry.c

"$(INTDIR)\geometry.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\heap.c

"$(INTDIR)\heap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hedges.c

"$(INTDIR)\hedges.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\info.c

"$(INTDIR)\info.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\intersect.c

"$(INTDIR)\intersect.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\kkutils.c

"$(INTDIR)\kkutils.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\legal.c

"$(INTDIR)\legal.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lu.c

"$(INTDIR)\lu.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\matinv.c

"$(INTDIR)\matinv.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\matrix_ops.c

"$(INTDIR)\matrix_ops.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\memory.c

"$(INTDIR)\memory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\neatoinit.c

"$(INTDIR)\neatoinit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\neatosplines.c

!IF  "$(CFG)" == "neatogen - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\neatogen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\neatosplines.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "neatogen - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /Fp"$(INTDIR)\neatogen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\neatosplines.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\opt_arrangement.c

"$(INTDIR)\opt_arrangement.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pca.c

"$(INTDIR)\pca.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\poly.c

"$(INTDIR)\poly.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\printvis.c

"$(INTDIR)\printvis.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\quad_prog_solve.c

"$(INTDIR)\quad_prog_solve.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\site.c

"$(INTDIR)\site.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\smart_ini_x.c

"$(INTDIR)\smart_ini_x.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\solve.c

"$(INTDIR)\solve.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stress.c

"$(INTDIR)\stress.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stuff.c

"$(INTDIR)\stuff.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\voronoi.c

"$(INTDIR)\voronoi.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

