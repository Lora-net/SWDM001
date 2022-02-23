/**
 * \file
 *
 * \brief McuHal-specific implementation of low-power timer functionality
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

#include <stdint.h>
#include <stdbool.h>

#include <sxlib/Timing/lptim/McuHal_lptim.h>
#include <sxlib/Debug/Assert/Assert.h>
#include <sxlib/Timing/OneShotTimerBase/Generic_OneShotTimerBase.h>
#include <sxlib/stm32_ll_base.h>

// For LED debugging
#include <sxlib/Gpio/Led/Generic_Led.h>
#include <smtc_shield.h>

#include <smtc_board.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/** If BLOCKING_SETCOMPARE is set, timer startup will never fail, but it will take more time to complete */
// #define BLOCKING_SETCOMPARE

/** If USE_LSE is set, the low-speed external timer is used instead of the low-speed internal timer */
#define USE_LSE

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

uint16_t sxlib_Timing_lptim_get_hardware_time( void )
{
    uint16_t t_old;
    uint16_t t = LPTIM1->CNT;

    do
    {
        t_old = t;
        t     = LPTIM1->CNT;
    } while( t != t_old );

    return t;
}

bool sxlib_Timing_lptim_set_trigger_time( uint16_t deadline )
{
    uint16_t condition;

#ifdef BLOCKING_SETCOMPARE
    LL_LPTIM_ClearFlag_CMPOK( LPTIM1 );
    LL_LPTIM_SetCompare( LPTIM1, deadline );
    while( !LL_LPTIM_IsActiveFlag_CMPOK( LPTIM1 ) )
    {
    }
#else
    if( !LL_LPTIM_IsActiveFlag_CMPOK( LPTIM1 ) )
    {
        return false;
    }
    LL_LPTIM_ClearFlag_CMPOK( LPTIM1 );
    LL_LPTIM_SetCompare( LPTIM1, deadline );
#endif

    // LPTIM1->ICR = LPTIM_ISR_CMPM;
    NVIC_ClearPendingIRQ( LPTIM1_IRQn );
    condition = sxlib_Timing_lptim_get_hardware_time( ) - deadline;

    // We missed the deadline. Just set the interrupt and go on.
    if( condition <= 0x8000 )
    {
        LPTIM1->ISR = LPTIM_ISR_CMPM;
    }

    // LPTIM1->IER |= LPTIM_IER_CMPMIE;

    return true;
}

bool sxlib_Timing_lptim_set_trigger_interval( int32_t delta )
{
    // This is a 16-bit timer -- don't let it overflow.
    // Also, this takes care of ONE_SHOT_TIMER_WAIT_FOREVER.
    if( ( delta == ONE_SHOT_TIMER_WAIT_FOREVER ) || ( delta > 0xF000 ) )
    {
        delta = 0xF000;
    }
    else if( delta < 2 )
    {
        return false;
    }

    uint16_t hardware_time = sxlib_Timing_lptim_get_hardware_time( );
    return sxlib_Timing_lptim_set_trigger_time( hardware_time + delta );
}

void sxlib_Timing_lptim_set_clock_frequency_log2( unsigned int freq_exp2 )
{
    sxlib_assert( freq_exp2 <= 15 );
    sxlib_assert( freq_exp2 > 7 );
    LPTIM1->CFGR &= ~LPTIM_CFGR_PRESC_Msk;
    LPTIM1->CFGR |= ( ( 15 - freq_exp2 ) << LPTIM_CFGR_PRESC_Pos );
}

unsigned int sxlib_Timing_lptim_get_clock_frequency_log2( void )
{
    return 15 - ( ( LPTIM1->CFGR & LPTIM_CFGR_PRESC_Msk ) >> LPTIM_CFGR_PRESC_Pos );
}

void sxlib_Timing_lptim_init( unsigned int freq_exp2 )
{
#ifdef USE_LSE
    LL_RCC_ForceBackupDomainReset( );
    LL_RCC_ReleaseBackupDomainReset( );

    LL_PWR_EnableBkUpAccess( );

    LL_RCC_LSE_SetDriveCapability( LL_RCC_LSEDRIVE_MEDIUMHIGH );
    LL_RCC_LSE_Enable( );
    LL_PWR_DisableBkUpAccess( );

    while( LL_RCC_LSE_IsReady( ) != 1 )
    {
    }
    LL_RCC_SetLPTIMClockSource( LL_RCC_LPTIM1_CLKSOURCE_LSE );
#else
    LL_RCC_ForceBackupDomainReset( );
    LL_RCC_ReleaseBackupDomainReset( );
    LL_RCC_LSI_Enable( );

    while( LL_RCC_LSI_IsReady( ) != 1 )
    {
    }
    LL_RCC_SetLPTIMClockSource( LL_RCC_LPTIM1_CLKSOURCE_LSI );
#endif

    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_LPTIM1 );
    LL_LPTIM_SetClockSource( LPTIM1, LL_LPTIM_CLK_SOURCE_INTERNAL );

    sxlib_Timing_lptim_set_clock_frequency_log2( freq_exp2 );

    LL_LPTIM_Enable( LPTIM1 );
    LL_LPTIM_SetAutoReload( LPTIM1, 0xFFFF );
    LL_LPTIM_StartCounter( LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS );

    NVIC_EnableIRQ( LPTIM1_IRQn );
    LPTIM1->IER |= LPTIM_IER_CMPMIE;

#ifndef BLOCKING_SETCOMPARE
    // This causes CMPOK to be set, allowing CMP to be set in the future
    LL_LPTIM_SetCompare( LPTIM1, 0xFFFF );
#endif
}

/* --- EOF ------------------------------------------------------------------ */
