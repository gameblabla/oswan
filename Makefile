CC = gcc

CFLAGS = -O3 -Wall ${DEFINES} -I./main/emu -I./main/sdl
DEFINES = -DHOME_SUPPORT -DSWITCHING_GRAPHICS -DSOUND_ON -DSOUND_EMULATION -DNATIVE_AUDIO
LDFLAGS = -lSDLmain -lSDL -Wl,--as-needed
OUTPUT = oswan

SDL = main/sdl/main.c main/sdl/menu.c main/sdl/drawing.c main/sdl/input.c
CPU = main/emu/cpu/nec.c
EMU_CORE = main/emu/WS.c main/emu/WSFileio.c main/emu/WSRender.c main/emu/WSApu.c
SOURCES = ${SDL} ${CPU} ${EMU_CORE}
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
	
clean:
	rm ${OBJS}
	rm ${OUTPUT}

install:
	mv $(OUTPUT) /usr/bin/$(OUTPUT)
