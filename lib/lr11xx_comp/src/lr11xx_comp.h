/**
 * \file
 *
 * \brief LR11XX radio complementary driver API
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

#ifndef LR11XX_COMP_H
#define LR11XX_COMP_H

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "lr11xx_radio.h"
#include "lr11xx_regmem.h"
#include "lr11xx_system.h"

#include "sx_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

#define LR11XX_COMP_DRV_INSTANTIATE                                                                                   \
    {                                                                                                                 \
        .reset_and_init_base         = lr11xx_comp_radio_reset_and_init_base,                                         \
        .antenna_switch_request_base = lr11xx_comp_antenna_switch_request_base,                                       \
        .set_standby_base            = lr11xx_comp_set_standby_base,                                                  \
        .clear_and_unpend_irqs_base  = lr11xx_comp_clear_and_unpend_irqs_base,                                        \
        .unpend_irqs_base = lr11xx_comp_unpend_irqs_base, .set_tx_cfg_base = lr11xx_comp_set_tx_cfg_base,             \
        .set_tcxo_startup_time_in_steps_base = lr11xx_comp_set_tcxo_startup_time_in_steps_base,                       \
        .read_buffer = lr11xx_comp_read_buffer, .write_buffer = lr11xx_comp_write_buffer,                             \
        .get_rx_buffer_interval = lr11xx_comp_get_rx_buffer_interval, .get_pkt_payload = lr11xx_comp_get_pkt_payload, \
    }

#define LR11XX_COMP_INSTANTIATE( )             \
    {                                          \
        .driver = LR11XX_COMP_DRV_INSTANTIATE, \
    }

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

typedef struct lr11xx_comp_tx_cfg_proposal_s
{
    lr11xx_radio_pa_cfg_t pa_cfg;
    int8_t                output_pwr_in_dbm_configured;
    int8_t                output_pwr_in_dbm_expected;
} lr11xx_comp_tx_cfg_proposal_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- SIMPLE PUBLIC FUNCTION PROTOTYPES ---------------------------------------
 */

sx_comp_status_t lr11xx_comp_read_buffer( const void* context, uint8_t offset, uint8_t* buffer, uint16_t size );
sx_comp_status_t lr11xx_comp_write_buffer( const void* context, uint8_t offset, const uint8_t* buffer, uint16_t size );
sx_comp_status_t lr11xx_comp_get_rx_buffer_interval( const void* context, uint8_t* offset, uint16_t* size );
sx_comp_status_t lr11xx_comp_get_pkt_payload( const void* context, uint16_t max_size_in_bytes, uint8_t* buffer,
                                              uint16_t* size_in_bytes );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES NEEDING RADIO DATA ---------------------------
 */

sx_comp_status_t lr11xx_comp_radio_reset_and_init_base( const void* context, const sx_comp_radio_data_t* radio_data );

sx_comp_status_t lr11xx_comp_antenna_switch_request_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                                          sx_comp_antenna_switch_request_t request );

sx_comp_status_t lr11xx_comp_set_standby_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                               sx_comp_standby_cfg_t standby_cfg );

sx_comp_status_t lr11xx_comp_clear_and_unpend_irqs_base( const void* context, const sx_comp_radio_data_t* radio_data );

sx_comp_status_t lr11xx_comp_unpend_irqs_base( const void* context, const sx_comp_radio_data_t* radio_data );

sx_comp_status_t lr11xx_comp_set_tx_cfg_base( const void* context, const sx_comp_radio_data_t* radio_data,
                                              int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz );

sx_comp_status_t lr11xx_comp_set_tcxo_startup_time_in_steps_base( const void*                 context,
                                                                  const sx_comp_radio_data_t* radio_data,
                                                                  uint32_t                    startup_time );

/*
 * static inlines
 */

static inline sx_comp_status_t lr11xx_comp_radio_reset_and_init( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_radio_reset_and_init_base( context, &sx_comp->radio_data );
}

static inline sx_comp_status_t lr11xx_comp_antenna_switch_request( const void*                      context,
                                                                   sx_comp_antenna_switch_request_t request )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_antenna_switch_request_base( context, &sx_comp->radio_data, request );
}

static inline sx_comp_status_t lr11xx_comp_set_standby( const void* context, sx_comp_standby_cfg_t standby_cfg )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_set_standby_base( context, &sx_comp->radio_data, standby_cfg );
}

static inline sx_comp_status_t lr11xx_comp_clear_and_unpend_irqs( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_clear_and_unpend_irqs_base( context, &sx_comp->radio_data );
}

static inline sx_comp_status_t lr11xx_comp_unpend_irqs( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_unpend_irqs_base( context, &sx_comp->radio_data );
}

static inline sx_comp_status_t lr11xx_comp_set_tx_cfg( const void* context, int8_t output_pwr_in_dbm,
                                                       uint32_t rf_freq_in_hz )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_set_tx_cfg_base( context, &sx_comp->radio_data, output_pwr_in_dbm, rf_freq_in_hz );
}

static inline sx_comp_status_t lr11xx_comp_set_tcxo_startup_time_in_steps( const void* context, uint32_t startup_time )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return lr11xx_comp_set_tcxo_startup_time_in_steps_base( context, &sx_comp->radio_data, startup_time );
}

#ifdef __cplusplus
}
#endif

#endif  // LR11XX_COMP_H

/* --- EOF ------------------------------------------------------------------ */
