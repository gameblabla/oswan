#ifndef SOUND_H
#define SOUND_H

#include <SDL/SDL.h>
#include <stdint.h>

extern void Init_Sound(void);
extern void Pause_Sound(void);
extern void Resume_Sound(void);
extern void Cleanup_Sound(void);
extern void Sound_APUClose(void);
extern void Sound_APU_Start(void);
extern void Sound_APU_End(void);
extern void Sound_Update(void);

#endif
