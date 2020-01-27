//NEON C code for SIMON 64/96.

/***************************************************************************************
 ** DISCLAIMER.  THIS SOFTWARE WAS WRITTEN BY EMPLOYEES OF THE U.S.
 ** GOVERNMENT AS A PART OF THEIR OFFICIAL DUTIES AND, THEREFORE, IS NOT
 ** PROTECTED BY COPYRIGHT.  THE U.S. GOVERNMENT MAKES NO WARRANTY, EITHER
 ** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES
 ** OF MERCANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, REGARDING THIS SOFTWARE.
 ** THE U.S. GOVERNMENT FURTHER MAKES NO WARRANTY THAT THIS SOFTWARE WILL NOT
 ** INFRINGE ANY OTHER UNITED STATES OR FOREIGN PATENT OR OTHER
 ** INTELLECTUAL PROPERTY RIGHT.  IN NO EVENT SHALL THE U.S. GOVERNMENT BE
 ** LIABLE TO ANYONE FOR COMPENSATORY, PUNITIVE, EXEMPLARY, SPECIAL,
 ** COLLATERAL, INCIDENTAL, CONSEQUENTIAL, OR ANY OTHER TYPE OF DAMAGES IN
 ** CONNECTION WITH OR ARISING OUT OF COPY OR USE OF THIS SOFTWARE.
 ***************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "Simon6496NEON.h"

int crypto_stream_simon6496ctr_neon(unsigned char *out, unsigned long long outlen, const unsigned char *n, const unsigned char *k);
inline __attribute__((always_inline)) int Encrypt(unsigned char *out, u32 nonce[], u128 rk[][8], u32 key[], int numbytes);
int crypto_stream_simon6496ctr_neon_xor(unsigned char *out, const unsigned char *in, unsigned long long inlen, const unsigned char *n, const unsigned char *k);
inline __attribute__((always_inline)) int Encrypt_Xor(unsigned char *out, const unsigned char *in, u32 nonce[], u128 rk[][8], u32 key[], int numbytes);
inline __attribute__((always_inline)) int ExpandKeyBS(u32 K[], u128 rk[][8]);
int ExpandKeyNBS(u32 K[], u128 rk[][8], u32 key[]);



int crypto_stream_simon6496ctr_neon(
  unsigned char *out,
  unsigned long long outlen,
  const unsigned char *n,
  const unsigned char *k
)
{
  int i;
  u32 nonce[2], K[4], key[44];
  unsigned char block[8];
  u128 rk[44][8];

  if (!outlen) return 0;

  nonce[0]=((u32 *)n)[0];
  nonce[1]=((u32 *)n)[1];

  for(i=0;i<numkeywords;i++) K[i]=((u32 *)k)[i];

  if (outlen>=8192){
    ExpandKeyBS(K,rk);

    while(outlen>=256){
      Encrypt(out,nonce,rk,key,256);
      out+=256; outlen-=256;
    }
  }

  if (!outlen) return 0;

  ExpandKeyNBS(K,rk,key);

  while (outlen>=128){
    Encrypt(out,nonce,rk,key,128);
    out+=128; outlen-=128;
  }

  if (outlen>=96){
    Encrypt(out,nonce,rk,key,96);
    out+=96; outlen-=96;
  }

  if (outlen>=64){
    Encrypt(out,nonce,rk,key,64);
    out+=64; outlen-=64;
  }

  if (outlen>=32){
    Encrypt(out,nonce,rk,key,32);
    out+=32; outlen-=32;
  }

  if (outlen>=16){
    Encrypt(out,nonce,rk,key,16);
    out+=16; outlen-=16;
  }

  if (outlen>=8){
    Encrypt(out,nonce,rk,key,8);
    out+=8; outlen-=8;
  }

  if (outlen>0){
    Encrypt(block,nonce,rk,key,8);
    for(i=0;i<outlen;i++) out[i]=block[i];
  }

  return 0;
}



inline __attribute__((always_inline)) int Encrypt(unsigned char *out, u32 nonce[], u128 rk[][8], u32 key[], int numbytes)
{
  u32  i,x[2],y[2];
  u128 X[8],Y[8],W[4];


  if (numbytes==8){
    x[0]=nonce[1]; y[0]=nonce[0]++;
    Enc(x,y,key,1);
    ((u32 *)out)[1]=x[0]; ((u32 *)out)[0]=y[0];

    return 0;
  }

  if (numbytes==16){
    x[0]=nonce[1]; y[0]=nonce[0]++;
    x[1]=nonce[1]; y[1]=nonce[0]++;
    Enc(x,y,key,2);
    ((u32 *)out)[1]=x[0]; ((u32 *)out)[0]=y[0];
    ((u32 *)out)[3]=x[1]; ((u32 *)out)[2]=y[1];

    return 0;
  }

  SET1(X[0],nonce[1]); SET4(Y[0],nonce[0]);

  if (numbytes==32) Enc(X,Y,rk,4);
  else{
    X[1]=X[0]; SET4(Y[1],nonce[0]);
    if (numbytes==64) Enc(X,Y,rk,8);
    else{
      X[2]=X[0]; SET4(Y[2],nonce[0]);
      if (numbytes==96) Enc(X,Y,rk,12);
      else{
        X[3]=X[0]; SET4(Y[3],nonce[0]);
        if (numbytes==128) Enc(X,Y,rk,16);
        else{
          X[4]=X[0]; SET4(Y[4],nonce[0]);
          X[5]=X[0]; SET4(Y[5],nonce[0]);
          X[6]=X[0]; SET4(Y[6],nonce[0]);
          X[7]=X[0]; SET4(Y[7],nonce[0]);
          Transpose(X); Transpose(Y);
          for(i=0;i<42;i+=2) R2x32(X,Y,rk,i,i+1);
          Transpose(X); Transpose(Y);
        }
      }
    }
  }

  STORE(out,X[0],Y[0]);
  if (numbytes>=64)  STORE(out+32,X[1],Y[1]);
  if (numbytes>=96)  STORE(out+64,X[2],Y[2]);
  if (numbytes>=128) STORE(out+96,X[3],Y[3]);
  if (numbytes>=256){
    STORE(out+128,X[4],Y[4]);
    STORE(out+160,X[5],Y[5]);
    STORE(out+192,X[6],Y[6]);
    STORE(out+224,X[7],Y[7]);
  }

  return 0;
}



int crypto_stream_simon6496ctr_neon_xor(
  unsigned char *out,
  const unsigned char *in,
  unsigned long long inlen,
  const unsigned char *n,
  const unsigned char *k
)
{
  u32 i,nonce[2],K[4],key[44];
  unsigned char block[8];
  u64 * const block64 = (u64 *)block;
  u128 rk[44][8];

  if (!inlen) return 0;

  nonce[0]=((u32 *)n)[0];
  nonce[1]=((u32 *)n)[1];

  for(i=0;i<numkeywords;i++) K[i]=((u32 *)k)[i];

  if (inlen>=8192){
    ExpandKeyBS(K,rk);

    while(inlen>=256){
      Encrypt_Xor(out,in,nonce,rk,key,256);
      in+=256; inlen-=256; out+=256;
    }
  }

  if (!inlen) return 0;

  ExpandKeyNBS(K,rk,key);

  while (inlen>=128){
    Encrypt_Xor(out,in,nonce,rk,key,128);
    in+=128; inlen-=128; out+=128;
  }

  if (inlen>=96){
    Encrypt_Xor(out,in,nonce,rk,key,96);
    in+=96; inlen-=96; out+=96;
  }

  if (inlen>=64){
    Encrypt_Xor(out,in,nonce,rk,key,64);
    in+=64; inlen-=64; out+=64;
  }

  if (inlen>=32){
    Encrypt_Xor(out,in,nonce,rk,key,32);
    in+=32; inlen-=32; out+=32;
  }

  if (inlen>=16){
    Encrypt_Xor(out,in,nonce,rk,key,16);
    in+=16; inlen-=16; out+=16;
  }

  if (inlen>=8){
    Encrypt_Xor(block,in,nonce,rk,key,8);
    ((u64 *)out)[0]=block64[0]^((u64 *)in)[0];
    in+=8; inlen-=8; out+=8;
  }

  if (inlen>0){
    Encrypt_Xor(block,in,nonce,rk,key,8);
    for(i=0;i<inlen;i++) out[i]=block[i]^in[i];
  }

  return 0;
}



inline __attribute__((always_inline)) int Encrypt_Xor(unsigned char *out, const unsigned char *in, u32 nonce[], u128 rk[][8], u32 key[], int numbytes)
{
  u32  i,x[2],y[2];
  u128 X[8],Y[8],W[4];


  if (numbytes==8){
    x[0]=nonce[1]; y[0]=nonce[0]++;
    Enc(x,y,key,1);
    ((u32 *)out)[1]=x[0]; ((u32 *)out)[0]=y[0];

    return 0;
  }

  if (numbytes==16){
    x[0]=nonce[1];   x[1]=nonce[1];
    y[0]=nonce[0]++; y[1]=nonce[0]++;
    Enc(x,y,key,2);
    ((u32 *)out)[1]=x[0]^((u32 *)in)[1]; ((u32 *)out)[0]=y[0]^((u32 *)in)[0];
    ((u32 *)out)[3]=x[1]^((u32 *)in)[3]; ((u32 *)out)[2]=y[1]^((u32 *)in)[2];

    return 0;
  }

  SET1(X[0],nonce[1]); SET4(Y[0],nonce[0]);

  if (numbytes==32) Enc(X,Y,rk,4);
  else{
    X[1]=X[0]; SET4(Y[1],nonce[0]);
    if (numbytes==64) Enc(X,Y,rk,8);
    else{
      X[2]=X[0]; SET4(Y[2],nonce[0]);
      if (numbytes==96) Enc(X,Y,rk,12);
      else{
        X[3]=X[0]; SET4(Y[3],nonce[0]);
        if (numbytes==128) Enc(X,Y,rk,16);
        else{
          X[4]=X[0]; SET4(Y[4],nonce[0]);
          X[5]=X[0]; SET4(Y[5],nonce[0]);
          X[6]=X[0]; SET4(Y[6],nonce[0]);
          X[7]=X[0]; SET4(Y[7],nonce[0]);
          Transpose(X); Transpose(Y);
          for(i=0;i<42;i+=2) R2x32(X,Y,rk,i,i+1);
          Transpose(X); Transpose(Y);
        }
      }
    }
  }

  XOR_STORE(in,out,X[0],Y[0]);
  if (numbytes>=64)  XOR_STORE(in+32,out+32,X[1],Y[1]);
  if (numbytes>=96)  XOR_STORE(in+64,out+64,X[2],Y[2]);
  if (numbytes>=128) XOR_STORE(in+96,out+96,X[3],Y[3]);
  if (numbytes>=256){
    XOR_STORE(in+128,out+128,X[4],Y[4]);
    XOR_STORE(in+160,out+160,X[5],Y[5]);
    XOR_STORE(in+192,out+192,X[6],Y[6]);
    XOR_STORE(in+224,out+224,X[7],Y[7]);
  }

  return 0;
}


inline __attribute__((always_inline)) int ExpandKeyBS(u32 K[],u128 rk[][8])
{
  int i,j;
  u128 W[4];

  for(i=0;i<3;i++){
    SET1(rk[i][0],K[i]);
    for(j=1;j<n;j++){
      rk[i][j]=rk[i][0];
    }
    Transpose(rk[i]);
  }

  EKBS(rk);

  return 0;
}



int ExpandKeyNBS(u32 K[], u128 rk[][8], u32 key[])
{
  u32 A=K[0], B=K[1], C=K[2];

  EKNBS(A,B,C,rk,key);

  return 0;
}
