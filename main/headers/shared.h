#ifndef SHARED_H
#define SHARED_H

#ifdef _TINSPIRE
#include <os.h>
#else
#include <SDL.h>
#endif

#ifdef GECKO
#include <ogcsys.h>
#include <gccore.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#ifdef GCW
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef JOYSTICK
	#define JOYSTICK
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
#endif

#ifdef RS97
	#ifndef POSIX
	#define POSIX
	#endif
#endif

#ifdef DINGOO
	#ifndef NOWAIT
	#define NOWAIT
	#endif
#endif

#ifdef _TINSPIRE
	#ifndef NO_WAIT
	#define NO_WAIT
	#endif
	
	#ifndef NOSCREENSHOTS
	#define NOSCREENSHOTS
	#endif
	
	#define SDL_Surface int
#endif

#ifdef DREAMCAST
	#ifndef POSIX
	#define POSIX
	#endif
	#ifndef JOYSTICK
	#define JOYSTICK
	#endif
#endif

#ifdef _TINSPIRE
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
#endif

#ifdef _TINSPIRE
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(GCW)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(RS97)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 480
#elif defined(DREAMCAST)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#else
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#endif

#ifdef _TINSPIRE
	#define PATH_DIRECTORY NDLESS_DIR
	#define SAVE_DIRECTORY ".oswan/"
	#define EXTENSION ".tns"
#elif defined(GCW)
	#define PATH_DIRECTORY getenv("HOME")
	#define SAVE_DIRECTORY "/.oswan/"
	#define EXTENSION ""
#elif defined(DREAMCAST)
	#define PATH_DIRECTORY "/ram/"
	#define SAVE_DIRECTORY ""
	#define EXTENSION ""
#else
	#ifdef HOME_SUPPORT
		#define PATH_DIRECTORY getenv("HOME")
		#define SAVE_DIRECTORY "/.oswan/"
	#else
		#define PATH_DIRECTORY "./"
		#define SAVE_DIRECTORY ""
	#endif
	#define EXTENSION ""
#endif

#ifdef _TINSPIRE
	#define PAD_UP		KEY_NSPIRE_UP
	#define PAD_LEFT	KEY_NSPIRE_LEFT
	#define PAD_RIGHT	KEY_NSPIRE_RIGHT
	#define PAD_DOWN	KEY_NSPIRE_DOWN

	#define PAD_XUP		KEY_NSPIRE_UP
	#define PAD_XLEFT	KEY_NSPIRE_LEFT
	#define PAD_XRIGHT	KEY_NSPIRE_RIGHT
	#define PAD_XDOWN	KEY_NSPIRE_DOWN
	
	#define PAD_YUP		KEY_NSPIRE_8
	#define PAD_YLEFT	KEY_NSPIRE_4
	#define PAD_YRIGHT	KEY_NSPIRE_6
	#define PAD_YDOWN	KEY_NSPIRE_5
	
	#define PAD_A		KEY_NSPIRE_CTRL
	#define PAD_B		KEY_NSPIRE_SHIFT
	
	#define PAD_X		KEY_NSPIRE_VAR
	#define PAD_Y		KEY_NSPIRE_DEL
	
	#define PAD_L		KEY_NSPIRE_L
	#define PAD_R		KEY_NSPIRE_R

	#define PAD_START		KEY_NSPIRE_TAB
	#define PAD_SELECT		KEY_NSPIRE_MENU
	
	#define PAD_SLIDER		KEY_NSPIRE_ENTER
	
	#define PAD_QUIT		KEY_NSPIRE_ESC

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
	

#elif defined(RS97)

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
	

#elif defined(DINGOO)

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
	
#elif defined(GECKO)

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
	
	#define PAD_SLIDER		0
	
	#define PAD_QUIT		0

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
	
	#define PAD_SLIDER		0
	
	#define PAD_QUIT		SDLK_ESCAPE
#endif

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

#define PIX_TO_RGB(fmt, r, g, b) (((r*8>>fmt->Rloss)<<fmt->Rshift)| ((g*6>>fmt->Gloss)<<fmt->Gshift)|((b*8>>fmt->Bloss)<<fmt->Bshift))

/* Oswan dependencies */
#include "../emu/WS.h"
#include "../emu/WSApu.h"
#include "../emu/WSFileio.h"
#include "../emu/WSRender.h"

#define cartridge_IsLoaded() (strlen(gameName) != 0)

typedef struct {
	unsigned short sndLevel;
	unsigned short m_ScreenRatio; 	/* 0 = 1x size, 1 = full screen, 2 = Keep Aspect */
	unsigned short OD_Joy[12]; 		/* each key mapping	*/
	unsigned short m_DisplayFPS;
	char current_dir_rom[MAX__PATH];
	unsigned short input_layout;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned short reserved3;
	unsigned short reserved4;
} gamecfg;

extern SDL_Surface* actualScreen;	/* Main program screen */
#if !defined(NOSCREENSHOTS)
extern SDL_Surface* screenshots;	
#endif	

#if !defined(_TINSPIRE)
extern SDL_Event event;
#endif

extern gamecfg GameConf;
extern unsigned int m_Flag;

extern char gameName[512];
extern char current_conf_app[MAX__PATH];

extern void system_loadcfg(const char *cfg_name);
extern void system_savecfg(const char *cfg_name);

extern void mainemuinit();

/* menu */
extern void screen_showtopmenu(void);
extern void print_string_video(short x, const short y, const char *s);

extern void Buttons(void);
extern short button_state[18];
extern unsigned char button_time[18];

#endif
