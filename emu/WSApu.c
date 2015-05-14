/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/
#include <stdio.h>

//#include "entry.h"
#include "WSHard.h"
#include "WS.h"
#include "WSApu.h"

#ifdef SOUND_ON
#include <SDL/SDL.h>
#endif

typedef struct
{
	unsigned int   frequency;
	unsigned short bits;
	unsigned char  channels;
	unsigned char  volume;
} WAVEOUT_INFO;

WAVEOUT_INFO mywaveinfo;

#define BUFSIZEN    0x10000
#define WAV_FREQ    12000
#define SND_BNKSIZE 256  
 
#if defined(SOUND_ON) && !defined(SOUND_EMULATION)
	#define SND_RNGSIZE ((44100/60)*2*64)
#else
	#define SND_RNGSIZE (64 * SND_BNKSIZE)
#endif

#define WAV_VOLUME  40

unsigned long WaveMap;
SOUND Ch[4];
int VoiceOn;
SWEEP Swp;
NOISE Noise;
int Sound[7] = {1, 1, 1, 1, 1, 1, 1};

#ifdef SOUND_EMULATION
static unsigned char PData[4][32];
static unsigned char PDataN[8][BUFSIZEN];
static unsigned int  RandData[BUFSIZEN];

static unsigned short sndbuffer[SND_RNGSIZE]; // Sound Ring Buffer
static int   rBuf, wBuf;
#endif

static int   StartupFlag;

extern BYTE *Page[16];
extern BYTE IO[0x100];

#ifdef SOUND_ON
	SDL_mutex *sound_mutex;
	SDL_cond *sound_cv;
#endif

// ----------------------------------------------------------------------------
// Resample
// ----------------------------------------------------------------------------
int apuBufLen(void)
{
#ifdef SOUND_EMULATION
	if (wBuf >= rBuf) return wBuf - rBuf;
	return SND_RNGSIZE + wBuf - rBuf;
#else
	return 0;
#endif
}

/*
short* apuBufGetLock(int size)
{
	if (apuBufLen() >= size) {
		return &sndbuffer[rBuf];
	}
	return NULL;
}

void apuBufGetUnlock(void* ptr, int size)
{
	if (ptr == (void*)&sndbuffer[rBuf]) {
		rBuf = (rBuf + size) % SND_RNGSIZE;
	}
}
*/

void mixaudioCallback(void *userdata, unsigned char *stream, int len)
{
#ifdef SOUND_ON
	int i=len;
	unsigned short *buffer = (unsigned short *) stream;

	SDL_LockMutex(sound_mutex);

	if (apuBufLen() < len) {
		memset(stream,0,len);
	}
	else {
		while(i>1) {
			*buffer++ = sndbuffer[rBuf];
			if (++rBuf >= SND_RNGSIZE) {
				rBuf = 0;
			}
			i -= 2;
		}
	}
	
	SDL_CondSignal(sound_cv);
	SDL_UnlockMutex(sound_mutex);
#endif
}

void apuWaveCreate(void)
{
#ifdef SOUND_ON
	memset(sndbuffer,0x00, SND_RNGSIZE);
#endif
}

void apuWaveDel(void)
{
#ifdef SOUND_ON
	SDL_PauseAudio(1);
#endif
}

void apuWaveClear(void)
{
}

int apuInit(void)
{
#ifdef SOUND_EMULATION
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 32; j++) {
            PData[i][j] = 8;
        }
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < BUFSIZEN; j++) {
            PDataN[i][j] = ((apuMrand(15 - i) & 1) ? 15 : 0);
        }
    }
    for (i = 0; i < BUFSIZEN; i++) {
        RandData[i] = apuMrand(15);
    }
    rBuf = 0;
    wBuf = 0;
	apuWaveCreate();
	
#ifdef SOUND_ON
	sound_mutex = SDL_CreateMutex();
	sound_cv = SDL_CreateCond();
#endif

#endif
    return 0;
}

void apuEnd(void)
{
#ifdef SOUND_EMULATION
	apuWaveDel();
#ifdef SOUND_ON
	SDL_CondSignal(sound_cv);
#endif
#endif
}

unsigned int apuMrand(unsigned int Degree)
{
#ifdef SOUND_EMULATION
#define BIT(n) (1<<n)
    typedef struct {
        unsigned int N;
        int InputBit;
        int Mask;
    } POLYNOMIAL;

    static POLYNOMIAL TblMask[]=
    {
        { 2,BIT(2) ,BIT(0)|BIT(1)},
        { 3,BIT(3) ,BIT(0)|BIT(1)},
        { 4,BIT(4) ,BIT(0)|BIT(1)},
        { 5,BIT(5) ,BIT(0)|BIT(2)},
        { 6,BIT(6) ,BIT(0)|BIT(1)},
        { 7,BIT(7) ,BIT(0)|BIT(1)},
        { 8,BIT(8) ,BIT(0)|BIT(2)|BIT(3)|BIT(4)},
        { 9,BIT(9) ,BIT(0)|BIT(4)},
        {10,BIT(10),BIT(0)|BIT(3)},
        {11,BIT(11),BIT(0)|BIT(2)},
        {12,BIT(12),BIT(0)|BIT(1)|BIT(4)|BIT(6)},
        {13,BIT(13),BIT(0)|BIT(1)|BIT(3)|BIT(4)},
        {14,BIT(14),BIT(0)|BIT(1)|BIT(4)|BIT(5)},
        {15,BIT(15),BIT(0)|BIT(1)},
        { 0,      0,      0},
    };
    static POLYNOMIAL *pTbl = TblMask;
    static int ShiftReg = BIT(2)-1;
    int XorReg = 0;
    int Masked;

    if(pTbl->N != Degree) {
        pTbl = TblMask;
        while(pTbl->N) {
            if(pTbl->N == Degree) {
                break;
            }
            pTbl++;
        }
        if(!pTbl->N) {
            pTbl--;
        }
        ShiftReg &= pTbl->InputBit-1;
        if(!ShiftReg) {
            ShiftReg = pTbl->InputBit-1;
        }
    }
    Masked = ShiftReg & pTbl->Mask;
    while(Masked) {
        XorReg ^= Masked & 0x01;
        Masked >>= 1;
    }
    if(XorReg) {
        ShiftReg |= pTbl->InputBit;
    }
    else {
        ShiftReg &= ~pTbl->InputBit;
    }
    ShiftReg >>= 1;
    return ShiftReg;
#else
	return 0;
#endif
}

void apuSetPData(int addr, unsigned char val)
{
#ifdef SOUND_EMULATION
    int i, j;

    i = (addr & 0x30) >> 4;
    j = (addr & 0x0F) << 1;
    PData[i][j]=(unsigned char)(val & 0x0F);
    PData[i][j + 1]=(unsigned char)((val & 0xF0)>>4);
#endif
}

unsigned char apuVoice(void)
{
#ifdef SOUND_EMULATION
    static int index = 0, b = 0;
    unsigned char v;

    if ((SDMACTL & 0x98) == 0x98) { // Hyper voice
        v = Page[SDMASB + b][SDMASA + index++];
        if ((SDMASA + index) == 0)
        {
            b++;
        }
        if (v < 0x80) {
            v += 0x80;
        }
        else {
            v -= 0x80;
        }
        if (SDMACNT <= index) {
            index = 0;
            b = 0;
        }
        return v;
    }
    else if ((SDMACTL & 0x88) == 0x80) { // DMA start
        IO[0x89] = Page[SDMASB + b][SDMASA + index++];
        if ((SDMASA + index) == 0)
        {
            b++;
        }
        if (SDMACNT <= index) {
            SDMACTL &= 0x7F; // DMA end
            SDMACNT = 0;
            index = 0;
            b = 0;
        }
    }
    return ((VoiceOn && Sound[4]) ? IO[0x89] : 0x80);
#else
	return 0;
#endif
}

unsigned char ws_apuhVoice(int count, BYTE *hvoice) {
#ifdef SOUND_EMULATION
	static int index = 0;

	if ( (IO[0x52] & 0x98) == 0x98) { // Hyper Voice On?
		int  address = (IO[0x4c] << 16) | (IO[0x4b] << 8) | IO[0x4a];
		int  size    =                          (IO[0x4f] << 8) | IO[0x4e];

		int value1 = cpu_readmem20(address + index);

    if (value1 < 0x80)
      *hvoice = (BYTE) (value1 + 0x80);
    else
      *hvoice = (BYTE) (value1 - 0x80);
    
    if (count == 0)
      if (size <= (++index))
       index = 0;
  }
  else {
    *hvoice = 0x80;
    index     = 0;
  }
  
#endif
  return 0;
}

unsigned char ws_apuVoice(int count)
{
#ifdef SOUND_EMULATION
    if ((SDMACTL & 0x88) == 0x80) { // DMA start
		int i =                          (IO[0x4f] << 8) | IO[0x4e]; // size
		int j = (IO[0x4c] << 16) | (IO[0x4b] << 8) | IO[0x4a]; // start bank:address
		int k = ((IO[0x52] & 0x03) == 0x03 ? 2 : 1);

		IO[0x89] = cpu_readmem20(j);

		if ((count % (44100 / 12000 / k)) == 0) {
		  i--;
		  j++;
		}

		if (i <= 0) {
		  i = 0;
		  IO[0x52] &= 0x7f; // DMA end
		}

		IO[0x4a] = (BYTE)  j;
		IO[0x4b] = (BYTE) (j >>  8);
		IO[0x4c] = (BYTE) (j >> 16);
		IO[0x4e] = (BYTE)  i;
		IO[0x4f] = (BYTE) (i >>  8);
    }
    return ((VoiceOn && Sound[4]) ? IO[0x89] : 0x80);
#else
	return 0;
#endif
}

void apuSweep(void)
{
#ifdef SOUND_EMULATION
    if ((Swp.step) && Swp.on) { // sweep on
        if (Swp.cnt < 0) {
            Swp.cnt = Swp.time;
            Ch[2].freq += Swp.step;
            Ch[2].freq &= 0x7ff;
        }
        Swp.cnt--;
    }
#endif
}

WORD apuShiftReg(void)
{
#ifdef SOUND_EMULATION
    static int nPos = 0;
    // Noise counter
    if (++nPos >= BUFSIZEN) {
        nPos = 0;
    }
    return RandData[nPos];
#else
	return 0;
#endif
}


void WsWaveSet(BYTE voice, BYTE hvoice)
{
#ifdef SOUND_EMULATION
    static  int point[] = {0, 0, 0, 0};
    static  int preindex[] = {0, 0, 0, 0};
    int     channel, index;
    signed short value, lVol[4];
    /*signed short rVol[4];*/
    signed short LL, vVol, hVol;
#define MULT 4
	static int conv=MULT;
	int i;

#ifdef SOUND_ON
	SDL_LockMutex(sound_mutex);
#endif
	
	for (channel = 0; channel < 4; channel++) {
		if (Ch[channel].on) {
			if (channel == 1 && VoiceOn && Sound[4]) {
				continue;
			}
			else if (channel == 2 && Swp.on && !Sound[5]) {
				continue;
			}
			else if (channel == 3 && Noise.on && Sound[6]){
				index = (3072000 / WAV_FREQ) * point[3] / (2048 - Ch[3].freq);
				if ((index %= BUFSIZEN) == 0 && preindex[3]) {
					point[3] = 0;
				}
				value = (short)PDataN[Noise.pattern][index] - 8;
			}
			else if (Sound[channel] == 0) {
				continue;
			}
			else {
				index = (3072000 / WAV_FREQ) * point[channel] / (2048 - Ch[channel].freq);
				if ((index %= 32) == 0 && preindex[channel]) {
					point[channel] = 0;
				}
				value = (short)PData[channel][index] - 8;
			}
			preindex[channel] = index;
			point[channel]++;
			lVol[channel] = value * Ch[channel].volL; // -8*15=-120, 7*15=105
			/*rVol[channel] = value * Ch[channel].volR;*/
		}
		else {
			lVol[channel] = 0;
			/*rVol[channel] = 0;*/
		}
	}
	vVol = ( ((short)voice) - 0x80) * 2;
	hVol = ( ((short) hvoice) - 0x80) * 2;
	// mix 16bits wave -32768 ` +32767 32768/120 = 273
	LL = (lVol[0] + lVol[1] + lVol[2] + lVol[3] + vVol +hVol) * WAV_VOLUME;

	if (conv ==MULT) 
		conv = (MULT+1);
	else
		conv = MULT;
	
	for (i=0;i<conv;i++)  { // 44100/12000
		sndbuffer[wBuf] = LL;

		if (++wBuf >= SND_RNGSIZE) {
			wBuf = 0;
		}
	}
#ifdef SOUND_ON
	SDL_CondSignal(sound_cv);
	SDL_UnlockMutex(sound_mutex);
#endif

#endif
}

void apuWaveSet(void)
{
#ifdef SOUND_EMULATION
	BYTE voice , hvoice;
	apuSweep();
	
	voice = ws_apuVoice(0);
	ws_apuhVoice(0, &hvoice);
	WsWaveSet(voice, hvoice);
	NCSR = apuShiftReg();
#endif
}

void apuStartupSound(void)
{
	StartupFlag = 1;
}
