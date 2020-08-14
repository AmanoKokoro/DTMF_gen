#ifndef __DTMF_H
#define __DTMF_H

/*Header*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "wave.h"


/*define macro*/
#define SAMPLESIZE 31965

/*Func*/
char* DTMFGen(int);
Sound DTMFWaveSet();


/**/

#endif // __DTMF_H
