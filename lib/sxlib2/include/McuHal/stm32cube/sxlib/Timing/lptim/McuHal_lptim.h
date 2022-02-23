/**
 * \file
 *
 * \brief McuHal-specific interface header for low-power timer functionality
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
#ifndef SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_TIMING_LPTIM_MCUHAL_LPTIM_H_
#define SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_TIMING_LPTIM_MCUHAL_LPTIM_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the low-power timer
 */
void sxlib_Timing_lptim_init( unsigned int freq_exp2 );

/**
 * @brief Read the 16-bit low-power timer hardware time
 *
 * @returns The current low-power time hardware time
 */
uint16_t sxlib_Timing_lptim_get_hardware_time( void );

/**
 * @brief Set the low-power timer clock frequency
 *
 * @param freq_exp2 The clock frequency will be 2 to the power of freq_exp2
 *
 * @remark freq_exp2 must be between 8 and 15
 */
void sxlib_Timing_lptim_set_clock_frequency_log2( unsigned int freq_exp2 );

/**
 * @brief Get the low-power timer prescaler
 *
 * @returns The base-2 logarithm of the clock frequency (which is a power of 2), between 8 and 15
 */
unsigned int sxlib_Timing_lptim_get_clock_frequency_log2( void );

/**
 * @brief Set the low-power timer trigger time
 *
 * @remark This is an absolute hardware timer time
 *
 * @remark Due to the STM32L4 LPTIM errata, this implementation does not stop the timer
 *
 * @remark If BLOCKING_SETCOMPARE is set, timer startup will never fail, but this function will take more time to
 * complete
 *
 * @returns true if the timer was started, false if timer startup failed.
 * Timer startup can fail if, for instance, this function was called twice within
 * a very short interval.
 */
bool sxlib_Timing_lptim_set_trigger_time( uint16_t deadline );

/**
 * @brief Set the low-power timer to trigger after a certain number of timer ticks
 *
 * @remark Due to the STM32L4 LPTIM errata, this implementation does not stop the timer
 *
 * @remark If BLOCKING_SETCOMPARE is set, timer startup will never fail, but this function will take more time to
 * complete
 *
 * @returns true if the timer was started, false if timer startup failed.
 * Timer startup can fail if, for instance, this function was called twice within
 * a very short interval.
 */
bool sxlib_Timing_lptim_set_trigger_interval( int32_t delta );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_MCUHAL_STM32CUBEL_SXLIB_TIMING_LPTIM_MCUHAL_LPTIM_H_

/* --- EOF ------------------------------------------------------------------ */
