#ifndef BROWSER_H
#define BROWSER_H

#include <SDL/SDL.h>
#include <stdint.h>

extern SDL_Surface* screen;

#define MAX_LENGH 8192

extern char file_to_start[MAX_LENGH];

#define TITLE_WINDOW "Gameblabla's Oswan"
#define DEFAULT_TEXT "Select a WS/WSC file"

#define EXTENSIONS 4

/*	Colors	*/
#define BLUE_C  255
#define RED_C  1700
#define GREEN_C 512
#define TUR_C 750
#define F_C  1023

extern uint32_t FileBrowser(void);

#endif

