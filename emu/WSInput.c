/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/
#include <SDL/SDL.h>

#include "../sdl/shared.h"

#include "WSInput.h"

extern SDL_Event event;

const unsigned short keyCoresp[7] = {
	0,0,0,0,
	// A      B   start
	1<<10, 1<<11, 1<<9,
};

int WsInputGetState(int mode)
{
	int button = 0; // Button state: 0.0.0.0.B.A.START.OPTION  X4.X3.X2.X1.Y4.Y3.Y2.Y1
		
	SDL_PollEvent(&event);
	unsigned char *keys = SDL_GetKeyState(NULL);
		
	switch (mode) {
		case 0: // Screen normal
			if (keys[PAD_UP] == SDL_PRESSED)    { button |= (1<<4); } // UP -> X1
			if (keys[PAD_RIGHT] == SDL_PRESSED) { button |= (1<<5); } // RIGHT -> X2
			if (keys[PAD_DOWN] == SDL_PRESSED)  { button |= (1<<6); } // DOWN -> X3
			if (keys[PAD_LEFT] == SDL_PRESSED)  { button |= (1<<7); } // LEFT -> X4

			if (keys[PAD_A] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[4]]; }  // Button A
			if (keys[PAD_B] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[5]]; }  // Button B

			if (keys[PAD_X] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[6]]; }  // Button X
			if (keys[PAD_Y] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[7]]; }  // Button Y

			if (keys[PAD_R] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[8]]; }  // Button R
			if (keys[PAD_L] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[9]]; }  // Button L
			
			if (keys[PAD_QUIT] == SDL_PRESSED) 
			{ 
					m_Flag = GF_MAINUI;
			}
			
			else if (keys[PAD_START] == SDL_PRESSED)  { button |=  keyCoresp[GameConf.OD_Joy[10]]; } // START -> START 
			else if (keys[PAD_SELECT] == SDL_PRESSED) { button |=  keyCoresp[GameConf.OD_Joy[11]]; } // SELECT -> START 
			
			break;
		case 1: // Screen landscape
			if (keys[PAD_UP] == SDL_PRESSED)    { button |= (1<<4); } // UP -> X1
			if (keys[PAD_RIGHT] == SDL_PRESSED) { button |= (1<<5); } // RIGHT -> X2
			if (keys[PAD_DOWN] == SDL_PRESSED)  { button |= (1<<6); } // DOWN -> X3
			if (keys[PAD_LEFT] == SDL_PRESSED)  { button |= (1<<7); } // LEFT -> X4

			if (keys[PAD_A] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[4]]; }  // Button A
			if (keys[PAD_B] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[5]]; }  // Button B

			if (keys[PAD_X] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[6]]; }  // Button X
			if (keys[PAD_Y] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[7]]; }  // Button Y

			if (keys[PAD_R] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[8]]; }  // Button R
			if (keys[PAD_L] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[9]]; }  // Button L
			
			if (keys[PAD_QUIT] == SDL_PRESSED) 
			{ 
					m_Flag = GF_MAINUI;
			}
			
			else if (keys[PAD_START] == SDL_PRESSED)  { button |=  keyCoresp[GameConf.OD_Joy[10]]; } // START -> START 
			else if (keys[PAD_SELECT] == SDL_PRESSED) { button |=  keyCoresp[GameConf.OD_Joy[11]]; } // SELECT -> START 
			
			break;
		/*case 1: 
			// Screen on landscape
			* 
			if (keys[PAD_UP] == SDL_PRESSED)    { button |= (1<<4); } // UP -> X1
			if (keys[PAD_RIGHT] == SDL_PRESSED) { button |= (1<<5); } // RIGHT -> X2
			if (keys[PAD_DOWN] == SDL_PRESSED)  { button |= (1<<6); } // DOWN -> X3
			if (keys[PAD_LEFT] == SDL_PRESSED)  { button |= (1<<7); } // LEFT -> X4

			if (keys[PAD_A] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[4]]; }  // Button A
			if (keys[PAD_B] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[5]]; }  // Button B

			if (keys[PAD_X] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[6]]; }  // Button X
			if (keys[PAD_Y] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[7]]; }  // Button Y

			if (keys[PAD_R] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[8]]; }  // Button R
			if (keys[PAD_L] == SDL_PRESSED) { button |= keyCoresp[GameConf.OD_Joy[9]]; }  // Button L
			
			if (keys[PAD_QUIT] == SDL_PRESSED) 
			{ 
					m_Flag = GF_MAINUI;
			}
			
			else if ( (keys[PAD_START] == SDL_PRESSED) )  { button |=  keyCoresp[GameConf.OD_Joy[10]]; } // START -> START 
			else if ( (keys[PAD_SELECT] == SDL_PRESSED) ) { button |=  keyCoresp[GameConf.OD_Joy[11]]; } // SELECT -> START 
			break;
		*/
	}
	return button;
}
