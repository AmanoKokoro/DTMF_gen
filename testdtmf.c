#include "src/dtmf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATACHANKSIZE 31965

void main(int argc, char *argv[])
{
    Sound dtmf_signal;
    signed char datachankdt[DATACHANKSIZE] = {0};

    if (argc != 2 || strstr(argv[2], ".wav") == NULL)
    {
        printf("使用方法: testdtmf [NUMBER] [FILE]\n");
        exit(1);
    }

    dtmf_signal = DTMFWaveSet(DATACHANKSIZE, datachankdt);
    dtmf_signal = DTMFGen(0);

    Write_Wave(argv[1], &dtmf_signal);
    Free_Sound(&dtmf_signal);
    exit(0);
}