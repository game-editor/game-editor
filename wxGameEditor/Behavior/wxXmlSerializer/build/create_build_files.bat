@echo off

rem premake\premake-win32.exe --target cb-gcc --unicode --with-wx-shared --shared
premake\premake-win32.exe --target cb-gcc --unicode --static-runtime
echo done...
echo.

rem premake\premake-win32.exe --target gnu --unicode --with-wx-shared --shared
premake\premake-win32.exe --target gnu --unicode --static-runtime
echo done...
echo.

rem premake\premake-win32.exe --target vs2005 --unicode --with-wx-shared --shared --no-builtin-wchar
premake\premake-win32.exe --target vs2005 --unicode --static-runtime --no-builtin-wchar
echo done...
echo.

echo Done generating all project files for wxXmlSerializer