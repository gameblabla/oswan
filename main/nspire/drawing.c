#include "drawing.h"
#include "n2DLib.h"

void Get_resolution(void)
{
	screen_scale.w_display = 320;
	screen_scale.h_display = 240;
}

void Set_resolution(unsigned short w, unsigned short h)
{
	screen_scale.w_display = w;
	screen_scale.h_display = h;
}

void SetVideo(unsigned char mode)
{
	initBuffering();
	clearBufferB();
	updateScreen();
}

void Set_DrawRegion(void)
{
	/* Clear screen too to avoid graphical glitches */
	clearBufferB();
	updateScreen();

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
		screen_to_draw_region.offset_x = ((320 - SYSVID_WIDTH)/2);
		screen_to_draw_region.offset_y = ((240 - SYSVID_HEIGHT)/2); 
	}
}

void screen_draw(void)
{
	unsigned short *buffer_scr = (unsigned short *)BUFF_BASE_ADDRESS;
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
		//W=screen_to_draw_region.w; 
		W=320;
		x=0;
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
	
	flip_screen(0);
}

void flip_screen(SDL_Surface* screen)
{
	updateScreen();
}

void take_screenshot(void)
{
}
