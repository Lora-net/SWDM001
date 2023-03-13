/**
 * @file      lr11xx_lr_fhss_ping_sync.c
 *
 * @brief     LR11XX LR-FHSS ping demo, example source code for synchronous implementation
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

#include "demos/lr11xx_lr_fhss_ping/lr11xx_lr_fhss_ping.h"

#include <sxlib/Radio/event_handler/Rtos_model_event_handler.h>
#include <sxlib/Timing/sync_timer/Generic_sync_timer.h>

#include "lr11xx_system.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void lr11xx_lr_fhss_ping_start( lr11xx_lr_fhss_ping_state_t* state )
{
    lr11xx_lr_fhss_ping_launch( state );

    while( 1 )
    {
        while( 1 )
        {
            sxlib_Radio_event_handler_wait_for_event( state->config->radio, SXLIB_RADIO_EVENT_HANDLER_TIMEOUT_FOREVER );

            lr11xx_system_irq_mask_t irq_status;
            lr11xx_system_get_and_clear_irq_status( state->config->radio, &irq_status );

            if( irq_status & LR11XX_SYSTEM_IRQ_TX_DONE )
            {
                lr11xx_lr_fhss_ping_handle_tx_done( state );
                break;
            }
        }

        sxlib_Timing_sync_timer_sleep( state->config->inter_pkt_delay );
        lr11xx_lr_fhss_ping_continue( state );
    };
}

/* --- EOF ------------------------------------------------------------------ */
