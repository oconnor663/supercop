/*
 * Implementation based on the SPONGENT implementation at 
 * https://sites.google.com/site/spongenthash/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elephant_176.h"

#if		defined(SPONGENT160)
#define nBits		160
#define nSBox		20 // 8 bit
#define nRounds		80
#define lfsrIV	    0x75
#elif   defined(SPONGENT176)
#define nBits		176
#define nSBox		22
#define nRounds		90
#define lfsrIV	    0x45
#else
#define nBits		0
#define nSBox		0
#define nRounds		0
#define lfsrIV	    0
#endif

#define GET_BIT(x,y) (x >> y) & 0x1

/* Spongent eight bit S-box */
int  sBoxLayer[256] = {
	0xee, 0xed, 0xeb, 0xe0, 0xe2, 0xe1, 0xe4, 0xef, 0xe7, 0xea, 0xe8, 0xe5, 0xe9, 0xec, 0xe3, 0xe6, 
	0xde, 0xdd, 0xdb, 0xd0, 0xd2, 0xd1, 0xd4, 0xdf, 0xd7, 0xda, 0xd8, 0xd5, 0xd9, 0xdc, 0xd3, 0xd6, 
	0xbe, 0xbd, 0xbb, 0xb0, 0xb2, 0xb1, 0xb4, 0xbf, 0xb7, 0xba, 0xb8, 0xb5, 0xb9, 0xbc, 0xb3, 0xb6, 
	0x0e, 0x0d, 0x0b, 0x00, 0x02, 0x01, 0x04, 0x0f, 0x07, 0x0a, 0x08, 0x05, 0x09, 0x0c, 0x03, 0x06, 
	0x2e, 0x2d, 0x2b, 0x20, 0x22, 0x21, 0x24, 0x2f, 0x27, 0x2a, 0x28, 0x25, 0x29, 0x2c, 0x23, 0x26, 
	0x1e, 0x1d, 0x1b, 0x10, 0x12, 0x11, 0x14, 0x1f, 0x17, 0x1a, 0x18, 0x15, 0x19, 0x1c, 0x13, 0x16, 
	0x4e, 0x4d, 0x4b, 0x40, 0x42, 0x41, 0x44, 0x4f, 0x47, 0x4a, 0x48, 0x45, 0x49, 0x4c, 0x43, 0x46, 
	0xfe, 0xfd, 0xfb, 0xf0, 0xf2, 0xf1, 0xf4, 0xff, 0xf7, 0xfa, 0xf8, 0xf5, 0xf9, 0xfc, 0xf3, 0xf6, 
	0x7e, 0x7d, 0x7b, 0x70, 0x72, 0x71, 0x74, 0x7f, 0x77, 0x7a, 0x78, 0x75, 0x79, 0x7c, 0x73, 0x76, 
	0xae, 0xad, 0xab, 0xa0, 0xa2, 0xa1, 0xa4, 0xaf, 0xa7, 0xaa, 0xa8, 0xa5, 0xa9, 0xac, 0xa3, 0xa6, 
	0x8e, 0x8d, 0x8b, 0x80, 0x82, 0x81, 0x84, 0x8f, 0x87, 0x8a, 0x88, 0x85, 0x89, 0x8c, 0x83, 0x86, 
	0x5e, 0x5d, 0x5b, 0x50, 0x52, 0x51, 0x54, 0x5f, 0x57, 0x5a, 0x58, 0x55, 0x59, 0x5c, 0x53, 0x56, 
	0x9e, 0x9d, 0x9b, 0x90, 0x92, 0x91, 0x94, 0x9f, 0x97, 0x9a, 0x98, 0x95, 0x99, 0x9c, 0x93, 0x96, 
	0xce, 0xcd, 0xcb, 0xc0, 0xc2, 0xc1, 0xc4, 0xcf, 0xc7, 0xca, 0xc8, 0xc5, 0xc9, 0xcc, 0xc3, 0xc6, 
	0x3e, 0x3d, 0x3b, 0x30, 0x32, 0x31, 0x34, 0x3f, 0x37, 0x3a, 0x38, 0x35, 0x39, 0x3c, 0x33, 0x36, 
	0x6e, 0x6d, 0x6b, 0x60, 0x62, 0x61, 0x64, 0x6f, 0x67, 0x6a, 0x68, 0x65, 0x69, 0x6c, 0x63, 0x66 
};

void PrintState(BYTE* state)
{
	for(int i = nSBox-1; i>=0; i--)
		printf("%02X ", state[i]);
	printf("\n");
}

BYTE lCounter(BYTE lfsr)
{
	lfsr = (lfsr << 1) | (((0x40 & lfsr) >> 6) ^ ((0x20 & lfsr) >> 5));
	lfsr &= 0x7f; 
	return lfsr;
}

BYTE retnuoCl(BYTE lfsr)
{
	return ((lfsr & 0x01) <<7) | ((lfsr & 0x02) << 5) | ((lfsr & 0x04) << 3)
		| ((lfsr & 0x08) << 1) | ((lfsr & 0x10) >> 1) | ((lfsr & 0x20) >> 3)
		| ((lfsr & 0x40) >> 5) | ((lfsr & 0x80) >> 7);		
}

int Pi(int i)
{
	if (i != nBits-1)
		return (i*nBits/4)%(nBits-1);
	else
		return nBits-1;
}

void pLayer(BYTE* state)
{
	int	PermutedBitNo;
	BYTE tmp[nSBox], x, y;
	
	for(int i = 0; i < nSBox; i++) tmp[i] = 0;
	
	for(int i = 0; i < nSBox; i++){
		for(int j = 0; j < 8; j++){ 
			x			= GET_BIT(state[i],j);
			PermutedBitNo	= Pi(8*i+j);
			y			= PermutedBitNo/8;
			tmp[y]		^= x << (PermutedBitNo - 8*y);
		}
	}	
	memcpy(state, tmp, nSBox);
}

void permutation(BYTE* state)
{
	BYTE IV = lfsrIV;
	BYTE INV_IV;

	for(int i = 0; i < nRounds; i++){
#ifdef _PrintState_
		printf("%3d\t", i);
		PrintState(state);
#endif
		/* Add counter values */
		state[0] ^= IV;
		INV_IV = retnuoCl(IV);
		state[nSBox-1] ^= INV_IV;
		IV	= lCounter(IV);
		
		/* sBoxLayer layer */
		for(int j = 0; j < nSBox; j++)	
			state[j] =  sBoxLayer[state[j]];
		
		/* pLayer */
		pLayer(state);
	}
#ifdef _PrintState_
	printf("%3d\t", i);
	PrintState(state);
#endif
}
