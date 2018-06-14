#include "shared.h"

void screen_draw(void);
void Get_resolution(void);
void Set_resolution(uint16_t w, uint16_t h);
void SetVideo(unsigned char mode);
void Set_DrawRegion(void);
void take_screenshot(void);

extern uint8_t FPS;
extern uint8_t HVMode;

struct wheretodraw
{
	uint16_t w;
	uint16_t h;
	uint16_t offset_x;
	uint16_t offset_y;
} screen_to_draw_region;

#define flip_screen SDL_Flip
SDL_Rect screen_position;

struct scaling
{
	uint16_t w_display;
	uint16_t h_display;
	float w_scale;
	float h_scale;
	float w_scale_size;
	float h_scale_size;
} screen_scale;
