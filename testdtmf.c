#include "src/dtmf.h"
#include <stdlib.h>
#include <stdio.h>

void main(int argc, char *argv[])
{
    Sound dtmf_signal;

    printf("\ninput = 0\n");

    dtmf_signal = DTMFWaveSet();
    dtmf_signal = DTMFGen(0);

    Write_Wave(argv[1], &dtmf_signal);

    Free_Sound(&dtmf_signal);
}
