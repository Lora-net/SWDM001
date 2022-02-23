/**
 * \file
 *
 * \brief basic event handler interface header
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

#include "lr1110_comp.h"
#include "lr1110_comp_hal.h"

#include <sxlib/Radio/event_handler/Rtos_model_event_handler.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

const lr1110_system_rfswitch_cfg_t lr1110_system_rfswitch_cfg = {
    .enable =
        ( LR1110_SYSTEM_RFSW0_HIGH | LR1110_SYSTEM_RFSW1_HIGH | LR1110_SYSTEM_RFSW2_HIGH | LR1110_SYSTEM_RFSW3_HIGH ),
    .standby = 0x00,
    .tx      = LR1110_SYSTEM_RFSW0_HIGH | LR1110_SYSTEM_RFSW1_HIGH,
    .tx_hp   = LR1110_SYSTEM_RFSW1_HIGH,
    .rx      = LR1110_SYSTEM_RFSW0_HIGH,
    .wifi    = LR1110_SYSTEM_RFSW3_HIGH,
    .gnss    = LR1110_SYSTEM_RFSW2_HIGH,
};

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

lr1110_hal_status_t lr1110_comp_hal_clear_and_unpend_irqs( const void* context, const sx_comp_radio_data_t* radio_data )
{
    lr1110_system_get_and_clear_irq_status( context, 0 );
    sxlib_Radio_event_handler_cancel_event( context );
    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_comp_hal_unpend_irqs( const void* context, const sx_comp_radio_data_t* radio_data )
{
    sxlib_Radio_event_handler_cancel_event( context );
    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_comp_hal_set_tcxo_startup_time_in_steps( const void*                 context,
                                                                    const sx_comp_radio_data_t* radio_data,
                                                                    uint32_t                    startup_time )
{
    lr1110_hal_status_t status = LR1110_HAL_STATUS_OK;

    lr1110_system_tcxo_supply_voltage_t tcxo_voltage =
        ( lr1110_system_tcxo_supply_voltage_t ) radio_data->radio_tcxo_voltage_in_radio_units;
    if( ( radio_data->sx_comp_flags & SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_MASK ) ==
        SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO )
    {
        status = ( lr1110_hal_status_t ) lr1110_system_set_tcxo_mode( context, tcxo_voltage, startup_time );
    }

    return status;
}

lr1110_hal_status_t lr1110_comp_hal_antenna_switch_request( const void* context, const sx_comp_radio_data_t* radio_data,
                                                            sx_comp_antenna_switch_request_t request )
{
    switch( request )
    {
    case SX_COMP_ANTENNA_SWITCH_CONFIGURE_RADIO:
        lr1110_system_set_dio_as_rf_switch( context, &lr1110_system_rfswitch_cfg );
        break;
    case SX_COMP_ANTENNA_SWITCH_TX0:
        break;
    case SX_COMP_ANTENNA_SWITCH_RX0:
        break;
    case SX_COMP_ANTENNA_SWITCH_STANDBY:
        break;
    case SX_COMP_ANTENNA_SWITCH_SLEEP:
        break;
    }
    return LR1110_HAL_STATUS_OK;
}

lr1110_status_t lr1110_comp_hal_set_standby( const void* context, const sx_comp_radio_data_t* radio_data,
                                             sx_comp_standby_cfg_t standby_cfg )
{
    // Default is RC oscillator
    lr1110_system_standby_cfg_t cfg = LR1110_SYSTEM_STANDBY_CFG_RC;

    if( standby_cfg == SX_COMP_STANDBY_CFG_XOSC )
    {
        cfg = LR1110_SYSTEM_STANDBY_CFG_XOSC;
    }

    return lr1110_system_set_standby( context, cfg );
}

lr1110_hal_status_t lr1110_comp_hal_set_tx_cfg( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz )
{
    lr1110_comp_tx_cfg_proposal_t tx_cfg_proposal;

    lr1110_comp_get_tx_cfg_proposal( output_pwr_in_dbm, rf_freq_in_hz, &tx_cfg_proposal );

    return ( lr1110_hal_status_t ) lr1110_comp_set_tx_cfg_from_proposal( context, LR1110_RADIO_RAMP_208_US,
                                                                         &tx_cfg_proposal );
}

/* --- EOF ------------------------------------------------------------------ */
