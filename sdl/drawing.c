#ifdef SCALING
#include <SDL/SDL.h>
#include "gfx/SDL_rotozoom.h"
#ifdef SYLLABLE
#include <gui/desktop.h>
#endif
#endif

#include "drawing.h"

void Get_resolution(unsigned short w, unsigned short h)
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
	int flags = FLAG_VIDEO | SDL_NOFRAME;
#else
	int flags = FLAG_VIDEO;
#endif
	
	unsigned short w = 320, h = 240;
	
	if (mode == 1) 
	{
		w = 224;
		h = 144;
	}

	Get_resolution(w, h);

	#if defined(SCALING)
		if (real_screen) SDL_FreeSurface(real_screen);
		if (actualScreen) SDL_FreeSurface(actualScreen);
		
		actualScreen = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, 16, 0,0,0,0);
		real_screen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
	#else
		if (actualScreen) SDL_FreeSurface(actualScreen);
		actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, 16, flags);
	#endif

}

void screen_draw(void)
{
	unsigned short *buffer_scr = (unsigned short *) actualScreen->pixels;
	unsigned int W,H,ix,iy,x,y, xfp,yfp;
	static char buffer[32];
	
	// Fullscreen
	if (GameConf.m_ScreenRatio)
	{
		x=0;
		y=0; 
		W=320;
		H=240;
		ix=(SYSVID_WIDTH<<16)/W;
		iy=(SYSVID_HEIGHT<<16)/H;
		xfp = 300;yfp = 1;

		do   
		{
			unsigned short *buffer_mem=(unsigned short *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
			W=320; x=0;
			do {
				*buffer_scr++=buffer_mem[x>>16];
				x+=ix;
			} while (--W);
			y+=iy;
		} while (--H);
	}
	else // 1x Size
	{ 
#ifdef SWITCHING_GRAPHICS
		x=0;
		y=0; 
		W=SYSVID_WIDTH;
		H=SYSVID_HEIGHT;
		ix=(SYSVID_WIDTH<<16)/W;
		iy=(SYSVID_HEIGHT<<16)/H;
		xfp = (x+SYSVID_WIDTH)-20;yfp = y+1;
		
		buffer_scr += (y)*224;
		buffer_scr += (x);
		do   
		{
			unsigned short *buffer_mem=(unsigned short *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
			W=224; x=0;
			do 
			{
				*buffer_scr++=buffer_mem[x>>16];
				x+=ix;
			} while (--W);
			y+=iy;
			buffer_scr += actualScreen->pitch - 224 - SYSVID_WIDTH;
		} while (--H);
#else
		x=((actualScreen->w - SYSVID_WIDTH)/2);
		y=((actualScreen->h - SYSVID_HEIGHT)/2); 
		W=SYSVID_WIDTH;
		H=SYSVID_HEIGHT;
		ix=(SYSVID_WIDTH<<16)/W;
		iy=(SYSVID_HEIGHT<<16)/H;
		xfp = (x+SYSVID_WIDTH)-20;yfp = y+1;
		
		buffer_scr += (y)*320;
		buffer_scr += (x);
		do   
		{
			unsigned short *buffer_mem=(unsigned short *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
			W=SYSVID_WIDTH; x=((actualScreen->w - SYSVID_WIDTH)/2);
			do 
			{
				*buffer_scr++=buffer_mem[x>>16];
				x+=ix;
			} while (--W);
			y+=iy;
			buffer_scr += actualScreen->pitch - 320 - SYSVID_WIDTH;
		} while (--H);
#endif
	}
	
	if (GameConf.m_DisplayFPS) 
	{
		sprintf(buffer,"%02d",FPS);
		print_string_video(xfp,yfp,buffer);
	}
}

#if defined(SCALING)
void flip_screen(SDL_Surface* screen)
{
	SDL_Surface* doble;
	doble = zoomSurface(screen,screen_scale.w_scale,screen_scale.h_scale,0);
	SDL_BlitSurface(doble,NULL,real_screen,&screen_position);
	SDL_Flip(real_screen);
	SDL_FreeSurface(doble);
}
#endif

void take_screenshot(void)
{
#if !defined(NOSCREENSHOTS)
	// Save current screen in screenshots's layer
	SDL_BlitSurface(actualScreen, 0, screenshots, 0);
#endif
}
