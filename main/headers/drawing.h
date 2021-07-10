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
extern void Clear_Menu(void);

extern SDL_Surface *actualScreen, *menuscreen;

#define Surface_to_Draw actualScreen->pixels
#define Surface_to_Draw_menu menuscreen->pixels

#define Screen_surface_size (SCREEN_REAL_WIDTH*SCREEN_REAL_HEIGHT*2)
#define Menu_surface_size (MENU_SCREEN_WIDTH*MENU_SCREEN_HEIGHT*2)

struct scaling
{
	uint32_t w_display;
	uint32_t h_display;
};

extern struct scaling screen_scale;

#endif
