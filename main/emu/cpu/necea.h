
static UINT32 EA;
static UINT16 EO;
static UINT16 E16;

static unsigned long EA_000(void) { EO=I.regs.w[BW]+I.regs.w[IX]; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_001(void) { EO=I.regs.w[BW]+I.regs.w[IY]; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_002(void) { EO=I.regs.w[BP]+I.regs.w[IX]; EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_003(void) { EO=I.regs.w[BP]+I.regs.w[IY]; EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_004(void) { EO=I.regs.w[IX]; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_005(void) { EO=I.regs.w[IY]; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_006(void) { EO=FETCH; EO+=FETCH<<8; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_007(void) { EO=I.regs.w[BW]; EA=DefaultBase(DS)+EO; return EA; }

static unsigned long EA_100(void) { EO=(I.regs.w[BW]+I.regs.w[IX]+(INT8)FETCH); EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_101(void) { EO=(I.regs.w[BW]+I.regs.w[IY]+(INT8)FETCH); EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_102(void) { EO=(I.regs.w[BP]+I.regs.w[IX]+(INT8)FETCH); EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_103(void) { EO=(I.regs.w[BP]+I.regs.w[IY]+(INT8)FETCH); EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_104(void) { EO=(I.regs.w[IX]+(INT8)FETCH); EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_105(void) { EO=(I.regs.w[IY]+(INT8)FETCH); EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_106(void) { EO=(I.regs.w[BP]+(INT8)FETCH); EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_107(void) { EO=(I.regs.w[BW]+(INT8)FETCH); EA=DefaultBase(DS)+EO; return EA; }

static unsigned long EA_200(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BW]+I.regs.w[IX]+(INT16)E16; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_201(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BW]+I.regs.w[IY]+(INT16)E16; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_202(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BP]+I.regs.w[IX]+(INT16)E16; EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_203(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BP]+I.regs.w[IY]+(INT16)E16; EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_204(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[IX]+(INT16)E16; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_205(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[IY]+(INT16)E16; EA=DefaultBase(DS)+EO; return EA; }
static unsigned long EA_206(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BP]+(INT16)E16; EA=DefaultBase(SS)+EO; return EA; }
static unsigned long EA_207(void) { E16=FETCH; E16+=FETCH<<8; EO=I.regs.w[BW]+(INT16)E16; EA=DefaultBase(DS)+EO; return EA; }

static unsigned long (*GetEA[192])(void)={
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
