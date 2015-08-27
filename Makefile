#CC = clang-3.6
CC = gcc

CFLAGS = -O3 -std=gnu89 -Wall ${DEFINES} -I./main/emu -I./main/sdl
DEFINES = -DHOME_SUPPORT -DSWITCHING_GRAPHICS -DSOUND_ON -DSOUND_EMULATION
LDFLAGS = -lSDLmain -lSDL -Wl,--as-needed
OUTPUT = oswan

SDL = main/sdl/main.c main/sdl/menu.c main/sdl/drawing.c main/sdl/input.c main/sdl/gfx/SDL_rotozoom.c
CPU = main/emu/cpu/nec.c
EMU_CORE = main/emu/WS.c main/emu/WSFileio.c main/emu/WSRender.c main/emu/WSApu.c 
SOURCES = ${SDL} ${CPU} ${EMU_CORE} 
OBJS = ${SOURCES:.c=.o}

# Enable this to support zip files
# Here, Support for zips is enabled
CFLAGS+=-DZIP_SUPPORT -I./minizip
LDFLAGS+=-lz
THIRD_PARTY = minizip/unzip.o minizip/ioapi.o
SOURCES+= ${THIRD_PARTY}


${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
	
clean:
	rm ${OBJS}
	rm ${OUTPUT}

install:
	mv $(OUTPUT) /usr/bin/$(OUTPUT)
