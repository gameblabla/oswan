CC = gcc

CFLAGS = -O2 -Wall ${DEFINES} -I./emu -I./sdl
DEFINES = -DSMOOTH -DSOUND_ON -DSOUND_EMULATION -DSPEEDHACKS -DHOME_SUPPORT -DSCALING
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDLmain -lSDL 
OUTPUT = oswan

SDL = sdl/main_od.c sdl/menu.c sdl/hack.c sdl/gfx/SDL_rotozoom.c sdl/drawing.c sdl/input.c
EMU_CORE = emu/cpu/nec.c emu/cpu/io.c emu/WS.c emu/WSApu.c emu/WSFileio.c emu/WSRender.c
SOURCES = ${SDL} ${EMU_CORE}
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
