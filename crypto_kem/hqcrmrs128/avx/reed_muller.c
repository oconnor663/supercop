/**
 * @file reed_muller.c
 * Constant time implementation of Reed-Muller code RM(1,7)
 */

#include "reed_muller.h"
#include "parameters.h"
#include <stdint.h>
#include <string.h>
#include <x86intrin.h>

// number of repeated code words
#define MULTIPLICITY                   CEIL_DIVIDE(PARAM_N2, 128)

// codeword is 128 bits, seen multiple ways
typedef union {
	__mmask16 mask[8];
	uint16_t u16[8];
	uint32_t u32[4];
} codeword __attribute__ ((aligned (16)));

// a 256 bit vector of 16 ints of 16 bits
typedef int16_t i16x16 __attribute__ ((vector_size (32)));
typedef uint16_t u16x16 __attribute__ ((vector_size (32)));

typedef union {
	__m256i mm;
	u16x16 v;
} vector __attribute__ ((aligned (32)));

// Expanded codeword is 16*128 bits, seen multiple ways
typedef union {
	__m256i mm[8];     // four intel vectors
	i16x16 v[8];       // four vectors of 32 ints
	int16_t i16[128];
} expandedCodeword __attribute__ ((aligned (32)));

const i16x16  count_vector = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const vector bit_vector = {.v = {
	1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7,
	1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15}};

// copy bit 0 into all bits of a 64 bit value
#define BIT0MASK(x) (int64_t)(-((x) & 1))

void encode(codeword *word, int32_t message);
void expand_and_sum(expandedCodeword *dst, codeword src[]);
void hadamard(expandedCodeword *src, expandedCodeword *dst);
int32_t find_peaks(expandedCodeword *transform);



/**
 * @brief Encode a single byte into a single codeword using RM(1,7)
 *
 * Encoding matrix of this code:
 * bit pattern (note that bits are numbered big endian)
 * 0   aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
 * 1   cccccccc cccccccc cccccccc cccccccc
 * 2   f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * 3   ff00ff00 ff00ff00 ff00ff00 ff00ff00
 * 4   ffff0000 ffff0000 ffff0000 ffff0000
 * 5   00000000 ffffffff 00000000 ffffffff
 * 6   00000000 00000000 ffffffff ffffffff
 * 7   ffffffff ffffffff ffffffff ffffffff
 *
 * @param[out] word An RM(1,7) codeword
 * @param[in] message A message to encode
 */
inline void encode(codeword *word, int32_t message) {
	// the four parts of the word are identical
	// except for encoding bits 5 and 6
	int32_t first_word;
	// bit 7 flips all the bits, do that first to save work
	first_word = BIT0MASK(message >> 7);
	// bits 0, 1, 2, 3, 4 are the same for all four longs
	// (Warning: in the bit matrix above, low bits are at the left!)
	first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
	first_word ^= BIT0MASK(message >> 1) & 0xcccccccc;
	first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
	first_word ^= BIT0MASK(message >> 3) & 0xff00ff00;
	first_word ^= BIT0MASK(message >> 4) & 0xffff0000;
	// we can store this in the first quarter
	word->u32[0] = first_word;
	// bit 5 flips entries 1 and 3; bit 6 flips 2 and 3
	first_word ^= BIT0MASK(message >> 5);
	word->u32[1] = first_word;
	first_word ^= BIT0MASK(message >> 6);
	word->u32[3] = first_word;
	first_word ^= BIT0MASK(message >> 5);
	word->u32[2] = first_word;
	return;
}



/**
 * @brief Add multiple codewords into expanded codeword
 *
 * Note: this does not write the codewords as -1 or +1 as the green machine does
 * instead, just 0 and 1 is used.
 * The resulting hadamard transform has:
 * all values are halved
 * the first entry is 64 too high
 *
 * @param[out] dst Structure that contain the expanded codeword
 * @param[in] src Structure that contain the codeword
 */
inline void expand_and_sum(expandedCodeword *dst, codeword src[]) {
	// start converting the first copy
	for (size_t part = 0 ; part < 8 ; part++) {
		dst->v[part] = src->u16[part] >> count_vector & 1;
	}
	// sum the rest of the copies
	for (size_t copy = 1 ; copy < MULTIPLICITY ; copy++) {
		for (size_t part = 0 ; part < 8 ; part++) {
			dst->v[part] += src[copy].u16[part] >> count_vector & 1;
		}
	}
}



/**
 * @brief Hadamard transform
 *
 * Perform hadamard transform of src and store result in dst
 * src is overwritten: it is also used as intermediate buffer
 * Method is best explained if we use H(3) instead of H(7):
 *
 * The routine multiplies by the matrix H(3):
 *                     [1  1  1  1  1  1  1  1]
 *                     [1 -1  1 -1  1 -1  1 -1]
 *                     [1  1 -1 -1  1  1 -1 -1]
 * [a b c d e f g h] * [1 -1 -1  1  1 -1 -1  1] = result of routine
 *                     [1  1  1  1 -1 -1 -1 -1]
 *                     [1 -1  1 -1 -1  1 -1  1]
 *                     [1  1 -1 -1 -1 -1  1  1]
 *                     [1 -1 -1  1 -1  1  1 -1]
 * You can do this in three passes, where each pass does this:
 * set lower half of buffer to pairwise sums,
 * and upper half to differences
 * index     0        1        2        3        4        5        6        7
 * input:    a,       b,       c,       d,       e,       f,       g,       h
 * pass 1:   a+b,     c+d,     e+f,     g+h,     a-b,     c-d,     e-f,     g-h
 * pass 2:   a+b+c+d, e+f+g+h, a-b+c-d, e-f+g-h, a+b-c-d, e+f-g-h, a-b-c+d, e-f-g+h
 * pass 3:   a+b+c+d+e+f+g+h   a+b-c-d+e+f-g-h   a+b+c+d-e-f-g-h   a+b-c-d-e+-f+g+h
 * a-b+c-d+e-f+g-h   a-b-c+d+e-f-g+h   a-b+c-d-e+f-g+h   a-b-c+d-e+f+g-h
 * This order of computation is chosen because it vectorises well.
 * Likewise, this routine multiplies by H(7) in seven passes.
 *
 * @param[out] src Structure that contain the expanded codeword
 * @param[out] dst Structure that contain the expanded codeword
 */
inline void hadamard(expandedCodeword *src, expandedCodeword *dst) {
	// the passes move data:
	// src -> dst -> src -> dst -> src -> dst -> src -> dst
	// using p1 and p2 alternately
	expandedCodeword *p1 = src;
	expandedCodeword *p2 = dst;
	for (size_t pass = 0 ; pass < 7 ; pass++) {
		// warning: hadd works "within lanes" as Intel call it
		// so you have to swap the middle 64 bit blocks of the result
		for (size_t part = 0 ; part < 4 ; part++) {
			p2->mm[part] = _mm256_permute4x64_epi64(_mm256_hadd_epi16(p1->mm[2 * part], p1->mm[2 * part + 1]), 0xd8);
			p2->mm[part + 4] = _mm256_permute4x64_epi64(_mm256_hsub_epi16(p1->mm[2 * part], p1->mm[2 * part + 1]), 0xd8);
		}
		// swap p1, p2 for next round
		expandedCodeword *p3 = p1;
		p1 = p2;
		p2 = p3;
	}
}



/**
 * @brief Finding the location of the highest value
 *
 * This is the final step of the green machine: find the location of the highest value,
 * and add 128 if the peak is positive
 * Notes on decoding
 * The standard "Green machine" decoder words as follows:
 * if the received codeword is W, compute (2 * W - 1) * H7
 * The entries of the resulting vector are always even and vary from
 * -128 (= the complement is a code word, add bit 7 to decode)
 * via 0 (this is a different codeword)
 * to 128 (this is the code word).
 *
 * Our decoding differs in two ways:
 * - We take W instead of 2 * W - 1 (so the entries are 0,1 instead of -1,1)
 * - We take the sum of the repititions (so the entries are 0..MULTIPLICITY)
 * This implies that we have to subtract 64M (M=MULTIPLICITY)
 * from the first entry to make sure the first codewords is handled properly
 * and that the entries vary from -64M to 64M.
 * -64M or 64M stands for a perfect codeword.
 * If there are fewer than 32M errors, there is always a unique codeword
 * which an entry with absolute value > 32M;
 * this is because an error changes an entry by 1.
 * The highest number that seem to be decodable is 50 errors, so that the
 * highest entries in the hadamard transform can be as low as 12.
 * But this is different for the repeated code.
 * Because multiple codewords are added, this changes: the lowest value of the
 * hadamard transform of the sum of six words is seen to be as low as 43 (!),
 * which is way less than 12*6.
 *
 * It is possible that there are more errors, but the word is still uniquely
 * decodable: we found a word with distance of 50 from the nearest codeword.
 * That means that the highest entry can be as low as 14M.
 * Since we have to do binary search, we search for the range 1-64M
 * which can be done in 6+l2g(M) steps.
 * The binary search is based on (values>32M are unique):
 * M  32M     min>  max>  firstStep #steps
 * 2   64       1   64    33 +- 16    6
 * 4  128       1  128    65 +- 32    7
 * 6  192       1  192   129 +- 64    8
 *
 * As a check, we run a sample for M=6 to see the peak value; it ranged
 * from 43 to 147, so my analysis looks right. Also, it shows that decoding
 * far beyond the bound of 32M is needed.
 *
 * For the vectors, it would be tempting to use 8 bit ints,
 * because the values "almost" fit in there.
 * We could use some trickery to fit it in 8 bits, like saturated add or
 * division by 2 in a late step.
 * Unfortunately, these instructions do not exist.
 * the adds _mm512_adds_epi8 is available only on the latest processors,
 * and division, shift, mulhi are not available at all for 8 bits.
 * So, we use 16 bit ints.
 *
 * For the search of the optimal comparison value,
 * remember the transform contains 64M-d,
 * where d are the distances to the codewords.
 * The highest value gives the most likely codeword.
 * There is not fast vectorized way to find this value, so we search for the
 * maximum value itself.
 * In each pass, we collect a bit map of the transform values that are,
 * say >bound.  There are three cases:
 * bit map = 0: all code words are further away than 64M-bound (decrease bound)
 * bit map has one bit: one unique code word has distance < 64M-bound
 * bit map has multiple bits: multiple words (increase bound)
 * We will search for the lowest value of bound that gives a nonzero bit map.
 *
 * @param[in] transform Structure that contain the expanded codeword
 */
inline int32_t find_peaks(expandedCodeword *transform) {
	// a whole lot of vector variables
	__m256i bitmap, abs_rows[8], bound, active_row, max_abs_rows;
	vector peak_mask;
	// compute absolute value of transform
	for (size_t i = 0 ; i < 8 ; i++) {
		abs_rows[i] = _mm256_abs_epi16(transform->mm[i]);
	}
	// compute a vector of 16 elements which contains the maximum somewhere
	// (later used to compute bits 0 through 3 of message)
	max_abs_rows = abs_rows[0];
	for (size_t i = 1 ; i < 8 ; i++) {
		max_abs_rows = _mm256_max_epi16(max_abs_rows, abs_rows[i]);
	}

	// do binary search for the highest value that is lower than the maximum
	// loop invariant: lower gives bit map = 0, lower + width gives bit map > 0
	int32_t lower = 1;
	// this gives 64, 128 or 256 for MULTIPLICITY = 2, 4, 6
	int32_t width = 1 << (5 + MULTIPLICITY / 2);
	// if you don't unroll this loop, it fits in the loop cache
	// uncomment the line below to speeding up the program by a few percent
	// #pragma GCC unroll 0
	while (width > 1) {
		width >>= 1;
		// compare with lower + width; put result in bitmap
		// make vector from value of new bound
		bound = _mm256_broadcastw_epi16(_mm_cvtsi32_si128(lower + width));
		bitmap = _mm256_cmpgt_epi16(max_abs_rows, bound);
		// step up if there are any matches
		// rely on compiler to use conditional move here
		int32_t step_mask = _mm256_testz_si256(bitmap, bitmap) - 1;
		lower += step_mask & width;
	}
	// lower+width contains the maximum value of the vector
	// or less, if the maximum is very high (which is OK)
	// normally, there is one maximum, but sometimes there are more
	// find where the maxima occur in the maximum vector
	// (each determines lower 4 bits of peak position)
	// construct vector filled with bound-1
	bound = _mm256_broadcastw_epi16(_mm_cvtsi32_si128(lower + width - 1));

	// find in which of the 8 groups a maximum occurs to compute bits 4, 5, 6 of message
	// find lowest value by searching backwards skip first check to save time
	size_t message = 0x70;
	for (int32_t i = 7 ; i >= 0 ; i--) {
		bitmap = _mm256_cmpgt_epi16(abs_rows[i], bound);
		int message_mask = (-(int16_t)(_mm256_testz_si256(bitmap, bitmap) == 0)) >> 15;
		message ^= message_mask & (message ^ (unsigned)i << 4);
	}
	// we decided which row of the matrix contains the lowest match
	// select proper row
	int8_t index = message >> 4;
	__m256i res;
	__m256i tmp = (__m256i) {0ULL, 0ULL, 0ULL, 0ULL};

	for (int8_t i = 0; i < 8; i++) {
		int8_t abs_value = (int8_t)(index - i);
		int8_t mask1 = abs_value >> 7;
		abs_value ^= mask1;
		abs_value -= mask1;
		int8_t mask2 = ((uint8_t) - abs_value >> 7);
		int64_t mask3 = (-1ULL) + mask2;
		__m256i vect_mask = (__m256i) {mask3, mask3, mask3, mask3};
		res = _mm256_and_si256(abs_rows[i], vect_mask);
		tmp = _mm256_or_si256(tmp, res);
	}
	
	active_row = tmp;

	// get the column number of the vector element
	// by setting the bits corresponding to the columns
	// and then adding elements within two groups of 8
	peak_mask.mm = _mm256_cmpgt_epi16(active_row, bound);
	peak_mask.v &= bit_vector.v;
	for (int32_t i = 0 ; i < 3 ; i++) {
		peak_mask.mm = _mm256_hadd_epi16(peak_mask.mm, peak_mask.mm);
	}
	// add low 4 bits of message
	message |= _bit_scan_forward(peak_mask.v[0] + peak_mask.v[8]);

	// set bit 7 if sign of biggest value is positive
	// make sure a jump isn't generated by the compiler
	tmp = (__m256i) {0ULL,0ULL, 0ULL, 0ULL};
	for (uint32_t i = 0; i < 8; i++){
		int64_t message_mask = (-(int64_t)(i == message/16))>>63;
		__m256i vect_mask = (__m256i) {message_mask, message_mask, message_mask, message_mask};
		tmp = _mm256_or_si256(tmp, _mm256_and_si256(vect_mask, transform->mm[i]));
	}
	uint16_t result = 0;
	for (uint32_t i = 0; i < 16; i++) {
		uint16_t *ptr = (uint16_t *) &tmp;
		int32_t message_mask = (-(int32_t)(i == message % 16))>> (sizeof(int32_t) * 8 - 1);
		result |= message_mask & ptr[i];
	}
	message |= (0x8000 & ~result) >> 8;
	return message;
}



/**
 * @brief Encodes the received word
 *
 * The message consists of N1 bytes each byte is encoded into PARAM_N2 bits,
 * or MULTIPLICITY repeats of 128 bits
 *
 * @param[out] cdw Array of size VEC_N1N2_SIZE_64 receiving the encoded message
 * @param[in] msg Array of size VEC_N1_SIZE_64 storing the message
 */
void reed_muller_encode(uint64_t *cdw, const uint64_t *msg) {
	uint8_t *message_array = (uint8_t *) msg;
	codeword *codeArray = (codeword *) cdw;
	for (size_t i = 0 ; i < VEC_N1_SIZE_BYTES ; i++) {
		// fill entries i * MULTIPLICITY to (i+1) * MULTIPLICITY
		int32_t pos = i * MULTIPLICITY;
		// encode first word
		encode(&codeArray[pos], message_array[i]);
		// copy to other identical codewords
		for (size_t copy = 1 ; copy < MULTIPLICITY ; copy++) {
			memcpy(&codeArray[pos + copy], &codeArray[pos], sizeof(codeword));
		}
	}
	return;
}



/**
 * @brief Decodes the received word
 *
 * Decoding uses fast hadamard transform, for a more complete picture on Reed-Muller decoding, see MacWilliams, Florence Jessie, and Neil James Alexander Sloane.
 * The theory of error-correcting codes codes @cite macwilliams1977theory
 *
 * @param[out] msg Array of size VEC_N1_SIZE_64 receiving the decoded message
 * @param[in] cdw Array of size VEC_N1N2_SIZE_64 storing the received word
 */
void reed_muller_decode(uint64_t *msg, const uint64_t *cdw) {
	uint8_t *message_array = (uint8_t *) msg;
	codeword *codeArray = (codeword *) cdw;
	expandedCodeword expanded;
	for (size_t i = 0 ; i < VEC_N1_SIZE_BYTES ; i++) {
		// collect the codewords
		expand_and_sum(&expanded, &codeArray[i * MULTIPLICITY]);
		// apply hadamard transform
		expandedCodeword transform;
		hadamard(&expanded, &transform);
		// fix the first entry to get the half Hadamard transform
		transform.i16[0] -= 64 * MULTIPLICITY;
		// finish the decoding
		message_array[i] = find_peaks(&transform);
	}
}
