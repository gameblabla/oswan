#include <SDL/SDL.h>
#include <string.h>

#include "input.h"
#include "drawing.h"
#include "shared.h"

#include "WSFileio.h"

extern SDL_Event event;
#ifdef JOYSTICK
	SDL_Joystick* joy;
	short x_joy = 0, y_joy = 0;
#endif

int WsInputGetState(int mode)
{
	char szFile[512];
	static unsigned char button_state[2], button_time[2];
	unsigned char i;
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
	int button = 0;
	int pad;
	
#ifndef PSP
	SDL_PollEvent(&event);
	unsigned char *keys = SDL_GetKeyState(NULL);
	button = Fire_buttons(keys);
#endif

	#ifdef JOYSTICK
	if (SDL_NumJoysticks() > 0)
		joy = SDL_JoystickOpen(0);
		
		x_joy = SDL_JoystickGetAxis(joy, 0);
		y_joy = SDL_JoystickGetAxis(joy, 1);
			
		SDL_JoystickUpdate();
	#endif
		
	for(i=0;i<2;i++)
	{	
		if (i==0) pad = PAD_L;
		else if (i==1) pad = PAD_R;
		
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

	
	// Save (L button)
	if (button_time[0] == 1) 
	{
		strcpy(szFile, gameName);
		printf("szFile = %s \n", szFile);
		WsLoadState(szFile, GameConf.reserved2);
	}
		
	// Load (R button)
	if (button_time[1] == 1) 
	{
		strcpy(szFile, gameName);
		printf("szFile = %s \n", szFile);
		WsSaveState(szFile, GameConf.reserved1);
	}

	if (GameConf.input_layout == 0)
	{
			if (keys[PAD_XRIGHT] == SDL_PRESSED) 
				button |= (1<<5); // RIGHT -> X2
			if (keys[PAD_XLEFT] == SDL_PRESSED)  
				button |= (1<<7); // LEFT -> X4
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= (1<<6); // DOWN -> X3
			if (keys[PAD_XUP] == SDL_PRESSED)    
				button |= (1<<4); // UP -> X1
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= (1<<10);  // Button A
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= (1<<11);  // Button B
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<1); // RIGHT -> Y1
			else if (x_joy < -7500) 
				button |= (1<<3); // LEFT -> Y1
					
			if (y_joy > 7500) 
				button |= (1<<2); // DOWN -> Y1
			else if (y_joy < -7500) 
				button |= (1<<0); // UP -> Y1
			#endif
	}
	else if (GameConf.input_layout == 1)
	{
			if (keys[PAD_XRIGHT] == SDL_PRESSED) 
				button |= (1<<1); // RIGHT -> Y2
			if (keys[PAD_XLEFT] == SDL_PRESSED)  
				button |= (1<<3); // LEFT -> Y4
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= (1<<2); // DOWN -> Y3
			if (keys[PAD_XUP] == SDL_PRESSED)    
				button |= (1<<0); // UP -> Y1
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= (1<<10);  // Button A
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= (1<<11);  // Button B
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<5); // RIGHT -> X1
			else if (x_joy < -7500) 
				button |= (1<<7); // LEFT -> X1
					
			if (y_joy > 7500) 
				button |= (1<<6); // DOWN -> X1
			else if (y_joy < -7500) 
				button |= (1<<4); // UP -> X1
			#endif
	}
	else if (GameConf.input_layout == 2)
	{
			if (keys[PAD_XRIGHT] == SDL_PRESSED) 
				button |= (1<<10);
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= (1<<11); 
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= (1<<6); // RIGHT -> X2
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= (1<<5); // LEFT -> X4
			if (keys[PAD_X] == SDL_PRESSED) 
				button |= (1<<7); // DOWN -> X3
			if (keys[PAD_Y] == SDL_PRESSED) 
				button |= (1<<4); // UP -> X1
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<1); // RIGHT -> Y1
			else if (x_joy < -7500) 
				button |= (1<<3); // LEFT -> Y1
					
			if (y_joy > 7500) 
				button |= (1<<2); // DOWN -> Y1
			else if (y_joy < -7500) 
				button |= (1<<0); // UP -> Y1
			#endif
	}
	else if (GameConf.input_layout == 3)
	{
			if (keys[PAD_XRIGHT] == SDL_PRESSED) 
				button |= (1<<5); // RIGHT -> X2
			if (keys[PAD_XLEFT] == SDL_PRESSED)  
				button |= (1<<7); // LEFT -> X4
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= (1<<6); // DOWN -> X3
			if (keys[PAD_XUP] == SDL_PRESSED)    
				button |= (1<<4); // UP -> X1
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= (1<<1); // DOWN -> Y1
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= (1<<2); // RIGHT -> Y1
			if (keys[PAD_X] == SDL_PRESSED) 
				button |= (1<<3); // LEFT -> Y1
			if (keys[PAD_Y] == SDL_PRESSED) 
				button |= (1<<0); // UP -> Y1
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<10);
			if (y_joy > 7500) 
				button |= (1<<11); 
			#endif
	}
			
	if (keys[PAD_QUIT] == SDL_PRESSED) 
	{
		take_screenshot();
		m_Flag = GF_MAINUI;
	}
	
#if defined(GCW)
	if (keys[PAD_SLIDER] == SDL_PRESSED) 
	{
		take_screenshot();
		m_Flag = GF_MAINUI;
	}
#endif
			
	if (keys[PAD_START] == SDL_PRESSED)  // START -> START 	
		button |= (1<<9); 
			
	return button;
}

int Fire_buttons(unsigned char * keys)
{
	static unsigned char x_button = 0, y_button = 0;
	int button = 0;
	
	if (GameConf.input_layout < 2)
	{
		if (keys[PAD_X]) 
			x_button++;		//(Rapid Fire A)
		else
			x_button = 0;
			
		if (keys[PAD_Y]) 
			y_button++;		//(Rapid Fire B)
		else
			y_button = 0;
	}	
	else if (GameConf.input_layout == 2)
	{
		if (keys[PAD_XLEFT]) 
			x_button++;		//(Rapid Fire A)
		else
			x_button = 0;
			
		if (keys[PAD_XUP]) 
			y_button++;		//(Rapid Fire B)
		else
			y_button = 0;
	}
	else if (GameConf.input_layout == 3)
	{
#ifdef JOYSTICK
		if (x_joy < -7500) 
			x_button++;   	//(Rapid Fire A)
		else
			x_button = 0;	
			
		if (y_joy < -7500) 
			y_button++;		//(Rapid Fire B)
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
