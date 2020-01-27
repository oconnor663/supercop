/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#ifndef FFT_H
#define FFT_H

#include <stdint.h>
#include "params.h"
#include "vec.h"

void fft(vec [][ GFBITS ], vec *);

#endif

