/*
$Date: 2010-05-11 09:13:27 -0400 (Tue, 11 May 2010) $
$Rev: 85 $
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
extern unsigned short ROMBanks;            // C-ROMバンク数
extern BYTE *RAMMap[0x100];     // C-RAMバンクマップ
extern unsigned char RAMBanks;            // C-RAMバンク数
extern int RAMSize;             // C-RAM総容量
extern WORD IEep[64];
extern struct EEPROM sIEep;
extern struct EEPROM sCEep;

#define CK_EEP 1
extern unsigned char CartKind;

void WriteIO(const DWORD A, BYTE V);
void WsReset (void);
void WsRomPatch(const BYTE *buf);
void WsSetDir(void);
int WsRun(void);
void WsInit(void);
void WsDeInit(void);
void SetHVMode(const unsigned char Mode);

#endif
