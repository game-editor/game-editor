<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: timidity - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP356.tmp" with contents
[
/nologo /W3 /Zi /Od /I "../../../SDL/include/" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../../../output/timidity/timidity.pch" /YX /Fo"../../../../output/timidity/" /Fd"../../../../output/timidity/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP356.tmp" 
Creating temporary file "c:\temp\RSP357.tmp" with contents
[
/nologo /out:"../../../../output/timidity.lib" 
\GAMEDEVELOPMENT\output\timidity\common.obj
\GAMEDEVELOPMENT\output\timidity\controls.obj
\GAMEDEVELOPMENT\output\timidity\filter.obj
\GAMEDEVELOPMENT\output\timidity\instrum.obj
\GAMEDEVELOPMENT\output\timidity\mix.obj
\GAMEDEVELOPMENT\output\timidity\output.obj
\GAMEDEVELOPMENT\output\timidity\playmidi.obj
\GAMEDEVELOPMENT\output\timidity\readmidi.obj
\GAMEDEVELOPMENT\output\timidity\resample.obj
\GAMEDEVELOPMENT\output\timidity\sdl_a.obj
\GAMEDEVELOPMENT\output\timidity\sdl_c.obj
\GAMEDEVELOPMENT\output\timidity\tables.obj
\GAMEDEVELOPMENT\output\timidity\timidity.obj
]
Creating command line "link.exe -lib @c:\temp\RSP357.tmp"
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
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(98) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(163) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(214) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(233) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(394) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(452) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(520) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(547) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\timidity\resample.c(725) : warning C4244: '=' : conversion from 'unsigned long ' to 'short ', possible loss of data
sdl_a.c
sdl_c.c
tables.c
timidity.c
Creating library...




<h3>Results</h3>
timidity.lib - 0 error(s), 9 warning(s)
</pre>
</body>
</html>
