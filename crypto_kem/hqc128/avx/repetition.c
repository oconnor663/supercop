/**
 * @file repetition.c
 * @brief Implementation of repetition codes
 */

#include "parameters.h"
#include "repetition.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <nmmintrin.h>

#define MASK_N2                              ((1UL << PARAM_N2) - 1)

/**
 * @brief Decoding the code words to a message using the repetition code
 *
 * We use a majority decoding. In fact we have that PARAM_N2 = 2 * PARAM_T + 1, thus,
 * if the Hamming weight of the vector is greater than PARAM_T, the code word is decoded
 * to 1 and 0 otherwise.
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void repetition_code_decode(uint64_t *m, const uint64_t *em) {
	size_t t = 0, b, bn, bi, c, cn, ci;
	uint64_t cx, ones;

	for (b = 0 ; b < PARAM_N1N2 - PARAM_N2 + 1 ; b += PARAM_N2) {
		bn = b >> 6;
		bi = b & 63;
		c = b + PARAM_N2 - 1;
		cn = c >> 6;
		ci = c & 63;
		cx = em[cn] << (63 - ci);
		int64_t verif = (cn == (bn + 1));
		ones = _mm_popcnt_u64(((em[bn] >> bi) & MASK_N2) | (cx * verif));
		m[t >> 6] |= ((uint64_t)(ones > PARAM_T)) << (t & 63);
		t++;
	}
}
