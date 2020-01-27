/**
 * @file crypto.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 */

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include <openssl/evp.h>
/* hash function */
#define _LOTUS_HASH_DIGEST_BYTES 64
#define _LOTUS_HASH_FLAG_G 0x01 /* flag for hash function G */
#define _LOTUS_HASH_FLAG_H 0x02 /* flag for hash function H */

/* symmetric key encryption */
typedef EVP_CIPHER_CTX LOTUS_SYMENC_ctx;
#define _LOTUS_SYMENC_TYPE EVP_aes_128_ctr() 
#define _LOTUS_SYMENC_BLOCK_BYTES 16
#define _LOTUS_SYMENC_KEY_BYTES 16

/* size for internal randomness pool */
#define _LOTUS_RANDOMNESS_BYTES 512
#define _LOTUS_RANDOMNESS_BITS (_LOTUS_RANDOMNESS_BYTES * 8)

/* bytes of seed */
#define _LOTUS_SEED_BYTES _LOTUS_SYMENC_KEY_BYTES

LOTUS_SYMENC_ctx *crypto_symenc_keysetup(LOTUS_SYMENC_ctx *ctx, const unsigned char *key);
void crypto_symenc_keystream(LOTUS_SYMENC_ctx *ctx, unsigned char *stream, const unsigned int len);
int crypto_symenc_encrypt(unsigned char *c, const unsigned char *m, const unsigned int mlen, const unsigned char *k);
int crypto_symenc_decrypt(unsigned char *m, const unsigned char *c, const unsigned int clen, const unsigned char *k);

#endif
