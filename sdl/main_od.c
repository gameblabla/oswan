#ifdef DREAMCAST
#include <kos.h>
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
#endif

#include  <sys/time.h>

#include "shared.h"
#include "drawing.h"
#include "hack.h"

unsigned int m_Flag;
unsigned int interval;

unsigned int gameCRC;
gamecfg GameConf;
char gameName[512];
char current_conf_app[MAX__PATH];

void exit_oswan();
extern void mixaudioCallback(void *userdata, unsigned char *stream, int len);
unsigned long nextTick, lastTick = 0, newTick, currentTick, wait;
int FPS = 60; 
int pastFPS = 0; 

#ifdef SWITCHING_GRAPHICS
	extern void screen_prepbackground(void);
#endif
	SDL_Surface *actualScreen, *screenshots;
	
SDL_Event event;

unsigned long SDL_UXTimerRead(void) 
{
	struct timeval tval; // timing
  
  	gettimeofday(&tval, 0);
	return (((tval.tv_sec*1000000) + (tval.tv_usec )));
}

void graphics_paint(void) 
{
	if(SDL_MUSTLOCK(actualScreen)) SDL_LockSurface(actualScreen);

	screen_draw();
	
#if defined(SMOOTH)
	pastFPS++;
	newTick = SDL_UXTimerRead();
	if ((newTick-lastTick)>1000000) {
		FPS = pastFPS;
		pastFPS = 0;
		lastTick = newTick;
	}
#endif

	if (SDL_MUSTLOCK(actualScreen)) SDL_UnlockSurface(actualScreen);
	flip_screen(actualScreen);
}

void initSDL(void) 
{
	SDL_Init(SDL_INIT_VIDEO);
#ifdef SOUND_ON
	SDL_Init(SDL_INIT_AUDIO);
#endif

#ifdef SCALING
	Get_resolution(320,240);
#endif

	SDL_ShowCursor(SDL_DISABLE);

	SetVideo(0);

#if !defined(NOSCREENSHOTS)
	screenshots = SDL_CreateRGBSurface(FLAG_VIDEO, 320, 240, 16, 0,0,0,0);
	if(screenshots == NULL) {
		fprintf(stderr, "Couldn't create surface: %s\n", SDL_GetError());
		exit(1);
	}
#endif
	
#ifdef SOUND_ON
	 //set up SDL sound 
    SDL_AudioSpec fmt, retFmt;

    fmt.freq = 44100;  
    fmt.format = AUDIO_S16;
    fmt.channels = 1;
    fmt.samples = 512;
    fmt.callback = mixaudioCallback;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, &retFmt) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
#endif
}


int main(int argc, char *argv[]) 
{
	double period;
	
#ifdef NSPIRE
	enable_relative_paths(argv);
#endif
	
#ifdef JOYSTICK
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
#endif

	getcwd(current_conf_app, MAX__PATH);
	
#if defined(HOME_SUPPORT)
	char home_path[256];
	snprintf(current_conf_app, sizeof(home_path), "%s/.oswan/oswan.cfg", getenv("HOME"));
	mkdir(home_path, 0755);	
	snprintf(current_conf_app, sizeof(current_conf_app), "%s/.oswan/oswan.cfg", PATH_DIRECTORY);
#else
	snprintf(current_conf_app, sizeof(current_conf_app), "%soswan.cfg%s", PATH_DIRECTORY, EXTENSION);
#endif
	
	// Init graphics & sound
	initSDL();
	
	m_Flag = GF_MAINUI;
	system_loadcfg(current_conf_app);

	SDL_WM_SetCaption("Oswan - Wonderswan Emulator", NULL);

    //load rom file via args if a rom path is supplied
	strcpy(gameName,"");
	if(argc > 1) 
	{
#ifdef SWITCHING_GRAPHICS
		SetVideo(1);
		screen_prepbackground();
#endif
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
#ifndef NSPIRE
				nextTick = SDL_UXTimerRead() + interval;
#endif
				}
				break;

			case GF_GAMEINIT:
				if (WsCreate(gameName)) 
				{
					WsInit();
					m_Flag = GF_GAMERUNNING;
					hack_period();
#ifndef NSPIRE
					// Init timing
					period = 1.0 / 60;
					period = period * 1000000;
					interval = (int) period;
					nextTick = SDL_UXTimerRead() + interval;
#endif
#ifdef SOUND_ON
					SDL_PauseAudio(0);
#endif
				}
				else 
				{
					fprintf(stderr,"cant't load %s : %s",gameName,SDL_GetError()); fflush(stderr);
					m_Flag = GF_GAMEQUIT;
				}
				break;
		
			case GF_GAMERUNNING:	
#ifndef NSPIRE
				currentTick = SDL_UXTimerRead(); 
				wait = (nextTick - currentTick);
				if (wait > 0) {
					if (wait < 1000000) 
					{
#ifndef NSPIRE
						usleep(wait);
#endif
					}
				}
#endif
				WsRun();
#ifndef NSPIRE
				nextTick += interval;
#endif
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
		
		// Free memory
		
	SDL_QuitSubSystem(SDL_INIT_VIDEO);	
		
	#ifdef SOUND_ON
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	#endif
	
	SDL_Quit();
	
	exit(0);
}
