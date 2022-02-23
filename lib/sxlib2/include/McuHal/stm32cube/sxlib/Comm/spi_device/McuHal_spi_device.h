/**
 * \file
 *
 * \brief McuHal-specific interface header for SPI communication
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
#ifndef SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_H_
#define SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/Comm/spi_device/Generic_spi_device.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** SPI controller instance */
typedef struct sxlib_Comm_spi_controller_s sxlib_Comm_spi_controller_t;

/** SPI controller configuration data */
typedef struct sxlib_Comm_spi_controller_config_s sxlib_Comm_spi_controller_config_t;

/** SPI device configuration data */
typedef struct sxlib_Comm_spi_device_config_s sxlib_Comm_spi_device_config_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize SPI controller
 *
 * @param cont SPI controller instance
 * @param config SPI controller configuration data
 */
void sxlib_Comm_spi_controller_init( sxlib_Comm_spi_controller_t*              cont,
                                     const sxlib_Comm_spi_controller_config_t* config );

/**
 * @brief De-initialize SPI controller
 *
 * @param cont SPI controller instance
 */
void sxlib_Comm_spi_controller_deinit( sxlib_Comm_spi_controller_t* cont );

/**
 * @brief Suspend SPI controller for entry to low-power mode
 *
 * @param cont SPI controller instance
 */
void sxlib_Comm_spi_controller_suspend( sxlib_Comm_spi_controller_t* cont );

/**
 * @brief Resume SPI controller after exit from low-power mode
 *
 * @param cont SPI controller instance
 */
void sxlib_Comm_spi_controller_resume( sxlib_Comm_spi_controller_t* cont );

/**
 * @brief Initialize SPI device
 *
 * @param device SPI device instance
 *
 * @remark The device instance must be fully defined before calling this function
 */
void sxlib_Comm_spi_device_init( const sxlib_Comm_spi_device_t* device );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_COMM_SPI_DEVICE_MCUHAL_SPI_DEVICE_H_

/* --- EOF ------------------------------------------------------------------ */
