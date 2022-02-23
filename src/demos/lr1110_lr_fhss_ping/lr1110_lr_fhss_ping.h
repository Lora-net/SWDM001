/**
 * @file      lr1110_lr_fhss_ping.h
 *
 * @brief     LR1110 LR-FHSS ping demo, main example header
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
#ifndef SRC_LR1110_LR_FHSS_PING_H_
#define SRC_LR1110_LR_FHSS_PING_H_

#include <stdint.h>
#include <stdbool.h>

#include <sxlib/System/config/Rtos_config.h>
#include <sxlib/display/interface/Generic_interface.h>

#ifdef CONFIG_SMTC_DEMO_ASYNCHRONOUS
#include <sxlib/Timing/async_timer/Rtos_async_timer_impl.h>
#endif

#include "lr1110_lr_fhss.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define LR_FHSS_SYNC_WORD_BYTES ( 4 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** @brief Demo configuration */
typedef struct lr1110_lr_fhss_ping_config_s
{
    const void*                      radio;           /**< transceiver driver context */
    const sxlib_generic_interface_t* interface;       /**< LEDs and display */
    uint32_t                         inter_pkt_delay; /**< inter packet delay, in platform timer ticks */
} lr1110_lr_fhss_ping_config_t;

/** @brief Demo state */
typedef struct lr1110_lr_fhss_ping_state_s
{
    const lr1110_lr_fhss_ping_config_t* config;         /**< Make demo configuration available from state pointer */
    unsigned int                        payload_length; /**< Payload length of current transmission */
    lr1110_lr_fhss_params_t             lr1110_lr_fhss_params; /**< LR-FHSS comm parameter storage */
    uint8_t                             payload[255];          /**< Payload storage */

#ifdef CONFIG_SMTC_DEMO_ASYNCHRONOUS
    sxlib_Timing_async_timer_t timer; /**< Used for MCU timekeeping */
#endif
} lr1110_lr_fhss_ping_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DECLARATIONS --------------------------------------------
 */

/**
 * @brief Initialize the application
 *
 * @param [in,out] state Pointer to application state
 * @param [in] config    Pointer to application configuration
 */
void lr1110_lr_fhss_ping_init( lr1110_lr_fhss_ping_state_t* state, const lr1110_lr_fhss_ping_config_t* config );

/**
 * @brief Start the application, after initialization
 *
 * @param [in,out] state Pointer to application state
 */
void lr1110_lr_fhss_ping_start( lr1110_lr_fhss_ping_state_t* state );

/**
 * @brief First step of application startup: Turn on LED and initiate transmission
 *
 * @param [in,out] state Pointer to application state
 */
void lr1110_lr_fhss_ping_launch( lr1110_lr_fhss_ping_state_t* state );

/**
 * @brief Wake up transceiver, increment payload_length, and call @ref sx126x_lr_fhss_ping_launch
 *
 * @param [in,out] state Pointer to application state
 */
void lr1110_lr_fhss_ping_continue( lr1110_lr_fhss_ping_state_t* state );

/**
 * @brief Inform the user and the LR-FHSS driver that transmission is over, and put transceiver to sleep
 *
 * @param [in,out] state Pointer to application state
 */
void lr1110_lr_fhss_ping_handle_tx_done( lr1110_lr_fhss_ping_state_t* state );

#ifdef __cplusplus
}
#endif

#endif  // SRC_LR1110_LR_FHSS_PING_H_

/* --- EOF ------------------------------------------------------------------ */
