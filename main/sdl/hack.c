#ifdef SPEEDHACKS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "WSHard.h"

#include "hack.h"
#include "md5.h"

const char tableau_games[128][33] =
{
	{"40b6a56d5d964231c82e00d8b40bbf26"},	//	Dicing Knight
	{"60045cc8567724881a341ab6b4030889"},	//	Mr. Driller
	{"0c7521832a182e630322c011ee9194e5"},	//	Digimon Tamers 02 Color (sequel to the B&W one)
	{"80a50a848456aaa7438c0b8a3aafbd3f"},	//	Digimon Tamers 01 (B&W)
	{"fb57455e67f1f76fdc190616092ee6ae"},	//	Final Fantasy IV
	{"8da17b62a981cada9890869275f7b78b"},	//	Final Fantasy II
	{"7e5d7b05b4f7c186fcc8be883e8dd974"},	//	Final Fantasy I
	{"473f37946ac7900e8663b5d59944e546"},	//	Digimon Spirit 1.5
	{"d27a18763ffa81775a463055bbfaa4d0"},	//	Rockman WS
	{"bf1ae975cbffae72ad029ac720304ebd"},	//	Rockman N1
	{"26d864a4c787614b8ad41ab8c0b4f009"},	//	Rivieria
	{"0c7ca84977be48828b7f3fd92ba7dd2e"},	//	Tetris
	{"ddbb286ddebe7fb563cb087c01a83707"}, 	//	Naruto
	{"8d02ef5711368fee511cdcd01dfc6c40"}, 	//	Kinnikuman
};

const unsigned char tab_periods[128] =
{
	25,	//	Dicing Knight
	20,	//	Mr. Driller
	5,	//	Digimon Tamers 02 Color (sequel to the B&W one)
	5,	//	Digimon Tamers 01 (B&W)
	25,	//	Final Fantasy IV
	25,	//	Final Fantasy II
	25,	//	Final Fantasy I
	10,	//	Digimon Spirit 1.5
	9,	//	Rockman WS
	100,	//	Rockman N1
	16,	//	Rivieria
	5,	//	Tetris
	17, // 	Naruto 
	6, 	//	Kinnikuman
};


/*
 * Hacky Workaround to speed up things a little bit 
 * (The speed effects be quite significant depending on the game, especially Digimon 02)
 * For each game (using their MD5 as identifier), choose the good period.
 * The less it is, the faster the emulator will run.
*/

void Check_MD5(unsigned char* buf, FILE* fp)
{
	char *out = (char*)malloc(33);
	unsigned char digest[16];
	unsigned char i;
	
	MD5_CTX md5;
	MD5_Init(&md5);
	MD5_Update(&md5, buf, sizeof(fp));
	MD5_Final(digest, &md5);
	
    for (i = 0; i < 16; i++) 
    {
        snprintf(&(out[i*2]), 16*2, "%02x", (unsigned char)digest[i]);
	}
  
  	period_hack = 0;
  
	for(i=0;i<128;i++)
	{	
		if (strcmp(out,tableau_games[i])==0)
		{
			period_hack = tab_periods[i];
			i=129;
		}
	}
	
	if (period_hack == 0) period_hack = 25;
	
    printf("Period % d\n", period_hack);   
    printf("%s\n", out);
}
 
#endif
