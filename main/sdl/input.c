#include "shared.h"
#include "menu.h"
#include "drawing.h"

static uint8_t *keys;

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

void exit_button(void)
{
	keys = SDL_GetKeyState(NULL);
	/* Get to Menu hotkey */
	if ( ((keys[SDLK_ESCAPE] == SDL_PRESSED) && (keys[SDLK_RETURN] == SDL_PRESSED)) 
	#ifdef RG99
	|| keys[SDLK_PAGEUP] == SDL_PRESSED
	#endif
	) 
	{ 
		m_Flag = GF_MAINUI;
		SetVideo(1);
	}
}

uint32_t WsInputGetState()
{
	uint32_t rotate_input = 0;
	SDL_Event event;
	int32_t button = 0;
	
	if (HVMode != 0) rotate_input = 1;
	
	SDL_PollEvent(&event);
	keys = SDL_GetKeyState(NULL);
	
	// UP -> Y1
	if (keys[ keys_config[rotate_input].buttons[0] ] == SDL_PRESSED)
	{
		button |= (1<<0);
	}
	
	// RIGHT -> Y2
	if (keys[ keys_config[rotate_input].buttons[1] ] == SDL_PRESSED)
	{
		button |= (1<<1);
	}
	
	// DOWN -> Y3
	if (keys[ keys_config[rotate_input].buttons[2] ] == SDL_PRESSED)
	{
		button |= (1<<2);
	}
	
	// LEFT -> Y4
	if (keys[ keys_config[rotate_input].buttons[3] ] == SDL_PRESSED)
	{
		button |= (1<<3);
	}
	
	// UP -> X1
	if (keys[ keys_config[rotate_input].buttons[4] ] == SDL_PRESSED)
	{
		button |= (1<<4);
	}
	
	// RIGHT -> X2
	if (keys[ keys_config[rotate_input].buttons[5] ] == SDL_PRESSED)
	{
		button |= (1<<5);
	}
	
	// DOWN -> X3
	if (keys[ keys_config[rotate_input].buttons[6] ] == SDL_PRESSED)
	{
		button |= (1<<6);
	}
	
	// LEFT -> X4
	if (keys[ keys_config[rotate_input].buttons[7] ] == SDL_PRESSED)
	{
		button |= (1<<7);
	}

	// SELECT/OTHER -> OPTION (Wonderswan button)
	if (keys[ keys_config[rotate_input].buttons[8] ] == SDL_PRESSED)
	{
		button |= (1<<8);
	}

	// START -> START (Wonderswan button)
	if (keys[ keys_config[rotate_input].buttons[9] ] == SDL_PRESSED)
	{
		button |= (1<<9);
	}
	
	// A -> A (Wonderswan button)
	if (keys[ keys_config[rotate_input].buttons[10] ] == SDL_PRESSED)
	{
		button |= (1<<10);
	}
	
	// B -> B (Wonderswan button)
	if (keys[ keys_config[rotate_input].buttons[11] ] == SDL_PRESSED)
	{
		button |= (1<<11);
	}
	
	return button;
}
