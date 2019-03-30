#include "drawing.h"
#include "menu.h"
#include "scaler.h"

SDL_Surface *actualScreen;
struct scaling screen_scale;

void Get_resolution(void)
{
#ifdef SCALING
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	screen_scale.w_display = info->current_w;
	screen_scale.h_display = info->current_h;
#else
	screen_scale.w_display = 320;
	screen_scale.h_display = 240;
#endif
}

void Set_resolution(uint16_t w, uint16_t h)
{
	screen_scale.w_display = w;
	screen_scale.h_display = h;
}

void SetVideo(uint8_t mode)
{
	int32_t flags = FLAG_VIDEO;
	uint16_t w = 320, h = 240;
	
	if (mode == 1) 
	{
		w = 224;
		h = 144;
	}
	
	if (!SDL_WasInit(SDL_INIT_VIDEO)) 
	{	
		SDL_Init(SDL_INIT_VIDEO);
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	if (actualScreen) SDL_FreeSurface(actualScreen);
	
	Set_resolution(w, h);
	
	actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
}

void Clear_Screen(void)
{
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#if defined(SDL_TRIPLEBUF) || defined(SDL_DOUBLEBUF)
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#endif
}

void Clear_Screen_Norefresh(void)
{
	//memset(Surface_to_Draw, 0, (screen_scale.w_display*screen_scale.h_display)*2);
	SDL_FillRect(actualScreen, NULL, 0);
}

void Draw_Rect_Menu(uint32_t y, uint32_t h)
{
	//memset(Surface_to_Draw + ((screen_scale.w_display*2)*(y)), RGB565(4,3,95), (screen_scale.w_display*h)*2);
	SDL_Rect pos;
	pos.x = 0;
	pos.y = y;
	pos.w = screen_scale.w_display;
	pos.h = h;
	SDL_FillRect(actualScreen, &pos, RGB565(4,3,95));
}

void Update_Screen()
{
	SDL_Flip(actualScreen);
}

/* It's worth noting that the virtual screen width is 320 in pixels, not 224. (Wonderswan's screen width)
 * Thus, it must be taken in account accordingly. */
void screen_draw(void)
{
	uint32_t xfp, yfp;
	
	SDL_LockSurface(actualScreen);
	
	switch(menu_oswan.scaling)
	{
		/* Thanks Pingflood. Unscaled crap */
		case 0:
			xfp = (actualScreen->w - 224) / 2;
			yfp = (actualScreen->h - 144) / 2;

			uint16_t *d = (uint16_t*)actualScreen->pixels + xfp + yfp * actualScreen->pitch / 2 ;
			uint16_t *s = (uint16_t*)FrameBuffer;
			for (uint32_t y = 0; y < 144; y++)
			{
				/* Only copy the first 224 pixels, pixels after are not supposed to be shown */
				memmove(d, s, 224 * sizeof(uint16_t));
				s += 320;
				d += actualScreen->w;
			}
		break;
		/* Fullscreen */
		case 1:
			bitmap_scale(0,0,224,144,screen_scale.w_display,screen_scale.h_display,320,0,FrameBuffer,actualScreen->pixels);
		break;
		/* Keep Aspect */
		case 2:
			bitmap_scale(0,0,224,144,screen_scale.w_display,206,320,0,FrameBuffer,actualScreen->pixels + (34 * screen_scale.w_display));
		break;
	}

	SDL_UnlockSurface(actualScreen);
	
	Update_Screen();
}

void Cleanup_Screen()
{
	if (actualScreen != NULL) SDL_FreeSurface(actualScreen);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);	
	SDL_Quit();
}
