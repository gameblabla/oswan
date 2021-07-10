#ifndef WS_H_
#define WS_H_

#include "WSHard.h"

struct EEPROM
{
    uint16_t *data;
    int32_t we;
};

/*extern uint32_t Run;*/
extern uint8_t *Page[0x10];
extern uint8_t IRAM[0x10000];
extern uint8_t IO[0x100];
extern uint8_t MemDummy[0x10000];
extern uint8_t *ROMMap[0x100];     			/* C-ROMバンクマップ 	*/
extern uint16_t ROMBanks;            		/* C-ROMバンク数 		*/
extern uint8_t *RAMMap[0x100];     			/* C-RAMバンクマップ	*/
extern uint8_t RAMBanks;            		/* C-RAMバンク数 		*/
extern uint32_t RAMSize;             			/* C-RAM総容量 		*/
extern uint16_t IEep[64];
extern struct EEPROM sIEep;
extern struct EEPROM sCEep;
extern uint8_t HVMode;

#define CK_EEP 1
extern uint8_t CartKind;

void WriteIO(const uint32_t A, uint8_t V);
void WsReset (void);
void WsRomPatch(char *buf);
void WsSetDir(void);
uint32_t WsRun(void);
void WsInit(void);
void WsDeInit(void);
void SetHVMode(uint32_t Mode);

#endif
