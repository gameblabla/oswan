/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/
#include <SDL/SDL.h>
#include <string.h>
#include "../sdl/shared.h"

#include "WSFileio.h"
#include "WSInput.h"

extern SDL_Event event;
SDL_Joystick* joy;

const unsigned short keyCoresp[7] = {
	0,0,0,0,
	// A      B   start
	1<<10, 1<<11, 1<<9,
};

int WsInputGetState(int mode)
{
	char szFile[512];
	int button = 0; // Button state: 0.0.0.0.B.A.START.OPTION  X4.X3.X2.X1.Y4.Y3.Y2.Y1
	
	#ifdef JOYSTICK
	short x_joy = 0, y_joy = 0;
	if (SDL_NumJoysticks() > 0)
		joy = SDL_JoystickOpen(0);
		
		x_joy = SDL_JoystickGetAxis(joy, 0);
		y_joy = SDL_JoystickGetAxis(joy, 1);
			
		SDL_JoystickUpdate();
	#endif
		
	SDL_PollEvent(&event);
	unsigned char *keys = SDL_GetKeyState(NULL);
		
	if (keys[PAD_XUP] == SDL_PRESSED)    
		button |= (1<<4); // UP -> X1
	if (keys[PAD_XRIGHT] == SDL_PRESSED) 
		button |= (1<<5); // RIGHT -> X2
	if (keys[PAD_XDOWN] == SDL_PRESSED)  
		button |= (1<<6); // DOWN -> X3
	if (keys[PAD_XLEFT] == SDL_PRESSED)  
		button |= (1<<7); // LEFT -> X4

	if (keys[PAD_A] == SDL_PRESSED) 
		button |= keyCoresp[GameConf.OD_Joy[4]];  // Button A
	if (keys[PAD_B] == SDL_PRESSED) 
		button |= keyCoresp[GameConf.OD_Joy[5]];  // Button B
		
	if (keys[PAD_X]) 
		button |= (1<<4);
	if (keys[PAD_Y]) 
		button |= (1<<5);
		
	// Load
	if (keys[PAD_L] == SDL_PRESSED) 
	{
		strcpy(szFile, gameName);
		WsSaveState(szFile, 0);
	}
	
	// Save
	if (keys[PAD_R] == SDL_PRESSED) 
	{
		strcpy(szFile, gameName);
		WsLoadState(szFile, 0);
	}

	#ifdef JOYSTICK
		if (x_joy > 7500) 
			button |= (1<<1); // RIGHT -> Y1
		else if (x_joy < -7500) 
			button |= (1<<3); // LEFT -> Y1
			
		if (y_joy > 7500) 
			button |= (1<<2); // DOWN -> Y1
		else if (y_joy < -7500) 
			button |= (1<<0); // UP -> Y1
	#else
		if (keys[PAD_YUP] == SDL_PRESSED)    
			button |= (1<<0); // UP -> Y1
		if (keys[PAD_YRIGHT] == SDL_PRESSED) 
			button |= (1<<2); // RIGHT -> Y2
		if (keys[PAD_YDOWN] == SDL_PRESSED)  
			button |= (1<<1); // DOWN -> Y3
		if (keys[PAD_YLEFT] == SDL_PRESSED)  
			button |= (1<<3); // LEFT -> Y4
	#endif
			
	if (keys[PAD_QUIT] == SDL_PRESSED) 
	{
		m_Flag = GF_MAINUI;
	}
			
	if (keys[PAD_START] == SDL_PRESSED)  { button |=  keyCoresp[GameConf.OD_Joy[11]]; } // START -> START 
	/*if (keys[PAD_SELECT] == SDL_PRESSED) { button |=  keyCoresp[GameConf.OD_Joy[11]]; }*/ // SELECT -> OPTION 
			
	return button;
}
