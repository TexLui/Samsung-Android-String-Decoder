/*
 *	Copyright (C) 2015 jonchliu 
 *
 *	Sample to decode encoded ASCII strings hidden in many Samsung Android binaries.
 *	To decode any newly found strings you would need to copy not only the encoded string
 *	but it's associated bitmask and mixer array.
 *
 *	Compiled with: gcc -Wall -O2 -o decode decode.c
 *
 */

#include <stdio.h>
#include <string.h>

char g_buf[100];

char *decode_string(unsigned int mask, int size, void *mixer, void *string)
{
	/* Organise into rows of 2 columns. Each bit of bitmask selects the column. */
	int  (*m)[2] = (int  (*)[2])mixer;
	char (*s)[2] = (char (*)[2])string; 

	int i = 0;
	int j = 0;

	if (size > sizeof(g_buf))
		return(NULL);

	memset(g_buf, 0, sizeof(g_buf));

	for(i=0; i<size; i++)
	{
		g_buf[i] = s[j][mask & 1];

		j = m[j][mask & 1];

		mask >>= 1;
	}

	g_buf[size] = '\0'; /* terminate the ASCII string */
		
	return(g_buf);
}

/* An example hidden string found in a Samsung binary.
   Decodes to "ro.serialno" */
#define ENCSTR ("ro^soril4tena.Gfarl1ly") 

int main(void)
{
	unsigned int bitmask = 0x22E; /* binary 1000101110 */
	char str[] = ENCSTR; 

	int mix[] = { 0, 6, 0, 5, 4, 0, 8, 0, 0, 0, 8, 2, 0, 1, 0, 0, 10, 3, 0, 0, 5, 0 };

	int size = sizeof(ENCSTR)/2; /* decoded string len is half encoded string len */

	printf("%s \n", decode_string(bitmask, size, mix, str));

	return 0;
}

