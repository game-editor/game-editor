# Microsoft Developer Studio Project File - Name="wxIFM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxIFM - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxIFM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxIFM.mak" CFG="wxIFM - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxIFM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxIFM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxIFM - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxIFM - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/wxGameEditor", BQPBAAAA"
# PROP Scc_LocalPath "..\..\..\gamedevelopment\wxgameeditor"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxIFM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib/vc_lib/"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /Zi /O2 /D "WIN32" /D "_LIB" /D "NDEBUG" /Fr /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc_lib/wxmsw26_ifm.lib"

!ELSEIF  "$(CFG)" == "wxIFM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "wxIFM___Win32_Debug"
# PROP BASE Intermediate_Dir "wxIFM___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib/vc_lib/"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc_lib/wxmsw26d_ifm.lib"

!ELSEIF  "$(CFG)" == "wxIFM - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "wxIFM___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "wxIFM___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib/vc_lib/"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D wxUSE_UNICODE=1 /D "WIN32" /D "_LIB" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/vc_lib/wxmsw25d_ifm.lib"
# ADD LIB32 /nologo /out:"lib/vc_lib/wxmsw25ud_ifm.lib"

!ELSEIF  "$(CFG)" == "wxIFM - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "wxIFM___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "wxIFM___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib/vc_lib/"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W4 /GR /GX /O2 /D "WIN32" /D "_MBCS" /D "_LIB" /D "NDEBUG" /Fr /YX /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /D "NDEBUG" /D wxUSE_UNICODE=1 /D "WIN32" /D "_LIB" /Fr /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib/vc_lib/wxmsw25_ifm.lib"
# ADD LIB32 /nologo /out:"lib/vc_lib/wxmsw25u_ifm.lib"

!ENDIF 

# Begin Target

# Name "wxIFM - Win32 Release"
# Name "wxIFM - Win32 Debug"
# Name "wxIFM - Win32 Unicode Debug"
# Name "wxIFM - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ifm\definterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\defplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\dragndrop.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\events.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\manager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ifm\resize.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\include\wx\ifm\definterface.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\defplugin.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\dragndrop.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\events.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\ifm.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\ifmdefs.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\manager.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\plugin.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\ifm\resize.h
# End Source File
# End Group
# Begin Group "docs"

# PROP Default_Filter ".dox"
# Begin Source File

SOURCE=.\docs\coordsystems.dox
# End Source File
# Begin Source File

SOURCE=.\docs\internals.dox
# End Source File
# Begin Source File

SOURCE=.\docs\main.dox
# End Source File
# Begin Source File

SOURCE=.\docs\overview.dox
# End Source File
# End Group
# Begin Source File

SOURCE=.\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
