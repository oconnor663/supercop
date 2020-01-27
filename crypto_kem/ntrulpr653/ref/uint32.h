#ifndef UINT32_H
#define UINT32_H

#define uint32_div_uint14 crypto_kem_ntrulpr653_ref_uint32_div_uint14
#define uint32_mod_uint14 crypto_kem_ntrulpr653_ref_uint32_mod_uint14
#define uint32_divmod_uint14 crypto_kem_ntrulpr653_ref_uint32_divmod_uint14

#include "uint16.h"

typedef uint32_t uint32;

/*
assuming 1 <= m < 16384:
q = uint32_div_uint14(x,m) means q = x/m
r = uint32_mod_uint14(x,m) means r = x/m
uint32_moddiv_uint14(&q,&r,x,m) means q = x/m, r = x%m
*/

extern uint32 uint32_div_uint14(uint32,uint16);
extern uint16 uint32_mod_uint14(uint32,uint16);
extern void uint32_divmod_uint14(uint32 *,uint16 *,uint32,uint16);

#endif
