#ifndef SHARED_H
#define SHARED_H

#ifdef _TINSPIRE
#include <os.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <SDL/SDL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#ifdef _TINSPIRE
	#define BITDEPTH_OSWAN has_colors ? 16 : 8
	#define FLAG_VIDEO SDL_SWSURFACE
#elif defined(GCW0)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
#elif defined(DREAMCAST)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
#else
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
#endif

#ifdef _TINSPIRE
	#define PATH_DIRECTORY "/documents/ndless/"
	#define EXTENSION ".tns"
#elif defined(GCW)
	#define PATH_DIRECTORY getenv("HOME")
	#define EXTENSION ""
#else
	#define PATH_DIRECTORY "./"
	#define EXTENSION ""
#endif
	
#ifdef GCW
	#ifndef HOME_SUPPORT
		#define HOME_SUPPORT
	#endif
	#ifndef JOYSTICK
		#define JOYSTICK
	#endif
#endif

#ifdef _TINSPIRE
	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN

	#define PAD_XUP		SDLK_UP
	#define PAD_XLEFT	SDLK_LEFT
	#define PAD_XRIGHT	SDLK_RIGHT
	#define PAD_XDOWN	SDLK_DOWN
	
	#define PAD_YUP		SDLK_UP_
	#define PAD_YLEFT	SDLK_LEFT_
	#define PAD_YRIGHT	SDLK_RIGHT_
	#define PAD_YDOWN	SDLK_DOWN_
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_TAB
	
	#define PAD_X		SDLK_LSHIFT
	#define PAD_Y		SDLK_SPACE
	
	#define PAD_L		SDLK_s
	#define PAD_R		SDLK_l

	#define PAD_START		SDLK_LSHIFT
	#define PAD_SELECT		SDLK_LSHIFT
	
	#define PAD_QUIT		SDLK_ESCAPE

#elif defined(GCW)

	#define PAD_XUP		SDLK_UP
	#define PAD_XLEFT	SDLK_LEFT
	#define PAD_XRIGHT	SDLK_RIGHT
	#define PAD_XDOWN	SDLK_DOWN

	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_LALT
	
	#define PAD_X		SDLK_LSHIFT
	#define PAD_Y		SDLK_SPACE
	
	#define PAD_L		SDLK_TAB
	#define PAD_R		SDLK_BACKSPACE
	
	#define PAD_START		SDLK_RETURN
	#define PAD_SELECT		SDLK_ESCAPE
	
	#define PAD_SLIDER		SDLK_HOME
	
	#define PAD_QUIT		SDLK_ESCAPE

#else

	#define PAD_XUP		SDLK_UP
	#define PAD_XLEFT	SDLK_LEFT
	#define PAD_XRIGHT	SDLK_RIGHT
	#define PAD_XDOWN	SDLK_DOWN
	
	#define PAD_YUP		SDLK_t
	#define PAD_YLEFT	SDLK_y
	#define PAD_YRIGHT	SDLK_u
	#define PAD_YDOWN	SDLK_i
	
	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_LALT
	
	#define PAD_X		SDLK_LSHIFT
	#define PAD_Y		SDLK_SPACE
	
	#define PAD_L		SDLK_s
	#define PAD_R		SDLK_l
	
	#define PAD_START		SDLK_RETURN
	#define PAD_SELECT		SDLK_BACKSPACE
	
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

#define PIX_TO_RGB(fmt, r, g, b) (((r*8>>fmt->Rloss)<<fmt->Rshift)| ((g*6>>fmt->Gloss)<<fmt->Gshift)|((b*8>>fmt->Bloss)<<fmt->Bshift))

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
	unsigned int input_layout;
	unsigned int reserved1;
	unsigned int reserved2;
	unsigned int reserved3;
	unsigned int reserved4;
#ifndef OLDSAVE_HACK
	unsigned char save_oldoswan_check;
#endif
} gamecfg;

//typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;

extern SDL_Surface* screen;						// Main program screen
extern SDL_Surface* actualScreen, *screenshots;						// Main program screen

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
