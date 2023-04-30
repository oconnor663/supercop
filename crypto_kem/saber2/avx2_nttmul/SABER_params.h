#ifndef PARAMS_H
#define PARAMS_H

/* Change this for different security strengths */
// #define SABER_K 2 /* LightSaber */
#define SABER_K 3 /* Saber */
// #define SABER_K 4 /* FireSaber */

/* Don't change anything below this line */
#if SABER_K == 2
#define SABER_MU 10
#define SABER_ET 3
#define Saber_type 1
#elif SABER_K == 3
#define SABER_MU 8
#define SABER_ET 4
#define Saber_type 2
#elif SABER_K == 4
#define SABER_MU 6
#define SABER_ET 6
#define Saber_type 3
#endif

#define SABER_EQ 13
#define SABER_EP 10

#define SABER_N 256
#define SABER_Q 8192 //2^13
#define SABER_P 1024

#define SABER_SEEDBYTES 32
#define SABER_NOISESEEDBYTES 32
#define SABER_COINBYTES 32
#define SABER_KEYBYTES 32

#define SABER_HASHBYTES 32

#define SABER_POLYBYTES 416 //13*256/8

#define SABER_POLYVECBYTES (SABER_K * SABER_POLYBYTES)

#define SABER_POLYVECCOMPRESSEDBYTES (SABER_K * 320) //10*256/8 NOTE : changed till here due to parameter adaptation

#define SABER_CIPHERTEXTBYTES (SABER_POLYVECCOMPRESSEDBYTES)

#define SABER_SCALEBYTES_KEM ((SABER_ET)*SABER_N / 8)

#define SABER_INDCPA_PUBLICKEYBYTES (SABER_POLYVECCOMPRESSEDBYTES + SABER_SEEDBYTES)
#define SABER_INDCPA_SECRETKEYBYTES (SABER_POLYVECBYTES)

#define SABER_PUBLICKEYBYTES (SABER_INDCPA_PUBLICKEYBYTES)

#define SABER_SECRETKEYBYTES (SABER_INDCPA_SECRETKEYBYTES + SABER_INDCPA_PUBLICKEYBYTES + SABER_HASHBYTES + SABER_KEYBYTES)

#define SABER_BYTES_CCA_DEC (SABER_POLYVECCOMPRESSEDBYTES + SABER_SCALEBYTES_KEM) /* Second part is for Targhi-Unruh */

#endif