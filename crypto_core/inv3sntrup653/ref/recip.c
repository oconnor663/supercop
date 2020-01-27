#include "crypto_core.h"

#include "params.h"

#include "crypto_int8.h"
#include "crypto_int16.h"
#include "crypto_int32.h"
#include "crypto_uint16.h"
#include "crypto_uint32.h"
#define int8 crypto_int8
#define int16 crypto_int16
#define int32 crypto_int32
#define uint16 crypto_uint16
#define uint32 crypto_uint32

/* ----- masks */

/* return -1 if x!=0; else return 0 */
static int int16_nonzero_mask(int16 x)
{
  uint16 u = x; /* 0, else 1...65535 */
  uint32 v = u; /* 0, else 1...65535 */
  v = -v; /* 0, else 2^32-65535...2^32-1 */
  v >>= 31; /* 0, else 1 */
  return -v; /* 0, else -1 */
}

/* return -1 if x<0; otherwise return 0 */
static int int16_negative_mask(int16 x)
{
  uint16 u = x;
  u >>= 15;
  return -(int) u;
  /* alternative with gcc -fwrapv: */
  /* x>>15 compiles to CPU's arithmetic right shift */
}

/* ----- arithmetic mod 3 */

typedef int8 small;
/* F3 is always represented as -1,0,1 */

/* works for -16384 <= x < 16384 */
static small F3_freeze(int16 x)
{
  return x-3*((10923*x+16384)>>15);
}

/* byte p of output is 0 if recip succeeded; else -1 */
int crypto_core(unsigned char *outbytes,const unsigned char *inbytes,const unsigned char *kbytes,const unsigned char *cbytes)
{ 
  small *out = (void *) outbytes;
  small *in = (void *) inbytes;
  small f[p+1],g[p+1],v[p+1],r[p+1];
  int i,loop,delta;
  int sign,swap,t;
  
  for (i = 0;i < p+1;++i) v[i] = 0;
  for (i = 0;i < p+1;++i) r[i] = 0;
  r[0] = 1;
  for (i = 0;i < p;++i) f[i] = 0;
  f[0] = 1; f[p-1] = f[p] = -1;
  for (i = 0;i < p;++i) {
    small i1 = in[i]&1;
    g[p-1-i] = i1-(in[i]&(i1<<1));
  }
  g[p] = 0;
    
  delta = 1; 

  for (loop = 0;loop < 2*p-1;++loop) {
    for (i = p;i > 0;--i) v[i] = v[i-1];
    v[0] = 0;
    
    sign = -g[0]*f[0];
    swap = int16_negative_mask(-delta) & int16_nonzero_mask(g[0]);
    delta ^= swap&(delta^-delta);
    delta += 1;
    
    for (i = 0;i < p+1;++i) {
      t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
      t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
    }
  
    for (i = 0;i < p+1;++i) g[i] = F3_freeze(g[i]+sign*f[i]);
    for (i = 0;i < p+1;++i) r[i] = F3_freeze(r[i]+sign*v[i]);

    for (i = 0;i < p;++i) g[i] = g[i+1];
    g[p] = 0;
  }
  
  sign = f[0];
  for (i = 0;i < p;++i) out[i] = sign*v[p-1-i];
  
  out[p] = int16_nonzero_mask(delta);
  return 0;
} 
