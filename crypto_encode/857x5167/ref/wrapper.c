#include "crypto_int16.h"
#include "crypto_encode.h"
#include "Encode.h"
#define int16 crypto_int16
#define uint16 crypto_uint16

#define q 5167
#define q12 2583
#define p 857

void crypto_encode(unsigned char *s,const void *v)
{
  const int16 *r = v;
  uint16 R[p],M[p];
  int i;
  
  for (i = 0;i < p;++i) R[i] = r[i]+q12;
  for (i = 0;i < p;++i) M[i] = q;
  Encode(s,R,M,p);
}
