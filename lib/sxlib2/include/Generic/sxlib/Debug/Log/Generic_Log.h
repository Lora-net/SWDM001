/**
 * \file
 *
 * \brief Generic interface header for logging infrastructure
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_DEBUG_LOG_GENERIC_LOG_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_DEBUG_LOG_GENERIC_LOG_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdarg.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Log writer implementation callback */
typedef void ( *sxlib_Debug_Log_writer_f )( void* arg, const uint8_t* data, unsigned int len );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * Log level verbosity ordering
 */

#define SXLIB_LOG_LEVEL_NONE ( 0 )

#ifndef SXLIB_LOG_APP
#define SXLIB_LOG_APP ( 1000 )
#endif
#ifndef SXLIB_LOG_LED
#define SXLIB_LOG_LED ( 1999 )
#endif
#ifndef SXLIB_LOG_LORAWAN
#define SXLIB_LOG_LORAWAN ( 2000 )
#endif
#ifndef SXLIB_LOG_COMM
#define SXLIB_LOG_COMM ( 3000 )
#endif
#ifndef SXLIB_LOG_SXLIB
#define SXLIB_LOG_SXLIB ( 4000 )
#endif
#ifndef SXLIB_LOG_SXLIB_TIMING
#define SXLIB_LOG_SXLIB_TIMING ( 5000 )
#endif
#ifndef SXLIB_LOG_APP_OPT
#define SXLIB_LOG_APP_OPT ( 6000 )
#endif

/** If SXLIB_LOG_LEVEL is 0, logging will be copmiled out */
#ifndef SXLIB_LOG_LEVEL
#define SXLIB_LOG_LEVEL 0
#endif

/** Line ending character sequence */
#define SXLIB_LOG_EOL "\r\n"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

#if( SXLIB_LOG_LEVEL > SXLIB_LOG_LEVEL_NONE )
#define SXLIB_LOG( level, print_args )      \
    do                                      \
    {                                       \
        if( SXLIB_LOG_LEVEL >= level )      \
        {                                   \
            sxlib_Debug_Log_log print_args; \
        }                                   \
    } while( 0 )
#else
#define SXLIB_LOG( level, print_args ) \
    do                                 \
    {                                  \
    } while( 0 )
#endif

#if( SXLIB_LOG_LEVEL > SXLIB_LOG_LEVEL_NONE )
#define SXLIB_LOG_HEX( level, title, data, n )         \
    do                                                 \
    {                                                  \
        if( SXLIB_LOG_LEVEL >= level )                 \
        {                                              \
            sxlib_Debug_Log_log_hex( title, data, n ); \
        }                                              \
    } while( 0 )
#else
#define SXLIB_LOG_HEX( level, title, data, n ) \
    do                                         \
    {                                          \
    } while( 0 )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the logging subsystem
 *
 * @param writer Writer callback
 * @param arg Opaque argument provided to writer callback
 */
void sxlib_Debug_Log_init( sxlib_Debug_Log_writer_f writer, void* arg );

/**
 * @brief Log data using a printf-style format string and a variable argument list
 *
 * @param format Printf-style format string
 */
void sxlib_Debug_Log_log( const char* format, ... );

/**
 * @brief Log data using a printf-style format string and a va_list argument
 *
 * @param format Printf-style format string
 * @param va_list Aggregated arguments
 */
void sxlib_Debug_Log_vlog( const char* format, va_list argp );

/**
 * @brief Log data to a list of hexadecimal bytes
 *
 * @param title String describing the data block
 * @param data Data buffer
 * @param n Length of data buffer, in bytes
 */
void sxlib_Debug_Log_log_hex( const char* title, const uint8_t* data, unsigned int n );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_DEBUG_LOG_GENERIC_LOG_H_

/* --- EOF ------------------------------------------------------------------ */
