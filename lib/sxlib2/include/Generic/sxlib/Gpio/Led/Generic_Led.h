/**
 * \file
 *
 * \brief Generic interface header for LED module
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_GPIO_LED_GENERIC_LED_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_GPIO_LED_GENERIC_LED_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** LED device instance */
struct sxlib_Gpio_Led_dev;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS ------------------------------------------------------------
 */

#ifdef SXLIB_GPIO_LED_DISABLE
#define SXLIB_GPIO_LED_ON( led ) \
    do                           \
    {                            \
    } while( 0 )
#define SXLIB_GPIO_LED_OFF( led ) \
    do                            \
    {                             \
    } while( 0 )
#define SXLIB_GPIO_LED_TOGGLE( led ) \
    do                               \
    {                                \
    } while( 0 )
#else
#define SXLIB_GPIO_LED_ON( led ) sxlib_Gpio_Led_on( led )
#define SXLIB_GPIO_LED_OFF( led ) sxlib_Gpio_Led_off( led )
#define SXLIB_GPIO_LED_TOGGLE( led ) sxlib_Gpio_Led_toggle( led )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Turn LED on
 *
 * @param dev LED device instance
 */
void sxlib_Gpio_Led_on( const struct sxlib_Gpio_Led_dev* dev );

/**
 * @brief Turn LED off
 *
 * @param dev LED device instance
 */
void sxlib_Gpio_Led_off( const struct sxlib_Gpio_Led_dev* dev );

/**
 * @brief Toggle LED state
 *
 * @param dev LED device instance
 */
void sxlib_Gpio_Led_toggle( const struct sxlib_Gpio_Led_dev* dev );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_GPIO_LED_GENERIC_LED_H_

/* --- EOF ------------------------------------------------------------------ */
