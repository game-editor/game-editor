<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: eiclib - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP60C.tmp" with contents
[
/nologo /Zp4 /W3 /Zi /Od /I "../src" /D "DEBUG" /D "ARM" /D "_ARM_" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "NO_ENV" /D "NO_ASSERT" /D "NO_TMPFILE" /D "NO_TIME" /D "NO_HTML" /Fp"../../../output/PocketPC/eic/eiclib.pch" /YX /Fo"../../../output/PocketPC/eic/" /Fd"../../../output/PocketPC/eic/" /MC /c 
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\assert.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\callback.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\dirent.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\eicfftoa.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\eicmod.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\emitter.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\errno.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\error.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\fcntl.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\itoa.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\math.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\parser.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preexpr.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\printf.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\readline.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\reset.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\signal.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\stab.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\starteic.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stat.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdarg.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdClib.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdio.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdlib.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\string.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\strrev.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\strround.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\symbol.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\time.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typemod.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\unistd.c"
"E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c"
]
Creating command line "clarm.exe @c:\temp\RSP60C.tmp" 
Creating temporary file "c:\temp\RSP60D.tmp" with contents
[
/nologo /out:"../../../output/PocketPC/eiclib.lib" 
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
Creating command line "link.exe -lib @c:\temp\RSP60D.tmp"
<h3>Output Window</h3>
Compiling...
assert.c
callback.c
cdecl.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(1397) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(269) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(297) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(350) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(231) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\cdecl.c(243) : warning C4761: integral size mismatch in argument; conversion supplied
dirent.c
eicfftoa.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\eicfftoa.c(63) : warning C4244: '=' : conversion from 'double ' to 'int ', possible loss of data
eicmod.c
emitter.c
errno.c
error.c
fcntl.c
func.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(355) : warning C4550: expression evaluates to a function which is missing an argument list
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(300) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(303) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(305) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(307) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(309) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(314) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(316) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(321) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(323) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(327) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(329) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(333) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(334) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\func.c(338) : warning C4761: integral size mismatch in argument; conversion supplied
interpre.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(466) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(484) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(510) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(592) : warning C4146: unary minus operator applied to unsigned type, result still unsigned
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(660) : warning C4146: unary minus operator applied to unsigned type, result still unsigned
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(838) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(853) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(854) : warning C4047: '=' : 'int ' differs in levels of indirection from 'struct InsT_t *'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(928) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(970) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1005) : warning C4047: '=' : 'struct InsT_t *' differs in levels of indirection from 'int '
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1031) : warning C4047: '=' : 'struct InsT_t *' differs in levels of indirection from 'int '
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1056) : warning C4047: '=' : 'int ' differs in levels of indirection from 'struct InsT_t *'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1088) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1097) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1315) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1492) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1551) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1567) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1595) : warning C4244: '=' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1614) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\interpre.c(1629) : warning C4018: '>=' : signed/unsigned mismatch
itoa.c
lexer.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(247) : warning C4244: 'initializing' : conversion from 'double ' to 'float ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(257) : warning C4244: '=' : conversion from '__int64 ' to 'double ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(269) : warning C4244: '=' : conversion from '__int64 ' to 'unsigned long ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(301) : warning C4244: '=' : conversion from '__int64 ' to 'unsigned long ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(305) : warning C4244: '=' : conversion from '__int64 ' to 'long ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(329) : warning C4244: '=' : conversion from '__int64 ' to 'unsigned long ', possible loss of data
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(205) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(209) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(224) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(359) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(360) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(428) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(441) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\lexer.c(496) : warning C4761: integral size mismatch in argument; conversion supplied
math.c
optomizer.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c(54) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c(92) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c(156) : warning C4018: '<=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c(159) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\optomizer.c(161) : warning C4018: '<' : signed/unsigned mismatch
parser.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\parser.c(136) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\parser.c(900) : warning C4018: '<' : signed/unsigned mismatch
preexpr.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preexpr.c(432) : warning C4146: unary minus operator applied to unsigned type, result still unsigned
preproc.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(231) : warning C4013: 'close' undefined; assuming extern returning int
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(398) : warning C4018: '>' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(540) : warning C4013: 'read' undefined; assuming extern returning int
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(617) : warning C4090: 'return' : different 'const' qualifiers
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(826) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(1040) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(1891) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(1907) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(1914) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(1928) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(694) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(728) : warning C4761: integral size mismatch in argument; conversion supplied
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\preproc.c(759) : warning C4761: integral size mismatch in argument; conversion supplied
printf.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\printf.c(35) : warning C4029: declared formal parameter list different from definition
readline.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\readline.c(641) : warning C4018: '>' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\readline.c(261) : warning C4700: local variable 'rv' used without having been initialized
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\readline.c(332) : warning C4761: integral size mismatch in argument; conversion supplied
reset.c
signal.c
stab.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\stab.c(106) : warning C4018: '>' : signed/unsigned mismatch
starteic.c
stat.c
stdarg.c
stdClib.c
stdio.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdio.c(1034) : warning C4034: sizeof returns 0
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdio.c(1058) : warning C4034: sizeof returns 0
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\module\stdClib\src\stdio.c(1089) : warning C4034: sizeof returns 0
stdlib.c
string.c
strrev.c
strround.c
symbol.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\symbol.c(653) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\symbol.c(997) : warning C4133: '=' : incompatible types - from 'void ** ' to 'struct code_t ** '
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\symbol.c(1624) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\symbol.c(107) : warning C4761: integral size mismatch in argument; conversion supplied
time.c
typemod.c
typesets.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(816) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(822) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(951) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(966) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(1111) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(1132) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(1463) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(1496) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(1582) : warning C4146: unary minus operator applied to unsigned type, result still unsigned
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2565) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2566) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2567) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2568) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2569) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2570) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\typesets.c(2574) : warning C4113: 'void (__cdecl *)()' differs in parameter lists from 'void (__cdecl *)(unsigned int ,struct token_t *,struct token_t *)'
unistd.c
ymem.c
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(63) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(73) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(86) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(110) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(128) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(162) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(167) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(191) : warning C4018: '>=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(204) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\GAMEENGINE\EIC\src\ymem.c(292) : warning C4018: '!=' : signed/unsigned mismatch
Creating library...

<h3>
--------------------Configuration: staticSDL - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP60E.tmp" with contents
[
/nologo /Zp4 /W3 /Zi /Od /I "../../../../OpenGL ES/include/" /I "../../../../PocketHAL/" /I "..\..\src" /I "..\..\src\audio" /I "..\..\src\video" /I "..\..\src\video\wincommon" /I "..\..\src\video\windx5" /I "..\..\src\events" /I "..\..\src\joystick" /I "..\..\src\cdrom" /I "..\..\src\thread" /I "..\..\src\thread\win32" /I "..\..\src\timer" /I "..\..\include" /I "..\..\include\SDL" /D "HAVE_OPENGL" /D WIN32_PLATFORM_PSPC=310 /D "ARM" /D "_ARM_" /D "NO_SIGNAL_H" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "ENABLE_GAPI" /D "_LIB" /U "ENABLE_WINDIB" /Fp"../../../../output/PocketPC/SDL/staticSDL.pch" /YX /Fo"../../../../output/PocketPC/SDL/" /Fd"../../../../output/PocketPC/SDL/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP60E.tmp" 
Creating temporary file "c:\temp\RSP60F.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP60F.tmp"
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

<h3>
--------------------Configuration: mikmod - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP610.tmp" with contents
[
/nologo /W3 /Zi /Od /I "..\..\mikmod" /I "../../../SDL/include" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "USE_RWOPS" /Fp"../../../../output/PocketPC/mikmod/mikmod.pch" /YX /Fo"../../../../output/PocketPC/mikmod/" /Fd"../../../../output/PocketPC/mikmod/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP610.tmp" 
Creating temporary file "c:\temp\RSP611.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP611.tmp"
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
--------------------Configuration: native_midi - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP612.tmp" with contents
[
/nologo /W3 /Zi /Od /I "../../../SDL/include/" /I "..\.." /I "..\..\native_midi" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../../../output/PocketPC/native_midi/native_midi.pch" /YX /Fo"../../../../output/PocketPC/native_midi/" /Fd"../../../../output/PocketPC/native_midi/" /MC /c 
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_common.c"
"E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_win32.c"
]
Creating command line "clarm.exe @c:\temp\RSP612.tmp" 
Creating command line "link.exe -lib /nologo /out:"../../../../output/PocketPC/native_midi.lib"  \GAMEDEVELOPMENT\output\PocketPC\native_midi\native_midi_common.obj \GAMEDEVELOPMENT\output\PocketPC\native_midi\native_midi_win32.obj "
<h3>Output Window</h3>
Compiling...
native_midi_common.c
native_midi_win32.c
E:\GAMEDEVELOPMENT\SDL\SDL_mixer\native_midi\native_midi_win32.c(397) : warning C4244: 'initializing' : conversion from 'double ' to 'int ', possible loss of data
Creating library...

<h3>
--------------------Configuration: timidity - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP613.tmp" with contents
[
/nologo /W3 /Zi /Od /I "../../../SDL/include/" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../../../output/PocketPC/timidity/timidity.pch" /YX /Fo"../../../../output/PocketPC/timidity/" /Fd"../../../../output/PocketPC/timidity/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP613.tmp" 
Creating temporary file "c:\temp\RSP614.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP614.tmp"
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
--------------------Configuration: staticSDL_mixer - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP615.tmp" with contents
[
/nologo /W3 /Zi /Od /I "../../SDL/include/" /I "..\SDL_mad" /I "..\mikmod" /I "..\timidity" /I "..\native_midi" /I "tremor" /D "OGG_MUSIC" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WAV_MUSIC" /D "MOD_MUSIC" /D "USE_RWOPS" /D "FPM_DEFAULT" /D "OPT_SPEED" /D "ASO_INTERLEAVE1" /D inline=__inline /U "USE_NATIVE_MIDI" /U "USE_TIMIDITY_MIDI" /U "MID_MUSIC" /Fp"../../../output/PocketPC/SDL_mixer/staticSDL_mixer.pch" /YX /Fo"../../../output/PocketPC/SDL_mixer/" /Fd"../../../output/PocketPC/SDL_mixer/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP615.tmp" 
Creating temporary file "c:\temp\RSP616.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP616.tmp"
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
--------------------Configuration: ENGINE - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP617.tmp" with contents
[
/nologo /Zp4 /W3 /Zi /Od /I "../../OpenGL ES/include/" /I "../../PocketHAL/" /I "../../SDL/SDL//include" /I "../../SDL/SDL_image" /D "KYRA_SUPPORT_OPENGL" /D "GAME_EDITOR_PROFESSIONAL" /D "STAND_ALONE_GAME" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"../../output/PocketPC/engine/ENGINE.pch" /YX /Fo"../../output/PocketPC/engine/" /Fd"../../output/PocketPC/engine/" /MC /c 
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
]
Creating command line "clarm.exe @c:\temp\RSP617.tmp" 
Creating temporary file "c:\temp\RSP618.tmp" with contents
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
\GAMEDEVELOPMENT\output\PocketPC\SDL.lib
\GAMEDEVELOPMENT\output\PocketPC\SDL_mixer.lib
]
Creating command line "link.exe -lib @c:\temp\RSP618.tmp"
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
E:\GAMEDEVELOPMENT\kyra\engine\spriteresource.cpp(108) : warning C4018: '<' : signed/unsigned mismatch
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
Creating library...

<h3>
--------------------Configuration: RakNet - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP619.tmp" with contents
[
/nologo /Zp4 /W3 /Zi /Od /I "./../../Include" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D "ARM" /D "_ARM_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "LITTLE_ENDIAN" /Fp"../../../output/PocketPC/RakNet/RakNet.pch" /YX /Fo"../../../output/PocketPC/RakNet/" /Fd"../../../output/PocketPC/RakNet/" /MC /c 
"E:\GAMEDEVELOPMENT\RAKNET\Source\AES128.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\AsynchronousFileIO.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\BitStream.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\CheckSum.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\DataBlockEncryptor.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\DistributedNetworkObject.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\DistributedNetworkObjectManager.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\DistributedNetworkObjectStub.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\EncodeClassName.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\ExtendedOverlappedPool.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\GetTime.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\HuffmanEncodingTree.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\HuffmanEncodingTreeFactory.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\InternalPacketPool.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\NetworkObject.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\NetworkTypes.cpp"
"E:\GAMEDEVELOPMENT\RAKNET\Source\PacketPool.cpp"
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
Creating command line "clarm.exe @c:\temp\RSP619.tmp" 
Creating temporary file "c:\temp\RSP61A.tmp" with contents
[
/nologo /out:"../../../output/PocketPC\RakNet.lib" 
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
Creating command line "link.exe -lib @c:\temp\RSP61A.tmp"
<h3>Output Window</h3>
Compiling...
AES128.cpp
AsynchronousFileIO.cpp
BitStream.cpp
CheckSum.cpp
DataBlockEncryptor.cpp
E:\GAMEDEVELOPMENT\RAKNET\Source\DataBlockEncryptor.cpp(142) : warning C4101: 'randomChar' : unreferenced local variable
DistributedNetworkObject.cpp
DistributedNetworkObjectManager.cpp
DistributedNetworkObjectStub.cpp
EncodeClassName.cpp
ExtendedOverlappedPool.cpp
GetTime.cpp
HuffmanEncodingTree.cpp
HuffmanEncodingTreeFactory.cpp
InternalPacketPool.cpp
NetworkObject.cpp
NetworkTypes.cpp
PacketPool.cpp
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
--------------------Configuration: gameEngine - Win32 (WCE ARM) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP61B.tmp" with contents
[
/nologo /Zp4 /W3 /Zi /Od /I "../OpenGL ES/include/" /I "../SDL/SDL_mixer" /I "../SDL/SDL/include" /I "../SDL/SDL_image" /I "../kyra" /I "EiC/src" /D "GAME_EDITOR_PROFESSIONAL" /D "STAND_ALONE_GAME" /D "DEBUG" /D "ARM" /D "_ARM_" /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC=310" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "NO_LONG_LONG" /D "NO_DIRENT" /D "NO_FCNTL" /D "NO_STAT" /D "NO_UNISTD" /D "NO_TERMIOS" /D "__WIN32__" /D "WIN32" /D "NO_PIPE" /D "NO_POSIX" /D "NO_LONGLONG" /D "NO_HISTORY" /D "LOAD_BMP" /D "LOAD_GIF" /Fp"../output/PocketPC/gameEngine/gameEngine.pch" /YX /Fo"../output/PocketPC/gameEngine/" /Fd"../output/PocketPC/gameEngine/" /MC /c 
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
Creating command line "clarm.exe @c:\temp\RSP61B.tmp" 
Creating temporary file "c:\temp\RSP61C.tmp" with contents
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
Creating command line "link.exe -lib @c:\temp\RSP61C.tmp"
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
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(14) : warning C4005: 'ASSERT' : macro redefinition
        E:\Windows CE Toolsramas\wce300\Pocket PC 2002\include\dbgapi.h(170) : see previous definition of 'ASSERT'
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(168) : warning C4018: '<' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2368) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2404) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2435) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2460) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2485) : warning C4244: 'return' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2671) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(2671) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3109) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3109) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3109) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3457) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3457) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3457) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3457) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3470) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3470) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3470) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3470) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(3587) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4269) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4269) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4269) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4274) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4558) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4558) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4559) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4559) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4577) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4577) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4578) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4578) : warning C4244: 'argument' : conversion from 'int' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4809) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4809) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4821) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4822) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4968) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(4968) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5084) : warning C4018: '==' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5169) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Actor.cpp(5231) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
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
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(3572) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(3572) : warning C4307: '*' : integral constant overflow
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4201) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4201) : warning C4307: '*' : integral constant overflow
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4418) : warning C4308: negative integral constant converted to unsigned type
E:\GAMEDEVELOPMENT\gameEngine\dlmalloc/dlmalloc.c(4418) : warning C4307: '*' : integral constant overflow
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
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(14) : warning C4005: 'ASSERT' : macro redefinition
        E:\Windows CE Toolsramas\wce300\Pocket PC 2002\include\dbgapi.h(170) : see previous definition of 'ASSERT'
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1325) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1325) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1326) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1326) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1328) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1328) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1347) : warning C4244: '+=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1351) : warning C4244: '/=' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1365) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1365) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1449) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(1449) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\GameControl.cpp(6770) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
GameMain.cpp
E:\GAMEDEVELOPMENT\gameEngine\Action.h(280) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Action.h(281) : warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\PathFinder/../../kyra/engine/RTree.h(14) : warning C4005: 'ASSERT' : macro redefinition
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
E:\GAMEDEVELOPMENT\gameEngine\maksKyra.cpp(1057) : warning C4018: '>' : signed/unsigned mismatch
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
E:\GAMEDEVELOPMENT\gameEngine\RegionLoad.cpp(242) : warning C4018: '!=' : signed/unsigned mismatch
E:\GAMEDEVELOPMENT\gameEngine\RegionLoad.cpp(250) : warning C4018: '!=' : signed/unsigned mismatch
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
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1649) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1649) : warning C4244: 'argument' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(1672) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\Script.cpp(3497) : warning C4018: '<' : signed/unsigned mismatch
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
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(548) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(549) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
E:\GAMEDEVELOPMENT\gameEngine\SlowCanvas.cpp(550) : warning C4244: '=' : conversion from 'double' to 'unsigned char', possible loss of data
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
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(195) : warning C4620: no postfix form of 'operator ++' found for type 'Iterator', using prefix form
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\../../kyra/engine/RTree.h(106) : see declaration of 'Iterator'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(522) : warning C4620: no postfix form of 'operator ++' found for type 'Iterator', using prefix form
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\../../kyra/engine/RTree.h(106) : see declaration of 'Iterator'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(962) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(963) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(964) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(965) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1075) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1076) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1077) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(1078) : warning C4305: 'argument' : truncation from 'const double' to 'float'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(65) : warning C4244: '/=' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(65) : while compiling class-template member function 'void __cdecl Vector2D<float>::operator /=(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(65) : warning C4244: '/=' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(65) : while compiling class-template member function 'void __cdecl Vector2D<float>::operator /=(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(112) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(111) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator *(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(112) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(111) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator *(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(121) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(120) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator /(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(121) : warning C4244: 'argument' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Vector2D.h(120) : while compiling class-template member function 'class Vector2D<float> __cdecl Vector2D<float>::operator /(double)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(58) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(55) : while compiling class-template member function 'bool __cdecl Box2D<float>::intersects(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(59) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(55) : while compiling class-template member function 'bool __cdecl Box2D<float>::intersects(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(69) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(69) : warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(70) : warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(70) : warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Box2D.h(65) : while compiling class-template member function 'void __cdecl Box2D<float>::Merge(class Box2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(171) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(172) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(176) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(177) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(181) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(182) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\2dmath\Line2D.h(163) : while compiling class-template member function 'bool __cdecl Line2D<float>::getLineSegmentsIntersectionPoint(class Line2D<float> &,class Line2D<float> &,class Vector2D<float> &)'
E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(777) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
        E:\GAMEDEVELOPMENT\gameEngine\PathFinder\GeoPathfinder.cpp(830) : see reference to function template instantiation 'bool __cdecl getBoxLineIntersection(class GeoPathFinder *,class Box *,class Line2D<float>,class Vector2D<float> &,int &)' being compiled
Creating library...




<h3>Results</h3>
gameEngine.lib - 0 error(s), 324 warning(s)
</pre>
</body>
</html>
