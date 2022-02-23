/**
 * \file
 *
 * \brief LR1110 driver HAL implementation of untimed operations
 *
 * If CONFIG_SMTC_AUTO_WAKE_RADIO is set, the "const void* radio" must be backed by a non-const radio object, so that
 * the sleeping flag can be managed. This is necessary because the radio drivers take a const void*, but LoRa Basics
 * Modem expects the radio driver to wake the radio automatically, which only possible if some mutable state is
 * associated with the radio.
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

#include <sxlib/System/config/Rtos_config.h>

#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>
#include <sxlib/Debug/Assert/Assert.h>
#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio_impl.h>
#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>

#include "lr1110_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
// This is necessary only because the driver does not export command IDs.
#define LR1110_SYSTEM_SET_SLEEP_OC_H ( 0x01 )
#define LR1110_SYSTEM_SET_SLEEP_OC_L ( 0x1B )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ---------------------------------------------
 */

void                lr1110_hal_wakeup_plain( const sxlib_Radio_plain_spi_radio_t* radio );
lr1110_hal_status_t lr1110_hal_reset_plain( const void* context );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

static inline void wait_until_ready( const struct sxlib_Gpio_Input_inst* busy_line )
{
    while( sxlib_Gpio_Input_get( busy_line ) == SXLIB_GPIO_INPUT_HIGH )
    {
    }
}

static inline void wait_until_awake_and_ready( const sxlib_Radio_plain_spi_radio_t* radio )
{
#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    sxlib_Radio_plain_spi_radio_t* nonconst_radio = ( sxlib_Radio_plain_spi_radio_t* ) radio;

    if( nonconst_radio->sleeping == true )
    {
        lr1110_hal_wakeup_plain( radio );
        nonconst_radio->sleeping = false;
    }
#endif
    wait_until_ready( radio->busy );
}

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

lr1110_hal_status_t lr1110_hal_wakeup( const void* context )
{
#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    // This returns faster than wait_until_awake_and_ready, since it doesn't wait until
    // the chip is ready, permitting background operations.
    sxlib_Radio_plain_spi_radio_t* radio = ( sxlib_Radio_plain_spi_radio_t* ) context;

    if( radio->sleeping == true )
    {
        lr1110_hal_wakeup_plain( context );
        radio->sleeping = false;
    }
#else
    lr1110_hal_wakeup_plain( context );
#endif

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_reset( const void* context )
{
    sxlib_Radio_plain_spi_radio_t* radio = ( sxlib_Radio_plain_spi_radio_t* ) context;

#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    radio->sleeping = false;
#endif

    return lr1110_hal_reset_plain( context );
}

lr1110_hal_status_t lr1110_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    sxlib_Radio_plain_spi_radio_t* radio = ( sxlib_Radio_plain_spi_radio_t* ) context;
#else
    const sxlib_Radio_plain_spi_radio_t* radio = ( const sxlib_Radio_plain_spi_radio_t* ) context;
#endif

    wait_until_awake_and_ready( radio );
    sxlib_Comm_spi_device_CS( radio->spi );
    sxlib_Comm_spi_device_write_then_write( radio->spi, command, command_length, data, data_length );
    sxlib_Comm_spi_device_nCS( radio->spi );

#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    if( ( command[0] == LR1110_SYSTEM_SET_SLEEP_OC_H ) && ( command[1] == LR1110_SYSTEM_SET_SLEEP_OC_L ) )
    {
        radio->sleeping = true;
    }
#endif

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    const sxlib_Radio_plain_spi_radio_t* radio = ( const sxlib_Radio_plain_spi_radio_t* ) context;

    wait_until_awake_and_ready( radio );

    // If command_length == 0, perform a simple multi-byte read (while writing zeros/NOP on the SPI bus)
    // This is needed by lr1110_system_get_stat1_stat2().

    if( command_length > 0 )
    {
        sxlib_Comm_spi_device_CS( radio->spi );
        sxlib_Comm_spi_device_write( radio->spi, command, command_length );
        sxlib_Comm_spi_device_nCS( radio->spi );

        if( data_length > 0 )
        {
            wait_until_awake_and_ready( radio );
            sxlib_Comm_spi_device_CS( radio->spi );
            uint8_t dummy;
            sxlib_Comm_spi_device_read( radio->spi, &dummy, sizeof( dummy ) );
            sxlib_Comm_spi_device_read( radio->spi, data, data_length );
            sxlib_Comm_spi_device_nCS( radio->spi );
        }
    }
    else
    {
        sxlib_Comm_spi_device_CS( radio->spi );
        sxlib_Comm_spi_device_read( radio->spi, data, data_length );
        sxlib_Comm_spi_device_nCS( radio->spi );
    }

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_direct_read( const void* context, uint8_t* data, const uint16_t data_length )
{
    return lr1110_hal_read( context, 0, 0, data, data_length );
}

/** This function is deprecated */
lr1110_hal_status_t lr1110_hal_write_read( const void* context, const uint8_t* command, uint8_t* data,
                                           const uint16_t data_length )
{
    sxlib_assert( 0 );
    return LR1110_HAL_STATUS_ERROR;
}

/* --- EOF ------------------------------------------------------------------ */
