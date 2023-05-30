//Huseyin Hisil, 2012.

#define NOXGCD

#ifdef NOXGCD
#include "_core.h"
#include "finite128.h"

void fp_inv_2e128mc_x8664(uni zn, uni_t prm, uni an){
	int i, j, ei;
	uni_t n01[FP_LEN] = {2, 0}, n02[FP_LEN] = {1, 0}, n03[FP_LEN] = {0, 0}, n04[FP_LEN], n05[FP_LEN], n06[FP_LEN];

	fp_sub_2e128mc_x8664(n01, prm, n03, n01);

	fp_sqr_2e128mc_x8664(n03, prm, NULL, an);
	fp_mul_2e128mc_x8664(n03, prm, NULL, n03, an);
	fp_sqr_2e128mc_x8664(n04, prm, NULL, n03);
	fp_sqr_2e128mc_x8664(n04, prm, NULL, n04);
	fp_mul_2e128mc_x8664(n04, prm, NULL, n04, n03);
	fp_sqr_2e128mc_x8664(n05, prm, NULL, n04);
	fp_sqr_2e128mc_x8664(n05, prm, NULL, n05);
	fp_sqr_2e128mc_x8664(n05, prm, NULL, n05);
	fp_sqr_2e128mc_x8664(n05, prm, NULL, n05);
	fp_mul_2e128mc_x8664(n05, prm, NULL, n05, n04);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n05);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n06, prm, NULL, n06);
	fp_mul_2e128mc_x8664(n06, prm, NULL, n06, n05);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n06);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_mul_2e128mc_x8664(n02, prm, NULL, n02, n06);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_mul_2e128mc_x8664(n02, prm, NULL, n02, n06);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
	fp_mul_2e128mc_x8664(n02, prm, NULL, n02, n06);

	fp_cnt_bits(&i, n01, 1);
	for(j = i; j > 0; j--){
		mam_ith_bit(ei, n01, j);
		fp_sqr_2e128mc_x8664(n02, prm, NULL, n02);
		if(ei == 1){
			fp_mul_2e128mc_x8664(n02, prm, NULL, n02, an);
		}
	}
	fp_cpy_2e128mc_x8664(zn, n02);
}

#else

#include "kernel.h"
#include "multiprecision.h"
#include "multiprecision_stack.h"
#include "finite128.h"

void fp_inv_2e128mc_x8664(uni zn, uni_t prm, uni an){
	MI_t z, a, p;
	int i;
	uni_t pn[2];

	/*Stack only version of inversion.*/
	pn[0] = 0 - prm; pn[1] = 0 - 1;
	p->v->n = (uni)pn; p->v->l = FP_LEN; p->s = POSITIVE;
	z->v->n = (uni)zn;
	a->v->n = (uni)an; a->s = POSITIVE;
	for(i = FP_LEN; (a->v->n[i - 1] == 0) && (i > 0); i--);
	a->v->l = i;
	mi_modinv_stack(z, a, p);
	for(i = z->v->l; i < FP_LEN; i++) zn[i] = 0;
}

#endif
