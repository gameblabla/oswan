#ifndef WSHARD_H_
#define WSHARD_H_

#include <stdint.h>

typedef void  (*WriteMemFn) (uint32_t A, uint8_t V);
extern WriteMemFn WriteMemFnTable[0x10];
uint8_t ReadMem(uint32_t A);
void WriteMem(uint32_t A, uint8_t V);
void WriteIO(uint32_t A, uint8_t V);
uint8_t ReadIO(uint32_t A);

#define cpu_readop(A)               (ReadMem(A))
#define cpu_readop_arg(A)           (ReadMem(A))
#define cpu_readmem20(A)            (ReadMem(A))
#define cpu_writemem20(A, D)        (WriteMem((A), (uint8_t)(D)))
#define cpu_readport(port)          (ReadIO((port)))
#define cpu_writeport(port, val)    (WriteIO((port), (uint8_t)(val)))
#define cpu_readport16(port)        (ReadIO((port)))
#define cpu_writeport16(port, val)  (WriteIO((port), (uint8_t)(val)))

/*
---------------------------------------------------------------------------
 I/O
---------------------------------------------------------------------------
*/
#define DSPCTL      0x00        
#define BORDER      0x01        
#define RSTRL       0x02         
#define RSTRLC      0x03         
#define SPRTAB      0x04         
#define SPRBGN      0x05         
#define SPRCNT      0x06         
#define SCRMAP      0x07         
#define SCR2WL      0x08         /*foreground X*/
#define SCR2WT      0x09         /*foreground Y*/
#define SCR2WR      0x0A         /*foreground X*/
#define SCR2WB      0x0B         /*foreground Y*/
#define SPRWL       0x0C         /*X*/
#define SPRWT       0x0D         /*Y*/
#define SPRWR       0x0E         /*X*/
#define SPRWB       0x0F         /*Y*/
#define SCR1X       0x10         
#define SCR1Y       0x11         
#define SCR2X       0x12         /*foreground X*/
#define SCR2Y       0x13         /*foreground Y*/
#define LCDSLP      0x14         /*LCD*/
#define LCDSEG      0x15         /*segment*/

#define COL         0x1C         
#define PAL         0x20         

#define DMASRC      0x40        
#define DMADST      0x44        
#define DMACNT      0x46        

#define SDMASL      0x4A        
#define SDMASH      0x4C         
#define SDMACNT     0x4E        
#define SDMACTL     0x52         

#define COLCTL      0x60         

#define SND1FRQ     0x80         
#define SND2FRQ     0x82         
#define SND3FRQ     0x84         
#define SND4FRQ     0x86         
#define SND1VOL     0x88         
#define SND2VOL     0x89         
#define SND3VOL     0x8A         
#define SND4VOL     0x8B         
#define SNDSWP      0x8C         
#define SWPSTP      0x8D         
#define NSCTL       0x8E         
#define WAVDTP      0x8F         /*WAVE*/
#define SNDMOD      0x90         
#define SNDOUT      0x91         
#define NCSR        0x92        

#define HWARCH      0xA0         

#define TIMCTL      0xA2         
#define HPRE        0xA4        
#define VPRE        0xA6        
#define HCNT        0xA8        
#define VCNTL       0xAA          /*L*/
#define VCNTH       0xAC          /*H*/

#define IRQBSE      0xB0         
#define COMDT       0xB1         
#define IRQENA      0xB2         
#define COMCTL      0xB3         
#define KEYCTL      0xB5         
#define IRQACK      0xB6         

#define EEPDATA     0xBA         
#define EEPCMD      0xBC         
#define EEPCTL      0xBE         

#define BNKSLCT     0xC0         
#define BNK1SLCT    0xC1         
#define BNK2SLCT    0xC2         
#define BNK3SLCT    0xC3         
#define CEEPDATA    0xC4         
#define CEEPCMD     0xC6         

#define RTCCMD      0xCA         
#define RTCDATA     0xCB          

/*
---------------------------------------------------------------------------
 
---------------------------------------------------------------------------
*/
#define TXD_IFLAG 0x01
#define KEY_IFLAG 0x02
#define CRT_IFLAG 0x04
#define RXD_IFLAG 0x08
#define RST_IFLAG 0x10
#define VTM_IFLAG 0x20
#define VBB_IFLAG 0x40
#define HTM_IFLAG 0x80

/*
---------------------------------------------------------------------------
*/
#endif
