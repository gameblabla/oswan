#include <stdio.h>

#include <SDL/SDL.h>

#include "WSHard.h"
#include "WSApu.h"

#define BUFSIZEN    0x10000
#define BPSWAV      12000 /* WSのHblankが12KHz */

/*4096*/
/*2048*/
#define SND_BNKSIZE 2048
#define MULT 4

#define SND_RNGSIZE (10 * SND_BNKSIZE)
#define WAV_VOLUME 30

SOUND Ch[4];
SWEEP Swp;
NOISE Noise; 
char VoiceOn;
short Sound[7] = {1, 1, 1, 1, 1, 1, 1};

static unsigned char PData[4][32];
static unsigned char PDataN[8][BUFSIZEN];
static unsigned int RandData[BUFSIZEN];
static short sndbuffer[SND_RNGSIZE][2]; /* Sound Ring Buffer */

static int  rBuf, wBuf;

extern BYTE *Page[16];
extern BYTE IO[0x100];

SDL_mutex *sound_mutex;
SDL_cond *sound_cv;

static unsigned long read_pos;

int apuBufLen(void)
{
	if (wBuf >= rBuf) 
	{
		read_pos = 0;
		return (wBuf - rBuf);
	}
	return SND_RNGSIZE + wBuf - rBuf;
}


void mixaudioCallback(void *userdata, unsigned char *stream, int len)
{
	int i = len;
	unsigned short *buffer = (unsigned short *) stream;
	
    if(len <= 0 || !buffer)
        return;
        
	SDL_LockMutex(sound_mutex);

	/*printf("%d\n", apuBufLen());*/
	
	if (/*SDL_GetAudioStatus() == SDL_AUDIO_PAUSED &&*/ apuBufLen() < len) 
	{
		memset(stream,0,len);
	}
	else 
	{
		while(i > 3) 
		{
			*buffer++ = sndbuffer[rBuf][0];
			*buffer++ = sndbuffer[rBuf][1];
			if (++rBuf >= SND_RNGSIZE) 
			{
				rBuf = 0;
			}
			i -= 4;
		}
	}

	SDL_UnlockMutex(sound_mutex);
	SDL_CondSignal(sound_cv);
}

void apuWaveCreate(void)
{
    //memset(sndbuffer,0x00, SND_RNGSIZE);
    memset(sndbuffer, 0x00, sizeof(*sndbuffer));
}

void apuWaveRelease(void)
{
	SDL_PauseAudio(1);
}

void apuInit(void)
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 32; j++)
        {
            PData[i][j] = 8;
        }
    }
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < BUFSIZEN; j++)
        {
            PDataN[i][j] = ((apuMrand(15 - i) & 1) ? 15 : 0);
        }
    }

    for (i = 0; i < BUFSIZEN; i++)
    {
        RandData[i] = apuMrand(15);
    }
    apuWaveCreate();
    
	sound_mutex = SDL_CreateMutex();
	sound_cv = SDL_CreateCond();
}

void apuEnd(void)
{
    apuWaveRelease();
	SDL_CondSignal(sound_cv);
}

unsigned int apuMrand(unsigned int Degree)
{
#define BIT(n) (1<<n)
    typedef struct
    {
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

    if(pTbl->N != Degree)
    {
        pTbl = TblMask;
        while(pTbl->N) {
            if(pTbl->N == Degree)
            {
                break;
            }
            pTbl++;
        }
        if(!pTbl->N)
        {
            pTbl--;
        }
        ShiftReg &= pTbl->InputBit-1;
        if(!ShiftReg)
        {
            ShiftReg = pTbl->InputBit-1;
        }
    }
    Masked = ShiftReg & pTbl->Mask;
    while(Masked)
    {
        XorReg ^= Masked & 0x01;
        Masked >>= 1;
    }
    if(XorReg)
    {
        ShiftReg |= pTbl->InputBit;
    }
    else
    {
        ShiftReg &= ~pTbl->InputBit;
    }
    ShiftReg >>= 1;
    return ShiftReg;
}

void apuSetPData(int addr, unsigned char val)
{
    int i, j;

    i = (addr & 0x30) >> 4;
    j = (addr & 0x0F) << 1;
    PData[i][j]=(unsigned char)(val & 0x0F);
    PData[i][j + 1]=(unsigned char)((val & 0xF0)>>4);
}

unsigned char apuVoice(void)
{
    static int index = 0, b = 0;
    unsigned char v;

    if ((IO[SDMACTL] & 0x98) == 0x98) /* Hyper voice */
    { 
        v = Page[IO[SDMASH] + b][*(WORD*)(IO + SDMASL) + index++];
        if ((*(WORD*)(IO + SDMASL) + index) == 0)
        {
            b++;
        }
        if (v < 0x80)
        {
            v += 0x80;
        }
        else
        {
            v -= 0x80;
        }
        if (*(WORD*)(IO+SDMACNT) <= index)
        {
            index = 0;
            b = 0;
        }
        return v;
    }
    else if ((IO[SDMACTL] & 0x88) == 0x80) /* DMA start */
    { 
        IO[SND2VOL] = Page[IO[SDMASH] + b][*(WORD*)(IO + SDMASL) + index++];
        if ((*(WORD*)(IO + SDMASL) + index) == 0)
        {
            b++;
        }
        if (*(WORD*)(IO + SDMACNT) <= index)
        {
            IO[SDMACTL] &= 0x7F; /* DMA end */
            *(WORD*)(IO + SDMACNT) = 0;
            index = 0;
            b = 0;
        }
    }
    return ((VoiceOn && Sound[4]) ? IO[SND2VOL] : 0x80);
}

void apuSweep(void)
{
    if ((Swp.step) && Swp.on) /* Sweep on */
    {
        if (Swp.cnt < 0)
        {
            Swp.cnt = Swp.time;
            Ch[2].freq += Swp.step;
            Ch[2].freq &= 0x7ff;
        }
        Swp.cnt--;
    }
}

WORD apuShiftReg(void)
{
    static int nPos = 0;
    // Noise counter
    if (++nPos >= BUFSIZEN)
    {
        nPos = 0;
    }
    return RandData[nPos];
}

void apuWaveSet(void)
{
    static  int point[] = {0, 0, 0, 0};
    static  int preindex[] = {0, 0, 0, 0};
    int     channel, index;
    short   value, lVol[4], rVol[4];
    short   LL, RR, vVol;
    /*static int wait = 0;*/
    /*short   *dataAdr;*/
    
    /* 
     * MULT 3 is the correct speed but it plays like ass !
     * It seems like it just can't keep up.
     * I need someone to improve this...
    */
	static int conv=MULT;
    int i;

    /*if (WsInputGetNowait())
    {
        wait = 1;
    }
    else if (wait)
    {
        wait = 0;
        apuWaveClear();
    }*/
    
    SDL_LockMutex(sound_mutex);
    
    apuSweep();
    
    for (channel = 0; channel < 4; channel++)
    {
        if (Ch[channel].on)
        {
            if (channel == 1 && VoiceOn && Sound[4])
            {
                continue;
            }
            else if (channel == 2 && Swp.on && !Sound[5])
            {
                continue;
            }
            else if (channel == 3 && Noise.on && Sound[6])
            {
                index = (3072000 / BPSWAV) * point[3] / (2048 - Ch[3].freq);
                if ((index %= BUFSIZEN) == 0 && preindex[3])
                {
                    point[3] = 0;
                }
                value = (short)PDataN[Noise.pattern][index] - 8;
            }
            else if (Sound[channel] == 0)
            {
                continue;
            }
            else {
                index = (3072000 / BPSWAV) * point[channel] / (2048 - Ch[channel].freq);
                if ((index %= 32) == 0 && preindex[channel])
                {
                    point[channel] = 0;
                }
                value = (short)PData[channel][index] - 8;
            }
            preindex[channel] = index;
            point[channel]++;
            lVol[channel] = value * Ch[channel].volL; // -8*15=-120, 7*15=105
            rVol[channel] = value * Ch[channel].volR;
        }
		else
		{
			lVol[channel] = 0;
			rVol[channel] = 0;	
		}
    }
    
    vVol = ((short)apuVoice() - 0x80);
    /* mix 16bits wave -32768 ～ +32767 32768/120 = 273 */
    LL = (lVol[0] + lVol[1] + lVol[2] + lVol[3] + vVol) * WAV_VOLUME;
    RR = (rVol[0] + rVol[1] + rVol[2] + rVol[3] + vVol) * WAV_VOLUME;

	if (conv == MULT) 
		conv = (MULT+1);
	else
		conv = MULT;

	for (i=0;i<conv;i++)	/* 48000/12000 */
	{ 
		sndbuffer[wBuf][0] = LL;
		sndbuffer[wBuf][1] = RR;
		if (++wBuf >= SND_RNGSIZE)
		{
			wBuf = 0;
		}
	}
    
#ifdef SOUND_ON
	SDL_UnlockMutex(sound_mutex);
	SDL_CondSignal(sound_cv);
#endif
    
}

