#ifndef DRAWING_H
#define DRAWING_H

#include "shared.h"

extern void screen_draw(void);
extern void Get_resolution(void);
extern void Set_resolution(uint16_t w, uint16_t h);
extern void SetVideo(uint8_t mode);
extern void Clear_Screen(void);
extern void Clear_Screen_Norefresh(void);
extern void Draw_Rect_Menu(uint32_t y, uint32_t h);
extern void Cleanup_Screen(void);
extern void Update_Screen(void);

extern SDL_Surface *actualScreen;

extern uint32_t HVMode;

#define Surface_to_Draw actualScreen->pixels

struct scaling
{
	uint32_t w_display;
	uint32_t h_display;
	float w_scale;
	float h_scale;
	float w_scale_size;
	float h_scale_size;
};

extern struct scaling screen_scale;

#endif
