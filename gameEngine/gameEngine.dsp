# Microsoft Developer Studio Project File - Name="gameEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gameEngine - Win32 Symbian Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gameEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gameEngine.mak" CFG="gameEngine - Win32 Symbian Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gameEngine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gameEngine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gameEngine - Win32 Symbian Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/gameEngine", MQDAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gameEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../output/Windows/Release"
# PROP Intermediate_Dir "../output/Windows/gameEngine/Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /Ob2 /I "../RakNet/Include" /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "Eic/src" /I "../SDL/SDL_image/png/libpng" /I "../SDL/SDL_image/png/zlib" /D "STAND_ALONE_GAME" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /U "GAME_EDITOR_HOME_EDITION" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/Windows/Debug"
# PROP Intermediate_Dir "../output/Windows/gameEngine/Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../RakNet/Include" /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "EiC/src" /I "../SDL/SDL_image/png/libpng" /I "../SDL/SDL_image/png/zlib" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /U "STAND_ALONE_GAME" /U "GL_OES_VERSION_1_1" /U "GAME_EDITOR_HOME_EDITION" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Symbian Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gameEngine___Win32_Symbian_Debug"
# PROP BASE Intermediate_Dir "gameEngine___Win32_Symbian_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/"
# PROP Intermediate_Dir "../output/gameEngine"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "EiC/src" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MD /W3 /Zi /Od /X /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "EiC/src" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fo"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gameEditorSymbian.PDB" /GF /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "gameEngine - Win32 Release"
# Name "gameEngine - Win32 Debug"
# Name "gameEngine - Win32 Symbian Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\ActivationEventsCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\Actor.cpp
# End Source File
# Begin Source File

SOURCE=.\ActorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ActorTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Axis.cpp
# End Source File
# Begin Source File

SOURCE=.\bit_byte.cpp
# End Source File
# Begin Source File

SOURCE=.\bit_bytx.cpp
# End Source File
# Begin Source File

SOURCE=.\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\compression.cpp
# End Source File
# Begin Source File

SOURCE=.\dlmalloc.c
# End Source File
# Begin Source File

SOURCE=.\EditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorDirectory.cpp
# End Source File
# Begin Source File

SOURCE=.\Envelope.cpp
# End Source File
# Begin Source File

SOURCE=.\GameControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMain.cpp
# End Source File
# Begin Source File

SOURCE=.\gedString.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericScript.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ListDirectory.cpp
# End Source File
# Begin Source File

SOURCE=.\ListPop.cpp
# End Source File
# Begin Source File

SOURCE=.\ListPopSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\loadGifMod.cpp
# End Source File
# Begin Source File

SOURCE=.\maksKyra.cpp
# End Source File
# Begin Source File

SOURCE=.\minilzo.c
# End Source File
# Begin Source File

SOURCE=.\MuteEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelQuestion.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\PathPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\RegionLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\Slide.cpp
# End Source File
# Begin Source File

SOURCE=.\SlowCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\system.c
# End Source File
# Begin Source File

SOURCE=.\Text.cpp
# End Source File
# Begin Source File

SOURCE=.\Tile.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitCursor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Action.h
# End Source File
# Begin Source File

SOURCE=.\ActivationEventsCanvas.h
# End Source File
# Begin Source File

SOURCE=.\Actor.h
# End Source File
# Begin Source File

SOURCE=.\ActorEdit.h
# End Source File
# Begin Source File

SOURCE=.\ActorTip.h
# End Source File
# Begin Source File

SOURCE=.\Axis.h
# End Source File
# Begin Source File

SOURCE=.\bit_byte.h
# End Source File
# Begin Source File

SOURCE=.\bit_bytx.h
# End Source File
# Begin Source File

SOURCE=.\Button.h
# End Source File
# Begin Source File

SOURCE=.\compression.h
# End Source File
# Begin Source File

SOURCE=.\dlmalloc.h
# End Source File
# Begin Source File

SOURCE=.\EditBox.h
# End Source File
# Begin Source File

SOURCE=.\EditorDirectory.h
# End Source File
# Begin Source File

SOURCE=.\Envelope.h
# End Source File
# Begin Source File

SOURCE=.\GameControl.h
# End Source File
# Begin Source File

SOURCE=.\gedString.h
# End Source File
# Begin Source File

SOURCE=.\GenericScript.h
# End Source File
# Begin Source File

SOURCE=.\gui.h
# End Source File
# Begin Source File

SOURCE=.\ListBox.h
# End Source File
# Begin Source File

SOURCE=.\ListDirectory.h
# End Source File
# Begin Source File

SOURCE=.\ListPop.h
# End Source File
# Begin Source File

SOURCE=.\ListPopSelect.h
# End Source File
# Begin Source File

SOURCE=.\lzoconf.h
# End Source File
# Begin Source File

SOURCE=.\maksKyra.h
# End Source File
# Begin Source File

SOURCE=.\minilzo.h
# End Source File
# Begin Source File

SOURCE=.\MuteEx.h
# End Source File
# Begin Source File

SOURCE=.\Panel.h
# End Source File
# Begin Source File

SOURCE=.\PanelInfo.h
# End Source File
# Begin Source File

SOURCE=.\PanelQuestion.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\PathPoint.h
# End Source File
# Begin Source File

SOURCE=.\RegionLoad.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\Sequence.h
# End Source File
# Begin Source File

SOURCE=.\Slide.h
# End Source File
# Begin Source File

SOURCE=.\SlowCanvas.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# Begin Source File

SOURCE=.\Text.h
# End Source File
# Begin Source File

SOURCE=.\Tile.h
# End Source File
# Begin Source File

SOURCE=.\WaitCursor.h
# End Source File
# End Group
# Begin Group "Ucl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Compact\alloc.c
# End Source File
# Begin Source File

SOURCE=.\Compact\getbit.h
# End Source File
# Begin Source File

SOURCE=.\Compact\n2_99.ch

!IF  "$(CFG)" == "gameEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compact\n2b_99.c
# End Source File
# Begin Source File

SOURCE=.\Compact\n2b_d.c
# End Source File
# Begin Source File

SOURCE=.\Compact\n2b_ds.c
# End Source File
# Begin Source File

SOURCE=.\Compact\n2d_99.c
# End Source File
# Begin Source File

SOURCE=.\Compact\n2d_d.c
# End Source File
# Begin Source File

SOURCE=.\Compact\n2d_ds.c
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl.h
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_conf.h
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_init.c
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_mchw.ch

!IF  "$(CFG)" == "gameEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_ptr.c
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_ptr.h
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_str.c
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_swd.ch

!IF  "$(CFG)" == "gameEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEngine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_util.c
# End Source File
# Begin Source File

SOURCE=.\Compact\ucl_util.h
# End Source File
# Begin Source File

SOURCE=.\Compact\uclconf.h
# End Source File
# End Group
# Begin Group "PathFinder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PathFinder\GeoPathfinder.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinder\GeoPathfinder.h
# End Source File
# End Group
# End Target
# End Project
