/*  
  COPYRIGHT (C) 2015, Bruno Marie 
  (aka Gameblabla)
  
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
                    Version 2, December 2004 

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net> 

 Everyone is permitted to copy and distribute verbatim or modified 
 copies of this license document, and changing it is allowed as long 
 as the name is changed. 

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "drawing.h"
#include "shared.h"
#include "browser.h"
#include "font_drawing.h"

const char ext[EXTENSIONS][10] = { ".ws", ".wsc", ".bin", ".zip" };

static char file_name[MAX_LENGH][512];
static char* currentdir;

static uint16_t file_type[MAX_LENGH];
static uint32_t button_state_brw[9];
static uint32_t fileid_selected;
static uint32_t choice;
static uint32_t scroll_choice;
static uint32_t numb_files;

char file_to_start[MAX_LENGH];

static void Controls()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						button_state_brw[0] = 1;
					break;
					case SDLK_DOWN:
						button_state_brw[1] = 1;
					break;
					case SDLK_LEFT:
						button_state_brw[2] = 1;
					break;
					case SDLK_RIGHT:
						button_state_brw[3] = 1;
					break;
					case SDLK_LCTRL:
						button_state_brw[4] = 1;
					break;
					case SDLK_LALT:
						button_state_brw[6] = 1;
					break;
				}
			break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						button_state_brw[0] = 0;
					break;
					case SDLK_DOWN:
						button_state_brw[1] = 0;
					break;
					case SDLK_LEFT:
						button_state_brw[2] = 0;
					break;
					case SDLK_RIGHT:
						button_state_brw[3] = 0;
					break;
					case SDLK_LCTRL:
						button_state_brw[4] = 0;
					break;
					case SDLK_LALT:
						button_state_brw[6] = 0;
					break;
				}
			break;
		}
	}

}

static void Set_Fileid()
{
	/* fileid_selected is the id of the file selected (first dimension of the file_name array) */
	fileid_selected = choice + (scroll_choice*12);
}

/* Is the path a folder ? */
static int32_t is_folder(char* str1)
{
	struct stat st;
	if ( stat( str1, &st ) == 0 && S_ISDIR( st.st_mode ) ) return 1;
	return 0;
}

/*
	Draw the list of files on-screen (it is divided by the scroll_choice variable)
	For example, scroll_choice = 1 means that files from id 12 to 23 will be shown etc...
*/
static void Draw_files_list()
{
	uint32_t i;
	for (i = 0; i < 12; i++)
	{
		print_string(file_name[i+(scroll_choice*12)],file_type[i+(scroll_choice*12)],0,48,40+(16*i), Surface_to_Draw);
	}
}

static void Refresh_screen(int32_t filemode)
{	
	if (filemode == 2)
	{
		choice = 0;
		scroll_choice = 0;
		Set_Fileid();
	}
	
	/* Then draw the cursor again after being cleared, it will be shown after we passed SDL_Flip/UpdateRect to it*/
	print_string("=>",RED_C,0,16,40+(choice*16), Surface_to_Draw);
	
	Draw_files_list();
	
	print_string(currentdir,GREEN_C,0,8,16, Surface_to_Draw);
	print_string(DEFAULT_TEXT,65535,0,8,6, Surface_to_Draw);
}

static void list_all_files(char* directory)
{
	DIR *dir;
	struct dirent *ent;
	int32_t temp;
	int32_t i, a;
	int32_t onedot;
	int32_t found_ext;
	char* pch[EXTENSIONS];
	
	/* Reset all the stored files to zero */
	for (i=0;i<MAX_LENGH;i++)
	{
		strcpy(file_name[i], "");
	}
	
	i = 0;
	temp = 0;
	onedot = -1;
	numb_files = 0;
	found_ext = 0;
	
	if ((dir = opendir (directory)) != NULL) 
	{
		while ( (ent = readdir (dir)) != NULL ) 
		{
			/* Add the .. string and then after, reject them all */
			if (i == 0)
			{
				strcpy(file_name[i], "..");
				file_type[i] = TUR_C;
				i++;
				numb_files++;
			}
			
			found_ext = 0;

			/* Reject these two signs and the executable itself */
			char* twodots = strstr (ent->d_name,"..");
			onedot = strncmp (ent->d_name, ".", 2);
			
			/* If file has ".tns" extension, is a folder and is not ".." and "." */
			if (twodots == NULL && onedot != 0)
			{
				/* Copy string cotent from ent->d_name to file_name[i] */
				strcpy(file_name[i], ent->d_name);
				
				for(a=0;a<EXTENSIONS;a++)
				{
					pch[a] = strstr (ent->d_name, ext[a]);
					if (pch[a] != NULL)
					{
						found_ext = 1;
						file_type[i] = BLUE_C;
					}
				}
				
				if (found_ext == 0)
				{
					if (is_folder(ent->d_name)) file_type[i] = F_C;
					else file_type[i] = GREEN_C;
				}
				
				i++;
				numb_files++;
			}
			
		}
		closedir (dir);
	} 
	numb_files = numb_files - 1;
}

static void Goto_folder()
{
	char buf[MAX_LENGH];
	char currentdir_temp[MAX_LENGH];
	
	/* Refresh current directory, just to make sure */
	currentdir = getcwd(buf, MAX_LENGH);

	snprintf(currentdir_temp, sizeof(currentdir_temp), "%s/%s", currentdir, file_name[fileid_selected]);

	chdir(currentdir_temp);
	
	/* Get current directory. */
	currentdir = getcwd(buf, MAX_LENGH);
	
	list_all_files(currentdir);
	Refresh_screen(2);
}

uint32_t FileBrowser()
{
	int32_t i;

	Goto_folder();
	
	for(i=0;i<7;i++)
		button_state_brw[i] = 0;
	
	while (1)
	{
		memset(Surface_to_Draw, 0, (screen_scale.w_display*screen_scale.h_display)*2);
		Refresh_screen(0);
			
		/* If Up button is pressed down... (or Left button held) */
		if (button_state_brw[0] == 1 || button_state_brw[2] > 0)
		{
			if (choice > 0) 
			{
				choice--;
				Set_Fileid();
			}
			else if (scroll_choice > 0)
			{
				choice = 11;
				scroll_choice = scroll_choice - 1;
				Set_Fileid();
			}
			button_state_brw[0] = 0;
		}
		else if (button_state_brw[2] == 1)
		{
			if (choice > 0+5) 
			{
				choice-=5;
				Set_Fileid();
			}
			else if (scroll_choice > 0)
			{
				choice = 11;
				scroll_choice = scroll_choice - 1;
				Set_Fileid();
			}
			button_state_brw[2] = 0;
		}
		
		/* If Down button is pressed down... (or Right button held) */
		else if (button_state_brw[1] == 1)
		{
			/* Don't let the user to scroll more than there are files... */
			if (fileid_selected < numb_files)
			{
				if (choice < 11) 
				{
					choice++;	
					Set_Fileid();
				}
				/* If the user wants to go down and there are more files, change the files to shown to the next one (thanks to scroll_choice) */
				else if (numb_files > 10)
				{
					scroll_choice = scroll_choice + 1;
					choice = 0;
					Set_Fileid();
				}
			}
			button_state_brw[1] = 0;
		}
		else if (button_state_brw[3] == 1)
		{
			/* Don't let the user to scroll more than there are files... */
			if (fileid_selected < numb_files)
			{
				if (choice < 11-5) 
				{
					choice+=5;	
					Set_Fileid();
				}
				/* If the user wants to go down and there are more files, change the files to shown to the next one (thanks to scroll_choice) */
				else if (numb_files > 10)
				{
					scroll_choice = scroll_choice + 1;
					choice = 0;
					Set_Fileid();
				}
			}
			button_state_brw[3] = 0;
		}
			
		if (button_state_brw[6] == 1) return 0;
			
		/* If Control/Return button is pressed... */
		if (button_state_brw[4] == 1 || button_state_brw[5] == 1)
		{
			if (file_type[fileid_selected] == BLUE_C) 
			{
				snprintf(file_to_start, MAX_LENGH, "%s/%s", currentdir, file_name[fileid_selected]);
				return 1;
			}
			/* If not then it is a folder, if thats the case then go to that folder */
			else if (file_type[fileid_selected] == F_C || choice == 0) 
				Goto_folder();
			
			button_state_brw[4] = 0;
			button_state_brw[5] = 0;
		}

		/* Don't waste CPU cycles */
		Update_Screen();
		
		Controls();
	}
	return 0;
}
