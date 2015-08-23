CC = gcc

CFLAGS = -O2 -Wall ${DEFINES} -I./main/emu -I./main/sdl
DEFINES = -DSMOOTH -DSOUND_ON -DSOUND_EMULATION -DSPEEDHACKS -DHOME_SUPPORT -DSCALING
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDLmain -lSDL 
OUTPUT = oswan

SDL = main/sdl/main.c main/sdl/menu.c main/sdl/hack.c main/sdl/gfx/SDL_rotozoom.c main/sdl/drawing.c main/sdl/input.c main/sdl/md5.c
CPU = main/emu/cpu/nec.c
EMU_CORE = main/emu/WS.c main/emu/WSApu.c main/emu/WSFileio.c main/emu/WSRender.c
SOURCES = ${SDL} ${EMU_CORE} ${CPU}
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
	
clean:
	rm emu/*.o
	rm emu/cpu/*.o
	rm sdl/*.o
	rm ${OUTPUT}

install:
	mv $(OUTPUT) /usr/bin/$(OUTPUT)
