# Microsoft Developer Studio Project File - Name="gvc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gvc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gvc.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gvc - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "." /I "..\.." /I "..\..\lib\cdt" /I "..\..\lib\pathplan" /I "..\..\lib\common" /I "..\..\lib\graph" /I "..\..\third-party\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /YX /FD /c
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
PostBuild_Cmds=copy .\Release\gvc.lib ..\lib\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gvc - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "..\.." /I "..\..\lib\cdt" /I "..\..\lib\pathplan" /I "..\..\lib\common" /I "..\..\lib\graph" /I "..\..\third-party\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "MSWIN32" /D "HAVE_CONFIG_H" /YX /FD /GZ /c
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
PostBuild_Cmds=copy .\Debug\gvc.lib ..\lib\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "gvc - Win32 Release"
# Name "gvc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dot_builtins.c
# End Source File
# Begin Source File

SOURCE=.\gvc.c
# End Source File
# Begin Source File

SOURCE=.\gvconfig.c
# End Source File
# Begin Source File

SOURCE=.\gvcontext.c
# End Source File
# Begin Source File

SOURCE=.\gvdevice.c
# End Source File
# Begin Source File

SOURCE=.\gvevent.c
# End Source File
# Begin Source File

SOURCE=.\gvjobs.c
# End Source File
# Begin Source File

SOURCE=.\gvlayout.c
# End Source File
# Begin Source File

SOURCE=.\gvplugin.c
# End Source File
# Begin Source File

SOURCE=.\gvrender.c
# End Source File
# Begin Source File

SOURCE=.\gvtextlayout.c
# End Source File
# Begin Source File

SOURCE=.\gvusershape.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\gvc.h
# End Source File
# Begin Source File

SOURCE=.\gvcint.h
# End Source File
# Begin Source File

SOURCE=.\gvcproc.h
# End Source File
# Begin Source File

SOURCE=.\gvplugin.h
# End Source File
# Begin Source File

SOURCE=.\gvplugin_display.h
# End Source File
# Begin Source File

SOURCE=.\gvplugin_layout.h
# End Source File
# Begin Source File

SOURCE=.\gvplugin_render.h
# End Source File
# End Group
# End Target
# End Project
