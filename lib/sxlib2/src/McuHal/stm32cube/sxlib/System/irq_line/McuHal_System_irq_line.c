/**
 * \file
 *
 * \brief McuHal-specific implementation for irq_line module
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

#include <sxlib/System/irq_line/McuHal_irq_line_impl.h>

#include <sxlib/stm32_ll_base.h>

#if defined SXTOOLS_McuFamily_STM32L0xx
#include "stm32l0xx_hal_gpio_ex.h"
#elif defined SXTOOLS_McuFamily_STM32L4xx
#include "stm32l4xx_hal_gpio_ex.h"
#elif defined SXTOOLS_McuFamily_STM32WBxx
#include "stm32wbxx_hal_gpio_ex.h"
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_System_irq_line_init( const sxlib_System_irq_line_config_t* irq_line )
{
    uint32_t temp;

    LL_GPIO_InitTypeDef init = {
        .Pin  = ( 1 << irq_line->pin ),
        .Mode = LL_GPIO_MODE_INPUT,
        .Pull = irq_line->pull,
    };

    LL_GPIO_Init( irq_line->port, &init );

    uint8_t pin_group         = irq_line->pin >> 2;
    uint8_t position_in_group = ( irq_line->pin & 0x03 ) << 2;
    SYSCFG->EXTICR[pin_group] = SYSCFG->EXTICR[pin_group] & ~( 0x0FUL << position_in_group ) |
                                ( GPIO_GET_INDEX( irq_line->port ) << position_in_group );

    LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };

    EXTI_InitStruct.Line_0_31 = 1 << irq_line->pin;
#ifndef SXTOOLS_McuFamily_STM32L0xx
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
#endif
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode        = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger     = irq_line->trigger;

    LL_EXTI_Init( &EXTI_InitStruct );

    NVIC_EnableIRQ( irq_line->IRQn );
}

unsigned int sxlib_System_irq_line_read_gpio_state( const sxlib_System_irq_line_config_t* irq_line )
{
    return LL_GPIO_IsInputPinSet( irq_line->port, 1 << irq_line->pin );
}

/* --- EOF ------------------------------------------------------------------ */
