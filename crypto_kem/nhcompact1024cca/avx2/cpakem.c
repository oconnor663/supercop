#include <string.h>
#include "api.h"
#include "cpapke.h"
#include "params.h"
#include "randombytes.h"
#include "fips202.h"
#include "verify.h"

 
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
  cpapke_keypair(pk, sk);                                                        /* First put the actual secret key into sk */

  return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
  unsigned char buf[2*NEWHOPECMPCT_SYMBYTES];

  randombytes(buf,NEWHOPECMPCT_SYMBYTES);
  
  shake256(buf,2*NEWHOPECMPCT_SYMBYTES,buf,NEWHOPECMPCT_SYMBYTES);                         /* Don't release system RNG output */

  cpapke_enc(ct, buf, pk, buf+NEWHOPECMPCT_SYMBYTES);                                 /* coins are in buf+NEWHOPE_SYMBYTES */

  shake256(ss, NEWHOPECMPCT_SYMBYTES, buf, NEWHOPECMPCT_SYMBYTES);                         /* hash pre-k to ss */
  return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
  cpapke_dec(ss, ct, sk);
  
  shake256(ss, NEWHOPECMPCT_SYMBYTES, ss, NEWHOPECMPCT_SYMBYTES);                          /* hash pre-k to ss */

  return 0;
}
