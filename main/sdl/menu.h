#include "shared.h"

typedef struct {
	const int8_t itemName[16];
	int16_t *itemPar;
	int16_t itemParMaxValue;
	const int8_t *itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int32_t itemNum; /* number of items	*/
	int32_t itemCur; /* current item	*/
	MENUITEM *m; /* array of items	*/
} MENU;


void clear_screen_menu(void);
void draw_bluerect_menu(uint8_t i);
void draw_bluerect_file(uint8_t i);

void menuReset(void);
void menuQuit(void);
void menuContinue(void);
void menuFileBrowse(void);
void menuSaveState(void);
void menuLoadState(void);
void screen_showkeymenu(void);
uint8_t ifactive(void);
#if !defined(NOSCREENSHOTS)
void menuSaveBmp(void);
#endif

const int8_t mnuABXY[5][16] = {"Normal", "Swap Dpad-Stick", "Swap DPAD-ABXY", "Swap ABXY-Stick", "Swap ALL"};
const int8_t mnuYesNo[2][16] = {"No", "Yes"};
const int8_t mnuSaves[10][16] = { "1","2","3","4","5","6","7","8","9"};
const int8_t mnuRatio[3][16] = { "1x size","Full screen", "Keep Aspect"};


#ifdef _TINSPIRE
const int8_t *file_ext[] = { 
	(const int8_t *) ".tns",
	NULL };
#else
const int8_t *file_ext[] = { 
	(const int8_t *) ".ws",  (const int8_t *) ".wsc", 
#ifdef ZIP_SUPPORT  
	(const int8_t *) ".zip",
#endif
	NULL };
#endif


