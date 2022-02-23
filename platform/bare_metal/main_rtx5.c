/**
 * \file
 *
 * \brief Startup file for synchronous code, based on RTX5
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

#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"

#include <sxlib/stm32_ll_base.h>
#include <sxlib/System/basic_events/Generic_basic_events.h>

#include "smtc_shield.h"
#include "smtc_board.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Code that initializes the radio and demo.
 *
 * @param context Transceiver context
 * @param radio_event_mask event mask dedicated to the transceiver
 */
void main_( const void* context, sxlib_System_basic_events_mask_t radio_event_mask );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

osThreadId_t  osThreadId_main;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

/** Main thread entry point */
__NO_RETURN static void app_main( void* argument )
{
    ( void ) argument;

    main_( &global_radio, SXLIB_DEFAULT_EVENT_MASK_RADIO0 );

    while( true )
    {
    }
}

int main( void )
{
#ifdef USE_HAL_DRIVER
    HAL_Init( );
#endif

    system_clock_init( );
    LL_SYSTICK_EnableIT( );

    smtc_board_init( );

    SystemCoreClockUpdate( );

    smtc_shield_init( );

    osKernelInitialize( );
    osThreadId_main = osThreadNew( app_main, NULL, NULL );
    osKernelStart( );

    for( ;; )
    {
    }
}

/* --- EOF ------------------------------------------------------------------ */
