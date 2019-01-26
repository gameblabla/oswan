#include <string.h>

#include "game_input.h"
#include "drawing.h"
#include "shared.h"

#include "WSFileio.h"

#ifdef JOYSTICK
	extern int16_t joystick_axies[4];
	#define JOYSTICK_UP (joystick_axies[1] < -2048 ? 1 : 0)
	#define JOYSTICK_RIGHT	(joystick_axies[0] > 2048 ? 1 : 0)
	#define JOYSTICK_LEFT	(joystick_axies[0] < -2048 ? 1 : 0)
	#define JOYSTICK_DOWN (joystick_axies[1] > 2048 ? 1 : 0)
#endif

void exit_button(void)
{
	/* SLIDER/SELECT or ESC -> MENU UI */
	if (button_state[12] || button_state[13] || (button_state[10] && button_state[11]))
	{
		m_Flag = GF_MAINUI;
		take_screenshot();
		/* HACK - FIX ME*/
#ifdef GCW
		if (GameConf.m_ScreenRatio == 2) SetVideo(0);
#endif
	}
}

int32_t WsInputGetState(int32_t mode)
{
	/*
	 * 0 = Up  (Y1)
	 * 1 = Right (Y1)
	 * 2 = Down (Y1)
	 * 3 = Left (Y1)
	 * 4 = Up  (X1)
	 * 5 = Right (X1)
	 * 6 = Down (X1)
	 * 7 = Left (X1)
	 * 8 = START
	 * 9 = OPTION
	 * 10 = A 
	 * 11 = B
	*/
	int8_t szFile[256];
	int32_t button = 0;
	
	Buttons();
	button = Fire_buttons();

	/* If Quick Saves are enabled */
	if (GameConf.reserved3)
	{
		/* Save (L button)	*/
		if (button_state[8]) 
		{
			strcpy(szFile, gameName);
			#ifdef _TINSPIRE
			strcpy(strrchr(szFile, '.'), ".sta.tns");
			#else
			strcpy(strrchr(szFile, '.'), ".sta");
			#endif
			WsSaveState(szFile, GameConf.reserved1);
		}
		/* Load (R button)	*/
		else if (button_state[9]) 
		{
			strcpy(szFile, gameName);
			#ifdef _TINSPIRE
			strcpy(strrchr(szFile, '.'), ".sta.tns");
			#else
			strcpy(strrchr(szFile, '.'), ".sta");
			#endif
			WsLoadState(szFile, GameConf.reserved2);
		}
	}
	
	switch(GameConf.input_layout)
	{
		case 0:
			/* RIGHT -> X2 */
			button |= button_state[15] ? (1<<1) : 0; 
			/* LEFT -> X4 */
			button |= button_state[14] ? (1<<3) : 0; 
			/* DOWN -> X3 */
			button |= button_state[17] ? (1<<2) : 0; 
			/* UP -> X1	*/
			button |= button_state[16] ? (1<<0) : 0; 
			
			/* Button A	*/
			button |= button_state[4] ? (1<<10) : 0; 
			/* Button B	*/
			button |= button_state[5] ? (1<<11) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[1] ? (1<<5) : 0; 	
			/* LEFT -> X1	*/
			button |= button_state[0] ? (1<<7) : 0; 	
			/* DOWN -> X1	*/
			button |= button_state[3] ? (1<<6) : 0; 	
			/* UP -> X1		*/
			button |= button_state[2] ? (1<<4) : 0; 	
		break;
		case 1:
			/* RIGHT -> X2 */
			button |= button_state[15] ? (1<<5) : 0; 
			/* LEFT -> X4 */
			button |= button_state[14] ? (1<<7) : 0; 
			/* DOWN -> X3 */
			button |= button_state[17] ? (1<<6) : 0; 
			/* UP -> X1	*/
			button |= button_state[16] ? (1<<4) : 0; 
			
			/* Button A	*/
			button |= button_state[4] ? (1<<10) : 0; 
			/* Button B	*/
			button |= button_state[5] ? (1<<11) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[1] ? (1<<1) : 0; 
			/* LEFT -> X1	*/
			button |= button_state[0] ? (1<<3) : 0; 
			/* DOWN -> X1	*/
			button |= button_state[3] ? (1<<2) : 0; 	
			/* UP -> X1		*/
			button |= button_state[2] ? (1<<0) : 0; 
		break;
		case 2:
			button |= button_state[15] ? (1<<10) : 0; 
			button |= button_state[17] ? (1<<11) : 0; 

			button |= button_state[4] ? (1<<5) : 0; 
			button |= button_state[5] ? (1<<6) : 0; 

			button |= button_state[6] ? (1<<7) : 0; 
			button |= button_state[7] ? (1<<4) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[1] ? (1<<1) : 0; 
			/* LEFT -> X1	*/
			button |= button_state[0] ? (1<<3) : 0; 
			/* DOWN -> X1	*/
			button |= button_state[3] ? (1<<2) : 0; 	
			/* UP -> X1		*/
			button |= button_state[2] ? (1<<0) : 0; 
		break;
		case 3:
			button |= button_state[15] ? (1<<5) : 0; 
			button |= button_state[14] ? (1<<7) : 0; 
			button |= button_state[17] ? (1<<6) : 0; 
			button |= button_state[16] ? (1<<4) : 0; 
			
			button |= button_state[4] ? (1<<1) : 0; 
			button |= button_state[5] ? (1<<2) : 0; 
			button |= button_state[6] ? (1<<3) : 0; 
			button |= button_state[7] ? (1<<0) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[1] ? (1<<10) : 0;  
			/* DOWN -> X1	*/
			button |= button_state[3] ? (1<<11) : 0; 	
		break;
		case 4:
			button |= button_state[15] ? (1<<1) : 0; 
			button |= button_state[14] ? (1<<3) : 0; 
			button |= button_state[17] ? (1<<2) : 0; 
			button |= button_state[16] ? (1<<0) : 0; 
			
			button |= button_state[4] ? (1<<5) : 0; 
			button |= button_state[5] ? (1<<6) : 0; 
			button |= button_state[6] ? (1<<7) : 0; 
			button |= button_state[7] ? (1<<4) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[1] ? (1<<10) : 0; 
			/* DOWN -> X1	*/
			button |= button_state[3] ? (1<<11) : 0; 	
		break;
		case 5:
			/* RIGHT -> X2 */
			button |= button_state[15] ? (1<<5) : 0; 
			/* LEFT -> X4 */
			button |= button_state[14] ? (1<<7) : 0; 
			/* DOWN -> X3 */
			button |= button_state[17] ? (1<<6) : 0; 
			/* UP -> X1	*/
			button |= button_state[16] ? (1<<4) : 0; 
			
			/* Button A	*/
			button |= button_state[4] ? (1<<5) : 0; 
			/* Button B	*/
			button |= button_state[5] ? (1<<7) : 0; 
			/* Button X	*/
			button |= button_state[6] ? (1<<6) : 0; 
			/* Button Y	*/
			button |= button_state[7] ? (1<<4) : 0; 
			
			/* RIGHT -> X1	*/
			button |= button_state[2] ? (1<<1) : 0; 
			/* LEFT -> X1	*/
			button |= button_state[3] ? (1<<3) : 0; 
			/* DOWN -> X1	*/
			button |= button_state[1] ? (1<<2) : 0; 	
			/* UP -> X1		*/
			button |= button_state[0] ? (1<<0) : 0; 
		break;
	}
			
	/* START BUTTON */		
	button |= button_state[10] ? (1<<9) : 0; 		
			
	return button;
}

int32_t Fire_buttons(void)
{
	static uint8_t x_button = 0, y_button = 0;
	int32_t button = 0;
	
	switch (GameConf.input_layout)
	{
		case 0:
		case 1:
			if (button_state[6]>0) 
				x_button++;			/* (Rapid Fire A) */
			else
				x_button = 0;
				
			if (button_state[7]>0) 
				y_button++;			/* (Rapid Fire B) */
			else
				y_button = 0;
		break;
		case 2:
			if (button_state[14]>0) 
				x_button++;			/* (Rapid Fire A) */
			else
				x_button = 0;
				
			if (button_state[16]>0) 
				y_button++;			/* (Rapid Fire B) */
			else
				y_button = 0;
		break;
		case 3:
		case 4:
			#ifdef JOYSTICK
				if (JOYSTICK_LEFT) 
					x_button++;   	/* (Rapid Fire A) */
				else
					x_button = 0;	
						
				if (JOYSTICK_UP) 
					y_button++;		/* (Rapid Fire B) */
				else
					y_button = 0;
			#endif
		break;
	}
	
	if (x_button == 1)
	{
		button |= (1<<11);
	}
	else if (x_button>1)
	{
		button |= (0<<11);
		x_button = 0;
	}
	
	if (y_button == 1)
	{
		button |= (1<<10);
	}
	else if (y_button>1)
	{
		button |= (0<<10);
		y_button = 0;
	}
		
	return button;
}
