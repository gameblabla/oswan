/* Stripped down version of SDL_endian.h */

/*@{*/
#define LIL_ENDIAN_APP	1234
#define BIG_ENDIAN_APP	4321
/*@}*/

#ifndef _BYTEORDER	/* Not defined in SDL_config.h? */
#ifdef __linux__
#include <endian.h>
#define _BYTEORDER  __BYTE_ORDER
#else /* __linux __ */
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define _BYTEORDER	BIG_ENDIAN_APP
#else
#define _BYTEORDER	LIL_ENDIAN_APP
#endif
#endif /* __linux __ */
#endif /* !SDL_BYTEORDER */
