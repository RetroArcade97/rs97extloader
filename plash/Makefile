SDL_INC=$(RETROGAME_INCLUDE)/SDL
CROSS=mipsel-linux-

all:
	@echo "Available targets"
	@echo "   clean - clean things"
	@echo "   plash - build plash"
	@echo ""
	@if [ -z "$(RETROGAME_INCLUDE)" ] ; then \
		echo "You need to define RETROGAME_INCLUDE to point to your"; \
		echo "Toolchain include directory"; \
	fi
	@type $(CROSS)gcc || echo "No suitable cross compiler"


plash: main.cpp
	$(CROSS)gcc -s main.cpp -o plash -lSDL_image -lSDL_ttf -lSDL -I$(SDL_INC)

clean:
	rm -rf plash
