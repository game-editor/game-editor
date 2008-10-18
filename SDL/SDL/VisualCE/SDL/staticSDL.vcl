<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: staticSDL - Win32 (WCE ARM) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP551.tmp" with contents
[
/nologo /W3 /Zi /O2 /Ob0 /I "../../../../OpenGL ES/include/" /I "../../../../PocketHAL/" /I "..\..\src" /I "..\..\src\audio" /I "..\..\src\video" /I "..\..\src\video\wincommon" /I "..\..\src\video\windx5" /I "..\..\src\events" /I "..\..\src\joystick" /I "..\..\src\cdrom" /I "..\..\src\thread" /I "..\..\src\thread\win32" /I "..\..\src\timer" /I "..\..\include" /I "..\..\include\SDL" /D "HAVE_OPENGL" /D WIN32_PLATFORM_PSPC=310 /D "NDEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "NO_SIGNAL_H" /D "ENABLE_GAPI" /D "_LIB" /U "ENABLE_WINDIB" /FR"../../../../output/PocketPC/SDL/" /Fp"../../../../output/PocketPC/SDL/staticSDL.pch" /YX /Fo"../../../../output/PocketPC/SDL/" /Fd"../../../../output/PocketPC/SDL/" /Oxs /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL\src\video\wincommon\SDL_syswm.c"
]
Creating command line "clarm.exe @c:\temp\RSP551.tmp" 
Creating temporary file "c:\temp\RSP552.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP552.tmp"
<h3>Output Window</h3>
Compiling...
SDL_syswm.c
Creating library...
Creating temporary file "c:\temp\RSP553.tmp" with contents
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
Creating command line "bscmake.exe @c:\temp\RSP553.tmp"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
SDL.lib - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
