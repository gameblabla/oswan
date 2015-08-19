#ifdef NSPIRE
#include <os.h>
#include <math.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "shared.h"
#include "WSFileio.h"
#include "font.h" // Font c array

#include "./data/oswan_background.h"
#include "./data/oswan_load.h"
#include "./data/oswan_skin.h"

extern unsigned int m_Flag;

bool gameMenu;
char filename_without_extensions[256];
void get_filename(const char *filename);

#ifdef LAYERS
#define COLOR_BG            PIX_TO_RGB(layer->format,05, 03, 02)
#define COLOR_HELP_TEXT		PIX_TO_RGB(layer->format,64, 240, 96)
#define COLOR_OK			PIX_TO_RGB(layer->format,0,0,255)
#define COLOR_KO			PIX_TO_RGB(layer->format,255,0,0)
#define COLOR_INFO			PIX_TO_RGB(layer->format,0,255,0)
#define COLOR_LIGHT			PIX_TO_RGB(layer->format,255,255,0)
#define COLOR_ACTIVE_ITEM   PIX_TO_RGB(layer->format,232, 253, 77)
#define COLOR_INACTIVE_ITEM PIX_TO_RGB(layer->format,67,89,153)
#else
#define COLOR_BG            PIX_TO_RGB(actualScreen->format,05, 03, 02)
#define COLOR_HELP_TEXT		PIX_TO_RGB(actualScreen->format,64, 240, 96)
#define COLOR_OK			PIX_TO_RGB(actualScreen->format,0,0,255)
#define COLOR_KO			PIX_TO_RGB(actualScreen->format,255,0,0)
#define COLOR_INFO			PIX_TO_RGB(actualScreen->format,0,255,0)
#define COLOR_LIGHT			PIX_TO_RGB(actualScreen->format,255,255,0)
#define COLOR_ACTIVE_ITEM   PIX_TO_RGB(actualScreen->format,232, 253, 77)
#define COLOR_INACTIVE_ITEM PIX_TO_RGB(actualScreen->format,67,89,153)
#endif
char *file_ext[] = { (char *) ".ws", (char *) ".wsc", (char *) ".WSC", (char *) ".WS", ".tns", NULL };

//---------------------------------------------------------------------------------------
int system_is_load_state(void);

void menuReset(void);
void menuQuit(void);
void menuContinue(void);
void menuFileBrowse(void);
void menuSaveBmp(void);
void menuSaveState(void);
void menuLoadState(void);
void screen_showkeymenu(void);
void menuReturn(void);

//---------------------------------------------------------------------------------------
typedef struct {
	char itemName[16];
	int *itemPar;
	int itemParMaxValue;
	char *itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int itemNum; // number of items
	int itemCur; // current item
	MENUITEM *m; // array of items
} MENU;

char mnuYesNo[2][16] = {"no", "yes"};
char mnuRatio[2][16] = { "1x size","Full screen"};

char mnuButtons[7][16] = {
  "Up","Down","Left","Right","But #1","But #2", "Options"
};

MENUITEM MainMenuItems[] = {
	{"Load ROM", NULL, 0, NULL, &menuFileBrowse},
	{"Continue", NULL, 0, NULL, &menuContinue},
	{"Reset", NULL, 0, NULL, &menuReset},
	{"Ratio: ", (int *) &GameConf.m_ScreenRatio, 1, (char *) &mnuRatio, NULL},
	{"Load State", NULL, 0, NULL, &menuLoadState},
	{"Save State", NULL, 0, NULL, &menuSaveState},
	/*{"Button Settings", NULL, 0, NULL, &screen_showkeymenu},*/
	{"Take Screenshot", NULL, 0, NULL, &menuSaveBmp},
	{"Show FPS: ", (int *) &GameConf.m_DisplayFPS, 1,(char *) &mnuYesNo, NULL},
	{"Exit", NULL, 0, NULL, &menuQuit}
};


MENU mnuMainMenu = { 
	9,
	0, (MENUITEM *) &MainMenuItems };

MENUITEM ConfigMenuItems[] = {
	{"Button A: ", (int *) &GameConf.OD_Joy[4], 6, (char *)  &mnuButtons, NULL},
	{"Button B: ", (int *) &GameConf.OD_Joy[5], 6, (char *)  &mnuButtons, NULL},
	{"Button X: ", (int *) &GameConf.OD_Joy[6], 6, (char *)  &mnuButtons, NULL},
	{"Button Y: ", (int *) &GameConf.OD_Joy[7], 6, (char *)  &mnuButtons, NULL},
	{"Button R: ", (int *) &GameConf.OD_Joy[8], 6, (char *)  &mnuButtons, NULL},
	{"Button L: ", (int *) &GameConf.OD_Joy[9], 6, (char *)  &mnuButtons, NULL},
	{"START   : ", (int *) &GameConf.OD_Joy[10], 6, (char *) &mnuButtons, NULL},
	{"SELECT  : ", (int *) &GameConf.OD_Joy[11], 6, (char *) &mnuButtons, NULL},
	{"Return to menu", NULL, 0, NULL, &menuReturn},
};
MENU mnuConfigMenu = { 9, 0, (MENUITEM *) &ConfigMenuItems };

//----------------------------------------------------------------------------------------------------
// Prints char on a given surface
void screen_showchar(SDL_Surface *s, int x, int y, unsigned char a, int fg_color, int bg_color) {
	unsigned short *dst;
	int w, h;

	//if(SDL_MUSTLOCK(s)) SDL_LockSurface(s);
	SDL_LockSurface(s);
	for(h = 8; h; h--) {
		dst = (unsigned short *)s->pixels + (y+8-h)*s->w + x;
		for(w = 8; w; w--) {
			unsigned short color = *dst; // background
			if((fontdata8x8[a*8 + (8-h)] >> w) & 1) color = fg_color;
			*dst++ = color;
		}
	}
	//if(SDL_MUSTLOCK(s)) SDL_UnlockSurface(s);
	SDL_UnlockSurface(s);
}

// copy-pasted mostly from gpsp emulator by Exophaze. 	thanks for it
void print_string(const char *s, unsigned short fg_color, unsigned short bg_color, int x, int y) {
	int i, j = strlen(s);
	#ifdef LAYERS
		for(i = 0; i < j; i++, x += 6) screen_showchar(layer, x, y, s[i], fg_color, bg_color);
	#else
		for(i = 0; i < j; i++, x += 6) screen_showchar(actualScreen, x, y, s[i], fg_color, bg_color);
	#endif
}

void print_string_video(int x, int y, const char *s) {
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 8) screen_showchar(actualScreen, x, y, s[i], PIX_TO_RGB(actualScreen->format,255, 0, 0), 0);
}

void screen_showitem(int x, int y, MENUITEM *m, int fg_color) {
	static char i_str[24];

	// if no parameters, show simple menu item
	if(m->itemPar == NULL) print_string(m->itemName, fg_color, COLOR_BG, x, y);
	else {
		if(m->itemParName == NULL) {
			// if parameter is a digit
			sprintf(i_str, "%s%i", m->itemName, *m->itemPar);
		} else {
			// if parameter is a name in array
			sprintf(i_str, "%s%s", m->itemName, m->itemParName+(*m->itemPar)*16);
		}
		print_string(i_str, fg_color, COLOR_BG, x, y);
	}
}

// flip the layer to screen
void screen_flip(void) {
#ifdef LAYERS
	SDL_BlitSurface(layer, 0, actualScreen, 0);
#endif
	SDL_Flip(actualScreen);
}

// draw default emulator design
void screen_prepback(SDL_Surface *s, unsigned char *bmpBuf, unsigned int bmpSize) {
	// load logo, Convert the image to optimal display format and Free the temporary surface
	SDL_RWops *rw = SDL_RWFromMem(bmpBuf, bmpSize);
	SDL_Surface *temp = SDL_LoadBMP_RW(rw, 1);
	SDL_Surface *image;
	image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	// Display image
 	SDL_BlitSurface(image, 0, s, 0);
	SDL_FreeSurface(image);
}

// draw main emulator design
void screen_prepbackground(void) 
{
	// draw default background
#ifdef LAYERS
	screen_prepback(layerbackgrey, OSWAN_BACKGROUND, OSWAN_BACKGROUND_SIZE);
#else
	screen_prepback(actualScreen, OSWAN_BACKGROUND, OSWAN_BACKGROUND_SIZE);
#endif
}

// Shows menu items and pointing arrow
#define SPRX (16)
void screen_showmenu(MENU *menu) {
	int i;
	MENUITEM *mi = menu->m;

	/* clear buffer */
#ifdef LAYERS
	SDL_BlitSurface(layerbackgrey, 0, layer, 0);
#endif

	/* Hacky Workaround for graphical bug */
#ifndef LAYERS
	static unsigned char once = 0;
	if (menu->itemNum == 9)
	{
		if (once == 0)
		{
			screen_prepbackground();
			once = 1;
		}
	}
	else
	{
		if (once == 1) once = 0;
	}
#endif

	/* show menu lines */
	for(i = 0; i < menu->itemNum; i++, mi++) 
	{
		int fg_color;
		if(menu->itemCur == i) fg_color = COLOR_ACTIVE_ITEM; else fg_color = COLOR_INACTIVE_ITEM;
		screen_showitem(SPRX+10, 59+i*15, mi, fg_color);
		#ifdef LAYERS
			if(menu->itemCur == i) print_string("-", fg_color, COLOR_BG, SPRX+10-12, 59+i*15);
		#endif
	}
}

// wait for a key
void screen_waitkey(void) {
	bool akey=false;
		
	while (!akey) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_KEYDOWN)
				akey = true;
		}
	}
}

void screen_waitkeyarelease(void) {
	unsigned char *keys;
		
	// wait key release and go in menu
	while (1) {
		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);
		if (keys[PAD_A] != SDL_PRESSED) break;
	}
}


// Main function that runs all the stuff
void screen_showmainmenu(MENU *menu) {
	unsigned char *keys;
	MENUITEM *mi;
	char szVal[100];
	int isSta = 0;
	unsigned int keya=0, keyb=0, keyup=0, keydown=0, keyleft=0, keyright=0;

	gameMenu=true;

	while(gameMenu) 
	{
		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);
		
		mi = menu->m + menu->itemCur; // pointer to highlit menu option

		// A - apply parameter or enter submenu
		if (keys[PAD_A] == SDL_PRESSED) 
		{ 
			if (!keya) {
				keya = 1; 
				screen_waitkeyarelease();
				if (mi->itemOnA != NULL) (*mi->itemOnA)();
			}
		}
		else keya=0;

		// B - exit or back to previous menu
		if (keys[PAD_B] == SDL_PRESSED) { 
			if (!keyb) {
				keyb = 1; 
				if (menu != &mnuMainMenu) 
				{	
					gameMenu = false;
				}
			}
		}
		else keyb=0;

		// UP - arrow up
		if (keys[PAD_UP] == SDL_PRESSED) { 
			if (!keyup) {
				keyup = 1; if(--menu->itemCur < 0) menu->itemCur = menu->itemNum - 1;
			}
			else {
				keyup++; if (keyup>12) keyup=0;
			}
		}
		else keyup=0;

		//DOWN - arrow down
		if (keys[PAD_DOWN] == SDL_PRESSED) { 
			if (!keydown) {
				keydown = 1; if(++menu->itemCur == menu->itemNum) menu->itemCur = 0;
			}
			else {
				keydown++; if (keydown>12) keydown=0;
			}
		}
		else keydown=0;

		// LEFT - decrease parameter value
		if (keys[PAD_LEFT] == SDL_PRESSED) { 
			if (!keyleft) {
				keyleft = 1;
				if(mi->itemPar != NULL && *mi->itemPar > 0)
				{ 
					 *mi->itemPar -= 1;
#ifndef LAYERS
					screen_prepbackground();
#endif
				}
				
				// big hack for key conf
				if (menu == &mnuConfigMenu)  
				{
					if (*mi->itemPar < 4) *mi->itemPar = 4;
				}
			}
			else {
				keyleft++; if (keyleft>12) keyleft=0;
			}
		}
		else keyleft=0;

		// RIGHT - increase parameter value
		if (keys[PAD_RIGHT] == SDL_PRESSED) { 
			if (!keyright) {
				keyright = 1; 
				if(mi->itemPar != NULL && *mi->itemPar < mi->itemParMaxValue) 
				{ 
					*mi->itemPar += 1;
#ifndef LAYERS
					screen_prepbackground();
#endif
				}
				
			}
			else {
				keyright++; if (keyright>12) keyright=0;
			}
		}
		else keyright=0;

		if (gameMenu) 
		{
			screen_showmenu(menu); // show menu items
			if (menu == &mnuMainMenu) 
			{
				if (cartridge_IsLoaded()) 
				{
					sprintf(szVal,"Game:%s",strrchr(gameName,'/')+1);szVal[(320/6)-2] = '\0'; 
					print_string(szVal, COLOR_LIGHT,COLOR_BG, 8,240-2-10-10);
					sprintf(szVal,"CRC:%08X",gameCRC); 
					print_string(szVal, COLOR_LIGHT, COLOR_BG,8,240-2-10);
					if (isSta) print_string("Load state available",COLOR_INFO, COLOR_BG,8+104,240-2-10);
				}
			}
		}

#ifdef NSPIRE
		SDL_Delay(8);
#else
		SDL_Delay(16);
#endif
		screen_flip();
	}
	
}



// Menu function that runs keys configuration
void screen_showkeymenu(void) 
{
	screen_showmainmenu(&mnuConfigMenu);
}

// Menu function that runs main top menu
void screen_showtopmenu(void) 
{
	
#ifdef SWITCHING_GRAPHICS
	if (!GameConf.m_ScreenRatio)
	{
		SDL_FreeSurface(actualScreen);
#ifdef NSPIRE
		actualScreen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE ); 
#else
		actualScreen = SDL_SetVideoMode(320, 240, BITDEPTH_OSWAN, SDL_SWSURFACE );
#endif
		SDL_FillRect(actualScreen, NULL, 0);
		SDL_Flip(actualScreen);
	}
#endif
	
	// Save screen in layer
#ifdef LAYERS
	SDL_BlitSurface(actualScreen, 0, layerback, 0);
#endif
	screen_prepbackground();

	// Display and manage main menu
	screen_showmainmenu(&mnuMainMenu);

	// save actual config
	system_savecfg(current_conf_app);
	
#ifdef SWITCHING_GRAPHICS
	if (!GameConf.m_ScreenRatio)
	{
		SDL_FreeSurface(actualScreen);
		screen_prepbackground();
#ifdef NSPIRE
		actualScreen = SDL_SetVideoMode(224, 144, has_colors ? 16 : 8, SDL_SWSURFACE ); 
#else
		actualScreen = SDL_SetVideoMode(224, 144, BITDEPTH_OSWAN, SDL_SWSURFACE );
#endif
		SDL_FillRect(actualScreen, NULL, 0);
		SDL_Flip(actualScreen);
	}
#else
	// if no ratio, put skin
	if (!GameConf.m_ScreenRatio) 
	{
		screen_prepback(actualScreen, OSWAN_SKIN, OSWAN_SKIN_SIZE);
		SDL_Flip(actualScreen);
	}
#endif
	
}

//----------------------------------------------------------------------
int system_is_load_state(void) {
	char name[512];
	int fd;
	int n=0;
  
	strcpy(name, gameName);
#ifdef NSPIRE
	strcpy(strrchr(name, '.'), ".sta.tns");
#else
	strcpy(strrchr(name, '.'), ".sta");
#endif

	fd = open(name, O_RDONLY | O_BINARY);
	if (fd >= 0) {
		n = 1;
		close(fd);
	}
	
	return (n ? 1 : 0);
}

// find a filename for bmp or state saving 
void findNextFilename(char *szFileFormat, char *szFilename) 
{
#if defined(DREAMCAST) || defined(KOLIBRI)
	int uBcl;
#else
	uint32_t uBcl;
#endif
	
	int fp;
  
	for (uBcl = 0; uBcl<999; uBcl++) {
		sprintf(szFilename,szFileFormat,uBcl);
		fp = open(szFilename,O_RDONLY | O_BINARY);
		if (fp <0) break;
		close(fp);
	}
	if (uBcl == 1000)
		strcpy(szFilename,"NOSLOT");
	if (fp>=0) close(fp);
}

// Reset current game
void menuReset(void) 
{
	if (cartridge_IsLoaded()) {
		WsReset();
		gameMenu=false;
		m_Flag = GF_GAMERUNNING;
	}
}

// Quit oswan emulator (oh noooo :P)
void menuQuit(void) {
	if (cartridge_IsLoaded()) 
	{
		WsDeInit();
	}
	gameMenu=false;
	m_Flag = GF_GAMEQUIT;
}

// Return to game if loaded
void menuContinue(void) 
{
	if (cartridge_IsLoaded()) 
	{
		gameMenu=false;
		m_Flag = GF_GAMERUNNING;
	}
}

// Rom file browser which is called from menu
#define MAX_FILES 512
typedef struct  
{
	char name[255];
	unsigned int type;
} filedirtype;
filedirtype filedir_list[MAX_FILES];

int sort_function(const void *src_str_ptr, const void *dest_str_ptr) 
{
  filedirtype *p1 = (filedirtype *) src_str_ptr;
  filedirtype *p2 = (filedirtype *) dest_str_ptr;
  
  return strcmp (p1->name, p2->name);
}

int strcmp_function(char *s1, char *s2)
{
	int i;
	/*int c;*/
	
	if (strlen(s1) != strlen(s2)) return 1;

	for(i=0; i<strlen(s1); i++) {
		if (toupper(s1[i]) != toupper(s2[i])) 
			return 1;
    }
	return 0;
}

signed int load_file(char **wildcards, char *result) 
{
	unsigned char *keys;
	unsigned int keya=0, keyb=0, keyup=0, kepufl=8, keydown=0, kepdfl=8 /*keyleft=0, keyright=0,*/;

	char current_dir_name[MAX__PATH];
	DIR *current_dir;
	struct dirent *current_file;
	struct stat file_info;
	char current_dir_short[81];
	unsigned int current_dir_length;
	unsigned int num_filedir;

	char *file_name;
	unsigned int file_name_length;
	unsigned int ext_pos = -1;
	/*unsigned int dialog_result = 1;*/
	signed int return_value = 1;
	unsigned int repeat;
	unsigned int i;

	unsigned int current_filedir_scroll_value;
	unsigned int current_filedir_selection;
	unsigned int current_filedir_in_scroll;
	unsigned int current_filedir_number;
	
	// Init dir with saved one
	strcpy(current_dir_name,GameConf.current_dir_rom);
	chdir(GameConf.current_dir_rom);

	while (return_value == 1) 
	{
		current_filedir_in_scroll = 0;
		current_filedir_scroll_value  = 0;
		current_filedir_number = 0;
		current_filedir_selection = 0;
		num_filedir = 0;
		
		getcwd(current_dir_name, MAX__PATH);
		current_dir = opendir(current_dir_name);
		
		do {
			if(current_dir) current_file = readdir(current_dir); else current_file = NULL;

			if(current_file)
			{
				file_name = current_file->d_name;
				file_name_length = strlen(file_name);

				if((stat(file_name, &file_info) >= 0) && ((file_name[0] != '.') || (file_name[1] == '.'))) 
				{
					if(S_ISDIR(file_info.st_mode)) 
					{
						filedir_list[num_filedir].type = 1; // 1 -> directory
						strcpy(filedir_list[num_filedir].name, file_name);
						num_filedir++;
						
					} else 
					{
						// Must match one of the wildcards, also ignore the .
						if(file_name_length >= 4) {
							if(file_name[file_name_length - 4] == '.') ext_pos = file_name_length - 4;
							else if(file_name[file_name_length - 3] == '.') ext_pos = file_name_length - 3;
							else ext_pos = 0;

							for(i = 0; wildcards[i] != NULL; i++) {
								if(!strcmp_function((file_name + ext_pos), wildcards[i])) {
									filedir_list[num_filedir].type = 0; // 0 -> file
									strcpy(filedir_list[num_filedir].name, file_name);
									num_filedir++;

									break;
								}
							}
						}
					}
				}
			}
		} while(current_file);

		if (num_filedir)
			qsort((void *)filedir_list, num_filedir, sizeof(filedirtype), sort_function);

		closedir(current_dir);

		current_dir_length = strlen(current_dir_name);
		if(current_dir_length > 39) {
			memcpy(current_dir_short, "...", 3);
			memcpy(current_dir_short + 3, current_dir_name + current_dir_length - (39-3), (39-3));
			current_dir_short[39] = 0;
		} else {
			memcpy(current_dir_short, current_dir_name, current_dir_length + 1);
		}

		repeat = 1;

		char print_buffer[81];

		while(repeat) 
		{
#ifdef LAYERS
			screen_prepback(layer, OSWAN_LOAD, OSWAN_LOAD_SIZE);
#else
			screen_prepback(actualScreen, OSWAN_LOAD, OSWAN_LOAD_SIZE);
#endif
			print_string(current_dir_short, COLOR_ACTIVE_ITEM, COLOR_BG, 4, 10*3);
			print_string("Press B to return to the main menu", COLOR_HELP_TEXT, COLOR_BG, 160-(34*8/2), 240-5 -10*3);
			for(i = 0, current_filedir_number = i + current_filedir_scroll_value; i < FILE_LIST_ROWS; i++, current_filedir_number++) {
#define CHARLEN ((320/6)-2)
				if(current_filedir_number < num_filedir) {
					if (filedir_list[current_filedir_number].type == 0) { // file (0) or dir (1) ?
						strncpy(print_buffer,filedir_list[current_filedir_number].name, CHARLEN);
					}
					else {
						strncpy(print_buffer+1,filedir_list[current_filedir_number].name, CHARLEN-1);
						print_buffer[0] = '[';
						if (strlen(filedir_list[current_filedir_number].name)<(CHARLEN-1)) 
							print_buffer[strlen(filedir_list[current_filedir_number].name)+1] = ']';
						else
							print_buffer[CHARLEN-1] = ']';
					}
					print_buffer[CHARLEN] = 0;
					if(current_filedir_number == current_filedir_selection) {
						print_string(print_buffer, COLOR_ACTIVE_ITEM, COLOR_BG, 4, 10*3+((i + 2) * 8));
					} else {
						print_string(print_buffer, COLOR_INACTIVE_ITEM, COLOR_BG, 4,10*3+ ((i + 2) * 8));
					}
				}
			}

			// Catch input
			SDL_PollEvent(&event);
			keys = SDL_GetKeyState(NULL);

			// A - choose file or enter directory
			if (keys[PAD_A] == SDL_PRESSED) { 
				if (!keya) {
					keya = 1; 
					screen_waitkeyarelease();
					if (filedir_list[current_filedir_selection].type == 1)  { // so it's a directory
						repeat = 0;
						chdir(filedir_list[current_filedir_selection].name);
					}
					else {
						repeat = 0;
						return_value = 0;
						sprintf(result, "%s/%s", current_dir_name, filedir_list[current_filedir_selection].name);
					}
				}
			}
			else keya=0;

			// B - exit or back to previous menu
			if (keys[PAD_B] == SDL_PRESSED) { 
				if (!keyb) {
					keyb = 1; 
					return_value = -1;
					repeat = 0;
				}
			}
			else keyb=0;

			// UP - arrow up
			if (keys[PAD_UP] == SDL_PRESSED) { 
				if (!keyup) {
					keyup = 1; 
					if(current_filedir_selection) {
						current_filedir_selection--;
						if(current_filedir_in_scroll == 0) {
							current_filedir_scroll_value--;
						} else {
							current_filedir_in_scroll--;
						}
					}
				}
				else {
					keyup++; if (keyup>kepufl) keyup=0;
					if (kepufl>2) kepufl--; 
				}
			}
			else { keyup=0; kepufl = 8; }

			//DOWN - arrow down
			if (keys[PAD_DOWN] == SDL_PRESSED) { 
				if (!keydown) {
					keydown = 1; 
					if(current_filedir_selection < (num_filedir - 1)) {
						current_filedir_selection++;
						if(current_filedir_in_scroll == (FILE_LIST_ROWS - 1)) {
							current_filedir_scroll_value++;
						} else {
							current_filedir_in_scroll++;
						}
					}
				}
				else {
					keydown++; if (keydown>kepdfl) keydown=0;
					if (kepdfl>2) kepdfl--; 
				}
			}
			else { keydown=0;	kepdfl = 8; }

			SDL_Delay(16);
			screen_flip();
		}
	}

	// Save current rom directory
	if (return_value != -1) {
		strcpy(GameConf.current_dir_rom,current_dir_name);
	}

	/* 
	   Hack, Show Menu graphics again after you quit ROM file browser
	   Remove this piece of code and the menu background will not be shown,
	   only the last one (the file browser one) !
	*/
#ifndef LAYERS
		screen_prepbackground();
#endif
	
	return return_value;
}

void menuFileBrowse(void) 
{
	if (load_file(file_ext, gameName) != -1) { // exit if file is chosen
		gameMenu=false;
		m_Flag = GF_GAMEINIT;
		// free memory if another game is loaded
		if (cartridge_IsLoaded()) 
			WsDeInit();
	}
}

// Take a screenshot of current game
void menuSaveBmp(void) 
{
#ifndef NO_SCREENSHOT
    char szFile[512], szFile1[512];
	
	if (cartridge_IsLoaded()) {
		sprintf(szFile,"./%s",strrchr(gameName,'/')+1);

#ifdef NSPIRE
		szFile[strlen(szFile)-12] = '%';
		szFile[strlen(szFile)-11] = '0';
		szFile[strlen(szFile)-10] = '3';
		szFile[strlen(szFile)-9] = 'd';
		szFile[strlen(szFile)-8] = '.';
		szFile[strlen(szFile)-7] = 'b';
		szFile[strlen(szFile)-6] = 'm';
		szFile[strlen(szFile)-5] = 'p';
		szFile[strlen(szFile)-4] = '.';
		szFile[strlen(szFile)-3] = 't';
		szFile[strlen(szFile)-2] = 'n';
		szFile[strlen(szFile)-1] = 's';
#else
		szFile[strlen(szFile)-8] = '%';
		szFile[strlen(szFile)-7] = '0';
		szFile[strlen(szFile)-6] = '3';
		szFile[strlen(szFile)-5] = 'd';
		szFile[strlen(szFile)-4] = '.';
		szFile[strlen(szFile)-3] = 'b';
		szFile[strlen(szFile)-2] = 'm';
		szFile[strlen(szFile)-1] = 'p';
#endif

		print_string("Saving...", COLOR_OK, COLOR_BG, 8,240-5 -10*3);
		screen_flip();
		findNextFilename(szFile,szFile1);
#ifdef LAYERS
		SDL_SaveBMP(layerback, szFile1);
#else
		SDL_SaveBMP(actualScreen, szFile1);
#endif
		print_string("Screen saved !", COLOR_OK, COLOR_BG, 8+10*8,240-5 -10*3);
		screen_flip();
		screen_waitkey();
	}
#endif
}

// Save current state of game emulated
void menuSaveState(void) 
{
    char szFile[512];
	
	if (cartridge_IsLoaded()) 
	{
		strcpy(szFile, gameName);
#ifdef NSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		print_string("Saving...", COLOR_OK, COLOR_BG, 8,240-5 -10*3);
		WsSaveState(szFile, 0);
		print_string("Save OK",COLOR_OK,COLOR_BG, 8+10*8,240-5 -10*3);
		screen_flip();
		screen_waitkey();
	}
}

// Load current state of game emulated
void menuLoadState(void) 
{
    char szFile[512];
	
	if (cartridge_IsLoaded()) 
	{
		strcpy(szFile, gameName);
#ifdef NSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		print_string("Loading...", COLOR_OK, COLOR_BG, 8,240-5 -10*3);
		WsLoadState(szFile, 0);
		print_string("Load OK",COLOR_OK,COLOR_BG, 8+10*8,240-5 -10*3);
		screen_flip();
		screen_waitkey();
		gameMenu=false;
		m_Flag = GF_GAMERUNNING;
	}
}

// Go back to menu
void menuReturn(void) 
{
	gameMenu=false;
}


void system_loadcfg(char *cfg_name) 
{
  int fd;

  fd = open(cfg_name, O_RDONLY | O_BINARY);
  if (fd >= 0) 
  {
	read(fd, &GameConf, sizeof(GameConf));
    close(fd);
#ifndef SWITCHING_GRAPHICS
	if (!GameConf.m_ScreenRatio) 
	{
		screen_prepback(actualScreen, OSWAN_SKIN, OSWAN_SKIN_SIZE);
		SDL_Flip(actualScreen);
	}
#else
		SDL_Flip(actualScreen);
#endif
  }
  else 
  {
	  // UP  DOWN  LEFT RIGHT  A  B  X  Y  R  L  START  SELECT
	  //  0,    1,    2,    3, 4, 5, 4, 5, 4, 5,     6,      6
		GameConf.OD_Joy[ 0] = 0;  GameConf.OD_Joy[ 1] = 1;
		GameConf.OD_Joy[ 2] = 2;  GameConf.OD_Joy[ 3] = 3;
		GameConf.OD_Joy[ 4] = 4;  GameConf.OD_Joy[ 5] = 5;
		GameConf.OD_Joy[ 6] = 4;  GameConf.OD_Joy[ 7] = 5;
		GameConf.OD_Joy[ 8] = 4;  GameConf.OD_Joy[ 9] = 5;
		GameConf.OD_Joy[10] = 6;  GameConf.OD_Joy[11] = 6;
	   
		GameConf.sndLevel=40;
		GameConf.m_ScreenRatio=1; // 0 = original show, 1 = full screen
		GameConf.m_DisplayFPS=1; // 0 = no
		getcwd(GameConf.current_dir_rom, MAX__PATH);
	}
}

void system_savecfg(char *cfg_name) 
{
	int fd;
	fd = open(cfg_name, O_CREAT | O_RDWR | O_BINARY | O_TRUNC, S_IREAD | S_IWRITE);
	
	if (fd >= 0) 
	{
		write(fd, &GameConf, sizeof(GameConf)); 
		close(fd);
	}
}

void get_filename(const char *filename)
{

}

