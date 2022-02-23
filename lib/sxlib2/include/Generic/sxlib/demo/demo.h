/**
 * \file
 *
 * \brief Generic interface header for demo execution
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
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_DEMO_DEMO_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_DEMO_DEMO_H_

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/** RAL radio state */
typedef struct ral_s ral_t;

/** LED/LCD interface definition */
typedef struct sxlib_generic_interface_s sxlib_generic_interface_t;

/** Callback that may be executed on asynchronous demo completion */
typedef void ( *sxlib_demo_done_callback_f )( void* arg );

/** Demo completion callback data that may be provided on demo initialization */
typedef struct sxlib_demo_config_base_s
{
    sxlib_demo_done_callback_f done;
    void*                      done_arg;
} sxlib_demo_config_base_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUCTION PROTOTYPES -----------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Launch a demo that uses a single radio
 *
 * @param [in] interface GUI interface information (LED, LCD, etc.)
 * @param [in] context   Chip implementation context
 */
void sxlib_demo_launcher( const sxlib_generic_interface_t* interface, const void* context );

/**
 * Launch a demo that uses a single RAL radio
 *
 * @param [in] interface GUI interface information (LED, LCD, etc.)
 * @param [in] radio  Pointer to radio data structure
 */
void sxlib_demo_ral_launcher( const sxlib_generic_interface_t* interface, const ral_t* radio );

// Prototypes of demo init/start functions:
typedef void ( *sxlib_demo_init )( void* arg_state, const sxlib_demo_config_base_t* config );
typedef void ( *sxlib_demo_start )( void* arg_state );

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_DEMO_DEMO_H_

/* --- EOF ------------------------------------------------------------------ */
