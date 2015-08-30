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
extern const unsigned char Layer[3];

void SetPalette(const int addr);
void RefreshLine(const unsigned short Line);
void RenderSleep(void);

#endif
