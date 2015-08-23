CC = gcc

CFLAGS = -O2 -Wall ${DEFINES} -I./main/emu -I./main/sdl
DEFINES = -DSMOOTH -DSOUND_ON -DSOUND_EMULATION -DSPEEDHACKS -DHOME_SUPPORT -DSCALING
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDLmain -lSDL 
OUTPUT = oswan

SDL = main/sdl/main.c main/sdl/menu.c main/sdl/hack.c main/sdl/gfx/SDL_rotozoom.c main/sdl/drawing.c main/sdl/input.c
EMU_CORE = main/emu/cpu/nec.c main/emu/cpu/io.c main/emu/WS.c main/emu/WSApu.c main/emu/WSFileio.c main/emu/WSRender.c
SOURCES = ${SDL} ${EMU_CORE}
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
	
clean:
	rm ${OBJS}
	rm ${OUTPUT}

install:
	mv $(OUTPUT) /usr/bin/$(OUTPUT)
