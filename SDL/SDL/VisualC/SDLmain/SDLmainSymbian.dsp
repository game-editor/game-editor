# Microsoft Developer Studio Project File - Name="SDLMAIN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SDLMAIN - Win32 Uni Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SDLmainSymbian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SDLmainSymbian.mak" CFG="SDLMAIN - Win32 Uni Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SDLMAIN - Win32 Uni Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SDLMAIN - Win32 Uni Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL/VisualC/SDLmain", NOIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SDLMAIN - Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Win32_U0"
# PROP BASE Intermediate_Dir ".\Win32_U0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../output/"
# PROP Intermediate_Dir "../../../../output/SDL_main"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MDd /W3 /Zi /Od /X /I "..\SDL\INCLUDE" /I "..\..\include" /I "..\..\include\SDL" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "_DEBUG" /D "_UNICODE" /FR /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\SDLMAIN.PDB" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estw32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\wserv.lib" /nologo /out:"../../../../output\SDLmain.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ELSEIF  "$(CFG)" == "SDLMAIN - Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Win32_Un"
# PROP BASE Intermediate_Dir ".\Win32_Un"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../output/"
# PROP Intermediate_Dir "../../../../output/SDL_main"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MD /W3 /O1 /Op /X /I "\SYMBIANPROJECTS\SDL\INCLUDE" /I "..\..\include" /I "..\..\include\SDL" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "NDEBUG" /D "_UNICODE" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\euser.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\fbscli.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estlib.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ws32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\hal.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gdi.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estw32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\wserv.lib" /nologo /out:"../../../../output\SDLmain.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ENDIF 

# Begin Target

# Name "SDLMAIN - Win32 Uni Debug"
# Name "SDLMAIN - Win32 Uni Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\src\main\epoc\SDL_main.cpp
DEP_CPP_SDL_M=\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\apgtask.h"\
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
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\estlib.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\estw32.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\f32file.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fbs.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\fntstore.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\gdi.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\globalflags.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\openfont.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32std.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.h"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\s32strm.inl"\
	"..\..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\w32std.h"\
	"..\..\include\begin_code.h"\
	"..\..\include\close_code.h"\
	"..\..\include\SDL_error.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\include\begin_code.h
# End Source File
# Begin Source File

SOURCE=..\..\include\close_code.h
# End Source File
# Begin Source File

SOURCE=..\..\include\SDL_error.h
# End Source File
# End Group
# End Target
# End Project
