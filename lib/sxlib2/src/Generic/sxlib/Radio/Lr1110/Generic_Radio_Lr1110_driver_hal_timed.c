/**
 * \file
 *
 * \brief LR1110 driver HAL implementation of timed operations
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

#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio_impl.h>
#include <sxlib/Comm/spi_device/McuHal_spi_device_impl.h>
#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>

#include "lr1110_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

lr1110_hal_status_t lr1110_hal_reset_plain( const void* context )
{
    const sxlib_Radio_plain_spi_radio_t* radio = ( const sxlib_Radio_plain_spi_radio_t* ) context;

    sxlib_Gpio_Output_set( radio->reset, SXLIB_GPIO_OUTPUT_LOW );
    sxlib_Timing_SystemTime_busywait_ticks( SXLIB_MBS_TO_TICK( 2 ) );
    sxlib_Gpio_Output_set( radio->reset, SXLIB_GPIO_OUTPUT_HIGH );

    return LR1110_HAL_STATUS_OK;
}

void lr1110_hal_wakeup_plain( const sxlib_Radio_plain_spi_radio_t* radio )
{
    sxlib_Comm_spi_device_CS( radio->spi );
    // Make sure it's down long enough (TODO: this should be timed with a microsecond timer)
    sxlib_Comm_spi_device_CS( radio->spi );
    sxlib_Comm_spi_device_nCS( radio->spi );
}

/* --- EOF ------------------------------------------------------------------ */
