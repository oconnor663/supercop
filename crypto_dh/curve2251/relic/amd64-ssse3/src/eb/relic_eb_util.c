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
 * Implementation of the binary elliptic curve utilities.
 *
 * @version $Id: relic_eb_util.c,v 1.1 2011/08/12 19:27:17 diego Exp $
 * @ingroup eb
 */

#include "relic.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

int eb_is_infty(eb_t p) {
	return (fb_is_zero(p->z) == 1);
}

void eb_set_infty(eb_t p) {
	fb_zero(p->x);
	fb_zero(p->y);
	fb_zero(p->z);
	p->norm = 1;
}

void eb_copy(eb_t r, eb_t p) {
	fb_copy(r->x, p->x);
	fb_copy(r->y, p->y);
	fb_copy(r->z, p->z);
	r->norm = p->norm;
}

int eb_cmp(eb_t p, eb_t q) {
	if (fb_cmp(p->x, q->x) != CMP_EQ) {
		return CMP_NE;
	}

	if (fb_cmp(p->y, q->y) != CMP_EQ) {
		return CMP_NE;
	}

	if (fb_cmp(p->z, q->z) != CMP_EQ) {
		return CMP_NE;
	}

	return CMP_EQ;
}

void eb_print(eb_t p) {
	fb_print(p->x);
	fb_print(p->y);
	fb_print(p->z);
}
