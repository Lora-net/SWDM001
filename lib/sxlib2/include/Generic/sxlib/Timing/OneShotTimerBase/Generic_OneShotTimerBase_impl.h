/**
 * \file
 *
 * \brief Generic implementation header of a timer linked list
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_IMPL_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_IMPL_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/Timing/OneShotTimerBase/Generic_OneShotTimerBase.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Callback that will be called on timer expiration */
typedef void ( *sxlib_Timing_OneShotTimerBase_callback_f )( );

/** Callback and opaque argument pair */
struct sxlib_Timing_OneShotTimerBase_callable
{
    void*                                    arg;      /*!< Opaque argument provided to callback */
    sxlib_Timing_OneShotTimerBase_callback_f callback; /*!< Callback */
};

/** Timer link structure with time delta, must be zero-initialized */
struct sxlib_Timing_OneShotTimerBase
{
    struct sxlib_Timing_OneShotTimerBase*         next;     /*!< Points to timer that will execute after this one */
    struct sxlib_Timing_OneShotTimerBase_callable callable; /*!< Callable to execute for this timer */
    sxlib_Timing_OneShotTimerBase_time_t
        delta; /*!< relative ticks between this timer deadline and next timer deadline */
};

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_ONESHOTTIMERBASE_GENERIC_ONESHOTTIMERBASE_IMPL_H_

/* --- EOF ------------------------------------------------------------------ */
