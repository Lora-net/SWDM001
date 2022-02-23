/**
 * \file
 *
 * \brief Generic asynchronous timer interface header
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMERTIMER_GENERIC_ASYNC_TIMERTIMER_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMERTIMER_GENERIC_ASYNC_TIMERTIMER_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include <sxlib/Timing/OneShotTimerBase/Generic_OneShotTimerBase.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Integral type for storing absolute and relative times */
typedef sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_async_timer_time_t;

/** Opaque structure used to refer to a timer instance */
typedef struct sxlib_Timing_async_timer_s sxlib_Timing_async_timer_t;

/** Callback that will be called on timer expiration */
typedef void ( *sxlib_Timing_async_timer_handler_f )( void* arg );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the timer instance
 *
 * @param timer The timer instance
 */
void sxlib_Timing_async_timer_init( sxlib_Timing_async_timer_t* timer );

/**
 * @brief Provide callback information that will be used on timer expiration
 *
 * @param timer The timer instance
 * @param handler A callback that will be executed on timer expiration
 * @param arg An opaque argument that will be provided to the handler
 *
 * @remark \ref sxlib_Timing_async_timer_init must be called before calling this function
 */
void sxlib_Timing_async_timer_set_handler( sxlib_Timing_async_timer_t* timer, sxlib_Timing_async_timer_handler_f irq_fn,
                                           void* arg );

/**
 * @brief Start the timer
 *
 * @param timer The timer instance
 * @param duration The minimum number of system ticks that must elapse before timer expiration
 *
 * @remark \ref sxlib_Timing_async_timer_set_handler must be called before calling this function
 */
void sxlib_Timing_async_timer_start( sxlib_Timing_async_timer_t* timer, sxlib_Timing_async_timer_time_t duration );

/**
 * @brief Stop the timer
 *
 * @param timer The timer instance
 *
 * @remark \ref sxlib_Timing_async_timer_set_handler must be called before calling this function
 * @remark The implementation must guarantee that the callback will not be called after a timer is stopped, and it must
 * be possible to stop a timer at any time, even if the timer was not previously running
 */
void sxlib_Timing_async_timer_stop( sxlib_Timing_async_timer_t* timer );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ASYNC_TIMERTIMER_GENERIC_ASYNC_TIMERTIMER_H_

/* --- EOF ------------------------------------------------------------------ */
