/*
 * This file has been automatically generated, do not edit directly.
 * Calling function: FpGen::gen_gfp_k(25519,4,9)
 */

#ifndef __MPFQ_P_25519_H__ 
#define __MPFQ_P_25519_H__ 
 
#include <gmp.h> 
#include "fixmp.h" 
#include "mpfq_p_common.h" 

/**** Typedefs ****/ 
 
// fields 
typedef mpfq_p_field mpfq_p_25519_field; 
typedef mpfq_p_src_field mpfq_p_25519_src_field; 
typedef mpfq_p_dst_field mpfq_p_25519_dst_field; 
 
// elements 
typedef mp_limb_t mpfq_p_25519_elt[4]; 
typedef mp_limb_t * mpfq_p_25519_dst_elt; 
typedef const mp_limb_t * mpfq_p_25519_src_elt; 
 
// unreduced elements 
typedef mp_limb_t mpfq_p_25519_elt_ur[9]; 
typedef mp_limb_t * mpfq_p_25519_dst_elt_ur; 
typedef const mp_limb_t * mpfq_p_25519_src_elt_ur; 

/**** Prototypes ****/ 

// External --- functions for which a function call does not hurt 


void mpfq_p_25519_field_tricks(mpfq_p_25519_dst_field k, mp_limb_t flags);
void mpfq_p_25519_field_clear(mpfq_p_25519_dst_field k);
void mpfq_p_25519_write(mpfq_p_25519_src_field k, char *str, mpfq_p_25519_src_elt x, int base);
int mpfq_p_25519_read(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt x, const char *str, int base);
void mpfq_p_25519_print(mpfq_p_25519_src_field k, mpfq_p_25519_src_elt x);
int mpfq_p_25519_inv(mpfq_p_25519_dst_field k, mpfq_p_25519_dst_elt z, mpfq_p_25519_src_elt x);
void mpfq_p_25519_random(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r);
void mpfq_p_25519_random2(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r);
void mpfq_p_25519_field_init(mpfq_p_25519_dst_field k, mp_limb_t *p);

// Inlined 


static inline void mpfq_p_25519_init(mpfq_p_25519_src_field k, mpfq_p_25519_elt * x);
static inline void mpfq_p_25519_clear(mpfq_p_25519_src_field k, mpfq_p_25519_elt * x);
static inline int mpfq_p_25519_cmp(mpfq_p_25519_src_field k, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y);
static inline void mpfq_p_25519_copy(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt x);
static inline void mpfq_p_25519_normalize(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt x);
static inline void mpfq_p_25519_elt_ur_init(mpfq_p_25519_src_field k, mpfq_p_25519_elt_ur * x);
static inline void mpfq_p_25519_elt_ur_clear(mpfq_p_25519_src_field k, mpfq_p_25519_elt_ur * x);
static inline void mpfq_p_25519_elt_ur_copy(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur x);
static inline void mpfq_p_25519_mul_ur(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt s1, mpfq_p_25519_src_elt s2);
static inline void mpfq_p_25519_sqr_ur(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt s1);
static inline void mpfq_p_25519_elt_ur_add(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur s1, mpfq_p_25519_src_elt_ur s2);
static inline void mpfq_p_25519_elt_ur_sub(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur s1, mpfq_p_25519_src_elt_ur s2);
static inline void mpfq_p_25519_mul(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1, mpfq_p_25519_src_elt s2);
static inline void mpfq_p_25519_mul_ui(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1, mp_limb_t c);
static inline void mpfq_p_25519_sqr(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1);
static inline void mpfq_p_25519_reduce(mpfq_p_25519_src_field k, mp_limb_t *z, mp_limb_t *x);
static inline void mpfq_p_25519_elt_ur_reduce(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_dst_elt_ur x);
static inline void mpfq_p_25519_add(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt z, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y);
static inline void mpfq_p_25519_sub(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt z, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y);

/**** Definitions of inlined functions ****/ 


static inline void mpfq_p_25519_init(mpfq_p_25519_src_field k, mpfq_p_25519_elt * x) {
  assert (k); assert (k->p); assert (*x);
}

static inline void mpfq_p_25519_clear(mpfq_p_25519_src_field k, mpfq_p_25519_elt * x) {
  assert (*x);
}

static inline int mpfq_p_25519_cmp(mpfq_p_25519_src_field k, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y) {
  return cmp_4(x, y);
}

static inline void mpfq_p_25519_copy(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt x) {
  int i; 
  assert (r);
  assert (x);
  for (i = 0; i < 4; ++i)
    r[i] = x[i];
}

static inline void mpfq_p_25519_normalize(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt x) {
  if (cmp_4(x,k->p)>=0) {
    mp_limb_t q[4+1];
    mpfq_p_25519_elt r;

    mpn_tdiv_qr(q, r, 0, x, 4, k->p, 4);
    mpfq_p_25519_copy(k, x, r);
  }
}

static inline void mpfq_p_25519_elt_ur_init(mpfq_p_25519_src_field k, mpfq_p_25519_elt_ur * x) {
  assert (k); assert (k->p); assert (*x);
}

static inline void mpfq_p_25519_elt_ur_clear(mpfq_p_25519_src_field k, mpfq_p_25519_elt_ur * x) {
  assert (*x);
}

static inline void mpfq_p_25519_elt_ur_copy(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur x) {
  int i; 
  assert (r);
  assert (x);
  for (i = 0; i < 9; ++i)
    r[i] = x[i];
}

static inline void mpfq_p_25519_mul_ur(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt s1, mpfq_p_25519_src_elt s2) {
  mul_4(r, s1, s2);
  r[9-1] = 0UL;
}

static inline void mpfq_p_25519_sqr_ur(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt s1) {
  sqr_4(r, s1);
  r[9-1] = 0UL;
}

static inline void mpfq_p_25519_elt_ur_add(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur s1, mpfq_p_25519_src_elt_ur s2) {
  mpn_add_n(r, s1, s2, 9);
} 

static inline void mpfq_p_25519_elt_ur_sub(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt_ur r, mpfq_p_25519_src_elt_ur s1, mpfq_p_25519_src_elt_ur s2) {
  mpn_sub_n(r, s1, s2, 9);
}

static inline void mpfq_p_25519_mul(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1, mpfq_p_25519_src_elt s2){
  mp_limb_t tmp[2*4];
  mul_4(tmp, s1, s2);
  mpfq_p_25519_reduce(k, r, tmp);
}

static inline void mpfq_p_25519_mul_ui(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1, mp_limb_t c){
  mp_limb_t tmp[2*4];
  int i;
  mul1_4(tmp, s1, c);
  for (i = 4+1; i < 2*4; ++i)
    tmp[i] = 0;
  mpfq_p_25519_reduce(k, r, tmp);
}

static inline void mpfq_p_25519_sqr(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_src_elt s1){
  mp_limb_t tmp[2*4];
  sqr_4(tmp, s1);
  mpfq_p_25519_reduce(k, r, tmp);
}

static inline void mpfq_p_25519_reduce(mpfq_p_25519_src_field k, mp_limb_t *z, mp_limb_t *x) {
// #ifndef __x86_64__
#if 1
  mp_limb_t tmp[4+1];
  mp_limb_t c;
  int i;
  for (i = 0; i < 4; ++i)
    tmp[i] = x[i];
  tmp[4] = 0UL;
  addmul1_4(tmp, x+4, 38UL);
  
  c = (tmp[4] << 1) | (tmp[4-1] >> (GMP_LIMB_BITS-1));
  c *= 19UL;

  tmp[4-1] &= ((-1UL) >> 1);  // kill last bit.
  c = mpn_add_1(z, tmp, 4, c);
  assert (c == 0UL);

  if (cmp_4(z,k->p)>=0)
    sub_4(z, z, k->p);
#else
  __asm__ volatile (
    "	### Copy x[0-3] into [r8,r9,r10,r11] and do addmul1\n"
    "	movq	%1, %%r13\n"
    "	movq	(%%r13), %%r8\n"
    "	movq	8(%%r13), %%r9\n"
    "	movq	16(%%r13), %%r10\n"
    "	movq	24(%%r13), %%r11\n"
    "	movq	32(%%r13), %%r12\n"
    "	movq	$38, %%r14\n"
    "	movq	%%r12, %%rax\n"
    "	mulq	%%r14\n"
    "	addq	%%rax, %%r8\n"
    "	movq	40(%%r13), %%rax\n"
    "	adcq	$0, %%rdx\n"
    "	movq	%%rdx, %%r15\n"
    
    "	mulq	%%r14\n"
    "	addq	%%rax, %%r15\n"
    "	adcq	$0, %%rdx\n"
    "	movq	48(%%r13), %%rax\n"
    "	addq	%%r15, %%r9\n"
    "	adcq	$0, %%rdx\n"
    "	movq	%%rdx, %%r15\n"

    "	mulq	%%r14\n"
    "	addq	%%rax, %%r15\n"
    "	adcq	$0, %%rdx\n"
    "	movq	56(%%r13), %%rax\n"
    "	addq	%%r15, %%r10\n"
    "	adcq	$0, %%rdx\n"
    "	movq	%%rdx, %%r15\n"

    "	mulq	%%r14\n"
    "	addq	%%rax, %%r15\n"
    "	adcq	$0, %%rdx\n"
    "	addq	%%r15, %%r11\n"
    "	adcq	$0, %%rdx\n"
    "	movq	%%rdx, %%r12\n"

    "	### At this point [r8,r9,r10,r11,r12] contains (semireduced) x\n"
    "	movq	%%r11, %%rax\n"
    "	shlq	$1, %%r12\n"
    "	movq	$9223372036854775807, %%rdx\n"
    "	movq	$19, %%r15\n"
    "	shrq	$63, %%rax\n"
    "	or	%%r12, %%rax\n"
    "	andq	%%rdx, %%r11\n"
    "	mulq	%%r15\n"
    "	addq	%%rax, %%r8\n"
    "	adcq	$0, %%r9\n"
    "	adcq	$0, %%r10\n"
    "	adcq	$0, %%r11\n"

    "	### At this point tmp = [r8,r9,r10,r11] contains (semireduced) x\n"
    "	### (it might still be necessary to subtract p once)\n"
    "   ### Subtract p to tmp and put a copy of tmp in tmp2=[r12,r13,r14,r15]\n"
    "   ### (in fact, add 2^256-p)\n"
    "	movq	%%r8, %%r12\n"
    "	addq	$19, %%r8\n"
    "	movq	%%r9, %%r13\n"
    "	adcq	$0, %%r9\n"
    "	movq	%%r10, %%r14\n"
    "	adcq	$0, %%r10\n"
    "	movq	$9223372036854775808, %%rax\n"
    "	movq	%%r11, %%r15\n"
    "	adcq	%%rax, %%r11\n"
    
    "	### CMOVS and copy to result\n"
    "	movq	%0, %%rax\n"
    "	cmovnc	%%r12, %%r8\n"
    "	cmovnc	%%r13, %%r9\n"
    "	cmovnc	%%r14, %%r10\n"
    "	cmovnc	%%r15, %%r11\n"
    "	movq	%%r8, (%%rax)\n"
    "	movq	%%r9, 8(%%rax)\n"
    "	movq	%%r10, 16(%%rax)\n"
    "	movq	%%r11, 24(%%rax)\n"
  : "+m" (z)
  : "m" (x)
  : "%rax", "%rdx", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "memory");
#endif
}

static inline void mpfq_p_25519_elt_ur_reduce(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt r, mpfq_p_25519_dst_elt_ur x) {
  mp_limb_t q[9+1];
  // TODO: That one can be very much optimized!
  if (x[9-1]>>(GMP_LIMB_BITS-1)) {
    // negative number, add bigmul_p to make it positive before reduction
    mpn_add_n(x, x, k->bigmul_p, 9);
  }
  mpn_tdiv_qr(q, r, 0, x, 9, k->p, 4);
}

static inline void mpfq_p_25519_add(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt z, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y){
#ifdef __x86_64__
  __asm__ volatile (
    "   ### Add s1 and s2 to tmp=[r8,r9,r10,r11]\n"
    "	movq	%1, %%rax\n"
    "	movq	%2, %%r15\n"
    "	movq	(%%rax), %%r8\n"
    "	addq	(%%r15), %%r8\n"
    "	movq	8(%%rax), %%r9\n"
    "	adcq	8(%%r15), %%r9\n"
    "	movq	16(%%rax), %%r10\n"
    "	adcq	16(%%r15), %%r10\n"
    "	movq	24(%%rax), %%r11\n"
    "	adcq	24(%%r15), %%r11\n"

    "   ### Subtract p to tmp and put a copy of tmp in tmp2=[r12,r13,r14,r15]\n"
    "   ### (in fact, add 2^256-p)\n"
    "	movq	%%r8, %%r12\n"
    "	addq	$19, %%r8\n"
    "	movq	%%r9, %%r13\n"
    "	adcq	$0, %%r9\n"
    "	movq	%%r10, %%r14\n"
    "	adcq	$0, %%r10\n"
    "	movq	$9223372036854775808, %%rax\n"
    "	movq	%%r11, %%r15\n"
    "	adcq	%%rax, %%r11\n"
    
    "	### CMOVS and copy to result\n"
    "	movq	%0, %%rax\n"
    "	cmovnc	%%r12, %%r8\n"
    "	cmovnc	%%r13, %%r9\n"
    "	cmovnc	%%r14, %%r10\n"
    "	cmovnc	%%r15, %%r11\n"
    "	movq	%%r8, (%%rax)\n"
    "	movq	%%r9, 8(%%rax)\n"
    "	movq	%%r10, 16(%%rax)\n"
    "	movq	%%r11, 24(%%rax)\n"
  : "+m" (z)
  : "m" (x), "m" (y)
  : "%rax", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "memory");
#else
  add_4(z, x, y);
  if (cmp_4(z, k->p)>=0) 
    sub_4(z, z, k->p);
#endif
}

static inline void mpfq_p_25519_sub(mpfq_p_25519_src_field k, mpfq_p_25519_dst_elt z, mpfq_p_25519_src_elt x, mpfq_p_25519_src_elt y){
#ifdef __x86_64__
  __asm__ volatile (
    "   ### Sub y to x and put result in tmp=[r8,r9,r10,r11]\n"
    "	movq	%1, %%rax\n"
    "	movq	%2, %%r15\n"
    "	xorq	%%r12, %%r12\n"
    "	xorq	%%r13, %%r13\n"
    "	xorq	%%r14, %%r14\n"
    "	movq	(%%rax), %%r8\n"
    "	subq	(%%r15), %%r8\n"
    "	movq	8(%%rax), %%r9\n"
    "	sbbq	8(%%r15), %%r9\n"
    "	movq	16(%%rax), %%r10\n"
    "	sbbq	16(%%r15), %%r10\n"
    "	movq	24(%%rax), %%r11\n"
    "	sbbq	24(%%r15), %%r11\n"
    "	### Get p or 0 according to carry\n"
    "	movq	$0, %%r15\n"
    "	movq	%3, %%rax\n"
    "	cmovc	(%%rax), %%r12\n"
    "	cmovc	8(%%rax), %%r13\n"
    "	cmovc	16(%%rax), %%r14\n"
    "	cmovc	24(%%rax), %%r15\n"

    "   ### add p or 0\n"
    "	movq	%0, %%rax\n"
    "	addq	%%r12, %%r8\n"
    "	adcq	%%r13, %%r9\n"
    "	adcq	%%r14, %%r10\n"
    "	adcq	%%r15, %%r11\n"
    "	movq	%%r8, (%%rax)\n"
    "	movq	%%r9, 8(%%rax)\n"
    "	movq	%%r10, 16(%%rax)\n"
    "	movq	%%r11, 24(%%rax)\n"
  : "+m" (z)
  : "m" (x), "m" (y), "m" (k->p)
  : "%rax", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "memory");
#else
  sub_4(z, x, y);
  if (z[4-1]>>(GMP_LIMB_BITS-1))
    sub_4(z, z, k->p);
#endif
}


#endif /* __MPFQ_P_25519_H__ */ 
