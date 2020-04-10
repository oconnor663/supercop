/*
 * crypto_box/try.c version 20200406
 * D. J. Bernstein
 * Public domain.
 * Auto-generated by trygen.py; do not edit.
 */

#include "crypto_box.h"
#include "try.h"
#include "randombytes.h"

const char *primitiveimplementation = crypto_box_IMPLEMENTATION;

#define TUNE_BYTES 1536
#ifdef SMALL
#define MAXTEST_BYTES 128
#else
#define MAXTEST_BYTES 4096
#endif
#ifdef SMALL
#define LOOPS 512
#else
#define LOOPS 4096
#endif

static unsigned char *a;
static unsigned char *b;
static unsigned char *y;
static unsigned char *z;
static unsigned char *e;
static unsigned char *f;
static unsigned char *n;
static unsigned char *m;
static unsigned char *c;
static unsigned char *d;
static unsigned char *t;
static unsigned char *a2;
static unsigned char *b2;
static unsigned char *y2;
static unsigned char *z2;
static unsigned char *e2;
static unsigned char *f2;
static unsigned char *n2;
static unsigned char *m2;
static unsigned char *c2;
static unsigned char *d2;
static unsigned char *t2;
#define alen crypto_box_SECRETKEYBYTES
#define blen crypto_box_SECRETKEYBYTES
#define ylen crypto_box_PUBLICKEYBYTES
#define zlen crypto_box_PUBLICKEYBYTES
#define elen crypto_box_BEFORENMBYTES
#define flen crypto_box_BEFORENMBYTES
#define nlen crypto_box_NONCEBYTES
unsigned long long mlen;
unsigned long long clen;
unsigned long long dlen;
unsigned long long tlen;

void preallocate(void)
{
}

void allocate(void)
{
  unsigned long long alloclen = 0;
  if (alloclen < TUNE_BYTES) alloclen = TUNE_BYTES;
  if (alloclen < MAXTEST_BYTES) alloclen = MAXTEST_BYTES;
  if (alloclen < crypto_box_SECRETKEYBYTES) alloclen = crypto_box_SECRETKEYBYTES;
  if (alloclen < crypto_box_SECRETKEYBYTES) alloclen = crypto_box_SECRETKEYBYTES;
  if (alloclen < crypto_box_PUBLICKEYBYTES) alloclen = crypto_box_PUBLICKEYBYTES;
  if (alloclen < crypto_box_PUBLICKEYBYTES) alloclen = crypto_box_PUBLICKEYBYTES;
  if (alloclen < crypto_box_BEFORENMBYTES) alloclen = crypto_box_BEFORENMBYTES;
  if (alloclen < crypto_box_BEFORENMBYTES) alloclen = crypto_box_BEFORENMBYTES;
  if (alloclen < crypto_box_NONCEBYTES) alloclen = crypto_box_NONCEBYTES;
  a = alignedcalloc(alloclen);
  b = alignedcalloc(alloclen);
  y = alignedcalloc(alloclen);
  z = alignedcalloc(alloclen);
  e = alignedcalloc(alloclen);
  f = alignedcalloc(alloclen);
  n = alignedcalloc(alloclen);
  m = alignedcalloc(alloclen);
  c = alignedcalloc(alloclen);
  d = alignedcalloc(alloclen);
  t = alignedcalloc(alloclen);
  a2 = alignedcalloc(alloclen);
  b2 = alignedcalloc(alloclen);
  y2 = alignedcalloc(alloclen);
  z2 = alignedcalloc(alloclen);
  e2 = alignedcalloc(alloclen);
  f2 = alignedcalloc(alloclen);
  n2 = alignedcalloc(alloclen);
  m2 = alignedcalloc(alloclen);
  c2 = alignedcalloc(alloclen);
  d2 = alignedcalloc(alloclen);
  t2 = alignedcalloc(alloclen);
}

void predoit(void)
{
  crypto_box_keypair(y,a);
  crypto_box_keypair(z,b);
  randombytes(m,mlen);
  randombytes(n,nlen);
}

void doit(void)
{
  crypto_box(c,m,TUNE_BYTES + crypto_box_ZEROBYTES,n,y,b);
  crypto_box_open(t,c,TUNE_BYTES + crypto_box_ZEROBYTES,n,z,a);
}

void test(void)
{
  unsigned long long j;
  unsigned long long loop;
  
  for (loop = 0;loop < LOOPS;++loop) {
    mlen = myrandom() % (MAXTEST_BYTES + 1);
    clen = mlen;
    dlen = mlen;
    tlen = mlen;
    if (mlen < crypto_box_ZEROBYTES) continue;
    
    output_prepare(y2,y,ylen);
    output_prepare(a2,a,alen);
    if (crypto_box_keypair(y,a) != 0) fail("crypto_box_keypair returns nonzero");
    checksum(y,ylen);
    checksum(a,alen);
    output_compare(y2,y,ylen,"crypto_box_keypair");
    output_compare(a2,a,alen,"crypto_box_keypair");
    
    output_prepare(z2,z,zlen);
    output_prepare(b2,b,blen);
    if (crypto_box_keypair(z,b) != 0) fail("crypto_box_keypair returns nonzero");
    checksum(z,zlen);
    checksum(b,blen);
    output_compare(z2,z,zlen,"crypto_box_keypair");
    output_compare(b2,b,blen,"crypto_box_keypair");
    
    output_prepare(c2,c,clen);
    input_prepare(m2,m,mlen);
    input_prepare(n2,n,nlen);
    memcpy(z2,z,zlen);
    double_canary(z2,z,zlen);
    memcpy(a2,a,alen);
    double_canary(a2,a,alen);
    for (j = 0;j < crypto_box_ZEROBYTES;++j) m[j] = 0;
    for (j = 0;j < crypto_box_ZEROBYTES;++j) m2[j] = 0;
    if (crypto_box(c,m,mlen,n,z,a) != 0) fail("crypto_box returns nonzero");
    for (j = 0;j < crypto_box_BOXZEROBYTES;++j)
      if (c[j] != 0) fail("crypto_box does not clear extra bytes");
    checksum(c,clen);
    output_compare(c2,c,clen,"crypto_box");
    input_compare(m2,m,mlen,"crypto_box");
    input_compare(n2,n,nlen,"crypto_box");
    input_compare(z2,z,zlen,"crypto_box");
    input_compare(a2,a,alen,"crypto_box");
    
    double_canary(c2,c,clen);
    double_canary(m2,m,mlen);
    double_canary(n2,n,nlen);
    double_canary(z2,z,zlen);
    double_canary(a2,a,alen);
    if (crypto_box(c2,m2,mlen,n2,z2,a2) != 0) fail("crypto_box returns nonzero");
    if (memcmp(c2,c,clen) != 0) fail("crypto_box is nondeterministic");
    
    double_canary(c2,c,clen);
    double_canary(m2,m,mlen);
    double_canary(n2,n,nlen);
    double_canary(z2,z,zlen);
    double_canary(a2,a,alen);
    if (crypto_box(m2,m2,mlen,n,z,a) != 0) fail("crypto_box with m=c overlap returns nonzero");
    if (memcmp(m2,c,clen) != 0) fail("crypto_box does not handle m=c overlap");
    memcpy(m2,m,mlen);
    if (crypto_box(n2,m,mlen,n2,z,a) != 0) fail("crypto_box with n=c overlap returns nonzero");
    if (memcmp(n2,c,clen) != 0) fail("crypto_box does not handle n=c overlap");
    memcpy(n2,n,nlen);
    if (crypto_box(z2,m,mlen,n,z2,a) != 0) fail("crypto_box with z=c overlap returns nonzero");
    if (memcmp(z2,c,clen) != 0) fail("crypto_box does not handle z=c overlap");
    memcpy(z2,z,zlen);
    if (crypto_box(a2,m,mlen,n,z,a2) != 0) fail("crypto_box with a=c overlap returns nonzero");
    if (memcmp(a2,c,clen) != 0) fail("crypto_box does not handle a=c overlap");
    memcpy(a2,a,alen);
    
    output_prepare(t2,t,tlen);
    memcpy(c2,c,clen);
    double_canary(c2,c,clen);
    memcpy(n2,n,nlen);
    double_canary(n2,n,nlen);
    memcpy(y2,y,ylen);
    double_canary(y2,y,ylen);
    memcpy(b2,b,blen);
    double_canary(b2,b,blen);
    if (crypto_box_open(t,c,clen,n,y,b) != 0) fail("crypto_box_open returns nonzero");
    if (memcmp(t,m,mlen) != 0) fail("crypto_box_open does not match m");
    checksum(t,tlen);
    output_compare(t2,t,tlen,"crypto_box_open");
    input_compare(c2,c,clen,"crypto_box_open");
    input_compare(n2,n,nlen,"crypto_box_open");
    input_compare(y2,y,ylen,"crypto_box_open");
    input_compare(b2,b,blen,"crypto_box_open");
    
    double_canary(t2,t,tlen);
    double_canary(c2,c,clen);
    double_canary(n2,n,nlen);
    double_canary(y2,y,ylen);
    double_canary(b2,b,blen);
    if (crypto_box_open(t2,c2,clen,n2,y2,b2) != 0) fail("crypto_box_open returns nonzero");
    if (memcmp(t2,t,tlen) != 0) fail("crypto_box_open is nondeterministic");
    
    double_canary(t2,t,tlen);
    double_canary(c2,c,clen);
    double_canary(n2,n,nlen);
    double_canary(y2,y,ylen);
    double_canary(b2,b,blen);
    if (crypto_box_open(c2,c2,clen,n,y,b) != 0) fail("crypto_box_open with c=t overlap returns nonzero");
    if (memcmp(c2,t,tlen) != 0) fail("crypto_box_open does not handle c=t overlap");
    memcpy(c2,c,clen);
    if (crypto_box_open(n2,c,clen,n2,y,b) != 0) fail("crypto_box_open with n=t overlap returns nonzero");
    if (memcmp(n2,t,tlen) != 0) fail("crypto_box_open does not handle n=t overlap");
    memcpy(n2,n,nlen);
    if (crypto_box_open(y2,c,clen,n,y2,b) != 0) fail("crypto_box_open with y=t overlap returns nonzero");
    if (memcmp(y2,t,tlen) != 0) fail("crypto_box_open does not handle y=t overlap");
    memcpy(y2,y,ylen);
    if (crypto_box_open(b2,c,clen,n,y,b2) != 0) fail("crypto_box_open with b=t overlap returns nonzero");
    if (memcmp(b2,t,tlen) != 0) fail("crypto_box_open does not handle b=t overlap");
    memcpy(b2,b,blen);
    
    output_prepare(e2,e,elen);
    memcpy(z2,z,zlen);
    double_canary(z2,z,zlen);
    memcpy(a2,a,alen);
    double_canary(a2,a,alen);
    if (crypto_box_beforenm(e,z,a) != 0) fail("crypto_box_beforenm returns nonzero");
    checksum(e,elen);
    output_compare(e2,e,elen,"crypto_box_beforenm");
    input_compare(z2,z,zlen,"crypto_box_beforenm");
    input_compare(a2,a,alen,"crypto_box_beforenm");
    
    double_canary(e2,e,elen);
    double_canary(z2,z,zlen);
    double_canary(a2,a,alen);
    if (crypto_box_beforenm(e2,z2,a2) != 0) fail("crypto_box_beforenm returns nonzero");
    if (memcmp(e2,e,elen) != 0) fail("crypto_box_beforenm is nondeterministic");
    
    double_canary(e2,e,elen);
    double_canary(z2,z,zlen);
    double_canary(a2,a,alen);
    if (crypto_box_beforenm(z2,z2,a) != 0) fail("crypto_box_beforenm with z=e overlap returns nonzero");
    if (memcmp(z2,e,elen) != 0) fail("crypto_box_beforenm does not handle z=e overlap");
    memcpy(z2,z,zlen);
    if (crypto_box_beforenm(a2,z,a2) != 0) fail("crypto_box_beforenm with a=e overlap returns nonzero");
    if (memcmp(a2,e,elen) != 0) fail("crypto_box_beforenm does not handle a=e overlap");
    memcpy(a2,a,alen);
    
    output_prepare(f2,f,flen);
    memcpy(y2,y,ylen);
    double_canary(y2,y,ylen);
    memcpy(b2,b,blen);
    double_canary(b2,b,blen);
    if (crypto_box_beforenm(f,y,b) != 0) fail("crypto_box_beforenm returns nonzero");
    checksum(f,flen);
    output_compare(f2,f,flen,"crypto_box_beforenm");
    input_compare(y2,y,ylen,"crypto_box_beforenm");
    input_compare(b2,b,blen,"crypto_box_beforenm");
    
    double_canary(f2,f,flen);
    double_canary(y2,y,ylen);
    double_canary(b2,b,blen);
    if (crypto_box_beforenm(f2,y2,b2) != 0) fail("crypto_box_beforenm returns nonzero");
    if (memcmp(f2,f,flen) != 0) fail("crypto_box_beforenm is nondeterministic");
    
    double_canary(f2,f,flen);
    double_canary(y2,y,ylen);
    double_canary(b2,b,blen);
    if (crypto_box_beforenm(y2,y2,b) != 0) fail("crypto_box_beforenm with y=f overlap returns nonzero");
    if (memcmp(y2,f,flen) != 0) fail("crypto_box_beforenm does not handle y=f overlap");
    memcpy(y2,y,ylen);
    if (crypto_box_beforenm(b2,y,b2) != 0) fail("crypto_box_beforenm with b=f overlap returns nonzero");
    if (memcmp(b2,f,flen) != 0) fail("crypto_box_beforenm does not handle b=f overlap");
    memcpy(b2,b,blen);
    
    output_prepare(d2,d,dlen);
    memcpy(m2,m,mlen);
    double_canary(m2,m,mlen);
    memcpy(n2,n,nlen);
    double_canary(n2,n,nlen);
    memcpy(e2,e,elen);
    double_canary(e2,e,elen);
    if (crypto_box_afternm(d,m,mlen,n,e) != 0) fail("crypto_box_afternm returns nonzero");
    if (memcmp(d,c,clen) != 0) fail("crypto_box_afternm does not match c");
    checksum(d,dlen);
    output_compare(d2,d,dlen,"crypto_box_afternm");
    input_compare(m2,m,mlen,"crypto_box_afternm");
    input_compare(n2,n,nlen,"crypto_box_afternm");
    input_compare(e2,e,elen,"crypto_box_afternm");
    
    double_canary(d2,d,dlen);
    double_canary(m2,m,mlen);
    double_canary(n2,n,nlen);
    double_canary(e2,e,elen);
    if (crypto_box_afternm(d2,m2,mlen,n2,e2) != 0) fail("crypto_box_afternm returns nonzero");
    if (memcmp(d2,d,dlen) != 0) fail("crypto_box_afternm is nondeterministic");
    
    double_canary(d2,d,dlen);
    double_canary(m2,m,mlen);
    double_canary(n2,n,nlen);
    double_canary(e2,e,elen);
    if (crypto_box_afternm(m2,m2,mlen,n,e) != 0) fail("crypto_box_afternm with m=d overlap returns nonzero");
    if (memcmp(m2,d,dlen) != 0) fail("crypto_box_afternm does not handle m=d overlap");
    memcpy(m2,m,mlen);
    if (crypto_box_afternm(n2,m,mlen,n2,e) != 0) fail("crypto_box_afternm with n=d overlap returns nonzero");
    if (memcmp(n2,d,dlen) != 0) fail("crypto_box_afternm does not handle n=d overlap");
    memcpy(n2,n,nlen);
    if (crypto_box_afternm(e2,m,mlen,n,e2) != 0) fail("crypto_box_afternm with e=d overlap returns nonzero");
    if (memcmp(e2,d,dlen) != 0) fail("crypto_box_afternm does not handle e=d overlap");
    memcpy(e2,e,elen);
    
    output_prepare(t2,t,tlen);
    memcpy(d2,d,dlen);
    double_canary(d2,d,dlen);
    memcpy(n2,n,nlen);
    double_canary(n2,n,nlen);
    memcpy(f2,f,flen);
    double_canary(f2,f,flen);
    if (crypto_box_open_afternm(t,d,dlen,n,f) != 0) fail("crypto_box_open_afternm returns nonzero");
    if (memcmp(t,m,mlen) != 0) fail("crypto_box_open_afternm does not match m");
    checksum(t,tlen);
    output_compare(t2,t,tlen,"crypto_box_open_afternm");
    input_compare(d2,d,dlen,"crypto_box_open_afternm");
    input_compare(n2,n,nlen,"crypto_box_open_afternm");
    input_compare(f2,f,flen,"crypto_box_open_afternm");
    
    double_canary(t2,t,tlen);
    double_canary(d2,d,dlen);
    double_canary(n2,n,nlen);
    double_canary(f2,f,flen);
    if (crypto_box_open_afternm(t2,d2,dlen,n2,f2) != 0) fail("crypto_box_open_afternm returns nonzero");
    if (memcmp(t2,t,tlen) != 0) fail("crypto_box_open_afternm is nondeterministic");
    
    double_canary(t2,t,tlen);
    double_canary(d2,d,dlen);
    double_canary(n2,n,nlen);
    double_canary(f2,f,flen);
    if (crypto_box_open_afternm(d2,d2,dlen,n,f) != 0) fail("crypto_box_open_afternm with d=t overlap returns nonzero");
    if (memcmp(d2,t,tlen) != 0) fail("crypto_box_open_afternm does not handle d=t overlap");
    memcpy(d2,d,dlen);
    if (crypto_box_open_afternm(n2,d,dlen,n2,f) != 0) fail("crypto_box_open_afternm with n=t overlap returns nonzero");
    if (memcmp(n2,t,tlen) != 0) fail("crypto_box_open_afternm does not handle n=t overlap");
    memcpy(n2,n,nlen);
    if (crypto_box_open_afternm(f2,d,dlen,n,f2) != 0) fail("crypto_box_open_afternm with f=t overlap returns nonzero");
    if (memcmp(f2,t,tlen) != 0) fail("crypto_box_open_afternm does not handle f=t overlap");
    memcpy(f2,f,flen);
  }
#include "test-more.inc"
}
