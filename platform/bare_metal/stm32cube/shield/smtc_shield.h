/**
 * \file
 *
 * \brief Generic shield declarations
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

#ifndef SHIELD_E406V03A_H_
#define SHIELD_E406V03A_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/System/config/Rtos_config.h>

#include <stdint.h>
#include <stdbool.h>

#include <sxlib/Gpio/Led/Generic_Led.h>
#include <sxlib/Comm/spi_device/Generic_spi_device.h>
#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio.h>
#include <sxlib/display/dmtft/Generic_display_dmtft.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

extern const struct sxlib_Gpio_Led_dev     global_gpio_led_rx;
extern const struct sxlib_Gpio_Led_dev     global_gpio_led_tx;
extern const sxlib_Comm_spi_device_t       global_spi_device;
extern const struct sxlib_Gpio_Output_inst global_gpio_output_antenna_switch;
extern const sxlib_display_dmtft_t         global_display;

// If radio driver manages 'sleeping' flag, then this structure cannot be const
#ifndef CONFIG_SMTC_AUTO_WAKE_RADIO
const
#endif
    extern sxlib_Radio_plain_spi_radio_t global_radio;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

void smtc_shield_init( void );

#ifdef __cplusplus
}
#endif

#endif  // SHIELD_E406V03A_H_

/* --- EOF ------------------------------------------------------------------ */
