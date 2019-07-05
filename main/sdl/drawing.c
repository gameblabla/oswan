#include "drawing.h"
#include "menu.h"
#include "scaler.h"

SDL_Surface *actualScreen, *menuscreen;
struct scaling screen_scale;

void Get_resolution(void)
{
#ifdef SCALING
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	screen_scale.w_display = info->current_w;
	screen_scale.h_display = info->current_h;
#else
	screen_scale.w_display = REAL_SCREEN_WIDTH;
	screen_scale.h_display = REAL_SCREEN_HEIGHT;
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
	uint16_t wi = REAL_SCREEN_WIDTH, he = REAL_SCREEN_HEIGHT;
	
	if (mode == 0) 
	{
		wi = 224;
		he = 144;
	}
	else
	{
		wi = REAL_SCREEN_WIDTH;
		he = REAL_SCREEN_HEIGHT;
	}
	
	if (!SDL_WasInit(SDL_INIT_VIDEO)) 
	{	
		SDL_Init(SDL_INIT_VIDEO);
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	if (actualScreen) SDL_FreeSurface(actualScreen);
	
	Set_resolution(wi, he);
	
	actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
	menuscreen = SDL_CreateRGBSurface(SDL_SWSURFACE, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, BITDEPTH_OSWAN, 0,0,0,0);
}

void Clear_Screen(void)
{
	SDL_FillRect(menuscreen, NULL, 0);
	
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#if defined(SDL_TRIPLEBUF) || defined(SDL_DOUBLEBUF)
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#endif
}

void Clear_Menu(void)
{
	SDL_FillRect(menuscreen, NULL, 0);
}


void Clear_Screen_Norefresh(void)
{
	//memset(Surface_to_Draw, 0, (screen_scale.w_display*screen_scale.h_display)*2);
	SDL_FillRect(menuscreen, NULL, 0);
	SDL_FillRect(actualScreen, NULL, 0);
}

void Draw_Rect_Menu(uint32_t y, uint32_t h)
{
	//memset(Surface_to_Draw + ((screen_scale.w_display*2)*(y)), RGB565(4,3,95), (screen_scale.w_display*h)*2);
	SDL_Rect pos;
	pos.x = 0;
	pos.y = y;
	pos.w = menuscreen->w;
	pos.h = h;
	SDL_FillRect(menuscreen, &pos, RGB565(4,3,95));
}

void Update_Screen()
{
	if (m_Flag == GF_MAINUI)
	{
		SDL_BlitSurface(menuscreen, NULL, actualScreen, NULL);
	}
	SDL_Flip(actualScreen);
}

/* It's worth noting that the virtual screen width is SCREEN_REAL_WIDTH in pixels, not 224. (Wonderswan's screen width)
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
			bitmap_scale(0,0,224,144,screen_scale.w_display,screen_scale.h_display,320,0,FrameBuffer,actualScreen->pixels);
			//bitmap_scale(0,0,224,144,screen_scale.w_display,206,SCREEN_REAL_WIDTH,0,FrameBuffer,actualScreen->pixels + (34 * screen_scale.w_display));
		break;
	}

	SDL_UnlockSurface(actualScreen);
	
	Update_Screen();
}

void Cleanup_Screen()
{
	if (actualScreen != NULL) SDL_FreeSurface(actualScreen);
	if (menuscreen != NULL) SDL_FreeSurface(menuscreen);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);	
	SDL_Quit();
}
