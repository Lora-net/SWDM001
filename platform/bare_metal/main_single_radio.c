/**
 * \file
 *
 * \brief Code initializing a single non-RAL radio and launching a demo app
 *
 * \todo Combine main_single_radio.c and main_single_ral_radio.c by adapting RAL/RALF so that they don't need special
 * treatment: https://eugit1.semtech.com/LoRa-node/drivers-new-arch/smtc_ral/-/issues/154
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

#include <sxlib/System/basic_events/Generic_basic_events.h>
#include <sxlib/Radio/event_handler/Rtos_model_event_handler.h>
#include <sxlib/Timing/SystemTime/Rtos_SystemTime.h>
#include <sxlib/Radio/Generic_identification.h>
#include <sxlib/demo/demo.h>

#include "prepare_interface.h"
#include "sx_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

sx_comp_t                                 sx_comp;
sxlib_Radio_event_handler_manager_state_t handler_manager_state;
sxlib_generic_interface_t                 interface;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void main_( const void* context, sxlib_System_basic_events_mask_t radio_event_mask )
{
    main_single_interface_init( &interface );
    sxlib_Timing_SystemTime_init( );

    sxlib_Radio_event_handler_manager_init( context, &handler_manager_state, radio_event_mask );
    sxlib_Radio_event_handler_manager_register( &handler_manager_state, context );

    sxlib_Radio_identify_radio( &sx_comp, context );

    sx_comp_radio_reset_and_init( context );

    sxlib_demo_launcher( &interface, context );
}

/* --- EOF ------------------------------------------------------------------ */
