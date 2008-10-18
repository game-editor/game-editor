noinst_PROGRAMS = krdemo

krdemo_SOURCES = bemgame.cpp \
blttest.cpp \
demos.cpp \
space.cpp \
spritetest.cpp \
tiletest.cpp \
gamefactory.cpp \
scale.cpp \
sinwave.cpp \
title.cpp \
text.cpp \
shooter.cpp \
demos.h \
allship.h \
bem.h


#dist_DATA = `find . -name *.bmp -name *.xml -name *.jpg -name *.gif -name *.tga -name *.pcx -name *.bin -name *.png -name ltconfig -name encodeall`

dist_data_DATA = encodeall 

#Hackery to copy all the datafiles to the distribution
dist-hook:
	find . -name \*.bmp -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.xml -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.jpg -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.gif -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.tga -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.pcx -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.bin -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.png -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.bmp -exec cp {} $(distdir) \; 2>/dev/null
	find . -name \*.txt -exec cp {} $(distdir) \; 2>/dev/null


krdemo_LDADD = @SDL_LIBS@
