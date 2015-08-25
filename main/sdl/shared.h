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

#ifdef GCW
	#ifndef HOME_SUPPORT
		#define HOME_SUPPORT
	#endif
	#ifndef JOYSTICK
		#define JOYSTICK
	#endif
	#ifndef UNIX
		#define UNIX
	#endif
#endif

#ifdef _TINSPIRE
	//#define BITDEPTH_OSWAN has_colors ? 16 : 8
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
#elif defined(GCW)
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
	#define SAVE_DIRECTORY ""
	#define EXTENSION ".tns"
#elif defined(GCW)
	#define PATH_DIRECTORY getenv("HOME")
	#define SAVE_DIRECTORY "/.oswan/"
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


#ifdef PSP
	#include <pspkernel.h>
	#include <pspctrl.h>
	#include <pspdisplay.h>
	#include <psppower.h>
	#define keys[x] keys(x)
	int keys(int controls);
	int keys(int controls)
	{
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(&pad, 1);
		if (pad.Buttons & controls)
			return 1;
		else
			return 0;
	}
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

#elif defined(PSP)

	#define PAD_XUP		PSP_CTRL_UP
	#define PAD_XLEFT	PSP_CTRL_LEFT
	#define PAD_XRIGHT	PSP_CTRL_RIGHT
	#define PAD_XDOWN	PSP_CTRL_DOWN

	#define PAD_UP		PSP_CTRL_UP
	#define PAD_LEFT	PSP_CTRL_LEFT
	#define PAD_RIGHT	PSP_CTRL_RIGHT
	#define PAD_DOWN	PSP_CTRL_DOWN
	
	#define PAD_A		PSP_CTRL_CROSS
	#define PAD_B		PSP_CTRL_CIRCLE
	#define PAD_X		PSP_CTRL_SQUARE
	#define PAD_Y		PSP_CTRL_TRIANGLE
	
	#define PAD_L		PSP_CTRL_L
	#define PAD_R		PSP_CTRL_R
	
	#define PAD_START		PSP_CTRL_START
	#define PAD_SELECT		0
	
	#define PAD_SLIDER		0
	
	#define PAD_QUIT		PSP_CTRL_HOME

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

#define PIX_TO_RGB(fmt, r, g, b) (((r*8>>fmt->Rloss)<<fmt->Rshift)| ((g*6>>fmt->Gloss)<<fmt->Gshift)|((b*8>>fmt->Bloss)<<fmt->Bshift))

// osWan dependencies
#include "../emu/WS.h"
#include "../emu/WSApu.h"
#include "../emu/WSFileio.h"
#include "../emu/WSRender.h"

#define cartridge_IsLoaded() (strlen(gameName) != 0)

typedef struct {
	unsigned short sndLevel;
	unsigned short m_ScreenRatio; // 0 = original show, 1 = full screen
	unsigned short OD_Joy[12]; // each key mapping
	unsigned short m_DisplayFPS;
	char current_dir_rom[MAX__PATH];
	unsigned short input_layout;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned short reserved3;
	unsigned short reserved4;
#ifndef NOSAVE_HACK
	unsigned char save_oldoswan_check;
#endif
} gamecfg;

extern SDL_Surface* screen;						// Main program screen
extern SDL_Surface* actualScreen, *screenshots;						// Main program screen

extern SDL_Event event;

extern gamecfg GameConf;
extern unsigned int m_Flag;

extern char gameName[512];
extern char current_conf_app[MAX__PATH];

extern void system_loadcfg(const char *cfg_name);
extern void system_savecfg(const char *cfg_name);

extern void mainemuinit();

// menu
extern void screen_showtopmenu(void);
extern void print_string_video(short x, const short y, const char *s);

#endif
