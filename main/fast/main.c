#ifdef DREAMCAST
#include <kos.h>
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "shared.h"
#include "drawing.h"
#include "hack.h"

#ifdef SWITCHING_GRAPHICS
	extern void screen_prepback(SDL_Surface *s);
#endif

unsigned int m_Flag;
unsigned int interval;

gamecfg GameConf;
char gameName[512];
char current_conf_app[MAX__PATH];

void exit_oswan();
void msleep(unsigned char milisec);
extern void mixaudioCallback(void *userdata, unsigned char *stream, int len);

#ifndef NOFRAMERATE_LIMIT	
	#ifdef NO_FLOAT
	const int real_FPS = 1000/75.47;
	#else
	const float real_FPS = 1000/75.47;
	#endif
#endif

SDL_Surface *actualScreen, *screenshots;
SDL_Event event;

#ifdef PSP
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psppower.h>
/* Define the module info section */
PSP_MODULE_INFO("OSWAN", 0, 1, 1);
PSP_HEAP_SIZE_MAX();
/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU)
int exit_callback(int arg1, int arg2, void *common) {
			m_Flag = GF_GAMEQUIT;
			return 0;
}
int CallbackThread(SceSize args, void *argp) {
			int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
			sceKernelRegisterExitCallback(cbid);
			sceKernelSleepThreadCB();
			return 0;
}
int SetupCallbacks(void) {
			int thid = sceKernelCreateThread("CallbackThread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
			if (thid >= 0) sceKernelStartThread(thid, 0, 0);
			return thid;
}
#endif

void msleep(unsigned char milisec)
{
#ifdef UNIX
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

void graphics_paint(void) 
{
	Uint32 start;
	if(SDL_MUSTLOCK(actualScreen)) SDL_LockSurface(actualScreen);

	screen_draw();

#ifndef NOFRAMERATE_LIMIT	
	start = SDL_GetTicks();
	if(real_FPS > SDL_GetTicks()-start) 
	{
		msleep(real_FPS-(SDL_GetTicks()-start));
	}
#endif
	
	if (SDL_MUSTLOCK(actualScreen)) SDL_UnlockSurface(actualScreen);
	flip_screen(actualScreen);
}

void initSDL(void) 
{
	SDL_Init(SDL_INIT_VIDEO);
	
	/* Get current resolution, does nothing on Windowed or bare metal platroms*/
	Get_resolution();
	
	SDL_ShowCursor(SDL_DISABLE);
	SetVideo(0);

#ifdef SOUND_ON
	SDL_Init(SDL_INIT_AUDIO);
	
	 //set up SDL sound 
    SDL_AudioSpec fmt, retFmt;
	fmt.freq = 12000;   
    fmt.format = AUDIO_S16SYS;
    fmt.channels = 2;
    fmt.samples = 512;
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
#ifdef _TINSPIRE
	enable_relative_paths(argv);
#endif

#ifdef PSP
	SetupCallbacks();
#endif
	
	// Init graphics & sound
	initSDL();
	
#ifdef JOYSTICK
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
#endif

	getcwd(current_conf_app, MAX__PATH);
	
#if defined(HOME_SUPPORT)
	char home_path[256];
	snprintf(home_path, sizeof(home_path), "%s/.oswan", PATH_DIRECTORY);
	mkdir(home_path, 0755);	
	snprintf(current_conf_app, sizeof(current_conf_app), "%s/.oswan/oswan.cfg", PATH_DIRECTORY);
#else
	snprintf(current_conf_app, sizeof(current_conf_app), "%soswan.cfg%s", PATH_DIRECTORY, EXTENSION);
#endif
	
	m_Flag = GF_MAINUI;
	system_loadcfg(current_conf_app);

	SDL_WM_SetCaption("Oswan", NULL);

    //load rom file via args if a rom path is supplied
	strcpy(gameName,"");
	if(argc > 1) 
	{
#ifdef SWITCHING_GRAPHICS
		SetVideo(1);
		screen_prepback(actualScreen);
#endif
		flip_screen(actualScreen);
		strcpy(gameName,argv[1]);
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
				break;

			case GF_GAMEINIT:
				if (WsCreate(gameName)) 
				{
					WsInit();
					m_Flag = GF_GAMERUNNING;
					#ifdef SOUND_ON
					SDL_PauseAudio(0);
					#endif
				}
				else 
				{
					fprintf(stderr,"Can't load %s : %s", gameName, SDL_GetError()); fflush(stderr);
					m_Flag = GF_GAMEQUIT;
				}
				break;
		
			case GF_GAMERUNNING:	
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

	// Free memory
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
	
	exit(0);
}
