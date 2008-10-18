# Microsoft Developer Studio Project File - Name="SDL_image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SDL_image - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SDL_image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SDL_image.mak" CFG="SDL_image - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SDL_image - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SDL_image - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_image", CVIAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SDL_image - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_image"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../../SDL/include" /I "../png/libpng" /I "../png/zlib" /I "../jpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "LOAD_BMP" /D "LOAD_GIF" /D "LOAD_LBM" /D "LOAD_PCX" /D "LOAD_PNM" /D "LOAD_XPM" /D "LOAD_JPG" /D "LOAD_PNG" /D "LOAD_TGA" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../../output/libpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SDL_image - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_image"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../SDL/include" /I "../png/libpng" /I "../png/zlib" /I "../jpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "LOAD_BMP" /D "LOAD_GIF" /D "LOAD_LBM" /D "LOAD_PCX" /D "LOAD_PNM" /D "LOAD_XPM" /D "LOAD_JPG" /D "LOAD_PNG" /D "LOAD_TGA" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../output/libpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SDL_image - Win32 Release"
# Name "SDL_image - Win32 Debug"
# Begin Group "SDL_image Sources"

# PROP Default_Filter ""
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

SOURCE=Version.rc
# End Source File
# End Group
# Begin Group "SDL_image Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SDL_image.h
# End Source File
# End Group
# End Target
# End Project
