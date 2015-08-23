#define _LIL_ENDIAN	1234
#define _BIG_ENDIAN	4321
/*@}*/

#ifndef SDL_BYTEORDER	/* Not defined in SDL_config.h? */
#ifdef __linux__
#include <endian.h>
#define ENDIAN_BYTEORDER  __BYTE_ORDER
#else /* __linux __ */
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define ENDIAN_BYTEORDER	BIG_ENDIAN__
#else
#define ENDIAN_BYTEORDER	LIL_ENDIAN__
#endif
#endif /* __linux __ */
#endif /* !SDL_BYTEORDER */
