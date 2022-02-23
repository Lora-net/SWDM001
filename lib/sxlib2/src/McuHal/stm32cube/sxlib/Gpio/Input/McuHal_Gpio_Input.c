/**
 * \file
 *
 * \brief McuHal-specific GPIO input implementation
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

#include <sxlib/Gpio/Input/McuHal_Input_impl.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_Gpio_Input_init( const struct sxlib_Gpio_Input_inst* inst )
{
    LL_GPIO_InitTypeDef init = {
        .Pin  = ( 1 << inst->pin ),
        .Mode = LL_GPIO_MODE_INPUT,
        .Pull = inst->pull,
    };

    LL_GPIO_Init( inst->port, &init );
}

void sxlib_Gpio_Input_deinit( const struct sxlib_Gpio_Input_inst* inst )
{
    LL_GPIO_InitTypeDef init = {
        .Pin  = ( 1 << inst->pin ),
        .Mode = LL_GPIO_MODE_ANALOG,
        .Pull = LL_GPIO_PULL_NO,
    };

    LL_GPIO_Init( inst->port, &init );
}

sxlib_Gpio_Input_state sxlib_Gpio_Input_get( const struct sxlib_Gpio_Input_inst* inst )
{
    return LL_GPIO_IsInputPinSet( inst->port, 1 << inst->pin );
}

/* --- EOF ------------------------------------------------------------------ */
