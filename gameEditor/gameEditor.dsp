# Microsoft Developer Studio Project File - Name="gameEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=gameEditor - Win32 Symbian Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gameEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gameEditor.mak" CFG="gameEditor - Win32 Symbian Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gameEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "gameEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "gameEditor - Win32 Symbian Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/gameEditor", TGBAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gameEditor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../output/Windows/Release"
# PROP Intermediate_Dir "../output/Windows/editor/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /Ob2 /I "../SDL/SDL_ttf/VisualC/FreeType/include" /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "STAND_ALONE_GAME" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /U "GAME_EDITOR_HOME_EDITION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG" /d "USE_ACTIVATION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib dxguid.lib shell32.lib comdlg32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"../output/gameEditor.exe" /fixed:no
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy res ..\output /Y /E	copy release\editor.dat ..\output /Y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/Windows/Debug"
# PROP Intermediate_Dir "../output/Windows/editor/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../SDL/SDL_ttf/VisualC/FreeType/include" /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /U "STAND_ALONE_GAME" /U "GAME_EDITOR_HOME_EDITION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib dxguid.lib shell32.lib comdlg32.lib /nologo /stack:0x10000,0x1000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"../output/gameEditor.exe" /fixed:no
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copy
PostBuild_Cmds=xcopy res ..\output /Y /E	copy release\editor.dat ..\output /Y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Symbian Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gameEditor___Win32_Symbian_Debug"
# PROP BASE Intermediate_Dir "gameEditor___Win32_Symbian_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/"
# PROP Intermediate_Dir "../output/editor"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MD /W3 /Zi /Od /X /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fo"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gameEditorSymbian.PDB" /GF /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../output/eiclib.lib ../output/SDL_mixer.lib ../output/gameEngine.lib ../output/SDL_image.lib ../output/engine.lib ../output/sdlmain.lib ../output/sdl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../output/engine.lib ../output/eiclib.lib ../output/gameEngine.lib "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\EEXE.LIB" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\sdl.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\sdlmain.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ecrt0.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\euser.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\fbscli.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estlib.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ws32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\hal.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gdi.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\apgrfx.lib" /nologo /entry:"_E32Startup" /subsystem:windows /pdb:"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB/HELLOMAKS.pdb" /debug /machine:IX86 /nodefaultlib /include:"?_E32Startup@@YGXXZ" /out:"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB/gameEditorSymbian.exe" /WARN:3
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copy
PostBuild_Cmds=xcopy res ..\output /Y /E	copy release\jpeg.dll ..\output /Y	copy release\libpng1.dll ..\output /Y	copy release\zlib.dll ..\output /Y	copy release\editor.dat ..\output /Y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "gameEditor - Win32 Release"
# Name "gameEditor - Win32 Debug"
# Name "gameEditor - Win32 Symbian Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActorProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\ActorText.cpp
# End Source File
# Begin Source File

SOURCE=.\AddActionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddActor.cpp
# End Source File
# Begin Source File

SOURCE=.\AddPathDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AdsControl.cpp
# End Source File
# Begin Source File

SOURCE=.\AdsShow.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseAction.cpp
# End Source File
# Begin Source File

SOURCE=.\CDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeAnimationDirection.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeParent.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePath.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ConditionalAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateActor.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DestroyActor.cpp
# End Source File
# Begin Source File

SOURCE=.\DestroyTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgActivationEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnimationFinish.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeneralEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMouseButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPocketPCKey.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EnableDisable.cpp
# End Source File
# Begin Source File

SOURCE=.\EventControl.cpp
# End Source File
# Begin Source File

SOURCE=.\EventKeyDown.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpressionEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\GridSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\jdiff.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadSave.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadSaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadSaveScript.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MainPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveTo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\PathDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysicalBounce.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaySong.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\SDL\SDL_ttf\SDL_ttf.c
# End Source File
# Begin Source File

SOURCE=.\SetTile.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTileSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\SetZDepth.cpp
# End Source File
# Begin Source File

SOURCE=.\ToAnteriorPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\ToMousePosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Transparency.cpp
# End Source File
# Begin Source File

SOURCE=.\Tutorial.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorialControlPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorialPaint.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorialTextEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorialTextPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoControl.cpp
# End Source File
# Begin Source File

SOURCE=.\VariableEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitForFrame.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActorProperty.h
# End Source File
# Begin Source File

SOURCE=.\ActorText.h
# End Source File
# Begin Source File

SOURCE=.\AddActionDialog.h
# End Source File
# Begin Source File

SOURCE=.\AddActor.h
# End Source File
# Begin Source File

SOURCE=.\AddPathDialog.h
# End Source File
# Begin Source File

SOURCE=.\AddSequenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\AdsControl.h
# End Source File
# Begin Source File

SOURCE=.\AdsShow.h
# End Source File
# Begin Source File

SOURCE=.\BaseAction.h
# End Source File
# Begin Source File

SOURCE=.\CDataFile.h
# End Source File
# Begin Source File

SOURCE=.\ChangeAnimation.h
# End Source File
# Begin Source File

SOURCE=.\ChangeAnimationDirection.h
# End Source File
# Begin Source File

SOURCE=.\ChangeCursor.h
# End Source File
# Begin Source File

SOURCE=.\ChangeParent.h
# End Source File
# Begin Source File

SOURCE=.\ChangePath.h
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\ConditionalAction.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\CreateActor.h
# End Source File
# Begin Source File

SOURCE=.\CreateTimer.h
# End Source File
# Begin Source File

SOURCE=.\CreateVar.h
# End Source File
# Begin Source File

SOURCE=.\DestroyActor.h
# End Source File
# Begin Source File

SOURCE=.\DestroyTimer.h
# End Source File
# Begin Source File

SOURCE=.\DlgActivationEvent.h
# End Source File
# Begin Source File

SOURCE=.\DlgAnimationFinish.h
# End Source File
# Begin Source File

SOURCE=.\DlgCollision.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeneralEvent.h
# End Source File
# Begin Source File

SOURCE=.\DlgMouseButton.h
# End Source File
# Begin Source File

SOURCE=.\DlgPocketPCKey.h
# End Source File
# Begin Source File

SOURCE=.\DlgTimer.h
# End Source File
# Begin Source File

SOURCE=.\EditSequenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\EnableDisable.h
# End Source File
# Begin Source File

SOURCE=.\EventControl.h
# End Source File
# Begin Source File

SOURCE=.\EventKeyDown.h
# End Source File
# Begin Source File

SOURCE=.\ExportGame.h
# End Source File
# Begin Source File

SOURCE=.\ExpressionEditor.h
# End Source File
# Begin Source File

SOURCE=.\FunctionPanel.h
# End Source File
# Begin Source File

SOURCE=.\GameSettings.h
# End Source File
# Begin Source File

SOURCE=.\GridSettings.h
# End Source File
# Begin Source File

SOURCE=.\jdiff.h
# End Source File
# Begin Source File

SOURCE=.\LoadSave.h
# End Source File
# Begin Source File

SOURCE=.\LoadSaveGame.h
# End Source File
# Begin Source File

SOURCE=.\LoadSaveScript.h
# End Source File
# Begin Source File

SOURCE=.\MainPanel.h
# End Source File
# Begin Source File

SOURCE=.\MoveTo.h
# End Source File
# Begin Source File

SOURCE=.\NewTimer.h
# End Source File
# Begin Source File

SOURCE=.\PathDialog.h
# End Source File
# Begin Source File

SOURCE=.\PhysicalBounce.h
# End Source File
# Begin Source File

SOURCE=.\PlaySong.h
# End Source File
# Begin Source File

SOURCE=.\ScriptGlobals.h
# End Source File
# Begin Source File

SOURCE=..\SDL\SDL_ttf\SDL_ttf.h
# End Source File
# Begin Source File

SOURCE=.\SetTile.h
# End Source File
# Begin Source File

SOURCE=.\SetTileSingle.h
# End Source File
# Begin Source File

SOURCE=.\SetZDepth.h
# End Source File
# Begin Source File

SOURCE=.\ToAnteriorPosition.h
# End Source File
# Begin Source File

SOURCE=.\ToMousePosition.h
# End Source File
# Begin Source File

SOURCE=.\Transparency.h
# End Source File
# Begin Source File

SOURCE=.\Tutorial.h
# End Source File
# Begin Source File

SOURCE=.\TutorialControlPanel.h
# End Source File
# Begin Source File

SOURCE=.\TutorialPaint.h
# End Source File
# Begin Source File

SOURCE=.\TutorialTextEditor.h
# End Source File
# Begin Source File

SOURCE=.\TutorialTextPanel.h
# End Source File
# Begin Source File

SOURCE=.\UndoControl.h
# End Source File
# Begin Source File

SOURCE=.\VariableEditor.h
# End Source File
# Begin Source File

SOURCE=.\WaitForFrame.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\pocketpc.rc
# End Source File
# Begin Source File

SOURCE=.\pocketpc_resource.h
# End Source File
# End Group
# Begin Group "ResourceEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ResourceEditor\ResourceEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceEditor\ResourceEditor.h
# End Source File
# Begin Source File

SOURCE=.\ResourceEditor\ResourceVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceEditor\ResourceVersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\ResourceEditor\util.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\release\changelog.txt

!IF  "$(CFG)" == "gameEditor - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Symbian Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\compilação.txt"
# End Source File
# Begin Source File

SOURCE="..\distribuição.txt"
# End Source File
# Begin Source File

SOURCE=.\res\functions.xml
# End Source File
# Begin Source File

SOURCE=".\others\plano de negócios.txt"

!IF  "$(CFG)" == "gameEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\Teoria sobre Jogos.txt"
# End Source File
# Begin Source File

SOURCE=.\todo.txt

!IF  "$(CFG)" == "gameEditor - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "gameEditor - Win32 Symbian Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
