# Microsoft Developer Studio Project File - Name="engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=engine - Win32 Uni Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "engineSymbian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "engineSymbian.mak" CFG="engine - Win32 Uni Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "engine - Win32 Uni Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "engine - Win32 Uni Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/kyra/engine", YAGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "engine - Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Win32_U0"
# PROP BASE Intermediate_Dir ".\Win32_U0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../output/"
# PROP Intermediate_Dir "../../output/engine"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MDd /W3 /Zi /Od /X /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "NO_SIGNAL_H" /D "ENABLE_EPOC" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /D "DISABLE_AUDIO" /D "_DEBUG" /D "_UNICODE" /FR /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\engine.PDB" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 /nologo /out:"../../output\engine.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ELSEIF  "$(CFG)" == "engine - Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Win32_Un"
# PROP BASE Intermediate_Dir ".\Win32_Un"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../output/"
# PROP Intermediate_Dir "../../output/engine"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MD /W3 /O1 /Op /X /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "NO_SIGNAL_H" /D "ENABLE_EPOC" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /D "DISABLE_AUDIO" /D "NDEBUG" /D "_UNICODE" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\euser.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\fbscli.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estlib.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ws32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\hal.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gdi.lib" /nologo /out:"../../output\engine.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ENDIF 

# Begin Target

# Name "engine - Win32 Uni Debug"
# Name "engine - Win32 Uni Release"
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\action.cpp
DEP_CPP_ACTIO=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\gllist.h"\
	"..\util\glmemorypool.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\encoder.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\box.cpp
DEP_CPP_BOX_C=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\box.h
# End Source File
# Begin Source File

SOURCE=.\boxresource.cpp
DEP_CPP_BOXRE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	
# End Source File
# Begin Source File

SOURCE=.\boxresource.h
# End Source File
# Begin Source File

SOURCE=.\canvas.cpp
DEP_CPP_CANVA=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvas.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\canvas.h
# End Source File
# Begin Source File

SOURCE=.\canvasresource.cpp
DEP_CPP_CANVAS=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvas.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\canvasresource.h
# End Source File
# Begin Source File

SOURCE=.\color.cpp
DEP_CPP_COLOR=\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\color.h"\
	".\kyrabuild.h"\
	
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\dataresource.cpp
DEP_CPP_DATAR=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\gllist.h"\
	"..\util\glmemorypool.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\color.h"\
	".\dataresource.h"\
	".\encoder.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\painter.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\dataresource.h
# End Source File
# Begin Source File

SOURCE=.\dirtyrectangle.cpp
DEP_CPP_DIRTY=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\gllist.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\painter.h"\
	
# End Source File
# Begin Source File

SOURCE=.\dirtyrectangle.h
# End Source File
# Begin Source File

SOURCE=.\encoder.cpp
DEP_CPP_ENCOD=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\..\SDL\SDL_image\SDL_image.h"\
	"..\gui\console.h"\
	"..\gui\widget.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dataresource.h"\
	".\dirtyrectangle.h"\
	".\encoder.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\parser.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\tileresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\encoder.h
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
DEP_CPP_ENGIN=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvas.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\splash.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\engine.h
# End Source File
# Begin Source File

SOURCE=.\fontresource.cpp
DEP_CPP_FONTR=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\encoder.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\painter.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\fontresource.h
# End Source File
# Begin Source File

SOURCE=.\image.cpp
DEP_CPP_IMAGE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\imagecontainer.cpp
# End Source File
# Begin Source File

SOURCE=.\imagecontainer.h
# End Source File
# Begin Source File

SOURCE=.\imagenode.h
# End Source File
# Begin Source File

SOURCE=.\imagetree.cpp
DEP_CPP_IMAGET=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\imagetree.h
# End Source File
# Begin Source File

SOURCE=.\imnode.cpp
DEP_CPP_IMNOD=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\imnode.h
# End Source File
# Begin Source File

SOURCE=.\krmath.cpp
DEP_CPP_KRMAT=\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\glfixed.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\krmath.h"\
	
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
DEP_CPP_KYRAR=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dataresource.h"\
	".\fontresource.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\spriteresource.h"\
	".\tileresource.h"\
	
# End Source File
# Begin Source File

SOURCE=.\kyraresource.h
# End Source File
# Begin Source File

SOURCE=.\namefield.cpp
DEP_CPP_NAMEF=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\gllist.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\namefield.h"\
	
# End Source File
# Begin Source File

SOURCE=.\namefield.h
# End Source File
# Begin Source File

SOURCE=.\ogltexture.cpp
DEP_CPP_OGLTE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ogltexture.h
# End Source File
# Begin Source File

SOURCE=.\painter.cpp
DEP_CPP_PAINT=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glmap.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\color.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\ogltexture.h"\
	".\painter.h"\
	
# End Source File
# Begin Source File

SOURCE=.\painter.h
# End Source File
# Begin Source File

SOURCE=.\parser.cpp
DEP_CPP_PARSE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\gllist.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\parser.h"\
	
# End Source File
# Begin Source File

SOURCE=.\parser.h
# End Source File
# Begin Source File

SOURCE=.\pixelblock.cpp
DEP_CPP_PIXEL=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pixelblock.h
# End Source File
# Begin Source File

SOURCE=.\rle.cpp
DEP_CPP_RLE_C=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\rle.h
# End Source File
# Begin Source File

SOURCE=.\sdlutil.cpp
DEP_CPP_SDLUT=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\gllist.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\sdlutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\sdlutil.h
# End Source File
# Begin Source File

SOURCE=.\splash.cpp
DEP_CPP_SPLAS=\
	".\splash.h"\
	
# End Source File
# Begin Source File

SOURCE=.\splash.h
# End Source File
# Begin Source File

SOURCE=.\sprite.cpp
DEP_CPP_SPRIT=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\spriteresource.cpp
DEP_CPP_SPRITE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\encoder.h"\
	".\image.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\spriteresource.h
# End Source File
# Begin Source File

SOURCE=.\tags.h
# End Source File
# Begin Source File

SOURCE=.\textbox.cpp
DEP_CPP_TEXTB=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\textbox.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\textbox.h
# End Source File
# Begin Source File

SOURCE=.\tile.cpp
DEP_CPP_TILE_=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\tile.h"\
	".\tileresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tile.h
# End Source File
# Begin Source File

SOURCE=.\tileresource.cpp
DEP_CPP_TILER=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\encoder.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\tileresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tileresource.h
# End Source File
# Begin Source File

SOURCE=.\vault.cpp
DEP_CPP_VAULT=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_endian.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\tinyxml\tinyxml.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\encoder.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\namefield.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sdlutil.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=.\vault.h
# End Source File
# End Group
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\gui\button.cpp
DEP_CPP_BUTTO=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\button.h"\
	"..\gui\eventmanager.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\textbox.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\button.h
# End Source File
# Begin Source File

SOURCE=..\gui\console.cpp
DEP_CPP_CONSO=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_opengl.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\console.h"\
	"..\gui\textwidget.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\canvas.h"\
	".\canvasresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\ogltexture.h"\
	".\painter.h"\
	".\pixelblock.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\textbox.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\console.h
# End Source File
# Begin Source File

SOURCE=..\gui\eventmanager.cpp
DEP_CPP_EVENT=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\eventmanager.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\eventmanager.h
# End Source File
# Begin Source File

SOURCE=..\gui\listbox.cpp
DEP_CPP_LISTB=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\listbox.h"\
	"..\gui\textwidget.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\listbox.h
# End Source File
# Begin Source File

SOURCE=..\gui\textwidget.cpp
DEP_CPP_TEXTW=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\eventmanager.h"\
	"..\gui\textwidget.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\textbox.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\textwidget.h
# End Source File
# Begin Source File

SOURCE=..\gui\widget.cpp
DEP_CPP_WIDGE=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\eventmanager.h"\
	"..\gui\widget.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\gui\widget.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\glbitstream.cpp
DEP_CPP_GLBIT=\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\glbitstream.h"\
	"..\util\gldebug.h"\
	"..\util\gltypes.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glbitstream.h
# End Source File
# Begin Source File

SOURCE=..\util\glcirclelist.h
# End Source File
# Begin Source File

SOURCE=..\util\gldebug.cpp
DEP_CPP_GLDEB=\
	"..\util\gldebug.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\gldebug.h
# End Source File
# Begin Source File

SOURCE=..\util\gldynarray.h
# End Source File
# Begin Source File

SOURCE=..\util\glfixed.h
# End Source File
# Begin Source File

SOURCE=..\util\glgraph.cpp
DEP_CPP_GLGRA=\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glgraph.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glgraph.h
# End Source File
# Begin Source File

SOURCE=..\util\glinsidelist.h
# End Source File
# Begin Source File

SOURCE=..\util\glintarrayset.cpp
DEP_CPP_GLINT=\
	"..\util\gldebug.h"\
	"..\util\glintarrayset.h"\
	"..\util\gllist.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glintarrayset.h
# End Source File
# Begin Source File

SOURCE=..\util\glisomath.cpp
DEP_CPP_GLISO=\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\gldebug.h"\
	"..\util\glfixed.h"\
	"..\util\glisomath.h"\
	"..\util\gltypes.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glisomath.h
# End Source File
# Begin Source File

SOURCE=..\util\gllist.h
# End Source File
# Begin Source File

SOURCE=..\util\glmap.cpp
# End Source File
# Begin Source File

SOURCE=..\util\glmap.h
# End Source File
# Begin Source File

SOURCE=..\util\glmemorypool.cpp
DEP_CPP_GLMEM=\
	"..\util\gldebug.h"\
	"..\util\glmemorypool.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glmemorypool.h
# End Source File
# Begin Source File

SOURCE=..\util\glperformance.cpp
DEP_CPP_GLPER=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\util\gldebug.h"\
	"..\util\glmap.h"\
	"..\util\glperformance.h"\
	"..\util\glprime.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glperformance.h
# End Source File
# Begin Source File

SOURCE=..\util\glprime.cpp
DEP_CPP_GLPRI=\
	"..\util\gldebug.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glprime.h
# End Source File
# Begin Source File

SOURCE=..\util\glrandom.cpp
DEP_CPP_GLRAN=\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\gldebug.h"\
	"..\util\glrandom.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glrandom.h
# End Source File
# Begin Source File

SOURCE=..\util\glstack.h
# End Source File
# Begin Source File

SOURCE=..\util\glstring.cpp
DEP_CPP_GLSTR=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glstring.h
# End Source File
# Begin Source File

SOURCE=..\util\gltypes.h
# End Source File
# Begin Source File

SOURCE=..\util\glutil.cpp
DEP_CPP_GLUTI=\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	
# End Source File
# Begin Source File

SOURCE=..\util\glutil.h
# End Source File
# End Group
# Begin Group "TinyXml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tinyxml\tinyxml.cpp
DEP_CPP_TINYX=\
	"..\..\gameEngine\gedString.h"\
	"..\tinyxml\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=..\tinyxml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\tinyxml\tinyxmlerror.cpp
DEP_CPP_TINYXM=\
	"..\..\gameEngine\gedString.h"\
	"..\tinyxml\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=..\tinyxml\tinyxmlparser.cpp
DEP_CPP_TINYXML=\
	"..\..\gameEngine\gedString.h"\
	"..\tinyxml\tinyxml.h"\
	
# End Source File
# End Group
# Begin Group "GuiExtended"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\guiExtended\KrImageListBox.cpp
DEP_CPP_KRIMA=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\eventmanager.h"\
	"..\gui\textwidget.h"\
	"..\gui\widget.h"\
	"..\guiExtended\KrImageListBox.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\fontresource.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\guiExtended\KrImageListBox.h
# End Source File
# Begin Source File

SOURCE=..\guiExtended\progress.cpp
DEP_CPP_PROGR=\
	"..\..\gameEngine\gedString.h"\
	"..\..\SDL\SDL\include\begin_code.h"\
	"..\..\SDL\SDL\include\close_code.h"\
	"..\..\SDL\SDL\include\SDL.h"\
	"..\..\SDL\SDL\include\SDL_active.h"\
	"..\..\SDL\SDL\include\SDL_audio.h"\
	"..\..\SDL\SDL\include\SDL_byteorder.h"\
	"..\..\SDL\SDL\include\SDL_cdrom.h"\
	"..\..\SDL\SDL\include\SDL_error.h"\
	"..\..\SDL\SDL\include\SDL_events.h"\
	"..\..\SDL\SDL\include\SDL_getenv.h"\
	"..\..\SDL\SDL\include\SDL_joystick.h"\
	"..\..\SDL\SDL\include\SDL_keyboard.h"\
	"..\..\SDL\SDL\include\SDL_keysym.h"\
	"..\..\SDL\SDL\include\SDL_main.h"\
	"..\..\SDL\SDL\include\SDL_mouse.h"\
	"..\..\SDL\SDL\include\SDL_mutex.h"\
	"..\..\SDL\SDL\include\SDL_quit.h"\
	"..\..\SDL\SDL\include\SDL_rwops.h"\
	"..\..\SDL\SDL\include\SDL_timer.h"\
	"..\..\SDL\SDL\include\SDL_types.h"\
	"..\..\SDL\SDL\include\SDL_version.h"\
	"..\..\SDL\SDL\include\SDL_video.h"\
	"..\gui\eventmanager.h"\
	"..\gui\widget.h"\
	"..\guiExtended\progress.h"\
	"..\util\glbitstream.h"\
	"..\util\glcirclelist.h"\
	"..\util\gldebug.h"\
	"..\util\gldynarray.h"\
	"..\util\glfixed.h"\
	"..\util\glinsidelist.h"\
	"..\util\gllist.h"\
	"..\util\glmap.h"\
	"..\util\glmemorypool.h"\
	"..\util\glprime.h"\
	"..\util\glstring.h"\
	"..\util\gltypes.h"\
	"..\util\glutil.h"\
	".\action.h"\
	".\box.h"\
	".\boxresource.h"\
	".\color.h"\
	".\dirtyrectangle.h"\
	".\engine.h"\
	".\image.h"\
	".\imagetree.h"\
	".\imnode.h"\
	".\krmath.h"\
	".\kyrabuild.h"\
	".\kyraresource.h"\
	".\painter.h"\
	".\rle.h"\
	".\sprite.h"\
	".\spriteresource.h"\
	".\vault.h"\
	
# End Source File
# Begin Source File

SOURCE=..\guiExtended\progress.h
# End Source File
# End Group
# End Target
# End Project
