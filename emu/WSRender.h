/*
$Date: 2010-05-11 09:13:27 -0400 (Tue, 11 May 2010) $
$Rev: 85 $
*/

#ifndef WSRENDER_H_
#define WSRENDER_H_

#include "WSHard.h"

#define LINE_SIZE (256)

extern BYTE *Scr1TMap;
extern BYTE *Scr2TMap;
extern BYTE *SprTTMap;
extern BYTE *SprETMap;
extern BYTE SprTMap[512];
extern WORD Palette[16][16];
extern WORD MonoColor[8];
extern WORD FrameBuffer[320*240];
extern WORD SegmentBuffer[(144 * 4) * (8 * 4)]; // 8 * 144 Ì4{ÌTCYÅ`æ
extern int Layer[3];
extern int Segment[11];

void SetPalette(int addr);
void RefreshLine(int Line);
void RenderSegment(void);
void RenderSleep(void);

#endif
