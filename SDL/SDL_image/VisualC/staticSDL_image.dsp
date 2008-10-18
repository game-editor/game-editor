# Microsoft Developer Studio Project File - Name="staticSDL_image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=staticSDL_image - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL_image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL_image.mak" CFG="staticSDL_image - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "staticSDL_image - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "staticSDL_image - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_image/VisualC", NWIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "staticSDL_image - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_image"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "../../SDL/include" /I "../png/libpng" /I "../png/zlib" /I "../jpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "LOAD_BMP" /D "LOAD_GIF" /D "LOAD_LBM" /D "LOAD_PCX" /D "LOAD_PNM" /D "LOAD_XPM" /D "LOAD_JPG" /D "LOAD_PNG" /D "LOAD_TGA" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../output/Windows/SDL_image.lib"

!ELSEIF  "$(CFG)" == "staticSDL_image - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_image"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../../SDL/include" /I "../png/libpng" /I "../png/zlib" /I "../jpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "LOAD_BMP" /D "LOAD_GIF" /D "LOAD_LBM" /D "LOAD_PCX" /D "LOAD_PNM" /D "LOAD_XPM" /D "LOAD_JPG" /D "LOAD_PNG" /D "LOAD_TGA" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "staticSDL_image - Win32 Release"
# Name "staticSDL_image - Win32 Debug"
# Begin Source File

SOURCE=..\IMG.c
# End Source File
# Begin Source File

SOURCE=..\IMG_bmp.c
# End Source File
# Begin Source File

SOURCE=..\IMG_gif.c
# End Source File
# Begin Source File

SOURCE=..\IMG_jpg.c
# End Source File
# Begin Source File

SOURCE=..\IMG_lbm.c
# End Source File
# Begin Source File

SOURCE=..\IMG_pcx.c
# End Source File
# Begin Source File

SOURCE=..\IMG_png.c
# End Source File
# Begin Source File

SOURCE=..\IMG_pnm.c
# End Source File
# Begin Source File

SOURCE=..\IMG_tga.c
# End Source File
# Begin Source File

SOURCE=..\IMG_tif.c
# End Source File
# Begin Source File

SOURCE=..\IMG_xcf.c
# End Source File
# Begin Source File

SOURCE=..\IMG_xpm.c
# End Source File
# Begin Source File

SOURCE=..\IMG_xxx.c
# End Source File
# Begin Source File

SOURCE=..\SDL_image.h
# End Source File
# End Target
# End Project
