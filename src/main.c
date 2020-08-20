#include "main.h"

int main(int argc, char *argv[])
{
	Sound dtmf;						   //Sound型構造体
	signed char wavedt[DATACHANCSIZE]; //Sound型dtmfに渡す15桁のDTMF信号のデータが入る配列
	signed char datadt[DATASIZE];	   //[n桁目の数字][]
	signed char pausedt[PAUSESIZE] = { 0 };

	if (argc != 3)
	{
		help();
	}

	//Sound Object set
	dtmf.channelnum = 1;
	dtmf.samplingrate = SAMPLINGRATE;
	dtmf.bit_per_sample = 8;
	dtmf.datanum = DATACHANCSIZE;
	dtmf.monaural8 = wavedt;
	dtmf.monaural16 = NULL;
	dtmf.stereo8 = NULL;
	dtmf.stereo16 = NULL;

	//pausegen(pausedt);

	for (char loop = 0; loop < DIGIT; loop++)
	{
		dtmfgen(argv[1][loop], datadt);
		memcat(wavedt, datadt, DATASIZE);
		memcat(wavedt, pausedt, PAUSESIZE);
	}

	if(Write_Wave(argv[2], &dtmf))
	{
		Free_Sound(&dtmf);
		return 1;
	}

	Free_Sound(&dtmf);
	return 0;
}

//Help
//Help表示関数
void help()
{
	printf("Usage\n");
	printf("DTMF_gen [NUMBER] [OUTPUTFILE]\n\n");
}

//memcat
//配列結合関数dstにsrcを追加する関数
//dstは書き換えられる
void memcat(char dst[], char src[], unsigned int limit)
{
	static unsigned long dstloop = 0;
	unsigned long srcloop;
	for (srcloop = 0; srcloop < limit; dstloop++, srcloop++)
	{
		dst[dstloop] = src[srcloop];
	}
}