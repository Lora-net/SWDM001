/**
 * \file
 *
 * \brief Startup file for asynchronous code, implementing an event loop
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

#include <stdbool.h>

#include <sxlib/Timing/async_timer/Rtos_async_timer_poll.h>
#include <sxlib/System/basic_events/Generic_basic_events.h>
#include <sxlib/Radio/event_handler/basic_event_handler.h>
#include <sxlib/Timing/lptim/McuHal_lptim.h>
#include <sxlib/System/sleep_management/Generic_sleep_management.h>
#include <sxlib/System/sleep_inhibition/Generic_sleep_inhibition.h>

#include "smtc_shield.h"
#include "smtc_board.h"

// For __disable_irq()
#include <sxlib/stm32_ll_base.h>

#include <sxlib/stm32_ll_base.h>
#include <sxlib/Comm/spi_device/McuHal_spi_device.h>
#include <smtc_board.h>
#include "main_various.h"

#include "stm32_seq.h"
#include "stm32_lpm.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void initialize_application( const void* context )
{
#if(1)
  smtc_board_init( );
  smtc_shield_init( );
  LL_RCC_MSI_EnablePLLMode( );
  sxlib_System_sleep_inhibition_inhibit_sleep( SXLIB_SLEEP_INHIBITION_MASK_MAIN,
                                               SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_0 );
  main_( context, SXLIB_DEFAULT_EVENT_MASK_RADIO0 );
#endif
}

#if( 1 )
void UTIL_SEQ_PreIdle( void )
{
    // When BLE is almost ready to sleep, run any timer callback that is ready
    sxlib_Timing_async_timer_poll( );
}

#if(0)
void UTIL_SEQ_PostIdle( void )
{
}
#endif

void UTIL_SEQ_Idle( void )
{
    /*
     * Interrupts are already disabled upon entry. Sxlib timer callbacks have been executed, and no BLE tasks are ready
     * to run. Interrupts will be re-enabled immediately after UTIL_SEQ_Idle() exits, so it is okay that the following
     * function re-enables interrupts. This is necessary in order to get some non-BLE work done without interrupts
     * disabled.
     */
    check_timers_and_events_then_sleep( );  // This re-enables interrupts
}
#endif

/* --- EOF ------------------------------------------------------------------ */
