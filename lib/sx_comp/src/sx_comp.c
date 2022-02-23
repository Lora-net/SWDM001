/**
 * \file
 *
 * \brief Generic radio complementary driver registry
 *
 * This file is optional. It can be used to manage context/sx_comp key/value pairs.
 * If this file is not used, the developer must implement sx_comp_get_obj().
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

#include "sx_comp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#ifndef SX_COMP_MAX_COUNT
#define SX_COMP_MAX_COUNT ( 4 )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

typedef struct registry_entry_s
{
    const void*      context;
    const sx_comp_t* sx_comp;
} registry_entry_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static registry_entry_t registry[SX_COMP_MAX_COUNT] = { 0 };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ------------------------------------------------------------
 */

sx_comp_status_t sx_comp_register( const sx_comp_t* sx_comp, const void* context )
{
    for( unsigned int i = 0; i < SX_COMP_MAX_COUNT; ++i )
    {
        if( registry[i].context == 0 )
        {
            registry[i].context = context;
            registry[i].sx_comp = sx_comp;
            return SX_COMP_STATUS_OK;
        }
    }

    return SX_COMP_STATUS_ERROR;
}

sx_comp_status_t sx_comp_unregister( const sx_comp_t* sx_comp, const void* context )
{
    for( unsigned int i = 0; i < SX_COMP_MAX_COUNT; ++i )
    {
        if( registry[i].context == context )
        {
            registry[i].context = 0;
            registry[i].sx_comp = 0;
            return SX_COMP_STATUS_OK;
        }
    }

    return SX_COMP_STATUS_ERROR;
}

const sx_comp_t* sx_comp_get_obj( const void* context )
{
    for( unsigned int i = 0; i < SX_COMP_MAX_COUNT; ++i )
    {
        if( registry[i].context == context )
        {
            return registry[i].sx_comp;
        }
    }

    return 0;
}

/* --- EOF ------------------------------------------------------------------ */
