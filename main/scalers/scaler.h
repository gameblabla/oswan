#ifndef SCALERS_H
#define SCALERS_H
#include <stdint.h>
extern void bitmap_scale(uint32_t startx, uint32_t starty, uint32_t viswidth, uint32_t visheight, uint32_t newwidth, uint32_t newheight,uint32_t pitchsrc,uint32_t pitchdest, uint16_t* restrict src, uint16_t* restrict dst);

extern void rotate_90_ccw(uint16_t* restrict dst, uint16_t* restrict src);
extern void upscale_224x144_to_320xXXX(uint16_t* restrict dst, uint16_t* restrict src, uint32_t height);
extern void upscale_144x224_to_320x240_rotate(uint16_t * restrict dst, uint16_t * restrict src);

#endif
