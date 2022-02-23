/**
 * \file
 *
 * \brief McuHal-specific implementation header for serial output devices
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

#ifndef SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SERIAL_MCUHAL_SERIAL_IMPL_H_
#define SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SERIAL_MCUHAL_SERIAL_IMPL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Comm/serial/McuHal_serial.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#if defined SXTOOLS_McuFamily_STM32L0xx
#define SXLIB_STM32_LPUART ( 0 )
#elif defined SXTOOLS_McuFamily_STM32L4xx
#define SXLIB_STM32_LPUART ( 0 )
#elif defined SXTOOLS_McuFamily_STM32WBxx
#define SXLIB_STM32_LPUART ( 0 )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * @brief Serial device configuration data
 */
typedef struct sxlib_Comm_serial_config_s
{
#if SXLIB_STM32_LPUART
    LL_LPUART_InitTypeDef init; /*!< STM32Cube LPUART initialization instance */
#else
    LL_USART_InitTypeDef init; /*!< STM32Cube USART initialization instance */
#endif

    USART_TypeDef* uart_dev; /*!< STM32Cube USART instance */
    GPIO_TypeDef*  tx_port;  /*!< STM32Cube HAL port for TX line */
    GPIO_TypeDef*  rx_port;  /*!< STM32Cube HAL port for RX line */

    uint8_t tx_alternate; /*!< STM32Cube alternate function for TX line */
    uint8_t rx_alternate; /*!< STM32Cube alternate function for RX line */

    uint8_t tx_pin; /*!< pin number for TX line */
    uint8_t rx_pin; /*!< pin number for RX line */
} sxlib_Comm_serial_config_t;

/**
 * @brief Serial device instance
 */
typedef struct sxlib_Comm_serial_s
{
    const sxlib_Comm_serial_config_t* config; /*!< Serial device configuration data */
} sxlib_Comm_serial_t;

#endif  // SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SERIAL_MCUHAL_SERIAL_IMPL_H_

/* --- EOF ------------------------------------------------------------------ */
