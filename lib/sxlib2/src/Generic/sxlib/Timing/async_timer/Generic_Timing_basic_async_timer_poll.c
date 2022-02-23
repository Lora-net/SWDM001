/**
 * \file
 *
 * \brief Generic implementation of polling operations for a basic asynchronous timer
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

#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>
#include <sxlib/Timing/async_timer/Rtos_async_timer_impl.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/** This is necessary to calculate the time_delta argument to \ref sxlib_Timing_async_timer_process */
static sxlib_Timing_SystemTime_absolute_time_t last_system_time;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

sxlib_Timing_SystemTime_absolute_time_t update_system_time_delta( void )
{
    sxlib_Timing_SystemTime_absolute_time_t current_system_time = sxlib_Timing_SystemTime_get_time( );
    sxlib_Timing_SystemTime_absolute_time_t delta               = current_system_time - last_system_time;
    last_system_time                                            = current_system_time;
    return delta;
}

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Timing_async_timer_poll_init( ) { last_system_time = sxlib_Timing_SystemTime_get_time( ); }

void sxlib_Timing_async_timer_poll( )
{
    sxlib_Timing_OneShotTimerBase_time_t queue_delta;
    do
    {
        sxlib_Timing_SystemTime_absolute_time_t time_delta = update_system_time_delta( );
        sxlib_Timing_async_timer_process( time_delta );
        queue_delta = sxlib_Timing_async_timer_get_delta( );
    } while( queue_delta == 0 );
}

/* --- EOF ------------------------------------------------------------------ */
