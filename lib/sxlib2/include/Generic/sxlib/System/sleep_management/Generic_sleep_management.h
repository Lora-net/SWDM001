/**
 * \file
 *
 * \brief Generic sleep management interface header
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

#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_MANAGEMENT_GENERIC_SLEEP_MANAGEMENT_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_MANAGEMENT_GENERIC_SLEEP_MANAGEMENT_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/System/sleep_inhibition/Generic_sleep_inhibition.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Enter sleep mode, taking into consideration any inhibited sleep classes
 *
 * @remark Interrupts will be disabled on entry, and must be kept disabled
 */
void sxlib_System_sleep( void );

/**
 * @brief Depending on the platform, this function will be called by \ref sxlib_System_sleep shortly before entering
 * sleep mode for certain sleep classes
 *
 * @remark Interrupts will be disabled on entry, and must be kept disabled
 */
void sxlib_System_pre_sleep( void );

/**
 * @brief Depending on the platform, this function may be called by \ref sxlib_System_sleep shortly after exiting sleep
 * mode for certain sleep classes
 *
 * @remark Interrupts will be disabled on entry, and must be kept disabled
 */
void sxlib_System_post_wake( void );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_MANAGEMENT_GENERIC_SLEEP_MANAGEMENT_H_

/* --- EOF ------------------------------------------------------------------ */
