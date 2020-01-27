#include "api.h"
#include "randombytes.h"
#include "gls254.h"
#include "tables.h"

#include "curves.incl"
#include "low.incl"
#include "ec.incl"
#include "scmul.incl"

int crypto_dh_gls254_ref_keypair(unsigned char *pk, unsigned char *sk) {
	Curve curve;
	curves_domParam(&curve, GLS254);
	
	//EBATS GENERATOR
	randombytes(sk, 32);
	sk[31] = sk[31] & 0x1F;

	//generate public-key
	scmul_end_halv_direct((ui64*) &pk[0], (ui64*) &pk[32], curve.Gx, curve.Gy, (ui64*) sk, &curve);

	return 0;
}

int crypto_dh_gls254_ref(unsigned char *out, const unsigned char *pk, const unsigned char *sk) {
	Curve curve;
	curves_domParam(&curve, GLS254);
	
	scmul_end_halv_direct((ui64*) &out[0], (ui64*) &out[32], (ui64*) &pk[0], (ui64*) &pk[32], (ui64*) sk, &curve);

	return 0;
}

