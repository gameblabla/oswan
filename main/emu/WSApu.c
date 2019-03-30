#include <stdio.h>

#include <SDL/SDL.h>

#include "WSHard.h"
#include "WSApu.h"
#include "sound.h"

SOUND Ch[4];
SWEEP Swp;
NOISE Noise; 

int16_t sndbuffer[2][SND_RNGSIZE]; /* Sound Ring Buffer */
int32_t rBuf, wBuf;

int8_t VoiceOn;
int16_t Sound[7] = {1, 1, 1, 1, 1, 1, 1};

static uint32_t convert_multiplier = MULT;

static uint8_t PData[4][32];
static uint8_t PDataN[8][BUFSIZEN];
static uint16_t RandData[BUFSIZEN];

extern uint8_t *Page[16];
extern uint8_t IO[0x100];

int32_t apuBufLen(void)
{
	if (wBuf >= rBuf) return wBuf - rBuf;
	return SND_RNGSIZE + (wBuf - rBuf);
}

void apuWaveCreate(void)
{
    /*memset(sndbuffer,0x00, SND_RNGSIZE);*/
    memset(sndbuffer, 0x00, sizeof(*sndbuffer));
}

void apuWaveRelease(void)
{
	Pause_Sound();
}

void apuInit(void)
{
    uint32_t i, j;
    
    convert_multiplier = MULT;

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
}

void apuEnd(void)
{
    apuWaveRelease();
	Sound_APUClose();
}

uint16_t apuMrand(uint32_t Degree)
{
	#define BIT(n) (1<<n)
    typedef struct
    {
        uint32_t N;
        uint16_t InputBit;
        int32_t Mask;
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
    static uint16_t ShiftReg = BIT(2)-1;
    int32_t XorReg = 0;
    int32_t Masked;

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

void apuSetPData(int32_t addr, uint8_t val)
{
    int32_t i, j;

    i = (addr & 0x30) >> 4;
    j = (addr & 0x0F) << 1;
    PData[i][j]=(uint8_t)(val & 0x0F);
    PData[i][j + 1]=(uint8_t)((val & 0xF0)>>4);
}

uint8_t apuVoice(void)
{
    static int32_t index = 0, b = 0;
    uint8_t v;

    if ((IO[SDMACTL] & 0x98) == 0x98) /* Hyper voice */
    { 
        v = Page[IO[SDMASH] + b][*(uint16_t*)(IO + SDMASL) + index++];
        if ((*(uint16_t*)(IO + SDMASL) + index) == 0)
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
        if (*(uint16_t*)(IO+SDMACNT) <= index)
        {
            index = 0;
            b = 0;
        }
        return v;
    }
    else if ((IO[SDMACTL] & 0x88) == 0x80) /* DMA start */
    { 
        IO[SND2VOL] = Page[IO[SDMASH] + b][*(uint16_t*)(IO + SDMASL) + index++];
        if ((*(uint16_t*)(IO + SDMASL) + index) == 0)
        {
            b++;
        }
        if (*(uint16_t*)(IO + SDMACNT) <= index)
        {
            IO[SDMACTL] &= 0x7F; /* DMA end */
            *(uint16_t*)(IO + SDMACNT) = 0;
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

uint16_t apuShiftReg(void)
{
    static uint32_t nPos = 0;
    /* Noise counter */
    if (++nPos >= BUFSIZEN)
    {
        nPos = 0;
    }
    return RandData[nPos];
}

void apuWaveSet(void)
{
	/* Do you like them, The Wonders of uninitialized variables ?
	* Especially when the compiler gives you no insight on that ?
	* If lVol and rVol are not initiliased, then it will sound wrong on
	* games like Klonoa with voices. After initializing them,
	* it would work and it would still sound fine if we're using a 16-bits
	* size for them. This should hopefully make things faster on
	* some platforms. No FPU needed !
	*/
	static uint16_t point[4] = {0, 0, 0, 0};
    static uint16_t preindex[4] = {0, 0, 0, 0};
    uint16_t value = 0, lVol[4] = {0, 0, 0, 0}, rVol[4] = {0, 0, 0, 0};
    int16_t LL, RR, vVol;
    uint16_t index;
    uint32_t channel;

    Sound_APU_Start();
    
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
                
				value = PDataN[Noise.pattern][index] - 8;
            }
            else if (Sound[channel] == 0)
            {
                continue;
            }
            else 
            {
                index = (3072000 / BPSWAV) * point[channel] / (2048 - Ch[channel].freq);
                if ((index %= 32) == 0 && preindex[channel])
                {
                    point[channel] = 0;
                }
                value = PData[channel][index] - 8;
            }
            preindex[channel] = index;
            point[channel]++;
            lVol[channel] = value * Ch[channel].volL; /* -8*15=-120, 7*15=105 */
            rVol[channel] = value * Ch[channel].volR;
        }
		else
		{
			lVol[channel] = 0;
			rVol[channel] = 0;	
		}
    }
    
    vVol = (apuVoice() - 0x80);
    /* mix 16bits wave -32768 ～ +32767 32768/120 = 273 */
    LL = (lVol[0] + lVol[1] + lVol[2] + lVol[3] + vVol) * WAV_VOLUME;
    RR = (rVol[0] + rVol[1] + rVol[2] + rVol[3] + vVol) * WAV_VOLUME;

	#ifdef NATIVE_AUDIO
	sndbuffer[0][wBuf] = LL;
	sndbuffer[1][wBuf] = RR;
	if (++wBuf >= SND_RNGSIZE)
	{
		wBuf = 0;
	}
	#else
	if (convert_multiplier == MULT) 
	{
		convert_multiplier = (MULT+1);
	}
	else
	{
		convert_multiplier = MULT;
	}

	for (uint32_t i=0;i<convert_multiplier;i++)	/* 48000/12000 */
	{ 
		sndbuffer[0][wBuf] = LL;
		sndbuffer[1][wBuf] = RR;
		if (++wBuf >= SND_RNGSIZE)
		{
			wBuf = 0;
		}
	}
	#endif
	
	Sound_APU_End();
}

