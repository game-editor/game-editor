# Microsoft Developer Studio Project File - Name="staticSDL_mixer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=staticSDL_mixer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL_mixer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "staticSDL_mixer.mak" CFG="staticSDL_mixer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "staticSDL_mixer - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "staticSDL_mixer - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameDevelopment/SDL/SDL_mixer/VisualC", RHJAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "staticSDL_mixer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_mixer"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "../../SDL/include/" /I "..\SDL_mad" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "tremor" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "MID_MUSIC" /D "USE_NATIVE_MIDI" /D "OGG_MUSIC" /D "USE_RWOPS" /D "_LIB" /D "FPM_DEFAULT" /D "OPT_SPEED" /D "ASO_INTERLEAVE1" /D "ASO_ZEROCHECK" /D inline=__inline /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../output/Windows/SDL_mixer.lib"

!ELSEIF  "$(CFG)" == "staticSDL_mixer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../output/Windows/"
# PROP Intermediate_Dir "../../../output/Windows/SDL_mixer"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../../SDL/include/" /I "..\SDL_mad\\" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "tremor" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "MID_MUSIC" /D "OGG_MUSIC" /D "USE_RWOPS" /D "USE_NATIVE_MIDI" /D "_LIB" /D "FPM_DEFAULT" /D "OPT_SPEED" /D "ASO_INTERLEAVE1" /D "ASO_ZEROCHECK" /D inline=__inline /FR /YX /FD /GZ /c
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

# Name "staticSDL_mixer - Win32 Release"
# Name "staticSDL_mixer - Win32 Debug"
# Begin Group "tremor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tremor\asm_arm.h
# End Source File
# Begin Source File

SOURCE=..\tremor\backends.h
# End Source File
# Begin Source File

SOURCE=..\tremor\bitwise.c
# End Source File
# Begin Source File

SOURCE=..\tremor\block.c
# End Source File
# Begin Source File

SOURCE=..\tremor\codebook.c
# End Source File
# Begin Source File

SOURCE=..\tremor\codebook.h
# End Source File
# Begin Source File

SOURCE=..\tremor\codec_internal.h
# End Source File
# Begin Source File

SOURCE=..\tremor\config_types.h
# End Source File
# Begin Source File

SOURCE=..\tremor\floor0.c
# End Source File
# Begin Source File

SOURCE=..\tremor\floor1.c
# End Source File
# Begin Source File

SOURCE=..\tremor\framing.c
# End Source File
# Begin Source File

SOURCE=..\tremor\info.c
# End Source File
# Begin Source File

SOURCE=..\tremor\ivorbiscodec.h
# End Source File
# Begin Source File

SOURCE=..\tremor\ivorbisfile.h
# End Source File
# Begin Source File

SOURCE=..\tremor\lsp_lookup.h
# End Source File
# Begin Source File

SOURCE=..\tremor\mapping0.c
# End Source File
# Begin Source File

SOURCE=..\tremor\mdct.c
# End Source File
# Begin Source File

SOURCE=..\tremor\mdct.h
# End Source File
# Begin Source File

SOURCE=..\tremor\mdct_lookup.h
# End Source File
# Begin Source File

SOURCE=..\tremor\misc.h
# End Source File
# Begin Source File

SOURCE=..\tremor\ogg.h
# End Source File
# Begin Source File

SOURCE=..\tremor\os.h
# End Source File
# Begin Source File

SOURCE=..\tremor\os_types.h
# End Source File
# Begin Source File

SOURCE=..\tremor\registry.c
# End Source File
# Begin Source File

SOURCE=..\tremor\registry.h
# End Source File
# Begin Source File

SOURCE=..\tremor\res012.c
# End Source File
# Begin Source File

SOURCE=..\tremor\sharedbook.c
# End Source File
# Begin Source File

SOURCE=..\tremor\synthesis.c
# End Source File
# Begin Source File

SOURCE=..\tremor\vorbisfile.c
# End Source File
# Begin Source File

SOURCE=..\tremor\window.c
# End Source File
# Begin Source File

SOURCE=..\tremor\window.h
# End Source File
# Begin Source File

SOURCE=..\tremor\window_lookup.h
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
