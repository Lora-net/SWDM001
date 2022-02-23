/**
 * \file
 *
 * \brief Generic implementation of a timer linked list
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

#include <stdbool.h>

#include <sxlib/Timing/OneShotTimerBase/Generic_OneShotTimerBase_impl.h>
#include <sxlib/Debug/Assert/Assert.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

int sxlib_Timing_OneShotTimerBase_insert( struct sxlib_Timing_OneShotTimerBase* root,
                                          struct sxlib_Timing_OneShotTimerBase* timer,
                                          sxlib_Timing_OneShotTimerBase_time_t  ticks )
{
    struct sxlib_Timing_OneShotTimerBase* i = root;

    if( root->next )
    {
        sxlib_assert( i->delta >= 0 );
        sxlib_assert( i->next != timer );

        sxlib_Timing_OneShotTimerBase_time_t cumulated_time = i->delta;

        while( i->next && ( cumulated_time <= ticks ) )
        {
            i = i->next;
            cumulated_time += i->delta;
        }

        timer->next  = i->next;
        timer->delta = cumulated_time - ticks;

        i->next = timer;
        i->delta -= timer->delta;

        if( i == root )
        {
            return ONE_SHOT_TIMER_BASE_STATUS_CHANGED;
        }
        else
        {
            return ONE_SHOT_TIMER_BASE_STATUS_OK;
        }
    }
    else
    {
        timer->next  = 0;
        timer->delta = -ticks;
        root->next   = timer;
        root->delta  = ticks;

        return ONE_SHOT_TIMER_BASE_STATUS_NEW;
    }
}

sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_OneShotTimerBase_get_remaining_time(
    struct sxlib_Timing_OneShotTimerBase* root, struct sxlib_Timing_OneShotTimerBase* timer )
{
    struct sxlib_Timing_OneShotTimerBase* i = root;

    sxlib_assert( i->delta >= 0 );
    sxlib_Timing_OneShotTimerBase_time_t cumulated_time = i->delta;

    while( ( i->next != 0 ) && ( i->next != timer ) )
    {
        i = i->next;
        cumulated_time += i->delta;
    }

    // Make sure timer was found
    sxlib_assert( i->next );
    return cumulated_time;
}

int sxlib_Timing_OneShotTimerBase_insert_adjusted( struct sxlib_Timing_OneShotTimerBase* root,
                                                   struct sxlib_Timing_OneShotTimerBase* timer,
                                                   sxlib_Timing_OneShotTimerBase_time_t  ticks,
                                                   sxlib_Timing_OneShotTimerBase_time_t  remaining )
{
    sxlib_Timing_OneShotTimerBase_time_t adjusted_ticks = ticks + remaining - root->delta;
    if( adjusted_ticks < 0 )
    {
        // Emit a warning?
        adjusted_ticks = 0;
    }
    return sxlib_Timing_OneShotTimerBase_insert( root, timer, adjusted_ticks );
}

void sxlib_Timing_OneShotTimerBase_execute_callables(
    struct sxlib_Timing_OneShotTimerBase* root,
    void ( *restart_timer_callback )( void* arg, sxlib_Timing_OneShotTimerBase_time_t delta ), void* arg )
{
    sxlib_assert( root->delta >= 0 );

    while( root->next && !root->delta )
    {
        struct sxlib_Timing_OneShotTimerBase_callable* callable = &root->next->callable;

        // First remove this timer entry so that the callable can safely
        // manipulate the timers, for instance, to insert itself again.
        root->delta = root->next->delta;
        root->next  = root->next->next;

        if( restart_timer_callback && root->next && root->delta )
        {
            restart_timer_callback( arg, root->delta );
        }

        callable->callback( callable->arg );
    }

    if( !root->next )
    {
        root->delta = 0;
    }
}

int sxlib_Timing_OneShotTimerBase_delete( struct sxlib_Timing_OneShotTimerBase* root,
                                          struct sxlib_Timing_OneShotTimerBase* timer )
{
    int                                   retval = ONE_SHOT_TIMER_BASE_STATUS_CHANGED;
    struct sxlib_Timing_OneShotTimerBase* i      = root;

    while( i->next != timer )
    {
        if( !i->next ) return ( ONE_SHOT_TIMER_BASE_STATUS_INEXISTENT );
        i      = i->next;
        retval = ONE_SHOT_TIMER_BASE_STATUS_OK;
    }

    i->delta += timer->delta;
    i->next = timer->next;

    if( !root->next )
    {
        root->delta = 0;
    }
    return retval;
}

void sxlib_Timing_OneShotTimerBase_update( struct sxlib_Timing_OneShotTimerBase* root,
                                           sxlib_Timing_OneShotTimerBase_time_t  ticks )
{
    struct sxlib_Timing_OneShotTimerBase* i = root;

    while( i->next && ( ticks > 0 ) )
    {
        sxlib_assert( i->delta >= 0 );

        // If your code hangs here, you have inserted the same timer twice.
        if( ticks <= i->delta )
        {
            i->delta -= ticks;
            return;
        }
        else
        {
            ticks -= i->delta;
            i->delta = 0;
            i        = i->next;
        }
    }
}

sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_OneShotTimerBase_get_delta(
    struct sxlib_Timing_OneShotTimerBase* root )
{
    // TODO: Make this iterate until non-zero.
    if( root->next )
    {
        sxlib_assert( root->delta >= 0 );
        return root->delta;
    }
    else
    {
        return ONE_SHOT_TIMER_WAIT_FOREVER;
    }
}

/* --- EOF ------------------------------------------------------------------ */
