/**
 * \file
 *
 * \brief Rtos-specific implementation of system time management system
 * \remark This implementation expects an ISR to periodically increment the time
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
#include <stdatomic.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static atomic_int_least32_t                    stored_time;
static uint32_t                                stored_time_prev;
static sxlib_Timing_SystemTime_absolute_time_t system_time;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Timing_SystemTime_init( void )
{
    stored_time = 0;

    stored_time_prev = 0;
    system_time      = 0;
}

sxlib_Timing_SystemTime_long_absolute_time_t sxlib_Timing_SystemTime_get_long_time( void )
{
    uint32_t current = stored_time;

    uint32_t delta   = current - stored_time_prev;
    stored_time_prev = current;
    system_time += delta;

    return system_time;
}

sxlib_Timing_SystemTime_absolute_time_t sxlib_Timing_SystemTime_get_time( void )
{
    return ( sxlib_Timing_SystemTime_absolute_time_t ) sxlib_Timing_SystemTime_get_long_time( );
}

void sxlib_Timing_SystemTime_increment_time( void ) { stored_time++; }

uint32_t sxlib_Timing_SystemTime_get_ticks_per_second( void ) { return CONFIG_SYS_CLOCK_TICKS_PER_SEC; }

void sxlib_Timing_SystemTime_busywait_ticks( uint32_t ticks )
{
    // Wait one extra tick to be sure that we always wait at-least the requested time
    uint32_t start_time = stored_time;
    while( stored_time - start_time < ticks + 1 )
    {
    }
}

/* --- EOF ------------------------------------------------------------------ */
