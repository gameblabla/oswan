#include <stdlib.h>
#include <stdio.h>
#include "hack.h"

short game_choosen;

extern unsigned int gameCRC;

int tableau_games[128][3] =
{
	{  902878940, 1},	//	Digimon 02 (Monochrome)
	{ -873928613, 1},	//	Digimon 02 (Color)
	{-2098137406, 25},	//	Mr. Driller
	{-1224026040, 6},	//	Riviera
	{432737515, 5},		//	Rockman EXE WS
	{877148804, 1},		//	Rockman EXE N1 	
	{-1521148107, 10},	//	Naruto
	{-965854311, 25},	//	FF1
	{-1941261760, 25},	//	FF4
	{1789917758, 25}	//	Dicing Knight
};

void hack_period();

/*
 * Hacky Workaround to speed up things a little bit 
 * (can be quite significant depending on the game, especially Digimon 02)
 * For each game (using their CRC as identifier), choose the good period.
 * The less it is, the faster the emulator will run.
*/

void hack_period()
{
	short i;
	short hack;
	hack = 0;
	
	for(i=0;i<128;i++)
	{
		if (gameCRC == tableau_games[i][0])
		{	
			hack = 1;
			game_choosen = i;
			gameCRC = tableau_games[i][0];
		}
	}
	
	if (hack == 0)
	{	
		gameCRC == tableau_games[2][0];
		game_choosen = 2;
	}
	
}
 
