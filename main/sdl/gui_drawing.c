#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "drawing.h"
#include "font.h"

/* Shows menu items and pointing arrow	*/
#define SPRX (16)
#define OFF_X 0

/* Re-adujusting Menu Y position */
#define OFF_Y (-6)

void screen_showchar(SDL_Surface *s, int x, int y, unsigned char a, const int fg_color, const int bg_color) 
{
	unsigned short *dst;
	unsigned short w, h;

	SDL_LockSurface(s);
	for(h = 8; h; h--) 
	{

		dst = (unsigned short *)s->pixels + ((y+8-h)*s->w + x);
		
		for(w = 8; w; w--) 
		{
			unsigned short color = *dst; /* background */
			if((fontdata8x8[a*8 + (8-h)] >> w) & 1) color = fg_color;
			*dst++ = color;
		}
	}
	SDL_UnlockSurface(s);
}

void print_string(const char *s, const  unsigned short fg_color, const unsigned short bg_color, int x, int y) 
{
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 6) screen_showchar(actualScreen, x, y, s[i], fg_color, bg_color);
}

void print_string_video(short x, const short y, const char *s) 
{
	if (GameConf.m_ScreenRatio != 1)
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 18;
		rect.h = 18;
		SDL_FillRect(actualScreen, &rect, 0);
	}
	
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 8) 
	{
		screen_showchar(actualScreen, x, y, s[i], SDL_MapRGB(actualScreen->format,255, 0, 0), 0);
	}
}


void clear_screen_menu(void)
{
	SDL_FillRect(actualScreen, NULL, 0);
}

void draw_bluerect_menu(unsigned char i)
{
	SDL_Rect position_select;
	position_select.w  = 320;
	position_select.h  = 12;
	position_select.x  = 0 + OFF_X;
	position_select.y  = (44+i*15)-2+8 + OFF_Y;
	SDL_FillRect(actualScreen, &position_select, SDL_MapRGB(actualScreen->format,0,0,255));
}

void draw_bluerect_file(unsigned char i)
{
	SDL_Rect position_select;
	position_select.w  = 320;
	position_select.h  = 8;
	position_select.x  = 0;
	position_select.y  = 10*3+((i + 2) * 8);
	SDL_FillRect(actualScreen, &position_select, SDL_MapRGB(actualScreen->format,0,0,255));
}


