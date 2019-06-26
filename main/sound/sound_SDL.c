#include <stdio.h>
#include <SDL/SDL.h>
#include "WSHard.h"
#include "WSApu.h"
#include "shared.h"
#include "sound.h"

#warning "The SDL Sound code is ABSOLUTE COMPLETE CRAP ! Replace, fix, anything please to get rid of this mess !"

#ifdef SOUND_ON
static SDL_mutex *sound_mutex;
static SDL_cond *sound_cv;

static void SDL_callback(void *userdata, uint8_t *stream, int32_t len)
{
	int32_t i;
	uint16_t *buffer = (uint16_t *) stream;
	
	len /= 4;
	
    if (len <= 0 || !buffer)
	{
		return;
	}

	SDL_LockMutex(sound_mutex);

	if (apuBufLen() < len) 
	{
		SDL_CondWait(sound_cv, sound_mutex);
	}
	else
	{
		for (i=0; i < len; i++)
		{
			*buffer++ = sndbuffer[0][rBuf];
			*buffer++ = sndbuffer[1][rBuf];
			if (++rBuf >= SND_RNGSIZE) 
			{
				rBuf = 0;
			}
		}
	}

	SDL_UnlockMutex(sound_mutex);
	SDL_CondSignal(sound_cv);
}
#endif

void Init_Sound()
{
#ifdef SOUND_ON
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec fmt, retFmt;
	
	/*	Set up SDL sound */
	fmt.freq = 44800;   
	fmt.samples = 1024;
	fmt.format = AUDIO_S16SYS;
	fmt.channels = 2;
	fmt.callback = SDL_callback;
	fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, &retFmt) < 0 )
	{
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        printf("Exiting Oswan...\n");
        exit(1);
    }
    
	sound_mutex = SDL_CreateMutex();
	sound_cv = SDL_CreateCond();
#endif
}

void Pause_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(1);
#endif
}

void Resume_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(0);
#endif
}

void Cleanup_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(1);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
}

void Sound_APUClose()
{
#ifdef SOUND_ON
	SDL_CondSignal(sound_cv);
#endif
}

void Sound_APU_Start()
{
#ifdef SOUND_ON
	SDL_LockMutex(sound_mutex);
#endif
}

void Sound_APU_End()
{
#ifdef SOUND_ON
	SDL_UnlockMutex(sound_mutex);
	SDL_CondSignal(sound_cv);
#endif
}

void Sound_Update()
{
	
}
