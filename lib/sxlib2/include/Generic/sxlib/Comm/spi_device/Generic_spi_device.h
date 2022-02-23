/**
 * \file
 *
 * \brief Generic interface header for SPI communication
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_COMM_SPI_DEVICE_GENERIC_SPI_DEVICE_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_COMM_SPI_DEVICE_GENERIC_SPI_DEVICE_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Debug/Status/Generic_Status.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** SPI device instance */
typedef struct sxlib_Comm_spi_device_s sxlib_Comm_spi_device_t;

/** SPI controller instance */
typedef struct sxlib_Comm_spi_controller_s sxlib_Comm_spi_controller_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Assert the SPI device chip-select line
 *
 * @param dev SPI device instance
 *
 * @returns Operation status
 */
sxlib_status_t sxlib_Comm_spi_device_CS( const sxlib_Comm_spi_device_t* dev );

/**
 * @brief De-assert the SPI device chip-select line
 *
 * @param dev SPI device instance
 */
void sxlib_Comm_spi_device_nCS( const sxlib_Comm_spi_device_t* dev );

/**
 * @brief Read data from the SPI device
 *
 * @param dev SPI device instance
 * @param data Data buffer
 * @param len Amount of data, in bytes
 */
void sxlib_Comm_spi_device_read( const sxlib_Comm_spi_device_t* dev, uint8_t* data, unsigned int len );

/**
 * @brief Write data to the SPI device
 *
 * @param dev SPI device instance
 * @param data Data buffer
 * @param len Amount of data, in bytes
 */
void sxlib_Comm_spi_device_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* data, unsigned int len );

/**
 * @brief Perform a simultaneous read/write operation with the SPI device
 *
 * @remark This may not be implemented on all platforms
 *
 * @param dev SPI device instance
 * @param data Data buffer
 * @param len Amount of data, in bytes
 */
void sxlib_Comm_spi_device_readwrite( const sxlib_Comm_spi_device_t* dev, uint8_t* data, unsigned int len );

/**
 * @brief Perform a SPI write operation immediately followed by a read operation
 *
 * @param dev SPI device instance
 * @param command Data buffer for write operation
 * @param command_length Length of write operation buffer, in bytes
 * @param data Data buffer for read operation
 * @param data_length Length of read operation buffer, in bytes
 */
void sxlib_Comm_spi_device_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                            const uint16_t command_length, uint8_t* data, const uint16_t data_length );

/**
 * @brief Perform a SPI write operation immediately followed by another write operation
 *
 * @param dev SPI device instance
 * @param command Data buffer for first write operation
 * @param command_length Length of first write operation buffer, in bytes
 * @param data Data buffer for second write operation
 * @param data_length Length of second write operation buffer, in bytes
 */
void sxlib_Comm_spi_device_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                             const uint16_t command_length, const uint8_t* data,
                                             const uint16_t data_length );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_COMM_SPI_DEVICE_GENERIC_SPI_DEVICE_H_

/* --- EOF ------------------------------------------------------------------ */
