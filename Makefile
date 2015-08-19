CC = gcc

CFLAGS = -O2 -Wall ${DEFINES} -I./emu -I./sdl
DEFINES = -DLAYERS -DSMOOTH -DSOUND_ON -DSOUND_EMULATION -DSPEEDHACKS -DHOME_SUPPORT -DBETTER_CPU
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDLmain -lSDL 
OUTPUT = oswan

SOURCES = sdl/main_od.c sdl/menu.c sdl/hack.c sdl/drawing.c emu/cpu/nec.c emu/WS.c emu/WSApu.c emu/WSFileio.c emu/WSInput.c emu/WSRender.c
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
