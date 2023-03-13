/**
 * \file
 *
 * \brief File describing and initializing transceiver shield hardware
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

#include <sxlib/stm32_ll_base.h>
#include <sxlib/Gpio/Led/McuHal_Led_impl.h>
#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>
#include <sxlib/Gpio/Input/McuHal_Input_impl.h>
#include <sxlib/Gpio/Output/McuHal_Output_impl.h>
#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio_impl.h>
#include <sxlib/System/irq_line/McuHal_irq_line_impl.h>

#include "smtc_board.h"
#include "smtc_shield.h"
#include "smtc_stm32cube_arduino_compat.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

void smtc_shield_display_init( void );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

const struct sxlib_Gpio_Led_dev global_gpio_led_rx = {
    .port  = SMTC_ARDUINO_PORT_A5,
    .pin   = SMTC_ARDUINO_PIN_A5,
    .flags = SXLIB_GPIO_LED_DEV_FLAGS_ACTIVE_HIGH,
};

const struct sxlib_Gpio_Led_dev global_gpio_led_tx = {
    .port  = SMTC_ARDUINO_PORT_A4,
    .pin   = SMTC_ARDUINO_PIN_A4,
    .flags = SXLIB_GPIO_LED_DEV_FLAGS_ACTIVE_HIGH,
};

const struct sxlib_Gpio_Output_inst global_gpio_output_nreset    = { .port = SMTC_ARDUINO_PORT_A0,
                                                                  .pin  = SMTC_ARDUINO_PIN_A0 };
const struct sxlib_Gpio_Input_inst  global_gpio_input_radio_busy = { .port = SMTC_ARDUINO_PORT_D3,
                                                                    .pin  = SMTC_ARDUINO_PIN_D3,
                                                                    .pull = LL_GPIO_PULL_NO };

// Unused display module NSS lines that must be kept high
const struct sxlib_Gpio_Output_inst global_gpio_output_flash_ss = { .port = SMTC_ARDUINO_PORT_D6,
                                                                    .pin  = SMTC_ARDUINO_PIN_D6 };

const sxlib_System_irq_line_config_t global_irq_line_radio_interrupt = {
    .id          = 1,
    .exti_source = SMTC_ARDUINO_LL_SYSCFG_EXTI_LINE_D5,
    .trigger     = LL_EXTI_TRIGGER_RISING,
    .port        = SMTC_ARDUINO_PORT_D5,
    .IRQn        = SMTC_ARDUINO_EXTI_IRQn_D5,
    .pull        = LL_GPIO_PULL_DOWN,
    .pin         = SMTC_ARDUINO_PIN_D5,
};

const sxlib_Comm_spi_device_config_t global_spi_device_config = {
    .ss_port = SMTC_ARDUINO_PORT_D7,
    .ss_pin  = SMTC_ARDUINO_PIN_D7,
    .flags   = LL_SPI_MODE_MASTER | LL_SPI_BAUDRATEPRESCALER_DIV16,
};

const sxlib_Comm_spi_device_t global_spi_device = {
    .cont   = &global_spi_controller,
    .config = &global_spi_device_config,
};

// If radio driver manages 'sleeping' flag, then this structure cannot be const
#ifndef CONFIG_SMTC_AUTO_WAKE_RADIO
const
#endif
    sxlib_Radio_plain_spi_radio_t global_radio = {
        .spi            = &global_spi_device,
        .busy           = &global_gpio_input_radio_busy,
        .reset          = &global_gpio_output_nreset,
        .antenna_switch = 0,
    };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void smtc_shield_init( void )
{
    // Unused display module NSS lines that must be kept high
    sxlib_Gpio_Output_init( &global_gpio_output_flash_ss, SXLIB_GPIO_OUTPUT_HIGH );

    sxlib_Gpio_Led_init( &global_gpio_led_rx );
    sxlib_Gpio_Led_init( &global_gpio_led_tx );
    sxlib_Gpio_Led_init( &global_gpio_led_debug );

    sxlib_Gpio_Output_init( &global_gpio_output_nreset, SXLIB_GPIO_OUTPUT_LOW );
    sxlib_Gpio_Input_init( &global_gpio_input_radio_busy );

    sxlib_System_irq_line_init( &global_irq_line_radio_interrupt );

    sxlib_Comm_spi_device_init( &global_spi_device );

#ifdef CONFIG_SMTC_DEMO_ENABLE_DISPLAY
    smtc_shield_display_init( );
#endif
}

/* --- EOF ------------------------------------------------------------------ */
