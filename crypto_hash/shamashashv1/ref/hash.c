#include "crypto_hash.h"
#include <stdlib.h>

/* 
the code below is the same as the encrypt.c of the Shamash submission,
except that since we do not need the decryption part we do not include it*/


typedef unsigned long long u64;
typedef unsigned int u32;

//number of rounds of initialization, times 11 (12x11)
#define NRINIT 132

//number of rounds of processing ad and message, times 11
#define NRMID 99

//extra number of rounds of finalization (after NRMID rounds are done), times 11
#define NREND 33

//increase of i for the round constants
#define STEP 11

#define ROTR(x,n) (((x)>>(n))|((x)<<(64-(n))))

void Difusion(u64 W[5]){
	W[3]^= ROTR(W[3],57)^ROTR(W[3],63);
	W[4]^= ROTR(W[4],3)^ROTR(W[4],26);
	W[0]^= ROTR(W[0],43)^ROTR(W[0],62);
	W[1]^= ROTR(W[1],21)^ROTR(W[1],46);
	W[2]^= ROTR(W[2],58)^ROTR(W[2],61);
	W[0]^=W[3]^W[4];W[1]^=W[3]^W[4];W[2]^=W[3]^W[4];
	W[3]^=W[0]^W[1]^W[2];W[4]^=W[0]^W[1]^W[2];
	W[0]=ROTR(W[0],8);
	W[1]=ROTR(W[1],24);
	W[2]=ROTR(W[2],40);
	W[3]=ROTR(W[3],56);
}

void Sbox(u64 W[5]){
	u64 x=W[0];
	u64 y=W[1];
	u64 z=W[2];
	u64 u=W[3];
	u64 v=W[4];
	W[0]=v^(x|u)^(y|(z^x));
	W[1]=x^(y|v)^(z|(u^y));
	W[2]=y^(z|x)^(u|(v^z));
	W[3]=z^(u|y)^(v|(x^u));
	W[4]=(~u)^(v|z)^(x|(y^v));
}




//R7R6....R1R0
void LESTOREu64in8bytearray(u64 Rv,unsigned char* R){
	for(u32 i=0;i<8;i++) {
		R[i]=(Rv&0xff);
		Rv=(Rv>>8);
	} 
} 


void PartialLESTOREu64inbytearray(u64 Rv,unsigned char* R,u64 Top){
	for(u32 i=0;i<Top;i++) {
		R[i]=(Rv&0xff);
		Rv=(Rv>>8);
	} 
} 


u64 LELOAD8bytearrayinu64(const unsigned char* R){
	u64 Rv=0;
	for(u32 i=0;i<8;i++) {
		Rv|=((u64)(R[i])<<(8*i));
	}
	return Rv; 
}

//should only be used if Top<8.
u64 PartialLELOADbytearrayinu64(const unsigned char* R,u64 Top){
	u64 Rv=(u64)R[0];
	for(u64 i=1;i<Top;i++) {
		Rv^=((u64)(R[i])<<(8*i));
	}
	return Rv;
}


void Ronda(u64 Rconst,u64 W[5]) {
	W[0]^=Rconst;
	Sbox(W);
	Difusion(W);
}



//aux funcs


void Initialize(u64 W[5],const unsigned char *npub,const unsigned char *k){
	W[0]=LELOAD8bytearrayinu64(k);k+=8;
	W[1]=LELOAD8bytearrayinu64(k);
	W[2]=LELOAD8bytearrayinu64(npub);npub+=8;
	W[3]=LELOAD8bytearrayinu64(npub);
	W[4]=ROTR(W[0]^W[1],32)^0xff;

	for(unsigned int i=0;i<NRINIT;i=i+STEP) Ronda(i^0xff,W);

}

void ProcessAd(u64 W[5],const unsigned char *ad,unsigned long long adlen){
		u64 remain=adlen;		
		while(remain>=16){
			W[3]^=LELOAD8bytearrayinu64(ad);	ad+=8;
			W[2]^=LELOAD8bytearrayinu64(ad);
			for(unsigned int  i=0;i<NRMID;i=i+STEP) Ronda(i^0xad,W);
			remain-=16;
			ad+=8;
		}//endwhile
		if(remain){
			if(remain<8){
				W[3]^=PartialLELOADbytearrayinu64(ad,remain);
				W[3]^=((u64)(1)<<(8*remain));
			}else{
				W[3]^=LELOAD8bytearrayinu64(ad);	ad+=8;remain-=8;
				if(remain) W[2]^=PartialLELOADbytearrayinu64(ad,remain);
				W[2]^=((u64)(1)<<(8*remain));
			}
			W[1]^=1;//distinguisher from remain=0 case
			for(unsigned int  i=0;i<NRMID;i=i+STEP) Ronda(i^0xad,W);
		}//endifadremain

	W[1]^=1;//delimiter ad/mesg

}

unsigned char* Processplaintxt(u64 W[5],unsigned char *c,const unsigned char *m,unsigned long long mlen){
		u64 remain=mlen;
		while(remain>=16){
			W[3]^=LELOAD8bytearrayinu64(m);
			LESTOREu64in8bytearray(W[3],c);m+=8;c+=8;
			W[2]^=LELOAD8bytearrayinu64(m);
			LESTOREu64in8bytearray(W[2],c);

			for(unsigned int  i=0;i<NRMID;i=i+STEP) Ronda(i^0xff,W);
			remain-=16;
			m+=8;c+=8;
		}//endwhile

		if(remain){
			if(remain<8){
				W[3]^=PartialLELOADbytearrayinu64(m,remain);
				PartialLESTOREu64inbytearray(W[3],c,remain);
				W[3]^=((u64)(1)<<(8*remain));
			}
			else{
				W[3]^=LELOAD8bytearrayinu64(m);
				LESTOREu64in8bytearray(W[3],c);m+=8;c+=8;remain-=8;
				if(remain){
					W[2]^=PartialLELOADbytearrayinu64(m,remain);
					PartialLESTOREu64inbytearray(W[2],c,remain);
				}
				W[2]^=((u64)(1)<<(8*remain));
			}
			W[1]^=1;//distinguisher from remain=0 case
			c+=remain;
			for(unsigned int  i=0;i<NRMID;i=i+STEP) Ronda(i^0xff,W);
		}//endifremain
return c;//need it to write the tag into c
}


int crypto_aead_encrypt(
unsigned char *c,unsigned long long *clen,
const unsigned char *m,unsigned long long mlen,
const unsigned char *ad,unsigned long long adlen,
const unsigned char *nsec,
const unsigned char *npub,
const unsigned char *k
){
/*generating a ciphertext c[0],c[1],...,c[*clen-1]
... from a plaintext m[0],m[1],...,m[mlen-1]
... and associated data ad[0],ad[1],...,ad[adlen-1]
... and nonce npub[0],npub[1],...
... and secret key k[0],k[1],...
... the implementation shall not use nsec*/

		(void)nsec;//avoid warning
//init
	u64 W[5];
	Initialize(W,npub,k);

//ad
		ProcessAd(W,ad,adlen);

//cipher
c=Processplaintxt(W,c,m,mlen);

//finalization
	for(unsigned int i=NRMID;i<NRMID+NREND;i=i+STEP) Ronda(i^0xff,W);

	//tag:
	LESTOREu64in8bytearray(W[0],c);c+=8;
	LESTOREu64in8bytearray(W[1],c);
	*clen = mlen+16;

return 0;
}










/* =========================================
    END incorporating functions of Shamash
    Now we use them
==============================================
*/




int crypto_hash(unsigned char *out,const unsigned char *in,unsigned long long inlen){
/*
generating a hash value out[0],out[1],...,out[CRYPTO_BYTES-1]
... from a message in[0],in[1],...,in[inlen-1]*/
unsigned int i;
unsigned char c[64];
unsigned long long clen;
unsigned char m[48];
for(i=0;i<48;i++) m[i]=0;
unsigned long long mlen=48;
unsigned char nsec[1]={0};
	unsigned char npub[16];
	unsigned char k[16];
for(i=0;i<16;i++) {k[i]=0;npub[i]=0;}
unsigned char *ad;
unsigned long long adlen;
if(inlen>8){	
	ad= (unsigned char*)calloc(2*(inlen-8), sizeof(char));
	adlen=2*(inlen-8);
	for(i=0;i<8;i++) {npub[i]=in[i];}
	npub[8]=9;
	for(i=8;i<inlen;i++){
		ad[2*(i-8)]=in[i];//8->0 9->2 10->4 while 1,3,etc remain 0.
	}//endfor
}//endif
else{
	ad=NULL;adlen=0;
	for(i=0;i<inlen;i++) {npub[i]=in[i];}
	npub[8]=inlen;
}//endelse

crypto_aead_encrypt(c,&clen,m,mlen,ad,adlen,nsec,npub,k);

for(i=0;i<32;i++) out[i]=c[2*i];


if(inlen>8) free(ad);




return 0;
}
