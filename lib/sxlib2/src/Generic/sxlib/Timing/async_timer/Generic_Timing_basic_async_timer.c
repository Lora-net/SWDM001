/**
 * \file
 *
 * \brief Generic implementation of a basic asynchronous timer
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

#include <sxlib/Timing/async_timer/Rtos_async_timer_impl.h>
#include <sxlib/Debug/Assert/Assert.h>
#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>

// This is necessary so that 'current time' is accurate when we request a delta sleep
sxlib_Timing_SystemTime_absolute_time_t update_system_time_delta( void );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/** This stores the linked list of timers */
static struct sxlib_Timing_OneShotTimerBase root;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

static void base_handler( void* arg )
{
    sxlib_Timing_async_timer_t* timer = ( sxlib_Timing_async_timer_t* ) arg;

    if( timer->queued )
    {
        timer->queued = false;
        if( timer->handler )
        {
            timer->handler( timer->arg );
        }
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Timing_async_timer_start( sxlib_Timing_async_timer_t* timer, sxlib_Timing_async_timer_time_t duration )
{
    sxlib_assert( !timer->queued );

    timer->base.callable.callback = base_handler;
    timer->base.callable.arg      = timer;

    timer->queued = true;
    update_system_time_delta( );
    sxlib_Timing_OneShotTimerBase_insert( &root, &timer->base, duration );
}

void sxlib_Timing_async_timer_stop( sxlib_Timing_async_timer_t* timer )
{
    if( timer->queued )
    {
        timer->queued = false;
        update_system_time_delta( );
        sxlib_Timing_OneShotTimerBase_delete( &root, &timer->base );
    }
}

void sxlib_Timing_async_timer_init( sxlib_Timing_async_timer_t* timer )
{
    timer->queued  = false;
    timer->handler = 0;
}

void sxlib_Timing_async_timer_set_handler( sxlib_Timing_async_timer_t*        timer,
                                           sxlib_Timing_async_timer_handler_f handler, void* arg )
{
    timer->handler = handler;
    timer->arg     = arg;
}

void sxlib_Timing_async_timer_process( sxlib_Timing_async_timer_time_t ticks )
{
    if( ticks > 0 )
    {
        sxlib_Timing_OneShotTimerBase_update( &root, ticks );
        sxlib_Timing_OneShotTimerBase_execute_callables( &root, 0, 0 );
    }
}

sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_async_timer_get_delta( )
{
    return sxlib_Timing_OneShotTimerBase_get_delta( &root );
}

/* --- EOF ------------------------------------------------------------------ */
