#ifndef SHARED_H
#define SHARED_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <SDL/SDL.h>

#ifdef NSPIRE
	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_TAB
	#define PAD_X		SDLK_BACKSPACE
	#define PAD_Y		SDLK_MENU

	#define PAD_L		SDLK_PLUS
	#define PAD_R		SDLK_MINUS
	
	#define PAD_START		SDLK_LSHIFT
	#define PAD_SELECT		SDLK_LSHIFT
	
	#define PAD_QUIT		SDLK_ESCAPE
#else
	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_LALT
	#define PAD_X		SDLK_SPACE
	#define PAD_Y		SDLK_LSHIFT

	#define PAD_L		SDLK_BACKSPACE
	#define PAD_R		SDLK_TAB
	
	#define PAD_START		SDLK_RETURN
	#define PAD_SELECT		SDLK_ESCAPE
	
	#define PAD_QUIT		SDLK_ESCAPE
#endif

// defines and macros
#define MAX__PATH 1024
#define FILE_LIST_ROWS 19

#define SYSVID_WIDTH	224
#define SYSVID_HEIGHT	144

#define GF_GAMEINIT    1
#define GF_MAINUI      2
#define GF_GAMEQUIT    3
#define GF_GAMERUNNING 4

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define true 1
#define false 0
typedef int bool;

#define PIX_TO_RGB(fmt, r, g, b) (((r>>fmt->Rloss)<<fmt->Rshift)| ((g>>fmt->Gloss)<<fmt->Gshift)|((b>>fmt->Bloss)<<fmt->Bshift))

// osWan dependencies
#include "../emu/WS.h"
#include "../emu/WSApu.h"
#include "../emu/WSFileio.h"
#include "../emu/WSRender.h"

#define cartridge_IsLoaded() (strlen(gameName) != 0)

typedef struct {
  unsigned int sndLevel;
  unsigned int m_ScreenRatio; // 0 = original show, 1 = full screen
  unsigned int OD_Joy[12]; // each key mapping
  unsigned int m_DisplayFPS;
  char current_dir_rom[MAX__PATH];
} gamecfg;

//typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;

extern SDL_Surface* screen;						// Main program screen
extern SDL_Surface* actualScreen;						// Main program screen

#ifdef LAYERS
extern SDL_Surface *layer,*layerback,*layerbackgrey;
#endif

extern SDL_Event event;

extern gamecfg GameConf;
extern unsigned int m_Flag;

extern char gameName[512];
extern char current_conf_app[MAX__PATH];

extern unsigned int gameCRC;

extern void system_loadcfg(char *cfg_name);
extern void system_savecfg(char *cfg_name);

extern unsigned long crc32 (unsigned int crc, const unsigned char *buf, unsigned int len);


extern void mainemuinit();

// menu
extern void screen_showtopmenu(void);
extern void print_string_video(int x, int y, const char *s);

#endif
