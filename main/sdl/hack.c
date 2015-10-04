#ifdef HACKS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "WSHard.h"

#include "hack.h"
#include "md5.h"

/*
 * Wonderswan Mono Hack
*/


const char tableau_games[128][33] =
{
	{"240b4e6487c27ef5b0f85983ae77fb47"},	/* Klonoa 			*/
	{"f88ec64325ba642a2dcdfbe55ce034fc"},	/* Pocket Fighter	*/
};

void Check_MD5(unsigned char* buf, FILE* fp)
{
	char *out = (char*)malloc(33);
	unsigned char digest[16];
	unsigned char i;
	
	MD5_CTX md5;
	MD5_Init(&md5);
	MD5_Update(&md5, buf, sizeof(fp));
	MD5_Final(digest, &md5);
	
	wsmono_hack = 0;
	
    for (i = 0; i < 16; i++) 
    {
        snprintf(&(out[i*2]), sizeof(&out), "%02x", (unsigned char)digest[i]);
	}
	
	for(i=0;i<128;i++)
	{	
		if (strcmp(out,tableau_games[i])==0)
		{
			wsmono_hack = 1;
			break;
		}
	}
	
    /*
    printf("Period % d\n", period_hack);   
    printf("%s\n", out);
    */
}
 
#endif
