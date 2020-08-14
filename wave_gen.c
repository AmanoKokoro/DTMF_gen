//VisualStudio�΍�i���w�萔�̎g�p������
//#define _USE_MATH_DEFINES

#include"src/wave.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define DATASIZE 48000

int main(int argc, char *argv[])
{
	unsigned long length,loop;
	signed short wavedt[DATASIZE];
	Sound snd;
	
	unsigned int freq = 880;
	signed short amplitude = 10000;
	
	if(argc != 2){
		printf("Usage: program <outputfile>\n");
		return 1;
	}

	

	snd.channelnum = 1;
	snd.samplingrate = 48000;
	snd.bit_per_sample = 16;
	snd.datanum = DATASIZE;
	snd.monaural16 = wavedt;
	
	printf("FILE INFO\n");
	printf("channelnum = %d\n",snd.channelnum);
	printf("samplingrate = %ld\n",snd.samplingrate);
	printf("bit_per_sample = %d\n",snd.bit_per_sample);
	printf("datanum = %ld\n",snd.datanum);
	
	printf("\nWAVE INFO\n");
	printf("freq = %d\n",freq);
	printf("amplitude = %d\n",amplitude);
	
	
	for(loop=0;loop<DATASIZE;loop++){
		wavedt[loop] = amplitude * sin(2*M_PI*freq*((double)loop/snd.samplingrate));

		printf("%d ≒ %f = %d * %f \n", wavedt[loop], amplitude * sin(2*M_PI*freq*((double)loop/snd.samplingrate)), amplitude, sin(2*M_PI*freq*((double)loop/snd.samplingrate)));
	}
	
	if(Write_Wave(argv[1], &snd)){
		Free_Sound(&snd);
		return 1;
	}

	Free_Sound(&snd);
	loop ++;

	return 0;
}
