/**
 * \file
 *
 * \brief sleep level inhibition implementation
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/System/sleep_inhibition/Generic_sleep_inhibition.h>
#include <sxlib/System/critical_section/McuHal_critical_section.h>

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static uint32_t sxlib_System_sleep_inhibition_masks[SXLIB_SYSTEM_SLEEP_INHIBITION_SENTINEL];

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTION DEFINITIONS --------------------------------------------
 */

void sxlib_System_sleep_inhibition_inhibit_sleep( sxlib_System_sleep_user_mask mask,
                                                  sxlib_System_sleep_inhibition_class_t class )
{
    SXLIB_SYSTEM_CRITICAL_SECTION_ENTER;

    sxlib_System_sleep_inhibition_masks[class] |= mask;

    SXLIB_SYSTEM_CRITICAL_SECTION_EXIT;
}

void sxlib_System_sleep_inhibition_allow_sleep( sxlib_System_sleep_user_mask mask,
                                                sxlib_System_sleep_inhibition_class_t class )
{
    SXLIB_SYSTEM_CRITICAL_SECTION_ENTER;

    sxlib_System_sleep_inhibition_masks[class] &= ~mask;

    SXLIB_SYSTEM_CRITICAL_SECTION_EXIT;
}

sxlib_System_sleep_inhibition_class_t sxlib_System_sleep_inhibition_get_first_inhibited_class( void )
{
    sxlib_System_sleep_inhibition_class_t i = SXLIB_SYSTEM_SLEEP_INHIBITION_CLASS_0;

    SXLIB_SYSTEM_CRITICAL_SECTION_ENTER;

    while( i < SXLIB_SYSTEM_SLEEP_INHIBITION_SENTINEL )
    {
        if( sxlib_System_sleep_inhibition_masks[i] != 0 )
        {
            break;
        }
        i++;
    }

    SXLIB_SYSTEM_CRITICAL_SECTION_EXIT;

    return i;
}

/* --- EOF ------------------------------------------------------------------ */
