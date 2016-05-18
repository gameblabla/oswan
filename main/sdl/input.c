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

short button_state[18];
unsigned char button_time[18];

/* Uses button_state global */
void Buttons(void)
{
	unsigned char i = 0;
#if defined(_TINSPIRE)
	t_key pad;
	#define CHECK_PAD isKeyPressed(pad)
#else
	int pad = 0;
	#define CHECK_PAD keys[pad]
	
	SDL_Event event;
	SDL_PollEvent(&event);
	unsigned char *keys = SDL_GetKeyState(NULL);
#endif

	for(i=0;i<18;i++)
	{	
		switch (i)
		{
			case 0:
				pad = PAD_LEFT;
			break;
			
			case 1:
				pad = PAD_RIGHT;
			break;
			
			case 2:
				pad = PAD_UP;
			break;
			
			case 3:
				pad = PAD_DOWN;
			break;
			
			case 4:
				pad = PAD_A;	
			break;
			
			case 5:
				pad = PAD_B;
			break;
			
			case 6:
				pad = PAD_X;	
			break;
			
			case 7:
				pad = PAD_Y;
			break;
			
			case 8:
				pad = PAD_L;	
			break;
			
			case 9:
				pad = PAD_R;
			break;
			
			case 10:
				pad = PAD_START;	
			break;
			
			case 11:
				pad = PAD_SELECT;
			break;
			
			case 12:
				pad = PAD_QUIT;
			break;
			
			case 13:
				pad = PAD_SLIDER;
			break;
			
			case 14:
				pad = PAD_XLEFT;
			break;
			
			case 15:
				pad = PAD_XRIGHT;
			break;
			
			case 16:
				pad = PAD_XUP;
			break;
			
			case 17:
				pad = PAD_XDOWN;
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

}
