#include "shared.h"

inline void screen_draw(void);
inline void Get_resolution(void);
inline void Set_resolution(unsigned short w, unsigned short h);
inline void SetVideo(unsigned char mode);
inline void Set_DrawRegion(void);
inline void take_screenshot(void);

extern unsigned char FPS;
extern unsigned char HVMode;

struct wheretodraw
{
	unsigned short w;
	unsigned short h;
	unsigned short offset_x;
	unsigned short offset_y;
} screen_to_draw_region;

#if !defined(SCALING)
	#define flip_screen SDL_Flip
#else
	void flip_screen(SDL_Surface* screen);
	SDL_Surface *real_screen;
#endif

struct scaling
{
	unsigned short w_display;
	unsigned short h_display;
	float w_scale;
	float h_scale;
	float w_scale_size;
	float h_scale_size;
} screen_scale;
SDL_Rect screen_position;
