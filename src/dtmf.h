#ifndef __DTMF_H
#define __DTMF_H

/*Header*/
#include <math.h>
#include <stdio.h>
#include "wave.h"


/*define macro*/
#define SAMPLESIZE 31965

/*Func*/
Sound DTMFGen(int);
Sound DTMFWaveSet();


/**/

#endif // __DTMF_H

#ifndef M_PI
#define M_PI (3.14159265358979323846)

#endif