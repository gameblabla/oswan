#ifdef SCALING

/*  

  SDL_rotozoom.c - rotozoomer for 32bit or 8bit surfaces

  LGPL (c) A. Schiffler

*/

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "SDL_rotozoom.h"

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

/* 
 
 32bit Zoomer with optional anti-aliasing by bilinear interpolation.

 Zoomes 32bit RGBA/ABGR 'src' surface to 'dst' surface.
 
*/

int zoomSurfaceRGBA(SDL_Surface * src, SDL_Surface * dst, int flipx, int flipy)
{
    int x, y, sx, sy, *sax, *say, *csax, *csay, csx, csy, ex, ey, t1, t2, sstep;
    tColorRGBA *c00, *c01, *c10, *c11;
    tColorRGBA *sp, *csp, *dp;
    int dgap;

    /*
     * Variable setup 
     */
	sx = (int) (65536.0 * (float) src->w / (float) dst->w);
	sy = (int) (65536.0 * (float) src->h / (float) dst->h);

    /*
     * Allocate memory for row increments 
     */
    if ((sax = (int *) malloc((dst->w + 1) * sizeof(Uint32))) == NULL) {
	return (-1);
    }
    if ((say = (int *) malloc((dst->h + 1) * sizeof(Uint32))) == NULL) {
	free(sax);
	return (-1);
    }

    /*
     * Precalculate row increments 
     */
    sp = csp = (tColorRGBA *) src->pixels;
    dp = (tColorRGBA *) dst->pixels;

    if (flipx) csp += (src->w-1);
    if (flipy) csp  = (tColorRGBA*)( (Uint8*)csp + src->pitch*(src->h-1) );

    csx = 0;
    csax = sax;
    for (x = 0; x <= dst->w; x++) {
	*csax = csx;
	csax++;
	csx &= 0xffff;
	csx += sx;
    }
    csy = 0;
    csay = say;
    for (y = 0; y <= dst->h; y++) {
	*csay = csy;
	csay++;
	csy &= 0xffff;
	csy += sy;
    }

    dgap = dst->pitch - dst->w * 4;

    /*
     * Switch between interpolating and non-interpolating code 
     */

	/*
	 * Non-Interpolating Zoom 
	 */

	csay = say;
	for (y = 0; y < dst->h; y++) {
	    sp = csp;
	    csax = sax;
	    for (x = 0; x < dst->w; x++) {
		/*
		 * Draw 
		 */
		*dp = *sp;
		/*
		 * Advance source pointers 
		 */
		csax++;
		sstep = (*csax >> 16);
		if (flipx) sstep = -sstep;
		sp += sstep;
		/*
		 * Advance destination pointer 
		 */
		dp++;
	    }
	    /*
	     * Advance source pointer 
	     */
	    csay++;
	    sstep = (*csay >> 16) * src->pitch;
	    if (flipy) sstep = -sstep;
	    csp = (tColorRGBA *) ((Uint8 *) csp + sstep);

	    /*
	     * Advance destination pointers 
	     */
	    dp = (tColorRGBA *) ((Uint8 *) dp + dgap);

    }

    /*
     * Remove temp arrays 
     */
    free(sax);
    free(say);

    return (0);
}

/* 
 
 32bit Rotozoomer with optional anti-aliasing by bilinear interpolation.

 Rotates and zoomes 32bit RGBA/ABGR 'src' surface to 'dst' surface.
 
*/

void transformSurfaceRGBA(SDL_Surface * src, SDL_Surface * dst, int cx, int cy, int isin, int icos, int flipx, int flipy)
{
    int x, y, t1, t2, dx, dy, xd, yd, sdx, sdy, ax, ay, ex, ey, sw, sh;
    tColorRGBA c00, c01, c10, c11;
    tColorRGBA *pc, *sp;
    int gap;

    /*
     * Variable setup 
     */
    xd = ((src->w - dst->w) << 15);
    yd = ((src->h - dst->h) << 15);
    ax = (cx << 16) - (icos * cx);
    ay = (cy << 16) - (isin * cx);
    sw = src->w - 1;
    sh = src->h - 1;
    pc = dst->pixels;
    gap = dst->pitch - dst->w * 4;


	for (y = 0; y < dst->h; y++) {
	    dy = cy - y;
	    sdx = (ax + (isin * dy)) + xd;
	    sdy = (ay - (icos * dy)) + yd;
	    for (x = 0; x < dst->w; x++) {
		dx = (short) (sdx >> 16);
		dy = (short) (sdy >> 16);
		if (flipx) dx = (src->w-1)-dx;
		if (flipy) dy = (src->h-1)-dy;
		if ((dx >= 0) && (dy >= 0) && (dx < src->w) && (dy < src->h)) {
		    sp = (tColorRGBA *) ((Uint8 *) src->pixels + src->pitch * dy);
		    sp += dx;
		    *pc = *sp;
		}
		sdx += icos;
		sdy += isin;
		pc++;
	    }
	    pc = (tColorRGBA *) ((Uint8 *) pc + gap);
	}
}

/* 
 
 rotozoomSurface()

 Rotates and zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'angle' is the rotation in degrees. 'zoom' a scaling factor. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.

*/

#define VALUE_LIMIT	0.001


/* Local rotozoom-size function with trig result return */

void rotozoomSurfaceSizeTrig(int width, int height, double angle, double zoomx, double zoomy, int *dstwidth, int *dstheight, 
			     double *canglezoom, double *sanglezoom)
{
    double x, y, cx, cy, sx, sy;
    double radangle;
    int dstwidthhalf, dstheighthalf;

    /*
     * Determine destination width and height by rotating a centered source box 
     */
    radangle = 0;
    *sanglezoom = 0;
    *canglezoom = 0;
    *sanglezoom *= zoomx;
    *canglezoom *= zoomx;
    x = width / 2;
    y = height / 2;
    cx = *canglezoom * x;
    cy = *canglezoom * y;
    sx = *sanglezoom * x;
    sy = *sanglezoom * y;
    dstwidthhalf = 0;
    dstheighthalf = 0;
    *dstwidth = 2 * dstwidthhalf;
    *dstheight = 2 * dstheighthalf;
}


/* Publically available rotozoom-size function */

void rotozoomSurfaceSizeXY(int width, int height, double angle, double zoomx, double zoomy, int *dstwidth, int *dstheight)
{
    double dummy_sanglezoom, dummy_canglezoom;

    rotozoomSurfaceSizeTrig(width, height, angle, zoomx, zoomy, dstwidth, dstheight, &dummy_sanglezoom, &dummy_canglezoom);
}

/* Publically available rotozoom-size function */

void rotozoomSurfaceSize(int width, int height, double angle, double zoom, int *dstwidth, int *dstheight)
{
    double dummy_sanglezoom, dummy_canglezoom;

    rotozoomSurfaceSizeTrig(width, height, angle, zoom, zoom, dstwidth, dstheight, &dummy_sanglezoom, &dummy_canglezoom);
}

/* Publically available rotozoom function */

SDL_Surface *rotozoomSurface(SDL_Surface * src, double angle, double zoom)
{
  return rotozoomSurfaceXY(src, angle, zoom, zoom);
}

/* Publically available rotozoom function */

SDL_Surface *rotozoomSurfaceXY(SDL_Surface * src, double angle, double zoomx, double zoomy)
{
    SDL_Surface *rz_src;
    SDL_Surface *rz_dst;
    double zoominv;
    double sanglezoom, canglezoom, sanglezoominv, canglezoominv;
    int dstwidthhalf, dstwidth, dstheighthalf, dstheight;
    int is32bit;
    int i, src_converted;
    int flipx,flipy;

    /*
     * Sanity check 
     */
    if (src == NULL)
	return (NULL);

    /*
     * Determine if source surface is 32bit or 8bit 
     */
    is32bit = (src->format->BitsPerPixel == 32);
	/*
	 * Use source surface 'as is' 
	 */
	rz_src = src;
	src_converted = 0;

    /*
     * Sanity check zoom factor 
     */
    flipx = (zoomx<0);
    if (flipx) zoomx=-zoomx;
    flipy = (zoomy<0);
    if (flipy) zoomy=-zoomy;
    if (zoomx < VALUE_LIMIT) zoomx = VALUE_LIMIT;
    if (zoomy < VALUE_LIMIT) zoomy = VALUE_LIMIT;
    zoominv = 65536.0 / (zoomx * zoomx);

    /*
     * Check if we have a rotozoom or just a zoom 
     */
    if (fabs(angle) > VALUE_LIMIT) {

	/*
	 * Angle!=0: full rotozoom 
	 */
	/*
	 * ----------------------- 
	 */

	/* Determine target size */
	rotozoomSurfaceSizeTrig(rz_src->w, rz_src->h, angle, zoomx, zoomy, &dstwidth, &dstheight, &canglezoom, &sanglezoom);

	/*
	 * Calculate target factors from sin/cos and zoom 
	 */
	sanglezoominv = sanglezoom;
	canglezoominv = canglezoom;
	sanglezoominv *= zoominv;
	canglezoominv *= zoominv;

	/* Calculate half size */
	dstwidthhalf = dstwidth / 2;
	dstheighthalf = dstheight / 2;

	/*
	 * Alloc space to completely contain the rotated surface 
	 */
	rz_dst = NULL;
	    /*
	     * Target surface is 32bit with source RGBA/ABGR ordering 
	     */
	    rz_dst =
		SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight, 32,
				     rz_src->format->Rmask, rz_src->format->Gmask,
				     rz_src->format->Bmask, rz_src->format->Amask);
	/*
	 * Lock source surface 
	 */
	SDL_LockSurface(rz_src);
	/*
	 * Check which kind of surface we have 
	 */
	    /*
	     * Call the 32bit transformation routine to do the rotation (using alpha) 
	     */
	    transformSurfaceRGBA(rz_src, rz_dst, dstwidthhalf, dstheighthalf,
				 (int) (sanglezoominv), (int) (canglezoominv), 
				 flipx, flipy);
				 
	/*
	 * Unlock source surface 
	 */
	SDL_UnlockSurface(rz_src);

    } else {

	/*
	 * Angle=0: Just a zoom 
	 */
	/*
	 * -------------------- 
	 */

	/*
	 * Calculate target size
	 */
	zoomSurfaceSize(rz_src->w, rz_src->h, zoomx, zoomy, &dstwidth, &dstheight);

	/*
	 * Alloc space to completely contain the zoomed surface 
	 */
	rz_dst = NULL;
	if (is32bit) {
	    /*
	     * Target surface is 32bit with source RGBA/ABGR ordering 
	     */
	    rz_dst =
		SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight, 32,
				     rz_src->format->Rmask, rz_src->format->Gmask,
				     rz_src->format->Bmask, rz_src->format->Amask);
	} else {
	    /*
	     * Target surface is 8bit 
	     */
	    rz_dst = SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0, 0, 0);
	}

	/*
	 * Lock source surface 
	 */
	SDL_LockSurface(rz_src);
	/*
	 * Check which kind of surface we have 
	 */
	    /*
	     * Call the 32bit transformation routine to do the zooming (using alpha) 
	     */
	    zoomSurfaceRGBA(rz_src, rz_dst, flipx, flipy);
	    /*
	     * Turn on source-alpha support 
	     */
	    /*SDL_SetAlpha(rz_dst, SDL_SRCALPHA, 255);*/
	/*
	 * Unlock source surface 
	 */
	SDL_UnlockSurface(rz_src);
    }

    /*
     * Cleanup temp surface 
     */
    if (src_converted) {
	SDL_FreeSurface(rz_src);
    }

    /*
     * Return destination surface 
     */
    return (rz_dst);
}

/* 
 
 zoomSurface()

 Zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.

*/

#define VALUE_LIMIT	0.001

void zoomSurfaceSize(int width, int height, double zoomx, double zoomy, int *dstwidth, int *dstheight)
{
    /*
     * Sanity check zoom factors 
     */
    if (zoomx < VALUE_LIMIT) {
	zoomx = VALUE_LIMIT;
    }
    if (zoomy < VALUE_LIMIT) {
	zoomy = VALUE_LIMIT;
    }

    /*
     * Calculate target size 
     */
    *dstwidth = (int) ((double) width * zoomx);
    *dstheight = (int) ((double) height * zoomy);
    if (*dstwidth < 1) {
	*dstwidth = 1;
    }
    if (*dstheight < 1) {
	*dstheight = 1;
    }
}

SDL_Surface *zoomSurface(SDL_Surface * src, double zoomx, double zoomy)
{
    SDL_Surface *rz_src;
    SDL_Surface *rz_dst;
    int dstwidth, dstheight;
    int is32bit;
    int i, src_converted;
    int flipx, flipy;

    /*
     * Sanity check 
     */
    if (src == NULL)
	return (NULL);

    /*
     * Determine if source surface is 32bit or 8bit 
     */
    is32bit = (src->format->BitsPerPixel == 32);
    if ((is32bit) || (src->format->BitsPerPixel == 8)) {
	/*
	 * Use source surface 'as is' 
	 */
	rz_src = src;
	src_converted = 0;
    } else {
	/*
	 * New source surface is 32bit with a defined RGBA ordering 
	 */
	rz_src =
	    SDL_CreateRGBSurface(SDL_SWSURFACE, src->w, src->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_BlitSurface(src, NULL, rz_src, NULL);
	src_converted = 1;
	is32bit = 1;
    }

    flipx = (zoomx<0);
    if (flipx) zoomx = -zoomx;
    flipy = (zoomy<0);
    if (flipy) zoomy = -zoomy;

    /* Get size if target */
    zoomSurfaceSize(rz_src->w, rz_src->h, zoomx, zoomy, &dstwidth, &dstheight);

    /*
     * Alloc space to completely contain the zoomed surface 
     */
    rz_dst = NULL;
    if (is32bit) {
	/*
	 * Target surface is 32bit with source RGBA/ABGR ordering 
	 */
	rz_dst =
	    SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight, 32,
				 rz_src->format->Rmask, rz_src->format->Gmask,
				 rz_src->format->Bmask, rz_src->format->Amask);
    } else {
	/*
	 * Target surface is 8bit 
	 */
	rz_dst = SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight, 8, 0, 0, 0, 0);
    }

    /*
     * Lock source surface 
     */
    SDL_LockSurface(rz_src);
    /*
     * Check which kind of surface we have 
     */
	/*
	 * Call the 32bit transformation routine to do the zooming (using alpha) 
	 */
	zoomSurfaceRGBA(rz_src, rz_dst, flipx, flipy);
    /*
     * Unlock source surface 
     */
    SDL_UnlockSurface(rz_src);

    /*
     * Cleanup temp surface 
     */
    if (src_converted) {
	SDL_FreeSurface(rz_src);
    }

    /*
     * Return destination surface 
     */
    return (rz_dst);
}

#endif
