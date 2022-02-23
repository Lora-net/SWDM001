/**
 * \file
 *
 * \brief McuHal-specific implementation of system time management system
 * \remark This implementation is designed for tickless operation, and
 * requires a low-power hardware timer
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

#include <stdint.h>

#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>
#include <sxlib/Timing/lptim/McuHal_lptim.h>
#include <sxlib/System/critical_section/McuHal_critical_section.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static uint16_t                                stored_time_prev;
static sxlib_Timing_SystemTime_absolute_time_t system_time;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Timing_SystemTime_init( void )
{
    stored_time_prev = 0;
    system_time      = 0;
}

sxlib_Timing_SystemTime_long_absolute_time_t sxlib_Timing_SystemTime_get_long_time( void )
{
    uint16_t current = sxlib_Timing_lptim_get_hardware_time( );

    SXLIB_SYSTEM_CRITICAL_SECTION_ENTER;
    uint16_t delta   = current - stored_time_prev;
    stored_time_prev = current;
    system_time += delta;
    SXLIB_SYSTEM_CRITICAL_SECTION_EXIT;

    return system_time;
}

sxlib_Timing_SystemTime_absolute_time_t sxlib_Timing_SystemTime_get_time( void )
{
    return ( sxlib_Timing_SystemTime_absolute_time_t ) sxlib_Timing_SystemTime_get_long_time( );
}

uint32_t sxlib_Timing_SystemTime_get_ticks_per_second( void )
{
    return 1 << sxlib_Timing_lptim_get_clock_frequency_log2( );
}

void sxlib_Timing_SystemTime_busywait_ticks( uint32_t ticks )
{
    // Wait one extra tick to be sure that we always wait at-least the requested time
    uint32_t start_time = sxlib_Timing_SystemTime_get_time( );
    while( sxlib_Timing_SystemTime_get_time( ) - start_time < ticks + 1 )
    {
    }
}

/* --- EOF ------------------------------------------------------------------ */
