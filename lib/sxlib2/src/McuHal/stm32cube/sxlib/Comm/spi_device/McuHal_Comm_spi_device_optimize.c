/**
 * \file
 *
 * \brief Secondary McuHal-specific implementation for SPI communication
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

#include <stdint.h>

#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS ---------------------------------------------
 */

#ifdef SXTOOLS_McuFamily_STM32L0xx
static void wait_then_read( SPI_TypeDef* inst, uint8_t* data )
{
    while( !LL_SPI_IsActiveFlag_RXNE( inst ) )
        ;
    *data = LL_SPI_ReceiveData8( inst );
}

#ifdef SXLIB_COMM_SPI_OPTIMIZE_CONTINUOUS
// WARNING! If SXLIB_COMM_SPI_OPTIMIZE_CONTINUOUS is set, sxlib_Comm_spi_device_fast_write_then_read IS TIME CRITICAL
// AND CAN FAIL IF THE MCU CORE DOES NOT RESPOND FAST ENOUGH TO PREVENT RX OVERRUN.
static void wait_then_write( SPI_TypeDef* inst, uint8_t data )
{
    while( !LL_SPI_IsActiveFlag_TXE( inst ) )
        ;
    LL_SPI_TransmitData8( inst, data );
}

void sxlib_Comm_spi_device_fast_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{
    uint8_t      dummy;
    uint8_t      tx_data;
    SPI_TypeDef* inst = dev->cont->config->controller_device;

    LL_SPI_TransmitData8( inst, command[0] );

    for( uint16_t tx_i = 1; tx_i < command_length + data_length; tx_i++ )
    {
        if( tx_i < command_length )
        {
            tx_data = command[tx_i];
        }
        else
        {
            tx_data = data[tx_i - command_length];
        }
        wait_then_write( inst, tx_data );
        wait_then_read( inst, &dummy );
    }
    wait_then_read( inst, &dummy );
}

void sxlib_Comm_spi_device_fast_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
    SPI_TypeDef* inst = dev->cont->config->controller_device;

    uint8_t tx_data;

    uint16_t rx_i = 0;
    uint8_t  dummy;
    uint8_t* rx_data_ptr = &dummy;

    LL_SPI_TransmitData8( inst, command[0] );

    for( uint16_t tx_i = 1; tx_i < command_length + data_length; tx_i++ )
    {
        if( tx_i < command_length )
        {
            tx_data = command[tx_i];
        }
        else
        {
            tx_data = 0;
        }
        wait_then_write( inst, tx_data );

        if( rx_i >= command_length )
        {
            rx_data_ptr = &data[rx_i - command_length];
        }
        wait_then_read( inst, rx_data_ptr );
        rx_i++;
    }
    wait_then_read( inst, &data[data_length - 1] );
}
#else   // SXLIB_COMM_SPI_OPTIMIZE_CONTINUOUS
void sxlib_Comm_spi_device_fast_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{
    uint8_t      dummy;
    uint8_t      tx_data;
    SPI_TypeDef* inst = dev->cont->config->controller_device;

    for( uint16_t tx_i = 0; tx_i < command_length + data_length; tx_i++ )
    {
        if( tx_i < command_length )
        {
            tx_data = command[tx_i];
        }
        else
        {
            tx_data = data[tx_i - command_length];
        }
        LL_SPI_TransmitData8( inst, tx_data );
        wait_then_read( inst, &dummy );
    }
}

void sxlib_Comm_spi_device_fast_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
    SPI_TypeDef* inst = dev->cont->config->controller_device;

    uint8_t tx_data;

    uint16_t rx_i = 0;
    uint8_t  dummy;
    uint8_t* rx_data_ptr = &dummy;

    for( uint16_t tx_i = 0; tx_i < command_length + data_length; tx_i++ )
    {
        if( tx_i < command_length )
        {
            tx_data = command[tx_i];
        }
        else
        {
            tx_data = 0;
        }
        LL_SPI_TransmitData8( inst, tx_data );

        if( rx_i >= command_length )
        {
            rx_data_ptr = &data[rx_i - command_length];
        }
        wait_then_read( inst, rx_data_ptr );
        rx_i++;
    }
}
#endif  // SXLIB_COMM_SPI_OPTIMIZE_CONTINUOUS
#else
static inline void sxlib_Comm_spi_device_wait_until_done( const sxlib_Comm_spi_device_t* dev )
{
    while( LL_SPI_GetTxFIFOLevel( dev->cont->config->controller_device ) != LL_SPI_TX_FIFO_EMPTY )
        ;
    while( LL_SPI_IsActiveFlag_BSY( dev->cont->config->controller_device ) )
        ;
    while( LL_SPI_GetRxFIFOLevel( dev->cont->config->controller_device ) != LL_SPI_TX_FIFO_EMPTY )
    {
        LL_SPI_ReceiveData8( dev->cont->config->controller_device );
    }
}

void sxlib_Comm_spi_device_fast_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{
    // Always keep the FIFO full
    const uint8_t* d_ptr;

    if( command )
    {
        d_ptr = command;
        while( d_ptr < command + command_length )
        {
            if( LL_SPI_IsActiveFlag_TXE( dev->cont->config->controller_device ) )
            {
                LL_SPI_TransmitData8( dev->cont->config->controller_device, *d_ptr++ );
            }
        }
    }

    if( data )
    {
        d_ptr = data;
        while( d_ptr < data + data_length )
        {
            if( LL_SPI_IsActiveFlag_TXE( dev->cont->config->controller_device ) )
            {
                LL_SPI_TransmitData8( dev->cont->config->controller_device, *d_ptr++ );
            }
        }
    }
    sxlib_Comm_spi_device_wait_until_done( dev );
}

void sxlib_Comm_spi_device_fast_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
    unsigned int rx_i = 0;
    unsigned int tx_i = 0;

    // Always keep the FIFO full
    while( rx_i < command_length + data_length )
    {
        if( ( LL_SPI_IsActiveFlag_TXE( dev->cont->config->controller_device ) ) &&
            ( tx_i < command_length + data_length ) )
        {
            uint8_t d = ( tx_i < command_length ) ? command[tx_i] : 0;
            tx_i++;
            LL_SPI_TransmitData8( dev->cont->config->controller_device, d );
        }

        if( LL_SPI_IsActiveFlag_RXNE( dev->cont->config->controller_device ) )
        {
            uint8_t d = LL_SPI_ReceiveData8( dev->cont->config->controller_device );
            if( rx_i >= command_length )
                data[rx_i - command_length] = d;
            rx_i++;
        }
    }
    sxlib_Comm_spi_device_wait_until_done( dev );
}
#endif  // SXTOOLS_McuFamily_STM32L4xx

/* --- EOF ------------------------------------------------------------------ */
