/**
 * \file
 *
 * \brief LR1110 radio complementary driver implementation
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

#include "lr1110_radio.h"
#include "lr1110_radio_types.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

sx_comp_status_t lr1110_comp_antenna_switch_request_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                                          sx_comp_antenna_switch_request_t request )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_antenna_switch_request( context, radio_data, request );
}

sx_comp_status_t lr1110_comp_radio_reset_and_init_base( const void* context, const sx_comp_radio_data_t* radio_data )
{
    sx_comp_status_t status = ( sx_comp_status_t ) lr1110_system_reset( context );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    status = ( sx_comp_status_t ) lr1110_system_set_reg_mode( context, LR1110_SYSTEM_REG_MODE_DCDC );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    if( ( radio_data->sx_comp_flags & SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_MASK ) ==
        SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO )
    {
        status = ( sx_comp_status_t ) lr1110_comp_set_tcxo_startup_time_in_steps_base(
            context, radio_data, radio_data->init_tcxo_startup_time_in_radio_units );
        if( status != SX_COMP_STATUS_OK )
        {
            return status;
        }

        status = ( sx_comp_status_t ) lr1110_system_calibrate(
            context, LR1110_SYSTEM_CALIB_LF_RC_MASK | LR1110_SYSTEM_CALIB_HF_RC_MASK | LR1110_SYSTEM_CALIB_PLL_MASK |
                         LR1110_SYSTEM_CALIB_ADC_MASK | LR1110_SYSTEM_CALIB_IMG_MASK |
                         LR1110_SYSTEM_CALIB_PLL_TX_MASK );
        if( status != SX_COMP_STATUS_OK )
        {
            return status;
        }
    }

    lr1110_system_clear_errors( context );

    uint16_t errors;
    lr1110_system_get_errors( context, &errors );
    lr1110_system_clear_errors( context );

    lr1110_comp_clear_and_unpend_irqs_base( context, radio_data );

    status = lr1110_comp_antenna_switch_request_base( context, radio_data, SX_COMP_ANTENNA_SWITCH_CONFIGURE_RADIO );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    // Calibrate to board matching frequency
    status = ( sx_comp_status_t ) lr1110_system_calibrate_image_in_mhz( context, radio_data->cal_img_freq1_in_mhz,
                                                                        radio_data->cal_img_freq2_in_mhz );
    if( status != SX_COMP_STATUS_OK )
    {
        return status;
    }

    return lr1110_comp_set_standby_base( context, radio_data, SX_COMP_STANDBY_CFG_DEFAULT );
}

sx_comp_status_t lr1110_comp_clear_and_unpend_irqs_base( const void* context, const sx_comp_radio_data_t* radio_data )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_clear_and_unpend_irqs( context, radio_data );
}

sx_comp_status_t lr1110_comp_unpend_irqs_base( const void* context, const sx_comp_radio_data_t* radio_data )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_unpend_irqs( context, radio_data );
}

sx_comp_status_t lr1110_comp_set_tx_cfg_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                              int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_set_tx_cfg( context, radio_data, output_pwr_in_dbm, rf_freq_in_hz );
}

sx_comp_status_t lr1110_comp_set_tcxo_startup_time_in_steps_base( const void*                 context,
                                                                  const sx_comp_radio_data_t* radio_data,
                                                                  uint32_t                    startup_time )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_set_tcxo_startup_time_in_steps( context, radio_data, startup_time );
}

sx_comp_status_t lr1110_comp_set_standby_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                               sx_comp_standby_cfg_t standby_cfg )
{
    return ( sx_comp_status_t ) lr1110_comp_hal_set_standby( context, radio_data, standby_cfg );
}

/* --- EOF ------------------------------------------------------------------ */
