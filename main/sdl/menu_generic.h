#include "shared.h"

MENUITEM MainMenuItems[] = {
	{"Load ROM", NULL, 0, NULL, &menuFileBrowse},
	{"Continue", NULL, 0, NULL, &menuContinue},
	{"Reset", NULL, 0, NULL, &menuReset},
	{"Load State: ", (short *) &GameConf.reserved1,  8, (char *) &mnuSaves, &menuLoadState},
	{"Save State: ", (short *) &GameConf.reserved2,  8, (char *) &mnuSaves, &menuSaveState},
	{"Show FPS: ", (short *) &GameConf.m_DisplayFPS, 1, (char *) &mnuYesNo, NULL},
	
	{"Quick Saves: ", (short *) &GameConf.reserved3, 1, (char *) &mnuYesNo, NULL},
	
#if !defined(NOSCREENSHOTS)
	{"Take Screenshot", NULL, 0, NULL, &menuSaveBmp},
#endif
	{"Ratio: ", (short *) &GameConf.m_ScreenRatio, 2, (char *) &mnuRatio, NULL},
	{"", (short *) &GameConf.input_layout, 		   3, (char *) &mnuABXY, NULL},
	{"Exit", NULL, 0, NULL, &menuQuit}
};


MENU mnuMainMenu = { 
#if defined(NOSCREENSHOTS)
	10,
#else
	11,
#endif
	0, (MENUITEM *) &MainMenuItems };
