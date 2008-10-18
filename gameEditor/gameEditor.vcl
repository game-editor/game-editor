<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: eiclib - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9DE.tmp" with contents
[
/nologo /out:"../../../output/PocketPC\eiclib.lib" 
\GAMEDEVELOPMENT\output\PocketPC\eic\assert.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\callback.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\cdecl.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\dirent.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\eicfftoa.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\eicmod.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\emitter.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\errno.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\error.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\fcntl.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\func.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\interpre.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\itoa.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\lexer.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\math.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\optomizer.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\parser.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\preexpr.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\preproc.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\printf.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\readline.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\reset.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\signal.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stab.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\starteic.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stat.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stdarg.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stdClib.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stdio.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\stdlib.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\string.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\strrev.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\strround.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\symbol.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\time.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\typemod.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\typesets.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\unistd.obj
\GAMEDEVELOPMENT\output\PocketPC\eic\ymem.obj
]
Creating command line "link.exe -lib @c:\temp\RSP9DE.tmp"
<h3>Output Window</h3>
Creating library...

<h3>
--------------------Configuration: staticSDL - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9DF.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../../../../OpenGL ES/include/" /I "../../../../PocketHAL/" /I "..\..\src" /I "..\..\src\audio" /I "..\..\src\video" /I "..\..\src\video\wincommon" /I "..\..\src\video\windx5" /I "..\..\src\events" /I "..\..\src\joystick" /I "..\..\src\cdrom" /I "..\..\src\thread" /I "..\..\src\thread\win32" /I "..\..\src\timer" /I "..\..\include" /I "..\..\include\SDL" /D "HAVE_OPENGL" /D WIN32_PLATFORM_PSPC=310 /D "NDEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "NO_SIGNAL_H" /D "ENABLE_GAPI" /D "_LIB" /U "ENABLE_WINDIB" /FR"../../../../output/PocketPC/SDL/" /Fp"../../../../output/PocketPC/SDL/staticSDL.pch" /YX /Fo"../../../../output/PocketPC/SDL/" /Fd"../../../../output/PocketPC/SDL/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\gapi\PocketHAL.cpp"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\SDL.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_active.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_audio.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_audiocvt.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_audiodev.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_audiomem.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_blit.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_blit_0.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_blit_1.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_blit_A.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_blit_N.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_bmp.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\cdrom\SDL_cdrom.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_cursor.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\windib\SDL_dibaudio.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\windib\SDL_dibevents.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\endian\SDL_endian.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\SDL_error.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_events.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_expose.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\SDL_fatal.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_gamma.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\gapi\SDL_gapievents.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\gapi\SDL_gapivideo.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\SDL_getenv.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\joystick\SDL_joystick.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_keyboard.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_mixer.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_mouse.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_pixels.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_quit.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\events\SDL_resize.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_RLEaccel.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\file\SDL_rwops.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_stretch.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_surface.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\cdrom\dummy\SDL_syscdrom.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\wincommon\SDL_sysevents.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\joystick\dummy\SDL_sysjoystick.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\wincommon\SDL_sysmouse.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\thread\win32\SDL_sysmutex.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\thread\win32\SDL_syssem.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\thread\win32\SDL_systhread.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\timer\win32\SDL_systimer.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\wincommon\SDL_syswm.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\thread\SDL_thread.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\timer\SDL_timer.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_video.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\audio\SDL_wave.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\wincommon\SDL_wingl.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_yuv.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_yuv_mmx.c"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\SDL_yuv_sw.c"
]
Creating command line "clarm.exe @c:\temp\RSP9DF.tmp" 
Creating temporary file "c:\temp\RSP9E0.tmp" with contents
[
/nologo /out:"../../../../output/PocketPC/SDL.lib" 
\GAMEDEVELOPMENT\output\PocketPC\SDL\PocketHAL.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_active.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audio.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiocvt.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiodev.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiomem.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_0.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_1.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_A.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_N.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_bmp.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_cdrom.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_cursor.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_dibaudio.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_dibevents.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_endian.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_error.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_events.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_expose.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_fatal.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gamma.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gapievents.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gapivideo.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_getenv.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_joystick.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_keyboard.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_mixer.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_mouse.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_pixels.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_quit.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_resize.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_RLEaccel.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_rwops.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_stretch.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_surface.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syscdrom.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysevents.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysjoystick.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysmouse.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysmutex.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syssem.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_systhread.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_systimer.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syswm.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_thread.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_timer.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_video.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_wave.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_wingl.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv_mmx.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv_sw.obj
]
Creating command line "link.exe -lib @c:\temp\RSP9E0.tmp"
<h3>Output Window</h3>
Compiling...
PocketHAL.cpp
SDL.c
SDL_active.c
SDL_audio.c
SDL_audiocvt.c
SDL_audiodev.c
SDL_audiomem.c
SDL_blit.c
SDL_blit_0.c
SDL_blit_1.c
SDL_blit_A.c
SDL_blit_N.c
SDL_bmp.c
SDL_cdrom.c
SDL_cursor.c
SDL_dibaudio.c
SDL_dibevents.c
SDL_endian.c
SDL_error.c
SDL_events.c
SDL_expose.c
SDL_fatal.c
SDL_gamma.c
SDL_gapievents.c
SDL_gapivideo.c
SDL_getenv.c
SDL_joystick.c
SDL_keyboard.c
SDL_mixer.c
SDL_mouse.c
SDL_pixels.c
SDL_quit.c
SDL_resize.c
SDL_RLEaccel.c
SDL_rwops.c
SDL_stretch.c
SDL_surface.c
SDL_syscdrom.c
SDL_sysevents.c
SDL_sysjoystick.c
SDL_sysmouse.c
SDL_sysmutex.c
SDL_syssem.c
SDL_systhread.c
SDL_systimer.c
SDL_syswm.c
SDL_thread.c
SDL_timer.c
SDL_video.c
SDL_wave.c
SDL_wingl.c
SDL_yuv.c
SDL_yuv_mmx.c
SDL_yuv_sw.c
Creating library...
Creating temporary file "c:\temp\RSP9E1.tmp" with contents
[
/nologo /o"../../../../output/PocketPC/staticSDL.bsc" 
\GAMEDEVELOPMENT\output\PocketPC\SDL\PocketHAL.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_active.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audio.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiocvt.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiodev.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_audiomem.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_0.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_1.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_A.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_blit_N.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_bmp.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_cdrom.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_cursor.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_dibaudio.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_dibevents.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_endian.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_error.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_events.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_expose.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_fatal.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gamma.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gapievents.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_gapivideo.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_getenv.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_joystick.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_keyboard.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_mixer.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_mouse.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_pixels.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_quit.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_resize.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_RLEaccel.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_rwops.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_stretch.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_surface.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syscdrom.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysevents.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysjoystick.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysmouse.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_sysmutex.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syssem.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_systhread.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_systimer.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_syswm.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_thread.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_timer.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_video.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_wave.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_wingl.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv_mmx.sbr
\GAMEDEVELOPMENT\output\PocketPC\SDL\SDL_yuv_sw.sbr]
Creating command line "bscmake.exe @c:\temp\RSP9E1.tmp"
Creating browse info file...
<h3>Output Window</h3>

<h3>
--------------------Configuration: mikmod - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9E2.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "..\..\mikmod" /I "../../../SDL/include" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "USE_RWOPS" /Fp"../../../../output/PocketPC/mikmod/mikmod.pch" /YX /Fo"../../../../output/PocketPC/mikmod/" /Fd"../../../../output/PocketPC/mikmod/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\drv_nos.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\drv_sdl.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\load_it.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\load_mod.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\load_s3m.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\load_xm.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mdreg.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mdriver.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mloader.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mlreg.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mlutil.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mmalloc.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mmerror.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mmio.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mplayer.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\munitrk.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\mwav.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\npertab.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\sloader.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\virtch.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\virtch2.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mikmod\virtch_common.c"
]
Creating command line "clarm.exe @c:\temp\RSP9E2.tmp" 
Creating temporary file "c:\temp\RSP9E3.tmp" with contents
[
/nologo /out:"../../../../output/PocketPC/mikmod.lib" 
\GAMEDEVELOPMENT\output\PocketPC\mikmod\drv_nos.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\drv_sdl.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\load_it.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\load_mod.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\load_s3m.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\load_xm.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mdreg.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mdriver.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mloader.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mlreg.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mlutil.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mmalloc.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mmerror.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mmio.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mplayer.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\munitrk.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\mwav.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\npertab.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\sloader.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\virtch.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\virtch2.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod\virtch_common.obj
]
Creating command line "link.exe -lib @c:\temp\RSP9E3.tmp"
<h3>Output Window</h3>
Compiling...
drv_nos.c
drv_sdl.c
load_it.c
load_mod.c
load_s3m.c
load_xm.c
mdreg.c
mdriver.c
mloader.c
mlreg.c
mlutil.c
mmalloc.c
mmerror.c
mmio.c
mplayer.c
munitrk.c
mwav.c
npertab.c
sloader.c
virtch.c
virtch2.c
virtch_common.c
Creating library...

<h3>
--------------------Configuration: native_midi - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9E4.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../../../SDL/include/" /I "..\.." /I "..\..\native_midi" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../../../output/PocketPC/native_midi/native_midi.pch" /YX /Fo"../../../../output/PocketPC/native_midi/" /Fd"../../../../output/PocketPC/native_midi/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_common.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_win32.c"
]
Creating command line "clarm.exe @c:\temp\RSP9E4.tmp" 
Creating command line "link.exe -lib /nologo /out:"../../../../output/PocketPC/native_midi.lib"  \GAMEDEVELOPMENT\output\PocketPC\native_midi\native_midi_common.obj \GAMEDEVELOPMENT\output\PocketPC\native_midi\native_midi_win32.obj "
<h3>Output Window</h3>
Compiling...
native_midi_common.c
native_midi_win32.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_win32.c(397) : warning C4244: 'initializing' : conversion from 'double ' to 'int ', possible loss of data
Creating library...

<h3>
--------------------Configuration: timidity - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9E5.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../../../SDL/include/" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../../../output/PocketPC/timidity/timidity.pch" /YX /Fo"../../../../output/PocketPC/timidity/" /Fd"../../../../output/PocketPC/timidity/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\common.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\controls.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\filter.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\instrum.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\mix.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\output.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\playmidi.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\readmidi.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\sdl_a.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\sdl_c.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\tables.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\timidity.c"
]
Creating command line "clarm.exe @c:\temp\RSP9E5.tmp" 
Creating temporary file "c:\temp\RSP9E6.tmp" with contents
[
/nologo /out:"../../../../output/PocketPC/timidity.lib" 
\GAMEDEVELOPMENT\output\PocketPC\timidity\common.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\controls.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\filter.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\instrum.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\mix.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\output.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\playmidi.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\readmidi.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\resample.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\sdl_a.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\sdl_c.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\tables.obj
\GAMEDEVELOPMENT\output\PocketPC\timidity\timidity.obj
]
Creating command line "link.exe -lib @c:\temp\RSP9E6.tmp"
<h3>Output Window</h3>
Compiling...
common.c
controls.c
filter.c
instrum.c
mix.c
output.c
playmidi.c
readmidi.c
resample.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(100) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(165) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(216) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(235) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(396) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(454) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(522) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(549) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(727) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
sdl_a.c
sdl_c.c
tables.c
timidity.c
Creating library...

<h3>
--------------------Configuration: staticSDL_mixer - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9E7.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../../SDL/include/" /I "..\SDL_mad" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "tremor" /D "OGG_MUSIC" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "USE_RWOPS" /D "FPM_DEFAULT" /D "OPT_SPEED" /D "ASO_INTERLEAVE1" /D inline=__inline /U "USE_NATIVE_MIDI" /U "USE_TIMIDITY_MIDI" /U "MID_MUSIC" /Fp"../../../output/PocketPC/SDL_mixer/staticSDL_mixer.pch" /YX /Fo"../../../output/PocketPC/SDL_mixer/" /Fd"../../../output/PocketPC/SDL_mixer/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\bitwise.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\block.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\codebook.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\floor0.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\floor1.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\framing.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\info.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\mapping0.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\mdct.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\registry.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\res012.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\sharedbook.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\synthesis.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\window.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\effect_position.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\effect_stereoreverse.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\effects_internal.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\load_aiff.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\load_ogg.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\load_voc.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\mixer.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\music.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\music_cmd.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\music_ogg.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\wavestream.c"
]
Creating command line "clarm.exe @c:\temp\RSP9E7.tmp" 
Creating temporary file "c:\temp\RSP9E8.tmp" with contents
[
/nologo /out:"../../../output/PocketPC/SDL_mixer.lib" 
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\bitwise.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\block.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\codebook.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\floor0.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\floor1.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\framing.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\info.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\mapping0.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\mdct.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\registry.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\res012.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\sharedbook.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\synthesis.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\vorbisfile.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\window.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\effect_position.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\effect_stereoreverse.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\effects_internal.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\load_aiff.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\load_ogg.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\load_voc.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\mixer.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\music.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\music_cmd.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\music_ogg.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer\wavestream.obj
\GAMEDEVELOPMENT\output\PocketPC\mikmod.lib
\GAMEDEVELOPMENT\output\PocketPC\native_midi.lib
\GAMEDEVELOPMENT\output\PocketPC\timidity.lib
]
Creating command line "link.exe -lib @c:\temp\RSP9E8.tmp"
<h3>Output Window</h3>
Compiling...
bitwise.c
block.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\block.c(404) : warning C4244: '-=' : conversion from '__int64 ' to 'int ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\block.c(407) : warning C4244: '+=' : conversion from '__int64 ' to 'int ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\block.c(420) : warning C4244: 'initializing' : conversion from '__int64 ' to 'long ', possible loss of data
codebook.c
floor0.c
floor1.c
framing.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\framing.c(382) : warning C4244: '=' : conversion from 'unsigned __int32 ' to 'unsigned char ', possible loss of data
info.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\info.c(36) : warning C4244: '=' : conversion from 'long ' to 'char ', possible loss of data
mapping0.c
mdct.c
registry.c
res012.c
sharedbook.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\sharedbook.c(399) : warning C4244: '=' : conversion from 'long ' to 'char ', possible loss of data
synthesis.c
vorbisfile.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(447) : warning C4244: 'return' : conversion from '__int64 ' to 'int ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(584) : warning C4244: '=' : conversion from '__int64 ' to 'int ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(671) : warning C4244: 'function' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(846) : warning C4244: 'return' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(850) : warning C4244: 'return' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(878) : warning C4244: '=' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(1373) : warning C4244: '=' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(1589) : warning C4244: '=' : conversion from '__int32 ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\tremor\vorbisfile.c(671) : warning C4761: integral size mismatch in argument; conversion supplied
window.c
effect_position.c
effect_stereoreverse.c
effects_internal.c
load_aiff.c
load_ogg.c
load_voc.c
mixer.c
music.c
music_cmd.c
music_ogg.c
wavestream.c
Creating library...

<h3>
--------------------Configuration: ENGINE - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9E9.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob2 /I "../../OpenGL ES/include/" /I "../../PocketHAL/" /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /D "GAME_EDITOR_PROFESSIONAL" /D "STAND_ALONE_GAME" /D "KYRA_SUPPORT_OPENGL" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /FAs /Fa"../../output/PocketPC/engine/" /Fp"../../output/PocketPC/engine/ENGINE.pch" /YX /Fo"../../output/PocketPC/engine/" /Fd"../../output/PocketPC/engine/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\kyra\engine\action.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\box.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\boxresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\canvas.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\canvasresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\color.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\dataresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\dirtyrectangle.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\encoder.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\engine.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\fontresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\image.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\imagecontainer.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\imagetree.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\imnode.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\krmath.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\kyraresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\namefield.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\ogltexture.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\painter.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\parser.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\pixelblock.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\rle.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\sdlutil.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\splash.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\sprite.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\spriteresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\textbox.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\tile.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\tileresource.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\vault.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\button.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\console.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\eventmanager.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\listbox.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\textwidget.cpp"
"E:\GAMEDEVELOPMENT\kyra\gui\widget.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glbitstream.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\gldebug.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glgraph.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glintarrayset.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glisomath.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glmap.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glmemorypool.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glperformance.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glprime.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glrandom.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glstring.cpp"
"E:\GAMEDEVELOPMENT\kyra\util\glutil.cpp"
"E:\GAMEDEVELOPMENT\kyra\tinyxml\tinyxml.cpp"
"E:\GAMEDEVELOPMENT\kyra\tinyxml\tinyxmlerror.cpp"
"E:\GAMEDEVELOPMENT\kyra\tinyxml\tinyxmlparser.cpp"
"E:\GAMEDEVELOPMENT\kyra\guiExtended\KrImageListBox.cpp"
"E:\GAMEDEVELOPMENT\kyra\guiExtended\progress.cpp"
"E:\GAMEDEVELOPMENT\kyra\engine\FastSprite.cpp"
]
Creating command line "clarm.exe @c:\temp\RSP9E9.tmp" 
Creating temporary file "c:\temp\RSP9EA.tmp" with contents
[
/nologo /out:"../../output/PocketPC/ENGINE.lib" 
\GAMEDEVELOPMENT\output\PocketPC\engine\action.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\box.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\boxresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\canvas.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\canvasresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\color.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\dataresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\dirtyrectangle.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\encoder.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\engine.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\fontresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\image.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\imagecontainer.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\imagetree.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\imnode.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\krmath.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\kyraresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\namefield.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\ogltexture.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\painter.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\parser.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\pixelblock.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\rle.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\sdlutil.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\splash.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\sprite.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\spriteresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\textbox.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\tile.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\tileresource.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\vault.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\button.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\console.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\eventmanager.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\listbox.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\textwidget.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\widget.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glbitstream.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\gldebug.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glgraph.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glintarrayset.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glisomath.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glmap.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glmemorypool.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glperformance.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glprime.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glrandom.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glstring.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\glutil.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\tinyxml.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\tinyxmlerror.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\tinyxmlparser.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\KrImageListBox.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\progress.obj
\GAMEDEVELOPMENT\output\PocketPC\engine\FastSprite.obj
\GAMEDEVELOPMENT\output\PocketPC\SDL.lib
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer.lib
]
Creating command line "link.exe -lib @c:\temp\RSP9EA.tmp"
<h3>Output Window</h3>
Compiling...
action.cpp
box.cpp
boxresource.cpp
canvas.cpp
canvasresource.cpp
color.cpp
dataresource.cpp
dirtyrectangle.cpp
encoder.cpp
engine.cpp
fontresource.cpp
E:\GAMEDEVELOPMENT\kyra\engine\fontresource.cpp(415) : warning C4018: '==' : signed/unsigned mismatch
image.cpp
imagecontainer.cpp
imagetree.cpp
imnode.cpp
krmath.cpp
kyraresource.cpp
namefield.cpp
ogltexture.cpp
painter.cpp
parser.cpp
pixelblock.cpp
rle.cpp
sdlutil.cpp
splash.cpp
sprite.cpp
E:\GAMEDEVELOPMENT\kyra\engine\sprite.cpp(392) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\kyra\engine\sprite.cpp(393) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
spriteresource.cpp
textbox.cpp
tile.cpp
tileresource.cpp
vault.cpp
button.cpp
console.cpp
eventmanager.cpp
listbox.cpp
textwidget.cpp
E:\GAMEDEVELOPMENT\kyra\gui\textwidget.cpp(1012) : warning C4244: '=' : conversion from 'float' to 'int', possible loss of data
widget.cpp
E:\GAMEDEVELOPMENT\kyra\gui\widget.cpp(234) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\kyra\gui\widget.cpp(254) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\kyra\gui\widget.cpp(270) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\kyra\gui\widget.cpp(284) : warning C4018: '<' : signed/unsigned mismatch
glbitstream.cpp
gldebug.cpp
glgraph.cpp
glintarrayset.cpp
glisomath.cpp
glmap.cpp
glmemorypool.cpp
glperformance.cpp
glprime.cpp
glrandom.cpp
glstring.cpp
E:\GAMEDEVELOPMENT\kyra\util\glstring.cpp(91) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\kyra\util\glstring.cpp(199) : warning C4018: '!=' : signed/unsigned mismatch
glutil.cpp
tinyxml.cpp
tinyxmlerror.cpp
tinyxmlparser.cpp
KrImageListBox.cpp
progress.cpp
FastSprite.cpp
Creating library...

<h3>
--------------------Configuration: RakNet - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9EB.tmp" with contents
[
/nologo /W3 /I "./../../Include" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "LITTLE_ENDIAN" /Fp"../../../output/PocketPC/RakNet/RakNet.pch" /YX /Fo"../../../output/PocketPC/RakNet/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\RAKNET\Source\RakClient.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\RakNetStatistics.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\RakNetworkFactory.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\RakPeer.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\RakServer.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\Rand.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\ReliabilityLayer.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\RPCNode.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\SHA1.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\SimpleMutex.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\SocketLayer.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\StringCompressor.cpp"
]
Creating command line "clarm.exe @c:\temp\RSP9EB.tmp" 
Creating temporary file "c:\temp\RSP9EC.tmp" with contents
[
/nologo /out:"../../../output/PocketPC/RakNet.lib" 
\GAMEDEVELOPMENT\output\PocketPC\RakNet\AES128.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\AsynchronousFileIO.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\BitStream.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\CheckSum.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\DataBlockEncryptor.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\DistributedNetworkObject.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\DistributedNetworkObjectManager.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\DistributedNetworkObjectStub.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\EncodeClassName.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\ExtendedOverlappedPool.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\GetTime.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\HuffmanEncodingTree.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\HuffmanEncodingTreeFactory.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\InternalPacketPool.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\NetworkObject.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\NetworkTypes.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\PacketPool.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RakClient.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RakNetStatistics.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RakNetworkFactory.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RakPeer.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RakServer.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\Rand.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\ReliabilityLayer.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\RPCNode.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\SHA1.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\SimpleMutex.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\SocketLayer.obj
\GAMEDEVELOPMENT\output\PocketPC\RakNet\StringCompressor.obj
]
Creating command line "link.exe -lib @c:\temp\RSP9EC.tmp"
<h3>Output Window</h3>
Compiling...
RakClient.cpp
RakNetStatistics.cpp
RakNetworkFactory.cpp
RakPeer.cpp
RakServer.cpp
Rand.cpp
ReliabilityLayer.cpp
RPCNode.cpp
SHA1.cpp
SimpleMutex.cpp
SocketLayer.cpp
StringCompressor.cpp
Creating library...

<h3>
--------------------Configuration: gameEngine - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP9ED.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob2 /I "../OpenGL ES/include/" /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "EiC/src" /D "GAME_EDITOR_PROFESSIONAL" /D "STAND_ALONE_GAME" /D "NDEBUG" /D "ARM" /D "_ARM_" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "WIN32" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "LOAD_BMP" /D "LOAD_GIF" /FR"../output/PocketPC/gameEngine/" /Fp"../output/PocketPC/gameEngine/gameEngine.pch" /YX /Fo"../output/PocketPC/gameEngine/" /Fd"../output/PocketPC/gameEngine/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\gameEngine\Action.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ActivationEventsCanvas.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ActorEdit.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ActorTip.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Axis.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\bit_byte.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\bit_bytx.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Button.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\libce\src\ce_time.c"
"E:\GAMEDEVELOPMENT\gameEngine\compression.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\dlmalloc.c"
"E:\GAMEDEVELOPMENT\gameEngine\EditBox.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\EditorDirectory.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Envelope.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\GameMain.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\gedString.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\GenericScript.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ListBox.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ListDirectory.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ListPop.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\ListPopSelect.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\loadGifMod.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\maksKyra.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\minilzo.c"
"E:\GAMEDEVELOPMENT\gameEngine\MuteEx.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Panel.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\PanelInfo.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\PanelQuestion.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Path.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\PathPoint.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\RegionLoad.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Script.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Sequence.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Slide.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\system.c"
"E:\GAMEDEVELOPMENT\gameEngine\Text.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\Tile.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\WaitCursor.cpp"
"E:\GAMEDEVELOPMENT\SDL\SDL\src\main\win32\SDL_main.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\alloc.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2b_99.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2b_d.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2b_ds.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2d_99.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2d_d.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\n2d_ds.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\ucl_init.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\ucl_ptr.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\ucl_str.c"
"E:\GAMEDEVELOPMENT\gameEngine\Compact\ucl_util.c"
"E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\BoxLineIntersection2D.cpp"
"E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp"
]
Creating command line "clarm.exe @c:\temp\RSP9ED.tmp" 
Creating temporary file "c:\temp\RSP9EE.tmp" with contents
[
/nologo /out:"../output/PocketPC/gameEngine.lib" 
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Action.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActivationEventsCanvas.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Actor.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActorEdit.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActorTip.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Axis.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\bit_byte.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\bit_bytx.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Button.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ce_time.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\compression.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\dlmalloc.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\EditBox.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\EditorDirectory.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Envelope.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GameControl.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GameMain.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\gedString.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GenericScript.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListBox.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListDirectory.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListPop.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListPopSelect.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\loadGifMod.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\maksKyra.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\minilzo.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\MuteEx.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Panel.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PanelInfo.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PanelQuestion.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Path.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PathPoint.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\RegionLoad.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Script.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Sequence.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Slide.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\SlowCanvas.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\system.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Text.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Tile.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\WaitCursor.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\SDL_main.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\alloc.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_99.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_d.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_ds.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_99.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_d.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_ds.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_init.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_ptr.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_str.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_util.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\BoxLineIntersection2D.obj
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GeoPathfinder.obj
\GAMEDEVELOPMENT\output\PocketPC\eiclib.lib
\GAMEDEVELOPMENT\output\PocketPC\ENGINE.lib
\GAMEDEVELOPMENT\output\PocketPC\RakNet.lib
]
Creating command line "link.exe -lib @c:\temp\RSP9EE.tmp"
<h3>Output Window</h3>
Compiling...
Action.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
ActivationEventsCanvas.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
Actor.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(20) : warning C4005: 'ASSERT' : macro redefinition
        E:\Windows CE Toolsramas\wce300\Pocket PC 2002\include\dbgapi.h(170) : see previous definition of 'ASSERT'
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(168) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2380) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2416) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2447) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2472) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2497) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2683) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2683) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3121) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3121) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3121) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3473) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3473) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3473) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3473) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3486) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3486) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3486) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3486) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4285) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4285) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4285) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4290) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4600) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4600) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4601) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4601) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4619) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4619) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4620) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4620) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4851) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4851) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4863) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4864) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5010) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5010) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5126) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5211) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5273) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
ActorEdit.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
ActorTip.cpp
Axis.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Axis.cpp(140) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Axis.cpp(140) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Axis.cpp(147) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Axis.cpp(147) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
bit_byte.cpp
bit_bytx.cpp
Button.cpp
ce_time.c
compression.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
dlmalloc.c
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc.c(4) : warning C4005: 'WIN32' : macro redefinition
        unknown(0) : see previous definition of 'WIN32'
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(3666) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(3666) : warning C4307: '*' : integral constant overflow
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4295) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4295) : warning C4307: '*' : integral constant overflow
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4512) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4512) : warning C4307: '*' : integral constant overflow
EditBox.cpp
EditorDirectory.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
Envelope.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
GameControl.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(20) : warning C4005: 'ASSERT' : macro redefinition
        E:\Windows CE Toolsramas\wce300\Pocket PC 2002\include\dbgapi.h(170) : see previous definition of 'ASSERT'
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1328) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1328) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1329) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1329) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1331) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1331) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1350) : warning C4244: '+=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1354) : warning C4244: '/=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1368) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1368) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1452) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1452) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(6774) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
GameMain.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(20) : warning C4005: 'ASSERT' : macro redefinition
        E:\Windows CE Toolsramas\wce300\Pocket PC 2002\include\dbgapi.h(170) : see previous definition of 'ASSERT'
gedString.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\gedString.cpp(1319) : warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\gedString.cpp(1323) : warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
GenericScript.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
EiC/src\eic.h(69) : warning C4005: 'malloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(711) : see previous definition of 'malloc'
EiC/src\eic.h(70) : warning C4005: 'calloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(713) : see previous definition of 'calloc'
EiC/src\eic.h(71) : warning C4005: 'free' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(714) : see previous definition of 'free'
EiC/src\eic.h(72) : warning C4005: 'realloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(712) : see previous definition of 'realloc'
ListBox.cpp
ListDirectory.cpp
ListPop.cpp
ListPopSelect.cpp
loadGifMod.cpp
maksKyra.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
EiC/src\eic.h(69) : warning C4005: 'malloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(711) : see previous definition of 'malloc'
EiC/src\eic.h(70) : warning C4005: 'calloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(713) : see previous definition of 'calloc'
EiC/src\eic.h(71) : warning C4005: 'free' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(714) : see previous definition of 'free'
EiC/src\eic.h(72) : warning C4005: 'realloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(712) : see previous definition of 'realloc'
E:\GAMEDEVELOPMENT\gameEngine\maksKyra.cpp(1058) : warning C4018: '>' : signed/unsigned mismatch
minilzo.c
MuteEx.cpp
Panel.cpp
PanelInfo.cpp
PanelQuestion.cpp
Path.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
PathPoint.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
RegionLoad.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\RegionLoad.cpp(252) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\RegionLoad.cpp(260) : warning C4018: '!=' : signed/unsigned mismatch
Script.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
EiC/src\eic.h(69) : warning C4005: 'malloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(711) : see previous definition of 'malloc'
EiC/src\eic.h(70) : warning C4005: 'calloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(713) : see previous definition of 'calloc'
EiC/src\eic.h(71) : warning C4005: 'free' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(714) : see previous definition of 'free'
EiC/src\eic.h(72) : warning C4005: 'realloc' : macro redefinition
        E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.h(712) : see previous definition of 'realloc'
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1648) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1648) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1671) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(3505) : warning C4018: '<' : signed/unsigned mismatch
Sequence.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Sequence.cpp(37) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Sequence.cpp(58) : warning C4244: '=' : conversion from 'long' to 'unsigned short', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Sequence.cpp(180) : warning C4018: '<' : signed/unsigned mismatch
Slide.cpp
SlowCanvas.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(545) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(546) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(547) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
system.c
Text.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
Tile.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Tile.cpp(159) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Tile.cpp(292) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Tile.cpp(303) : warning C4018: '<' : signed/unsigned mismatch
WaitCursor.cpp
SDL_main.c
alloc.c
n2b_99.c
n2b_d.c
n2b_ds.c
n2d_99.c
n2d_d.c
n2d_ds.c
ucl_init.c
ucl_ptr.c
ucl_str.c
ucl_util.c
BoxLineIntersection2D.cpp
GeoPathfinder.cpp
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\../fib/fib.c(57) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(974) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(975) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(976) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(977) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1087) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1088) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1089) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1090) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(65) : warning C4244: '/=' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(65) : while compiling class-template member function 'void __cdecl Vector2D<float>::operator /=(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(65) : warning C4244: '/=' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(65) : while compiling class-template member function 'void __cdecl Vector2D<float>::operator /=(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(112) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(111) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator *(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(112) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(111) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator *(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(121) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(120) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator /(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(121) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/vector2d.h(120) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator /(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(58) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(55) : while compiling class-template member function 'bool __cdecl Box2D<float>::intersects(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(59) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(55) : while compiling class-template member function 'bool __cdecl Box2D<float>::intersects(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(69) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(69) : warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(70) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(70) : warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(171) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(172) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(176) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(177) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(181) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(182) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath/Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(789) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(842) : see reference to function template instantiation 'bool __cdecl getBoxLineIntersection(class GeoPathFinder *,class Box *,class Line2D<float>,class Vector2D<float> &,int &)' being compiled
Creating library...
Creating temporary file "c:\temp\RSP9EF.tmp" with contents
[
/nologo /o"../output/PocketPC/gameEngine.bsc" 
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Action.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActivationEventsCanvas.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Actor.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActorEdit.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ActorTip.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Axis.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\bit_byte.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\bit_bytx.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Button.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ce_time.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\compression.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\dlmalloc.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\EditBox.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\EditorDirectory.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Envelope.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GameControl.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GameMain.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\gedString.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GenericScript.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListBox.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListDirectory.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListPop.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ListPopSelect.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\loadGifMod.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\maksKyra.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\minilzo.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\MuteEx.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Panel.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PanelInfo.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PanelQuestion.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Path.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\PathPoint.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\RegionLoad.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Script.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Sequence.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Slide.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\SlowCanvas.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\system.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Text.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\Tile.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\WaitCursor.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\SDL_main.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\alloc.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_99.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_d.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2b_ds.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_99.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_d.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\n2d_ds.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_init.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_ptr.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_str.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\ucl_util.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\BoxLineIntersection2D.sbr
\GAMEDEVELOPMENT\output\PocketPC\gameEngine\GeoPathfinder.sbr]
Creating command line "bscmake.exe @c:\temp\RSP9EF.tmp"
Creating browse info file...
<h3>Output Window</h3>

<h3>
--------------------Configuration: gameEditor - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"../output/PocketPC/editor/pocketpc.res" /d UNDER_CE=300 /d _WIN32_WCE=300 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WIN32_PLATFORM_PSPC=310" /d "ARM" /d "_ARM_" /r "E:\GAMEDEVELOPMENT\gameEditor\pocketpc.rc"" 
Creating temporary file "c:\temp\RSP9F0.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../SDL/SDL_mixer" /I "../gameEngine" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "../gameEngine/Eic/src" /D "GAME_EDITOR_PROFESSIONAL" /D "STAND_ALONE_GAME" /D "ARM" /D "_ARM_" /D "NDEBUG" /D UNDER_CE=300 /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "UNICODE" /D "_UNICODE" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /FR"../output/PocketPC/editor/" /Fp"../output/PocketPC/editor/gameEditor.pch" /YX /Fo"../output/PocketPC/editor/" /Fd"../output/PocketPC/editor/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\gameEditor\AboutDlg.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ActorProperty.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ActorText.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\AddActionDialog.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\AddActor.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\AddPathDialog.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\AddSequenceDialog.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\BaseAction.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ChangeAnimation.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ChangeAnimationDirection.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ChangeCursor.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ChangeParent.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ChangePath.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ConditionalAction.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\CreateActor.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\CreateTimer.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\CreateVar.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DestroyActor.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DestroyTimer.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgActivationEvent.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgAnimationFinish.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgCollision.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgGeneralEvent.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgMouseButton.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\DlgTimer.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\EditSequenceDialog.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\EnableDisable.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\EventControl.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\EventKeyDown.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ExpressionEditor.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\GameSettings.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\GridSettings.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\LoadSave.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\LoadSaveGame.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\LoadSaveScript.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\main.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\MainPanel.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\MoveTo.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\NewTimer.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\PathDialog.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\PhysicalBounce.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\PlaySong.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ScriptGlobals.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\SetTile.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\SetTileSingle.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\SetZDepth.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ToAnteriorPosition.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\ToMousePosition.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\Transparency.cpp"
"E:\GAMEDEVELOPMENT\gameEditor\WaitForFrame.cpp"
]
Creating command line "clarm.exe @c:\temp\RSP9F0.tmp" 
Creating temporary file "c:\temp\RSP9F1.tmp" with contents
[
commctrl.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"../output/gameEditorPocket.pdb" /map:"../output/PocketPC/editor/gameEditorPocket.map" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib" /out:"../output/gameEditorPocket.exe" /subsystem:windowsce,3.00 /align:"4096" /MACHINE:ARM 
\GAMEDEVELOPMENT\output\PocketPC\editor\AboutDlg.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ActorProperty.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ActorText.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\AddActionDialog.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\AddActor.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\AddPathDialog.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\AddSequenceDialog.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\BaseAction.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeAnimation.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeAnimationDirection.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeCursor.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeParent.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangePath.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ConditionalAction.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateActor.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateTimer.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateVar.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DestroyActor.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DestroyTimer.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgActivationEvent.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgAnimationFinish.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgCollision.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgGeneralEvent.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgMouseButton.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgTimer.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\EditSequenceDialog.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\EnableDisable.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\EventControl.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\EventKeyDown.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ExpressionEditor.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\GameSettings.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\GridSettings.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSave.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSaveGame.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSaveScript.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\main.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\MainPanel.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\MoveTo.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\NewTimer.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\PathDialog.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\PhysicalBounce.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\PlaySong.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ScriptGlobals.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\SetTile.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\SetTileSingle.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\SetZDepth.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ToAnteriorPosition.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\ToMousePosition.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\Transparency.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\WaitForFrame.obj
\GAMEDEVELOPMENT\output\PocketPC\editor\pocketpc.res
\GAMEDEVELOPMENT\output\PocketPC\gameEngine.lib
]
Creating command line "link.exe @c:\temp\RSP9F1.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
AboutDlg.cpp
ActorProperty.cpp
ActorText.cpp
AddActionDialog.cpp
AddActor.cpp
AddPathDialog.cpp
AddSequenceDialog.cpp
BaseAction.cpp
ChangeAnimation.cpp
ChangeAnimationDirection.cpp
ChangeCursor.cpp
ChangeParent.cpp
ChangePath.cpp
ConditionalAction.cpp
CreateActor.cpp
CreateTimer.cpp
CreateVar.cpp
DestroyActor.cpp
DestroyTimer.cpp
DlgActivationEvent.cpp
DlgAnimationFinish.cpp
DlgCollision.cpp
DlgGeneralEvent.cpp
DlgMouseButton.cpp
DlgTimer.cpp
EditSequenceDialog.cpp
EnableDisable.cpp
EventControl.cpp
EventKeyDown.cpp
ExpressionEditor.cpp
GameSettings.cpp
GridSettings.cpp
LoadSave.cpp
LoadSaveGame.cpp
LoadSaveScript.cpp
main.cpp
MainPanel.cpp
../gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
../gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
MoveTo.cpp
NewTimer.cpp
PathDialog.cpp
PhysicalBounce.cpp
PlaySong.cpp
ScriptGlobals.cpp
SetTile.cpp
SetTileSingle.cpp
SetZDepth.cpp
ToAnteriorPosition.cpp
ToMousePosition.cpp
Transparency.cpp
WaitForFrame.cpp
Linking...
Creating temporary file "c:\temp\RSP9F4.tmp" with contents
[
/nologo /o"../output/gameEditor.bsc" 
\GAMEDEVELOPMENT\output\PocketPC\editor\AboutDlg.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ActorProperty.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ActorText.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\AddActionDialog.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\AddActor.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\AddPathDialog.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\AddSequenceDialog.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\BaseAction.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeAnimation.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeAnimationDirection.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeCursor.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangeParent.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ChangePath.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ConditionalAction.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateActor.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateTimer.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\CreateVar.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DestroyActor.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DestroyTimer.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgActivationEvent.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgAnimationFinish.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgCollision.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgGeneralEvent.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgMouseButton.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\DlgTimer.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\EditSequenceDialog.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\EnableDisable.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\EventControl.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\EventKeyDown.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ExpressionEditor.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\GameSettings.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\GridSettings.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSave.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSaveGame.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\LoadSaveScript.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\main.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\MainPanel.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\MoveTo.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\NewTimer.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\PathDialog.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\PhysicalBounce.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\PlaySong.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ScriptGlobals.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\SetTile.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\SetTileSingle.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\SetZDepth.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ToAnteriorPosition.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\ToMousePosition.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\Transparency.sbr
\GAMEDEVELOPMENT\output\PocketPC\editor\WaitForFrame.sbr]
Creating command line "bscmake.exe @c:\temp\RSP9F4.tmp"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
gameEditorPocket.exe - 0 error(s), 204 warning(s)
</pre>
</body>
</html>
