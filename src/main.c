#include "dtmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help();

int main(int argv, char *argc[])
{
	Sound dtmf;								//Sound型構造体
	signed char wavedt[DATACHANCSIZE];		//Sound型dtmfに渡す15桁のDTMF信号のデータが入る配列
	signed char datadt[DIGIT][DATASIZE];	//[n桁目の数字][]
	signed char pausedt[PAUSESIZE];

	if (argv != 3 || strstr(argc[2], ".wav") || strstr(argc[1], "-h"))
	{
		help();
	}

	pausegen(pausedt);

	return 0;
}

void help()
{
	printf("Usage\n");
	printf("DTMF_gen [NUMBER] [OUTPUTFILE]\n\n");
	printf("OPTIONS\n");
	printf("-h\tPrint this help.\n");
}