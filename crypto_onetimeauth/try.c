/*
 * crypto_onetimeauth/try.c version 20200406
 * D. J. Bernstein
 * Public domain.
 * Auto-generated by trygen.py; do not edit.
 */

#include "crypto_onetimeauth.h"
#include "try.h"

const char *primitiveimplementation = crypto_onetimeauth_IMPLEMENTATION;

#define TUNE_BYTES 1536
#ifdef SMALL
#define MAXTEST_BYTES 128
#else
#define MAXTEST_BYTES 4096
#endif
#ifdef SMALL
#define LOOPS 4096
#else
#define LOOPS 32768
#endif

static unsigned char *h;
static unsigned char *m;
static unsigned char *k;
static unsigned char *h2;
static unsigned char *m2;
static unsigned char *k2;
#define hlen crypto_onetimeauth_BYTES
unsigned long long mlen;
#define klen crypto_onetimeauth_KEYBYTES

void preallocate(void)
{
}

void allocate(void)
{
  unsigned long long alloclen = 0;
  if (alloclen < TUNE_BYTES) alloclen = TUNE_BYTES;
  if (alloclen < MAXTEST_BYTES) alloclen = MAXTEST_BYTES;
  if (alloclen < crypto_onetimeauth_BYTES) alloclen = crypto_onetimeauth_BYTES;
  if (alloclen < crypto_onetimeauth_KEYBYTES) alloclen = crypto_onetimeauth_KEYBYTES;
  h = alignedcalloc(alloclen);
  m = alignedcalloc(alloclen);
  k = alignedcalloc(alloclen);
  h2 = alignedcalloc(alloclen);
  m2 = alignedcalloc(alloclen);
  k2 = alignedcalloc(alloclen);
}

void predoit(void)
{
}

void doit(void)
{
  crypto_onetimeauth(h,m,TUNE_BYTES,k);
  crypto_onetimeauth_verify(h,m,TUNE_BYTES,k);
}

void test(void)
{
  unsigned long long loop;
  
  for (loop = 0;loop < LOOPS;++loop) {
    mlen = myrandom() % (MAXTEST_BYTES + 1);
    
    output_prepare(h2,h,hlen);
    input_prepare(m2,m,mlen);
    input_prepare(k2,k,klen);
    if (crypto_onetimeauth(h,m,mlen,k) != 0) fail("crypto_onetimeauth returns nonzero");
    checksum(h,hlen);
    output_compare(h2,h,hlen,"crypto_onetimeauth");
    input_compare(m2,m,mlen,"crypto_onetimeauth");
    input_compare(k2,k,klen,"crypto_onetimeauth");
    
    double_canary(h2,h,hlen);
    double_canary(m2,m,mlen);
    double_canary(k2,k,klen);
    if (crypto_onetimeauth(h2,m2,mlen,k2) != 0) fail("crypto_onetimeauth returns nonzero");
    if (memcmp(h2,h,hlen) != 0) fail("crypto_onetimeauth is nondeterministic");
    
    double_canary(h2,h,hlen);
    double_canary(m2,m,mlen);
    double_canary(k2,k,klen);
    if (crypto_onetimeauth(m2,m2,mlen,k) != 0) fail("crypto_onetimeauth with m=h overlap returns nonzero");
    if (memcmp(m2,h,hlen) != 0) fail("crypto_onetimeauth does not handle m=h overlap");
    memcpy(m2,m,mlen);
    if (crypto_onetimeauth(k2,m,mlen,k2) != 0) fail("crypto_onetimeauth with k=h overlap returns nonzero");
    if (memcmp(k2,h,hlen) != 0) fail("crypto_onetimeauth does not handle k=h overlap");
    memcpy(k2,k,klen);
    
    memcpy(h2,h,hlen);
    double_canary(h2,h,hlen);
    memcpy(m2,m,mlen);
    double_canary(m2,m,mlen);
    memcpy(k2,k,klen);
    double_canary(k2,k,klen);
    if (crypto_onetimeauth_verify(h,m,mlen,k) != 0) fail("crypto_onetimeauth_verify returns nonzero");
    input_compare(h2,h,hlen,"crypto_onetimeauth_verify");
    input_compare(m2,m,mlen,"crypto_onetimeauth_verify");
    input_compare(k2,k,klen,"crypto_onetimeauth_verify");
    
    double_canary(h2,h,hlen);
    double_canary(m2,m,mlen);
    double_canary(k2,k,klen);
    if (crypto_onetimeauth_verify(h2,m2,mlen,k2) != 0) fail("crypto_onetimeauth_verify returns nonzero");
    
    h[myrandom() % hlen] += 1 + (myrandom() % 255);
    if (crypto_onetimeauth_verify(h,m,mlen,k) == 0)
      if (memcmp(h2,h,hlen) != 0)
        fail("crypto_onetimeauth_verify accepts modified authenticators");
    h[myrandom() % hlen] += 1 + (myrandom() % 255);
    if (crypto_onetimeauth_verify(h,m,mlen,k) == 0)
      if (memcmp(h2,h,hlen) != 0)
        fail("crypto_onetimeauth_verify accepts modified authenticators");
    h[myrandom() % hlen] += 1 + (myrandom() % 255);
    if (crypto_onetimeauth_verify(h,m,mlen,k) == 0)
      if (memcmp(h2,h,hlen) != 0)
        fail("crypto_onetimeauth_verify accepts modified authenticators");
  }
#include "test-more.inc"
}
