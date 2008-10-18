# Microsoft Developer Studio Project File - Name="eiclib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=eiclib - Win32 Symbian Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eiclib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eiclib.mak" CFG="eiclib - Win32 Symbian Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eiclib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "eiclib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "eiclib - Win32 Symbian Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/gameEngine/EiC/Visual", BNFAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eiclib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/Windows/Release"
# PROP Intermediate_Dir "../../../output/Windows/eic/Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /Ob2 /I "../src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "eiclib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/Windows/Debug"
# PROP Intermediate_Dir "../../../output/Windows/eic/Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "eiclib - Win32 Symbian Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "eiclib___Win32_Symbian_Debug"
# PROP BASE Intermediate_Dir "eiclib___Win32_Symbian_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/"
# PROP Intermediate_Dir "../../../output/eic"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MD /W3 /Zi /Od /X /I "../src" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "WIN32" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fo"\Symbian\6.1\Series60\EPOC32\BUILD\SYMBIANPROJECTS\gameEditorSymbian\EPOC\gameEditorSymbian\WINS\UDEB/" /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gameEditorSymbian.PDB" /GF /c
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

# Name "eiclib - Win32 Release"
# Name "eiclib - Win32 Debug"
# Name "eiclib - Win32 Symbian Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Samples"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\main\examples\embedEiC.c

!IF  "$(CFG)" == "eiclib - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Symbian Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\main\main.c

!IF  "$(CFG)" == "eiclib - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Symbian Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\main.c

!IF  "$(CFG)" == "eiclib - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "eiclib - Win32 Symbian Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
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
# End Target
# End Project
