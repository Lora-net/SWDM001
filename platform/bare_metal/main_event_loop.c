/**
 * \file
 *
 * \brief Startup file for asynchronous code, implementing an event loop
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

#include <sxlib/Timing/async_timer/Rtos_async_timer_poll.h>
#include <sxlib/System/basic_events/Generic_basic_events.h>
#include <sxlib/Radio/event_handler/basic_event_handler.h>
#include <sxlib/Timing/lptim/McuHal_lptim.h>
#include <sxlib/System/sleep_management/Generic_sleep_management.h>
#include <sxlib/System/sleep_inhibition/Generic_sleep_inhibition.h>
#include <sxlib/weak.h>

#include "smtc_shield.h"
#include "smtc_board.h"
#include "main_various.h"

// For __disable_irq()
#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

__weak void initialize_application( const void* context )
{
}

#ifdef CONFIG_SMTC_ENABLE_TICKLESS_MODE
void check_timers_and_events_then_sleep( void )
{
    sxlib_Timing_OneShotTimerBase_time_t delta = sxlib_Timing_async_timer_get_delta( );
    sxlib_System_basic_events_mask_t     active_events =
        sxlib_System_basic_events_get_and_clear( SXLIB_DEFAULT_EVENT_MASK_RADIO0 | SXLIB_DEFAULT_EVENT_MASK_INIT );
    if( !active_events && delta )
    {
        bool timer_started = sxlib_Timing_lptim_set_trigger_interval( delta );
        if( timer_started )
        {
            sxlib_System_sleep( );
        }

        __enable_irq( );
    }
    else
    {
        __enable_irq( );

        if( ( active_events & SXLIB_DEFAULT_EVENT_MASK_RADIO0 ) != 0 )
        {
            sxlib_Radio_basic_event_handler_handle( &global_radio );
        }
        if( ( active_events & SXLIB_DEFAULT_EVENT_MASK_INIT ) != 0 )
        {
            initialize_application( &global_radio );
        }
    }
}
#else
void check_timers_and_events_then_sleep( void )
{
    __enable_irq( );

    sxlib_System_basic_events_mask_t active_events =
        sxlib_System_basic_events_get_and_clear( SXLIB_DEFAULT_EVENT_MASK_RADIO0 );
    if( ( active_events & SXLIB_DEFAULT_EVENT_MASK_RADIO0 ) != 0 )
    {
        sxlib_Radio_basic_event_handler_handle( &global_radio );
    }
    if( ( active_events & SXLIB_DEFAULT_EVENT_MASK_INIT ) != 0 )
    {
        initialize_application( &global_radio );
    }
}
#endif

/* --- EOF ------------------------------------------------------------------ */
