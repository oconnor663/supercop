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
 * Implementation of the multiple precision comparison functions.
 *
 * @version $Id: relic_bn_cmp.c,v 1.1 2011/08/12 19:27:15 diego Exp $
 * @ingroup bn
 */

#include "relic.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

int bn_cmp_abs(bn_t a, bn_t b) {
	int result;

	result = bn_cmpn_low(a->dp, b->dp, a->used);

	if (a->used > b->used) {
		return CMP_GT;
	}

	if (a->used < b->used) {
		return CMP_LT;
	}

	return result;
}

int bn_cmp_dig(bn_t a, dig_t b) {
	int result;

	if (a->sign == BN_NEG) {
		return CMP_LT;
	}

	if (a->used > 1) {
		return CMP_GT;
	}

	result = bn_cmp1_low(a->dp[0], b);

	return result;
}

int bn_cmp(bn_t a, bn_t b) {
	if (a->sign == BN_POS && b->sign == BN_NEG) {
		return CMP_GT;
	}
	if (a->sign == BN_NEG && b->sign == BN_POS) {
		return CMP_LT;
	}

	if (a->sign == BN_NEG) {
		return bn_cmp_abs(b, a);
	}

	return bn_cmp_abs(a, b);
}
