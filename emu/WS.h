/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/

#ifndef WS_H_
#define WS_H_

#include "WSHard.h"

struct EEPROM
{
    WORD *data;
    int we;
};

extern int Run;
extern BYTE *Page[0x10];
extern BYTE IRAM[0x10000];
extern BYTE IO[0x100];
extern BYTE MemDummy[0x10000];
extern BYTE *ROMMap[0x100];     // C-ROMバンクマップ
extern int ROMBanks;            // C-ROMバンク数
extern BYTE *RAMMap[0x100];     // C-RAMバンクマップ
extern int RAMBanks;            // C-RAMバンク数
extern int RAMSize;             // C-RAM総容量
extern WORD IEep[64];
extern struct EEPROM sIEep;
extern struct EEPROM sCEep;

#define CK_EEP 1
extern int CartKind;

void WriteIO(DWORD A, BYTE V);
void WsReset (void);
void WsRomPatch(BYTE *buf);
int WsRun(void);
void WsCpyPdata(BYTE* dst);
void Sleep(int);
void SetHVMode(int Mode);

void WsInit(void);
void WsDeInit(void);

#endif
