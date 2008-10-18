# Microsoft Developer Studio Project File - Name="wxGameEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxGameEditor - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxGameEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxGameEditor.mak" CFG="wxGameEditor - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxGameEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "wxGameEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/wxGameEditor", BQPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxGameEditor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMSW"
# PROP BASE Intermediate_Dir "ReleaseMSW"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../output/wxGameEditor/ReleaseMSW"
# PROP Intermediate_Dir "../output/wxGameEditor/ReleaseMSW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "E:\wxWidgets/include" /I "E:\wxWidgets/contrib/include" /I "E:\wxWidgets/lib/vc_lib/msw" /I "E:\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /Fd"ReleaseMSW\wxGameEditor.pdb" /FD /EHsc /c
# ADD CPP /nologo /MT /W3 /GR /Zi /O2 /I "../Graphviz/lib/gd" /I "../Graphviz/lib/fdpgen" /I "../Graphviz/lib/cdt" /I "../Graphviz/lib/graph" /I "../Graphviz/lib/pathplan" /I "../Graphviz/lib/common" /I "../Graphviz/lib/gvc" /I "../Graphviz" /I "../SDL/SDL_ttf/VisualC/FreeType/include" /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "NOPCH" /D "__WXMSW__" /D "wxUSE_GUI" /D "HAVE_CONFIG_H" /D "MSWIN32" /U "STAND_ALONE_GAME" /FR /Fd"ReleaseMSW\wxGameEditor.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "E:\wxWidgets/include" /i "E:\wxWidgets/contrib/include" /i "E:\wxWidgets/lib/vc_lib/msw" /i "E:\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x409 /i "E:\wxWidgets/include" /i "E:\wxWidgets/contrib/include" /i "E:\wxWidgets/lib/vc_lib/msw" /i "E:\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "NDEBUG" /d "USE_ACTIVATION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_ifm.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw26_adv.lib wxmsw26_html.lib wxmsw26_xrc.lib wxbase26_net.lib wxbase26_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /libpath:"E:\wxWidgets/lib/vc_lib" /libpath:"E:\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw26_ifm.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib dxguid.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../output/wxGameEditor.exe" /libpath:"E:\wxWidgets/lib/vc_lib" /libpath:"E:\Microsoft Visual Studio\vc98\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy resources.xrc "../output/"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "wxGameEditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugMSW"
# PROP BASE Intermediate_Dir "DebugMSW"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/wxGameEditor/DebugMSW"
# PROP Intermediate_Dir "../output/wxGameEditor/DebugMSW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GR /Zi /Od /I "E:\wxWidgets/include" /I "E:\wxWidgets/contrib/include" /I "E:\wxWidgets/lib/vc_lib/mswd" /I "E:\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /Fd"DebugMSW\wxGameEditor.pdb" /FD /EHsc /c
# ADD CPP /nologo /MTd /W3 /GR /Zi /Od /I "../Graphviz/lib/gd" /I "../Graphviz/lib/fdpgen" /I "../Graphviz/lib/cdt" /I "../Graphviz/lib/graph" /I "../Graphviz/lib/pathplan" /I "../Graphviz/lib/common" /I "../Graphviz/lib/gvc" /I "../Graphviz" /I "../SDL/SDL_ttf/VisualC/FreeType/include" /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "GAME_EDITOR_PROFESSIONAL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "NOPCH" /D "__WXMSW__" /D "__WXDEBUG__" /D "wxUSE_GUI" /D "HAVE_CONFIG_H" /D "MSWIN32" /U "STAND_ALONE_GAME" /FR /Fd"DebugMSW\wxGameEditor.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "E:\wxWidgets/include" /i "E:\wxWidgets/contrib/include" /i "E:\wxWidgets/lib/vc_lib/mswd" /i "E:\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
# ADD RSC /l 0x409 /i "E:\wxWidgets/include" /i "E:\wxWidgets/contrib/include" /i "E:\wxWidgets/lib/vc_lib/mswd" /i "E:\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_ifm.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw26d_adv.lib wxmsw26d_html.lib wxmsw26d_xrc.lib wxbase26d_net.lib wxbase26d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"E:\wxWidgets/lib/vc_lib" /libpath:"E:\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw26d_ifm.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib dxguid.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../output/wxGameEditor.exe" /libpath:"E:\wxWidgets/lib/vc_lib" /libpath:"E:\Microsoft Visual Studio\vc98\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy resources.xrc "../output/"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "wxGameEditor - Win32 Release"
# Name "wxGameEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgSaveLayout.cpp
# End Source File
# Begin Source File

SOURCE=MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelActors.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelGenericProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\wxGed\spinrealm.cpp
# End Source File
# Begin Source File

SOURCE=wxgameeditor.cpp
# End Source File
# Begin Source File

SOURCE=wxGameEditor.rc
# End Source File
# Begin Source File

SOURCE=.\wxGed\wxGedControls.cpp
# End Source File
# Begin Source File

SOURCE=.\wxGed\wxGedStatusBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgSaveLayout.h
# End Source File
# Begin Source File

SOURCE=MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\PanelActors.h
# End Source File
# Begin Source File

SOURCE=.\PanelEvents.h
# End Source File
# Begin Source File

SOURCE=.\PanelGenericProperty.h
# End Source File
# Begin Source File

SOURCE=.\PanelProperty.h
# End Source File
# Begin Source File

SOURCE=.\wxGed\spinreal.h
# End Source File
# Begin Source File

SOURCE=.\wxGed\spinrealm.h
# End Source File
# Begin Source File

SOURCE=wxgameeditor.h
# End Source File
# Begin Source File

SOURCE=.\wxGed\wxGedControls.h
# End Source File
# Begin Source File

SOURCE=.\wxGed\wxGedStatusBar.h
# End Source File
# End Group
# Begin Group "GameEditor"

# PROP Default_Filter ""
# Begin Group "ResourceEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\gameEditor\ResourceEditor\ResourceEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ResourceEditor\ResourceEditor.h
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ResourceEditor\ResourceVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ResourceEditor\ResourceVersionInfo.h
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ResourceEditor\util.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\gameEditor\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ActorProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ActorText.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AddActionDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AddActor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AddPathDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AddSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AdsControl.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\AdsShow.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\BaseAction.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\CDataFile.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ChangeAnimation.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ChangeAnimationDirection.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ChangeCursor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ChangeParent.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ChangePath.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ConditionalAction.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\Config.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\CreateActor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\CreateTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\CreateVar.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DestroyActor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DestroyTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgActivationEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgAnimationFinish.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgGeneralEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgMouseButton.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgPocketPCKey.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\DlgTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\EditSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\EnableDisable.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\EventControl.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\EventKeyDown.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ExportGame.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ExpressionEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\FunctionPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\GameSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\GridSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\jdiff.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\LoadSave.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\LoadSaveGame.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\LoadSaveScript.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\main.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\MainPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\MoveTo.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\NewTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\PathDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\PhysicalBounce.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\PlaySong.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ScriptGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\SDL\SDL_ttf\SDL_ttf.c
# End Source File
# Begin Source File

SOURCE=..\gameEditor\SetTile.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\SetTileSingle.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\SetZDepth.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ToAnteriorPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\ToMousePosition.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\Transparency.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\Tutorial.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\TutorialControlPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\TutorialPaint.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\TutorialTextEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\TutorialTextPanel.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\UndoControl.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\VariableEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\gameEditor\WaitForFrame.cpp
# End Source File
# End Group
# Begin Group "wxPropertyGrid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\propgrid\src\propgrid\advprops.cpp
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\advprops.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\propgrid\src\propgrid\manager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\manager.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\propgrid\src\propgrid\odcombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\odcombo.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\pg_dox_mainpage.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\propdev.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\propgrid\src\propgrid\propgrid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\include\wx\propgrid\propgrid.h
# End Source File
# Begin Source File

SOURCE=..\..\wxWidgets\contrib\propgrid\src\propgrid\props.cpp
# End Source File
# End Group
# Begin Group "Graphviz"

# PROP Default_Filter ""
# Begin Group "gvc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\dot_builtins.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvc.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvc.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvcext.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvcint.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvcjob.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvconfig.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvcontext.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvcproc.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvdevice.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvevent.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvjobs.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvlayout.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin_device.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin_layout.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin_render.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin_textlayout.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvplugin_usershape.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvrender.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\gvc\gvtextlayout.c
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\common\args.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\arith.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\arrows.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\color.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\colorprocs.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\colortbl.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\colxlate.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\const.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\emit.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\fontmetrics.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\gdtextsize.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\geom.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\geom.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\geomprocs.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\globals.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\globals.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\input.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\labels.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\logic.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\macros.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\memory.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\memory.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\ns.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\output.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\pointset.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\pointset.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\postproc.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\psusershape.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\render.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\routespl.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\shapes.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\splines.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\strcasecmp.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\strncasecmp.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\timing.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\types.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\utils.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\utils.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\common\xdgen.c
# End Source File
# End Group
# Begin Group "graph"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\graph\agxbuf.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\agxbuf.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\attribs.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\edge.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\graph.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\graph.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\graphio.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\lexer.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\libgraph.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\node.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\parser.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\parser.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\refstr.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\trie.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\graph\triefa.h
# End Source File
# End Group
# Begin Group "cdt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\ast_common.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\cdt.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtclose.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtdisc.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtextract.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtflatten.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dthash.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dthdr.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtlist.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtmethod.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtopen.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtrenew.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtrestore.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtsize.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtstat.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtstrhash.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dttree.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtview.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\cdt\dtwalk.c
# End Source File
# End Group
# Begin Group "pathplan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\cvt.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\inpoly.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\pathgeom.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\pathplan.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\pathutil.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\route.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\shortest.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\shortestpth.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\solvers.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\solvers.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\tri.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\triang.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\util1.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\vis.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\visibility.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\pathplan\vispath.h
# End Source File
# End Group
# Begin Group "plugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\plugin\gvlayout_dot_layout.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\plugin\gvplugin_dot_layout.c
# End Source File
# End Group
# Begin Group "dotgen"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\acyclic.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\class1.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\class2.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\cluster.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\compound.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\conc.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\decomp.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\dot.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\dotinit.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\dotprocs.h
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\dotsplines.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\fastgr.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\flat.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\mincross.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\position.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\rank.c
# End Source File
# Begin Source File

SOURCE=..\Graphviz\lib\dotgen\sameport.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\Graphviz\config.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cross.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\GameGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\GameGraph.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\resources.xrc
# End Source File
# Begin Source File

SOURCE=.\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\wxGameEditor.pjd
# End Source File
# End Target
# End Project
