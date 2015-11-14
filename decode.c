/*
 *	Copyright (C) 2015 jonchliu 
 *
 *	Sample to decode encoded ASCII strings hidden in many Samsung Android binaries.
 *
 *	Compiled with: gcc -Wall -O2 -o decode decode.c
 *
 */

#include <stdio.h>
#include <string.h>

/* Buffer to hold decoded string */
char g_buf[100];

char *decode_string(unsigned int mask, int len, void *mixer, void *string)
{
	/* Organise into rows of 2 columns. Each bit of bitmask selects the column. */
	int  (*m)[2] = (int  (*)[2])mixer;
	char (*s)[2] = (char (*)[2])string; 

	int decLen = len/2; /* decoded string len is half encoded string len */

	if (decLen > sizeof(g_buf))
		return(NULL);

	memset(g_buf, 0, sizeof(g_buf));

	int i = 0;
	int j = 0;

	for(i=0; i<decLen; i++) 
	{
		g_buf[i] = s[j][mask & 1];

		j = m[j][mask & 1];

		mask >>= 1;
	}

	g_buf[decLen] = '\0'; /* terminate the ASCII string */
		
	return(g_buf);
}

int main(void)
{
	/* An example hidden string found in a Samsung binary along with it's associated mixer & bitmask. */

	char str[] = "ro^soril4tena.Gfarl1ly"; /* decodes to "ro.serialno" */
	int mix[] = { 0, 6, 0, 5, 4, 0, 8, 0, 0, 0, 8, 2, 0, 1, 0, 0, 10, 3, 0, 0, 5, 0 };
	unsigned int bitmask = 0x22E; /* binary 1000101110 */

	printf("Encoded string = %s \n", str);
	printf("Deocded string = %s \n", decode_string(bitmask, strlen(str), mix, str));

	return 0;
}

