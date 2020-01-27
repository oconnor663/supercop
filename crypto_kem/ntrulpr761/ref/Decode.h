#ifndef Decode_H
#define Decode_H

#define Decode crypto_kem_ntrulpr761_ref_Decode

/* Decode(R,s,M,len) */
/* assumes 0 < M[i] < 16384 */
/* produces 0 <= R[i] < M[i] */
extern void Decode(uint16 *,const unsigned char *,const uint16 *,long long);

#endif
