#include "shared.h"

#ifdef SCALING
void Get_resolution(unsigned short w, unsigned short h);
#endif

void SetVideo(unsigned char mode);
void gcw_screen_draw(void);
void screen_draw(void);
void take_screenshot(void);

#if !defined(SCALING)
	#define flip_screen SDL_Flip
#else
	void flip_screen(SDL_Surface* screen);
	SDL_Surface *real_screen;
#endif

extern int FPS;

struct scaling
{
	unsigned short w_display;
	unsigned short h_display;
	unsigned char w_scale;
	unsigned char h_scale;
	unsigned short w_scale_size;
	unsigned short h_scale_size;
} screen_scale;
SDL_Rect screen_position;
