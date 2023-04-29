/**
 * @file prvhash42ec.h
 *
 * @brief The inclusion file for the "prvhash42_ec" endianness-correction
 * functions.
 *
 * @mainpage
 *
 * @section intro_sec Introduction
 *
 * Description is available at https://github.com/avaneev/prvhash
 *
 * @section license License
 *
 * Copyright (c) 2020 Aleksey Vaneev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * @version 2.31
 */

#ifndef PRVHASH42EC_INCLUDED
#define PRVHASH42EC_INCLUDED

#include <stdint.h>
#include <string.h>

#if defined( _WIN32 ) || defined( __LITTLE_ENDIAN__ ) || ( defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
	#define PRVHASH42_LITTLE_ENDIAN 1
#elif defined( __BIG_ENDIAN__ ) || ( defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
	#define PRVHASH42_LITTLE_ENDIAN 0
#else
	#error PRVHASH42: cannot obtain endianness
#endif

#if PRVHASH42_LITTLE_ENDIAN

/**
 * An auxiliary function that returns an unsigned 64-bit value created out of
 * individual bytes in a buffer. This function is used to preserve endianness
 * of supplied 64-bit unsigned values.
 *
 * @param p 8-byte buffer. Alignment is unimportant.
 */

inline uint64_t prvhash42_u64ec( const uint8_t* const p )
{
	uint64_t v;
	memcpy( &v, p, 8 );

	return( v );
}

/**
 * An auxiliary function that returns an unsigned 32-bit value created out of
 * individual bytes in a buffer. This function is used to preserve endianness
 * of supplied 32-bit unsigned values.
 *
 * @param p 4-byte buffer. Alignment is unimportant.
 */

inline uint32_t prvhash42_u32ec( const uint8_t* const p )
{
	uint32_t v;
	memcpy( &v, p, 4 );

	return( v );
}

#else // PRVHASH42_LITTLE_ENDIAN

#if defined( __GNUC__ ) || defined( __INTEL_COMPILER ) || defined( __clang__ )

inline uint64_t prvhash42_u64ec( const uint8_t* const p )
{
	uint64_t v;
	memcpy( &v, p, 8 );

	return( __builtin_bswap64( v ));
}

inline uint32_t prvhash42_u32ec( const uint8_t* const p )
{
	uint32_t v;
	memcpy( &v, p, 4 );

	return( __builtin_bswap32( v ));
}

#elif defined( _MSC_VER )

inline uint64_t prvhash42_u64ec( const uint8_t* const p )
{
	uint64_t v;
	memcpy( &v, p, 8 );

	return( _byteswap_uint64( v ));
}

inline uint32_t prvhash42_u32ec( const uint8_t* const p )
{
	uint32_t v;
	memcpy( &v, p, 4 );

	return( _byteswap_ulong( v ));
}

#endif // defined( _MSC_VER )

#endif // PRVHASH42_LITTLE_ENDIAN

#if PRVHASH42_LITTLE_ENDIAN

/**
 * This function corrects (inverses) endianness of the specified hash value,
 * based on 32-bit word.
 *
 * @param[in,out] Hash The hash to correct endianness of. On systems where
 * this is relevant, this address should be aligned to 32 bits.
 * @param HashLen The required hash length, in bytes, should be >= 4, in
 * increments of 4. 
 */

inline void prvhash42_ec32( uint8_t* const Hash, const int HashLen )
{
}

#else // PRVHASH42_LITTLE_ENDIAN

inline void prvhash42_ec32( uint8_t* const Hash, const int HashLen )
{
	int k;

	for( k = 0; k < HashLen; k += 4 )
	{
		*(uint32_t*) ( Hash + k ) = prvhash42_u32ec( Hash + k );
	}
}

#endif // PRVHASH42_LITTLE_ENDIAN

/**
 * Function loads 32-bit message word and pads it with the "final byte" if
 * reading occurs beyond message end.
 *
 * @param Msg Message pointer, alignment is unimportant.
 * @param MsgEnd Message's end pointer.
 * @param fb Final byte used for padding.
 */

inline uint32_t prvhash42_lp32( const uint8_t* Msg,
	const uint8_t* const MsgEnd, const uint8_t fb )
{
	if( Msg < MsgEnd - 3 )
	{
		return( prvhash42_u32ec( Msg ));
	}

	uint32_t r = ( Msg < MsgEnd ? *Msg : fb ) | (uint32_t) fb << 24;
	Msg++;
	r |= (uint32_t) ( Msg < MsgEnd ? *Msg : fb ) << 8;
	Msg++;
	r |= (uint32_t) ( Msg < MsgEnd ? *Msg : fb ) << 16;

	return( r );
}

/**
 * Function loads 32-bit message word and pads it with the "final byte" if
 * reading occurs beyond message end. This variant of the function assumes
 * that Msg < MsgEnd.
 *
 * @param Msg Message pointer, alignment is unimportant.
 * @param MsgEnd Message's end pointer.
 * @param fb Final byte used for padding.
 */

inline uint32_t prvhash42_lp32_1( const uint8_t* Msg,
	const uint8_t* const MsgEnd, const uint8_t fb )
{
	if( Msg < MsgEnd - 3 )
	{
		return( prvhash42_u32ec( Msg ));
	}

	uint32_t r = *Msg | (uint32_t) fb << 24;
	Msg++;
	r |= (uint32_t) ( Msg < MsgEnd ? *Msg : fb ) << 8;
	Msg++;
	r |= (uint32_t) ( Msg < MsgEnd ? *Msg : fb ) << 16;

	return( r );
}

/**
 * Function loads 64-bit message word and pads it with the "final byte" if
 * reading occurs beyond message end.
 *
 * @param Msg Message pointer, alignment is unimportant.
 * @param MsgEnd Message's end pointer.
 * @param fb Final byte used for padding.
 */

inline uint64_t prvhash42_lp64( const uint8_t* Msg,
	const uint8_t* const MsgEnd, const uint8_t fb )
{
	return( prvhash42_lp32( Msg, MsgEnd, fb ) |
		(uint64_t) prvhash42_lp32( Msg + 4, MsgEnd, fb ) << 32 );
}

/**
 * Function loads 64-bit message word and pads it with the "final byte" if
 * reading occurs beyond message end. This variant of the function assumes
 * that Msg < MsgEnd.
 *
 * @param Msg Message pointer, alignment is unimportant.
 * @param MsgEnd Message's end pointer.
 * @param fb Final byte used for padding.
 */

inline uint64_t prvhash42_lp64_1( const uint8_t* Msg,
	const uint8_t* const MsgEnd, const uint8_t fb )
{
	return( prvhash42_lp32_1( Msg, MsgEnd, fb ) |
		(uint64_t) prvhash42_lp32( Msg + 4, MsgEnd, fb ) << 32 );
}

#endif // PRVHASH42EC_INCLUDED
