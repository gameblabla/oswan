#include "shared.h"

/*
0: PAD_LEFT;	
1: PAD_RIGHT;	
2: PAD_UP;
3: PAD_DOWN;
4: PAD_A;	
5: PAD_B;
6: PAD_X;	
7: PAD_Y;
8: PAD_L;	
9: PAD_R;
10: PAD_START;	
11: PAD_SELECT;
12: PAD_QUIT;
13: PAD_SLIDER;
14: PAD_XLEFT;
15: PAD_XRIGHT;
16: PAD_XUP;
17: PAD_XDOWN;
*/

int16_t button_state[18];
uint8_t button_time[18];

#ifdef JOYSTICK
	SDL_Joystick* joystick_sdl[2];
	int16_t joystick_axies[4] = {0, 0, 0, 0};
	#define JOYSTICK_UP (joystick_axies[1] < -2048 ? 1 : 0)
	#define JOYSTICK_RIGHT	(joystick_axies[0] > 2048 ? 1 : 0)
	#define JOYSTICK_LEFT	(joystick_axies[0] < -2048 ? 1 : 0)
	#define JOYSTICK_DOWN (joystick_axies[1] > 2048 ? 1 : 0)
	
	#define JOYSTICK_AXIS2_UP (joystick_axies[3] < -2048 ? 1 : 0)
	#define JOYSTICK_AXIS2_RIGHT	(joystick_axies[2] > 2048 ? 1 : 0)
	#define JOYSTICK_AXIS2_LEFT	(joystick_axies[2] < -2048 ? 1 : 0)
	#define JOYSTICK_AXIS2_DOWN (joystick_axies[3] > 2048 ? 1 : 0)
#else
	#define JOYSTICK_UP (0)
	#define JOYSTICK_RIGHT	(0)
	#define JOYSTICK_LEFT	(0)
	#define JOYSTICK_DOWN (0)
	#define JOYSTICK_AXIS2_UP (0)
	#define JOYSTICK_AXIS2_RIGHT	(0)
	#define JOYSTICK_AXIS2_LEFT	(0)
	#define JOYSTICK_AXIS2_DOWN (0)
#endif


/* Uses button_state global */
void Buttons(void)
{
	uint8_t i = 0;
#if defined(_TINSPIRE)
	t_key pad;
	#define CHECK_PAD isKeyPressed(pad)
#else
	int32_t pad, pad2, pad3;
#ifdef JOYSTICK
	if (SDL_NumJoysticks() > 0)
		joystick_sdl[0] = SDL_JoystickOpen(0);
		
	for(i=0;i<4;i++)
		joystick_axies[i] = SDL_JoystickGetAxis(joystick_sdl[0], i);

	SDL_JoystickUpdate();
	#define CHECK_PAD (keys[pad] || SDL_JoystickGetButton(joystick_sdl[0], pad2) || (pad3))
#else
	#define CHECK_PAD keys[pad]
#endif
	SDL_Event event;
	SDL_PollEvent(&event);
	uint8_t *keys = SDL_GetKeyState(NULL);
#endif

	for(i=0;i<18;i++)
	{	
		switch (i)
		{
			case 0:
				pad = PAD_LEFT;
				pad2 = 16;
				pad3 = JOYSTICK_LEFT;
			break;
			
			case 1:
				pad = PAD_RIGHT;
				pad2 = 16;
				pad3 = JOYSTICK_RIGHT;
			break;
			
			case 2:
				pad = PAD_UP;
				pad2 = 16;
				pad3 = JOYSTICK_UP;
			break;
			
			case 3:
				pad = PAD_DOWN;
				pad2 = 16;
				pad3 = JOYSTICK_DOWN;
			break;
			
			case 4:
				pad = PAD_A;
				pad2 = 2;	
				pad3 = 0;
			break;
			
			case 5:
				pad = PAD_B;
				pad2 = 1;
				pad3 = 0;	
			break;
			
			case 6:
				pad = PAD_X;
				pad2 = 0;	
				pad3 = 0;	
			break;
			
			case 7:
				pad = PAD_Y;
				pad2 = 3;
				pad3 = 0;	
			break;
			
			case 8:
				pad = PAD_L;	
				pad2 = 4;
				pad3 = 0;
			break;
			
			case 9:
				pad = PAD_R;
				pad2 = 5;
				pad3 = 0;
			break;
			
			case 10:
				pad = PAD_START;
				pad2 = 9;
				pad3 = 0;	
			break;
			
			case 11:
				pad = PAD_SELECT;
				pad2 = 8;	
				pad3 = 0;
			break;
			
			case 12:
				pad = PAD_QUIT;
				pad2 = 16;	
				pad3 = 0;
			break;
			
			case 13:
				pad = PAD_SLIDER;
				pad2 = 16;
				pad3 = 0;
			break;
			
			case 14:
				pad = PAD_XLEFT;
				pad2 = 16;
				pad3 = 0;
			break;
			
			case 15:
				pad = PAD_XRIGHT;
				pad2 = 16;
				pad3 = 0;
			break;
			
			case 16:
				pad = PAD_XUP;
				pad2 = 16;
				pad3 = 0;
			break;
			
			case 17:
				pad = PAD_XDOWN;
				pad2 = 16;
				pad3 = 0;
			break;
		}

		switch (button_state[i])
		{
			/* To avoid for the button for being pressed again */
			case -1:
				if (!(CHECK_PAD))
				{
					button_time[i]++;
				}
					
				if (button_time[i] > 1)
				{
					button_state[i] = 0;
					button_time[i] = 0;
				}
			break;
			
			case 0:
				if (CHECK_PAD)
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
				if (!(CHECK_PAD))
				{
					button_state[i] = 0;
					button_time[i] = 0;
				}
			break;
		}
	
	}	

}
