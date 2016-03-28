#include <os.h>
#include "n2DLib.h"
#include "shared.h"

/* Shows menu items and pointing arrow	*/
#define SPRX (16)
#define OFF_X 0

/* Re-adujusting Menu Y position */
#define OFF_Y (-6)

void screen_showchar(SDL_Surface *s, int x, int y, unsigned char a, const int fg_color, const int bg_color) 
{
	drawChar(&x, &y, 0, a, fg_color, bg_color);
}

void print_string(const char *s, const  unsigned short fg_color, const unsigned short bg_color, int x, int y) 
{
	drawString(&x, &y, 0, s, fg_color, bg_color);
}

void print_string_video(short x, const short y, const char *s) 
{
	if (GameConf.m_ScreenRatio != 1)
	{
		fillRect(0, 0, 18, 18, 0);
	}
	
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 8) 
	{
		screen_showchar(0, x, y, s[i], (255 >> 3) << 11 	| (0 >> 2) << 5 | (0 >> 3), 0);
	}
}

void clear_screen_menu(void)
{
	clearBufferB();
}

void draw_bluerect_menu(unsigned char i)
{
	fillRect(0 + OFF_X, (44+i*15)-2+8 + OFF_Y, 320, 12, ((0 >> 3) << 11) | ((0 >> 2) << 5) | (255 >> 3));
}

void draw_bluerect_file(unsigned char i)
{
	fillRect(0, 10*3+((i + 2) * 8), 320, 8, ((0 >> 3) << 11) | ((0 >> 2) << 5) | (255 >> 3));
}
