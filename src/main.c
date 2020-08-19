#include "main.h"

int main(int argc, char *argv[])
{
	Sound dtmf;						   //Sound型構造体
	signed char wavedt[DATACHANCSIZE]; //Sound型dtmfに渡す15桁のDTMF信号のデータが入る配列
	signed char datadt[DATASIZE];	   //[n桁目の数字][]
	signed char pausedt[PAUSESIZE];

	if (argc != 3)
	{
		help();
	}

	pausegen(pausedt);

	for (char loop = 0; loop < DIGIT; loop++)
	{
		dtmfgen(argv[1][loop], datadt);
		memcat(wavedt, datadt, DATASIZE);
		memcat(wavedt, pausedt, PAUSESIZE);
	}

	//出来上がったDTMF信号の配列でSound型構造体を作成
	dtmf = dtmfwaveset(wavedt);

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
	for (srcloop = 0; srcloop < limit || dstloop < DATACHANCSIZE; dstloop++, srcloop++)
	{
		dst[dstloop] = src[srcloop];
	}
}