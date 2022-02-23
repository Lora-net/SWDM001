/**
 * \file
 *
 * \brief sleep level inhibition interface header
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_INHIBITION_GENERIC_SLEEP_INHIBITION_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_INHIBITION_GENERIC_SLEEP_INHIBITION_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/** Mask used to identify the caller that inhibits sleep immediately from main() */
#define SXLIB_SLEEP_INHIBITION_MASK_MAIN ( 1 << 15 )
#define SXLIB_SLEEP_INHIBITION_MASK_SXLIB ( 1 << 14 )
#define SXLIB_SLEEP_INHIBITION_MASK_USER ( 1 << 13 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Each user of this module must provide a unique single-bit mask for identification */
typedef uint32_t sxlib_System_sleep_user_mask;

/** Definition of different sleep classes, where higher classes yield higher power savings */
typedef enum sxlib_System_sleep_inhibition_class_e
{
    SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_0,
    SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_1,
    SXLIB_SYSTEM_SLEEP_INHIBITION_SENTINEL,
} sxlib_System_sleep_inhibition_class_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inhibit a given sleep class
 *
 * @remark If a sleep class is inhibited, all higher sleep classes will implicitly be inhibited
 *
 * @param mask Sleep inhibition caller identity mask
 * @param class Sleep class to inhibit
 */
void sxlib_System_sleep_inhibition_inhibit_sleep( sxlib_System_sleep_user_mask mask,
                                                  sxlib_System_sleep_inhibition_class_t class );

/**
 * @brief Allow a given sleep class that was previously inhibited
 *
 * @param mask Sleep inhibition caller identity mask
 * @param class Sleep class to allow
 */
void sxlib_System_sleep_inhibition_allow_sleep( sxlib_System_sleep_user_mask mask,
                                                sxlib_System_sleep_inhibition_class_t class );

/**
 * @brief Return the lowest class for which sleep has been inhibited by at least one caller
 *
 * @param mask Sleep inhibition caller identity mask
 * @param class Sleep class to allow
 */
sxlib_System_sleep_inhibition_class_t sxlib_System_sleep_inhibition_get_first_inhibited_class( void );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_SYSTEM_SLEEP_INHIBITION_GENERIC_SLEEP_INHIBITION_H_

/* --- EOF ------------------------------------------------------------------ */
