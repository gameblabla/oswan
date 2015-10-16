CC 			= clang-3.6

CFLAGS  	= -O3 -fno-omit-frame-pointer -fdata-sections -ffunction-sections -g -std=gnu89 -I./main/emu -I./main/sdl 
CFLAGS 	   += ${DEFINES}
DEFINES 	= -DHOME_SUPPORT -DSOUND_ON -DSOUND_EMULATION -DPOSIX -DNATIVE_RESOLUTION
LDFLAGS 	= -lSDLmain -lSDL -Wl,--as-needed
OUT  		= oswan

SDL 		= main/sdl/main.c main/sdl/menu.c  main/sdl/drawing.c main/sdl/input.c main/sdl/game_input.c
CPU 		= main/emu/cpu/nec.c
CORE 		= main/emu/WS.c main/emu/WSFileio.c main/emu/WSRender.c main/emu/WSApu.c 
SOURCES 	= ${SDL} ${CPU} ${CORE} 

# Comment the 3 lines below to disable zip support
CFLAGS 	   += -DZIP_SUPPORT -I./minizip
LDFLAGS	   += -lz
THIRD_PARTY+= minizip/unzip.o minizip/ioapi.o

# Comment the 2 lines below to disable Scaling support
#CFLAGS 	   += -DSCALING
#THIRD_PARTY+= main/sdl/gfx/SDL_rotozoom.c

SOURCES    += ${THIRD_PARTY}

OBJS 		= ${SOURCES:.c=.o}

${OUT}	: ${OBJS}
		${CC} -o ${OUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
		
clean	:
		rm ${OBJS}
		rm ${OUT}
