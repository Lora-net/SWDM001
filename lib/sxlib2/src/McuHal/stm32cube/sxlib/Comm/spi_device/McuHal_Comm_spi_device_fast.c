/**
 * \file
 *
 * \brief Main McuHal-specific implementation for SPI communication
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
#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ---------------------------------------------
 */

void sxlib_Comm_spi_device_fast_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length );
void sxlib_Comm_spi_device_fast_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void deinit_spi_gpios( sxlib_Comm_spi_controller_t* cont )
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Mode  = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;

    /* SPI MISO GPIO pin configuration  */
    if( cont->config->miso_port )
    {
        GPIO_InitStruct.Pin = ( 1 << cont->config->miso_pin );
        LL_GPIO_Init( cont->config->miso_port, &GPIO_InitStruct );
    }

    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;

    /* SPI SCK GPIO pin configuration  */
    if( cont->config->sck_port )
    {
        GPIO_InitStruct.Pin = ( 1 << cont->config->sck_pin );
        LL_GPIO_Init( cont->config->sck_port, &GPIO_InitStruct );
    }

    /* SPI MOSI GPIO pin configuration  */
    if( cont->config->mosi_port )
    {
        GPIO_InitStruct.Pin = ( 1 << cont->config->mosi_pin );
        LL_GPIO_Init( cont->config->mosi_port, &GPIO_InitStruct );
    }
}

void sxlib_Comm_spi_controller_init( sxlib_Comm_spi_controller_t*              cont,
                                     const sxlib_Comm_spi_controller_config_t* config )
{
    cont->last_device = 0;

    if( config != 0 )
    {
        cont->config = config;
    }

    if( cont->config->flags & SXLIB_COMM_SPI_CONTROLLER_CONFIG_FLAGS_ENABLE_IRQ )
    {
        NVIC_EnableIRQ( cont->config->spi_IRQn );
    }

    LL_SPI_InitTypeDef  SPI_InitStruct  = { 0 };
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SPI1 );
    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOA );

    /** SPI1 GPIO Configuration
    PA5   ------> SPI1_SCK
    PA6   ------> SPI1_MISO
    PA7   ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate  = LL_GPIO_AF_5;
    LL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV4;
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly           = 7;
    LL_SPI_Init( cont->config->controller_device, &SPI_InitStruct );

    LL_SPI_SetStandard( cont->config->controller_device, LL_SPI_PROTOCOL_MOTOROLA );
    LL_SPI_DisableNSSPulseMgt( cont->config->controller_device );

    LL_SPI_Enable( cont->config->controller_device );
    while( LL_SPI_IsEnabled( cont->config->controller_device ) == 0 )
    {
    };

    LL_SPI_SetRxFIFOThreshold( cont->config->controller_device, LL_SPI_RX_FIFO_TH_QUARTER );
}

void sxlib_Comm_spi_controller_deinit( sxlib_Comm_spi_controller_t* cont )
{
    LL_SPI_Disable( cont->config->controller_device );
    LL_APB2_GRP1_DisableClock( LL_APB2_GRP1_PERIPH_SPI1 );
    deinit_spi_gpios( cont );
}

void sxlib_Comm_spi_controller_suspend( sxlib_Comm_spi_controller_t* cont )
{
    LL_SPI_Disable( cont->config->controller_device );

    LL_GPIO_SetPinPull( cont->config->miso_port, 1 << cont->config->miso_pin, LL_GPIO_PULL_DOWN );
    LL_GPIO_SetPinPull( cont->config->sck_port, 1 << cont->config->sck_pin, LL_GPIO_PULL_DOWN );
    LL_GPIO_SetPinPull( cont->config->mosi_port, 1 << cont->config->mosi_pin, LL_GPIO_PULL_DOWN );

    LL_GPIO_SetPinMode( cont->config->miso_port, 1 << cont->config->miso_pin, LL_GPIO_MODE_INPUT );
    LL_GPIO_SetPinMode( cont->config->sck_port, 1 << cont->config->sck_pin, LL_GPIO_MODE_INPUT );
    LL_GPIO_SetPinMode( cont->config->mosi_port, 1 << cont->config->mosi_pin, LL_GPIO_MODE_INPUT );
}

void sxlib_Comm_spi_controller_resume( sxlib_Comm_spi_controller_t* cont )
{
    LL_GPIO_SetPinPull( cont->config->miso_port, 1 << cont->config->miso_pin, LL_GPIO_PULL_NO );
    LL_GPIO_SetPinPull( cont->config->sck_port, 1 << cont->config->sck_pin, LL_GPIO_PULL_NO );
    LL_GPIO_SetPinPull( cont->config->mosi_port, 1 << cont->config->mosi_pin, LL_GPIO_PULL_NO );

    LL_GPIO_SetPinMode( cont->config->miso_port, 1 << cont->config->miso_pin, LL_GPIO_MODE_ALTERNATE );
    LL_GPIO_SetPinMode( cont->config->sck_port, 1 << cont->config->sck_pin, LL_GPIO_MODE_ALTERNATE );
    LL_GPIO_SetPinMode( cont->config->mosi_port, 1 << cont->config->mosi_pin, LL_GPIO_MODE_ALTERNATE );

    LL_SPI_Enable( cont->config->controller_device );
    while( LL_SPI_IsEnabled( cont->config->controller_device ) == 0 )
    {
    };

    LL_SPI_SetRxFIFOThreshold( cont->config->controller_device, LL_SPI_RX_FIFO_TH_QUARTER );
}

void sxlib_Comm_spi_device_init( const sxlib_Comm_spi_device_t* device )
{
    LL_GPIO_InitTypeDef init = {
        .Pin        = ( 1 << device->config->ss_pin ),
        .Mode       = LL_GPIO_MODE_OUTPUT,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull       = LL_GPIO_PULL_NO,
        .Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH,
    };

    sxlib_Comm_spi_device_nCS( device );
    LL_GPIO_Init( device->config->ss_port, &init );
}

sxlib_status_t sxlib_Comm_spi_device_CS( const sxlib_Comm_spi_device_t* dev )
{
    if( dev->config->flags & SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH )
    {
        LL_GPIO_SetOutputPin( dev->config->ss_port, 1 << dev->config->ss_pin );
    }
    else
    {
        LL_GPIO_ResetOutputPin( dev->config->ss_port, 1 << dev->config->ss_pin );
    }

    return SXLIB_STATUS_OK;
}

void sxlib_Comm_spi_device_nCS( const sxlib_Comm_spi_device_t* dev )
{
    if( dev->config->flags & SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH )
    {
        LL_GPIO_ResetOutputPin( dev->config->ss_port, 1 << dev->config->ss_pin );
    }
    else
    {
        LL_GPIO_SetOutputPin( dev->config->ss_port, 1 << dev->config->ss_pin );
    }
}

static void sxlib_Comm_spi_configure( const sxlib_Comm_spi_device_t* dev )
{
    if( dev->cont->last_device != dev )
    {
        // Device speed, phase, and polarity are cached here
        uint32_t cr1 = ( ( uint32_t ) dev->config->flags & ~SXLIB_COMM_SPI_DEVICE_CONFIG_FLAGS_CS_ACTIVE_HIGH_Msk ) |
                       LL_SPI_FULL_DUPLEX | LL_SPI_NSS_SOFT | LL_SPI_CRCCALCULATION_DISABLE;
        WRITE_REG( dev->cont->config->controller_device->CR1, cr1 );
        dev->cont->last_device = dev;
    }
}

static inline void sxlib_Comm_spi_device_enable( const sxlib_Comm_spi_device_t* dev )
{
    uint16_t cr1 = READ_REG( dev->cont->config->controller_device->CR1 );
    WRITE_REG( dev->cont->config->controller_device->CR1, cr1 | SPI_CR1_SPE );
}

void sxlib_Comm_spi_device_read( const sxlib_Comm_spi_device_t* dev, uint8_t* data, unsigned int len )
{
    if( len > 0 )
    {
        sxlib_Comm_spi_configure( dev );

        sxlib_Comm_spi_device_enable( dev );
        sxlib_Comm_spi_device_fast_write_then_read( dev, 0, 0, data, len );
    }
}

void sxlib_Comm_spi_device_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* data, unsigned int len )
{
    if( len > 0 )
    {
        sxlib_Comm_spi_configure( dev );

        sxlib_Comm_spi_device_enable( dev );
        sxlib_Comm_spi_device_fast_write_then_write( dev, data, len, 0, 0 );
    }
}

void sxlib_Comm_spi_device_readwrite( const sxlib_Comm_spi_device_t* dev, uint8_t* data, unsigned int len )
{
    sxlib_assert( 0 );
}

void sxlib_Comm_spi_device_write_then_write( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                             const uint16_t command_length, const uint8_t* data,
                                             const uint16_t data_length )
{
    sxlib_Comm_spi_configure( dev );

    sxlib_Comm_spi_device_enable( dev );
    sxlib_Comm_spi_device_fast_write_then_write( dev, command, command_length, data, data_length );
}

void sxlib_Comm_spi_device_write_then_read( const sxlib_Comm_spi_device_t* dev, const uint8_t* command,
                                            const uint16_t command_length, uint8_t* data, const uint16_t data_length )
{
    sxlib_Comm_spi_configure( dev );

    sxlib_Comm_spi_device_enable( dev );
    sxlib_Comm_spi_device_fast_write_then_read( dev, command, command_length, data, data_length );
}

/* --- EOF ------------------------------------------------------------------ */
