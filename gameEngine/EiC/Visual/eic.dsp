# Microsoft Developer Studio Project File - Name="eic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=eic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eic.mak" CFG="eic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eic - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "eic - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion "6.0"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/"
# PROP Intermediate_Dir "../../../output/eic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "eic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/"
# PROP Intermediate_Dir "../../../output/eic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "eic - Win32 Release"
# Name "eic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\module\stdClib\src\assert.c
# End Source File
# Begin Source File

SOURCE=..\src\callback.c
# End Source File
# Begin Source File

SOURCE=..\src\cdecl.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\ctype.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\dirent.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\eicfftoa.c
# End Source File
# Begin Source File

SOURCE=..\src\eicmod.c
# End Source File
# Begin Source File

SOURCE=..\src\emitter.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\errno.c
# End Source File
# Begin Source File

SOURCE=..\src\error.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\fcntl.c
# End Source File
# Begin Source File

SOURCE=..\src\func.c
# End Source File
# Begin Source File

SOURCE=..\src\interpre.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\itoa.c
# End Source File
# Begin Source File

SOURCE=..\src\lexer.c
# End Source File
# Begin Source File

SOURCE=..\src\main.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\math.c
# End Source File
# Begin Source File

SOURCE=..\src\optomizer.c
# End Source File
# Begin Source File

SOURCE=..\src\parser.c
# End Source File
# Begin Source File

SOURCE=..\src\preexpr.c
# End Source File
# Begin Source File

SOURCE=..\src\preproc.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\printf.c
# End Source File
# Begin Source File

SOURCE=..\src\readline.c
# End Source File
# Begin Source File

SOURCE=..\src\reset.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\signal.c
# End Source File
# Begin Source File

SOURCE=..\src\stab.c
# End Source File
# Begin Source File

SOURCE=..\src\starteic.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stat.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdarg.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdClib.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdio.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdlib.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\string.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\strrev.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\strround.c
# End Source File
# Begin Source File

SOURCE=..\src\symbol.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\time.c
# End Source File
# Begin Source File

SOURCE=..\src\typemod.c
# End Source File
# Begin Source File

SOURCE=..\src\typesets.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\unistd.c
# End Source File
# Begin Source File

SOURCE=..\src\ymem.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\module\stdClib\src\assert.h
# End Source File
# Begin Source File

SOURCE=..\src\assertp.h
# End Source File
# Begin Source File

SOURCE=..\src\cdecl.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\ctype.h
# End Source File
# Begin Source File

SOURCE=..\src\datastruct.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\dirent.h
# End Source File
# Begin Source File

SOURCE=..\src\eic.h
# End Source File
# Begin Source File

SOURCE=..\src\eicval.h
# End Source File
# Begin Source File

SOURCE=..\src\emitter.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\errno.h
# End Source File
# Begin Source File

SOURCE=..\src\error.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\fcntl.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\float.h
# End Source File
# Begin Source File

SOURCE=..\src\func.h
# End Source File
# Begin Source File

SOURCE=..\src\global.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\iso646.h
# End Source File
# Begin Source File

SOURCE=..\src\lexer.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\limits.h
# End Source File
# Begin Source File

SOURCE=..\src\MachSet.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\math.h
# End Source File
# Begin Source File

SOURCE=..\src\metric.h
# End Source File
# Begin Source File

SOURCE=..\src\modules.h
# End Source File
# Begin Source File

SOURCE=..\src\parser.h
# End Source File
# Begin Source File

SOURCE=..\src\preproc.h
# End Source File
# Begin Source File

SOURCE=..\src\reset.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\setjmp.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\signal.h
# End Source File
# Begin Source File

SOURCE=..\src\stab.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdarg.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdClib2eic.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stddef.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdio.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdlib.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdliblocal.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\string.h
# End Source File
# Begin Source File

SOURCE=..\src\symbol.h
# End Source File
# Begin Source File

SOURCE=..\src\symentry.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\termio.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\termios.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\time.h
# End Source File
# Begin Source File

SOURCE=..\src\typemod.h
# End Source File
# Begin Source File

SOURCE=..\src\typesets.h
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\unistd.h
# End Source File
# Begin Source File

SOURCE=..\src\xalloc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
