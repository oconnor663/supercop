#ifndef Decode_H
#define Decode_H

#define Decode crypto_decode_653x1541_ref_Decode

#include "crypto_uint16.h"

/* Decode(R,s,M,len) */
/* assumes 0 < M[i] < 16384 */
/* produces 0 <= R[i] < M[i] */
extern void Decode(crypto_uint16 *,const unsigned char *,const crypto_uint16 *,long long);

#endif
