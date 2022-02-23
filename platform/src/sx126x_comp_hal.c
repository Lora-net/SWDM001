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

#include "sx126x.h"
#include "sx126x_comp.h"
#include "sx126x_comp_hal.h"

#include <sxlib/Radio/event_handler/Rtos_model_event_handler.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

sx126x_hal_status_t sx126x_comp_hal_clear_and_unpend_irqs( const void* context, const sx_comp_radio_data_t* radio_data )
{
    sx126x_get_and_clear_irq_status( context, 0 );
    sxlib_Radio_event_handler_cancel_event( context );
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_comp_hal_unpend_irqs( const void* context, const sx_comp_radio_data_t* radio_data )
{
    sxlib_Radio_event_handler_cancel_event( context );
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t sx126x_comp_hal_set_tcxo_startup_time_in_steps( const void*                 context,
                                                                    const sx_comp_radio_data_t* radio_data,
                                                                    uint32_t                    startup_time )
{
    sx126x_hal_status_t status = SX126X_HAL_STATUS_OK;

    sx126x_tcxo_ctrl_voltages_t tcxo_voltage =
        ( sx126x_tcxo_ctrl_voltages_t ) radio_data->radio_tcxo_voltage_in_radio_units;
    if( ( radio_data->sx_comp_flags & SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_MASK ) ==
        SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO )
    {
        status =
            ( sx126x_hal_status_t ) sx126x_set_dio3_as_tcxo_ctrl( context, tcxo_voltage, startup_time );  // 640 = 10ms
    }

    return status;
}

sx126x_status_t sx126x_comp_hal_set_standby( const void* context, const sx_comp_radio_data_t* radio_data,
                                             sx_comp_standby_cfg_t standby_cfg )
{
    // Default is RC oscillator
    sx126x_standby_cfg_t cfg = SX126X_STANDBY_CFG_RC;

    if( standby_cfg == SX_COMP_STANDBY_CFG_XOSC )
    {
        cfg = SX126X_STANDBY_CFG_XOSC;
    }

    return sx126x_set_standby( context, cfg );
}

sx126x_hal_status_t sx126x_comp_hal_set_tx_cfg( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz )
{
    sx126x_comp_tx_cfg_proposal_t tx_cfg_proposal;

    sx126x_comp_get_tx_cfg_proposal( radio_data, output_pwr_in_dbm, rf_freq_in_hz, &tx_cfg_proposal );

    return ( sx126x_hal_status_t ) sx126x_comp_set_tx_cfg_from_proposal( context, SX126X_RAMP_200_US,
                                                                         &tx_cfg_proposal );
}

/* --- EOF ------------------------------------------------------------------ */
