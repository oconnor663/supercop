/**
 * @file kem.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief An implementation of LOTUS-KEM
 */

#include "type.h"
#include "param.h"
#include "crypto.h"
#include "cpa-pke.h"
#include "pack.h"
#include "sampler.h"
#include "randombytes.h"
#include "crypto_hash_sha512.h"
#include "crypto_kem.h"

#include <string.h>

int util_cmp_const(const void *a, const void *b, const size_t size);
void xor_ss(U8 *r, const U8 *a, const U8 *b);


/**
 * @brief LOTUS KeyGen algorithm
 * @param[out] pk a public key, contains matrices A and P
 * @param[out] sk a secret key, contains matrix S, and public key pk
 * @return 0 as success (and always)
 * @note pk is required in the decapsulation process, so embedded into sk in this implementation.
 */
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk){
  U16 pkt[_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT],
    skt[_LOTUS_LWE_DIM * _LOTUS_LWE_PT];
  
  /* Generate key pair */
  lotus_cpa_pke_keypair(pkt, skt);

  /* Pack key pair */
  pack_pk(pk, pkt);
  pack_sk(sk, skt);

  /* Append public key to secret key */
  memcpy(sk + _LOTUS_PACK_PRVKEY_BYTES, pk, _LOTUS_PACK_PUBKEY_BYTES);

  /* Cleanse memory positions stored secrets */
  OPENSSL_cleanse(skt, _LOTUS_LWE_DIM * _LOTUS_LWE_PT * sizeof(U16));

  return 0;
}

/**
 * @brief LOTUS-KEM Encapsulation algorithm
 * @param[out] ct a ciphertext corresponding to the shared secret ss
 * @param[out] ss a shared secret, randomly generated in this function
 * @param[in] pk a public key
 * @return 0 as success (and always)
 */
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk){
  U8 sigma[_LOTUS_LWE_PT_BYTES * 2 + 1], digest[_LOTUS_HASH_DIGEST_BYTES];
  U16 pkt[_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT],
    ctt[_LOTUS_LWE_DIM + _LOTUS_LWE_PT];
  /* accessor */
  U8 *csym = ct + _LOTUS_PACK_CT_BYTES;

  /* Unpack public key */
  unpack_pk(pkt, pk);
  
  /* Pick shread secret ss randomly. */
  randombytes(ss, _LOTUS_LWE_PT_BYTES);
  
  /* Encrypt shared secret ss by csym = H(sigma) xor ss. */
  randombytes(sigma, _LOTUS_LWE_PT_BYTES);
  sigma[_LOTUS_LWE_PT_BYTES] = _LOTUS_HASH_FLAG_G;
  crypto_hash_sha512(digest, sigma, _LOTUS_LWE_PT_BYTES + 1);
  xor_ss(csym, digest, ss);

  /* compute seed by H(sigma, csym) and set the seed to discrete gaussian sampler */
  memcpy(sigma + _LOTUS_LWE_PT_BYTES, csym, _LOTUS_LWE_PT_BYTES);
  sigma[_LOTUS_LWE_PT_BYTES * 2] = _LOTUS_HASH_FLAG_H;
  crypto_hash_sha512(digest, sigma, _LOTUS_LWE_PT_BYTES * 2 + 1);

  /* Set the digest as seed and encrypt */
  sampler_set_seed(digest);
  lotus_cpa_pke_enc(ctt, sigma, pkt);

  /* Pack ciphertext */
  pack_ct(ct, ctt);

  /* Cleanse memory positions stored secrets */
  OPENSSL_cleanse(sigma, _LOTUS_LWE_PT_BYTES * 2 + 1);
  OPENSSL_cleanse(digest, _LOTUS_HASH_DIGEST_BYTES);
  
  return 0;
}

/**
 * @brief LOTUS-KEM Decapculation algorithm
 * @param[out] ss a shared secret corresponding to the ciphertext ct
 * @param[in] ct a ciphertext to be decrypted
 * @param[in] sk a secret key
 * @return 0 if decrypted successfully, otherwise -1
 * @note Since current API does not allow to have public key as an argument, we embed it into secret key in this implementation.
 */
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk){
  U8 sigma[_LOTUS_LWE_PT_BYTES * 2 + 1], digest[_LOTUS_HASH_DIGEST_BYTES], ss_[_LOTUS_LWE_PT_BYTES * 2];
  U16 cc[_LOTUS_LWE_DIM + _LOTUS_LWE_PT],
    ctt[_LOTUS_LWE_DIM + _LOTUS_LWE_PT],
    pkt[_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT],
    skt[_LOTUS_LWE_DIM * _LOTUS_LWE_PT];
  /* accessors */
  const U8 *csym = ct + _LOTUS_PACK_CT_BYTES;

  /* Unpack key pair, and ciphertext */
  unpack_sk(skt, sk);
  unpack_pk(pkt, sk + _LOTUS_PACK_PRVKEY_BYTES);
  unpack_ct(ctt, ct);

  /* Decrypt the ciphertext */
  lotus_cpa_pke_dec(sigma, ctt, skt);

  /* Extract a seed from sigma. */
  memcpy(sigma + _LOTUS_LWE_PT_BYTES, csym, _LOTUS_LWE_PT_BYTES);
  sigma[_LOTUS_LWE_PT_BYTES * 2] = _LOTUS_HASH_FLAG_H;
  crypto_hash_sha512(digest, sigma, _LOTUS_LWE_PT_BYTES * 2 + 1);

  /* Validate the ciphertext. */
  sampler_set_seed(digest);
  lotus_cpa_pke_enc(cc, sigma, pkt);

  /* Decrypt shared secret ss by ss = H(sigma) xor csym. */
  sigma[_LOTUS_LWE_PT_BYTES] = _LOTUS_HASH_FLAG_G;
  crypto_hash_sha512(digest, sigma, _LOTUS_LWE_PT_BYTES + 1);
  xor_ss(ss_, csym, digest);
  memset(ss_ + _LOTUS_LWE_PT_BYTES, 0, _LOTUS_LWE_PT_BYTES);

  /* Cleanse memory positions stored secrets */
  OPENSSL_cleanse(sigma, _LOTUS_LWE_PT_BYTES * 2 + 1);
  OPENSSL_cleanse(digest, _LOTUS_HASH_DIGEST_BYTES);
  OPENSSL_cleanse(skt, _LOTUS_LWE_DIM * _LOTUS_LWE_PT * sizeof(U16));

  int r = util_cmp_const(ctt, cc, (_LOTUS_LWE_DIM + _LOTUS_LWE_PT) * sizeof(U16)) > 0;
  memcpy(ss, ss_ + r * _LOTUS_LWE_PT_BYTES, _LOTUS_LWE_PT_BYTES);
  OPENSSL_cleanse(ctt, (_LOTUS_LWE_DIM + _LOTUS_LWE_PT) * sizeof(U16));
  OPENSSL_cleanse(ss_, _LOTUS_LWE_PT_BYTES * 2);
  return -r;
}


/**
 * @brief constant-time comparison, from https://cryptocoding.net/index.php/Coding_rules
 */
int util_cmp_const(const void *a, const void *b, const size_t size) 
{
  const unsigned char *_a = (const unsigned char *) a;
  const unsigned char *_b = (const unsigned char *) b;
  unsigned char result = 0;
  size_t i;
 
  for (i = 0; i < size; i++) {
    result |= _a[i] ^ _b[i];
  }
  return result; /* returns 0 if equal, nonzero otherwise */
}

/**
 * @brief xoring two array, as r = a ^ b
 */
void xor_ss(U8 *r, const U8 *a, const U8 *b){
  int i;
  for(i = 0; i < _LOTUS_LWE_PT_BYTES; ++i) r[i] = a[i] ^ b[i];
}
