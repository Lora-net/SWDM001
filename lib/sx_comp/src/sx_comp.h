/**
 * \file
 *
 * \brief Generic radio complementary driver API
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

#ifndef SX_COMP_H
#define SX_COMP_H

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_POS ( 0 )
#define SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_MASK ( 1UL << SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_POS )

#define SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO ( SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO_MASK )
#define SX_COMP_RADIO_DATA_SX_COMP_FLAGS_NO_TCXO ( 0 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

#include <stdint.h>

/**
 * sx_comp status codes, compatible with radio driver status codes.
 */
typedef enum sx_comp_status_e
{
    SX_COMP_STATUS_OK,
    SX_COMP_STATUS_UNSUPPORTED_FEATURE,
    SX_COMP_STATUS_UNKNOWN_VALUE,
    SX_COMP_STATUS_ERROR,
} sx_comp_status_t;

/**
 * Transceiver configuration data.
 */
typedef struct sx_comp_radio_data_s
{
    const char* name; /**< Radio part number, in lowercase to permit RAL driver matching */
    uint32_t    init_tcxo_startup_time_in_radio_units; /**< TCXO startup time in radio units */
    uint16_t    cal_img_freq1_in_mhz;                  /**< Used for post-reset calibration */
    uint16_t    cal_img_freq2_in_mhz;                  /**< Used for post-reset calibration */
    uint16_t    sx_comp_flags;                         /**< General flags */
    uint16_t    radio_flags;                           /**< Flags for a specific radio */
    uint8_t     radio_tcxo_voltage_in_radio_units;     /**< TCXO voltage in radio units */
} sx_comp_radio_data_t;

/**
 * Standby mode configurations.
 *
 * Default argument allows the caller to set standby mode, leaving oscillator choice to the BSP.
 */
typedef enum sx_comp_standby_cfg_e
{
    SX_COMP_STANDBY_CFG_DEFAULT = 0,
    SX_COMP_STANDBY_CFG_RC,
    SX_COMP_STANDBY_CFG_XOSC,
} sx_comp_standby_cfg_t;

/**
 * Antenna switch configuration requests
 */
typedef enum sx_comp_antenna_switch_request_e
{
    SX_COMP_ANTENNA_SWITCH_CONFIGURE_RADIO = 0,
    SX_COMP_ANTENNA_SWITCH_RX0,
    SX_COMP_ANTENNA_SWITCH_TX0,
    SX_COMP_ANTENNA_SWITCH_STANDBY,
    SX_COMP_ANTENNA_SWITCH_SLEEP,
} sx_comp_antenna_switch_request_t;

/*
 * -----------------------------------------------------------------------------
 * --- FUNCTION POINTER TYPES FOR FUNCTIONS NEEDING RADIO_DATA -----------------
 */

typedef const char* ( *sx_comp_radio_get_name_base_f )( const void* context, const sx_comp_radio_data_t* radio_data );

typedef sx_comp_status_t ( *sx_comp_radio_reset_and_init_base_f )( const void*                 context,
                                                                   const sx_comp_radio_data_t* radio_data );

typedef sx_comp_status_t ( *sx_comp_antenna_switch_request_base_f )( const void*                      context,
                                                                     const sx_comp_radio_data_t*      radio_data,
                                                                     sx_comp_antenna_switch_request_t request );

typedef sx_comp_status_t ( *sx_comp_set_standby_base_f )( const void* context, const sx_comp_radio_data_t* radio_data,
                                                          sx_comp_standby_cfg_t standby_cfg );

typedef sx_comp_status_t ( *sx_comp_clear_and_unpend_irqs_base_f )( const void*                 context,
                                                                    const sx_comp_radio_data_t* radio_data );

typedef sx_comp_status_t ( *sx_comp_unpend_irqs_base_f )( const void* context, const sx_comp_radio_data_t* radio_data );

typedef sx_comp_status_t ( *sx_comp_set_tx_cfg_base_f )( const void* context, const sx_comp_radio_data_t* radio_data,
                                                         int8_t output_pwr_in_dbm, uint32_t rf_freq_in_hz );

typedef sx_comp_status_t ( *sx_comp_set_tcxo_startup_time_in_steps_base_f )( const void*                 context,
                                                                             const sx_comp_radio_data_t* radio_data,
                                                                             uint32_t                    startup_time );

/*
 * -----------------------------------------------------------------------------
 * --- FUNCTION POINTER TYPES FOR SIMPLE FUNCTIONS -----------------------------
 */

typedef sx_comp_status_t ( *sx_comp_read_buffer_f )( const void* context, uint8_t offset, uint8_t* buffer,
                                                     uint16_t size );
typedef sx_comp_status_t ( *sx_comp_write_buffer_f )( const void* context, uint8_t offset, const uint8_t* buffer,
                                                      uint16_t size );
typedef sx_comp_status_t ( *sx_comp_get_rx_buffer_interval_f )( const void* context, uint8_t* offset, uint16_t* size );
typedef sx_comp_status_t ( *sx_comp_get_pkt_payload_f )( const void* context, uint16_t max_size_in_bytes,
                                                         uint8_t* buffer, uint16_t* size_in_bytes );

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function pointer storage
 */
typedef struct sx_comp_drv_s
{
    sx_comp_radio_get_name_base_f                 get_name_base;
    sx_comp_radio_reset_and_init_base_f           reset_and_init_base;
    sx_comp_antenna_switch_request_base_f         antenna_switch_request_base;
    sx_comp_set_standby_base_f                    set_standby_base;
    sx_comp_clear_and_unpend_irqs_base_f          clear_and_unpend_irqs_base;
    sx_comp_unpend_irqs_base_f                    unpend_irqs_base;
    sx_comp_set_tx_cfg_base_f                     set_tx_cfg_base;
    sx_comp_set_tcxo_startup_time_in_steps_base_f set_tcxo_startup_time_in_steps_base;
    sx_comp_read_buffer_f                         read_buffer;
    sx_comp_write_buffer_f                        write_buffer;
    sx_comp_get_rx_buffer_interval_f              get_rx_buffer_interval;
    sx_comp_get_pkt_payload_f                     get_pkt_payload;
} sx_comp_drv_t;

/**
 * sx_comp configuration and function pointers
 */
typedef struct sx_comp_s
{
    sx_comp_radio_data_t radio_data;
    sx_comp_drv_t        driver;
} sx_comp_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS -------------------------------------------------------
 */

/**
 * Used internally to implement context to sx_comp hook
 */
const sx_comp_t* sx_comp_get_obj( const void* context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS --------------------------------------------------------
 */

/**
 * @brief Register sx_comp to a context, permitting the sx_comp API to be called using a context argument
 *
 * @param [in]  sx_comp sx_comp context
 * @param [in]  context Chip implementation context
 *
 * @note This registers a sx_comp instance to a context by storing both pointers in a look-up table.
 *
 * @returns Operation status
 */
sx_comp_status_t sx_comp_register( const sx_comp_t* sx_comp, const void* context );

/**
 * @brief Unegister sx_comp from a context
 *
 * @param [in]  sx_comp sx_comp context
 * @param [in]  context Chip implementation context
 *
 * @note This undoes the operation performed by @ref sx_comp_register
 *
 * @returns Operation status
 */
sx_comp_status_t sx_comp_unregister( const sx_comp_t* sx_comp, const void* context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC STATIC INLINES FOR FUNCTIONS NEEDING RADIO_DATA ------------------
 */

/**
 * @brief Return the transceiver part number to permit RAL driver lookup

 *
 * @param [in]  context Chip implementation context
 *
 * @returns Transceiver part number, in lowercase
 */
static inline const char* sx_comp_radio_get_name( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->radio_data.name;
}

/**
 * @brief Reset and initialize radio
 *
 * @param [in]  context Chip implementation context
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_radio_reset_and_init( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.reset_and_init_base( context, &sx_comp->radio_data );
}

/**
 * @brief Request an antenna-related operation
 *
 * @param [in]  context Chip implementation context
 * @param [in]  request Desired antenna operation
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_antenna_switch_request( const void*                      context,
                                                               sx_comp_antenna_switch_request_t request )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.antenna_switch_request_base( context, &sx_comp->radio_data, request );
}

/**
 * @brief Enter standby mode (this function allows the BSP to define a default standby mode)
 *
 * @param [in]  context     Chip implementation context
 * @param [in]  standby_cfg Desired standby mode
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_set_standby( const void* context, sx_comp_standby_cfg_t standby_cfg )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.set_standby_base( context, &sx_comp->radio_data, standby_cfg );
}

/**
 * @brief Clear all interrupts, ensuring that no past event will be received by the event handler after this call
 * returns
 *
 * @param [in]  context Chip implementation context
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_clear_and_unpend_irqs( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.clear_and_unpend_irqs_base( context, &sx_comp->radio_data );
}

/**
 * @brief Unpend interrupts, ensuring that no past event will be received by the event handler after this call
 * returns
 *
 * @remark If this function is called while the chip is in sleep or standby mode, and no transceiver interrupts are
 * currently active, then no previously pending interrupt will be delivered
 *
 * @param [in]  context Chip implementation context
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_unpend_irqs( const void* context )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.unpend_irqs_base( context, &sx_comp->radio_data );
}

/**
 * @brief Configure the transmission-related parameters
 *
 * @param [in]  context           Chip implementation context
 * @param [in]  output_pwr_in_dbm Output power - in dBm
 * @param [in]  rf_freq_in_hz     RF frequency - in Hertz
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_set_tx_cfg( const void* context, int8_t output_pwr_in_dbm,
                                                   uint32_t rf_freq_in_hz )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.set_tx_cfg_base( context, &sx_comp->radio_data, output_pwr_in_dbm, rf_freq_in_hz );
}

/**
 * @brief Configure the TCXO startup time, if TCXO is present, letting the BSP set the proper TCXO voltage
 *
 * @param [in]  context      Chip implementation context
 * @param [in]  startup_time TCXO startup time, in transceiver units
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_set_tcxo_startup_time_in_steps( const void* context, uint32_t startup_time )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.set_tcxo_startup_time_in_steps_base( context, &sx_comp->radio_data, startup_time );
}

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC STATIC INLINES FOR SIMPLE FUNCTIONS ------------------------------
 */

/**
 * @brief Read data from radio Rx buffer memory space.
 *
 * @param [in]  context Chip implementation context
 * @param [in]  offset  Start address in the Rx buffer of the chip
 * @param [in]  buffer  The buffer of bytes to be filled with content from Rx radio buffer
 * @param [in]  size    The number of bytes to read from the Rx radio buffer
 *
 * @returns Operation status
 *
 * @see sx_comp_write_buffer
 */
static inline sx_comp_status_t sx_comp_read_buffer( const void* context, uint8_t offset, uint8_t* buffer,
                                                    uint16_t size )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.read_buffer( context, offset, buffer, size );
}

/**
 * @brief Write data into radio Tx buffer memory space.
 *
 * @param [in]  context Chip implementation context
 * @param [in]  offset  Start address in the Tx buffer of the chip
 * @param [in]  buffer  The buffer of bytes to write into radio buffer
 * @param [in]  size    The number of bytes to write into Tx radio buffer
 *
 * @returns Operation status
 *
 * @see sx_comp_read_buffer
 */
static inline sx_comp_status_t sx_comp_write_buffer( const void* context, uint8_t offset, const uint8_t* buffer,
                                                     uint16_t size )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.write_buffer( context, offset, buffer, size );
}

/**
 * @brief Determine the offset and size of last received data
 *
 * @param [in]  context Chip implementation context
 * @param [in]  offset  Data offset in the Rx buffer of the chip
 * @param [in]  size    Number of bytes received
 *
 * @returns Operation status
 *
 * @see sx_comp_read_buffer
 */
static inline sx_comp_status_t sx_comp_get_rx_buffer_interval( const void* context, uint8_t* offset, uint16_t* size )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.get_rx_buffer_interval( context, offset, size );
}

/**
 * @brief Fetch a received payload from the transceiver
 *
 * @param [in]  context           Chip implementation context
 * @param [in]  max_size_in_bytes Size of the application buffer - in bytes
 * @param [out] buffer            Pointer to the buffer to be filled with received data
 * @param [out] size_in_bytes     Size of the received buffer - in bytes
 *
 * @returns Operation status
 */
static inline sx_comp_status_t sx_comp_get_pkt_payload( const void* context, uint16_t max_size_in_bytes,
                                                        uint8_t* buffer, uint16_t* size_in_bytes )
{
    const sx_comp_t* sx_comp = sx_comp_get_obj( context );
    return sx_comp->driver.get_pkt_payload( context, max_size_in_bytes, buffer, size_in_bytes );
}

#ifdef __cplusplus
}
#endif

#endif  // SX_COMP_H

/* --- EOF ------------------------------------------------------------------ */
