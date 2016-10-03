#include <string.h>

#include "game_input.h"
#include "drawing.h"
#include "shared.h"

#include "WSFileio.h"

#ifdef JOYSTICK
	SDL_Joystick* joy;
	short x_joy = 0, y_joy = 0;
	#define JOYSTICK_UP y_joy < -7500
	#define JOYSTICK_LEFT	x_joy < -7500
	#define JOYSTICK_DOWN y_joy > 7500
	#define JOYSTICK_RIGHT	x_joy > 7500
#endif

void exit_button(void)
{
	 /* SLIDER/SELECT or ESC -> MENU UI */
	if (button_state[12] || button_state[13])
	{
		m_Flag = GF_MAINUI;
		take_screenshot();
		/* HACK - FIX ME*/
#ifdef GCW
		if (GameConf.m_ScreenRatio == 2) SetVideo(0);
#endif
	}
}

int WsInputGetState(int mode)
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
	char szFile[256];
	int button = 0;
	
	Buttons();
	button = Fire_buttons();

	#ifdef JOYSTICK
		if (SDL_NumJoysticks() > 0)
			joy = SDL_JoystickOpen(0);
		
		x_joy = SDL_JoystickGetAxis(joy, 0);
		y_joy = SDL_JoystickGetAxis(joy, 1);
			
		SDL_JoystickUpdate();
	#endif

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
			
			#ifdef JOYSTICK
			if (JOYSTICK_RIGHT) 
				button |= (1<<1); 	/* RIGHT -> Y1 */
			else if (JOYSTICK_LEFT) 
				button |= (1<<3); 	/* LEFT -> Y1 */
					
			if (JOYSTICK_DOWN) 
				button |= (1<<2); 	/* DOWN -> Y1 */
			else if (JOYSTICK_UP) 
				button |= (1<<0); 	/*UP -> Y1 */
			#endif
		break;
		case 1:
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
			
			#ifdef JOYSTICK
			if (JOYSTICK_RIGHT) 
				button |= (1<<5); 	/* RIGHT -> X1	*/
			else if (JOYSTICK_LEFT) 
				button |= (1<<7); 	/* LEFT -> X1	*/
					
			if (JOYSTICK_DOWN) 
				button |= (1<<6); 	/* DOWN -> X1	*/
			else if (JOYSTICK_UP) 
				button |= (1<<4); 	/* UP -> X1		*/
			#endif
		break;
		case 2:
			button |= button_state[15] ? (1<<10) : 0; 
			button |= button_state[17] ? (1<<11) : 0; 

			button |= button_state[4] ? (1<<5) : 0; 
			button |= button_state[5] ? (1<<6) : 0; 

			button |= button_state[6] ? (1<<7) : 0; 
			button |= button_state[7] ? (1<<4) : 0; 
			
			#ifdef JOYSTICK
			if (JOYSTICK_RIGHT) 
				button |= (1<<1); /* RIGHT -> Y1*/
			else if (JOYSTICK_LEFT) 
				button |= (1<<3); /* LEFT -> Y1	*/
					
			if (JOYSTICK_DOWN) 
				button |= (1<<2); /* DOWN -> Y1	*/
			else if (JOYSTICK_UP) 
				button |= (1<<0); /* UP -> Y1	*/
			#endif
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
			
			#ifdef JOYSTICK
			if (JOYSTICK_RIGHT) 
				button |= (1<<10);
			if (JOYSTICK_DOWN) 
				button |= (1<<11); 
			#endif
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
			
			#ifdef JOYSTICK
			if (JOYSTICK_RIGHT) 
				button |= (1<<10);
			if (JOYSTICK_DOWN) 
				button |= (1<<11);
			#endif
		break;
	}
			
	/* START BUTTON */		
	button |= button_state[10] ? (1<<9) : 0; 		
			
	return button;
}

int Fire_buttons(void)
{
	static unsigned char x_button = 0, y_button = 0;
	int button = 0;
	
	switch (GameConf.input_layout)
	{
		case 0:
		case 1:
			if (button_state[6]>0) 
				x_button++;			/*(Rapid Fire A)*/
			else
				x_button = 0;
				
			if (button_state[7]>0) 
				y_button++;			/*(Rapid Fire B)*/
			else
				y_button = 0;
		break;
		case 2:
			if (button_state[14]>0) 
				x_button++;			/*(Rapid Fire A)*/
			else
				x_button = 0;
				
			if (button_state[16]>0) 
				y_button++;			/*(Rapid Fire B)*/
			else
				y_button = 0;
		break;
		case 3:
		case 4:
			#ifdef JOYSTICK
				if (JOYSTICK_LEFT) 
					x_button++;   	/*(Rapid Fire A)*/
				else
					x_button = 0;	
						
				if (JOYSTICK_UP) 
					y_button++;		/*(Rapid Fire B)*/
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
