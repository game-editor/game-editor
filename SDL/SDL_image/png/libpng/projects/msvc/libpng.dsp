# Microsoft Developer Studio Project File - Name="libpng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LIBPNG - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak" CFG="LIBPNG - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_image/png/libpng/projects/msvc", ENLAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpng - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../../../output/Windows/png"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_LIB" /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "..\.." /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /Yu"png.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../../../output/Windows/png"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Zi /Od /D "_DEBUG" /D "WIN32" /D "_LIB" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\.." /I "..\..\..\zlib" /D "DEBUG" /D "_DEBUG" /D PNG_DEBUG=1 /D "WIN32" /FR /Yu"png.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libpng - Win32 Release"
# Name "libpng - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\png.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\png.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\png32ms.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\pngerror.c
# ADD CPP /Yc"png.h"
# End Source File
# Begin Source File

SOURCE=..\..\pngget.c
# End Source File
# Begin Source File

SOURCE=..\..\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\..\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\..\pngread.c
# End Source File
# Begin Source File

SOURCE=..\..\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\..\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\..\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\pngset.c
# End Source File
# Begin Source File

SOURCE=..\..\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\pngvcrd.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\..\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\..\pngwutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\png.h
# End Source File
# Begin Source File

SOURCE=..\..\pngconf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\readme.txt
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
