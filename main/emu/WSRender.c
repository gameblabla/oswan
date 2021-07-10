#include "WSRender.h"
#include "WS.h"
#include "shared.h"
#include "cpu/necintrf.h"

#define MAP_TILE 0x01FF
#define MAP_PAL  0x1E00
#define MAP_BANK 0x2000
#define MAP_HREV 0x4000
#define MAP_VREV 0x8000
uint8_t *Scr1TMap;
uint8_t *Scr2TMap;

#define SPR_TILE 0x01FF
#define SPR_PAL  0x0E00
#define SPR_CLIP 0x1000
#define SPR_LAYR 0x2000
#define SPR_HREV 0x4000
#define SPR_VREV 0x8000
uint8_t *SprTTMap;
uint8_t *SprETMap;
uint8_t SprTMap[512];

uint16_t Palette[16][16];
uint16_t MonoColor[8];

#ifdef NOSDL_FB
uint16_t FrameBuffer[240*144];
#else
uint16_t* FrameBuffer;
#endif

const uint8_t Layer[3] = {1, 1, 1};

uint8_t *pbTData;
uint32_t Index0[2];			// 32bit access to Index
uint8_t *Index = (uint8_t *)Index0;	// Index[8]
int32_t TMap;

void SetPalette(const uint32_t addr)
{
    uint16_t color, r, g, b;
    uint16_t pal;

    /* RGB444 format */
    color = *(uint16_t*)(IRAM + (addr & 0xFFFE));

	/* RGB565 */
	r = ((color & 0x0F00) << 4);
	g = ((color & 0x00F0) << 3);
	b = ((color & 0x000F) << 1);
	pal = (r) | (g) | (b);

	Palette[(addr & 0x1E0)>>5][(addr & 0x1E) >> 1] = pal;
}

static inline void MakeIndex(void)
{
	register uint_fast8_t pbTData0 = pbTData[0];
	register uint_fast8_t pbTData1 = pbTData[1];
	register uint_fast8_t pbTData2;
	register uint_fast8_t pbTData3;

	switch ( ((IO[COLCTL] & 0x60)>>5) | ((TMap & SPR_HREV)>>12) )
	{
	case 0:		// 4 Colors
		Index0[0] = (( pbTData0>>7   )|((pbTData1>>6)&2))	|
			    (((pbTData0>>6)&1)|((pbTData1>>5)&2))<<8	|
			    (((pbTData0>>5)&1)|((pbTData1>>4)&2))<<16	|
			    (((pbTData0>>4)&1)|((pbTData1>>3)&2))<<24	;
		Index0[1] = (((pbTData0>>3)&1)|((pbTData1>>2)&2))	|
			    (((pbTData0>>2)&1)|((pbTData1>>1)&2))<<8	|
			    (((pbTData0>>1)&1)|( pbTData1    &2))<<16	|
			    (( pbTData0    &1)|((pbTData1<<1)&2))<<24	;
		break;
	case 1:		// 4 Colors	Packed Mode
		Index0[0] =  (pbTData0>>6   )		|
			    ((pbTData0>>4)&3)<<8	|
			    ((pbTData0>>2)&3)<<16	|
			     (pbTData0    &3)<<24	;
		Index0[1] =  (pbTData1>>6   )		|
			    ((pbTData1>>4)&3)<<8	|
			    ((pbTData1>>2)&3)<<16	|
			     (pbTData1    &3)<<24	;
		break;
	case 2:		// 16 Colors
		pbTData2 = pbTData[2];
		pbTData3 = pbTData[3];
		Index0[0] = (( pbTData0>>7   )|((pbTData1>>6)&2)|((pbTData2>>5)&4)|((pbTData3>>4)&8))		|
			    (((pbTData0>>6)&1)|((pbTData1>>5)&2)|((pbTData2>>4)&4)|((pbTData3>>3)&8))<<8	|
			    (((pbTData0>>5)&1)|((pbTData1>>4)&2)|((pbTData2>>3)&4)|((pbTData3>>2)&8))<<16	|
			    (((pbTData0>>4)&1)|((pbTData1>>3)&2)|((pbTData2>>2)&4)|((pbTData3>>1)&8))<<24	;
		Index0[1] = (((pbTData0>>3)&1)|((pbTData1>>2)&2)|((pbTData2>>1)&4)|( pbTData3    &8))		|
			    (((pbTData0>>2)&1)|((pbTData1>>1)&2)|( pbTData2    &4)|((pbTData3<<1)&8))<<8	|
			    (((pbTData0>>1)&1)|( pbTData1    &2)|((pbTData2<<1)&4)|((pbTData3<<2)&8))<<16	|
			    (( pbTData0    &1)|((pbTData1<<1)&2)|((pbTData2<<2)&4)|((pbTData3<<3)&8))<<24	;
		break;
	case 3:		// 16 Colors	Packed Mode
		pbTData2 = pbTData[2];
		pbTData3 = pbTData[3];
		Index0[0] = (pbTData0>>4)		|
			    (pbTData0 & 0x0F)<<8	|
			    (pbTData1>>4)<<16		|
			    (pbTData1 & 0x0F)<<24	;
		Index0[1] = (pbTData2>>4)		|
			    (pbTData2 & 0x0F)<<8	|
			    (pbTData3>>4)<<16		|
			    (pbTData3 & 0x0F)<<24	;
		break;
	case 4:		// 4 Colors			SPR_HREV
		Index0[0] = (( pbTData0    &1)|((pbTData1<<1)&2))	|
			    (((pbTData0>>1)&1)|( pbTData1    &2))<<8	|
			    (((pbTData0>>2)&1)|((pbTData1>>1)&2))<<16	|
			    (((pbTData0>>3)&1)|((pbTData1>>2)&2))<<24	;
		Index0[1] = (((pbTData0>>4)&1)|((pbTData1>>3)&2))	|
			    (((pbTData0>>5)&1)|((pbTData1>>4)&2))<<8	|
			    (((pbTData0>>6)&1)|((pbTData1>>5)&2))<<16	|
			    (( pbTData0>>7   )|((pbTData1>>6)&2))<<24	;
		break;
	case 5:		// 4 Colors	Packed Mode	SPR_HREV
		Index0[0] = ( pbTData1    &3)		|
			    ((pbTData1>>2)&3)<<8	|
			    ((pbTData1>>4)&3)<<16	|
			    ( pbTData1>>6   )<<24	;
		Index0[1] = ( pbTData0    &3)		|
			    ((pbTData0>>2)&3)<<8	|
			    ((pbTData0>>4)&3)<<16	|
			    ( pbTData0>>6   )<<24	;
		break;
	case 6:		// 16 Colors 			SPR_HREV
		pbTData2 = pbTData[2];
		pbTData3 = pbTData[3];
		Index0[0] = (( pbTData0    &1)|((pbTData1<<1)&2)|((pbTData2<<2)&4)|((pbTData3<<3)&8))		|
			    (((pbTData0>>1)&1)|( pbTData1    &2)|((pbTData2<<1)&4)|((pbTData3<<2)&8))<<8	|
			    (((pbTData0>>2)&1)|((pbTData1>>1)&2)|( pbTData2    &4)|((pbTData3<<1)&8))<<16	|
			    (((pbTData0>>3)&1)|((pbTData1>>2)&2)|((pbTData2>>1)&4)|( pbTData3    &8))<<24	;
		Index0[1] = (((pbTData0>>4)&1)|((pbTData1>>3)&2)|((pbTData2>>2)&4)|((pbTData3>>1)&8))		|
			    (((pbTData0>>5)&1)|((pbTData1>>4)&2)|((pbTData2>>3)&4)|((pbTData3>>2)&8))<<8	|
			    (((pbTData0>>6)&1)|((pbTData1>>5)&2)|((pbTData2>>4)&4)|((pbTData3>>3)&8))<<16	|
			    (( pbTData0>>7   )|((pbTData1>>6)&2)|((pbTData2>>5)&4)|((pbTData3>>4)&8))<<24	;
		break;
	case 7:		// 16 Colors	Packed Mode	SPR_HREV
		pbTData2 = pbTData[2];
		pbTData3 = pbTData[3];
		Index0[0] = (pbTData3 & 0x0F)		|
			    (pbTData3>>4)<<8		|
			    (pbTData2 & 0x0F)<<16	|
			    (pbTData2>>4)<<24		;
		Index0[1] = (pbTData1 & 0x0F)		|
			    (pbTData1>>4)<<8		|
			    (pbTData0 & 0x0F)<<16	|
			    (pbTData0>>4)<<24		;
		break;
	}
}

void RefreshLine(const uint16_t Line)
{
    uint16_t *pSBuf;		/* データ書き込みバッファ */
    uint16_t *pSWrBuf;		/* ↑の書き込み位置用ポインタ*/
    uint8_t *pZ;		/* ↓のインクリメント用ポインタ*/
    uint8_t ZBuf[0x100];	/* FGレイヤーの非透明部を保存*/
    uint8_t *pW;		/*↓のインクリメント用ポインタ*/
    uint8_t WBuf[0x100];	/* FGレイヤーのウィンドーを保存*/
    int32_t OffsetX;
    int32_t OffsetY;
    uint8_t *pbTMap;
    int32_t TMapX;
    int32_t TMapXEnd;
    int32_t TMapTemp;
    int32_t PalIndex;
    int16_t i, j, k;
    uint16_t BaseCol; 
    pSBuf = FrameBuffer + Line * 240 + 8;	// +8 offset
    pSWrBuf = pSBuf;

    if(IO[LCDSLP] & 0x01)
    {
        if(IO[COLCTL] & 0xE0) BaseCol = Palette[(IO[BORDER] & 0xF0) >> 4][IO[BORDER] & 0x0F];
        else BaseCol = MonoColor[IO[BORDER] & 0x07];
    }
    else BaseCol = 0;

    for(i = 0; i < 224; i+=2) *(uint32_t*)(pSWrBuf+i) = BaseCol|(BaseCol<<16);

    if(!(IO[LCDSLP] & 0x01)) return;
/*********************************************************************/
    if((IO[DSPCTL] & 0x01) && Layer[0])                                 /* BG layer */
    {
        OffsetX = IO[SCR1X] & 0x07;
        pSWrBuf = pSBuf - OffsetX;
        i = Line + IO[SCR1Y];
        OffsetY = (i & 0x07);

        pbTMap = Scr1TMap + ((i & 0xF8) << 3);
        TMapX = (IO[SCR1X] & 0xF8) >> 2;
        TMapXEnd = ((IO[SCR1X] + 224 + 7) >> 2) & 0xFFE;

        for(; TMapX < TMapXEnd;)
        {
            TMap = *(pbTMap + (TMapX++ & 0x3F));
            TMap |= *(pbTMap + (TMapX++ & 0x3F)) << 8;

            if(IO[COLCTL] & 0x40) /* 16 colors */
            {
                if(TMap & MAP_BANK) pbTData = IRAM + 0x8000;
                else pbTData = IRAM + 0x4000;
                pbTData += (TMap & MAP_TILE) << 5;
                if(TMap & MAP_VREV) pbTData += (7 - OffsetY) << 2;
                else pbTData += OffsetY << 2;
            } else {
                if((IO[COLCTL] & 0x80) && (TMap & MAP_BANK)) /* 4 colors and bank 1 */
                     pbTData = IRAM + 0x4000;
                else pbTData = IRAM + 0x2000;
                pbTData += (TMap & MAP_TILE) << 4;
                if(TMap & MAP_VREV) pbTData += (7 - OffsetY) << 1;
                else pbTData += OffsetY << 1;
            }

	    MakeIndex();
            PalIndex = (TMap & MAP_PAL) >> 9;
	    TMapTemp = ((TMap & 0x0800) || (IO[COLCTL] & 0x40));

            if (!((!Index[0]) && (TMapTemp))) *(pSWrBuf+0) = Palette[PalIndex][Index[0]];
            if (!((!Index[1]) && (TMapTemp))) *(pSWrBuf+1) = Palette[PalIndex][Index[1]];
            if (!((!Index[2]) && (TMapTemp))) *(pSWrBuf+2) = Palette[PalIndex][Index[2]];
            if (!((!Index[3]) && (TMapTemp))) *(pSWrBuf+3) = Palette[PalIndex][Index[3]];
            if (!((!Index[4]) && (TMapTemp))) *(pSWrBuf+4) = Palette[PalIndex][Index[4]];
            if (!((!Index[5]) && (TMapTemp))) *(pSWrBuf+5) = Palette[PalIndex][Index[5]];
            if (!((!Index[6]) && (TMapTemp))) *(pSWrBuf+6) = Palette[PalIndex][Index[6]];
            if (!((!Index[7]) && (TMapTemp))) *(pSWrBuf+7) = Palette[PalIndex][Index[7]];
	    pSWrBuf+=8;
        }
    }
/*********************************************************************/
    memset(ZBuf, 0, sizeof(ZBuf));
    if((IO[DSPCTL] & 0x02) && Layer[1])          /* FG layer表示 */
    {
        if((IO[DSPCTL] & 0x30) == 0x20) {	/* ウィンドウ内部のみに表示 */
            for(i = 0, pW = WBuf + 8; i < 224; i+=4) *(uint32_t*)(pW+i) = 0x01010101;
            if((Line >= IO[SCR2WT]) && (Line <= IO[SCR2WB]))
                { for(i = IO[SCR2WL], pW = WBuf + 8 + i; (i <= IO[SCR2WR]) && (i < 224); i++) *pW++ = 0; }
        } else if((IO[DSPCTL] & 0x30) == 0x30) {/* ウィンドウ外部のみに表示 */
            for(i = 0, pW = WBuf + 8; i < 224; i+=4) *(uint32_t*)(pW+i) = 0;
            if((Line >= IO[SCR2WT]) && (Line <= IO[SCR2WB]))
                { for(i = IO[SCR2WL], pW = WBuf + 8 + i; (i <= IO[SCR2WR]) && (i < 224); i++) *pW++ = 1; }
        } else {
	    for(i = 0, pW = WBuf + 8; i < 224; i+=4) *(uint32_t*)(pW+i) = 0;
	}

        OffsetX = IO[SCR2X] & 0x07;
        pSWrBuf = pSBuf - OffsetX;
        i = Line + IO[SCR2Y];
        OffsetY = (i & 0x07);

        pbTMap = Scr2TMap + ((i & 0xF8) << 3);
        TMapX = (IO[SCR2X] & 0xF8) >> 2;
        TMapXEnd = ((IO[SCR2X] + 224 + 7) >> 2) & 0xFFE;

        pW = WBuf + 8 - OffsetX;
        pZ = ZBuf + 8 - OffsetX;
        
        for(; TMapX < TMapXEnd;)
        {
            TMap = *(pbTMap + (TMapX++ & 0x3F));
            TMap |= *(pbTMap + (TMapX++ & 0x3F)) << 8;

            if(IO[COLCTL] & 0x40)
            {
                if(TMap & MAP_BANK) pbTData = IRAM + 0x8000;
                else pbTData = IRAM + 0x4000;
                pbTData += (TMap & MAP_TILE) << 5;
                if(TMap & MAP_VREV) pbTData += (7 - OffsetY) << 2;
                else pbTData += OffsetY << 2;
            }
            else
            {
                if((IO[COLCTL] & 0x80) && (TMap & MAP_BANK))	/* 4 colors and bank 1 */
                     pbTData = IRAM + 0x4000;
                else pbTData = IRAM + 0x2000;
                pbTData += (TMap & MAP_TILE) << 4;
                if(TMap & MAP_VREV) pbTData += (7 - OffsetY) << 1;
                else pbTData += OffsetY << 1;
            }

	    MakeIndex();
            PalIndex = (TMap & MAP_PAL) >> 9;
	    TMapTemp = ((TMap & 0x0800) || (IO[COLCTL] & 0x40));

            if ( !(((!Index[0]) && (TMapTemp)) || (*(pW+0))) ) { *(pSWrBuf+0) = Palette[PalIndex][Index[0]]; *(pZ+0) = 1; }
            if ( !(((!Index[1]) && (TMapTemp)) || (*(pW+1))) ) { *(pSWrBuf+1) = Palette[PalIndex][Index[1]]; *(pZ+1) = 1; }
            if ( !(((!Index[2]) && (TMapTemp)) || (*(pW+2))) ) { *(pSWrBuf+2) = Palette[PalIndex][Index[2]]; *(pZ+2) = 1; }
            if ( !(((!Index[3]) && (TMapTemp)) || (*(pW+3))) ) { *(pSWrBuf+3) = Palette[PalIndex][Index[3]]; *(pZ+3) = 1; }
            if ( !(((!Index[4]) && (TMapTemp)) || (*(pW+4))) ) { *(pSWrBuf+4) = Palette[PalIndex][Index[4]]; *(pZ+4) = 1; }
            if ( !(((!Index[5]) && (TMapTemp)) || (*(pW+5))) ) { *(pSWrBuf+5) = Palette[PalIndex][Index[5]]; *(pZ+5) = 1; }
            if ( !(((!Index[6]) && (TMapTemp)) || (*(pW+6))) ) { *(pSWrBuf+6) = Palette[PalIndex][Index[6]]; *(pZ+6) = 1; }
            if ( !(((!Index[7]) && (TMapTemp)) || (*(pW+7))) ) { *(pSWrBuf+7) = Palette[PalIndex][Index[7]]; *(pZ+7) = 1; }
            pW+=8; pZ+=8; pSWrBuf+=8;
        }
    }
/*********************************************************************/
    if((IO[DSPCTL] & 0x04) && Layer[2])         /* Sprites */
    {
        if (IO[DSPCTL] & 0x08)     /* Sprite window */
        {
            for (i = 0, pW = WBuf + 8; i < 224; i+=4) *(uint32_t*)(pW+i) = 0x01010101;
            if ((Line >= IO[SPRWT]) && (Line <= IO[SPRWB]))
                { for (i = IO[SPRWL], pW = WBuf + 8 + i; (i <= IO[SPRWR]) && (i < 224); i++) *pW++ = 0; }
        }

        for (pbTMap = SprETMap; pbTMap >= SprTTMap; pbTMap -= 4)
        {
            if (pbTMap[2] > 0xF8) j = pbTMap[2] - 0x100;
            else j = pbTMap[2];
            if (pbTMap[3] > 0xF8) k = pbTMap[3] - 0x100;
            else k = pbTMap[3];

            if (Line < j) continue;
            if (Line >= j + 8) continue;
            if (224 <= k) continue;

            TMap = pbTMap[0] | (pbTMap[1] << 8);

            if (IO[COLCTL] & 0x40)
            {
                pbTData = IRAM + 0x4000 + ((TMap & SPR_TILE) << 5);
                if (TMap & SPR_VREV) pbTData += (7 - Line + j) << 2;
                else pbTData += (Line - j) << 2;
            }
            else
            {
                pbTData = IRAM + 0x2000 + ((TMap & SPR_TILE) << 4);
                if (TMap & SPR_VREV) pbTData += (7 - Line + j) << 1;
                else pbTData += (Line - j) << 1;
            }

            pSWrBuf = pSBuf + k;
            pW = WBuf + 8 + k;
            pZ = ZBuf + k + 8;
	    MakeIndex();
            PalIndex = ((TMap & SPR_PAL) >> 9) + 8;
	    TMapTemp = ((TMap & 0x0800) || (IO[COLCTL] & 0x40));

            for(i = 0; i < 8; i++)
            {
                if (IO[DSPCTL] & 0x08) {
                    if (TMap & SPR_CLIP) {
                        if (!*(pW+i)) continue;
                    } else {
                        if (*(pW+i)) continue;
                    }
                }
                if ( (!Index[i]) && (TMapTemp) )  continue;
                if ((*(pZ+i)) && (!(TMap & SPR_LAYR))) continue;
                *(pSWrBuf+i) = Palette[PalIndex][Index[i]];
            }
        }
    }
    
}
