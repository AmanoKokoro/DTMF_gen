#define _USE_MATH_DEFINES

#include "dtmf.h"
#include <math.h>


#define DATACHANKSIZE
#define DATASIZE 1311
#define PAUSESIZE 820
#define SAMPLINGRATE 16384
#define LOW 0
#define HIGH 1

static unsigned long low_rate[4] = { 697, 770, 852, 941 };
static unsigned long high_rate[4] = { 1209, 1336, 1477, 1633 };
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

//DTMFGen 
//2種類のsin波を合成した配列を作成する関数
//引数numで受け取った数字のDTMF信号を、low, highを足すことで生成する
//ampは63(signed char_MAX =127 に収めるため) 2 * 63 = 126が最大
void dtmfgen(char num, char wavedt[DATASIZE])
{
    unsigned long loop = 0;
    signed short amplitude = 63;

    num = num - '0';

    for (loop = 0; loop < DATASIZE; loop++) {
        wavedt[loop] = amplitude *
            (sin(2 * M_PI * low_rate[dtmf_num[num][LOW]] * ((double)loop / SAMPLINGRATE)) +
                sin(2 * M_PI * high_rate[dtmf_num[num][HIGH]] * ((double)loop / SAMPLINGRATE)));
    }
}

//PauseGen
//PAUSESIZEだけ0で埋めた配列を作る
void pausegen(char* pausedt[PAUSESIZE])
{
    unsigned int loop;

    for (loop = 0; loop < PAUSESIZE; loop++) {
        pausedt[loop] = 0;
    }

}

//DTMF初期設定関数
//Channel = 1 条件よりモノラル
//SamplingRate = 16384 fmax = 1633[Hz] * 2(最小限) *5(課題の条件)  = 16330 fmaxの以上で2の累乗の近似値を使う 
//Bit_per_sample = 8 条件より8[bit/sample]
//Datanum = 1311 信号音 0.08[sec] * 16384 = 1311[sample]
Sound  dtmfwaveset(char wavedt[])
{
    Sound snd;

    snd.channelnum = 1;
    snd.samplingrate = SAMPLINGRATE;
    snd.bit_per_sample = 8;
    snd.datanum = DATASIZE;
    snd.monaural8 = wavedt;
    snd.monaural16 = NULL;
    snd.stereo8 = NULL;
    snd.stereo16 = NULL;

    return snd;
}