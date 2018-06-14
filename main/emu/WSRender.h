#ifndef WSRENDER_H_
#define WSRENDER_H_

#include "WSHard.h"

#define LINE_SIZE (256)

extern uint8_t *Scr1TMap;
extern uint8_t *Scr2TMap;
extern uint8_t *SprTTMap;
extern uint8_t *SprETMap;
extern uint8_t SprTMap[512];
extern uint16_t Palette[16][16];
extern uint16_t MonoColor[8];
extern uint16_t FrameBuffer[320*240];
extern const uint8_t Layer[3];

void SetPalette(const uint32_t addr);
void RefreshLine(const uint16_t Line);

#endif
