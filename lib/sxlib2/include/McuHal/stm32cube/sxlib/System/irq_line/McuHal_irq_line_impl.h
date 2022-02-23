/**
 * \file
 *
 * \brief McuHal-specific implementation header for irq_line module
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
#ifndef SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_SYSTEM_IRQ_LINE_MCUHAL_IRQ_LINE_IMPL_H_
#define SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_SYSTEM_IRQ_LINE_MCUHAL_IRQ_LINE_IMPL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/System/irq_line/McuHal_irq_line.h>

#include <sxlib/stm32_ll_base.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** IRQ line configuration data */
typedef struct sxlib_System_irq_line_config_s
{
    GPIO_TypeDef* port;        /*!< GPIO port */
    uint32_t      exti_source; /*!< EXTI interrupt source */
    uint8_t       trigger;     /*!< LL_EXTI_TRIGGER_RISING or other STM32Cube alternatives */
    uint8_t       id;          /*!< Depending on implementation, may be used to distinguish different irq lines */
    IRQn_Type     IRQn;        /*!< STM32Cube LL Physical IRQ line identifier */
    uint8_t       pull;        /*!< LL_GPIO_PULL_DOWN or other STM32Cube pull alternatives */
    uint8_t       pin;         /*!< Pin number */
} sxlib_System_irq_line_config_t;

#endif  // SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_SYSTEM_IRQ_LINE_MCUHAL_IRQ_LINE_IMPL_H_

/* --- EOF ------------------------------------------------------------------ */
