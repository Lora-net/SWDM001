/**
 * \file
 *
 * \brief McuHal-specific implementation header for SPI communication
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
#ifndef SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_IMPL_H_
#define SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_IMPL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Comm/spi_device/McuHal_spi_device.h>
#include <sxlib/Comm/spi_device/Generic_spi_device.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define SXLIB_COMM_SPI_CONTROLLER_CONFIG_FLAGS_ENABLE_IRQ_Pos ( 0 )
#define SXLIB_COMM_SPI_CONTROLLER_CONFIG_FLAGS_ENABLE_IRQ \
    ( 0x01UL << SXLIB_COMM_SPI_CONTROLLER_CONFIG_FLAGS_ENABLE_IRQ_Pos )

// NOTE: The fields in comments are reserved by the STM32 HAL!!
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_PHASE_2EDGE_Pos (0)
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_POLARITY_HIGH_Pos (1)
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_MSTR_Pos (2)
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_RATE_Pos (3)
#define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH_Pos ( 6 )
#define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH_Msk \
    ( 0x01UL << SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH_Pos )
#define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH_Msk
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_LSBFIRST_Pos (7)
// #define SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_SSI_Pos (8)

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** SPI controller configuration data */
typedef struct sxlib_Comm_spi_controller_config_s
{
    SPI_TypeDef* controller_device; /*!< STM32Cube SPI controller instance */

    GPIO_TypeDef* sck_port;  /*!< STM32Cube HAL port for SCK line */
    GPIO_TypeDef* miso_port; /*!< STM32Cube HAL port for MISO line */
    GPIO_TypeDef* mosi_port; /*!< STM32Cube HAL port for MOSI line */

    IRQn_Type spi_IRQn; /*!< STM32Cube SPI interrupt number */

    uint8_t flags; /*!< Set to 0, or SXLIB_COMM_SPI_CONTROLLER_CONFIG_FLAGS_ENABLE_IRQ if SPI IRQ is needed */

    uint8_t sck_alternate;  /*!< STM32Cube alternate function for SCK line */
    uint8_t miso_alternate; /*!< STM32Cube alternate function for MISO line */
    uint8_t mosi_alternate; /*!< STM32Cube alternate function for MOSI line */

    uint8_t sck_pin;  /*!< pin number for SCK line */
    uint8_t miso_pin; /*!< pin number for MISO line */
    uint8_t mosi_pin; /*!< pin number for MOSI line */
} sxlib_Comm_spi_controller_config_t;

/** SPI controller instance */
typedef struct sxlib_Comm_spi_controller_s
{
    const sxlib_Comm_spi_controller_config_t* config;      /*!< SPI controller configuration data */
    const sxlib_Comm_spi_device_t*            last_device; /*!< Used for device configuration caching */
} sxlib_Comm_spi_controller_t;

/** SPI controller configuration data -- this can be const */
typedef struct sxlib_Comm_spi_device_config_s
{
    GPIO_TypeDef* ss_port; /*!< STM32Cube HAL port for SS line */
    uint16_t      flags;   /*!< Set to 0, or SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH */
    uint8_t       ss_pin;  /*!< pin number for SS line */
} sxlib_Comm_spi_device_config_t;

/** SPI device instance -- this can be const */
typedef struct sxlib_Comm_spi_device_s
{
    sxlib_Comm_spi_controller_t*          cont;   /*!< SPI controller instance */
    const sxlib_Comm_spi_device_config_t* config; /*!<  SPI controller configuration data */
} sxlib_Comm_spi_device_t;

#endif  // SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_IMPL_H_

/* --- EOF ------------------------------------------------------------------ */
