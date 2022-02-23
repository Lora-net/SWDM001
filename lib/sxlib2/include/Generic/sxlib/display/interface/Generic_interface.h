/**
 * \file
 *
 * \brief Generic header for LED/LCD interface API
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_INTERFACE_GENERIC_INTERFACE_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_INTERFACE_GENERIC_INTERFACE_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Gpio/Led/Generic_Led.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Points to RX and TX leds typically available on transceiver shields */
typedef struct sxlib_generic_led_interface_s
{
    const struct sxlib_Gpio_Led_dev* led_rx;
    const struct sxlib_Gpio_Led_dev* led_tx;
} sxlib_generic_led_interface_t;

/** Points to an opaque LCD display interface with 24-bit background and foreground color selection */
typedef struct sxlib_generic_display_interface_s
{
    const void* display;
    uint32_t    bg_rgb888;
    uint32_t    fg_rgb888;
} sxlib_generic_display_interface_t;

/** Combined LED and LCD display configuration */
typedef struct sxlib_generic_interface_s
{
    sxlib_generic_led_interface_t     led_interface;
    sxlib_generic_display_interface_t display_interface;
} sxlib_generic_interface_t;

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_INTERFACE_GENERIC_INTERFACE_H_

/* --- EOF ------------------------------------------------------------------ */
