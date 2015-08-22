#include <SDL/SDL.h>
#include <string.h>

#include "input.h"
#include "drawing.h"
#include "shared.h"

#include "WSFileio.h"

extern SDL_Event event;
SDL_Joystick* joy;

const unsigned short keyCoresp[7] = {
	0,0,0,0,
	// A      B   start
	1<<10, 1<<11, 1<<9,
};

int x_button = 0, y_button = 0;

int WsInputGetState(int mode)
{
	char szFile[512];
	
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

	SDL_PollEvent(&event);
	unsigned char *keys = SDL_GetKeyState(NULL);

	button = Fire_buttons(keys);
	
	#ifdef JOYSTICK
	short x_joy = 0, y_joy = 0;
	if (SDL_NumJoysticks() > 0)
		joy = SDL_JoystickOpen(0);
		
		x_joy = SDL_JoystickGetAxis(joy, 0);
		y_joy = SDL_JoystickGetAxis(joy, 1);
			
		SDL_JoystickUpdate();
	#endif
		
	// Load
	if (keys[PAD_L] == SDL_PRESSED) 
	{
		strcpy(szFile, gameName);
#ifdef _TINSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		WsSaveState(szFile, 0);
	}
	
	// Save
	if (keys[PAD_R] == SDL_PRESSED) 
	{
		strcpy(szFile, gameName);
#ifdef _TINSPIRE
		strcpy(strrchr(szFile, '.'), ".sta.tns");
#else
		strcpy(strrchr(szFile, '.'), ".sta");
#endif
		WsLoadState(szFile, 0);
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
				button |= keyCoresp[GameConf.OD_Joy[4]];  // Button A
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[5]];  // Button B
			
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
				button |= (1<<1); // RIGHT -> X2
			if (keys[PAD_XLEFT] == SDL_PRESSED)  
				button |= (1<<3); // LEFT -> X4
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= (1<<2); // DOWN -> X3
			if (keys[PAD_XUP] == SDL_PRESSED)    
				button |= (1<<0); // UP -> X1
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[4]];  // Button A
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[5]];  // Button B
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= (1<<5); // RIGHT -> Y1
			else if (x_joy < -7500) 
				button |= (1<<7); // LEFT -> Y1
					
			if (y_joy > 7500) 
				button |= (1<<6); // DOWN -> Y1
			else if (y_joy < -7500) 
				button |= (1<<4); // UP -> Y1
			#endif
	}
	else if (GameConf.input_layout == 2)
	{
			if (keys[PAD_XRIGHT] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[5]];
			if (keys[PAD_XLEFT] == SDL_PRESSED)  
				button |= keyCoresp[GameConf.OD_Joy[4]];   	//(Rapid Fire A)
			if (keys[PAD_XDOWN] == SDL_PRESSED)  
				button |= keyCoresp[GameConf.OD_Joy[4]]; 
			if (keys[PAD_XUP] == SDL_PRESSED)    
				button |= keyCoresp[GameConf.OD_Joy[5]];	//(Rapid Fire B)
				
			if (keys[PAD_A] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[4]];  // Button A
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[5]];  // Button B
			if (keys[PAD_X] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[4]];  // Button A
			if (keys[PAD_Y] == SDL_PRESSED) 
				button |= keyCoresp[GameConf.OD_Joy[5]];  // Button B	
			
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
				button |= (1<<2); // DOWN -> Y1
			if (keys[PAD_B] == SDL_PRESSED) 
				button |= (1<<1); // RIGHT -> Y1
			if (keys[PAD_X] == SDL_PRESSED) 
				button |= (1<<3); // LEFT -> Y1
			if (keys[PAD_Y] == SDL_PRESSED) 
				button |= (1<<0); // UP -> Y1
			
			#ifdef JOYSTICK
			if (x_joy > 7500) 
				button |= keyCoresp[GameConf.OD_Joy[5]];
			else if (x_joy < -7500) 
				button |= keyCoresp[GameConf.OD_Joy[4]];   	//(Rapid Fire A)
					
			if (y_joy > 7500) 
				button |= keyCoresp[GameConf.OD_Joy[4]]; 
			else if (y_joy < -7500) 
				button |= keyCoresp[GameConf.OD_Joy[5]];	//(Rapid Fire B)
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
		button |=  keyCoresp[GameConf.OD_Joy[11]];
			
	return button;
}

int Fire_buttons(unsigned char * keys)
{
	int button = 0;
	
	if (GameConf.input_layout < 2)
	{
		if (keys[PAD_X]) 
			x_button++;
		else
			x_button = 0;
			
		if (keys[PAD_Y]) 
			y_button++;
		else
			y_button = 0;
	}	
	
	if (x_button==1)
	{
		button |= (1<<10);
	}
	else if (x_button==2)
	{
		button |= (0<<10);
		x_button = 0;
	}
	
	if (y_button==1)
	{
		button |= (1<<11);
	}
	else if (y_button==2)
	{
		button |= (0<<11);
		y_button = 0;
	}
		
	return button;
}
