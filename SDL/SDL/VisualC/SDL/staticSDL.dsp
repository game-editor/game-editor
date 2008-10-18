# Microsoft Developer Studio Project File - Name="staticSDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=staticSDL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL.mak" CFG="staticSDL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "staticSDL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "staticSDL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL/VisualC/SDL", EOIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "staticSDL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../output/Windows/Release"
# PROP Intermediate_Dir "../../../../output/Windows/staticSDL/Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "..\..\src" /I "..\..\src\audio" /I "..\..\src\video" /I "..\..\src\video\wincommon" /I "..\..\src\video\windx5" /I "..\..\src\events" /I "..\..\src\joystick" /I "..\..\src\cdrom" /I "..\..\src\thread" /I "..\..\src\thread\win32" /I "..\..\src\timer" /I "..\..\include" /I "..\..\include\SDL" /D "HAVE_OPENGL" /D "USE_ASM_MIXER_VC" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ENABLE_WINDIB" /D _WIN32_WINNT=0x0400 /D "_LIB" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /U "ENABLE_DIRECTX" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "staticSDL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../output/Windows/Debug"
# PROP Intermediate_Dir "../../../../output/Windows/staticSDL/Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\src" /I "..\..\src\audio" /I "..\..\src\video" /I "..\..\src\video\wincommon" /I "..\..\src\video\windx5" /I "..\..\src\events" /I "..\..\src\joystick" /I "..\..\src\cdrom" /I "..\..\src\thread" /I "..\..\src\thread\win32" /I "..\..\src\timer" /I "..\..\include" /I "..\..\include\SDL" /D "HAVE_OPENGL" /D "USE_ASM_MIXER_VC" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ENABLE_WINDIB" /D _WIN32_WINNT=0x0400 /D "_LIB" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /U "ENABLE_DIRECTX" /U "GL_OES_VERSION_1_1" /U "USE_PRIMARY_BUFFER" /FR /YX /FD /GZ /c
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

# Name "staticSDL - Win32 Release"
# Name "staticSDL - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\SDL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_active.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_audio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_audiocvt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_audiomem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_blit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_blit_0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_blit_1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_blit_A.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_blit_N.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_bmp.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cpuinfo\SDL_cpuinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_cursor.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\windib\SDL_dibaudio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\windib\SDL_dibevents.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\windib\SDL_dibvideo.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\windx5\SDL_dx5audio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\windx5\SDL_dx5events.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\windx5\SDL_dx5video.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\windx5\SDL_dx5yuv.c
# End Source File
# Begin Source File

SOURCE=..\..\src\endian\SDL_endian.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SDL_error.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_events.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_expose.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SDL_fatal.c
# End Source File
# Begin Source File

SOURCE=..\..\src\joystick\SDL_joystick.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_keyboard.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_mixer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_mixer_MMX_VC.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_mouse.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_pixels.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_quit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\events\SDL_resize.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_RLEaccel.c
# End Source File
# Begin Source File

SOURCE=..\..\src\file\SDL_rwops.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_stretch.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_surface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\wincommon\SDL_sysevents.c
# End Source File
# Begin Source File

SOURCE=..\..\src\joystick\dummy\SDL_sysjoystick.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\wincommon\SDL_sysmouse.c
# End Source File
# Begin Source File

SOURCE=..\..\src\thread\win32\SDL_sysmutex.c
# End Source File
# Begin Source File

SOURCE=..\..\src\thread\win32\SDL_syssem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\thread\win32\SDL_systhread.c
# End Source File
# Begin Source File

SOURCE=..\..\src\timer\win32\SDL_systimer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\wincommon\SDL_syswm.c
# End Source File
# Begin Source File

SOURCE=..\..\src\thread\SDL_thread.c
# End Source File
# Begin Source File

SOURCE=..\..\src\timer\SDL_timer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_video.c
# End Source File
# Begin Source File

SOURCE=..\..\src\audio\SDL_wave.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\wincommon\SDL_wingl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_yuv.c
# End Source File
# Begin Source File

SOURCE=..\..\src\video\SDL_yuv_sw.c
# End Source File
# End Target
# End Project
