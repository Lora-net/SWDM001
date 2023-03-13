/**
 * @file      lr11xx_lr_fhss_ping.c
 *
 * @brief     LR11XX LR-FHSS ping demo, main example source code
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

#include "lr11xx_radio.h"
#include "lr11xx_system.h"
#include "lr11xx_comp.h"
#include "lr11xx_lr_fhss.h"

#include "demos/lr11xx_lr_fhss_ping/lr11xx_lr_fhss_ping.h"

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
static void enter_standby_then_sleep_mode( const lr11xx_lr_fhss_ping_state_t* state );

/**
 * @brief Exit sleep mode if CONFIG_ALLOW_SMTC_RADIO_SLEEP is defined
 *
 * @param [in] state           Pointer to application state
 */
static void exit_sleep_mode( const lr11xx_lr_fhss_ping_state_t* state );

/**
 * @brief Send the provided payload over LR-FHSS, using the parameters provided in this function
 *
 * @param [in,out] state      Pointer to application state
 * @param [in] payload        Pointer to application payload
 * @param [in] payload_length Length of application payload
 */
static lr11xx_status_t lr_fhss_send_packet( lr11xx_lr_fhss_ping_state_t* state, const uint8_t* payload,
                                            uint16_t payload_length );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void lr11xx_lr_fhss_ping_launch( lr11xx_lr_fhss_ping_state_t* state )
{
    sxlib_Gpio_Led_on( state->config->interface->led_interface.led_tx );

    lr11xx_status_t status = lr_fhss_send_packet( state, state->payload, state->payload_length );
    if( status != LR11XX_STATUS_OK )
    {
        sxlib_Gpio_Led_off( state->config->interface->led_interface.led_tx );
        SXLIB_LOG( SXLIB_LOG_APP, ( "Failed status=%d" SXLIB_LOG_EOL, status ) );
    }
}

void lr11xx_lr_fhss_ping_init( lr11xx_lr_fhss_ping_state_t* state, const lr11xx_lr_fhss_ping_config_t* config )
{
    state->config = config;

    SXLIB_LOG( SXLIB_LOG_APP, ( "LR11XX-LR-FHSS Ping Init" SXLIB_LOG_EOL ) );

    sxlib_Gpio_Led_off( state->config->interface->led_interface.led_tx );
    sxlib_Gpio_Led_off( state->config->interface->led_interface.led_rx );

    // Transceiver has already been reset in main()

    uint32_t seed = 0;
    lr11xx_system_get_random_number( state->config->radio, &seed );
    srand( seed );

    lr11xx_system_set_dio_irq_params( state->config->radio, LR11XX_SYSTEM_IRQ_TX_DONE, LR11XX_SYSTEM_IRQ_NONE );

    for( int i = 0; i < sizeof( state->payload ); ++i )
    {
        state->payload[i] = i;
    }
    state->payload_length = MIN_PAYLOAD_LENGTH;
}

void lr11xx_lr_fhss_ping_continue( lr11xx_lr_fhss_ping_state_t* state )
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

    lr11xx_lr_fhss_ping_launch( state );
}

void lr11xx_lr_fhss_ping_handle_tx_done( lr11xx_lr_fhss_ping_state_t* state )
{
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
    lr11xx_system_set_standby( context, LR11XX_SYSTEM_STANDBY_CFG_RC );
    lr11xx_comp_clear_and_unpend_irqs( context );
}

static void enter_standby_then_sleep_mode( const lr11xx_lr_fhss_ping_state_t* state )
{
    set_standby_clear_all_irqs( state->config->radio );
#if defined CONFIG_ALLOW_SMTC_RADIO_SLEEP
    const lr11xx_system_sleep_cfg_t sleep_cfg = { .is_warm_start = true, .is_rtc_timeout = false };
    lr11xx_system_set_sleep( state->config->radio, sleep_cfg, 0 );
#endif
}

static void exit_sleep_mode( const lr11xx_lr_fhss_ping_state_t* state )
{
#if defined CONFIG_ALLOW_SMTC_RADIO_SLEEP
    lr11xx_system_wakeup( state->config->radio );
#endif
}

static lr11xx_status_t lr_fhss_send_packet( lr11xx_lr_fhss_ping_state_t* state, const uint8_t* payload,
                                            uint16_t payload_length )
{
    lr11xx_system_stat1_t stat1;

    SXLIB_LOG_HEX( SXLIB_LOG_APP, "Packet to send: ", payload, payload_length );

    state->lr11xx_lr_fhss_params.lr_fhss_params.modulation_type = LR_FHSS_V1_MODULATION_TYPE_GMSK_488;
    state->lr11xx_lr_fhss_params.lr_fhss_params.cr              = LR_FHSS_V1_CR_5_6;
    state->lr11xx_lr_fhss_params.lr_fhss_params.grid            = LR_FHSS_V1_GRID_25391_HZ;
    state->lr11xx_lr_fhss_params.lr_fhss_params.enable_hopping  = true;
    state->lr11xx_lr_fhss_params.lr_fhss_params.bw              = LR_FHSS_V1_BW_1574219_HZ;
    state->lr11xx_lr_fhss_params.lr_fhss_params.header_count    = 2;
    state->lr11xx_lr_fhss_params.lr_fhss_params.sync_word       = lr_fhss_sync_word;
    state->lr11xx_lr_fhss_params.device_offset                  = 0;

    lr11xx_status_t status = lr11xx_lr_fhss_init( state->config->radio );
    if( status != LR11XX_STATUS_OK )
    {
        return status;
    }

    const uint16_t hop_sequence_id = rand( ) % lr11xx_lr_fhss_get_hop_sequence_count( &state->lr11xx_lr_fhss_params );

    status = lr11xx_radio_set_rf_freq( state->config->radio, RF_FREQUENCY );
    if( status != LR11XX_STATUS_OK )
    {
        return status;
    }

    // This performs lr11xx_radio_set_pa_cfg and lr11xx_radio_set_tx_params
    lr11xx_comp_set_tx_cfg( state->config->radio, POWER_IN_DBM, RF_FREQUENCY );

    // If TCXO is used, this calls lr11xx_system_set_tcxo_mode(), with a board-appropriate voltage
    lr11xx_comp_set_tcxo_startup_time_in_steps( state->config->radio, 328 * 10 );  // 10ms = 328

    status = lr11xx_lr_fhss_build_frame( state->config->radio, &state->lr11xx_lr_fhss_params, hop_sequence_id, payload,
                                         payload_length );
    if( status != LR11XX_STATUS_OK )
    {
        return status;
    }

    status = lr11xx_system_get_status( state->config->radio, &stat1, 0, 0 );
    if( status != LR11XX_STATUS_OK )
    {
        return status;
    }

    if( stat1.command_status != LR11XX_SYSTEM_CMD_STATUS_OK )
    {
        return LR11XX_STATUS_ERROR;
    }

    return lr11xx_radio_set_tx( state->config->radio, 0 );
}

/* --- EOF ------------------------------------------------------------------ */
