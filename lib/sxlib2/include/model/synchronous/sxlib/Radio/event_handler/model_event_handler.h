/**
 * \file
 *
 * \brief Generic transceiver event handling subsystem header for the synchronous model
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

#ifndef SXLIB_INCLUDE_MODEL_SYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_MODEL_EVENT_HANDLER_H_
#define SXLIB_INCLUDE_MODEL_SYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_MODEL_EVENT_HANDLER_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/System/config/Rtos_config.h>

#include <sxlib/Radio/event_handler/Generic_event_handler.h>
#include <sxlib/Debug/Status/Generic_Status.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define SXLIB_RADIO_EVENT_HANDLER_TIMEOUT_FOREVER ( -1 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** A timeout type, in system tick units */
typedef int32_t sxlib_Radio_event_handler_radio_time_t;

/** A type returned by the radio event wait function */
typedef enum sxlib_Radio_event_handler_status_e
{
    SXLIB_RADIO_EVENT_HANDLER_STATUS_REGISTRY_FULL   = -2,
    SXLIB_RADIO_EVENT_HANDLER_STATUS_NOT_IMPLEMENTED = -1,
    SXLIB_RADIO_EVENT_HANDLER_STATUS_OK              = 0,
    SXLIB_RADIO_EVENT_HANDLER_STATUS_TIMEOUT         = 1,
} sxlib_Radio_event_handler_status_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/** For documentation of this private function, see \ref sxlib_Radio_event_handler_wait_for_event */
sxlib_Radio_event_handler_status_t sxlib_Radio_event_handler_wait_for_event_base(
    sxlib_Radio_event_handler_manager_state_t* state, sxlib_Radio_event_handler_radio_time_t timeout );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Wait until a radio event occurs, or until a timeout
 *
 * @param context A previously registered generic radio-driver context
 * @param timeout The maximum amount of time to wait for a radio event, or SXLIB_RADIO_EVENT_HANDLER_TIMEOUT_FOREVER
 *
 * @returns Operation status
 */
static inline sxlib_Radio_event_handler_status_t sxlib_Radio_event_handler_wait_for_event(
    const void* context, sxlib_Radio_event_handler_radio_time_t timeout )
{
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state =
        sxlib_Radio_event_handler_manager_get_obj( context );
    return sxlib_Radio_event_handler_wait_for_event_base( handler_manager_state, timeout );
}

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_MODEL_SYNCHRONOUS_SXLIB_RADIO_EVENT_HANDLER_MODEL_EVENT_HANDLER_H_

/* --- EOF ------------------------------------------------------------------ */
