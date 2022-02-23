/**
 * \file
 *
 * \brief Board clock initialization function definitions
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

#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_lptim.h"
#include <sxlib/Timing/lptim/McuHal_lptim.h>
#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void system_clock_init( void )
{
    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SYSCFG );
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

    // Latency=2 for 48MHz clock
    LL_FLASH_SetLatency( LL_FLASH_LATENCY_2 );

    if( LL_FLASH_GetLatency( ) != LL_FLASH_LATENCY_2 )
    {
        while( 1 )
        {
        }
    }

    LL_PWR_SetRegulVoltageScaling( LL_PWR_REGU_VOLTAGE_SCALE1 );

    LL_RCC_MSI_Enable( );
    while( LL_RCC_MSI_IsReady( ) != 1 )
    {
    }

    LL_RCC_PLL_Disable( );
    LL_RCC_PLL_DisableDomain_SYS( );

    LL_RCC_SetAHBPrescaler( LL_RCC_SYSCLK_DIV_1 );
    LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_MSI );
    while( LL_RCC_GetSysClkSource( ) != LL_RCC_SYS_CLKSOURCE_STATUS_MSI )
    {
    };

    LL_RCC_SetAPB1Prescaler( LL_RCC_APB1_DIV_1 );
    LL_RCC_SetAPB2Prescaler( LL_RCC_APB2_DIV_1 );

    LL_RCC_MSI_EnableRangeSelection( );
    LL_RCC_MSI_SetRange( LL_RCC_MSIRANGE_11 );

    LL_InitTick(48000000, CONFIG_SYS_CLOCK_TICKS_PER_SEC);
    LL_SetSystemCoreClock( 48000000 );

#ifdef CONFIG_SMTC_ENABLE_TICKLESS_MODE
    sxlib_Timing_lptim_init( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 );
#endif

    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );
    LL_RCC_SetClkAfterWakeFromStop( LL_RCC_STOP_WAKEUPCLOCK_MSI );

    LL_RCC_SetUSARTClockSource( LL_RCC_USART2_CLKSOURCE_PCLK1 );

    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_RNG );
    LL_RCC_SetRNGClockSource( LL_RCC_RNG_CLKSOURCE_MSI );
}

/* --- EOF ------------------------------------------------------------------ */
