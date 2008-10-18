# Microsoft Developer Studio Project File - Name="SDL_mixer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SDL_mixer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SDL_mixer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SDL_mixer.mak" CFG="SDL_mixer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SDL_mixer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SDL_mixer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SDL_mixer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_mixer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../../SDL/include/" /I "..\SDL_mad" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "..\OggVorbis\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "MID_MUSIC" /D "USE_NATIVE_MIDI" /D "OGG_MUSIC" /D "MP3_MUSIC" /D "USE_RWOPS" /U "USE_TIMIDITY_MIDI" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib ../../../output/SDL.lib ../../../output/smpeg.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "SDL_mixer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_mixer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../SDL/include/" /I "..\SDL_mad" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "..\OggVorbis\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "MID_MUSIC" /D "USE_TIMIDITY_MIDI" /D "OGG_MUSIC" /D "MP3_MUSIC" /D "USE_RWOPS" /D "USE_NATIVE_MIDI" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib ../../../output/SDL.lib ../../../output/smpeg.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SDL_mixer - Win32 Release"
# Name "SDL_mixer - Win32 Debug"
# Begin Group "OggVorbis"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\OggVorbis\bitrate.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\bitwise.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\block.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\codebook.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\envelope.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\floor0.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\floor1.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\framing.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\info.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\lookup.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\lpc.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\lsp.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\mapping0.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\mdct.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\psy.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\registry.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\res0.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\sharedbook.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\smallft.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\synthesis.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\vorbisfile.c
# End Source File
# Begin Source File

SOURCE=..\OggVorbis\window.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\effect_position.c
# End Source File
# Begin Source File

SOURCE=..\effect_stereoreverse.c
# End Source File
# Begin Source File

SOURCE=..\effects_internal.c
# End Source File
# Begin Source File

SOURCE=..\effects_internal.h
# End Source File
# Begin Source File

SOURCE=..\load_aiff.c
# End Source File
# Begin Source File

SOURCE=..\load_aiff.h
# End Source File
# Begin Source File

SOURCE=..\load_ogg.c
# End Source File
# Begin Source File

SOURCE=..\load_ogg.h
# End Source File
# Begin Source File

SOURCE=..\load_voc.c
# End Source File
# Begin Source File

SOURCE=..\load_voc.h
# End Source File
# Begin Source File

SOURCE=..\mixer.c
# End Source File
# Begin Source File

SOURCE=..\music.c
# End Source File
# Begin Source File

SOURCE=..\music_cmd.c
# End Source File
# Begin Source File

SOURCE=..\music_cmd.h
# End Source File
# Begin Source File

SOURCE=..\music_ogg.c
# End Source File
# Begin Source File

SOURCE=..\music_ogg.h
# End Source File
# Begin Source File

SOURCE=..\SDL_mixer.h
# End Source File
# Begin Source File

SOURCE=Version.rc
# End Source File
# Begin Source File

SOURCE=..\wavestream.c
# End Source File
# Begin Source File

SOURCE=..\wavestream.h
# End Source File
# End Target
# End Project
