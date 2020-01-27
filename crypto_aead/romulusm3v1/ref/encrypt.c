/*
 * Date: 29 November 2018
 * Contact: Thomas Peyrin - thomas.peyrin@gmail.com
 * Mustafa Khairallah - mustafam001@e.ntu.edu.sg
 */

#include "crypto_aead.h"
#include "api.h"
#include "variant.h"
#include "skinny.h"
#include <stdio.h>
#include <stdlib.h>

/*void display_vector (const unsigned char* x, int lenx) {
  int i;

  for (i = 0; i < lenx; i++) {
    //printf("%02x",x[i]);
  }
  //printf("\n");
  
  }*/


void pad (const unsigned char* m, unsigned char* mp, int l, int len8) {
  int i;

  for (i = 0; i < l; i++) {
    if (i < len8) {      
      mp[i] = m[i];
    }
    else if (i == l - 1) {
      mp[i] = (len8 & 0x0f);
    }
    else {
      mp[i] = 0x00;
    }      
  }
  
}

void g8A (unsigned char* s, unsigned char* c) {
  int i;

  for (i = 0; i < 16; i++) {
    c[i] = (s[i] >> 1) ^ (s[i] & 0x80) ^ ((s[i] & 0x01) << 7);
  }
  
}

void rho_ad (const unsigned char* m,
	     unsigned char* s,
	     int len8,
	     int ver) {
  int i;
  unsigned char mp [16];  

  //printf("rho in m  = ");display_vector(m,len8);
  pad(m,mp,ver,len8);
  //printf("rho in mp = ");display_vector(mp,16);
  //printf("rho in s  = ");display_vector(s,16);
  for (i = 0; i < ver; i++) {
    s[i] = s[i] ^ mp[i];
  }
  //printf("rho out s = ");display_vector(s,16);
  
}

void rho (const unsigned char* m,
	  unsigned char* c,
	  unsigned char* s,
	  int len8,
	  int ver) {
  int i;
  unsigned char mp [16];

  //printf("rho in m  = ");display_vector(m,len8);
  pad(m,mp,ver,len8);
  //printf("rho in mp = ");display_vector(mp,16);
  //printf("rho in s  = ");display_vector(s,16);

  g8A(s,c);
  for (i = 0; i < ver; i++) {
    s[i] = s[i] ^ mp[i];
    if (i < len8) {
      c[i] = c[i] ^ mp[i];
    }
    else {
      c[i] = 0;
    }
  }
  //printf("rho out s = ");display_vector(s,16);
  //printf("rho out c = ");display_vector(c,16);
  
}

void irho (unsigned char* m,
	  const unsigned char* c,
	  unsigned char* s,
	  int len8,
	  int ver) {
  int i;
  unsigned char cp [16];

  //printf("irho in c  = ");display_vector(c,len8);
  pad(c,cp,ver,len8);
  //printf("irho in cp = ");display_vector(cp,16);
  //printf("irho in s  = ");display_vector(s,16);

  g8A(s,m);
  for (i = 0; i < ver; i++) {
    if (i < len8) {
      s[i] = s[i] ^ cp[i] ^ m[i];
    }
    else {
      s[i] = s[i] ^ cp[i];
    }
    if (i < len8) {
      m[i] = m[i] ^ cp[i];
    }
    else {
      m[i] = 0;
    }
  }
  //printf("irho out s = ");display_vector(s,16);
  //printf("irho out m = ");display_vector(c,16);
  
}


void reset_lfsr_gf24 (unsigned char* CNT) {
  CNT[0] = 0x01;
  CNT[1] = 0x00;
  CNT[2] = 0x00;
}

void lfsr_gf24 (unsigned char* CNT) {
  unsigned char fb0;
  
  fb0 = CNT[2] >> 7;
  
  CNT[2] = (CNT[2] << 1) | (CNT[1] >> 7);
  CNT[1] = (CNT[1] << 1) | (CNT[0] >> 7);
  if (fb0 == 1) {
    CNT[0] = (CNT[0] << 1) ^ 0x95;
  }
  else {
    CNT[0] = (CNT[0] << 1);
  }
}

void compose_tweakey (unsigned char* KT,
		      const unsigned char* K,
		      unsigned char* T,
		      unsigned char* CNT,
		      unsigned char D,
		      int t) {

  int i;

  for (i = 0; i < 3; i++) {
    KT[i] = CNT[i];
  }
  KT[i] = D;
  for (i = 0; i < t; i++) {
    KT[i+4] = T[i];
  }
  for (i = 0; i < 16; i++) {
    KT[i+4+t] = K[i];
  }
  for (i = 0; i < 3; i++) {
    KT[i+4+t+16] = CNT[i+3];
  }
  for (i = 0; i < 13; i++) {
    KT[i+4+t+16+3] = 0x00;
  }

}

void block_cipher(unsigned char* s,
		  const unsigned char* k, unsigned char* T,
		  unsigned char* CNT, unsigned char D, int t, int n) {
  unsigned char KT [48];

  (void)n;
  compose_tweakey(KT,k,T,CNT,D,t);
  //printf("BC in kt = ");display_vector(KT,3*n);
  //printf("BC in s  = ");display_vector(s,n);
  skinny_128_256_enc (s,KT);
  //printf("BC out s = ");display_vector(s,n);

}

void nonce_encryption (const unsigned char* N,
		       unsigned char* CNT,
		       unsigned char*s, const unsigned char* k,
		       int t, int n, unsigned char D) {
  unsigned char T [16];
  int i;
  
  for (i = 0; i < t; i++) {
    T[i] = N[i];
  }  
  //printf("nonce cnt = ");display_vector(CNT,7);    
  block_cipher(s,k,T,CNT,D,t,n);

}

void generate_tag (unsigned char** c, unsigned char* s,
		   int n, unsigned long long* clen) {
  
  g8A(s, *c);
  *c = *c + n;
  *c = *c - *clen;

}

unsigned long long msg_encryption (const unsigned char** M, unsigned char** c,
				   const unsigned char* N,
				   unsigned char* CNT,
				   unsigned char*s, const unsigned char* k,
				   unsigned int n, unsigned int t, unsigned char D,
				   unsigned long long mlen) {
  int len8;

  
  if (mlen >= n) {
    len8 = n;
    mlen = mlen - n;
  }
  else {
    len8 = mlen;
    mlen = 0;
  }
  rho(*M, *c, s, len8, n);
  *c = *c + len8;
  *M = *M + len8;
  lfsr_gf24(CNT);
  if (mlen != 0) {
    nonce_encryption(N,CNT,s,k,t,n,D);
  }
  return mlen;
}



unsigned long long msg_decryption (unsigned char** M, const unsigned char** c,
				   const unsigned char* N,
				   unsigned char* CNT,
				   unsigned char*s, const unsigned char* k,
				   unsigned int n, unsigned int t, unsigned char D,
				   unsigned long long clen) {
  int len8;

  if (clen >= n) {
    len8 = n;
    clen = clen - n;
  }
  else {
    len8 = clen;
    clen = 0;
  }
  irho(*M, *c, s, len8, n);
  *c = *c + len8;
  *M = *M + len8;
  lfsr_gf24(CNT);
  nonce_encryption(N,CNT,s,k,t,n,D);
  return clen;
}

unsigned long long ad2msg_encryption (const unsigned char** M,
				      unsigned char* CNT,
				      unsigned char*s, const unsigned char* k,
				      unsigned int t, unsigned int n, unsigned char D,
				      unsigned long long mlen) {
  unsigned char T [16];
  int len8;

  if (mlen <= t) {
    len8 = mlen;
    mlen = 0;
  }
  else {
    len8 = t;
    mlen = mlen - t;
  }

  pad (*M,T,t,len8);
  //printf("ad2msg nonce cnt = ");display_vector(CNT,7);    
  block_cipher(s,k,T,CNT,D,t,n);
  lfsr_gf24(CNT);
  *M = *M + len8;
  
  return mlen;

}


unsigned long long ad_encryption (const unsigned char** A, unsigned char* s,
				  const unsigned char* k, unsigned long long adlen,
				  unsigned char* CNT,
				  unsigned char D,				  
				  unsigned int n, unsigned int t) {

  unsigned char T [16];
  int len8;
  
  if (adlen >= n) {
    len8 = n;
    adlen = adlen - n;
  }
  else {
    len8 = adlen;
    adlen = 0;
  }
  //printf("AD in = ");display_vector(*A,len8);  
  rho_ad(*A, s, len8, n);
  *A = *A + len8;
  lfsr_gf24(CNT);
  //printf("AD cnt = ");display_vector(CNT,7);    
  if (adlen != 0) {
    if (adlen >= t) {
      len8 = t;
      adlen = adlen - t;
    }
    else {
      len8 = adlen;
      adlen = 0;    
    }
    pad(*A, T, t, len8);
    *A = *A + len8;
    block_cipher(s,k,T,CNT,D,t,n);
    lfsr_gf24(CNT);
  }

  return adlen;
}

int crypto_aead_encrypt (
			 unsigned char* c, unsigned long long* clen,
			 const unsigned char* m, unsigned long long mlen,
			 const unsigned char* ad, unsigned long long adlen,
			 const unsigned char* nsec,
			 const unsigned char* npub,
			 const unsigned char* k
			 )
{
  unsigned char s[16];
  unsigned char CNT[7];
  unsigned char T[16];
  const unsigned char* N;
  unsigned int n, t, i;
  unsigned char w;
  unsigned long long xlen;

  (void)nsec;
  N = npub;
  
  n = AD_BLK_LEN_ODD;
  t = AD_BLK_LEN_EVN;

  xlen = mlen;

  for (i = 0; i < n; i++) {
    s[i] = 0;
  }      
  reset_lfsr_gf24(CNT);
  //printf("s = ");display_vector(s,16);
  //printf("cnt = ");display_vector(CNT,7);

  w = 176;

  if (adlen == 0) {
    w = w ^ 2;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }   
  }
  else if (adlen%(n+t) == 0) {
    w = w ^ 8;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < n) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == n) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else if (adlen%(n+t) < n) {
    w = w ^ 2;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else if (adlen%(n+t) == n) {
    w = w ^ 0;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else {
    w = w ^ 10;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < n) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == n) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  
  if (adlen == 0) { // AD is an empty string
    lfsr_gf24(CNT);    
    //xlen = ad2msg_encryption (&m,CNT,s,k,t,n,172,xlen);      
  }
  else while (adlen > 0) {
      adlen = ad_encryption(&ad,s,k,adlen,CNT,168,n,t);
    }

  if ((w & 8) == 0) {
    xlen = ad2msg_encryption (&m,CNT,s,k,t,n,172,xlen);      
  }
  else if (mlen == 0) {
    lfsr_gf24(CNT);    
  }  
  while (xlen > 0) {
    xlen = ad_encryption(&m,s,k,xlen,CNT,172,n,t);
  }
  nonce_encryption(N,CNT,s,k,t,n,w);
  
  
  // Tag generation 
  g8A(s, T);
  //printf("T = ");display_vector(T,n);

  m = m - mlen;
  
  reset_lfsr_gf24(CNT);

  for (i = 0; i < n; i = i + 1) {
    s[i] = T[i];
  }

  n = MSG_BLK_LEN;
  *clen = mlen + n;



  if (mlen > 0) {
    nonce_encryption(N,CNT,s,k,t,n,164);  
    while (mlen > n) {
      mlen = msg_encryption(&m,&c,N,CNT,s,k,n,t,164,mlen);
    }
    rho(m, c, s, mlen, 16);
    c = c + mlen;
    m = m + mlen;    
  }

  // Tag Concatenation
  for (i = 0; i < 16; i = i + 1) {
    *(c + i) = T[i];
  }

  c = c - *clen;

  
  
  return 0;
}

int crypto_aead_decrypt(
unsigned char *m,unsigned long long *mlen,
unsigned char *nsec,
const unsigned char *c,unsigned long long clen,
const unsigned char *ad,unsigned long long adlen,
const unsigned char *npub,
const unsigned char *k
)
{
  unsigned char s[16];
  unsigned char CNT[7];
  unsigned char T[16];
  const unsigned char* N;
  unsigned int n, t, i;
  unsigned char w;
  unsigned long long xlen;
  const unsigned char* mauth;

  (void)nsec;
  mauth = m;

  N = npub;
  
  n = AD_BLK_LEN_ODD;
  t = AD_BLK_LEN_EVN;

  xlen = clen-16;

  reset_lfsr_gf24(CNT);

  for (i = 0; i < 16; i++) {
    T[i] = *(c + clen - 16 + i);
  }

  for (i = 0; i < n; i = i + 1) {
    s[i] = T[i];
  }

  n = MSG_BLK_LEN;
  clen = clen - 16;
  *mlen = clen;


  if (clen > 0) {    
    nonce_encryption(N,CNT,s,k,t,n,164);
    while (clen > n) {
      clen = msg_decryption(&m,&c,N,CNT,s,k,n,t,164,clen);
    }
    irho(m, c, s, clen, 16);
    c = c + clen;
    m = m + clen;
  }
  

  for (i = 0; i < n; i++) {
    s[i] = 0;
  }      
  reset_lfsr_gf24(CNT);
  //printf("s = ");display_vector(s,16);
  //printf("cnt = ");display_vector(CNT,7);

    w = 176;

  if (adlen == 0) {
    w = w ^ 2;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }   
  }
  else if (adlen%(n+t) == 0) {
    w = w ^ 8;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < n) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == n) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else if (adlen%(n+t) < n) {
    w = w ^ 2;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else if (adlen%(n+t) == n) {
    w = w ^ 0;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < t) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == t) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  else {
    w = w ^ 10;
    if (xlen == 0) {
      w =w ^ 1;
    }
    else if (xlen%(n+t) == 0) {
      w = w ^ 4;
    }
    else if (xlen%(n+t) < n) {
      w = w ^ 1;
    }
    else if (xlen%(n+t) == n) {
      w = w ^ 0;
    }
    else {
      w = w ^ 5;
    }       
  }
  
  if (adlen == 0) { // AD is an empty string
    lfsr_gf24(CNT);    
    //xlen = ad2msg_encryption (&mauth,CNT,s,k,t,n,172,xlen);      
  }
  else while (adlen > 0) {
     adlen = ad_encryption(&ad,s,k,adlen,CNT,168,n,t);
    }

  if ((w & 8) == 0) {
    xlen = ad2msg_encryption (&mauth,CNT,s,k,t,n,172,xlen);      
  }
  else if (clen == 0) {
    lfsr_gf24(CNT);    
  }    
  while (xlen > 0) {
    xlen = ad_encryption(&mauth,s,k,xlen,CNT,172,n,t);
  }
  nonce_encryption(N,CNT,s,k,t,n,w);

    // Tag generation 
  g8A(s, T);
  //printf("T = ");display_vector(T,16);
  //printf("t = ");display_vector(c,16);
  for (i = 0; i < 16; i++) {
    if (T[i] != (*(c+i))) {
      return -1;
    }    
  }
  
  return 0;
}
