/**
 * @file      sx126x_lr_fhss_ping_async.c
 *
 * @brief     SX126x LR-FHSS ping demo, example source code for asynchronous implementation
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

#include "demos/sx126x_lr_fhss_ping/sx126x_lr_fhss_ping.h"
#include <sxlib/Radio/event_handler/Rtos_model_event_handler.h>
#include <sxlib/Timing/async_timer/Generic_async_timer.h>

#include "sx126x.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DECLARATIONS -------------------------------------------
 */

/**
 * @brief Handle a timer event
 *
 * @param [in,out] arg Pointer to application state
 */
static void handle_timer_event( void* arg );

/**
 * @brief Handle a radio interrupt
 *
 * @param [in,out] arg Pointer to application state
 */
static void handle_radio_event( void* arg );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sx126x_lr_fhss_ping_start( sx126x_lr_fhss_ping_state_t* state )
{
    sxlib_Timing_async_timer_init( &state->timer );
    sxlib_Timing_async_timer_set_handler( &state->timer, handle_timer_event, state );
    sxlib_Radio_event_handler_set( state->config->radio, handle_radio_event, state );

    sx126x_lr_fhss_ping_launch( state );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

static void handle_timer_event( void* arg )
{
    sx126x_lr_fhss_ping_state_t* state = ( sx126x_lr_fhss_ping_state_t* ) arg;

    sx126x_lr_fhss_ping_continue( state );
}

static void handle_radio_event( void* arg )
{
    sx126x_lr_fhss_ping_state_t* state = ( sx126x_lr_fhss_ping_state_t* ) arg;

    sx126x_irq_mask_t irq_status;
    sx126x_get_and_clear_irq_status( state->config->radio, &irq_status );

    if( irq_status & SX126X_IRQ_LR_FHSS_HOP )
    {
        sx126x_lr_fhss_ping_handle_hop( state );
    }
    else if( irq_status & SX126X_IRQ_TX_DONE )
    {
        sx126x_lr_fhss_ping_handle_tx_done( state );
        sxlib_Timing_async_timer_start( &state->timer, state->config->inter_pkt_delay );
    }
}

/* --- EOF ------------------------------------------------------------------ */
