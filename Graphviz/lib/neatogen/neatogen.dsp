# Microsoft Developer Studio Project File - Name="neatogen" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=neatogen - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "neatogen.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "neatogen - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\neatogen.lib ..\lib\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "neatogen - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "." /I "..\.." /I "..\common" /I "..\pack" /I "..\gvc" /I "..\gd" /I "..\cdt" /I "..\graph" /I "..\pathplan" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\neatogen.lib ..\lib\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "neatogen - Win32 Release"
# Name "neatogen - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\adjust.c
# End Source File
# Begin Source File

SOURCE=.\bfs.c
# End Source File
# Begin Source File

SOURCE=.\circuit.c
# End Source File
# Begin Source File

SOURCE=.\closest.c
# End Source File
# Begin Source File

SOURCE=.\compute_hierarchy.c
# End Source File
# Begin Source File

SOURCE=.\conjgrad.c
# End Source File
# Begin Source File

SOURCE=.\constrained_majorization.c
# End Source File
# Begin Source File

SOURCE=.\constraint.c
# End Source File
# Begin Source File

SOURCE=.\dijkstra.c
# End Source File
# Begin Source File

SOURCE=.\edges.c
# End Source File
# Begin Source File

SOURCE=.\embed_graph.c
# End Source File
# Begin Source File

SOURCE=.\find_ints.c
# End Source File
# Begin Source File

SOURCE=.\geometry.c
# End Source File
# Begin Source File

SOURCE=.\heap.c
# End Source File
# Begin Source File

SOURCE=.\hedges.c
# End Source File
# Begin Source File

SOURCE=.\info.c
# End Source File
# Begin Source File

SOURCE=.\intersect.c
# End Source File
# Begin Source File

SOURCE=.\kkutils.c
# End Source File
# Begin Source File

SOURCE=.\legal.c
# End Source File
# Begin Source File

SOURCE=.\lu.c
# End Source File
# Begin Source File

SOURCE=.\matinv.c
# End Source File
# Begin Source File

SOURCE=.\matrix_ops.c
# End Source File
# Begin Source File

SOURCE=.\memory.c
# End Source File
# Begin Source File

SOURCE=.\neatoinit.c
# End Source File
# Begin Source File

SOURCE=.\neatosplines.c

!IF  "$(CFG)" == "neatogen - Win32 Release"

# SUBTRACT CPP /O<none>

!ELSEIF  "$(CFG)" == "neatogen - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\opt_arrangement.c
# End Source File
# Begin Source File

SOURCE=.\pca.c
# End Source File
# Begin Source File

SOURCE=.\poly.c
# End Source File
# Begin Source File

SOURCE=.\printvis.c
# End Source File
# Begin Source File

SOURCE=.\quad_prog_solve.c
# End Source File
# Begin Source File

SOURCE=.\site.c
# End Source File
# Begin Source File

SOURCE=.\smart_ini_x.c
# End Source File
# Begin Source File

SOURCE=.\solve.c
# End Source File
# Begin Source File

SOURCE=.\stress.c
# End Source File
# Begin Source File

SOURCE=.\stuff.c
# End Source File
# Begin Source File

SOURCE=.\voronoi.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\adjust.h
# End Source File
# Begin Source File

SOURCE=.\digcola.h
# End Source File
# Begin Source File

SOURCE=.\edges.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\heap.h
# End Source File
# Begin Source File

SOURCE=.\hedges.h
# End Source File
# Begin Source File

SOURCE=.\info.h
# End Source File
# Begin Source File

SOURCE=.\mem.h
# End Source File
# Begin Source File

SOURCE=.\neato.h
# End Source File
# Begin Source File

SOURCE=.\neatoprocs.h
# End Source File
# Begin Source File

SOURCE=.\poly.h
# End Source File
# Begin Source File

SOURCE=.\quad_prog_solver.h
# End Source File
# Begin Source File

SOURCE=.\simple.h
# End Source File
# Begin Source File

SOURCE=.\site.h
# End Source File
# Begin Source File

SOURCE=.\voronoi.h
# End Source File
# End Group
# End Target
# End Project
