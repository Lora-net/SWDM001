/**
 * \file
 *
 * \brief McuHal-specific implementation for LED module
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

#include <sxlib/Gpio/Led/McuHal_Led_impl.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ------------------------------------------------------------
 */

void sxlib_Gpio_Led_init( const struct sxlib_Gpio_Led_dev* dev )
{
    if( !dev ) return;

    LL_GPIO_InitTypeDef init = {
        .Pin   = ( 1 << dev->pin ),
        .Mode  = LL_GPIO_MODE_OUTPUT,
        .Pull  = LL_GPIO_PULL_NO,
        .Speed = LL_GPIO_SPEED_FREQ_LOW,
    };

    sxlib_Gpio_Led_off( dev );
    LL_GPIO_Init( dev->port, &init );
}

void sxlib_Gpio_Led_on( const struct sxlib_Gpio_Led_dev* dev )
{
    if( !dev ) return;

    if( dev->flags & SXLIB_GPIO_LED_DEV_FLAGS_ACTIVE_LOW )
    {
        LL_GPIO_ResetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
    else
    {
        LL_GPIO_SetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
}

void sxlib_Gpio_Led_off( const struct sxlib_Gpio_Led_dev* dev )
{
    if( !dev ) return;

    if( dev->flags & SXLIB_GPIO_LED_DEV_FLAGS_ACTIVE_LOW )
    {
        LL_GPIO_SetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
    else
    {
        LL_GPIO_ResetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
}

void sxlib_Gpio_Led_toggle( const struct sxlib_Gpio_Led_dev* dev )
{
    if( !dev ) return;

    if( LL_GPIO_IsOutputPinSet( dev->port, ( 1 << dev->pin ) ) )
    {
        LL_GPIO_ResetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
    else
    {
        LL_GPIO_SetOutputPin( dev->port, ( 1 << dev->pin ) );
    }
}

/* --- EOF ------------------------------------------------------------------ */
