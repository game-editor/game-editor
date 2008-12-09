#!/bin/sh
#

# Autodetect wxWidgets settings
if wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if ! wx-config --debug >/dev/null 2>/dev/null; then
	debug="--disable-wx-debug"
fi

# premake/premake-linux --target cb-gcc --shared --with-wx-shared $unicode $debug $1
premake/premake-linux --target cb-gcc $unicode $debug $1
echo done...
echo 
#
# premake/premake-linux --target gnu --shared --with-wx-shared $unicode $debug $1
premake/premake-linux --target gnu $unicode $debug $1
echo done...
echo 
#
echo Done generating all project files for wxXmlSerializer.
#
