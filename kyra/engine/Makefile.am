lib_LTLIBRARIES = libkyra.la
libkyraincludedir = $(includedir)/Kyra/engine

#nasty awful horrible but effective hook :P
uninstall-hook:
	rm -rf $(includedir)/Kyra 

#and another
install-data-hook:
	mkdir -p $(includedir)/Kyra/util
	cp ../util/*.h $(includedir)/Kyra/util
	cp ../kyra.h $(includedir)/Kyra
	mkdir -p $(includedir)/Kyra/gui
	cp ../gui/*.h $(includedir)/Kyra/gui
	mkdir -p $(includedir)/Kyra/guiExtended
	cp ../guiExtended/*.h $(includedir)/Kyra/guiExtended
 
#and another ... could probably be combined with install-data-hook
install-exec-hook:
	mkdir -p $(includedir)/Kyra/tinyxml
	cp ../tinyxml/tinyxml.h $(includedir)/Kyra/tinyxml/tinyxml.h	

libkyrainclude_HEADERS = action.h \
box.h \
boxresource.h \
canvas.h \
canvasresource.h \
color.h \
dirtyrectangle.h \
encoder.h \
engine.h \
fontresource.h \
image.h \
imagecontainer.h \
imagenode.h \
imagetree.h \
imnode.h \
krmath.h \
kyra.h \
kyrabuild.h \
kyraresource.h \
namefield.h \
ogltexture.h \
painter.h \
parser.h \
pixelblock.h \
rle.h \
sdlutil.h \
splash.h \
sprite.h \
spriteresource.h \
tags.h \
textbox.h \
tile.h \
tileresource.h \
vault.h \
dataresource.h

libkyra_la_SOURCES = action.cpp \
box.cpp \
boxresource.cpp \
canvas.cpp \
canvasresource.cpp \
color.cpp \
dataresource.cpp \
dirtyrectangle.cpp \
encoder.cpp \
engine.cpp \
fontresource.cpp \
image.cpp \
imagecontainer.cpp \
imagetree.cpp \
imnode.cpp \
krmath.cpp \
kyraresource.cpp \
namefield.cpp \
ogltexture.cpp \
painter.cpp \
parser.cpp \
pixelblock.cpp \
rle.cpp \
sdlutil.cpp \
splash.cpp \
sprite.cpp \
spriteresource.cpp \
textbox.cpp \
tile.cpp \
tileresource.cpp \
vault.cpp \
action.h \
box.h \
boxresource.h \
canvas.h \
canvasresource.h \
color.h \
dataresource.h \
dirtyrectangle.h \
encoder.h \
engine.h \
fontresource.h \
image.h \
imagecontainer.h \
imagenode.h \
imagetree.h \
imnode.h \
krmath.h \
kyra.h \
kyrabuild.h \
kyraresource.h \
namefield.h \
ogltexture.h \
painter.h \
parser.h \
pixelblock.h \
rle.h \
sdlutil.h \
sprite.h \
spriteresource.h \
tags.h \
textbox.h \
tile.h \
tileresource.h \
vault.h



libkyra_la_LIBADD = \
	../util/libutil.la \
	../tinyxml/libtinyxml.la \
	../gui/libgui.la \
	../guiExtended/libguiExtended.la \
	@SDL_LIBS@
