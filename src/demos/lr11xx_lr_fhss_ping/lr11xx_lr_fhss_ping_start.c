/**
 * @file      lr11xx_lr_fhss_ping_start.c
 *
 * @brief     LR11XX LR-FHSS ping demo, launcher code
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

#include <stdint.h>

#include <sxlib/System/config/Rtos_config.h>
#include <sxlib/display/interface/Generic_interface.h>
#include <sxlib/Timing/SystemTime/Generic_SystemTime.h>
#include <sxlib/demo/demo.h>

#include "lr11xx_lr_fhss_ping.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define INTER_PKT_DELAY_IN_MS ( 20000 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

lr11xx_lr_fhss_ping_state_t  lr11xx_lr_fhss_ping_state;
lr11xx_lr_fhss_ping_config_t lr11xx_lr_fhss_ping_config;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void sxlib_demo_launcher( const sxlib_generic_interface_t* interface, const void* context )
{
    lr11xx_lr_fhss_ping_config.interface       = interface;
    lr11xx_lr_fhss_ping_config.radio           = context;
    lr11xx_lr_fhss_ping_config.inter_pkt_delay = SXLIB_MS_TO_TICK( INTER_PKT_DELAY_IN_MS );

    lr11xx_lr_fhss_ping_init( &lr11xx_lr_fhss_ping_state, &lr11xx_lr_fhss_ping_config );
    lr11xx_lr_fhss_ping_start( &lr11xx_lr_fhss_ping_state );
}

/* --- EOF ------------------------------------------------------------------ */
