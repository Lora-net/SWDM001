/**
 * \file
 *
 * \brief Generic interface header for polling operations for a basic asynchronous timer implementation
 *
 * This interface defines the functions that need to be called to allow simple polling for asynchronous timer events
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMER_POLL_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMER_POLL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/Timing/async_timer/Generic_basic_async_timer.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get ready to start polling for asynchronous timer events
 *
 * @remark The system time must already be available when calling this function
 */
void sxlib_Timing_async_timer_poll_init( );

/**
 * @brief Poll for asynchronous events
 *
 * The event loop can call this function at any time. The time that has elapsed since the previous call will be
 * evaluated, and \ref sxlib_Timing_async_timer_process will be called to execute any expired timer callbacks.
 */
void sxlib_Timing_async_timer_poll( );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMER_POLL_H_

/* --- EOF ------------------------------------------------------------------ */
