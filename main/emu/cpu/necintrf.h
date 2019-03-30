/* ASG 971222 -- rewrote this interface */
#ifndef __NEC_H_
#define __NEC_H_

enum {
	NEC_IP=1, NEC_AW, NEC_CW, NEC_DW, NEC_BW, NEC_SP, NEC_BP, NEC_IX, NEC_IY,
	NEC_FLAGS, NEC_ES, NEC_CS, NEC_SS, NEC_DS,
	NEC_VECTOR, NEC_PENDING, NEC_NMI_STATE, NEC_IRQ_STATE };

/* Public variables */
extern int32_t nec_ICount;

/* Public functions */

void nec_set_reg(const int32_t regnum, const uint32_t val);
int32_t nec_execute(const int32_t cycles);
uint32_t nec_get_reg(const uint32_t regnum);
void nec_reset (void *param);
void nec_int(const uint32_t wektor);

#endif
