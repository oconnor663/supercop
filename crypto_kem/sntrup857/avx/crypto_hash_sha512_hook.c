#include "crypto_hash_sha512.h"

int crypto_kem_sntrup857_avx_crypto_hash_sha512(unsigned char *v0,const unsigned char *v1,unsigned long long v2)
{
  return crypto_hash_sha512(v0,v1,v2);
}
