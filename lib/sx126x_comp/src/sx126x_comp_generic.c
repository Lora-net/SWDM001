/**
 * \file
 *
 * \brief SX126x radio complementary driver implementation
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

#include "sx126x_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

struct sx126x_comp_pa_data_s
{
    sx126x_pa_cfg_params_t pa_cfg;
    int8_t                 pwr_offset;
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

const struct sx126x_comp_pa_data_s sx1261_comp_pa_data[] = {
    {
        // 14dBm or less
        .pa_cfg = {
            .pa_duty_cycle = 4,
            .hp_max = 0,
            .device_sel = 1,
            .pa_lut = 1,
        },
        .pwr_offset = 0,
    },
    {
        // 15dBm
        .pa_cfg = {
            .pa_duty_cycle = 6,
            .hp_max = 0,
            .device_sel = 1,
            .pa_lut = 1,
        },
        .pwr_offset = -1,
    },
};

const struct sx126x_comp_pa_data_s sx1262_comp_pa_data[] = {
    {
        // 22dBm (down to 21dBm)
        .pa_cfg = {
            .pa_duty_cycle = 4,
            .hp_max = 7,
            .device_sel = 0,
            .pa_lut = 1,
        },
        .pwr_offset = 0,
    },
};

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sx1261_comp_get_tx_cfg_proposal( int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz,
                                      sx126x_comp_tx_cfg_proposal_t* proposal )
{
    const struct sx126x_comp_pa_data_s* data;

    if( output_pwr_in_dbm <= 14 )
    {
        if( output_pwr_in_dbm < -17 )
        {
            output_pwr_in_dbm = -17;
        }
        data = &sx1261_comp_pa_data[0];
    }
    else
    {
        output_pwr_in_dbm = 15;
        data              = &sx1261_comp_pa_data[1];
    }

    proposal->pa_cfg                       = data->pa_cfg;
    proposal->output_pwr_in_dbm_expected   = output_pwr_in_dbm;
    proposal->output_pwr_in_dbm_configured = output_pwr_in_dbm + data->pwr_offset;
    proposal->ocp_value                    = SX126X_OCP_PARAM_VALUE_60_MA;
}

void sx1262_comp_get_tx_cfg_proposal( int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz,
                                      sx126x_comp_tx_cfg_proposal_t* proposal )
{
    const struct sx126x_comp_pa_data_s* data;

    if( output_pwr_in_dbm <= 22 )
    {
        if( output_pwr_in_dbm < -9 )
        {
            output_pwr_in_dbm = -9;
        }
        data = &sx1262_comp_pa_data[0];
    }
    else
    {
        output_pwr_in_dbm = 22;
        data              = &sx1262_comp_pa_data[0];
    }

    proposal->pa_cfg                       = data->pa_cfg;
    proposal->output_pwr_in_dbm_expected   = output_pwr_in_dbm;
    proposal->output_pwr_in_dbm_configured = output_pwr_in_dbm + data->pwr_offset;
    proposal->ocp_value                    = SX126X_OCP_PARAM_VALUE_140_MA;
}

void sx126x_comp_get_tx_cfg_proposal( const sx_comp_radio_data_t* radio_data, int8_t output_pwr_in_dbm,
                                      uint32_t rf_freq_in_hz, sx126x_comp_tx_cfg_proposal_t* proposal )
{
    if( ( radio_data->radio_flags & SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX126X_TYPE_MASK ) ==
        SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX1262 )
    {
        sx1262_comp_get_tx_cfg_proposal( output_pwr_in_dbm, rf_freq_in_hz, proposal );
    }
    else
    {
        sx1261_comp_get_tx_cfg_proposal( output_pwr_in_dbm, rf_freq_in_hz, proposal );
    }
}

sx_comp_status_t sx126x_comp_set_tx_cfg_from_proposal( const void* context, sx126x_ramp_time_t ramp_time,
                                                       const sx126x_comp_tx_cfg_proposal_t* proposal )
{
    sx_comp_status_t status = ( sx_comp_status_t ) sx126x_set_pa_cfg( context, &proposal->pa_cfg );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    status = ( sx_comp_status_t ) sx126x_set_ocp_value( context, proposal->ocp_value );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    return ( sx_comp_status_t ) sx126x_set_tx_params( context, proposal->output_pwr_in_dbm_configured, ramp_time );
}

sx_comp_status_t sx126x_comp_set_tx_cfg_helper( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, sx126x_ramp_time_t ramp_time,
                                                uint32_t rf_freq_in_hz )
{
    sx126x_comp_tx_cfg_proposal_t tx_cfg_proposal;
    sx126x_comp_get_tx_cfg_proposal( radio_data, output_pwr_in_dbm, rf_freq_in_hz, &tx_cfg_proposal );

    return sx126x_comp_set_tx_cfg_from_proposal( context, ramp_time, &tx_cfg_proposal );
}

sx_comp_status_t sx126x_comp_read_buffer( const void* context, uint8_t offset, uint8_t* buffer, uint16_t size )
{
    return ( sx_comp_status_t ) sx126x_read_buffer( context, offset, buffer, size );
}

sx_comp_status_t sx126x_comp_write_buffer( const void* context, uint8_t offset, const uint8_t* buffer, uint16_t size )
{
    return ( sx_comp_status_t ) sx126x_write_buffer( context, offset, buffer, size );
}

sx_comp_status_t sx126x_comp_get_rx_buffer_interval( const void* context, uint8_t* offset, uint16_t* size )
{
    sx126x_rx_buffer_status_t rx_buffer_status;

    sx_comp_status_t status = ( sx_comp_status_t ) sx126x_get_rx_buffer_status( context, &rx_buffer_status );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }
    if( offset != 0 )
    {
        *offset = rx_buffer_status.buffer_start_pointer;
    }

    if( size != 0 )
    {
        *size = rx_buffer_status.pld_len_in_bytes;
    }
    return status;
}

sx_comp_status_t sx126x_comp_get_pkt_payload( const void* context, uint16_t max_size_in_bytes, uint8_t* buffer,
                                              uint16_t* size_in_bytes )
{
    uint8_t  offset;
    uint16_t size;

    sx_comp_status_t status = sx126x_comp_get_rx_buffer_interval( context, &offset, &size );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    if( size_in_bytes != 0 )
    {
        *size_in_bytes = size;
    }

    if( size <= max_size_in_bytes )
    {
        status = sx126x_comp_read_buffer( context, offset, buffer, size );
    }
    else
    {
        status = SX_COMP_STATUS_ERROR;
    }

    return status;
}

/* --- EOF ------------------------------------------------------------------ */
