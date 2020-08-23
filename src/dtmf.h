#ifndef __DTMF_H
#define __DTMF_H

/*Header*/
#include <math.h>
#include <stdio.h>
#include "wave.h"

/*define macro*/
#define DATACHANCSIZE 31965
#define SIGNALSIZE 1311
#define PAUSESIZE 820
#define SAMPLINGRATE 16384
#define LOW 0
#define HIGH 1

/*Func*/
/*0~9の1音のDTMFを作成する*/
void dtmfgen(char, unsigned char *);
/*配列に配列を結合する*/
void argcat(unsigned char *, unsigned char *, unsigned int);

#endif /*__DTMF_H*/

#ifndef M_PI
#define M_PI (3.14159265358979323846)

#endif /*M_PI*/