CC = gcc

CFLAGS = -O1 -g -pg -Wall ${DEFINES} -I./main/emu -I./main/sdl
DEFINES = -DSMOOTH -DSOUND_ON -DSOUND_EMULATION -DHOME_SUPPORT
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDLmain -lSDL -pg -g
OUTPUT = oswan

SDL = main/fast/main.c main/sdl/menu.c main/sdl/hack.c main/sdl/gfx/SDL_rotozoom.c main/sdl/drawing.c main/sdl/input.c main/sdl/md5.c
CPU = main/emu/cpu/nec.c
EMU_CORE = main/fast/WS.c main/emu/WSApu.c main/emu/WSFileio.c main/emu/WSRender.c
SOURCES = ${SDL} ${CPU} ${EMU_CORE}
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
	
clean:
	rm ${OBJS}
	rm ${OUTPUT}

install:
	mv $(OUTPUT) /usr/bin/$(OUTPUT)
