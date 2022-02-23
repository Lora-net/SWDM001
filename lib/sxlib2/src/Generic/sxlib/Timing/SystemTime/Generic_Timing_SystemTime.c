/**
 * \file
 *
 * \brief Generic implementation of system time management system
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

sxlib_Timing_SystemTime_long_relative_time_t sxlib_Timing_SystemTime_get_long_relative_time(
    sxlib_Timing_SystemTime_long_absolute_time_t timestamp )
{
    sxlib_Timing_SystemTime_long_absolute_time_t current = sxlib_Timing_SystemTime_get_time( );
    sxlib_Timing_SystemTime_long_relative_time_t delta   = current - timestamp;

    return delta;
}

sxlib_Timing_SystemTime_relative_time_t sxlib_Timing_SystemTime_get_relative_time(
    sxlib_Timing_SystemTime_absolute_time_t timestamp )
{
    sxlib_Timing_SystemTime_absolute_time_t current = sxlib_Timing_SystemTime_get_time( );
    sxlib_Timing_SystemTime_relative_time_t delta   = current - timestamp;

    return delta;
}

void sxlib_Timing_SystemTime_busywait_mbs( uint32_t mbs )
{
    sxlib_Timing_SystemTime_busywait_ticks( SXLIB_MBS_TO_TICK( mbs ) );
}

void sxlib_Timing_SystemTime_busywait_ms( uint32_t ms )
{
    sxlib_Timing_SystemTime_busywait_ticks( SXLIB_MS_TO_TICK( ms ) );
}

/* --- EOF ------------------------------------------------------------------ */
