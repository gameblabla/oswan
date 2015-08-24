#ifndef __NEC_H__
#define __NEC_H__

#include "memory.h"
	// DEBUG TEXT
#include "menu.h"
#include "WSHard.h"

typedef enum { ES, CS, SS, DS } SREGS;
typedef enum { AW, CW, DW, BW, SP, BP, IX, IY } WREGS;

typedef enum { AL,AH,CL,CH,DL,DH,BL,BH,SPL,SPH,BPL,BPH,IXL,IXH,IYL,IYH } BREGS;

enum {
 NEC_IP=1, NEC_AW, NEC_CW, NEC_DW, NEC_BW, NEC_SP, NEC_BP, NEC_IX, NEC_IY,
 NEC_FLAGS, NEC_ES, NEC_CS, NEC_SS, NEC_DS,
 NEC_VECTOR, NEC_PENDING, NEC_NMI_STATE, NEC_IRQ_STATE };

extern int nec_ICount;

void nec_set_reg(void);
int nec_execute(int cycles);
void nec_get_reg(void);
void nec_reset (void *param);
void nec_int(unsigned long wektor);

/* Todo:  Remove these later - plus readword could overflow */
#define cpuReadByte(ea) (/*nec_ICount-=((ea)&1)?1:0,*/ (BYTE)cpu_readmem20((ea)))
#define cpuReadWord(ea) (/*nec_ICount-=((ea)&1)?1:0,*/ cpu_readmem20((ea))+(cpu_readmem20(((ea)+1))<<8))

#define cpuWriteByte(ea,val) { /*nec_ICount-=((ea)&1)?1:0*/; cpu_writemem20((ea),val); }
#define cpuWriteWord(ea,val) { /*nec_ICount-=((ea)&1)?1:0*/; cpu_writemem20((ea),(BYTE)(val)); cpu_writemem20(((ea)+1),(val)>>8); }

#define ioReadByte(ea) (/*nec_ICount-=((ea)&1)?1:0,*/ (BYTE)cpu_readmem20((ea)))
#define ioReadWord(ea) (/*nec_ICount-=((ea)&1)?1:0,*/ cpu_readmem20((ea))+(cpu_readmem20(((ea)+1))<<8))
#define ioWriteByte(ea,val) { /*nec_ICount-=((ea)&1)?1:0*/; cpu_writemem20((ea),val); }
#define ioWriteWord(ea,val) { /*nec_ICount-=((ea)&1)?1:0*/; cpu_writemem20((ea),(BYTE)(val)); cpu_writemem20(((ea)+1),(val)>>8); }

typedef union
{
    unsigned short w[8];
    unsigned char b[16];
} necbasicregs;

typedef struct
{
 necbasicregs regs;
  unsigned short sregs[4];

 unsigned short ip;

 signed long SignVal;
    unsigned long AuxVal, OverVal, ZeroVal, CarryVal, ParityVal;
 unsigned char TF, IF, DF, MF;
 unsigned long int_vector;
 unsigned long pending_irq;
 unsigned long nmi_state;
 unsigned long irq_state;
 int (*irq_callback)(int irqline);
} nec_Regs;

#endif
