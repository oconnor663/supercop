/**
 * @file api.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief API header for submission of NIST PQC standardization
 */

#ifndef _API_H
#define _API_H

#define CRYPTO_PUBLICKEYBYTES 1470976
#define CRYPTO_SECRETKEYBYTES 1630720
#define CRYPTO_BYTES 1768

#define CRYPTO_ALGNAME "LOTUS256PKE"
#define CRYPTO_VERSION "1.0a"

int crypto_encrypt_keypair(unsigned char *pk, unsigned char *sk);
int crypto_encrypt(unsigned char *c, unsigned long long *clen, const unsigned char *m, const unsigned long long mlen, const unsigned char *pk);
int crypto_encrypt_open(unsigned char *m, unsigned long long *mlen, const unsigned char *c, unsigned long long clen, const unsigned char *sk);

#endif
