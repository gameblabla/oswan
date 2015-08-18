#include "drawing.h"

#ifdef GCW_LOWERRES
void gcw_screen_draw(void)
{
	unsigned short *buffer_scr = (unsigned short *) actualScreen->pixels;
	unsigned int W,H,ix,iy,x,y, xfp,yfp;
	static char buffer[32];
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
	
	if (GameConf.m_DisplayFPS) 
	{
		sprintf(buffer,"%02d",FPS);
		print_string_video(xfp,yfp,buffer);
	}
}
#endif

#if !defined(GCW_LOWERRES)
void screen_draw(void)
{
	unsigned short *buffer_scr = (unsigned short *) actualScreen->pixels;
	unsigned int W,H,ix,iy,x,y, xfp,yfp;
	static char buffer[32];
	if (GameConf.m_ScreenRatio)  // Fullscreen
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
#endif
