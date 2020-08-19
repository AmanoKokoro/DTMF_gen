#include "dtmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void help();

int main(int argv, char* argc[])
{
	Sound dtmf;
	int num;
	int digit = 15;

	if (argv != 3 || strstr(argc[2], ".wav") || strstr(argc[1], "-h"))
	{
		help();
	}

	num = atoi(argc[1]);

	dtmf = dtmfwaveset();

	return 0;
}

void help()
{
	printf("Usage\n");
	printf("DTMF_gen [NUMBER] [OUTPUTFILE]\n\n");
	printf("OPTIONS\n");
	printf("-h\tPrint this help.");
}