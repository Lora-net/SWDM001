/**
 * \file
 *
 * \brief Generic transceiver event handler manager registration function implementation
 *
 * This file is optional. It can be used to manage context/handler_manager key/value pairs. If this file is not used,
 * the developer must implement sxlib_Radio_event_handler_manager_get_obj().
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

#include <sxlib/Radio/event_handler/Generic_event_handler.h>
#include <sxlib/Debug/Status/Generic_Status.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

#ifndef INSTANCE_MAX_COUNT
//* Maximum number of possible transceiver registrations */
#define INSTANCE_MAX_COUNT ( 4 )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

typedef struct registry_entry_s
{
    const void*                                context;
    sxlib_Radio_event_handler_manager_state_t* handler_manager;
} registry_entry_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static registry_entry_t registry[INSTANCE_MAX_COUNT] = { 0 };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

sxlib_status_t sxlib_Radio_event_handler_manager_register(
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state, const void* context )
{
    for( unsigned int i = 0; i < INSTANCE_MAX_COUNT; ++i )
    {
        if( registry[i].context == 0 )
        {
            registry[i].context         = context;
            registry[i].handler_manager = handler_manager_state;
            return SXLIB_STATUS_OK;
        }
    }

    return SXLIB_STATUS_ERROR;
}

sxlib_status_t sxlib_Radio_event_handler_manager_unregister(
    sxlib_Radio_event_handler_manager_state_t* handler_manager_state, const void* context )
{
    for( unsigned int i = 0; i < INSTANCE_MAX_COUNT; ++i )
    {
        if( registry[i].context == context )
        {
            registry[i].context         = 0;
            registry[i].handler_manager = 0;
            return SXLIB_STATUS_OK;
        }
    }

    return SXLIB_STATUS_ERROR;
}

sxlib_Radio_event_handler_manager_state_t* sxlib_Radio_event_handler_manager_get_obj( const void* context )
{
    for( unsigned int i = 0; i < INSTANCE_MAX_COUNT; ++i )
    {
        if( registry[i].context == context )
        {
            return registry[i].handler_manager;
        }
    }

#ifndef NDEBUG
    while( 1 )
    {
    };
#endif

    return 0;
}

/* --- EOF ------------------------------------------------------------------ */
