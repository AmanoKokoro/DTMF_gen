#include "dtmf.h"
#include <math.h>

#define DATASIZE 1311
#define PAUSESIZE 820
#define LOW 0
#define HIGH 1

static unsigned long low_rate[4] = {697, 770, 852, 941};
static unsigned long high_rate[4] = {1209, 1336, 1477, 1633};
static unsigned char dtmf_num[10][2] = {
    /*low_rate, high_rate 0~ 9*/
    {3, 1},
    {0, 0},
    {0, 1},
    {0, 2},
    {1, 0},
    {1, 1},
    {1, 2},
    {2, 0},
    {2, 1},
    {2, 2}
};

signed char wavedt[DATASIZE];
signed char pausedt[PAUSESIZE];

//DTMFGen 
//合成したSound型構造体を返す
//引数numで受け取った数字のDTMF信号を、Sound型構造体 low, highを足すことで生成する
//ampは100(char_MAX = 255 に収めるため)
//イメージ重視でSound型構造体を使いまわさず、low,high, reverseを作る
//※（無駄とか言わない、1ヶ月後の自分が見て追えるように）
Sound DTMFGen(int num)
{
    unsigned long loop;
    signed short amplitude = 100;
    Sound low;
    Sound high;
    Sound reverse_signal;

    low = DTMFWaveSet();
    high = DTMFWaveSet();
    reverse_signal = DTMFWaveSet();

    printf("Low INFO\n");
	printf("channelnum = %d\n", low.channelnum);
	printf("samplingrate = %ld\n",low.samplingrate);
	printf("bit_per_sample = %d\n",low.bit_per_sample);
	printf("datanum = %ld\n",low.datanum);
	printf("freq = %ld\n",low_rate[dtmf_num[num][LOW]]);
	printf("amplitude = %d\n",amplitude);

	printf("High INFO\n");
	printf("channelnum = %d\n",high.channelnum);
	printf("samplingrate = %ld\n",high.samplingrate);
	printf("bit_per_sample = %d\n",high.bit_per_sample);
	printf("datanum = %ld\n",high.datanum);
	printf("freq = %ld\n",high_rate[dtmf_num[num][HIGH]]);
	printf("amplitude = %d\n",amplitude);

    for(loop=0;loop<DATASIZE;loop++){
        wavedt[loop] = amplitude *
             (sin(2 * M_PI * low_rate[dtmf_num[num][LOW]]* ((double)loop/low.samplingrate)) +
              sin(2 * M_PI * high_rate[dtmf_num[num][HIGH]]* ((double)loop/high.samplingrate)));
    }

    return reverse_signal;
}

void PauseGen()
{
    unsigned int loop;
    Sound pause;
    
    for(loop=0;loop<PAUSESIZE;loop++){
        pausedt[loop] = 0;
    }

}

//DTMF初期設定関数
//Channel = 1 条件よりモノラル
//SamplingRate = 16384 fmax = 1633[Hz] * 2(最小限) *5(課題の条件)  = 16330 fmaxの以上で2の累乗の近似値を使う 
//Bit_per_sample = 8 条件より8[bit/sample]
//Datanum = 1311 信号音 0.08[sec] * 16384 = 1311[sample]
Sound  DTMFWaveSet()
{
    Sound snd;

    snd.channelnum = 1;
    snd.samplingrate = 16384;
    snd.bit_per_sample = 8;
    snd.datanum = DATASIZE;
    snd.monaural8 = wavedt;
    snd.monaural16 = NULL;
	snd.stereo8 = NULL;
	snd.stereo16 = NULL;

    return snd;
}