/**
 * \file api.h
 * \brief NIST KEM API used by the LOCKER scheme
 */


#ifndef API_H
#define API_H


#define CRYPTO_ALGNAME "LOCKER-II"

#define CRYPTO_SECRETKEYBYTES 1119
#define CRYPTO_PUBLICKEYBYTES 1079
#define CRYPTO_BYTES 64
#define CRYPTO_CIPHERTEXTBYTES 1207


int crypto_kem_keypair(unsigned char* pk, unsigned char* sk);
int crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk);
int crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk);

#endif

