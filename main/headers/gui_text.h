#include "shared.h"

MENUITEM MainMenuItems[] = {
#ifndef NOROMLOADER
	{"Load ROM", NULL, 0, NULL, &menuFileBrowse},
#endif
	{"Continue", NULL, 0, NULL, &menuContinue},
	{"Reset", NULL, 0, NULL, &menuReset},
	{"Load State: ", (int16_t *) &GameConf.reserved1,  8, (int8_t *) &mnuSaves, &menuLoadState},
	{"Save State: ", (int16_t *) &GameConf.reserved2,  8, (char *) &mnuSaves, &menuSaveState},
	{"Show FPS: ", (int16_t *) &GameConf.m_DisplayFPS, 1, (int8_t *) &mnuYesNo, NULL},
	
	{"Quick Saves: ", (int16_t *) &GameConf.reserved3, 1, (int8_t *) &mnuYesNo, NULL},
	
#if !defined(NOSCREENSHOTS)
	{"Take Screenshot", NULL, 0, NULL, &menuSaveBmp},
#endif

#ifndef NATIVE_RESOLUTION
	{"Ratio: ", (int16_t *) &GameConf.m_ScreenRatio, 2, (int8_t *) &mnuRatio, NULL},
#endif

	{"", (int16_t *) &GameConf.input_layout, 		   4, (int8_t *) &mnuABXY, NULL},
	{"Exit", NULL, 0, NULL, &menuQuit}
};


MENU mnuMainMenu = { 
	11
	#if defined(NOROMLOADER)
	-1
	#endif
	#if defined(NOSCREENSHOTS)
	-1
	#endif
	#if defined(NATIVE_RESOLUTION)
	-1
	#endif
	,
	0, (MENUITEM *) &MainMenuItems };
