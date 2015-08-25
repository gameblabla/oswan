#ifdef _TINSPIRE
#include <os.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "shared.h"
#include "drawing.h"
#include "WSFileio.h"
#include "font.h" // Font c array

extern unsigned int m_Flag;
unsigned char gameMenu;

SDL_Rect position_select;

#define COLOR_BG           	SDL_MapRGB(actualScreen->format,5,3,2)
#define COLOR_OK			SDL_MapRGB(actualScreen->format,0,0,255)
#define COLOR_KO			SDL_MapRGB(actualScreen->format,255,0,0)
#define COLOR_INFO			SDL_MapRGB(actualScreen->format,0,255,0)
#define COLOR_LIGHT			SDL_MapRGB(actualScreen->format,255,255,0)
#define COLOR_ACTIVE_ITEM   SDL_MapRGB(actualScreen->format,255, 255, 255)
#define COLOR_INACTIVE_ITEM SDL_MapRGB(actualScreen->format,255,255,255)

const char *file_ext[] = { (const char *) ".ws",  (const char *) ".wsc",  (const char *) ".WSC",  (const char *) ".WS", 
#ifdef _TINSPIRE
	 (const char *) ".tns", 
#endif
	NULL };


void menuReset(void);
void menuQuit(void);
void menuContinue(void);
void menuFileBrowse(void);
void menuSaveState(void);
void menuLoadState(void);
void screen_showkeymenu(void);
unsigned char ifactive(const unsigned char *keys);
void menuReturn(void);
#if !defined(NOSCREENSHOTS)
void menuSaveBmp(void);
#endif

//---------------------------------------------------------------------------------------
typedef struct {
	const char itemName[16];
	short *itemPar;
	short itemParMaxValue;
	const char *itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int itemNum; // number of items
	int itemCur; // current item
	MENUITEM *m; // array of items
} MENU;

const char mnuABXY[4][16] = {"Normal", "Wonderswan-like", "Swap DPAD,ABXY", "Swap ABXY,Stick"};
const char mnuYesNo[2][16] = {"No", "Yes"};
const char mnuRatio[2][16] = { "1x size","Full screen"};
const char mnuSaves[10][16] = { "1","2","3","4","5","6","7","8","9"};


MENUITEM MainMenuItems[] = {
	{"Load ROM", NULL, 0, NULL, &menuFileBrowse},
	{"Continue", NULL, 0, NULL, &menuContinue},
	{"Reset", NULL, 0, NULL, &menuReset},
	{"Load State: ", (short *) &GameConf.reserved1, 8, (char *) &mnuSaves, &menuLoadState},
	{"Save State: ", (short *) &GameConf.reserved2, 8, (char *) &mnuSaves, &menuSaveState},
	{"Show FPS: ", (short *) &GameConf.m_DisplayFPS, 1,(char *) &mnuYesNo, NULL},
#if !defined(NOSCREENSHOTS)
	{"Take Screenshot", NULL, 0, NULL, &menuSaveBmp},
#endif
	{"Ratio: ", (short *) &GameConf.m_ScreenRatio, 1, (char *) &mnuRatio, NULL},
	{"", (short *) &GameConf.input_layout, 3, (char *) &mnuABXY, NULL},
	{"Exit", NULL, 0, NULL, &menuQuit}
};


MENU mnuMainMenu = { 
#if defined(NOSCREENSHOTS)
	9,
#else
	10,
#endif
	0, (MENUITEM *) &MainMenuItems };

//----------------------------------------------------------------------------------------------------
// Prints char on a given surface
void screen_showchar(SDL_Surface *s, const short x, const short y, unsigned char a, const int fg_color, const int bg_color) 
{
	unsigned short *dst;
	unsigned short w, h;

	//if(SDL_MUSTLOCK(s)) SDL_LockSurface(s);
	SDL_LockSurface(s);
	for(h = 8; h; h--) 
	{
#if BITDEPTH_OSWAN == 32
		dst = (unsigned short *)s->pixels + ((y+8-h)*s->w + x)*2;
#else
		dst = (unsigned short *)s->pixels + ((y+8-h)*s->w + x);
#endif
		for(w = 8; w; w--) 
		{
			unsigned short color = *dst; // background
			if((fontdata8x8[a*8 + (8-h)] >> w) & 1) color = fg_color;
			*dst++ = color;
#if BITDEPTH_OSWAN == 32
			*dst++ = color;
#endif
		}
	}
	//if(SDL_MUSTLOCK(s)) SDL_UnlockSurface(s);
	SDL_UnlockSurface(s);
}

// copy-pasted mostly from gpsp emulator by Exophaze. 	thanks for it
void print_string(const char *s, const  unsigned short fg_color, const unsigned short bg_color, short x, const short y) 
{
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 6) screen_showchar(actualScreen, x, y, s[i], fg_color, bg_color);
}

void screen_showitem(const short x, const short y, MENUITEM *m, int fg_color) 
{
	static char i_str[24];

	// if no parameters, show simple menu item
	if(m->itemPar == NULL) print_string(m->itemName, fg_color, COLOR_BG, x, y);
	else {
		if(m->itemParName == NULL) {
			// if parameter is a digit
			snprintf(i_str, sizeof(i_str), "%s%i", m->itemName, *m->itemPar);
		} else {
			// if parameter is a name in array
			snprintf(i_str, sizeof(i_str), "%s%s", m->itemName, m->itemParName+(*m->itemPar)*16);
		}
		print_string(i_str, fg_color, COLOR_BG, x, y);
	}
}

void print_string_video(short x, const short y, const char *s) 
{
	int i, j = strlen(s);
	for(i = 0; i < j; i++, x += 8) screen_showchar(actualScreen, x, y, s[i], SDL_MapRGB(actualScreen->format,255, 0, 0), 0);
}

// Clear screen
void screen_prepback(SDL_Surface *s) 
{
	// load logo, Convert the image to optimal display format and Free the temporary surface
	SDL_FillRect(s, NULL, 0);
}

// Shows menu items and pointing arrow
#define SPRX (16)
void screen_showmenu(MENU *menu) 
{
	unsigned char i;
	MENUITEM *mi = menu->m;
	char szVal[100];
	
	/* show menu lines */
	for(i = 0; i < menu->itemNum; i++, mi++) 
	{
		unsigned short fg_color;
		
		if(menu->itemCur == i)
		{
			position_select.w  = 320;
			position_select.h  = 12;
			position_select.x  = 0;
			position_select.y  = (44+i*15)-2+8;
			SDL_FillRect(actualScreen, &position_select, SDL_MapRGB(actualScreen->format,0,0,255));
		}
		
		if(menu->itemCur == i) 
			fg_color = COLOR_ACTIVE_ITEM; 
		else 
			fg_color = COLOR_INACTIVE_ITEM;
		screen_showitem((SPRX+10)+72, (44+i*15)+8, mi, fg_color);
	}
	
	if (cartridge_IsLoaded()) 
	{
		snprintf(szVal, sizeof(szVal), "Game:%s",strrchr(gameName,'/')+1);
		szVal[(320/6)-2] = '\0'; 
		print_string(szVal, COLOR_LIGHT, COLOR_BG, 8,240-2-10-10);
		/*sprintf(szVal,"CRC:%08X",gameCRC); 
		print_string(szVal, COLOR_LIGHT, COLOR_BG,8,240-2-10);*/
	}
	
	flip_screen(actualScreen);
}

// wait for a key
void screen_waitkey(void) 
{
	unsigned char akey=false;
		
	while (!akey) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_KEYDOWN)
				akey = true;
		}
	}
}

void screen_waitkeyarelease(void) 
{
	unsigned char *keys;
		
	// wait key release and go in menu
	while (1) 
	{
		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);
		if (keys[PAD_A] != SDL_PRESSED) break;
	}
}

unsigned char ifactive(const unsigned char *keys)
{
	if (keys[PAD_A] || keys[PAD_B] || keys[PAD_UP] || keys[PAD_DOWN] || keys[PAD_LEFT] || keys[PAD_RIGHT]) 
		return 1; 	// Yes, active
	else
		return 0;	// No, inactive
}

// Main function that runs all the stuff
void screen_showmainmenu(MENU *menu) 
{
	/* [0] == LEFT,  [1] == RIGHT*/
	static unsigned char button_state[5], button_time[5];
	unsigned short pad, i;
	unsigned char *keys;
	unsigned char keya=0, keyb=0, keyup=0, keydown=0, keyleft=0, keyright=0;
	MENUITEM *mi;

	/* Show Menu on-screen (Load ROM, Reset...)*/
	screen_showmenu(menu);

	gameMenu=true;

	while(gameMenu) 
	{
		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);
		
		for(i=0;i<4;i++)
		{	
			if (i==0) pad = PAD_LEFT;
			else if (i==1) pad = PAD_RIGHT;
			else if (i==2) pad = PAD_UP;
			else if (i==3) pad = PAD_DOWN;
			
			switch (button_state[i])
			{
				case 0:
					if (keys[pad] == SDL_PRESSED)
					{
						button_state[i] = 1;
						button_time[i] = 0;
					}
				break;
				
				case 1:
					button_time[i]++;
					
					if (button_time[i] > 0)
					{
						button_state[i] = 2;
						button_time[i] = 0;
					}
				break;
				
				case 2:
					if (!(keys[pad] == SDL_PRESSED))
					{
						button_state[i] = 3;
						button_time[i] = 0;
					}
				break;
				
				case 3:
					button_time[i]++;
					
					if (button_time[i] > 1)
					{
						button_state[i] = 0;
						button_time[i] = 0;
					}
				break;
			}
		}	
		
		mi = menu->m + menu->itemCur; // pointer to highlit menu option

		// A - apply parameter or enter submenu
		if (keys[PAD_A] == SDL_PRESSED) 
		{ 
			if (!keya) 
			{
				keya = 1; 
				screen_waitkeyarelease();
				if (mi->itemOnA != NULL) (*mi->itemOnA)();
			}
		}
		else keya=0;

		// B - exit or back to previous menu
		if (keys[PAD_B] == SDL_PRESSED) 
		{ 
			if (!keyb) 
			{
				keyb = 1; 
				if (menu != &mnuMainMenu) 
				{	
					gameMenu = false;
				}
				else
				{
					menuContinue();
				}
			}
		}
		else keyb=0;

		// UP - arrow up
		if (button_state[2]==1) 
		{ 
			if (!keyup) 
			{
				keyup = 1; 
				SDL_FillRect(actualScreen, NULL, 0);
				if(--menu->itemCur < 0)
				{
					menu->itemCur = menu->itemNum - 1;
				}
			}
			else 
			{
				keyup++; if (keyup>12) keyup=0;
			}
		}
		else keyup=0;

		//DOWN - arrow down
		if (button_state[3]==1) 
		{ 
			if (!keydown) 
			{
				keydown = 1; 
				SDL_FillRect(actualScreen, NULL, 0);
				if(++menu->itemCur == menu->itemNum) 
				{
					menu->itemCur = 0;
				}
			}
			else 
			{
				keydown++; if (keydown>12) keydown=0;
			}
		}
		else keydown=0;

		// LEFT - decrease parameter value
		if (button_state[0]==1) 
		{ 
			if (!keyleft) 
			{
				keyleft = 1;
				if(mi->itemPar != NULL && *mi->itemPar > 0)
				{ 
					 *mi->itemPar -= 1;
				      SDL_FillRect(actualScreen, NULL, 0);
				}
			}
			else 
			{
				keyleft++; if (keyleft>12) keyleft=0;
			}
		}
		else keyleft=0;

		// RIGHT - increase parameter value
		if (button_state[1]==1) 
		{ 
			if (!keyright) 
			{
				keyright = 1; 
				if(mi->itemPar != NULL && *mi->itemPar < mi->itemParMaxValue) 
				{ 
					*mi->itemPar += 1;
					SDL_FillRect(actualScreen, NULL, 0);
				}
				
			}
			else 
			{
				keyright++; if (keyright>12) keyright=0;
			}
		}
		else keyright=0;

		if (gameMenu) 
		{
			if (ifactive(keys)) 
			{
				screen_showmenu(menu); // show menu items
			}	
			else
			{
#ifdef _TINSPIRE
		sleep(1);
#else
		SDL_Delay(4);
#endif	
			}
		}
	}
	
}


// Menu function that runs main top menu
void screen_showtopmenu(void) 
{
#ifdef SWITCHING_GRAPHICS
	if (!GameConf.m_ScreenRatio)
	{
		SetVideo(0);
	}
#endif

	SDL_FillRect(actualScreen, NULL, 0);

	// Display and manage main menu
	screen_showmainmenu(&mnuMainMenu);

	// save actual config
	system_savecfg(current_conf_app);
	
	if (!GameConf.m_ScreenRatio)
	{
#ifdef SWITCHING_GRAPHICS
		SetVideo(1);
#endif
		screen_prepback(actualScreen);
		flip_screen(actualScreen);
	}
	
}

// find a filename for bmp or state saving 
void findNextFilename(const char *szFileFormat, char *szFilename) 
{
	unsigned short uBcl;
	short fp;
	
	for (uBcl = 0; uBcl<999; uBcl++) 
	{
		snprintf(szFilename, 512, szFileFormat, uBcl);
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

char strcmp_function(const char *s1, const char *s2)
{
	int i;

	if (strlen(s1) != strlen(s2)) return 1;

	for(i=0; i<strlen(s1); i++) {
		if (toupper(s1[i]) != toupper(s2[i])) 
			return 1;
    }
	return 0;
}

signed int load_file(const char **wildcards, char *result) 
{
	unsigned char *keys;
	unsigned int keya=0, keyb=0, keyup=0, kepufl=8, keydown=0, kepdfl=8 /*keyleft=0, keyright=0,*/;
	
	static unsigned char button_state[7], button_time[7];
	int pad;

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
	signed int return_value = 1;
	unsigned int repeat;
	unsigned int i;

	unsigned int current_filedir_scroll_value;
	unsigned int current_filedir_selection;
	unsigned int current_filedir_in_scroll;
	unsigned int current_filedir_number;
	
	char print_buffer[81];
	
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
	
#ifdef DREAMCAST
		snprintf(current_dir_name, sizeof(current_dir_name), "/cd/");
#else
		getcwd(current_dir_name, MAX__PATH);
#endif
		current_dir = opendir(current_dir_name);
		
		do 
		{
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
						
					} 
					else 
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
		if(current_dir_length > 39) 
		{
			memcpy(current_dir_short, "...", 3);
			memcpy(current_dir_short + 3, current_dir_name + current_dir_length - (39-3), (39-3));
			current_dir_short[39] = 0;
		} 
		else 
		{
			memcpy(current_dir_short, current_dir_name, current_dir_length + 1);
		}

		repeat = 1;

		while(repeat) 
		{
			#define CHARLEN ((320/6)-2)
			
			screen_prepback(actualScreen);
			
			print_string(current_dir_short, COLOR_ACTIVE_ITEM, COLOR_BG, 4, 10*3);
			print_string("Press B to return to the main menu", COLOR_ACTIVE_ITEM, COLOR_BG, 160-(34*8/2), 240-5 -10*3);
				
			for(i = 0, current_filedir_number = i + current_filedir_scroll_value; i < FILE_LIST_ROWS; i++, current_filedir_number++)
			{
				if(current_filedir_number < num_filedir) 
				{
					if (filedir_list[current_filedir_number].type == 0) // file (0) or dir (1) ?
					{ 
						strncpy(print_buffer,filedir_list[current_filedir_number].name, CHARLEN);
					}
					else 
					{
						strncpy(print_buffer+1,filedir_list[current_filedir_number].name, CHARLEN-1);
						print_buffer[0] = '[';
						if (strlen(filedir_list[current_filedir_number].name)<(CHARLEN-1)) 
							print_buffer[strlen(filedir_list[current_filedir_number].name)+1] = ']';
						else
							print_buffer[CHARLEN-1] = ']';
					}
						
					print_buffer[CHARLEN] = 0;
						
					if(current_filedir_number == current_filedir_selection) 
					{
						/* Put Blue rectangle on screen before the text is printed*/
						position_select.w  = 320;
						position_select.h  = 8;
						position_select.x  = 0;
						position_select.y  = 10*3+((i + 2) * 8);
						SDL_FillRect(actualScreen, &position_select, SDL_MapRGB(actualScreen->format,0,0,255));
						print_string(print_buffer, COLOR_ACTIVE_ITEM, COLOR_BG, 4, 10*3+((i + 2) * 8));
					} 
					else 
					{
						print_string(print_buffer, COLOR_INACTIVE_ITEM, COLOR_BG, 4,10*3+ ((i + 2) * 8));
					}
				}
			}
		
			// Catch input
			SDL_PollEvent(&event);
			keys = SDL_GetKeyState(NULL);
			
			/* Detect Hold, Pressed, released... buttons */
			for(i=0;i<6;i++)
			{	
				if (i==0) pad = PAD_LEFT;
				else if (i==1) pad = PAD_RIGHT;
				else if (i==2) pad = PAD_UP;
				else if (i==3) pad = PAD_DOWN;
				else if (i==4) pad = PAD_L;
				else if (i==5) pad = PAD_R;
				
				switch (button_state[i])
				{
					case 0:
						if (keys[pad] == SDL_PRESSED)
						{
							button_state[i] = 1;
							button_time[i] = 0;
						}
					break;
					
					case 1:
						button_time[i]++;
						
						if (button_time[i] > 0)
						{
							button_state[i] = 2;
							button_time[i] = 0;
						}
					break;
					
					case 2:
						if (!(keys[pad] == SDL_PRESSED))
						{
							button_state[i] = 3;
							button_time[i] = 0;
						}
					break;
					
					case 3:
						button_time[i]++;
						
						if (button_time[i] > 1)
						{
							button_state[i] = 0;
							button_time[i] = 0;
						}
					break;
				}
			}	
		

			// A - choose file or enter directory
			if (keys[PAD_A] == SDL_PRESSED) 
			{ 
				if (!keya) 
				{
					keya = 1; 
					screen_waitkeyarelease();
					if (filedir_list[current_filedir_selection].type == 1)   // so it's a directory
					{ 
						repeat = 0;
						chdir(filedir_list[current_filedir_selection].name);
					}
					else 
					{
						repeat = 0;
						return_value = 0;
						snprintf(result, sizeof(gameName), "%s/%s", current_dir_name, filedir_list[current_filedir_selection].name);
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

			// UP or (L shoulder) - arrow up
			if (button_state[2]==1 || button_state[4]==2) 
			{ 
				if (!keyup) 
				{
					keyup = 1; 
					if(current_filedir_selection) 
					{
						current_filedir_selection--;
						
						if(current_filedir_in_scroll == 0) 
						{
							current_filedir_scroll_value--;
						} 
						else 
						{
							current_filedir_in_scroll--;
						}
					}
				}
				else 
				{
					keyup++; 
					if (keyup>kepufl) keyup=0;
					if (kepufl>2) kepufl--; 
				}
			}
			else 
			{ 
				keyup=0; 
				kepufl = 8; 
			}

			//DOWN or (R shoulder) - arrow down
			if (button_state[3]==1 || button_state[5]==2) 
			{ 
				if (!keydown) {
					keydown = 1; 
					if(current_filedir_selection < (num_filedir - 1)) 
					{
						current_filedir_selection++;
						
						if(current_filedir_in_scroll == (FILE_LIST_ROWS - 1)) 
						{
							current_filedir_scroll_value++;
						} 
						else 
						{
							current_filedir_in_scroll++;
						}
					}
				}
				else {
					keydown++; if (keydown>kepdfl) keydown=0;
					if (kepdfl>2) kepdfl--; 
				}
			}
			else
			{ 
				keydown=0;	
				kepdfl = 8; 
			}
			
			flip_screen(actualScreen);
#ifdef _TINSPIRE
		sleep(1);
#else
		SDL_Delay(4);
#endif	
			
		}
	}

	// Save current rom directory
	if (return_value != -1) {
		strcpy(GameConf.current_dir_rom,current_dir_name);
	}

	SDL_FillRect(actualScreen, NULL, 0);

	return return_value;
}


void menuFileBrowse(void) 
{
	if (load_file(file_ext, gameName) != -1) // exit if file is chosen
	{ 
		gameMenu=false;
		m_Flag = GF_GAMEINIT;
		// free memory if another game is loaded
		if (cartridge_IsLoaded()) 
			WsDeInit();
	}
}

// Take a screenshot of current game
#if !defined(NOSCREENSHOTS)
void menuSaveBmp(void) 
{
    char szFile[512], szFile1[512];
	
	if (cartridge_IsLoaded()) {
		snprintf(szFile, sizeof(szFile), "%s%s%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(gameName,'/')+1);

#ifdef _TINSPIRE
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
		flip_screen(actualScreen);
		findNextFilename(szFile,szFile1);
		SDL_SaveBMP(screenshots, szFile1);
		print_string("Screen saved !", COLOR_OK, COLOR_BG, 8+10*8,240-5 -10*3);
		flip_screen(actualScreen);
		screen_waitkey();
	}
}
#endif

// Save current state of game emulated
void menuSaveState(void) 
{
    char szFile[512];
	
	if (cartridge_IsLoaded()) 
	{
		strcpy(szFile, gameName);
#ifdef _TINSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		print_string("Saving...", COLOR_OK, COLOR_BG, 8,240-5 -10*3);
		WsSaveState(szFile, GameConf.reserved2);
		print_string("Save OK",COLOR_OK,COLOR_BG, 8+10*8,240-5 -10*3);
		flip_screen(actualScreen);
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
#ifdef _TINSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		print_string("Loading...", COLOR_OK, COLOR_BG, 8,240-5 -10*3);
		WsLoadState(szFile, GameConf.reserved1);
		print_string("Load OK",COLOR_OK,COLOR_BG, 8+10*8,240-5 -10*3);
		flip_screen(actualScreen);
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


void system_loadcfg(const char *cfg_name) 
{
  int fd;
  fd = open(cfg_name, O_RDONLY | O_BINARY);
  if (fd >= 0) 
  {
	read(fd, &GameConf, sizeof(GameConf));
    close(fd);
    /*To remove in the future...*/
#ifndef NOSAVE_HACK
	if (GameConf.save_oldoswan_check!=128) 
	{
		printf("Old Save format ! Let's remove it...\n");
		remove(cfg_name);
		GameConf.input_layout = 0;
		GameConf.save_oldoswan_check = 128;
	} 
#endif
#ifndef SWITCHING_GRAPHICS
	if (!GameConf.m_ScreenRatio) 
	{
		screen_prepback(actualScreen);
		flip_screen(actualScreen);
	}
#else
		flip_screen(actualScreen);
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
#ifndef OLDSAVE_HACK
		GameConf.save_oldoswan_check=128;
#endif
		getcwd(GameConf.current_dir_rom, MAX__PATH);
	}
}

void system_savecfg(const char *cfg_name) 
{
	int fd;
	fd = open(cfg_name, O_CREAT | O_RDWR | O_BINARY | O_TRUNC, S_IREAD | S_IWRITE);
	
	if (fd >= 0) 
	{
		write(fd, &GameConf, sizeof(GameConf)); 
		close(fd);
	}
}

