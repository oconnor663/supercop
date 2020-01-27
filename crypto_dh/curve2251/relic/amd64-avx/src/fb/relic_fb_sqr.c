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
 * Implementation of the binary field squaring.
 *
 * @version $Id: relic_fb_sqr.c,v 1.1 2011/08/13 04:38:37 diego Exp $
 * @ingroup fb
 */

#include "relic.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

#if FB_SQR == BASIC || !defined(STRIP)

void fb_sqr_basic(fb_t c, fb_t a) {
	dv_t t;

	dv_null(t);

	TRY {
		/* We need a temporary variable so that c can be a or b. */
		dv_new(t);
		fb_sqrn_low(t, a);
		fb_rdc(c, t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		dv_free(t);
	}
}

#endif

#if FB_SQR == TABLE || !defined(STRIP)

void fb_sqr_table(fb_t c, fb_t a) {
	dv_t t;

	dv_null(t);

	TRY {
		/* We need a temporary variable so that c can be a or b. */
		dv_new(t);
		fb_sqrl_low(t, a);
		fb_rdc(c, t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		dv_free(t);
	}
}

#endif

#if FB_SQR == INTEG || !defined(STRIP)

void fb_sqr_integ(fb_t c, fb_t a) {
	fb_sqrm_low(c, a);
}

#endif
