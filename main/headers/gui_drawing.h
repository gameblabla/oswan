#ifdef _TINSPIRE
	#include <os.h>
	#include "n2DLib.h"
	#define COLOR_BG           	(5 >> 3) << 11 		| (3 >> 2) << 5 	| (2 >> 3)
	#define COLOR_OK			(0 >> 3) << 11 		| (0 >> 2) << 5 	| (255 >> 3)
	#define COLOR_KO			(255 >> 3) << 11 	| (0 >> 2) << 5 	| (0 >> 3)
	#define COLOR_INFO			(0 >> 3) << 11 		| (255 >> 2) << 5 	| (0 >> 3)
	#define COLOR_LIGHT			(255 >> 3) << 11 	| (255 >> 2) << 5 	| (0 >> 3)
	#define COLOR_ACTIVE_ITEM   (255 >> 3) << 11 	| (255 >> 2) << 5 	| (255 >> 3)
	#define COLOR_INACTIVE_ITEM (255 >> 3) << 11 	| (255 >> 2) << 5 	| (255 >> 3)
#else
	#include "shared.h"
	#define COLOR_BG           	SDL_MapRGB(actualScreen->format,5,3,2)
	#define COLOR_OK			SDL_MapRGB(actualScreen->format,0,0,255)
	#define COLOR_KO			SDL_MapRGB(actualScreen->format,255,0,0)
	#define COLOR_INFO			SDL_MapRGB(actualScreen->format,0,255,0)
	#define COLOR_LIGHT			SDL_MapRGB(actualScreen->format,255,255,0)
	#define COLOR_ACTIVE_ITEM   SDL_MapRGB(actualScreen->format,255, 255, 255)
	#define COLOR_INACTIVE_ITEM SDL_MapRGB(actualScreen->format,255,255,255)
#endif


/* Shows menu items and pointing arrow	*/
#define SPRX (16)
#define OFF_X 0

/* Re-adujusting Menu Y position */
#define OFF_Y (-6)

void screen_showchar(SDL_Surface *s, int x, int y, unsigned char a, const int fg_color, const int bg_color);

void print_string(const char *s, const  unsigned short fg_color, const unsigned short bg_color, int x, int y);
void print_string_video(short x, const short y, const char *s) ;

void clear_screen_menu(void);

void draw_bluerect_menu(unsigned char i);
void draw_bluerect_file(unsigned char i);
