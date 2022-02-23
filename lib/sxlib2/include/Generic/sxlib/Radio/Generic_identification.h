/**
 * \file
 *
 * \brief Generic radio identification header
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

#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_GENERIC_IDENTIFICATION_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_GENERIC_IDENTIFICATION_H_

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** Complementary radio driver data and API */
typedef struct sx_comp_s sx_comp_t;

/** Radio abstraction layer */
typedef struct ral_s ral_t;

/** Full radio abstraction layer */
typedef struct ralf_s ralf_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Complete any radio-specific information in the sx_comp complementary radio driver structure
 */
void sxlib_Radio_identify_radio( sx_comp_t* sx_comp, const void* context );

/**
 * @brief Complete any radio-specific information in the sx_comp complementary radio driver structure and RAL
 */
void sxlib_Radio_identify_ral_radio( ral_t* ral_radio, sx_comp_t* sx_comp, const void* context );

/**
 * @brief Complete any radio-specific information in the sx_comp complementary radio driver structure and RALF
 */
void sxlib_Radio_identify_ralf_radio( ralf_t* ralf_radio, sx_comp_t* sx_comp, const void* context );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_RADIO_GENERIC_IDENTIFICATION_H_

/* --- EOF ------------------------------------------------------------------ */
