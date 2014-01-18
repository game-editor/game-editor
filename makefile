## Makefile principal do projeto GameEditor
##


## Game Editor Version Definitions

GAMEEDITOR_PROFESSIONAL=-DGAME_EDITOR_PROFESSIONAL
GAMEEDITOR_PROFESSIONAL_STAND_ALONE=-DSTAND_ALONE_GAME -DGAME_EDITOR_PROFESSIONAL
GAMEEDITOR_BETA=-DGAME_EDITOR_PROFESSIONAL -DGAME_EDITOR_BETA
GAMEEDITOR_BETA_STAND_ALONE=-DSTAND_ALONE_GAME -DGAME_EDITOR_PROFESSIONAL -DGAME_EDITOR_BETA


GAMEEDITOR_DEMO=#no defines
GAMEEDITOR_DEMO_STAND_ALONE=-DSTAND_ALONE_GAME

## Target machine (LLVM, linux)

TARGET_MACHINE=linux

## Release Definitions
GE_DEBUG=-DDEBUG -D_DEBUG -g

ifeq ($(TARGET_MACHINE),LLVM)
        GE_RELEASE=#
else
		GE_RELEASE=#
		#GE_RELEASE=-s -Os #-O3 #Os for size, O3 for speed	
endif


## Build Configuration

GAMEEDITOR_TYPE=$(GAMEEDITOR_BETA)
BUILD_TYPE=$(GE_RELEASE)

## Defines
# linux -> -D__linux -Dlinux -D__linux__ -DLINUX_I386

ifeq ($(TARGET_MACHINE),LLVM)
        TARGET= -DLLVM
else
		TARGET= -D__linux -Dlinux -D__linux__ -DLINUX_I386	
endif

## Commands

#linux -> gcc, make (use gcc with the -m32 option on 64bit linux)
#cygwin ->
#Sourcery G++ -> arm-none-linux-gnueabi-gcc, cs-make
#llvm -> llvm-gcc -emit-llvm -c, make 

MAKE = make	

ifeq ($(TARGET_MACHINE),LLVM)
        CC = llvm-gcc -emit-llvm -c			
else
		CC = gcc -m32 -lm		
endif



all:
	mkdir -p output output/eic output/gameEngine output/engine
	$(MAKE) -f makefile.zlib -C SDL/SDL_image/png/libpng/projects/msvc 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.libpng -C SDL/SDL_image/png/libpng/projects/msvc 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.jpegm -C SDL/SDL_image/jpeg/VisualC 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'

	$(MAKE) -f makefile.sdl -C SDL/SDL/VisualC/SDL 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'

	$(MAKE) -f makefile.sdlimage -C SDL/SDL_image/VisualC 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'

	$(MAKE) -f makefile.mikmod -C SDL/SDL_mixer/VisualC/mikmod 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.nativemidi -C SDL/SDL_mixer/VisualC/native_midi 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.sdlmixer -C SDL/SDL_mixer/VisualC 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'

	$(MAKE) -f makefile.freetype -C SDL/SDL_ttf/VisualC/FreeType/builds/win32/visualc 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'

	$(MAKE) -f makefile.eic -C gameEngine/EiC/Visual/ 'GAMEEDITOR_TYPE=$(GAMEEDITOR_TYPE)' 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.kyra -C kyra/engine/ 'GAMEEDITOR_TYPE=$(GAMEEDITOR_TYPE)' 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.gengine -C gameEngine/ 'GAMEEDITOR_TYPE=$(GAMEEDITOR_TYPE)' 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	$(MAKE) -f makefile.editor -C gameEditor/ 'GAMEEDITOR_TYPE=$(GAMEEDITOR_TYPE)' 'BUILD_TYPE=$(BUILD_TYPE)' 'CC=$(CC)' 'TARGET=$(TARGET)'
	cp -R bin/editor.dat bin/Tutorials bin/Docs output/

clean:
	$(MAKE) -f makefile.zlib -C SDL/SDL_image/png/libpng/projects/msvc clean
	$(MAKE) -f makefile.libpng -C SDL/SDL_image/png/libpng/projects/msvc clean
	$(MAKE) -f makefile.jpegm -C SDL/SDL_image/jpeg/VisualC clean

	$(MAKE) -f makefile.sdl -C SDL/SDL/VisualC/SDL clean

	$(MAKE) -f makefile.sdlimage -C SDL/SDL_image/VisualC clean

	$(MAKE) -f makefile.mikmod -C SDL/SDL_mixer/VisualC/mikmod clean
	$(MAKE) -f makefile.nativemidi -C SDL/SDL_mixer/VisualC/native_midi clean
	$(MAKE) -f makefile.sdlmixer -C SDL/SDL_mixer/VisualC clean

	$(MAKE) -f makefile.freetype -C SDL/SDL_ttf/VisualC/FreeType/builds/win32/visualc clean

	$(MAKE) -f makefile.eic -C gameEngine/EiC/Visual/ clean
	$(MAKE) -f makefile.kyra -C kyra/engine/ clean
	$(MAKE) -f makefile.gengine -C gameEngine/ clean
	$(MAKE) -f makefile.editor -C gameEditor/ clean

	rm -rf output/eic output/gameEngine output/engine output/editor.dat output/Tutorials output/Docs output/GameEditor.ini
