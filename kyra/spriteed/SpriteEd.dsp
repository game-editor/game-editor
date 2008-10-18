# Microsoft Developer Studio Project File - Name="SpriteEd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SpriteEd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpriteEd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpriteEd.mak" CFG="SpriteEd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpriteEd - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SpriteEd - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SpriteEd"
# PROP Scc_LocalPath "."
# PROP WCE_FormatVersion ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpriteEd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../output"
# PROP Intermediate_Dir "../../output"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../SDL/SDL/include" /I "../../SDL/SDL_image" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "KYRA_SUPPORT_OPENGL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib sdl.lib sdlmain.lib sdl_image.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Release/krspriteed.exe" /libpath:"c:\bin"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "SpriteEd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../output"
# PROP Intermediate_Dir "../../output"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../../SDL/SDL/include" /I "../../SDL/SDL_image" /D "_DEBUG" /D "_WINDOWS" /D "DEBUG" /D "WIN32" /D "_MBCS" /D "KYRA_SUPPORT_OPENGL" /Fr /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib sdl.lib sdlmain.lib sdl_image.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"Debug/krspriteed.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SpriteEd - Win32 Release"
# Name "SpriteEd - Win32 Debug"
# Begin Source File

SOURCE=.\CONSOLEFONT.cpp
# End Source File
# Begin Source File

SOURCE=.\CONSOLEFONT.h
# End Source File
# Begin Source File

SOURCE=.\dom.cpp
# End Source File
# Begin Source File

SOURCE=.\dom.h
# End Source File
# Begin Source File

SOURCE=.\sharedstate.cpp
# End Source File
# Begin Source File

SOURCE=.\sharedstate.h
# End Source File
# Begin Source File

SOURCE=.\spriteed.cpp
# End Source File
# Begin Source File

SOURCE=.\statealign.cpp
# End Source File
# Begin Source File

SOURCE=.\statehelp.cpp
# End Source File
# Begin Source File

SOURCE=.\statemachine.cpp
# End Source File
# Begin Source File

SOURCE=.\statemachine.h
# End Source File
# Begin Source File

SOURCE=.\statemovie.cpp
# End Source File
# Begin Source File

SOURCE=.\states.h
# End Source File
# Begin Source File

SOURCE=.\stateview.cpp
# End Source File
# End Target
# End Project
