/**
 * \file
 *
 * \brief File describing and initializing main MCU board hardware
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

#include <sxlib/Debug/Assert/Assert.h>
#include <sxlib/Comm/serial/McuHal_serial_impl.h>
#include <sxlib/Debug/Assert/Assert.h>
#include <sxlib/Debug/Log/Generic_Log.h>
#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>
#include <sxlib/Gpio/Led/McuHal_Led_impl.h>

#include <sxlib/stm32_ll_base.h>

#include "smtc_stm32cube_arduino_compat.h"
#include "smtc_board.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

const struct sxlib_Gpio_Led_dev global_gpio_led_debug = {
    .port  = GPIOC,
    .pin   = 10,
    .flags = SXLIB_GPIO_LED_DEV_FLAGS_ACTIVE_HIGH,
};

const sxlib_Comm_spi_controller_config_t global_spi_controller_config = {
    .controller_device = SPI1,

    .sck_port  = SMTC_ARDUINO_PORT_D13,
    .miso_port = SMTC_ARDUINO_PORT_D12,
    .mosi_port = SMTC_ARDUINO_PORT_D11,

    .sck_alternate  = LL_GPIO_AF_5,
    .miso_alternate = LL_GPIO_AF_5,
    .mosi_alternate = LL_GPIO_AF_5,

    .sck_pin  = SMTC_ARDUINO_PIN_D13,
    .miso_pin = SMTC_ARDUINO_PIN_D12,
    .mosi_pin = SMTC_ARDUINO_PIN_D11,

    .flags = 0,
};

sxlib_Comm_spi_controller_t global_spi_controller;

#if( SXLIB_LOG_LEVEL > SXLIB_LOG_LEVEL_NONE )
static const sxlib_Comm_serial_config_t serial_config = {
    .init = {
        .BaudRate             = 115200,
        .DataWidth            = LL_USART_DATAWIDTH_8B,
        .StopBits             = LL_USART_STOPBITS_1,
        .Parity               = LL_USART_PARITY_NONE,
        .HardwareFlowControl  = LL_USART_HWCONTROL_NONE,
        .TransferDirection    = LL_USART_DIRECTION_TX_RX,
    },

    .uart_dev = USART2,

    .tx_port = GPIOA,
    .rx_port = GPIOA,

    .tx_alternate = LL_GPIO_AF_7,
    .rx_alternate = LL_GPIO_AF_7,

    .tx_pin = 2,
    .rx_pin = 3,
};

static sxlib_Comm_serial_t serial;
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void smtc_board_init( void )
{
    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOA );
    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOB );
    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOC );
    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SPI1 );
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

#if( SXLIB_LOG_LEVEL > SXLIB_LOG_LEVEL_NONE )
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_USART2 );
    serial.config = &serial_config;
    sxlib_Comm_serial_init( &serial );
    sxlib_Debug_Log_init( sxlib_Comm_serial_write, &serial );
#endif

    sxlib_Comm_spi_controller_init( &global_spi_controller, &global_spi_controller_config );
}

/* --- EOF ------------------------------------------------------------------ */
