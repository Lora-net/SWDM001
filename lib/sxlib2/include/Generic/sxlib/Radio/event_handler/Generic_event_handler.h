/**
 * \file
 *
 * \brief Generic transceiver event handling subsystem header for synchronous and asynchronous models
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_EVENT_HANDLER_GENERIC_EVENT_HANDLER_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_EVENT_HANDLER_GENERIC_EVENT_HANDLER_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Debug/Status/Generic_Status.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * Opaque structure holding data necessary for radio event handling.
 * This can be defined for synchronous or asynchronous event handling models.
 */
typedef struct sxlib_Radio_event_handler_manager_state_s sxlib_Radio_event_handler_manager_state_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/** For documentation of this private function, see \ref sxlib_Radio_event_handler_trigger */
void sxlib_Radio_event_handler_trigger_base( sxlib_Radio_event_handler_manager_state_t* state );

/** For documentation of this private function, see \ref sxlib_Radio_event_handler_trigger_with_timestamp */
void sxlib_Radio_event_handler_trigger_with_timestamp_base( sxlib_Radio_event_handler_manager_state_t* state );

/** For documentation of this private function, see \ref sxlib_Radio_event_handler_cancel_event */
void sxlib_Radio_event_handler_cancel_event_base( sxlib_Radio_event_handler_manager_state_t* state );

/** For documentation of this private function, see \ref sxlib_Radio_event_handler_retrieve_timestamp */
uint32_t sxlib_Radio_event_handler_retrieve_timestamp_base( sxlib_Radio_event_handler_manager_state_t* state,
                                                            const void*                                context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Associate an event handler manager to a generic radio context, so
 * that its API can be called using a standard radio 'context' argument. This
 * Function must be called before any other event_handler API functions.
 *
 * @param handler_manager_state The event handler manager state data
 * @param context A generic radio-driver context
 *
 * @returns Operation status
 */
sxlib_status_t sxlib_Radio_event_handler_manager_register(
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state, const void* context );

/**
 * @brief Remove a previously made association between an event handler
 * manager and a generic radio context. Once this is done, the event_handler
 * API must no longer be used, with the exception of \ref
 * sxlib_Radio_event_handler_manager_register.
 *
 * @param handler_manager_state The event handler manager state data
 * @param context A previously registered generic radio-driver context
 *
 * @returns Operation status
 */
sxlib_status_t sxlib_Radio_event_handler_manager_unregister(
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state, const void* context );

/**
 * @brief Given a previously registered context, return the associated event handler state.
 *
 * @param context A previously registered generic radio-driver context
 *
 * @returns The associated event handler state for a previously registered context
 *
 * @remark Returns 0 for a context that has never been registered.
 */
sxlib_Radio_event_handler_manager_state_t* sxlib_Radio_event_handler_manager_get_obj( const void* context );

/**
 * @brief Signal that a radio event has occurred and must be handled
 *
 * This function may be called by low-level software when a radio event
 * occurs. How the event is handled depends on the type of event handler,
 * which may be synchronous or asynchronous. It is not necessary to call
 * both this function and sxlib_Radio_event_handler_trigger_with_timestamp.
 * One of them is sufficient, depending on whether or not it is necessary to
 * store a timestamp.
 *
 * \remark Depending on the implementation, this function may be called from an interrupt service routine
 *
 * @param context A previously registered generic radio-driver context
 */
static inline void sxlib_Radio_event_handler_trigger( const void* context )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_event_handler_trigger_base( handler_manager_state );
}

/**
 * @brief Signal that a radio event has occurred and must be handled, and store a timestamp indicating the current time
 *
 * This function may be called by low-level software when a radio event
 * occurs. How the event is handled depends on the type of event handler,
 * which may be synchronous or asynchronous. It is not necessary to call
 * both this function and sxlib_Radio_event_handler_trigger. One of them
 * is sufficient, depending on whether or not it is necessary to store
 * a timestamp.
 *
 * \remark Depending on the implementation, this function may be called from an interrupt service routine
 *
 * @param context A previously registered generic radio-driver context
 */
static inline void sxlib_Radio_event_handler_trigger_with_timestamp( const void* context )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_event_handler_trigger_with_timestamp_base( handler_manager_state );
}

/**
 * @brief Cancel any pending event handler for this transceiver context
 *
 * It is required that any transceiver event that occurred before this function
 * was called will not be handled by the event handler. This effectively
 * cancels an event that may have occurred in the past.
 *
 * @param context A previously registered generic radio-driver context
 */
static inline void sxlib_Radio_event_handler_cancel_event( const void* context )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_event_handler_cancel_event_base( handler_manager_state );
}

/**
 * @brief Return the previously timestamp previously stored by \ref sxlib_Radio_event_handler_trigger_with_timestamp
 *
 * @param context A previously registered generic radio-driver context
 *
 * @returns The timestamp value recorded on the last \ref sxlib_Radio_event_handler_trigger_with_timestamp invocation.
 *
 * @remark If \ref sxlib_Radio_event_handler_trigger_with_timestamp has not previously been called, the return value is
 * undefined.
 */
static inline uint32_t sxlib_Radio_event_handler_retrieve_timestamp( const void* context )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_event_handler_retrieve_timestamp_base( handler_manager_state, context );
}

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_EVENT_HANDLER_GENERIC_EVENT_HANDLER_H_

/* --- EOF ------------------------------------------------------------------ */
