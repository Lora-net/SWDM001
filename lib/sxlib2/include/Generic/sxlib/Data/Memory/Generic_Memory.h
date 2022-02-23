/**
 * \file
 *
 * \brief Generic macros facilitating memory access, endianness conversion, and static variable declaration
 */

/**
 * \file
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_DATA_MEMORY_GENERIC_MEMORY_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_DATA_MEMORY_GENERIC_MEMORY_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/** Extract least-significant byte from a wider integer type */
#define SXLIB_BYTE0( x ) ( ( uint8_t )( ( x ) &0xFF ) )

/** Extract second-least-significant byte from a wider integer type */
#define SXLIB_BYTE1( x ) ( ( uint8_t )( ( ( x ) >> 8 ) & 0xFF ) )

/** Extract third-least-significant byte from a wider integer type */
#define SXLIB_BYTE2( x ) ( ( uint8_t )( ( ( x ) >> 16 ) & 0xFF ) )

/** Extract fourth-least-significant byte from a wider integer type */
#define SXLIB_BYTE3( x ) ( ( uint8_t )( ( ( x ) >> 24 ) & 0xFF ) )

/** Provide a big-endian comma-separated pair from a uint16_t or wider */
#define SXLIB_BE_PAIR( x ) SXLIB_BYTE1( x ), SXLIB_BYTE0( x )

/** Select a byte from a given offset out of an array */
#define SXLIB_MEMBYTE( addr, offset ) ( ( ( uint8_t* ) addr )[offset] )

/** Standard big-endian to host-order conversion */
#define SXLIB_BETOHS( addr ) ( ( ( ( uint16_t ) SXLIB_MEMBYTE( addr, 0 ) ) << 8 ) + SXLIB_MEMBYTE( addr, 1 ) )

/** Standard big-endian to host-order conversion */
#define SXLIB_BETOHL( addr )                                                                                      \
    ( ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 0 ) ) << 24 ) + ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 1 ) ) << 16 ) + \
      ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 2 ) ) << 8 ) + SXLIB_MEMBYTE( addr, 3 ) )

/** Standard little-endian to host-order conversion */
#define SXLIB_LETOHS( addr ) ( ( ( ( uint16_t ) SXLIB_MEMBYTE( addr, 1 ) ) << 8 ) + SXLIB_MEMBYTE( addr, 0 ) )

/** Standard little-endian to host-order conversion */
#define SXLIB_LETOHL( addr )                                                                                      \
    ( ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 3 ) ) << 24 ) + ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 2 ) ) << 16 ) + \
      ( ( ( uint32_t ) SXLIB_MEMBYTE( addr, 1 ) ) << 8 ) + SXLIB_MEMBYTE( addr, 0 ) )

/** Standard host-order to big-endian conversion */
#define SXLIB_HTOBEL( addr, value )                  \
    do                                               \
    {                                                \
        const uint32_t v         = value;            \
        ( ( uint8_t* ) addr )[0] = SXLIB_BYTE3( v ); \
        ( ( uint8_t* ) addr )[1] = SXLIB_BYTE2( v ); \
        ( ( uint8_t* ) addr )[2] = SXLIB_BYTE1( v ); \
        ( ( uint8_t* ) addr )[3] = SXLIB_BYTE0( v ); \
    } while( 0 )

/** Standard network to host-order conversion */
#define SXLIB_NTOHS( addr ) SXLIB_BETOHS( addr )

/** Standard network to host-order conversion */
#define SXLIB_NTOHL( addr ) SXLIB_BETOHL( addr )

/** Standard network to host-order conversion */
#define SXLIB_HTONL( addr, value ) SXLIB_HTOBEL( addr, value )

/** Cast data to 32-bit unsigned int */
#define SXLIB_CAST_U32( x ) ( ( uint32_t )( x ) )

/** Helper for creating unnamed constant arrays on the fly */
#define SXLIB_ARGCNT( ... ) ( sizeof( ( int[] ){ 0, ##__VA_ARGS__ } ) / sizeof( int ) - 1 )

/** Helper for creating unnamed constant arrays on the fly */
#define SXLIB_UNNAMED_DECL_T( TYPE, ... ) \
    ( const TYPE[] ) { __VA_ARGS__ }

/** Helper for creating unnamed constant arrays on the fly */
#define SXLIB_UNNAMED_CONST_DECL_T( TYPE, ... ) \
    ( const TYPE[] ) { __VA_ARGS__ }

/** Provide an unnamed uint8_t[] array, followed by its length, for function arguments */
#define SXLIB_UINT8_T_N( ... ) SXLIB_UNNAMED_DECL_T( uint8_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

/** Provide an unnamed uint16_t[] array, followed by its length, for function arguments */
#define SXLIB_UINT16_T_N( ... ) SXLIB_UNNAMED_DECL_T( uint16_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

/** Provide an unnamed uint32_t[] array, followed by its length, for function arguments */
#define SXLIB_UINT32_T_N( ... ) SXLIB_UNNAMED_DECL_T( uint32_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

/** Provide an unnamed conts uint8_t[] array, followed by its length, for function arguments */
#define SXLIB_CONST_UINT8_T_N( ... ) SXLIB_UNNAMED_CONST_DECL_T( uint8_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

/** Provide an unnamed conts uint16_t[] array, followed by its length, for function arguments */
#define SXLIB_CONST_UINT16_T_N( ... ) SXLIB_UNNAMED_CONST_DECL_T( uint16_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

/** Provide an unnamed conts uint32_t[] array, followed by its length, for function arguments */
#define SXLIB_CONST_UINT32_T_N( ... ) SXLIB_UNNAMED_CONST_DECL_T( uint32_t, __VA_ARGS__ ), SXLIB_ARGCNT( __VA_ARGS__ )

// Determine the number of elements in an array, at compile-time
#define SXLIB_COUNTOF( arr ) ( sizeof( arr ) / sizeof( arr[0] ) )

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_DATA_MEMORY_GENERIC_MEMORY_H_

/* --- EOF ------------------------------------------------------------------ */
