#ifndef _UINT32_UNPACK_BIG____
#define _UINT32_UNPACK_BIG____

#include "crypto_uint32.h"

#define uint32_unpack_big crypto_scalarmult_nistp256_mj32_uint32_unpack_big

extern crypto_uint32 uint32_unpack_big(const unsigned char *);

#endif
