#ifndef WSAPU_H_
#define WSAPU_H_

#include <stdint.h>

typedef struct sound {
    int32_t on;
    int32_t freq;
    int32_t volL;
    int32_t volR;
} SOUND;
typedef struct sweep {
    int32_t on;
    int32_t time;
    int32_t step;
    int32_t cnt;
} SWEEP;
typedef struct noise {
    int32_t on;
    int32_t pattern;
} NOISE;

extern SOUND Ch[4];
extern SWEEP Swp;
extern NOISE Noise;

extern int8_t VoiceOn;
extern int16_t Sound[7];

int32_t apuBufLen(void);
void apuWaveVolume(int32_t);
void apuWaveCreate(void);
void apuWaveDel(void);
void apuWaveClear(void);
void apuInit(void);
void apuEnd(void);
uint32_t apuMrand(uint32_t);
void apuSetPData(int32_t, uint8_t);
uint8_t apuVoice(void);
void apuSweep(void);
uint16_t apuShiftReg(void);
void apuWaveSet(void);
void apuStartupSound(void);

#endif
