#include "shared.h"

typedef struct {
	const char itemName[16];
	short *itemPar;
	short itemParMaxValue;
	const char *itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int itemNum; /* number of items	*/
	int itemCur; /* current item	*/
	MENUITEM *m; /* array of items	*/
} MENU;


void clear_screen_menu(void);
void draw_bluerect_menu(unsigned char i);
void draw_bluerect_file(unsigned char i);

void menuReset(void);
void menuQuit(void);
void menuContinue(void);
void menuFileBrowse(void);
void menuSaveState(void);
void menuLoadState(void);
void screen_showkeymenu(void);
unsigned char ifactive(void);
#if !defined(NOSCREENSHOTS)
void menuSaveBmp(void);
#endif

const char mnuABXY[4][16] = {"Normal", "Wonderswan-like", "Swap DPAD,ABXY", "Swap ABXY,Stick"};
const char mnuYesNo[2][16] = {"No", "Yes"};
const char mnuSaves[10][16] = { "1","2","3","4","5","6","7","8","9"};
#if !defined(PSP)
const char mnuRatio[3][16] = { "1x size","Full screen", "Keep Aspect"};
#endif


#ifdef _TINSPIRE
const char *file_ext[] = { 
	(const char *) ".tns",
	NULL };
#else
const char *file_ext[] = { 
	(const char *) ".ws",  (const char *) ".wsc", 
#ifdef ZIP_SUPPORT  
	(const char *) ".zip",
#endif
	NULL };
#endif


