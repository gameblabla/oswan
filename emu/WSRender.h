/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/

#ifndef WSRENDER_H_
#define WSRENDER_H_

#include "WSHard.h"

extern BYTE *Scr1TMap;
extern BYTE *Scr2TMap;
extern BYTE *SprTTMap;
extern BYTE *SprETMap;
extern BYTE SprTMap[512];
extern WORD Palette[16][16];
extern WORD MonoColor[8];
extern WORD FrameBuffer[320*240];
extern WORD SegmentBuffer[(LCD_MAIN_H * 4) * (8 * 4)];
extern int Layer[3];
extern int Segment[11];

void SetPalette(int addr);
void RefreshLine(int Line);
void RenderSegment(void);
void RenderSleep(void);

#endif
