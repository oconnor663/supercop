#include <string.h>
#include <stdlib.h>
#include "api.h"
#include "crypto_aead.h"

#include "simple.h"

int crypto_aead_encrypt(
unsigned char *c,unsigned long long *clen,
const unsigned char *m,unsigned long long mlen,
const unsigned char *ad,unsigned long long adlen,
const unsigned char *nsec,
const unsigned char *npub,
const unsigned char *k
){
	simple_encrypt(c, clen,
						m, mlen,
						ad, adlen,
						nsec,
						npub,
						k
	);
	return 0;
}


int crypto_aead_decrypt(
unsigned char *m,unsigned long long *mlen,
unsigned char *nsec,
const unsigned char *c, unsigned long long clen,
const unsigned char *ad, unsigned long long adlen,
const unsigned char *npub,
const unsigned char *k
)
{
	return simple_decrypt(m, mlen,
						nsec,
						c, clen,
						ad, adlen,
						npub,
						k
	);
}