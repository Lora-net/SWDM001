/**
 * \file
 *
 * \brief Hardware Abstraction Layer for LR1110 complementary driver
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

#ifndef LR1110_COMP_HAL_H
#define LR1110_COMP_HAL_H

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr1110_hal.h"
#include "sx_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This helper can be used to obtain recommended PA, OCP, and TxParams values for a power/frequency pair
 *
 * @param [in]  output_pwr_in_dbm  The Tx output power in dBm
 * @param [in]  rf_freq_in_hz      Requested RF frequency
 * @param [out] proposal           A proposal structure for storing the requested configuration
 *
 * @returns Operation status
 */
void lr1110_comp_get_tx_cfg_proposal( int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz,
                                      lr1110_comp_tx_cfg_proposal_t* proposal );

/**
 * This helper can be used to configure PA, OCP, and TxParams from a proposal
 *
 * @param [in] context            Chip implementation context.
 * @param [in] ramp_time          The ramp time configuration for the PA
 * @param [in] proposal           A proposal structure containing the requested configuration
 *
 * @returns Operation status
 */
sx_comp_status_t lr1110_comp_set_tx_cfg_from_proposal( const void* context, lr1110_radio_ramp_time_t ramp_time,
                                                       const lr1110_comp_tx_cfg_proposal_t* proposal );

/**
 * This helper can be used by \ref lr1110_comp_hal_set_tx_cfg() to configure PA, OCP, and TxParams
 *
 * @param [in] context            Chip implementation context.
 * @param [in] radio_data         Transceiver configuration data
 * @param [in] output_pwr_in_dbm  The Tx output power in dBm
 * @param [in] ramp_time          The ramp time configuration for the PA
 * @param [in] rf_freq_in_hz      Requested RF frequency
 *
 * @returns Operation status
 */
sx_comp_status_t lr1110_comp_set_tx_cfg_helper( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, lr1110_radio_ramp_time_t ramp_time,
                                                uint32_t rf_freq_in_hz );

/**
 * Clear all transceiver irqs as well as any MCU pending status
 *
 * @param [in] context     Chip implementation context.
 * @param [in] radio_data  Transceiver configuration data
 *
 * @remark If this function is called after entering sleep or standby, no previously pending interrupt will be delivered
 *
 * @returns Operation status
 */
lr1110_hal_status_t lr1110_comp_hal_clear_and_unpend_irqs( const void*                 context,
                                                           const sx_comp_radio_data_t* radio_data );

/**
 * Clear MCU pending status for all transceiver irqs
 *
 * @param [in] context     Chip implementation context.
 * @param [in] radio_data  Transceiver configuration data
 *
 * @remark If this function is called while the chip is in sleep or standby mode, and no transceiver interrupts are
 * currently active, then no previously pending interrupt will be delivered
 *
 * @returns Operation status
 */
lr1110_hal_status_t lr1110_comp_hal_unpend_irqs( const void* context, const sx_comp_radio_data_t* radio_data );

/**
 * Perform antenna reconfiguration as indicated in request
 *
 * @param [in] context     Chip implementation context.
 * @param [in] radio_data  Transceiver configuration data
 * @param [in] request     Configuration request; If request = lr1110_comp_antenna_switch_configure_radio, call
 * @ref lr1110_set_dio2_as_rf_sw_ctrl if needed
 *
 * @returns Operation status
 */
lr1110_hal_status_t lr1110_comp_hal_antenna_switch_request( const void* context, const sx_comp_radio_data_t* radio_data,
                                                            sx_comp_antenna_switch_request_t request );

/**
 * Enter standby mode
 *
 * @param [in] context      Chip implementation context.
 * @param [in] radio_data   Transceiver configuration data
 * @param [in] standby_cfg  Requested standby mode. If equal to SX_COMP_STANDBY_CFG_DEFAULT, provide a platform-specific
 * default.
 *
 * @returns Operation status
 */
lr1110_status_t lr1110_comp_hal_set_standby( const void* context, const sx_comp_radio_data_t* radio_data,
                                             sx_comp_standby_cfg_t standby_cfg );

/**
 * Configure PA, OCP, and TxParams, as needed
 *
 * @param [in] context            Chip implementation context.
 * @param [in] radio_data         Transceiver configuration data
 * @param [in] output_pwr_in_dbm  The Tx output power in dBm
 * @param [in] rf_freq_in_hz      Requested RF frequency
 *
 * Note that @ref lr1110_comp_set_tx_cfg_helper can be called by this function, if needed.
 *
 * @returns Operation status
 */
lr1110_hal_status_t lr1110_comp_hal_set_tx_cfg( const void* context, const sx_comp_radio_data_t* radio_data,
                                                int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz );

/**
 * Configure the TCXO startup time, as requested
 *
 * @param [in] context       Chip implementation context.
 * @param [in] radio_data    Transceiver configuration data
 * @param [in] startup_time  Requested TCXO startup time
 *
 * @remark This board support function can specify the TCXO voltage, as needed by the TCXO
 *
 * @returns Operation status
 */
lr1110_hal_status_t lr1110_comp_hal_set_tcxo_startup_time_in_steps( const void*                 context,
                                                                    const sx_comp_radio_data_t* radio_data,
                                                                    uint32_t                    startup_time );

#ifdef __cplusplus
}
#endif

#endif  // LR1110_COMP_HAL_H

/* --- EOF ------------------------------------------------------------------ */
