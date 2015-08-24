#include "nec.h"

#define s32 signed int
#define u32 unsigned long
#define u16 unsigned short
#define s8 signed char

s32 nec_ICount;
static nec_Regs I;
static u32 prefix_base;
s8 seg_prefix;

static void i_add_br8(void);
static void i_add_wr16(void);
static void i_add_r8b(void);
static void i_add_r16w(void);
static void i_add_ald8(void);
static void i_add_axd16(void);
static void i_push_es(void);
static void i_pop_es(void);
static void i_or_br8(void);
static void i_or_r8b(void);
static void i_or_wr16(void);
static void i_or_r16w(void);
static void i_or_ald8(void);
static void i_or_axd16(void);
static void i_push_cs(void);
static void i_pre_nec(void);
static void i_adc_br8(void);
static void i_adc_wr16(void);
static void i_adc_r8b(void);
static void i_adc_r16w(void);
static void i_adc_ald8(void);
static void i_adc_axd16(void);
static void i_push_ss(void);
static void i_pop_ss(void);
static void i_sbb_br8(void);
static void i_sbb_wr16(void);
static void i_sbb_r8b(void);
static void i_sbb_r16w(void);
static void i_sbb_ald8(void);
static void i_sbb_axd16(void);
static void i_push_ds(void);
static void i_pop_ds(void);
static void i_and_br8(void);
static void i_and_r8b(void);
static void i_and_wr16(void);
static void i_and_r16w(void);
static void i_and_ald8(void);
static void i_and_axd16(void);
static void i_es(void);
static void i_daa(void);
static void i_sub_br8(void);
static void i_sub_wr16(void);
static void i_sub_r8b(void);
static void i_sub_r16w(void);
static void i_sub_ald8(void);
static void i_sub_axd16(void);
static void i_cs(void);
static void i_das(void);
static void i_xor_br8(void);
static void i_xor_r8b(void);
static void i_xor_wr16(void);
static void i_xor_r16w(void);
static void i_xor_ald8(void);
static void i_xor_axd16(void);
static void i_ss(void);
static void i_aaa(void);
static void i_cmp_br8(void);
static void i_cmp_wr16(void);
static void i_cmp_r8b(void);
static void i_cmp_r16w(void);
static void i_cmp_ald8(void);
static void i_cmp_axd16(void);
static void i_ds(void);
static void i_aas(void);
static void i_inc_ax(void);
static void i_inc_cx(void);
static void i_inc_dx(void);
static void i_inc_bx(void);
static void i_inc_sp(void);
static void i_inc_bp(void);
static void i_inc_si(void);
static void i_inc_di(void);
static void i_dec_ax(void);
static void i_dec_cx(void);
static void i_dec_dx(void);
static void i_dec_bx(void);
static void i_dec_sp(void);
static void i_dec_bp(void);
static void i_dec_si(void);
static void i_dec_di(void);
static void i_push_ax(void);
static void i_push_cx(void);
static void i_push_dx(void);
static void i_push_bx(void);
static void i_push_sp(void);
static void i_push_bp(void);
static void i_push_si(void);
static void i_push_di(void);
static void i_pop_ax(void);
static void i_pop_cx(void);
static void i_pop_dx(void);
static void i_pop_bx(void);
static void i_pop_sp(void);
static void i_pop_bp(void);
static void i_pop_si(void);
static void i_pop_di(void);
static void i_pusha(void);
static void i_popa(void);
static void i_chkind(void);
static void i_repnc(void);
static void i_repc(void);
static void i_push_d16(void);
static void i_imul_d16(void);
static void i_push_d8(void);
static void i_imul_d8(void);
static void i_insb(void);
static void i_insw(void);
static void i_outsb(void);
static void i_outsw(void);
static void i_jo(void);
static void i_jno(void);
static void i_jc(void);
static void i_jnc(void);
static void i_jz(void);
static void i_jnz(void);
static void i_jce(void);
static void i_jnce(void);
static void i_js(void);
static void i_jns(void);
static void i_jp(void);
static void i_jnp(void);
static void i_jl(void);
static void i_jnl(void);
static void i_jle(void);
static void i_jnle(void);
static void i_80pre(void);
static void i_82pre(void);
static void i_81pre(void);
static void i_83pre(void);
static void i_test_br8(void);
static void i_test_wr16(void);
static void i_xchg_br8(void);
static void i_xchg_wr16(void);
static void i_mov_br8(void);
static void i_mov_r8b(void);
static void i_mov_wr16(void);
static void i_mov_r16w(void);
static void i_mov_wsreg(void);
static void i_lea(void);
static void i_mov_sregw(void);
static void i_invalid(void);
static void i_popw(void);
static void i_nop(void);
static void i_xchg_axcx(void);
static void i_xchg_axdx(void);
static void i_xchg_axbx(void);
static void i_xchg_axsp(void);
static void i_xchg_axbp(void);
static void i_xchg_axsi(void);
static void i_xchg_axdi(void);
static void i_cbw(void);
static void i_cwd(void);
static void i_call_far(void);
static void i_pushf(void);
static void i_popf(void);
static void i_sahf(void);
static void i_lahf(void);
static void i_mov_aldisp(void);
static void i_mov_axdisp(void);
static void i_mov_dispal(void);
static void i_mov_dispax(void);
static void i_movsb(void);
static void i_movsw(void);
static void i_cmpsb(void);
static void i_cmpsw(void);
static void i_test_ald8(void);
static void i_test_axd16(void);
static void i_stosb(void);
static void i_stosw(void);
static void i_lodsb(void);
static void i_lodsw(void);
static void i_scasb(void);
static void i_scasw(void);
static void i_mov_ald8(void);
static void i_mov_cld8(void);
static void i_mov_dld8(void);
static void i_mov_bld8(void);
static void i_mov_ahd8(void);
static void i_mov_chd8(void);
static void i_mov_dhd8(void);
static void i_mov_bhd8(void);
static void i_mov_axd16(void);
static void i_mov_cxd16(void);
static void i_mov_dxd16(void);
static void i_mov_bxd16(void);
static void i_mov_spd16(void);
static void i_mov_bpd16(void);
static void i_mov_sid16(void);
static void i_mov_did16(void);
static void i_rotshft_bd8(void);
static void i_rotshft_wd8(void);
static void i_ret_d16(void);
static void i_ret(void);
static void i_les_dw(void);
static void i_lds_dw(void);
static void i_mov_bd8(void);
static void i_mov_wd16(void);
static void i_enter(void);
static void i_leave(void);
static void i_retf_d16(void);
static void i_retf(void);
static void i_int3(void);
static void i_int(void);
static void i_into(void);
static void i_iret(void);
static void i_rotshft_b(void);
static void i_rotshft_w(void);
static void i_rotshft_bcl(void);
static void i_rotshft_wcl(void);
static void i_aam(void);
static void i_aad(void);
static void i_setalc(void);
static void i_trans(void);
static void i_fpo(void);
static void i_loopne(void);
static void i_loope(void);
static void i_loop(void);
static void i_jcxz(void);
static void i_inal(void);
static void i_inax(void);
static void i_outal(void);
static void i_outax(void);
static void i_call_d16(void);
static void i_jmp_d16(void);
static void i_jmp_far(void);
static void i_jmp_d8(void);
static void i_inaldx(void);
static void i_inaxdx(void);
static void i_outdxal(void);
static void i_outdxax(void);
static void i_lock(void);
static void i_repne(void);
static void i_repe(void);
static void i_hlt(void);
static void i_cmc(void);
static void i_f6pre(void);
static void i_f7pre(void);
static void i_clc(void);
static void i_stc(void);
static void i_di(void);
static void i_ei(void);
static void i_cld(void);
static void i_std(void);
static void i_fepre(void);
static void i_ffpre(void);
static void i_wait(void);

void (*nec_instruction[256])(void) =
{
    i_add_br8,
    i_add_wr16,
    i_add_r8b,
    i_add_r16w,
    i_add_ald8,
    i_add_axd16,
    i_push_es,
    i_pop_es,
    i_or_br8,
    i_or_wr16,
    i_or_r8b,
    i_or_r16w,
    i_or_ald8,
    i_or_axd16,
    i_push_cs,
	i_pre_nec,
    i_adc_br8,
    i_adc_wr16,
    i_adc_r8b,
    i_adc_r16w,
    i_adc_ald8,
    i_adc_axd16,
    i_push_ss,
    i_pop_ss,
    i_sbb_br8,
    i_sbb_wr16,
    i_sbb_r8b,
    i_sbb_r16w,
    i_sbb_ald8,
    i_sbb_axd16,
    i_push_ds,
    i_pop_ds,
    i_and_br8,
    i_and_wr16,
    i_and_r8b,
    i_and_r16w,
    i_and_ald8,
    i_and_axd16,
    i_es,
    i_daa,
    i_sub_br8,
    i_sub_wr16,
    i_sub_r8b,
    i_sub_r16w,
    i_sub_ald8,
    i_sub_axd16,
    i_cs,
    i_das,
    i_xor_br8,
    i_xor_wr16,
    i_xor_r8b,
    i_xor_r16w,
    i_xor_ald8,
    i_xor_axd16,
    i_ss,
    i_aaa,
    i_cmp_br8,
    i_cmp_wr16,
    i_cmp_r8b,
    i_cmp_r16w,
    i_cmp_ald8,
    i_cmp_axd16,
    i_ds,
    i_aas,
    i_inc_ax,
    i_inc_cx,
    i_inc_dx,
    i_inc_bx,
    i_inc_sp,
    i_inc_bp,
    i_inc_si,
    i_inc_di,
    i_dec_ax,
    i_dec_cx,
    i_dec_dx,
    i_dec_bx,
    i_dec_sp,
    i_dec_bp,
    i_dec_si,
    i_dec_di,
    i_push_ax,
    i_push_cx,
    i_push_dx,
    i_push_bx,
    i_push_sp,
    i_push_bp,
    i_push_si,
    i_push_di,
    i_pop_ax,
    i_pop_cx,
    i_pop_dx,
    i_pop_bx,
    i_pop_sp,
    i_pop_bp,
    i_pop_si,
    i_pop_di,
    i_pusha,
    i_popa,
    i_chkind,
    i_invalid,
    i_repnc,
    i_repc,
    i_invalid,
    i_invalid,
    i_push_d16,
    i_imul_d16,
    i_push_d8,
    i_imul_d8,
    i_insb,
    i_insw,
    i_outsb,
    i_outsw,
    i_jo,
    i_jno,
    i_jc,
    i_jnc,
    i_jz,
    i_jnz,
    i_jce,
    i_jnce,
    i_js,
    i_jns,
    i_jp,
    i_jnp,
    i_jl,
    i_jnl,
    i_jle,
    i_jnle,
    i_80pre,
    i_81pre,
	i_82pre,
    i_83pre,
    i_test_br8,
    i_test_wr16,
    i_xchg_br8,
    i_xchg_wr16,
    i_mov_br8,
    i_mov_wr16,
    i_mov_r8b,
    i_mov_r16w,
    i_mov_wsreg,
    i_lea,
    i_mov_sregw,
    i_popw,
    i_nop,
    i_xchg_axcx,
    i_xchg_axdx,
    i_xchg_axbx,
    i_xchg_axsp,
    i_xchg_axbp,
    i_xchg_axsi,
    i_xchg_axdi,
    i_cbw,
    i_cwd,
    i_call_far,
    i_wait,
    i_pushf,
    i_popf,
    i_sahf,
    i_lahf,
    i_mov_aldisp,
    i_mov_axdisp,
    i_mov_dispal,
    i_mov_dispax,
    i_movsb,
    i_movsw,
    i_cmpsb,
    i_cmpsw,
    i_test_ald8,
    i_test_axd16,
    i_stosb,
    i_stosw,
    i_lodsb,
    i_lodsw,
    i_scasb,
    i_scasw,
    i_mov_ald8,
    i_mov_cld8,
    i_mov_dld8,
    i_mov_bld8,
    i_mov_ahd8,
    i_mov_chd8,
    i_mov_dhd8,
    i_mov_bhd8,
    i_mov_axd16,
    i_mov_cxd16,
    i_mov_dxd16,
    i_mov_bxd16,
    i_mov_spd16,
    i_mov_bpd16,
    i_mov_sid16,
    i_mov_did16,
    i_rotshft_bd8,
    i_rotshft_wd8,
    i_ret_d16,
    i_ret,
    i_les_dw,
    i_lds_dw,
    i_mov_bd8,
    i_mov_wd16,
    i_enter,
    i_leave,
    i_retf_d16,
    i_retf,
    i_int3,
    i_int,
    i_into,
    i_iret,
    i_rotshft_b,
    i_rotshft_w,
    i_rotshft_bcl,
    i_rotshft_wcl,
    i_aam,
    i_aad,
    i_setalc,
    i_trans,
    i_fpo,
    i_fpo,
    i_fpo,
    i_fpo,
    i_fpo,
    i_fpo,
    i_fpo,
    i_fpo,
    i_loopne,
    i_loope,
    i_loop,
    i_jcxz,
    i_inal,
    i_inax,
    i_outal,
    i_outax,
    i_call_d16,
    i_jmp_d16,
    i_jmp_far,
    i_jmp_d8,
    i_inaldx,
    i_inaxdx,
    i_outdxal,
    i_outdxax,
    i_lock,
    i_invalid,
    i_repne,
    i_repe,
    i_hlt,
    i_cmc,
    i_f6pre,
    i_f7pre,
    i_clc,
    i_stc,
    i_di,
    i_ei,
    i_cld,
    i_std,
    i_fepre,
    i_ffpre,
};

static u32 EA;
static u16 EO;
static u16 E16;

static unsigned EA_000(void) { EO=I.regs.w[BW]+I.regs.w[IX]; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_001(void) { EO=I.regs.w[BW]+I.regs.w[IY]; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_002(void) { EO=I.regs.w[BP]+I.regs.w[IX]; EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_003(void) { EO=I.regs.w[BP]+I.regs.w[IY]; EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_004(void) { EO=I.regs.w[IX]; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_005(void) { EO=I.regs.w[IY]; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_006(void) { EO=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); EO+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_007(void) { EO=I.regs.w[BW]; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }

static unsigned EA_100(void) { EO=(I.regs.w[BW]+I.regs.w[IX]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_101(void) { EO=(I.regs.w[BW]+I.regs.w[IY]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_102(void) { EO=(I.regs.w[BP]+I.regs.w[IX]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_103(void) { EO=(I.regs.w[BP]+I.regs.w[IY]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_104(void) { EO=(I.regs.w[IX]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_105(void) { EO=(I.regs.w[IY]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_106(void) { EO=(I.regs.w[BP]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_107(void) { EO=(I.regs.w[BW]+(signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }

static unsigned EA_200(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BW]+I.regs.w[IX]+(signed short)E16; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_201(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BW]+I.regs.w[IY]+(signed short)E16; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_202(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BP]+I.regs.w[IX]+(signed short)E16; EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_203(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BP]+I.regs.w[IY]+(signed short)E16; EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_204(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[IX]+(signed short)E16; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_205(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[IY]+(signed short)E16; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }
static unsigned EA_206(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BP]+(signed short)E16; EA=((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+EO; return EA; }
static unsigned EA_207(void) { E16=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); E16+=(cpuReadByte((I.sregs[CS]<<4)+I.ip++))<<8; EO=I.regs.w[BW]+(signed short)E16; EA=((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+EO; return EA; }

static unsigned (*GetEA[192])(void)={
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
 EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,

 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
 EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,

 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
 EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207
};

static struct {
		
	struct {
		WREGS w[256];
		BREGS b[256];
	} reg;
		
	struct {
		WREGS w[256];
		BREGS b[256];
	} RM;	
		
} Mod_RM;
		
static int no_interrupt;
static unsigned char parity_table[256];
		
		// Functions
		
void nec_reset (void *param)
{
	u32 i, j, c;
	BREGS reg_name[8] = { AL, CL, DL, BL, AH, CH, DH, BH };
		
	memset( &I, 0, sizeof(I) );
		
	no_interrupt = 0;
	I.sregs[CS] = 0xFFFF;
		
	for (i=0; i<256; ++i) {
		for (j=i, c=0; j>0; j>>=1)
			c += j & 0x01;
			
		parity_table[i] = !(c & 1);
	}
		
	I.ZeroVal = I.ParityVal = 1;
	I.MF = 1;
		
    for (i = 0; i < 256; i++) {
		Mod_RM.reg.b[i] = reg_name[(i & 0x38) >> 3];
		Mod_RM.reg.w[i] = (WREGS)( (i & 0x38) >> 3) ;
    }
		
	for (i = 0xc0; i < 0x100; i++) {
		Mod_RM.RM.w[i] = (WREGS)( i & 7 );
		Mod_RM.RM.b[i] = (BREGS)reg_name[i & 7];
	}
		
	I.regs.w[SP] = 0x2000;
}

void nec_int(unsigned long wektor)
{
	u32 dest_seg, dest_off;
		
	if(I.IF) {
		i_pushf();
		I.TF = I.IF = 0;
			
		dest_off = cpuReadByte(wektor) | cpuReadByte((wektor)+1) << 8;
		dest_seg = cpuReadByte(wektor + 2) | cpuReadByte(wektor + 3) << 8;
			
		I.regs.w[SP] -= 2;
		cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP], I.sregs[CS]);
		cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP] + 1, I.sregs[CS] >> 8);
			
		I.regs.w[SP] -= 2;
		cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP], I.ip);
		cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP] + 1, I.ip >> 8);
			
		I.ip = dest_off;
		I.sregs[CS] = dest_seg;
	}
}

static void nec_interrupt(unsigned int_num,int md_flag)
{
	u32 dest_seg, dest_off;
		
	if (int_num == -1)
		return;
		
	i_pushf();
	I.TF = I.IF = 0;
		
	dest_off = cpuReadByte(int_num << 2) | cpuReadByte((int_num << 2) + 1) << 8;
	dest_seg = cpuReadByte((int_num << 2) + 2) | cpuReadByte(((int_num << 2) + 3) << 8);
		
	I.regs.w[SP] -= 2;
		
	cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP], I.sregs[CS]);
	cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP] + 1, I.sregs[CS] >> 8);
		
	I.regs.w[SP] -= 2;
		
	cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP], I.ip);
	cpuWriteByte((I.sregs[SS] << 4) + I.regs.w[SP] + 1, I.ip >> 8);
		
	I.ip = dest_off & 0xFFFF;
	I.sregs[CS] = dest_seg & 0xFFFF;
}

static void i_add_br8(void)
{
	u32 ModRM = cpuReadByte((I.sregs[CS] << 4) + I.ip++), src, dst;
	src = I.regs.b[Mod_RM.reg.b[ModRM]];
	dst = (ModRM) >= 0xC0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : cpuReadByte((*GetEA[ModRM])());
		
	u32 res = dst + src;
	I.CarryVal = res & 0x100;
	I.OverVal = (res ^ src) & (res ^ dst) & 0x80;
	I.AuxVal = (res ^ (src ^ dst)) & 0x10;
	I.SignVal = I.ZeroVal = I.ParityVal = (s8)res;
	dst = res & 0xFF;
		
	if(ModRM >= 0xC0) {
		I.regs.b[Mod_RM.RM.b[ModRM]] = dst;
		nec_ICount -= 1;
	} else {
		cpuWriteByte(EA, dst);
		nec_ICount -= 3;
	}
}

static void i_add_wr16(void)
{
	u32 ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst;
	src = I.regs.w[Mod_RM.reg.w[ModRM]];
	dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) ));
		
	u32 res = dst + src;
	I.CarryVal = res & 0x10000;
	I.OverVal = (res ^ src) & (res ^ dst) & 0x8000;
	I.AuxVal = (res ^ (src ^ dst)) & 0x10;
	I.SignVal = I.ZeroVal = I.ParityVal = (signed short)res;
	dst = res & 0xFFFF;
		
	if(ModRM >= 0xc0) {
		I.regs.w[Mod_RM.RM.w[ModRM]]=dst;
		nec_ICount -= 1;
	} else {
		cpuWriteByte(EA, dst);
		cpuWriteByte(EA+1, dst >> 8);
		nec_ICount -= 3;
	}
}

static void i_add_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_add_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_add_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_add_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_push_es(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.sregs[ES]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.sregs[ES])>>8); }; }; nec_ICount-=2; }
static void i_pop_es(void) { { I.sregs[ES] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=3; }

static void i_or_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_or_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_or_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_or_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_or_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_or_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_push_cs(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.sregs[CS]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.sregs[CS])>>8); }; }; nec_ICount-=2; }
static void i_pre_nec(void) { { I.sregs[CS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=2; }

static void i_adc_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_adc_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_adc_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_adc_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_adc_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_adc_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_push_ss(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.sregs[SS]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.sregs[SS])>>8); }; }; nec_ICount-=2; }
static void i_pop_ss(void) { { I.sregs[SS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=3; no_interrupt=1; }

static void i_sbb_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_sbb_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_sbb_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_sbb_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_sbb_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_sbb_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_push_ds(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.sregs[DS]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.sregs[DS])>>8); }; }; nec_ICount-=2; }
static void i_pop_ds(void) { { I.sregs[DS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=3; }

static void i_and_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_and_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_and_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_and_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_and_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_and_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_es(void) { seg_prefix=1; prefix_base=I.sregs[ES]<<4; nec_ICount-=1; nec_instruction[(cpuReadByte((I.sregs[CS]<<4)+I.ip++))](); seg_prefix=0; }
static void i_daa(void) { if ((I.AuxVal!=0) || ((I.regs.b[AL] & 0xf) > 9)) { int tmp; I.regs.b[AL] = tmp = I.regs.b[AL] + 6; I.AuxVal = 1; } if ((I.CarryVal!=0) || (I.regs.b[AL] > 0x9f)) { I.regs.b[AL] += 0x60; I.CarryVal = 1; } (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(I.regs.b[AL])); nec_ICount-=10; }

static void i_sub_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_sub_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_sub_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_sub_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_sub_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_sub_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_cs(void) { seg_prefix=1; prefix_base=I.sregs[CS]<<4; nec_ICount-=1; nec_instruction[(cpuReadByte((I.sregs[CS]<<4)+I.ip++))](); seg_prefix=0; }
static void i_das(void) { if ((I.AuxVal!=0) || ((I.regs.b[AL] & 0xf) > 9)) { int tmp; I.regs.b[AL] = tmp = I.regs.b[AL] + -6; I.AuxVal = 1; } if ((I.CarryVal!=0) || (I.regs.b[AL] > 0x9f)) { I.regs.b[AL] += -0x60; I.CarryVal = 1; } (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(I.regs.b[AL])); nec_ICount-=10; }

static void i_xor_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_xor_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_xor_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_xor_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_xor_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); I.regs.b[AL]=dst; nec_ICount-=1; }
static void i_xor_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); I.regs.w[AW]=dst; nec_ICount-=1; }
static void i_ss(void) { seg_prefix=1; prefix_base=I.sregs[SS]<<4; nec_ICount-=1; nec_instruction[(cpuReadByte((I.sregs[CS]<<4)+I.ip++))](); seg_prefix=0; }
static void i_aaa(void) { if ((I.AuxVal!=0) || ((I.regs.b[AL] & 0xf) > 9)) { I.regs.b[AL] += 6; I.regs.b[AH] += 1; I.AuxVal = 1; I.CarryVal = 1; } else { I.AuxVal = 0; I.CarryVal = 0; } I.regs.b[AL] &= 0x0F; nec_ICount-=9; }

static void i_cmp_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_cmp_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_cmp_r8b(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.b[Mod_RM.reg.b[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_cmp_r16w(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_cmp_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; nec_ICount-=1; }
static void i_cmp_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; nec_ICount-=1; }
static void i_ds(void) { seg_prefix=1; prefix_base=I.sregs[DS]<<4; nec_ICount-=1; nec_instruction[(cpuReadByte((I.sregs[CS]<<4)+I.ip++))](); seg_prefix=0; }
static void i_aas(void) { if ((I.AuxVal!=0) || ((I.regs.b[AL] & 0xf) > 9)) { I.regs.b[AL] += -6; I.regs.b[AH] += -1; I.AuxVal = 1; I.CarryVal = 1; } else { I.AuxVal = 0; I.CarryVal = 0; } I.regs.b[AL] &= 0x0F; nec_ICount-=9; }

static void i_inc_ax(void) { unsigned tmp = (unsigned)I.regs.w[AW]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[AW]=tmp1; nec_ICount-=1; }
static void i_inc_cx(void) { unsigned tmp = (unsigned)I.regs.w[CW]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[CW]=tmp1; nec_ICount-=1; }
static void i_inc_dx(void) { unsigned tmp = (unsigned)I.regs.w[DW]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[DW]=tmp1; nec_ICount-=1; }
static void i_inc_bx(void) { unsigned tmp = (unsigned)I.regs.w[BW]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[BW]=tmp1; nec_ICount-=1; }
static void i_inc_sp(void) { unsigned tmp = (unsigned)I.regs.w[SP]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[SP]=tmp1; nec_ICount-=1; }
static void i_inc_bp(void) { unsigned tmp = (unsigned)I.regs.w[BP]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[BP]=tmp1; nec_ICount-=1; }
static void i_inc_si(void) { unsigned tmp = (unsigned)I.regs.w[IX]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[IX]=tmp1; nec_ICount-=1; }
static void i_inc_di(void) { unsigned tmp = (unsigned)I.regs.w[IY]; unsigned tmp1 = tmp+1; I.OverVal = (tmp == 0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[IY]=tmp1; nec_ICount-=1; }

static void i_dec_ax(void) { unsigned tmp = (unsigned)I.regs.w[AW]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[AW]=tmp1; nec_ICount-=1; }
static void i_dec_cx(void) { unsigned tmp = (unsigned)I.regs.w[CW]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[CW]=tmp1; nec_ICount-=1; }
static void i_dec_dx(void) { unsigned tmp = (unsigned)I.regs.w[DW]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[DW]=tmp1; nec_ICount-=1; }
static void i_dec_bx(void) { unsigned tmp = (unsigned)I.regs.w[BW]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[BW]=tmp1; nec_ICount-=1; }
static void i_dec_sp(void) { unsigned tmp = (unsigned)I.regs.w[SP]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[SP]=tmp1; nec_ICount-=1; }
static void i_dec_bp(void) { unsigned tmp = (unsigned)I.regs.w[BP]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[BP]=tmp1; nec_ICount-=1; }
static void i_dec_si(void) { unsigned tmp = (unsigned)I.regs.w[IX]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[IX]=tmp1; nec_ICount-=1; }
static void i_dec_di(void) { unsigned tmp = (unsigned)I.regs.w[IY]; unsigned tmp1 = tmp-1; I.OverVal = (tmp == 0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); I.regs.w[IY]=tmp1; nec_ICount-=1; }

static void i_push_ax(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[AW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[AW])>>8); }; }; nec_ICount-=1; }
static void i_push_cx(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[CW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[CW])>>8); }; }; nec_ICount-=1; }
static void i_push_dx(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[DW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[DW])>>8); }; }; nec_ICount-=1; }
static void i_push_bx(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BW])>>8); }; }; nec_ICount-=1; }
static void i_push_sp(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[SP]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[SP])>>8); }; }; nec_ICount-=1; }
static void i_push_bp(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BP]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BP])>>8); }; }; nec_ICount-=1; }
static void i_push_si(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[IX]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[IX])>>8); }; }; nec_ICount-=1; }
static void i_push_di(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[IY]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[IY])>>8); }; }; nec_ICount-=1; }

static void i_pop_ax(void) { { I.regs.w[AW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_cx(void) { { I.regs.w[CW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_dx(void) { { I.regs.w[DW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_bx(void) { { I.regs.w[BW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_sp(void) { { I.regs.w[SP] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_bp(void) { { I.regs.w[BP] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_si(void) { { I.regs.w[IX] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }
static void i_pop_di(void) { { I.regs.w[IY] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=1; }

static void i_pusha(void) {
 unsigned tmp=I.regs.w[SP];
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[AW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[AW])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[CW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[CW])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[DW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[DW])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BW]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BW])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),tmp); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(tmp)>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BP]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BP])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[IX]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[IX])>>8); }; };
 { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[IY]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[IY])>>8); }; };
 nec_ICount-=9;
}
static void i_popa(void) {
    unsigned tmp;
 { I.regs.w[IY] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[IX] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[BP] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
    { tmp = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[BW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[DW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[CW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 { I.regs.w[AW] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 nec_ICount-=8;
}
static void i_chkind(void) {
 unsigned long low,high,tmp;
 unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++);
    low = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) ));
    high= (cpuReadByte((EA&0xf0000)|((EA+2)&0xffff)) | (cpuReadByte(((EA&0xf0000)|((EA+2)&0xffff))+1)<<8));
    tmp= I.regs.w[Mod_RM.reg.w[ModRM]];
    if (tmp<low || tmp>high) {
  nec_interrupt(5,0);
  nec_ICount-=7;
    }
  nec_ICount-=13;
}


static void i_repnc(void) { }
static void i_repc(void) { }

static void i_push_d16(void) { unsigned long tmp; { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),tmp); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(tmp)>>8); }; }; nec_ICount-=1; }
static void i_imul_d16(void) { unsigned long tmp; unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; dst = (signed long)((signed short)src)*(signed long)((signed short)tmp); I.CarryVal = I.OverVal = (((signed long)dst) >> 15 != 0) && (((signed long)dst) >> 15 != -1); I.regs.w[Mod_RM.reg.w[ModRM]]=(unsigned short)dst; { nec_ICount-=( ModRM >=0xc0 )?3:4; };}
static void i_push_d8(void) { unsigned long tmp = (unsigned short)((signed short)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)))); { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),tmp); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(tmp)>>8); }; }; nec_ICount-=1; }
static void i_imul_d8(void) { unsigned long src2; unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; dst = I.regs.w[Mod_RM.reg.w[ModRM]]; src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src2= (unsigned short)((signed short)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)))); dst = (signed long)((signed short)src)*(signed long)((signed short)src2); I.CarryVal = I.OverVal = (((signed long)dst) >> 15 != 0) && (((signed long)dst) >> 15 != -1); I.regs.w[Mod_RM.reg.w[ModRM]]=(unsigned short)dst; { nec_ICount-=( ModRM >=0xc0 )?3:4; }; }
static void i_insb(void) { { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),(ioReadByte(I.regs.w[DW]))); }; I.regs.w[IY]+= -2 * I.DF + 1; nec_ICount-=6; }
static void i_insw(void) { { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),(ioReadByte(I.regs.w[DW]))); }; { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+((I.regs.w[IY]+1)&0xffff)),(ioReadByte((I.regs.w[DW]+1)&0xffff))); }; I.regs.w[IY]+= -4 * I.DF + 2; nec_ICount-=6; }
static void i_outsb(void) { ioWriteByte(I.regs.w[DW], (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX]))))); I.regs.w[IX]+= -2 * I.DF + 1; nec_ICount-=7; }
static void i_outsw(void) { ioWriteByte(I.regs.w[DW], (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX]))))); ioWriteByte((I.regs.w[DW]+1)&0xffff, (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((I.regs.w[IX]+1)&0xffff))))); I.regs.w[IX]+= -4 * I.DF + 2; nec_ICount-=7; }

static void i_jo(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if ((I.OverVal!=0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jno(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!(I.OverVal!=0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jc(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if ((I.CarryVal!=0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnc(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!(I.CarryVal!=0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jz(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (I.ZeroVal==0) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnz(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!(I.ZeroVal==0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jce(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if ((I.CarryVal!=0) || (I.ZeroVal==0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnce(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!((I.CarryVal!=0) || (I.ZeroVal==0))) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_js(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if ((I.SignVal<0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jns(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!(I.SignVal<0)) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jp(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (parity_table[(unsigned char)I.ParityVal]) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnp(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (!parity_table[(unsigned char)I.ParityVal]) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jl(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (((I.SignVal<0)!=(I.OverVal!=0))&&(!(I.ZeroVal==0))) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnl(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (((I.ZeroVal==0))||((I.SignVal<0)==(I.OverVal!=0))) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jle(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (((I.ZeroVal==0))||((I.SignVal<0)!=(I.OverVal!=0))) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }
static void i_jnle(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); if (((I.SignVal<0)==(I.OverVal!=0))&&(!(I.ZeroVal==0))) { I.ip = (unsigned short)(I.ip+tmp); nec_ICount -= 3; return; }; nec_ICount-=1; }

static void i_80pre(void) { unsigned long dst, src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++));
 { nec_ICount-=( ModRM >=0xc0 )?1:3; }
 switch (ModRM & 0x38) {
     case 0x00: { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x08: dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x10: src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x18: src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
  case 0x20: dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x28: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x30: dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x38: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; break;
    }
}

static void i_81pre(void) { unsigned long dst, src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); src+= ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8);
 { nec_ICount-=( ModRM >=0xc0 )?1:3; }
    switch (ModRM & 0x38) {
     case 0x00: { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x08: dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x10: src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x18: src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
  case 0x20: dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x28: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x30: dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x38: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; break;
    }
}

static void i_82pre(void) { unsigned long dst, src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); src = (unsigned char)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)));
 { nec_ICount-=( ModRM >=0xc0 )?1:3; }
 switch (ModRM & 0x38) {
     case 0x00: { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x08: dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x10: src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x18: src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
  case 0x20: dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x28: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x30: dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=dst; else {cpuWriteByte((EA),dst); }; }; break;
     case 0x38: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; break;
    }
}

static void i_83pre(void) { unsigned long dst, src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); src = (unsigned short)((signed short)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))));
 { nec_ICount-=( ModRM >=0xc0 )?1:3; }
    switch (ModRM & 0x38) {
     case 0x00: { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x08: dst|=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x10: src+=(I.CarryVal!=0); { unsigned long res=dst+src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((res) ^ (src)) & ((res) ^ (dst)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x18: src+=(I.CarryVal!=0); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
  case 0x20: dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x28: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x30: dst^=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=dst; else {cpuWriteByte((EA),dst); cpuWriteByte(((EA)+1),(dst)>>8); }; }; break;
     case 0x38: { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; break;
    }
}

static void i_test_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_test_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { nec_ICount-=( ModRM >=0xc0 )?1:2; }; }
static void i_xchg_br8(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.b[Mod_RM.reg.b[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); I.regs.b[Mod_RM.reg.b[ModRM]]=dst; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=src; else {cpuWriteByte((EA),src); }; }; { nec_ICount-=( ModRM >=0xc0 )?3:5; }; }
static void i_xchg_wr16(void) { unsigned long ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)),src,dst; src = I.regs.w[Mod_RM.reg.w[ModRM]]; dst = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); I.regs.w[Mod_RM.reg.w[ModRM]]=dst; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=src; else {cpuWriteByte((EA),src); cpuWriteByte(((EA)+1),(src)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?3:5; }; }

static void i_mov_br8(void) { unsigned char src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = I.regs.b[Mod_RM.reg.b[ModRM]]; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=src; else {cpuWriteByte(((*GetEA[ModRM])()),src); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:1; }; }
static void i_mov_wr16(void) { unsigned short src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = I.regs.w[Mod_RM.reg.w[ModRM]]; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=src; else { (*GetEA[ModRM])(); {cpuWriteByte((EA),src); cpuWriteByte(((EA)+1),(src)>>8); }; } }; { nec_ICount-=( ModRM >=0xc0 )?1:1; }; }
static void i_mov_r8b(void) { unsigned char src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); I.regs.b[Mod_RM.reg.b[ModRM]]=src; { nec_ICount-=( ModRM >=0xc0 )?1:1; }; }
static void i_mov_r16w(void) { unsigned short src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); I.regs.w[Mod_RM.reg.w[ModRM]]=src; { nec_ICount-=( ModRM >=0xc0 )?1:1; }; }
static void i_mov_wsreg(void) { unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=I.sregs[(ModRM & 0x38) >> 3]; else { (*GetEA[ModRM])(); {cpuWriteByte((EA),I.sregs[(ModRM & 0x38) >> 3]); cpuWriteByte(((EA)+1),(I.sregs[(ModRM & 0x38) >> 3])>>8); }; } }; { nec_ICount-=( ModRM >=0xc0 )?1:1; }; }
static void i_lea(void) { unsigned short ModRM = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); (void)(*GetEA[ModRM])(); I.regs.w[Mod_RM.reg.w[ModRM]]=EO; nec_ICount-=1; }
static void i_mov_sregw(void) { unsigned short src; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); { nec_ICount-=( ModRM >=0xc0 )?2:3; };
    switch (ModRM & 0x38) {
     case 0x00: I.sregs[ES] = src; break;
  case 0x08: I.sregs[CS] = src; break;
     case 0x10: I.sregs[SS] = src; break;
     case 0x18: I.sregs[DS] = src; break;
  default: ;
    }
 no_interrupt=1;
}
static void i_popw(void) { unsigned short tmp; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); { tmp = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=tmp; else { (*GetEA[ModRM])(); {cpuWriteByte((EA),tmp); cpuWriteByte(((EA)+1),(tmp)>>8); }; } }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; }
static void i_nop(void) { nec_ICount-=1;

 if (no_interrupt==0 && nec_ICount>0 && ((cpuReadByte((I.sregs[CS]<<4)+I.ip)))==0xeb && ((cpuReadByte((I.sregs[CS]<<4)+I.ip+1)))==0xfd)
  nec_ICount%=15;
}
static void i_xchg_axcx(void) { I.regs.w[CW] ^= (I.regs.w[AW] ^= (I.regs.w[CW] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axdx(void) { I.regs.w[DW] ^= (I.regs.w[AW] ^= (I.regs.w[DW] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axbx(void) { I.regs.w[BW] ^= (I.regs.w[AW] ^= (I.regs.w[BW] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axsp(void) { I.regs.w[SP] ^= (I.regs.w[AW] ^= (I.regs.w[SP] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axbp(void) { I.regs.w[BP] ^= (I.regs.w[AW] ^= (I.regs.w[BP] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axsi(void) { I.regs.w[IX] ^= (I.regs.w[AW] ^= (I.regs.w[IX] ^= I.regs.w[AW])); nec_ICount-=3; }
static void i_xchg_axdi(void) { I.regs.w[IY] ^= (I.regs.w[AW] ^= (I.regs.w[IY] ^= I.regs.w[AW])); nec_ICount-=3; }

static void i_cbw(void) { I.regs.b[AH] = (I.regs.b[AL] & 0x80) ? 0xff : 0; nec_ICount-=1; }
static void i_cwd(void) { I.regs.w[DW] = (I.regs.b[AH] & 0x80) ? 0xffff : 0; nec_ICount-=1; }
static void i_call_far(void) { unsigned long tmp, tmp2; { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { tmp2=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.sregs[CS]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.sregs[CS])>>8); }; }; { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.ip); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.ip)>>8); }; }; I.ip = (unsigned short)tmp; I.sregs[CS] = (unsigned short)tmp2; nec_ICount-=10; }
static void i_wait(void) {}
static void i_pushf(void) { { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),(unsigned short)((I.CarryVal!=0) | (parity_table[(unsigned char)I.ParityVal] << 2) | ((I.AuxVal!=0) << 4) | ((I.ZeroVal==0) << 6) | ((I.SignVal<0) << 7) | (I.TF << 8) | (I.IF << 9) | (I.DF << 10) | ((I.OverVal!=0) << 11))); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),((unsigned short)((I.CarryVal!=0) | (parity_table[(unsigned char)I.ParityVal] << 2) | ((I.AuxVal!=0) << 4) | ((I.ZeroVal==0) << 6) | ((I.SignVal<0) << 7) | (I.TF << 8) | (I.IF << 9) | (I.DF << 10) | ((I.OverVal!=0) << 11)))>>8); }; }; nec_ICount-=2; }
static void i_popf(void) { unsigned long tmp; { tmp = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; { I.CarryVal = (tmp) & 1; I.ParityVal = !((tmp) & 4); I.AuxVal = (tmp) & 16; I.ZeroVal = !((tmp) & 64); I.SignVal = (tmp) & 128 ? -1 : 0; I.TF = ((tmp) & 256) == 256; I.IF = ((tmp) & 512) == 512; I.DF = ((tmp) & 1024) == 1024; I.OverVal = (tmp) & 2048; I.MF = ((tmp) & 0x8000) == 0x8000; }; nec_ICount-=3;}
static void i_sahf(void) { unsigned long tmp = ((unsigned short)((I.CarryVal!=0) | (parity_table[(unsigned char)I.ParityVal] << 2) | ((I.AuxVal!=0) << 4) | ((I.ZeroVal==0) << 6) | ((I.SignVal<0) << 7) | (I.TF << 8) | (I.IF << 9) | (I.DF << 10) | ((I.OverVal!=0) << 11)) & 0xff00) | (I.regs.b[AH] & 0xd5); { I.CarryVal = (tmp) & 1; I.ParityVal = !((tmp) & 4); I.AuxVal = (tmp) & 16; I.ZeroVal = !((tmp) & 64); I.SignVal = (tmp) & 128 ? -1 : 0; I.TF = ((tmp) & 256) == 256; I.IF = ((tmp) & 512) == 512; I.DF = ((tmp) & 1024) == 1024; I.OverVal = (tmp) & 2048; I.MF = ((tmp) & 0x8000) == 0x8000; }; nec_ICount-=4; }
static void i_lahf(void) { I.regs.b[AH] = (unsigned short)((I.CarryVal!=0) | (parity_table[(unsigned char)I.ParityVal] << 2) | ((I.AuxVal!=0) << 4) | ((I.ZeroVal==0) << 6) | ((I.SignVal<0) << 7) | (I.TF << 8) | (I.IF << 9) | (I.DF << 10) | ((I.OverVal!=0) << 11)) & 0xff; nec_ICount-=2; }

static void i_mov_aldisp(void) { unsigned long addr; { addr=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; I.regs.b[AL] = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(addr)))); nec_ICount-=1; }
static void i_mov_axdisp(void) { unsigned long addr; { addr=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; I.regs.b[AL] = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(addr)))); I.regs.b[AH] = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((addr+1)&0xffff)))); nec_ICount-=1; }
static void i_mov_dispal(void) { unsigned long addr; { addr=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { cpuWriteByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(addr)),(I.regs.b[AL])); }; nec_ICount-=1; }
static void i_mov_dispax(void) { unsigned long addr; { addr=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { cpuWriteByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(addr)),(I.regs.b[AL])); }; { cpuWriteByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((addr+1)&0xffff)),(I.regs.b[AH])); }; nec_ICount-=1; }
static void i_movsb(void) { unsigned long tmp = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX])))); { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),(tmp)); }; I.regs.w[IY] += -2 * I.DF + 1; I.regs.w[IX] += -2 * I.DF + 1; nec_ICount-=5; }
static void i_movsw(void) { unsigned long tmp = ((unsigned short)cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX]))) + (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((I.regs.w[IX])+1)))<<8) ); { { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),((tmp)&0xFF)); }; { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+((I.regs.w[IY])+1)),((tmp)>>8)); }; }; I.regs.w[IY] += -4 * I.DF + 2; I.regs.w[IX] += -4 * I.DF + 2; nec_ICount-=5; }
static void i_cmpsb(void) { unsigned long src = (cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])))); unsigned long dst = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX])))); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.w[IY] += -2 * I.DF + 1; I.regs.w[IX] += -2 * I.DF + 1; nec_ICount-=6; }
static void i_cmpsw(void) { unsigned long src = ((unsigned short)cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY]))) + (cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+((I.regs.w[IY])+1)))<<8) ); unsigned long dst = ((unsigned short)cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX]))) + (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((I.regs.w[IX])+1)))<<8) ); { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[IY] += -4 * I.DF + 2; I.regs.w[IX] += -4 * I.DF + 2; nec_ICount-=6; }

static void i_test_ald8(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.b[AL]; dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); nec_ICount-=1; }
static void i_test_axd16(void) { unsigned long src = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned long dst = I.regs.w[AW]; src += ((cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8); dst&=src; I.CarryVal=I.OverVal=I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); nec_ICount-=1; }
static void i_stosb(void) { { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),(I.regs.b[AL])); }; I.regs.w[IY] += -2 * I.DF + 1; nec_ICount-=3; }
static void i_stosw(void) { { { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])),((I.regs.w[AW])&0xFF)); }; { cpuWriteByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+((I.regs.w[IY])+1)),((I.regs.w[AW])>>8)); }; }; I.regs.w[IY] += -4 * I.DF + 2; nec_ICount-=3; }
static void i_lodsb(void) { I.regs.b[AL] = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX])))); I.regs.w[IX] += -2 * I.DF + 1; nec_ICount-=3; }
static void i_lodsw(void) { I.regs.w[AW] = ((unsigned short)cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(I.regs.w[IX]))) + (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+((I.regs.w[IX])+1)))<<8) ); I.regs.w[IX] += -4 * I.DF + 2; nec_ICount-=3; }
static void i_scasb(void) { unsigned long src = (cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY])))); unsigned long dst = I.regs.b[AL]; { unsigned long res=dst-src; (I.CarryVal = (res) & 0x100); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x80); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(res)); dst=(unsigned char)res; }; I.regs.w[IY] += -2 * I.DF + 1; nec_ICount-=4; }
static void i_scasw(void) { unsigned long src = ((unsigned short)cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+(I.regs.w[IY]))) + (cpuReadByte((((seg_prefix && (ES==DS || ES==SS)) ? prefix_base : I.sregs[ES] << 4)+((I.regs.w[IY])+1)))<<8) ); unsigned long dst = I.regs.w[AW]; { unsigned long res=dst-src; (I.CarryVal = (res) & 0x10000); (I.OverVal = ((dst) ^ (src)) & ((dst) ^ (res)) & 0x8000); (I.AuxVal = ((res) ^ ((src) ^ (dst))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(res)); dst=(unsigned short)res; }; I.regs.w[IY] += -4 * I.DF + 2; nec_ICount-=4; }

static void i_mov_ald8(void) { I.regs.b[AL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_cld8(void) { I.regs.b[CL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_dld8(void) { I.regs.b[DL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_bld8(void) { I.regs.b[BL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_ahd8(void) { I.regs.b[AH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_chd8(void) { I.regs.b[CH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_dhd8(void) { I.regs.b[DH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_bhd8(void) { I.regs.b[BH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }

static void i_mov_axd16(void) { I.regs.b[AL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[AH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_cxd16(void) { I.regs.b[CL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[CH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_dxd16(void) { I.regs.b[DL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[DH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_bxd16(void) { I.regs.b[BL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[BH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_spd16(void) { I.regs.b[SPL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[SPH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_bpd16(void) { I.regs.b[BPL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[BPH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_sid16(void) { I.regs.b[IXL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[IXH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }
static void i_mov_did16(void) { I.regs.b[IYL] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[IYH] = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=1; }

static void i_rotshft_bd8(void) {
 unsigned long src, dst; unsigned char c;
 unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned)((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst=src;
 c=(cpuReadByte((I.sregs[CS]<<4)+I.ip++));
 c&=0x1f;
 { nec_ICount-=( ModRM >=0xc0 )?3:5; };
 if (c) switch (ModRM & 0x38) {
  case 0x00: do { I.CarryVal = dst & 0x80; dst = (dst << 1)+(I.CarryVal!=0); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x08: do { I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<7); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x10: do { dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x100); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x18: do { dst = ((I.CarryVal!=0)<<8)+dst; I.CarryVal = dst & 0x01; dst >>= 1; c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x20: dst <<= c; (I.CarryVal = (dst) & 0x100); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.AuxVal = 1; break;
  case 0x28: dst >>= c-1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.AuxVal = 1; break;
  case 0x30: break;
  case 0x38: dst = ((signed char)dst) >> (c-1); I.CarryVal = dst & 0x1; dst = ((signed char)((unsigned char)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
 }
}

static void i_rotshft_wd8(void) {
 unsigned long src, dst; unsigned char c;
 unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned)((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst=src;
 c=(cpuReadByte((I.sregs[CS]<<4)+I.ip++));
 c&=0x1f;
 { nec_ICount-=( ModRM >=0xc0 )?3:5; };
    if (c) switch (ModRM & 0x38) {
  case 0x00: do { I.CarryVal = dst & 0x8000; dst = (dst << 1)+(I.CarryVal!=0); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x08: do { I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<15); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x10: do { dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x10000); c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x18: do { dst = ((I.CarryVal!=0)<<16)+dst; I.CarryVal = dst & 0x01; dst >>= 1; c--; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x20: dst <<= c; (I.CarryVal = (dst) & 0x10000); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1; break;
  case 0x28: dst >>= c-1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1; break;
  case 0x30: break;
  case 0x38: dst = ((signed short)dst) >> (c-1); I.CarryVal = dst & 0x1; dst = ((signed short)((unsigned short)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
 }
}

static void i_ret_d16(void) { unsigned long count = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); count += (cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8; { I.ip = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; I.regs.w[SP]+=count; nec_ICount-=6; }
static void i_ret(void) { { I.ip = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=6; }
static void i_les_dw(void) { unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); unsigned short tmp = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); I.regs.w[Mod_RM.reg.w[ModRM]]=tmp; I.sregs[ES] = (cpuReadByte((EA&0xf0000)|((EA+2)&0xffff)) | (cpuReadByte(((EA&0xf0000)|((EA+2)&0xffff))+1)<<8)); nec_ICount-=6; }
static void i_lds_dw(void) { unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); unsigned short tmp = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); I.regs.w[Mod_RM.reg.w[ModRM]]=tmp; I.sregs[DS] = (cpuReadByte((EA&0xf0000)|((EA+2)&0xffff)) | (cpuReadByte(((EA&0xf0000)|((EA+2)&0xffff))+1)<<8)); nec_ICount-=6; }
static void i_mov_bd8(void) { unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); else { (*GetEA[ModRM])(); {cpuWriteByte((EA),(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); }; } }; nec_ICount-=1; }
static void i_mov_wd16(void) { unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); { unsigned short val; if (ModRM >= 0xc0) { I.regs.w[Mod_RM.RM.w[ModRM]]=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; } else { (*GetEA[ModRM])(); { val=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; } {cpuWriteByte((EA),val); cpuWriteByte(((EA)+1),(val)>>8); }; } }; nec_ICount-=1; }

static void i_enter(void) {
    unsigned long nb = (cpuReadByte((I.sregs[CS]<<4)+I.ip++));
    unsigned long i,level;

    nec_ICount-=19;
    nb += (cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8;
    level = (cpuReadByte((I.sregs[CS]<<4)+I.ip++));
    { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BP]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BP])>>8); }; };
    I.regs.w[BP]=I.regs.w[SP];
    I.regs.w[SP] -= nb;
    for (i=1;i<level;i++) {
  { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),((unsigned short)cpuReadByte((((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+(I.regs.w[BP]-i*2))) + (cpuReadByte((((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+((I.regs.w[BP]-i*2)+1)))<<8) )); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(((unsigned short)cpuReadByte((((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+(I.regs.w[BP]-i*2))) + (cpuReadByte((((seg_prefix && (SS==DS)) ? prefix_base : I.sregs[SS] << 4)+((I.regs.w[BP]-i*2)+1)))<<8) ))>>8); }; };
  nec_ICount-=4;
    }
    if (level) { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.regs.w[BP]); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.regs.w[BP])>>8); }; };
}
static void i_leave(void) {
 I.regs.w[SP]=I.regs.w[BP];
 { I.regs.w[BP] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; };
 nec_ICount-=2;
}
static void i_retf_d16(void) { unsigned long count = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); count += (cpuReadByte((I.sregs[CS]<<4)+I.ip++)) << 8; { I.ip = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; { I.sregs[CS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; I.regs.w[SP]+=count; nec_ICount-=9; }
static void i_retf(void) { { I.ip = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; { I.sregs[CS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; nec_ICount-=8; }
static void i_int3(void) { nec_interrupt(3,0); nec_ICount-=9; }
static void i_int(void) { nec_interrupt((cpuReadByte((I.sregs[CS]<<4)+I.ip++)),0); nec_ICount-=10; }
static void i_into(void) { if ((I.OverVal!=0)) { nec_interrupt(4,0); nec_ICount-=13; } else nec_ICount-=6; }
static void i_iret(void) { { I.ip = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; { I.sregs[CS] = (cpuReadByte((((I.sregs[SS]<<4)+I.regs.w[SP]))) | (cpuReadByte(((((I.sregs[SS]<<4)+I.regs.w[SP])))+1)<<8)); I.regs.w[SP]+=2; }; i_popf(); nec_ICount-=10; }

static void i_rotshft_b(void) {
 unsigned long src, dst; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned long)((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst=src;
 { nec_ICount-=( ModRM >=0xc0 )?1:3; };
    switch (ModRM & 0x38) {
  case 0x00: I.CarryVal = dst & 0x80; dst = (dst << 1)+(I.CarryVal!=0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80; break;
  case 0x08: I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<7); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80; break;
  case 0x10: dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x100); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80; break;
  case 0x18: dst = ((I.CarryVal!=0)<<8)+dst; I.CarryVal = dst & 0x01; dst >>= 1; { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80; break;
  case 0x20: dst <<= 1; (I.CarryVal = (dst) & 0x100); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80;I.AuxVal = 1; break;
  case 0x28: dst >>= 1 -1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = (src^dst)&0x80;I.AuxVal = 1; break;
  case 0x30: break;
  case 0x38: dst = ((signed char)dst) >> (1 -1); I.CarryVal = dst & 0x1; dst = ((signed char)((unsigned char)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.OverVal = 0; break;
 }
}

static void i_rotshft_w(void) {
 unsigned long src, dst; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned long)((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst=src;
 { nec_ICount-=( ModRM >=0xc0 )?1:3; };
 switch (ModRM & 0x38) {
  case 0x00: I.CarryVal = dst & 0x8000; dst = (dst << 1)+(I.CarryVal!=0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.OverVal = (src^dst)&0x8000; break;
  case 0x08: I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<15); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.OverVal = (src^dst)&0x8000; break;
  case 0x10: dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x10000); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.OverVal = (src^dst)&0x8000; break;
  case 0x18: dst = ((I.CarryVal!=0)<<16)+dst; I.CarryVal = dst & 0x01; dst >>= 1; { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.OverVal = (src^dst)&0x8000; break;
  case 0x20: dst <<= 1; (I.CarryVal = (dst) & 0x10000); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1;I.OverVal = (src^dst)&0x8000; break;
  case 0x28: dst >>= 1 -1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1;I.OverVal = (src^dst)&0x8000; break;
  case 0x30: break;
  case 0x38: dst = ((signed short)dst) >> (1 -1); I.CarryVal = dst & 0x1; dst = ((signed short)((unsigned short)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1;I.OverVal = 0; break;
 }
}

static void i_rotshft_bcl(void) {
 unsigned long src, dst; unsigned char c; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned long)((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])()))); dst=src;
 c=I.regs.b[CL];
 { nec_ICount-=( ModRM >=0xc0 )?3:5; };
 c&=0x1f;
 if (c) switch (ModRM & 0x38) {
  case 0x00: do { I.CarryVal = dst & 0x80; dst = (dst << 1)+(I.CarryVal!=0); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x08: do { I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<7); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x10: do { dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x100); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x18: do { dst = ((I.CarryVal!=0)<<8)+dst; I.CarryVal = dst & 0x01; dst >>= 1; c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
  case 0x20: dst <<= c; (I.CarryVal = (dst) & 0x100); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.AuxVal = 1; break;
  case 0x28: dst >>= c-1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; I.AuxVal = 1;break;
  case 0x30: break;
  case 0x38: dst = ((signed char)dst) >> (c-1); I.CarryVal = dst & 0x1; dst = ((signed char)((unsigned char)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(dst)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)dst; else {cpuWriteByte((EA),(unsigned char)dst); }; }; break;
 }
}

static void i_rotshft_wcl(void) {
 unsigned long src, dst; unsigned char c; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); src = (unsigned long)((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) )); dst=src;
 c=I.regs.b[CL];
 c&=0x1f;
 { nec_ICount-=( ModRM >=0xc0 )?3:5; };
    if (c) switch (ModRM & 0x38) {
  case 0x00: do { I.CarryVal = dst & 0x8000; dst = (dst << 1)+(I.CarryVal!=0); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x08: do { I.CarryVal = dst & 0x1; dst = (dst >> 1)+((I.CarryVal!=0)<<15); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x10: do { dst = (dst << 1) + (I.CarryVal!=0); (I.CarryVal = (dst) & 0x10000); c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x18: do { dst = ((I.CarryVal!=0)<<16)+dst; I.CarryVal = dst & 0x01; dst >>= 1; c--; nec_ICount-=1; } while (c>0); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
  case 0x20: dst <<= c; (I.CarryVal = (dst) & 0x10000); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1; break;
  case 0x28: dst >>= c-1; I.CarryVal = dst & 0x1; dst >>= 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; I.AuxVal = 1; break;
  case 0x30: break;
  case 0x38: dst = ((signed short)dst) >> (c-1); I.CarryVal = dst & 0x1; dst = ((signed short)((unsigned short)dst)) >> 1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(dst)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)dst; else {cpuWriteByte((EA),(unsigned short)dst); cpuWriteByte(((EA)+1),((unsigned short)dst)>>8); }; }; break;
 }
}

static void i_aam(void) { unsigned long mult=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); mult=0; I.regs.b[AH] = I.regs.b[AL] / 10; I.regs.b[AL] %= 10; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(I.regs.w[AW])); nec_ICount-=17; }
static void i_aad(void) { unsigned long mult=(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); mult=0; I.regs.b[AL] = I.regs.b[AH] * 10 + I.regs.b[AL]; I.regs.b[AH] = 0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(I.regs.b[AL])); nec_ICount-=6; }
static void i_setalc(void) { I.regs.b[AL] = ((I.CarryVal!=0))?0xff:0x00; nec_ICount-=3; }

static void i_trans(void) { unsigned long dest = (I.regs.w[BW]+I.regs.b[AL])&0xffff; I.regs.b[AL] = (cpuReadByte((((seg_prefix && (DS==SS)) ? prefix_base : I.sregs[DS] << 4)+(dest)))); nec_ICount-=5; }
static void i_fpo(void) { }

static void i_loopne(void) { signed char disp = (signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.w[CW]--; if (!(I.ZeroVal==0) && I.regs.w[CW]) { I.ip = (unsigned short)(I.ip+disp); nec_ICount-=6; } else nec_ICount-=3; }
static void i_loope(void) { signed char disp = (signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.w[CW]--; if ( (I.ZeroVal==0) && I.regs.w[CW]) { I.ip = (unsigned short)(I.ip+disp); nec_ICount-=6; } else nec_ICount-=3; }
static void i_loop(void) { signed char disp = (signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.w[CW]--; if (I.regs.w[CW]) { I.ip = (unsigned short)(I.ip+disp); nec_ICount-=5; } else nec_ICount-=2; }
static void i_jcxz(void) { signed char disp = (signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++)); if (I.regs.w[CW] == 0) { I.ip = (unsigned short)(I.ip+disp); nec_ICount-=4; } else nec_ICount-=1; }
static void i_inal(void) { unsigned char port = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[AL] = ioReadByte(port); nec_ICount-=6; }
static void i_inax(void) { unsigned char port = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.regs.b[AL] = ioReadByte(port); I.regs.b[AH] = ioReadByte(port+1); nec_ICount-=6; }
static void i_outal(void) { unsigned char port = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); ioWriteByte(port, I.regs.b[AL]); nec_ICount-=6; }
static void i_outax(void) { unsigned char port = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); ioWriteByte(port, I.regs.b[AL]); ioWriteByte(port+1, I.regs.b[AH]); nec_ICount-=6; }

static void i_call_d16(void) { unsigned long tmp; { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.ip); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.ip)>>8); }; }; I.ip = (unsigned short)(I.ip+(signed short)tmp); nec_ICount-=5; }
static void i_jmp_d16(void) { unsigned long tmp; { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; I.ip = (unsigned short)(I.ip+(signed short)tmp); nec_ICount-=4; }
static void i_jmp_far(void) { unsigned long tmp,tmp1; { tmp=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; { tmp1=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; I.sregs[CS] = (unsigned short)tmp1; I.ip = (unsigned short)tmp; nec_ICount-=7; }
static void i_jmp_d8(void) { int tmp = (int)((signed char)(cpuReadByte((I.sregs[CS]<<4)+I.ip++))); nec_ICount-=4;
 if (tmp==-2 && no_interrupt==0 && nec_ICount>0) nec_ICount%=12;
 I.ip = (unsigned short)(I.ip+tmp);
}
static void i_inaldx(void) { I.regs.b[AL] = ioReadByte(I.regs.w[DW]); nec_ICount-=6;}
static void i_inaxdx(void) { unsigned long port = I.regs.w[DW]; I.regs.b[AL] = ioReadByte(port); I.regs.b[AH] = ioReadByte(port+1); nec_ICount-=6; }
static void i_outdxal(void) { ioWriteByte(I.regs.w[DW], I.regs.b[AL]); nec_ICount-=6; }
static void i_outdxax(void) { unsigned long port = I.regs.w[DW]; ioWriteByte(port, I.regs.b[AL]); ioWriteByte(port+1, I.regs.b[AH]); nec_ICount-=6; }

static void i_lock(void) { no_interrupt=1; nec_ICount-=1; }

static void i_repne(void) { unsigned long next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned short c = I.regs.w[CW];
    switch(next) {
     case 0x26: seg_prefix=1; prefix_base=I.sregs[ES]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x2e: seg_prefix=1; prefix_base=I.sregs[CS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x36: seg_prefix=1; prefix_base=I.sregs[SS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x3e: seg_prefix=1; prefix_base=I.sregs[DS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
 }

    switch(next) {
     case 0x6c: nec_ICount-=2; if (c) do { i_insb(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6d: nec_ICount-=2; if (c) do { i_insw(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6e: nec_ICount-=2; if (c) do { i_outsb(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6f: nec_ICount-=2; if (c) do { i_outsw(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa4: nec_ICount-=2; if (c) do { i_movsb(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa5: nec_ICount-=2; if (c) do { i_movsw(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa6: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_cmpsb(); c--; nec_ICount-=3; } while (c>0 && (I.ZeroVal==0)==0); I.regs.w[CW]=c; break;
     case 0xa7: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_cmpsw(); c--; nec_ICount-=3; } while (c>0 && (I.ZeroVal==0)==0); I.regs.w[CW]=c; break;
     case 0xaa: nec_ICount-=2; if (c) do { i_stosb(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xab: nec_ICount-=2; if (c) do { i_stosw(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xac: nec_ICount-=2; if (c) do { i_lodsb(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xad: nec_ICount-=2; if (c) do { i_lodsw(); c--; } while (c>0); I.regs.w[CW]=c; break;
     case 0xae: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_scasb(); c--; nec_ICount-=5; } while (c>0 && (I.ZeroVal==0)==0); I.regs.w[CW]=c; break;
     case 0xaf: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_scasw(); c--; nec_ICount-=5; } while (c>0 && (I.ZeroVal==0)==0); I.regs.w[CW]=c; break;
  default: nec_instruction[next]();
    }
 seg_prefix=0;
}
static void i_repe(void) { unsigned long next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); unsigned short c = I.regs.w[CW];
    switch(next) {
     case 0x26: seg_prefix=1; prefix_base=I.sregs[ES]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x2e: seg_prefix=1; prefix_base=I.sregs[CS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x36: seg_prefix=1; prefix_base=I.sregs[SS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
     case 0x3e: seg_prefix=1; prefix_base=I.sregs[DS]<<4; next = (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); nec_ICount-=2; break;
 }

    switch(next) {
     case 0x6c: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_insb(); c--; nec_ICount-=0; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6d: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_insw(); c--; nec_ICount-=0; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6e: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_outsb(); c--; nec_ICount-=-1; } while (c>0); I.regs.w[CW]=c; break;
     case 0x6f: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_outsw(); c--; nec_ICount-=-1; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa4: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_movsb(); c--; nec_ICount-=2; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa5: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_movsw(); c--; nec_ICount-=2; } while (c>0); I.regs.w[CW]=c; break;
     case 0xa6: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_cmpsb(); c--; nec_ICount-=4; } while (c>0 && (I.ZeroVal==0)==1); I.regs.w[CW]=c; break;
     case 0xa7: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_cmpsw(); c--; nec_ICount-=4; } while (c>0 && (I.ZeroVal==0)==1); I.regs.w[CW]=c; break;
     case 0xaa: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_stosb(); c--; nec_ICount-=3; } while (c>0); I.regs.w[CW]=c; break;
     case 0xab: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_stosw(); c--; nec_ICount-=3; } while (c>0); I.regs.w[CW]=c; break;
     case 0xac: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_lodsb(); c--; nec_ICount-=3; } while (c>0); I.regs.w[CW]=c; break;
     case 0xad: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_lodsw(); c--; nec_ICount-=3; } while (c>0); I.regs.w[CW]=c; break;
     case 0xae: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_scasb(); c--; nec_ICount-=4; } while (c>0 && (I.ZeroVal==0)==1); I.regs.w[CW]=c; break;
     case 0xaf: nec_ICount-=5; if (c) do { if(nec_ICount<0){ if(seg_prefix) I.ip-=(unsigned short)3; else I.ip-=(unsigned short)2; break;}; i_scasw(); c--; nec_ICount-=4; } while (c>0 && (I.ZeroVal==0)==1); I.regs.w[CW]=c; break;
  default: nec_instruction[next]();
    }
 seg_prefix=0;
}
static void i_hlt(void) { nec_ICount=0; }
static void i_cmc(void) { I.CarryVal = !(I.CarryVal!=0); nec_ICount-=4; }
static void i_f6pre(void) { unsigned long tmp; unsigned long uresult,uresult2; signed long result,result2;
 unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); tmp = ((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])())));
    switch (ModRM & 0x38) {
  case 0x00: tmp &= (cpuReadByte((I.sregs[CS]<<4)+I.ip++)); I.CarryVal = I.OverVal = I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(tmp)); { nec_ICount-=( ModRM >=0xc0 )?1:2; }; break;
  case 0x08: break;
   case 0x10: { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=~tmp; else {cpuWriteByte((EA),~tmp); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x18: I.CarryVal=(tmp!=0);tmp=(~tmp)+1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(tmp)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=tmp&0xff; else {cpuWriteByte((EA),tmp&0xff); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x20: uresult = I.regs.b[AL]*tmp; I.regs.w[AW]=(unsigned short)uresult; I.CarryVal=I.OverVal=(I.regs.b[AH]!=0); { nec_ICount-=( ModRM >=0xc0 )?3:4; }; break;
  case 0x28: result = (signed short)((signed char)I.regs.b[AL])*(signed short)((signed char)tmp); I.regs.w[AW]=(unsigned short)result; I.CarryVal=I.OverVal=(I.regs.b[AH]!=0); { nec_ICount-=( ModRM >=0xc0 )?3:4; }; break;
  case 0x30: if (tmp) { uresult = I.regs.w[AW]; uresult2 = uresult % tmp; if ((uresult /= tmp) > 0xff) { nec_interrupt(0,0); break; } else { I.regs.b[AL] = uresult; I.regs.b[AH] = uresult2; }; } else nec_interrupt(0,0); { nec_ICount-=( ModRM >=0xc0 )?15:16; }; break;
  case 0x38: if (tmp) { result = (signed short)I.regs.w[AW]; result2 = result % (signed short)((signed char)tmp); if ((result /= (signed short)((signed char)tmp)) > 0xff) { nec_interrupt(0,0); break; } else { I.regs.b[AL] = result; I.regs.b[AH] = result2; }; } else nec_interrupt(0,0); { nec_ICount-=( ModRM >=0xc0 )?17:18; }; break;
   }
}

static void i_f7pre(void) { unsigned long tmp,tmp2; unsigned long uresult,uresult2; signed long result,result2;
 unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); tmp = ((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) ));
    switch (ModRM & 0x38) {
  case 0x00: { tmp2=cpuReadByte((((I.sregs[CS]<<4)+I.ip)))+(cpuReadByte((((I.sregs[CS]<<4)+I.ip+1)))<<8); I.ip+=2; }; tmp &= tmp2; I.CarryVal = I.OverVal = I.AuxVal=0; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp)); { nec_ICount-=( ModRM >=0xc0 )?1:2; }; break;
  case 0x08: break;
   case 0x10: { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=~tmp; else {cpuWriteByte((EA),~tmp); cpuWriteByte(((EA)+1),(~tmp)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x18: I.CarryVal=(tmp!=0); tmp=(~tmp)+1; (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=tmp&0xffff; else {cpuWriteByte((EA),tmp&0xffff); cpuWriteByte(((EA)+1),(tmp&0xffff)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x20: uresult = I.regs.w[AW]*tmp; I.regs.w[AW]=uresult&0xffff; I.regs.w[DW]=((unsigned long)uresult)>>16; I.CarryVal=I.OverVal=(I.regs.w[DW]!=0); { nec_ICount-=( ModRM >=0xc0 )?3:4; }; break;
  case 0x28: result = (signed long)((signed short)I.regs.w[AW])*(signed long)((signed short)tmp); I.regs.w[AW]=result&0xffff; I.regs.w[DW]=result>>16; I.CarryVal=I.OverVal=(I.regs.w[DW]!=0); { nec_ICount-=( ModRM >=0xc0 )?3:4; }; break;
  case 0x30: if (tmp) { uresult = (((unsigned long)I.regs.w[DW]) << 16) | I.regs.w[AW]; uresult2 = uresult % tmp; if ((uresult /= tmp) > 0xffff) { nec_interrupt(0,0); break; } else { I.regs.w[AW]=uresult; I.regs.w[DW]=uresult2; }; } else nec_interrupt(0,0); { nec_ICount-=( ModRM >=0xc0 )?23:24; }; break;
  case 0x38: if (tmp) { result = ((unsigned long)I.regs.w[DW] << 16) + I.regs.w[AW]; result2 = result % (signed long)((signed short)tmp); if ((result /= (signed long)((signed short)tmp)) > 0xffff) { nec_interrupt(0,0); break; } else { I.regs.w[AW]=result; I.regs.w[DW]=result2; }; } else nec_interrupt(0,0); { nec_ICount-=( ModRM >=0xc0 )?24:25; }; break;
  }
}

static void i_clc(void) { I.CarryVal = 0; nec_ICount-=4; }
static void i_stc(void) { I.CarryVal = 1; nec_ICount-=4; }
static void i_di(void) { (I.IF = (0)); nec_ICount-=4; }
static void i_ei(void) { (I.IF = (1)); nec_ICount-=4; }
static void i_cld(void) { (I.DF = (0)); nec_ICount-=4; }
static void i_std(void) { (I.DF = (1)); nec_ICount-=4; }
static void i_fepre(void) { unsigned long tmp, tmp1; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); tmp=((ModRM) >= 0xc0 ? I.regs.b[Mod_RM.RM.b[ModRM]] : (cpuReadByte((*GetEA[ModRM])())));
    switch(ModRM & 0x38) {
     case 0x00: tmp1 = tmp+1; I.OverVal = (tmp==0x7f); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(tmp1)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)tmp1; else {cpuWriteByte((EA),(unsigned char)tmp1); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x08: tmp1 = tmp-1; I.OverVal = (tmp==0x80); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed char)(tmp1)); { if (ModRM >= 0xc0) I.regs.b[Mod_RM.RM.b[ModRM]]=(unsigned char)tmp1; else {cpuWriteByte((EA),(unsigned char)tmp1); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
 }
}
static void i_ffpre(void) { unsigned long tmp, tmp1; unsigned long ModRM=cpuReadByte((I.sregs[CS]<<4)+I.ip++); tmp=((ModRM) >= 0xc0 ? I.regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), (cpuReadByte(EA) | (cpuReadByte((EA)+1)<<8)) ));
    switch(ModRM & 0x38) {
     case 0x00: tmp1 = tmp+1; I.OverVal = (tmp==0x7fff); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)tmp1; else {cpuWriteByte((EA),(unsigned short)tmp1); cpuWriteByte(((EA)+1),((unsigned short)tmp1)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x08: tmp1 = tmp-1; I.OverVal = (tmp==0x8000); (I.AuxVal = ((tmp1) ^ ((tmp) ^ (1))) & 0x10); (I.SignVal=I.ZeroVal=I.ParityVal=(signed short)(tmp1)); { if (ModRM >= 0xc0) I.regs.w[Mod_RM.RM.w[ModRM]]=(unsigned short)tmp1; else {cpuWriteByte((EA),(unsigned short)tmp1); cpuWriteByte(((EA)+1),((unsigned short)tmp1)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:3; }; break;
  case 0x10: { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.ip); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.ip)>>8); }; }; I.ip = (unsigned short)tmp; { nec_ICount-=( ModRM >=0xc0 )?5:6; }; break;
  case 0x18: tmp1 = I.sregs[CS]; I.sregs[CS] = (cpuReadByte((EA&0xf0000)|((EA+2)&0xffff)) | (cpuReadByte(((EA&0xf0000)|((EA+2)&0xffff))+1)<<8)); { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),tmp1); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(tmp1)>>8); }; }; { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),I.ip); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(I.ip)>>8); }; }; I.ip = tmp; { nec_ICount-=( ModRM >=0xc0 )?1:12; }; break;
  case 0x20: I.ip = tmp; { nec_ICount-=( ModRM >=0xc0 )?4:5; }; break;
  case 0x28: I.ip = tmp; I.sregs[CS] = (cpuReadByte((EA&0xf0000)|((EA+2)&0xffff)) | (cpuReadByte(((EA&0xf0000)|((EA+2)&0xffff))+1)<<8)); { nec_ICount-=( ModRM >=0xc0 )?1:10; }; break;
  case 0x30: { I.regs.w[SP] -= 2; {cpuWriteByte(((((I.sregs[SS]<<4)+I.regs.w[SP]))),tmp); cpuWriteByte((((((I.sregs[SS]<<4)+I.regs.w[SP])))+1),(tmp)>>8); }; }; { nec_ICount-=( ModRM >=0xc0 )?1:2; }; break;
  default: ;
 }
}

static void i_invalid(void)
{
	nec_ICount-=10;
}

int nec_execute(int cycles)
{
	nec_ICount = cycles;
		
	while(nec_ICount > 0)
		nec_instruction[(cpuReadByte((I.sregs[CS]<<4)+I.ip++))]();
		
	return cycles - nec_ICount;
}

void nec_set_reg(void)
{
}

void nec_get_reg(void)
{
}
