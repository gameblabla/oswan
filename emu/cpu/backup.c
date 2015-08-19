OP( 0x0f, i_pre_nec  ) { UINT32 ModRM, tmp, tmp2; /* pop cs at V30MZ? */
    switch (FETCH) {
        case 0x10 : BITOP_BYTE; CLKS(3,3,4); tmp2 = I.regs.b[CL] & 0x7; I.ZeroVal = (tmp & (1<<tmp2)) ? 1 : 0;  I.CarryVal=I.OverVal=0; break; /* Test */
        case 0x11 : BITOP_WORD; CLKS(3,3,4); tmp2 = I.regs.b[CL] & 0xf; I.ZeroVal = (tmp & (1<<tmp2)) ? 1 : 0;  I.CarryVal=I.OverVal=0; break; /* Test */
        case 0x12 : BITOP_BYTE; CLKS(5,5,4); tmp2 = I.regs.b[CL] & 0x7; tmp &= ~(1<<tmp2);  PutbackRMByte(ModRM,tmp);   break; /* Clr */
        case 0x13 : BITOP_WORD; CLKS(5,5,4); tmp2 = I.regs.b[CL] & 0xf; tmp &= ~(1<<tmp2);  PutbackRMWord(ModRM,tmp);   break; /* Clr */
        case 0x14 : BITOP_BYTE; CLKS(4,4,4); tmp2 = I.regs.b[CL] & 0x7; tmp |= (1<<tmp2);   PutbackRMByte(ModRM,tmp);   break; /* Set */
        case 0x15 : BITOP_WORD; CLKS(4,4,4); tmp2 = I.regs.b[CL] & 0xf; tmp |= (1<<tmp2);   PutbackRMWord(ModRM,tmp);   break; /* Set */
        case 0x16 : BITOP_BYTE; CLKS(4,4,4); tmp2 = I.regs.b[CL] & 0x7; BIT_NOT;            PutbackRMByte(ModRM,tmp);   break; /* Not */
        case 0x17 : BITOP_WORD; CLKS(4,4,4); tmp2 = I.regs.b[CL] & 0xf; BIT_NOT;            PutbackRMWord(ModRM,tmp);   break; /* Not */

        case 0x18 : BITOP_BYTE; CLKS(4,4,4); tmp2 = (FETCH) & 0x7;  I.ZeroVal = (tmp & (1<<tmp2)) ? 1 : 0;  I.CarryVal=I.OverVal=0; break; /* Test */
        case 0x19 : BITOP_WORD; CLKS(4,4,4); tmp2 = (FETCH) & 0xf;  I.ZeroVal = (tmp & (1<<tmp2)) ? 1 : 0;  I.CarryVal=I.OverVal=0; break; /* Test */
        case 0x1a : BITOP_BYTE; CLKS(6,6,4); tmp2 = (FETCH) & 0x7;  tmp &= ~(1<<tmp2);      PutbackRMByte(ModRM,tmp);   break; /* Clr */
        case 0x1b : BITOP_WORD; CLKS(6,6,4); tmp2 = (FETCH) & 0xf;  tmp &= ~(1<<tmp2);      PutbackRMWord(ModRM,tmp);   break; /* Clr */
        case 0x1c : BITOP_BYTE; CLKS(5,5,4); tmp2 = (FETCH) & 0x7;  tmp |= (1<<tmp2);       PutbackRMByte(ModRM,tmp);   break; /* Set */
        case 0x1d : BITOP_WORD; CLKS(5,5,4); tmp2 = (FETCH) & 0xf;  tmp |= (1<<tmp2);       PutbackRMWord(ModRM,tmp);   break; /* Set */
        case 0x1e : BITOP_BYTE; CLKS(5,5,4); tmp2 = (FETCH) & 0x7;  BIT_NOT;                PutbackRMByte(ModRM,tmp);   break; /* Not */
        case 0x1f : BITOP_WORD; CLKS(5,5,4); tmp2 = (FETCH) & 0xf;  BIT_NOT;                PutbackRMWord(ModRM,tmp);   break; /* Not */

        case 0x20 : ADD4S; CLKS(7,7,2); break;
        case 0x22 : SUB4S; CLKS(7,7,2); break;
        case 0x26 : CMP4S; CLKS(7,7,2); break;
        case 0x28 : ModRM = FETCH; tmp = GetRMByte(ModRM); tmp <<= 4; tmp |= I.regs.b[AL] & 0xf; I.regs.b[AL] = (I.regs.b[AL] & 0xf0) | ((tmp>>8)&0xf); tmp &= 0xff; PutbackRMByte(ModRM,tmp); CLKM(9,15); break;
        case 0x2a : ModRM = FETCH; tmp = GetRMByte(ModRM); tmp2 = (I.regs.b[AL] & 0xf)<<4; I.regs.b[AL] = (I.regs.b[AL] & 0xf0) | (tmp&0xf); tmp = tmp2 | (tmp>>4); PutbackRMByte(ModRM,tmp); CLKM(13,19); break;
        case 0x31 : ModRM = FETCH; ModRM=0; break;
        case 0x33 : ModRM = FETCH; ModRM=0; break;
        case 0x92 : CLK(2); break; /* V25/35 FINT */
        case 0xe0 : ModRM = FETCH; ModRM=0; break;
        case 0xf0 : ModRM = FETCH; ModRM=0; break;
        case 0xff : ModRM = FETCH; ModRM=0; break;
        default:    break;
    }
}
