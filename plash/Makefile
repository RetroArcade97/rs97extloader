SDL_INC=$(RETROGAME_INCLUDE)/SDL
CROSS=mipsel-linux-

help:
	@echo "Available targets"
	@echo "   clean - clean things"
	@echo "   plash - build plash"
	@echo "   sdlmenu - build sdlmenu"
	@echo "   all - build all targets"
	@echo ""
	@if [ -z "$(RETROGAME_INCLUDE)" ] ; then \
		echo "You need to define RETROGAME_INCLUDE to point to your"; \
		echo "Toolchain include directory"; \
	fi
	@type $(CROSS)gcc || echo "No suitable cross compiler"

all: plash sdlmenu

plash: main.cpp
	$(CROSS)gcc -s $< -o $@ -lSDL_image -lSDL_ttf -lSDL -I$(SDL_INC)

sdlmenu: sdlmenu.c
	$(CROSS)gcc -s $< -o $@ -lSDL_image -lSDL_ttf -lSDL -I$(SDL_INC)


clean:
	rm -rf plash sdlmenu
