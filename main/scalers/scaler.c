#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void bitmap_scale(uint32_t startx, uint32_t starty, uint32_t viswidth, uint32_t visheight, uint32_t newwidth, uint32_t newheight,uint32_t pitchsrc,uint32_t pitchdest, uint16_t* restrict src, uint16_t* restrict dst)
{
    uint32_t W,H,ix,iy,x,y;
    y=starty<<16;
    W=newwidth;
    H=newheight;
    ix=(viswidth<<16)/W;
    iy=(visheight<<16)/H;

    do 
    {
        uint16_t* restrict buffer_mem=&src[(y>>16)*pitchsrc];
        W=newwidth; x=startx<<16;
        do 
        {
            *dst++=buffer_mem[x>>16];
            x+=ix;
        } while (--W);
        dst+=pitchdest;
        y+=iy;
    } while (--H);
}

void rotate_90_ccw(uint16_t* restrict dst, uint16_t* restrict src)
{
    int32_t h = 224, w = 144;
    src += w * h - 1;
    for (int32_t col = w - 1; col >= 0; --col)
    {
        uint16_t *outcol = dst + col;
        for(int32_t row = 0; row < h; ++row, outcol += w)
        {
            *outcol = *src--;
		}
    }
}

#define AVERAGE16(c1, c2) (((c1) + (c2) + (((c1) ^ (c2)) & 0x0821))>>1)  //More accurate

void upscale_224x144_to_320xXXX(uint16_t* restrict dst, uint16_t* restrict src, uint32_t height)
{
    register uint_fast16_t a, b, c, d, e, f, g;
    int Eh = 0;
    int vf = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < 320/10; x++)
        {
            a = *(src+0);
            b = *(src+1);
            c = *(src+2);
            d = *(src+3);
            e = *(src+4);
            f = *(src+5);
            g = *(src+6);

            if (vf) {
                a = AVERAGE16(a, *(src+240));
                b = AVERAGE16(b, *(src+241));
                c = AVERAGE16(c, *(src+242));
                d = AVERAGE16(d, *(src+243));
                e = AVERAGE16(e, *(src+244));
                f = AVERAGE16(f, *(src+245));
                g = AVERAGE16(g, *(src+246));
            }

            *(uint32_t*)(dst+0) = a|(b<<16);
            *(uint32_t*)(dst+2) = AVERAGE16(b,c)|(c<<16);
            *(uint32_t*)(dst+4) = d|(AVERAGE16(d,e)<<16);
            *(uint32_t*)(dst+6) = e|(f<<16);
            *(uint32_t*)(dst+8) = AVERAGE16(f,g)|(g<<16);

            src+=7;
            dst+=10;

        }

        Eh += 144;
        if(Eh >= height) {
            Eh -= height;
            vf = 0;
	    src += (240 - 224);
        }
        else {
            vf = 1;
	    src -= 224;
	}
    }
}

void upscale_144x224_to_320x240_rotate(uint16_t * restrict dst, uint16_t * restrict src)	//9:14 > 20:15
{
    register uint_fast16_t a, b;
    int Eh = 0;
    int vf = 0;
    int x,y;

#define	RAB		a = *src++; b = *src++
#define RWAB		RAB; WA; WB
#define	R14_W15		RWAB; RWAB; RWAB; RWAB; RWAB; RWAB; RAB; WA; a = AVERAGE16(a,b); WA; WB
#define	R14_W15b	RWAB; RWAB; RWAB; RWAB; RWAB; RWAB; RAB; WA; a = AVERAGE16(a,b); WAb; WB

    for (x = 0; x < 144; x++)
    {
	if (vf) {	// 1(.5) to 3 lines
	    if (((uintptr_t)dst & 3) == 0) {	// alignment check
		for (y = 0; y < 224/14; y++)
		{
#define	WA	*(uint32_t*)dst = a|a<<16; *(dst+2) = AVERAGE16(a,*(src+238)); dst-=320
#define	WAb	*(uint32_t*)dst = a|a<<16; *(dst+2) = AVERAGE16(a,AVERAGE16(*(src+238),*(src+239))); dst-=320
#define	WB	*(uint32_t*)dst = b|b<<16; *(dst+2) = AVERAGE16(b,*(src+239)); dst-=320
		R14_W15b;
		}
	    } else {
		for (y = 0; y < 224/14; y++)
		{
#undef	WA
#undef	WAb
#undef	WB
#define	WA	*dst = a; *(uint32_t*)(dst+1) = a|AVERAGE16(a,*(src+238))<<16; dst-=320
#define	WAb	*dst = a; *(uint32_t*)(dst+1) = a|AVERAGE16(a,AVERAGE16(*(src+238),*(src+239)))<<16; dst-=320
#define	WB	*dst = b; *(uint32_t*)(dst+1) = b|AVERAGE16(b,*(src+239))<<16; dst-=320
		R14_W15b;
		}
	    }
	    dst += (320*240)+3;
	} else {	// 1 to 2 lines
	    if (((uintptr_t)dst & 3) == 0) {	// alignment check
		for (y = 0; y < 224/14; y++)
		{
#undef	WA
#undef	WAb
#undef	WB
#define	WA	*(uint32_t*)dst = a|a<<16; dst-=320
#define	WB	*(uint32_t*)dst = b|b<<16; dst-=320
		R14_W15;
		}
	    } else {
	        for (y = 0; y < 224/14; y++)
		{
#undef	WA
#undef	WB
#define	WA	*dst = a; *(dst+1) = a; dst-=320
#define	WB	*dst = b; *(dst+1) = b; dst-=320
		R14_W15;
		}
	    }
	    dst += (320*240)+2;
	}
	src += (240 - 224);
	Eh += 125;	// 144;
	if (Eh >= (320/2)) { Eh -= (320/2); vf = 0; } else vf = 1;
    }
#undef	WA
#undef	WB
#undef	RAB
#undef	RWAB
#undef	R14_W15
}
