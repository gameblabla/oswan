#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#define RGB565(r,g,b) ((r << 8) | (g << 3) | (b >> 3))

#define Y_MAIN_MENU \
	(  (48 + (menu_oswan.Choose_Menu_value * 24)) - 4)
	
#define Y_MENU_CONTROLS \
	( (48 + (menu_oswan.Choose_Menu_value * 16)) - 4)

#define MAX_TEXT_SIZE 48

#define EMULATOR_MAIN_MENU 0
#define CONTROLS_MENU 1
#define SETTINGS_KEY_SCREEN 2
#define CONTROLS_MENU_NOSET 3

struct Menu__
{
	uint32_t Choose_Menu_value;
	uint32_t maximum_menu;
	uint32_t state_number;
	uint32_t scaling;
	uint32_t menu_state;
};

struct hardcoded_keys
{
	uint32_t buttons[14];
};

extern uint32_t profile_config;
extern struct hardcoded_keys keys_config[2];
extern struct Menu__ menu_oswan;

extern void Menu(void);
extern void load_config(void);
extern void save_config(void);

#endif
