#include "crypto_core_mult3sntrup761.h"

int crypto_kem_sntrup761_avx_crypto_core_mult3sntrup761(unsigned char *v0,const unsigned char *v1,const unsigned char *v2,const unsigned char *v3)
{
  return crypto_core_mult3sntrup761(v0,v1,v2,v3);
}
