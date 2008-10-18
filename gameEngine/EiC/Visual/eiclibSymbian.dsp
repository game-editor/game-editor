# Microsoft Developer Studio Project File - Name="eiclib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=eiclib - Win32 Uni Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eiclibSymbian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eiclibSymbian.mak" CFG="eiclib - Win32 Uni Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eiclib - Win32 Uni Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "eiclib - Win32 Uni Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/gameEngine/EiC/Visual", BNFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eiclib - Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Win32_U0"
# PROP BASE Intermediate_Dir ".\Win32_U0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output"
# PROP Intermediate_Dir "../../../output/eic"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MDd /W3 /Zi /Od /X /I "../src" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "_DEBUG" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "NO_ENV" /FR /Fd"\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\eiclib.PDB" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 /nologo /out:"../../../output/eiclib.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ELSEIF  "$(CFG)" == "eiclib - Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Win32_Un"
# PROP BASE Intermediate_Dir ".\Win32_Un"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output"
# PROP Intermediate_Dir "../../../output/eic"
MTL=midl.exe
# ADD CPP /nologo /Zp8 /MD /W3 /O1 /Op /X /I "../src" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE" /I "\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC" /D "NDEBUG" /D "__SYMBIAN32__" /D "__VC32__" /D "__WINS__" /D "__AVKON_ELAF__" /D "__EXE__" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /GF /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /machine:IX86
# ADD LIB32 "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\euser.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\fbscli.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\estlib.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\ws32.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\hal.lib" "\Symbian\6.1\Series60\EPOC32\RELEASE\WINS\UDEB\gdi.lib" /nologo /out:"../../../output/eiclib.lib" /subsystem:windows /machine:IX86 /nodefaultlib

!ENDIF 

# Begin Target

# Name "eiclib - Win32 Uni Debug"
# Name "eiclib - Win32 Uni Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\module\stdClib\src\assert.c
DEP_CPP_ASSER=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\callback.c
DEP_CPP_CALLB=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	"..\src\emitter.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\MachSet.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\cdecl.c
DEP_CPP_CDECL=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\cdecl.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\MachSet.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\ctype.c
DEP_CPP_CTYPE=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\dirent.c
DEP_CPP_DIREN=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\dirent.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\dirent.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\reent.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\stdio_t.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\eicfftoa.c
DEP_CPP_EICFF=\
	"..\module\stdClib\src\stdliblocal.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\eicmod.c
DEP_CPP_EICMO=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\cdecl.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\emitter.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\emitter.c
DEP_CPP_EMITT=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\MachSet.h"\
	"..\src\stab.h"\
	"..\src\typemod.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\errno.c
DEP_CPP_ERRNO=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\error.c
DEP_CPP_ERROR=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\fcntl.c
DEP_CPP_FCNTL=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\func.c
DEP_CPP_FUNC_=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\cdecl.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\interpre.c
DEP_CPP_INTER=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\MachSet.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\itoa.c
DEP_CPP_ITOA_=\
	"..\module\stdClib\src\stdliblocal.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\lexer.c
DEP_CPP_LEXER=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\math.c
DEP_CPP_MATH_=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\optomizer.c
DEP_CPP_OPTOM=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\MachSet.h"\
	"..\src\stab.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\parser.c
DEP_CPP_PARSE=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\cdecl.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\MachSet.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\preexpr.c
DEP_CPP_PREEX=\
	"..\src\preproc.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\preproc.c
DEP_CPP_PREPR=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\preexpr.c"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\printf.c
DEP_CPP_PRINT=\
	"..\module\stdClib\src\stdliblocal.h"\
	
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\src\readline.c
DEP_CPP_READL=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\reset.c
DEP_CPP_RESET=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\signal.c
DEP_CPP_SIGNA=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\stab.c
DEP_CPP_STAB_=\
	"..\src\stab.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\starteic.c
DEP_CPP_START=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\emitter.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\MachSet.h"\
	"..\src\preproc.h"\
	"..\src\reset.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stat.c
DEP_CPP_STAT_=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdarg.c
DEP_CPP_STDAR=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdClib.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdio.c
DEP_CPP_STDIO=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\stdlib.c
DEP_CPP_STDLI=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\string.c
DEP_CPP_STRIN=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\strrev.c
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\strround.c
# End Source File
# Begin Source File

SOURCE=..\src\symbol.c
DEP_CPP_SYMBO=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\MachSet.h"\
	"..\src\preproc.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\time.c
DEP_CPP_TIME_=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\typemod.c
DEP_CPP_TYPEM=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\cdecl.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\func.h"\
	"..\src\global.h"\
	"..\src\metric.h"\
	"..\src\stab.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\typesets.c
DEP_CPP_TYPES=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\lexer.h"\
	"..\src\MachSet.h"\
	"..\src\stab.h"\
	"..\src\symbol.h"\
	"..\src\typemod.h"\
	"..\src\typesets.h"\
	"..\src\xalloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\module\stdClib\src\unistd.c
DEP_CPP_UNIST=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\module\stdClib\src\stdliblocal.h"\
	"..\src\eic.h"\
	"..\src\eicval.h"\
	
# End Source File
# Begin Source File

SOURCE=..\src\ymem.c
DEP_CPP_YMEM_=\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\_ansi.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\sys\unistd.h"\
	"..\..\..\..\Symbian\6.1\Series60\EPOC32\INCLUDE\LIBC\unistd.h"\
	"..\src\assertp.h"\
	"..\src\datastruct.h"\
	"..\src\eicval.h"\
	"..\src\global.h"\
	"..\src\stab.h"\
	"..\src\typemod.h"\
	"..\src\xalloc.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
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
