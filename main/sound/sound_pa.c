#include <stdio.h>
#include <stdint.h>
#include <portaudio.h>
#include "WSHard.h"
#include "WSApu.h"
#include "shared.h"
#include "sound.h"

#warning "Doesn't work properly. I mean at all. Use SDL sound code in the time being."

#define SOUND_FREQUENCY 44800
#define SOUND_SAMPLES_SIZE 4096

PaStream *apu_stream;
static int16_t buffer_snd[SOUND_FREQUENCY * 2];

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    
	int32_t i = framesPerBuffer;
	uint16_t *buffer = (uint16_t *) outputBuffer;
	
    if (framesPerBuffer <= 0 || !buffer)
	{
		return;
	}
	   
	if (apuBufLen() < framesPerBuffer) 
	{
		memset(outputBuffer,0,framesPerBuffer);
	}
	else
	{
		while(i > 3) 
		{
			*buffer++ = sndbuffer[rBuf][0];
			*buffer++ = sndbuffer[rBuf][1];
			if (++rBuf >= SND_RNGSIZE) 
			{
				rBuf = 0;
			}
			i -= 4;
		}
	}
	
    return 0;
}

void Init_Sound()
{
	int32_t err;
	err = Pa_Initialize();
	
	PaStreamParameters outputParameters;
	
	outputParameters.device = Pa_GetDefaultOutputDevice();
	
	if (outputParameters.device == paNoDevice) 
	{
		printf("No sound output\n");
		return;
	}

	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paInt16;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	
	err = Pa_OpenStream( &apu_stream, NULL, &outputParameters, SOUND_FREQUENCY, SOUND_SAMPLES_SIZE, paNoFlag, patestCallback, NULL);
	err = Pa_StartStream( apu_stream );
}

void Pause_Sound()
{
}

void Resume_Sound()
{
}

void Cleanup_Sound()
{
	int32_t err;
	err = Pa_CloseStream( apu_stream );
	err = Pa_Terminate();
}

void Sound_APUClose()
{
}

void Sound_APU_Start()
{
}

void Sound_APU_End()
{
}

void Sound_Update()
{
}
