CC 			= clang

CFLAGS  	= -O2 -g -Weverything -fno-common -fdata-sections -ffunction-sections -Imain/emu -Imain/sdl -Imain/headers -Imain/menu -Imain/sound -Imain/scalers
CFLAGS 	   += ${DEFINES}
CFLAGS 	   += $(shell sdl-config --cflags) -std=gnu99
DEFINES 	= -DHOME_SUPPORT -DSOUND_ON -DSOUND_EMULATION -DPOSIX -DRS90
LDFLAGS 	= $(shell sdl-config --libs) -Wl,--as-needed -Wl,--gc-sections
OUT  		= oswan

SDL 		= main/sdl/main.c main/sdl/input.c main/sound/sound_SDL.c
CPU 		= main/emu/cpu/nec.c
CORE 		= main/emu/WS.c main/emu/WSFileio.c main/emu/WSRender.c main/emu/WSApu.c 
DRAWING		= main/sdl/drawing.c main/scalers/scaler.c
MENU 		= main/menu/menu.c main/menu/font_drawing.c

SOURCES 	= ${SDL} ${CPU} ${CORE} ${DRAWING} ${MENU}

# Comment the 3 lines below to disable zip support
#CFLAGS 	   += -DZIP_SUPPORT -I./minizip
#LDFLAGS	   += 
#THIRD_PARTY+= minizip/unzip.o minizip/ioapi.o minizip/fileio.o minizip/miniz.o

CFLAGS 	   += -DJOYSTICK

SOURCES    += ${THIRD_PARTY}

OBJS 		= ${SOURCES:.c=.o}

${OUT}	: ${OBJS}
		${CC} -o ${OUT} ${SOURCES} ${CFLAGS} ${LDFLAGS}
		
clean	:
		rm ${OBJS}
		rm ${OUT}
