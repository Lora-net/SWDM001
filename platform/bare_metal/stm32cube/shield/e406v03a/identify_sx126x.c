/**
 * \file
 *
 * \brief Identify radio and initialize sx_comp
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

#include <sxlib/Radio/Generic_identification.h>

#include "sx126x_comp_samples.h"

#include <sxlib/stm32_ll_base.h>
#include <sxlib/Gpio/Input/McuHal_Input_impl.h>

#include "smtc_stm32cube_arduino_compat.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static const struct sxlib_Gpio_Input_inst gpio_input_matching_frequency_915_868 = { .port = SMTC_ARDUINO_PORT_A1,
                                                                                    .pin  = SMTC_ARDUINO_PIN_A1 };
static const struct sxlib_Gpio_Input_inst gpio_input_sx1262_sx1261              = { .port = SMTC_ARDUINO_PORT_A2,
                                                                       .pin  = SMTC_ARDUINO_PIN_A2 };
static const struct sxlib_Gpio_Input_inst gpio_input_tcxo_xtal                  = { .port = SMTC_ARDUINO_PORT_A3,
                                                                   .pin  = SMTC_ARDUINO_PIN_A3 };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Radio_identify_radio( sx_comp_t* sx_comp, const void* context )
{
    sxlib_Gpio_Input_init( &gpio_input_matching_frequency_915_868 );
    sxlib_Gpio_Input_init( &gpio_input_sx1262_sx1261 );
    sxlib_Gpio_Input_init( &gpio_input_tcxo_xtal );

    if( sxlib_Gpio_Input_get( &gpio_input_sx1262_sx1261 ) == SXLIB_GPIO_INPUT_HIGH )
    {
        if( sxlib_Gpio_Input_get( &gpio_input_tcxo_xtal ) == SXLIB_GPIO_INPUT_HIGH )
        {
            *sx_comp = sx_comp_sample_sx1261_xtal;
        }
        else
        {
            *sx_comp = sx_comp_sample_sx1261_tcxo;
        }
    }
    else
    {
        if( sxlib_Gpio_Input_get( &gpio_input_tcxo_xtal ) == SXLIB_GPIO_INPUT_HIGH )
        {
            *sx_comp = sx_comp_sample_sx1262_xtal;
        }
        else
        {
            *sx_comp = sx_comp_sample_sx1262_tcxo;
        }
    }

    if( sxlib_Gpio_Input_get( &gpio_input_matching_frequency_915_868 ) == SXLIB_GPIO_INPUT_HIGH )
    {
        sx_comp->radio_data.cal_img_freq1_in_mhz = 863;
        sx_comp->radio_data.cal_img_freq2_in_mhz = 870;
    }
    else
    {
        sx_comp->radio_data.cal_img_freq1_in_mhz = 902;
        sx_comp->radio_data.cal_img_freq2_in_mhz = 928;
    }

    sx_comp_register( sx_comp, context );

    sxlib_Gpio_Input_deinit( &gpio_input_matching_frequency_915_868 );
    sxlib_Gpio_Input_deinit( &gpio_input_sx1262_sx1261 );
    sxlib_Gpio_Input_deinit( &gpio_input_tcxo_xtal );
}

/* --- EOF ------------------------------------------------------------------ */
