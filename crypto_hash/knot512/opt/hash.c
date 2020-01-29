#include <stdio.h>
#include "api.h"
#include"crypto_hash.h"

typedef unsigned char u8;
typedef unsigned long long u64;
typedef long long i64;

#define PRH_ROUNDS 140
#define LITTLE_ENDIAN
//#define BIG_ENDIAN

#define RATE (64 / 8)
#define ROTR(x,n) (((x)>>(n))|((x)<<(64-(n))))

#ifdef BIG_ENDIAN
#define EXT_BYTE(x,n) ((u8)((u64)(x)>>(8*(n))))
#define INS_BYTE(x,n) ((u64)(x)<<(8*(n)))
#define U64BIG(x) (x)
#endif

#ifdef LITTLE_ENDIAN
#define EXT_BYTE(x,n) ((u8)((u64)(x)>>(8*(7-(n)))))
#define INS_BYTE(x,n) ((u64)(x)<<(8*(7-(n))))
#define U64BIG(x) \
    ((ROTR(x, 8) & (0xFF000000FF000000ULL)) | \
     (ROTR(x,24) & (0x00FF000000FF0000ULL)) | \
     (ROTR(x,40) & (0x0000FF000000FF00ULL)) | \
     (ROTR(x,56) & (0x000000FF000000FFULL)))
#endif

static const u8 constant8[255] = { 0x01, 0x02, 0x04, 0x08, 0x11, 0x23, 0x47,
		0x8e, 0x1c, 0x38, 0x71, 0xe2, 0xc4, 0x89, 0x12, 0x25, 0x4b, 0x97, 0x2e,
		0x5c, 0xb8, 0x70, 0xe0, 0xc0, 0x81, 0x03, 0x06, 0x0c, 0x19, 0x32, 0x64,
		0xc9, 0x92, 0x24, 0x49, 0x93, 0x26, 0x4d, 0x9b, 0x37, 0x6e, 0xdc, 0xb9,
		0x72, 0xe4, 0xc8, 0x90, 0x20, 0x41, 0x82, 0x05, 0x0a, 0x15, 0x2b, 0x56,
		0xad, 0x5b, 0xb6, 0x6d, 0xda, 0xb5, 0x6b, 0xd6, 0xac, 0x59, 0xb2, 0x65,
		0xcb, 0x96, 0x2c, 0x58, 0xb0, 0x61, 0xc3, 0x87, 0x0f, 0x1f, 0x3e, 0x7d,
		0xfb, 0xf6, 0xed, 0xdb, 0xb7, 0x6f, 0xde, 0xbd, 0x7a, 0xf5, 0xeb, 0xd7,
		0xae, 0x5d, 0xba, 0x74, 0xe8, 0xd1, 0xa2, 0x44, 0x88, 0x10, 0x21, 0x43,
		0x86, 0x0d, 0x1b, 0x36, 0x6c, 0xd8, 0xb1, 0x63, 0xc7, 0x8f, 0x1e, 0x3c,
		0x79, 0xf3, 0xe7, 0xce, 0x9c, 0x39, 0x73, 0xe6, 0xcc, 0x98, 0x31, 0x62,
		0xc5, 0x8b, 0x16, 0x2d, 0x5a, 0xb4, 0x69, 0xd2, 0xa4, 0x48, 0x91, 0x22,
		0x45, 0x8a, 0x14, 0x29, 0x52, 0xa5, 0x4a, 0x95, 0x2a, 0x54, 0xa9, 0x53,
		0xa7, 0x4e, 0x9d, 0x3b, 0x77, 0xee, 0xdd, 0xbb, 0x76, 0xec, 0xd9, 0xb3,
		0x67, 0xcf, 0x9e, 0x3d, 0x7b, 0xf7, 0xef, 0xdf, 0xbf, 0x7e, 0xfd, 0xfa,
		0xf4, 0xe9, 0xd3, 0xa6, 0x4c, 0x99, 0x33, 0x66, 0xcd, 0x9a, 0x35, 0x6a,
		0xd4, 0xa8, 0x51, 0xa3, 0x46, 0x8c, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x07,
		0x0e, 0x1d, 0x3a, 0x75, 0xea, 0xd5, 0xaa, 0x55, 0xab, 0x57, 0xaf, 0x5f,
		0xbe, 0x7c, 0xf9, 0xf2, 0xe5, 0xca, 0x94, 0x28, 0x50, 0xa1, 0x42, 0x84,
		0x09, 0x13, 0x27, 0x4f, 0x9f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0,
		0xe1, 0xc2, 0x85, 0x0b, 0x17, 0x2f, 0x5e, 0xbc, 0x78, 0xf1, 0xe3, 0xc6,
		0x8d, 0x1a, 0x34, 0x68, 0xd0, 0xa0, 0x40, 0x80 };
#define sbox(a, b, c, d, e, f, g, h)                                                                            \
{                                                                                                                             \
	t1 = ~a; t2 = b & t1;t3 = c ^ t2; h = d ^ t3; t5 = b | c; t6 = d ^ t1; g = t5 ^ t6; t8 = b ^ d; t9 = t3 & t6; e = t8 ^ t9; t11 = g & t8; f = t3 ^ t11; \
}

#define ARR_SIZE(a) (sizeof((a))/sizeof((a[0])))
#define LOTR1281(a,b,n) (((a)<<(n))|((b)>>(64-n)))
#define LOTR1282(a,b,n) (((b)<<(n))|((a)>>(64-n)))

#define ROUND512(i) ({\
		x31^=constant8[i];\
		sbox(x30, x20, x10, x00, b30, b20, b10, b00);\
		sbox(x31, x21, x11, x01, b31, b21, b11, b01);\
		x30=b30;\
		x20=LOTR1281(b20,b21,1);\
		x10=LOTR1281(b10,b11,16);\
		x00=LOTR1281(b00,b01,25);\
		x31=b31;\
		x21=LOTR1282(b20,b21,1);\
		x11=LOTR1282(b10,b11,16);\
		x01=LOTR1282(b00,b01,25);\
})
int crypto_hash(unsigned char *out, const unsigned char *in,
		unsigned long long inlen) {

	u64 b01, b11, b21, b31, b00, b10, b20, b30;
	u64 t1, t2, t3, t5, t6, t8, t9, t11;
	u64 x30 = 0, x20 = 0, x10 = 0, x00 = 0, x31 = 0, x21 = 0, x11 = 0, x01 = 0;
	u64 rlen, i;

	// initialization
	//absorb
	if (inlen > 0) {
		rlen = inlen;
		//RATE=8
		while (rlen >= RATE) {
			x00 ^= U64BIG(*(u64* )in);
			for (i = 0; i < PRH_ROUNDS; i++) {
				ROUND512(i);
			}

			rlen -= RATE;
			in += RATE;
		}
		for (i = 0; i < rlen; ++i, ++in)
			x00 ^= INS_BYTE(*in, i);
		x00 ^= INS_BYTE(0x80, rlen);
		for (i = 0; i < PRH_ROUNDS; i++) {
			ROUND512(i);
		}
	}
	//sequeez

	((u64*) out)[0] = U64BIG(x00);
	((u64*) out)[1] = U64BIG(x01);
	((u64*) out)[2] = U64BIG(x10);
	((u64*) out)[3] = U64BIG(x11);
	out += CRYPTO_BYTES / 2;
	for (i = 0; i < PRH_ROUNDS; i++) {
		ROUND512(i);
	}
	((u64*) out)[0] = U64BIG(x00);
	((u64*) out)[1] = U64BIG(x01);
	((u64*) out)[2] = U64BIG(x10);
	((u64*) out)[3] = U64BIG(x11);
	return 0;
}

