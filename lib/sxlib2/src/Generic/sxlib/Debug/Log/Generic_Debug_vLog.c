/**
 * \file
 *
 * \brief Generic logging initialization and helper functions
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdarg.h>
#include <stdio.h>

#include <sxlib/Debug/Log/Generic_Log.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE DATA ------------------------------------------------------------
 */

#if( SXLIB_LOG_LEVEL > 0 )
static sxlib_Debug_Log_writer_f sxlib_Debug_Log_writer;
static void*                    sxlib_Debug_Log_writer_arg;
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#ifndef SXLIB_DEBUG_LOG_MAX_LENGTH
#define SXLIB_DEBUG_LOG_MAX_LENGTH ( 160 )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ---------------------------------------------
 */

int vsnprintf( char* str, size_t size, const char* format, va_list ap );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Debug_Log_init( sxlib_Debug_Log_writer_f writer, void* arg )
{
#if( SXLIB_LOG_LEVEL > 0 )
    sxlib_Debug_Log_writer     = writer;
    sxlib_Debug_Log_writer_arg = arg;
#endif
}

#if( SXLIB_LOG_LEVEL > 0 )
void sxlib_Debug_Log_vlog( const char* format, va_list argp )
{
    int  len;
    char buf[SXLIB_DEBUG_LOG_MAX_LENGTH];

    len = vsnprintf( buf, sizeof( buf ), format, argp );

    if( len >= sizeof( buf ) )
    {
        len = sizeof( buf ) - 1;
    }
    sxlib_Debug_Log_writer( sxlib_Debug_Log_writer_arg, ( uint8_t* ) buf, len );
}
#endif

/* --- EOF ------------------------------------------------------------------ */
