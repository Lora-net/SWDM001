/**
 * \file
 *
 * \brief Generic API of a timer linked list
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS -------------------------------------------------------
 */

/** Indicates that the operation completed successfully */
#define ONE_SHOT_TIMER_BASE_STATUS_OK ( 0 )

/** Indicates that the provided timer argument is not in the timer queue */
#define ONE_SHOT_TIMER_BASE_STATUS_INEXISTENT ( 1 )

/** Indicates that the operation completed successfully, and the first timer timeout has been changed */
#define ONE_SHOT_TIMER_BASE_STATUS_CHANGED ( 2 )

/** Indicates that the operation completed successfully, and the first timer has been added */
#define ONE_SHOT_TIMER_BASE_STATUS_NEW ( 3 )

/** Returned by sxlib_Timing_OneShotTimerBase_get_delta, indicating that the timer queue is empty */
#define ONE_SHOT_TIMER_WAIT_FOREVER ( -1 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES -----------------------------------------------------------
 */

/** integral type used to store timer ticks */
typedef int32_t sxlib_Timing_OneShotTimerBase_time_t;

/** Timer link structure forward declaration */
struct sxlib_Timing_OneShotTimerBase;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Insert timer into the linked list at appropriate place
 *
 * @param root Root of timer linked list
 * @param timer Timer instance
 * @param timer Ticks until timer expiration
 */
int sxlib_Timing_OneShotTimerBase_insert( struct sxlib_Timing_OneShotTimerBase* root,
                                          struct sxlib_Timing_OneShotTimerBase* timer,
                                          sxlib_Timing_OneShotTimerBase_time_t  ticks );

/**
 * @brief Execute all expired callbacks (for which delta is zero), and then remove them from linked list
 *
 * @param root Root of timer linked list
 * @param restart_timer_callback must be NULL
 * @param arg must be NULL
 */
void sxlib_Timing_OneShotTimerBase_execute_callables(
    struct sxlib_Timing_OneShotTimerBase* root,
    void ( *restart_timer_callback )( void* arg, sxlib_Timing_OneShotTimerBase_time_t delta ), void* arg );

/**
 * @brief Remove timer from linked list
 *
 * @param root Root of timer linked list
 * @param timer Timer instance
 */
int sxlib_Timing_OneShotTimerBase_delete( struct sxlib_Timing_OneShotTimerBase* root,
                                          struct sxlib_Timing_OneShotTimerBase* timer );

/**
 * @brief Subtract give number of ticks from the timer list, implementing the passage of time
 *
 * @remark Any timers that expire will be left in the list, with a delta of zero
 *
 * @param root Root of timer linked list
 * @param ticks Number of ticks to subtract
 */
void sxlib_Timing_OneShotTimerBase_update( struct sxlib_Timing_OneShotTimerBase* root,
                                           sxlib_Timing_OneShotTimerBase_time_t  ticks );

/**
 * @brief Return the number of ticks before the next timer expires
 *
 * @remark If the list is empty, return ONE_SHOT_TIMER_WAIT_FOREVER
 *
 * @param root Root of timer linked list
 */
sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_OneShotTimerBase_get_delta(
    struct sxlib_Timing_OneShotTimerBase* root );

/**
 * @brief Return the number of ticks before the given timer expires
 *
 * @param root Root of timer linked list
 * @param timer Timer instance
 */
sxlib_Timing_OneShotTimerBase_time_t sxlib_Timing_OneShotTimerBase_get_remaining_time(
    struct sxlib_Timing_OneShotTimerBase* root, struct sxlib_Timing_OneShotTimerBase* timer );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_H_

/* --- EOF ------------------------------------------------------------------ */
