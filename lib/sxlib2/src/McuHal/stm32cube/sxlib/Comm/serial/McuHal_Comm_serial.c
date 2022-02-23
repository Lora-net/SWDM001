/**
 * \file
 *
 * \brief McuHal-specific implementation for serial output devices
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

#include <sxlib/Comm/serial/McuHal_serial_impl.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Comm_serial_init( sxlib_Comm_serial_t* serial )
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;

    LL_USART_Init( serial->config->uart_dev, ( LL_USART_InitTypeDef* ) &serial->config->init );
    LL_USART_ConfigAsyncMode( serial->config->uart_dev );
    LL_USART_Enable( serial->config->uart_dev );

    while( LL_USART_IsEnabled( serial->config->uart_dev ) == 0 )
        ;

    if( serial->config->rx_port )
    {
        GPIO_InitStruct.Pin       = 1 << serial->config->rx_pin;
        GPIO_InitStruct.Alternate = serial->config->rx_alternate;
        LL_GPIO_Init( serial->config->rx_port, &GPIO_InitStruct );
    }

    if( serial->config->tx_port )
    {
        LL_GPIO_SetPinSpeed(serial->config->tx_port, 1 << serial->config->tx_pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
        LL_GPIO_SetPinPull(serial->config->tx_port, 1 << serial->config->tx_pin, LL_GPIO_PULL_UP);
        if (serial->config->tx_pin < 8)
        {
            LL_GPIO_SetAFPin_0_7(serial->config->tx_port, 1 << serial->config->tx_pin, serial->config->tx_alternate);
        }
        else
        {
            LL_GPIO_SetAFPin_8_15(serial->config->tx_port, 1 << serial->config->tx_pin, serial->config->tx_alternate);
        }
        LL_GPIO_SetPinMode(serial->config->tx_port, 1 << serial->config->tx_pin, LL_GPIO_MODE_ALTERNATE);
    }
}

void sxlib_Comm_serial_write_char( sxlib_Comm_serial_t* serial, uint8_t c )
{
    while( !LL_USART_IsActiveFlag_TXE( serial->config->uart_dev ) )
        ;
    LL_USART_TransmitData8( serial->config->uart_dev, c );
}

void sxlib_Comm_serial_write( void* arg, const uint8_t* data, unsigned int len )
{
    sxlib_Comm_serial_t* dev = ( sxlib_Comm_serial_t* ) arg;

    const uint8_t* data_ptr = data;

    for( unsigned int i = 0; i < len; ++i )
    {
        sxlib_Comm_serial_write_char( dev, *data_ptr++ );
    }
}

/* --- EOF ------------------------------------------------------------------ */
