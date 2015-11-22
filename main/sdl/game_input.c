#include <string.h>

#include "game_input.h"
#include "drawing.h"
#include "shared.h"

#include "WSFileio.h"

#define JOYSTICK_UP y_joy < -7500
#define JOYSTICK_LEFT	x_joy < -7500
#define JOYSTICK_DOWN y_joy > 7500
#define JOYSTICK_RIGHT	x_joy > 7500


#ifdef JOYSTICK
	#ifdef GECKO
		struct expansion_t data;
	#endif
	SDL_Joystick* joy;
	short x_joy = 0, y_joy = 0;
#endif

inline void exit_button(void)
{
	 /* SLIDER/SELECT or ESC -> MENU UI*/
	if (button_state[12] || button_state[13])
	{
		take_screenshot();
		m_Flag = GF_MAINUI;
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
	char szFile[512];
	int button = 0;
	
	Buttons();
	button = Fire_buttons();

	#ifdef JOYSTICK
		#ifdef GECKO
			struct expansion_t data;
			WPAD_Expansion(WPAD_CHAN_0, &data);
			if (data.type == WPAD_EXP_NUNCHUK)
			{ 
				//Nunchuck Up
				if((data.nunchuk.js.ang>=315 || data.nunchuk.js.ang<=45) && data.nunchuk.js.mag>=0.9)
				{
					y_joy = -8000;
				}
				//Nunchuck Down
				else if((data.nunchuk.js.ang>=180-45 && data.nunchuk.js.ang<=180+45) && data.nunchuk.js.mag>=0.9)
				{
					y_joy = 8000;
				}
				//Nunchuck Left
				if((data.nunchuk.js.ang>=270-45 && data.nunchuk.js.ang<=270+45) && data.nunchuk.js.mag>=0.9)
				{
					x_joy = -8000;
				}
				// Nunchuck Right
				else if((data.nunchuk.js.ang>=90-45 && data.nunchuk.js.ang<=90+45) && data.nunchuk.js.mag>=0.9)
				{
					x_joy = 8000;
				}
			}
		#else
		if (SDL_NumJoysticks() > 0)
			joy = SDL_JoystickOpen(0);
		
		x_joy = SDL_JoystickGetAxis(joy, 0);
		y_joy = SDL_JoystickGetAxis(joy, 1);
			
		SDL_JoystickUpdate();
		#endif
	#endif

	/* If Quick Saves are enabled */
	if (GameConf.reserved3)
	{
		/* Save (L button)	*/
		if (button_state[8] == 1) 
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
		else if (button_state[9] == 1) 
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
	

	if (GameConf.input_layout == 0)
	{
			if (button_state[15]>0) 
				button |= (1<<5); 	/* RIGHT -> X2	*/
			if (button_state[14]>0)  
				button |= (1<<7); 	/* LEFT -> X4	*/
			if (button_state[17]>0)  
				button |= (1<<6); 	/* DOWN -> X3	*/
			if (button_state[16]>0)    
				button |= (1<<4); 	/* UP -> X1		*/
				
			if (button_state[4]>0) 
				button |= (1<<10);  /* Button A	*/
			if (button_state[5]>0) 
				button |= (1<<11);  /* Button B	*/
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<1); 	/* RIGHT -> Y1 */
			else if (x_joy < -7500) 
				button |= (1<<3); 	/* LEFT -> Y1 */
					
			if (y_joy > 7500) 
				button |= (1<<2); 	/* DOWN -> Y1 */
			else if (y_joy < -7500) 
				button |= (1<<0); 	/*UP -> Y1 */
			#endif
	}
	else if (GameConf.input_layout == 1)
	{
			if (button_state[15]>0) 
				button |= (1<<1); 	/* RIGHT -> Y2 */
			if (button_state[14]>0)  
				button |= (1<<3); 	/* LEFT -> Y4 */
			if (button_state[17]>0)  
				button |= (1<<2); 	/* DOWN -> Y3 */
			if (button_state[16]>0)    
				button |= (1<<0); 	/* UP -> Y1	*/
				
			if (button_state[4]>0) 
				button |= (1<<10);  /* Button A */
			if (button_state[5]>0) 
				button |= (1<<11);  /* Button B */
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<5); 	/* RIGHT -> X1	*/
			else if (x_joy < -7500) 
				button |= (1<<7); 	/* LEFT -> X1	*/
					
			if (y_joy > 7500) 
				button |= (1<<6); 	/* DOWN -> X1	*/
			else if (y_joy < -7500) 
				button |= (1<<4); 	/* UP -> X1		*/
			#endif
	}
	else if (GameConf.input_layout == 2)
	{
			if (button_state[15]>0) 
				button |= (1<<10);
			if (button_state[17]>0)  
				button |= (1<<11); 
				
			if (button_state[4]>0) 
				button |= (1<<5); 
			if (button_state[5]>0) 
				button |= (1<<6); 
			if (button_state[6]>0) 
				button |= (1<<7); 
			if (button_state[7]>0) 
				button |= (1<<4); 
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<1); /* RIGHT -> Y1*/
			else if (x_joy < -7500) 
				button |= (1<<3); /* LEFT -> Y1	*/
					
			if (y_joy > 7500) 
				button |= (1<<2); /* DOWN -> Y1	*/
			else if (y_joy < -7500) 
				button |= (1<<0); /* UP -> Y1	*/
			#endif
	}
	else if (GameConf.input_layout == 3)
	{
			if (button_state[15]>0) 
				button |= (1<<5); /* RIGHT -> X2	*/
			if (button_state[14]>0)  
				button |= (1<<7); /* LEFT -> X4		*/
			if (button_state[17]>0)  
				button |= (1<<6); /* DOWN -> X3		*/
			if (button_state[16]>0)    
				button |= (1<<4); /* UP -> X1		*/
				
			if (button_state[4]>0) 
				button |= (1<<1); /* DOWN -> Y1	*/
			if (button_state[5]>0) 
				button |= (1<<2); /* RIGHT -> Y1*/
			if (button_state[6]>0) 
				button |= (1<<3); /* LEFT -> Y1	*/
			if (button_state[7]>0) 
				button |= (1<<0); /* UP -> Y1	*/
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<10);
			if (y_joy > 7500) 
				button |= (1<<11); 
			#endif
	}
	else if (GameConf.input_layout == 4)
	{
			if (button_state[15]>0) 
				button |= (1<<1); 	/* RIGHT -> X2	*/
			if (button_state[14]>0)  
				button |= (1<<3); 	/* LEFT -> X4	*/
			if (button_state[17]>0)  
				button |= (1<<2); 	/* DOWN -> X3	*/
			if (button_state[16]>0)    
				button |= (1<<0); 	/* UP -> X1		*/
				
			if (button_state[4]>0) 
				button |= (1<<5);  /* Button A - Right	*/
			if (button_state[5]>0) 
				button |= (1<<6);  /* Button B - Down	*/
				
			if (button_state[6]>0) 
				button |= (1<<7);  /* Button X - Left	*/
			if (button_state[7]>0) 
				button |= (1<<4);  /* Button Y - Up	*/
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<10);
			if (y_joy > 7500) 
				button |= (1<<11);
			#endif
	}
			
			
	if (button_state[10])  /* START -> START */
		button |= (1<<9); 
			
	return button;
}

int Fire_buttons(void)
{
	static unsigned char x_button = 0, y_button = 0;
	int button = 0;
	
	if (GameConf.input_layout < 2)
	{
		if (button_state[6]>0) 
			x_button++;		/*(Rapid Fire A)*/
		else
			x_button = 0;
			
		if (button_state[7]>0) 
			y_button++;		/*(Rapid Fire B)*/
		else
			y_button = 0;
	}	
	else if (GameConf.input_layout == 2)
	{
		if (button_state[14]>0) 
			x_button++;		/*(Rapid Fire A)*/
		else
			x_button = 0;
			
		if (button_state[16]>0) 
			y_button++;		/*(Rapid Fire B)*/
		else
			y_button = 0;
	}
	else if (GameConf.input_layout == 3 || GameConf.input_layout == 4)
	{
#ifdef JOYSTICK
		if (x_joy < -7500) 
			x_button++;   	/*(Rapid Fire A)*/
		else
			x_button = 0;	
			
		if (y_joy < -7500) 
			y_button++;		/*(Rapid Fire B)*/
		else
			y_button = 0;
#endif
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
