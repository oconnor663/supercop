/*
 * RELIC is an Efficient LIbrary for Cryptography
 * Copyright (C) 2007-2011 RELIC Authors
 *
 * This file is part of RELIC. RELIC is legal property of its developers,
 * whose names are not listed here. Please refer to the COPYRIGHT file
 * for contact information.
 *
 * RELIC is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * RELIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RELIC. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Implementation of the multiple precision integer memory management routines.
 *
 * @version $Id: relic_bn_mem.c,v 1.1 2011/08/13 04:38:37 diego Exp $
 * @ingroup bn
 */

#include <stdlib.h>
#include <errno.h>

#if ALLOC != AUTO
#include <malloc.h>
#endif

#include "relic.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bn_init(bn_t a, int digits) {
#if ALLOC == DYNAMIC
	int r;

	if (digits % BN_SIZE != 0) {
		/* Pad the number of digits to a multiple of the block. */
		digits += (BN_SIZE - digits % BN_SIZE);
	}

	if (a != NULL && a->dp == NULL) {
#if ALIGN == 1
		a->dp = malloc(digits * sizeof(dig_t));
#elif OPSYS == WINDOWS
		a->dp = _aligned_malloc(digits * sizeof(dig_t), ALIGN);
#else
		r = posix_memalign((void **)&(a->dp), ALIGN, digits * sizeof(dig_t));
		if (r == ENOMEM) {
			THROW(ERR_NO_MEMORY);
		}
		if (r == EINVAL) {
			THROW(ERR_INVALID);
		}
#endif
	}

	if (a->dp == NULL) {
		free(a);
		THROW(ERR_NO_MEMORY);
	}

#else
	/* Verify if the number of digits is sane. */
	if (digits > BN_SIZE) {
		THROW(ERR_NO_PRECISION);
	} else {
		digits = BN_SIZE;
	}
#if ALLOC == STATIC
	if (a != NULL && a->dp == NULL) {
		a->dp = pool_get();
		if (a->dp == NULL) {
			THROW(ERR_NO_MEMORY);
		}
	}
#endif
#endif
	if (a != NULL) {
		a->used = 0;
		a->alloc = digits;
		a->sign = BN_POS;
	}
}

void bn_clean(bn_t a) {
#if ALLOC == DYNAMIC
	if (a != NULL) {
		if (a->dp != NULL) {
#if OPSYS == WINDOWS && ALIGN > 1
			_aligned_free(a->dp);
#else
			free(a->dp);
#endif
			a->dp = NULL;
		}
		a->alloc = 0;
	}
#endif
#if ALLOC == STATIC
	if (a != NULL && a->dp != NULL) {
		pool_put(a->dp);
		a->dp = NULL;
	}
#endif
	if (a != NULL) {
		a->used = 0;
		a->sign = BN_POS;
	}
}

void bn_grow(bn_t a, int digits) {
#if ALLOC == DYNAMIC
	dig_t *t;

	if (a->alloc < digits) {
		/* At least add BN_SIZE more digits. */
		digits += (BN_SIZE * 2) - (digits % BN_SIZE);
		t = (dig_t *)realloc(a->dp, sizeof(dig_t) * digits);
		if (t == NULL) {
			THROW(ERR_NO_MEMORY);
		}
		a->dp = t;
		/* Set the newly allocated digits to zero. */
		a->alloc = digits;
	}
#else /* ALLOC == STATIC || ALLOC == STACK */
	if (digits > BN_SIZE) {
		THROW(ERR_NO_PRECISION)
	}
	(void)a;
#endif
}

void bn_trim(bn_t a) {
	while (a->used > 0 && a->dp[a->used - 1] == 0) {
		--(a->used);
	}
	/* Zero can't be negative. */
	if (a->used <= 0) {
		a->used = 1;
		a->dp[0] = 0;
		a->sign = BN_POS;
	}
}
