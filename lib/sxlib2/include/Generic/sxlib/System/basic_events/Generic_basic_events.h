/**
 * \file
 *
 * \brief basic event handler interface header
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_BASIC_EVENTS_GENERIC_BASIC_EVENTS_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_BASIC_EVENTS_GENERIC_BASIC_EVENTS_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#define SXLIB_DEFAULT_EVENT_MASK_INIT ( 1 << 0 )
#define SXLIB_DEFAULT_EVENT_MASK_TIMER ( 1 << 1 )
#define SXLIB_DEFAULT_EVENT_MASK_DONGLE ( 1 << 2 )
#define SXLIB_DEFAULT_EVENT_MASK_RADIO0 ( 1 << 3 )
#define SXLIB_DEFAULT_EVENT_MASK_RADIO1 ( 1 << 4 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** intergral datatype used to store event information */
typedef uint32_t sxlib_System_basic_events_mask_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Anytime \ref sxlib_System_basic_events_event_set is called, this function will be called.
 *
 * This function must be implemented by the application, but may be kept empty.
 */
void sxlib_System_basic_events_signal( void );

/**
 * @brief Set an event so that it will be processed by the event loop
 *
 * This function must be callable from interrupt service routines
 */
sxlib_System_basic_events_mask_t sxlib_System_basic_events_set( sxlib_System_basic_events_mask_t mask );

/**
 * @brief Atomically get and clear events
 *
 * \remark This function is meant to be called on each iteration of an event loop
 */
sxlib_System_basic_events_mask_t sxlib_System_basic_events_get_and_clear( sxlib_System_basic_events_mask_t mask );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_BASIC_EVENTS_GENERIC_BASIC_EVENTS_H_

/* --- EOF ------------------------------------------------------------------ */
