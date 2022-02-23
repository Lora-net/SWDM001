/**
 * @file      sx126x_lr_fhss_ping.c
 *
 * @brief     SX126x LR-FHSS ping demo, main example source code
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
#include <string.h>
#include <stdlib.h>

#include <sxlib/Gpio/Led/Generic_Led.h>
#include <sxlib/Debug/Log/Generic_Log.h>

#include "sx126x.h"
#include "sx126x_comp.h"
#include "sx126x_lr_fhss.h"

#include "demos/sx126x_lr_fhss_ping/sx126x_lr_fhss_ping.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/** @brief Transmit power */
#define POWER_IN_DBM ( -9 )

/** @brief Center frequency */
#define RF_FREQUENCY ( 868000000 )

/** @brief First payload length used. Must be between 1 and the maximum allowed payload length for the LR-FHSS comm
 * parameters.
 */
#define MIN_PAYLOAD_LENGTH ( 1 )

/** @brief Largest payload length used before returning to MIN_PAYLOAD_LENGTH. Must be between MIN_PAYLOAD_LENGTH and
 * the maximum allowed payload length for the LR-FHSS comm parameters */
#define MAX_PAYLOAD_LENGTH ( 50 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/** @brief LR-FHSS sync word storage */
static const uint8_t lr_fhss_sync_word[LR_FHSS_SYNC_WORD_BYTES] = { 0x2C, 0x0F, 0x79, 0x95 };

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DECLARATIONS -------------------------------------------
 */

/**
 * @brief Enter standby mode, then clear all interrupts
 *
 * @param [in] context Chip implementation context
 */
static void set_standby_clear_all_irqs( const void* context );

/**
 * @brief Enter standby mode, clear all interrupts, and then optionally sleep
 * if CONFIG_ALLOW_SMTC_RADIO_SLEEP is defined
 *
 * @param [in] state           Pointer to application state
 */
static void enter_standby_then_sleep_mode( const sx126x_lr_fhss_ping_state_t* state );

/**
 * @brief Exit sleep mode if CONFIG_ALLOW_SMTC_RADIO_SLEEP is defined
 *
 * @param [in] state           Pointer to application state
 */
static void exit_sleep_mode( const sx126x_lr_fhss_ping_state_t* state );

/**
 * @brief Send the provided payload over LR-FHSS, using the parameters provided in this function
 *
 * @param [in,out] state      Pointer to application state
 * @param [in] payload        Pointer to application payload
 * @param [in] payload_length Length of application payload
 */
static sx126x_status_t lr_fhss_send_packet( sx126x_lr_fhss_ping_state_t* state, const uint8_t* payload,
                                            uint16_t payload_length );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sx126x_lr_fhss_ping_launch( sx126x_lr_fhss_ping_state_t* state )
{
    sxlib_Gpio_Led_on( state->config->interface->led_interface.led_tx );

    sx126x_status_t status = lr_fhss_send_packet( state, state->payload, state->payload_length );
    if( status != SX126X_STATUS_OK )
    {
        sxlib_Gpio_Led_off( state->config->interface->led_interface.led_tx );
        SXLIB_LOG( SXLIB_LOG_APP, ( "Failed status=%d" SXLIB_LOG_EOL, status ) );
    }
}

void sx126x_lr_fhss_ping_init( sx126x_lr_fhss_ping_state_t* state, const sx126x_lr_fhss_ping_config_t* config )
{
    state->config = config;

    SXLIB_LOG( SXLIB_LOG_APP, ( "SX126X-LR-FHSS Ping" SXLIB_LOG_EOL ) );

    sxlib_Gpio_Led_off( state->config->interface->led_interface.led_tx );
    sxlib_Gpio_Led_off( state->config->interface->led_interface.led_rx );

    // Transceiver has already been reset in main()

    uint32_t seed;
    sx126x_get_random_numbers( state->config->radio, &seed, 1 );
    srand( seed );

    sx126x_set_dio_irq_params( state->config->radio, SX126X_IRQ_ALL, SX126X_IRQ_TX_DONE | SX126X_IRQ_LR_FHSS_HOP,
                               SX126X_IRQ_NONE, SX126X_IRQ_NONE );

    for( int i = 0; i < sizeof( state->payload ); ++i )
    {
        state->payload[i] = i;
    }
    state->payload_length = MIN_PAYLOAD_LENGTH;
}

void sx126x_lr_fhss_ping_continue( sx126x_lr_fhss_ping_state_t* state )
{
    exit_sleep_mode( state );

    if( state->payload_length >= MAX_PAYLOAD_LENGTH )
    {
        state->payload_length = MIN_PAYLOAD_LENGTH;
    }
    else
    {
        state->payload_length += 1;
    }

    sx126x_lr_fhss_ping_launch( state );
}

void sx126x_lr_fhss_ping_handle_hop( sx126x_lr_fhss_ping_state_t* state )
{
    sx126x_lr_fhss_handle_hop( state->config->radio, &state->sx126x_lr_fhss_params, &state->lr_fhss_state );
    SXLIB_LOG( SXLIB_LOG_APP, ( "Hopped." SXLIB_LOG_EOL ) );
}

void sx126x_lr_fhss_ping_handle_tx_done( sx126x_lr_fhss_ping_state_t* state )
{
    sx126x_lr_fhss_handle_tx_done( state->config->radio, &state->sx126x_lr_fhss_params, &state->lr_fhss_state );
    SXLIB_LOG( SXLIB_LOG_APP, ( "Packet sent!" SXLIB_LOG_EOL ) );
    sxlib_Gpio_Led_off( state->config->interface->led_interface.led_tx );
    enter_standby_then_sleep_mode( state );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

static void set_standby_clear_all_irqs( const void* context )
{
    sx126x_set_standby( context, SX126X_STANDBY_CFG_RC );
    sx126x_comp_clear_and_unpend_irqs( context );
}

static void enter_standby_then_sleep_mode( const sx126x_lr_fhss_ping_state_t* state )
{
    set_standby_clear_all_irqs( state->config->radio );
#if defined CONFIG_ALLOW_SMTC_RADIO_SLEEP
    sx126x_set_sleep( state->config->radio, SX126X_SLEEP_CFG_WARM_START );
#endif
}

static void exit_sleep_mode( const sx126x_lr_fhss_ping_state_t* state )
{
#if defined CONFIG_ALLOW_SMTC_RADIO_SLEEP
    sx126x_wakeup( state->config->radio );
#endif
}

static sx126x_status_t lr_fhss_send_packet( sx126x_lr_fhss_ping_state_t* state, const uint8_t* payload,
                                            uint16_t payload_length )
{
    SXLIB_LOG_HEX( SXLIB_LOG_APP, "Packet to send: ", payload, payload_length );

    state->sx126x_lr_fhss_params.lr_fhss_params.modulation_type = LR_FHSS_V1_MODULATION_TYPE_GMSK_488;
    state->sx126x_lr_fhss_params.lr_fhss_params.cr              = LR_FHSS_V1_CR_5_6;
    state->sx126x_lr_fhss_params.lr_fhss_params.grid            = LR_FHSS_V1_GRID_3906_HZ;
    state->sx126x_lr_fhss_params.lr_fhss_params.enable_hopping  = true;
    state->sx126x_lr_fhss_params.lr_fhss_params.bw              = LR_FHSS_V1_BW_136719_HZ;
    state->sx126x_lr_fhss_params.lr_fhss_params.header_count    = 2;
    state->sx126x_lr_fhss_params.lr_fhss_params.sync_word       = lr_fhss_sync_word;
    state->sx126x_lr_fhss_params.center_freq_in_pll_steps       = sx126x_convert_freq_in_hz_to_pll_step( RF_FREQUENCY );
    state->sx126x_lr_fhss_params.device_offset                  = 0;

    sx126x_status_t status = sx126x_lr_fhss_init( state->config->radio, &state->sx126x_lr_fhss_params );
    if( status != SX126X_STATUS_OK )
    {
        return status;
    }

    const uint16_t hop_sequence_id = rand( ) % sx126x_lr_fhss_get_hop_sequence_count( &state->sx126x_lr_fhss_params );

    // This performs sx126x_set_pa_cfg, sx126x_set_ocp_value, and sx126x_set_tx_params
    sx126x_comp_set_tx_cfg( state->config->radio, POWER_IN_DBM, RF_FREQUENCY );

    // If TCXO is used, this calls sx126x_set_dio3_as_tcxo_ctrl(), with a board-appropriate voltage
    sx126x_comp_set_tcxo_startup_time_in_steps( state->config->radio, 640 * 10 );  // 10ms = 640

    status = sx126x_lr_fhss_build_frame( state->config->radio, &state->sx126x_lr_fhss_params, &state->lr_fhss_state,
                                         hop_sequence_id, payload, payload_length, 0 );
    if( status != SX126X_STATUS_OK )
    {
        return status;
    }

    return sx126x_set_tx( state->config->radio, 0 );
}

/* --- EOF ------------------------------------------------------------------ */
