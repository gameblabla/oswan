#include <SDL/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "font_drawing.h"
#include "browser.h"
#include "drawing.h"
#include "shared.h"
#include "menu.h"

static uint32_t done_menu = 0;
uint32_t profile_config = 0;

struct hardcoded_keys keys_config[9];
struct Menu__ menu_oswan;

const char Main_Menu_Text[7][MAX_TEXT_SIZE] =
{
	"Continue",
	"Load State :",
	"Save State :",
	"Scaling :",
	"Input Options",
	"Exit"
}; 

const char Scaling_Text[3][MAX_TEXT_SIZE] =
{
	"Native",
	"Fullscreen",
	"Keep Aspect"
}; 

const char Controls_Text[12][MAX_TEXT_SIZE] =
{
	"Y1",    "Y2",
	"Y3",    "Y4",
	"X1",    "X2",
	"X3",    "X4",
	"OPTION", "START",
	"A", 	   "B"
}; 

const char Controls_Text_Nocenter[12][MAX_TEXT_SIZE] =
{
	{"Y1 (UP)   "},
	{"Y2 (RIGHT)"},
	{"Y3 (DOWN) "},
	{"Y4 (LEFT) "},
	{"X1 (UP)   "},
	{"X2 (RIGHT)"},
	{"X3 (DOWN) "},
	{"X4 (LEFT) "},
	{"OPTION    "},
	{"START     "},
	{"A         "},
	{"B         "}
}; 

/* Configuration files */

void load_config(void)
{
	uint32_t i, a;
	char home_path[PATH_MAX], cfg_path[PATH_MAX];
	FILE* fp;
	
	snprintf(home_path, sizeof(home_path), "%s/.oswanemu", getenv("HOME"));
	snprintf(cfg_path, sizeof(cfg_path), "%s/config.bin", home_path);
	
	if (access( home_path, F_OK ) == -1)
	{
		mkdir(home_path, 0755);
	}
	
	fp = fopen(cfg_path, "rb");
	if (fp)
	{
		fread(&menu_oswan, sizeof(uint8_t), sizeof(menu_oswan), fp);
		fread(&keys_config, sizeof(uint8_t), (14*4)*sizeof(uint32_t), fp);
		fclose(fp);
	}
	else
	{
		/* Set default settings */
		for (i=1;i<9;i++)
		{
			for (a=0;a<14;a++)
			{
				keys_config[i].buttons[a] = 0;
			}
		}
		
		/* Default profile */
		/* Should work for landscape. They can always configure it themselves should they need portrait mode. */
		keys_config[0].buttons[0] = 273;
		keys_config[0].buttons[1] = 275;
		keys_config[0].buttons[2] = 274;
		keys_config[0].buttons[3] = 276;
		keys_config[0].buttons[4] = 273;
		keys_config[0].buttons[5] = 275;
		keys_config[0].buttons[6] = 274;
		keys_config[0].buttons[7] = 276;
		keys_config[0].buttons[8] = 8;
		keys_config[0].buttons[9] = 13;
		keys_config[0].buttons[10] = 306;
		keys_config[0].buttons[11] = 308;
	}
}


void save_config(void)
{
	char home_path[PATH_MAX], cfg_path[PATH_MAX];
	FILE* fp;
	
	snprintf(home_path, sizeof(home_path), "%s/.oswan", getenv("HOME"));
	snprintf(cfg_path, sizeof(cfg_path), "%s/config.bin", home_path);
	
	mkdir(home_path, 0655);
	fp = fopen(cfg_path, "wb");
	if (fp)
	{
		fwrite(&menu_oswan, sizeof(uint8_t), sizeof(menu_oswan), fp);
		fwrite(&keys_config, sizeof(uint8_t), (14*9)*sizeof(uint32_t), fp);
		fclose(fp);
	}
}

/* Save states */

/* Save current state of game emulated	*/
void Save_State(void) 
{
    char szFile[PATH_MAX];
	
	if (cartridge_IsLoaded()) 
	{
		m_Flag = GF_GAMERUNNING;
		strcpy(szFile, gameName);
		WsSaveState(szFile, menu_oswan.menu_state);
		done_menu = 1;
	}
}

/* Load current state of game emulated	*/
void Load_State(void) 
{
    char szFile[PATH_MAX];
	
	if (cartridge_IsLoaded()) 
	{
		m_Flag = GF_GAMERUNNING;
		strcpy(szFile, gameName);
		WsLoadState(szFile, menu_oswan.menu_state);
		done_menu = 1;
	}
}

/* Menu code */

static void Set_Menu(uint32_t submenu)
{
	Clear_Screen_Norefresh();
	if (submenu == CONTROLS_MENU)
	{
		menu_oswan.Choose_Menu_value = 0;
		menu_oswan.maximum_menu = 12;
		menu_oswan.state_number = 0;
		if (!menu_oswan.scaling) menu_oswan.scaling = 0;
		menu_oswan.menu_state = 1;
		Draw_Rect_Menu(Y_MENU_CONTROLS, 15);
	}
	else if (submenu == EMULATOR_MAIN_MENU)
	{
		menu_oswan.Choose_Menu_value = 0;
		menu_oswan.maximum_menu = 6;
		menu_oswan.state_number = 0;
		if (!menu_oswan.scaling) menu_oswan.scaling = 0;
		menu_oswan.menu_state = 0;
		Draw_Rect_Menu(Y_MAIN_MENU - 22, 16);
	}
	else if (submenu == SETTINGS_KEY_SCREEN)
	{
		menu_oswan.menu_state = 2;
	}
	/* Useful for going back to Controls menu without resetting everything back to zero */
	else if (submenu == CONTROLS_MENU_NOSET)
	{
		menu_oswan.menu_state = 1;
	}
}

void AddItem(const char* text, uint32_t entry)
{
	uint32_t y = 24 + (entry * 24);
	uint32_t x = 8;
	
	print_string(text, TextWhite, 0, x, y, Surface_to_Draw_menu);
}

void AddItem_Alt(const char* text, uint32_t entry)
{
	uint32_t y = 0 + (entry * 15);
	uint32_t x = 8;
	
	print_string(text, TextWhite, 0, x, y, Surface_to_Draw_menu);
}

void print_text_center(const char* text, uint32_t y)
{
	uint32_t sizeofarray = strnlen(text, MAX_TEXT_SIZE);
	uint32_t x = (screen_scale.w_display - (sizeofarray * 8)) / 2;
	
	print_string(text, TextWhite, 0, x, y, Surface_to_Draw_menu);
}

static uint32_t sdl_controls_update_input(SDLKey k, int32_t p)
{
	switch(menu_oswan.menu_state)
	{
		case 0:
		switch(k)
		{
			case SDLK_UP:
				if (menu_oswan.Choose_Menu_value == 0) menu_oswan.Choose_Menu_value = menu_oswan.maximum_menu-1;
				else menu_oswan.Choose_Menu_value--;
			break;
			case SDLK_DOWN:
				menu_oswan.Choose_Menu_value++;
				if (menu_oswan.Choose_Menu_value > menu_oswan.maximum_menu-1) menu_oswan.Choose_Menu_value = 0;
			break;
			case SDLK_LEFT:
				if (menu_oswan.Choose_Menu_value == 1 || menu_oswan.Choose_Menu_value == 2)
				{
					if (menu_oswan.state_number == 0) menu_oswan.state_number = 9;
					else menu_oswan.state_number--;
				}
				else if (menu_oswan.Choose_Menu_value == 3)
				{
					if (menu_oswan.scaling == 0) menu_oswan.scaling = 2;
					else menu_oswan.scaling--;
				}
			break;
			case SDLK_RIGHT:
				if (menu_oswan.Choose_Menu_value == 1 || menu_oswan.Choose_Menu_value == 2)
				{
					menu_oswan.state_number++;
					if (menu_oswan.state_number > 9) menu_oswan.state_number = 0;
				}
				else if (menu_oswan.Choose_Menu_value == 3)
				{
					menu_oswan.scaling++;
					if (menu_oswan.scaling > 2) menu_oswan.scaling = 0;
				}
			break;
			case SDLK_LCTRL:
			case SDLK_RETURN:
				if (menu_oswan.Choose_Menu_value == 4) Set_Menu(CONTROLS_MENU);
				else if (menu_oswan.Choose_Menu_value == 5)
				{
					done_menu = 1;
					m_Flag = GF_GAMEQUIT;
				}
				else if (menu_oswan.Choose_Menu_value == 0)
				{
					done_menu = 1;
					m_Flag = GF_GAMERUNNING;
				}
				else if (menu_oswan.Choose_Menu_value == 1)
				{
					Load_State();
				}
				else if (menu_oswan.Choose_Menu_value == 2)
				{
					Save_State();
				}
			break;
		}
		break;
		case 1:
		switch(k)
		{	
			case SDLK_UP:
				if (menu_oswan.Choose_Menu_value == 0) menu_oswan.Choose_Menu_value = menu_oswan.maximum_menu-1;
				else menu_oswan.Choose_Menu_value--;
			break;
			case SDLK_DOWN:
				menu_oswan.Choose_Menu_value++;
				if (menu_oswan.Choose_Menu_value > menu_oswan.maximum_menu-1) menu_oswan.Choose_Menu_value = 0;
			break;
			case SDLK_TAB:
			case SDLK_LEFT:
				if (profile_config > 0) profile_config--;
			break;
			case SDLK_BACKSPACE:
			case SDLK_RIGHT:
				if (profile_config < 8) profile_config++;
			break;
			case SDLK_LCTRL:
			case SDLK_RETURN:
				Set_Menu(SETTINGS_KEY_SCREEN);
			break;
			case SDLK_LALT:
				Set_Menu(EMULATOR_MAIN_MENU);
			break;
		}
		break;
		case 2:
			keys_config[profile_config].buttons[menu_oswan.Choose_Menu_value] = k;
			Set_Menu(CONTROLS_MENU_NOSET);
		break;
	}
	return 0;
}

static void Controls_Menu()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				sdl_controls_update_input(event.key.keysym.sym, 0);
			break;
		}
	}
}

/* Main Menu */

void Menu()
{
	char text[MAX_TEXT_SIZE];
	uint32_t i;
	
	Set_Menu(0);
	
	done_menu = 0;
	
	while(!done_menu)
	{
		Clear_Menu();
		Controls_Menu();

		switch(menu_oswan.menu_state)
		{
			case 0:
			
			Draw_Rect_Menu(Y_MAIN_MENU - 22, 16);
			
			print_text_center("Gameblabla's Oswan", 6);
			
			for (i=0;i<menu_oswan.maximum_menu;i++)
			{
				/* Load/Save States */
				if (i == 1 || i == 2)
				{
					snprintf(text, sizeof(text), "%s %u", Main_Menu_Text[i], menu_oswan.state_number);
					AddItem(text, i);
				}
				/* Scaling */
				else if (i == 3)
				{
					snprintf(text, sizeof(text), "%s %s", Main_Menu_Text[i], Scaling_Text[menu_oswan.scaling]);
					AddItem(text, i);
				}
				else
				{
					AddItem(Main_Menu_Text[i], i);
				}
			}
			break;
			case 1:
				if (menu_oswan.Choose_Menu_value == 11)
				{
					Draw_Rect_Menu(0, 12);
					snprintf(text, sizeof(text), "%s : %u", Controls_Text_Nocenter[11], keys_config[profile_config].buttons[11]);
					AddItem_Alt(text, 0);
				}
				else
				{
					Draw_Rect_Menu((menu_oswan.Choose_Menu_value * 15), 12);
					for (i=0;i<menu_oswan.maximum_menu;i++)
					{
						snprintf(text, sizeof(text), "%s : %u", Controls_Text_Nocenter[i], keys_config[profile_config].buttons[i]);
						AddItem_Alt(text, i);
					}
				}
				
			break;
			case 2:
				snprintf(text, sizeof(text), "Press a key for %s", Controls_Text[menu_oswan.Choose_Menu_value]);
				print_text_center(text, 128);
			break;
		}

		Update_Screen();
	}
	
	/* Clear the screen before going back to Game or exiting the emulator */
	Clear_Screen();
}
