/**
 * \file
 *
 * \brief McuHal-specific sleep management implementation
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

#include <sxlib/System/sleep_management/Generic_sleep_management.h>
#include <sxlib/System/sleep_inhibition/Generic_sleep_inhibition.h>

#include <sxlib/stm32_ll_base.h>

#include <smtc_board.h>

// For LED debugging
#include <sxlib/Gpio/Led/Generic_Led.h>
#include <smtc_shield.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_System_sleep( void )
{
    sxlib_System_sleep_inhibition_class_t first_inhibited_class =
        sxlib_System_sleep_inhibition_get_first_inhibited_class( );

    // sxlib_Gpio_Led_on( &global_gpio_led_debug );

    switch( first_inhibited_class )
    {
    // Stop 1 is inhibited, enter sleep.
    case SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_0:
        LL_SYSTICK_DisableIT( );
        LL_LPM_EnableSleep( );
#if defined( __CC_ARM )
        __force_stores( );
#endif
        __WFI( );
        LL_SYSTICK_EnableIT( );
        break;

    // Stop 2 is inhibited, enter stop 1.
    case SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_1:
        LL_SYSTICK_DisableIT( );
        sxlib_System_pre_sleep( );
        LL_PWR_SetPowerMode( LL_PWR_MODE_STOP1 );
        LL_LPM_EnableDeepSleep( );
#if defined( __CC_ARM )
        __force_stores( );
#endif
        __WFI( );
        sxlib_System_post_wake( );
        LL_SYSTICK_EnableIT( );
        break;

    // Nothing is inhibited, enter stop 2.
    case SXLIB_SYSTEM_SLEEP_INHIBITION_SENTINEL:
        LL_SYSTICK_DisableIT( );
        sxlib_System_pre_sleep( );
        LL_PWR_SetPowerMode( LL_PWR_MODE_STOP2 );
        LL_LPM_EnableDeepSleep( );
#if defined( __CC_ARM )
        __force_stores( );
#endif
        __WFI( );
        sxlib_System_post_wake( );
        LL_SYSTICK_EnableIT( );
        break;

    default:
        break;
    }

    // sxlib_Gpio_Led_off( &global_gpio_led_debug );
}

/* --- EOF ------------------------------------------------------------------ */
