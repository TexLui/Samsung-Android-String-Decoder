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

char *decode_string(unsigned int mask, unsigned int len, void *mixer, void *string)
{
	if ((mixer == NULL) || (string == NULL))
		return(NULL);

	/* Organise into rows of 2 columns. Each bit of bitmask selects the column. */
	int  (*m)[2] = (int  (*)[2])mixer;
	char (*s)[2] = (char (*)[2])string; 

	/* Length of the decoded string is half of the encoded string */
	int decLen = len/2; 

	if (decLen > sizeof(g_buf))
		return(NULL);

	memset(g_buf, 0, sizeof(g_buf));

	int i = 0;
	int j = 0;

	/* Decode... */
	for(i=0; i<decLen; i++) 
	{
		g_buf[i] = s[j][mask & 1];

		j = m[j][mask & 1];

		mask >>= 1;
	}

	/* Terminate the ASCII string */
	g_buf[decLen] = '\0'; 
		
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

