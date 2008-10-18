# Microsoft Developer Studio Project File - Name="engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=engine - Win32 Symbian Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "engine.mak" CFG="engine - Win32 Symbian Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "engine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "engine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "engine - Win32 Symbian Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "engine"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "engine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../output/Windows/Release"
# PROP Intermediate_Dir "../../output/Windows/engine/Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /Ob2 /I "../../SDL/SDL/include" /I "../../SDL/SDL_image" /D "STAND_ALONE_GAME" /D "GAME_EDITOR_PROFESSIONAL" /D "KYRA_SUPPORT_OPENGL" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "engine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../output/Windows/Debug"
# PROP Intermediate_Dir "../../output/Windows/engine/Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /Zi /Od /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /D "GAME_EDITOR_PROFESSIONAL" /D "KYRA_SUPPORT_OPENGL" /D "_DEBUG" /D "_LIB" /D "DEBUG" /D "WIN32" /D "_MBCS" /U "STAND_ALONE_GAME" /U "GL_OES_VERSION_1_1" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "engine - Win32 Symbian Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "engine___Win32_Symbian_Debug"
# PROP BASE Intermediate_Dir "engine___Win32_Symbian_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../output"
# PROP Intermediate_Dir "../../output/engine"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /Gm /ZI /Od /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /D "_DEBUG" /D "_LIB" /D "DEBUG" /D "WIN32" /D "_MBCS" /D "KYRA_SUPPORT_OPENGL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MD /W3 /Zi /Od /X /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_DEBUG" /D "_UNICODE" /FR"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fo"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gameEditorSymbian.PDB" /GF /c
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

# Name "engine - Win32 Release"
# Name "engine - Win32 Debug"
# Name "engine - Win32 Symbian Debug"
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\action.cpp
# End Source File
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\box.cpp
# End Source File
# Begin Source File

SOURCE=.\box.h
# End Source File
# Begin Source File

SOURCE=.\boxresource.cpp
# End Source File
# Begin Source File

SOURCE=.\boxresource.h
# End Source File
# Begin Source File

SOURCE=.\canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\canvas.h
# End Source File
# Begin Source File

SOURCE=.\canvasresource.cpp
# End Source File
# Begin Source File

SOURCE=.\canvasresource.h
# End Source File
# Begin Source File

SOURCE=.\color.cpp
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\dataresource.cpp
# End Source File
# Begin Source File

SOURCE=.\dataresource.h
# End Source File
# Begin Source File

SOURCE=.\dirtyrectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\dirtyrectangle.h
# End Source File
# Begin Source File

SOURCE=.\encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder.h
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\engine.h
# End Source File
# Begin Source File

SOURCE=.\fontresource.cpp
# End Source File
# Begin Source File

SOURCE=.\fontresource.h
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\imagetree.cpp
# End Source File
# Begin Source File

SOURCE=.\imagetree.h
# End Source File
# Begin Source File

SOURCE=.\imnode.cpp
# End Source File
# Begin Source File

SOURCE=.\imnode.h
# End Source File
# Begin Source File

SOURCE=.\krmath.cpp
# End Source File
# Begin Source File

SOURCE=.\krmath.h
# End Source File
# Begin Source File

SOURCE=.\kyra.h
# End Source File
# Begin Source File

SOURCE=.\kyrabuild.h
# End Source File
# Begin Source File

SOURCE=.\kyraresource.cpp
# End Source File
# Begin Source File

SOURCE=.\kyraresource.h
# End Source File
# Begin Source File

SOURCE=.\namefield.cpp
# End Source File
# Begin Source File

SOURCE=.\ogltexture.cpp
# End Source File
# Begin Source File

SOURCE=.\ogltexture.h
# End Source File
# Begin Source File

SOURCE=.\painter.cpp
# End Source File
# Begin Source File

SOURCE=.\painter.h
# End Source File
# Begin Source File

SOURCE=.\parser.cpp

!IF  "$(CFG)" == "engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "engine - Win32 Debug"

!ELSEIF  "$(CFG)" == "engine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\parser.h
# End Source File
# Begin Source File

SOURCE=.\pixelblock.cpp
# End Source File
# Begin Source File

SOURCE=.\pixelblock.h
# End Source File
# Begin Source File

SOURCE=.\rle.cpp
# End Source File
# Begin Source File

SOURCE=.\rle.h
# End Source File
# Begin Source File

SOURCE=.\scrap.c
# End Source File
# Begin Source File

SOURCE=.\scrap.h
# End Source File
# Begin Source File

SOURCE=.\sdlutil.cpp
# End Source File
# Begin Source File

SOURCE=.\sdlutil.h
# End Source File
# Begin Source File

SOURCE=.\splash.cpp
# End Source File
# Begin Source File

SOURCE=.\splash.h
# End Source File
# Begin Source File

SOURCE=.\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\spriteresource.cpp
# End Source File
# Begin Source File

SOURCE=.\spriteresource.h
# End Source File
# Begin Source File

SOURCE=.\textbox.cpp
# End Source File
# Begin Source File

SOURCE=.\textbox.h
# End Source File
# Begin Source File

SOURCE=.\tile.cpp
# End Source File
# Begin Source File

SOURCE=.\tile.h
# End Source File
# Begin Source File

SOURCE=.\tileresource.cpp
# End Source File
# Begin Source File

SOURCE=.\tileresource.h
# End Source File
# Begin Source File

SOURCE=.\vault.cpp
# End Source File
# Begin Source File

SOURCE=.\vault.h
# End Source File
# End Group
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\gui\button.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\button.h
# End Source File
# Begin Source File

SOURCE=..\gui\console.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\console.h
# End Source File
# Begin Source File

SOURCE=..\gui\eventmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\eventmanager.h
# End Source File
# Begin Source File

SOURCE=..\gui\listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\listbox.h
# End Source File
# Begin Source File

SOURCE=..\gui\textwidget.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\textwidget.h
# End Source File
# Begin Source File

SOURCE=..\gui\UndoFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\UndoFrame.h
# End Source File
# Begin Source File

SOURCE=..\gui\widget.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\widget.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\glbitstream.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glbitstream.h
# End Source File
# Begin Source File

SOURCE=..\Util\glcirclelist.h
# End Source File
# Begin Source File

SOURCE=..\util\gldebug.cpp
# End Source File
# Begin Source File

SOURCE=..\Util\gldebug.h
# End Source File
# Begin Source File

SOURCE=..\Util\gldynarray.h
# End Source File
# Begin Source File

SOURCE=..\util\glfixed.h
# End Source File
# Begin Source File

SOURCE=..\util\glgraph.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glgraph.h
# End Source File
# Begin Source File

SOURCE=..\Util\glinsidelist.h
# End Source File
# Begin Source File

SOURCE=..\util\glisomath.cpp
# End Source File
# Begin Source File

SOURCE=..\Util\glisomath.h
# End Source File
# Begin Source File

SOURCE=..\Util\gllist.h
# End Source File
# Begin Source File

SOURCE=..\util\glmap.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glmap.h
# End Source File
# Begin Source File

SOURCE=..\util\glmemorypool.cpp
# End Source File
# Begin Source File

SOURCE=..\Util\glmemorypool.h
# End Source File
# Begin Source File

SOURCE=..\util\glperformance.cpp
# End Source File
# Begin Source File

SOURCE=..\Util\glperformance.h
# End Source File
# Begin Source File

SOURCE=..\util\glprime.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glprime.h
# End Source File
# Begin Source File

SOURCE=..\util\glrandom.cpp
# End Source File
# Begin Source File

SOURCE=..\Util\glrandom.h
# End Source File
# Begin Source File

SOURCE=..\Util\glstack.h
# End Source File
# Begin Source File

SOURCE=..\util\glstring.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glstring.h
# End Source File
# Begin Source File

SOURCE=..\Util\gltypes.h
# End Source File
# Begin Source File

SOURCE=..\Util\glutil.h
# End Source File
# End Group
# Begin Group "TinyXml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\tinyXml\tinyxml.h

!IF  "$(CFG)" == "engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "engine - Win32 Debug"

!ELSEIF  "$(CFG)" == "engine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "GuiExtended"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\guiExtended\KrImageListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\guiExtended\KrImageListBox.h
# End Source File
# Begin Source File

SOURCE=..\guiExtended\progress.cpp
# End Source File
# Begin Source File

SOURCE=..\guiExtended\progress.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FastSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\FastSprite.h

!IF  "$(CFG)" == "engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "engine - Win32 Symbian Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\kyra.h
# End Source File
# End Target
# End Project
