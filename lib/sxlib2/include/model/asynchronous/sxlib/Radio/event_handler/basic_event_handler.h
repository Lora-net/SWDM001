/**
 * \file
 *
 * \brief Implementation header for a simple generic asynchronous transceiver event handling system
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

#ifndef SXLIB_INCLUDE_MODEL_ASYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_BASIC_EVENT_HANDLER_H_
#define SXLIB_INCLUDE_MODEL_ASYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_BASIC_EVENT_HANDLER_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdatomic.h>

#include <sxlib/Radio/event_handler/model_event_handler.h>
#include <sxlib/System/basic_events/Generic_basic_events.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

typedef struct sxlib_Radio_event_handler_manager_state_s
{
    sxlib_Radio_event_handler_f      handler;
    void*                            arg;
    atomic_int_least32_t             timestamp;
    sxlib_System_basic_events_mask_t event_mask;
} sxlib_Radio_event_handler_manager_state_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/** For documentation of this private function, see \ref sxlib_Radio_basic_event_handler_handle */
void sxlib_Radio_basic_event_handler_handle_base( sxlib_Radio_event_handler_manager_state_t* state );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Initialize event handling subsystem
 *
 * @param context A previously registered generic radio-driver context
 * @param handler_manager_state The event handler manager state to initialize
 * @param event_mask A bitmask that may be used by the implementation to identify this particular event
 */
void sxlib_Radio_event_handler_manager_init( const void*                                context,
                                             sxlib_Radio_event_handler_manager_state_t* handler_manager_state,
                                             sxlib_System_basic_events_mask_t           event_mask );

/**
 * @brief Handle a transceiver event by calling the event handler callback
 *
 * @param context A previously registered generic radio-driver context
 */
static inline void sxlib_Radio_basic_event_handler_handle( const void* context )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_basic_event_handler_handle_base( handler_manager_state );
}

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_MODEL_ASYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_BASIC_EVENT_HANDLER_H_

/* --- EOF ------------------------------------------------------------------ */
