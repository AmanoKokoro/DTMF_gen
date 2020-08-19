#include "dtmf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATACHANKSIZE 31965

int main(int argc, char *argv[])
{
    Sound dtmf_signal;
    signed char datachankdt[DATACHANKSIZE] = {0};

    if (argc != 2 || strstr(argv[2], ".wav") == NULL)
    {
        printf("使用方法: testdtmf [NUMBER] [FILE]\n");
        exit(1);
    }

    dtmf_signal = dtmfwaveset();

    Write_Wave(argv[1], &dtmf_signal);
    Free_Sound(&dtmf_signal);
    
    return 0;
}