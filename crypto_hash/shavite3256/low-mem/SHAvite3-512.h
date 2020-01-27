#include "portable.h"
#include "AESround.h"
#include "SHA3api_ref.h"

#define ExternalRounds512 16
 
#define NonLinExpansion512(rk,counter,x0,x1,x2,x3,y0,y1,y2,y3,round_index)\
{\
        x0 = rk[1];\
        x1 = rk[2];\
        x2 = rk[3];\
        x3 = rk[0];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[0] = y0^rk[28];\
        rk[1] = y1^rk[29];\
        rk[2] = y2^rk[30];\
        rk[3] = y3^rk[31];\
        if (round_index==1) { \
           rk[0] ^= counter[0];\
           rk[1] ^= counter[1];\
           rk[2] ^= counter[2];\
           rk[3] ^= ~counter[3];\
        } \
	x0 = rk[5];\
        x1 = rk[6];\
        x2 = rk[7];\
        x3 = rk[4];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[4] = y0^rk[0];\
        rk[5] = y1^rk[1];\
        rk[6] = y2^rk[2];\
        rk[7] = y3^rk[3];\
        if (round_index==5) { \
           rk[4] ^= counter[3];\
           rk[5] ^= counter[2];\
           rk[6] ^= counter[1];\
           rk[7] ^= ~counter[0];\
        } \
        x0 = rk[9];\
        x1 = rk[10];\
        x2 = rk[11];\
        x3 = rk[8];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[8]  = y0^rk[4];\
        rk[9]  = y1^rk[5];\
        rk[10] = y2^rk[6];\
        rk[11] = y3^rk[7];\
        if (round_index==15) {\
           rk[8] ^= ~counter[1];\
           rk[9] ^= ~counter[0];\
           rk[10]^= ~counter[3];\
           rk[11]^= counter[2];\
        }\
        x0 = rk[13];\
        x1 = rk[14];\
        x2 = rk[15];\
        x3 = rk[12];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[12] = y0^rk[8];\
        rk[13] = y1^rk[9];\
        rk[14] = y2^rk[10];\
        rk[15] = y3^rk[11];\
	if (round_index==11) {\
           rk[12] ^= ~counter[2];\
           rk[13] ^= ~counter[3];\
           rk[14] ^= ~counter[0];\
           rk[15] ^= counter[1];\
        }\
        x0 = rk[17];\
        x1 = rk[18];\
        x2 = rk[19];\
        x3 = rk[16];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[16] = y0^rk[12];\
        rk[17] = y1^rk[13];\
        rk[18] = y2^rk[14];\
        rk[19] = y3^rk[15];\
        if (round_index==3) { \
           rk[16] ^= ~counter[0];\
           rk[17] ^= ~counter[1];\
           rk[18] ^= ~counter[2];\
           rk[19] ^= counter[3];\
        } \
	x0 = rk[21];\
        x1 = rk[22];\
        x2 = rk[23];\
        x3 = rk[20];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[20] = y0^rk[16];\
        rk[21] = y1^rk[17];\
        rk[22] = y2^rk[18];\
        rk[23] = y3^rk[19];\
        if (round_index==7) { \
           rk[20] ^= ~counter[3];\
           rk[21] ^= ~counter[2];\
           rk[22] ^= ~counter[1];\
           rk[23] ^= counter[0];\
        } \
        x0 = rk[25];\
        x1 = rk[26];\
        x2 = rk[27];\
        x3 = rk[24];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[24] = y0^rk[20];\
        rk[25] = y1^rk[21];\
        rk[26] = y2^rk[22];\
        rk[27] = y3^rk[23];\
        if (round_index==13) {\
           rk[24] ^= counter[1];\
           rk[25] ^= counter[0];\
           rk[26] ^= counter[3];\
           rk[27] ^= ~counter[2];\
        }\
        x0 = rk[29];\
        x1 = rk[30];\
        x2 = rk[31];\
        x3 = rk[28];\
        roundAESnokey(x0,x1,x2,x3,y0,y1,y2,y3);\
        rk[28] = y0^rk[24];\
        rk[29] = y1^rk[25];\
        rk[30] = y2^rk[26];\
        rk[31] = y3^rk[27];\
	if (round_index==9) {\
           rk[28] ^= counter[2];\
           rk[29] ^= counter[3];\
           rk[30] ^= counter[0];\
           rk[31] ^= ~counter[1];\
        }\
}


#define LinExpansion512(rk,temp0)\
{\
   for (temp0=0;temp0<7;temp0++)\
       rk[temp0]^=rk[temp0+25];\
   for (temp0=7;temp0<32;temp0++)\
       rk[temp0]^=rk[temp0-7];\
}


#define oneround(state0,state1,state2,state3,state4,state5,state6,state7,state8,state9,state10,state11,state12,state13,state14,state15,x0,x1,x2,x3,y0,y1,y2,y3,rk)\
{\
   x0 = state4^rk[0];\
   x1 = state5^rk[1];\
   x2 = state6^rk[2];\
   x3 = state7^rk[3];\
   roundAES(x0,x1,x2,x3,y0,y1,y2,y3,rk[4],rk[5],rk[6],rk[7]);\
   roundAES(y0,y1,y2,y3,x0,x1,x2,x3,rk[8],rk[9],rk[10],rk[11]);\
   roundAES(x0,x1,x2,x3,y0,y1,y2,y3,rk[12],rk[13],rk[14],rk[15]);\
   roundAESnokey(y0,y1,y2,y3,x0,x1,x2,x3);\
   state0^=x0;\
   state1^=x1;\
   state2^=x2;\
   state3^=x3;\
   x0 = state12^rk[16];\
   x1 = state13^rk[17];\
   x2 = state14^rk[18];\
   x3 = state15^rk[19];\
   roundAES(x0,x1,x2,x3,y0,y1,y2,y3,rk[20],rk[21],rk[22],rk[23]);\
   roundAES(y0,y1,y2,y3,x0,x1,x2,x3,rk[24],rk[25],rk[26],rk[27]);\
   roundAES(x0,x1,x2,x3,y0,y1,y2,y3,rk[28],rk[29],rk[30],rk[31]);\
   roundAESnokey(y0,y1,y2,y3,x0,x1,x2,x3);\
   state8^=x0;\
   state9^=x1;\
   state10^=x2;\
   state11^=x3;\
}

void E512(u32 pt[16], u32 ct[16], u32 message[32], u32 counter[4])
{
   u32 state[16];
   u32 x0,x1,x2,x3,y0,y1,y2,y3,i,j;
   u32 rk[32];

   for (i=0;i<16;i++) state[i]=pt[i]; 
   
   for (i=0;i<32;i++) rk[i]=message[i];

   for (i=0;i<ExternalRounds512; i++) {
      oneround(state[0],state[1],state[2],state[3],state[4],state[5],state[6],state[7],state[8],state[9],state[10],state[11],state[12],state[13],state[14],state[15],x0,x1,x2,x3,y0,y1,y2,y3,rk);
      if (((i&0x1) == 1) && (i<ExternalRounds512))
         LinExpansion512(rk,j);
      if ((i&0x1) == 0)
            NonLinExpansion512(rk,counter,x0,x1,x2,x3,y0,y1,y2,y3,i+1);
      x0=state[0]; x1=state[1]; x2=state[2]; x3=state[3];
      state[0]=state[12]; state[1]=state[13]; state[2]=state[14]; state[3]=state[15];
      state[12]=state[8]; state[13]=state[9]; state[14]=state[10]; state[15]=state[11];
      state[8]=state[4]; state[9]=state[5]; state[10]=state[6]; state[11]=state[7];
      state[4]=x0; state[5]=x1; state[6]=x2; state[7]=x3;
   }
   
   for (i=0;i<16;i++) ct[i]=state[i]; 

   return;
}

/* The actual compression function C_{512}                           */

void Compress512(const u8 *message_block, u8 *chaining_value, u64 counter)
{    
   u32 pt[16],ct[16];
   u32 msg_u32[32];
   u32 cnt[4];
   int i;

/* Translating all the inputs to 32-bit words                        */

   for (i=0;i<16;i++)
      pt[i]=U8TO32_LITTLE(chaining_value+4*i);
   

   for (i=0;i<32;i++)
      msg_u32[i]=U8TO32_LITTLE(message_block+4*i);

   cnt[1]=(u32)(counter>>32);
   cnt[0]=(u32)(counter & 0xFFFFFFFFULL);

/* Due to NIST specifications, there are no messages of length       */
/* longer than 2^64 that are expected				     */

   cnt[2]=0;
   cnt[3]=0;

/* Computing the encryption function                                 */

   E512(pt, ct, msg_u32, cnt);


/* Davies-Meyer transformation                                       */
 
   for (i=0;i<16;i++) 
       pt[i]^=ct[i]; 

/* Translating the output to 8-bit words                             */

   for(i=0; i<16; i++)
       U32TO8_LITTLE(chaining_value+i*4, pt[i]); 

   return;
}
