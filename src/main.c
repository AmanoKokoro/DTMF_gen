#include "main.h"

int main(int argc, char *argv[])
{
	Sound dtmf;								  /*Sound型構造体*/
	unsigned char wavedt[DATACHANCSIZE];	  /*Sound型dtmfに渡す15桁のDTMF信号のデータが入る配列*/
	unsigned char datadt[SIGNALSIZE] = {0};	  /*[n桁目の数字]*/
	unsigned char pausedt[PAUSESIZE] = {128}; /*ポーズ 8bitモノラルでは無音を128で表すため、これが初期値*/

	if (argc != 3)
	{
		help();
	}

	/*Sound Object set*/
	dtmf.channelnum = 1;
	dtmf.samplingrate = SAMPLINGRATE;
	dtmf.bit_per_sample = 8;
	dtmf.datanum = DATACHANCSIZE;
	dtmf.monaural8 = wavedt;
	dtmf.monaural16 = NULL;
	dtmf.stereo8 = NULL;
	dtmf.stereo16 = NULL;

	for (char loop = 0; loop < DIGIT; loop++)
	{
		dtmfgen(argv[1][loop], datadt); /*一桁ごとに信号を生成*/

		/*以下生成した信号とポーズを一つの配列にまとめる処理*/
		argcat(wavedt, datadt, SIGNALSIZE);
		argcat(wavedt, pausedt, PAUSESIZE);
	}

	/*正常終了以外でエラー*/
	if (Write_Wave(argv[2], &dtmf) != 0)
	{
		return 1;
	}

	return 0;
}
/*void help()*/
/*Help表示関数*/
void help()
{
	printf("Usage\n");
	printf("DTMF_gen [NUMBER] [OUTPUTFILE]\n\n");
}