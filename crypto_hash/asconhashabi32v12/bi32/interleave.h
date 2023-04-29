#ifndef INTERLEAVE_H_
#define INTERLEAVE_H_

#include <stdint.h>

#include "config.h"
#include "bendian.h"
#include "forceinline.h"

#if ASCON_INLINE_BI

#define TOBI deinterleave32
#define FROMBI interleave32

#else

uint64_t TOBI(uint64_t in);
uint64_t FROMBI(uint64_t in);

#endif

extern const uint32_t B[3];

forceinline uint32_t deinterleave16(uint32_t x) {
  uint32_t t;
  t = (x ^ (x >> 1)) & B[0], x ^= t ^ (t << 1);
  t = (x ^ (x >> 2)) & B[1], x ^= t ^ (t << 2);
  t = (x ^ (x >> 4)) & B[2], x ^= t ^ (t << 4);
  t = (x ^ (x >> 8)) & 0xff00, x ^= t ^ (t << 8);
  return x;
}

forceinline uint32_t interleave16(uint32_t x) {
  uint32_t t;
  t = (x ^ (x >> 8)) & 0xff00, x ^= t ^ (t << 8);
  t = (x ^ (x >> 4)) & B[2], x ^= t ^ (t << 4);
  t = (x ^ (x >> 2)) & B[1], x ^= t ^ (t << 2);
  t = (x ^ (x >> 1)) & B[0], x ^= t ^ (t << 1);
  return x;
}

/* credit to Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002 */
forceinline uint64_t deinterleave32(uint64_t in) {
  in = U64BIG(in);
  uint32_t hi = in >> 32;
  uint32_t lo = in;
  uint32_t t0, t1, e, o;
  t0 = deinterleave16(lo);
  t1 = deinterleave16(hi);
  e = (t1 << 16) | (t0 & 0x0000FFFF);
  o = (t1 & 0xFFFF0000) | (t0 >> 16);
  return (uint64_t)o << 32 | e;
}

/* credit to Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002 */
forceinline uint64_t interleave32(uint64_t in) {
  uint32_t e = in;
  uint32_t o = in >> 32;
  uint32_t t0, t1, lo, hi;
  t0 = (o << 16) | (e & 0x0000FFFF);
  t1 = (o & 0xFFFF0000) | (e >> 16);
  lo = interleave16(t0);
  hi = interleave16(t1);
  return U64BIG((uint64_t)hi << 32 | lo);
}

#endif /* INTERLEAVE_H_ */
