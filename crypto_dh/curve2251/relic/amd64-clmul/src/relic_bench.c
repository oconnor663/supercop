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
 * Implementation of useful benchmark routines.
 *
 * @version $Id: relic_bench.c,v 1.1 2011/08/12 19:27:15 diego Exp $
 * @ingroup relic
 */

#include <stdio.h>
#include <string.h>

#include "relic.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Timer type.
 */
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD

#include <sys/time.h>
#include <time.h>
typedef struct timespec bench_t;

#elif TIMER == ANSI

#include <time.h>
typedef clock_t bench_t;

#elif TIMER == POSIX

#include <sys/time.h>
typedef struct timeval bench_t;

#elif TIMER == CYCLE

typedef unsigned long long bench_t;
#if ARCH == X86
static inline bench_t cycles(void) {
	unsigned long long int x;
	asm volatile (".byte 0x0f, 0x31\n\t":"=A" (x));
	return x;
}
#elif ARCH == X86_64
static inline bench_t cycles(void) {
	unsigned int hi, lo;
	asm volatile ("rdtsc\n\t":"=a" (lo), "=d"(hi));
	return ((bench_t) lo) | (((bench_t) hi) << 32);
}
#elif ARCH == MSP
#include "msp430util.h"
#define cycles()		msp430_get_cycles()
#endif

#else

typedef unsigned long long bench_t;

#endif

/**
 * Shared parameter for these timer.
 */
#if TIMER == HREAL
#define CLOCK			CLOCK_REALTIME
#elif TIMER == HPROC
#define CLOCK			CLOCK_PROCESS_CPUTIME_ID
#elif TIMER == HTHRD
#define CLOCK			CLOCK_THREAD_CPUTIME_ID
#else
#define CLOCK			NULL
#endif

/**
 * Stores the time measured before the execution of the benchmark.
 */
static bench_t before;

/**
 * Stores the time measured after the execution of the benchmark.
 */
static bench_t after;

/**
 * Stores the sum of timings for the current benchmark.
 */
static long long total;

/**
 * Benchmarking overhead to be measured and subtracted from benchmarks.
 */
static long long overhead = 0;

/**
 * Dummy function for measuring benchmarking overhead.
 *
 * @param a				- the dummy parameter.
 */
static void empty(int *a) {
	(*a)++;
}

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bench_reset() {
#if TIMER != NONE
	total = 0;
#else
	(void)before;
	(void)after;
	(void)overhead;
	(void)empty;
#endif
}

void bench_before() {
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD
	clock_gettime(CLOCK, &before);
#elif TIMER == ANSI
	before = clock();
#elif TIMER == POSIX
	gettimeofday(&before, NULL);
#elif TIMER == CYCLE
	before = cycles();
#endif
}

void bench_after() {
	long long result;
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD
	clock_gettime(CLOCK, &after);
	result = ((long)after.tv_sec - (long)before.tv_sec) * 1000000000;
	result += (after.tv_nsec - before.tv_nsec);
#elif TIMER == ANSI
	after = clock();
	result = (after - before) * 1000000000 / CLOCKS_PER_SEC;
#elif TIMER == POSIX
	gettimeofday(&after, NULL);
	result = ((long)after.tv_sec - (long)before.tv_sec) * 1000000000;
	result += (after.tv_usec - before.tv_usec) * 1000;
#elif TIMER == CYCLE
	after = cycles();
	result = (after - before);
#endif

#if TIMER != NONE
	total += result;
#else
	(void)result;
#endif
}

void bench_compute(int benches) {
#if TIMER != NONE
	total = total / benches - overhead;
#else
	(void)benches;
#endif
}

void bench_print() {
#if TIMER == CYCLE
	util_print("%lld cycles", total);
#else
	util_print("%lld nanosec", total);
#endif
	if (total < 0) {
		util_print(" (bad overhead estimation)\n");
	} else {
		util_print("\n");
	}
}

unsigned long long bench_total() {
	return total;
}
