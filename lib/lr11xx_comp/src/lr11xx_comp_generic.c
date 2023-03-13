/**
 * \file
 *
 * \brief LR11XX radio complementary driver implementation
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

#include "lr11xx_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

struct lr11xx_comp_pa_data_s
{
    lr11xx_radio_pa_cfg_t pa_cfg;
    int8_t                pwr_offset;
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

const struct lr11xx_comp_pa_data_s lr11xx_comp_pa_data[] = {
    {
        // High frequency, 13dBm or less
        .pa_cfg = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HF,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 4,
            .pa_hp_sel     = 0,
        },
        .pwr_offset = 0,
    },
    {
        // LP: 14dBm or less
        .pa_cfg = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 4,
            .pa_hp_sel     = 0,
        },
        .pwr_offset = 0,
    },
    {
        // LP: 15dBm
        .pa_cfg = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_LP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VREG,
            .pa_duty_cycle = 7,
            .pa_hp_sel     = 0,
        },
        .pwr_offset = -1,
    },
    {
        // HP: 22dBm (down to 16dBm)
        .pa_cfg = {
            .pa_sel        = LR11XX_RADIO_PA_SEL_HP,
            .pa_reg_supply = LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
            .pa_duty_cycle = 4,
            .pa_hp_sel     = 7,
        },
        .pwr_offset = 0,
    },
};

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void lr11xx_comp_get_tx_cfg_proposal( int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz,
                                      lr11xx_comp_tx_cfg_proposal_t* proposal )
{
    const struct lr11xx_comp_pa_data_s* data;

    if( rf_freq_in_hz > 2000000000UL )
    {
        if( output_pwr_in_dbm > 13 )
        {
            output_pwr_in_dbm = 13;
        }
        if( output_pwr_in_dbm < -9 )
        {
            output_pwr_in_dbm = -9;
        }
        data = &lr11xx_comp_pa_data[0];
    }
    else
    {
        if( output_pwr_in_dbm <= 14 )
        {
            if( output_pwr_in_dbm < -17 )
            {
                output_pwr_in_dbm = -17;
            }
            data = &lr11xx_comp_pa_data[1];
        }
        else if( output_pwr_in_dbm <= 15 )
        {
            data = &lr11xx_comp_pa_data[2];
        }
        else if( output_pwr_in_dbm <= 22 )
        {
            data = &lr11xx_comp_pa_data[3];
        }
        else
        {
            output_pwr_in_dbm = 22;
            data              = &lr11xx_comp_pa_data[3];
        }
    }

    proposal->pa_cfg                       = data->pa_cfg;
    proposal->output_pwr_in_dbm_expected   = output_pwr_in_dbm;
    proposal->output_pwr_in_dbm_configured = output_pwr_in_dbm + data->pwr_offset;
}

sx_comp_status_t lr11xx_comp_set_tx_cfg_from_proposal( const void* context, lr11xx_radio_ramp_time_t ramp_time,
                                                       const lr11xx_comp_tx_cfg_proposal_t* proposal )
{
    sx_comp_status_t status = ( sx_comp_status_t ) lr11xx_radio_set_pa_cfg( context, &proposal->pa_cfg );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    return ( sx_comp_status_t ) lr11xx_radio_set_tx_params( context, proposal->output_pwr_in_dbm_configured,
                                                            ramp_time );
}

sx_comp_status_t lr11xx_comp_set_tx_cfg_helper( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, lr11xx_radio_ramp_time_t ramp_time,
                                                uint32_t rf_freq_in_hz )
{
    lr11xx_comp_tx_cfg_proposal_t tx_cfg_proposal;
    lr11xx_comp_get_tx_cfg_proposal( output_pwr_in_dbm, rf_freq_in_hz, &tx_cfg_proposal );

    return lr11xx_comp_set_tx_cfg_from_proposal( context, ramp_time, &tx_cfg_proposal );
}

sx_comp_status_t lr11xx_comp_read_buffer( const void* context, uint8_t offset, uint8_t* buffer, uint16_t size )
{
    return ( sx_comp_status_t ) lr11xx_regmem_read_buffer8( context, buffer, offset, size );
}

sx_comp_status_t lr11xx_comp_write_buffer( const void* context, uint8_t offset, const uint8_t* buffer, uint16_t size )
{
    if( offset != 0 )
    {
        return ( sx_comp_status_t ) lr11xx_regmem_write_buffer8( context, buffer, size );
    }
    else
    {
        return SX_COMP_STATUS_ERROR;
    }
}

sx_comp_status_t lr11xx_comp_get_rx_buffer_interval( const void* context, uint8_t* offset, uint16_t* size )
{
    lr11xx_radio_rx_buffer_status_t rx_buffer_status;

    sx_comp_status_t status = ( sx_comp_status_t ) lr11xx_radio_get_rx_buffer_status( context, &rx_buffer_status );
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

sx_comp_status_t lr11xx_comp_get_pkt_payload( const void* context, uint16_t max_size_in_bytes, uint8_t* buffer,
                                              uint16_t* size_in_bytes )
{
    uint8_t  offset;
    uint16_t size;

    sx_comp_status_t status = lr11xx_comp_get_rx_buffer_interval( context, &offset, &size );
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
        status = lr11xx_comp_read_buffer( context, offset, buffer, size );
    }
    else
    {
        status = SX_COMP_STATUS_ERROR;
    }

    return status;
}

/* --- EOF ------------------------------------------------------------------ */
