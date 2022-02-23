/**
 * \file
 *
 * \brief basic event handler interface header
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

#include "sx126x.h"
#include "sx126x_comp.h"
#include "sx126x_comp_hal.h"

#include "smtc_shield.h"

#include <sxlib/Gpio/Output/Generic_Output.h>
#include <sxlib/Radio/plain_spi_radio/Generic_plain_spi_radio_impl.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

sx126x_hal_status_t sx126x_comp_hal_antenna_switch_request( const void* context, const sx_comp_radio_data_t* radio_data,
                                                            sx_comp_antenna_switch_request_t request )
{
    const sxlib_Radio_plain_spi_radio_t* radio = ( const sxlib_Radio_plain_spi_radio_t* ) context;

    switch( request )
    {
    case SX_COMP_ANTENNA_SWITCH_CONFIGURE_RADIO:
        sx126x_set_dio2_as_rf_sw_ctrl( context, true );
        break;
    case SX_COMP_ANTENNA_SWITCH_TX0:
        sxlib_Gpio_Output_set( radio->antenna_switch, SXLIB_GPIO_OUTPUT_HIGH );
        break;
    case SX_COMP_ANTENNA_SWITCH_RX0:
        sxlib_Gpio_Output_set( radio->antenna_switch, SXLIB_GPIO_OUTPUT_HIGH );
        break;
    case SX_COMP_ANTENNA_SWITCH_STANDBY:
        break;
    case SX_COMP_ANTENNA_SWITCH_SLEEP:
        sxlib_Gpio_Output_set( radio->antenna_switch, SXLIB_GPIO_OUTPUT_LOW );
        break;
    }
    return SX126X_HAL_STATUS_OK;
}

/* --- EOF ------------------------------------------------------------------ */
