#ifdef DREAMCAST
#include <kos.h>
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#ifdef HOME_SUPPORT
#include <unistd.h>
#endif

#include "shared.h"
#include "drawing.h"
#include "game_input.h"
#include "hack.h"

#ifndef NO_WAIT
void msleep(unsigned char milisec);
#endif
void exit_oswan();

extern void mixaudioCallback(void *userdata, unsigned char *stream, int len);

#ifdef PSP
#include <pspkernel.h>
#include <pspdisplay.h>
#include <psppower.h>
PSP_MODULE_INFO("OSWAN", 0, 1, 1);
PSP_HEAP_SIZE_MAX();
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
int exit_callback(int arg1, int arg2, void *common) 
{
	exit_oswan(); return 0;
}
int CallbackThread(SceSize args, void *argp) 
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);	sceKernelSleepThreadCB();
	return 0;
}
int SetupCallbacks(void) 
{
	int thid = sceKernelCreateThread("CallbackThread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if (thid >= 0) sceKernelStartThread(thid, 0, 0); return thid;
}
#endif

unsigned int m_Flag;
unsigned int interval;

gamecfg GameConf;
char gameName[512];
char current_conf_app[MAX__PATH];

unsigned long nextTick, lastTick = 0, newTick, currentTick, wait;
unsigned char FPS = 60; 
unsigned char pastFPS = 0; 

SDL_Surface *actualScreen;
#if !defined(NOSCREENSHOTS)
SDL_Surface *screenshots;
#endif

unsigned long SDL_UXTimerRead(void) 
{
	struct timeval tval; /* timing	*/
  	gettimeofday(&tval, 0);
	return (((tval.tv_sec*1000000) + (tval.tv_usec )));
}

void graphics_paint(void) 
{
	screen_draw();
	pastFPS++;
	newTick = SDL_UXTimerRead();
	if ((newTick-lastTick)>1000000) 
	{
		FPS = pastFPS;
		pastFPS = 0;
		lastTick = newTick;
	}
}

void initSDL(void) 
{
	/* Get current resolution, does nothing on Windowed or bare metal platroms*/
	Get_resolution();
	
	SetVideo(0);

#ifdef SOUND_ON
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec fmt, retFmt;
	
	/*	Set up SDL sound */
	fmt.freq = 44800;   
	fmt.samples = 2048;
	/*fmt.freq = 12000;   
	fmt.samples = 512;*/
	fmt.format = AUDIO_S16SYS;
	fmt.channels = 2;
	fmt.callback = mixaudioCallback;
	fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, &retFmt) < 0 )
	{
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
#endif
}


int main(int argc, char *argv[]) 
{
#ifdef PSP
	SetupCallbacks();
	scePowerSetClockFrequency(333, 333, 166);
#endif

#ifdef GECKO
	fatInitDefault();
	WPAD_Init();
#endif
	
	/* Init graphics & sound	*/
	initSDL();
	
#ifdef JOYSTICK
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
#endif

	getcwd(current_conf_app, MAX__PATH);
	
#if defined(HOME_SUPPORT)
	char home_path[256];
	snprintf(home_path, sizeof(home_path), "%s/.oswan", PATH_DIRECTORY);
	/* 
	 * If folder does not exists then create it 
	 * This can speeds up startup if the folder already exists
	*/
	if(access( home_path, F_OK ) == -1) 
	{
		mkdir(home_path, 0755);	
	}
	snprintf(current_conf_app, sizeof(current_conf_app), "%s/.oswan/oswan.cfg", PATH_DIRECTORY);
#else
	snprintf(current_conf_app, sizeof(current_conf_app), "%soswan.cfg%s", PATH_DIRECTORY, EXTENSION);
#endif
	
	m_Flag = GF_MAINUI;
	system_loadcfg(current_conf_app);

    /*	load rom file via args if a rom path is supplied	*/
	strcpy(gameName,"");
	
	if(argc > 1) 
	{
#ifdef NATIVE_RESOLUTION
		SetVideo(1);
#endif
		flip_screen(actualScreen);
		snprintf(gameName, sizeof(gameName) ,"%s", argv[1]);
		m_Flag = GF_GAMEINIT;
	}

	while (m_Flag != GF_GAMEQUIT) 
	{
		switch (m_Flag) 
		{
			case GF_MAINUI:
				#ifdef SOUND_ON
				SDL_PauseAudio(1);
				#endif
				screen_showtopmenu();
				if (cartridge_IsLoaded()) 
				{
					#ifdef SOUND_ON
					SDL_PauseAudio(0);
					#endif
				}
				#ifndef NO_WAIT
				nextTick = SDL_UXTimerRead() + interval;
				#endif
				break;

			case GF_GAMEINIT:
			
				Set_DrawRegion();
				
				if (WsCreate(gameName)) 
				{
					WsInit();
					m_Flag = GF_GAMERUNNING;
					#ifdef SOUND_ON
					SDL_PauseAudio(0);
					#endif
					#ifndef NO_WAIT
					/* Init timing */
					interval = (1.0 / 60) * 1000000;
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
				break;
		}
	}
	
	exit_oswan();
	return 0;
}

void exit_oswan()
{
	#ifdef SOUND_ON
		SDL_PauseAudio(1);
	#endif

	/* Free memory	*/
	#ifdef _TINSPIRE
		deinitBuffering();
	#else
		
	#ifndef NOSCREENSHOTS
		if (screenshots != NULL) SDL_FreeSurface(screenshots);
	#endif
		if (actualScreen != NULL) SDL_FreeSurface(actualScreen);
	#if defined(SCALING)
		if (real_screen != NULL) SDL_FreeSurface(real_screen);
	#endif
		SDL_QuitSubSystem(SDL_INIT_VIDEO);	
	#ifdef SOUND_ON
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	#endif
		SDL_Quit();
	#endif
	
#ifdef PSP
	sceDisplayWaitVblankStart();
	sceKernelExitGame(); 	
#endif
}

#ifndef NO_WAIT
void msleep(unsigned char milisec)
{
/* 
 * nanosleep is better in every way.
 * Only use SDL_Delay as a last resort. 
*/
#ifdef POSIX
	struct timespec req={0};
	time_t sec=(unsigned short)(milisec/1000);

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

