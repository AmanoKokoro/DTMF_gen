#include "dtmf.h"
#include "math.h"

#define DATASIZE 1311
#define LOW 0
#define HIGH 1

static unsigned long high_rate[4] = {697, 770, 852, 941};
static unsigned long low_rate[4] = {1209, 1336, 1477, 1633};
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

signed short wavedt[DATASIZE];

void DTMFGen(int num)
{
    unsigned long loop;
    signed short amplitude = 10000;
    Sound *psnd;

    psnd = DTMFWaveSet();

    printf("Low INFO\n");
	printf("channelnum = %d\n",psnd->channelnum);
	printf("samplingrate = %d\n",psnd->samplingrate);
	printf("bit_per_sample = %d\n",psnd->bit_per_sample);
	printf("datanum = %d\n",psnd->datanum);
	printf("freq = %d\n",low_rate[dtmf_num[num][LOW]]);
	printf("amplitude = %d\n",amplitude);

	printf("High INFO\n");
	printf("channelnum = %d\n",psnd->channelnum);
	printf("samplingrate = %d\n",psnd->samplingrate);
	printf("bit_per_sample = %d\n",psnd->bit_per_sample);
	printf("datanum = %d\n",psnd->datanum);
	printf("freq = %d\n",high_rate[dtmf_num[num][HIGH]]);
	printf("amplitude = %d\n",amplitude);

    for(loop=0;loop<DATASIZE;loop++){
        wavedt[loop] = amplitude *
             (sin(2 * M_PI * low_rate[dtmf_num[num][LOW]]* ((double)loop/psnd->samplingrate)) +
              sin(2 * M_PI * high_rate[dtmf_num[num][HIGH]]* ((double)loop/psnd->samplingrate)));
    }
}


//Channel = 1 条件よりモノラル
//SamplingRate = 16384 fmax = 1633[Hz] * 2(最小限) *5(課題の条件)  = 16330 fmaxの以上で2の累乗の近似値を使う 
//Bit_per_sample = 8 条件より8[bit/sample]
//Datanum = 1311 信号音 0.08[sec] * 16384 = 1311[sample]
Sound*  DTMFWaveSet()
{
    Sound* snd;

    snd->channelnum = 1;
    snd->samplingrate = 16384;
    snd->bit_per_sample = 8;
    snd->datanum = DATASIZE;
    snd->monaural8 = wavedt[DATASIZE];

    return snd;
}