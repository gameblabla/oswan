#ifndef WSAPU_H_
#define WSAPU_H_

#include <stdint.h>

#define BPSWAV      12000 /* WSのHblankが12KHz */

#ifdef NATIVE_AUDIO
#define BUFSIZEN    0x10000
#define SND_BNKSIZE 512
#define SND_RNGSIZE (32 * SND_BNKSIZE) /* Was 10 * SND_BNKSIZE */
#else
#define BUFSIZEN    0x10000
#define SND_BNKSIZE 1024
#define SND_RNGSIZE (32 * SND_BNKSIZE) /* Was 10 * SND_BNKSIZE */
#endif

#define MULT 3
#define WAV_VOLUME 30

typedef struct sound 
{
    uint16_t on;
    uint16_t freq;
    uint16_t volL;
    uint16_t volR;
} SOUND;

typedef struct sweep 
{
    int32_t on;
    int32_t time;
    int32_t step;
    int32_t cnt;
} SWEEP;

typedef struct noise 
{
    int32_t on;
    int32_t pattern;
} NOISE;

extern SOUND Ch[4];
extern SWEEP Swp;
extern NOISE Noise;

extern int8_t VoiceOn;
extern int16_t Sound[7];

/* Required for sound.c */
extern int32_t rBuf, wBuf;
extern int32_t apuBufLen(void);
extern int16_t sndbuffer[2][SND_RNGSIZE];

int32_t apuBufLen(void);
void apuWaveVolume(int32_t);
void apuWaveCreate(void);
void apuWaveDel(void);
void apuWaveClear(void);
void apuInit(void);
void apuEnd(void);
uint16_t apuMrand(uint32_t);
void apuSetPData(int32_t, uint8_t);
uint8_t apuVoice(void);
void apuSweep(void);
uint16_t apuShiftReg(void);
void apuWaveSet(void);
void apuStartupSound(void);

int16_t* apuBufGetLock(uint32_t size);
void apuBufGetUnlock(void* ptr, uint32_t size);

#endif
