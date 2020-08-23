#ifndef __DTMF_H
#define __DTMF_H

/*Header*/
#include <math.h>
#include <stdio.h>
#include "wave.h"


/*define macro*/
#define DATACHANCSIZE 31965
#define DATASIZE 1311
#define PAUSESIZE 820
#define SAMPLINGRATE 16384
#define LOW 0
#define HIGH 1


/*Func*/
void dtmfgen(char, unsigned char*);

#endif // __DTMF_H

#ifndef M_PI
#define M_PI (3.14159265358979323846)

#endif //M_PI