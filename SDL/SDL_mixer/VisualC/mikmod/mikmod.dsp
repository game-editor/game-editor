# Microsoft Developer Studio Project File - Name="mikmod" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mikmod - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mikmod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mikmod.mak" CFG="mikmod - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mikmod - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mikmod - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_mixer/mikmod", TYIAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mikmod - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../output/Windows/mikmod"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\mikmod" /I "../../../SDL/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "USE_RWOPS" /YX /FD /c
# ADD BASE RSC /l 0x416
# ADD RSC /l 0x416
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "mikmod - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../output/Windows/mikmod"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\..\mikmod" /I "../../../SDL/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "USE_RWOPS" /YX /FD /c
# ADD BASE RSC /l 0x416
# ADD RSC /l 0x416
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "mikmod - Win32 Release"
# Name "mikmod - Win32 Debug"
# Begin Source File

SOURCE=..\..\mikmod\drv_nos.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\drv_sdl.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\load_it.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\load_mod.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\load_s3m.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\load_xm.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mdreg.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mdriver.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mikmod.h
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mikmod_build.h
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mikmod_internals.h
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mloader.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mlreg.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mlutil.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mmalloc.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mmerror.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mmio.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mplayer.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\munitrk.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\mwav.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\npertab.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\sloader.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\virtch.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\virtch2.c
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\virtch_common.c
# End Source File
# End Target
# End Project
