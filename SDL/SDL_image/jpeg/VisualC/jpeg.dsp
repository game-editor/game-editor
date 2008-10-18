# Microsoft Developer Studio Project File - Name="jpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpeg.mak" CFG="jpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_image/jpeg/VisualC", LYLAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../output/Windows/jpeg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../output/Windows/"
# PROP Intermediate_Dir "../../../../output/Windows/jpeg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "jpeg - Win32 Release"
# Name "jpeg - Win32 Debug"
# Begin Source File

SOURCE=..\CDERROR.H
# End Source File
# Begin Source File

SOURCE=..\CDJPEG.H
# End Source File
# Begin Source File

SOURCE=..\JCAPIMIN.C
# End Source File
# Begin Source File

SOURCE=..\JCAPISTD.C
# End Source File
# Begin Source File

SOURCE=..\JCCOEFCT.C
# End Source File
# Begin Source File

SOURCE=..\JCCOLOR.C
# End Source File
# Begin Source File

SOURCE=..\JCDCTMGR.C
# End Source File
# Begin Source File

SOURCE=..\JCHUFF.C
# End Source File
# Begin Source File

SOURCE=..\JCHUFF.H
# End Source File
# Begin Source File

SOURCE=..\JCINIT.C
# End Source File
# Begin Source File

SOURCE=..\JCMAINCT.C
# End Source File
# Begin Source File

SOURCE=..\JCMARKER.C
# End Source File
# Begin Source File

SOURCE=..\JCMASTER.C
# End Source File
# Begin Source File

SOURCE=..\JCOMAPI.C
# End Source File
# Begin Source File

SOURCE=..\JCONFIG.H
# End Source File
# Begin Source File

SOURCE=..\JCPARAM.C
# End Source File
# Begin Source File

SOURCE=..\JCPHUFF.C
# End Source File
# Begin Source File

SOURCE=..\JCPREPCT.C
# End Source File
# Begin Source File

SOURCE=..\JCSAMPLE.C
# End Source File
# Begin Source File

SOURCE=..\JCTRANS.C
# End Source File
# Begin Source File

SOURCE=..\JDAPIMIN.C
# End Source File
# Begin Source File

SOURCE=..\JDAPISTD.C
# End Source File
# Begin Source File

SOURCE=..\JDATADST.C
# End Source File
# Begin Source File

SOURCE=..\JDATASRC.C
# End Source File
# Begin Source File

SOURCE=..\JDCOEFCT.C
# End Source File
# Begin Source File

SOURCE=..\JDCOLOR.C
# End Source File
# Begin Source File

SOURCE=..\JDCT.H
# End Source File
# Begin Source File

SOURCE=..\JDDCTMGR.C
# End Source File
# Begin Source File

SOURCE=..\JDHUFF.C
# End Source File
# Begin Source File

SOURCE=..\JDHUFF.H
# End Source File
# Begin Source File

SOURCE=..\JDINPUT.C
# End Source File
# Begin Source File

SOURCE=..\JDMAINCT.C
# End Source File
# Begin Source File

SOURCE=..\JDMARKER.C
# End Source File
# Begin Source File

SOURCE=..\JDMASTER.C
# End Source File
# Begin Source File

SOURCE=..\JDMERGE.C
# End Source File
# Begin Source File

SOURCE=..\JDPHUFF.C
# End Source File
# Begin Source File

SOURCE=..\JDPOSTCT.C
# End Source File
# Begin Source File

SOURCE=..\JDSAMPLE.C
# End Source File
# Begin Source File

SOURCE=..\JDTRANS.C
# End Source File
# Begin Source File

SOURCE=..\JERROR.C
# End Source File
# Begin Source File

SOURCE=..\JERROR.H
# End Source File
# Begin Source File

SOURCE=..\JFDCTFLT.C
# End Source File
# Begin Source File

SOURCE=..\JFDCTFST.C
# End Source File
# Begin Source File

SOURCE=..\JFDCTINT.C
# End Source File
# Begin Source File

SOURCE=..\JIDCTFLT.C
# End Source File
# Begin Source File

SOURCE=..\JIDCTFST.C
# End Source File
# Begin Source File

SOURCE=..\JIDCTINT.C
# End Source File
# Begin Source File

SOURCE=..\JIDCTRED.C
# End Source File
# Begin Source File

SOURCE=..\JINCLUDE.H
# End Source File
# Begin Source File

SOURCE=..\JMEMMGR.C
# End Source File
# Begin Source File

SOURCE=..\JMEMNOBS.C
# End Source File
# Begin Source File

SOURCE=..\JMEMSYS.H
# End Source File
# Begin Source File

SOURCE=..\JMORECFG.H
# End Source File
# Begin Source File

SOURCE=..\JPEGINT.H
# End Source File
# Begin Source File

SOURCE=..\JPEGLIB.H
# End Source File
# Begin Source File

SOURCE=..\JPEGTRAN.C
# End Source File
# Begin Source File

SOURCE=..\JQUANT1.C
# End Source File
# Begin Source File

SOURCE=..\JQUANT2.C
# End Source File
# Begin Source File

SOURCE=..\JUTILS.C
# End Source File
# Begin Source File

SOURCE=..\JVERSION.H
# End Source File
# End Target
# End Project
