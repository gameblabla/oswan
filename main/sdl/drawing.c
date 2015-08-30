#ifdef SCALING
#include <SDL/SDL.h>
#include "gfx/SDL_rotozoom.h"
#ifdef SYLLABLE
#include <gui/desktop.h>
#endif
#endif

#include "drawing.h"

void Get_resolution(void)
{
#ifdef SCALING
	#ifdef SYLLABLE
	os::Desktop cDesktop;
	os::IPoint point = cDesktop.GetResolution();
	screen_scale.w_display = point.x;
	screen_scale.h_display = point.y;
	#else
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	screen_scale.w_display = info->current_w;
	screen_scale.h_display = info->current_h;
	#endif
#else
	screen_scale.w_display = 320;
	screen_scale.h_display = 240;
#endif
}

void Set_resolution(unsigned short w, unsigned short h)
{
#ifdef SCALING
	screen_scale.w_scale = screen_scale.w_display / w;
	screen_scale.h_scale = screen_scale.h_display / h;
  
	screen_scale.w_scale_size = screen_scale.w_scale * w;
	screen_scale.h_scale_size = screen_scale.h_scale * h;
	
	screen_position.x = (screen_scale.w_display - screen_scale.w_scale_size)/2;
	screen_position.y = (screen_scale.h_display - screen_scale.h_scale_size)/2;	
#else
	screen_scale.w_display = w;
	screen_scale.h_display = h;
#endif

}

void SetVideo(unsigned char mode)
{
#ifdef SCALING	
	int flags = FLAG_VIDEO | SDL_NOFRAME | SDL_FULLSCREEN;
#else
	int flags = FLAG_VIDEO;
#endif
	unsigned short w = 320, h = 240;
	
	if (mode == 1) 
	{
		w = 224;
		h = 144;
	}
	
	#if defined(SCALING)
		if (real_screen) SDL_FreeSurface(real_screen);
		if (actualScreen) SDL_FreeSurface(actualScreen);
	#else
		if (actualScreen) SDL_FreeSurface(actualScreen);
	#endif
	
	#if !defined(NOSCREENSHOTS)
		if (screenshots) SDL_FreeSurface(screenshots);
	#endif
	
	Set_resolution(w, h);

	#if defined(SCALING)
		real_screen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
		actualScreen = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, BITDEPTH_OSWAN, 0,0,0,0);
	#else
		actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
	#endif
	
	#if !defined(NOSCREENSHOTS)
		screenshots = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, BITDEPTH_OSWAN, 0,0,0,0);
	#endif
}

void Set_DrawRegion(void)
{
	/* Clear screen too to avoid graphical glitches */
	SDL_FillRect(actualScreen, NULL, 0);

	if (GameConf.m_ScreenRatio == 2)
	{
		screen_to_draw_region.w	= 320;
		screen_to_draw_region.h	= 204;
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 18; 
	}
	else if (GameConf.m_ScreenRatio == 1)
	{
		screen_to_draw_region.w	= 320;
		screen_to_draw_region.h	= 240;
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 0; 
	}
	else if (GameConf.m_ScreenRatio == 0)
	{
		screen_to_draw_region.w	= 224;
		screen_to_draw_region.h	= 144;
#ifdef SWITCHING_GRAPHICS
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 0; 
#else
		screen_to_draw_region.offset_x = ((actualScreen->w - SYSVID_WIDTH)/2);
		screen_to_draw_region.offset_y = ((actualScreen->h - SYSVID_HEIGHT)/2); 
#endif
	}
}

void screen_draw(void)
{
	unsigned short *buffer_scr = (unsigned short *) actualScreen->pixels;
	unsigned int W,H,ix,iy,x,y;
	
	x=screen_to_draw_region.offset_x;
	y=screen_to_draw_region.offset_y; 
	W=screen_to_draw_region.w;
	H=screen_to_draw_region.h;
	ix=(SYSVID_WIDTH<<16)/W;
	iy=(SYSVID_HEIGHT<<16)/H;
	
	buffer_scr += (y)*320;
	buffer_scr += (x);
	do   
	{
		unsigned short *buffer_mem=(unsigned short *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
		W=screen_to_draw_region.w; x=0;
		do 
		{
			*buffer_scr++=buffer_mem[x>>16];
#if BITDEPTH_OSWAN == 32
			*buffer_scr++=buffer_mem[x>>16];
#endif
			x+=ix;
		} while (--W);
		y+=iy;
#ifndef SWITCHING_GRAPHICS
		if (screen_to_draw_region.w == 224) buffer_scr += actualScreen->pitch - 320 - SYSVID_WIDTH;
#endif
	} while (--H);
	
	static char buffer[4];
	if (GameConf.m_DisplayFPS) 
	{
#ifndef SWITCHING_GRAPHICS
		if (GameConf.m_ScreenRatio == 2 || GameConf.m_ScreenRatio == 0)
#else
		if (GameConf.m_ScreenRatio == 2)
#endif
		{
			SDL_Rect pos;
			pos.x = 0;
			pos.y = 0;
			pos.w = 17;
			pos.h = 16;
			SDL_FillRect(actualScreen, &pos, 0);
		}
		sprintf(buffer,"%d",FPS);
		print_string_video(2,2,buffer);
	}
}

#if defined(SCALING)
void flip_screen(SDL_Surface* screen)
{
	SDL_Surface* doble;
	doble = zoomSurface(screen,screen_scale.w_scale,screen_scale.h_scale);
	SDL_BlitSurface(doble,NULL,real_screen,&screen_position);
	SDL_Flip(real_screen);
	SDL_FreeSurface(doble);
}
#endif

void take_screenshot(void)
{
#if !defined(NOSCREENSHOTS)
	// Save current screen in screenshots's layer
	SDL_BlitSurface(actualScreen, NULL, screenshots, NULL);
#endif
}
