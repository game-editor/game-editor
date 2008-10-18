# Microsoft Developer Studio Project File - Name="SDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SDL - Win32 Uni Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdlSymbian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdlSymbian.mak" CFG="SDL - Win32 Uni Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SDL - Win32 Uni Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SDL - Win32 Uni Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL/VisualC/SDL", EOIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SDL - Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Win32_U0"
# PROP BASE Intermediate_Dir ".\Win32_U0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../output/"
# PROP Intermediate_Dir "../../../../output/SDL"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MDd /W3 /Zi /Od /X /I "..\..\..\SDL\INCLUDE" /I "..\..\..\SDL\SRC" /I "..\..\..\SDL\SRC\VIDEO" /I "..\..\..\SDL\SRC\VIDEO\EPOC" /I "..\..\..\SDL\SRC\EVENTS" /I "..\..\..\SDL\SRC\MAIN\EPOC" /I "..\..\..\SDL\SRC\THREAD" /I "..\..\..\SDL\SRC\THREAD\GENERIC" /I "..\..\..\SDL\SRC\THREAD\EPOC" /I "..\..\..\SDL\SRC\TIMER" /I "..\..\..\SDL\SRC\JOYSTICK" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "NO_SIGNAL_H" /D "ENABLE_EPOC" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /D "DISABLE_AUDIO" /D "_DEBUG" /D "_UNICODE" /U "_WIN32" /FR /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\SDL.PDB" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 /nologo /out:"../../../../output/SDL.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ELSEIF  "$(CFG)" == "SDL - Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Win32_Un"
# PROP BASE Intermediate_Dir ".\Win32_Un"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../output/"
# PROP Intermediate_Dir "../../../../output/SDL"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MD /W3 /O1 /Op /X /I "..\SDL\INCLUDE" /I "..\SDL\SRC" /I "..\SDL\SRC\VIDEO" /I "..\SDL\SRC\VIDEO\EPOC" /I "..\SDL\SRC\EVENTS" /I "..\SDL\SRC\MAIN\EPOC" /I "..\SDL\SRC\THREAD" /I "..\SDL\SRC\THREAD\GENERIC" /I "..\SDL\SRC\THREAD\EPOC" /I "..\SDL\SRC\TIMER" /I "..\SDL\SRC\JOYSTICK" /I "..\..\..\SDL\INCLUDE" /I "..\..\..\SDL\SRC" /I "..\..\..\SDL\SRC\VIDEO" /I "..\..\..\SDL\SRC\VIDEO\EPOC" /I "..\..\..\SDL\SRC\EVENTS" /I "..\..\..\SDL\SRC\MAIN\EPOC" /I "..\..\..\SDL\SRC\THREAD" /I "..\..\..\SDL\SRC\THREAD\GENERIC" /I "..\..\..\SDL\SRC\THREAD\EPOC" /I "..\..\..\SDL\SRC\TIMER" /I "..\..\..\SDL\SRC\JOYSTICK" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "NO_SIGNAL_H" /D "ENABLE_EPOC" /D "DISABLE_JOYSTICK" /D "DISABLE_CDROM" /D "DISABLE_AUDIO" /D "NDEBUG" /D "_UNICODE" /U "_WIN32" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\euser.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\fbscli.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estlib.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ws32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\hal.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gdi.lib" /nologo /out:"../../../../output/SDL.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ENDIF 

# Begin Target

# Name "SDL - Win32 Uni Debug"
# Name "SDL - Win32 Uni Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Sdl.c
DEP_CPP_SDL_C=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\Sdl_fatal.h"\
	"..\..\SRC\VIDEO\Sdl_leaks.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\EPOC\Sdl.mmp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_active.c
DEP_CPP_SDL_A=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_blit.c
DEP_CPP_SDL_B=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_memops.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_rleaccel_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_blit_0.c
DEP_CPP_SDL_BL=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_blit_1.c
DEP_CPP_SDL_BLI=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_blit_a.c
DEP_CPP_SDL_BLIT=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_blit_n.c
DEP_CPP_SDL_BLIT_=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	
NODEP_CPP_SDL_BLIT_=\
	"..\..\SRC\Video\HeadMMX.h"\
	"..\..\SRC\Video\HeadX86.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_bmp.c
DEP_CPP_SDL_BM=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_cursor.c
DEP_CPP_SDL_CU=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\EVENTS\Sdl_sysevents.h"\
	"..\..\SRC\VIDEO\Default_cursor.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\Sdl_cursor_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Endian\sdl_endian.c
DEP_CPP_SDL_E=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\epoc\sdl_epocevents.cpp
DEP_CPP_SDL_EP=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitbase.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitdev.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitdev.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitmap.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitstd.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32base.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32base.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32hal.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32keys.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32pccd.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32svr.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fbs.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fntstore.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\hal.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\hal_data.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\openfont.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\w32std.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\VIDEO\EPOC\Sdl_epocevents_c.h"\
	"..\..\SRC\VIDEO\EPOC\Sdl_epocvideo.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\epoc\sdl_epocvideo.cpp
DEP_CPP_SDL_EPO=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitbase.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitdev.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitdev.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitmap.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\bitstd.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\coedef.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32base.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32base.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32hal.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32keys.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32pccd.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32svr.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fbs.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fntstore.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\guldef.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\hal.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\hal_data.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\openfont.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\w32std.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\EPOC\Sdl_epocevents_c.h"\
	"..\..\SRC\VIDEO\EPOC\Sdl_epocvideo.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Sdl_error.c
DEP_CPP_SDL_ER=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_types.h"\
	"..\..\SRC\Sdl_error_c.h"\
	"..\..\SRC\THREAD\GENERIC\Sdl_systhread_c.h"\
	"..\..\SRC\THREAD\Sdl_thread_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_events.c
DEP_CPP_SDL_EV=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\INCLUDE\Sdl_thread.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\EVENTS\Sdl_sysevents.h"\
	"..\..\src\joystick\SDL_joystick_c.h"\
	"..\..\SRC\TIMER\Sdl_timer_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Sdl_fatal.c
DEP_CPP_SDL_F=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\Sdl_fatal.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_gamma.c
DEP_CPP_SDL_G=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_keyboard.c
DEP_CPP_SDL_K=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\EVENTS\Sdl_sysevents.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_mouse.c
DEP_CPP_SDL_M=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\VIDEO\Sdl_cursor_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_pixels.c
DEP_CPP_SDL_P=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_rleaccel_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_quit.c
DEP_CPP_SDL_Q=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Events\sdl_resize.c
DEP_CPP_SDL_R=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_rleaccel.c
DEP_CPP_SDL_RL=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_memops.h"\
	"..\..\SRC\VIDEO\Sdl_rleaccel_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\File\sdl_rwops.c
DEP_CPP_SDL_RW=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_stretch.c
DEP_CPP_SDL_S=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_surface.c
DEP_CPP_SDL_SU=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\Sdl_cursor_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_leaks.h"\
	"..\..\SRC\VIDEO\Sdl_memops.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_rleaccel_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Thread\epoc\sdl_sysmutex.cpp
DEP_CPP_SDL_SY=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_types.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Thread\epoc\sdl_syssem.cpp
DEP_CPP_SDL_SYS=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\INCLUDE\Sdl_thread.h"\
	"..\..\include\SDL_types.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Thread\epoc\sdl_systhread.cpp
DEP_CPP_SDL_SYST=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\INCLUDE\Sdl_thread.h"\
	"..\..\include\SDL_types.h"\
	"..\..\SRC\Sdl_error_c.h"\
	"..\..\SRC\THREAD\EPOC\Sdl_systhread_c.h"\
	"..\..\SRC\THREAD\Sdl_systhread.h"\
	"..\..\SRC\THREAD\Sdl_thread_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Timer\epoc\sdl_systimer.cpp
DEP_CPP_SDL_SYSTI=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32def.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des16.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32des8.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32hal.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\e32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\INCLUDE\Sdl_thread.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\SRC\TIMER\Sdl_timer_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Thread\sdl_thread.c
DEP_CPP_SDL_T=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\INCLUDE\Sdl_thread.h"\
	"..\..\include\SDL_types.h"\
	"..\..\SRC\Sdl_error_c.h"\
	"..\..\SRC\THREAD\GENERIC\Sdl_systhread_c.h"\
	"..\..\SRC\THREAD\Sdl_systhread.h"\
	"..\..\SRC\THREAD\Sdl_thread_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Timer\sdl_timer.c
DEP_CPP_SDL_TI=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\SRC\TIMER\Sdl_systimer.h"\
	"..\..\SRC\TIMER\Sdl_timer_c.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_video.c
DEP_CPP_SDL_V=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\EVENTS\Sdl_events_c.h"\
	"..\..\SRC\EVENTS\Sdl_sysevents.h"\
	"..\..\SRC\VIDEO\Sdl_blit.h"\
	"..\..\SRC\VIDEO\Sdl_cursor_c.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_pixels_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_yuv.c
DEP_CPP_SDL_Y=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	"..\..\SRC\VIDEO\Sdl_yuv_sw_c.h"\
	"..\..\SRC\VIDEO\Sdl_yuvfuncs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_yuv_mmx.c
DEP_CPP_SDL_YU=\
	"..\..\include\SDL_types.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Video\sdl_yuv_sw.c
DEP_CPP_SDL_YUV=\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\INCLUDE\Sdl_syswm.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	"..\..\SRC\VIDEO\SDL_glfuncs.h"\
	"..\..\SRC\VIDEO\Sdl_stretch_c.h"\
	"..\..\SRC\VIDEO\Sdl_sysvideo.h"\
	"..\..\SRC\VIDEO\Sdl_yuv_sw_c.h"\
	"..\..\SRC\VIDEO\Sdl_yuvfuncs.h"\
	
NODEP_CPP_SDL_YUV=\
	"..\..\SRC\Video\HeadX86.h"\
	
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Begin_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Close_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Default_cursor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_active.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_audio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_blit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_blit_a.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_byteorder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_cdrom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_cursor_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_endian.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\EPOC\Sdl_epocevents_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\EPOC\Sdl_epocvideo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Sdl_error_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_events.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\EVENTS\Sdl_events_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\Sdl_fatal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_getenv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_joystick.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_keyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_keysym.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_leaks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_main.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_memops.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_mouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_pixels_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_quit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_rleaccel_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_rwops.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_stretch_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\EVENTS\Sdl_sysevents.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\THREAD\Sdl_systhread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\THREAD\EPOC\Sdl_systhread_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\THREAD\GENERIC\Sdl_systhread_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\TIMER\Sdl_systimer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_sysvideo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_syswm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_thread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\THREAD\Sdl_thread_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_timer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\TIMER\Sdl_timer_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\INCLUDE\Sdl_video.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_yuv_sw_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL\SRC\VIDEO\Sdl_yuvfuncs.h
# End Source File
# End Group
# Begin Group "SDL_image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG.c
DEP_CPP_IMG_C=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_bmp.c
DEP_CPP_IMG_B=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_gif.c
DEP_CPP_IMG_G=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
NODEP_CPP_IMG_G=\
	"..\..\..\SDL_image\image.h"\
	"..\..\..\SDL_image\rwTable.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_jpg.c
DEP_CPP_IMG_J=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_lbm.c
DEP_CPP_IMG_L=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_pcx.c
DEP_CPP_IMG_P=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_png.c
DEP_CPP_IMG_PN=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_pnm.c
DEP_CPP_IMG_PNM=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_tga.c
DEP_CPP_IMG_T=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_tif.c
DEP_CPP_IMG_TI=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_xcf.c
DEP_CPP_IMG_X=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_endian.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_xpm.c
DEP_CPP_IMG_XP=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\IMG_xxx.c
DEP_CPP_IMG_XX=\
	"..\..\..\SDL_image\SDL_image.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL.h"\
	"..\..\include\SDL_active.h"\
	"..\..\include\SDL_audio.h"\
	"..\..\include\SDL_byteorder.h"\
	"..\..\include\SDL_cdrom.h"\
	"..\..\include\SDL_error.h"\
	"..\..\include\SDL_events.h"\
	"..\..\include\SDL_getenv.h"\
	"..\..\include\SDL_joystick.h"\
	"..\..\include\SDL_keyboard.h"\
	"..\..\include\SDL_keysym.h"\
	"..\..\include\SDL_main.h"\
	"..\..\include\SDL_mouse.h"\
	"..\..\include\SDL_mutex.h"\
	"..\..\include\SDL_quit.h"\
	"..\..\include\SDL_rwops.h"\
	"..\..\include\SDL_timer.h"\
	"..\..\include\SDL_types.h"\
	"..\..\include\SDL_version.h"\
	"..\..\include\SDL_video.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\SDL_image\SDL_image.h
# End Source File
# End Group
# End Target
# End Project
