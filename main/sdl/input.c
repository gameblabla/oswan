#include "shared.h"
#include "menu.h"

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
	if ( ((keys[SDLK_ESCAPE] == SDL_PRESSED) && (keys[SDLK_RETURN] == SDL_PRESSED)) || (keys[SDLK_END] == SDL_PRESSED) ) 
	{ 
		m_Flag = GF_MAINUI;
	}
}

uint32_t WsInputGetState(uint32_t mode)
{
	SDL_Event event;
	int32_t button = 0;
	
	SDL_PollEvent(&event);
	keys = SDL_GetKeyState(NULL);
	
	// UP -> Y1
	if (keys[ keys_config[profile_config].buttons[0] ] == SDL_PRESSED)
	{
		button |= (1<<0);
	}
	
	// RIGHT -> Y2
	if (keys[ keys_config[profile_config].buttons[1] ] == SDL_PRESSED)
	{
		button |= (1<<1);
	}
	
	// DOWN -> Y3
	if (keys[ keys_config[profile_config].buttons[2] ] == SDL_PRESSED)
	{
		button |= (1<<2);
	}
	
	// LEFT -> Y4
	if (keys[ keys_config[profile_config].buttons[3] ] == SDL_PRESSED)
	{
		button |= (1<<3);
	}
	
	// UP -> X1
	if (keys[ keys_config[profile_config].buttons[4] ] == SDL_PRESSED)
	{
		button |= (1<<4);
	}
	
	// RIGHT -> X2
	if (keys[ keys_config[profile_config].buttons[5] ] == SDL_PRESSED)
	{
		button |= (1<<5);
	}
	
	// DOWN -> X3
	if (keys[ keys_config[profile_config].buttons[6] ] == SDL_PRESSED)
	{
		button |= (1<<6);
	}
	
	// LEFT -> X4
	if (keys[ keys_config[profile_config].buttons[7] ] == SDL_PRESSED)
	{
		button |= (1<<7);
	}

	// SELECT/OTHER -> OPTION (Wonderswan button)
	if (keys[ keys_config[profile_config].buttons[8] ] == SDL_PRESSED)
	{
		button |= (1<<8);
	}

	// START -> START (Wonderswan button)
	if (keys[ keys_config[profile_config].buttons[9] ] == SDL_PRESSED)
	{
		button |= (1<<9);
	}
	
	// A -> A (Wonderswan button)
	if (keys[ keys_config[profile_config].buttons[10] ] == SDL_PRESSED)
	{
		button |= (1<<10);
	}
	
	// B -> B (Wonderswan button)
	if (keys[ keys_config[profile_config].buttons[11] ] == SDL_PRESSED)
	{
		button |= (1<<11);
	}
	
	return button;
}
