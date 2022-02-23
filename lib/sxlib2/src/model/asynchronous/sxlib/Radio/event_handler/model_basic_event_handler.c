/**
 * \file
 *
 * \brief Implementation of a simple generic asynchronous transceiver event handling system
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

#include <sxlib/Radio/event_handler/basic_event_handler.h>
#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>
#include <sxlib/System/basic_events/Generic_basic_events.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Radio_event_handler_manager_init( const void*                                context,
                                             sxlib_Radio_event_handler_manager_state_t* handler_manager_state,
                                             sxlib_System_basic_events_mask_t           event_mask )
{
    handler_manager_state->handler    = 0;
    handler_manager_state->event_mask = event_mask;
    handler_manager_state->timestamp  = 0;
    sxlib_System_basic_events_get_and_clear( handler_manager_state->event_mask );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

void sxlib_Radio_event_handler_trigger_base( sxlib_Radio_event_handler_manager_state_t* state )
{
    sxlib_System_basic_events_set( state->event_mask );
}

void sxlib_Radio_event_handler_trigger_with_timestamp_base( sxlib_Radio_event_handler_manager_state_t* state )
{
    state->timestamp = sxlib_Timing_SystemTime_get_time( );
    sxlib_System_basic_events_set( state->event_mask );
}

void sxlib_Radio_event_handler_cancel_event_base( sxlib_Radio_event_handler_manager_state_t* state )
{
    sxlib_System_basic_events_get_and_clear( state->event_mask );
}

void sxlib_Radio_event_handler_set_base( sxlib_Radio_event_handler_manager_state_t* state,
                                         sxlib_Radio_event_handler_f handler, void* arg )
{
    state->handler = handler;
    state->arg     = arg;
}

uint32_t sxlib_Radio_event_handler_retrieve_timestamp_base( sxlib_Radio_event_handler_manager_state_t* state,
                                                            const void*                                context )
{
    return state->timestamp;
}

void sxlib_Radio_basic_event_handler_handle_base( sxlib_Radio_event_handler_manager_state_t* state )
{
    state->handler( state->arg );
}

/* --- EOF ------------------------------------------------------------------ */
