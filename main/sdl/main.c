#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "shared.h"
#include "drawing.h"
#include "input.h"
#include "menu.h"
#include "sound.h"

#ifndef NO_WAIT
void msleep(long milisec);
#endif

uint32_t m_Flag;
static long interval;
static long nextTick, lastTick = 0, newTick, currentTick, wait;
static int32_t FPS = 60; 
static int32_t pastFPS = 0; 
char gameName[512];

uint32_t game_alreadyloaded = 0;

#ifdef FRAMESKIP
extern int32_t FrameSkip;
#endif

static uint32_t SDL_UXTimerRead(void) 
{
	/* Timing. */
	struct timeval tval;
  	gettimeofday(&tval, 0);
	return (((tval.tv_sec*1000000) + (tval.tv_usec)));
}

void graphics_paint(void) 
{
	screen_draw();
	pastFPS++;
	newTick = SDL_UXTimerRead();
	if ( (newTick-lastTick) > 1000000) 
	{
		FPS = pastFPS;
		pastFPS = 0;
		lastTick = newTick;
	}
	#ifdef FRAMESKIP
	FrameSkip = 80 - FPS;
	if (FrameSkip < 0) FrameSkip = 0;
	else if (FrameSkip > 4) FrameSkip = 4;
	#endif
}

static void initSDL(void) 
{
	/* Get current resolution, does nothing on Windowed or bare metal platroms */
	Get_resolution();
	SetVideo(0);
	Init_Sound();
}

static void exit_oswan()
{
	save_config();
	WsDeInit();
	Cleanup_Sound();
	Cleanup_Screen();
}

int main(int argc, char *argv[]) 
{
#ifdef NOROMLOADER
	if (argc < 2) return 0;	
#endif
	m_Flag = GF_MAINUI;
	
	/* Init graphics & sound */
	initSDL();
	
	load_config();

    /*	load rom file via args if a rom path is supplied	*/
	strcpy(gameName,"");
	
	if(argc > 1) 
	{
#ifdef NATIVE_RESOLUTION
		SetVideo(1);
#endif
		Update_Screen();
		snprintf(gameName, sizeof(gameName) ,"%s", argv[1]);
		m_Flag = GF_GAMEINIT;
		game_alreadyloaded = 1;
	}

	while (m_Flag != GF_GAMEQUIT) 
	{
		switch (m_Flag) 
		{
			case GF_MAINUI:
				Pause_Sound();
				Menu();
				if (cartridge_IsLoaded()) 
				{
					Resume_Sound();
				}
				#ifndef NO_WAIT
				nextTick = SDL_UXTimerRead() + interval;
				#endif
				break;

			case GF_GAMEINIT:
				if (WsCreate(gameName)) 
				{
					WsInit();
					m_Flag = GF_GAMERUNNING;
					Resume_Sound();
					game_alreadyloaded = 1;
					#ifndef NO_WAIT
					/* Init timing */
					interval = (1.0f / 60) * 1000000;
					nextTick = SDL_UXTimerRead() + interval;
					#endif
				}
				else 
				{
					fprintf(stderr,"Can't load %s", gameName); 
					fflush(stderr);
					m_Flag = GF_GAMEQUIT;
				}
				
				break;
		
			case GF_GAMERUNNING:	
				#ifndef NO_WAIT
				currentTick = SDL_UXTimerRead(); 
				wait = (nextTick - currentTick);
				if (wait > 0) 
				{
					if (wait < 1000000) 
					{
						msleep(wait/1000);
					}
				}
				nextTick += interval;
				#endif
				exit_button();
				WsRun();
				Sound_Update();
				break;
		}
	}
	
	exit_oswan();
	
	return 0;
}

#ifndef NO_WAIT
void msleep(long milisec)
{
/* nanosleep is more precise and has less latency. Only use SDL_Delay as a last resort. */
#ifdef POSIX
	struct timespec req={0};
	time_t sec = (milisec/1000);

	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;

	while(nanosleep(&req,&req)==-1)
	continue;
#else
	SDL_Delay(milisec);
#endif
}
#endif

