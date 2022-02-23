/**
 * \file
 *
 * \brief Generic implementation header for standard SPI radio
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_PLAIN_SPI_RADIO_GENERIC_PLAIN_SPI_RADIO_IMPL_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_PLAIN_SPI_RADIO_GENERIC_PLAIN_SPI_RADIO_IMPL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/System/config/Rtos_config.h>

#include <stdbool.h>

#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio.h>

#include <sxlib/Comm/spi_device/Generic_spi_device.h>
#include <sxlib/Gpio/Input/Generic_Input.h>
#include <sxlib/Gpio/Output/Generic_Output.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Generic type representing an SPI transceiver with busy and reset lines, and an RF switch */
typedef struct sxlib_Radio_plain_spi_radio_s
{
    const sxlib_Comm_spi_device_t*       spi;            /*!< SPI device */
    const struct sxlib_Gpio_Input_inst*  busy;           /*!< busy GPIO line */
    const struct sxlib_Gpio_Output_inst* reset;          /*!< reset GPIO line */
    const struct sxlib_Gpio_Output_inst* antenna_switch; /*!< RF switch GPIO line */
#ifdef CONFIG_SMTC_AUTO_WAKE_RADIO
    bool sleeping; /*!< If the driver is compiled to support auto-wake, this state data is necessary */
#endif
} sxlib_Radio_plain_spi_radio_t;

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_PLAIN_SPI_RADIO_GENERIC_PLAIN_SPI_RADIO_IMPL_H_

/* --- EOF ------------------------------------------------------------------ */
