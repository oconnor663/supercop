/* ****************************** *
 * Titanium_CCA_toy               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Samplers                       *
 * ****************************** */
 
#ifndef SAMPLER_H
#define SAMPLER_H

#include "param.h"
#include <stdint.h>

void sampler_zb(uint64_t sample[T][N + K1 + 1]);
void sampler_zq(uint64_t *sample, uint32_t slen, uint32_t bytpc);
void sampler_binomial(uint64_t sample[T][D + K1 + 1]);

#endif
