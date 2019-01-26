#include "drawing.h"
#include "shared.h"

void Get_resolution(void)
{
}

void Set_resolution(uint16_t  w, uint16_t  h)
{
}

void SetVideo(unsigned char mode)
{
	if (!SDL_WasInit(SDL_INIT_VIDEO)) 
	{	
		SDL_Init(SDL_INIT_VIDEO);
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	if (actualScreen) SDL_FreeSurface(actualScreen);
	
	#if !defined(NOSCREENSHOTS)
		if (screenshots) SDL_FreeSurface(screenshots);
	#endif
	
	Set_resolution(0, 0);

	actualScreen = SDL_SetVideoMode(480, 272, 16, SDL_HWSURFACE);
	
	#if !defined(NOSCREENSHOTS)
		screenshots = SDL_CreateRGBSurface(FLAG_VIDEO, 480, 272, BITDEPTH_OSWAN, 0,0,0,0);
	#endif
}

void Set_DrawRegion(void)
{
	/* Clear screen too to avoid graphical glitches */
	SDL_FillRect(actualScreen, NULL, 0);
}

void screen_draw(void)
{
	uint16_t  *buffer_scr = (uint16_t  *) actualScreen->pixels;
	uint32_t  W,H,x,y,iy,ix;
	
	SDL_LockSurface(actualScreen);
	
	x=0;
	y=0; 
	W=480;
	H=272;
	ix=(SYSVID_WIDTH<<16)/W;
	iy=(SYSVID_HEIGHT<<16)/H;
	do   
	{
		uint16_t  *buffer_mem=(uint16_t  *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
		W=480; x=0;
		do 
		{
			*buffer_scr++=buffer_mem[x>>16];
			x+=ix;
		} while (--W);
		y+=iy;
	} while (--H);
	
	static char buffer[3];
	if (GameConf.m_DisplayFPS) 
	{
		sprintf(buffer,"%d",FPS);
		print_string_video(2,2,buffer);
	}
	
	SDL_UnlockSurface(actualScreen);
	flip_screen(actualScreen);
}

void take_screenshot(void)
{
#if !defined(NOSCREENSHOTS)
	/* Save current screen in screenshots's layer	*/
	SDL_BlitSurface(actualScreen, NULL, screenshots, NULL);
#endif
}
