/**
 * \file
 *
 * \brief Generic interface header for system time management
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

#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_SYSTEMTIME_GENERIC_SYSTEMTIME_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_SYSTEMTIME_GENERIC_SYSTEMTIME_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/**
 * @brief Used for storing long absolute time
 */
typedef uint64_t sxlib_Timing_SystemTime_long_absolute_time_t;

/**
 * @brief Used for storing absolute time
 */
typedef uint32_t sxlib_Timing_SystemTime_absolute_time_t;

/**
 * @brief Used for storing long relative time
 */
typedef uint64_t sxlib_Timing_SystemTime_long_relative_time_t;

/**
 * @brief Used for storing relative time
 */
typedef uint32_t sxlib_Timing_SystemTime_relative_time_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

#ifndef CONFIG_SYS_CLOCK_TICKS_PER_SEC
#define CONFIG_SYS_CLOCK_TICKS_PER_SEC ( 1 << ( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 ) )
#endif

#if CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2
#if( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 > 10 )
// Fast conversion from 1024ths of a second, to ticks, adding 1 to avoid waiting less time than requested
#define SXLIB_MBS_TO_TICK( mbs ) ( 1 + ( mbs << ( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 - 10 ) ) )
#define SXLIB_LONG_MBS_TO_TICK( mbs ) ( 1 + ( ( ( uint64_t ) mbs ) << ( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 - 10 ) ) )
#else  // ( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 > 10 )
// Fast conversion from 1024ths of a second, to ticks, adding 1 to avoid waiting less time than requested
#define SXLIB_MBS_TO_TICK( mbs ) ( 1 + ( mbs >> ( 10 - CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 ) ) )
#define SXLIB_LONG_MBS_TO_TICK( mbs ) ( 1 + ( ( ( uint64_t ) mbs ) << ( 10 - CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 ) ) )
#endif  // ( CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 > 10 )
// Conversion from milliseconds, to ticks, rounding up, using division
#define SXLIB_MS_TO_TICK( ms ) ( ( ( ms * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) + 999 ) / 1000 )
#define SXLIB_LONG_MS_TO_TICK( ms ) ( ( ( ( ( uint64_t ) ms ) * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) + 999 ) / 1000 )
#else  // CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2
// Fast conversion from 1024ths of a second, to ticks, adding 1 to avoid waiting less time than requested
#define SXLIB_MBS_TO_TICK( mbs ) ( 1 + ( ( mbs * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) >> 10 ) )
#define SXLIB_LONG_MBS_TO_TICK( mbs ) ( 1 + ( ( ( ( uint64_t ) mbs ) * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) >> 10 ) )
// Conversion from milliseconds, to ticks, rounding up, using division
#define SXLIB_MS_TO_TICK( ms ) ( ( ( ms * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) + 999 ) / 1000 )
#define SXLIB_LONG_MS_TO_TICK( ms ) ( ( ( ( ( uint64_t ) ms ) * CONFIG_SYS_CLOCK_TICKS_PER_SEC ) + 999 ) / 1000 )
#endif  // CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2

#if CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2
#define SXLIB_TICK_TO_MS( tick ) ( ( tick * 1000 ) >> CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 )
#define SXLIB_LONG_TICK_TO_MS( tick ) ( ( ( ( uint64_t ) tick ) * 1000 ) >> CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 )
#else
#define SXLIB_TICK_TO_MS( tick ) ( tick * 1000 / CONFIG_SYS_CLOCK_TICKS_PER_SEC )
#define SXLIB_LONG_TICK_TO_MS( tick ) ( ( ( uint64_t ) tick ) * 1000 / CONFIG_SYS_CLOCK_TICKS_PER_SEC )
#endif

#if CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2
#define SXLIB_TICK_TO_S( tick ) ( tick >> CONFIG_SYS_CLOCK_TICKS_PER_SEC_LOG2 )
#else
#define SXLIB_TICK_TO_S( tick ) ( tick / CONFIG_SYS_CLOCK_TICKS_PER_SEC )
#endif

#define SXLIB_LONG_TICK_TO_S( tick ) SXLIB_TICK_TO_S( tick )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the system time
 *
 * @remark Must be callable from interrupt service routines
 *
 * @returns Current time
 */
sxlib_Timing_SystemTime_absolute_time_t sxlib_Timing_SystemTime_get_time( void );

/**
 * @brief Get the long system time
 *
 * @remark Depending on platform, not necessarily callable from interrupt service routines
 *
 * @returns Current time
 */
sxlib_Timing_SystemTime_long_absolute_time_t sxlib_Timing_SystemTime_get_long_time( void );

/**
 * @brief Get long elapsed time, relative to a long timestamp
 *
 * @param timestamp Start time, in ticks
 *
 * @remark Depending on platform, not necessarily callable from interrupt service routines
 *
 * @returns Elapsed time
 */
sxlib_Timing_SystemTime_long_relative_time_t sxlib_Timing_SystemTime_get_long_relative_time(
    sxlib_Timing_SystemTime_long_absolute_time_t timestamp );

/**
 * @brief Get elapsed time, relative to a timestamp
 *
 * @param timestamp Start time, in ticks
 *
 * @remark Must be callable from interrupt service routines
 *
 * @returns Elapsed time
 */
sxlib_Timing_SystemTime_relative_time_t sxlib_Timing_SystemTime_get_relative_time(
    sxlib_Timing_SystemTime_absolute_time_t timestamp );

/**
 * @brief Get system clock frequency, in ticks per second
 *
 * @returns Clock frequency, in ticks per second
 */
uint32_t sxlib_Timing_SystemTime_get_ticks_per_second( void );

/**
 * @brief Busywait at least a certain number of ticks
 *
 * @param ticks Delay, in ticks
 */
void sxlib_Timing_SystemTime_busywait_ticks( uint32_t ticks );

/**
 * @brief Busywait at least a certain number of mibi-seconds (1s / 1024)
 *
 * @param mbs Delay, in mibi-seconds
 *
 * This is meant to be fast, and require few calculations.
 */
void sxlib_Timing_SystemTime_busywait_mbs( uint32_t mbs );

/**
 * @brief Busywait at least a certain number of milli-seconds
 *
 * @param ms Delay, in milliseconds
 */
void sxlib_Timing_SystemTime_busywait_ms( uint32_t ms );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_TIMING_SYSTEMTIME_GENERIC_SYSTEMTIME_H_

/* --- EOF ------------------------------------------------------------------ */
